#pragma once

#include "utils.hpp"
#include <condition_variable>
#include <mutex>

namespace Imdb {
    struct NameRecord {
        // nconst (alphanumeric unique identifier of the name/person)
        std::string nameID;
        // name of person
        std::string primaryName;
        // person's date of birth
        std::string birthYear;
        // person's date of death
        std::string deathYear;
        // top 3 professions of the person
        std::vector<std::string> primaryProfession;
    };

    struct PrincipalRecord {
        // tconst (alphanumeric unique identifier of the title)
        std::string titleID;
        // nconst (alphanumeric unique identifier of the name/person)
        std::string nameID;
        // name of character if applicable
        std::string character;
    };

    struct TitleRecord {
        // tconst (alphanumeric unique identifier of the title)
        std::string titleID;
        // movie title
        std::string primaryTitle;
        // movie release date
        std::string startYear;
        // movie last aired date
        std::string endYear;
        // all genres applied to the movie
        std::vector<std::string> genres;
    };

    struct Records {
        std::vector<NameRecord> names;
        std::vector<PrincipalRecord> principals;
        std::vector<TitleRecord> titles;
        std::condition_variable searchCondition;
        std::mutex mut;
        bool isSearchOk {false};
    };

    void loadDatasets(utils::Reader *, Records &);
} // namespace Imdb