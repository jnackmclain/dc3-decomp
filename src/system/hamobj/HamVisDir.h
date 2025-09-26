#pragma once
#include "hamobj/Pose.h"
#include "gesture/SkeletonDir.h"
#include "gesture/FreestyleMotionFilter.h"
#include "math/Mtx.h"
#include "obj/Object.h"
#include "rndobj/Anim.h"
#include "utl/MemMgr.h"

struct PoseOwner {
    PoseOwner();
    ~PoseOwner();

    Pose *pose; // 0x0
    Pose *holder; // 0x4
    bool unk8; // 0x8
    Symbol unkc; // 0xc
};

class HamVisDir : public SkeletonDir {
public:
    // Hmx::Object
    virtual ~HamVisDir();
    OBJ_CLASSNAME(HamVisDir);
    OBJ_SET_TYPE(HamVisDir);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    // RndPollable
    virtual void Enter();
    // SkeletonCallback
    virtual void PostUpdate(const struct SkeletonUpdateData *);

    OBJ_MEM_OVERLOAD(0x1D)
    NEW_OBJ(HamVisDir)

protected:
    HamVisDir();

    Transform unk284; // 0x284
    FreestyleMotionFilter *unk2c4; // 0x2c4
    bool unk2c8; // 0x2c8
    std::vector<unsigned int> unk2cc; // 0x2cc
    int unk2d8; // 0x2d8
    int unk2dc; // 0x2dc
    ObjPtr<RndAnimatable> unk2e0; // 0x2e0
    ObjPtr<RndAnimatable> unk2f4; // 0x2f4
    ObjPtr<RndAnimatable> unk308; // 0x308
    ObjPtr<RndAnimatable> unk31c; // 0x31c
    bool unk330; // 0x330
    float unk334; // 0x334
    PoseOwner unk338[2]; // 0x338
    PoseOwner unk358[2]; // 0x358
};
