#include "Game/DecisionSequence.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Decisions/Decisions.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/Time.hpp"

RandomNumberGenerator* g_rng = nullptr;

DecisionSequence::DecisionSequence()
{
	m_printFont = g_renderContext->CreateOrGetBitmapFontFromFile("SquirrelFixedFont");
	g_rng = new RandomNumberGenerator();

	m_workTexture = g_renderContext->CreateOrGetTextureFromFile("Data/Images/WorkCostume.png");
	m_chillTexture = g_renderContext->CreateOrGetTextureFromFile("Data/Images/ChillCostume.png");
	m_foodTexture = g_renderContext->CreateOrGetTextureFromFile("Data/Images/FoodCostume.png");
}

DecisionSequence::~DecisionSequence()
{

}

void DecisionSequence::UpdateDS(float deltaSeconds)
{
	UNUSED(deltaSeconds);
	float time = static_cast<float>(GetCurrentTimeSeconds());
	m_pulseValue = (sin(time) + 1.f) * 0.5f;

	//Update decision sequence here
	if(WORK_HOURS_COMPLETED + POST_HOURS_COMPLETED + FOOD_HOURS_COMPLETED > MAX_HOURS)
	{
		//Game lost
		m_gameOver = true;
	}

	if(WORK_HOURS_COMPLETED > 8 && m_sequenceStep == 1)
	{
		m_canProceed = true;
	}
}

