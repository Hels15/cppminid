#include <iostream>
#include "../Include/parser.h"

// Recursive descent top-down parser lookahead(1)
// Only works with Binary expressions

minid_ast& minid_term(minid_parser& parser,  minid_ast& cur_state);
minid_ast& minid_expr(minid_parser& parser, minid_ast& cur_state);
minid_ast& minid_factor(minid_parser& parser, minid_ast& cur_state);

minid_token minid_parser_next(minid_parser& parser){
    return parser.cur_token = minid_lexer_next(parser);
}

std::ostream& operator<<(std::ostream &o, minid_token& token){
    o << "Value: " << token.value << "\n";
    o << "Kind: " << de_get_name_from_token(token.kind) << "\n";
    o << "Length: " << token.length << "\n";
    o << "Line: " << token.line << "\n";
    return o;
}

// DEBUG
// Todo: Add operators to AST
static
void print_token(minid_parser& parser){
    while (parser.cur_token.kind != DONSUS_END){
        std::cout << parser.cur_token << "\n";
        minid_parser_next(parser);sched_yield();
    }
}

static
void print_ast(minid_parser& parser, minid_ast& result) {
    std::cout << '\n' << result.left->value; // call custom operator <<
    std::cout << '\n' << result.right->value; // call custom operator <<
}

minid_parser& minid_parser_init(minid_lexer& lexer){
    // This function initialises the parser returning a unique pointer.
    auto parser = new minid_parser();
    parser->lexer = lexer; // setting up the lexer struct
    parser->cur_token = minid_lexer_next(*parser); // Initialise current token(starting token)
    parser->error = false;
    return *parser;
}

minid_ast& minid_parse(minid_parser& parser,minid_ast& base){
    // start parsing
    // PRINT OUT TOKENS
/*    #ifdef DEBUG
*//*    print_token(parser);*//*
    #endif*/
    minid_ast& result = minid_expr(parser, base);
    // PRINT OUT THE AST
    #ifdef DEBUG
    // G(result) root (first one)
    print_ast(parser, result);
    #endif
    return result;
    // back-tracing with result
}

// MIGHT BE NOT PERFECT
minid_token minid_peek(minid_parser& parser){
    // return next token
    minid_lexer save = parser.lexer;
    minid_token result = minid_lexer_next(parser);
    parser.lexer = save;
    return result;
}

// expr | term
minid_ast& minid_expr(minid_parser& parser, minid_ast& cur_state) {
    // get left part of the expression
    minid_ast& left = minid_term(parser, cur_state);

    if(minid_peek(parser).kind != DONSUS_PLUS  && minid_peek(parser).kind != DONSUS_MINUS){
        return cur_state;
    }

    cur_state.left = &left;  // error here
    // check whether the current character is a scalar or not

    minid_parser_next(parser);
    // Check whether the current token is a plus or a minus

    while(parser.cur_token.kind == DONSUS_PLUS || parser.cur_token.kind == DONSUS_MINUS) {
        switch (parser.cur_token.kind) {
            case DONSUS_PLUS: {
                minid_parser_next(parser); // skip DONSUS_PLUS
                minid_ast& right = minid_term(parser, cur_state);
                cur_state.right = &right;
                break;
            }

            case DONSUS_MINUS: {
                minid_parser_next(parser); // skip DONSUS_MINUS
                minid_ast& right = minid_term(parser, cur_state);
                cur_state.right = &right;
                minid_parser_next(parser); // skip DONSUS_MINUS
                break;
            }

            default: break;
        }
    }
    return minid_expr(parser, cur_state);
}

minid_ast& minid_term(minid_parser& parser, minid_ast& cur_state){
    minid_ast& left = minid_factor(parser, cur_state);
    if(minid_peek(parser).kind != DONSUS_STAR && minid_peek(parser).kind  != DONSUS_SLASH){
        return left;
    }
    cur_state.left = &left;
    minid_parser_next(parser);

    while(parser.cur_token.kind == DONSUS_STAR || parser.cur_token.kind == DONSUS_SLASH) {
        switch(parser.cur_token.kind){
            case DONSUS_STAR: {
                minid_parser_next(parser);
                minid_ast& right = minid_factor(parser, cur_state);
                cur_state.right = &right;
                break;
            }
            case DONSUS_SLASH: {
                minid_parser_next(parser);
                minid_ast& right = minid_factor(parser, cur_state);
                cur_state.right = &right;
                break;
            }
            default: break;
        }
    }

    return cur_state;
}

minid_ast& minid_factor(minid_parser& parser, minid_ast& cur_state){
    //check for lpar, since that will be an expression we can recall minid_expr
    // memory leaks -> use unique pointer

    auto token = parser.cur_token;
    auto *factor = new minid_ast(); // create an ast node on the heap

    if(token.kind == DONSUS_LPAR) {
        minid_parser_next(parser);
        minid_ast& result = minid_expr(parser, cur_state);
        factor->left = &result;
        minid_parser_next(parser);

        if(parser.cur_token.kind != DONSUS_RPAR){
            throw std::invalid_argument("Mismatched parentheses");
        }
        return result;
    }
    factor->value = token;
    return *factor;
}