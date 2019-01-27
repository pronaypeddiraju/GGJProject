#include "Game/DataDriver/DataDriver.hpp"
#include "Game/Decisions/Decisions.hpp"

DataDriver::DataDriver()
{

}

DataDriver::~DataDriver()
{

}

void DataDriver::LoadWorkDecisionData()
{
	const char* xmlDocPath = "Data/Gameplay/WorkDecisions.xml";
	tinyxml2::XMLDocument decisionsDoc;
	decisionsDoc.LoadFile(xmlDocPath);

	if(decisionsDoc.ErrorID() != tinyxml2::XML_SUCCESS)
	{
		printf("\n >> Error loading XML file from %s ", xmlDocPath);
		printf("\n >> Error ID : %i ", decisionsDoc.ErrorID());
		printf("\n >> Error line number is : %i", decisionsDoc.ErrorLineNum());
		printf("\n >> Error name : %s", decisionsDoc.ErrorName());
		ERROR_AND_DIE(">> Error loading GameConfig XML file ")
			return;
	}
	else
	{
		//We read everything fine. Now just shove all that data into the black board
		XMLElement* rootElement = decisionsDoc.RootElement();
		
		//g_gameConfigBlackboard.PopulateFromXmlElementAttributes(*rootElement);
		for(rootElement = rootElement->FirstChildElement(); rootElement; rootElement = rootElement->NextSiblingElement())
		{
			Decision* decision = new Decision(*rootElement);

			int decisionID = decision->GetDecisionID();
			Decision::s_workDecisions[decisionID] = decision;
		}


	}
}

void DataDriver::LoadPostWorkDecisionData()
{
	const char* xmlDocPath = "Data/Gameplay/PostWorkDecisions.xml";
	tinyxml2::XMLDocument decisionsDoc;
	decisionsDoc.LoadFile(xmlDocPath);

	if(decisionsDoc.ErrorID() != tinyxml2::XML_SUCCESS)
	{
		printf("\n >> Error loading XML file from %s ", xmlDocPath);
		printf("\n >> Error ID : %i ", decisionsDoc.ErrorID());
		printf("\n >> Error line number is : %i", decisionsDoc.ErrorLineNum());
		printf("\n >> Error name : %s", decisionsDoc.ErrorName());
		ERROR_AND_DIE(">> Error loading GameConfig XML file ")
			return;
	}
	else
	{
		//We read everything fine. Now just shove all that data into the black board
		XMLElement* rootElement = decisionsDoc.RootElement();

		//g_gameConfigBlackboard.PopulateFromXmlElementAttributes(*rootElement);
		for(rootElement = rootElement->FirstChildElement(); rootElement; rootElement = rootElement->NextSiblingElement())
		{
			Decision* decision = new Decision(*rootElement);

			int decisionID = decision->GetDecisionID();
			Decision::s_postWorkDecisions[decisionID] = decision;
		}


	}
}

void DataDriver::LoadFoodDecisionData()
{
	const char* xmlDocPath = "Data/Gameplay/FoodDecisions.xml";
	tinyxml2::XMLDocument decisionsDoc;
	decisionsDoc.LoadFile(xmlDocPath);

	if(decisionsDoc.ErrorID() != tinyxml2::XML_SUCCESS)
	{
		printf("\n >> Error loading XML file from %s ", xmlDocPath);
		printf("\n >> Error ID : %i ", decisionsDoc.ErrorID());
		printf("\n >> Error line number is : %i", decisionsDoc.ErrorLineNum());
		printf("\n >> Error name : %s", decisionsDoc.ErrorName());
		ERROR_AND_DIE(">> Error loading GameConfig XML file ")
			return;
	}
	else
	{
		//We read everything fine. Now just shove all that data into the black board
		XMLElement* rootElement = decisionsDoc.RootElement();

		//g_gameConfigBlackboard.PopulateFromXmlElementAttributes(*rootElement);
		for(rootElement = rootElement->FirstChildElement(); rootElement; rootElement = rootElement->NextSiblingElement())
		{
			Decision* decision = new Decision(*rootElement);

			int decisionID = decision->GetDecisionID();
			Decision::s_foodDecisions[decisionID] = decision;
		}


	}
}

