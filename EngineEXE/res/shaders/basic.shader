#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 u_world;
uniform mat4 u_viewProjection;

void main()
{
    gl_Position = u_viewProjection * u_world * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
    
#shader fragment
#version 330 core

out vec4 FragColor;

struct Light
{
    vec4 ambient;
};

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 u_lightColor;
uniform Light u_light;

void main()
{
    vec4 ambient = u_light.ambient * texture(ourTexture, TexCoord).rgba;

    vec4 result = ambient * u_lightColor;

    FragColor = vec4(result * 1.0);
    if (FragColor.a < 0.1f)
    {
        discard;
    }
}
