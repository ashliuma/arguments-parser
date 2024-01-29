#include "ArgParser.h"


using argument = std::variant<IntValues, StringValues, BoolValues>;

template <typename T>
bool CorrectType(const argument& arg) {
    return std::holds_alternative<T>(arg);
}

bool ArgumentParser::ArgParser::IsNumeric(const std::string& value) {
    if (!value.empty() && (value[0] == '-' || isdigit(value[0]))) {
        for (size_t i = 1; i < value.size(); ++i) {
            if (!isdigit(value[i])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool ArgumentParser::ArgParser::EqualArg(const std::string& parameter, argument& variable) const {
    std::pair<std::string, std::string> args;

    if (CorrectType<IntValues>(variable)) {
        args = reinterpret_cast<IntValues&>(variable).GetArguments();

    } else if (CorrectType<StringValues>(variable)) {
        args = reinterpret_cast<StringValues&>(variable).GetArguments();

    } else {
        args = reinterpret_cast<BoolValues&>(variable).GetArguments();
    }
    return parameter == args.first || parameter == args.second;
}

bool ArgumentParser::ArgParser::EqualArg(char parameter, argument& arg) const {
    return EqualArg(std::string(1, parameter), arg);
}

void ArgumentParser::ArgParser::AddPositional(std::vector<argument>& arguments, const std::string& value) {
    static argument* argptr;

    if (argptr == nullptr) {
        for (argument& i: arguments) {
            bool correct_int = CorrectType<IntValues>(i) && reinterpret_cast<IntValues&>(i).IsPositional();
            bool correct_string = CorrectType<StringValues>(i) && reinterpret_cast<StringValues&>(i).IsPositional();
            if (correct_int || correct_string) {
                argptr = &i;
                break;
            }
        }

        if (argptr == nullptr) {
            std::cerr << "Error: No positional argument was given for the value: " + value << std::endl;
            exit(1);
        }
        if (CorrectType<StringValues>(*argptr)) {
            reinterpret_cast<StringValues&>(*argptr).Add(value);

        } else {
            if (!IsNumeric(value)) {
                std::cerr << "Invalid argument: The parameter is only for integers, but the value is not an integer." << std::endl;
                exit(1);
            }
            reinterpret_cast<IntValues&>(*argptr).Add(std::stoll(value));
        }

    } else {
        if (CorrectType<StringValues>(*argptr)) {
            reinterpret_cast<StringValues&>(*argptr).Add(value);
        } else {
            if (!IsNumeric(value)) {
                std::cerr << "Invalid argument: The parameter is only for integers, but value is not an integer." << std::endl;
                exit(1);
            }
            reinterpret_cast<IntValues&>(*argptr).Add(std::stoll(value));
        }
    }
}

ArgumentParser::ArgParser::ArgParser() {
    name_ = "Parser";
}

ArgumentParser::ArgParser::ArgParser(const std::string& name, const std::string& description) {
    this->name_ = name;
    this->description_ = description;
}

IntValues& ArgumentParser::ArgParser::AddIntArgument(const char short_arg, const std::string&  full_arg, const std::string& description) {
    IntValues argument(short_arg,  full_arg, description);
    args_.emplace_back(argument);
    ArgumentParser::ArgParser::AddHelp(short_arg, full_arg, description);
    return reinterpret_cast<IntValues&>(args_.back());
}

IntValues& ArgumentParser::ArgParser::AddIntArgument(const std::string&  full_arg, const std::string& description) {
    IntValues argument( full_arg, description);
    args_.emplace_back(argument);
    return reinterpret_cast<IntValues&>(args_.back());
}

int64_t ArgumentParser::ArgParser::GetIntValue(const std::string& full_arg, const size_t index) const {
    for (auto arg : args_) {
        if (CorrectType<IntValues>(arg) && ArgumentParser::ArgParser::EqualArg(full_arg, arg)) {
            return reinterpret_cast<IntValues&>(arg).GetValue(index);
        }
    }
    std::cerr << "Error: There's no value for the argument: " + full_arg << std::endl;
    exit(1);
}

StringValues& ArgumentParser::ArgParser::AddStringArgument(const std::string& full_arg, const std::string& description) {
    StringValues argument(full_arg, description);
    args_.emplace_back(argument);
    //ArgumentParser::ArgParser::AddHelp(full_arg, description);
    return reinterpret_cast<StringValues&>(args_.back());
}

StringValues& ArgumentParser::ArgParser::AddStringArgument(const char short_arg, const std::string&  full_arg, const std::string& description) {
    StringValues argument(short_arg,  full_arg, description);
    args_.emplace_back(argument);
    ArgumentParser::ArgParser::AddHelp(short_arg, full_arg, description);
    return reinterpret_cast<StringValues&>(args_.back());
}

std::string ArgumentParser::ArgParser::GetStringValue(const std::string& full_arg, const size_t index) const {
    for (auto arg : args_) {
        if (CorrectType<StringValues>(arg) && ArgumentParser::ArgParser::EqualArg(full_arg, arg)) {
            return reinterpret_cast<StringValues&>(arg).GetValue();
        }
    }
    std::cerr << "Error: There's no value for the argument: " + full_arg << std::endl;
    exit(1);
}

BoolValues& ArgumentParser::ArgParser::AddBoolArgument(const std::string& full_arg, const std::string& description) {
    BoolValues argument(full_arg, description);
    args_.emplace_back(argument);
    ArgumentParser::ArgParser::AddHelp('\0',full_arg, description);
    return reinterpret_cast<BoolValues&>(args_.back());
}

BoolValues& ArgumentParser::ArgParser::AddBoolArgument(const char short_arg, const std::string&  full_arg, const std::string& description) {
    BoolValues argument(short_arg,  full_arg, description);
    args_.emplace_back(argument);
    ArgumentParser::ArgParser::AddHelp(short_arg, full_arg, description);
    return reinterpret_cast<BoolValues&>(args_.back());
}

bool ArgumentParser::ArgParser::GetBoolValue(const std::string& full_arg) const {
    for (auto arg : args_) {
        if (CorrectType<BoolValues>(arg) && ArgumentParser::ArgParser::EqualArg(full_arg, arg)) {
            return reinterpret_cast<BoolValues&>(arg).GetValue();
        }
    }
    std::cerr << "There's no value for the argument: " + full_arg << std::endl;
    exit(1);
}

BoolValues& ArgumentParser::ArgParser::AddFlag(const char short_arg, const std::string& full_arg, const std::string& description) {
    BoolValues argument(short_arg, full_arg, description);
    args_.emplace_back(argument);
    ArgumentParser::ArgParser::AddHelp(short_arg, full_arg, description);
    return reinterpret_cast<BoolValues&>(args_.back());
}

BoolValues& ArgumentParser::ArgParser::AddFlag(const std::string& full_arg, const std::string& description) {
    BoolValues argument(full_arg, description);
    args_.emplace_back(argument);
    ArgumentParser::ArgParser::AddHelp('\0', full_arg, description);
    return reinterpret_cast<BoolValues&>(args_.back());
}

bool ArgumentParser::ArgParser::GetFlag(const std::string& full_arg) const{
    for (auto arg : args_) {
        if (CorrectType<BoolValues>(arg) && EqualArg(full_arg, arg)) {
            return reinterpret_cast<BoolValues&>(arg).GetValue();
        }
    }
    std::cerr << "Error: There's no value for " + full_arg << std::endl;
    exit(1);
}

bool ArgumentParser::ArgParser::AddHelp(const char short_arg, const std::string& full_arg, const std::string& description) {
    IntValues arg(short_arg, full_arg, description);
    if (CorrectType<IntValues>(arg)) {
        help_text_ += reinterpret_cast<IntValues&>(arg).GetInfo() + "\n";
        return true;
    } else if (CorrectType<StringValues>(arg)) {
        help_text_ += reinterpret_cast<StringValues&>(arg).GetInfo() + "\n";
        return true;
    } else if (CorrectType<StringValues>(arg)) {
        help_text_ += reinterpret_cast<BoolValues&>(arg).GetInfo() + "\n";
        return true;
    }
    return false;
}

bool ArgumentParser::ArgParser::Help() {
    return !ArgumentParser::ArgParser::HelpDescription().empty();
}

std::string ArgumentParser::ArgParser::HelpDescription() {
    help_text_ = "";
    help_text_ += name_ + "\n";
    help_text_ += description_;
    help_text_ += (description_.empty()) ? "" : "\n";

    for(auto& arg : args_) {
        if (CorrectType<IntValues>(arg)) {
            help_text_ += reinterpret_cast<IntValues&>(arg).GetInfo() + "\n";
        } else if (CorrectType<StringValues>(arg)) {
            help_text_ += reinterpret_cast<StringValues&>(arg).GetInfo() + "\n";
        } else if (CorrectType<StringValues>(arg)) {
            help_text_ += reinterpret_cast<BoolValues&>(arg).GetInfo() + "\n";
        }
    }
    ArgumentParser::ArgParser::AddHelp('h', "help", "Display this help and exit");
    return help_text_;
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv) {
    static std::vector<std::string> temp;
    temp.reserve(argc);
    for (size_t i = 0; i < argc; ++i) {
        temp.emplace_back(argv[i]);
    }
    return Parse(temp);
}

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        bool correct_argument = false;

        if (arr[i][0] == '-') {
            if (arr[i][1] == '-') {
                std::string parameter = arr[i].substr(2, arr[i].find('=') - 2);

                if (arr[i].find('=') != std::string::npos){
                    std::string value = arr[i].substr(arr[i].find('=') + 1, arr[i].size() - arr[i].find('=') - 1);

                    for (auto& arg : args_) {
                        if (CorrectType<BoolValues>(arg) && EqualArg(parameter, arg)) {
                            std::cerr << "Invalid argument: Flag can't has any value." << std::endl;
                            exit(1);
                        }

                        if (CorrectType<StringValues>(arg) && EqualArg(parameter, arg)) {
                            correct_argument = true;
                            reinterpret_cast<StringValues&>(arg).Add(value);
                            break;

                        } else if (CorrectType<IntValues>(arg) && EqualArg(parameter, arg)) {
                            if (!IsNumeric(value)) {
                                std::cerr << "Invalid argument: The parameter is only for integers, but the value is not an integer." << std::endl;
                                exit(1);
                            }
                            correct_argument = true;
                            reinterpret_cast<IntValues&>(arg).Add(std::stoll(value));
                            break;
                        }
                    }

                } else {
                    for (auto& arg : args_) {
                        if (std::holds_alternative<BoolValues>(arg)) {
                            std::pair<std::string, std::string> arguments = reinterpret_cast<BoolValues&>(arg).GetArguments();
                            if (arguments.first == parameter || arguments.second == parameter) {
                                reinterpret_cast<BoolValues&>(arg).Add(true);
                                correct_argument = true;
                                break;
                            }
                        }
                    }
                }
                if (parameter == "h" || parameter == "help" ) {
                    return ArgumentParser::ArgParser::Help();
                }
                if (!correct_argument) {
                    std::cerr << "Invalid argument: No such argument was given: " + parameter << std::endl;
                    exit(1);
                }

            } else {
                std::string parameter = arr[i].substr(1, arr[i].find('=') - 1);
                if (arr[i].find('=') != std::string::npos){
                    size_t splitter = arr[i].find_first_of('=');
                    std::string value = arr[i].substr( splitter + 1, arr[i].size() - splitter - 1);

                    for (auto& arg : args_) {
                        if (CorrectType<StringValues>(arg) && EqualArg(parameter, arg)) {
                            correct_argument = true;
                            reinterpret_cast<StringValues&>(arg).Add(value);
                            break;

                        } else if (CorrectType<IntValues>(arg) && EqualArg(parameter, arg)) {
                            if (!IsNumeric(value)) {
                                std::cerr << "Invalid argument: The parameter is only for integers, but the value is not an integer." << std::endl;
                                exit(1);
                            }
                            correct_argument = true;
                            reinterpret_cast<IntValues&>(arg).Add(std::stoll(value));
                            break;
                        }
                    }

                } else {
                    if (IsNumeric(arr[i])) {
                        AddPositional(args_, arr[i]);
                    } else {
                        for (size_t j = 1; j < arr[i].size(); ++j) {
                            bool is_argument = false;

                            for (auto& arg: args_) {
                                if (CorrectType<BoolValues>(arg) && EqualArg(arr[i][j], arg)) {
                                    is_argument = true;
                                    reinterpret_cast<BoolValues&>(arg).Add(true);
                                    correct_argument = true;
                                    break;
                                }
                            }

                            if (!correct_argument) {
                                std::cerr << "Invalid argument: " + std::string(1,arr[i][j]) + " isn't an argument." << std::endl;
                                exit(1);
                            }
                        }
                    }
                }

                if (!correct_argument) {
                    std::cerr << "Invalid argument: No such argument was given : " + arr[i] << std::endl;
                    exit(1);
                }
            }

        } else if (description_.empty() && !IsNumeric(arr[i])) {
            description_ = arr[i];
        } else {
            AddPositional(args_, arr[i]);
        }
    }

    bool correct_parse = true;
    for (auto& arg : args_) {
        if (CorrectType<BoolValues>(arg)) {
            correct_parse &= reinterpret_cast<BoolValues&>(arg).IsUsed();
        } else if (CorrectType<IntValues>(arg)) {
            correct_parse &= reinterpret_cast<IntValues&>(arg).IsUsed();
            correct_parse &= reinterpret_cast<IntValues&>(arg).IsMultiValueFull();
        } else {
            correct_parse &= reinterpret_cast<StringValues&>(arg).IsUsed();
            correct_parse &= reinterpret_cast<StringValues&>(arg).IsMultiValueFull();
        }
    }
    return correct_parse;
}
