varying vec3 normal;
varying vec4 position;



void main() {
	gl_Position = ftransform();
	position = gl_Vertex;
}
