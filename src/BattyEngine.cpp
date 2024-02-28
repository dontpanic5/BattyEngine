static const int screenWidth = 980;
static const int screenHeight = 490;

#include <cstdio>
#include "raylib.h"
#include <math.h>
#include "BattyEngine.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "Constants.h"
#include "Utils.h"
#include "Ui.h"
#include "AnimationMgr.h"
#include "LevelMgr.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

static double frameTime;
static double previousFrameTime;
static double gameTime;
static bool windowShouldClose = false;
Font defaultFont;

Shader g_lighting;

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

    // Load basic lighting shader
    g_lighting = LoadShader(
        TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
        TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION)
    );
    // Get some required shader locations
    g_lighting.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(g_lighting, "viewPos");
    // NOTE: "matModel" location name is automatically assigned on shader loading, 
    // no need to get the location again if using that uniform name
    //shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");

    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(g_lighting, "ambient");
    // TODO ambient light should be game specific or even level specific
    float ambient = 0.01f;
    float value[4] = { ambient, ambient, ambient, 1.0f };
    SetShaderValue(g_lighting, ambientLoc, value, SHADER_UNIFORM_VEC4);

    int transLoc = GetShaderLocation(g_lighting, "transparent");
    int transparent = 0;
    SetShaderValue(g_lighting, transLoc, &transparent, SHADER_UNIFORM_INT);

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
    int isFirefox = EM_ASM_INT({
        let firefox = navigator.userAgent.toLowerCase().includes('firefox');
        let firefoxInt = firefox ? 1 : 0;
        return firefoxInt;
    });
    int fps = (int) (1.0f / TICK);
    if (isFirefox)
        fps *= 2;
    emscripten_set_main_loop(startLoop, fps, 1);
#else
    startLoop();
#endif
}

static void startLoop(void)
{    
    static double lastLoop = -1.0;
    double loopStart = GetTime();
    //if (lastLoop != -1.0)
    //    printf("loop gap: %f\n", loopStart - lastLoop);
    // Main game loop. PLATFORM_WEB doesn't use this because emscripten controls
    // when the function is called
#ifndef PLATFORM_WEB
    windowShouldClose = WindowShouldClose();
    while (!windowShouldClose)    // Detect window close button or ESC key
    {
        // after the game logic runs, see if it's time to run the game logic
        // again. That needs to be the priority over the rendering.
        while (gameTime < GetTime())
        {
            //if (GetTime() - gameTime > 0.001)
            //    printf("running tick %f with delta %f\n", gameTime, GetTime() - gameTime);
#endif // !PLATFORM_WEB
            double beforeTime = GetTime();
            gameTime = beforeTime + TICK;
            UpdateLogic();
            double afterTime = GetTime();
            double delta = afterTime - beforeTime;
            //printf("delta logic: %f\n", delta);
#ifndef PLATFORM_WEB
        }
#endif // !PLATFORM_WEB

        if (frameTime < GetTime())
        {
            double time = GetTime();
            double frameDeltaTime = time - previousFrameTime;
            //printf("frameDeltaTime: %f\n", frameDeltaTime);
            setLastFrame(frameDeltaTime);
            //printf("running frame %f with delta %f\n", frameTime, frameDeltaTime);
            previousFrameTime = time;
            // TODO probably make the framerate settable as a user option
            frameTime = time + 1.0 / 120.0;
            UpdateDrawFrame();
            //printf("delta draw: %f\n", GetTime() - time);
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
    lastLoop = GetTime();
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

#ifndef PLATFORM_WEB
    if (!windowShouldClose)
        windowShouldClose = WindowShouldClose();
#endif
}

void UpdateDrawFrame()
{
    BeginDrawing();

    s_drawCb();

//#ifdef DEBUG
    int fps = getFps();
    char fpsStr[8];
    snprintf(fpsStr, 8, "%d", fps);
    DrawUiText(fpsStr, 0, 0, FontSize::m, true);
//#endif // DEBUG

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