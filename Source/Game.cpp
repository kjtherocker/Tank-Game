#include "Game.h"
#include <time.h>


namespace GameDev2D
{
    Game::Game()
    {
        Physics::World::GetInstance()->SetListener(this);

        LoadTexture("SpriteAtlas");
        LoadTexture("Background");
        LoadTexture("Shell");
        srand(time(nullptr));
        BarrelCount = 48;

        m_TimeStep = 1.0f / (double)TARGET_FPS;


        LoadFont("slkscr", "ttf", 48);

        m_Win = false;
        m_Winner = new Label(GetFont("slkscr", "ttf", 48));

        m_GreenCount = 4;
        m_BlueCount = 4;

        m_BlueTank = new Tank(TankColor::TankBlue);
        m_BlueTank->SetLeftInput(Keyboard::A);
        m_BlueTank->SetRightInput(Keyboard::D);
        m_BlueTank->SetUpInput(Keyboard::W);
        m_BlueTank->SetDownInput(Keyboard::S);
        m_BlueTank->SetTurretLeftInput(Keyboard::Q);
        m_BlueTank->SetTurretRightInput(Keyboard::E);
        m_BlueTank->SetFireTurretInput(Keyboard::Tab);

        m_GreenTank = new Tank(TankColor::TankGreen);
        m_GreenTank->SetLeftInput(Keyboard::Left);
        m_GreenTank->SetRightInput(Keyboard::Right);
        m_GreenTank->SetUpInput(Keyboard::Up);
        m_GreenTank->SetDownInput(Keyboard::Down);
        m_GreenTank->SetTurretLeftInput(Keyboard::N);
        m_GreenTank->SetTurretRightInput(Keyboard::M);
        m_GreenTank->SetFireTurretInput(Keyboard::Spacebar);


        m_Background = new Sprite(GetTexture("Background"));
        m_Background->SetPosition(Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2));
        m_Background->SetAnchor(Vector2(0.5f, 0.5f));


        for (int i = 0; i < 3; i++)
        {
            m_BlueShots[i] = new SpriteAtlas("Shell");
            m_BlueShots[i]->UseFrame("full-shell");
            m_BlueShots[i]->SetAnchor(Vector2(0.5, 0.5));
            m_BlueShots[i]->SetPosition(Vector2(4 + 20 * i, 0));

            m_GreenShots[i] = new SpriteAtlas("Shell");
            m_GreenShots[i]->UseFrame("full-shell");
            m_GreenShots[i]->SetAnchor(Vector2(0.5, 0.5));
            m_GreenShots[i]->SetPosition(Vector2(4 + 20 * i, 0));
        }

