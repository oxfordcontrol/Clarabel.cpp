
<h1 align="center" margin=0px>
  <img src="https://github.com/oxfordcontrol/ClarabelDocs/blob/main/docs/src/assets/logo-banner-light-rs.png#gh-light-mode-only" width=60%>
  <img src="https://github.com/oxfordcontrol/ClarabelDocs/blob/main/docs/src/assets/logo-banner-dark-rs.png#gh-dark-mode-only"   width=60%>
  <br>
Interior Point Conic Optimization for C/C++
</h1>
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

__Clarabel.cpp__ is a C/C++ implementation of an interior point numerical solver for convex optimization problems using a novel homogeneous embedding.  Clarabel.cpp solves the following problem:

$$
\begin{array}{r}
\text{minimize} & \frac{1}{2}x^T P x + q^T x\\\\[2ex]
 \text{subject to} & Ax + s = b \\\\[1ex]
        & s \in \mathcal{K}
 \end{array}
$$

with decision variables
$x \in \mathbb{R}^n$,
$s \in \mathbb{R}^m$
and data matrices
$P=P^\top \succeq 0$,
$q \in \mathbb{R}^n$,
$A \in \mathbb{R}^{m \times n}$, and
$b \in \mathbb{R}^m$.
The convex set $\mathcal{K}$ is a composition of convex cones.

__For more information see the Clarabel Documentation ([stable](https://oxfordcontrol.github.io/ClarabelDocs/stable) |  [dev](https://oxfordcontrol.github.io/ClarabelDocs/dev)).__

Clarabel is also available in a [Rust / Python](https://github.com/oxfordcontrol/Clarabel.rs) and a [Julia](https://github.com/oxfordcontrol/Clarabel.jl) implementation.
 

## Features

* __Versatile__: Clarabel.cpp solves linear programs (LPs), quadratic programs (QPs), second-order cone programs (SOCPs) and semidefinite programs (SDPs). It also solves problems with exponential and power cone constraints.
* __Quadratic objectives__: Unlike interior point solvers based on the standard homogeneous self-dual embedding (HSDE), Clarabel.cpp handles quadratic objectives without requiring any epigraphical reformulation of the objective.   It can therefore be significantly faster than other HSDE-based solvers for problems with quadratic objective functions.
* __Infeasibility detection__: Infeasible problems are detected using a homogeneous embedding technique.
* __Open Source__: Our code is available on [GitHub](https://github.com/oxfordcontrol/Clarabel.cpp) and distributed under the Apache 2.0 License

# Installation

Clarabel.cpp uses CMake to generate the build system and requires the following dependencies:

- Rust
- Clarabel.rs (included as a submodule)
- A compiler that supports C11 and C++11
- Eigen (optional for the C++ interface)

You may install Eigen via the system package manager on Unix-like systems or [vcpkg](https://vcpkg.io/en/getting-started) on Windows.

Ubuntu:

```sh
sudo apt install libeigen3-dev
```

Windows:

```sh
vcpkg install eigen3:x64-windows
```

## Clone this repo

```sh
git clone --recurse-submodules https://github.com/oxfordcontrol/Clarabel.cpp.git
cd Clarabel.cpp
```

## Build

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

You may specify a particular build system using the `-G` flag for `cmake` (e.g. `-G "Unix Makefiles"`).

### Windows

If you are using vcpkg on Windows, you may need to specify the path to the vcpkg toolchain file when generating the build system using `cmake ..`:

```sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=PATH_TO_VCPKG/scripts/buildsystems/vcpkg.cmake
```

The path can be obtained by running:

```sh
vcpkg integrate install
```

## Run examples

Examples for both C and C++ are available in `examples/c` and `examples/cpp` and can be run from the `build` directory using the following commands:

```sh
./examples/c/example_NAME
./examples/cpp/example_NAME
```

# License 🔍
This project is licensed under the Apache License 2.0 - see the [LICENSE.md](LICENSE.md) file for details.
