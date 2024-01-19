#include <iostream>
#include "../Include/file.h"
#include "../Include/parser.h"

int Du_Main(int argc, char**argv){
    std::string result = handle_file(argv);
    minid_parser save;

    auto* base = new minid_ast();

    minid_lexer lexer = minid_new_lexer(result);
    minid_parser parser = minid_parser_init(lexer);


   minid_parse(parser, *base);

    return 0;

}
