#pragma once
#include "hamobj/SongUtl.h"
#include "obj/Object.h"
#include "utl/MemMgr.h"

class CrazeHollaback : public Hmx::Object {
public:
    // Hmx::Object
    virtual ~CrazeHollaback() {}
    OBJ_CLASSNAME(CrazeHollaback);
    OBJ_SET_TYPE(CrazeHollaback);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);

    OBJ_MEM_OVERLOAD(0x10)
    NEW_OBJ(CrazeHollaback)

protected:
    CrazeHollaback();

    Range mMusicRange; // 0x2c
    Range mPlayRange; // 0x34
};
