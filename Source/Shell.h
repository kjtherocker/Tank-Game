#ifndef __SHELL_H__
#define __SHELL_H__

#include "Framework\Math\Vector2.h"
#include "Constants.h"

//Shell constants
const float SHELL_SPEED = 1000.0f;
const float SHELL_FORCE = 60.0f;

namespace GameDev2D
{
	//Forward declaration
	class Sprite;
	namespace Physics
	{
		class Body;
	}

	class Shell : public Transformable
	{
	public:
		Shell();
		~Shell();

		void Update(double delta);
		void Draw();

		float GetWidth();
		float GetHeight();

		bool IsEnabled();

		void Disable();

		void Fire(Vector2 position, float angleInDegrees);
        Physics::Body* GetPhysicsBody();
	private:
		//Member variables
		bool m_IsEnabled;
		SpriteAtlas* m_Sprite;
		Physics::Body* m_PhysicsBody;
	};
}

#endif