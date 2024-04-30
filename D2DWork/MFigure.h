#pragma once
#include "Mode.h"
class MFigure : public Mode
{
public:
	MFigure();
	virtual ~MFigure();

	virtual void Initialize();
	virtual void Draw();

	bool bRect = true;
	int prevx;
	int prevy;
};

