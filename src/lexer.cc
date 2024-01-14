#include <iostream>
#include "../Include/parser.h"
#define TOKEN_MAX_STRLEN 100

minid_token* token_init(minid_token_kind kind,unsigned length ,unsigned line, std::string value,  minid_token * token){
    token->kind = kind;
    token->length = length;
    token->line = line;
    token->value = value;
    return token;
}

std::string de_get_name_from_token(minid_token_kind kind) {
    switch (kind) {
        case DONSUS_NAME: return "DONSUS_NAME";
        case DONSUS_NUMBER: return "DONSUS_NUMBER";
        case DONSUS_STRING: return "DONSUS_STRING";
        case DONSUS_NEWLINE: return "DONSUS_NEWLINE";
        case DONSUS_LPAR: return "DONSUS_LPAR";
        case DONSUS_RPAR: return "DONSUS_RPAR";
        case DONSUS_LSQB: return "DONSUS_LSQB";
        case DONSUS_RSQB: return "DONSUS_RSQB";
        case DONSUS_COLO: return "DONSUS_COLO";
        case DONSUS_COMM: return "DONSUS_COMM";
        case DONSUS_PLUS: return "DONSUS_PLUS";
        case DONSUS_PLUS_EQUAL: return "DONSUS_PLUS_EQUAL";
        case DONSUS_MINUS: return "DONSUS_MINUS";
        case DONSUS_MINUS_EQUAL: return "DONSUS_MINUS_EQUAL";
        case DONSUS_STAR: return "DONSUS_STAR";
        case DONSUS_STAR_EQUAL: return "DONSUS_STAR_EQUAL";
        case DONSUS_SLASH: return "DONSUS_SLASH";
        case DONSUS_SLASH_EQUAL: return "DONSUS_SLASH_EQUAL";
        case DONSUS_LESS: return "DONSUS_LESS";
        case DONSUS_LESS_EQUAL: return "DONSUS_LESS_EQUAL";
        case DONSUS_GREATER: return "DONSUS_GREATER";
        case DONSUS_GREATER_EQUAL: return "DONSUS_GREATER_EQUAL";
        case DONSUS_EQUAL: return "DONSUS_EQUAL";
        case DONSUS_DOUBLE_EQUAL: return "DONSUS_DOUBLE_EQUAL";
        case DONSUS_DOT: return "DONSUS_DOT";
        case DONSUS_PERCENT: return "DONSUS_PERCENT";
        case DONSUS_LBRACE: return "DONSUS_LBRACE";
        case DONSUS_RBRACE: return "DONSUS_RBRACE";
        case DONSUS_CIRCUMFLEX: return "DONSUS_CIRCUMFLEX";
        case DONSUS_EXCLAMATION: return "DONSUS_EXCLAMATION";
        case DONSUS_COMMENT: return "DONSUS_COMMENT";
        case DONSUS_SINGLE_QUOTE : return "DONSUS_SINGLE_QUOTE";
        case DONSUS_DOUBLE_QUOTE : return "DONSUS_DOUBLE_QUOTE";
        case DONSUS_THREE_DOTS: return "DONSUS_THREE_DOTS";
        case DONSUS_END: return "DONSUS_END";

        default:
            return "UNKNOWN_TOKEN_KIND";
    }
}

static bool isstart_identifier(char c){
    // entry point of an identifier
    return isalpha(c) || c == '_';
}

static bool iscontinue_identifier(char c){
    // continue point of an identifier
    return isstart_identifier(c) || isdigit(c);
}

minid_lexer peak_for_token(minid_parser * parser){
     minid_lexer result;

     minid_lexer old_lexer = parser->lexer;
     minid_lexer new_token = minid_new_lexer(parser->lexer.string);

    result = new_token;
    parser->lexer = old_lexer;

    return result;
}


char peak_for_char(minid_parser * parser){
    char next_char = parser->lexer.string[parser->lexer.cur_pos + 1];

    if (next_char == '\0') {
        return '\0';
    }

    return next_char;
}

bool eat(minid_parser &parser){
    if((parser.lexer.cur_char = parser.lexer.string[++parser.lexer.cur_pos]) != '\0') {
        return true;
    }

    return false;
}

static std::string get_text_between_pos(minid_parser &parser, int start, int end) {
    // returns string from the starting point to the end
    return {std::begin(parser.lexer.string) + start, std::begin(parser.lexer.string) + end};

}

