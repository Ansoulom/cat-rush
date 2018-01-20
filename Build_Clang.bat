@echo off
mkdir "Build"
cd "Build"
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install .. -s compiler="Clang" --build missing
cmake .. -G "Clang"
cmake --build . --config Release
pause
