#include "records.hpp"
#include <thread>

namespace Imdb {
    // clang-format off
    void loadDatasets(utils::Reader *rd, Records &rc) {
        std::lock_guard lock(rc.mut);
        std::vector<std::thread> threads;
        
        threads.emplace_back(std::thread([&]() {
            if (auto reader = rd->read("../assets/movie.names.tsv")) {
                rc.names.reserve(100'000);
                for (const auto &recordData : *reader) {
                    NameRecord record = {
                        recordData[0], 
                        recordData[1], 
                        recordData[2], 
                        recordData[3],
                        utils::split(recordData[4], ',')
                    };

                    rc.names.push_back(record);
                }
            }
        }));

        threads.emplace_back(std::thread([&]() {
            if (auto reader = rd->read("../assets/movie.principals.tsv")) {
                rc.principals.reserve(100'000);
                for (const auto &recordData : *reader) {
                    PrincipalRecord record = {
                        recordData[0], 
                        recordData[2], 
                        recordData[5]
                    };

                    rc.principals.push_back(record);
                }
            }
        }));

        threads.emplace_back(std::thread([&]() {
            if (auto reader = rd->read("../assets/movie.titles.tsv")) {
                rc.titles.reserve(100'000);
                for (const auto &recordData : *reader) {
                    TitleRecord record = {
                        recordData[0],
                        recordData[2],
                        recordData[5],
                        recordData[6],
                        utils::split(recordData[8], ',')
                    };

                    rc.titles.push_back(record);
                }
            }
        }));

        for (auto &thread : threads) {
            thread.join();
        }

        rc.isSearchOk = true;
        rc.searchCondition.notify_one();
    }
    // clang-format on
} // namespace Imdb