#include "meta_ham/AccomplishmentManager.h"
#include "game/GameMode.h"
#include "hamobj/HamGameData.h"
#include "hamobj/HamPlayerData.h"
#include "meta_ham/Accomplishment.h"
#include "meta_ham/AccomplishmentCategory.h"
#include "meta_ham/AccomplishmentGroup.h"
#include "meta_ham/AccomplishmentProgress.h"
#include "meta_ham/Award.h"
#include "meta_ham/HamProfile.h"
#include "meta_ham/HamSongMetadata.h"
#include "meta_ham/HamSongMgr.h"
#include "meta_ham/ProfileMgr.h"
#include "obj/Data.h"
#include "obj/Dir.h"
#include "obj/Object.h"
#include "os/ContentMgr.h"
#include "os/DateTime.h"
#include "os/Debug.h"
#include "os/PlatformMgr.h"
#include "ui/UILabel.h"
#include "utl/Symbol.h"

AccomplishmentManager *TheAccomplishmentMgr;

AccomplishmentManager::AccomplishmentManager(DataArray *cfg) {
    MILO_ASSERT(!TheAccomplishmentMgr, 0x2B);
    TheAccomplishmentMgr = this;
    SetName("acc_mgr", ObjectDir::Main());
    ConfigureAccomplishmentGroupData(cfg->FindArray("accomplishment_groups"));
    ConfigureAccomplishmentCategoryData(cfg->FindArray("accomplishment_categories"));
    ConfigureAccomplishmentData(cfg->FindArray("accomplishments"));
    ConfigureAwardData(cfg->FindArray("awards"));
    ConfigureAccomplishmentCategoryGroupingData();
    ConfigureAccomplishmentGroupToCategoriesData();
    ConfigureAccomplishmentRewardData(cfg->FindArray("accomplishment_rewards"));
    for (int i = 0; i < 2; i++) {
        unk30[i] = false;
    }
    TheContentMgr.RegisterCallback(this, false);
}

AccomplishmentManager::~AccomplishmentManager() { Cleanup(); }

BEGIN_HANDLERS(AccomplishmentManager)
    HANDLE_ACTION(
        earn_accomplishment,
        EarnAccomplishmentForProfile(_msg->Obj<HamProfile>(2), _msg->Sym(3), true)
    )
    HANDLE_ACTION(
        earn_accomplishment_for_player,
        EarnAccomplishmentForPlayer(_msg->Int(2), _msg->Sym(3))
    )
    HANDLE_ACTION(
        earn_accomplishment_for_all, EarnAccomplishmentForAll(_msg->Sym(2), _msg->Int(3))
    )
    HANDLE_EXPR(get_num_goals, GetNumAccomplishments())
    HANDLE_EXPR(
        get_num_completed_goals, GetNumCompletedAccomplishments(_msg->Obj<HamUser>(2))
    )
    HANDLE_EXPR(
        has_completed_goal,
        HasCompletedAccomplishment(_msg->Obj<HamUser>(2), _msg->Sym(3))
    )
    HANDLE_EXPR(has_new_awards, HasNewAwards())
    HANDLE_EXPR(
        get_name_for_first_new_award, GetNameForFirstNewAward(_msg->Obj<HamProfile>(2))
    )
    HANDLE_EXPR(
        get_description_for_first_new_award,
        GetDescriptionForFirstNewAward(_msg->Obj<HamProfile>(2))
    )
    HANDLE_EXPR(
        get_art_for_first_new_award, GetArtForFirstNewAward(_msg->Obj<HamProfile>(2))
    )
    HANDLE_EXPR(
        has_art_for_first_new_award, HasArtForFirstNewAward(_msg->Obj<HamProfile>(2))
    )
    HANDLE_EXPR(get_profile_for_first_new_award, GetProfileForFirstNewAward())
    HANDLE_ACTION(clear_first_new_award, ClearFirstNewAward(_msg->Obj<HamProfile>(2)))
    HANDLE_ACTION(
        update_reason_for_first_new_award,
        UpdateReasonLabelForFirstNewAward(_msg->Obj<HamProfile>(2), _msg->Obj<UILabel>(3))
    )
    HANDLE_ACTION(clear_goal_info, ClearGoalProgressionAcquisitionInfo())
    HANDLE_EXPR(is_unlockable_asset, IsUnlockableAsset(_msg->Sym(2)))
    HANDLE_EXPR(is_group_complete, IsGroupComplete(_msg->Obj<HamProfile>(2), _msg->Sym(3)))
    HANDLE_EXPR(
        is_category_complete, IsCategoryComplete(_msg->Obj<HamProfile>(2), _msg->Sym(3))
    )
    HANDLE_MESSAGE(SigninChangedMsg)
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS

