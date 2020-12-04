#version 330 core

uniform float drawAstro;
uniform float drawDiscoball;
uniform float drawLightSource;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

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
            vec3 result = CalcPointLight(posOutput, norm, viewDir);
            if (drawAstro == 1.0)
            {
                float edge = max(0, dot(norm, viewDir));
                if (edge < 0.01)
                    fragColor = vec4(0.0);
                else
                    fragColor = vec4(result, 1.0);
            }
            else if (drawAstro == 0.0)
                fragColor = vec4(result, 1.0);
        }
    }
}

// Calculates the color when using a point light.
vec3 CalcPointLight(vec3 fragPos, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // Attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0f / (lightAtten.x + lightAtten.y * distance + lightAtten.z * distance * distance);

    return (ambient * lightCol.x + vec3(texture(lobbyTexture, texcoordOutput)) + specular * spec * lightCol.z * attenuation);
}