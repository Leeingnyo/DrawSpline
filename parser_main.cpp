#include <iostream>

#include "parser.h"

int main(int argc, char** argv) {
    Parser parser;
    parser.Parse(argv[1]);
	return 0;
}
