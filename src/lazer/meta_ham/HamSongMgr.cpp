#include "lazer/meta_ham/HamSongMgr.h"
#include "HamSongMetadata.h"
#include "lazer/meta_ham/Playlist.h"
#include "macros.h"
#include "meta/DataArraySongInfo.h"
#include "meta/Jukebox.h"
#include "meta/SongMgr.h"
#include "obj/Data.h"
#include "obj/Dir.h"
#include "obj/Object.h"
#include "os/ContentMgr.h"
#include "os/Debug.h"
#include "os/File.h"
#include "os/System.h"
#include "stl/_vector.h"
#include "utl/BinStream.h"
#include "utl/FakeSongMgr.h"
#include "utl/Locale.h"
#include "utl/MakeString.h"
#include "utl/SongInfoCopy.h"
#include "utl/Std.h"
#include "utl/Symbol.h"

HamSongMgr TheHamSongMgr;

HamSongMgr::HamSongMgr() : unkd0(0), mJukebox(Jukebox(2000)), unk168(false) {
    ClearAndShrink<String>(unk150);
}

BEGIN_HANDLERS(HamSongMgr)
    HANDLE_EXPR(has_song, HasSong(_msg->Sym(2)))
    HANDLE_EXPR(song_path, SongPath(_msg->Sym(2), 0))
    HANDLE_EXPR(song_file_path, SongFilePath(_msg->Sym(2), _msg->Str(3), 0))
    HANDLE_EXPR(
        get_meta_data, (HamSongMetadata *)Data(GetSongIDFromShortName(_msg->Sym(2)))
    )
    HANDLE_EXPR(get_bpm, GetBPM(_msg->Sym(2)))
    HANDLE_EXPR(rank_tier, RankTier(_msg->Int(2)))
    HANDLE_EXPR(rank_tier_for_song, RankTier(_msg->Sym(2)))
    HANDLE_EXPR(dancer_for_song, GetCharacter(_msg->Sym(2)))
    HANDLE_EXPR(num_rank_tiers, (int)unk134.size())
    HANDLE_EXPR(rank_tier_token, RankTierToken(_msg->Int(2)))
    HANDLE(get_random_song, OnGetRandomSong)
    HANDLE_ACTION(add_recent_song, AddRecentSong(_msg->Sym(2)))
    HANDLE_EXPR(album_art_path, GetAlbumArtPath(_msg->Sym(2)))
    HANDLE_EXPR(is_dummy_song, IsDummySong(_msg->Sym(2)))
    HANDLE_EXPR(get_song_id, GetSongIDFromShortName(_msg->Sym(2)))
    HANDLE_EXPR(barks_file, BarksFile(_msg->Sym(2)))
    HANDLE_EXPR(midi_file, MidiFile(_msg->Sym(2)))
    HANDLE_EXPR(toggle_random_song_debug, ToggleRandomSongDebug())
    HANDLE_ACTION(add_songs, AddSongs(_msg->Array(2)))
    HANDLE_EXPR(get_artist_name_from_short_name, GetArtistNameFromShortName(_msg->Sym(2)))
    HANDLE_SUPERCLASS(SongMgr)
END_HANDLERS

struct SongRankCmp {
    SongRankCmp(HamSongMgr *h) : mMgr(h) {}
    bool operator()(int, int) const;

    HamSongMgr *mMgr;
};

void HamSongMgr::ContentDone() {
    SongMgr::ContentDone();
    unk11c.clear();
    FOREACH (it, mAvailableSongs) {
        const HamSongMetadata *data = Data(*it);
        if (data->IsRanked()) {
            unk11c.push_back(*it);
        }
    }
    std::sort(unk11c.begin(), unk11c.end(), SongRankCmp(this));
    unk128.clear();
    FOREACH (it, unk11c) {
        if (!Data(*it)->IsDownload()) {
            unk128.push_back(*it);
        }
    }
    InitializePlaylists();
    UploadSongLibraryToServer();
}

