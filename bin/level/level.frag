uniform sampler2D colorMap;

varying vec2 texCoord;

void main(void)
{
	gl_FragColor = texture2D(colorMap, texCoord);
}