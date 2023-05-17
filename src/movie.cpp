#include "movie.hpp"
#include <algorithm>
#include <format>

namespace Imdb {
    [[nodiscard]] constexpr std::vector<TitleRecord> Movie::search(std::string_view in, const Records &rc) {
        std::vector<std::string_view> splittedIn = utils::split(in, '+');

        return rc._titles | std::views::filter([&](const auto &trc) {
                   auto titleCpy =
                       trc._primaryTitle | std::views::transform(::tolower) | std::ranges::to<std::string>();
                   return std::ranges::all_of(splittedIn, [&](const auto &s) {
                       return titleCpy.find(s, 0) != std::string::npos;
                   });
               }) |
               std::ranges::to<std::vector>();
    }

    [[nodiscard]] constexpr std::vector<NameRecord> Movie::searchForActors(std::string_view in, const Records &rc) {
        std::vector<std::string_view> splittedIn = utils::split(in, '+');

        return rc._names | std::views::filter([&](const auto &nrc) {
                   auto nameCpy = nrc._primaryName | std::views::transform(::tolower) | std::ranges::to<std::string>();
                   return std::ranges::all_of(splittedIn, [&](const auto &s) {
                       return nameCpy.find(s, 0) != std::string::npos;
                   });
               }) |
               std::ranges::to<std::vector>();
    }

    constexpr void Movie::displayMovies(const std::vector<TitleRecord> &movies) {
        // 'operator<<<std::char_traits<char>>' cannot be used in a constant expression
        // that's why this is std::format
        std::cout << std::format("---------------\n");

        for (size_t pos = 0; pos < movies.size(); pos++) {
            auto &movie = movies[pos];

            std::cout << std::format("{}:\nTitle: {}\nYear: {}\nGenre: ", pos, movie._primaryTitle, movie._startYear);
            std::ranges::copy(movie._genres, std::ostream_iterator<std::string>(std::cout, " "));
            std::cout << std::endl;
            std::cout << std::format("---------------\n");
        }
    }

    constexpr void Movie::displayActors(const std::vector<NameRecord> &actors) {
        std::cout << std::format("---------------\n");

        for (size_t pos = 0; pos < actors.size(); pos++) {
            auto &actor = actors[pos];
            std::cout << std::format("{}:\n{}\n", pos, actor._primaryName);
            std::ranges::copy(actor._primaryProfession, std::ostream_iterator<std::string>(std::cout, ", "));
            std::cout << std::endl;
            std::cout << std::format("({}-{})\n", actor._birthYear, actor._deathYear);
            std::cout << std::format("---------------\n");
        }
    }

    constexpr void Movie::displayCast(std::string_view selectedMovie, Records &rc) {
        auto precs = rc._principals | std::views::filter([&](const auto &prc) {
                         return prc._titleID == selectedMovie && prc._character != "\\N";
                     }) |
                     std::ranges::to<std::vector>();

        auto nrecs = precs | std::views::transform([&](const auto &prc) {
                         return rc._names | std::views::filter([&](const auto &nrc) {
                                    return nrc._nameID == prc._nameID;
                                });
                     }) |
                     std::views::join | std::ranges::to<std::vector>();

        for (size_t i = 0; i < precs.size(); i++) {
            std::cout << std::format("{} {}\n", nrecs[i]._primaryName, precs[i]._character);
        }

        std::cout << std::endl;
    }

    constexpr void Movie::displayActorsTitles(std::string_view selectedActor, Records &rc) {
        // pick the only actor that matches the selected
        const auto &nrec = *(rc._names | std::views::filter([&](const auto &nrc) {
                                 return nrc._nameID == selectedActor;
                             }) |
                             std::views::take(1))
                                .begin();

        // get all of the characters this actor has played
        auto precs = rc._principals | std::views::filter([&](const auto &prc) {
                         return prc._nameID == nrec._nameID;
                     }) |
                     std::ranges::to<std::vector>();

        // get all of the titles this actor has partcipated in
        auto trecs = precs | std::views::transform([&](const auto &prc) {
                         return rc._titles | std::views::filter([&](const auto &trc) {
                                    return trc._titleID == prc._titleID;
                                });
                     }) |
                     std::views::join | std::ranges::to<std::vector>();

        for (size_t i = 0; i < precs.size(); i++) {
            std::cout << std::format("{} {} {}\n", trecs[i]._primaryTitle, trecs[i]._startYear, precs[i]._character);
        }
    }
} // namespace Imdb

namespace Imdb::Searcher {
    void movieSearch(OpMovie opMovie, OpRecords opRecs) {
        if (auto movie = *opMovie; auto recs = *opRecs) {
            if (!recs->_isSearchOk) {
                std::unique_lock lock(recs->_mut);
                recs->_searchCondition.wait(lock, [&] {
                    return recs->_isSearchOk;
                });
            }

            std::cout << "Enter search phrase: ";

            std::string in;
            if (std::cin >> in) {
                Imdb::utils::toLowercase(in);
            } else {
                std::cout << "Malinformed input\n";
                return;
            }

            auto movies = movie->search(in, std::ref(*recs));
            if (movies.empty()) {
                std::cout << "No match found!\n\n";
                return;
            }
            movie->displayMovies(movies);

            std::cout << "Select a movie to see its actors/actresses (-1 to go back to the previous menu): ";

            int32_t movieChoice;
            if (std::cin >> movieChoice) {
                if (movieChoice == -1) {
                    std::cout << std::endl;
                    return;
                } else if (movieChoice < -1 || movieChoice > static_cast<int>(movies.size())) {
                    std::cout << "Invalid option!\n";
                    return;
                }
                movie->displayCast(movies[movieChoice]._titleID, std::ref(*recs));
            } else {
                std::cout << "Malinformed input\n";
                return;
            }
        }
    }

    void actorSearch(OpMovie opMovie, OpRecords opRecs) {
        if (auto movie = *opMovie; auto recs = *opRecs) {
            if (!recs->_isSearchOk) {
                std::unique_lock lock(recs->_mut);
                recs->_searchCondition.wait(lock, [&] {
                    return recs->_isSearchOk;
                });
            }

            std::cout << "Search for actors/actresses: ";

            std::string in;
            while (in != "done") {
                if (std::cin >> in) {
                    Imdb::utils::toLowercase(in);
                    if (in == "done") {
                        std::cout << std::endl;
                        return;
                    }
                } else {
                    std::cout << "Malinformed input\n";
                    return;
                }

                auto actors = movie->searchForActors(in, std::ref(*recs));
                if (actors.empty()) {
                    std::cout << "No match found!\n\n";
                    return;
                }
                movie->displayActors(actors);

                std::cout << "Select an actor/actress to see movies (-1 to go back to the previous menu): ";

                int32_t actorChoice;
                if (std::cin >> actorChoice) {
                    if (actorChoice == -1) {
                        std::cout << std::endl;
                        return;
                    } else if (actorChoice < -1 || actorChoice > static_cast<int>(actors.size())) {
                        std::cout << "Invalid option!\n";
                        return;
                    }
                    movie->displayActorsTitles(actors[actorChoice]._nameID, std::ref(*recs));
                } else {
                    std::cout << "Malinformed input\n";
                    return;
                }

                std::cout << "Enter search phrase (type done to go back to the previous menu): ";
            }
        }
    }
} // namespace Imdb::Searcher