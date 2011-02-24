uniform float focusDistance;
uniform float aperture;
uniform float pointBrightness;
uniform float rgbBrightness;
uniform float lineWidth;
uniform float aspectRatio;

attribute float side;
attribute vec3 next;

const mat2 rotate90 = mat2(
	0, -1,
	+1, 0);

void main() {
	// get the homogeneous 2d positions
  vec4 curScreenPosition = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 nextScreenPosition = gl_ModelViewProjectionMatrix * vec4(next, 1.);
	
	vec2 diff = nextScreenPosition.xy - curScreenPosition.xy;
	vec2 normal = normalize(diff) * rotate90;
	
  float diameter = abs(curScreenPosition.z - focusDistance) * aperture + lineWidth;
	
	// divide the color alpha by the area
	gl_FrontColor = gl_Color;
  gl_FrontColor.a /= (diameter / lineWidth);
	gl_FrontColor.a *= pointBrightness;
	gl_FrontColor.rgb *= rgbBrightness;
	
	vec4 offset = vec4(side * normal * diameter, 0, 0);
	offset.y *= aspectRatio;
	gl_Position = curScreenPosition + offset;
}
