#pragma once

struct FVector4;
struct FVector;
struct FQuat;

struct alignas(16) FMatrix
{
	float M[4][4];

	FMatrix();
	FMatrix(const FVector4& InX, const FVector4& InY, const FVector4& InZ, const FVector4& InW);

	static FMatrix Identity();
	static FMatrix Transpose(const FMatrix& Matrix);
	static FMatrix GetTranslateMatrix(float X, float Y, float Z);
	static FMatrix GetTranslateMatrix(FVector Translation);
	static FMatrix GetScaleMatrix(float X, float Y, float Z);
	static FMatrix GetScaleMatrix(const FVector& InScale);
	static FMatrix GetRotateMatrix(const FQuat& Q);
	static FMatrix LookAtLH(const FVector& EyePosition, const FVector& FocusPoint, const FVector& WorldUp);
	static FMatrix PerspectiveFovLH(float FieldOfView, float AspectRatio, float NearPlane, float FarPlane);

	static FMatrix OrthoLH(float ViewWidth, float ViewHeight, float NearPlane, float FarPlane);

	FMatrix operator+(const FMatrix& Other) const;
	FMatrix operator+=(const FMatrix& Other);
	FMatrix operator-(const FMatrix& Other) const;
	FMatrix operator-=(const FMatrix& Other);
	FMatrix operator*(const FMatrix& Other) const;
	FMatrix operator*=(const FMatrix& Other);
	FMatrix operator*(float Other) const;
	FMatrix operator*=(float Other);
	bool operator==(const FMatrix& Other) const;
	bool operator!=(const FMatrix& Other) const;

	FMatrix GetTransposed() const;
	float Determinant() const;
	FMatrix Inverse() const;

	FVector GetTranslation() const;
	FVector GetScale() const;
	FVector GetRotation() const;

	FVector4 TransformVector4(const FVector4& Vector) const;

	float GetElement(int Row, int Column) const;
	
	class FTransform GetTransform() const;
	
};

inline float FMatrix::GetElement(int column, int row) const
{
	return M[column][row];
}