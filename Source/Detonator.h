#include "Framework\Math\Vector2.h"
#include "Barrel.h"
#include "Tank.h"
#include "Framework\Physics\Manifold.h"
#include "Framework\Physics\World.h"

namespace Physics
{
    class Body;
}


namespace GameDev2D
{
    class Detonator : public Transformable
    {
    public:
        Detonator(bool gr);
        Detonator();
        ~Detonator();

        void Update(double delta);

        float GetWidth();
        float GetHeight();

        void Draw();

        void CheckCollision(Vector2 TankPos);

        void SetPosition(Vector2 loc);

		void Enable();

        void Disable();

        bool GetEnabled();

    private:
        Barrel* m_barrel;
        SpriteAtlas* m_Det;
        
        bool m_Activated;

        bool green;

        Vector2 m_Position;

        Physics::Body* m_PhysicsBody;

        
    };
}