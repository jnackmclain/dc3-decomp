#include "utl/MemTracker.h"
#include "AllocInfo.h"
#include "MemMgr.h"
#include "MemTrack.h"
#include "obj/DataFunc.h"
#include "os/Debug.h"
#include "utl/KeylessHash.h"
#include "math/Sort.h"
#include "utl/Symbol.h"

bool gMemTrackerTracking;
String gMemLogType;

int HashKey(void *ptr, int size) {
    MILO_ASSERT((uint(ptr) & 7) == 0, 0x25);
    return (uint(ptr) / 8) % size;
}

MemTracker::MemTracker(int x, int y)
    : mHashMem(nullptr), mHashTable(nullptr), mTimeSlice(0), mCurStatTable(0),
      mFreedInfos(y), mLog(0), unk18190(0), mHeap(x) {
    mHashMem = DebugHeapAlloc(y * 8);
    MILO_ASSERT(mHashMem, 0x4E);
    mHashTable = new KeylessHash<void *, AllocInfo *>(
        x * 2, (AllocInfo *)0, (AllocInfo *)-1, (AllocInfo **)mHashMem
    );
    mFreeSysMem = _GetFreeSystemMemory();
    mFreePhysMem = _GetFreePhysicalMemory();
    DataRegisterFunc("spit_alloc_info", SpitAllocInfo);
    DataRegisterFunc("sai", SpitAllocInfo);
}

void *MemTracker::operator new(unsigned int size) { return DebugHeapAlloc(size); }
void MemTracker::operator delete(void *mem) { DebugHeapFree(mem); }

const AllocInfo *MemTracker::GetInfo(void *info) const {
    AllocInfo **found = mHashTable->Find(info);
    if (found) {
        return *found;
    } else
        return nullptr;
}

void MemTracker::Alloc(
    int requestedSize,
    int actualSize,
    const char *type,
    void *memory,
    signed char heap,
    bool pooled,
    unsigned char strat,
    const char *cc8,
    int i9
) {
    if (!gMemTrackerTracking)
        return;
    MILO_ASSERT(type, 0x6D);
    if (mHeap != -1 && heap != mHeap) {
        return;
    }
    gMemTrackerTracking = false;
    AllocInfo::bPrintCsv = true;
    if (!unk18195) {
        String str1;
        String str2;
        AllocInfo *info = new AllocInfo(
            requestedSize, actualSize, type, memory, heap, pooled, strat, cc8, i9, str1, str2
        );
        mHashTable->Insert(info);
        if (pooled || gMemLogType != gNullStr || gMemLogType == type) {
            if (pooled || mHeap != -1 && heap != mHeap) {
                if (mLog) {
                    *mLog << " ((com new) " << "(mem " << !memory << ") " << info
                          << ")\n";
                }
                if (unk181a0) {
                    TheDebug << "::Alloc::" << info->mType << " Allocated "
                             << info->mActSize << " Requested " << info->mReqSize
                             << " Address " << info->mMem << " Heap " << info->mHeap
                             << str1.c_str() << ":" << str2.c_str() << "\n";
                }
            }
        } else {
            // if !mLog goto above
            *mLog << " new, ";
            info->PrintCsv(*mLog);
            *mLog << "\n";
        }
    }
    if (!pooled) {
        mHeapStats[heap].Alloc(actualSize, requestedSize);
    }
    gMemTrackerTracking = true;
}
