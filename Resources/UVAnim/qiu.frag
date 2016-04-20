#ifdef GL_ES
varying mediump vec2 v_texture_coord;
#else
varying vec2 v_texture_coord;
#endif

uniform vec4 u_color;

void main(void)
{
    vec2 texCoord  = v_texture_coord;
	texCoord.y = 1.0 - texCoord.y;
    gl_FragColor   = u_color*texture2D(CC_Texture0, texCoord);
}