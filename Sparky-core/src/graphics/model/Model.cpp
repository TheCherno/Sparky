#include "Model.h"

namespace sparky { namespace graphics { 

	Model::Model(const std::string& path, float scale)
		: m_Path(path), m_Scale(scale)
	{
		load();
	}

	void Model::load()
	{
		FILE* file = fopen(m_Path.c_str(), "rt");

		int res;
		char header[128];
		while ((res = fscanf(file, "%s", header)) != EOF)
		{
			if (strcmp(header, "v") == 0)
			{
				maths::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				std::cout << "Vertex: " << vertex.x << ", " << vertex.y << ", " << vertex.z << std::endl;
			}
			else if (strcmp(header, "vt") == 0)
			{
				maths::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				std::cout << "UV: " << uv.x << ", " << uv.y << std::endl;
			}
			else if (strcmp(header, "vn") == 0)
			{
				maths::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				std::cout << "Normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
			}
		}
	}

}
}