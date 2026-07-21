#version 100
precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec4 flashColor;
uniform float flashStrength;

void main()
{
    vec4 tex = texture2D(texture0, fragTexCoord);
    if (tex.a <= 0.01) discard;
    vec3 rgb = mix(tex.rgb, flashColor.rgb, flashStrength);
    gl_FragColor = vec4(rgb, tex.a) * colDiffuse;
}
