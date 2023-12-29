#version 330 core
out vec4 FragColor;

in vec2 uvs;  // Assuming you pass the texture coordinates as an attribute

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform float time;

// Adjust the scrolling speed by changing this value
const float scrollSpeed = 0.0000;

void main()
{
    // Calculate the scrolled texture coordinates
    vec2 scrolledUVs = vec2(uvs.x, uvs.y - time * scrollSpeed);

    // Wrap the texture coordinates to create a seamless scrolling effect
    scrolledUVs.y = fract(scrolledUVs.y);

    // Sample the textures
    vec4 color0 = texture(tex0, scrolledUVs);
    vec4 color1 = texture(tex1, scrolledUVs);
    vec4 color2 = texture(tex2, scrolledUVs);
    vec4 color3 = texture(tex3, scrolledUVs);

    // Blend the textures based on their alpha values
    FragColor = color0 * (1.0 - color1.a) * (1.0 - color2.a) * (1.0 - color3.a)
              + color1 * color1.a * (1.0 - color2.a) * (1.0 - color3.a)
              + color2 * color2.a * (1.0 - color3.a)
              + color3 * color3.a;
}
