#include <iostream>

#define PICOARG_IMPL
#include "picoarg.hpp"

int main(int argc, char* argv[])
{
        OptionParser parser;
        parser.add('h');
        parser.add('v');
        parser.add('f', true);

        if (!parser.parse(argc, argv)) {
                return -1;
        }

        if (parser.has('h')) {
                std::cout << "Usage: a.out [OPTION]" << std::endl;
                std::cout << "  -v        show version information" << std::endl;
                std::cout << "  -f<file>  process <file>" << std::endl;
                return 0;
        }

        if (parser.has('v')) {
                std::cout << "version 0.0.1" << std::endl;
        }

        while (parser.has('f')) {
                std::string filename = parser.popValue('f');
                std::cout << "processing '" << filename << "'" << std::endl;
        }

        return 0;
}