void AccomplishmentManager::ContentDone() { InitializeDiscSongs(); }

bool AccomplishmentManager::IsUnlockableAsset(Symbol s) const {
    return mAssetToAward.find(s) != mAssetToAward.end();
}

void AccomplishmentManager::InitializeDiscSongs() {
    mDiscSongs.clear();
    std::vector<int> songs;
    TheHamSongMgr.GetRankedSongs(songs);
    FOREACH (it, songs) {
        int curSongID = *it;
        const HamSongMetadata *pSongData = TheHamSongMgr.Data(curSongID);
        MILO_ASSERT(pSongData, 0x9D);
        static Symbol ham3("ham3");
        if (pSongData->GameOrigin() == ham3) {
            Symbol shortname = TheHamSongMgr.GetShortNameFromSongID(curSongID);
            mDiscSongs.push_back(shortname);
        }
    }
}

void AccomplishmentManager::UpdateConsecutiveDaysPlayed(HamProfile *profile) {
    DateTime dt;
    AccomplishmentProgress &progress = profile->AccessAccomplishmentProgress();
}

void AccomplishmentManager::AddGoalAcquisitionInfo(Symbol s1, const char *cc, Symbol s2) {
    GoalAcquisitionInfo info;
    info.unk0 = s1;
    info.unk4 = cc;
    info.unkc = s2;
    mGoalAcquisitionInfos.push_back(info);
}

// @BUG: what is the point of this
void AccomplishmentManager::Init(DataArray *cfg) { new AccomplishmentManager(cfg); }

void AccomplishmentManager::ClearGoalProgressionAcquisitionInfo() {
    mGoalAcquisitionInfos.clear();
    mGoalProgressionInfos.clear();
}

void AccomplishmentManager::Cleanup() {
    FOREACH (it, mAccomplishments) {
        RELEASE(it->second);
    }
    mAccomplishments.clear();
    FOREACH (it, mAccomplishmentCategories) {
        RELEASE(it->second);
    }
    mAccomplishmentCategories.clear();
    FOREACH (it, m_mapCategoryToAccomplishmentSet) {
        RELEASE(it->second);
    }
    m_mapCategoryToAccomplishmentSet.clear();
    FOREACH (it, m_mapGroupToCategories) {
        RELEASE(it->second);
    }
    m_mapGroupToCategories.clear();
    FOREACH (it, mAccomplishmentGroups) {
        RELEASE(it->second);
    }
    mAccomplishmentGroups.clear();
    FOREACH (it, mAwards) {
        RELEASE(it->second);
    }
    mAwards.clear();
    mAssetToAward.clear();
    mAwardToSource.clear();
    TheContentMgr.UnregisterCallback(this, false);
    TheAccomplishmentMgr = nullptr;
}

void AccomplishmentManager::AddAssetAward(Symbol s1, Symbol s2) {
    if (GetAssetAward(s1) != gNullStr) {
        MILO_NOTIFY("Asset:%s is earned by multiple sources!", s1.Str());
    } else {
        mAssetToAward[s1] = s2;
    }
}

void AccomplishmentManager::AddAwardSource(Symbol s1, Symbol s2) {
    Accomplishment *pAcc = GetAccomplishment(s2);
    MILO_ASSERT(pAcc, 0x230);
    if (GetAwardSource(s1) != gNullStr) {
        if (!pAcc->IsSecondaryGoal()) {
            MILO_NOTIFY("Award:%s is earned by multiple sources!", s1.Str());
        }
    } else {
        if (pAcc->IsSecondaryGoal()) {
            MILO_NOTIFY(
                "Award:%s has a first source that is marked as a secondary goal!",
                s1.Str()
            );
        }
        mAwardToSource[s1] = s2;
    }
}

bool AccomplishmentManager::HasAccomplishmentCategory(Symbol s) const {
    return mAccomplishmentCategories.find(s) != mAccomplishmentCategories.end();
}

bool AccomplishmentManager::HasAccomplishmentGroup(Symbol s) const {
    return mAccomplishmentGroups.find(s) != mAccomplishmentGroups.end();
}

