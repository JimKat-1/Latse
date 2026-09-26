char parser__endsof_atom[] = {' ', ')', '.'};

LatseValue parser__in_string(char code[], size_t i) {
    if code[i] == 0 { return LatseErr; }

}

// get in here when finding a num, could become a symbol for example 1+ is a
// symbol even though it starts with a number
LatseValue parser__in_num(char code[], size_t i) {
    if code[i] == 0 { return LatseErr; }

}

LatseValue parser__in_symbol(char code[], size_t i) {
    for (int i = 0; i<code.len; i++) {
        c = code[i];

    }
}

//LatseValue parser__in_paren(char code[], size_t i) {
//    if code[i] == 0 { return LatseErr; }
//
//    for (int i = 0; i<code.len; i++)
//        c = code[i];
//
//        if () {
//
//        }
//
//        switch c {
//            case '(':
//                parser__in_paren(code,i+1);
//                break;
//            case ')':
//                break;
//        }
//    }
//}

