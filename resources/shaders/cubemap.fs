#version 330

// rzeczywisty wektor kierunku przekazany z cubemap.vs
in vec3 fragPosition;

uniform sampler2D equirectangularMap; // panorama

out vec4 finalColor;

vec2 SampleSphericalMap(vec3 dir)
{
        // biegun sfery wzdłuż OY
        vec2 uv = vec2(atan(dir.z, dir.x), acos(dir.y)); // u(-pi,pi) / v(0, pi)
        uv *= vec2(0.15915494, 0.31830988); // (1/2pi, 1/pi)
        uv.x += 0.5; // u(0,1) v(0,1)

        return uv;
}

void main()
{
        // u - kąt od bieguna, v - wzdłóż równika, znormalizowane
        vec2 uv = SampleSphericalMap(normalize(fragPosition));

        // odczytaj kolor w zadanych współrzędnych
        vec3 Color = texture(equirectangularMap, uv).rgb;
        finalColor = vec4(Color, 1.0);
}
