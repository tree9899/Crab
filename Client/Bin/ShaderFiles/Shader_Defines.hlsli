


RasterizerState RS_Default
{
    FillMode = Solid;
    CullMode = BACK;
};

DepthStencilState DSS_Default
{
    DepthEnable = true;
    DepthWriteMask = all;
    DepthFunc = LESS_EQUAL;
};

RasterizerState RS_Sky
{
    FillMode = Solid;
    CullMode = FRONT;
};

DepthStencilState DSS_Sky
{
    DepthEnable = false;
    DepthWriteMask = zero;
    //DepthFunc = ;
};