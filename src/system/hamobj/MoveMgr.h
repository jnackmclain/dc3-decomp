#pragma once
#include "MoveGraph.h"
#include "hamobj/SongLayout.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include <map>
#include <set>

class MoveMgr : public Hmx::Object {
public:
    // Hmx::Object
    virtual ~MoveMgr();
    virtual DataNode Handle(DataArray *, bool);

    void RegisterSongLayout(SongLayout *);
    void UnRegisterSongLayout(SongLayout *);

protected:
    MoveMgr();

    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    std::map<int, MoveVariant *> unk54[3];
    int unk9c;
    int unka0;
    MoveGraph unka4;
    std::set<const MoveVariant *> unk104;
    std::vector<const MoveParent *> unk11c[2];
    std::vector<const MoveVariant *> unk134[2];
    Symbol unk14c;
    std::vector<std::pair<const MoveVariant *, const MoveVariant *> > unk150[2];
    int unk16c;
    int unk170;
    int unk174;
    int unk178;
    int unk17c;
    int unk180;
    int unk184;
    int unk188;
    int unk18c;
    int unk190;
    int unk194;
    int unk198;
    int unk19c;
    int unk1a0;
    int unk1a4;
};

extern MoveMgr *TheMoveMgr;