void AccomplishmentManager::ConfigureAccomplishmentCategoryData(DataArray *cfg) {
    for (int i = 1; i < cfg->Size(); i++) {
        AccomplishmentCategory *pAccomplishmentCategory =
            new AccomplishmentCategory(cfg->Array(i), i);
        MILO_ASSERT(pAccomplishmentCategory, 0xED);
        Symbol name = pAccomplishmentCategory->GetName();
        if (HasAccomplishmentCategory(name)) {
            MILO_NOTIFY("%s accomplishment category already exists, skipping", name.Str());
            delete pAccomplishmentCategory;
        } else {
            Symbol group = pAccomplishmentCategory->GetGroup();
            if (!HasAccomplishmentGroup(group)) {
                MILO_NOTIFY(
                    "%s accomplishment category has invalid group: %s, skipping",
                    name.Str(),
                    group.Str()
                );
                delete pAccomplishmentCategory;
            } else {
                mAccomplishmentCategories[name] = pAccomplishmentCategory;
                if (pAccomplishmentCategory->HasAward()) {
                    AddAwardSource(
                        pAccomplishmentCategory->GetAward(),
                        pAccomplishmentCategory->GetName()
                    );
                }
            }
        }
    }
}

void AccomplishmentManager::ConfigureAccomplishmentCategoryGroupingData() {
    MILO_ASSERT(m_mapCategoryToAccomplishmentSet.empty(), 0x15C);
    FOREACH (it, mAccomplishments) {
        Accomplishment *pAccomplishment = it->second;
        MILO_ASSERT(pAccomplishment, 0x164);
        Symbol category = pAccomplishment->GetCategory();
        Symbol name = pAccomplishment->GetName();
        std::set<Symbol> *set = GetAccomplishmentSetForCategory(category);
        if (!set) {
            set = new std::set<Symbol>();
            m_mapCategoryToAccomplishmentSet[category] = set;
        }
        set->insert(name);
    }
}

bool AccomplishmentManager::HasAccomplishment(Symbol s) const {
    return mAccomplishments.find(s) != mAccomplishments.end();
}

void AccomplishmentManager::ConfigureAccomplishmentData(DataArray *cfg) {
    for (int i = 1; i < cfg->Size(); i++) {
        Accomplishment *pAccomplishment = FactoryCreateAccomplishment(cfg->Array(i), i);
        MILO_ASSERT(pAccomplishment, 0x13E);
        Symbol name = pAccomplishment->GetName();
        if (HasAccomplishment(name)) {
            MILO_NOTIFY("%s accomplishment already exists, skipping", name.Str());
            delete pAccomplishment;
        } else {
            Symbol cat = pAccomplishment->GetCategory();
            if (!HasAccomplishmentCategory(cat)) {
                MILO_NOTIFY(
                    "%s accomplishment is using unknown category: %s",
                    name.Str(),
                    cat.Str()
                );
                delete pAccomplishment;
            } else {
                mAccomplishments[name] = pAccomplishment;
                if (pAccomplishment->HasAward()) {
                    AddAwardSource(
                        pAccomplishment->GetAward(), pAccomplishment->GetName()
                    );
                }
            }
        }
    }
}

void AccomplishmentManager::ConfigureAccomplishmentGroupData(DataArray *cfg) {
    for (int i = 1; i < cfg->Size(); i++) {
        AccomplishmentGroup *pAccomplishmentGroup =
            new AccomplishmentGroup(cfg->Array(i), i);
        MILO_ASSERT(pAccomplishmentGroup, 0x10F);
        Symbol name = pAccomplishmentGroup->GetName();
        if (HasAccomplishmentGroup(name)) {
            MILO_NOTIFY("%s accomplishment category already exists, skipping", name.Str());
            delete pAccomplishmentGroup;
        } else {
            mAccomplishmentGroups[name] = pAccomplishmentGroup;
            if (pAccomplishmentGroup->HasAward()) {
                AddAwardSource(
                    pAccomplishmentGroup->GetAward(), pAccomplishmentGroup->GetName()
                );
            }
        }
    }
}

