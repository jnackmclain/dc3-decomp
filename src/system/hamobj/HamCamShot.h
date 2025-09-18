#pragma once
#include "char/Character.h"
#include "math/Mtx.h"
#include "obj/Object.h"
#include "rndobj/Anim.h"
#include "rndobj/Draw.h"
#include "rndobj/Env.h"
#include "rndobj/Trans.h"
#include "world/CameraShot.h"

/** "Hammer specific camera shot" */
class HamCamShot : public CamShot {
public:
    struct Target {
        Target(Hmx::Object *owner)
            : mFastForward(0), mEnvOverride(owner), mForceLOD(kLODPerFrame),
              mTeleport(false), mReturn(true), mSelfShadow(false), unk68p4(true),
              unk68p3(true) {
            mTo.Reset();
        }
        void UpdateTarget(Symbol, HamCamShot *);
        void Store(HamCamShot *);

        /** "Symbolic name of target" */
        Symbol mTarget; // 0x0
        /** "the transform to teleport the character to" */
        Transform mTo; // 0x4
        /** "Name of CharClipGroup to play on character" */
        Symbol mAnimGroup; // 0x44
        /** "Fast forward chosen animation by this time, in camera units" */
        float mFastForward; // 0x48
        /** "Event to fastforward relative to" */
        Symbol mForwardEvent; // 0x4c
        /** "environment override for this target during this shot" */
        ObjPtr<RndEnviron> mEnvOverride; // 0x50
        /** "Forces LOD, kLODPerFrame is normal behavior of picking per frame,
            the others force the lod (0 is highest res lod, 2 is lowest res lod)" */
        int mForceLOD : 3; // 0x64
        // 0x68
        /** "do we teleport this character?" */
        bool mTeleport : 1;
        /** "return to original position after shot?" */
        bool mReturn : 1;
        /** "should character cast a self shadow" */
        bool mSelfShadow : 1;
        bool unk68p4 : 1;
        bool unk68p3 : 1;
    };

    // size 0x4c
    struct TargetCache {
        TargetCache() : unk0(0), unk4(0) { unkxfm.Reset(); }

        Symbol unksym; // 0x8
        int unk0; // 0x0
        RndTransformable *unk4; // 0x4
        Transform unkxfm; // 0xc
    };

    // Hmx::Object
    OBJ_CLASSNAME(HamCamShot);
    OBJ_SET_TYPE(HamCamShot);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    // RndAnimatable
    virtual void StartAnim();
    virtual void EndAnim();
    virtual void SetFrame(float frame, float blend);
    virtual float EndFrame();
    virtual void ListAnimChildren(std::list<RndAnimatable *> &) const;
    // CamShot
    virtual void SetPreFrame(float, float);
    virtual CamShot *CurrentShot();
    virtual bool CheckShotStarted();
    virtual bool CheckShotOver(float);

    RndTransformable *FindTarget(Symbol);

protected:
    HamCamShot();

    virtual void SetFrameEx(float, float);

    void CheckNextShots();
    void ResetNextShot();
    std::list<TargetCache>::iterator CreateTargetCache(Symbol);

    static std::list<TargetCache> sCache;

    ObjList<Target> mTargets; // 0x284
    int unk290; // 0x290
    int unk294; // 0x294
    float unk298; // 0x298
    int unk29c; // 0x29c
    ObjPtrList<HamCamShot> unk2a0; // 0x2a0
    int unk2b4; // 0x2b4
    ObjPtr<HamCamShot> unk2b8; // 0x2b8
    float unk2cc; // 0x2cc
    float unk2d0; // 0x2d0
    bool unk2d4; // 0x2d4
    float unk2d8; // 0x2d8
    bool unk2dc; // 0x2dc
    bool unk2dd; // 0x2dd
    ObjPtrList<RndAnimatable> unk2e0; // 0x2e0
    int unk2f4; // 0x2f4
    ObjPtrList<RndDrawable> unk2f8; // 0x2f8
    ObjPtrList<RndDrawable> unk30c; // 0x30c
    ObjPtrList<RndDrawable> unk320; // 0x320
    std::vector<RndDrawable *> unk334; // 0x334
    ObjPtrList<RndDrawable> unk340; // 0x340
    ObjPtrList<RndDrawable> unk354; // 0x354
    ObjPtrList<RndDrawable> unk368; // 0x368
    std::vector<RndDrawable *> unk37c; // 0x37c
    bool unk388; // 0x388
};
