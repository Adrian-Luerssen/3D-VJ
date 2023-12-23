#version 330 core
out vec4 FragColor;

in vec2 uvs;  // Assuming you pass the texture coordinates as an attribute

uniform sampler2D tex0;
uniform float time;

// Adjust the scrolling speed by changing this value
const float scrollSpeed = 0.00005;

void main()
{
    // Calculate the scrolled texture coordinates
    vec2 scrolledUVs = vec2(uvs.x, uvs.y - time * scrollSpeed);

    // Wrap the texture coordinates to create a seamless scrolling effect
    scrolledUVs.y = fract(scrolledUVs.y);

    // Sample the texture using the scrolled coordinates
    FragColor = texture(tex0, scrolledUVs);
}
