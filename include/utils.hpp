#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

namespace Imdb::utils {
    template <typename T>
    concept String = std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>;

    // const& is redundant for string_view but not for string.
    template <String T> [[nodiscard]] inline constexpr std::vector<T> split(const T &line, char delim) {
        return line | std::views::split(delim) | std::views::transform([](auto &&s) {
                   return T(s.begin(), s.end());
               }) |
               std::ranges::to<std::vector>();
    }

    inline constexpr void toLowercase(std::string &in) {
        in = in | std::views::transform([](uint8_t x) {
                 return std::tolower(x);
             }) |
             std::ranges::to<std::string>();
    }

    class Reader {
    public:
        [[nodiscard]] inline std::optional<std::vector<std::vector<std::string>>> read(std::string_view filePath) {
            std::vector<std::vector<std::string>> result;

            std::ifstream ifs(filePath.data());
            if (!ifs) {
                std::cout << "Failed to open " << filePath << '\n';
                return std::nullopt;
            }

            std::string line;
            std::getline(ifs, line);

            while (std::getline(ifs, line)) {
                auto fields = split(line, '\t');
                result.push_back(fields);
            }

            return std::optional(result);
        }
    };
} // namespace Imdb::utils