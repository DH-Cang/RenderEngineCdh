//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj; 
};

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


