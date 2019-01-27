#pragma once
#include <string>
#include "Engine/Core/XMLUtils/XMLUtils.hpp"
#include "Engine/Renderer/Rgba.hpp"
#include <map>

class Decision
{

public:
	explicit			Decision( XMLElement& decisionEntry);
	~Decision();

	int					GetDecisionID();

public:
	static std::map<int, Decision*>		s_workDecisions;
	static std::map<int, Decision*>		s_postWorkDecisions;
	static std::map<int, Decision*>		s_foodDecisions;
	
private:

	int									m_decisionID = 0;
	std::string							m_decisionString = "DATA ERROR! NO DECISION FOUND!";
	int									m_decisionHours = 0;
	int									m_decisionWealth = 0;
	int									m_decisionHappiness = 0;
	int									m_decisionGuilt = 0;
	Rgba								m_decisionColor = Rgba::WHITE;
};