#pragma once
#include "Engine/Core/XMLUtils/XMLUtils.hpp"

class DataDriver
{
	DataDriver();
	~DataDriver();

	void				LoadWorkDecisionData();
	void				LoadPostWorkDecisionData();
	void				LoadFoodDecisionData();
};
