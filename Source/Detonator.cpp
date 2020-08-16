#include "Detonator.h"
#include "Barrel.h"
#include <GameDev2D.h>
#include "Framework\Math\Math.h"



namespace GameDev2D
{
    Detonator::Detonator(bool aGr) :
        m_PhysicsBody(nullptr)
    {
        green = aGr;

        m_Activated = false;

        m_Position = Vector2(0, 0);

        m_Det = new SpriteAtlas("SpriteAtlas");
        if (green == false)
        {
            m_Det->UseFrame("Detonator-Blue");
        }
        else
        {
            m_Det->UseFrame("Detonator-Green");
        }
        m_Det->SetAnchor(Vector2(0.5f, 0.5f));



        //Initialize the parameters to create the Physics body
        float density = 1.0f;
        float radius = Math::PixelsToMeters(GetHeight() / 2.7f);
        Physics::CircleCollider* circleCollider = new Physics::CircleCollider(radius);

        //Create the Physics body to attach to the tank
        m_PhysicsBody = Physics::World::GetInstance()->CreateBody(circleCollider, density);
        m_PhysicsBody->SetPosition(Math::PixelsToMeters(m_Position));
        m_PhysicsBody->SetLinearDamping(Vector2(0.2f, 0.2));
        m_PhysicsBody->SetAngularDamping(1.0f);
        m_PhysicsBody->SetGameObject(this);

        m_Det->AttachTo(this);


    }

    Detonator::Detonator()
    {
    }

    Detonator::~Detonator()
    {
        delete m_Det;
        m_Det = nullptr;


        delete m_PhysicsBody;
        m_PhysicsBody = nullptr;

    }

    void Detonator::Update(double delta)
    {
        SetPosition(Math::MetersToPixels(m_PhysicsBody->GetPosition()));
        m_Det->SetPosition(Math::MetersToPixels(m_PhysicsBody->GetPosition()));
    }

    float Detonator::GetWidth()
    {
        return m_Det->GetWidth();

    }
    float Detonator::GetHeight()
    {
        return m_Det->GetHeight();

    }

    void Detonator::Draw()
    {
        if (m_Activated == true)
        {
            m_Det->Draw();
        }
    }


    void Detonator::CheckCollision(Vector2 TankPos)
    {


    }

    void Detonator::SetPosition(Vector2 loc)
    {
        m_PhysicsBody->SetPosition(Math::PixelsToMeters(loc));
    }

    void Detonator::Enable()
    {
        m_Activated = true;
    }

    void Detonator::Disable()
    {
        m_Activated = false;
    }

    bool Detonator::GetEnabled()
    {
        return m_Activated;
    }


}

