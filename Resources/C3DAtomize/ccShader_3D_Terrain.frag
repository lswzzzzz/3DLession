#ifdef GL_ES
precision lowp float;
#endif

#ifdef GL_ES
varying mediump vec2 v_texCoord;
varying mediump vec3 v_normal;
varying float v_fogFactor;              //weight for fog
#else
varying vec2 v_texCoord;
varying vec3 v_normal;
varying float v_fogFactor;              //weight for fog
#endif


uniform vec3 u_color;
uniform int u_has_alpha;
uniform sampler2D u_alphaMap;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform float u_detailSize[4];
uniform vec4 u_fogColor;

void main()
{

	vec3 light_direction = vec3(1,-1,0);
	float lightFactor = dot(-light_direction,v_normal);
	if(u_has_alpha<=0)
	{
		vec4 finalColor  =  texture2D(u_texture0, v_texCoord)*lightFactor;
		gl_FragColor     = finalColor;        //out put finalColor
	}
	else
	{
		vec4 blendFactor =texture2D(u_alphaMap,v_texCoord);
		vec4 color = vec4(0,0,0,0);
		color = texture2D(u_texture0, v_texCoord*u_detailSize[0])*blendFactor.r +
		texture2D(u_texture1, v_texCoord*u_detailSize[1])*blendFactor.g + texture2D(u_texture2, v_texCoord*u_detailSize[2])*blendFactor.b;
		float grayFactor =dot(blendFactor.rgb, vec3(1, 1, 1));
		color +=texture2D(u_texture3, v_texCoord*u_detailSize[3])*(1.0-grayFactor);
		gl_FragColor = vec4(color.rgb*lightFactor, 1.0);
		gl_FragColor.rgb += vec3(0.384, 0.769, 1.0) * gl_FragColor.rgb*0.8;
		gl_FragColor = mix(u_fogColor, gl_FragColor, v_fogFactor);
	}
}
