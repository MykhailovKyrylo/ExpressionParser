#include <vector>

#include "ExpressionParser.h"

void DrawPlotWithPython(const std::pair<std::vector<double>, std::vector<double>>& x_y_out) {
    const std::string separator = ",";

    auto make_list = [&separator] (const std::vector<double>& values) {
        std::ostringstream out;
        std::copy(std::begin(values), std::end(values), std::ostream_iterator<double>(out, separator.c_str()));

        // removing extra separator in the end
        std::string result = std::move(out.str());
        result.pop_back();

        return result;
    };

    const std::string call_python = "python3 ../plotting.py";
    const std::string syscall = call_python + ' ' + make_list(x_y_out.first) + ' ' + make_list(x_y_out.second);

    std::cout << syscall << std::endl;

    // python sys call to draw function plot
    system(syscall.c_str());
}

int main(int argc, char** argv) {
    ExpressionParser expr_parser;
    auto tokens = std::vector<ExpressionParser::Token>(argv + 1, argv + argc);
    DrawPlotWithPython(expr_parser.evaluate(0.0, 6.5, 0.01, tokens));
}
