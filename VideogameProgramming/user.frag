#version 330 core
out vec4 FragColor;

in vec3 color;
in vec3 normals;
in vec2 uvs;
in mat3 TBN;

uniform sampler2D tex0;
uniform sampler2D texNormals;
uniform sampler2D texRough;
uniform sampler2D texMetallic;
uniform sampler2D texEmissive;

uniform vec3 tint = vec3(1.0, 1.0, 1.0);
uniform vec3 lightDirection = vec3(-0.5, -1, 0.0);

void main()
{
    vec4 texColor = texture(tex0, uvs);
    vec3 normalMap = (texture(texNormals, uvs).rgb * 2.0f) - 1.0f;
    normalMap = normalize(TBN * normalMap);

    float dotProduct = 1.0 - (dot(normalize(lightDirection), normalize(normalMap)) + 1.0) / 2.0;
    dotProduct = clamp(dotProduct, 0.0, 1.0);

    vec4 directionalLight = vec4(dotProduct, dotProduct, dotProduct, 1.0);
    
    // Apply roughness and metallic factors
    float roughness = texture(texRough, uvs).r;
    float metallic = texture(texMetallic, uvs).r;

    vec3 specularColor = mix(vec3(0.04), texColor.rgb, metallic);
    vec3 reflectedLight = normalize(reflect(lightDirection, normalMap));
    float specularIntensity = pow(max(dot(reflectedLight, normalize(vec3(0, 0, 1))), 0.0), 4.0);
    vec3 specular = specularIntensity * specularColor;

    // Apply emissive texture
    vec3 emissive = texture(texEmissive, uvs).rgb;

    // Combine diffuse, specular, and emissive
    vec3 finalColor = texColor.rgb * directionalLight.rgb * (1.0 - metallic) + specular + emissive;

    // Apply tint
    finalColor *= tint;

    FragColor = vec4(finalColor, texColor.a);
}
