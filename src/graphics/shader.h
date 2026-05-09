#ifndef __SHADER_H__
#define __SHADER_H__

#include "color.h"
#include "short_types.h"


typedef enum UniformDataType {
    UNIFORM_FLOAT = 0,       // Shader uniform type: float
    UNIFORM_VEC2,            // Shader uniform type: vec2 (2 float)
    UNIFORM_VEC3,            // Shader uniform type: vec3 (3 float)
    UNIFORM_VEC4,            // Shader uniform type: vec4 (4 float)
    UNIFORM_INT,             // Shader uniform type: int
    UNIFORM_IVEC2,           // Shader uniform type: ivec2 (2 int)
    UNIFORM_IVEC3,           // Shader uniform type: ivec3 (3 int)
    UNIFORM_IVEC4,           // Shader uniform type: ivec4 (4 int)
    UNIFORM_SAMPLER2D        // Shader uniform type: sampler2d
} UniformDataType;

typedef struct Shader {
    u32 id;
    i32* locs;
} Shader;

Shader* Shader_Get(const char* vsName, const char* fsName);
void Shader_SetUniform(Shader* shader, const char* uniformName, void* uniformValue, UniformDataType type);

#endif // __SHADER_H__