cbuffer c_buf
{
    float4 colors[8];
};

float4 main(uint tid : SV_PrimitiveID) : SV_TARGET
{
    return colors[tid / 2 % 8];
}