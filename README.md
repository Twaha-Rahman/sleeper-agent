# WIP

This is still an actively maintained project....

## TODO

- [] Replace the dummy `seed::Add` API with real daemon logic

- [] Read and parse `/proc/net/dev` at a set interval
- [] If condition is met, hold the inhibitor lock (using `org.freedesktop.login1`
D-Bus interface)
- [] Add daemon config handling
- [x] Add daemon lifecycle (and diagnostic) log writing functionality to systemd journal

# Running Tests

The tests use [GoogleTest](https://github.com/google/googletest), which is
fetched automatically on the first configure. No manual installation is needed.

To run all the tests, the following commands below should be run from the
*project root*:

   ```bash
   cmake -S . -B build
   cmake --build build -j12
   ctest --test-dir ./build --output-on-failure
   ```

We can also run the test binary directly for more detailed output:
```bash
./build/tests/test_libseed
```

# Running AddressSanitizer (ASAN)

To use `ASAN` to catch memory errors (buffer overflows, use-after-free, and leaks) at runtime.

1. Run the following command:
   ```bash
   cmake -B ./build -DCMAKE_BUILD_TYPE=Debug \
     -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer" \
     -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address"

   cmake --build ./build -j12 && ./build/sleeper-agent
   ```

Any memory errors will be reported in the output. To check for leaks specifically, run with:
```bash
ASAN_OPTIONS=detect_leaks=1 ./build/sleeper-agent
```

# Using GDB

Basically build the project using the `Debug` build type. That should add the
debug symbols. Then, basically open the built binary in GDB.

```bash
   cmake -B build -DCMAKE_BUILD_TYPE=Debug

   cmake --build build -j12 && ./build/sleeper-agent

   gdb ./build/sleeper-agent
```

## Setting breakpoints in shared libraries

To set breakpoints in shared libraries, we need to first run the program in
GDB. This will load the libraries and allow us to set breakpoints. For example:

```
b main
r
b libseed.cpp:5
```

## Seeing the source code in the upper-half of the screen (in TUI)

To see the source code while debugging using GDB, run the following commands
inside GDB:
```
layout src
```

We can also use `<C-X>A` to toggle it on and off.

Sometimes, when the program we're debugging might glitch out GDB TUI while
printing to stdout. To fix it, run the following command in `gdb`:
```
ref
```