void AccomplishmentManager::ConfigureAccomplishmentGroupToCategoriesData() {
    MILO_ASSERT(m_mapGroupToCategories.empty(), 0x178);
    FOREACH (it, mAccomplishmentCategories) {
        AccomplishmentCategory *pCategory = it->second;
        MILO_ASSERT(pCategory, 0x180);
        Symbol name = pCategory->GetName();
        Symbol group = pCategory->GetGroup();
        std::list<Symbol> *list = GetCategoryListForGroup(group);
        if (!list) {
            list = new std::list<Symbol>();
            m_mapGroupToCategories[group] = list;
        }
        list->push_back(name);
    }
}

void AccomplishmentManager::ConfigureAccomplishmentRewardData(DataArray *cfg) {
    DataArray *pLeaderboardThresholds = cfg->Array(1);
    for (int i = 0; i < 4; i++) {
        MILO_ASSERT(pLeaderboardThresholds->Array(i+1)->Int(0) == i, 0x197);
        mLeaderboardThresholds[i] = pLeaderboardThresholds->Array(i + 1)->Int(1);
    }
    DataArray *pIconThresholds = cfg->Array(2);
    for (int i = 0; i < 4; i++) {
        MILO_ASSERT(pIconThresholds->Array( i + 1 )->Int( 0 ) == i, 0x19E);
        mIconThresholds[i] = pIconThresholds->Array(i + 1)->Int(1);
    }
}

bool AccomplishmentManager::HasAward(Symbol s) const {
    return mAwards.find(s) != mAwards.end();
}

void AccomplishmentManager::ConfigureAwardData(DataArray *cfg) {
    for (int i = 1; i < cfg->Size(); i++) {
        Award *pAward = new Award(cfg->Array(i), i);
        MILO_ASSERT(pAward, 0x129);
        Symbol name = pAward->GetName();
        if (HasAward(name)) {
            MILO_NOTIFY("%s award already exists, skipping", name.Str());
            delete pAward;
        } else {
            mAwards[name] = pAward;
        }
    }
}

Symbol AccomplishmentManager::GetAssetAward(Symbol s) const {
    auto it = mAssetToAward.find(s);
    if (it != mAssetToAward.end())
        return it->second;
    else
        return gNullStr;
}

Symbol AccomplishmentManager::GetAssetSource(Symbol s) const {
    Symbol award = GetAssetAward(s);
    return GetAwardSource(award);
}

Award *AccomplishmentManager::GetAward(Symbol s) const {
    auto it = mAwards.find(s);
    if (it != mAwards.end())
        return it->second;
    else
        return nullptr;
}

Symbol AccomplishmentManager::GetAwardSource(Symbol s) const {
    auto it = mAwardToSource.find(s);
    if (it != mAwardToSource.end())
        return it->second;
    else
        return gNullStr;
}

void AccomplishmentManager::EarnAwardForAll(Symbol s1, bool b2) {
    if (b2) {
        for (int i = 0; i < 2; i++) {
            HamPlayerData *pPlayer = TheGameData->Player(i);
            MILO_ASSERT(pPlayer, 0x2D1);
            HamProfile *profile = TheProfileMgr.GetProfileFromPad(pPlayer->PadNum());
            if (profile && profile->HasValidSaveData() && pPlayer->IsPlaying()) {
                EarnAwardForProfile(profile, s1);
            }
        }
    } else {
        std::vector<HamProfile *> profiles = TheProfileMgr.GetSignedInProfiles();
        FOREACH (it, profiles) {
            HamProfile *pProfile = *it;
            MILO_ASSERT(pProfile, 0x2E2);
            if (pProfile && pProfile->HasValidSaveData()) {
                EarnAwardForProfile(pProfile, s1);
            }
        }
    }
}

void AccomplishmentManager::EarnAwardForProfile(HamProfile *i_pProfile, Symbol s2) {
    MILO_ASSERT(i_pProfile, 0x2C2);
    AccomplishmentProgress &progress = i_pProfile->AccessAccomplishmentProgress();
    progress.AddAward(s2, gNullStr);
}

bool AccomplishmentManager::HasNewAwards() const {
    for (int i = 0; i < 2; i++) {
        HamPlayerData *pPlayer = TheGameData->Player(i);
        MILO_ASSERT(pPlayer, 0x6A5);
        HamProfile *profile = TheProfileMgr.GetProfileFromPad(pPlayer->PadNum());
        if (profile && profile->HasValidSaveData()
            && profile->GetAccomplishmentProgress().HasNewAwards()) {
            return true;
        }
    }
    if (TheGameMode->InMode("campaign", true)) {
        HamProfile *pProfile = TheProfileMgr.GetActiveProfile(true);
        MILO_ASSERT(pProfile, 0x6B7);
        if (pProfile->GetAccomplishmentProgress().HasNewAwards()) {
            return true;
        }
    }
    return false;
}

