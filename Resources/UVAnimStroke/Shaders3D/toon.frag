#ifdef GL_ES
varying mediump vec2 v_texture_coord;
#else
varying vec2 v_texture_coord;
#endif
varying vec3 v_normal;
uniform sampler2D toonTexture;


void main(void)
{
	vec3 light_direction = vec3(1,-1,-1);
	light_direction = normalize(light_direction);

	vec3 light_color = vec3(1,1,1);

	vec3 normal  = normalize(v_normal);

	float diffuse_factor = dot(normal,-light_direction);

    if (diffuse_factor > 0.70)      diffuse_factor = 0.9;
    else if (diffuse_factor > 0.3)  diffuse_factor = 0.5;
	else							diffuse_factor = 0.2;

	light_color = light_color * diffuse_factor;

	vec4 diffuse_color = texture2D(CC_Texture0,v_texture_coord);
 	vec4 toon_color = texture2D(toonTexture,vec2(diffuse_factor,v_texture_coord.y));

	gl_FragColor = toon_color * diffuse_color ;
}