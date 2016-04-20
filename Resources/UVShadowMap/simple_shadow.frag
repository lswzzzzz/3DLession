#ifdef GL_ES
varying mediump vec2 v_texture_coord;
varying mediump vec4 v_position;
#else
varying vec2 v_texture_coord;
varying vec4 v_position;
#endif
uniform sampler2D u_shadowTexture;
uniform vec3 u_target_pos;
uniform vec4 u_color;
void main(void)
{
	float Radius = 4.0;
	vec2  coord;  
	coord.x = (v_position.x - u_target_pos.x)*(1.0 / Radius) + 0.5;
	coord.y = (v_position.z - u_target_pos.z)*(1.0 / Radius) + 0.5;

	gl_FragColor = u_color*texture2D(CC_Texture0,v_texture_coord)*texture2D(u_shadowTexture,coord);
}