#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

namespace sparky {

	class FileUtils
	{
	public:
		static std::string read_file(const char* filepath)
		{
			std::ifstream in(filepath);
			if (in)
			{
				std::string contents;
				in.seekg(0, std::ios::end);
				contents.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&contents[0], contents.size());
				in.close();
				return contents;
			}
			throw std::runtime_error("failed to open file");
		}
	};

}