void DecisionSequence::GameOverScene() const
{
	std::vector<Vertex_PCU> fontVerts;

	m_printFont->AddVertsForTextInBox2D(fontVerts, AABB2(Vec2(25.f, 45.f), Vec2(175.f, 55.f)), 10.f, "GAME OVER!", Rgba::RED, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, m_pulseValue * 2.f);

	m_printFont->AddVertsForTextInBox2D(fontVerts, AABB2(Vec2(20.f, 30.f), Vec2(180.f, 35.f)), m_fontCellSize, "You exhausted your 24 hours without any sleep! You're F@*king crazy dude!", Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   
	m_printFont->AddVertsForTextInBox2D(fontVerts, AABB2(Vec2(20.f, 25.f), Vec2(180.f, 30.f)), m_fontCellSize, "Press F8 to RESTART the game", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

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
	ShowFoodDecisions();
	RenderHUD();
	DrawSelectionTriangle();
	break;
	case 4:
	//Completed Screen
	GameCompleteSequence();
	break;
	default:
	break;
	}
}

void DecisionSequence::RenderInfoScreen() const
{
	//Pre-Game start info screen to display lore and info about decisions
	std::vector<Vertex_PCU>	textVerts;
	
	m_printFont->AddVertsForTextInBox2D(textVerts, m_headerBox, 3.f, "24 Hours in Manali", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED);

	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 80.f) , Vec2(180.f, 85.f)), m_fontCellSize, "Lore:", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 75.f) , Vec2(200.f, 78.f)), 2.f, "You live in the town of Manali in India. A place so beautiful, words can't do justice", Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 72.f) , Vec2(200.f, 75.f)), 2.f, "But just like any other town, you can do a lot of things here. Legal and Illegal", Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 69.f) , Vec2(200.f, 72.f)), 2.f, "You are to live 24 hours in this town and perform 4 activities: work, chill, eat and sleep", Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 66.f) , Vec2(200.f, 69.f)), 2.f, "Each activity takes up time and rewards you with cash and joy if legal", Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 63.f) , Vec2(200.f, 66.f)), 2.f, "Illegal activities marked in RED gives you more cash and more joy but builds up guilt", Rgba::RED, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 60.f) , Vec2(200.f, 63.f)), 2.f, "Choose wisely among your options and spend your 24 hours in Manali to finish the game", Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);

	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 50.f) , Vec2(180.f, 55.f)), m_fontCellSize, "What to consider while making decisions:", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 47.f) , Vec2(200.f, 50.f)), 2.f, "Legal decisions are marked in White (Low payout and joy but no guilt)", Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 45.f) , Vec2(200.f, 47.f)), 2.f, "Illegal decisions are marked in Red (High payout and joy but also guilt)", Rgba::RED, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 42.f) , Vec2(200.f, 45.f)), 2.f, "6 or more hours of sleep add 20 Joy and reduce 20 Guilt", Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 39.f) , Vec2(200.f, 42.f)), 2.f, "Less than 6 hours of sleep add 20 Guilt and reduce 20 Joy", Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);

	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 30.f) , Vec2(180.f, 35.f)), m_fontCellSize, "How to WIN:", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 27.f) , Vec2(200.f, 30.f)), 2.f, "End your 24 hours with $50 or more, 80 or more Joy and 20 or less Guilt", Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(10.f, 25.f) , Vec2(200.f, 27.f)), 2.f, "Make your decisions wisely", Rgba::GREEN, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, 1.f);

	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 10.f) , Vec2(180.f, 20.f)), m_fontCellSize, "(Press Enter to Start!)", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 999999, m_pulseValue * 2.f);

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
	
	m_printFont->AddVertsForTextInBox2D(textVerts, m_restartBox, m_fontCellSize, "RESTART", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED);
	m_printFont->AddVertsForTextInBox2D(textVerts, m_restartKeyBox, m_fontCellSize, "F8", Rgba::YELLOW, 1.f, Vec2::ALIGN_TOP_CENTERED);

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

	//Wealth 
	m_printFont->AddVertsForTextInBox2D(textVerts, m_wealthTextBox, m_fontCellSize, "Money", Rgba::YELLOW);
	m_printFont->AddVertsForTextInBox2D(textVerts, m_wealthBox, m_cellSize, std::to_string(WEALTH), Rgba::GREEN);

	//Happiness Bar
	m_printFont->AddVertsForTextInBox2D(textVerts, m_happinessText, m_fontCellSize, "Joy", Rgba::YELLOW);
	m_printFont->AddVertsForTextInBox2D(textVerts, m_happinessValue, m_cellSize, std::to_string(HAPPINESS_LEVEL), Rgba::ORANGE);

	std::vector<Vertex_PCU> barVerts;
	AddVertsForAABB2D(barVerts, m_happinessBarBack, Rgba::DARK_GREY);

	float barWidth = m_happinessBarBack.m_maxBounds.x - m_happinessBarBack.m_minBounds.x;
	float actualBarMax = (static_cast<float>(HAPPINESS_LEVEL) / static_cast<float>(MAX_HAPPINESS)) * barWidth;

	AABB2 happyBar = m_happinessBar;
	happyBar.m_maxBounds.x = happyBar.m_minBounds.x + actualBarMax;
	AddVertsForAABB2D(barVerts, happyBar, Rgba::ORANGE);

	//Guilt bar
	m_printFont->AddVertsForTextInBox2D(textVerts, m_guiltText, m_fontCellSize, "Guilt", Rgba::YELLOW);
	m_printFont->AddVertsForTextInBox2D(textVerts, m_guiltValue, m_cellSize, std::to_string(GUILT_LEVEL), Rgba::RED);

	AddVertsForAABB2D(barVerts, m_guiltBarBack, Rgba::DARK_GREY);

	barWidth = m_guiltBarBack.m_maxBounds.x - m_guiltBarBack.m_minBounds.x;
	actualBarMax = (static_cast<float>(GUILT_LEVEL) / static_cast<float>(MAX_GUILT)) * barWidth;

	AABB2 guiltBar = m_guiltBar;
	guiltBar.m_maxBounds.x = guiltBar.m_minBounds.x + actualBarMax;
	AddVertsForAABB2D(barVerts, guiltBar, Rgba::RED);
	g_renderContext->DrawVertexArray(barVerts);

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(textVerts);

	std::vector<Vertex_PCU> textureVerts;
	switch (m_sequenceStep)
	{
	case 1:
	//Work
	g_renderContext->BindTexture(m_workTexture);
	break;
	case 2:
	//Chill
	g_renderContext->BindTexture(m_chillTexture);
	break;
	case 3:
	//Food
	g_renderContext->BindTexture(m_foodTexture);
	break;
	case 4:
	//Completed
	g_renderContext->BindTexture(m_chillTexture);
	break;
	}

	if(m_flipSprite)
	{
		AddVertsForAABB2D(textureVerts, m_characterBox, Rgba::WHITE, m_spriteUVmins, m_spriteUVmaxs);
	}
	else
	{
		AddVertsForAABB2D(textureVerts, m_characterBox, Rgba::WHITE);
	}

	g_renderContext->DrawVertexArray(textureVerts);
	
}

