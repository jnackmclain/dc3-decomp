#include "meta_ham/NavListNode.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "ui/UILabel.h"
#include "ui/UIListCustom.h"
#include "ui/UIListLabel.h"
#include "utl/Symbol.h"

NavListNode::NavListNode(NavListItemSortCmp *cmp) : mCmp(cmp), mParent(0) {}
NavListNode::~NavListNode() { RELEASE(mCmp); }

int NavListNode::Compare(const NavListNode *n, NavListNodeType t) const {
    return mCmp->Compare(n->mCmp, t);
}

void NavListNode::GetID(DataArray *a) {
    a->Resize(0);
    if (mParent) {
        mParent->GetID(a);
    }
    int size = a->Size();
    a->Resize(size + 1);
    DataNode &n = a->Node(size);
    n = GetToken();
}

NavListSortNode::NavListSortNode(NavListItemSortCmp *cmp)
    : NavListNode(cmp), mShortcut(0), mStartIx(-1) {}

NavListSortNode::~NavListSortNode() { RELEASE(mCmp); }

BEGIN_HANDLERS(NavListSortNode)
    HANDLE_EXPR(album_art_path, GetAlbumArtPath())
    HANDLE_EXPR(get_node_type, GetType())
    HANDLE_EXPR(get_index, mStartIx)
    HANDLE_EXPR(get_token, GetToken())
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS

void NavListSortNode::FinishSort(NavListSort *sort) {
    FOREACH (it, mChildren) {
        (*it)->FinishSort(sort);
    }
}

void NavListSortNode::Text(UIListLabel *listLabel, UILabel *label) const {
    label->SetTextToken(gNullStr);
}

void NavListSortNode::Custom(UIListCustom *custom, Hmx::Object *obj) const {
    if (custom->Matches("stars")) {
        // uses HamStarDisplay
    }
}

void NavListSortNode::DeleteAll() {
    FOREACH (it, mChildren) {
        (*it)->DeleteAll();
        RELEASE(*it);
    }
    mChildren.clear();
}

void NavListSortNode::Renumber(std::vector<NavListSortNode *> &nodes) {
    mStartIx = nodes.size();
    FOREACH (it, mChildren) {
        (*it)->Renumber(nodes);
    }
}

void NavListSortNode::FinishBuildList(NavListSort *sort) {
    FOREACH (it, mChildren) {
        (*it)->FinishBuildList(sort);
    }
}

void NavListSortNode::SetShortcut(NavListShortcutNode *shortcut) { mShortcut = shortcut; }

NavListShortcutNode::NavListShortcutNode(
    NavListItemSortCmp *cmp, Symbol token, bool localize
)
    : NavListNode(cmp), mToken(token), mLocalizeToken(localize), mDateTime(nullptr) {
    mParent = nullptr;
}

void NavListShortcutNode::DeleteAll() {
    FOREACH (it, mChildren) {
        (*it)->DeleteAll();
        RELEASE(*it);
    }
    mChildren.clear();
}
