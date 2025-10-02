#include "hamobj/MocapSkeletonIterator.h"
#include "hamobj/HamCharacter.h"
#include "hamobj/HamDirector.h"
#include "hamobj/HamGameData.h"
#include "math/Utl.h"
#include "obj/Task.h"
#include "os/Debug.h"

MocapSkeletonIterator::MocapSkeletonIterator(int x, int y)
    : mDancer(TheHamDirector->GetCharacter(0)), mInput(mDancer), unk24b0(x), unk24b4(y) {
    MILO_ASSERT(TheGameData, 0x16);
    unk24c4.Init();
    unk24bc = -kHugeFloat;
    unk24b8 = unk24b0;
    unk2f98 = TheTaskMgr.Seconds(TaskMgr::kRealTime);
    HamCharacter *c = mDancer;
    if (mDancer) {
        unk2f9c = mDancer->LocalXfm();
        c->Enter();
        mDancer->DirtyLocalXfm().Reset();
        mDancer->Teleport(nullptr);
        Update();
        mInput.ResetSkeletonCharOrigin();
    }
}

MocapSkeletonIterator::~MocapSkeletonIterator() {
    if (mDancer) {
        mDancer->DirtyLocalXfm().Reset();
        mDancer->Teleport(nullptr);
        mDancer->SetLocalXfm(unk2f9c);
    }
    TheTaskMgr.SetSeconds(unk2f98, true);
}

MocapSkeletonIterator::operator bool() { return mDancer && unk24b8 < unk24b4; }

void MocapSkeletonIterator::operator++() {
    unk24b8++;
    Update();
}

bool MocapSkeletonIterator::PrevSkeleton(
    const Skeleton &s, int i2, ArchiveSkeleton &as, int &i3
) const {
    return PrevFromArchive(*this, s, i2, as, i3);
}

void MocapSkeletonIterator::Update() { MILO_ASSERT(mDancer, 0x55); }
