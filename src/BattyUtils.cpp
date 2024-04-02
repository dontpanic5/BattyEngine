#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
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

// Generate cubemap texture from HDR texture
static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format)
{
    TextureCubemap cubemap = { 0 };

    rlDisableBackfaceCulling();     // Disable backface culling to render inside the cube

    // STEP 1: Setup framebuffer
    //------------------------------------------------------------------------------------------
    unsigned int rbo = rlLoadTextureDepth(size, size, true);
    cubemap.id = rlLoadTextureCubemap(0, size, format);

    unsigned int fbo = rlLoadFramebuffer(size, size);
    rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
    rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

    // Check if framebuffer is complete with attachments (valid)
    if (rlFramebufferComplete(fbo)) TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", fbo);
    //------------------------------------------------------------------------------------------

    // STEP 2: Draw to framebuffer
    //------------------------------------------------------------------------------------------
    // NOTE: Shader is used to convert HDR equirectangular environment map to cubemap equivalent (6 faces)
    rlEnableShader(shader.id);

    // Define projection matrix and send it to shader
    Matrix matFboProjection = MatrixPerspective(90.0 * DEG2RAD, 1.0, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
    rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matFboProjection);

    // Define view matrix for every side of the cubemap
    Matrix fboViews[6] = {
        MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 1.0f,  0.0f,  0.0f }, (Vector3) { 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { -1.0f,  0.0f,  0.0f }, (Vector3) { 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 0.0f,  1.0f,  0.0f }, (Vector3) { 0.0f,  0.0f,  1.0f }),
        MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 0.0f, -1.0f,  0.0f }, (Vector3) { 0.0f,  0.0f, -1.0f }),
        MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 0.0f,  0.0f,  1.0f }, (Vector3) { 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector3) { 0.0f,  0.0f, -1.0f }, (Vector3) { 0.0f, -1.0f,  0.0f })
    };

    rlViewport(0, 0, size, size);   // Set viewport to current fbo dimensions

    // Activate and enable texture for drawing to cubemap faces
    rlActiveTextureSlot(0);
    rlEnableTexture(panorama.id);

    for (int i = 0; i < 6; i++)
    {
        // Set the view matrix for the current cube face
        rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);

        // Select the current cubemap face attachment for the fbo
        // WARNING: This function by default enables->attach->disables fbo!!!
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
        rlEnableFramebuffer(fbo);

        // Load and draw a cube, it uses the current enabled texture
        rlClearScreenBuffers();
        rlLoadDrawCube();

        // ALTERNATIVE: Try to use internal batch system to draw the cube instead of rlLoadDrawCube
        // for some reason this method does not work, maybe due to cube triangles definition? normals pointing out?
        // TODO: Investigate this issue...
        //rlSetTexture(panorama.id); // WARNING: It must be called after enabling current framebuffer if using internal batch system!
        //rlClearScreenBuffers();
        //DrawCubeV(Vector3Zero(), Vector3One(), WHITE);
        //rlDrawRenderBatchActive();
    }
    //------------------------------------------------------------------------------------------

    // STEP 3: Unload framebuffer and reset state
    //------------------------------------------------------------------------------------------
    rlDisableShader();          // Unbind shader
    rlDisableTexture();         // Unbind texture
    rlDisableFramebuffer();     // Unbind framebuffer
    rlUnloadFramebuffer(fbo);   // Unload framebuffer (and automatically attached depth texture/renderbuffer)

    // Reset viewport dimensions to default
    rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
    rlEnableBackfaceCulling();
    //------------------------------------------------------------------------------------------

    cubemap.width = size;
    cubemap.height = size;
    cubemap.mipmaps = 1;
    cubemap.format = format;

    return cubemap;
}
