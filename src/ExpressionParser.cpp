#include "ExpressionParser.h"

ExpressionParser::Operators::Operators(std::unordered_map<Operator, OperatorInfo>&& operators)
    : operators(std::move(operators)) {}

std::pair<std::vector<double>, std::vector<double>> ExpressionParser::evaluate(double from, double to, double delta,
                                                                             const std::vector<Token>& input) const {
    assert(from < to);

    std::vector<double> y_out;
    std::vector<double> x_out;

    // try to parse
    auto rpn = parse(input);

    double x = from;
    while (x < to) {
        if (auto y = evaluate(rpn, x); y.has_value()) {
            x_out.push_back(x);
            y_out.push_back(y.value());
        }

        x += delta;
    }

    std::cout << "parsed and evaluated\n";

    return {x_out, y_out};
}

std::optional<double> ExpressionParser::getValue(const Token& token) const {
    if (isConstant(token)) {
        return constants_.at(token);
    }

    try {
        const double value = std::stof(token);
        return value;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

bool ExpressionParser::isOperator(const Token& token) const {
    return operators_.operators.count(token);
}

bool ExpressionParser::isFunction(const Token& token) const {
    return functions_.count(token);
}

bool ExpressionParser::isConstant(const Token& token) const {
    return constants_.count(token);
}

ExpressionParser::Operators::Priority ExpressionParser::getPriority(const Token& token) const {
    return operators_.operators.count(token) ? operators_.operators.at(token).priority : 0;
}

ExpressionParser::Operators::Associates ExpressionParser::getAssociativity(const Token& token) const {
    return operators_.operators.count(token) ? operators_.operators.at(token).associativity : Operators::Associates::None;
}

std::vector<ExpressionParser::Token> ExpressionParser::parse(const std::vector<Token>& tokens) const {
    std::cout << "Input:\n" << tokens << '\n';

    std::vector<Token> output;
    std::stack<Token> stack;

    for (auto& token : tokens) {
        if (IsNumber(token) || IsVariable(token) || isConstant(token)) {
            output.push_back(token);
        } else if (isOperator(token) || IsParenthesis(token) || isFunction(token)) {
            if (!IsOpenParenthesis(token) && !isFunction(token)) {
                while (!stack.empty() &&
                        ((IsCloseParenthesis(token) && !isFunction(stack.top()) && !IsOpenParenthesis(stack.top())) ||
                        (getPriority(stack.top()) > getPriority(token)) ||
                        (getPriority(stack.top()) == getPriority(token) &&
                         getAssociativity(token) == Operators::Associates::Left))) {
                    output.push_back(stack.top());
                    stack.pop();
                }

                if (IsCloseParenthesis(token)) {
                    if (isFunction(stack.top())) {
                        output.push_back(stack.top());
                    }
                    stack.pop();
                }
            }

            if (!IsCloseParenthesis(token)) {
                stack.push(token);
            }
        } else {
            std::cerr << "unexpected token\n";
            abort();
        }
    }

    while (!stack.empty()) {
        output.push_back(stack.top());
        stack.pop();
    }

    std::cout << "RNP:\n" << output << '\n';

    return output;
}

std::optional<double> ExpressionParser::evaluate(const std::vector<Token>& rpn, double x) const {
    std::stack<Token> stack;

    auto get_stack_top = [this, &stack, x] () -> double {
        assert(!stack.empty());

        auto top = IsVariable(stack.top()) ? x : getValue(stack.top());
        stack.pop();

        assert(top.has_value());
        return top.value();
    };

    for (const auto& token : rpn) {
        if (isOperator(token)) {
            const double a = get_stack_top();
            const double b = get_stack_top();

            switch (GetMathOperator(token)) {
                case MathOperator::Addition: {
                    stack.push(std::to_string(b + a));
                    break;
                }
                case MathOperator::Subtraction: {
                    stack.push(std::to_string(b - a));
                    break;
                }
                case MathOperator::Division: {
                    stack.push(std::to_string(b / a));
                    if (std::abs(a) < 0.01f) {
                        return std::nullopt;
                    }
                    break;
                }
                case MathOperator::Multiplication: {
                    stack.push(std::to_string(b * a));
                    break;
                }
                case MathOperator::Power: {
                    stack.push(std::to_string(pow(a, b)));
                    break;
                }
                default: {
                    std::cerr << "Unreachable code\n";
                    std::abort();
                }
            }
        } else if (isFunction(token)) {
            const double a = get_stack_top();

            switch (GetMathFunction(token)) {
                case MathFunction::Sinus: {
                    stack.push(std::to_string(std::sin(a)));
                    break;
                }

                case MathFunction::Cosinus: {
                    stack.push(std::to_string(std::cos(a)));
                    break;
                }

                case MathFunction::Tangens: {
                    stack.push(std::to_string(std::tan(a)));
                    break;
                }

                case MathFunction::Exsponent: {
                    stack.push(std::to_string(std::exp(a)));
                    break;
                }

                default: {
                    std::cerr << "Unreachable code\n";
                    std::abort();
                }
            }
        } else {
            assert(IsNumber(token) || IsVariable(token) || isConstant(token));
            stack.push(token);
        }
    }

    assert(stack.size() == 1);

    return getValue(stack.top());;
}

std::ostream& operator<< (std::ostream& out, const std::vector<ExpressionParser::Token>& tokens) {
    for (const auto& token : tokens) {
        out << token << ' ';
    }
    return out;
}
