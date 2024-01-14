#include <fstream>
#include <iostream>

std::size_t get_file_size(std::ifstream& file) {
    file.seekg(0, file.end); // Go to the end of the file
    std::size_t size = file.tellg(); // Read the current position
    file.seekg(0, file.beg);  // Go to the beginning of the file
    return size;
}


std::string handle_file(char **argv){
    char *path = argv[1]; // the path of the file
    std::ifstream in(path, std::ios_base::in);

    if (!in){
        std::cout << "FILE DOES NOT EXIST AT THIS PATH" << std::endl;
        exit(1);
    }

    std::size_t size = get_file_size(in);
    std::string result = (char*)malloc(size + 1);

    std::string str(size, '\0'); // string buffer

    if (in.read(&str[0],  size))
        result = str;

    return result;
}

