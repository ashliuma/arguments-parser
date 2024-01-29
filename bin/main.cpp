#include <lib/ArgParser.h>

#include <functional>
#include <iostream>
#include <numeric>


struct Options {
    bool sum = false;
    bool mult = false;
};

int main(int argc, char** argv) {
    Options opt;
    ArgumentParser::ArgParser parser("My Parser");
    std::vector<int64_t> int_values, values;

    parser.AddIntArgument("Param1").MultiValue(1).Positional().StoreValues(values);

    if(!parser.Parse(argc, argv)) {
        std::cout << "Error: The wrong argument" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return 1;
    }

    for(int i = 0; i < values.size(); i++){
        std::cout << i << ": " << values[i] << "\n";
    }

    if(parser.Help()) {
        std::cout << parser.Help() << std::endl;
        return 0;
    }

    if(opt.sum) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
    } else if(opt.mult) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<int64_t>()) << std::endl;
    } else {
        std::cout << "No one options had chosen." << std::endl;
        std::cout << parser.Help();
        return 1;
    }
    return 0;
}
