cbuffer LightCBuf
{
    float3 lightPos;
    float3 matColor;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLinear;
    float attQuad;
};

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_TARGET
{
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    const float attenuation = 1.0f / (attConst + attLinear * distToL + attQuad * pow(distToL, 2));
    const float3 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(dirToL, n));
    
    return float4(saturate(diffuse + ambient) * matColor, 1.0f);
}