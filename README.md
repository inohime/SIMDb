<h1 align="center">SIMDb</h1>
<p align="center">A minimal set IMDb tool using C++</p>
<p align="center">Searches require a '+' in-between keywords</p>

## Features
- Search for movies and actors/actresses
- View movies an actor or actress has appeared in

## Example

![SIMDb Example Video](./assets/SIMDb-Example.gif)

## Building
- This project uses C++20, make sure your compiler supports it
```shell
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=release -G "Ninja" -DCMAKE_CXX_COMPILER_LAUNCHER=ccache .. && ninja
```

- To clean up the executable, use the below command in your terminal
```shell
cmake --build ./build --target clean
```

- To setup clangd with this project
	1. Run the build step first!
	2. cd build (unless already in build folder)
	3. Run the following command
```shell
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -G "Ninja" ..
```