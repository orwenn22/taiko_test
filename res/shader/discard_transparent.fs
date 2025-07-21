#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);

    // This makes it so transparent pixels don't get written and therefore won't change values in the depth buffer
    // It looks kinda bad but that's the best solution i thought of for now
    if (texelColor.a <= 0.3) discard;

    finalColor = texelColor*colDiffuse*fragColor;
}
