uniform vec2 scale;

attribute vec4 inPos;
attribute vec2 inTexCoord;

varying vec2 texCoord;

void main()
{
	gl_Position =  inPos;
	
	vec2 coord = inTexCoord.xy;
  coord.x *= scale.x;
  coord.y *= scale.y;
  
  vec2 one_m_scale = vec2(1.0, 1.0) - scale;
  
  coord += one_m_scale/2;
  
  texCoord = coord;
}