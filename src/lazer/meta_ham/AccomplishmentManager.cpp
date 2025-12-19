#include "meta_ham/AccomplishmentManager.h"
#include "meta_ham/HamProfile.h"
#include "obj/Data.h"
#include "obj/Dir.h"
#include "obj/Object.h"
#include "os/ContentMgr.h"
#include "os/Debug.h"
#include "os/PlatformMgr.h"
#include "ui/UILabel.h"

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
    return unk94.find(s) != unk94.end();
}
