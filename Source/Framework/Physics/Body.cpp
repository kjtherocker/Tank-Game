#include "Body.h"
#include "Collider.h"
#include "../Core/Transformable.h"

namespace GameDev2D
{
	namespace Physics
	{
		Body::Body(Collider* aCollider, float aDensity) :
			m_Position(0.0f, 0.0f),
			m_Angle(0.0f),
			m_LinearVelocity(0.0f, 0.0f),
			m_AngularVelocity(0.0f),
			m_Force(0.0f, 0.0f),
			m_Torque(0.0f),
			m_LinearDamping(0.0f, 0.0f),
			m_AngularDamping(0.0f),
			m_Mass(0.0f),
			m_InverseMass(0.0f),
			m_Inertia(0.0f),
			m_InverseInertia(0.0f),

			m_Collider(aCollider)
  
		{
			SetMass(m_Collider->ComputeMass(aDensity));
			SetInertia(m_Collider->ComputeInertia(GetMass()));

			m_Collider->SetBody(this);
		}

		Body::~Body()
		{
			if (m_Collider != nullptr)
			{
				delete m_Collider;
				m_Collider = nullptr;
			}
		}

		void Body::SetPosition(Vector2 aPosition)
		{
			m_Position = aPosition;
		}

		Vector2 Body::GetPosition()
		{
			return m_Position;
		}

		void Body::SetAngle(float aAngle)
		{
			m_Angle = aAngle;
			m_Collider->SetAngle(m_Angle);
		}

		float Body::GetAngle()
		{
			return m_Angle;
		}

		void Body::SetLinearVelocity(Vector2 aLinearVelocity)
		{
			m_LinearVelocity = aLinearVelocity;
		}

		Vector2 Body::GetLinearVelocity()
		{
			return m_LinearVelocity;
		}

		void Body::SetAngularVelocity(float aAngularVelocity)
		{
			m_AngularVelocity = aAngularVelocity;
		}

		float Body::GetAngularVelocity()
		{
			return m_AngularVelocity;
		}

		void Body::SetLinearDamping(Vector2 aLinearDamping)
		{
			m_LinearDamping = aLinearDamping;
		}

		Vector2 Body::GetLinearDamping()
		{
			return m_LinearDamping;
		}

		void Body::SetAngularDamping(float aAngularDamping)
		{
			m_AngularDamping = aAngularDamping;
		}

		float Body::GetAngularDamping()
		{
			return m_AngularDamping;
		}

		void Body::SetMass(float aMass)
		{
			m_Mass = aMass;
			m_InverseMass = (m_Mass == 0.0f ? 0.0f : 1.0f / m_Mass);
		}

		float Body::GetMass()
		{
			return m_Mass;
		}

		float Body::GetInverseMass()
		{
			return m_InverseMass;
		}

		void Body::SetInertia(float aInertia)
		{
			m_Inertia = aInertia;
			m_InverseInertia = (m_Inertia == 0.0f ? 0.0f : 1.0f / m_Inertia);
		}

		float Body::GetInertia()
		{
			return m_Inertia;
		}

		float Body::GetInverseInertia()
		{
			return m_InverseInertia;
		}

		void Body::ApplyForces(Vector2 aForce)
		{
			m_Force += aForce;
		}

		void Body::ApplyTorque(float aTorque)
		{
			m_Torque += aTorque;
		}

		void Body::ApplyLinearImpulse(Vector2 aLinearImpulse)
		{
			m_LinearVelocity += aLinearImpulse * m_InverseMass;
		}

		void Body::ApplyAngularImpulse(float aAngularImpulse)
		{
			m_AngularVelocity += aAngularImpulse * m_InverseInertia;
		}

		void Body::ClearForces()
		{
			m_Force = Vector2(0.0f, 0.0f);
			m_Torque = 0.0f;
		}

		void Body::SyncForces(double aTimeStep, Vector2 aGravity)
		{
			if (GetInverseMass() == 0.0f)
			{
				return;
			}

			//Calculate the acceleration based on the mass and force
			Vector2 acceleration = m_Force * GetInverseMass();
			m_LinearVelocity.x += acceleration.x * aTimeStep;
			m_LinearVelocity.y += acceleration.y * aTimeStep;

			//Next apply the effects of gravity
			m_LinearVelocity.x += aGravity.x * aTimeStep;
			m_LinearVelocity.y += aGravity.y * aTimeStep;

			//Calculate the angular acceleration
			m_AngularVelocity += (m_Torque * GetInverseInertia()) * aTimeStep;

			//Lastly apply damping
			m_LinearVelocity.x *= 1.0f / (1.0f + m_LinearDamping.x * aTimeStep);
			m_LinearVelocity.y *= 1.0f / (1.0f + m_LinearDamping.y * aTimeStep);
			m_AngularVelocity *= 1.0f / (1.0f + m_AngularDamping * aTimeStep);
		}

		void Body::SyncVelocities(double aTimeStep)
		{
			m_Position.x += m_LinearVelocity.x * aTimeStep;
			m_Position.y += m_LinearVelocity.y * aTimeStep;
			m_Angle += m_AngularVelocity * aTimeStep;
		}
        BodyTypes Body::GetObject1()
        {
            return m_Object;
        }

        void Body::SetObject(BodyTypes object)
        {
            m_Object = object;
        }

        Collider* Body::GetCollider()
        {
            return m_Collider;
        }

    }
}