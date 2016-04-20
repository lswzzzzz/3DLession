attribute vec4 a_position;
attribute vec2 a_texCoord;
varying   vec2 v_texture_coord;

uniform   float SwingSpeed;			//摇摆速度
uniform   float UVRunTime;          //运行时间
//顶点摇动
vec3  SwingVertex( float   CurrTime , float   vSwingSpeed )
{
	float	TimeOffset = CurrTime * vSwingSpeed  ;
	vec3	PosOffset  = vec3(cos(TimeOffset)*4,0,sin(TimeOffset)*4) ;
    return	PosOffset;
}
void main(void)
{
    vec3	PosOffset    = SwingVertex(UVRunTime,10.0 * a_position.y * 0.001);
	vec4	NewPosition  = vec4(a_position.x +PosOffset.x , a_position.y ,a_position.z +PosOffset.z,a_position.w);

    gl_Position = CC_MVPMatrix * NewPosition;
	
    v_texture_coord = a_texCoord;
}
