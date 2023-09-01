attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec4 a_color;

varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

void main()
{
	//store the texture coordinates
	v_uv = a_uv;

	//store the color in the varying var to use it from the pixel shader
	v_color = a_color;

	// Don’t apply transformations to the vertex!
	gl_Position = vec4(a_vertex, 1.0);
}
