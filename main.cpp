#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

namespace {
	std::map<std::string, std::vector<double>> header_time_map;
	
	void GetElapsedTime(const std::filesystem::path& json_file) {
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

		header_time_map[filename].push_back(elapsed);
	}
}

int main(int argc, char** argv) {
	try {		
		auto files_path = std::filesystem::absolute(std::filesystem::path(argv[0]).remove_filename());

		for (auto build : std::filesystem::directory_iterator(files_path)) {
			auto build_path = std::filesystem::path(build) / "CMakeFiles";

			if (!std::filesystem::exists(build_path))
				continue;

			for (auto& el : std::filesystem::directory_iterator(build_path)) {
				if (!std::filesystem::is_directory(el))
					continue;


				if (std::filesystem::path(el).string().find(".dir") == std::string::npos)
					continue;

				for (auto& el_file : std::filesystem::directory_iterator(el)) {
					if (std::filesystem::path(el_file).extension().string() == ".json") {
						GetElapsedTime(el_file);
					}
				}
			}
		}
		
		auto markdown_table = std::ofstream("check_compile_times.wiki/Home.md");
		
		markdown_table << "# Boost headers signal compilation impact" << std::endl;

		markdown_table << "| Header 	| Time, ms 	|" << std::endl;
		markdown_table << "|-	|-	|" << std::endl;
		
		std::vector<std::pair<std::string, double>> sorted_times;
		for (auto& el : header_time_map) {
			auto time = std::accumulate(el.second.begin(), el.second.end(), 0.0);
			time /= el.second.size();
			sorted_times.emplace_back(el.first, time);
			markdown_table << "|" << el.first << "\t|" << time << "\t|" << std::endl;
		}
		
		markdown_table << std::endl << std::endl << std::endl;
		std::partial_sort(sorted_times.begin(), sorted_times.begin() + 5, sorted_times.end(), [](auto&lhs, auto&rhs) {
			return lhs.second > rhs.second;
		});
		sorted_times.resize(5);
		
		markdown_table << "# Top-5 boost headers signal compilation impact" << std::endl;
		markdown_table << "| Header 	| Time, ms 	|" << std::endl;
		markdown_table << "|-	|-	|" << std::endl;
		for (auto& el : sorted_times) {
			markdown_table << "|" << el.first << "\t|" << el.second << "\t|" << std::endl;
		}
		
		
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
