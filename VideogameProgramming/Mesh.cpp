#include "Mesh.h"


Mesh::Mesh(const char* filePath)
{
	std::cout << "read mesh " << filePath << std::endl;
	
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::string myText;
	std::ifstream MyReadFile(filePath);

	std::string objFormat = "full";

	while (getline(MyReadFile, myText)) {

		if (myText.rfind("v ", 0) == 0) {
			glm::vec3 vertex;
			sscanf_s(myText.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (myText.rfind("vt ", 0) == 0) {
			glm::vec2 uv;
			sscanf_s(myText.c_str(), "vt %f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (myText.rfind("vn ", 0) == 0) {
			glm::vec3 normal;
			sscanf_s(myText.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (myText.rfind("f ", 0) == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			if (myText.find("//") != -1) {
				int matches = sscanf_s(myText.c_str(), "f %d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				objFormat = "noUv";
			}
			else if (myText.find("/") != -1) {
				int matches = sscanf_s(myText.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				objFormat = "full";
			}
			else {
				int matches = sscanf_s(myText.c_str(), "f %d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				objFormat = "noUvNoNormals";
			}
		}
	}

	for (int i = 0; i < vertexIndices.size(); i++) {

		Vertex vertex = Vertex();
		if (objFormat == "full") {
			vertex.position = temp_vertices[vertexIndices[i] - 1];
			vertex.normal = temp_normals[normalIndices[i] - 1];
			vertex.color = glm::vec3(1., 1., 1.);
			vertex.texUV = temp_uvs[uvIndices[i] - 1];
		}
		else if (objFormat == "noUv") {
			vertex.position = temp_vertices[vertexIndices[i] - 1];
			vertex.normal = temp_normals[normalIndices[i] - 1];
			vertex.color = glm::vec3(1., 1., 1.);
			vertex.texUV = glm::vec2(0., 0.);
		}
		else if (objFormat == "noUvNoNormals"){
			vertex.position = temp_vertices[vertexIndices[i] - 1];
			vertex.normal = glm::vec3(0., 0., 0.);
		    vertex.color = glm::vec3(1., 1., 1.);
			vertex.texUV = glm::vec2(0., 0.);
		}

		vertices.push_back(vertex);
	}

	printf("VERTEX");

	for (int i = 0; i < vertexIndices.size(); i++) {

		indices.push_back(i);
	}

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Delete()
{
	VAO.Delete();
}
