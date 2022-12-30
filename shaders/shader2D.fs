#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform vec4 color;
uniform sampler2D tex;
uniform bool colorOnly = false;

void main(){
	if(colorOnly){
		FragColor = color;
	}
	else{
		vec4 texColor = texture(tex, texCoord);
		FragColor = texColor*color;
	}
}