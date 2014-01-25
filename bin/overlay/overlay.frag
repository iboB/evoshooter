uniform sampler2D colorMap;

varying vec2 texCoord;

void main(void)
{	
	vec4 color = texture2D(colorMap, texCoord);
  gl_FragColor = color * (1 - color.x);
}