#pragma once
#include "meta_ham/Accomplishment.h"
#include "meta_ham/AccomplishmentCategory.h"
#include "meta_ham/AccomplishmentGroup.h"
#include "meta_ham/Award.h"
#include "meta_ham/HamProfile.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "os/ContentMgr.h"
#include "os/PlatformMgr.h"

struct GoalAcquisitionInfo {
    int unk0;
    String unk4;
    int unkc;
};

struct GoalProgressionInfo {
    int unk0;
    String unk4;
    int unkc;
    int unk10;
};

class AccomplishmentManager : public Hmx::Object, public ContentMgr::Callback {
public:
    AccomplishmentManager(DataArray *);
    // Hmx::Object
    virtual ~AccomplishmentManager();
    virtual DataNode Handle(DataArray *, bool);
    // ContentMgr::Callback
    virtual void ContentDone();

    void EarnAccomplishmentForProfile(HamProfile *, Symbol, bool);
    void EarnAccomplishmentForPlayer(int, Symbol);
    void EarnAccomplishmentForAll(Symbol, bool);
    int GetNumAccomplishments() const;
    bool HasCompletedAccomplishment(HamUser *, Symbol) const;
    bool HasNewAwards() const;
    Symbol GetNameForFirstNewAward(HamProfile *) const;
    Symbol GetDescriptionForFirstNewAward(HamProfile *) const;
    String GetArtForFirstNewAward(HamProfile *) const;
    bool HasArtForFirstNewAward(HamProfile *) const;
    HamProfile *GetProfileForFirstNewAward() const;
    void ClearFirstNewAward(HamProfile *);
    void UpdateReasonLabelForFirstNewAward(HamProfile *, UILabel *);
    void ClearGoalProgressionAcquisitionInfo();
    bool IsUnlockableAsset(Symbol) const;
    bool IsGroupComplete(HamProfile *, Symbol) const;
    bool IsCategoryComplete(HamProfile *, Symbol) const;
    int GetNumCompletedAccomplishments(HamUser *) const;

private:
    void InitializeDiscSongs();

protected:
    void Cleanup();
    void ConfigureAccomplishmentGroupData(DataArray *);
    void ConfigureAccomplishmentCategoryData(DataArray *);
    void ConfigureAccomplishmentData(DataArray *);
    void ConfigureAwardData(DataArray *);
    void ConfigureAccomplishmentCategoryGroupingData();
    void ConfigureAccomplishmentGroupToCategoriesData();
    void ConfigureAccomplishmentRewardData(DataArray *);

    DataNode OnMsg(const SigninChangedMsg &);

    bool unk30[2]; // 0x30
    std::map<Symbol, Accomplishment *> unk34;
    std::map<Symbol, AccomplishmentCategory *> unk4c;
    std::map<Symbol, AccomplishmentGroup *> unk64;
    std::map<Symbol, Award *> unk7c;
    std::map<Symbol, Symbol> unk94;
    std::map<Symbol, Symbol> unkac;
    std::map<Symbol, std::list<Symbol> *> unkc4;
    std::map<Symbol, std::set<Symbol> *> unkdc;
    int unkf4;
    int unkf8;
    int unkfc;
    int unk100;
    int unk104;
    int unk108;
    int unk10c;
    int unk110;
    std::vector<GoalAcquisitionInfo> unk114;
    std::vector<GoalProgressionInfo> unk120;
    std::vector<Symbol> unk12c;
};

extern AccomplishmentManager *TheAccomplishmentMgr;
