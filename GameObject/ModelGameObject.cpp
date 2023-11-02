#include "ModelGameObject.h"

void ModelGameObject::Draw(ID3D12GraphicsCommandList *cmd_list, DescriptorCacheGPU *descriptor_cache)
{
    cmd_list->SetGraphicsRootSignature(m_material->GetRootSignature());
    
    cmd_list->IASetVertexBuffers(0, 1, m_mesh->GetVertexBufferView());
    cmd_list->IASetIndexBuffer(m_mesh->GetIndexBufferView());
    cmd_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    // update shader data
    m_material->PassParametersToShader(cmd_list, descriptor_cache);

    cmd_list->DrawIndexedInstanced(
        m_mesh->GetIndicesCount(), 
        1, 0, 0, 0);
}
