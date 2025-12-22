#pragma once
#include "meta_ham/HamProfile.h"
#include "obj/Object.h"

class FitnessGoalMgr : public Hmx::Object {
public:
    void DeleteFitnessGoalFromRC(HamProfile *);
    void OnSendFitnessGoalToRC(HamProfile *);
};

extern FitnessGoalMgr *TheFitnessGoalMgr;
