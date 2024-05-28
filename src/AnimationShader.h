#ifndef ANIMATION_SHADER_H_INCLUDED
#define ANIMATION_SHADER_H_INCLUDED

#include <vector>
#include "raylib.h"
#include "raymath.h"

class AnimationShader
{
public:
    static constexpr int MaxBones = 64;

    Shader BaseShader = { 0 };

    std::vector<int> ShaderBoneLocations;

    void Setup(Shader& shader)
    {
        BaseShader = shader;

        ShaderBoneLocations.resize(MaxBones);

        for (int i = 0; i < MaxBones; i++)
        {
            ShaderBoneLocations[i] = GetShaderLocation(BaseShader, TextFormat("bones[%d]", i));

            SetShaderValueMatrix(BaseShader, ShaderBoneLocations[i], MatrixIdentity());
        }
    }
}

#endif // !ANIMATION_SHADER_H_INCLUDED