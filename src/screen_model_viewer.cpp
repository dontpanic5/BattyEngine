#include "screen_model_viewer.h"
#include "BattyEngine.h"
#include "AnimationMgr.h"

void InitModelViewerScreen()
{
	const char* path = modelViewerPath;
	const char** pPath = &path;
	AnimationMgr::Instance().LoadMultipleModelAnimations(pPath, 1);
}

void UpdateModelViewerScreen()
{

}

void DrawModelViewerScreen()
{
}
