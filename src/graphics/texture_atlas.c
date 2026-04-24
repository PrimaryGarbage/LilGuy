#include "texture_atlas.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "logging.h"
#include "result.h"
#include <stdio.h>
#include <stdlib.h>

TextureAtlas TextureAtlas_New(const char* imagePath, u32 atlasWidth, u32 atlasHeight)
{
    Image atlasImage = Image_Load(imagePath);
    if(atlasImage.data == NULL)
        PANIC_EX(LogErrorM("Failed to load texture atlas image. Filepath: %s", imagePath););

    TextureAtlas atlas = {
        .atlasWidth = atlasWidth,
        .atlasHeight = atlasHeight,
        .texturesCount = atlasWidth * atlasHeight,
        .textures = NULL,
        .textureSize  = { .x = atlasImage.width / atlasWidth, .y = atlasImage.height / atlasHeight },
    };
    atlas.textures = malloc(sizeof(Texture2D) * atlas.texturesCount);


    for(u32 i = 0u; i < atlasWidth; ++i)
    {
        for(u32 j = 0u; j < atlasHeight; ++j)
        {
            Rect textureImageRect = {
                .x = atlas.textureSize.x * i,
                .y = atlas.textureSize.y * j,
                .width = atlas.textureSize.x,
                .height = atlas.textureSize.y,
            };
            Image textureImage = Image_FromImage(&atlasImage, textureImageRect);
            atlas.textures[i + j * atlas.atlasWidth] = Graphics_LoadTextureFromImage(&textureImage);
            Image_Free(&textureImage);
        }
    }

    return atlas;
}

Texture2D* TextureAtlas_TextureByCoords(const TextureAtlas* atlas, u32 x, u32 y)
{
    x = Clampu(0u, atlas->atlasWidth - 1, x);
    y = Clampu(0u, atlas->atlasHeight - 1, y);
    return &atlas->textures[x + y * atlas->atlasWidth];
}

Texture2D* TextureAtlas_TextureByIdx(const TextureAtlas* atlas, u32 idx)
{
    idx = Clampu(0u, atlas->texturesCount - 1, idx);
    return &atlas->textures[idx];
}

void TextureAtlas_Free(TextureAtlas* atlas)
{
    for(u32 i = 0u; i < atlas->texturesCount; ++i)
        Graphics_UnloadTexture(atlas->textures[i]);

    free(atlas->textures);
    atlas->texturesCount = 0u;
}
