#pragma once
#include "Engine/Renderer/BitmapFont.hpp"

class DecisionSequence
{
public:
	DecisionSequence();
	~DecisionSequence();

	void UpdateDS(float deltaSeconds);
	void Render() const;

private:
	BitmapFont* m_printFont = nullptr;
};