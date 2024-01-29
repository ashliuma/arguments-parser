#pragma once

#include "types//int_values.h"
#include "types/string_values.h"
#include "types/bool_values.h"

#include <variant>


namespace ArgumentParser {

    class ArgParser {
    private:
        std::string name_;
        std::string description_;
        std::vector<std::variant<IntValues, StringValues, BoolValues>> args_;
        std::string help_text_;
        using argument = std::variant<IntValues, StringValues, BoolValues>;

        bool IsNumeric(const std::string& value);

        bool EqualArg(const std::string& parameter, argument& variable) const;

        bool EqualArg(char parameter, argument& variable) const;

        void AddPositional(std::vector<argument>& args, const std::string& value);

    public:
        ArgParser();

        explicit ArgParser(const std::string& name, const std::string& description = "");

        bool Parse(int argc, char** argv);

        bool Parse(const std::vector<std::string>& data);

        IntValues& AddIntArgument(const std::string& full_arg, const std::string& description = "");

        IntValues& AddIntArgument(char short_arg, const std::string& full_arg, const std::string& description = "");

        [[nodiscard]] int64_t GetIntValue(const std::string& full_arg, size_t index = -1) const;

        StringValues& AddStringArgument(const std::string& full_arg, const std::string& description = "");

        StringValues& AddStringArgument(char short_arg, const std::string& full_arg, const std::string& description = "");

        [[nodiscard]] std::string GetStringValue(const std::string& full_arg, size_t index = -1) const;

        BoolValues& AddBoolArgument(const std::string& full_arg, const std::string& description = "");

        BoolValues& AddBoolArgument(char short_arg, const std::string& full_arg, const std::string& description = "");

        [[nodiscard]] bool GetBoolValue(const std::string& full_arg) const;

        BoolValues& AddFlag(char short_arg, const std::string& full_arg, const std::string& description = "");

        BoolValues& AddFlag(const std::string& full_arg, const std::string& description = "");

        [[nodiscard]] bool GetFlag(const std::string& full_arg) const;

        bool AddHelp(const char short_arg = '\0', const std::string& full_arg = "", const std::string& description = "");

        bool Help();

        std::string HelpDescription();
    };
}