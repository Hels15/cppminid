#include <iostream>
#include <memory>

#ifndef PARSER_H
#define PARSER_H

#define DEBUG 1

typedef enum{
    DONSUS_NAME, // IDENTIFIER
    DONSUS_NUMBER, // 69
    DONSUS_STRING, // "hello world"
    DONSUS_NEWLINE, // \n
    DONSUS_INDENT, //----something here
    DONSUS_DEDENT, // first token after indent
    DONSUS_END, // mark the end of the code

    DONSUS_LPAR, // (
    DONSUS_RPAR, // )
    DONSUS_LSQB, // [
    DONSUS_RSQB, // ]
    DONSUS_COLO, // :
    DONSUS_COMM,  // ,
    DONSUS_PLUS, // +
    DONSUS_PLUS_EQUAL, // +=
    DONSUS_MINUS, // -
    DONSUS_MINUS_EQUAL, // -=
    DONSUS_STAR, // *
    DONSUS_STAR_EQUAL, // *=
    DONSUS_SLASH, // /
    DONSUS_SLASH_EQUAL, // /=
    DONSUS_LESS, // x < y
    DONSUS_LESS_EQUAL, // x <= y
    DONSUS_GREATER, // x > y
    DONSUS_GREATER_EQUAL, // x >= y
    DONSUS_EQUAL, // =
    DONSUS_DOUBLE_EQUAL, // ==
    DONSUS_DOT, // .
    DONSUS_PERCENT, // %
    DONSUS_LBRACE, // {
    DONSUS_RBRACE, // }
    DONSUS_CIRCUMFLEX, // ^
    DONSUS_INCREMENT, // ++
    DONSUS_DECREMENT, // --
    DONSUS_EXCLAMATION, // !
    DONSUS_COMMENT, // #
    DONSUS_SINGLE_QUOTE, // '
    DONSUS_DOUBLE_QUOTE, // "
    DONSUS_THREE_DOTS, // ...
    DONSUS_NULL_VALUE
} minid_token_kind;

struct minid_token{
    minid_token_kind kind;
    std::string value;
    unsigned length;
    unsigned line;
};

// AST implementation
struct minid_ast{
    minid_token value;
    minid_ast *left = nullptr;
    minid_ast *right = nullptr;
};

struct minid_lexer {
    std::string string;
    char cur_char;
    unsigned int cur_pos, cur_line;
};

struct minid_parser{
    bool error;
    minid_token cur_token;
    minid_lexer lexer;
};

/*typedef struct ParseTree ParseTree;

struct ParseTree{struct minid_token *token; ParseTree * left; ParseTree * right;};*/

minid_token minid_lexer_next(minid_parser& parser);
minid_lexer minid_new_lexer(std::string string);

void de_printout_single_token(minid_token);
std::string de_get_name_from_token(minid_token_kind kind);
minid_parser& minid_parser_init(minid_lexer& lexer);
minid_token minid_parser_next(minid_parser &parser);
minid_ast& minid_parse(minid_parser& parser, minid_ast& base);
#endif // PARSER_H