#include "raylib.h"
#include "raymath.h"
#include "Utils.h"

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
