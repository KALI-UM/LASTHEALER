#include "pch.h"
#include "Transform.h"

Transform::Transform()
{

}

Transform::~Transform()
{

}


D2D_MATRIX_3X2_F Transform::TranslateMatrixToZero()
{
	D2D_MATRIX_3X2_F _TMatrix;
	_TMatrix._11 = 1.0f;
	_TMatrix._12 = 0.0f;
	_TMatrix._21 = 0.0f;
	_TMatrix._22 = 1.0f;
	_TMatrix._31 = -m_Center.x;
	_TMatrix._32 = -m_Center.y;

	return _TMatrix;
}

D2D_MATRIX_3X2_F Transform::TranslateMatrixToOrigin()
{
	D2D_MATRIX_3X2_F _TMatrix;
	_TMatrix._11 = 1.0f;
	_TMatrix._12 = 0.0f;
	_TMatrix._21 = 0.0f;
	_TMatrix._22 = 1.0f;
	_TMatrix._31 = m_Center.x;
	_TMatrix._32 = m_Center.y;

	return _TMatrix;
}

D2D_MATRIX_3X2_F Transform::TranslateMatrix(float TransX, float TransY)
{
	D2D_MATRIX_3X2_F _TMatrix;
	_TMatrix._11 = 1.0f;
	_TMatrix._12 = 0.0f;
	_TMatrix._21 = 0.0f;
	_TMatrix._22 = 1.0f;
	_TMatrix._31 = +TransX;
	_TMatrix._32 = +TransY;

	return _TMatrix;
}

D2D_MATRIX_3X2_F Transform::TranslateMatrixTo(POINT myPos, POINT goalPos)
{
	D2D_MATRIX_3X2_F _TMatrix;
	_TMatrix._11 = 1.0f;
	_TMatrix._12 = 0.0f;
	_TMatrix._21 = 0.0f;
	_TMatrix._22 = 1.0f;
	_TMatrix._31 = goalPos.x - myPos.x;
	_TMatrix._32 = goalPos.y - myPos.y;

	return _TMatrix;
}

D2D_MATRIX_3X2_F Transform::TranslateMatrixTo(float myPosX, float myPosY, float goalPosX, float goalPosY)
{
	D2D_MATRIX_3X2_F _TMatrix;
	_TMatrix._11 = 1.0f;
	_TMatrix._12 = 0.0f;
	_TMatrix._21 = 0.0f;
	_TMatrix._22 = 1.0f;
	_TMatrix._31 = goalPosX - myPosX;
	_TMatrix._32 = goalPosY - myPosY;

	return _TMatrix;
}

D2D_MATRIX_3X2_F Transform::RotateMatrix(float angle)
{
	D2D_MATRIX_3X2_F _RMatrix;
	_RMatrix._11 = cosf(angle);
	_RMatrix._12 = sinf(angle);
	_RMatrix._21 = -sinf(angle);
	_RMatrix._22 = cosf(angle);
	_RMatrix._31 = 0.f;
	_RMatrix._32 = 0.f;

	return _RMatrix;
}

D2D_MATRIX_3X2_F Transform::ScaleMatrix(float scale)
{
	D2D_MATRIX_3X2_F _SMatrix;
	_SMatrix._11 = scale;
	_SMatrix._12 = 0.f;
	_SMatrix._21 = 0.f;
	_SMatrix._22 = scale;
	_SMatrix._31 = 0.f;
	_SMatrix._32 = 0.f;

	return _SMatrix;
}

D2D_MATRIX_3X2_F Transform::ScaleMatrix(float scaleX, float scaleY)
{
	D2D_MATRIX_3X2_F _SMatrix;
	_SMatrix._11 = scaleX;
	_SMatrix._12 = 0.f;
	_SMatrix._21 = 0.f;
	_SMatrix._22 = scaleY;
	_SMatrix._31 = 0.f;
	_SMatrix._32 = 0.f;

	return _SMatrix;
}

D2D_MATRIX_3X2_F Transform::ScaleMatrix(D2D_SIZE_F scale)
{
	D2D_MATRIX_3X2_F _SMatrix;
	_SMatrix._11 = scale.width;
	_SMatrix._12 = 0.f;
	_SMatrix._21 = 0.f;
	_SMatrix._22 = scale.height;
	_SMatrix._31 = 0.f;
	_SMatrix._32 = 0.f;

	return _SMatrix;
}