void HamSongMgr::Init() {
    SongMgr::Init();
    SetName("song_mgr", ObjectDir::Main());
    TheContentMgr.RegisterCallback(this, false);
    unkd4.clear();
    unkec.clear();
    static Symbol song_mgr("song_mgr");
    static Symbol alt_dirs("alt_dirs");
    DataArray *cfg = SystemConfig(song_mgr);
    DataArray *altDirsArray = cfg->FindArray(alt_dirs, false);
    if (altDirsArray) {
        for (int i = 1; i < altDirsArray->Size(); i++) {
            const char *curDir = altDirsArray->Array(i)->Str(0);
            if (strlen(curDir) != 0) {
                unk150.push_back(curDir);
            }
        }
    }
    static Symbol tier_ranges("tier_ranges");
    DataArray *tierArr = cfg->FindArray(tier_ranges);
    int numTiers = tierArr->Size() - 1;
    unk134.reserve(numTiers);
    for (int i = 1; i < numTiers; i++) {
        unk134.push_back(
            std::make_pair(tierArr->Array(i)->Int(0), tierArr->Array(i)->Int(1))
        );
    }
}

void HamSongMgr::Terminate() {
    RELEASE(unkd0);
    TheContentMgr.UnregisterCallback(this, false);
    unkd4.clear();
    unkec.clear();
    ClearAndShrink<String>(unk150);
    ClearPlaylists();
}

SongInfo *HamSongMgr::SongAudioData(int songID) const {
    auto data = Data(songID);
    if (!data)
        return nullptr;
    else {
        SongInfo *songInfo = data->SongBlock();
        MILO_ASSERT(songInfo, 0x19E);
        HamSongMgr *me = const_cast<HamSongMgr *>(this);
        RELEASE(me->unkd0);
        me->unkd0 = new DataArraySongInfo(songInfo);
        const char *content = ContentName(songID);
        if (content && TheContentMgr.IsMounted(content)) {
            const char *root = ContentNameRoot(content);
            if (root) {
                const char *base = unkd0->GetBaseFileName();
                MILO_ASSERT(!streq(root, ""), 0x1B1);
                unkd0->SetBaseFileName(MakeString("%s/%s", root, base));
            }
        }
        return unkd0;
    }
}

Symbol HamSongMgr::GetShortNameFromSongID(int songID, bool fail) const {
    MILO_ASSERT(songID != kSongID_Invalid && songID != kSongID_Any && songID != kSongID_Random, 0x166);
    auto it = unkd4.find(songID);
    if (it != unkd4.end()) {
        return it->second;
    } else {
        auto it = unk104.find(songID);
        if (it != unk104.end()) {
            return it->second;
        } else {
            if (fail) {
                MILO_FAIL("Couldn't find song short name for SONG_ID %d", songID);
            }
            return gNullStr;
        }
    }
}

int HamSongMgr::GetSongIDFromShortName(Symbol shortname, bool fail) const {
    auto it = unkec.find(shortname);
    if (it != unkec.end()) {
        MILO_ASSERT(it->second != kSongID_Invalid, 0x17D);
        return it->second;
    } else {
        FOREACH (it, unk104) {
            if (it->second == shortname) {
                MILO_ASSERT(it->first != kSongID_Invalid, 0x186);
                return it->first;
            }
        }
        if (fail) {
            MILO_FAIL(
                "Couldn't find song ID for short name %s, does song have a SONG_ID?",
                shortname.Str()
            );
        }
        return 0;
    }
}

void HamSongMgr::AddSongData(DataArray *a, DataLoader *dl, ContentLocT loc) {
    const char *str = ".";
    if (dl) {
        str = FileGetPath(FileGetPath(dl->LoaderFile().c_str()));
    }
    std::vector<int> vec;
    AddSongData(a, mUncachedSongMetadata, str, loc, vec);
}

