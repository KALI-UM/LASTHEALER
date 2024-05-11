#pragma once
#include <list>
#include "GameObject.h"
#define NW 0
#define NE 1
#define SW 2
#define SE 3

class QuadTree : public GameObject
{
public:
	QuadTree(D2D1_VECTOR_2F pos, D2D1_VECTOR_2F size, int maxcnt, int maxdepth);
	~QuadTree();

private:
	D2D1_VECTOR_2F size;	//width, height
	list<GameObject*> container;
	list<GameObject*> overlap_container;		//���ҵ� ������ ��ģ ������Ʈ�� ���⿡ ��´�

	QuadTree* NWSE[4];	//�ڽ� ���
	int maxcnt;			//������ �������� �Ǵ� ������Ʈ�� ��
	int depth;			//������ ���Ѽ�
	int cnt;			//���� ������Ʈ���� �� ������Ʈ ��
public:
	HRESULT Init() override;
	void Release() override;
	void Reset() override;
	void Update() override;
	void Render() override;

private:
	void Divide();

public:
	void AddObject(GameObject* o);
	GameObject* CollisionCheck(D2D1_RECT_F collider);
	int GetCnt();
	bool AreaCheck(D2D1_RECT_F collider);
};