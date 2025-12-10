#pragma once
#include "obj/Data.h"
#include "obj/Object.h"
#include "os/DateTime.h"
#include "ui/UILabel.h"
#include "ui/UIListLabel.h"
#include "utl/Symbol.h"

enum NavListNodeType {
    // stole these from RB3 lmao
    kNodeNone = 0,
    kNodeShortcut = 1,
    // kNodeHeader = 2,
    // kNodeSubheader = 3,
    // kNodeSong = 4,
    // kNodeFunction = 5,
    // kNodeSetlist = 6,
    // kNodeStoreSong = 7
};

class NavListItemSortCmp {
public:
    NavListItemSortCmp() {}
    virtual ~NavListItemSortCmp() {}
    virtual int Compare(const NavListItemSortCmp *, NavListNodeType) const = 0;
    virtual bool HasSubheader() const { return false; }
    virtual const class DifficultyCmp *GetDifficultyCmp() const;
    virtual const class SongCmp *GetSongCmp() const;
    virtual const class ArtistCmp *GetArtistCmp() const; // tentative
    virtual const class LocationCmp *GetLocationCmp() const; // tentative
    virtual const class DecadeCmp *GetDecadeCmp() const; // tentative
    virtual const class VenueCmp *GetVenueCmp() const; // tentative
    virtual const class DateCmp *GetDateCmp() const; // tentative
    virtual const class AlbumCmp *GetAlbumCmp() const; // tentative
    virtual const class VocalPartsCmp *GetVocalPartsCmp() const; // tentative
    virtual const class PlaylistTypeCmp *GetPlaylistTypeCmp() const;
    virtual const class ChallengeScoreCmp *GetChallengeScoreCmp() const;
    virtual const class MQSongCharCmp *GetMQSongCharCmp() const; // tentative
    virtual const class FitnessCalorieSortCmp *GetFitnessCalorieSortCmp() const;
};

class NavListNode : public Hmx::Object {
public:
    NavListNode(NavListItemSortCmp *);
    virtual ~NavListNode();
    virtual NavListNodeType GetType() const = 0;
    virtual Symbol GetToken() const = 0;
    virtual bool LocalizeToken() const { return true; }
    virtual const DateTime *GetDateTime() const { return nullptr; }

    int Compare(const NavListNode *, NavListNodeType) const;
    void GetID(DataArray *);

protected:
    NavListItemSortCmp *mCmp; // 0x2c
    NavListNode *mParent; // 0x30
};

class NavListSort;
class NavListShortcutNode;

class NavListSortNode : public NavListNode {
public:
    NavListSortNode(NavListItemSortCmp *);
    virtual ~NavListSortNode();
    virtual DataNode Handle(DataArray *, bool);
    virtual void FinishSort(NavListSort *);
    virtual Symbol OnSelect() = 0;
    virtual Symbol Select() = 0;
    virtual Symbol OnSelectDone() { return gNullStr; }
    virtual void OnHighlight() = 0;
    virtual void OnUnHighlight() = 0;
    virtual void SetCollapseIconLabel(UILabel *) = 0;
    virtual int GetItemCount() = 0;
    virtual NavListSortNode *GetFirstActive() = 0;
    virtual void OnContentMounted(const char *, const char *) {}
    virtual void Text(UIListLabel *, UILabel *) const;
    virtual void Custom(UIListCustom *, Hmx::Object *) const;
    virtual RndMat *Mat(UIListMesh *) const { return nullptr; }
    virtual bool IsActive() const = 0;
    virtual bool IsEnabled() const = 0;
    virtual const char *GetAlbumArtPath() = 0;
    virtual void DeleteAll();
    virtual void Renumber(std::vector<NavListSortNode *> &);
    virtual void FinishBuildList(NavListSort *);

    void SetShortcut(NavListShortcutNode *);

protected:
    std::list<NavListSortNode *> mChildren; // 0x34
    NavListShortcutNode *mShortcut; // 0x3c
    int mStartIx; // 0x40
};

class NavListShortcutNode : public NavListNode {
public:
    NavListShortcutNode(NavListItemSortCmp *, Symbol, bool);
    virtual ~NavListShortcutNode();
    virtual NavListNodeType GetType() const { return kNodeShortcut; }
    virtual Symbol GetToken() const { return mToken; }
    virtual bool LocalizeToken() const { return mLocalizeToken; }
    virtual const DateTime *GetDateTime() const { return mDateTime; }
    virtual void DeleteAll();

protected:
    Symbol mToken; // 0x34
    bool mLocalizeToken; // 0x38
    DateTime *mDateTime; // 0x3c
    std::list<NavListSortNode *> mChildren; // 0x40
};

// NavListItemNode
// NavListHeaderNode
// NavListFunctionNode
