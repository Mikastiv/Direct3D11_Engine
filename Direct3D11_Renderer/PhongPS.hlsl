cbuffer LightCBuf
{
    float3 lightPos;
    float ambient;
    float diffuseIntensity;
    float attConst;
    float attLinear;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float3 matColor;
    float specularIntensity;
    float specularPower;
};

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_TARGET
{
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    const float attenuation = 1.0f / (attConst + attLinear * distToL + attQuad * pow(distToL, 2));
    const float diffuse = diffuseIntensity * attenuation * max(0.0f, dot(dirToL, n));
    
    const float3 r = normalize(reflect(dirToL, n));
    const float specular = attenuation * specularIntensity * pow(max(0.0f, dot(r, normalize(worldPos))), specularPower);
    
    return float4(saturate(diffuse + ambient + specular) * matColor, 1.0f);
}