#pragma once
#include "Engine/Commons/EngineCommon.hpp"


constexpr float WORLD_WIDTH = 200.f;
constexpr float WORLD_HEIGHT = 100.f;
constexpr float WORLD_CENTER_X = WORLD_WIDTH / 2.f;
constexpr float WORLD_CENTER_Y = WORLD_HEIGHT / 2.f;
constexpr float SCREEN_ASPECT = 16.f/9.f;

constexpr int WEALTH = 0;
constexpr int MAX_HOURS = 0;
constexpr int WORK_HOURS_COMPLETED = 0;
constexpr int POST_HOURS_COMPLETED = 0;
constexpr int FOOD_HOURS_COMPLETED = 0;
constexpr int SLEEP_HOURS_COMPLETED = 0;

constexpr int HAPPINESS_LEVEL = 80;
constexpr int MAX_HAPPINESS = 100;
constexpr int GUILT_LEVEL = 0;
constexpr int MAX_GUILT = 0;

constexpr float CAMERA_SHAKE_REDUCTION_PER_SECOND = 1.f;
constexpr float MAX_SHAKE = 2.0f;

constexpr float DEVCONSOLE_LINE_HEIGHT = 2.0f;

class RenderContext;
class InputSystem;
class AudioSystem;

extern RenderContext* g_renderContext;
extern InputSystem* g_inputSystem;
extern AudioSystem* g_audio;