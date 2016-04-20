#ifdef GL_ES
varying mediump vec2 v_texture_coord;
#else
varying vec2 v_texture_coord;
#endif

uniform vec4 u_color;
uniform sampler2D caustics; 
uniform vec4 blendTextureColor;
uniform vec2  UVSpeed;
uniform float UVRunTime;

void main(void)
{
    vec2 texCoord1 = v_texture_coord;
	texCoord1.y = 1.0 - texCoord1.y;
	//texCoord1.x  += UVSpeed.y*UVRunTime;

    vec2 texCoord2 = v_texture_coord;
	texCoord2.y += UVSpeed.y*UVRunTime;

    gl_FragColor = u_color*texture2D(CC_Texture0, texCoord1) + texture2D(caustics,texCoord2) * blendTextureColor * 0.5;
}