cbuffer c_buf1
{
    float4 colors[6];
};

float4 main(uint tid : SV_PrimitiveID) : SV_TARGET
{
    return colors[tid / 2];
}