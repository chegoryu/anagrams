# Anagrams

Anagrams for lksh

# Build

Warning: "facil" framework updated and does not work now. Use "facil.iofix" branch to build.

To build you need the "facil" framework, please run the following command to get it:

git clone -b stable https://github.com/boazsegev/facil.io extern/facil.io

Then build as usual CMake project:

mkdir build && cd build && cmake .. && make

# Deps:

cmake pkg-config build-essential libicu-dev
