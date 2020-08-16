#include "World.h"
#include "Body.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "../Math/Math.h"
#include "../GameDev2D.h"

namespace GameDev2D
{
	namespace Physics
	{
		World* World::s_Instance = nullptr;

		World* World::GetInstance()
		{
			if (s_Instance == nullptr)
			{
				s_Instance = new World();
			}
			return s_Instance;
		}

		World::World():
			m_Gravity(0.0f, 0.0f),
			m_Listener(nullptr)
		{

		}

		World::~World()
		{
			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				delete m_Bodies.at(i);
			}
			m_Bodies.clear();
		}

		void World::Step(double aTimeStep)
		{
			m_Contacts.clear();



			//Check Collision
			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				for (unsigned int j = 0; j < m_Bodies.size(); j++)
				{
					Body* a = m_Bodies.at(i);
					Body* b = m_Bodies.at(j);

					if (a->GetInverseMass() == 0.0f || b->GetInverseMass() == 0.0f)
					{
						continue;
					}

					Manifold manifold(a, b);

					//Check the collision
					if (CheckCollision(a, b, &manifold) == true)
					{
						//There was a collision, notify the listener
						if (m_Listener != nullptr)
						{
							if (m_Listener->CollisionCallBack(a, b) == true)
							{
								m_Contacts.push_back(manifold);
							}
						}
						else
						{
							m_Contacts.push_back(manifold);
						}
					}
				}
			}

			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				m_Bodies.at(i)->SyncForces(aTimeStep, m_Gravity);
			}

			for (unsigned int i = 0; i < m_Contacts.size(); i++)
			{
				m_Contacts.at(i).CorrectOverlap();
			}

			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				m_Bodies.at(i)->SyncVelocities(aTimeStep);
			}

			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				m_Bodies.at(i)->ClearForces();
			}
		}

		void World::DebugDraw()
		{
#ifdef DEBUG_DRAW
			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				Body* body = m_Bodies.at(i);
				Collider* collider = body->GetCollider();

				if (collider->GetType() == ColliderType_Circle)
				{
					CircleCollider* circle = (CircleCollider*)collider;
					float radius = Math::MetersToPixels(circle->GetRadius());
					Vector2 position = Math::MetersToPixels(body->GetPosition());

					GameDev2D::DrawCircle(position.x - radius, position.y - radius, radius, Color::YellowColor(), false);
				}
				else if (collider->GetType() == ColliderType_Box)
				{
					BoxCollider* box = (BoxCollider*)collider;
					float width = Math::MetersToPixels(box->GetWidth());
					float height = Math::MetersToPixels(box->GetHeight());
					Vector2 position = Math::MetersToPixels(body->GetPosition());
					float angle = Math::RadiansToDegrees(body->GetAngle());

					GameDev2D::DrawRectangle(position.x, position.y, width, height, angle, Color::YellowColor(), false);
				}
			}
