#include"PointShaderHeader.hlsli"
static const uint vertNum = 4;

static const float4 ofset_array[vertNum] = 
{
	float4(-1.0f , - 1.0f,0,0),
	float4(-1.0f , + 1.0f,0,0),
	float4(+1.0f , - 1.0f,0,0),
	float4(+1.0f , + 1.0f,0,0),
};

static const float2 uvs[4] =
{
	float2(0.0,0.0),
	float2(0.0,1.0),
	float2(1.0,0.0),
	float2(1.0,1.0),

};

[maxvertexcount(vertNum)]
void GSmain(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	for (uint i = 0; i < vertNum; i++)
	{
		float4 offset = ofset_array[i];
		offset.x *= input[0].scale.x;
		offset.y *= input[0].scale.y;
		offset = mul(billboardMat, offset);
		element.svpos = input[0].svpos + offset;
		element.svpos = mul(mat, element.svpos);
		element.uv = uvs[i];
		element.scale = input[0].scale;
		element.color = input[0].color;

		output.Append(element);
	}
}