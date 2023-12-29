#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

struct BoundingBox;
struct Model;

float GetBattyFrameTime();

BoundingBox BattyGetModelBoundingBox(Model model);

#define _batty_countof(array) (sizeof(array) / sizeof(array[0]))

void setLastFrame(double frame);
int getFps();

#endif // !UTILS_H_INCLUDED
