#include "Game/DecisionSequence.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Decisions/Decisions.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

RandomNumberGenerator* g_rng = nullptr;

DecisionSequence::DecisionSequence()
{
	m_printFont = g_renderContext->CreateOrGetBitmapFontFromFile("SquirrelFixedFont");
	g_rng = new RandomNumberGenerator();
}

DecisionSequence::~DecisionSequence()
{

}

void DecisionSequence::UpdateDS(float deltaSeconds)
{
	//Update decision sequence here
	if(WORK_HOURS_COMPLETED + POST_HOURS_COMPLETED + FOOD_HOURS_COMPLETED > MAX_HOURS)
	{
		//Game lost
		m_gameOver = true;
	}
}

void DecisionSequence::GameOverScene() const
{
	std::vector<Vertex_PCU> fontVerts;

	m_printFont->AddVertsForTextInBox2D(fontVerts, AABB2(Vec2(25.f, 45.f), Vec2(175.f, 55.f)), 10.f, "GAME OVER!", Rgba::RED);

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(fontVerts);
}

void DecisionSequence::GetNumOptions()
{
	switch( m_sequenceStep )
	{
	case 1:
	m_currentOptionSet = static_cast<int>(Decision::s_workDecisions.size());
	break;
	case 2:
	m_currentOptionSet = static_cast<int>(Decision::s_postWorkDecisions.size());
	break;
	case 3:
	m_currentOptionSet = static_cast<int>(Decision::s_foodDecisions.size());
	break;
	default:
	break;
	}
}

void DecisionSequence::Render() const
{
	if(m_gameOver)
	{
		GameOverScene();
		return;
	}

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
	DrawSelectionTriangle();
	break;
	case 2:
	//Render post work decisions
	ShowPostWorkDecisions();
	RenderHUD();
	DrawSelectionTriangle();
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

	//Draw the heading
	std::vector<Vertex_PCU> textVerts;
	
	//Work hours
	m_printFont->AddVertsForTextInBox2D(textVerts, m_workHoursTextBox, m_fontCellSize, "Work", Rgba::YELLOW);
	m_printFont->AddVertsForTextInBox2D(textVerts, m_workHoursBox, m_cellSize, std::to_string(WORK_HOURS_COMPLETED), Rgba::WHITE);

	//chill hours
	m_printFont->AddVertsForTextInBox2D(textVerts, m_chillHoursTextBox, m_fontCellSize, "Chill", Rgba::YELLOW);
	m_printFont->AddVertsForTextInBox2D(textVerts, m_chillHoursBox, m_cellSize, std::to_string(POST_HOURS_COMPLETED), Rgba::WHITE);

	//food hours
	m_printFont->AddVertsForTextInBox2D(textVerts, m_foodHoursTextBox, m_fontCellSize, "Food", Rgba::YELLOW);
	m_printFont->AddVertsForTextInBox2D(textVerts, m_foodHoursBox, m_cellSize, std::to_string(FOOD_HOURS_COMPLETED), Rgba::WHITE);

	//Sleep Hours
	int sleepHours = MAX_HOURS - WORK_HOURS_COMPLETED - POST_HOURS_COMPLETED - FOOD_HOURS_COMPLETED;
	m_printFont->AddVertsForTextInBox2D(textVerts, m_sleepHoursTextBox, m_fontCellSize, "Sleep", Rgba::YELLOW);
	m_printFont->AddVertsForTextInBox2D(textVerts, m_sleepHoursBox, m_cellSize, std::to_string(sleepHours), Rgba::WHITE);

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(textVerts);
	
}