#endif
		}

		Body* World::CreateBody(Collider* aCollider, float aDensity)
		{
			Body* body = new Body(aCollider, aDensity);
			m_Bodies.push_back(body);
			return body;
		}

		void World::SetGravity(Vector2 aGravity)
		{
			m_Gravity = aGravity;
		}

		Vector2 World::GetGravity()
		{
			return m_Gravity;
		}

		void World::SetListener(WorldListener* aListener)
		{
			m_Listener = aListener;
		}


		bool World::CheckCollision(Body* aBodyA, Body* aBodyB, Manifold* aManifold)
		{
			bool result = false;

			if (aBodyA == aBodyB)
			{
				return result;
			}

			//Determine the type of collision check to perform
			if (aBodyA->GetCollider()->GetType() == ColliderType_Circle &&
				aBodyB->GetCollider()->GetType() == ColliderType_Circle &&
				aBodyA != aBodyB)
			{
				result = CheckCircleToCircle(aBodyA, aBodyB, aManifold);
			}
			else if (aBodyA->GetCollider()->GetType() == ColliderType_Circle &&
				aBodyB->GetCollider()->GetType() == ColliderType_Box)
			{
				result = CheckCircleToBox(aBodyA, aBodyB, aManifold);
			}
			else if (aBodyA->GetCollider()->GetType() == ColliderType_Box &&
				aBodyB->GetCollider()->GetType() == ColliderType_Circle)
			{
				result = CheckCircleToBox(aBodyB, aBodyA, aManifold);

				aManifold->FlipNormal();
			}

			return result;
		}

		bool World::CheckCircleToCircle(Body* aBodyA, Body* aBodyB, Manifold* aManifold)
		{
			//Get the circle colliders
			CircleCollider* circleColliderA = (CircleCollider*)aBodyA->GetCollider();
			CircleCollider* circleColliderB = (CircleCollider*)aBodyB->GetCollider();

			if (circleColliderA->GetIsEnabled() == false || circleColliderB->GetIsEnabled() == false)
			{
				return false;
			}

			//Check the distance against the radii for collision
			float distanceSquared = Math::CalculateDistanceSquared(aBodyA->GetPosition(), aBodyB->GetPosition());
			float combinedRadii = circleColliderA->GetRadius() + circleColliderB->GetRadius();
			float radiiSquared = combinedRadii * combinedRadii;

			if (distanceSquared > radiiSquared)
			{
				return false;
			}

			float distance = sqrtf(distanceSquared);
			float overlap = combinedRadii - distance;
			Vector2 normal = Vector2(aBodyB->GetPosition() - aBodyA->GetPosition()) / distance;
			aManifold->SetContact(overlap, normal);

			return true;
		}

		bool World::CheckCircleToBox(Body* aBodyA, Body* aBodyB, Manifold* aManifold)
		{
			CircleCollider* circleCollider = (CircleCollider*)aBodyA->GetCollider();
			BoxCollider* boxCollider = (BoxCollider*)aBodyB->GetCollider();

			float radius = circleCollider->GetRadius();
			float radiusSquared = radius * radius;
			float angle = boxCollider->GetAngle();
			float c = cosf(angle);
			float s = sinf(angle);
			float cx = aBodyA->GetPosition().x;
			float cy = aBodyA->GetPosition().y;
			float rx = aBodyB->GetPosition().x;
			float ry = aBodyB->GetPosition().y;

			float circleX = c* (cx - rx) - s* (cy - ry);
			float circleY = s* (cx - rx) + c* (cy - ry);
			Vector2 circleCenter = Vector2(circleX, circleY);

			float maxSeperation = -FLT_MAX;
			unsigned int faceNormal = 0;

			for (unsigned int i = 0; i < BOX_COLLIDER_VERTEX_COUNT; i++)
			{
				Vector2 vertex1 = boxCollider->GetNormalsAtIndex(i);
				Vector2 vertex2 = circleCenter - boxCollider->GetVerticesAtIndex(i);
				float seperation = Math::Dot(vertex1, vertex2);

				if (seperation > radius)
				{
					return false;
				}
				if (seperation > maxSeperation)
				{
					maxSeperation = seperation;
					faceNormal = i;
				}
			}
			if (maxSeperation < EPSILON)
			{
				float overlap = circleCollider->GetRadius();
				Vector2 faceNormalIV = boxCollider->GetNormalsAtIndex(faceNormal);
				Vector2 normal = Vector2(c * faceNormalIV.x + s* faceNormalIV.y, -s * faceNormalIV.x + c * faceNormalIV.y) * -1.0f;
				aManifold->SetContact(overlap, normal);

				return true;
			}
			Vector2 vertex1 = boxCollider->GetVerticesAtIndex(faceNormal);
			unsigned int index2 = (faceNormal + 1 < BOX_COLLIDER_VERTEX_COUNT) ? faceNormal + 1 : 0;
			Vector2 vertex2 = boxCollider->GetVerticesAtIndex(index2);

			Vector2 closestPoint = Math::CalculateClosestPoint(circleCenter, radius, vertex1, vertex2);

			float distance = Math::CalculateDistanceSquared(closestPoint, circleCenter);

			if (distance <= radiusSquared)
			{
				float overlap = radius - sqrtf(distance);
				Vector2 normal = circleCenter - closestPoint;
				normal = Vector2(c * normal.x + s * normal.y, -s * normal.x + c * normal.y) * -1.0f;
				normal = Math::Normalize(normal);
				aManifold->SetContact(overlap, normal);

				return true;
			}
			return false;
		}
	}
}