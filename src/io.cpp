#include <cstdio>
#include <iostream>
#include <istream>
#include <ostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

#include "MiniLua/io.hpp"
#include "MiniLua/values.hpp"

namespace minilua {

auto close() -> Value { return true; }

auto flush() -> Value { return true; }

auto lines(const Value& a...) {}

auto write(const CallContext& ctx) -> Value {
    for (int i = 1; i < ctx.arguments().size(); i++) {
        auto v = ctx.arguments().get(i);

        if (v.type() == "Number" || v.type() == "String") {
            // TODO: write
        } else {
            // TODO: error-message
        }
        i++;
    }
    return 1;
}

namespace io {

std::istream* stdin = &std::cin;
std::ostream* stdout = &std::cout;
std::ostream* stderr = &std::cerr;

auto static open_file(const String& filename, const String& mode) -> Vallist {
    std::FILE* file = std::fopen(filename.value.c_str(), mode.value.c_str());
    if (file == nullptr) {
        // TODO: differnciate between different errors
        return Vallist({Nil(), "could not open file", 2});
    }
    Table file_tab;
    file_tab.set("close", [file]() { std::fclose(file); });
    file_tab.set("flush", minilua::flush);
    file_tab.set("write", write);
    return Vallist();
}

auto open(const CallContext& ctx) -> Vallist {
    auto file = ctx.arguments().get(0);
    auto mode = ctx.arguments().get(1);

    return std::visit(
        overloaded{
            [](const String& file, Nil /*unused*/) -> Vallist {
                return open_file(file, std::get<String>("r"));
            },
            [](const String& file, const String& mode) -> Vallist {
                std::regex modes(R"(([rwa]\+?b?))");

                if (std::regex_match(mode.value, modes)) {
                    return open_file(file, mode);
                } else {
                    throw std::runtime_error("invalid mode");
                }
            },
            [](auto /*unused*/, auto /*unused*/) -> Vallist { throw std::runtime_error(""); }},
        file.raw(), mode.raw());
}
} // namespace io
} // end namespace minilua