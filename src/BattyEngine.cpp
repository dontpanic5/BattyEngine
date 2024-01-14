static const int screenWidth = 980;
static const int screenHeight = 490;

#include <cstdio>
#include "raylib.h"
#include <math.h>
#include "BattyEngine.h"
#include "Constants.h"
#include "Utils.h"
#include "Ui.h"
#include "AnimationMgr.h"
#include "LevelMgr.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

static double frameTime;
static double previousFrameTime;
static double gameTime;
Font defaultFont;

static LogicCbType s_logicCb;
static DrawCbType s_drawCb;

static void startLoop();
static void UpdateLogic();
static void UpdateDrawFrame();


void Init(const char* name)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, name);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_WINDOW_MAXIMIZED);

    InitAudioDevice();      // Initialize audio device

    SetExitKey(KEY_BACKSPACE);

    SetTextLineSpacing(50); // for UI

    // track when we do the next frame
    frameTime = GetTime();
    previousFrameTime = GetTime();
    // track when we do the next game tick
    gameTime = GetTime();
}

void RunMainLoop()
{
#if defined(PLATFORM_WEB)
    // TODO may want to bump this rate up to make higher framerates possible
    emscripten_set_main_loop(startLoop, 1 / TICK, 1);
#else
    startLoop();
#endif
}

static void startLoop(void)
{
    // Main game loop. PLATFORM_WEB doesn't use this because emscripten controls
    // when the function is called
#ifndef PLATFORM_WEB
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // after the game logic runs, see if it's time to run the game logic
        // again. That needs to be the priority over the rendering.
        while (gameTime < GetTime())
        {
            //if (GetTime() - gameTime > 0.001)
            //    printf("running tick %f with delta %f\n", gameTime, GetTime() - gameTime);
            gameTime = GetTime() + TICK;
#endif // !PLATFORM_WEB
            UpdateLogic();
#ifndef PLATFORM_WEB
        }
#endif // !PLATFORM_WEB

        if (frameTime < GetTime())
        {
            double frameDeltaTime = GetTime() - previousFrameTime;
            setLastFrame(frameDeltaTime);
            //printf("running frame %f with delta %f\n", frameTime, frameDeltaTime);
            previousFrameTime = frameTime;
            // TODO probably make the framerate settable as a user option
            frameTime = GetTime() + 1.0 / 120.0;
            UpdateDrawFrame();
        }

#ifndef PLATFORM_WEB
        /*printf(
            "time: %f\ngame time: %f\nframe time: %f\n",
            GetTime(), gameTime, frameTime
        );*/
        if (GetTime() < fmin(gameTime, frameTime))
        {
            double toWait = fmin(gameTime, frameTime) - GetTime();
            //printf("wait time: %f\n", toWait);
            WaitTime(toWait);
        }
    }
#endif // !PLATFORM_WEB
}

void SetLogicCb(LogicCbType cb)
{
    s_logicCb = cb;
}

void SetDrawCb(DrawCbType cb)
{
    s_drawCb = cb;
}

void SetDefaultFont(Font font)
{
    defaultFont = font;
}

void UpdateLogic()
{
    // this is the ideal place to poll for inputs but it doesn't work on web
#ifndef PLATFORM_WEB
    PollInputEvents();
#endif // PLATFORM_WEB

    s_logicCb();

    // this sucks. Now the input is delayed for a tick. Why doesn't the ideal
    // location for polling work?
#ifdef PLATFORM_WEB
    PollInputEvents();
#endif // PLATFORM_WEB
}

void UpdateDrawFrame()
{
    BeginDrawing();

    s_drawCb();

#ifdef DEBUG
    int fps = getFps();
    char fpsStr[8];
    snprintf(fpsStr, 8, "%d", fps);
    DrawUiText(fpsStr, 0, 0, FontSize::m, true);
#endif // DEBUG

    EndDrawing();

    SwapScreenBuffer();                  // Copy back buffer to front buffer (screen)
}

void DeInit()
{
    LevelMgr::Instance().Unload();

    AnimationMgr::Instance().Unload();

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
}