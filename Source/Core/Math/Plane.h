#pragma once
#include "Vector.h"

#define PIDIV2 3.141592654/2

struct alignas(16) FQuat : public FVector4{
    using FVector::X;
    using FVector::Y;
    using FVector::Z;
    using FVector4::W;

    FQuat() : FVector4(0.0f, 0.0f, 0.0f, 1.0f) {}
	explicit FQuat(float InX, float InY, float InZ, float InW) : FVector4(InX, InY, InZ, InW) {}
    explicit FQuat(FVector Rotation) : FVector4(EulerToQuaternion(Rotation)) {}
    FQuat(const FVector& Axis, float AngleInDegrees) : FVector4(AxisAngleToQuaternion(Axis, AngleInDegrees)) {}


    static FQuat EulerToQuaternion(FVector Euler);
    static FVector QuaternionToEuler(const FQuat& quaternion);
    static FQuat AxisAngleToQuaternion(const FVector& Axis, float AngleInDegrees);

    static FQuat AddQuaternions(const FQuat& q1, const FQuat& q2);
    static FQuat MultiplyQuaternions(const FQuat& q1, const FQuat& q2);
    static FQuat SubtractQuaternions(const FQuat& q1, const FQuat& q2);

    static FQuat MakeFromRotationMatrix(const struct FMatrix& M);
    FVector GetEuler() const { return QuaternionToEuler(*this); }

    static FQuat FindBetween(const FVector A, const FVector B);
};
