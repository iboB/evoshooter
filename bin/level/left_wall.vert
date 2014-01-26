uniform mat4 pvm;

attribute vec4 inPos;
attribute vec2 inTexCoord;

varying vec2 texCoord;

void main()
{
	gl_Position =  pvm * inPos;

	texCoord = inTexCoord.xy*10;
}