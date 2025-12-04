#pragma once
#include "rndobj/CubeTex.h"
#include "xdk/d3d9i/resource.h"

class DxCubeTex : public RndCubeTex {
public:
    DxCubeTex();
    virtual ~DxCubeTex();
    OBJ_CLASSNAME(CubeTex);
    OBJ_SET_TYPE(CubeTex);
    virtual void Select(int);

private:
    virtual void Reset();
    virtual void Sync();

    D3DResource *mResource; // 0x1ac
};
