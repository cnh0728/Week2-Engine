#include "Vector.h"
#include "Matrix.h"

const FVector FVector::ZeroVector = {};
const FVector FVector::OneVector = {};

FVector FVector4::xyz() const
{
	return XYZ();
}

FVector FVector4::XYZ() const
{
	return FVector(X,Y,Z);
}

FVector FVector4::GetCoord() const
{
	if (abs(W) < SMALL_NUMBER)
	{
		return FVector(0, 0, 0);
	}
	float denom = 1 / W;
	return FVector(X, Y, Z) * denom;
}

FVector4 FVector4::operator*(const FMatrix& RHS) const
{
	return RHS.TransformVector4(*this);
}

FVector4& FVector4::operator*=(float Scalar)
{
	X *= Scalar; Y *= Scalar; Z *= Scalar; W *= Scalar;
	return *this;
}

FVector4& FVector4::operator/=(float Scalar)
{
	X /= Scalar; Y /= Scalar; Z /= Scalar; W /= Scalar;
	return *this;
}

FVector4 FVector4::operator-(const FVector4& Other) const
{
	return { X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W};
}
