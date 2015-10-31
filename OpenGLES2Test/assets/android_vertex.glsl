attribute lowp vec2 attrPosition;
attribute lowp vec4 attrColor;
attribute lowp vec2 attrTexCoord;

uniform lowp mat4 unifProjection;
uniform lowp mat4 unifModel;
uniform lowp float unifLayer;

varying lowp vec4 varyColor;
varying lowp vec2 varyTexCoord;

void main()
{
	varyColor = attrColor;
	varyTexCoord = attrTexCoord;
	gl_Position = unifProjection * unifModel * vec4(attrPosition, unifLayer, 1.0);
}