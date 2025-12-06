#pragma once
#include "math/Geo.h"
#include "math/Vec.h"
#include "utl/MemMgr.h"
#include <list>

// kdTriList

// kdTree size: 0x2c
template <class T>
class kdTree {
public:
    enum SplitPlaneType {
    };

    class kdTriList {
    public:
        // MEM_OVERLOAD kDTriList, 0x6C

        static kdTriList *Allocate(unsigned int);

        // this has to be a union, i've seen word and float assignments here
        union {
            int integer;
            float real;
        } mValue; // 0x0
    };

    class kdTreeNode {
    public:
        struct Stack {};

        kdTreeNode() : unk0(0), unk4(0x8000) {
            unk0->mValue.real = 0;
            unk0->mValue.integer &= 0xfffffffc;
        }
        ~kdTreeNode() {}
        kdTriList *unk0;
        short unk4;

        bool FindSplit_Mean(const Box &, const std::list<T *> &);
        float
        EvaluateSplit(const Box &, const std::list<T *> &, unsigned char, float) const;
        bool FindSplit_SAH(const Box &, const std::list<T *> &);
        void
        Pack(SplitPlaneType, const Box &, std::list<T *> &, kdTreeNode *, unsigned char);

        MEM_ARRAY_OVERLOAD(kdTreeNode, 0xEC);
    };

    kdTree(const Box &box) {
        unkc.Set(box.mMin, box.mMax);
        mNodes = new kdTreeNode[0x8000];
        for (int i = 0; i < 0x8000; i++) {
            mNodes[i].unk4 |= i;
        }
    }
    ~kdTree();

    bool Intersect(const Vector3 &, const Vector3 &, float, float &) const;

private:
    std::list<T *> unk0; // 0x0 - objects?
    kdTreeNode *mNodes; // 0x8
    Box unkc; // 0xc - bounding box of the tree?
};
