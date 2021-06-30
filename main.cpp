#include <algorithm>
#include <cmath>
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
		auto boost_ending = filename.find("-hpp_main.cpp.json");
		if (boost_ending == std::string::npos)
			filename.resize(filename.find("_main.cpp.json"));
		else  {
			filename.resize(boost_ending);		
			std::replace(filename.begin(), filename.end(), '-', '/');
			filename += ".hpp";
		}

		std::ifstream json_stream(json_file);
		auto str = std::string((std::istreambuf_iterator<char>(json_stream)), std::istreambuf_iterator<char>());

		auto offset = str.find("avg ms");
		str = str.substr(offset + 8);
		str.resize(str.find('}'));
		auto elapsed = std::atof(str.c_str());

		header_time_map[filename].push_back(elapsed);
	}
	
	template <typename T>
	void PrintTop5(const std::string& name, T& vector, double baseline, std::ostream& table) {
		std::partial_sort(vector.begin(), vector.begin() + std::min(static_cast<int>(vector.size()), 5), vector.end(), [](auto&lhs, auto&rhs) {
			return lhs.second > rhs.second;
		});
		
		table << "# Top-5 " << name << " headers signal compilation impact" << std::endl;
		table << "| Header 	| Time, ms 	| Relative slowdown 	|" << std::endl;
		table << "|-	|-	|-	|" << std::endl;
		for (std::size_t i = 0; i < std::min(static_cast<int>(vector.size()), 5); ++i)
			table << "|" << vector[i].first << "\t|" << vector[i].second << "\t|" << vector[i].second / baseline << "\t|" << std::endl;
			
		table << std::endl << std::endl << std::endl;
	}
	
	template <typename T>
	void PrintTable(const std::string& name, T& vector, double baseline, std::ostream& table) {
		std::sort(vector.begin(), vector.end(), [](auto&lhs, auto&rhs) {
			return lhs.first < rhs.first;
		});
		
		table << "# " << name << " headers signal compilation impact" << std::endl;

		table << "| Header 	| Time, ms 	| Relative slowdown 	|" << std::endl;
		table << "|-	|-	|-	|" << std::endl;
		

		for (auto& el : vector) 
			table << "|" << el.first << "\t|" << el.second << "\t|" << el.second / baseline << "\t|" << std::endl;
		
		table << std::endl << std::endl << std::endl;
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
		std::ostream* table_ptr = &std::cout;
		if (markdown_table.is_open()) 
			table_ptr = &markdown_table;
		
		std::vector<std::pair<std::string, double>> sorted_times_stl;
		std::vector<std::pair<std::string, double>> sorted_times_boost;
		double baseline = -1;
		for (auto& el : header_time_map) {
			auto time = std::accumulate(el.second.begin(), el.second.end(), 0.0);
			time /= el.second.size();
			if (el.first.find("baseline") != std::string::npos)
				baseline = time;
			
			if (el.first.find("boost") != std::string::npos)
				sorted_times_boost.emplace_back(el.first, time);
			else
				sorted_times_stl.emplace_back(el.first, time);	
		}
		
		PrintTop5("STL", sorted_times_stl, baseline, *table_ptr);
		PrintTop5("Boost", sorted_times_boost, baseline, *table_ptr);
		PrintTable("STL", sorted_times_stl, baseline, *table_ptr);
		PrintTable("Boost", sorted_times_boost, baseline, *table_ptr);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
