#include "raylib.h"
#include "raymath.h"
#include "BattyUtils.h"
#ifndef PLATFORM_WEB
#include <crtdbg.h>
#endif // !PLATFORM_WEB
#include <stdio.h>

float GetBattyFrameTime()
{
#ifndef DEBUGGER
    return GetFrameTime();
#else
    float frameTime = GetFrameTime();
    // 60 fps
    static float rememberedFrameTime = 0.0166667f;
    if (frameTime == 0)
    {
        return frameTime;
    }
    else if (frameTime < 0.2f)
    {
        rememberedFrameTime = frameTime;
    } 
    return rememberedFrameTime;
#endif // DEBUGGER
}

// Compute mesh bounding box limits
BoundingBox GetMeshBoundingBoxEx(Mesh mesh, Matrix transform, bool useAnim)
{
    // Get min and max vertex to construct bounds (AABB)
    Vector3 minVertex = { 0 };
    Vector3 maxVertex = { 0 };

    if (mesh.vertices != NULL)
    {
        minVertex = Vector3Transform({
            useAnim ? mesh.animVertices[0] : mesh.vertices[0],
            useAnim ? mesh.animVertices[1] : mesh.vertices[1],
            useAnim ? mesh.animVertices[2] : mesh.vertices[2]
        }, transform);
        maxVertex = Vector3Transform({
            useAnim ? mesh.animVertices[0] : mesh.vertices[0],
            useAnim ? mesh.animVertices[1] : mesh.vertices[1],
            useAnim ? mesh.animVertices[2] : mesh.vertices[2]
        }, transform);

        for (int i = 1; i < mesh.vertexCount; i++)
        {
            minVertex = Vector3Min(
                minVertex,
                Vector3Transform({
                    useAnim ? mesh.animVertices[i * 3] : mesh.vertices[i * 3],
                    useAnim ? mesh.animVertices[i * 3 + 1] : mesh.vertices[i * 3 + 1],
                    useAnim ? mesh.animVertices[i * 3 + 2] : mesh.vertices[i * 3 + 2]
            }, transform));
            maxVertex = Vector3Max(
                maxVertex,
                Vector3Transform({
                    useAnim ? mesh.animVertices[i * 3] : mesh.vertices[i * 3],
                    useAnim ? mesh.animVertices[i * 3 + 1] : mesh.vertices[i * 3 + 1],
                    useAnim ? mesh.animVertices[i * 3 + 2] : mesh.vertices[i * 3 + 2]
                }, transform));
        }
    }

    // Create the bounding box
    BoundingBox box;
    box.min = minVertex;
    box.max = maxVertex;

    return box;
} 

// Compute model bounding box limits (considers all meshes)
BoundingBox BattyGetModelBoundingBox(Model model, bool anim)
{
    BoundingBox bounds;

    if (model.meshCount > 0)
    {
        Vector3 temp = { 0 };
        bounds = GetMeshBoundingBoxEx(model.meshes[0], model.transform, anim);

        for (int i = 1; i < model.meshCount; i++)
        {
            BoundingBox tempBounds = GetMeshBoundingBoxEx(model.meshes[i], model.transform, anim);

            temp.x = (bounds.min.x < tempBounds.min.x) ? bounds.min.x : tempBounds.min.x;
            temp.y = (bounds.min.y < tempBounds.min.y) ? bounds.min.y : tempBounds.min.y;
            temp.z = (bounds.min.z < tempBounds.min.z) ? bounds.min.z : tempBounds.min.z;
            bounds.min = temp;

            temp.x = (bounds.max.x > tempBounds.max.x) ? bounds.max.x : tempBounds.max.x;
            temp.y = (bounds.max.y > tempBounds.max.y) ? bounds.max.y : tempBounds.max.y;
            temp.z = (bounds.max.z > tempBounds.max.z) ? bounds.max.z : tempBounds.max.z;
            bounds.max = temp;
        }
    }

    return bounds;
}

Vector3 ClosestPointBox(Vector3 point, BoundingBox bb)
{
    Vector3 result;
    // For each coordinate axis, if the point coordinate value is
    // outside box, clamp it to the box, else keep it as is
    // also check inside
    float v = point.x;
    v = fmaxf(v, bb.min.x);
    v = fminf(v, bb.max.x);
    result.x = v;
    v = point.y;
    v = fmaxf(v, bb.min.y);
    v = fminf(v, bb.max.y);
    result.y = v;
    v = point.z;
    v = fmaxf(v, bb.min.z);
    v = fminf(v, bb.max.z);
    result.z = v;

    if (Vector3Equals(point, result))
    {
        float dists[6];

        dists[0] = fabs(point.x - bb.max.x);
        dists[1] = fabs(point.x - bb.min.x);
        dists[2] = fabs(point.y - bb.max.y);
        dists[3] = fabs(point.y - bb.min.y);
        dists[4] = fabs(point.z - bb.max.z);
        dists[5] = fabs(point.z - bb.min.z);

        int smallest = 0;
        for (int i = 1; i < 6; i++)
        {
            if (dists[i] < dists[i - 1])
                smallest = i;
        }

        switch (smallest)
        {
            case 0:
                result.x = bb.max.x;
                break;
            case 1:
                result.x = bb.min.x;
                break;
            case 2:
                result.y = bb.max.y;
                break;
            case 3:
                result.y = bb.min.y;
                break;
            case 4:
                result.z = bb.max.z;
                break;
            case 5:
                result.z = bb.min.z;
                break;
        default:
            break;
        }
    }

    return result;
}

