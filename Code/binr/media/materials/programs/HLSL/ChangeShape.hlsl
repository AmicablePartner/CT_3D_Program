
void Main_VP(float4 position : POSITION,
	float3 normal : NORMAL,
	float2 Uv : TEXCOORD0,
	out float2 oUv : TEXCOORD0,
	out float4 oPosition : POSITION,
	out float3 oVertexNormal : TEXCOORD1,
	out float4 oWorldPosition : TEXCOORD2,
	uniform float4x4 worldViewProj,
	uniform float4x4 WorldMatrix,
	uniform float4 CenterPos,
	uniform float4 ForcePower)
{
	oUv = Uv;
	oWorldPosition = mul(WorldMatrix, position);
	oVertexNormal = normal;

	float4 newPos = oWorldPosition;
	float Power_x = ForcePower.x;
	float Power_y = ForcePower.y;
	float Power_z = ForcePower.z;
	//计算垂直方向的力量的影响范围
	double base_Range = Power_z * 0.2;

	//计算当前位置与中心点的距离
	double CurDis = distance(newPos, CenterPos);

	if (CurDis < base_Range)
	{
		newPos.y = -CurDis * 0.01;
		oWorldPosition.w = 999.9;
	}
	oPosition = newPos;
}

float4 Main_FP(float2 Uv : TEXCOORD0,
	float3 oVertexNormal : TEXCOORD1,
	float4 oWorldPosition : TEXCOORD2) : COLOR
{
	float4 oColor = float4(0, 1, 0, 1);
	if (oWorldPosition.w == 999.9)
	{
		oColor = float4(1, 0, 0, 1);
	}
	return oColor;
}