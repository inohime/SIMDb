Small IMDb

![imdb logo banner](./assets/banner.png)

How to build:
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=release -G "Ninja" .. && ninja

If you need to clean up the files:
cmake --build ./build --target clean

Setup clangd:
run build step first!
cd build (unless already in build folder)
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -G "Ninja" ..

Searches require a '+' inbetween keywords

Tests:
Search for movies -> God+Father+II
Output: (index number, title, year, genres)
Verify it's correct!

Search for actors -> scarlett+johan
Output: (index number, name, birth year - death year, roles)

Setup with Docker and Catch2 (if necessary)