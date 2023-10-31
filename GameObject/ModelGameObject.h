#pragma once
#include "GameObject.h"
#include "Mesh/Mesh.h"
#include "Material/Material.h"

class ModelGameObject : public GameObject
{
public:
    ModelGameObject(const std::string& Name): GameObject(Name) {}
    virtual ~ModelGameObject() = default;
    ModelGameObject() = delete;

    void SetMesh(Mesh* mesh) { m_mesh = mesh; }
    void SetMaterial(Material* material) { m_material = material; }
    void Draw(ID3D12GraphicsCommandList* cmd_list, DescriptorCacheGPU *descriptor_cache);

private:
    Mesh* m_mesh = nullptr;
    Material* m_material = nullptr;

};
