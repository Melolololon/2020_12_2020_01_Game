#include"ObjShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 PSmain(GSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv);
	float4 shaderColor;


	const float shininess = 4.0f;//����x

	float3 lightDir = normalize(light);

	//�J�������W�����ƒx��ĂȂ�?
	//�Ȃ����ˑR����ɑ����悤�ɂȂ���(cameraPosY��X�ɓ����Ă����ǂȂ����s��)
	//�Ȃ����[�ɍs���Ɩ��邳���ς�邵�����Е�����
	//eyeDir��float3����Ȃ���float�ɂȂ��Ă�
	//GPU�ɂ�float4�^��������Ȃ��̂�float3�ɂ��Ă�����
	const float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
	//const float eyeDir = normalize(float3(0,11,-6) - input.worldPos.xyz);


	float3 dotlightnormal = dot(-lightDir, input.normal);
	float3 reflect = normalize(lightDir + 2 * dotlightnormal * input.normal);

	float3 ambient = m_ambient;
	float3 diffuse = dotlightnormal * m_diffuse;

	//��������������?
	float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;


	shaderColor.rgb = (ambient + diffuse + specular) * lightColor;
	shaderColor.a = m_alpha;

	return shaderColor * texColor;
	//return float4(lightColor.x, lightColor.y, lightColor.z, 1);

	//return float4(s,0,0, 1);

	//return float4(input.worldPos.x, input.worldPos.y, input.worldPos.z, 1);
	//return float4(cameraPos.x, cameraPos.y, cameraPos.z, 1);

}