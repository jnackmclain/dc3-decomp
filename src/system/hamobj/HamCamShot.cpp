#include "hamobj/HamCamShot.h"
#include "obj/Object.h"
#include "world/CameraShot.h"

HamCamShot *gHamCamShot;
std::list<HamCamShot::TargetCache> HamCamShot::sCache;

HamCamShot::HamCamShot()
    : mTargets(this), unk290(0), unk294(0), unk298(0), unk29c(0), unk2a0(this), unk2b4(0),
      unk2b8(this), unk2cc(0), unk2d0(0), unk2d4(0), unk2d8(0), unk2dc(0), unk2dd(0),
      unk2e0(this), unk2f4(0), unk2f8(this), unk30c(this), unk320(this), unk340(this),
      unk354(this), unk368(this), unk388(false) {
    mNearPlane = 10;
    mFarPlane = 10000;
    unk2b4 = 0;
}

#define SYNC_PROP_SET_TARGET_BIT(s, member)                                              \
    {                                                                                    \
        _NEW_STATIC_SYMBOL(s)                                                            \
        if (sym == _s) {                                                                 \
            if (_op == kPropSet) {                                                       \
                member = _val.Int();                                                     \
            } else {                                                                     \
                _val = member;                                                           \
            }                                                                            \
            return true;                                                                 \
        }                                                                                \
    }

BEGIN_CUSTOM_PROPSYNC(HamCamShot::Target)
    SYNC_PROP_SET(target, o.mTarget, o.UpdateTarget(_val.Sym(), gHamCamShot))
    SYNC_PROP(to, o.mTo)
    SYNC_PROP_MODIFY(anim_group, o.mAnimGroup, gHamCamShot->StartAnim())
    SYNC_PROP(fast_forward, o.mFastForward)
    SYNC_PROP(forward_event, o.mForwardEvent)
    SYNC_PROP_SET_TARGET_BIT(force_lod, o.mForceLOD)
    SYNC_PROP_SET_TARGET_BIT(teleport, o.mTeleport)
    SYNC_PROP_SET_TARGET_BIT(return, o.mReturn)
    SYNC_PROP_SET_TARGET_BIT(self_shadow, o.mSelfShadow)
    SYNC_PROP(env_override, o.mEnvOverride)
    SYNC_PROP_SET(target_ptr, gHamCamShot->FindTarget(o.mTarget), )
END_CUSTOM_PROPSYNC

BEGIN_PROPSYNCS(HamCamShot)
    gHamCamShot = this;
    SYNC_PROP(targets, mTargets)
    SYNC_PROP_SET(player_flag, unk29c, unk29c = _val.Int())
    SYNC_PROP(zero_time, unk298)
    SYNC_PROP(min_time, unk290)
    SYNC_PROP(max_time, unk294)
    SYNC_PROP_MODIFY(next_shots, unk2a0, CheckNextShots(); ResetNextShot();)
    SYNC_PROP(master_anims, unk2e0)
    SYNC_SUPERCLASS(CamShot)
END_PROPSYNCS

void HamCamShot::Target::Store(HamCamShot *shot) {
    if (!mTarget.Null()) {
        std::list<TargetCache>::iterator it = shot->CreateTargetCache(mTarget);
        if (it->unk4) {
            mTo = it->unk4->LocalXfm();
        }
        HamCamShot::sCache.erase(it);
    }
}

void HamCamShot::Target::UpdateTarget(Symbol s, HamCamShot *shot) {
    if (mTarget != s) {
        mTarget = s;
        mAnimGroup = "";
    }
    Store(shot);
}

std::list<HamCamShot::TargetCache>::iterator HamCamShot::CreateTargetCache(Symbol s) {
    TargetCache cache;
    sCache.push_back(cache);
    cache.unksym = s;
    cache.unk4 = FindTarget(s);
    return sCache.begin();
}
