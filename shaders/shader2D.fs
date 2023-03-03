#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform vec4 color;
uniform sampler2D tex;
uniform bool colorOnly = false;
uniform bool isText = false;
uniform uint textOutline = 0u;
uniform vec3 textOutlineColor = vec3(0.f, 0.f, 0.f);
uniform float circleRadius = 0.f;
uniform vec2 circlePos = vec2(0.f, 0.f);

vec3 rgb2hsv(vec3 c){
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(){
	/* if(circleRadius != 0.f){
		if(pow(gl_FragCoord.x - circlePos.x, 2) + pow(gl_FragCoord.y - circlePos.y, 2) > pow(circleRadius, 2)){
			discard;
		}
	} */
	if(colorOnly){
		FragColor = color;
	}
	else if(isText){
		FragColor = vec4(color.rgb, texture(tex, texCoord).r);
		for(int i = 0; i<int(textOutline); i++){
			for(int j = -i; j<i; j++){
				if(texture(tex, vec2(texCoord.x + j, texCoord.y - i)).r > 0.f ||
					texture(tex, vec2(texCoord.x + i , texCoord.y + j)).r > 0.f ||
					texture(tex, vec2(texCoord.x - j, texCoord.y + i)).r > 0.f ||
					texture(tex, vec2(texCoord.x - i, texCoord.y - j)).r > 0.f
				){
					FragColor = vec4(textOutlineColor, 1.f);
					return;
				}
			}
		}
	}
	else{
		vec4 texColor = texture(tex, texCoord);
		vec3 finalColor = rgb2hsv(texColor.rgb);
		vec3 finalColor2 = rgb2hsv(color.rgb);
		if(finalColor2.g != 0.0){
			finalColor.r = finalColor2.r;
		}
		finalColor = hsv2rgb(finalColor) * color.rgb;
		if(finalColor.r > 1.f || finalColor.g > 1.f || finalColor.b > 1.f){
			float maxColor = max(max(finalColor.r, finalColor.g),finalColor.b);
			finalColor /= maxColor;
		}
		FragColor = vec4(finalColor, texColor.a * color.a);
	}
}