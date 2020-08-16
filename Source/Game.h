#pragma once

#include <GameDev2D.h>
#include "../Source/Framework/Physics/WorldListener.h"
#include "Tank.h"
#include "Shell.h"
#include "Barrel.h"
#include "Constants.h"

namespace GameDev2D
{
    const int SHELL_POOL_SIZE = 10;

    enum BodyTypes
    {
        Body_Tank = 0,
        Body_Barrel,
        Body_Shell,
        Body_Blue,
        Body_Green
    };



	//Class definition
	class Game : public Physics::WorldListener
	{
	public:
		Game();  //Constructor
		~Game(); //Destructor

		void Update(double delta);
		void Draw();

		void HandleLeftMouseClick(float mouseX, float mouseY);
		void HandleRightMouseClick(float mouseX, float mouseY);
        void HandleKeyPress(Keyboard::Key key);

		unsigned int RandomRange(unsigned int min, unsigned int max);

	private:
		//Return a disabled shell from the pool
        Shell* GetShell();

        //Private methods
        void UpdateTank(Tank* tank, double delta);

        //Collision callback, called whenever collision occurs
        bool CollisionCallBack(Physics::Body* bodyA, Physics::Body* bodyB);

        //Member variable should go here
        Tank* m_BlueTank;
        Tank* m_GreenTank;
		
        Shell* m_Shells[SHELL_POOL_SIZE];
        double m_TimeStep;
        int BarrelCount;
		Barrel* m_Barrel[48];

        Label* m_Winner;
        bool m_Win;

        SpriteAtlas* m_BlueShots[3];
        SpriteAtlas* m_GreenShots[3];

        int m_BlueCount;
        int m_GreenCount;

        Sprite* m_Background;
	};
}
