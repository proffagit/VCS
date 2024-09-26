#include <iostream>
#include <filesystem>
#include <fstream>
#include "json.hpp"

// g++ -std=c++17 -I ./json.hpp main.cpp -o out

namespace fs = std::filesystem;
using json = nlohmann::json;

// Helper function to create inverted JSON tree structure with specific order
json createInvertedJsonTree(const fs::path& path) {

    json tree;
    
    // Set "name" first
    tree["name"] = path.filename().string();
    
    // Set "type" second
    tree["type"] = fs::is_directory(path) ? "directory" : "file";

    // SET "hash" FIELD (NEWLY ADDED)
    tree["hash"] = "#"; 


    // SET "TAG" FIELD AS EMPTY ARRAY (NEWLY ADDED)
    tree["tag"] = json::array(); // Initialize as an empty array
    
    // Set "parent" last, only for directories
    if (fs::is_directory(path)) {

        json parent = json::array();

        for (const auto& entry : fs::directory_iterator(path)) {
            parent.push_back(createInvertedJsonTree(entry.path()));
        }
        tree["parent"] = parent;
        
    }
    
    return tree;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <FOLDER_PATH> [JSON_PATH]" << std::endl;
        return 1;
    }

    std::string FOLDER_PATH = argv[1];
    std::string JSON_PATH = (argc > 2) ? argv[2] : "./tree.json";

    json folderTree = createInvertedJsonTree(FOLDER_PATH);
    
    // Write JSON to file
    std::ofstream jsonFile(JSON_PATH);
    jsonFile << std::setw(4) << folderTree << std::endl;
    
    std::cout << "Folder structure has been mapped to " << JSON_PATH << std::endl;
    
    return 0;
}
