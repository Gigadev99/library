#pragma once
#include <format>
#include "Structures/matrix.cpp"

template<MatType M>
struct std::formatter<M> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(const M& m, std::format_context& ctx) const {
        auto out = std::format_to(ctx.out(), "[");
        for (int i = 0; i < m.rows(); ++i) {
            if (i) out = std::format_to(out, ", ");
            out = std::format_to(out, "[");
            for (int j = 0; j < m.cols(); ++j) {
                if (j) out = std::format_to(out, ", ");
                out = std::format_to(out, "{}", m[i, j]);
            }
            out = std::format_to(out, "]");
        }
        return std::format_to(out, "]");
    }
};