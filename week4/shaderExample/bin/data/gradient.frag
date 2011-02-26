
varying vec4 position;	// values that are changing that are passed through from .vert to .frag

uniform float color;

void main() {

	gl_FragColor = vec4 (color,color,color, 1);
	
}
