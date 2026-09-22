#version 330

in vec3 vertexPosition;

// macierze transformacji
uniform mat4 matProjection;
uniform mat4 matView;

// output do fragment shadera
out vec3 direction;

void main()
{
        direction = normalize(vertexPosition);

        // usuń część z translacją
        mat4 rotView = mat4(mat3(matView));
        vec4 clipPos = matProjection * rotView * vec4(vertexPosition, 1.0);

        gl_Position = clipPos;
}
