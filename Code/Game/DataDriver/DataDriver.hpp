#pragma once
#include "Engine/Core/XMLUtils/XMLUtils.hpp"

class DataDriver
{
public:
	DataDriver();
	~DataDriver();

	void				LoadWorkDecisionData();
	void				LoadPostWorkDecisionData();
	void				LoadFoodDecisionData();

private:
};
