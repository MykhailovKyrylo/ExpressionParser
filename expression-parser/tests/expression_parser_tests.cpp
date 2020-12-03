#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ExpressionParser.h"

namespace {
    std::vector<ExpressionParser::Token> GetTokens(std::string&& input) {
        std::istringstream input_stream(input);

        std::vector<ExpressionParser::Token> tokens;

        ExpressionParser::Token token;
        while (input_stream >> token) {
            tokens.push_back(std::move(token));
        }

        return tokens;
    }

    template <typename T>
    std::string GetString(T&& tokens) {
        std::ostringstream out;

        for (size_t i = 0; i < tokens.size(); i++) {
            out << tokens[i] << ' ';
        }

        return out.str();
    }

    struct ExpressionParserMock : public ExpressionParser {
        inline std::vector<Token> parse(const std::vector<Token>& tokens) const {
            return ExpressionParser::parse(tokens);
        }
    };
}

TEST_CASE("Parse tests") {
    ExpressionParserMock expr_parser;

    SECTION("basic") {
        const auto tokens = GetTokens("x ^ 2 - 2 * x");

        auto rpn = expr_parser.parse(tokens);
        REQUIRE(GetString(rpn) == "x 2 ^ 2 x * - ");
    }

    SECTION("double test") {
        const auto tokens = GetTokens("3.576 * x - x ^ 2");

        auto rpn = expr_parser.parse(tokens);
        REQUIRE(GetString(rpn) == "3.576 x * x 2 ^ - ");
    }

    SECTION("brackets") {
        const auto tokens = GetTokens("3.576 * ( x - x ^ 2 )");

        auto rpn = expr_parser.parse(tokens);
        REQUIRE(GetString(rpn) == "3.576 x x 2 ^ - * ");
    }

    SECTION("function") {
        const auto tokens = GetTokens("sin( x )");

        auto rpn = expr_parser.parse(tokens);
        REQUIRE(GetString(rpn) == "x sin( ");
    }

    SECTION("function2") {
        const auto tokens = GetTokens("sin( x ^ 2 - 2 * x )");

        auto rpn = expr_parser.parse(tokens);
        REQUIRE(GetString(rpn) == "x 2 ^ 2 x * - sin( ");
    }

    SECTION("constant") {
        const auto tokens = GetTokens("x * PI");

        auto rpn = expr_parser.parse(tokens);
        REQUIRE(GetString(rpn) == "x PI * ");
    }

    SECTION("example") {
        const auto tokens = GetTokens("2 * sin( 1 / ( exp( 3 * x ) + 1 ) - tg( x + PI / 2 )");

        auto rpn = expr_parser.parse(tokens);
        REQUIRE(GetString(rpn) == "2 1 3 x * exp( 1 + / x PI 2 / + tg( - sin( * ");
    }
}
