#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {
	auto GetElapsedTime(const std::filesystem::path& json_file) {
		auto filename = std::filesystem::path(json_file).filename().string();
		filename.resize(filename.find("-hpp_main.cpp.json"));
		std::replace(filename.begin(), filename.end(), '-', '/');
		filename += ".hpp";

		std::ifstream json_stream(json_file);
		auto str = std::string((std::istreambuf_iterator<char>(json_stream)), std::istreambuf_iterator<char>());

		auto offset = str.find("avg ms");
		str = str.substr(offset + 8);
		str.resize(str.find('}'));
		auto elapsed = std::atof(str.c_str());
		
		return std::make_pair(filename, elapsed);
	}
}

int main(int argc, char** argv) {
	auto cmake_files_path = std::filesystem::absolute(std::filesystem::path(argv[0]).root_directory() / "CMakeFiles");

	for(auto&el:std::filesystem::directory_iterator(cmake_files_path)) {
		if(!std::filesystem::is_directory(el))
			continue;
		if(std::filesystem::path(el).string().find(".dir") == std::string::npos)
			continue;

		for (auto& el_file : std::filesystem::directory_iterator(el)) {
			if(std::filesystem::path(el_file).extension().string() == ".json") {
				const auto name_time = GetElapsedTime(el_file);
				std::cout << name_time.first << "\t" << name_time.second << std::endl;
			}
		}
	}
	
	return 0;
}
