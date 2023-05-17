#include "movie.hpp"
#include "menu.hpp"

int main() {
    Imdb::Records recs;
    Imdb::utils::Reader reader;
    Imdb::Movie movie;

    std::jthread loader(Imdb::loadDatasets, &reader, std::ref(recs));

    // clang-format off
    auto menu = Imdb::Menu::create("Select a menu option:")
                    .addItem(
                        "Search for movies",
                        Imdb::Searcher::movieSearch, std::make_pair(&movie, &recs))
                    .addItem(
                        "Search for actors/actresses",
                        Imdb::Searcher::actorSearch, std::make_pair(&movie, &recs))
                    .addItem("Exit", [&](Imdb::OpMovie, Imdb::OpRecords) {
                        Imdb::Menu::setAliveState(false);
                    }, std::nullopt)
                    .build();
    // clang-format on

    while (menu->isAlive()) {
        menu->display();
    }

    return 0;
}