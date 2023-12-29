#version 330 core
out vec4 FragColor;

in vec3 color;
in vec3 normals;
in vec2 uvs;
in mat3 TBN;

uniform sampler2D tex0;
uniform sampler2D texNormals;
uniform sampler2D texEmissive;

uniform vec3 tint = vec3(1.0, 1.0, 1.0);
uniform vec3 lightDirection = vec3(-0.5, -1, 0.0);

uniform float time; // Add a time uniform for animation

void main()
{
    vec4 texColor = texture(tex0, uvs);

    // Sample the normal map and transform it to world space
    vec3 normalMap = (texture(texNormals, uvs).rgb * 2.0f) - 1.0f;
    normalMap = normalize(TBN * normalMap);

    // Sample the emissive map
    vec3 emissiveColor = texture(texEmissive, uvs).rgb;

    // Calculate the specular reflection based on the roughness and metallic values
    float dotProduct = 1.0 - (dot(normalize(lightDirection), normalize(normalMap)) + 1.0) / 2.0;
    dotProduct = clamp(dotProduct, 0.0, 1.0);

    

    vec4 directionalLight = vec4(dotProduct, dotProduct, dotProduct, 1.0);

    // Add pulsing effect to emissive color using sine function
    vec3 pulsingEmissive = emissiveColor;

    // Combine the emissive color with the final result
    vec4 emissiveContribution = vec4(pulsingEmissive, 1.0);
    FragColor = texColor * vec4(tint, 1.0) * directionalLight + emissiveContribution;

}
