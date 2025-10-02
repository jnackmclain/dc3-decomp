#include "hamobj/ClipPlayer.h"
#include "HamRegulate.h"
#include "char/CharClip.h"
#include "hamobj/HamCharacter.h"
#include "hamobj/HamDirector.h"
#include "hamobj/SongUtl.h"
#include "math/Utl.h"
#include "obj/Data.h"
#include "rndobj/PropAnim.h"
#include "rndobj/PropKeys.h"

const char *ClipPlayer::sRestStepNames[4] = {
    "rest_step_left", "rest_step_right", "rest_step_fwd", "rest_step_back"
};

bool ClipPlayer::Init(RndPropAnim *anim) {
    mClipDir = TheHamDirector->ClipDir();
    if (anim) {
        PropKeys *clipKeys = anim->GetKeys(TheHamDirector, DataArrayPtr(Symbol("clip")));
        if (clipKeys) {
            mClipKeys = clipKeys->AsSymbolKeys();
        }
        PropKeys *clipCrossoverKeys =
            anim->GetKeys(TheHamDirector, DataArrayPtr(Symbol("clip_crossover")));
        if (clipCrossoverKeys) {
            mClipCrossoverKeys = clipCrossoverKeys->AsSymbolKeys();
        }
        PropKeys *masterKeys = TheHamDirector->GetMasterKeys("clip");
        if (masterKeys) {
            mMasterClipKeys = masterKeys->AsSymbolKeys();
        }
        if (mClipKeys && mMasterClipKeys && mClipDir) {
            Key<Symbol> *k1;
            Key<Symbol> *k2;
            if (TheHamDirector->GetPracticeFrames(k1, k2)) {
                unk20 = Round(FrameToBeat(k1->frame));
                unk24 = Round(FrameToBeat(k2->frame)) - 1.0f;
                String str(k1->value);
                str.ReplaceAll('*', '\0');
                mInClip =
                    mClipDir->Find<CharClip>(MakeString("%s_in", str.c_str()), false);
                str = k2->value;
                str.ReplaceAll('*', '\0');
                mOutClip =
                    mClipDir->Find<CharClip>(MakeString("%s_out", str.c_str()), false);
                mRestClip = mClipDir->Find<CharClip>("rest", false);
                for (int i = 0; i < 4; i++) {
                    mRestStepClips[i] =
                        mClipDir->Find<CharClip>(sRestStepNames[i], false);
                }
            }
            return true;
        }
    }
    return false;
}

void ClipPlayer::PlayAnims(HamCharacter *c, float f1, float f2, int x) {
    unk48 = x;
    unkc = FrameToBeat(f1);
    unk10 = FrameToBeat(f2);
    unk1c = c->SongDriver();
    unk44 = 0;
    unk1c->Clear();
    HamRegulate *reg = c->Regulator();
    PlayNormal(-kHugeFloat, nullptr, "");
    reg->RegulateWay(c->GetWaypoint(), 8);
}
