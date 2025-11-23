#pragma once
#include "obj/Object.h"
#include "rndobj/Poll.h"
#include "utl/MemMgr.h"

class RhythmBattlePlayer : public RndPollable {
public:
    // Hmx::Object
    virtual ~RhythmBattlePlayer();
    OBJ_CLASSNAME(RhythmBattlePlayer);
    OBJ_SET_TYPE(RhythmBattlePlayer);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    // RndPollable
    virtual void Poll();
    virtual void Enter();

    OBJ_MEM_OVERLOAD(0x19)
    NEW_OBJ(RhythmBattlePlayer)

protected:
    RhythmBattlePlayer();
};
