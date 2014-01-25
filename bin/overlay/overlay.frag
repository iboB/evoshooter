attribute vec4 inPos;
attribute vec2 inTexCoord;

varying vec2 texCoord;

void main()
{
	gl_Position =  inPos;
	
	texCoord = inTexCoord.xy;
}