void DecisionSequence::DrawSelectionTriangle() const
{
	//Draw selection triangle based on which option you have selected
	float offset = (m_yBoxEnd - m_yBoxStart) / 2.f;
	float tip = m_yBoxEnd - offset;

	std::vector<Vertex_PCU> triVecs;

	if(m_selectionID != m_currentOptionSet - 1)
	{
		triVecs.push_back(Vertex_PCU(Vec3(15.f, (m_yBoxEnd - 1.f) - (m_selectionID * m_cellSize), 0.f), Rgba::WHITE, Vec2::ZERO));
		triVecs.push_back(Vertex_PCU(Vec3(15.f, (m_yBoxStart + 1.f) - (m_selectionID * m_cellSize), 0.f), Rgba::WHITE, Vec2::ZERO));
		triVecs.push_back(Vertex_PCU(Vec3(17.5f, tip - (m_selectionID * m_cellSize), 0.f), Rgba::WHITE, Vec2::ZERO));
	}
	else
	{
		triVecs.push_back(Vertex_PCU(Vec3(15.f, 26.f, 0.f), Rgba::WHITE, Vec2::ZERO));
		triVecs.push_back(Vertex_PCU(Vec3(15.f, 29.f, 0.f), Rgba::WHITE, Vec2::ZERO));
		triVecs.push_back(Vertex_PCU(Vec3(17.5f, 27.5f, 0.f), Rgba::WHITE, Vec2::ZERO));
	}
	
	g_renderContext->BindTexture(nullptr);
	g_renderContext->DrawVertexArray(triVecs);
}

void DecisionSequence::ShowWorkDecisions() const
{
	int numDecisions = static_cast<int>(Decision::s_workDecisions.size());
	
	std::vector<Vertex_PCU> textVerts;

	//Header
	m_printFont->AddVertsForTextInBox2D(textVerts, m_headerBox, 3.f, "TIME TO WORK", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED);
	PrintDecisionHeader();
	
	//Decisions
	for(int decisionIndex = 0; decisionIndex < numDecisions - 1; decisionIndex++)
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
		if(pay.minInt != pay.maxInt)
		{
			decisionString.append( " to ");
			decisionString.append(std::to_string(pay.maxInt));
		}
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

	if(m_canProceed)
	{
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 25.f), Vec2(110, 30.f)), m_fontCellSize, "Proceed to next decision set", Rgba::GREEN, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	}

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(textVerts);
}

