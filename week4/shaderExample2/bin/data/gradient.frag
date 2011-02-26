
varying vec4 position;

uniform float x;
uniform float y;
uniform float w;
uniform float h;

uniform float color;

void main() {


	float pctx = (position.x - x) / w;
	float pcty = (position.y - y) / h;
	gl_FragColor = vec4 (pctx, pcty, color,1);
	
}
