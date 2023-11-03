#include "ModelGameObject.h"

void ModelGameObject::Draw(CameraGameObject* camera, ID3D12GraphicsCommandList *cmd_list, DescriptorCacheGPU *descriptor_cache)
{
    // set mvp matrix
    // this matrix class is designed for postmultiplying : pos * view, thus it should pass a transposed ViewMatrix to GPU
    Matrix worldmatrix = GetGameObjectTransform().GetTransformMatrixLH();
    Matrix viewmatrix = camera->GetViewMatrix();
    Matrix projmatrix = camera->GetProjMatrix();
    m_material->SetParameter("gWorldViewProj", (worldmatrix * viewmatrix * projmatrix).Transpose());

    // issue draw cmd
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