void DecisionSequence::PrintDecisionHeader() const
{
	std::vector<Vertex_PCU> textVerts;

	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 75.f), Vec2(110, 80.f)), 3.f, "Job options", Rgba::YELLOW, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(110.f, 75.f), Vec2(130.f, 80.f)), 3.f, " Hours", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(130.f, 75.f), Vec2(150.f, 80.f)), 3.f, " Pay", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(150.f, 75.f), Vec2(170.f, 80.f)), 3.f, " Joy", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(170.f, 75.f), Vec2(190.f, 80.f)), 3.f, " Guilt", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);

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
	if(!m_canProceed)
	{
		if(m_selectionID < m_currentOptionSet - 2)
		{
			m_selectionID++;
		}
	}
	else
	{
		if(m_selectionID < m_currentOptionSet - 1)
		{
			m_selectionID++;
		}
	}
	/*
	if(m_selectionID == m_currentOptionSet - 2 && m_canProceed)
	{
		m_selectionID = m_currentOptionSet - 1;
		//DrawProceedTriangle();
	}
	*/

	break;
	case ENTER_KEY:
	//Selection
	if(m_sequenceStep == 0 )
	{
		m_sequenceStep++;
		GetNumOptions();
	}
	else if(m_selectionID == m_currentOptionSet - 1)
	{
		m_selectionID = 0;
		m_sequenceStep++;
		GetNumOptions();
		m_canProceed = false;
		if(m_sequenceStep == 4)
		{
			CheckSleepHours();
		}
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
	PerformPostDecision();
	break;
	case 3:
	//select the option in the food decisions
	PerformFoodDecision();
	break;
	case 4:
	return;
	break;
	default:
	break;
	}

	m_flipSprite = !m_flipSprite;
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

void DecisionSequence::PerformPostDecision()
{
	IntRange wealthRange = Decision::s_postWorkDecisions[m_selectionID + 1]->m_decisionWealth;
	
	if(wealthRange.minInt < 0 && wealthRange.maxInt > 0)
	{
		//-ve to +ve wealth
		wealthRange.maxInt = wealthRange.maxInt - wealthRange.minInt;
		wealthRange.minInt *= -1;

		int wealth = g_rng->GetRandomIntInRange(wealthRange.minInt, wealthRange.maxInt);
		WEALTH -= wealth;
	}
	else
	{
		//Normal range handling
		wealthRange.minInt *= -1;
		wealthRange.maxInt *= -1;

		int wealth = g_rng->GetRandomIntInRange(wealthRange.minInt, wealthRange.maxInt);
		WEALTH -= wealth;
	}

	POST_HOURS_COMPLETED += Decision::s_postWorkDecisions[m_selectionID + 1]->m_decisionHours;
	HAPPINESS_LEVEL += Decision::s_postWorkDecisions[m_selectionID + 1]->m_decisionHappiness;

	if(HAPPINESS_LEVEL > 100)
		HAPPINESS_LEVEL = 100;
	if(HAPPINESS_LEVEL < 0)
		HAPPINESS_LEVEL = 0;

	GUILT_LEVEL += Decision::s_postWorkDecisions[m_selectionID + 1]->m_decisionGuilt;

	if(GUILT_LEVEL > 100)
		GUILT_LEVEL = 100;
	if(GUILT_LEVEL < 0)
		GUILT_LEVEL = 0;

	m_selectionID = 0;
	m_canProceed = true;
}

void DecisionSequence::PerformFoodDecision()
{
	IntRange wealthRange = Decision::s_foodDecisions[m_selectionID + 1]->m_decisionWealth;

	wealthRange.minInt *= -1;
	wealthRange.maxInt *= -1;

	int wealth = g_rng->GetRandomIntInRange(wealthRange.minInt, wealthRange.maxInt);

	FOOD_HOURS_COMPLETED += Decision::s_foodDecisions[m_selectionID + 1]->m_decisionHours;

	WEALTH -= wealth;
	HAPPINESS_LEVEL += Decision::s_foodDecisions[m_selectionID + 1]->m_decisionHappiness;

	if(HAPPINESS_LEVEL > 100)
		HAPPINESS_LEVEL = 100;
	if(HAPPINESS_LEVEL < 0)
		HAPPINESS_LEVEL = 0;

	GUILT_LEVEL += Decision::s_foodDecisions[m_selectionID + 1]->m_decisionGuilt;

	if(GUILT_LEVEL > 100)
		GUILT_LEVEL = 100;
	if(GUILT_LEVEL < 0)
		GUILT_LEVEL = 0;

	m_selectionID = 0;
	m_canProceed = true;
}

