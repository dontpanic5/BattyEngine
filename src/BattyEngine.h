#ifndef BATTY_ENGINE_H_INCLUDED
#define BATTY_ENGINE_H_INCLUDED

#include <cstring>

struct Font;
struct Shader;

typedef void (*LogicCbType)();
typedef void (*DrawCbType)();

void Init(const char* name, float ambientLight = 2.0f);

extern Shader g_lighting;
extern Shader g_skinning;

struct FileToWatch
{
    static constexpr int MAX_FILENAME_SIZE = 128;

    FileToWatch()
    {
        memset(m_filename, 0, sizeof(char) * MAX_FILENAME_SIZE);
    }

    char m_filename[MAX_FILENAME_SIZE];
    double m_lastUpdated = -1.0;
    bool m_updatePending = false;
};
static constexpr int MAX_FILES_TO_WATCH = 8;
extern FileToWatch filesToWatch[MAX_FILES_TO_WATCH];
extern int filesToWatchIdx;

extern bool gamePaused;

void RunMainLoop(void);

void SetLogicCb(LogicCbType cb);
void SetDrawCb(DrawCbType cb);

void SetDefaultFont(Font font);
extern Font defaultFont;

void DeInit();

#endif // !BATTY_ENGINE_H_INCLUDED
