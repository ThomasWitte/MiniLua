#include "interpreter.hpp"
#include "MiniLua/interpreter.hpp"
#include "tree_sitter/tree_sitter.hpp"

#include <cassert>
#include <iostream>
#include <set>

namespace minilua::details {

// TODO remove the unimplemented stuff once everything is implemented
class UnimplementedException : public InterpreterException {
public:
    UnimplementedException(const std::string& where, const std::string& what)
        : InterpreterException("unimplemented: \"" + what + "\" in " + where) {}
};

#define UNIMPLEMENTED(what)                                                                        \
    UnimplementedException(                                                                        \
        std::string(__func__) + " (" + std::string(__FILE__) + ":" + std::to_string(__LINE__) +    \
            ")",                                                                                   \
        what)

// class Interpreter
Interpreter::Interpreter(const InterpreterConfig& config) : config(config) {}

auto Interpreter::run(const ts::Tree& tree, Environment& env) -> EvalResult {
    try {
        return this->visit_root(tree.root_node(), env);
    } catch (const InterpreterException&) {
        throw;
    } catch (const std::exception& e) {
        throw InterpreterException("unknown error");
    }
}

auto Interpreter::tracer() const -> std::ostream& { return *this->config.target; }
void Interpreter::trace_enter_node(ts::Node node) const {
    if (this->config.trace_nodes) {
        this->tracer() << "Enter node: " << ts::debug_print_node(node) << "\n";
    }
}
void Interpreter::trace_exit_node(ts::Node node) const {
    if (this->config.trace_nodes) {
        this->tracer() << "Exit node: " << ts::debug_print_node(node) << "\n";
    }
}
void Interpreter::trace_function_call(
    const std::string& function_name, const std::vector<Value>& arguments) const {
    if (this->config.trace_calls) {
        this->tracer() << "Calling function: " << function_name << " with arguments (";
        for (const auto& arg : arguments) {
            this->tracer() << arg << ", ";
        }
        this->tracer() << ")\n";
    }
}
void Interpreter::trace_function_call_result(
    const std::string& function_name, const CallResult& result) const {
    if (this->config.trace_calls) {
        this->tracer() << "Function call to: " << function_name << " resulted in "
                       << result.values();
        if (result.source_change().has_value()) {
            this->tracer() << " with source changes " << result.source_change().value();
        }
        this->tracer() << "\n";
    }
}

auto Interpreter::combine_source_changes(
    const std::optional<SourceChangeTree>& lhs, const std::optional<SourceChangeTree>& rhs)
    -> std::optional<SourceChangeTree> {
    if (lhs.has_value() && rhs.has_value()) {
        return SourceChangeCombination({*lhs, *rhs});
    } else if (lhs.has_value()) {
        return lhs;
    } else {
        return rhs;
    }
}

// helper functions
static const std::set<std::string> IGNORE_NODES{";", "comment"};

static auto should_ignore_node(ts::Node node) -> bool {
    return IGNORE_NODES.find(node.type()) != IGNORE_NODES.end();
}

static auto convert_range(ts::Range range) -> Range {
    return Range{
        .start = {range.start.point.row, range.start.point.column, range.start.byte},
        .end = {range.end.point.row, range.end.point.column, range.end.byte},
    };
}

// interpreter implementation
auto Interpreter::visit_root(ts::Node node, Environment& env) -> EvalResult {
    assert(node.type() == std::string("program"));
    this->trace_enter_node(node);

    EvalResult result;

    for (auto child : node.children()) {
        EvalResult sub_result;
        if (child.type() == std::string("variable_declaration")) {
            sub_result = this->visit_variable_declaration(child, env);
        } else if (child.type() == std::string("function_call")) {
            sub_result = this->visit_function_call(child, env);
            // TODO on "return" set value
        } else if (should_ignore_node(child)) {
        } else {
            throw UNIMPLEMENTED(child.type());
        }
        result.source_change =
            this->combine_source_changes(result.source_change, sub_result.source_change);
    }

    this->trace_exit_node(node);

    return result;
}

auto Interpreter::visit_variable_declaration(ts::Node node, Environment& env) -> EvalResult {
    assert(node.type() == std::string("variable_declaration"));
    this->trace_enter_node(node);

    auto declarator = node.named_child(0).value();
    auto expr = node.named_child(1).value();

    auto value = this->visit_expression(expr, env);

    env.add(this->visit_variable_declarator(declarator, env), value.value);

    this->trace_exit_node(node);
    return EvalResult();
}
auto Interpreter::visit_variable_declarator(ts::Node node, Environment& env) -> std::string {
    assert(node.type() == std::string("variable_declarator"));
    this->trace_enter_node(node);
    return this->visit_identifier(node.child(0).value(), env);
}

auto Interpreter::visit_identifier(ts::Node node, Environment& env) -> std::string {
    assert(node.type() == std::string("identifier"));
    this->trace_enter_node(node);
    this->trace_exit_node(node);
    return node.text();
}

auto Interpreter::visit_expression(ts::Node node, Environment& env) -> EvalResult {
    this->trace_enter_node(node);

    EvalResult result;

    if (node.type() == "number"s) {
        auto value = parse_number_literal(node.text());
        Origin origin = LiteralOrigin{.location = convert_range(node.range())};
        result.value = value.with_origin(origin);
    } else if (node.type() == "true"s) {
        Origin origin = LiteralOrigin{.location = convert_range(node.range())};
        result.value = Value(true).with_origin(origin);
    } else if (node.type() == "false"s) {
        Origin origin = LiteralOrigin{.location = convert_range(node.range())};
        result.value = Value(false).with_origin(origin);
    } else if (node.type() == "nil"s) {
        Origin origin = LiteralOrigin{.location = convert_range(node.range())};
        result.value = Value(Nil()).with_origin(origin);
    } else if (node.type() == "string"s) {
        try {
            auto value = parse_string_literal(node.text());
            Origin origin = LiteralOrigin{.location = convert_range(node.range())};
            result.value = value.with_origin(origin);
        } catch (const std::runtime_error& e) {
            throw InterpreterException(e.what());
        }
    } else if (node.type() == "identifier"s) {
        auto variable_name = this->visit_identifier(node, env);
        result.value = env.get(variable_name);
    } else if (node.type() == "unary_operation"s) {
        result = this->visit_unary_operation(node, env);
    } else if (node.type() == "binary_operation"s) {
        result = this->visit_binary_operation(node, env);
    } else if (node.type() == std::string("function_call")) {
        result = this->visit_function_call(node, env);
    } else {
        throw UNIMPLEMENTED(node.type());
    }

    this->trace_exit_node(node);
    return result;
}

auto Interpreter::visit_binary_operation(ts::Node node, Environment& env) -> EvalResult {
    assert(node.type() == std::string("binary_operation"));
    this->trace_enter_node(node);

    EvalResult result;

    auto lhs_node = node.child(0).value();
    auto operator_node = node.child(1).value();
    auto rhs_node = node.child(2).value();

    EvalResult lhs_result = this->visit_expression(lhs_node, env);
    EvalResult rhs_result = this->visit_expression(rhs_node, env);

    if (operator_node.type() == "=="s) {
        result.value = lhs_result.value.equals(rhs_result.value);
        result.source_change =
            combine_source_changes(lhs_result.source_change, rhs_result.source_change);
    } else {
        throw UNIMPLEMENTED(node.type());
    }

    this->trace_exit_node(node);
    return result;
}

auto Interpreter::visit_unary_operation(ts::Node node, Environment& env) -> EvalResult {
    assert(node.type() == std::string("unary_operation"));
    this->trace_enter_node(node);

    auto operator_ = node.child(0).value();
    auto expr = node.child(1).value();

    EvalResult result = this->visit_expression(expr, env);

    if (operator_.type() == "-"s) {
        result.value = result.value.negate(convert_range(node.range()));
    } else if (operator_.type() == "not"s) {
        result.value = result.value.invert(convert_range(node.range()));
    } else {
        throw UNIMPLEMENTED(operator_.type());
    }

    this->trace_exit_node(node);
    return result;
}

auto Interpreter::visit_function_call(ts::Node node, Environment& env) -> CallResult {
    assert(node.type() == std::string("function_call"));
    this->trace_enter_node(node);

    auto function_name = this->visit_identifier(node.named_child(0).value(), env);

    std::vector<Value> arguments;
    auto argument = node.named_child(1).value().child(0);
    // skip node `(` at the start
    argument = argument->next_sibling();

    while (argument.has_value()) {
        auto expr = this->visit_expression(argument.value(), env);
        // TODO source_changes
        arguments.push_back(expr.value);

        // skip nodes `,` in the middle and node `)` at the end
        argument = argument->next_sibling();
        argument = argument->next_sibling();
    }

    this->trace_function_call(function_name, arguments);

    // call function
    // this will produce an error if the obj is not callable
    auto obj = env.get(function_name);
    auto ctx = CallContext(&env).make_new(Vallist(arguments));

    CallResult result;
    try {
        result = obj.call(ctx);
    } catch (const std::runtime_error& e) {
        throw InterpreterException(
            std::string("failed to call ") + function_name + ": " + e.what());
    }

    this->trace_function_call_result(function_name, result);

    this->trace_exit_node(node);
    return result;
}

} // namespace minilua::details