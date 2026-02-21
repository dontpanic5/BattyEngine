#include "screen_model_viewer.h"
#include "GameCamera.h"
#include "BattyEngine.h"
#include "AnimationMgr.h"
#include "Entity.h"
#include "Constants.h"

GameCamera modelViewerCam(true, 80.0f);
Entity* entity = nullptr;

void InitModelViewerScreen()
{
	const char* path = modelViewerPath;
	const char** pPath = &path;
	AnimationMgr::Instance().LoadMultipleModelAnimations(pPath, 1);

	entity = new Entity(modelViewerPath, modelViewerScale, true);

	modelViewerCam.CinematicWatchEntity(*entity, (float)TICK, true, { 0.0f, 30.0f, 40.0f});
	modelViewerCam.SetUp();
}

void UpdateModelViewerScreen()
{
	entity->UpdateEntity();
}

void DrawModelViewerScreen()
{
	ClearBackground({ 40,40,40,255 });
	modelViewerCam.Begin3DDrawing();

	entity->Draw();

	modelViewerCam.EndDrawing();
}
