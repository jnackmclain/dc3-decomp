#include "ui/UIList.h"
#include "ui/UITransitionHandler.h"
#include "utl/Std.h"

UIList::UIList()
    : UITransitionHandler(this), unk8c(this), mListState(this, this), unkf8(0),
      unkfc(100), unk100(0), unk104(0), unk108(0), mExtendedLabelEntries(this),
      mExtendedMeshEntries(this), mExtendedCustomEntries(this), unk148(2), unk14c(0),
      unk150(1), unk154(0), unk158(-1), unk15c(0), unk15d(0), unk160(1), unk164(1) {}

UIList::~UIList() {
    DeleteAll(unka4);
    // RELEASE(unkf8);
}

BEGIN_PROPSYNCS(UIList)
    SYNC_PROP_MODIFY(list_resource, unk8c, Update())
    SYNC_PROP_SET(display_num, NumDisplay(), SetNumDisplay(_val.Int()))
    SYNC_PROP_SET(grid_span, GridSpan(), SetGridSpan(_val.Int()))
    SYNC_PROP_SET(circular, Circular(), SetCircular(_val.Int()))
    SYNC_PROP_SET(scroll_time, Speed(), SetSpeed(_val.Float()))
    SYNC_PROP(paginate, unk100)
    SYNC_PROP_SET(
        min_display, mListState.MinDisplay(), mListState.SetMinDisplay(_val.Int())
    )
    SYNC_PROP_SET(
        scroll_past_min_display,
        mListState.ScrollPastMinDisplay(),
        mListState.SetScrollPastMinDisplay(_val.Int())
    )
    SYNC_PROP_SET(
        scroll_past_min_display,
        mListState.ScrollPastMinDisplay(),
        mListState.SetScrollPastMinDisplay(_val.Int())
    )
    SYNC_PROP_SET(
        max_display, mListState.MaxDisplay(), mListState.SetMaxDisplay(_val.Int())
    )
    SYNC_PROP_SET(
        scroll_past_max_display,
        mListState.ScrollPastMaxDisplay(),
        mListState.SetScrollPastMaxDisplay(_val.Int())
    )
    SYNC_PROP_MODIFY(num_data, unkfc, Update())
    SYNC_PROP(auto_scroll_pause, unk148)
    SYNC_PROP(auto_scroll_send_messages, unk14c)
    SYNC_PROP(extended_label_entries, mExtendedLabelEntries)
    SYNC_PROP(extended_mesh_entries, mExtendedMeshEntries)
    SYNC_PROP(extended_custom_entries, mExtendedCustomEntries)
    SYNC_PROP_SET(in_anim, GetInAnim(), SetInAnim(_val.Obj<RndAnimatable>()))
    SYNC_PROP_SET(out_anim, GetOutAnim(), SetOutAnim(_val.Obj<RndAnimatable>()))
    SYNC_PROP_SET(
        limit_circular_display_num_to_data_num, unk15e, LimitCircularDisplay(_val.Int())
    )
    SYNC_SUPERCLASS(ScrollSelect)
    SYNC_SUPERCLASS(UIComponent)
END_PROPSYNCS
