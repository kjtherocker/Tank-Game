#include "GameDev2D.h"
#include "Tank.h"
#include "Framework\Graphics\SpriteAtlas.h"
#include "Framework\Math\Math.h"

namespace GameDev2D
{
	Tank::Tank(TankColor aTankColor) :
		m_TankColor(aTankColor),
		m_Body(nullptr),
		m_Turret(nullptr),
		m_PhysicsBody(nullptr),
		m_FireDelay(0.0),
        m_Destroyed(false)
	{
		//Initialize the Tank's position and angle based on its color
		SetPosition(TANK_STARTING_POSITION[m_TankColor]);
		SetAngle(TANK_STARTING_ANGLE[m_TankColor]);

		//Initialize the input array
		for (int i = 0; i < TankInputCount; i++)
		{
			m_Input[i] = TANK_DEFAULT_INPUT[i];
		}

		string atlasKey[] = { "Tank-Blue", "Tank-Green" };

  
        m_AmmoisEmpty = false;

        m_Timer = 0;
		m_Shell = new Shell();
		//Create the 'Tank' Sprite object
		m_Body = new SpriteAtlas("SpriteAtlas");
		m_Body->UseFrame(atlasKey[m_TankColor]);
		m_Body->SetAnchor(Vector2(0.5f, 0.5f));
		m_Body->AttachTo(this);

		//Create the 'Turret' Sprite object
		m_Turret = new SpriteAtlas("SpriteAtlas");
		m_Turret->UseFrame("Turret");
		m_Turret->SetAnchor(Vector2(0.2f, 0.5f));
		m_Turret->AttachTo(this);

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

        m_Explosion->SetAnchor(Vector2(0.9f, 0.5f));
        m_Explosion->AttachTo(this);

        //Initialize the parameters to create the Physics body
        float density = 2.0f;
        float width = Math::PixelsToMeters(GetWidth());
        float height = Math::PixelsToMeters(GetHeight());
        Physics::BoxCollider* collider = new Physics::BoxCollider(width, height);

		//Initialize the physics body
		m_PhysicsBody = Physics::World::GetInstance()->CreateBody(collider, density);
		m_PhysicsBody->SetPosition(Math::PixelsToMeters(GetPosition()));
		m_PhysicsBody->SetAngle(Math::DegreesToRadians(GetAngle()));
		m_PhysicsBody->SetMass(18.4082031f);
		m_PhysicsBody->SetInertia(47.6998405f);
		m_PhysicsBody->SetLinearDamping(Vector2(1.5f, 1.5f));
		m_PhysicsBody->SetAngularDamping(1.5f);
		m_PhysicsBody->SetObject(Body_Tank);
	}

	Tank::~Tank()
	{
        delete m_Turret;
		delete m_Body;
        delete m_PhysicsBody;
        delete m_Shell;
	}

	void Tank::Update(double aDelta)
	{

        if (m_AmmoisEmpty == true)
        {
            m_Timer += aDelta;
          
        }
 

		m_Shell->Update(aDelta);
		if (m_PhysicsBody == nullptr)
		{
			return;
		}

		//Countdown the fire delay timer, if its above zero
		if (m_FireDelay > 0.0)
		{
			m_FireDelay -= aDelta;
			if (m_FireDelay <= 0.0)
			{
				m_FireDelay = 0.0;
			}

	
		}

        //Impulse Movement
        Vector2 LateralNormal = Vector2(0, 1);
        float c = cosf((m_PhysicsBody->GetAngle()));
        float s = sinf((m_PhysicsBody->GetAngle()));
        Vector2 rotatedNormal;
        rotatedNormal.x = c * LateralNormal.x - s * LateralNormal.y;
        rotatedNormal.y = s * LateralNormal.x + c * LateralNormal.y;
        //Getting magnitude
        float magnitude = (rotatedNormal.DotProduct(m_PhysicsBody->GetLinearVelocity()));
        //Getting lateral velcoity
        Vector2 lateralVelocity = rotatedNormal * magnitude;
        //Getting impulse
        Vector2  impulse = -lateralVelocity * m_PhysicsBody->GetMass();

        //Applying Linear Pulses
        m_PhysicsBody->ApplyLinearImpulse(impulse);
  
        //Applying angular impuse
        float Angularimpulse = 0.1 * m_PhysicsBody->GetInertia() * -m_PhysicsBody->GetAngularVelocity();
        m_PhysicsBody->ApplyAngularImpulse(Angularimpulse);
  


            
            
		//Set the Polygon's position and angle, using the data from the Physics Body
		SetPosition(Math::MetersToPixels(m_PhysicsBody->GetPosition()));
		SetAngle(Math::RadiansToDegrees(m_PhysicsBody->GetAngle()));

        if (m_Destroyed == false)
        {
            //Handle the turret rotation angle
            if (IsKeyDown(m_Input[TankInputTurretLeft]) == true)
            {
                m_Turret->SetAngle(m_Turret->GetAngle() + TURRET_ANGULAR_SPEED * aDelta);
            }
            if (IsKeyDown(m_Input[TankInputTurretRight]) == true)
            {
                m_Turret->SetAngle(m_Turret->GetAngle() - TURRET_ANGULAR_SPEED * aDelta);
            }

            //Reset the tank's speed
            float force = 0.0f;

            //Handle the tank's speed controls
            if (IsKeyDown(m_Input[TankInputUp]) == true)
            {
                force += TANK_FORCE_AMOUNT;
            }
            if (IsKeyDown(m_Input[TankInputDown]) == true)
            {
                force -= TANK_FORCE_AMOUNT;
            }

            //Apply the force to the physics body 
            Vector2 force2d = Vector2(cosf(m_PhysicsBody->GetAngle()) * force, sinf(m_PhysicsBody->GetAngle()) * force);
            m_PhysicsBody->ApplyForces(force2d);

            //Local variable to keep track of the amount of torque to apply
            float torque = 0.0f;

            //Handle the tank's rotation controls


            if (IsKeyDown(m_Input[TankInputLeft]) == true)
            {
                torque += TANK_TORQUE_AMOUNT;
            }

            if (IsKeyDown(m_Input[TankInputRight]) == true)
            {
                torque -= TANK_TORQUE_AMOUNT;
            }

            //Apply the torque to the physics body 
            if (force != 0.0)
            {
                m_PhysicsBody->ApplyTorque(torque);
            }
            m_PhysicsBody->SyncForces(aDelta, Vector2::Zero);
            m_PhysicsBody->SyncVelocities(aDelta);
            m_PhysicsBody->ClearForces();
        }
        if (m_Destroyed == true)
        {
            m_Body->UseFrame("Tank-Exploded");
            m_Turret->UseFrame("Turret-Exploded");
        }

	}

