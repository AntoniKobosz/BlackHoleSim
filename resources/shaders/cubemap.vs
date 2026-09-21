#version 330

in vec3 vertexPosition;

uniform mat4 matProjection;
uniform mat4 matView;

out vec3 fragPosition;

void main()
{
	fragPosition = vertexPosition;
	gl_Position = matProjection * matView * vec4(vertexPosition,1.0);
}
