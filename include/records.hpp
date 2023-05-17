#pragma once

#include "utils.hpp"
#include <condition_variable>
#include <mutex>

namespace Imdb {
    struct NameRecord {
        // nconst (alphanumeric unique identifier of the name/person)
        std::string _nameID;
        // name of person
        std::string _primaryName;
        // person's date of birth
        std::string _birthYear;
        // person's date of death
        std::string _deathYear;
        // top 3 professions of the person
        std::vector<std::string> _primaryProfession;
    };

    struct PrincipalRecord {
        // tconst (alphanumeric unique identifier of the title)
        std::string _titleID;
        // nconst (alphanumeric unique identifier of the name/person)
        std::string _nameID;
        // name of character if applicable
        std::string _character;
    };

    struct TitleRecord {
        // tconst (alphanumeric unique identifier of the title)
        std::string _titleID;
        // movie title
        std::string _primaryTitle;
        // movie release date
        std::string _startYear;
        // movie last aired date
        std::string _endYear;
        // all genres applied to the movie
        std::vector<std::string> _genres;
    };

    struct Records {
        std::vector<NameRecord> _names;
        std::vector<PrincipalRecord> _principals;
        std::vector<TitleRecord> _titles;
        std::condition_variable _searchCondition;
        std::mutex _mut;
        bool _isSearchOk {false};
    };

    void loadDatasets(utils::Reader *, Records &);
} // namespace Imdb