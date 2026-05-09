#ifndef __BLEND_MODE_H__
#define __BLEND_MODE_H__

typedef enum {
    BLEND_MODE_ALPHA = 0,                // Blend textures considering alpha (default)
    BLEND_MODE_ADDITIVE,                 // Blend textures adding colors
    BLEND_MODE_MULTIPLIED,               // Blend textures multiplying colors
    BLEND_MODE_ADD_COLORS,               // Blend textures adding colors (alternative)
    BLEND_MODE_SUBTRACT_COLORS,          // Blend textures subtracting colors (alternative)
    BLEND_MODE_ALPHA_PREMULTIPLY,        // Blend premultiplied textures considering alpha
    BLEND_MODE_CUSTOM,                   // Blend textures using custom src/dst factors (use rlSetBlendFactors())
    BLEND_MODE_CUSTOM_SEPARATE           // Blend textures using custom rgb/alpha separate src/dst factors (use rlSetBlendFactorsSeparate())
} BlendMode;

#endif // __BLEND_MODE_H__