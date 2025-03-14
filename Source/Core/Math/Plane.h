#pragma once
#include "Vector.h"

struct FMatrix;

#define PIDIV2 3.141592654/2

struct alignas(16) FQuat{
    float X, Y, Z, W;

    FQuat() : X(0), Y(0), Z(0), W(0) {}
    FQuat(const FQuat& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) {}
    explicit FQuat(float InX, float InY, float InZ, float InW) : X(InX), Y(InY), Z(InZ), W(InW) {}
    explicit FQuat(FVector Rotation) : FQuat(EulerToQuaternion(Rotation)) {}
    FQuat(const FVector& Axis, float AngleInDegrees) : FQuat(AxisAngleToQuaternion(Axis, AngleInDegrees)) {}


    static FQuat EulerToQuaternion(FVector Euler);
    static FVector QuaternionToEuler(const FQuat& quaternion);
    static FQuat AxisAngleToQuaternion(const FVector& Axis, float AngleInDegrees);

    static FQuat AddQuaternions(const FQuat& q1, const FQuat& q2);
    static FQuat MultiplyQuaternions(const FQuat& q1, const FQuat& q2);
    static FQuat SubtractQuaternions(const FQuat& q1, const FQuat& q2);

    //static FQuat MakeFromRotationMatrix(const FMatrix& M);
    FVector GetEuler() const { return QuaternionToEuler(*this); }
};
