@echo off
cd "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build"
@call "vcvars64"
cd "%~dp0"
mkdir "Visual Studio"
cd "Visual Studio"
cmake -G "Visual Studio 15 2017 Win64" ..
MSBuild CatRush.sln