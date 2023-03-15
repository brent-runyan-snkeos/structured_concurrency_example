# structured_concurrency_example
An example on using structured concurrency based on P2300 to build up a sever application
# brent's notes
```sh
git clone https://github.com/lucteo/structured_concurrency_example.git
cd structured_concurrency_example
git submodule init
git submodule update
```

```sh
# external/p2300
mkdir build-win-clang && cd build-win-clang
conan install .. --build=missing -s build_type=Release -pr:b vs22-clang
cmake .. -G "Visual Studio 17 2022" -T ClangCL -A x64
```


```sh
sudo apt-get install pkg-config
```
```sh
mkdir build-wsl && cd build-wsl
conan install ..
cmake ..
```
