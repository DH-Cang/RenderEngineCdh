#pragma once
#include "Math/Math.h"
#include "Component/Component.h"
#include <vector>
#include <memory>


class GameObject
{
public:
	GameObject(const std::string& Name): m_name(Name) {}
	GameObject() = delete;

	virtual ~GameObject() {}

	virtual void Tick(float DeltaSeconds) {}

public:
	template<typename T>
	T* AddComponent()
	{
		auto NewComponent = std::make_unique<T>();
		T* Result = NewComponent.get();
		Components.push_back(std::move(NewComponent));

		return Result;
	}

	std::vector<Component*> GetComponents()
	{
		std::vector<Component*> ret;

		for (const auto& component : m_components)
		{
			ret.push_back(component.get());
		}

		return ret;
	}

	template<typename T>
	std::vector<T*> GetComponentsOfClass()
	{
		std::vector<T*> Result;
		for (const auto& component : m_components)
		{
			T* ComponentOfClass = dynamic_cast<T*>(component.get());
			if (ComponentOfClass)
			{
				Result.push_back(ComponentOfClass);
			}
		}

		return Result;
	}

    Component* GetRootComponent() const;

	virtual void SetGameObjectTransform(const Transform& new_transform);

	Transform GetGameObjectTransform() const;

	void SetGameObjectLocation(const Vector3& new_location);

	Vector3 GetGameObjectLocation() const;

	void SetGameObjectRotation(const Rotator& new_rotator);

	Rotator GetGameObjectRotation() const;

	void SetName(const std::string& new_name) { m_name = new_name; }

	std::string GetName() const { return m_name; }

protected:
	std::string m_name;

	std::vector<std::unique_ptr<Component>> m_components;

    Component* m_root_component = nullptr;
};