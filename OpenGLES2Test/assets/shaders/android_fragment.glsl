varying lowp vec4 varyColor;
varying lowp vec2 varyTexCoord;

uniform lowp sampler2D unifTexture;
uniform lowp float unifNoTexture;
uniform lowp float unifFontTexture;

void main()
{
	// ES2.0 doesn't support booleans or ints in fragment shader
	if (unifNoTexture > 0.5)
	{
		gl_FragColor = varyColor;
	}
	else 
	{
		if (unifFontTexture > 0.5)
		{
			lowp float alpha = texture2D(unifTexture, varyTexCoord).a * varyColor.a;
			gl_FragColor = vec4(varyColor.rgb, alpha);
		}
		else
		{
			gl_FragColor = texture2D(unifTexture, varyTexCoord);
			gl_FragColor *= varyColor;
		}
	}
}