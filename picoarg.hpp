#ifndef _PICO_ARG_HPP
#define _PICO_ARG_HPP

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
        void add(const std::string& name, bool expectsArg);

        /**
         * Checks whether an option exists.
         *
         * @param name The name of the option
         *
         * @return true if the option exists
         */
        bool has(const std::string& name);

        /**
         * Returns the argument of an option. If an option doesn't exist or it
         * doesn't take an argument, an empty string is returned.
         * Use 'has' to check whether an option exists.
         *
         * @param name The name of the option
         *
         * @return The argument
         */
        std::string popArgument(const std::string& name);

private:
        /**
         * The internal representation of an option. If an option doesn't take
         * an argument, 'argument' contains an empty string.
         */
        struct Option {
                std::string name;
                std::string argument;
                bool expectsArg;
        };

        /**
         * A helper struct to find an option by its name.
         */
        struct Compare {
                Compare(const std::string& str)
                        : str(str)
                {
                }

                bool operator()(const Option& option)
                {
                        return option.name == str;
                }

                std::string str;
        };

        /**
         * Retrieves the name of the option in 'token'. Only works with short
         * options, e.g. '-v'.
         *
         * @param token The token to parse
         *
         * @return The name of the option
         */
        std::string parseName(const std::string& token);

        /**
         * Checks whether 'token' starts with a '-' character.
         *
         * @param The token to check
         *
         * @return True if the token isn't empty and starts with '-'
         */
        bool startsWithDash(const std::string& token);

        std::vector<Option> options;
        std::vector<Option> parsed;
};

#endif // _PICO_ARG_HPP

#ifdef PICOARG_IMPL

bool OptionParser::parse(int& argc, char* argv[])
{
        std::vector<std::string> args(argv + 1, argv + argc);

        for(auto it = args.begin(); it < args.end(); ++it) {
                std::string token = *it;
                std::string next;

                // Get next token if exists
                if (std::next(it) != args.end()) {
                        next = *std::next(it);
                }

                if (!startsWithDash(token)) {
                        // TODO: Error option expected
                        std::cout << "Expected an option, found " << token
                                << std::endl;
                        return false;
                }

                std::string name = parseName(token);
                std::string argument;

                // Try to find the option by name
                auto optionIt = std::find_if(options.begin(),
                                options.end(), Compare(name));

                if (optionIt == options.end()) {
                        // TODO: Error unknown option
                        std::cout << "Unknown option " << name << std::endl;
                        return false;
                }

                Option option = *optionIt;

#ifdef PICOOPTIONS_DEBUG
                std::cout << "Found option '" << option.name << "'"
                        << std::endl;
#endif

                // Has inline argument or is next an argument?
                if (token.length() > 2) {
                        argument = token.substr(2);
                } else if (!startsWithDash(next)) {
                        argument = next;
                        // Skip next element
                        ++it;
                }

#ifdef PICOOPTIONS_DEBUG
                std::cout << "Found argument '" << argument << "'" << std::endl;
#endif

                // Option doesn't expect an argument but an argument was found
                if (argument != "" && !option.expectsArg) {
                        // TODO: Error no argument expected
                        std::cout << "Option '" << option.name
                                << "' doesn't expect an argument" << std::endl;
                        return false;
                }

                // Option expects an argument but no argument was found
                if (argument == "" && option.expectsArg) {
                        // TODO: Error no argument found
                        std::cout << "Option '" << option.name
                                << "' expects an argument" << std::endl;
                        return false;
                }

                // Store the argument
                option.argument = argument;

                // Add to parsed options
                parsed.push_back(option);
        }

        options.clear();
        return true;
}

void OptionParser::add(const std::string& name, bool expectsArg = false)
{
        options.push_back({ name, "", expectsArg });
}

bool OptionParser::has(const std::string& name)
{
        auto it = std::find_if(parsed.begin(), parsed.end(),
                        Compare(name));

        return it != parsed.end();
}

std::string OptionParser::popArgument(const std::string& name)
{
        auto it = std::find_if(parsed.begin(), parsed.end(),
                        Compare(name));

        // TODO: Pop option from 'options' if found

        return it == parsed.end() ? "" : (*it).argument;
}

std::string OptionParser::parseName(const std::string& token)
{
        return token.substr(0, 2);
}

bool OptionParser::startsWithDash(const std::string& token)
{
        return token.empty() ? false : token[0] == '-';
}

#endif // PICOARG_IMPL
