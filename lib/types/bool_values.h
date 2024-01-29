#pragma once

#include<iostream>
#include <vector>
#include <string>


class BoolValues {
private:
    bool store_value_;
    bool default_;
    bool used_;
    bool value_;
    bool* value_ptr_;
    std::string full_argument_;
    std::string short_argument_;
    std::string description_;

public:
    BoolValues(const std::string& full_param, const std::string& description = "");

    BoolValues(const char short_param, const std::string& full_param = "", const std::string& description = "");

    [[nodiscard]] bool IsUsed() const;

    [[nodiscard]] std::string ArgumentName() const;

    [[nodiscard]] std::pair<std::string, std::string> GetArguments() const;

    [[nodiscard]] bool GetValue() const;

    [[nodiscard]] std::string GetInfo() const;

    void Add(bool value);

    BoolValues& StoreValue(bool& variable);

    BoolValues& Default(bool value);
};
