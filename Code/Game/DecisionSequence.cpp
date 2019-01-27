#include "Game/DecisionSequence.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Game/GameCommon.hpp"

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

}

void DecisionSequence::Render() const
{
	std::vector<Vertex_PCU> fontVerts;
	m_printFont->AddVertsForTextInBox2D(fontVerts, AABB2(Vec2(25.f, 45.f), Vec2(175.f, 55.f)), 10.f, "LET THE GAMES BEGIN!", Rgba::WHITE);

	g_renderContext->BindTexture(m_printFont->GetTexture());
	g_renderContext->DrawVertexArray(fontVerts);
}




