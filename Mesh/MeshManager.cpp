#include "MeshManager.h"


Mesh *MeshManager::GetMesh(const std::string &name)
{
    const auto& pair = m_meshes.find(name);
    assert(pair != m_meshes.end());
    return &(pair->second);
}

void MeshManager::LoadMeshFromFile(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList)
{
    // there is no obj file for now, so it just generates basic shape
    GeometryGenerator geo_generator;

    auto box_data = geo_generator.CreateBox(2, 2, 2, 0);
    Mesh box;
    box.SetIndicesCPU(box_data.GetIndices16());
    box.SetVerticesCPU(box_data.Vertices);
    box.UploadDataToGPU(device, cmdList);
    m_meshes["box"] = std::move(box);
}

void MeshManager::ReleaseUploadBuffer()
{
    for(auto& iter = m_meshes.begin(); iter != m_meshes.end(); iter++ )
    {
        iter->second.ReleaseUploadBuffer();
    }
}
