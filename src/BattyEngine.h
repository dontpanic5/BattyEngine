#ifndef BATTY_ENGINE_H_INCLUDED
#define BATTY_ENGINE_H_INCLUDED

struct Font;
struct Shader;

typedef void (*LogicCbType)();
typedef void (*DrawCbType)();

void Init(const char* name, float ambientLight = 2.0f);

extern Shader g_lighting;
extern Shader g_skinning;

void RunMainLoop(void);

void SetLogicCb(LogicCbType cb);
void SetDrawCb(DrawCbType cb);

void SetDefaultFont(Font font);
extern Font defaultFont;

void DeInit();

#endif // !BATTY_ENGINE_H_INCLUDED
