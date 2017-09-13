/*
   picoarg.hpp - 1.0.2

   Author:
        Paul Meffle

   Summary:
        picoarg is a single-file header that implements a simple commandline
        option parser.

   Revision history:
        1.0   (15.08.2017) initial release
        1.0.1 (09.09.2017) allow arguments multiple times
        1.0.2 (09.09.2017) use chars for option names
*/

#ifndef _PICOARG_HPP
#define _PICOARG_HPP

#include <vector>
#include <algorithm>

class OptionParser {
public:
        /**
         * Parses the options passed to the program. Each option in 'argv' gets
         * compared with the user added options. If it is found and has an
         * argument, the argument gets parsed aswell.
         *
         * @param argc The option cound
         * @param argv The actual options
         *
         * @return true if successful
         */
        bool parse(int& argc, char* argv[]);

        /**
         * Adds an option to the internal 'options' list that is used by the
         * 'parse' function.
         *
         * @param name The name of the option
         * @param expectsArg Indicates whether the option takes an argument
         */
        void add(const char& name, bool expectsArg);

        /**
         * Checks whether an option exists.
         *
         * @param name The name of the option
         *
         * @return True if the option exists
         */
        bool has(const char& name);

        /**
         * Returns the argument of an option. If an option doesn't exist or it
         * doesn't take an argument, an empty string is returned.
         * Use 'has' to check whether an option exists.
         *
         * @param name The name of the option
         *
         * @return The argument
         */
        std::string popArgument(const char& name);

private:
        /**
         * The internal representation of an option. If an option doesn't take
         * an argument, 'argument' contains an empty string.
         */
        struct Option {
                char name;
                std::string argument;
                bool expectsArg;
        };

        /**
         * A helper struct to find an option by its name.
         */
        struct Compare {
                Compare(const char& name)
                        : name(name)
                {
                }

                bool operator()(const Option& option)
                {
                        return option.name == name;
                }

                char name;
        };

        /**
         * Retrieves the name of the option in 'token'. Only works with short
         * options, e.g. '-v'.
         *
         * @param token The token to parse
         *
         * @return The name of the option
         */
        char parseName(const std::string& token);

        /**
         * Checks whether 'token' is two characters long and starts with a '-'
         * character.
         *
         * @param The token to check
         *
         * @return True if the token is two characters long and starts with '-'
         */
        bool isOption(const std::string& token);

        std::vector<Option> options;
        std::vector<Option> parsed;
};

#endif // _PICOARG_HPP

#ifdef PICOARG_IMPL

bool OptionParser::parse(int& argc, char* argv[])
{
        std::vector<std::string> args(argv + 1, argv + argc);

        for(auto it = args.begin(); it < args.end(); ++it) {
                std::string token = *it;
                std::string next = std::next(it) == args.end() ? ""
                        : *std::next(it);

                if (!isOption(token)) {
                        std::cout << "Expected an option, found '" << token
                                << "'." << std::endl;
                        return false;
                }

                char name = token[1];

                auto optionIt = std::find_if(options.begin(), options.end(),
                                Compare(name));

                if (optionIt == options.end()) {
                        std::cout << "Unknown option '-" << name << "'" << std::endl;
                        return false;
                }

                Option option = *optionIt;

                if (option.expectsArg && (isOption(next) || next.empty())) {
                                std::cout << "Option '-" << name
                                        << "' expects argument." << std::endl;
                                return false;
                }

                if (option.expectsArg && !isOption(next)) {
                        option.argument = next;
                        ++it;
                }

                parsed.push_back(option);
        }

        options.clear();
        return true;
}

void OptionParser::add(const char& name, bool expectsArg = false)
{
        options.push_back({ name, "", expectsArg });
}

bool OptionParser::has(const char& name)
{
        auto it = std::find_if(parsed.begin(), parsed.end(),
                        Compare(name));

        return it != parsed.end();
}

std::string OptionParser::popArgument(const char& name)
{
        auto it = std::find_if(parsed.begin(), parsed.end(),
                        Compare(name));

        std::string arg = it == parsed.end() ? "" : (*it).argument;
        parsed.erase(it);

        return arg;
}

char OptionParser::parseName(const std::string& token)
{
        return token[1];
}

bool OptionParser::isOption(const std::string& token)
{
        return (token.size() == 2 && token[0] == '-');
}

#endif // PICOARG_IMPL

/*
   zlib license:

   Copyright (c) 2017 Paul Meffle http://github.com/sro5h
   This software is provided 'as-is', without any express or implied warranty.
   In no event will the authors be held liable for any damages arising from the
   use of this software.
   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:
     1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in
        a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
     2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
     3. This notice may not be removed or altered from any source distribution.
*/
