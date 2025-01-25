#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stdexcept>

#include "HuffmanCodingTree.hpp"

std::string read_file(std::filesystem::path path) {
    try {
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("File does not exist.");
        }
        if (!std::filesystem::is_regular_file(path)) {
            throw std::runtime_error("Path is not a regular file.");
        }

        std::ifstream f(path, std::ios::in | std::ios::binary);
        if (!f) {
            throw std::runtime_error("Failed to open file.");
        }

        const auto sz = std::filesystem::file_size(path);
        std::string result(sz, '\0');
        f.read(result.data(), sz);

        return result;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error reading file: ") + e.what());
    }
}

std::unordered_map<char, int> calculate_frequency(const std::string& text) {
    std::unordered_map<char, int> frequency;
    for (char c : text) {
        frequency[c]++;
    }
    return frequency;
}

std::vector<std::pair<char, int>> map_to_vector(const std::unordered_map<char, int>& f) {
    return std::vector<std::pair<char, int>>(f.begin(), f.end());
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_name>.txt" << std::endl;
        return 1;
    }
    
    try {
        std::string data = read_file(argv[1]);
        auto frequency = map_to_vector(calculate_frequency(data));
		
        auto root = build_huffman_tree(frequency, frequency.size());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}