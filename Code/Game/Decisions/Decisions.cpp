#include "Game/Decisions/Decisions.hpp"

STATIC std::map<int, Decision*>		Decision::s_workDecisions;
STATIC std::map<int, Decision*>		Decision::s_postWorkDecisions;
STATIC std::map<int, Decision*>		Decision::s_foodDecisions;

Decision::Decision( XMLElement& decisionEntry )
{
	m_decisionID = ParseXmlAttribute(decisionEntry, "ID", -1);
	m_decisionString = ParseXmlAttribute(decisionEntry, "decision", m_decisionString);
	m_decisionHours = ParseXmlAttribute(decisionEntry, "hours", m_decisionHours);
	m_decisionWealth = ParseXmlAttribute(decisionEntry, "wealth", m_decisionWealth);
	m_decisionHappiness = ParseXmlAttribute(decisionEntry, "happiness", m_decisionHappiness);
    m_decisionGuilt = ParseXmlAttribute(decisionEntry, "guilt", m_decisionGuilt);
	m_decisionColor = ParseXmlAttribute(decisionEntry, "color", m_decisionColor);
}

Decision::~Decision()
{

}

int Decision::GetDecisionID()
{
	if(m_decisionID)
	{
		ERROR_AND_DIE("The decision ID was 0");
	}
	else
	{
		return m_decisionID;
	}
}

