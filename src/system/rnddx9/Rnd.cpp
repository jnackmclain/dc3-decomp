#include "rnddx9/Rnd.h"
#include "obj/Object.h"
#include "os/Debug.h"
#include "rndobj/Bitmap.h"
#include "xdk/D3D9.h"

DxRnd TheDxRnd;

BEGIN_HANDLERS(DxRnd)
    HANDLE_ACTION(suspend, Suspend())
    HANDLE_SUPERCLASS(Rnd)
END_HANDLERS

bool DxRnd::Offscreen() const {
    D3DSurface *back = BackBuffer();
    D3DSurface *target = D3DDevice_GetRenderTarget(mD3DDevice, 0);
    bool ret = back != target;
    if (target) {
        D3DResource_Release(target);
    }
    if (back) {
        D3DResource_Release(back);
    }
    return ret;
}

void DxRnd::PreDeviceReset() {
    if (mOcclusionQueryMgr) {
        mOcclusionQueryMgr->ReleaseQueries();
    }
    FOREACH (it, unk2b0) {
        (*it)->PreDeviceReset();
    }
    ReleaseAutoRelease();
}

void DxRnd::PostDeviceReset() {
    FOREACH (it, unk2b0) {
        (*it)->PostDeviceReset();
    }
    MakeDrawTarget();
    InitRenderState();
}

D3DFORMAT DxRnd::D3DFormatForBitmap(const RndBitmap &bitmap) {
    int fmt = bitmap.Order() & 0x38;
    int bpp = bitmap.Bpp();
    if (fmt != 0) {
        switch (fmt) {
        case 8:
            return D3DFMT_DXT1;
        case 0x10:
            return D3DFMT_DXT3;
        case 0x18:
            return D3DFMT_DXT5;
        case 0x20:
            return D3DFMT_DXN;
        default:
            MILO_FAIL("Invalid dxt format: %d", fmt);
            MILO_ASSERT(fmt != D3DFMT_UNKNOWN, 999);
            return (D3DFORMAT)0xffffffff;
        }
    } else {
        switch (bpp) {
        case 4:
        case 8:
            return D3DFMT_A8R8G8B8;
        case 0x10:
            return D3DFMT_A1R5G5B5;
        case 0x18:
            return D3DFMT_X8R8G8B8;
        case 0x20:
            return D3DFMT_A8R8G8B8;
        default:
            MILO_FAIL("Invalid bpp: %d", bpp);
            MILO_ASSERT(fmt != D3DFMT_UNKNOWN, 999);
            return (D3DFORMAT)0xffffffff;
        }
    }
}

int DxRnd::BitmapOrderForD3DFormat(D3DFORMAT fmt) {
    switch (fmt) {
    case D3DFMT_DXT1:
    case D3DFMT_LIN_DXT1:
        return 8;
    case D3DFMT_DXT3:
    case D3DFMT_LIN_DXT3:
        return 0x10;
    case D3DFMT_DXT5:
    case D3DFMT_LIN_DXT5:
        return 0x18;
    case D3DFMT_DXN:
    case D3DFMT_LIN_DXN:
        return 0x20;
    default:
        return 0;
    }
}