        int count = 0;
        unsigned int BlueDet;
        unsigned int GreenDet;
        do
        {
            BlueDet = RandomRange(0, 47);
            GreenDet = RandomRange(0, 47);
        } while (BlueDet == GreenDet);

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                m_Barrel[count] = new Barrel(Vector2(i * 128 + 60, j * 96 + 168));
                if (BlueDet == count)
                {
                    m_Barrel[count]->SetDetonator(1);
                }
                else if (GreenDet == count)
                {
                    m_Barrel[count]->SetDetonator(2);
                }
                else
                {
                }
                count++;
            }
        }

        for (int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            m_Shells[i] = new Shell();
        }
    }

    Game::~Game()
    {
        delete m_GreenTank;
        delete m_BlueTank;

        for (int i = 0; i < BarrelCount; i++)
        {
            delete m_Barrel[i];
        }

        delete m_Winner;

        for (int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            delete m_Shells[i];
        }
        UnloadTexture("shell.png");
        UnloadTexture("Full bullet.png");
        UnloadFont("slkscr", "ttf", 48);
    }

    void Game::Update(double delta)
    {
        m_BlueTank->Tank::Update(delta);
        m_GreenTank->Tank::Update(delta);
        for (int i = 0; i < BarrelCount; i++)
        {
            m_Barrel[i]->Update(delta);
        }

        UpdateTank(m_BlueTank, delta);
        UpdateTank(m_GreenTank, delta);

        for (int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            m_Shells[i]->Update(delta);
        }

        Physics::World::GetInstance()->Step(m_TimeStep);

        //Shells
        if (m_BlueCount == 3)
        {
            m_BlueShots[2]->UseFrame("shell");
        }

        if (m_BlueCount == 2)
        {
            m_BlueShots[1]->UseFrame("shell");
        }
        if (m_BlueCount == 1)
        {
            m_BlueShots[0]->UseFrame("shell");
            m_BlueTank->SetAmmoIsEmpty();
        }
        if (m_BlueCount == 0)
        {
            if (m_BlueTank->GetTimer() >= 3)
            {
                m_BlueCount = 4;
                m_BlueShots[2]->UseFrame("full-shell");
                m_BlueShots[1]->UseFrame("full-shell");
                m_BlueShots[0]->UseFrame("full-shell");
            }
        }

    }

    void Game::Draw()
    {
        m_Background->Draw();


        DrawRectangle(40, 50, 40, m_BlueTank->GetTimer() * 45, 0, Color::AntiqueWhiteColor(), true);
        for (int i = 0; i < 3; i++)
        {
            m_BlueShots[i]->Draw();
        }
        for (int i = 0; i < BarrelCount; i++)
        {
            m_Barrel[i]->Draw();
        }

        for (int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            if (m_Shells[i]->IsEnabled() == true)
            {
                m_Shells[i]->Draw();
            }
        }
        if (m_Win = true)
        {
            m_Winner->Draw();
        }
        m_BlueTank->Draw();
        m_GreenTank->Draw();
    }

    void Game::HandleLeftMouseClick(float mouseX, float mouseY)
    {
        m_BlueCount--;
    }

    void Game::HandleRightMouseClick(float mouseX, float mouseY)
    {
        for (int i = 0; i < 48; i++)
        {
            m_Barrel[i]->Explosions();
        }
    }
    void Game::HandleKeyPress(Keyboard::Key key)
    {
        if (key == Keyboard::Spacebar)
        {
            m_GreenCount--;
        }
        if (key == Keyboard::Tab)
        {
            m_BlueCount--;
        }
    }

    Shell* Game::GetShell()
    {
        for (unsigned int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            if (m_Shells[i]->IsEnabled() == false)
            {
                return m_Shells[i];
            }
        }

        return nullptr;
    }

    void Game::UpdateTank(Tank* aTank, double aDelta)
    {
        aTank->Update(aDelta);

        if (aTank->CanFireShell() == true)
        {
            Shell* shell = GetShell();
            if (shell != nullptr)
            {
                GetShell()->Fire(aTank->GetShellPosition(), aTank->GetTurretAngle());
                aTank->ResetFireDelay();
            }
        }
    }

    bool Game::CollisionCallBack(Physics::Body* aBodyA, Physics::Body* aBodyB)
    {
        if ((aBodyA->GetObject1() == Body_Shell && aBodyB->GetObject1() == Body_Barrel) ||
            (aBodyA->GetObject1() == Body_Barrel && aBodyB->GetObject1() == Body_Shell))
        {
            for (int i = 0; i < BarrelCount; i++)
            {

                if (aBodyA == m_Barrel[i]->GetPhysicsBody() || aBodyB == m_Barrel[i]->GetPhysicsBody())
                {
                    for (int j = 0; j < 10; j++)
                    {
                        if (aBodyA == m_Shells[j]->GetPhysicsBody() || aBodyB == m_Shells[j]->GetPhysicsBody())
                        {
                            m_Shells[j]->Disable();
                        }
                        m_Barrel[i]->Explosions();
                    }

                }
            }
        }
        if ((aBodyA->GetObject1() == Body_Tank && aBodyB->GetObject1() == Body_Green) ||
            aBodyA->GetObject1() == Body_Green && aBodyB->GetObject1() == Body_Tank)
        {
            if (aBodyA == m_BlueTank->GetPhysicsBody() || aBodyB == m_BlueTank->GetPhysicsBody())
            {
                m_GreenTank->DestroyTank();
                m_Winner->SetColor(Color::BlueColor());
                m_Winner->SetText("BLUE WINS!");
                m_Winner->SetPosition(Vector2(GetScreenHeight() / 2, GetScreenWidth() / 2));
                for (int i = 0; i < BarrelCount; i++)
                {
                    if (aBodyA == m_Barrel[i]->GetPhysicsBody() || aBodyB == m_Barrel[i]->GetPhysicsBody())
                    {
                        m_Barrel[i]->RemoveDetonator();
                    }
                }
            }
        }

        if ((aBodyA->GetObject1() == Body_Tank && aBodyB->GetObject1() == Body_Blue) ||
            aBodyA->GetObject1() == Body_Blue && aBodyB->GetObject1() == Body_Tank)
        {
            if (aBodyA == m_GreenTank->GetPhysicsBody() || aBodyB == m_GreenTank->GetPhysicsBody())
            {
                m_Winner->SetColor(Color::GreenColor());
                m_Winner->SetText("GREEN WINS!");
                m_Winner->SetPosition(Vector2(GetScreenHeight() / 2, GetScreenWidth() / 2));
                m_BlueTank->DestroyTank();
                for (int i = 0; i < BarrelCount; i++)
                {
                    if (aBodyA == m_Barrel[i]->GetPhysicsBody() || aBodyB == m_Barrel[i]->GetPhysicsBody())
                    {
                        m_Barrel[i]->RemoveDetonator();
                    }
                }
            }
        }
        return true;
    }
    unsigned int Game::RandomRange(unsigned int min, unsigned int max)
    {
        unsigned int output = min + (rand() % (max - min + 1));
        return output;
    }
}