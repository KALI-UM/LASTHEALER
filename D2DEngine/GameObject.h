#pragma once
//��� ������Ʈ�� �θ� ��������Class
//ȸ��, �浹, ��ȣ�ۿ� ���� �����ϰ� ���̰� �� �� �ִ�

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