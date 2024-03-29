#include "raylib.h"
#include "raymath.h"
#include "BattyUtils.h"

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
            BoundingBox tempBounds = GetMeshBoundingBoxEx(model.meshes[i], model.transform, true);

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

    toTravel.x /= normDirection.x;
    toTravel.y /= normDirection.y;
    toTravel.z /= normDirection.z;
    toTravel.x = fabs(toTravel.x);
    toTravel.y = fabs(toTravel.y);
    toTravel.z = fabs(toTravel.z);

    if (toTravel.y < toTravel.x)
    {
        if (toTravel.z < toTravel.y)
            return toTravel.z;
        return toTravel.y;
    }
    else if (toTravel.z < toTravel.x)
    {
        return toTravel.z;
    }
    return toTravel.x;
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
