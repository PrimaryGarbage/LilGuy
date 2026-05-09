#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;      // the tint you pass to DrawTexture() (e.g. WHITE)

uniform mat4 mvp;
uniform mat4 matModel;    // transforms vertices to world space

#define MAX_LIGHTS 32

struct PointLight {
    vec3 position;        // world‑space (z can be 0 for 2D)
    vec4 color;           // RGB + intensity (alpha)
    float intensity;
    float radius;         // falloff in world units
};

uniform PointLight pointLights[MAX_LIGHTS];
uniform int pointLightsCount;
uniform vec4 globalLightColor;
uniform float globalLightIntensity;

out vec2 fragTexCoord;
out vec4 fragVertexColor; // will be multiplied with the texture

void main()
{
    // World position of this vertex
    vec3 worldPos = (matModel * vec4(vertexPosition, 1.0)).xyz;

    // Accumulate lighting from all active lights
    vec3 lightSum = vec3(0.0);
    for (int i = 0; i < pointLightsCount; i++)
    {
        float dist = distance(worldPos, pointLights[i].position);
        float intensity = max(0.0, 1.0 - dist / pointLights[i].radius) * pointLights[i].intensity; // linear falloff
        lightSum += pointLights[i].color.rgb * intensity * pointLights[i].color.a;
    }

    // Multiply incoming vertex colour by the light, clamped to [0,1]
    fragVertexColor.rgb = clamp(vertexColor.rgb * globalLightColor.rgb * globalLightIntensity + vertexColor.rgb * lightSum, 0.0, 1.0);
    fragVertexColor.a = vertexColor.a;    // preserve alpha

    fragTexCoord = vertexTexCoord;
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}