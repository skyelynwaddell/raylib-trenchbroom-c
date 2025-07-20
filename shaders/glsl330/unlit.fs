#version 330 core

in vec2 fragTexCoord;
uniform sampler2D texture0;

out vec4 fragColor;

void main()
{
    vec4 texColor = texture(texture0, fragTexCoord);
    
    // Output texture color directly (unlit)
    fragColor = texColor;

    // Or multiply by a constant to brighten slightly:
    // fragColor = vec4(texColor.rgb * 1.2, texColor.a);
}