	void Tank::Draw()
	{
		m_Body->Draw();
		m_Turret->Draw();
		m_Shell->Draw();
        if (m_Destroyed == true)
        {
            m_Explosion->Draw();
        }
	}

	float Tank::GetWidth()
	{
		return m_Body->GetWidth();
	}

	float Tank::GetHeight()
	{
		return m_Body->GetHeight();
	}

	void Tank::SetLeftInput(Keyboard::Key aLeftInput)
	{
		m_Input[TankInputLeft] = aLeftInput;
	}

	void Tank::SetRightInput(Keyboard::Key aRightInput)
	{
		m_Input[TankInputRight] = aRightInput;
	}

	void Tank::SetUpInput(Keyboard::Key aUpInput)
	{
		m_Input[TankInputUp] = aUpInput;
	}

	void Tank::SetDownInput(Keyboard::Key aDownInput)
	{
		m_Input[TankInputDown] = aDownInput;
	}

	void Tank::SetTurretLeftInput(Keyboard::Key aLeftTurretInput)
	{
		m_Input[TankInputTurretLeft] = aLeftTurretInput;
	}

	void Tank::SetTurretRightInput(Keyboard::Key aRightTurretInput)
	{
		m_Input[TankInputTurretRight] = aRightTurretInput;
	}

	void Tank::SetFireTurretInput(Keyboard::Key aFireTurretInput)
	{   
		m_Input[TankInputFireTurret] = aFireTurretInput;
	}

	Vector2 Tank::GetShellPosition()
	{
		//Calculate the starting position of the shell
		float turretLength = m_Turret->GetWidth() * (1.0f - m_Turret->GetAnchor().x);
		Vector2 offset = Math::CalculateDirection(GetTurretAngle()) * Vector2(turretLength, turretLength);
		Vector2 position = (m_Turret->GetPosition() + GetPosition());

		return position + offset;
	}

	float Tank::GetTurretAngle()
	{
		return m_Turret->GetAngle() + GetAngle();
	}
	bool Tank::CanFireShell()
	{
        if (m_AmmoisEmpty == false)
        {
            return IsKeyDown(m_Input[TankInputFireTurret]) && m_FireDelay <= 0.0;
    
        }
        else if(m_AmmoisEmpty == true)
        {
        
        }
    }
	void Tank::ResetFireDelay()
	{
		m_FireDelay = TANK_FIRE_DELAY;
	}
    void Tank::DestroyTank()
    {
        m_Destroyed = true;
        Services::GetGraphics()->GetCamera()->Shake(2.5f, 0.5f);
        m_Explosion->SetFrameIndex(0);
    }
    Physics::Body * Tank::GetPhysicsBody()
    {
        return m_PhysicsBody;
    }

    void Tank::SetAmmoIsEmpty()
    {
         m_AmmoisEmpty = true;
         if (m_Timer >= 3)
         { 
             m_AmmoisEmpty = false;
         }
    }

    float Tank::GetTimer()
    {
        return m_Timer;
    }

    
}