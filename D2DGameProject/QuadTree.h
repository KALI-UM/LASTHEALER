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
	list<GameObject*> overlap_container;		//분할된 공간에 걸친 오브젝트는 여기에 담는다

	QuadTree* NWSE[4];	//자식 노드
	int maxcnt;			//분할이 기준점이 되는 오브젝트의 수
	int depth;			//분할의 하한선
	int cnt;			//하위 오브젝트까지 총 오브젝트 수
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