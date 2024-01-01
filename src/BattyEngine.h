#ifndef BATTY_ENGINE_H_INCLUDED
#define BATTY_ENGINE_H_INCLUDED

struct Font;

typedef void (*LogicCbType)();
typedef void (*DrawCbType)();

void Init(const char* name);

void RunMainLoop(void);

void SetLogicCb(LogicCbType cb);
void SetDrawCb(DrawCbType cb);

void SetDefaultFont(Font font);
extern Font defaultFont;

void DeInit();

#endif // !BATTY_ENGINE_H_INCLUDED
