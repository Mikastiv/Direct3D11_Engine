Texture2D tex;

SamplerState samplr;

float4 main(float2 tc : TexCoord) : SV_Target
{
    return tex.Sample(samplr, tc);
}