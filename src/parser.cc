#include <iostream>
#include "../Include/parser.h"

// Recursive descent top-down parser lookahead(1)
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
void print_token(minid_parser& parser){
    while (parser.cur_token.kind != DONSUS_END){
        std::cout << parser.cur_token << "\n";
        minid_parser_next(parser);
    }
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
    #ifdef DEBUG
    print_token(parser);
    #endif


    minid_ast& result = minid_expr(parser, base);
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

minid_ast& minid_expr(minid_parser& parser, minid_ast& cur_state) {
    // get left part of the expression
    minid_ast& left = minid_term(parser, cur_state);

    cur_state.left = &left;

    // check whether the current character is a scalar or not
    if(minid_peek(parser).kind != DONSUS_PLUS && minid_peek(parser).kind  != DONSUS_MINUS){
        return cur_state;
    }

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

    cur_state.left = &left;

    if(minid_peek(parser).kind != DONSUS_STAR && minid_peek(parser).kind  != DONSUS_SLASH){
        return cur_state;
    }

    while(parser.cur_token.kind == DONSUS_STAR || parser.cur_token.kind == DONSUS_MINUS) {
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
    return left;
}

minid_ast& minid_factor(minid_parser& parser, minid_ast& cur_state){
    //check for lpar, since that will be an expression we can recall minid_expr

    minid_token token = parser.cur_token;
    auto *factor = new minid_ast();

    auto*factor_token = new minid_token();

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

// DEBUG PRINTS


/*struct minid_ast minid_expr(struct minid_parser *parser, struct minid_ast& cur_state);
struct minid_ast minid_term(struct minid_parser *parser);
struct minid_ast minid_factor(struct minid_parser *parser);
struct minid_ast minid_unary(struct minid_parser *parser);*/


// MIGHT USE REFERENCES IN THE FUTURE GLOBALLY

/*struct minid_parser minid_parser_init(struct minid_lexer lexer) {
    struct minid_parser parser;
    parser.lexer = lexer;
    parser.cur_token = minid_lexer_next(&parser);
    parser.error = false;
    return parser;
}

struct minid_token minid_parser_next(struct minid_parser* parser) {
    return parser->cur_token = minid_lexer_next(parser);
}

struct minid_ast& minid_parse(struct minid_parser *parser, struct minid_parser * save, struct minid_ast& base){
    *save = *parser;

x
    // print out tokens if debug is enabled
    #ifdef DEBUG
    while (parser->cur_token.kind != DONSUS_END){
        de_printout_single_token(parser->cur_token);

        minid_parser_next(parser);
    }

    #endif

    *parser = *save; // set back the parser
    // start parsing
    struct minid_ast whole_thing;
    struct minid_ast expr_result =  minid_expr(parser, whole_thing);

    base = expr_result;

    return base;
}*/


/*
struct minid_token minid_peek(struct minid_parser *parser){
    // return next token
    struct minid_lexer save = parser->lexer;
    struct minid_token result = minid_lexer_next(parser);

    parser->lexer = save;
    return result;
}

// expression: term | expression '+' term | expression '-' term
struct minid_ast& minid_expr(struct minid_parser *parser, struct minid_ast& cur_state){
    struct minid_ast left = minid_term(parser); // get leftmost term 5
    //adding left term to ast
    cur_state.left = left;

    if(minid_peek(parser).kind != DONSUS_PLUS && minid_peek(parser).kind  != DONSUS_MINUS){
        return cur_state;
    }

    minid_parser_next(parser);

    while(parser->cur_token.kind == DONSUS_PLUS || parser->cur_token.kind == DONSUS_MINUS){
        switch (parser->cur_token.kind) {

            case DONSUS_PLUS: {
                minid_parser_next(parser); // skip DONSUS_PLUS
                struct minid_ast right = minid_term(parser);
                cur_state.right = right;
                break;
            }

            case DONSUS_MINUS: {
                minid_parser_next(parser); // skip DONSUS_MINUS

*/
/*                // unary here
                if (parser->cur_token.kind == DONSUS_LPAR){
                    std::cout << "Unary operation";
                    return minid_unary(parser);
                }*//*

                //adding operator
                struct minid_ast right = minid_term(parser); // 2
                cur_state.right = right
                break;
            }

            default: break;
        }
    }
    return minid_expr(parser, cur_state);
}

// 5-2-1
// term : factor | term '*' factor | term '/' factor
struct minid_ast minid_term(struct minid_parser *parser, struct minid_ast cur_state) {
    struct minid_ast left = minid_factor(parser); // get leftmost term 5
    // check for next token if it is not STAR OR SLASH ->epsilon
    cur_state.left = left;

    if(minid_peek(parser).kind != DONSUS_STAR && minid_peek(parser).kind != DONSUS_SLASH){
        return cur_state;
    }

    minid_parser_next(parser);

    while (parser->cur_token.kind  == DONSUS_STAR || parser->cur_token.kind == DONSUS_SLASH){
        switch(parser->cur_token.kind){
            case DONSUS_STAR: {
                minid_parser_next(parser);
                struct minid_ast right = minid_factor(parser);
                cur_state.right = right
                break;
            }
            case DONSUS_SLASH: {
                minid_parser_next(parser);
                struct minid_ast right = minid_factor(parser);
                cur_state.right = right
                break;
            }
            default: break;
        }
    }
    return cur_state;
}

struct minid_ast minid_factor(struct minid_parser *parser) {
    //check for lpar, since that will be an expression we can recall minid_expr

    struct minid_token token = parser->cur_token;
    struct minid_ast factor;

    if(token.kind == DONSUS_LPAR) {
        minid_parser_next(parser);
        struct minid_ast& result = minid_expr(parser);
        factor.left = result;
        minid_parser_next(parser, factor);

        if(parser->cur_token.kind != DONSUS_RPAR){
            throw std::invalid_argument("Mismatched parentheses");
        }
        return result;
    }

    int buf;
    std::sscanf(token.value, "%d", &buf);

    factor.value = buf;
    return factor;
}

*/
/*
struct minid_ast minid_unary(struct minid_parser *parser){

    minid_parser_next(parser);
    minid_parser_next(parser);

    int result = minid_expr(parser);

    minid_parser_next(parser);

    if(parser->cur_token.kind != DONSUS_RPAR){
        throw std::invalid_argument("Mismatched parentheses");
    }


    return -result;
}*/

