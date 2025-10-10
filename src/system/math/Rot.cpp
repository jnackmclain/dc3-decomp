#include "Rot.h"
#include "math/Mtx.h"
#include "math/Vec.h"
#include "os/Debug.h"
#include <cmath>

TextStream &operator<<(TextStream &ts, const Hmx::Quat &v) {
    ts << "(x:" << v.x << " y:" << v.y << " z:" << v.z << " w:" << v.w << ")";
    return ts;
}

TextStream &operator<<(TextStream &ts, const Vector3 &v) {
    ts << "(x:" << v.x << " y:" << v.y << " z:" << v.z << ")";
    return ts;
}

TextStream &operator<<(TextStream &ts, const Vector2 &v) {
    ts << "(x:" << v.x << " y:" << v.y << ")";
    return ts;
}

TextStream &operator<<(TextStream &ts, const Hmx::Matrix3 &m) {
    ts << "\n\t" << m.x << "\n\t" << m.y << "\n\t" << m.z;
    return ts;
}

TextStream &operator<<(TextStream &ts, const Transform &t) {
    ts << t.m << "\n\t" << t.v;
    return ts;
}

Hmx::Matrix4 &Hmx::Matrix4::Zero() {
    memset(this, 0, sizeof(*this));
    return *this;
}

void Hmx::Quat::Set(const Vector3 &v, float f) {
    float scale = Sine(f / 2);
    w = Cosine(f / 2);
    x = v.x * scale;
    y = v.y * scale;
    z = v.z * scale;
}

float GetXAngle(const Hmx::Matrix3 &m) { return atan2(m.y.z, m.y.y); }
float GetYAngle(const Hmx::Matrix3 &m) { return atan2(-m.x.z, m.z.z); }
float GetZAngle(const Hmx::Matrix3 &m) {
    float res = atan2(m.y.x, m.y.y);
    return -res;
}

void MakeEuler(const Hmx::Matrix3 &m, Vector3 &v) {
    if (fabsf(m.y.z) > 0.99999988f) {
        v.x = m.y.z > 0 ? PI / 2 : -PI / 2;
        v.z = std::atan2(m.x.y, m.x.x);
        v.y = 0;
    } else {
        v.z = std::atan2(-m.y.x, m.y.y);
        v.x = std::asin(m.y.z);
        v.y = GetYAngle(m);
    }
}

void MakeScale(const Hmx::Matrix3 &m, Vector3 &v) {
    float zlen = Length(m.z);
    Vector3 cross;
    Cross(m.y, m.z, cross);
    float det = Dot(m.x, cross);
    if (det <= 0) {
        zlen = -zlen;
    }
    v.x = Length(m.x);
    v.y = Length(m.y);
    v.z = zlen;
}

void Normalize(const Hmx::Quat &qin, Hmx::Quat &qout) {
    float res = qin.w * qin.w + qin.z * qin.z + qin.x * qin.x + qin.y * qin.y;
    if (res == 0) {
        MILO_NOTIFY_ONCE("trying to normalize zero quat, probable error");
        qout.Reset();
    } else {
        res = 1 / std::sqrt(res);
        qout.Set(qin.x * res, qin.y * res, qin.z * res, qin.w * res);
    }
}

void Interp(const Hmx::Quat &q1, const Hmx::Quat &q2, float r, Hmx::Quat &qres) {
    Nlerp(q1, q2, r, qres);
}

void Interp(const Hmx::Matrix3 &m1, const Hmx::Matrix3 &m2, float r, Hmx::Matrix3 &res) {
    Hmx::Quat q40(m1);
    Hmx::Quat q50(m2);
    Hmx::Quat q60;
    Nlerp(q40, q50, r, q60);
    MakeRotMatrix(q60, res);
}

void MakeEuler(const Hmx::Quat &q, Vector3 &v) {
    Hmx::Matrix3 m;
    MakeRotMatrix(q, m);
    MakeEuler(m, v);
}

// void MakeEuler(Quat *param_1,Vector3 *param_2)

// {
//   Matrix3 MStack_40;

//   MakeRotMatrix(param_1,&MStack_40);
//   MakeEuler(&MStack_40,param_2);
//   return;
// }
