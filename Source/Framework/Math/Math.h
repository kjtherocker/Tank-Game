#ifndef __GameDev2D_Math_h__
#define __GameDev2D_Math_h__

#include "Vector2.h"

namespace GameDev2D
{
	//Local constants
	const float PTM_RATIO = 32.0f; //Pixel to meter ratio
	const float EPSILON = 0.0001f;

    namespace Math
    {
        float DegreesToRadians(float degrees);
        float RadiansToDegrees(float radians);

		Vector2 CalculateLinearVelocity(Vector2 displacement, float duration);
		Vector2 CalculateLinearVelocity(float speed, float angleInDegrees);
		Vector2 CalculateDisplacement(Vector2 velocity, float duration);
		Vector2 CalculateDisplacement(float aDistance, float aAngleInDegrees);

		float CalculateSpeed(Vector2 aVelocity);
		float CalculateDistance(Vector2 aDisplacement);
		float CalculateDistance(Vector2 pointA, Vector2 pointB);
		float CalculateDistanceSquared(Vector2 aPointA, Vector2 aPointB);
		
		Vector2 CalculateDirection(float angleInDegrees);

		float CalculateSineAmplitude(float value, float wavelength, float amplitude, float frequency);

		float CalculateAngle(float x, float y);
		float CalculateAngle(Vector2 vector);
		Vector2 CalculateOrbit(Vector2 aOrigin, float aAngleInDegrees, float aDistance);

		float PixelsToMeters(float pixels);
		Vector2 PixelsToMeters(Vector2 pixels);
		
		float MetersToPixels(float meters);
		Vector2 MetersToPixels(Vector2 meters);

		Vector2 Normalize(Vector2 vector);
		float Dot(Vector2 a, Vector2 b);

		Vector2 CalculateClosestPoint(Vector2 circle, float readius, Vector2 lineStart, Vector2 lineEnd);

		float Clamp(float value, float min, float max);

        float Random();                         //Returns a float value between 0.0 and 1.0
        float Random(float max);                //Returns a float value between 0.0 and a max value
        float Random(float min, float max);     //Returns a float value between a min value and a max value
    }
}

#endif