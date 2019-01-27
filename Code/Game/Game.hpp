#pragma once
#include "Engine/Math/Vertex_PCU.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Audio/AudioSystem.hpp"

class Texture;
class BitmapFont;
class SpriteAnimDefenition;
class Image;
class DecisionSequence;
class DataDriver;

class Game
{
public:
	Game();
	~Game();
	
	static bool TestEvent(EventArgs& args);
	
	void StartUp();
	void HandleKeyPressed( unsigned char keyCode );
	void DebugEnabled();

	void HandleKeyReleased( unsigned char keyCode );
	void Render() const;
	void PostRender();

	void SplashSequence() const;


	//Debug renders
	void DebugRenderTextures() const;
	void DebugRenderSprites() const;
	void DebugRenderSpriteAnims() const;
	void DebugRenderTextAlignment() const;
	void DebugRender() const;
	
	void Update( float deltaTime );
	void UpdateCamera( float deltaTime );
	void ClearGarbageEntities();
	void CheckXboxInputs();
	void CheckCollisions();

	bool IsAlive();
private:
	bool m_isGameAlive = false;
	bool m_consoleDebugOnce = false;

	DecisionSequence* m_decisionSequence = nullptr;
	DataDriver* m_dataDriver = nullptr;

public:
	BitmapFont* m_squirrelFont = nullptr;
	float m_animTime = 0.f;

	bool m_splashEnabled = true;
	bool m_gameCompletedState = false;
	bool m_gameInitiated = false;

};
