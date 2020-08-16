#include "Math.h"
#include <stdlib.h>
#include <complex>

namespace GameDev2D
{
    namespace Math
    {
        float DegreesToRadians(float aDegrees)
        {
            return aDegrees * 0.01745329251f;
        }

        float RadiansToDegrees(float aRadians)
        {
            return aRadians * 57.2957795131f;
        }

		Vector2 CalculateLinearVelocity(Vector2 aDisplacement, float aDuration)
		{
			return Vector2(aDisplacement.x / aDuration, aDisplacement.y / aDuration);
		}

		Vector2 CalculateLinearVelocity(float aSpeed, float aAngleInDegrees)
		{
			Vector2 direction = CalculateDirection(aAngleInDegrees);
			Vector2 linearVelocity = direction * aSpeed;
			return linearVelocity;
		}

		float CalculateSpeed(Vector2 aVelocity)
		{
			float speed = sqrtf(aVelocity.x * aVelocity.x + aVelocity.y * aVelocity.y);
			return speed;
		}

		Vector2 CalculateDisplacement(float aDistance, float aAngleInDegrees)
		{
			Vector2 direction = Math::CalculateDirection(aAngleInDegrees);
			Vector2 displacement = Vector2(direction.x * aDistance, direction.y * aDistance);
			return displacement;
		}

		Vector2 CalculateDisplacement(Vector2 aVelocity, float aDuration)
		{
			return Vector2(aVelocity.x * aDuration, aVelocity.y * aDuration);
		}

		float CalculateDistance(Vector2 aPointA, Vector2 aPointB)
		{
			float xSquared = (aPointB.x - aPointA.x) * (aPointB.x - aPointA.x);
			float ySquared = (aPointB.y - aPointA.y) * (aPointB.y - aPointA.y);
			float distance = std::sqrtf(xSquared + ySquared);
			return distance;
		}

		float CalculateDistance(Vector2 aDisplacement)
		{
			float distance = sqrtf(aDisplacement.x * aDisplacement.x + aDisplacement.y * aDisplacement.y);
			return distance;
		}

		float CalculateDistanceSquared(Vector2 aPointA, Vector2 aPointB)
		{
			float xSquared = (aPointB.x - aPointA.x) * (aPointB.x - aPointA.x);
			float ySquared = (aPointB.y - aPointA.y) * (aPointB.y - aPointA.y);
			return xSquared + ySquared;
		}

		Vector2 CalculateDirection(float aAngleInDegrees)
		{
			//Convert the angle to radians
			float angleInRadians = DegreesToRadians(aAngleInDegrees);

			//Calculate the direction vector
			Vector2 direction = Vector2(cosf(angleInRadians), sinf(angleInRadians));
			return direction;
		}

		float CalculateSineAmplitude(float aValue, float aWaveLength, float aAmplitude, float aFrequency)
		{
			float wavelength = aWaveLength / (M_PI * 2.0f);
			float frequency = aFrequency * (M_PI * 2.0f);
			return sinf(frequency + (aValue / wavelength)) * aAmplitude;
		}

		float CalculateAngle(float aX, float aY)
		{
			float angleInRadians = atan2f(aY, aX);
			float angleInDegrees = RadiansToDegrees(angleInRadians);
			return angleInDegrees;
		}

		float CalculateAngle(Vector2 aVector)
		{
			return CalculateAngle(aVector.x, aVector.y);
		}

		Vector2 CalculateOrbit(Vector2 aOrigin, float aAngleInDegrees, float aDistance)
		{
			Vector2 offset = Math::CalculateDisplacement(aDistance, aAngleInDegrees);
			return aOrigin + offset;
		}

		float PixelsToMeters(float aPixels)
		{
			return aPixels / PTM_RATIO;
		}

		Vector2 PixelsToMeters(Vector2 aPixels)
		{
			return aPixels / PTM_RATIO;
		}

		float MetersToPixels(float aMeters)
		{
			return aMeters * PTM_RATIO;
		}

		Vector2 MetersToPixels(Vector2 aMeters)
		{
			return aMeters * PTM_RATIO;
		}

		Vector2 Normalize(Vector2 aVector)
		{
			float magnitude = Math::CalculateDistance(Vector2(0.0f, 0.0f), aVector);
			if (magnitude > EPSILON)
			{
				aVector /= magnitude;
			}

			return aVector;
		}

		float Dot(Vector2 aA, Vector2 aB)
		{
			return aA.x * aB.x + aA.y * aB.y;
		}

		Vector2 CalculateClosestPoint(Vector2 circle, float radius, Vector2 lineStart, Vector2 lineEnd)
		{
            //Calculate the circle vector
            Vector2 circleVector = circle - lineStart;

            //Calculate the line segment vector
            Vector2 lineVector = lineEnd - lineStart;

            //Normalize the line segment vector        			
            Vector2 normalizedVector = Math::Normalize(lineVector);

            //Calculate the dot product between the circle vector and the normalized line segment vector       			
            float magnitude = Math::Dot(normalizedVector, circleVector);

            //Calculate the projection using the result of the dot product and multiply it by the normalized line segment
            Vector2 projection = normalizedVector * magnitude;

            //Calculate the closest point on the line segment, by adding the project vector to the line start vector
            Vector2 closestPoint = lineStart + projection;
            return closestPoint;
		}

        float Clamp(float aValue, float aMin, float aMax)
        {
            if (aValue < aMin)
            {
                return aMin;
            }

            if (aValue > aMax)
            {
                return aMax;
            }

            return aValue;
        }

        float Random()
        {
            return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }

        float Random(float aMax)
        {
            return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / aMax));
        }

        float Random(float aMin, float aMax)
        {
            return aMin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (aMax - aMin)));
        }
    }
}