//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj; 
};

// cbuffer cbPass
// {
//     float4x4 gView;
//     float4x4 gInvView;
//     float4x4 gProj;
//     float4x4 gInvProj;
//     float4x4 gViewProj;
//     float4x4 gInvViewProj;
//     float4x4 gPrevViewProj;
//     float3 gEyePosW;
//     float cbPassPad1;
//     float2 gRenderTargetSize;
//     float2 gInvRenderTargetSize;
//     float gNearZ;
//     float gFarZ;
//     float gTotalTime;
//     float gDeltaTime;

//     // Allow application to change fog parameters once per frame.
//     // For example, we may only use fog for certain times of day.
//     float4 gFogColor;
//     float gFogStart;
//     float gFogRange;
//     float2 cbPassPad2;
// };

// cbuffer LightCommonData
// {
//     uint LightCount;
// };


struct VertexIn
{
    float3 PosL : POSITION;
    float3 Normal : NORMAL;
    float3 TangentU : TANGENT;
    float2 TexCoord : TEXCOORD;
};

Texture2D gDiffuseMap : register(t0);
SamplerState gsamLinear : register(s0);

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

    vout.TexCoord = vin.TexCoord;
    
    return vout;
}



float4 PS(VertexOut pin) : SV_Target
{
    float4 albedo = gDiffuseMap.Sample(gsamLinear, pin.TexCoord);
    // return pin.Color * albedo;
    return albedo;
}


