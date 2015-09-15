#version 330 core
		
layout (location = 0) out vec4 color;
		
uniform sampler2D tex;
		
in DATA
{
	vec2 uv;
} fs_in;

float resolution = 1024.0;
float radius = 4.0;
vec2 dir = vec2(1.0, 0.0);
		
void main()
{
	vec4 sum = vec4(0.0);
	vec2 tc = fs_in.uv;

	// Blur amount in pixels	
	float blur = radius / resolution; 
    
    //the direction of our blur
    //(1.0, 0.0) -> x-axis blur
    //(0.0, 1.0) -> y-axis blur
	float hstep = dir.x;
	float vstep = dir.y;
	
    //apply blurring, using a 9-tap filter with predefined gaussian weights
    
	sum += texture(tex, vec2(tc.x - 4.0*blur*hstep, tc.y - 4.0*blur*vstep)) * 0.0162162162;
	sum += texture(tex, vec2(tc.x - 3.0*blur*hstep, tc.y - 3.0*blur*vstep)) * 0.0540540541;
	sum += texture(tex, vec2(tc.x - 2.0*blur*hstep, tc.y - 2.0*blur*vstep)) * 0.1216216216;
	sum += texture(tex, vec2(tc.x - 1.0*blur*hstep, tc.y - 1.0*blur*vstep)) * 0.1945945946;
	
	sum += texture(tex, vec2(tc.x, tc.y)) * 0.2270270270;
	
	sum += texture(tex, vec2(tc.x + 1.0*blur*hstep, tc.y + 1.0*blur*vstep)) * 0.1945945946;
	sum += texture(tex, vec2(tc.x + 2.0*blur*hstep, tc.y + 2.0*blur*vstep)) * 0.1216216216;
	sum += texture(tex, vec2(tc.x + 3.0*blur*hstep, tc.y + 3.0*blur*vstep)) * 0.0540540541;
	sum += texture(tex, vec2(tc.x + 4.0*blur*hstep, tc.y + 4.0*blur*vstep)) * 0.0162162162;

	//discard alpha for our simple demo, multiply by vertex color and return
	color = vec4(sum.rgb, 1.0);
}