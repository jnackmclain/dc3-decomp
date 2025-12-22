#include "meta_ham/HamProfile.h"
#include "hamobj/HamLabel.h"
#include "meta/FixedSizeSaveable.h"
#include "meta/FixedSizeSaveableStream.h"
#include "meta_ham/FitnessGoalMgr.h"
#include "meta_ham/HamSongMgr.h"
#include "meta_ham/MetagameRank.h"
#include "meta_ham/MoveRatingHistory.h"
#include "meta_ham/SongStatusMgr.h"
#include "obj/Object.h"
#include "os/OnlineID.h"
#include "utl/Std.h"
#include "utl/Symbol.h"

HamProfile::HamProfile(int i1)
    : Profile(i1), unk34(this), unk2fc(0), unk2fd(0), unk300(130), unk304(0), unk308(0),
      unk30c(0), unk310(0), unk314(0), unk318(new OnlineID()), unk31c(0), unk320(0),
      unk324(0), unk328(0), unk32c(0), unk330(0), unk334(0), unk338(gNullStr), unk33c(0),
      unk340(0), unk344(0), unk348(0), unk34c(0), unk350(0), unk354(0), unk358(0),
      unk35c(0), unk360(0), unk364(0), unk368(0), unk36c(1), unk370(0), unk374(3) {
    mSaveSizeMethod = SaveSize;
    unk18 = new SongStatusMgr(&TheHamSongMgr);
    unk208 = new MetagameStats();
    unk20c = new MetagameRank(this);
    unk210 = new MoveRatingHistory();
    unk158[0].SetProfile(this);
    unk158[1].SetProfile(this);
    unk158[2].SetProfile(this);
    ResetOutfitPrefs();
    static Symbol playlist_custom("playlist_custom");
    for (int i = 0; i < 5; i++) {
        Symbol cur = MakeString("playlist_custom_%02i", i + 1);
        unk220[i].SetParentProfile(this);
        unk220[i].SetName(cur);
    }
}

HamProfile::~HamProfile() {
    DeleteAll();
    delete unk18;
    delete unk208;
    delete unk20c;
    delete unk210;
}

void HamProfile::SaveFixed(FixedSizeSaveableStream &fs) const {
    fs << *unk18;
    FixedSizeSaveable::SaveStd(fs, unk1c, 100);
    FixedSizeSaveable::SaveStd(fs, unk28, 100);
    fs << unk34;
    fs << unk158[0];
    fs << unk158[1];
    fs << unk158[2];
    fs << *unk208;
    fs << *unk20c;
    fs << *unk210;
    int numPrefs = unk214.size();
    fs << numPrefs;
    for (int i = 0; i < numPrefs; i++) {
        const CharacterPref &curPref = unk214[i];
        FixedSizeSaveable::SaveSymbolID(fs, curPref.unk0);
        FixedSizeSaveable::SaveSymbolID(fs, curPref.unk4);
    }
    for (int i = 0; i < 5; i++) {
        fs << unk220[i];
    }
    fs << unk314;
    fs << unk2fd;
    fs << unk300;
    fs << unk304;
    fs << unk324;
    fs << unk340;
    fs << unk344;
    fs << unk348;
    fs << unk34c;
    fs << unk350;
    fs << unk354;
    fs << unk358;
    fs << unk35c;
    fs << unk32c;
    fs << unk364;
    fs << unk368;
    fs << unk36c;
    fs << unk370;
    fs << unk374;
    fs << unk33c;
    fs << unk360;
    const_cast<HamProfile *>(this)->mDirty = false;
}

void HamProfile::LoadFixed(FixedSizeSaveableStream &fs, int i2) {
    DeleteAll();
    fs >> *unk18;
    FixedSizeSaveable::LoadStd(fs, unk1c, 100);
    FixedSizeSaveable::LoadStd(fs, unk28, 100);
    fs >> unk34;
    fs >> unk158[0];
    fs >> unk158[1];
    fs >> unk158[2];
    fs >> *unk208;
    fs >> *unk20c;
    fs >> *unk210;
    int count;
    fs >> count;
    for (int i = 0; i < count; i++) {
        CharacterPref pref;
        FixedSizeSaveable::LoadSymbolFromID(fs, pref.unk0);
        FixedSizeSaveable::LoadSymbolFromID(fs, pref.unk0);
        // some finding here
    }
    for (int i = 0; i < 5; i++) {
        fs >> unk220[i];
    }
    fs >> unk314;
    fs >> unk2fd;
    fs >> unk300;
    fs >> unk304;
    fs >> unk324;
    fs >> unk340;
    fs >> unk344;
    fs >> unk348;
    fs >> unk34c;
    fs >> unk350;
    fs >> unk354;
    fs >> unk358;
    fs >> unk35c;
    unk328 = 0;
    fs >> unk32c;
    fs >> unk364;
    fs >> unk368;
    fs >> unk36c;
    fs >> unk370;
    fs >> unk374;
    fs >> unk33c;
    fs >> unk360;
    mDirty = false;
    unk2fc = false;
}

