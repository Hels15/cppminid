#include <iostream>
#include "../Include/runner.h"

#define VERSION_STRING "0.0.1"

const char* USAGE_TEXT = "TBD";

void print_usage(void){
    std::cout << USAGE_TEXT << std::endl;
}

int main(int argc, char** argv){
    if (argc <= 1) print_usage();
    std::cout << "DONSUS_COMPILER: " << VERSION_STRING << std::endl;

    return Du_Main(argc, argv);
}