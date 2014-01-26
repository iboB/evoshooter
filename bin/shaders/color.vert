uniform mat4 pvm;

attribute vec4 inPos;
attribute vec4 inColor;

varying vec4 color;

void main()
{
	gl_Position =  pvm * inPos;
	
	color = inColor;
}