/*
   picoarg.hpp - 1.1.1

   Author:
        Paul Meffle

   Summary:
        picoarg is a single-file header that implements a simple commandline
        option parser.

   Revision history:
        1.0   (15.08.2017) initial release
        1.0.1 (09.09.2017) allow values multiple times
        1.0.2 (09.09.2017) use chars for option keys
        1.1.0 (13.09.2017) remove parsing of inline values
        1.1.1 (13.09.2017) apply new naming convention
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
         * value, the value gets parsed aswell.
         *
         * @param argc The option count
         * @param argv The actual options
         *
         * @return True if successful
         */
        bool parse(int& argc, char* argv[]);

        /**
         * Adds an option to the internal 'options' list that is used by the
         * 'parse' function.
         *
         * @param key The name of the option
         * @param expectsValue Indicates whether the option takes a value
         */
        void add(const char& key, bool expectsValue);

        /**
         * Checks whether an option exists.
         *
         * @param key The name of the option
         *
         * @return True if the option exists
         */
        bool has(const char& key);

        /**
         * Returns the value of an option. If an option doesn't exist or it
         * doesn't take a value, an empty string is returned.
         * Use 'has' to check whether an option exists.
         *
         * @param key The name of the option
         *
         * @return The value
         */
        std::string popValue(const char& key);

private:
        /**
         * The internal representation of an option. If an option doesn't take
         * a value, 'value' contains an empty string.
         */
        struct Option {
                char key;
                std::string value;
                bool expectsValue;
        };

        /**
         * A helper struct to find an option by its key.
         */
        struct Compare {
                Compare(const char& key)
                        : key(key)
                {
                }

                bool operator()(const Option& option)
                {
                        return option.key == key;
                }

                char key;
        };

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

                if (!isOption(token)) {
                        std::cout << "Expected an option, found '" << token
                                << "'" << std::endl;
                        return false;
                }

                char key = token[1];

                auto optionIt = std::find_if(options.begin(), options.end(),
                                Compare(key));

                if (optionIt == options.end()) {
                        std::cout << "Unknown option '-" << key << "'" << std::endl;
                        return false;
                }

                Option option = *optionIt;

                if (option.expectsValue && token.size() <= 2) {
                        std::cout << "Missing value after '-" << key << "'"
                                << std::endl;
                        return false;
                }

                if (!option.expectsValue && token.size() > 2) {
                        std::cout << "Option '-" << key << "' doesn't allow"
                                << " a value" << std::endl;
                        return false;
                }

                if (option.expectsValue && token.size() > 2) {
                        option.value = token.substr(2);
                }

                parsed.push_back(option);
        }

        options.clear();
        return true;
}

void OptionParser::add(const char& key, bool expectsValue = false)
{
        options.push_back({ key, "", expectsValue });
}

bool OptionParser::has(const char& key)
{
        auto it = std::find_if(parsed.begin(), parsed.end(),
                        Compare(key));

        return it != parsed.end();
}

std::string OptionParser::popValue(const char& key)
{
        auto it = std::find_if(parsed.begin(), parsed.end(),
                        Compare(key));

        std::string value = it == parsed.end() ? "" : (*it).value;
        parsed.erase(it);

        return value;
}

bool OptionParser::isOption(const std::string& token)
{
        return (token.size() > 1 && token[0] == '-');
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
