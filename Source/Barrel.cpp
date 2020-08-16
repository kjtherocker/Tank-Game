#include "Barrel.h"
#include "../Source/Framework/Physics/Body.h"

namespace GameDev2D
{
	Barrel::Barrel(Vector2 Postion)
	{
		LoadTexture("SpriteAtlas");

		m_Barrel = new SpriteAtlas("SpriteAtlas");
		m_Barrel->UseFrame("Barrel");
		m_Barrel->SetAnchor(Vector2(0.5f, 0.5f));
        m_Barrel->AttachTo(this);
		m_Barrel->SetPosition(Postion);

        float density = 1.0f;
        float radius = Math::PixelsToMeters(m_Barrel->GetHeight() / 2);
        Physics::CircleCollider* collider = new Physics::CircleCollider(radius);
        m_Existance = true;
    
        m_PhysicsBody = Physics::World::GetInstance()->CreateBody(collider, density);
        m_PhysicsBody->SetMass(3.65);
        m_PhysicsBody->SetLinearDamping(Vector2(0.2f, 0.2));
        m_PhysicsBody->SetAngularDamping(1.0f);
        m_PhysicsBody->SetPosition(Math::PixelsToMeters(Postion));
        m_PhysicsBody->SetObject(Body_Barrel);
        m_DidCollide = false;
        m_Existance = true;

		m_Detonator = 0;

		//Explosion
		m_Explosion = new AnimatedSprite("SpriteAtlas");

		m_Explosion->AddFrame("Explosion-1");
		m_Explosion->AddFrame("Explosion-2");
		m_Explosion->AddFrame("Explosion-3");
		m_Explosion->AddFrame("Explosion-4");
		m_Explosion->AddFrame("Explosion-5");
		m_Explosion->AddFrame("Explosion-6");
		m_Explosion->AddFrame("Explosion-7");
		m_Explosion->AddFrame("Explosion-8");
		m_Explosion->AddFrame("Explosion-9");
		m_Explosion->AddFrame("Explosion-10");
		m_Explosion->AddFrame("Explosion-11");
		m_Explosion->AddFrame("Explosion-12");
		m_Explosion->AddFrame("Explosion-13");
		m_Explosion->AddFrame("Explosion-14");
		m_Explosion->AddFrame("Explosion-15");
		m_Explosion->AddFrame("Explosion-16");

		m_Explosion->SetAnchor(Vector2(0.5f, 0.5f));

		
	}


	Barrel::~Barrel()
	{
        delete m_Barrel;
        delete m_Explosion;
        delete m_PhysicsBody;
        UnloadTexture("SpriteAtlas");
	}

	void Barrel::Update(double delta)
	{
        m_Barrel->SetPosition(Math::MetersToPixels(m_PhysicsBody->GetPosition()));
        m_Explosion->SetPosition(m_Barrel->GetPosition());
	}

	void Barrel::Draw()
	{
        if (m_DidCollide == false && m_Existance == true)
		{
			m_Barrel->Draw();
		}
		if (m_DidCollide == true)
		{
            m_Explosion->Draw();
            m_Existance = false;  
            if (m_Detonator > 0)
            {
                m_Barrel->Draw();
            }
		}
	}

    void Barrel::Explosions()
    {
        if (m_DidCollide == false)
        {
			Services::GetGraphics()->GetCamera()->Shake(2.5f, 0.5f);
            m_Explosion->SetFrameIndex(0);
            m_DidCollide = true;
			if (m_Detonator == 0)
			{
                m_PhysicsBody->GetCollider()->SetIsEnabled(false);
				m_Existance = false;
			}
			else if (m_Detonator == 1)
			{
				m_Barrel->UseFrame("Detonator-Blue");
                m_PhysicsBody->SetObject(Body_Blue);
			}
			else if (m_Detonator == 2)
			{
				m_Barrel->UseFrame("Detonator-Green");
                m_PhysicsBody->SetObject(Body_Green);
			}
        }
       
    }
    bool Barrel::DoesExist()
    {
        return m_Existance;
    }
    void Barrel::SetDetonator(int det)
    {
        m_Detonator = det;
    }
    void Barrel::RemoveDetonator()
    {
        m_PhysicsBody->GetCollider()->SetIsEnabled(false);
  
        m_Detonator = 0;
    }
	Physics::Body * Barrel::GetPhysicsBody()
	{
		return m_PhysicsBody;
	}
}