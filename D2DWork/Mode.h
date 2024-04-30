#pragma once
#include <string>
class Mode
{
public:
	Mode();
	virtual ~Mode();

	virtual void Initialize();
	virtual void Draw();
};

