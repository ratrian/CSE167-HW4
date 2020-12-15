#version 330 core

uniform float drawDiscoball;
uniform float drawLightSource;
uniform float drawAstro;
uniform float noParticleEffect;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform vec3 lightDir;
uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 lightAtten;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D lobbyTexture;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 posOutput;
in vec2 texcoordOutput;
in vec3 normalOutput;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcPointLight(vec3 fragPos, vec3 normal, vec3 viewDir);

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    if (drawDiscoball == 1.0)
    {
        vec3 I = normalize(cameraPos - posOutput);
        vec3 R = reflect(I, normalize(normalOutput));
        fragColor = vec4(texture(skybox, R).rgb, 1.0);
    }
    else if (drawDiscoball == 0.0)
    {
        vec3 norm = normalize(normalOutput);

        if (drawLightSource == 1.0)
            fragColor = vec4(lightCol, 1.0);
        else if (drawLightSource == 0.0)
        {
            vec3 viewDir = normalize(posOutput);
            vec3 result = CalcDirLight(norm, viewDir);
            if (drawAstro == 1.0)
            {
                if (noParticleEffect == 1.0) {
                    float edge = max(0, dot(norm, viewDir));
                    if (edge < 0.01)
                        fragColor = vec4(0.0);
                    else
                        fragColor = vec4(result, 1.0);
                }
            }
            else if (drawAstro == 0.0)
                fragColor = vec4(result, 1.0);
        }
    }
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
    vec3 unitLightDir = normalize(-lightDir);
    // Diffuse shading
    float diff = max(dot(normal, unitLightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-unitLightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    if (drawAstro == 1.0) {
        vec3 color = ambient * lightCol.x + diffuse * diff * lightCol.y + specular * spec * lightCol.z;
        float intensity = diff * spec;
        if (intensity > 0.95)
        {
            color.x *= 1.0;
            color.y *= 1.0;
            color.z *= 1.0;
        }
        else if (intensity > 0.5)
        {
            color.x *= 0.7;
            color.y *= 0.7;
            color.z *= 0.7;
        }
        else if (intensity > 0.05)
        {
            color.x *= 0.35;
            color.y *= 0.35;
            color.z *= 0.35;
        }
        else
        {
            color.x *= 0.1;
            color.y *= 0.1;
            color.z *= 0.1;
        }
        return color;
    }
    vec3 color = ambient * lightCol.x + vec3(texture(lobbyTexture, texcoordOutput)) + specular * spec * lightCol.z;
    return color;
}

// Calculates the color when using a point light.
vec3 CalcPointLight(vec3 fragPos, vec3 normal, vec3 viewDir)
{
    vec3 unitLightDir = normalize(lightPos - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, unitLightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-unitLightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // Attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0f / (lightAtten.x + lightAtten.y * distance + lightAtten.z * distance * distance);

    if (drawAstro == 1.0) {
        vec3 color = ambient * lightCol.x + diffuse * diff * lightCol.y * attenuation + specular * spec * lightCol.z * attenuation;
        float intensity = diff * spec;
        if (intensity > 0.95)
        {
            color.x *= 1.0;
            color.y *= 1.0;
            color.z *= 1.0;
        }
        else if (intensity > 0.5)
        {
            color.x *= 0.7;
            color.y *= 0.7;
            color.z *= 0.7;
        }
        else if (intensity > 0.05)
        {
            color.x *= 0.35;
            color.y *= 0.35;
            color.z *= 0.35;
        }
        else
        {
            color.x *= 0.1;
            color.y *= 0.1;
            color.z *= 0.1;
        }
        return color;
    }
    vec3 color = ambient * lightCol.x + vec3(texture(lobbyTexture, texcoordOutput)) + specular * spec * lightCol.z * attenuation;
    return color;
}