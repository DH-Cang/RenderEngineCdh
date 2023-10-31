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

Vector3 GameObject::GetGameObjectLocation() const
{
    return m_root_component->GetWorldLocation();
}

void GameObject::SetGameObjectRotation(const Rotator &new_rotator)
{
    m_root_component->SetWorldRotation(new_rotator);
}

Rotator GameObject::GetGameObjectRotation() const
{
    return m_root_component->GetWorldRotation();
}
