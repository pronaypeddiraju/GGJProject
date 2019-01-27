#pragma once
#include "Engine/Renderer/BitmapFont.hpp"

class DecisionSequence
{
public:
	DecisionSequence();
	~DecisionSequence();

	void UpdateDS(float deltaSeconds);
	void GameOverScene() const;
	void GetNumOptions();
	void Render() const;
	void RenderInfoScreen() const;
	
	void RenderHUD() const;
	void DrawSelectionTriangle() const;
	void ShowWorkDecisions() const;
	void PrintDecisionHeader() const;
	void HandleKeyPressed( unsigned char keyCode );
	void PerformDecision();
	void PerformWorkDecision();
	void PerformPostDecision();
	void PerformFoodDecision();
	void ShowPostWorkDecisions() const;
	void ShowFoodDecisions() const;
	void GameCompleteSequence() const;

	bool CheckWinState() const;
	void CheckSleepHours();
private:
	BitmapFont*							m_printFont = nullptr;
	int									m_sequenceStep = 0;
	Rgba								m_HUDBaseBoxColor = Rgba::GREY;
	Vec2								m_spriteUVmins = Vec2(1.f, 0.f);
	Vec2								m_spriteUVmaxs = Vec2(0.f, 1.f);
	bool								m_flipSprite = false;

	float								m_pulseValue = 0.f;

	Texture*							m_workTexture = nullptr;
	Texture*							m_chillTexture = nullptr;
	Texture*							m_foodTexture = nullptr;

	AABB2								m_headerBox = AABB2(Vec2(40.f, 80.f),Vec2(160.f, 100.f));
	AABB2								m_ProceedBox = AABB2(Vec2(40.f, 20.f),Vec2(160.f, 30.f));

	//HUD Boxes
	AABB2								m_workHoursTextBox = AABB2(Vec2(80.f, 15.f), Vec2(100.f, 20.f));
	AABB2								m_workHoursBox = AABB2(Vec2(80.f, 10.f), Vec2(100.f, 15.f));

	AABB2								m_chillHoursTextBox = AABB2(Vec2(100.f, 15.f), Vec2(120.f, 20.f));
	AABB2								m_chillHoursBox = AABB2(Vec2(100.f, 10.f), Vec2(120.f, 15.f));

	AABB2								m_foodHoursTextBox = AABB2(Vec2(120.f, 15.f), Vec2(140.f, 20.f));
	AABB2								m_foodHoursBox = AABB2(Vec2(120.f, 10.f), Vec2(140.f, 15.f));

	AABB2								m_sleepHoursTextBox = AABB2(Vec2(140.f, 15.f), Vec2(160.f, 20.f));
	AABB2								m_sleepHoursBox = AABB2(Vec2(140.f, 10.f), Vec2(160.f, 15.f));

	AABB2								m_wealthTextBox = AABB2(Vec2(160.f, 15.f), Vec2(180.f, 20.f));
	AABB2								m_wealthBox = AABB2(Vec2(160.f, 10.f), Vec2(180.f, 15.f));

	AABB2								m_happinessText = AABB2(Vec2(60.f, 5.5f),Vec2(80.f, 9.f));
	AABB2								m_happinessValue = AABB2(Vec2(182.f, 5.5f),Vec2(200.f, 9.f));
	AABB2								m_happinessBarBack= AABB2(Vec2(80.f, 5.5f), Vec2(180.f, 9.f));
	AABB2								m_happinessBar = AABB2(Vec2(80.f, 5.5f), Vec2(180.f, 9.f));

	AABB2								m_guiltText = AABB2(Vec2(60.f, 1.f),Vec2(80.f, 5.5f));
	AABB2								m_guiltValue = AABB2(Vec2(182.f, 1.f),Vec2(200.f, 5.5f));
	AABB2								m_guiltBarBack= AABB2(Vec2(80.f, 1.f), Vec2(180.f, 4.5f));
	AABB2								m_guiltBar = AABB2(Vec2(80.f, 1.f), Vec2(180.f, 4.5f));

	AABB2								m_characterBox = AABB2(Vec2(20.f, 0.5f), Vec2(40.f, 19.5f));

	float								m_yBoxStart = 65.f;
	float								m_yBoxEnd = 70.f;

	int									m_selectionID = 1;
	int									m_currentOptionSet = 0;
	float								m_cellSize = 4.f;
	float								m_fontCellSize = 2.0f;

	bool								m_canProceed = false;
	bool								m_gameOver = false;

	//------------------------------------------------------------------------------------------------------------------------------
	//Game Values
	//------------------------------------------------------------------------------------------------------------------------------
	int				WEALTH = 0;
	const int		MAX_HOURS = 24;
	int				WORK_HOURS_COMPLETED = 0;
	int				POST_HOURS_COMPLETED = 0;
	int				FOOD_HOURS_COMPLETED = 0;
	int				SLEEP_HOURS_COMPLETED = 0;

	int				HAPPINESS_LEVEL = 80;
	const int		MAX_HAPPINESS = 100;
	int				GUILT_LEVEL = 0;
	const int		MAX_GUILT = 100;

};