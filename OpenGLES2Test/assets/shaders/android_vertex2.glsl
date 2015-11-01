precision highp float;

attribute vec2 attrPosition;
attribute vec4 attrColor;
attribute vec2 attrTexCoord;

uniform mat4 unifProjection;
uniform mat4 unifModel;
uniform float unifLayer;

varying vec4 varyColor;
varying vec2 varyTexCoord;

void main()
{
	varyColor = attrColor;
	varyTexCoord = attrTexCoord;
	gl_Position = unifProjection * unifModel * vec4(attrPosition, unifLayer, 1.0);
}