#pragma once

#include "xdk/d3d9i/device.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "xdk/d3d9i/d3dtypes.h"

typedef struct D3DIndexBuffer D3DIndexBuffer;
typedef struct D3DVertexBuffer D3DVertexBuffer;

/// https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dindexbuffer-desc
typedef struct D3DINDEXBUFFER_DESC {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;
} D3DINDEXBUFFER_DESC, *LPD3DINDEXBUFFER_DESC;

/// https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dvertexbuffer-desc
typedef struct D3DVERTEXBUFFER_DESC {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;
    DWORD FVF;
} D3DVERTEXBUFFER_DESC, *LPD3DVERTEXBUFFER_DESC;

// source: the kinect joyride pdb
typedef struct D3DResource { /* Size=0x18 */
    /* 0x0000 */ UINT Common;
    /* 0x0004 */ UINT ReferenceCount;
    /* 0x0008 */ UINT Fence;
    /* 0x000c */ UINT ReadFence;
    /* 0x0010 */ UINT Identifier;
    /* 0x0014 */ UINT BaseFlush;
} D3DResource;

// D3DResource methods
D3DRESOURCETYPE D3DResource_GetType(D3DResource *self);
DWORD D3DResource_AddRef(D3DResource *self);
VOID D3DResource_GetDevice(D3DResource *self, D3DDevice **);
BOOL D3DResource_IsSet(D3DResource *self, D3DDevice *);
DWORD D3DResource_Release(D3DResource *self);

// both guesses cause there's no mangling and the args are different from ID3DDevice9
struct D3DVertexBuffer *
D3DDevice_CreateVertexBuffer(UINT Length, DWORD Usage, D3DPOOL Pool);

struct D3DIndexBuffer *
D3DDevice_CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool);

HRESULT D3DIndexBuffer_GetDesc(D3DIndexBuffer *, D3DINDEXBUFFER_DESC *pDesc);
HRESULT
D3DIndexBuffer_Lock(D3DIndexBuffer *, UINT OffsetToLock, UINT SizeToLock, DWORD Flags);
HRESULT D3DIndexBuffer_Unlock(D3DIndexBuffer *);

HRESULT D3DVertexBuffer_GetDesc(D3DVertexBuffer *, D3DVERTEXBUFFER_DESC *pDesc);
HRESULT
D3DVertexBuffer_Lock(D3DVertexBuffer *, UINT OffsetToLock, UINT SizeToLock, DWORD Flags);
HRESULT D3DVertexBuffer_Unlock(D3DVertexBuffer *);

HRESULT D3DDevice_SetStreamSource(
    D3DDevice *,
    UINT StreamNumber,
    struct D3DVertexBuffer *pStreamData,
    UINT OffsetInBytes,
    UINT Stride,
    uint unk_r8
);

#ifdef __cplusplus
}
#endif
