#include "int_values.h"


IntValues::IntValues(const std::string& full_param, const std::string& description) {
    full_argument_ = full_param;
    description_ = description;

    values_ptr_ = nullptr;
    value_int_ = 0;
    value_int_ptr_ = nullptr;

    min_args_count_ = 0;
    full_multi_value_ = true;

    one_value_ = true;
    positional_ = false;
    store_value_ = false;
    default_ = false;
    used_ = false;
}

IntValues::IntValues(const char short_param, const std::string& full_param, const std::string& description) {
    full_argument_ = full_param;
    short_argument_ = short_param;
    this->description_ = description;

    values_ptr_ = nullptr;
    value_int_ = 0;
    value_int_ptr_ = nullptr;

    min_args_count_ = 0;
    full_multi_value_ = true;

    one_value_ = true;
    positional_ = false;
    store_value_ = false;
    default_ = false;
    used_ = false;
}

bool IntValues::IsUsed() const {
    return used_;
}

bool IntValues::IsPositional() const {
    return positional_;
}

bool IntValues::IsMultiValueFull() const {
    return full_multi_value_;
}

std::string IntValues::ArgumentName() const {
    return (full_argument_.empty()) ? short_argument_ : full_argument_;
}

std::pair<std::string, std::string> IntValues::GetArguments() const {
    return {short_argument_, full_argument_};
}

int64_t IntValues::GetValue(size_t index) const {
    if (!used_) {
        std::cerr << "Invalid argument: There's no value for parameter : " + ArgumentName() << std::endl;
        exit(1);
    }

    if (store_value_ && ((one_value_ && value_int_ptr_ == nullptr) || (!one_value_ && values_ptr_ == nullptr))) {
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
        return (store_value_) ? *value_int_ptr_ : value_int_;
    }
    return (store_value_) ? (*values_ptr_)[index] : values_[index];
}

std::string IntValues::GetInfo() const {
    std::string data = "-" + short_argument_ + " --" + full_argument_ + " ";
    data += description_ + " [default=";
    data += (default_)? "true" : "false";
    data += (positional_)? ", positional" : ", not positional";
    data +=(one_value_)? ", one value" : ", multivalue, min args=" + std::to_string(min_args_count_);
    data += "]";
    return data;
}

void IntValues::Add(int64_t value) {
    if (used_ && !default_ && one_value_) {
        std::cerr << "Invalid argument: Argument " + ArgumentName() << std::endl;
        std::cerr << " has already had a value and it's not multi-value." << std::endl;
        exit(1);

    }
    if (store_value_ && ((one_value_ && value_int_ptr_ == nullptr) || (!one_value_ && values_ptr_ == nullptr))) {
        std::cerr << "Invalid argument: The variable isn't initialized." << std::endl;
        exit(1);
    }

    if (one_value_) {
        if (store_value_) {
            *value_int_ptr_ = value;
        } else {
            value_int_ = value;
        }
    } else {
        values_ptr_->push_back(value);
        store_value_ = true;
        full_multi_value_ = (values_ptr_->size() >= min_args_count_);
    }
    used_ = true;
}

IntValues& IntValues::MultiValue(size_t size) {
    if (default_) {
        std::cerr << "Error: Argument " + ArgumentName() + " has a default value. It can't be multi-value." << std::endl;
        exit(1);
    }
    full_multi_value_ = true;
    min_args_count_ = size;
    one_value_ = false;
    return *this;
}

IntValues& IntValues::MultiValue() {
    if (default_) {
        std::cerr << "Error: Argument " + ArgumentName() + " has a default value. It can't be multi-value." << std::endl;
        exit(1);
    }
    one_value_ = false;
    return *this;
}

IntValues& IntValues::StoreValue(int64_t& variable) {
    if (!one_value_) {
        std::cerr << "Error: Argument "  + ArgumentName() + " is multi-value argument. It can't be StoreValues type." << std::endl;
        exit(1);
    }
    store_value_ = true;
    value_int_ptr_ = &variable;
    return *this;
}

IntValues& IntValues::StoreValues(std::vector<int64_t>& variable) {
    if (one_value_) {
        std::cerr << "Error: Argument " + ArgumentName() + " is one-value argument. It can't be StoreValues type." << std::endl;
        exit(1);
    }
    if (!variable.empty()){
        store_value_ = true;
    }
    values_ptr_ = &variable;
    return *this;
}

IntValues& IntValues::Positional() {
    positional_ = true;
    return *this;
}

IntValues& IntValues::Default(int64_t value) {
    if (!one_value_) {
        std::cerr << "Invalid argument: Default value is only for one-value arguments." << std::endl;
        exit(1);
    }
    if (store_value_ && value_int_ptr_ == nullptr) {
        std::cerr << "Invalid argument: There's no pointer to the variable in the argument: " + ArgumentName() << std::endl;
        exit(1);
    }

    if (store_value_) {
        *value_int_ptr_ = value;
    } else {
        value_int_ = value;
    }
    used_ = true;
    default_ = true;
    return *this;
}
