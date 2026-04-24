#ifndef __TEXTURE_ATLAS_H__
#define __TEXTURE_ATLAS_H__

#include "texture2d.h"

typedef struct TextureAtlas {
    Texture2D* textures;
    u32 texturesCount;
    u32 atlasWidth;
    u32 atlasHeight;
    Vector2u textureSize;
} TextureAtlas;

TextureAtlas TextureAtlas_New(const char* imagePath, u32 atlasWidth, u32 atlasHeight);
Texture2D* TextureAtlas_TextureByCoords(const TextureAtlas* atlas, u32 x, u32 y);
Texture2D* TextureAtlas_TextureByIdx(const TextureAtlas* atlas, u32 idx);
void TextureAtlas_Free(TextureAtlas* atlas);

#endif // __TEXTURE_ATLAS_H__