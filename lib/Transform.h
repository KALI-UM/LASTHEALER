#pragma once
//이동, 회전, 스케일 행렬을 뱉어주는 함수를 보유
class Transform
{
public:
	Transform();
	~Transform();

private:
	D2D_POINT_2F		m_Center;
	D2D_SIZE_F			m_Size;

public:

	D2D_POINT_2F GetCenterPoint() { return m_Center; };
	void SetCenterPoint(float x, float y) { m_Center.x = x; m_Center.y = y; };
	void SetCenterPoint(D2D_POINT_2F point) { m_Center = point; }

	D2D_MATRIX_3X2_F TranslateMatrixToZero();
	D2D_MATRIX_3X2_F TranslateMatrixToOrigin();

	//이동량, 위치
	D2D_MATRIX_3X2_F TranslateMatrix(float TransX, float TransY);
	D2D_MATRIX_3X2_F TranslateMatrixTo(POINT myPos, POINT goalPos);
	D2D_MATRIX_3X2_F TranslateMatrixTo(float myPosX, float myPosY, float goalPosX, float goalPosY);
	
	D2D_MATRIX_3X2_F RotateMatrix(float angle);

	D2D_MATRIX_3X2_F ScaleMatrix(float scale);
	D2D_MATRIX_3X2_F ScaleMatrix(float scaleX, float scaleY);
	D2D_MATRIX_3X2_F ScaleMatrix(D2D_SIZE_F scale);

};

