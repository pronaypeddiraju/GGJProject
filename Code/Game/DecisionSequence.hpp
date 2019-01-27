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
	void LoadNextSequence();
	void ShowWorkDecisions() const;
	void HandleKeyPressed( unsigned char keyCode );
	void PerformDecision();
	void PerformWorkDecision();
	void ShowPostWorkDecisions() const;
	void ShowFoodDecisions() const;

private:
	BitmapFont*							m_printFont = nullptr;
	int									m_sequenceStep = 0;
	Rgba								m_HUDBaseBoxColor = Rgba::GREY;

	AABB2								m_headerBox = AABB2(Vec2(40.f, 80.f),Vec2(160.f, 100.f));
	AABB2								m_ProceedBox = AABB2(Vec2(40.f, 20.f),Vec2(160.f, 30.f));

	//HUD Boxes
	AABB2								m_workHoursTextBox = AABB2(Vec2(40.f, 15.f), Vec2(60.f, 20.f));
	AABB2								m_workHoursBox = AABB2(Vec2(40.f, 10.f), Vec2(60.f, 15.f));

	AABB2								m_chillHoursTextBox = AABB2(Vec2(60.f, 15.f), Vec2(80.f, 20.f));
	AABB2								m_chillHoursBox = AABB2(Vec2(60.f, 10.f), Vec2(80.f, 15.f));

	AABB2								m_foodHoursTextBox = AABB2(Vec2(80.f, 15.f), Vec2(100.f, 20.f));
	AABB2								m_foodHoursBox = AABB2(Vec2(80.f, 10.f), Vec2(100.f, 15.f));

	AABB2								m_sleepHoursTextBox = AABB2(Vec2(100.f, 15.f), Vec2(120.f, 20.f));
	AABB2								m_sleepHoursBox = AABB2(Vec2(100.f, 10.f), Vec2(120.f, 15.f));


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