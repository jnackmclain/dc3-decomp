#include "hamobj/HamDirector.h"
#include "hamobj/HamGameData.h"
#include "math/Rand.h"
#include "math/Utl.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "os/Debug.h"
#include "rndobj/Draw.h"
#include "utl/Symbol.h"

HamDirector *TheHamDirector;

HamDirector::HamDirector()
    : unk8c(this), unka0(this), unkb4(this), unkc8(0), unkcc(""), mBackupDrift(1),
      mMerger(this), mMoveMerger(this), mGameModeMerger(this), mCurWorld(this),
      unk124(this), unk140(0), unk14c(0), unk150(this), mCamPostProc(this),
      mForcePostProc(this), unk18c(this), mForcePostProcBlend(0),
      mForcePostProcBlendRate(1), unk1a8(this), unk1bc(this), unk1d0(0), unk1d4(0),
      unk1d8(this), unk1ec(this), mFreestyleEnabled(1), unk204(this), unk218(this),
      unk22c(this), unk240(this), unk254(0), mDisabled(0), unk25a(0), mCurShot(this),
      unk270(this), unk284(this), unk29c(-kHugeFloat), mDisablePicking(0), unk2a1(0),
      unk2a4(0), unk2a8(-kHugeFloat), unk2ac(1), mPlayerFreestyle(0),
      mPlayerFreestylePaused(0), unk2c0(this), mPracticeStart(0), mPracticeEnd(0),
      mStartLoopMargin(1), mEndLoopMargin(1), mBlendDebug(0), unk304(0), unk308(this),
      unk31c(this), mNoTransitions(0), mCollisionChecks(1), mLoadedNewSong(1), unk338(0),
      unk33c(RandomInt(0, 2)), unk33d(0), unk340(this), unk354(this), unk369(0),
      unk36c(0) {
    static DataNode &n = DataVariable("hamdirector");
    n = this;
    TheHamDirector = this;
    mDirCutKeys.reserve(100);
}

HamDirector::~HamDirector() {
    MILO_ASSERT(TheGameData, 0xC5);
    TheGameData->Clear();
    if (TheHamDirector == this) {
        static DataNode &n = DataVariable("hamdirector");
        n = NULL_OBJ;
        TheHamDirector = nullptr;
    }
}

BEGIN_PROPSYNCS(HamDirector)
    SYNC_PROP_SET(shot, mShot, SetShot(_val.Sym()))
    static Symbol none("none");
    SYNC_PROP_SET(postproc, NULL_OBJ, )
    SYNC_PROP_SET(world_event, none, SetWorldEvent(_val.Sym()))
    SYNC_PROP_SET(clip, ClosestMove(), )
    SYNC_PROP_SET(practice, Symbol(), )
    SYNC_PROP_SET(move, Symbol(), )
    SYNC_PROP_SET(move_instance, Symbol(), )
    SYNC_PROP_SET(move_parents, Symbol(), )
    SYNC_PROP_SET(clip_crossover, Symbol(), )
    SYNC_PROP(merger, mMerger)
    SYNC_PROP(game_mode_merger, mGameModeMerger)
    SYNC_PROP(move_merger, mMoveMerger)
    SYNC_PROP(disable_picking, mDisablePicking)
    SYNC_PROP_SET(player_freestyle, mPlayerFreestyle, UpdatePlayerFreestyle(_val.Int()))
    SYNC_PROP_SET(
        pause_player_freestyle, mPlayerFreestylePaused, PausePlayerFreestyle(_val.Int())
    )
    SYNC_PROP(force_postproc, mForcePostProc)
    SYNC_PROP(force_postproc_blend, mForcePostProcBlend)
    SYNC_PROP(force_postproc_blend_rate, mForcePostProcBlendRate)
    SYNC_PROP(disabled, mDisabled)
    SYNC_PROP(excitement, mExcitement)
    SYNC_PROP(num_players_failed, mNumPlayersFailed)
    SYNC_PROP(cam_postproc, mCamPostProc)
    SYNC_PROP_SET(cur_shot, mCurShot.Ptr(), )
    SYNC_PROP_SET(cur_world, mCurWorld.Ptr(), )
    SYNC_PROP_SET(backup_drift, mBackupDrift, )
    SYNC_PROP_SET(spot_instructor, Symbol("off"), SetCharSpot("instructor", _val.Sym()))
    SYNC_PROP(practice_start, mPracticeStart)
    SYNC_PROP(practice_end, mPracticeEnd)
    SYNC_PROP(start_loop_margin, mStartLoopMargin)
    SYNC_PROP(end_loop_margin, mEndLoopMargin)
    SYNC_PROP(blend_debug, mBlendDebug)
    SYNC_PROP(no_transitions, mNoTransitions)
    SYNC_PROP(collision_checks, mCollisionChecks)
    SYNC_PROP_SET(
        dancer_face_clip, GetMainFaceOverrideClip(), SetMainFaceOverrideClip(_val.Sym())
    )
    SYNC_PROP_SET(
        dancer_face_weight,
        GetMainFaceOverrideWeight(),
        SetMainFaceOverrideWeight(_val.Float())
    )
    SYNC_PROP(freestyle_enabled, mFreestyleEnabled)
    SYNC_PROP(loaded_new_song, mLoadedNewSong)
    SYNC_SUPERCLASS(RndDrawable)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS
