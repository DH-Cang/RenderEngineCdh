#pragma once
#include <unordered_map>
#include <string>
#include "Mesh.h"
#include "GeometryGenerator.h"

class MeshManager
{
public:
    MeshManager() = default;
    ~MeshManager() = default;

    Mesh* GetMesh(const std::string& name);
    void LoadMeshFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList); // called when init
    void ReleaseUploadBuffer(); // called between init and runtime

private:
    std::unordered_map<std::string, Mesh> m_meshes;
};