static std::string next_number(minid_parser& parser,minid_token * token, int start_pos) {
    // TBD: we are checking isdigit 2 times here. We can't eat.

    while (isdigit(parser.lexer.cur_char)){
        token->length++;
        eat(parser);
    }
    return get_text_between_pos(parser, start_pos, parser.lexer.cur_pos);
}

static std::string next_identifier(minid_parser& parser, minid_token * token, int start_pos){
    while (iscontinue_identifier(parser.lexer.cur_char)){
        token->length++;
        eat(parser);
    }
    return get_text_between_pos(parser, start_pos, parser.lexer.cur_pos);
}

minid_token minid_lexer_next(minid_parser& parser) {
    minid_token token;
    minid_token *cur_token = token_init(DONSUS_END, 0, 0, "", &token);

    switch (parser.lexer.cur_char) {
        case '\r':
        case '\t':
        case ' ': {
            if (!eat(parser)) {
                cur_token->kind = DONSUS_END;
                cur_token->line = parser.lexer.cur_line;
                return *cur_token;
            }
            break;
        }
        case '\n': {
            cur_token->line = ++parser.lexer.cur_line;
            cur_token->kind = DONSUS_NEWLINE;
            cur_token->length = 1;
            eat(parser);
            return *cur_token;
        }
        case '+': {
            cur_token->kind = DONSUS_PLUS;
            cur_token->length = 1;
            cur_token->value = "+";
            cur_token->line = parser.lexer.cur_line;
            eat(parser);
            return *cur_token;
        }

        case '-': {
            cur_token->kind = DONSUS_MINUS;
            cur_token->length = 1;
            cur_token->value = "-";
            cur_token->line = parser.lexer.cur_line;
            eat(parser);
            return *cur_token;
        }

        case '*': {
            cur_token->kind = DONSUS_STAR;
            cur_token->length = 1;
            cur_token->value = "*";
            cur_token->line = parser.lexer.cur_line;
            eat(parser);
            return *cur_token;
        }

        case '/': {
            cur_token->kind = DONSUS_SLASH;
            cur_token->length = 1;
            cur_token->value = "/";
            cur_token->line = parser.lexer.cur_line;
            eat(parser);
            return *cur_token;
        }

        case '^': {
            cur_token->kind = DONSUS_CIRCUMFLEX;
            cur_token->length = 1;
            cur_token->value = "^";
            cur_token->line = parser.lexer.cur_line;
            eat(parser);
            return *cur_token;
        }

        case '=': {
            cur_token->kind = DONSUS_EQUAL;
            cur_token->length = 1;
            cur_token->value = "=";
            cur_token->line = parser.lexer.cur_line;
            eat(parser);
            return *cur_token;
        }

        case '(': {
            cur_token->kind = DONSUS_LPAR;
            cur_token->length = 1;
            cur_token->value = "(";
            cur_token->line = parser.lexer.cur_line;
            eat(parser);
            return *cur_token;
        }

        case ')': {
            cur_token->kind = DONSUS_RPAR;
            cur_token->length = 1;
            cur_token->value = ")";
            cur_token->line = parser.lexer.cur_line;
            eat(parser);
            return *cur_token;
        }

        default: {

            // numbers
            if (isdigit(parser.lexer.cur_char)) {
                cur_token->kind = DONSUS_NUMBER;
                cur_token->length = 0; // will be changed during next_number
                cur_token->value = next_number(parser, cur_token, parser.lexer.cur_pos);
                cur_token->line = parser.lexer.cur_line;
                return *cur_token;
            }

            // identifiers
            if (isstart_identifier(parser.lexer.cur_char)) {
                cur_token->kind = DONSUS_NAME;
                cur_token->length = 0; // will be changed during next_number
                cur_token->value = next_identifier(parser, cur_token, parser.lexer.cur_pos);
                cur_token->line = parser.lexer.cur_line;
                return *cur_token;
            }

            // if none of them are true we end the execution.(error|end of the file)
            return *cur_token;
        }
    }
}

minid_lexer minid_new_lexer(std::string string){
    minid_lexer lexer;
    lexer.string = string;
    lexer.cur_pos = 0;
    lexer.cur_line = 1;
    lexer.cur_char = lexer.string[0];

    return lexer;
}