#include "hamobj/MoveGraph.h"
#include "hamobj/Difficulty.h"
#include "math/Rand.h"
#include "obj/Data.h"
#include "os/Debug.h"

MoveParent::MoveParent() {}
MoveParent::MoveParent(const MoveParent *other) {
    unk4 = other->unk4;
    mDifficulty = other->mDifficulty;
    unkc = false;
}

MoveParent::MoveParent(MoveGraph *graph, DataArray *arr) {
    static Symbol name("name");
    static Symbol genre_flags("genre_flags");
    static Symbol era_flags("era_flags");
    static Symbol variant("variant");
    static Symbol difficulty("difficulty");
    if (arr->Type(0) == kDataInt) {
        unk4 = MakeString("%i", arr->Int(0));
    } else {
        unk4 = arr->Sym(0);
    }
    DataArray *diffArr = arr->FindArray(difficulty, false);
    if (diffArr) {
        mDifficulty = SymToDifficulty(diffArr->Sym(1));
    } else {
        mDifficulty = kDifficultyExpert;
    }
    mGenreFlags.clear();
    DataArray *genreArr = arr->FindArray(genre_flags, false);
    if (genreArr) {
        for (int i = 1; i < genreArr->Size(); i++) {
            mGenreFlags.push_back(genreArr->Sym(i));
        }
    }
    mEraFlags.clear();
    DataArray *eraArr = arr->FindArray(era_flags, false);
    if (eraArr) {
        for (int i = 1; i < eraArr->Size(); i++) {
            mEraFlags.push_back(eraArr->Sym(i));
        }
    }
    mVariants.clear();
    DataArray *variantArr = arr->FindArray(variant, true);
    for (int i = 1; i < variantArr->Size(); i++) {
        DataArray *curVarArr = variantArr->Array(i);
        MoveVariant *pVariant = new MoveVariant(graph, curVarArr, this);
        MILO_ASSERT(pVariant, 0x44);
        mVariants.push_back(pVariant);
    }
    unkc = false;
}

MoveParent::~MoveParent() {
    for (int i = 0; i < mVariants.size(); i++) {
        RELEASE(mVariants[i]);
    }
    mVariants.clear();
}

bool MoveParent::IsValidForMiniGame() const {
    for (std::vector<MoveVariant *>::const_iterator it = mVariants.begin();
         it != mVariants.end();
         ++it) {
        if ((*it)->IsValidForMinigame())
            return true;
    }
    return false;
}

const MoveVariant *MoveParent::PickRandomVariant() const {
    return mVariants[RandomInt(0, mVariants.size())];
}

bool MoveParent::HasPrevAdjacent(const MoveParent *parent) const {
    for (std::vector<const MoveParent *>::const_iterator it = mPrevAdjacents.begin();
         it != mPrevAdjacents.end();
         ++it) {
        if (parent == *it)
            return true;
    }
    return false;
}
