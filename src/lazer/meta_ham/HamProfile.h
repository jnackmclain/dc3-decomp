#pragma once
#include "game/HamUser.h"
#include "hamobj/Difficulty.h"
#include "hamobj/HamLabel.h"
#include "meta/Profile.h"
#include "meta_ham/AccomplishmentProgress.h"
#include "meta_ham/CampaignProgress.h"
#include "meta_ham/MetagameRank.h"
#include "meta_ham/MetagameStats.h"
#include "meta_ham/MoveRatingHistory.h"
#include "meta_ham/Playlist.h"
#include "meta_ham/SongStatusMgr.h"
#include "obj/Msg.h"
#include "os/OnlineID.h"

struct CharacterPref {
    Symbol unk0;
    Symbol unk4;
    int unk8;
};

DECLARE_MESSAGE(SingleItemEnumCompleteMsg, "single_item_enum_complete")
END_MESSAGE

class HamProfile : public Profile {
public:
    HamProfile(int);
    virtual ~HamProfile();
    // Hmx::Object
    virtual DataNode Handle(DataArray *, bool);

    virtual bool HasCheated() const;
    virtual bool IsUnsaved() const;
    virtual void SaveLoadComplete(ProfileSaveState);
    virtual bool HasSomethingToUpload();
    virtual void DeleteAll();

    static int SaveSize(int);

    void CheckForIconManUnlock();
    void CheckForNinjaUnlock();
    void SetFitnessMode(bool);
    HamUser *GetHamUser() const;
    SongStatusMgr *GetSongStatusMgr() const;
    CampaignProgress &GetCampaignProgress(Difficulty) const;
    const AccomplishmentProgress &GetAccomplishmentProgress() const;
    AccomplishmentProgress &AccessAccomplishmentProgress();
    void EarnAccomplishment(Symbol);
    void GetFitnessStats(float &, float &, float &);
    void UnlockContent(Symbol);
    void MarkContentNotNew(Symbol);
    Symbol CharacterOutfit(Symbol) const;
    void SetCharacterOutfit(Symbol, Symbol);
    const char *NextOutfitSample(Symbol);
    bool HasSongStatus(Symbol) const;
    void SetFitnessPounds(float);
    void SetFitnessStats(int, float, float);
    void UpdateFitnessTime(HamLabel *);
    void UpdateFitnessTotalTime(HamLabel *);
    void UpdateFitnessWeight(HamLabel *);
    void UpdateInfinitePlaylistTime(HamLabel *);
    int GetBattleWonCount(int) const;
    int GetBattleLostCount(int) const;
    bool GetWonLastBattle(int) const;
    void SetFitnessGoalsThrough(HamLabel *);
    void SetFitnessGoalDays(HamLabel *);
    void SetFitnessGoalCalories(HamLabel *);
    void SetFitnessGoalDaysResult(HamLabel *);
    void SetFitnessGoalCaloriesResult(HamLabel *);
    void ResetFitnessGoal();
    void SetFitnessGoalTargetDays(int);
    void SetFitnessGoalTargetCalories(int);
    void SendFitnessGoalToRC();
    void SetLastNewSong();
    bool NeedsToBeNagged();
    Symbol Nag();
    void CompleteCurrentNag(bool);
    void CompleteNag(int, bool);
    void ResetNags();
    bool IsFitnessDaysGoalMet() const;
    bool IsFitnessCaloriesGoalMet() const;
    void IncrementSkippedSongCount() { unk328++; }
    void UpdateNag() { unk368++; }

private:
    // FixedSizeSaveable
    virtual void SaveFixed(FixedSizeSaveableStream &) const;
    virtual void LoadFixed(FixedSizeSaveableStream &, int);

    void ResetOutfitPrefs();
    void RefreshPlaylists();

    DataNode OnMsg(const SingleItemEnumCompleteMsg &);

    SongStatusMgr *unk18; // 0x18
    std::vector<Symbol> unk1c;
    std::vector<Symbol> unk28;
    AccomplishmentProgress unk34;
    CampaignProgress unk158[4];
    MetagameStats *unk208;
    MetagameRank *unk20c;
    MoveRatingHistory *unk210;
    std::vector<CharacterPref> unk214;
    CustomPlaylist unk220[5];
    bool unk2fc;
    bool unk2fd;
    float unk300;
    bool unk304;
    float unk308;
    float unk30c;
    float unk310;
    int unk314;
    OnlineID *unk318;
    bool unk31c;
    int unk320;
    int unk324;
    int unk328;
    int unk32c;
    int unk330;
    bool unk334;
    Symbol unk338;
    bool unk33c;
    int unk340;
    int unk344;
    int unk348;
    int unk34c;
    int unk350;
    int unk354;
    int unk358;
    int unk35c;
    bool unk360;
    int unk364;
    int unk368;
    bool unk36c;
    int unk370;
    int unk374;
};
