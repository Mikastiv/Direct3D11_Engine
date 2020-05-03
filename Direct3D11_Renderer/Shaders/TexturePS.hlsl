Texture2D tex;

SamplerState samplr;

float4 main(float2 tc : UV) : SV_Target
{
    return tex.Sample(samplr, tc);
}