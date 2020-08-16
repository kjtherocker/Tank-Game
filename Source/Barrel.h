#pragma once

#include <GameDev2D.h>
#include "Shell.h"
#include "Constants.h"

namespace GameDev2D
{

	class Body;

	class Barrel : public Transformable
	{

	public:
		Barrel(Vector2 Postion);
		~Barrel();

		void Update(double delta);

		void Draw();

        void Explosions();
        bool DoesExist();
        void SetDetonator(int det);
        void RemoveDetonator();

		Physics::Body* GetPhysicsBody();
	private:
		SpriteAtlas* m_Barrel;
		AnimatedSprite * m_Explosion;
        Physics::Body* m_PhysicsBody;

        bool  m_Existance;
		bool m_DidCollide;
        int m_Detonator;
	};
}
