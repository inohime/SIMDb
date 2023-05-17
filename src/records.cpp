#include "records.hpp"
#include <thread>

namespace Imdb {
    // clang-format off
    void loadDatasets(utils::Reader *rd, Records &rc) {
        std::lock_guard lock(rc._mut);
        std::vector<std::thread> threads;
        
        threads.emplace_back(std::thread([&]() {
            if (auto reader = rd->read("../assets/movie.names.tsv")) {
                rc._names.reserve(100'000);
                for (const auto &recordData : *reader) {
                    NameRecord record = {
                        recordData[0], 
                        recordData[1], 
                        recordData[2], 
                        recordData[3],
                        utils::split(recordData[4], ',')
                    };

                    rc._names.push_back(record);
                }
            }
        }));

        threads.emplace_back(std::thread([&]() {
            if (auto reader = rd->read("../assets/movie.principals.tsv")) {
                rc._principals.reserve(100'000);
                for (const auto &recordData : *reader) {
                    PrincipalRecord record = {
                        recordData[0], 
                        recordData[2], 
                        recordData[5]
                    };

                    rc._principals.push_back(record);
                }
            }
        }));

        threads.emplace_back(std::thread([&]() {
            if (auto reader = rd->read("../assets/movie.titles.tsv")) {
                rc._titles.reserve(100'000);
                for (const auto &recordData : *reader) {
                    TitleRecord record = {
                        recordData[0],
                        recordData[2],
                        recordData[5],
                        recordData[6],
                        utils::split(recordData[8], ',')
                    };

                    rc._titles.push_back(record);
                }
            }
        }));

        for (auto &thread : threads) {
            thread.join();
        }

        rc._isSearchOk = true;
        rc._searchCondition.notify_one();
    }
    // clang-format on
} // namespace Imdb