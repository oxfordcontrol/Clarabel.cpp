

<p align="center">
  <a href="https://github.com/oxfordcontrol/Clarabel.cpp/actions"><img src="https://github.com/oxfordcontrol/Clarabel.cpp/workflows/ci/badge.svg?branch=main"></a>
  <!-- <a href="https://codecov.io/gh/oxfordcontrol/Clarabel.rs"><img src="https://codecov.io/gh/oxfordcontrol/Clarabel.rs/branch/main/graph/badge.svg"></a> -->
  <a href="https://oxfordcontrol.github.io/ClarabelDocs/stable"><img src="https://img.shields.io/badge/Documentation-stable-purple.svg"></a>
  <a href="https://opensource.org/licenses/Apache-2.0"><img src="https://img.shields.io/badge/License-Apache%202.0-blue.svg"></a>
  <a href="https://github.com/oxfordcontrol/Clarabel.cpp/releases"><img src="https://img.shields.io/badge/Release-None-blue.svg"></a>
</p>

<p align="center">
  <a href="#features">Features</a> •
  <a href="#installation">Installation</a> •
  <a href="#license-">License</a> •
  <a href="https://oxfordcontrol.github.io/ClarabelDocs/stable">Documentation</a>
</p>

__Clarabel.cpp__ is a C/C++ wrapper of [Clarabel.rs](https://github.com/oxfordcontrol/Clarabel.rs).

# Installation

Clarabel.cpp uses CMake to generate the build system and requires the following dependencies:

- Rust
- Clarabel.rs (included as a submodule)
- A compiler that supports C11 and C++11
- Eigen (optional for the C++ interface)

You may install Eigen via the system package manager on Unix-like systems or vcpkg on Windows.

- Follow the [vcpkg](https://vcpkg.io/en/getting-started) instructions to compile vcpkg and add `vcpkg.exe` to `PATH`.

Ubuntu:

```sh
sudo apt install libeigen3-dev
```

Windows:

```sh
vcpkg install eigen3:x64-windows
```

- For 32-bit platforms, use `eigen3:x86-windows` instead of `eigen3:x64-windows`.

## Clone this repo

```sh
git clone --recurse-submodules https://github.com/oxfordcontrol/Clarabel.cpp.git
cd Clarabel.cpp
```

## Building the solver

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

You may specify a particular build system using the `-G` flag for `cmake` (e.g. `-G "Unix Makefiles"`).

### Windows

If you are using vcpkg on Windows, you may find the vcpkg toolchain file using:

```sh
vcpkg integrate install
```

and then specify the vcpkg toolchain file path and target triplet to generate the build system using:

```sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=VCPKG_TOOLCHAIN_PATH -DVCPKG_TARGET_TRIPLET=x64-windows
```

where `VCPKG_TOOLCHAIN_PATH` is the path to the vcpkg toolchain file.

- For 32-bit platforms, use `x86-windows` instead of `x64-windows`.

## Optional solver features

Clarabel.rs supports a variety of build options for semidefinite program (SDP) support, JSON file read/write, 3rd party linear solvers etc.  Feature options can be passed to cargo via CMake using `-DCLARABEL_CARGO_FEATURES = "feature1,feature2,..."`.

| Feature | Description |
|---------|-------------|
| `serde` | enables read/write of problem instances to .json files (enabled by default) |
| `faer-sparse` | enables "faer" as an optional direct_solve_method |
| `pardiso-mkl` | enables "mkl" as an optional direct_solve_method (Intel only) |
| `pardiso-panua` | enables "panua" as an optional direct_solve_method (requires separate license) |
| `pardiso` | enables both the "mkl" and "panua" options |
| `sdp-accelerate` | enables solution of SDPs using Apple's "accelerate" BLAS/LAPACK implementation (OSX only) |
| `sdp-mkl` | enables solution of SDPs using Intel's BLAS/LAPACK implementation (Intel only) |
| `sdp-openblas` | enables solution of SDPs using OpenBlas |
| `sdp-netlib` | enables solution of SDPs using the Netlib reference BLAS/LAPACK (not recommended) |
| `buildinfo` | adds a buildinfo function to the package that reports on the build configuration |

### Linking to Pardiso
To enable dynamic linking to MKL Pardiso, the MKL Pardiso libary (e.g. `libmkl_rt.so`) must be on the system library path (e.g. on `LD_LIBRARY_PATH` on Linux). Alternatively, set the `MKLROOT` environment variable to the root of the MKL installation or `MKL_PARDISO_PATH` to the location of the library. The Intel MKL library is available as part of the Intel oneAPI toolkit and is only available on x86_64 platforms.

To enable dynamic linking to Panua Pardiso, the Panua Pardiso library (e.g. `libpardiso.so`) must be on the system library path (e.g. on `LD_LIBRARY_PATH` on Linux). Alternatively, set the `PARDISO_PATH` environment variable to the location of the library.

Panua Pardiso is a commercial solver and requires a separate license.

## Unit tests

By default, unit tests are disabled to reduce build time. To enable unit tests, set `-DCLARABEL_BUILD_TESTS=true` in cmake.

## Release mode

The solver will build the Rust source in debug mode.   To build in release mode, set `-DCMAKE_BUILD_TYPE=Release` in cmake.

## Running examples

Examples for both C and C++ are available in `examples/c` and `examples/cpp` and can be run from the `build` directory using:

```sh
./examples/c/C_EXAMPLE
./examples/cpp/CPP_EXAMPLE
```

# Usage

- Link to the `libclarabel_c_shared` (shared library) or `libclarabel_c_static` (static library) target in CMake.
- `#include <clarabel.hpp>` in your C++ source files or <clarabel.h>` in your C source files.

# License 🔍
This project is licensed under the Apache License 2.0 - see the [LICENSE.md](LICENSE.md) file for details.
