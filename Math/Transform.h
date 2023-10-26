#pragma once

#include "Math.h"

struct Rotator
{
	float Roll; //Angle of rotation around the z-axis, in degrees.
	float Pitch; // Angle of rotation around the x-axis, in degrees.
	float Yaw; // Angle of rotation around the y-axis, in degrees.

	Rotator()
		:Roll(0.0f), Pitch(0.0f), Yaw(0.0f)
	{}

	Rotator(float InRoll, float InPitch, float InYaw)
		:Roll(InRoll), Pitch(InPitch), Yaw(InYaw)
	{}

	static const Rotator Zero;
};

class Transform
{
public:
	Transform()
	{
		Location = Vector3::Zero;
		Rotation = Rotator::Zero;
		Scale = Vector3::One;
	}

	Matrix GetTransformMatrixLH() const
	{
		Matrix S = Matrix::CreateScale(Scale);
		Matrix R = Matrix::CreateFromYawPitchRoll(DegreesToRadians(Rotation.Yaw), DegreesToRadians(Rotation.Pitch), DegreesToRadians(Rotation.Roll));
		Matrix T = Matrix::CreateTranslation(Location);

		return S * R * T;
	}

    Matrix GetTransformMatrixRH() const
    {
        return GetTransformMatrixLH().Transpose();
    }

public:
	Vector3 Location;
	Rotator Rotation;
	Vector3 Scale;
};
