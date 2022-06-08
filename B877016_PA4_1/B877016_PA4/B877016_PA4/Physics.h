#pragma once
#include <vector>
#include "Utility.h"

class Physics
{
private:
	bool isDirty;

public:
	Physics();
	~Physics();

	bool Update();
	bool CheckCollision(Model* model);
	
};