void HamSongMgr::AddSongData(
    DataArray *a,
    std::map<int, SongMetadata *> &map,
    const char *,
    ContentLocT loc,
    std::vector<int> &vec
) {
    int aSize = a->Size();
    int i = 0;
    for (; i < aSize; i++) {
        if (a->Type(i) == kDataArray)
            break;
    }
    for (int j = i; j < aSize; j++) {
        DataArray *curArr = a->Array(j);
        static Symbol song_id("song_id");
        Symbol curSym = curArr->Sym(0);
        int songID = 0;
        curArr->FindData(song_id, songID, false);
        static Symbol missing_song_data("missing_song_data");
        DataArray *missingArr = SystemConfig(missing_song_data)->FindArray(curSym, false);
        if (HasSong(songID)) {
            MILO_LOG("The song %s was found twice in the song manager data\n", curSym);
        } else {
            bool root = loc == kLocationRoot;
            AddSongIDMapping(songID, curSym);
            if (map.find(songID) != map.end()) {
                delete map.find(songID)->second;
            }
            if (missingArr) {
                map[songID] = new HamSongMetadata(missingArr, curArr, root);
            } else {
                map[songID] = new HamSongMetadata(curArr, nullptr, root);
            }
            mAvailableSongs.insert(songID);
            vec.push_back(songID);
        }
    }
}

void HamSongMgr::AddSongIDMapping(int songID, Symbol shortname) {
    auto it = unkd4.find(songID);
    if (it != unkd4.end() && songID != 0 && it->second != shortname) {
        MILO_NOTIFY(
            "Song %s and song %s have duplicate song_id %d!", shortname, it->second, songID
        );
    }
    auto it2 = unkec.find(shortname);
    if (it2 != unkec.end() && it2->second != songID) {
        MILO_NOTIFY(
            "SongID %d and SongID %d have duplicate short name %s!",
            songID,
            it2->second,
            shortname
        );
    }
    unkd4[songID] = shortname;
    unkec[shortname] = songID;
}

void HamSongMgr::ReadCachedMetadataFromStream(BinStream &bs, int) {
    int num;
    bs >> num;
    for (int i = 0; i < num; i++) {
        int cacheKey;
        bs >> cacheKey;
        HamSongMetadata *hsm = new HamSongMetadata();
        hsm->Load(bs);
        mCachedSongMetadata[cacheKey] = hsm;
        static Symbol missing_song_data("missing_song_data");
        DataArray *arr =
            SystemConfig(missing_song_data)->FindArray(hsm->ShortName(), false);
        if (arr) {
            hsm->Integrate(arr, nullptr, false);
        }
    }
}

void HamSongMgr::WriteCachedMetadataToStream(BinStream &bs) const {
    bs << mCachedSongMetadata.size();
    FOREACH (it, mCachedSongMetadata) {
        bs << it->first;
        it->second->Save(bs);
    }
}

bool HamSongMgr::ToggleRandomSongDebug() {
    bool old = unk168;
    unk168 = !unk168;
    return unk168;
}

const char *HamSongMgr::MidiFile(Symbol shortname) const {
    SongInfo *info = SongMgr::SongAudioData(shortname);
    return info ? FakeSongMgr::MidiFile(info) : gNullStr;
}

char const *HamSongMgr::GetAlbumArtPath(Symbol s) const {
    if (SongMgr::HasSong(s, true)) {
        return SongMgr::SongFilePath(s, "_keep.png", 0);
    }
    return gNullStr;
}

void HamSongMgr::AddRecentSong(Symbol s) {
    int id = GetSongIDFromShortName(s, true);
    mJukebox.Play(id);
}

Symbol HamSongMgr::GetArtistNameFromShortName(Symbol s) {
    int id = GetSongIDFromShortName(s, true);
    const HamSongMetadata *meta = (const HamSongMetadata *)Data(id);
    char const *artist = meta->Artist(); // so what was the point of this
    return meta->Artist();
}

Playlist *HamSongMgr::GetPlaylist(Symbol p) {
    FOREACH (it, mPlaylists) {
        Playlist *pPlaylist = *it;
        MILO_ASSERT(pPlaylist, 0xa4);
        if (pPlaylist->unk4 == p) {
            return pPlaylist;
        }
    }
    return nullptr;
}

