
varying vec4 position;

uniform float mousex;
uniform float mousey;

//uniform float color;

void main() {


	//float pctx = (position.x - x) / w;
	//float pcty = (position.y - y) / h;
	
	vec2 pixelpos = vec2(position.x, position.y);
	vec2 mousepos = vec2(mousex, mousey);
	float dist = distance(pixelpos, mousepos);
	float val = 0.0;
	if (dist < 100.0){
		val = 1.0 - dist/100.0;
	}
	gl_FragColor = vec4 (val, val, val,1);
	
}
