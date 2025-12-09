#pragma once
#include "xdk/D3D9.h"
#include "xdk/XAPILIB.h"
#include "xdk/win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _D3DXINCLUDE_TYPE {
    D3DXINC_LOCAL = 0x0000,
    D3DXINC_SYSTEM = 0x0001,
    D3DXINC_FORCE_DWORD = 0x7fffffff,
} D3DXINCLUDE_TYPE;

// https://learn.microsoft.com/en-us/windows/win32/direct3d9/id3dxinclude
struct ID3DXInclude {
    virtual HRESULT Open(
        D3DXINCLUDE_TYPE IncludeType,
        LPCSTR pFileName,
        LPCVOID pParentData,
        LPCVOID *ppData,
        UINT *pBytes,
        LPSTR pFullPath,
        DWORD cbFullPath
    );
    virtual HRESULT Close(LPCVOID pData);

    //   public: ID3DXInclude(const ID3DXInclude&);
    //   public: ID3DXInclude();
    //   public: ID3DXInclude& operator=(const ID3DXInclude&);
};

#ifdef __cplusplus
}
#endif
