#include "SongMgr.h"
#include "SongMetadata.h"
#include "macros.h"
#include "obj/Data.h"
#include "obj/DataFile.h"
#include "obj/Object.h"
#include "os/ContentMgr.h"
#include "os/Debug.h"
#include "os/File.h"
#include "stl/_vector.h"
#include "utl/BufStream.h"
#include "utl/MemMgr.h"
#include "utl/MemStream.h"
#include "utl/Symbol.h"
#include <set>
#include <vector>
#include <map>

SongMgr *TheBaseSongManager;
const char *SONG_CACHE_CONTAINER_NAME = "songcache_bb";

int GetSongID(DataArray *main_arr, DataArray *backup_arr) {
    static Symbol song_id("song_id");
    int x = 0;
    main_arr->FindData(song_id, x, false);
    if (x == 0 && backup_arr != nullptr) {
        backup_arr->FindData(song_id, x, false);
    }
    return x;
}

int CountSongsInArray(DataArray *arr) {
    int i = 0;
    int size = arr->Size();
    for (; i < size && arr->Node(i).Type() != kDataArray; i++)
        ;
    return size - i;
}

#pragma region Hmx::Object

SongMgr::~SongMgr() {}

BEGIN_HANDLERS(SongMgr)
    HANDLE_EXPR(content_name, Symbol(ContentName(_msg->Int(2))))
    HANDLE_EXPR(content_name_from_sym, Symbol(ContentName(_msg->Sym(2), true)))
    HANDLE_EXPR(is_song_mounted, IsSongMounted(_msg->Sym(2)))
    HANDLE_ACTION(clear_from_cache, ClearFromCache(_msg->Sym(2)))
    HANDLE_EXPR(get_total_num_songs, (int)mAvailableSongs.size())
    HANDLE_EXPR(get_song_id_from_short_name, GetSongIDFromShortName(_msg->Sym(2), true))
    HANDLE_EXPR(data, (Hmx::Object *)Data(_msg->Int(2)))
    HANDLE_ACTION(cache_mgr_mount_result, OnCacheMountResult(_msg->Int(2)))
    HANDLE_ACTION(cache_write_result, OnCacheWriteResult(_msg->Int(2)))
    HANDLE_ACTION(cache_mgr_unmount_result, OnCacheUnmountResult(_msg->Int(2)))
    HANDLE_EXPR(num_songs_in_content, NumSongsInContent(_msg->Sym(2)))
    HANDLE_ACTION(dump_songs, DumpSongMgrContents(false))
    HANDLE_ACTION(dump_all_songs, DumpSongMgrContents(true))
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS

#pragma endregion
#pragma region ContentMgr::Callback

void SongMgr::ContentStarted() {
    mAvailableSongs.clear();
    FOREACH (it, mCachedSongMetadata) {
        it->second->IncrementAge();
    }
    mContentUsedForSong.clear();
}

bool SongMgr::ContentDiscovered(Symbol s) {
    if (SongIDInContent(s)) {
        std::vector<int> vec;
        GetSongsInContent(s, vec);
        FOREACH (it, vec) {
            int curInt = *it;
            auto found = mCachedSongMetadata.find(curInt);
            if (found != mCachedSongMetadata.end()) {
                found->second->ResetAge();
                if (!HasSong(curInt)) {
                    mAvailableSongs.insert(curInt);
                    mContentUsedForSong[curInt] = s;
                    AddSongIDMapping(curInt, Data(curInt)->ShortName());
                }
            }
        }
        return true;
    } else
        return false;
}

void SongMgr::ContentMounted(const char *cc1, const char *cc2) {
    unkmap5[cc1] = cc2;
    if (!SongIDInContent(cc1)) {
        std::vector<int> songs;
        mSongIDsInContent[cc1] = songs;
    }
}

void SongMgr::ContentUnmounted(char const *cc) {
    std::map<Symbol, String>::iterator it = unkmap5.find(Symbol(cc));
    if (it != unkmap5.end()) {
        unkmap5.erase(it);
    }
}

void SongMgr::ContentLoaded(Loader *loader, ContentLocT loct, Symbol s) {
    DataLoader *d = dynamic_cast<DataLoader *>(loader);
    MILO_ASSERT(d, 0xDC);
    DataArray *data = d->Data();
    if (data) {
        if (AllowContentToBeAdded(data, loct)) {
            if (!streq(s.Str(), ".")) {
                CacheSongData(data, d, loct, s);
            } else {
                AddSongData(data, d, loct);
            }
        } else {
            std::vector<int> somevecidk;
            int datasize = data->Size();
            for (int i = datasize - CountSongsInArray(data); i < datasize; i++) {
                int songID = GetSongID(data->Array(i), 0);
                auto idIt = mAvailableSongs.find(songID);
                if (idIt != mAvailableSongs.end()) {
                    mAvailableSongs.erase(idIt);
                }
                auto contentIt = mContentUsedForSong.find(songID);
                if (contentIt != mContentUsedForSong.end()) {
                    mContentUsedForSong.erase(contentIt);
                }
            }
            ClearFromCache(s);
        }
    } else
        ClearFromCache(s);
}

void SongMgr::ContentDone() {
    if (!unkbc)
        return;
    mSongCacheNeedsWrite = true;
}

#pragma endregion
#pragma region SongMgr

void SongMgr::Init() {
    mState = kSongMgr_Nil;
    mSongCacheID = nullptr;
    mSongCache = nullptr;
    unkbc = false;
    mSongCacheNeedsWrite = false;
}

