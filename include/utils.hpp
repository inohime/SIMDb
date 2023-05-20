#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <vector>
#include <cstdint>

namespace Imdb::utils {
    template <class T>
    concept StringType = std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>;

    // const& is redundant for string_view but not for string.
    template <StringType T> [[nodiscard]] inline constexpr std::vector<T> split(const T &line, char delim) {
        auto splitIn = line | std::views::split(delim) | std::views::transform([](auto &&s) {
                   return T(s.begin(), s.end());
               });

        return std::vector<T>(splitIn.begin(), splitIn.end());
    }

    inline constexpr void toLowercase(std::string &in) {
        for (auto &c : in) {
            c = std::tolower(static_cast<uint8_t>(c));
        }
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