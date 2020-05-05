cbuffer LightCBuf
{
    float3 lightPos;
};

static const float3 matColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLinear = 0.045f;
static const float attQuad = 0.0075f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_TARGET
{
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    const float attenuation = 1.0f / (attConst + attLinear * distToL + attQuad * pow(distToL, 2));
    const float3 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(dirToL, n));
    
    return float4(saturate(diffuse + ambient), 1.0f);
}