struct GerstnerWave
{
	float2 Direction;
	float Frequency;
	float Amplitude;
	float PhaseSpeed;
	float3 _Padding;
};

#define NumberOfWaves 32

cbuffer ConstantBufferPerFrame
{
	float Resolution;
	float3 CameraPosition;
	GerstnerWave WaveParameter[NumberOfWaves];
	float Steepness;
	float Time;
}

cbuffer ConstantBufferPerObject
{
	float4x4 WorldViewProjection;
}

TextureCube EnvironmentMap;

SamplerState AnisotropicSampler
{
	Filter = ANISOTROPIC;
};

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
	float3 Normal : NORMAL;
	float Parameter : TEXCOORD;
};

struct GeometryOutput
{
	float4 Position : SV_Position;
	float3 WorldPosition : POSITION;
	float3 Normal : NORMAL;
	float Parameter : TEXCOORD;
};

RasterizerState DisableCulling
{
	CullMode = NONE;
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

float3 CalculateWaveOffset(float2 position, GerstnerWave wave)
{
	float2 direction = normalize(wave.Direction);
	float steepness = Steepness / (wave.Frequency * wave.Amplitude * NumberOfWaves);
	float phase = dot(wave.Frequency * direction, position.xy) + wave.PhaseSpeed * Time;
	float deltaX = steepness * wave.Amplitude * direction.x * cos(phase);
	float deltaY = steepness * wave.Amplitude * direction.y * cos(phase);
	float deltaZ = wave.Amplitude * sin(phase);
	return float3(deltaX, deltaY, deltaZ);
}

float3 CalculateNormal(float2 position, GerstnerWave wave)
{
	float2 direction = normalize(wave.Direction);
	float steepness = Steepness / (wave.Frequency * wave.Amplitude * NumberOfWaves);
	float phase = dot(wave.Frequency * direction, position.xy) + wave.PhaseSpeed * Time;
	float normalX = -direction.x * wave.Frequency * wave.Amplitude * cos(phase);
	float normalY = -direction.y * wave.Frequency * wave.Amplitude * cos(phase);
	float normalZ = -steepness * wave.Frequency * wave.Amplitude * sin(phase);
	return float3(normalX, normalY, normalZ);
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
	float3 normal;
	for (int i = 0; i < NumberOfWaves; i++)
	{
		position += CalculateWaveOffset(position.xy, WaveParameter[i]);
		normal += CalculateNormal(position.xy, WaveParameter[i]);
	}
	normal.z = 1.0f - normal.z;
	output.Position = float4(position, 1.0f);
	output.Normal = normalize(normal);
	output.Parameter = parameter;
	return output;
}

[maxvertexcount(3)]
void WireFrameMain(triangle DomainOutput input[3], inout LineStream<GeometryOutput> lineStream)
{
	GeometryOutput output;
	for (int i = 0; i < 3; i++)
	{
		output.Position = mul(input[i].Position, WorldViewProjection);
		output.WorldPosition = input[i].Position;
		output.Normal = input[i].Normal;
		output.Parameter = input[i].Parameter;
		lineStream.Append(output);
	}
}

[maxvertexcount(3)]
void NormalMain(triangle DomainOutput input[3], inout TriangleStream<GeometryOutput> triangeStream)
{
	GeometryOutput output;
	for (int i = 0; i < 3; i++)
	{
		output.Position = mul(input[i].Position, WorldViewProjection);
		output.WorldPosition = input[i].Position;
		output.Normal = input[i].Normal;
		output.Parameter = input[i].Parameter;
		triangeStream.Append(output);
	}
}

float4 WireFramePixelMain(GeometryOutput input) : SV_Target
{
	return float4(input.Parameter, input.Parameter, input.Parameter, 1.0f);
}

#define R0 0.02f
#define WaterColor float3(0.04f, 0.28f, 0.46f)
float4 PixelMain(GeometryOutput input) : SV_Target
{
	float3 viewDirection = normalize(CameraPosition - input.WorldPosition);
	float dotProduct = dot(input.Normal, viewDirection);
	float reflectionAmount = R0 + (1.0f - R0) * pow(1.0f - dotProduct, 5.0f);
	float3 reflectionDirection = (-viewDirection) + 2.0f * dotProduct * input.Normal;
	float3 transformedReflectionDirection = float3(reflectionDirection.x, reflectionDirection.z, reflectionDirection.y);
	float3 environment = EnvironmentMap.Sample(AnisotropicSampler, transformedReflectionDirection).rgb;
	float3 color = lerp(WaterColor, environment, reflectionAmount);
	return float4(color, 1.0f);
}

technique11 main11
{
	pass Normal
	{
		SetVertexShader(CompileShader(vs_5_0, VertexMain()));
		SetHullShader(CompileShader(hs_5_0, HullMain()));
		SetDomainShader(CompileShader(ds_5_0, DomainMain()));
		SetGeometryShader(CompileShader(gs_5_0, NormalMain()));
		SetPixelShader(CompileShader(ps_5_0, PixelMain()));
		SetRasterizerState(DisableCulling);
	}
	pass WireFrame
	{
		SetVertexShader(CompileShader(vs_5_0, VertexMain()));
		SetHullShader(CompileShader(hs_5_0, HullMain()));
		SetDomainShader(CompileShader(ds_5_0, DomainMain()));
		SetGeometryShader(CompileShader(gs_5_0, WireFrameMain()));
		SetPixelShader(CompileShader(ps_5_0, WireFramePixelMain()));
		SetRasterizerState(DisableCulling);
	}
}