#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

struct BoundingBox;
struct Model;
struct Vector3;

float GetBattyFrameTime();

BoundingBox BattyGetModelBoundingBox(Model model, bool anim = true);

float GetOverlapDistanceBoxBox(BoundingBox bb1, BoundingBox bb2, Vector3 normDirection);
float GetOverlapDistanceBoxSphere(BoundingBox bb, Vector3 pos, float radius);

#define _batty_countof(array) (sizeof(array) / sizeof(array[0]))

void setLastFrame(double frame);
int getFps();

#endif // !UTILS_H_INCLUDED
