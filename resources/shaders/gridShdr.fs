#version 330

out vec4 finalColor;

void main()
{
	vec3 RGB = vec3(3.,160.,98.) / 256.;
	// finalColor = vec4(33./256.,252./256.,143./256.,1.) ;
	finalColor = vec4(RGB,1.);
	// finalColor = vec4(3.,.5,1.,1.) ;
}
