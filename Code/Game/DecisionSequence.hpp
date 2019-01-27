#pragma once
#include "Engine/Renderer/BitmapFont.hpp"

class DecisionSequence
{
public:
	DecisionSequence();
	~DecisionSequence();

	void UpdateDS(float deltaSeconds);
	void Render() const;
	void RenderInfoScreen() const;
	
	void RenderHUD() const;

	void LoadNextSequence();
	void ShowWorkDecisions() const;
	void HandleKeyPressed( unsigned char keyCode );
	void ShowPostWorkDecisions() const;
	void ShowFoodDecisions() const;

private:
	BitmapFont*							m_printFont = nullptr;
	int									m_sequenceStep = 0;
	Rgba								m_HUDBaseBoxColor = Rgba::GREY;

	AABB2								m_headerBox = AABB2(Vec2(40.f, 80.f),Vec2(160.f, 100.f));
	AABB2								m_ProceedBox = AABB2(Vec2(40.f, 20.f),Vec2(160.f, 30.f));

	float								m_yBoxStart = 65.f;
	float								m_yBoxEnd = 70.f;
};