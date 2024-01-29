#include "bool_values.h"


BoolValues::BoolValues(const std::string& full_param, const std::string& description) {
    full_argument_ = full_param;
    this->description_ = description;

    value_ = false;
    value_ptr_ = nullptr;

    store_value_ = false;
    default_ = false;
    used_ = false;
}

BoolValues::BoolValues(const char short_param, const std::string& full_param, const std::string& description) {
    full_argument_ = full_param;
    short_argument_ = short_param;
    this->description_ = description;

    value_ = false;
    value_ptr_ = nullptr;
    
    store_value_ = false;
    default_ = false;
    used_ = false;
}

std::pair<std::string, std::string> BoolValues::GetArguments() const {
    return {full_argument_, short_argument_};
}

std::string BoolValues::ArgumentName() const {
    return full_argument_.empty() ? short_argument_ : full_argument_;
}

bool BoolValues::GetValue() const {
    if (!used_) {
        std::cerr << "Invalid argument: There's no value for parameter: " + ArgumentName() << std::endl;
        exit(1);
    }

    if (store_value_ && value_ptr_ == nullptr) {
        std::cerr << "Invalid argument: There's no pointer to the variable in the argument: " + ArgumentName() << std::endl;
        exit(1);
    }
    return value_;
}

std::string BoolValues::GetInfo() const {
    std::string data = "-" + short_argument_ + " --" + full_argument_ + " ";
    data += description_ + " [default=";
    data += (default_)? "true" : "false";
    data += "]";
    return data;
}

bool BoolValues::IsUsed() const {
    return used_;
}

void BoolValues::Add(bool value) {
    if (used_ && !default_) {
        std::cerr << "Invalid argument: Argument " + ArgumentName() + ", has already had a value." << std::endl;
        exit(1);
    }

    if (store_value_ && value_ptr_ == nullptr) {
        std::cerr << "Invalid argument: There's no initialized variable for StoreValue." << std::endl;
        exit(1);
    }

    if (store_value_) {
        *value_ptr_ = value;
    } else {
        value_ = value;
    }
    used_ = true;
}

BoolValues& BoolValues::StoreValue(bool& variable) {
    value_ptr_ = &variable;
    store_value_ = true;
    return *this;
}

BoolValues& BoolValues::Default(bool value) {
    if (store_value_ && value_ptr_ == nullptr) {
        std::cerr << "Invalid argument: There's no pointer to the variable in the argument: " + ArgumentName() << std::endl;
        exit(1);
    }

    if (store_value_) {
        *value_ptr_ = value;
    } else {
        value_ = value;
    }
    default_ = true;
    used_ = true;
    return *this;
}
