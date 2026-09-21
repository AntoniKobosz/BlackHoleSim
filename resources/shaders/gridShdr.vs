#version 330

in vec3 vertexPosition;

uniform mat4 matProjection;
uniform mat4 matView;

out vec3 fragPosition;

float displacement(vec3 v){

	float r2 = v.x*v.x + v.z*v.z;
	float eps = 0.05;
	float strength = 100;
	return strength / (r2+eps);
}

void main()
{
	fragPosition = vertexPosition;
	fragPosition.y -= displacement(fragPosition);
	gl_Position = matProjection * matView * vec4(fragPosition,1.0);
}
