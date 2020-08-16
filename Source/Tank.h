#ifndef __TANK_H__
#define __TANK_H__

#include "Framework\Math\Vector2.h"
#include "Framework\Input\Keyboard.h"
#include "Shell.h"
namespace GameDev2D
{
	//Constants
	const Vector2 TANK_STARTING_POSITION[] = { Vector2(500.0f, 100.0f), Vector2(500.0f, 700.0f) };
	const float TANK_STARTING_ANGLE[] = { 90.0f, 270.0f };
	const float TANK_FORCE_AMOUNT = 60.0f;
	const float TANK_TORQUE_AMOUNT = 35.0f;
	const float TANK_ANGULAR_SPEED = 180.0f;
	const float TURRET_ANGULAR_SPEED = 180.0f;
	const Keyboard::Key TANK_DEFAULT_INPUT[] = { Keyboard::D, Keyboard::A, Keyboard::W, Keyboard::S, Keyboard::Left, Keyboard::Right, Keyboard::Spacebar };
	const double TANK_FIRE_DELAY = 1.0;

	enum TankInput
	{
		TankInputLeft = 0,
		TankInputRight,
		TankInputUp,
		TankInputDown,
		TankInputTurretLeft,
		TankInputTurretRight,
		TankInputFireTurret,
		TankInputCount
	};

	//Enum to manage the tank's color
	enum TankColor
	{
		TankBlue = 0,
		TankGreen
	};

	//Forward declarations
	class SpriteAtlas;

	//The Tank class definition
	class Tank : public Transformable
	{
	public:
		Tank(TankColor tankColor);
		~Tank();

		//Updates the tank
		void Update(double delta);

		//Draw the tank
		void Draw();

		//Returns the width and height of the tank
		float GetWidth();
		float GetHeight();

		//Sets the input key codes that control the tank
		void SetLeftInput(Keyboard::Key leftInput);
		void SetRightInput(Keyboard::Key rightInput);
		void SetUpInput(Keyboard::Key upInput);
		void SetDownInput(Keyboard::Key downInput);
		void SetTurretLeftInput(Keyboard::Key leftTurretInput);
		void SetTurretRightInput(Keyboard::Key rightTurretInput);
		void SetFireTurretInput(Keyboard::Key fireTurretInput);

		Vector2 GetShellPosition();
		float GetTurretAngle();

		bool CanFireShell();
		void ResetFireDelay();

        void DestroyTank();
        Physics::Body* GetPhysicsBody();

        void SetAmmoIsEmpty();
        float GetTimer();

	private:
		TankColor m_TankColor;
		Keyboard::Key m_Input[TankInputCount];
		Shell * m_Shell;
		SpriteAtlas* m_Body;
		SpriteAtlas* m_Turret;
		Physics::Body* m_PhysicsBody;

        AnimatedSprite * m_Explosion;
        float m_Timer;

		double m_FireDelay;
        bool m_Destroyed;
        bool m_AmmoisEmpty;
        
	};
}

#endif