Symbol AccomplishmentManager::GetReasonForFirstNewAward(HamProfile *i_pProfile) const {
    MILO_ASSERT(i_pProfile, 0x6C7);
    const AccomplishmentProgress &progress = i_pProfile->GetAccomplishmentProgress();
    Symbol out;
    if (progress.HasNewAwards()) {
        out = progress.GetFirstNewAwardReason();
    } else {
        MILO_ASSERT(false, 0x6D2);
        out = "";
    }
    return out;
}

HamProfile *AccomplishmentManager::GetProfileForFirstNewAward() const {
    for (int i = 0; i < 2; i++) {
        HamPlayerData *pPlayer = TheGameData->Player(i);
        MILO_ASSERT(pPlayer, 0x74E);
        HamProfile *profile = TheProfileMgr.GetProfileFromPad(pPlayer->PadNum());
        if (profile && profile->HasValidSaveData()
            && profile->GetAccomplishmentProgress().HasNewAwards()) {
            return profile;
        }
    }
    if (TheGameMode->InMode("campaign", true)) {
        HamProfile *pProfile = TheProfileMgr.GetActiveProfile(true);
        MILO_ASSERT(pProfile, 0x762);
        if (pProfile->GetAccomplishmentProgress().HasNewAwards()) {
            return pProfile;
        }
    }
    MILO_ASSERT(false, 0x76C);
    return nullptr;
}

void AccomplishmentManager::ClearFirstNewAward(HamProfile *i_pProfile) {
    MILO_ASSERT(i_pProfile, 0x78E);
    i_pProfile->AccessAccomplishmentProgress().ClearFirstNewAward();
}

Symbol AccomplishmentManager::GetNameForFirstNewAward(HamProfile *i_pProfile) const {
    MILO_ASSERT(i_pProfile, 0x6DC);
    const AccomplishmentProgress &progress = i_pProfile->GetAccomplishmentProgress();
    Symbol out;
    if (progress.HasNewAwards()) {
        Symbol firstAward = progress.GetFirstNewAward();
        Award *pAward = GetAward(firstAward);
        MILO_ASSERT(pAward, 0x6E4);
        out = pAward->GetDisplayName();
    } else {
        MILO_ASSERT(false, 0x6EA);
        out = "";
    }
    return out;
}

Symbol
AccomplishmentManager::GetDescriptionForFirstNewAward(HamProfile *i_pProfile) const {
    MILO_ASSERT(i_pProfile, 0x6F4);
    const AccomplishmentProgress &progress = i_pProfile->GetAccomplishmentProgress();
    Symbol firstAward;
    if (progress.HasNewAwards()) {
        firstAward = progress.GetFirstNewAward();
    }
    Symbol out;
    if (firstAward != "") {
        Award *pAward = GetAward(firstAward);
        MILO_ASSERT(pAward, 0x700);
        out = MakeString("%s%s%s", "award_", pAward->GetDisplayName(), "_desc");
    } else {
        MILO_ASSERT(false, 0x706);
        out = "";
    }
    return out;
}

void AccomplishmentManager::UpdateReasonLabelForFirstNewAward(
    HamProfile *i_pProfile, UILabel *i_pLabel
) {
    MILO_ASSERT(i_pProfile, 0x710);
    MILO_ASSERT(i_pLabel, 0x711);
    Symbol symReason = GetReasonForFirstNewAward(i_pProfile);
    MILO_ASSERT(symReason != "", 0x715);
    if (HasAccomplishment(symReason)) {
        static Symbol campaign_award_earned_by_goal("campaign_award_earned_by_goal");
        i_pLabel->SetTokenFmt(campaign_award_earned_by_goal, symReason);
    } else if (HasAccomplishmentCategory(symReason)) {
        static Symbol campaign_award_earned_by_category(
            "campaign_award_earned_by_category"
        );
        i_pLabel->SetTokenFmt(campaign_award_earned_by_category, symReason);
    } else if (HasAccomplishmentGroup(symReason)) {
        static Symbol campaign_award_earned_by_group("campaign_award_earned_by_group");
        i_pLabel->SetTokenFmt(campaign_award_earned_by_group, symReason);
    } else {
        MILO_ASSERT(false, 0x729);
    }
}
