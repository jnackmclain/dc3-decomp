#pragma once
#include "xdk/XAPILIB.h"
#include "xdk/D3D9.h"
// This is where Xbox related functions that use D3D9 stuff will go.

#ifdef __cplusplus
extern "C" {
#endif

UINT XGSurfaceSize(UINT, UINT, D3DFORMAT, D3DMULTISAMPLE_TYPE);
UINT XGHierarchicalZSize(UINT, UINT, D3DMULTISAMPLE_TYPE);

#ifdef __cplusplus
}
#endif
