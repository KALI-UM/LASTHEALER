#include "pch.h"
#include "QuadTree.h"

QuadTree::QuadTree(D2D1_VECTOR_2F pos, D2D1_VECTOR_2F size, int maxcnt, int maxdepth)
	:size(size), maxcnt(maxcnt), depth(maxdepth)
{
	_Transform._Position = pos;
	NWSE[NW] = nullptr;
	NWSE[NE] = nullptr;
	NWSE[SW] = nullptr;
	NWSE[SE] = nullptr;
}

QuadTree::~QuadTree()
{
}

bool QuadTree::AreaCheck(D2D1_RECT_F collider)
{
	return g_D2DEngine.IsCollisionAABB(_RectCollider, collider) == TRUE ? true : false;
}

HRESULT QuadTree::Init()
{
	if (container.size() > maxcnt && depth - 1 > 0)
		Divide();

	RectColliderSetting(SizeF(size.x, size.y), _CAMERAMANAGER->GetCamPos());
	return S_OK;
}

void QuadTree::Release()
{
}

void QuadTree::Reset()
{
}

void QuadTree::Update()
{
	if (container.size() > maxcnt && depth - 1 > 0)
		Divide();

	RectColliderSetting(SizeF(size.x, size.y), _CAMERAMANAGER->GetCamPos());
	if (NWSE[NW] != nullptr)
	{
		NWSE[NW]->Update();
		NWSE[NE]->Update();
		NWSE[SW]->Update();
		NWSE[SE]->Update();
	}
}

void QuadTree::Render()
{
#ifdef _DEBUG

	if (_GAMEMANAGER->GetShowDataInfo())
	{
		g_D2DEngine.DrawTextOut("container: %d",container.size(), _RectCollider.left, _RectCollider.bottom-60, BrushColor::White);
		g_D2DEngine.DrawTextOut("overlap_container: %d", overlap_container.size(), _RectCollider.left, _RectCollider.bottom-40, BrushColor::White);
		g_D2DEngine.DrawTextOut("count: %d",GetCnt(), _RectCollider.left, _RectCollider.bottom-20, BrushColor::White);
		

		g_D2DEngine.DrawRect(_RectCollider, BrushColor::Yellow);
		if (NWSE[NW] != nullptr)
		{
			NWSE[NW]->Render();
			NWSE[NE]->Render();
			NWSE[SW]->Render();
			NWSE[SE]->Render();
		}
	}
#endif
}

GameObject* QuadTree::CollisionCheck(D2D1_RECT_F collider)
{
	if (NWSE[NW] == nullptr)
	{
		list<GameObject*>::iterator it = container.begin();
		for (it; it != container.end();)
		{
			if ((*it)->_IsActive && g_D2DEngine.IsCollisionAABB(collider, (*it)->_RectCollider))
				return(*it);
			else if (!(*it)->_IsActive)
				container.erase(it);
			else
				it++;
		}
	}
	else
	{
		list<GameObject*>::iterator it = overlap_container.begin();
		for (it; it != overlap_container.end();)
		{
			if ((*it)->_IsActive && g_D2DEngine.IsCollisionAABB(collider, (*it)->_RectCollider))
				return(*it);
			else if (!(*it)->_IsActive)
				overlap_container.erase(it);
			else
				it++;
		}

		if (NWSE[NW]->AreaCheck(collider))return NWSE[NW]->CollisionCheck(collider);
		if (NWSE[NE]->AreaCheck(collider))return NWSE[NE]->CollisionCheck(collider);
		if (NWSE[SW]->AreaCheck(collider))return NWSE[SW]->CollisionCheck(collider);
		if (NWSE[SE]->AreaCheck(collider))return NWSE[SE]->CollisionCheck(collider);
	}

	return nullptr;
}

void QuadTree::Divide()
{
	NWSE[NW] = new QuadTree({ _Transform._Position.x - size.x / 4, _Transform._Position.y - size.y / 4 }, { size.x / 2, size.y / 2 }, maxcnt, depth - 1);
	NWSE[NE] = new QuadTree({ _Transform._Position.x + size.x / 4, _Transform._Position.y - size.y / 4 }, { size.x / 2, size.y / 2 }, maxcnt, depth - 1);
	NWSE[SW] = new QuadTree({ _Transform._Position.x - size.x / 4, _Transform._Position.y + size.y / 4 }, { size.x / 2, size.y / 2 }, maxcnt, depth - 1);
	NWSE[SE] = new QuadTree({ _Transform._Position.x + size.x / 4, _Transform._Position.y + size.y / 4 }, { size.x / 2, size.y / 2 }, maxcnt, depth - 1);

	NWSE[NW]->Init();
	NWSE[NE]->Init();
	NWSE[SW]->Init();
	NWSE[SE]->Init();

	while (!container.empty())
	{
		GameObject* temp = container.front();
		container.pop_front();

		bool nw = NWSE[NW]->AreaCheck(temp->_RectCollider);
		bool ne = NWSE[NE]->AreaCheck(temp->_RectCollider);
		bool sw = NWSE[SW]->AreaCheck(temp->_RectCollider);
		bool se = NWSE[SE]->AreaCheck(temp->_RectCollider);

		if (nw && ne || nw && sw || ne && se || sw && se)
			overlap_container.push_back(temp);
		else if (nw)
			NWSE[NW]->AddObject(temp);
		else if (ne)
			NWSE[NE]->AddObject(temp);
		else if (sw)
			NWSE[SW]->AddObject(temp);
		else if (se)
			NWSE[SE]->AddObject(temp);
	}

	NWSE[NW]->Update();
	NWSE[NE]->Update();
	NWSE[SW]->Update();
	NWSE[SE]->Update();
}

int QuadTree::GetCnt()
{
	int temp = container.size() + overlap_container.size();
	if (NWSE[NW] != nullptr)
	{
		temp += NWSE[NW]->GetCnt();
		temp += NWSE[NE]->GetCnt();
		temp += NWSE[SW]->GetCnt();
		temp += NWSE[SE]->GetCnt();
	}
	cnt = temp;
	return temp;
}

void QuadTree::AddObject(GameObject* o)
{
	container.push_back(o);
}
