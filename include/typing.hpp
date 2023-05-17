#pragma once

#include <optional>
#include <utility>

namespace Imdb {
    class Movie;
    struct Records;

    using OpArgs = std::optional<std::pair<Movie *, Records *>>;
    using OpMovie = std::optional<Movie *>;
    using OpRecords = std::optional<Records *>;
} // namespace Imdb