BEGIN_HANDLERS(HamProfile)
    HANDLE_EXPR(get_accomplishment_progress, &unk34)
    HANDLE_EXPR(get_stats, unk208)
    HANDLE_EXPR(get_rank, unk20c)
    HANDLE_ACTION(mark_content_not_new, MarkContentNotNew(_msg->ForceSym(2)))
    HANDLE_EXPR(character_outfit, CharacterOutfit(_msg->Sym(2)))
    HANDLE_ACTION(set_character_outfit, SetCharacterOutfit(_msg->Sym(2), _msg->Sym(3)))
    HANDLE_EXPR(next_outfit_vo_sample, NextOutfitSample(_msg->Sym(2)))
    HANDLE_EXPR(has_song_status, HasSongStatus(_msg->Sym(2)))
    HANDLE_ACTION(set_pad_num, mPadNum = _msg->Int(2))
    HANDLE_EXPR(get_pad_num, GetPadNum())
    HANDLE_EXPR(in_fitness_mode, unk2fd)
    HANDLE_EXPR(is_fitness_weight_entered, unk304)
    HANDLE_ACTION(set_fitness_pounds, SetFitnessPounds(_msg->Float(2)))
    HANDLE_EXPR(get_fitness_pounds, unk300)
    HANDLE_ACTION(
        set_fitness_stats, SetFitnessStats(_msg->Int(2), _msg->Float(3), _msg->Float(4))
    )
    HANDLE_ACTION(toggle_fitness_mode, SetFitnessMode(!unk2fd))
    HANDLE_EXPR(get_fitness_time, (int)unk308)
    HANDLE_ACTION(update_fitness_time, UpdateFitnessTime(_msg->Obj<HamLabel>(2)))
    HANDLE_ACTION(
        update_fitness_total_time, UpdateFitnessTotalTime(_msg->Obj<HamLabel>(2))
    )
    HANDLE_ACTION(update_fitness_weight, UpdateFitnessWeight(_msg->Obj<HamLabel>(2)))
    HANDLE_EXPR(get_fitness_calories, (int)unk30c)
    HANDLE_ACTION(
        update_infinite_playlist_time, UpdateInfinitePlaylistTime(_msg->Obj<HamLabel>(2))
    )
    HANDLE_EXPR(get_skipped_song_count, unk328)
    HANDLE_ACTION(increment_skipped_song_count, IncrementSkippedSongCount())
    HANDLE_EXPR(get_battle_won_count, GetBattleWonCount(_msg->Int(2)))
    HANDLE_EXPR(get_battle_lost_count, GetBattleLostCount(_msg->Int(2)))
    HANDLE_EXPR(get_won_last_battle, GetWonLastBattle(_msg->Int(2)))
    HANDLE_EXPR(is_fitness_goal_set, unk33c)
    HANDLE_EXPR(is_fitness_days_goal_met, IsFitnessDaysGoalMet())
    HANDLE_EXPR(is_fitness_calories_goal_met, IsFitnessCaloriesGoalMet())
    HANDLE_ACTION(
        set_fitness_goals_through, SetFitnessGoalsThrough(_msg->Obj<HamLabel>(2))
    )
    HANDLE_ACTION(set_fitness_goal_days, SetFitnessGoalDays(_msg->Obj<HamLabel>(2)))
    HANDLE_ACTION(
        set_fitness_goal_calories, SetFitnessGoalCalories(_msg->Obj<HamLabel>(2))
    )
    HANDLE_ACTION(
        set_fitness_goal_days_result, SetFitnessGoalDaysResult(_msg->Obj<HamLabel>(2))
    )
    HANDLE_ACTION(
        set_fitness_goal_calories_result,
        SetFitnessGoalCaloriesResult(_msg->Obj<HamLabel>(2))
    )
    HANDLE_ACTION(reset_fitness_goal, ResetFitnessGoal())
    HANDLE_ACTION(send_fitness_goal_target_days, SetFitnessGoalTargetDays(_msg->Int(2)))
    HANDLE_ACTION(
        send_fitness_goal_target_calories, SetFitnessGoalTargetCalories(_msg->Int(2))
    )
    HANDLE_ACTION(send_fitness_goal_to_rc, SendFitnessGoalToRC())
    HANDLE_ACTION(set_last_new_song, SetLastNewSong())
    HANDLE_ACTION(update_nag, UpdateNag())
    HANDLE_EXPR(needs_to_be_nagged, NeedsToBeNagged())
    HANDLE_EXPR(nag, Nag())
    HANDLE_ACTION(complete_current_nag, CompleteCurrentNag(_msg->Int(2)))
    HANDLE_ACTION(complete_nag, CompleteNag(_msg->Int(2), _msg->Int(3)))
    HANDLE_ACTION(reset_nags, ResetNags())
    HANDLE_ACTION(refresh_playlists, RefreshPlaylists())
    HANDLE_ACTION(unlock_content, UnlockContent(_msg->Sym(2)))
    HANDLE_MESSAGE(SingleItemEnumCompleteMsg)
    HANDLE_SUPERCLASS(Profile)
END_HANDLERS

bool HamProfile::IsFitnessDaysGoalMet() const { return unk33c && unk354 >= unk34c; }
bool HamProfile::IsFitnessCaloriesGoalMet() const { return unk33c && unk358 >= unk350; }

void HamProfile::SendFitnessGoalToRC() {
    if (unk33c) {
        TheFitnessGoalMgr->OnSendFitnessGoalToRC(this);
    } else {
        TheFitnessGoalMgr->DeleteFitnessGoalFromRC(this);
    }
}

void HamProfile::CheckForNinjaUnlock() {}
void HamProfile::CheckForIconManUnlock() {}
