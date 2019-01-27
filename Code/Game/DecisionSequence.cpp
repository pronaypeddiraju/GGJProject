#include "Game/DecisionSequence.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Decisions/Decisions.hpp"
#include "Engine/Core/VertexUtils.hpp"

DecisionSequence::DecisionSequence()
{
	m_printFont = g_renderContext->CreateOrGetBitmapFontFromFile("SquirrelFixedFont");
}

DecisionSequence::~DecisionSequence()
{

}

void DecisionSequence::UpdateDS(float deltaSeconds)
{
	//Update decision sequence here

	//Get the user's input and move the decision pointer accordingly
}

void DecisionSequence::Render() const
{
	//Render screens based on game state
	switch( m_sequenceStep )
	{
	case 0:
	//Show the info screen
	RenderInfoScreen();
	break;
	case 1:
	//Render the work decision
	ShowWorkDecisions();
	RenderHUD();
	break;
	case 2:
	//Render post work decisions
	ShowPostWorkDecisions();
	RenderHUD();
	break;
	case 3:
	//Render food decisions
	break;
	default:
	break;
	}
}

void DecisionSequence::RenderInfoScreen() const
{
	//Pre-Game start info screen to display lore and info about decisions
	std::vector<Vertex_PCU>	textVerts;
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(25.f,45.f), Vec2(175.f, 55.f)), 2.f, "Info Screen", Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);


	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(textVerts);
}

void DecisionSequence::RenderHUD() const
{
	//Render the hud for the user 

	//Draw the first base box here
	std::vector<Vertex_PCU>	boxVerts;
	AABB2 HudBase = AABB2(Vec2(0.f, 0.f), Vec2(200.f, 20.f));
	AddVertsForAABB2D(boxVerts, HudBase, m_HUDBaseBoxColor);

	g_renderContext->BindTexture(nullptr);
	g_renderContext->DrawVertexArray(boxVerts);
}

void DecisionSequence::LoadNextSequence()
{
	switch( m_sequenceStep )
	{
	case 1:
	//Load the decisions for work
	break;
	default:
	break;
	}
}

void DecisionSequence::ShowWorkDecisions() const
{
	int numDecisions = static_cast<int>(Decision::s_workDecisions.size());

	std::vector<Vertex_PCU> textVerts;

	//Header
	m_printFont->AddVertsForTextInBox2D(textVerts, m_headerBox, 3.f, "TIME TO WORK", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 75.f), Vec2(90, 80.f)), 3.f, "Job options", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(90.f, 75.f), Vec2(120.f, 80.f)), 3.f, " Hours", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(120.f, 75.f), Vec2(140.f, 80.f)), 3.f, " Pay", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(140.f, 75.f), Vec2(170.f, 80.f)), 3.f, " Joy", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(170.f, 75.f), Vec2(200.f, 80.f)), 3.f, " Guilt", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);

	//Decisions
	for(int decisionIndex = 0; decisionIndex < numDecisions; decisionIndex++)
	{
		std::string decisionString = Decision::s_workDecisions[decisionIndex + 1]->GetDecisionString();
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, m_yBoxStart - decisionIndex * 4.f), Vec2(90.f, m_yBoxEnd - decisionIndex * 4.f)), 2.f, decisionString, Decision::s_workDecisions[decisionIndex + 1]->GetDecisionColor(), 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   
		decisionString = Decision::s_workDecisions[decisionIndex + 1]->m_decisionHours;
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(90.f, m_yBoxStart - decisionIndex * 4.f), Vec2(120.f, m_yBoxEnd - decisionIndex * 4.f)), 2.f, decisionString, Decision::s_workDecisions[decisionIndex + 1]->GetDecisionColor(), 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   
	}

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(textVerts);
}

void DecisionSequence::HandleKeyPressed( unsigned char keyCode )
{
	switch( keyCode )
	{
	case UP_ARROW:
	//Up arrow pressed
	break;
	case DOWN_ARROW:
	//Down
	break;
	case ENTER_KEY:
	//Selection
	if(m_sequenceStep == 0)
	{
		m_sequenceStep++;
		LoadNextSequence();
	}
	break;
	default:
	break;
	}
}

void DecisionSequence::ShowPostWorkDecisions() const
{
	int numDecisions = static_cast<int>(Decision::s_workDecisions.size());

	std::vector<Vertex_PCU> textVerts;

	//Header
	m_printFont->AddVertsForTextInBox2D(textVerts, m_headerBox, 3.f, "PICK YOUR POISON", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED);

	//Decisions
	for(int decisionIndex = 0; decisionIndex < numDecisions; decisionIndex++)
	{
		std::string decisionString = Decision::s_postWorkDecisions[decisionIndex + 1]->GetDecisionString();
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(40.f, 75.f - decisionIndex * 5.f), Vec2(160.f, 80.f - decisionIndex * 5.f)), 3.f, decisionString, Decision::s_postWorkDecisions[decisionIndex + 1]->GetDecisionColor(), 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   
	}

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(textVerts);
}

