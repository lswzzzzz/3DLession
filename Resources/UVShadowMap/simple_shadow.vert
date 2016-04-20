attribute vec4 a_position;
attribute vec2 a_texCoord;
varying vec2 v_texture_coord;
varying vec4 v_position;
uniform mat4 u_model_matrix;
void main(void)
{
    gl_Position =  CC_MVPMatrix * a_position;
    v_texture_coord = a_texCoord;
	//世界空间的位置
	v_position = u_model_matrix * a_position;
}
