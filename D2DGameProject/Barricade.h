#pragma once
class Barricade : public GameObject
{
	public:
	HRESULT Init() override;
	void Release() override;
	void Reset() override;
	void Update() override;
	void Render() override;
};

