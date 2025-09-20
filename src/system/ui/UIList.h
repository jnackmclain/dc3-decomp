#pragma once
#include "UIListDir.h"
#include "obj/Object.h"
#include "rndobj/Mesh.h"
#include "ui/ResourceDirPtr.h"
#include "ui/UIComponent.h"
#include "ui/UILabel.h"
#include "ui/UIListProvider.h"
#include "ui/ScrollSelect.h"
#include "ui/UIListState.h"
#include "ui/UIListWidget.h"
#include "ui/UITransitionHandler.h"

/**
 * @brief A UI Object representing a list element.
 * Original _objects description:
 * "Component for displaying 1- or 2-dimensional lists of data.
 * Can be oriented horizontally or vertically, can scroll normally or
 * circularly, and can have any number of visible elements (even just
 * one, a.k.a. a spin button)."
 */
class UIList : public UIComponent,
               public UIListProvider,
               public ScrollSelect,
               public UIListStateCallback,
               public UITransitionHandler {
public:
    // Hmx::Object
    virtual ~UIList();
    OBJ_CLASSNAME(UIList)
    OBJ_SET_TYPE(UIList)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual void PreLoad(BinStream &);
    virtual void PostLoad(BinStream &);
    // UIListProvider
    virtual int NumData() const;
    // RndDrawable
    virtual float GetDistanceToPlane(const Plane &, Vector3 &);
    virtual void DrawShowing();
    virtual RndDrawable *CollideShowing(const Segment &, float &, Plane &);
    virtual int CollidePlane(const Plane &);
    // ScrollSelect
    virtual int SelectedAux() const;
    virtual void SetSelectedAux(int);
    // UIListStateCallback
    virtual void StartScroll(const UIListState &, int, bool);
    virtual void CompleteScroll(const UIListState &);
    // UITransitionHandler
    virtual void FinishValueChange();
    virtual bool IsEmptyValue() const;

    void SetNumDisplay(int);
    void SetGridSpan(int);
    void SetCircular(bool);
    void SetSpeed(float speed) { mListState.SetSpeed(speed); }
    void LimitCircularDisplay(bool);

    int NumDisplay() const { return mListState.NumDisplay(); }
    int GridSpan() const { return mListState.GridSpan(); }
    bool Circular() const { return mListState.Circular(); }
    float Speed() const { return mListState.Speed(); }

private:
    void Update();

protected:
    UIList();

    ResourceDirPtr<UIListDir> unk8c; // 0x8c
    std::vector<UIListWidget *> unka4; // 0xa4
    UIListState mListState; // 0xb0
    int unkf8; // 0xf8
    int unkfc; // 0xfc
    bool unk100; // 0x100
    int unk104; // 0x104
    int unk108; // 0x108
    ObjPtrList<UILabel> mExtendedLabelEntries; // 0x10c
    ObjPtrList<RndMesh> mExtendedMeshEntries; // 0x120
    ObjPtrList<Hmx::Object> mExtendedCustomEntries; // 0x134
    float unk148;
    bool unk14c;
    int unk150;
    bool unk154;
    float unk158;
    bool unk15c;
    bool unk15d;
    bool unk15e;
    int unk160;
    bool unk164;
};
