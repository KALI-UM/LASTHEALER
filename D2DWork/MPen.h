#pragma once
#include "Mode.h"
struct Point;
class MPen : public Mode
{
public:
	MPen();
	virtual ~MPen();

	virtual void Initialize();
	virtual void Draw();

private : 
	int size;
	

	int prevx=0;
	int prevy=0;
};

