#include "Plane.h"
#include "Core/Math/Matrix.h"
#include "Vector.h"

FQuat FQuat::AxisAngleToQuaternion(const FVector& Axis, float AngleInDegrees) {
    float AngleInRadians = FMath::DegreesToRadians(AngleInDegrees);
    float HalfAngle = AngleInRadians * 0.5f;
    float s = sinf(HalfAngle);
    return FQuat(
        Axis.X * s,
        Axis.Y * s,
        Axis.Z * s,
        cosf(HalfAngle)
    );
}

FQuat FQuat::EulerToQuaternion(FVector Euler)
{
    float roll = FMath::DegreesToRadians(Euler.X);
    float pitch = FMath::DegreesToRadians(Euler.Y);
    float yaw = FMath::DegreesToRadians(Euler.Z);

    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);

    FQuat q;
    q.W = cr * cp * cy + sr * sp * sy;
    q.X = sr * cp * cy - cr * sp * sy;
    q.Y = cr * sp * cy + sr * cp * sy;
    q.Z = cr * cp * sy - sr * sp * cy;

    return q;
}

FVector FQuat::QuaternionToEuler(const FQuat& Quat) {
    FVector angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (Quat.W * Quat.X + Quat.Y * Quat.Z);
    double cosr_cosp = 1 - 2 * (Quat.X * Quat.X + Quat.Y * Quat.Y);
    angles.X = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (Quat.W * Quat.Y - Quat.X * Quat.Z));
    double cosp = std::sqrt(1 - 2 * (Quat.W * Quat.Y - Quat.X * Quat.Z));
    angles.Y = 2 * std::atan2(sinp, cosp) - PI / 2;

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (Quat.W * Quat.Z + Quat.X * Quat.Y);
    double cosy_cosp = 1 - 2 * (Quat.Y * Quat.Y + Quat.Z * Quat.Z);
    angles.Z = std::atan2(siny_cosp, cosy_cosp);

    angles.X = FMath::RadiansToDegrees(angles.X);
    angles.Y = FMath::RadiansToDegrees(angles.Y);
    angles.Z = FMath::RadiansToDegrees(angles.Z);

    return angles;
}

FQuat FQuat::AddQuaternions(const FQuat& q1, const FQuat& q2) {
    return FQuat(
        q1.X + q2.X,
        q1.Y + q2.Y,
        q1.Z + q2.Z,
        q1.W + q2.W
    );
}

FQuat FQuat::MultiplyQuaternions(const FQuat& q1, const FQuat& q2) {
    return FQuat(
        q1.W * q2.X + q1.X * q2.W + q1.Y * q2.Z - q1.Z * q2.Y, // X
        q1.W * q2.Y - q1.X * q2.Z + q1.Y * q2.W + q1.Z * q2.X, // Y
        q1.W * q2.Z + q1.X * q2.Y - q1.Y * q2.X + q1.Z * q2.W, // Z
        q1.W * q2.W - q1.X * q2.X - q1.Y * q2.Y - q1.Z * q2.Z  // W
    );
}

FQuat FQuat::SubtractQuaternions(const FQuat& q1, const FQuat& q2) {
    return FQuat(
        q1.X - q2.X,
        q1.Y - q2.Y,
        q1.Z - q2.Z,
        q1.W - q2.W
    );
}
//
//FQuat FQuat::MakeFromRotationMatrix(const FMatrix& M)
//{
//    FQuat Q;
//
//    float trace = M.M[0][0] + M.M[1][1] + M.M[2][2]; // 행렬의 Trace 값 (대각합)
//
//    if (trace > 0.0f)
//    {
//        float S = sqrtf(trace + 1.0f) * 2.0f; // S는 4배의 W
//        Q.W = 0.25f * S;
//        Q.X = (M.M[2][1] - M.M[1][2]) / S;
//        Q.Y = (M.M[0][2] - M.M[2][0]) / S;
//        Q.Z = (M.M[1][0] - M.M[0][1]) / S;
//    }
//    else
//    {
//        if (M.M[0][0] > M.M[1][1] && M.M[0][0] > M.M[2][2])
//        {
//            float S = sqrtf(1.0f + M.M[0][0] - M.M[1][1] - M.M[2][2]) * 2.0f;
//            Q.W = (M.M[2][1] - M.M[1][2]) / S;
//            Q.X = 0.25f * S;
//            Q.Y = (M.M[0][1] + M.M[1][0]) / S;
//            Q.Z = (M.M[0][2] + M.M[2][0]) / S;
//        }
//        else if (M.M[1][1] > M.M[2][2])
//        {
//            float S = sqrtf(1.0f + M.M[1][1] - M.M[0][0] - M.M[2][2]) * 2.0f;
//            Q.W = (M.M[0][2] - M.M[2][0]) / S;
//            Q.X = (M.M[0][1] + M.M[1][0]) / S;
//            Q.Y = 0.25f * S;
//            Q.Z = (M.M[1][2] + M.M[2][1]) / S;
//        }
//        else
//        {
//            float S = sqrtf(1.0f + M.M[2][2] - M.M[0][0] - M.M[1][1]) * 2.0f;
//            Q.W = (M.M[1][0] - M.M[0][1]) / S;
//            Q.X = (M.M[0][2] + M.M[2][0]) / S;
//            Q.Y = (M.M[1][2] + M.M[2][1]) / S;
//            Q.Z = 0.25f * S;
//        }
//    }
//
//    return Q;
//}