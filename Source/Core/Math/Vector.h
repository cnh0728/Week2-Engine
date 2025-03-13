#pragma once
#include "MathUtility.h"


struct FVector
{
    float X, Y, Z;
    FVector() : X(0), Y(0), Z(0) {}
    FVector(float X, float Y, float Z) : X(X), Y(Y), Z(Z) {}

    static const FVector ZeroVector;
    static const FVector OneVector;

public:
    static FVector Zero() { return {0, 0, 0}; }
    static FVector One() { return {1, 1, 1}; }

    static float DotProduct(const FVector& A, const FVector& B);
    static FVector CrossProduct(const FVector& A, const FVector& B);

    static float Distance(const FVector& V1, const FVector& V2);

    float Length() const;
    float LengthSquared() const;

    bool Normalize(float Tolerance = 1.e-8f);

    FVector GetUnsafeNormal() const;
    FVector GetSafeNormal(float Tolerance = 1.e-8f) const;

    float Dot(const FVector& Other) const;
    FVector Cross(const FVector& Other) const;

    FVector operator+(const FVector& Other) const;
    FVector& operator+=(const FVector& Other);

    FVector operator-(const FVector& Other) const;
    FVector& operator-=(const FVector& Other);

    FVector operator*(const FVector& Other) const;
    FVector operator*(float Scalar) const;
    FVector& operator*=(float Scalar);

    FVector operator/(const FVector& Other) const;
    FVector operator/(float Scalar) const;
    FVector& operator/=(float Scalar);

    FVector operator-() const;

    bool operator==(const FVector& Other) const;
    bool operator!=(const FVector& Other) const;
};

inline float FVector::DotProduct(const FVector& A, const FVector& B)
{
    return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

inline FVector FVector::CrossProduct(const FVector& A, const FVector& B)
{
    return {
        A.Y * B.Z - A.Z * B.Y,
        A.Z * B.X - A.X * B.Z,
        A.X * B.Y - A.Y * B.X
    };
}

inline float FVector::Distance(const FVector& V1, const FVector& V2)
{
    return FMath::Sqrt(FMath::Square(V2.X-V1.X) + FMath::Square(V2.Y-V1.Y) + FMath::Square(V2.Z-V1.Z));
}

inline float FVector::Length() const
{
    return FMath::Sqrt(X*X + Y*Y + Z*Z);
}

inline float FVector::LengthSquared() const
{
    return X*X + Y*Y + Z*Z;
}

inline FVector FVector::GetUnsafeNormal() const
{
    const float Scale = FMath::InvSqrt(X*X+Y*Y+Z*Z);
    return {X*Scale, Y*Scale, Z*Scale};
}

inline FVector FVector::GetSafeNormal(float Tolerance) const
{
    const float SquareSum = X*X + Y*Y + Z*Z;

    // Not sure if it's safe to add tolerance in there. Might introduce too many errors
    if(SquareSum == 1.f)
    {
        return *this;
    }		
    else if(SquareSum < Tolerance)
    {
        return ZeroVector;
    }
    const float Scale = FMath::InvSqrt(SquareSum);
    return {X*Scale, Y*Scale, Z*Scale};
}

inline bool FVector::Normalize(float Tolerance)
{
    const float SquareSum = X*X + Y*Y + Z*Z;
    if(SquareSum > Tolerance)
    {
        const float Scale = FMath::InvSqrt(SquareSum);
        X *= Scale; Y *= Scale; Z *= Scale;
        return true;
    }
    return false;
}

inline float FVector::Dot(const FVector& Other) const
{
    return DotProduct(*this, Other);
}

inline FVector FVector::Cross(const FVector& Other) const
{
    return CrossProduct(*this, Other);
}

inline FVector FVector::operator+(const FVector& Other) const
{
    return {X + Other.X, Y + Other.Y, Z + Other.Z};
}

inline FVector& FVector::operator+=(const FVector& Other)
{
    X += Other.X; Y += Other.Y; Z += Other.Z;
    return *this;
}

inline FVector FVector::operator-(const FVector& Other) const
{
    return {X - Other.X, Y - Other.Y, Z - Other.Z};
}

inline FVector& FVector::operator-=(const FVector& Other)
{
    X -= Other.X; Y -= Other.Y; Z -= Other.Z;
    return *this;
}

inline FVector FVector::operator*(const FVector& Other) const
{
    return {X * Other.X, Y * Other.Y, Z * Other.Z};
}

inline FVector FVector::operator*(float Scalar) const
{
    return {X * Scalar, Y * Scalar, Z * Scalar};
}

inline FVector& FVector::operator*=(float Scalar)
{
    X *= Scalar; Y *= Scalar; Z *= Scalar;
    return *this;
}

inline FVector FVector::operator/(const FVector& Other) const
{
    return {X / Other.X, Y / Other.Y, Z / Other.Z};
}

inline FVector FVector::operator/(float Scalar) const
{
    return {X / Scalar, Y / Scalar, Z / Scalar};
}

inline FVector& FVector::operator/=(float Scalar)
{
    X /= Scalar; Y /= Scalar; Z /= Scalar;
    return *this;
}

inline FVector FVector::operator-() const
{
    return {-X, -Y, -Z};
}

inline bool FVector::operator==(const FVector& Other) const
{
    return X == Other.X && Y == Other.Y && Z == Other.Z;
}

inline bool FVector::operator!=(const FVector& Other) const
{
    return X != Other.X || Y != Other.Y || Z != Other.Z;
}

struct alignas(16) FVector4
{
    float X, Y, Z, W;

    FVector4()
        : X(0), Y(0), Z(0), W(0)
    {
    }
    FVector4(float InX, float InY, float InZ, float InW)
        : X(InX), Y(InY), Z(InZ), W(InW)
    {
    }
};