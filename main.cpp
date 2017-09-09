#include <iostream>

#define PICOARG_IMPL
#include "picoarg.hpp"

int main(int argc, char* argv[])
{
        OptionParser parser;
        parser.add('v');
        parser.add('f', true);

        if (!parser.parse(argc, argv)) {
                return -1;
        }

        if (parser.has('v')) {
                std::cout << "version 0.0.1" << std::endl;
        }

        while (parser.has('f')) {
                std::string filename = parser.popArgument('f');
                std::cout << "processing '" << filename << "'" << std::endl;
        }

        return 0;
}