void DecisionSequence::DrawSelectionTriangle() const
{
	//Draw selection triangle based on which option you have selected
	float offset = (m_yBoxEnd - m_yBoxStart) / 2.f;
	float tip = m_yBoxEnd - offset;

	std::vector<Vertex_PCU> triVecs;

	triVecs.push_back(Vertex_PCU(Vec3(15.f, (m_yBoxEnd - 1.f) - (m_selectionID * m_cellSize), 0.f), Rgba::WHITE, Vec2::ZERO));
	triVecs.push_back(Vertex_PCU(Vec3(15.f, (m_yBoxStart + 1.f) - (m_selectionID * m_cellSize), 0.f), Rgba::WHITE, Vec2::ZERO));
	triVecs.push_back(Vertex_PCU(Vec3(17.5f, tip - (m_selectionID * m_cellSize), 0.f), Rgba::WHITE, Vec2::ZERO));

	g_renderContext->BindTexture(nullptr);
	g_renderContext->DrawVertexArray(triVecs);
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
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 75.f), Vec2(110, 80.f)), 3.f, "Job options", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(110.f, 75.f), Vec2(130.f, 80.f)), 3.f, " Hours", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(130.f, 75.f), Vec2(150.f, 80.f)), 3.f, " Pay", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(150.f, 75.f), Vec2(170.f, 80.f)), 3.f, " Joy", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(170.f, 75.f), Vec2(190.f, 80.f)), 3.f, " Guilt", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);

	//Decisions
	for(int decisionIndex = 0; decisionIndex < numDecisions; decisionIndex++)
	{
		//Print decision
		std::string decisionString = Decision::s_workDecisions[decisionIndex + 1]->GetDecisionString();
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(110.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, Decision::s_workDecisions[decisionIndex + 1]->GetDecisionColor(), 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   
		
		//Print hours
		int hours = Decision::s_workDecisions[decisionIndex + 1]->m_decisionHours;
		decisionString = std::to_string(hours);
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(110.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(130.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   
		
		//Print Payout range
		IntRange pay = Decision::s_workDecisions[decisionIndex + 1]->m_decisionWealth;
		decisionString = std::to_string(pay.minInt);
		decisionString.append( "-");
		decisionString.append(std::to_string(pay.maxInt));
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(130.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(150.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   
		
		//Print Joy
		int happiness = Decision::s_workDecisions[decisionIndex + 1]->m_decisionHappiness;
		decisionString = std::to_string(happiness);
		Rgba color;
		if(happiness > 0)
		{
			color = Rgba::WHITE;
		}
		else
		{
			color = Rgba::RED;
		}
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(150.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(170.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, color, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		//Print Guilt
		int guilt = Decision::s_workDecisions[decisionIndex + 1]->m_decisionGuilt;
		decisionString = std::to_string(guilt);
		if(guilt <= 0)
		{
			color = Rgba::WHITE;
		}
		else
		{
			color = Rgba::RED;
		}
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(170.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(190.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, color, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   
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
	if(m_selectionID > 0)
	{
		m_selectionID--;
	}
	break;
	case DOWN_ARROW:
	//Down
	if(m_selectionID < m_currentOptionSet - 1)
	{
		m_selectionID++;
	}
	break;
	case ENTER_KEY:
	//Selection
	if(m_sequenceStep == 0)
	{
		m_sequenceStep++;
		LoadNextSequence();
		GetNumOptions();
	}
	else
	{
		PerformDecision();
	}
	break;
	default:
	break;
	}
}

void DecisionSequence::PerformDecision()
{
	switch( m_sequenceStep )
	{
	case 1:
	//select the option in the work decisions
	PerformWorkDecision();
	break;
	case 2:
	//select the option in the post work decisions
	break;
	case 3:
	//select the option in the food decisions
	break;
	default:
	break;
	}
}

void DecisionSequence::PerformWorkDecision()
{
	IntRange wealthRange = Decision::s_workDecisions[m_selectionID + 1]->m_decisionWealth;
	int wealth = g_rng->GetRandomIntInRange(wealthRange.minInt, wealthRange.maxInt);

	WORK_HOURS_COMPLETED += Decision::s_workDecisions[m_selectionID + 1]->m_decisionHours;
	
	if(WORK_HOURS_COMPLETED > 8)
	{
		//Allow proceed to next decision set
		m_canProceed = true;
	}
	
	WEALTH += wealth;
	HAPPINESS_LEVEL += Decision::s_workDecisions[m_selectionID + 1]->m_decisionHappiness;
	
	if(HAPPINESS_LEVEL > 100)
		HAPPINESS_LEVEL = 100;
	if(HAPPINESS_LEVEL < 0)
		HAPPINESS_LEVEL = 0;

	GUILT_LEVEL += Decision::s_workDecisions[m_selectionID + 1]->m_decisionGuilt;

	if(GUILT_LEVEL > 100)
		GUILT_LEVEL = 100;
	if(GUILT_LEVEL < 0)
		GUILT_LEVEL = 0;

	m_selectionID = 0;
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

