#ifdef GL_ES
precision mediump float;
#endif

varying vec2 fragTexCoord;
uniform sampler2D texture0;

void main()
{
    vec4 texColor = texture2D(texture0, fragTexCoord);

    // Output texture color directly (unlit)
    gl_FragColor = texColor;

    // To brighten slightly:
    // gl_FragColor = vec4(texColor.rgb * 1.2, texColor.a);
}