const SongMetadata *SongMgr::Data(int id) const {
    if (!HasSong(id))
        return 0;
    else {
        auto it = mUncachedSongMetadata.find(id);
        if (it != mUncachedSongMetadata.end())
            return it->second;
        else {
            auto cit = mCachedSongMetadata.find(id);
            if (cit != mCachedSongMetadata.end())
                return cit->second;
            else {
                MILO_ASSERT(false, 0x8B);
                return 0;
            }
        }
    }
}

void SongMgr::GetContentNames(Symbol s, std::vector<Symbol> &vec) const {
    const char *cntName = ContentName(s, false);
    if (cntName) {
        vec.push_back(cntName);
    }
}

void SongMgr::ClearCachedContent() {
    mSongIDsInContent.clear();
    FOREACH (it, mCachedSongMetadata) {
        delete it->second;
    }
    mCachedSongMetadata.clear();
}

SongInfo *SongMgr::SongAudioData(Symbol s) const {
    return SongAudioData(GetSongIDFromShortName(s, true));
}

bool SongMgr::IsSongCacheWriteDone() const {
    return mState == kSongMgr_Ready || mState == kSongMgr_Failure;
}

char const *SongMgr::GetCachedSongInfoName() const { return SONG_CACHE_CONTAINER_NAME; }

char const *SongMgr::SongPath(Symbol s, int i) const {
    const char *filename = SongAudioData(s)->GetBaseFileName();
    if (i != 0) {
        int songID = GetSongIDFromShortName(s, true);
        if (Data(songID)->Version() < i) {
            const char *base = FileGetBase(filename);
            filename = MakeString("%s%s/%s", AlternateSongDir(), base, base);
        }
    }
    return filename;
}

char const *SongMgr::SongFilePath(Symbol s, char const *path, int i) const {
    const char *pathstr = MakeString("%s%s", SongPath(s, i), path);
    return CachedPath(s, pathstr, i);
}

// void SongMgr::DumpSongMgrContents(bool all) {}

bool SongMgr::HasSong(int id) const {
    return mAvailableSongs.find(id) != mAvailableSongs.end();
}

bool SongMgr::HasSong(Symbol s, bool b) const {
    int songid = GetSongIDFromShortName(s, b);
    return songid != 0 && HasSong(songid);
}

const char *SongMgr::ContentName(int x) const {
    const SongMetadata *data = Data(x);
    if (data && !data->IsOnDisc()) {
        auto it = mContentUsedForSong.find(x);
        MILO_ASSERT(it != mContentUsedForSong.end(), 0x158);
        return it->second.Str();
    } else
        return nullptr;
}

int SongMgr::GetCachedSongInfoSize() const {
    MemStream ms(false);
    ms << 0; // rev
    ms << mSongIDsInContent;
    WriteCachedMetadataFromStream(ms);
    return ms.Tell();
}

// bool SongMgr::IsSongMounted(Symbol s) const { return true; }

// bool SongMgr::SaveCachedSongInfo(BufStream &bs) { return true; }

// bool SongMgr::IsContentUsedForSong(Symbol s, int i) const { return true; }

void SongMgr::StartSongCacheWrite() {
    if (SongCacheNeedsWrite()) {
        ClearSongCacheNeedsWrite();
        if (mState == kSongMgr_SaveUnmount && mSongCache) {
            SetState(kSongMgr_SaveWrite);
        } else
            SetState(kSongMgr_SaveMount);
    }
}

void SongMgr::ClearFromCache(Symbol s) {
    auto it = mSongIDsInContent.find(s);
    MILO_ASSERT_FMT(it != mSongIDsInContent.end(), "Content %s isn't cached!", s);
    mSongIDsInContent.erase(it);
}

// char const *SongMgr::ContentName(int i) const { return nullptr; }

char const *SongMgr::ContentName(Symbol s, bool b) const {
    return ContentName(GetSongIDFromShortName(s, b));
}

// bool SongMgr::LoadCachedSongInfo(BufStream &) { return true; }

// char const *SongMgr::CachedPath(Symbol, char const *, int) const { return nullptr; }

// void SongMgr::SaveMount() {}

// void SongMgr::SaveUnmount() {}

// void SongMgr::SaveWrite() {}

void SongMgr::GetSongsInContent(Symbol s, std::vector<int> &vec) const {
    std::map<Symbol, std::vector<int> >::const_iterator it = mSongIDsInContent.find(s);
    if (it != mSongIDsInContent.end())
        vec = it->second;
}

char const *SongMgr::ContentNameRoot(Symbol s) const {
    auto it = unkmap5.find(s);
    if (it == unkmap5.end())
        return nullptr;
    else
        return it->second.c_str();
}

int SongMgr::NumSongsInContent(Symbol s) const {
    auto it = mSongIDsInContent.find(s);
    if (it != mSongIDsInContent.end())
        return it->second.size();
    else
        return 0;
}

void SongMgr::SetState(SongMgrState state) {
    if (mState == state)
        return;
    mState = state;
    switch (mState) {
    case kSongMgr_SaveMount:
        SaveMount();
        break;
    case kSongMgr_SaveWrite:
        SaveWrite();
        break;
    case kSongMgr_SaveUnmount:
        SaveUnmount();
        break;
    default:
        break;
    }
}

// void SongMgr::OnCacheMountResult(int) {}

// void SongMgr::OnCacheWriteResult(int) {}

// void SongMgr::OnCacheUnmountResult(int) {}

// void SongMgr::CacheSongData(DataArray *, DataLoader *, ContentLocT, Symbol) {}

// void SongMgr::ContentLoaded(Loader *, ContentLocT, Symbol) {}
