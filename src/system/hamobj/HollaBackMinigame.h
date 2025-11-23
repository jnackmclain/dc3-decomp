#pragma once
#include "obj/Object.h"
#include "rndobj/Poll.h"
#include "utl/MemMgr.h"

class HollaBackMinigame : public RndPollable {
public:
    // Hmx::Object
    virtual ~HollaBackMinigame();
    OBJ_CLASSNAME(HollaBackMinigame);
    OBJ_SET_TYPE(HollaBackMinigame);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    // RndPollable
    virtual void Poll();
    virtual void Enter() { RndPollable::Enter(); }
    virtual void Exit() { RndPollable::Exit(); }

    OBJ_MEM_OVERLOAD(0x21)
    NEW_OBJ(HollaBackMinigame)

protected:
    HollaBackMinigame();
};
