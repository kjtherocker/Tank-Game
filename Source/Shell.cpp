#include "Shell.h"
#include "GameDev2D.h"


namespace GameDev2D
{
	Shell::Shell():
		m_Sprite(),
		m_PhysicsBody()
	{
		//Create the Sprite and add it as a child
		m_Sprite = new SpriteAtlas("SpriteAtlas");
		m_Sprite->UseFrame("Shell");
		m_Sprite->SetAnchor(Vector2(1.0f, 0.5f));
		m_Sprite->AttachTo(this);

		//Initialize the Shell to be disabled
		m_IsEnabled = false;

        //Initialize the parameters to create the Physics body
        float density = 1.0f;
        float radius = Math::PixelsToMeters(GetHeight() / 2);
        Physics::CircleCollider* collider = new Physics::CircleCollider(radius);

        collider->SetIsEnabled(false);

		//Create the Physics body to attach to the tank
		m_PhysicsBody = Physics::World::GetInstance()->CreateBody(collider, density);
		m_PhysicsBody->SetLinearDamping(Vector2(0.2f, 0.2));
		m_PhysicsBody->SetAngularDamping(1.0f);
        m_PhysicsBody->SetObject(Body_Shell);
	}

	Shell::~Shell()
	{
		m_Sprite = nullptr;
		m_PhysicsBody = nullptr;
	}

	void Shell::Update(double aDelta)
	{
		//If the shell is enabled, update it and check it's bounds
		if (m_IsEnabled == true)
		{
			//Set the Polygon's position and angle, using the data from the Physics Body
			SetPosition(Math::MetersToPixels(m_PhysicsBody->GetPosition()));
			SetAngle(Math::RadiansToDegrees(m_PhysicsBody->GetAngle()));

			//If the shell went off-screen, disable it
			if (GetPosition().x < 0.0f || GetPosition().x > GetScreenWidth() || GetPosition().y < 0.0f || GetPosition().y > GetScreenHeight())
			{
				m_IsEnabled = false;
				m_PhysicsBody->ClearForces();
			}
		}
        else if (m_IsEnabled == false)
        {
            m_PhysicsBody->ClearForces();
            m_PhysicsBody->GetCollider()->SetIsEnabled(false);
        }
	}

	void Shell::Draw()
	{
		if (m_IsEnabled == true)
		{
			m_Sprite->Draw();
		}
	}

	float Shell::GetWidth()
	{
		return m_Sprite->GetWidth();
	}

	float Shell::GetHeight()
	{
		return m_Sprite->GetHeight();
	}

	bool Shell::IsEnabled()
	{
		return m_IsEnabled;
	}

	void Shell::Disable()
	{
        m_IsEnabled = false;
        m_PhysicsBody->GetCollider()->SetIsEnabled(m_IsEnabled);
	}


	void Shell::Fire(Vector2 aPosition, float aAngleInDegrees)
	{
		m_IsEnabled = true;
        m_PhysicsBody->GetCollider()->SetIsEnabled(m_IsEnabled);

		//Is there a physics Body set?
		if (m_PhysicsBody != nullptr)
		{
			//Reset the Body's linear and angular velocities
			m_PhysicsBody->SetLinearVelocity(Vector2(0.0f, 0.0f));
			m_PhysicsBody->SetAngularVelocity(0.0f);

			//Set the Shell's position, angle and linear velocity
			m_PhysicsBody->SetPosition(Math::PixelsToMeters(aPosition));

			//
			float angleInRadians = Math::DegreesToRadians(aAngleInDegrees);
			m_PhysicsBody->SetAngle(angleInRadians);

			//
			Vector2 force2d = Vector2(cosf(angleInRadians) * SHELL_FORCE, sinf(angleInRadians) * SHELL_FORCE);
			m_PhysicsBody->ApplyForces(force2d);
		}
		else
		{
			SetPosition(aPosition);
			SetAngle(aAngleInDegrees);
		}
	}
    Physics::Body * Shell::GetPhysicsBody()
    {
        return m_PhysicsBody;
    }
}