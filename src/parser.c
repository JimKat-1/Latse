#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.c"
#include "value.c"

enum TokenKinds {
    SYMBOL,
    STRING,
    NUMBER,
    SPECIAL_CONSTRUCT,
    PAREN_OPEN,
    PAREN_CLOSE,
}

typedef struct {
    TokenKind kind;
    StringView s;
} Token;

Cons
parse(String code) {
    Arena a = Arena_new(code.len/8);

    /* Tokenize */
    size_t i = 0;
    size_t c = 0;
    while (i<code.len) {
        size_t size = push_next_token(a, code+i);
        i += size;
        c++;
    }

    /* Parse */
    return in_paren(a, 0);
}

#define GET_TOKEN(a, i) *(Token*)Arena_vec_index(a, sizeof(Token), i);

Cons
in_paren(Arena a, size_t *i) {
    while () {
        Token t = GET_TOKEN(a, *i)
        if (t.kind == TokenKind.PAREN_OPEN) {
            (*i)++;
            cons(in_paren(a, i), );
        }
    }
}

// Characters that end symbols and numbers
char ENDERS[] = {' ', '(', ')', '\n', '\'', '.', ';', ':'};
// Characters that are ENDERS but if found in the start are starters
char STARTERS[] = {':', ';', '\''}

size_t
push_next_token(Arena a, char *code) {
    StringView s; s.s = code; s.len = 0;
    Token t;

    for (size_t i = 0; s.s[i] == 0; i++) {
        if (IN(s[i], ENDERS)) {
            if (s.len == 0) {
                if (IN(s[i], STARTERS)) {
                    s.len += 1;
                    continue;
                }

                s.s += 1;
                continue;
            } else {
                s.len += 1;
                t.kind = token_kind(s);
                ARENA_PUSH(a, t);
                return s.len;
            }
        }

        s.len += 1;
    }

    return 0;
}

TokenKind
token_kind(StringView s) {
    bool number = true;
    for (usize_t i = 0; i<s.len; i++) {
        if (!(s.s[i] >= '0' && s.s[i] <= '9')) {
            number = false;
            break;
        }
    }
    if (number) return TokenKinds.NUMBER;
    if (s.s[0] == '.' || s.s[0] == '\'') return TokenKinds.SPECIAL_CONSTRUCT;
    if (s.len == 1) return TokenKinds.SYMBOL;
    if (s.len == 2) {
        if (s.s[0] == ':' && s.s[1] == ':') {
            return TokenKinds.SPECIAL_CONSTRUCT;
        } else {
            return TokenKinds.SYMBOL;
        }
    }

    number = true;
    if (s.s[0] == '0') {
        switch (s.s[1]) {
            case 'b':
                for (usize_t i = 2; i<s.len; i++) {
                    char c = s.s[i];
                    if (!(c=='1' || c=='0')) {
                        number = false;
                        break;
                    }
                }
                break;
            case 'o':
                for (usize_t i = 2; i<s.len; i++) {
                    char c = s.s[i];
                    if (!(c>='0' && c<='7')) {
                        number = false;
                        break;
                    }
                }
                break;
            case 'x':
                for (usize_t i = 2; i<s.len; i++) {
                    char c = s.s[i];
                    if (!((c>='0' && c<='9') || (c>='A' && c<='F') || (c>='a' && c<='f'))) {
                        number = false;
                        break;
                    }
                }
                break;
        }
    }

    if (number) return TokenKinds.NUMBER;
    return TokenKinds.SYMBOL;
}
