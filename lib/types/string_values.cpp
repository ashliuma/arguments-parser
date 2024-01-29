#include "string_values.h"


StringValues::StringValues(const std::string& full_param, const std::string& description) {
    full_argument_ = full_param;
    description_ = description;

    values_ptr_ = nullptr;
    value_str_ptr_ = nullptr;

    min_args_count_ = 0;
    full_multi_value_ = true;

    one_value_ = true;
    positional_ = false;
    store_value_ = false;
    default_ = false;
    used_ = false;
}

StringValues::StringValues(const char short_param, const std::string& full_param, const std::string& description) {
    full_argument_ = full_param;
    short_argument_ = short_param;
    this->description_ = description;

    values_ptr_ = nullptr;
    value_str_ptr_ = nullptr;

    min_args_count_ = 0;
    full_multi_value_ = true;

    one_value_ = true;
    positional_ = false;
    store_value_ = false;
    default_ = false;
    used_ = false;
}

bool StringValues::IsUsed() const {
    return used_;
}

bool StringValues::IsPositional() const {
    return positional_;
}

bool StringValues::IsMultiValueFull() const {
    return full_multi_value_;
}

std::string StringValues::ArgumentName() const {
    return (full_argument_.empty()) ? short_argument_ : full_argument_;
}

std::pair<std::string, std::string> StringValues::GetArguments() const {
    return {short_argument_, full_argument_};
}

std::string StringValues::GetValue(size_t index) const {
    if (!used_) {
        std::cerr << "Invalid argument: There's no value for parameter: " + ArgumentName() << std::endl;
        exit(1);
    }

    if (store_value_ && ((one_value_ && value_str_ptr_ == nullptr) || (!one_value_ && values_ptr_ == nullptr))) {
        std::cerr << "Invalid argument: There's no pointer to the variable in the argument: " + ArgumentName() << std::endl;
        exit(1);
    }

    if (!one_value_) {
        if (index == -1) {
            std::cerr << "Invalid argument: There's no index was given." << std::endl;
            exit(1);
        }
        if (store_value_ && index >= values_ptr_->size()) {
            std::cerr << "Invalid argument: Index is out of range." << std::endl;
            exit(1);
        }
        if (!store_value_ && index >= values_.size()) {
            std::cerr << "Invalid argument: Index is out of range." << std::endl;
            exit(1);
        }
    }

    if (one_value_) {
        return (store_value_) ? *value_str_ptr_ : value_str_;
    }
    return (store_value_) ? (*values_ptr_)[index] : values_[index];
}

void StringValues::Add(const std::string& value) {
    if (used_ && !default_ && one_value_) {
        std::cerr << "Invalid argument: Argument " + ArgumentName() + ", has already had a value." << std::endl;
        exit(1);
    }
    if (store_value_ && ((one_value_ && value_str_ptr_ == nullptr) || (!one_value_ && values_ptr_ == nullptr))) {
        std::cerr << "Invalid argument: There's no initialized variable for StoreValue." << std::endl;
        exit(1);
    }

    if (one_value_) {
        if (store_value_) {
            *value_str_ptr_ = value;
        } else {
            value_str_ = value;
        }
    } else {
        values_ptr_->push_back(value);
        store_value_ = true;
        full_multi_value_ = (values_ptr_->size() >= min_args_count_);
    }
    used_ = true;
}

std::string StringValues::GetInfo() const {
    std::string data = "-" + short_argument_ + " --" + full_argument_ + " ";
    data += description_ + " [default=";
    data += (default_)? "true" : "false";
    data += (positional_)? ", positional" : ", not positional";
    data +=(one_value_)? ", one value" : ", multivalue, min args=" + std::to_string(min_args_count_);
    data += "]";
    return data;
}

StringValues& StringValues::MultiValue(size_t size) {
    if (default_) {
        std::cerr << "Error: Argument " + ArgumentName() + " has a default value. It can't be multi-value." << std::endl;
        exit(1);
    }
    full_multi_value_ = true;
    min_args_count_ = size;
    one_value_ = false;
    return *this;
}

StringValues& StringValues::MultiValue() {
    if (default_) {
        std::cerr << "Error: Argument " + ArgumentName() + " has a default value. It can't be multi-value." << std::endl;
        exit(1);
    }
    one_value_ = false;
    return *this;
}

StringValues& StringValues::StoreValue(std::string& variable) {
    if (!one_value_) {
        std::cerr << "Error: Argument " + ArgumentName() + " is multi-value argument." << std::endl;
        exit(1);
    }
    store_value_ = true;
    value_str_ptr_ = &variable;
    return *this;
}

StringValues& StringValues::StoreValues(std::vector<std::string>& variable) {
    if (one_value_) {
        std::cerr << "Error: Argument " + ArgumentName() + " is one-value argument." << std::endl;
        exit(1);
    }
    if (!variable.empty()) {
        store_value_ = true;
    }
    values_ptr_ = &variable;
    return *this;
}

StringValues& StringValues::Positional() {
    positional_ = true;
    return *this;
}

StringValues& StringValues::Default(const std::string& value) {
    if (!one_value_) {
        std::cerr << "Invalid argument: Default value is only for one-value arguments." << std::endl;
        exit(1);
    }
    if (store_value_ && value_str_ptr_ == nullptr) {
        std::cerr << "Invalid argument: There's no pointer to the variable in the argument: " + ArgumentName() << std::endl;
        exit(1);
    }

    if (store_value_) {
        *value_str_ptr_ = value;
    } else {
        value_str_ = value;
    }
    default_ = true;
    used_ = true;
    return *this;
}