void DecisionSequence::ShowPostWorkDecisions() const
{
	int numDecisions = static_cast<int>(Decision::s_postWorkDecisions.size());

	std::vector<Vertex_PCU> textVerts;

	//Header
	m_printFont->AddVertsForTextInBox2D(textVerts, m_headerBox, 3.f, "TIME TO CHILL", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED);
	PrintDecisionHeader();

	//Decisions
	for(int decisionIndex = 0; decisionIndex < numDecisions - 1; decisionIndex++)
	{
		//Print decision
		std::string decisionString = Decision::s_postWorkDecisions[decisionIndex + 1]->GetDecisionString();
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(110.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, Decision::s_postWorkDecisions[decisionIndex + 1]->GetDecisionColor(), 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		//Print hours
		int hours = Decision::s_postWorkDecisions[decisionIndex + 1]->m_decisionHours;
		decisionString = std::to_string(hours);
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(110.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(130.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		//Print Payout range
		IntRange pay = Decision::s_postWorkDecisions[decisionIndex + 1]->m_decisionWealth;
		decisionString = std::to_string(pay.minInt);
		if(pay.minInt != pay.maxInt)
		{
			decisionString.append( " to ");
			decisionString.append(std::to_string(pay.maxInt));

		}
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(130.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(150.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		//Print Joy
		int happiness = Decision::s_postWorkDecisions[decisionIndex + 1]->m_decisionHappiness;
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
		int guilt = Decision::s_postWorkDecisions[decisionIndex + 1]->m_decisionGuilt;
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

	if(m_canProceed)
	{
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 25.f), Vec2(110, 30.f)), m_fontCellSize, Decision::s_postWorkDecisions[numDecisions]->m_decisionString, Rgba::GREEN, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	}

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(textVerts);
}

void DecisionSequence::ShowFoodDecisions() const
{
	int numDecisions = static_cast<int>(Decision::s_foodDecisions.size());

	std::vector<Vertex_PCU> textVerts;

	//Header
	m_printFont->AddVertsForTextInBox2D(textVerts, m_headerBox, 3.f, "GRAB SOME GRUB", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED);
	PrintDecisionHeader();

	//Decisions
	for(int decisionIndex = 0; decisionIndex < numDecisions - 1; decisionIndex++)
	{
		//Print decision
		std::string decisionString = Decision::s_foodDecisions[decisionIndex + 1]->GetDecisionString();
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(110.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, Decision::s_foodDecisions[decisionIndex + 1]->GetDecisionColor(), 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		//Print hours
		int hours = Decision::s_foodDecisions[decisionIndex + 1]->m_decisionHours;
		decisionString = std::to_string(hours);
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(110.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(130.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		//Print Payout range
		IntRange pay = Decision::s_foodDecisions[decisionIndex + 1]->m_decisionWealth;
		decisionString = std::to_string(pay.minInt);
		if(pay.minInt != pay.maxInt)
		{
			decisionString.append( " to ");
			decisionString.append(std::to_string(pay.maxInt));

		}
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(130.f, m_yBoxStart - decisionIndex * m_cellSize), Vec2(150.f, m_yBoxEnd - decisionIndex * m_cellSize)), m_fontCellSize, decisionString, Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		//Print Joy
		int happiness = Decision::s_foodDecisions[decisionIndex + 1]->m_decisionHappiness;
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
		int guilt = Decision::s_foodDecisions[decisionIndex + 1]->m_decisionGuilt;
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

	if(m_canProceed)
	{
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 25.f), Vec2(110, 30.f)), m_fontCellSize, Decision::s_foodDecisions[numDecisions]->m_decisionString, Rgba::GREEN, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);
	}

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(textVerts);
}

void DecisionSequence::GameCompleteSequence() const
{
	//Display HUD in the bottom of the screen as usual
	RenderHUD();

	std::vector<Vertex_PCU> textVerts;

	//Header
	m_printFont->AddVertsForTextInBox2D(textVerts, m_headerBox, 3.f, "THE DAY HAS ENDED", Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED);

	std::string decisionString;

	if(MAX_HOURS - WORK_HOURS_COMPLETED - POST_HOURS_COMPLETED - FOOD_HOURS_COMPLETED >= 6)
	{
		//WE had 6+ hours of sleep
		decisionString = "You had 6 or more hours of sleep. You gained 20 Joy and lost 20 Guilt";
	}
	else
	{
		//Sleep was short
		decisionString = "You less than 6 hours of sleep. You lost 20 Joy and gained 20 Guilt";
	}
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart), Vec2(180.f, m_yBoxEnd)), m_fontCellSize, decisionString, Rgba::WHITE, 1.f, Vec2::ALIGN_LEFT_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

	bool winState = CheckWinState();
	if(winState)
	{
		//You won the game!
		decisionString = "You completed the day with the following conditions met:";
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart - 5.f), Vec2(180.f, m_yBoxEnd - 5.f)), m_fontCellSize, decisionString, Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   
		
		decisionString = "More than $50 in Cash, More than 80 Happiness and less than 20 Guilt";
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart - 10.f), Vec2(180.f, m_yBoxEnd - 10.f)), m_fontCellSize, decisionString, Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		decisionString = "You WIN the game! 24 hours in Manali well spent!";
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart - 25.f), Vec2(180.f, m_yBoxEnd - 25.f)), m_fontCellSize * 2, decisionString, Rgba::GREEN, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, m_pulseValue * 2.0f);   
	}
	else
	{
		//Lost the game
		//You won the game!
		decisionString = "You completed the day with the following conditions failed:";
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart - 5.f), Vec2(180.f, m_yBoxEnd - 5.f)), m_fontCellSize, decisionString, Rgba::RED, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		decisionString = "More than $50 in Cash, More than 80 Happiness and less than 20 Guilt";
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart - 10.f), Vec2(180.f, m_yBoxEnd - 10.f)), m_fontCellSize, decisionString, Rgba::WHITE, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

		decisionString = "You lost the game! 24 hours in Manali spent poorly!";
		m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, m_yBoxStart - 25.f), Vec2(180.f, m_yBoxEnd - 20.f)), m_fontCellSize * 2, decisionString, Rgba::RED, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, m_pulseValue * 2.0f);   
	}

	decisionString = "Press F8 to RESTART the game";
	m_printFont->AddVertsForTextInBox2D(textVerts, AABB2(Vec2(20.f, 25.f), Vec2(180.f, 30.f)), m_fontCellSize, decisionString, Rgba::YELLOW, 1.f, Vec2::ALIGN_CENTERED, TEXT_BOX_MODE_SHRINK, 9999999, 1.f);   

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(textVerts);

	RenderHUD();
}

bool DecisionSequence::CheckWinState() const
{
	if(WEALTH >= 50 && GUILT_LEVEL <= 20 && HAPPINESS_LEVEL >= 80)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void DecisionSequence::CheckSleepHours()
{
	//based on game state, display win message
	//if 6 hours sleep, reduce guilt by 20?
	int sleepHours = MAX_HOURS - WORK_HOURS_COMPLETED - POST_HOURS_COMPLETED - FOOD_HOURS_COMPLETED;
	if(sleepHours >= 6)
	{
		HAPPINESS_LEVEL += 20;
		GUILT_LEVEL -= 20;
	}
	else
	{
		HAPPINESS_LEVEL -= 20;
		GUILT_LEVEL -= 20;
	}

	if(GUILT_LEVEL < 0)
	{
		GUILT_LEVEL = 0;
	}
	else if(GUILT_LEVEL > 100)
	{
		GUILT_LEVEL = 100;
	}

	if(HAPPINESS_LEVEL > 100)
	{
		HAPPINESS_LEVEL = 100;
	}
	else if(HAPPINESS_LEVEL < 0)
	{
		HAPPINESS_LEVEL = 0;
	}
}