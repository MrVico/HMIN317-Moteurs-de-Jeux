#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;
varying float height;
varying vec4 color;

//! [0]
void main()
{
    // Set fragment color from texture
    //gl_FragColor = texture3D(texture, v_texcoord);
    gl_FragColor = height * color;
}
//! [0]

