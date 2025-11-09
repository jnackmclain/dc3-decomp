#pragma once
#include "utl/NetCacheMgr.h"

class NetCacheMgrXbox : public NetCacheMgr {
public:
    NetCacheMgrXbox();
    virtual ~NetCacheMgrXbox();
    virtual DataNode Handle(DataArray *, bool);
    virtual void Poll();

    unsigned int GetIP();

protected:
    virtual void LoadInit();
    virtual bool IsDoneLoading() const;
    virtual void UnloadInit();
    virtual bool IsDoneUnloading() const;

    bool mDoneLoading; // 0x68
    int unk6c;
    // XLSPConnection at 0x70
};
