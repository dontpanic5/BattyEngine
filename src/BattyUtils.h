#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

struct BoundingBox;
struct Model;
struct Vector3;

class Entity;
class EnvironmentalObject;

float GetBattyFrameTime();

BoundingBox BattyGetModelBoundingBox(Model model, bool anim = true);

Vector3 ClosestPointBox(Vector3 point, BoundingBox bb);

float GetOverlapDistanceBoxBox(BoundingBox bb1, BoundingBox bb2, Vector3 normDirection);
float GetOverlapDistanceBoxSphere(BoundingBox bb, Vector3 pos, float radius);
float GetOverlapDistanceSphereSphere(Vector3 pos1, float radius1, Vector3 pos2, float radius2);

#define _batty_countof(array) (sizeof(array) / sizeof(array[0]))

void setLastFrame(double frame);
int getFps();

#endif // !UTILS_H_INCLUDED
