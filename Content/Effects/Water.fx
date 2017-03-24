cbuffer ConstantBufferPerFrame
{
	float Resolution;
	float3 CameraPosition;
}

cbuffer ConstantBufferPerObject
{
	float4x4 WorldViewProjection;
}

struct VertexInput
{
	float4 Position : POSITION;
	float2 FactorRange : TEXCOORD;
};

struct VertexOutput
{
	float4 Position : POSITION;
	float2 FactorRange : TEXCOORD;
};

struct HullConstantOutput
{
	float EdgeFactors[4] : SV_TessFactor;
	float InsideFactors[2] : SV_InsideTessFactor;
};

struct HullOutput
{
	float4 Position : POSITION;
	float2 FactorRange : TEXCOORD;
};

struct DomainOutput
{
	float4 Position : POSITION;
	float Parameter : TEXCOORD;
};

struct GeometryOutput
{
	float4 Position : SV_Position;
	float Parameter : TEXCOORD;
};

VertexOutput VertexMain(VertexInput input)
{
	VertexOutput output;
	output.Position = input.Position;
	output.FactorRange = input.FactorRange;
	return output;
}

HullConstantOutput HullConstantMain(InputPatch<VertexOutput, 4> patch, uint patchID : SV_PrimitiveID)
{
	HullConstantOutput output;
	float factor = patch[0].FactorRange.x;
	output.EdgeFactors[0] = Resolution / factor;
	output.EdgeFactors[1] = Resolution / factor;
	output.EdgeFactors[2] = Resolution / factor;
	output.EdgeFactors[3] = Resolution / factor;
	output.InsideFactors[0] = Resolution / factor;
	output.InsideFactors[1] = Resolution / factor;
	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("HullConstantMain")]
HullOutput HullMain(InputPatch<VertexOutput, 4> patch, uint controlPointID : SV_OutputControlPointID)
{
	HullOutput output;
	output.Position = patch[controlPointID].Position;
	output.FactorRange = patch[controlPointID].FactorRange;
	return output;
}

#define MorphPercent 0.2f

[domain("quad")]
DomainOutput DomainMain(HullConstantOutput input, float2 uv : SV_DomainLocation, const OutputPatch<HullOutput, 4> patch)
{
	DomainOutput output;
	float4 v0 = lerp(patch[0].Position, patch[1].Position, uv.x);
	float4 v1 = lerp(patch[3].Position, patch[2].Position, uv.x);
	float3 position = lerp(v0, v1, uv.y).xyz;
	float distanceFromCamera = distance(position, CameraPosition);
	float factor = patch[0].FactorRange.x;
	float range = patch[0].FactorRange.y;
	float dimension = Resolution / factor;
	float parameter = clamp((distanceFromCamera - (1.0f - MorphPercent * 0.5f) * range) / (MorphPercent * 0.5f * range), 0.0f, 1.0f);
	float2 morphedUV = frac(uv * dimension * 0.5) * 2.0 / dimension * parameter + uv;
	v0 = lerp(patch[0].Position, patch[1].Position, morphedUV.x);
	v1 = lerp(patch[3].Position, patch[2].Position, morphedUV.x);
	position = lerp(v0, v1, morphedUV.y).xyz;
	output.Position = float4(position, 1.0f);
	output.Parameter = parameter;
	return output;
}

[maxvertexcount(3)]
void GeometryMain(triangle DomainOutput input[3], inout LineStream<GeometryOutput> lineStream)
{
	GeometryOutput output;
	for (int i = 0; i < 3; i++)
	{
		output.Position = mul(input[i].Position, WorldViewProjection);
		output.Parameter = input[i].Parameter;
		lineStream.Append(output);
	}
}

float4 PixelMain(GeometryOutput input) : SV_Target
{
	return float4(0.0f, 1.0f, input.Parameter, 1.0f);
}

technique11 main11
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexMain()));
		SetHullShader(CompileShader(hs_5_0, HullMain()));
		SetDomainShader(CompileShader(ds_5_0, DomainMain()));
		SetGeometryShader(CompileShader(gs_5_0, GeometryMain()));
		SetPixelShader(CompileShader(ps_5_0, PixelMain()));
	}
}