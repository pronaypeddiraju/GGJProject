#pragma once
#include <string>
#include "Engine/Core/XMLUtils/XMLUtils.hpp"
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Math/IntRange.hpp"
#include <map>

class Decision
{

public:
	explicit			Decision( XMLElement& decisionEntry);
	~Decision();

	int						GetDecisionID();
	const std::string&		GetDecisionString() const;
	const Rgba&				GetDecisionColor() const;

public:
	static std::map<int, Decision*>		s_workDecisions;
	static std::map<int, Decision*>		s_postWorkDecisions;
	static std::map<int, Decision*>		s_foodDecisions;

	int									m_decisionID = 0;
	std::string							m_decisionString = "DATA ERROR! NO DECISION FOUND!";
	int									m_decisionHours = 0;
	IntRange						    m_decisionWealth = IntRange::Zero;
	int									m_decisionHappiness = 0;
	int									m_decisionGuilt = 0;
	Rgba								m_decisionColor = Rgba::WHITE;
};