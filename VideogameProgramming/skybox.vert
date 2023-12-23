#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 uvs;
out vec3 normals;
out vec3 fragPos;  // New output variable for fragment position

uniform float scale = 1.0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);

   uvs = aTex;
   normals = aNormals;

   // Calculate fragment position in world space
   fragPos = vec3(model * vec4(aPos, 1.0));
}
