#version 330 core

in vec3 fragNormal;
in vec2 fragTexCoord;

uniform sampler2D texture0;

out vec4 fragColor;

const vec3 lightDir = normalize(vec3(0.5, 0.8, 0.3));
const float ambient = 0.8;
const float diffuseStrength = 0.6;

void main()
{
    vec4 texColor = texture(texture0, fragTexCoord);

    vec3 N = normalize(fragNormal);
    float diff = max(dot(N, lightDir), 0.0);
    float lighting = ambient + diffuseStrength * diff;

    vec3 color = texColor.rgb * lighting;

    fragColor = vec4(color, texColor.a);
}
