@echo off
mkdir "Build"
cd "Build"
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install .. -s compiler="Visual Studio" -s build_type=Debug -s compiler.version=15 --build missing
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake -G "Visual Studio 15 2017 Win64"
cmake --build . --config Debug
pause
