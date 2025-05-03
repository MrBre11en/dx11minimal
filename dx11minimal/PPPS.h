float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(input.pos.xyz, 1.0);
    //return (input.vnorm/2+.5);
}