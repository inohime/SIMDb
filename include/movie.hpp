#pragma once

#include "typing.hpp"
#include "records.hpp"

namespace Imdb {
    class Movie {
    public:
        [[nodiscard]] constexpr std::vector<TitleRecord> search(std::string_view in, const Records &rc);
        [[nodiscard]] constexpr std::vector<NameRecord> searchForActors(std::string_view in, const Records &rc);
        constexpr void displayMovies(const std::vector<TitleRecord> &movies);
        constexpr void displayActors(const std::vector<NameRecord> &actors);
        constexpr void displayCast(std::string_view selectedMovie, Records &rc);
        constexpr void displayActorsTitles(std::string_view selectedActor, Records &rc);
    };
} // namespace Imdb

namespace Imdb::Searcher {
    void movieSearch(OpMovie, OpRecords);
    void actorSearch(OpMovie, OpRecords);
} // namespace Imdb::Searcher