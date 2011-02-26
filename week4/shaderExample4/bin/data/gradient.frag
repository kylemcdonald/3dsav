
varying vec4 position;

uniform float time;


void main() {


	float val = noise1(vec4(position.x/100.0, position.y/100.0, position.z/100.0, time));
	gl_FragColor = vec4 (val, val, val,1);
	
}
