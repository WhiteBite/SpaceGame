#include "model.h"

int VERTEX_LENGTH = 11;

CModel::CModel(std::string path, int rt)
{
	render_type = rt;
	CModelLoader ml = CModelLoader();
	if (ml.LoadModel(this, path))
	{
		glGenVertexArrays(1, &uiVAO);
		glGenBuffers(1, &uiVBO);
		/*glGenBuffers(1, &uiEBO);*/

		glBindVertexArray(uiVAO);

		std::vector<GLfloat> vert;
		for (unsigned int i = 0; i < vertecies.size(); ++i)
		{
			vert.push_back(vertecies[i][0]);
			vert.push_back(vertecies[i][1]);
			vert.push_back(vertecies[i][2]);

			vert.push_back(uvs[i][0]);
			vert.push_back(uvs[i][1]);

			vert.push_back(normals[i][0]);
			vert.push_back(normals[i][1]);
			vert.push_back(normals[i][2]);

			vert.push_back(0.2f);
			vert.push_back(0.2f);
			vert.push_back(0.2f);

			/*vert.push_back(colors[i][0]);
			vert.push_back(colors[i][1]);
			vert.push_back(colors[i][2]);*/
		}
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(GLfloat), &vert[0], GL_STATIC_DRAW);
		nodes = vert.size() / VERTEX_LENGTH;
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_LENGTH * sizeof(GLfloat), nullptr);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_LENGTH * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_LENGTH * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VERTEX_LENGTH * sizeof(GLfloat), (void *)(8 * sizeof(GLfloat)));
		
		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLint), &indicies[0], GL_STATIC_DRAW);*/
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
}

void CModel::Render(CShaderProgram* sp, glm::vec3* viewPos, glm::vec3* pos, glm::quat* orientation, glm::vec3* scale)
{
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, *pos);

	if (render_type == M_BILLBOARD)
	{
		glm::vec3 look = glm::normalize(*viewPos - *pos);
		glm::vec3 right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), look);
		glm::vec3 up2 = cross(look, right);
		glm::mat4 transform;
		transform[0] = glm::vec4(right, 0);
		transform[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0); //or up2
		transform[2] = glm::vec4(look, 0);
		M = M * transform;
	}
	else if (render_type == M_SMART_SPRITE)
	{
		glDisable(GL_CULL_FACE);
		GLint side = GetSide(*pos - *viewPos, (*orientation * glm::vec3(0.0f, 0.0f, -1.0f)));

		if (side == LEFT)
			M = glm::rotate(M, glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::mat4_cast(*orientation);
		else if (side == RIGHT)
			M = glm::rotate(M, -glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::mat4_cast(*orientation);
		else
			M = M * glm::mat4_cast(*orientation);
	}
	else if (render_type == M_FULLSCREEN)
	{
		glm::vec3 look = glm::normalize(*viewPos - *pos);
		glm::vec3 right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), look);
		glm::vec3 up2 = cross(look, right);
		glm::mat4 transform;
		transform[0] = glm::vec4(right, 0);
		transform[1] = glm::vec4(up2, 0); //or up2
		transform[2] = glm::vec4(look, 0);
		M = M * transform;
	}
	else
	{
		M = M * glm::mat4_cast(*orientation);
	}
	M = glm::scale(M, *scale);

	glBindVertexArray(uiVAO);

	sp->SetMat4(&M[0][0], "M");

	glDrawArrays(RENDER_MODE, 0, nodes);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
}

bool CModelLoader::LoadModel(CModel* m, std::string path)
{
	m->SetRenderMode(GL_TRIANGLES);
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE* file;
	path = "./data/assets/models/" + path + ".obj";
	fopen_s(&file, path.c_str(), "r");
	if (!file)
	{
		std::cout << "Impossible to open the file" << path.c_str() << "!\n";   
		return false;
	}

	m->vertecies.clear();
	m->uvs.clear();
	m->normals.clear();

	while (1)
	{
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) 
		{
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		glm::vec3 vertex = temp_vertices[vertexIndices[i] - 1];
		m->vertecies.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++)
	{
		glm::vec2 vertex = temp_uvs[uvIndices[i] - 1];
		m->uvs.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++)
	{
		glm::vec3 vertex = temp_normals[normalIndices[i] - 1];
		m->normals.push_back(vertex);
	}
	fclose(file);

	return true;
}