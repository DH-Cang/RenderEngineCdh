#include "GameObject.h"

Component *GameObject::GetRootComponent() const
{
    return m_root_component;
}

void GameObject::SetGameObjectTransform(const Transform &new_transform)
{
    m_root_component->SetWorldTransform(new_transform);
}

Transform GameObject::GetGameObjectTransform() const
{
    return m_root_component->GetWorldTransform();
}

void GameObject::SetGameObjectLocation(const Vector3 &new_location)
{
    m_root_component->SetWorldLocation(new_location);
}

void GameObject::SetGameObjectLocation(const float x, const float y, const float z)
{
    SetGameObjectLocation(Vector3(x, y, z));
}

Vector3 GameObject::GetGameObjectLocation() const
{
    return m_root_component->GetWorldLocation();
}

void GameObject::SetGameObjectRotation(const Rotator &new_rotator)
{
    m_root_component->SetWorldRotation(new_rotator);
}

//Roll: Angle of rotation around the z-axis, in degrees.
//pitch: Angle of rotation around the x-axis, in degrees.
//yaw: Angle of rotation around the y-axis, in degrees.
void GameObject::SetGameObjectRotation(const float roll, const float pitch, const float yaw)
{
    Rotator rotate(roll, pitch, yaw);
    SetGameObjectRotation(rotate);
}

Rotator GameObject::GetGameObjectRotation() const
{
    return m_root_component->GetWorldRotation();
}