float GetOverlapDistanceBoxBox(BoundingBox bb1, BoundingBox bb2, Vector3 normDirection)
{
    Vector3 toTravel;

    if (normDirection.x > 0.0f)
    {
        toTravel.x = bb1.max.x - bb2.min.x;
    }
    else if (normDirection.x == 0.0f)
    {
        toTravel.x = 0.0f;
    }
    else
    {
        toTravel.x = bb2.max.x - bb1.min.x;
    }

    if (normDirection.y > 0.0f)
    {
        toTravel.y = bb1.max.y - bb2.min.y;
    }
    else if (normDirection.y == 0.0f)
    {
        toTravel.y = 0.0f;
    }
    else
    {
        toTravel.y = bb2.max.y - bb1.min.y;
    }

    if (normDirection.z > 0.0f)
    {
        toTravel.z = bb1.max.z - bb2.min.z;
    }
    else if (normDirection.z == 0.0f)
    {
        toTravel.z = 0.0f;
    }
    else
    {
        toTravel.z = bb2.max.z - bb1.min.z;
    }

    if (normDirection.x != 0.0f)
        toTravel.x /= normDirection.x;
    if (normDirection.y != 0.0f)
        toTravel.y /= normDirection.y;
    if (normDirection.z != 0.0f)
        toTravel.z /= normDirection.z;
    toTravel.x = fabs(toTravel.x);
    toTravel.y = fabs(toTravel.y);
    toTravel.z = fabs(toTravel.z);

    /*if (toTravel.y < toTravel.x)
    {
        if (toTravel.z < toTravel.y)
            return toTravel.z;
        return toTravel.y;
    }
    else if (toTravel.z < toTravel.x)
    {
        return toTravel.z;
    }
    return toTravel.x;*/
    return Vector3Length(toTravel);
}

inline static float squared(float v) { return v * v; }

float GetOverlapDistanceBoxSphere(BoundingBox bb, Vector3 pos, float radius)
{
    Ray ray;
    ray.position = pos;
    RayCollision rc;

    ray.direction = {1.0f, 0.0f, 0.0f};
    rc = GetRayCollisionBox(ray, bb);
    if (rc.hit)
    {
        return radius - rc.distance;
    }

    ray.direction = {-1.0f, 0.0f, 0.0f};
    rc = GetRayCollisionBox(ray, bb);
    if (rc.hit)
    {
        return radius - rc.distance;
    }

    ray.direction = {0.0f, 1.0f, 0.0f};
    rc = GetRayCollisionBox(ray, bb);
    if (rc.hit)
    {
        return radius - rc.distance;
    }

    ray.direction = { 0.0f, -1.0f, 0.0f };
    rc = GetRayCollisionBox(ray, bb);
    if (rc.hit)
    {
        return radius - rc.distance;
    }

    ray.direction = { 0.0f, 0.0f, 1.0f };
    rc = GetRayCollisionBox(ray, bb);
    if (rc.hit)
    {
        return radius - rc.distance;
    }

    ray.direction = { -1.0f, 0.0f, -1.0f };
    rc = GetRayCollisionBox(ray, bb);
    if (rc.hit)
    {
        return radius - rc.distance;
    }

    printf("bad! Not good! Bad!\n");
    return 0.0f;
}

float GetOverlapDistanceSphereSphere(Vector3 pos1, float radius1, Vector3 pos2, float radius2)
{
    float posDist = Vector3Distance(pos1, pos2);
    float radiuses = radius1 + radius2;
    float result = radiuses - posDist;
    if (result > 0.0f)
        return result;
    else
        return 0.0f;
}


float GetOverlapDistanceBoxSphere(Vector3 center, float radius, Vector3 normDirection)
{
    return 0.0f;
}


#define FPS_CAPTURE_FRAMES_COUNT    90      // captures

static int index = 0;
static float history[FPS_CAPTURE_FRAMES_COUNT] = { 0 };
static float average = 0, last = 0;

void setLastFrame(double frame)
{
    if (frame == 0.0) return;

    last = (float)GetBattyFrameTime();
    index = (index + 1) % FPS_CAPTURE_FRAMES_COUNT;
    average -= history[index];
    history[index] = frame / FPS_CAPTURE_FRAMES_COUNT;
    average += history[index];
}

int getFps()
{
    return (int)roundf(1.0f / average);
}