Playlist *HamSongMgr::GetPlaylist(int index) {
    MILO_ASSERT((0) <= (index) && (index) < (mPlaylists.size()), 0xc2);
    return mPlaylists[index];
}

Playlist *HamSongMgr::GetPlaylistWithLocalizedName(String p) {
    FOREACH (it, mPlaylists) {
        Playlist *playlist = *it;
        MILO_ASSERT(playlist, 0xb5);
        const char *locName = Localize(playlist->unk4, nullptr, TheLocale);
        if (locName == p.c_str()) {
            return playlist;
        }
    }
    return nullptr;
}

char const *HamSongMgr::BarksFile(Symbol song) const {
    static Symbol nor("nor");
    Symbol lang = SystemLanguage();
    int ver = 10;
    if (lang == nor) {
        ver = 0xb;
    }

    const char *songpath = SongMgr::SongPath(song, ver);
    const char *filepath = FileGetPath(songpath);
    songpath = MakeString("%s/loc/%s/barks.milo", filepath, lang);

    return SongMgr::CachedPath(song, songpath, ver);
}

int HamSongMgr::GetDuration(Symbol song) const {
    int id = GetSongIDFromShortName(song, true);
    const HamSongMetadata *data = (const HamSongMetadata *)Data(id);
    MILO_ASSERT(data, 0x1f6);
    int length = data->LengthMs();
    return length / 1000;
}

Symbol HamSongMgr::GetCharacter(Symbol song) const {
    int id = GetSongIDFromShortName(song, true);
    const HamSongMetadata *data = (const HamSongMetadata *)Data(id);
    MILO_ASSERT(data, 0x1fe);
    Symbol character = data->Character();
    return character;
}

int HamSongMgr::GetBPM(Symbol song) const {
    int id = GetSongIDFromShortName(song, true);
    const HamSongMetadata *data = (const HamSongMetadata *)Data(id);
    MILO_ASSERT(data, 0x207);
    int bpm = data->Bpm();
    return bpm;
}

Symbol HamSongMgr::RankTierToken(int token) const {
    return MakeString("song_tier_%i", token);
}

void HamSongMgr::InitializePlaylists() {
    ClearPlaylists();
    static Symbol playlists("playlists");
    static Symbol songs("songs");
    DataArray *playlistArray = SystemConfig(playlists);
    MILO_ASSERT(playlistArray, 0xd9);

    if (1 < playlistArray->Size()) {
        for (int i = 1; i < playlistArray->Size(); i++) {
            DataArray *playlistEntry = playlistArray->Node(i).Array();
            MILO_ASSERT(playlistEntry, 0xdf);

            Symbol s = playlistEntry->Sym(0);
            Playlist *p = new Playlist();

            static Symbol is_fitness("is_fitness");
            p->unk4 = s;
            p->unk8 = false;
            DataArray *songArray = playlistEntry->FindArray(songs, true);
            MILO_ASSERT(songArray, 0xed);

            if (1 < songArray->Size()) {
                for (int i = 1; i < songArray->Size(); i++) {
                    Symbol sym = songArray->Sym(i);
                    int songID = GetSongIDFromShortName(sym, 0);
                    if (songID == 0) {
                        MILO_NOTIFY(
                            "HMX Playlist: %s is referring to unknown song: %s",
                            sym,
                            songID
                        );
                    } else {
                        p->AddSong(songID);
                    }
                }
            }
            if (!p->m_vSongs.empty()) {
                mPlaylists.push_back(p);
            }
        }
    }
    char buffer[16] = {};
}

void HamSongMgr::ClearPlaylists() {
    FOREACH (it, mPlaylists) {
        delete (*it);
        *it = nullptr;
    }
    mPlaylists.clear();
}

DataNode HamSongMgr::OnGetRandomSong(DataArray *) { return GetRandomSong(); }
