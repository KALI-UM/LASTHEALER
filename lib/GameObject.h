#pragma once
//모든 오브젝트의 부모 순수가상Class
//회전, 충돌, 상호작용 등을 깰꼼하게 보이게 할 수 있다

class Transform;
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	Transform* pTransform;
public:
	virtual void Initialize() = 0;
	virtual void Render() = 0;
	virtual void Reset() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};