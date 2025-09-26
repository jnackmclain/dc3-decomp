#include "hamobj/HamVisDir.h"
#include "Pose.h"
#include "gesture/BaseSkeleton.h"
#include "gesture/SkeletonUpdate.h"
#include "os/Debug.h"

PoseOwner::PoseOwner() : pose(0), holder(0), unk8(0) {}

PoseOwner::~PoseOwner() {
    MILO_ASSERT(!pose || pose != holder, 0x1C);
    delete pose;
    delete holder;
}

HamVisDir::HamVisDir()
    : unk2c4(0), unk2c8(0), unk2d8(0), unk2dc(0), unk2e0(this), unk2f4(this),
      unk308(this), unk31c(this), unk330(1), unk334(0) {
    SkeletonUpdateHandle handle = SkeletonUpdate::InstanceHandle();
    if (!handle.HasCallback(this)) {
        handle.AddCallback(this);
    }
    for (int i = 0; i < 2; i++) {
        unk338[i].unkc = MakeString("pose_squat_%i", i);
        unk338[i].pose = new Pose(10, (Pose::ScoreMode)1);
        unk338[i].pose->AddElement(
            new JointDistPoseElement(kJointHead, kJointKneeRight, 0, 0.9)
        );
        unk338[i].pose->AddElement(
            new JointDistPoseElement(kJointHead, kJointKneeLeft, 0, 0.9)
        );
        unk338[i].pose->AddElement(new CamDistancePoseElement(2, 1));

        unk338[i].holder = new Pose(10, (Pose::ScoreMode)0);
        unk338[i].holder->AddElement(
            new JointDistPoseElement(kJointHead, kJointKneeRight, 0, 0.94)
        );
        unk338[i].holder->AddElement(
            new JointDistPoseElement(kJointHead, kJointKneeLeft, 0, 0.94)
        );
        unk338[i].holder->AddElement(new CamDistancePoseElement(1.6, 1));

        unk358[i].unkc = MakeString("pose_y_%i", i);
        unk358[i].pose = new Pose(10, (Pose::ScoreMode)1);
        unk358[i].pose->AddElement(new BoneAngleRangePoseElement(
            kBoneArmLowerRight, Vector3(1, 1, 0), 0.7853981852531433, 1
        ));
        unk358[i].pose->AddElement(new BoneAngleRangePoseElement(
            kBoneArmLowerLeft, Vector3(-1, 1, 0), 0.7853981852531433, 1
        ));
        unk358[i].pose->AddElement(new BoneAngleRangePoseElement(
            kBoneArmUpperRight, Vector3(1, 1, 0), 0.7853981852531433, 1
        ));
        unk358[i].pose->AddElement(new BoneAngleRangePoseElement(
            kBoneArmUpperLeft, Vector3(-1, 1, 0), 0.7853981852531433, 1
        ));
        unk358[i].pose->AddElement(new BoneAngleRangePoseElement(
            kBoneLegLowerRight, Vector3(1, -1, 0), 0.7853981852531433, 1
        ));
        unk358[i].pose->AddElement(new BoneAngleRangePoseElement(
            kBoneLegLowerLeft, Vector3(-1, -1, 0), 0.7853981852531433, 1
        ));
        unk358[i].pose->AddElement(new BoneAngleRangePoseElement(
            kBoneLegUpperRight, Vector3(1, -1, 0), 0.7853981852531433, 1
        ));
        unk358[i].pose->AddElement(new BoneAngleRangePoseElement(
            kBoneLegUpperLeft, Vector3(-1, -1, 0), 0.7853981852531433, 1
        ));

        unk358[i].holder = new Pose(10, (Pose::ScoreMode)0);
        unk358[i].holder->AddElement(new BoneAngleRangePoseElement(
            kBoneArmLowerRight, Vector3(1, 1, 0), 1.047197580337524, 0
        ));
        unk358[i].holder->AddElement(new BoneAngleRangePoseElement(
            kBoneArmLowerLeft, Vector3(-1, 1, 0), 1.047197580337524, 0
        ));
        unk358[i].holder->AddElement(new BoneAngleRangePoseElement(
            kBoneArmUpperRight, Vector3(1, 1, 0), 1.047197580337524, 0
        ));
        unk358[i].holder->AddElement(new BoneAngleRangePoseElement(
            kBoneArmUpperLeft, Vector3(-1, 1, 0), 1.047197580337524, 0
        ));
        unk358[i].holder->AddElement(new BoneAngleRangePoseElement(
            kBoneLegLowerRight, Vector3(1, -1, 0), 1.047197580337524, 0
        ));
        unk358[i].holder->AddElement(new BoneAngleRangePoseElement(
            kBoneLegLowerLeft, Vector3(-1, -1, 0), 1.047197580337524, 0
        ));
        unk358[i].holder->AddElement(new BoneAngleRangePoseElement(
            kBoneLegUpperRight, Vector3(1, -1, 0), 1.047197580337524, 0
        ));
        unk358[i].holder->AddElement(new BoneAngleRangePoseElement(
            kBoneLegUpperLeft, Vector3(-1, -1, 0), 1.047197580337524, 0
        ));
    }
}

HamVisDir::~HamVisDir() {
    RELEASE(unk2c4);
    SkeletonUpdateHandle handle = SkeletonUpdate::InstanceHandle();
    if (handle.HasCallback(this)) {
        handle.RemoveCallback(this);
    }
}
