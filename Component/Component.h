#pragma once
#include "Math/Transform.h"


class Component
{
public:
	Component() = default;

	virtual ~Component() = default;

public:
	virtual void SetWorldLocation(const Vector3& Location)
	{
		WorldTransform.Location = Location;
	}

	Vector3 GetWorldLocation() const
	{
		return WorldTransform.Location;
	}

	virtual void SetWorldRotation(const Rotator& Rotation)
	{
		WorldTransform.Rotation = Rotation;
	}

	Rotator GetWorldRotation() const
	{
		return WorldTransform.Rotation;
	}

	void SetWorldTransform(const Transform& Transform)
	{
		WorldTransform = Transform;
	}

	Transform GetWorldTransform() const
	{
		return WorldTransform;
	}

protected:
	Transform WorldTransform;

};
