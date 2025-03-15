#include "Vector.h"
#include "Matrix.h"

const FVector FVector::ZeroVector = {};
const FVector FVector::OneVector = {};

FVector4 FVector4::operator*(FMatrix Mat)
{
    return FVector4(
        X * Mat.GetElement(0, 0) + Y * Mat.GetElement(1, 0) + Z*Mat.GetElement(2, 0) + Mat.GetElement(3, 0),
        X * Mat.GetElement(0, 1) + Y * Mat.GetElement(1, 1) + Z*Mat.GetElement(2, 1) + Mat.GetElement(3, 1),
        X * Mat.GetElement(0, 2) + Y * Mat.GetElement(1, 2) + Z*Mat.GetElement(2, 2) + Mat.GetElement(3, 2),
        X * Mat.GetElement(0, 3) + Y * Mat.GetElement(1, 3) + Z*Mat.GetElement(2, 3) + Mat.GetElement(3, 3)
    );
}

FVector4 FVector4::operator/(float Scalar)
{
    return FVector4(X/Scalar, Y/Scalar, Z/Scalar, 1/Scalar);
}
