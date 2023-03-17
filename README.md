:information_source: This is a fork of Lucian Radu Teodorescu's wonderful [structured concurrency example](https://github.com/lucteo/structured_concurrency_example). I was unable to get his repo to build with my compiler (gcc 11) so created this fork, updated the submodule reference in `external/p2300` to the current (March 2023) head ([e697e74](https://github.com/NVIDIA/stdexec/commit/e697e746a019f1b5b1119da12fe32d7a96f86654)), and made the minimal changes required to get it to build.

For more information about this example check out Lucian's excellent talks
- [Structured Concurrency](https://youtu.be/Xq2IMOPjPs0) from the ACCU 2022 conference ([cued to example](https://youtu.be/Xq2IMOPjPs0?t=3889))
- [A Pattern Language for Structured Concurrency](https://youtu.be/0i2MnO2_uic) form CppCon 2022 ([cued to example](https://youtu.be/0i2MnO2_uic?t=2591))

# structured_concurrency_example
An example on using structured concurrency based on P2300 to build up a sever application

# Building
:warning: So far I have only built this repository with gcc 11, so if you are trying to build with another compiler be prepared to handle unexpected errors.

After cloning this repository, make sure that you have also cloned the external code required to build the project.
```sh
# from within this repository's root directory
git submodule init
git submodule update
```
Then create a `build` directory, install the required dependencies, and build the project.
```sh
mkdir build && cd build
conan install .. --build=missing
cmake ..
cmake --build .
```

# Running the example
See [here](doc/example.md) for instructions on how to run the example.
