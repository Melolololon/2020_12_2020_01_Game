#include"PointShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float2 scale : TEXCOORD, float4 color : COLOR)
{
	VSOutput output;
	output.svpos = pos;//�s��ƍ��W����Z
	output.scale = scale;
	output.color = color;

	return output;
}