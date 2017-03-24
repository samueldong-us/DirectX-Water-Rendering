cbuffer ConstantBufferPerObject
{
	float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
}

struct VertexInput
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float4 Color : COLOR;
};

RasterizerState DisableCulling
{
	CullMode = NONE;
};

VertexOutput VertexShaderMain(VertexInput input)
{
	VertexOutput output;
	output.Position = mul(input.Position, WorldViewProjection);
	output.Color = input.Color;
	return output;
}

float4 PixelShaderMain(VertexOutput input) : SV_Target
{
	return input.Color;
}

technique11 main11
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderMain()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderMain()));
		SetRasterizerState(DisableCulling);
	}
}