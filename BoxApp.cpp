//***************************************************************************************
// BoxApp.cpp by Frank Luna (C) 2015 All Rights Reserved.
//
// Shows how to draw a box in Direct3D 12.
//
// Controls:
//   Hold the left mouse button down and move the mouse to rotate.
//   Hold the right mouse button down and move the mouse to zoom in and out.
//***************************************************************************************
#include "BoxApp.h"

BoxApp::BoxApp(HINSTANCE hInstance)
: D3DApp(hInstance) 
{
}

BoxApp::~BoxApp()
{
}

bool BoxApp::Initialize()
{
    if(!D3DApp::Initialize())
		return false;
		
    // Reset the command list to prep for initialization commands.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));
 
    BuildDescriptorHeaps();
	BuildMaterials();
    BuildShadersAndInputLayout();
    BuildBoxGeometry();
    BuildPSO();
	LoadTexture();

    // Execute the initialization commands.
    ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until initialization is complete.
    FlushCommandQueue();

	m_texture_manager.ReleaseUploadBuffer();
    m_mesh_manager.ReleaseUploadBuffer();
    
    SetMaterial();
    SetGameObject();

	return true;
}

void BoxApp::OnResize()
{
	D3DApp::OnResize();

    // The window resized, so update the aspect ratio and recompute the projection matrix.
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

void BoxApp::Update(const GameTimer& gt)
{
    Rotator rotate;
    m_chest_go->SetGameObjectRotation(Math::RadiansToDegrees(mPhi), 0, Math::RadiansToDegrees(mTheta));
}

void BoxApp::Draw(const GameTimer& gt)
{
    // Reuse the memory associated with command recording.
    // We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// reset gpu cache descriptor
	m_descriptor_cache->ResetCachedHeap();

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
    // Reusing the command list reuses memory.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), m_PSO_manager.GetPSO("commonPSO")));

    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    // Clear the back buffer and depth buffer.
    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	
    // Specify the buffers we are going to render to.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_descriptor_cache->GetCachedCbvSrvUavDescriptorHeap() };
	mCommandList->SetDescriptorHeaps(1, descriptorHeaps);

    // Draw
    m_chest_go->Draw(m_camera.get(), mCommandList.Get(), m_descriptor_cache.get());
	
    // Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    // Done recording commands.
	ThrowIfFailed(mCommandList->Close());
 
    // Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	
	// swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	FlushCommandQueue();
}

void BoxApp::BuildDescriptorHeaps()
{
    m_descriptor_manager = std::make_unique<DescriptorManager>(md3dDevice.Get(), 64, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    m_descriptor_cache = std::make_unique<DescriptorCacheGPU>(md3dDevice.Get());
}

void BoxApp::BuildMaterials()
{
    m_material = std::make_unique<Material>();
}

void BoxApp::BuildShadersAndInputLayout()
{
    ShaderInfo info;
	info.b_create_VS = true;
	info.b_create_PS = true;
	info.file_name = std::string("../../../Shaders/color.hlsl");
	m_shader = std::make_unique<Shader>(info, md3dDevice.Get());
}

void BoxApp::BuildBoxGeometry()
{
	m_mesh_manager.LoadMeshFromFile(md3dDevice.Get(), mCommandList.Get());
}

void BoxApp::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    std::vector<D3D12_INPUT_ELEMENT_DESC> input_layout(Vertex::GetVSInputLayout());
    psoDesc.InputLayout = { input_layout.data(), (UINT)input_layout.size() };
    psoDesc.pRootSignature = m_shader->m_root_signature.Get();
    psoDesc.VS = 
	{ 
		reinterpret_cast<BYTE*>(m_shader->m_shader_stage["VS"]->GetBufferPointer()), 
		m_shader->m_shader_stage["VS"]->GetBufferSize() 
	};
    psoDesc.PS = 
	{ 
		reinterpret_cast<BYTE*>(m_shader->m_shader_stage["PS"]->GetBufferPointer()), 
		m_shader->m_shader_stage["PS"]->GetBufferSize() 
	};
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = mBackBufferFormat;
    psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    psoDesc.DSVFormat = mDepthStencilFormat;

	m_PSO_manager.CreatePSO("commonPSO", psoDesc, md3dDevice.Get());
}

void BoxApp::LoadTexture()
{
	m_texture_manager.LoadTextureFromFile(md3dDevice.Get(), mCommandList.Get());
	auto wood_tex = m_texture_manager.GetTexture("woodCrateTex");

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = wood_tex->Resource->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = wood_tex->Resource->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	wood_tex->m_srv = std::make_unique<ShaderResourceView>(srvDesc, wood_tex->Resource.Get(), md3dDevice.Get(), m_descriptor_manager.get());
}

void BoxApp::SetMaterial()
{   
    m_material->SetShader(m_shader.get());
    m_material->CreateCb(md3dDevice.Get());
    m_material->SetParameter("gDiffuseMap", m_texture_manager.GetTexture("woodCrateTex")->m_srv.get());
}

void BoxApp::SetGameObject()
{
    // world coord is Left hand coord
    // x right; y up; z inside the screen
    m_chest_go = std::make_unique<ModelGameObject>(std::string("chest"));
    m_chest_go->SetMaterial(m_material.get());
    m_chest_go->SetMesh(m_mesh_manager.GetMesh("box"));
    m_chest_go->SetGameObjectLocation(0, 0, 5);

    m_camera = std::make_unique<CameraGameObject>(std::string("camera"));
    m_camera->LookAt(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
    m_camera->SetLens(Math::DegreesToRadians(60), (float)mClientWidth/mClientHeight, 0.00001f, 10000.0f);
    m_camera->UpdateViewMatrix();
}

void BoxApp::OnMouseDown(WPARAM btnState, int x, int y)
{
    mLastMousePos.x = x;
    mLastMousePos.y = y;

    SetCapture(mhMainWnd);
}

void BoxApp::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void BoxApp::OnMouseMove(WPARAM btnState, int x, int y)
{
    if((btnState & MK_LBUTTON) != 0)
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
        float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

        // Update angles based on input to orbit camera around box.
        mTheta -= dx;
        mPhi -= dy;

        // Restrict the angle mPhi.
        // mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
    }
    else if((btnState & MK_RBUTTON) != 0)
    {
        // Make each pixel correspond to 0.005 unit in the scene.
        float dx = 0.005f*static_cast<float>(x - mLastMousePos.x);
        float dy = 0.005f*static_cast<float>(y - mLastMousePos.y);

        // Update the camera radius based on input.
        mRadius += dx - dy;

        // Restrict the radius.
        mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}
