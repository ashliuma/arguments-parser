#pragma once

#include <iostream>
#include <cinttypes>
#include <vector>
#include <string>


class IntValues {
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
    std::vector<int64_t> values_;
    std::vector<int64_t>* values_ptr_;
    int64_t value_int_;
    int64_t* value_int_ptr_;
    size_t min_args_count_;

public:
    explicit IntValues(const std::string& full_param, const std::string& description = "");

    explicit IntValues(const char short_param, const std::string& full_param = "", const std::string& description = "");

    [[nodiscard]] bool IsUsed() const;

    [[nodiscard]] bool IsPositional() const;

    [[nodiscard]] bool IsMultiValueFull() const;

    [[nodiscard]] std::string ArgumentName() const;

    [[nodiscard]] std::pair<std::string, std::string> GetArguments() const;

    [[nodiscard]] int64_t GetValue(size_t index = -1) const;

    [[nodiscard]] std::string GetInfo() const;

    void Add(int64_t value);

    IntValues& MultiValue();

    IntValues& MultiValue(size_t size);

    IntValues& StoreValue(int64_t& variable);

    IntValues& StoreValues(std::vector<int64_t>& variable);

    IntValues& Positional();

    IntValues& Default(int64_t value);
};
