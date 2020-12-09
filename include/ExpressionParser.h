#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

class ExpressionParser {
  protected:
    enum class MathOperator {
        Addition,       // +
        Subtraction,    // -
        Multiplication, // *
        Division,       // /
        Power           // ^
    };

    enum class MathFunction {
        Sinus,          // sin(
        Cosinus,        // cos(
        Tangens,        // tg(
        Exsponent       // exp(
    };

    enum class MathConstant {
        PI
    };

    struct Operators {
        using Priority = int;
        using Operator = std::string;

        enum class Associates {
            None,
            Left,
            Right
        };

        struct OperatorInfo {
            Priority priority;
            Associates associativity;
        };

        explicit Operators(std::unordered_map<Operator, OperatorInfo>&& operators);

        const std::unordered_map<Operator, OperatorInfo> operators;
    };

  public:
    using Token = std::string;

    std::pair<std::vector<double>, std::vector<double>> evaluate(double from, double to, double delta,
                                                               const std::vector<Token>& input) const;

  protected:
    static bool IsNumber(const Token& token) {
        try {
            std::stof(token);
        } catch (const std::exception&) {
            return false;
        }

        return true;
    }

    static bool IsOpenParenthesis(const Token& token) {
        return token == "(";
    }

    static bool IsCloseParenthesis(const Token& token) {
        return token == ")";
    }

    static bool IsParenthesis(const Token& token) {
        return IsOpenParenthesis(token) || IsCloseParenthesis(token);
    }

    static bool IsVariable(const Token& token) {
        return token == "x";
    }

    static MathFunction GetMathFunction(const Token& token) {
        if (token == "sin(") {
            return MathFunction::Sinus;
        } else if (token == "cos(") {
            return MathFunction::Cosinus;
        } else if (token == "tg(") {
            return MathFunction::Tangens;
        } else if (token == "exp(") {
            return MathFunction::Exsponent;
        } else {
            std::cerr << "unreachable code\n";
            std::abort();
        }
    }

    static MathOperator GetMathOperator(const Token& token) {
        if (token == "^") {
            return MathOperator::Power;
        } else if (token == "*") {
            return MathOperator::Multiplication;
        } else if (token == "/") {
            return MathOperator::Division;
        } else if (token == "+") {
            return MathOperator::Addition;
        } else if (token == "-") {
            return MathOperator::Subtraction;
        }

        std::abort();
    }

    std::optional<double> getValue(const Token& token) const;

    bool isOperator(const Token& token) const;

    bool isFunction(const Token& token) const;

    bool isConstant(const Token& token) const;

    Operators::Priority getPriority(const Token& token) const;

    Operators::Associates getAssociativity(const Token& token) const;

    std::vector<Token> parse(const std::vector<Token>& tokens) const;

    std::optional<double> evaluate(const std::vector<Token>& rpn, double x) const;

  protected:
    using Functions = std::unordered_set<std::string>;

    const Functions functions_{
        { "sin(" },
        { "cos(" },
        { "tg("  },
        { "exp(" }
    };

    using Constants = std::unordered_map<std::string, double>;

    const Constants constants_{
        { "PI" , M_PI}
    };

    const Operators operators_{
        {   { "^", { 4, Operators::Associates::Right } },
            { "*", { 3, Operators::Associates::Left  } },
            { "/", { 3, Operators::Associates::Left  } },
            { "+", { 2, Operators::Associates::Left  } },
            { "-", { 2, Operators::Associates::Left  } } } };
};

std::ostream& operator<< (std::ostream& out, const std::vector<ExpressionParser::Token>& tokens);
