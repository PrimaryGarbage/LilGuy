#include "shader.h"

#include <math.h>
#include <string.h>

#include "cleanup.h"
#include "color.h"
#include "graphics.h"
#include "image.h"
#include "raylib_wrapper.h"
#include "result.h"
#include "math_helpers.h"

#define SHADER_COUNT_MAX 64
#define SHADER_UNIFORM_LOCATION_COUNT_MAX 32
#define SHADER_KEY_LENGTH_MAX 64
#define SHADER_PATH_PREFIX "res/shaders/"

typedef struct ShaderUniformLocation {
    const char* name;
    i32 location;
} ShaderUniformLocation;

typedef struct ShaderCacheUnit {
    char key[SHADER_KEY_LENGTH_MAX];
    Shader shader;
    ShaderUniformLocation uniformLocations[SHADER_UNIFORM_LOCATION_COUNT_MAX];
    u32 uniformLocationsCount;
} ShaderCacheUnit;


static ShaderCacheUnit s_shaderCache[SHADER_COUNT_MAX];
static u32 s_shaderCacheCount;
static Texture2D s_lightTexture;
static bool s_initialized;

static void Cleanup()
{
    for(u32 i = 0u; i < s_shaderCacheCount; ++i)
    {
        Raylib_UnloadShader(&s_shaderCache[i].shader);
        s_shaderCache[i].uniformLocationsCount = 0u;
    }
}

static void EnsureInitialized()
{
    if (s_initialized) return;

    // Generate a 128x128 radial gradient light cookie
    Image image = Image_FromColor(128, 128, COLOR_NOCOLOR);
    float halfWidth = image.width / 2.0f;
    float halfHeight = image.height / 2.0f;
    for (u32 y = 0; y < image.height; y++) {
        for (u32 x = 0; x < image.width; x++) {
            float dx = (x - halfWidth) / halfWidth;
            float dy = (y - halfHeight) / halfHeight;
            float dist = sqrtf(dx*dx + dy*dy);
            float alpha = 1.0f - Smoothstepf(0.0f, 1.0f, dist); // soft falloff
            alpha = alpha * alpha; // optional: squarer falloff
            Color c = { 255, 255, 255, (u8)(alpha * 255.0f) };
            Image_DrawPixel(&image, x, y, c);
        }
    }

    s_lightTexture = Graphics_LoadTextureFromImage(&image);
    Image_Free(&image);

    Cleanup_AddCallback(Cleanup);

    s_initialized = true;
}

static Shader* GetShaderFromCache(const char* vsName, const char* fsName)
{
    char key[SHADER_KEY_LENGTH_MAX] = {};
    strcat(key, vsName);
    strcat(key, fsName);

    for(u32 i = 0u; i < s_shaderCacheCount; ++i)
    {
        if (strcmp(key, s_shaderCache[i].key) == 0)
            return &s_shaderCache[i].shader;
    }

    if (s_shaderCacheCount == SHADER_COUNT_MAX)
        PANIC_M("Max number of cached shaders exceeded!");

    char vsFilepath[256] = {};
    char fsFilepath[256] = {};
    strcat(vsFilepath, SHADER_PATH_PREFIX);
    strcat(fsFilepath, SHADER_PATH_PREFIX);
    strcat(vsFilepath, vsName);
    strcat(fsFilepath, fsName);

    Shader shader = Raylib_LoadShader(vsFilepath, fsFilepath);

    ShaderCacheUnit* cacheUnit = &s_shaderCache[s_shaderCacheCount];
    cacheUnit->shader = shader;
    memcpy(cacheUnit->key, key, SHADER_KEY_LENGTH_MAX);
    s_shaderCacheCount++;

    return &cacheUnit->shader;
}

static i32 GetShaderUniformLocationFromCache(Shader* shader, const char* uniformName)
{
    for(u32 i = 0u; i < s_shaderCacheCount; ++i)
    {
        if (s_shaderCache[i].shader.id == shader->id)
        {
            for(u32 j = 0u; j < s_shaderCache[i].uniformLocationsCount; ++j)
            {
                if (strcmp(s_shaderCache[i].uniformLocations[j].name, uniformName) == 0)
                    return s_shaderCache[i].uniformLocations[j].location;
            }

            i32 newLocationIdx = Raylib_GetShaderLocation(shader, uniformName);
            if (newLocationIdx < 0) PANIC_EX(LogErrorM("Failed to find shader uniform location. Shader key: %s, Uniform name: %s", s_shaderCache[i].key, uniformName););

            ShaderUniformLocation* newUniformLocation  = &s_shaderCache[i].uniformLocations[s_shaderCache[i].uniformLocationsCount];
            newUniformLocation->location = newLocationIdx;
            newUniformLocation->name = uniformName;
            s_shaderCache[i].uniformLocationsCount++;

            return newLocationIdx;
        }
    }

    return -1;
}

Shader* Shader_Get(const char* vsName, const char* fsName)
{
    EnsureInitialized();

    return GetShaderFromCache(vsName, fsName);
}

void Shader_SetUniform(Shader* shader, const char* uniformName, void* uniformValue, UniformDataType type)
{
    EnsureInitialized();

    i32 uniformLocation = GetShaderUniformLocationFromCache(shader, uniformName);
    if (uniformLocation < 0) PANIC_M("Failed to find shader uniform location");
    Raylib_SetShaderValue(shader, uniformLocation, uniformValue, type);
}