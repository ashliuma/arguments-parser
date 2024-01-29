#pragma once

#include <iostream>
#include <vector>
#include <string>


class StringValues {
private:
    bool positional_;
    bool store_value_;
    bool one_value_;
    bool default_;
    bool used_;
    bool full_multi_value_;
    std::string full_argument_;
    std::string short_argument_;
    std::string description_;
    std::vector<std::string> values_;
    std::vector<std::string>* values_ptr_;
    std::string value_str_;
    std::string* value_str_ptr_;
    size_t min_args_count_;

public:
    explicit StringValues(const std::string& full_param, const std::string& description = "");

    explicit StringValues(const char short_param, const std::string& full_param = "", const std::string& description = "");

    [[nodiscard]] bool IsUsed() const;

    [[nodiscard]] bool IsPositional() const;

    [[nodiscard]] bool IsMultiValueFull() const;

    [[nodiscard]] std::string ArgumentName() const;

    [[nodiscard]] std::pair<std::string, std::string> GetArguments() const;

    [[nodiscard]] std::string GetValue(size_t index = -1) const;

    [[nodiscard]] std::string GetInfo() const;

    void Add(const std::string& value);

    StringValues& MultiValue(size_t size);

    StringValues& MultiValue();

    StringValues& StoreValue(std::string& variable);

    StringValues& StoreValues(std::vector<std::string>& variable);

    StringValues& Positional();

    StringValues& Default(const std::string& value);
};
