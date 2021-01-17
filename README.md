# E-ANTIC — (Real Embedded) Algebraic Number Theory

E-ANTIC is a C/C++/Python library to deal with real embedded number fields
built on top of ANTIC (https://github.com/wbhart/antic). Its aim is to have as
fast as possible exact arithmetic operations and comparisons.

Source tarballs can be downloaded at https://github.com/flatsurf/e-antic/releases.

This repository contains two related projects:

* **libeantic** a C/C++ library
* **pyeantic** a Python wrapper for **libeantic**

The dependencies are:

 - [FLINT 2.6](http://flintlib.org)
 - [Arb](http://arblib.org/)
 - [ANTIC](https://github.com/wbhart/antic)

## Build from the Source Code Repository or a Tarball

If you have cloned the source directory you will need to setup the
configure script and Makefile using autotools. That is

    git submodule update --init
    ./bootstrap

If you obtained a tarball of the sources or if the preceding step
worked, you just have to do

    ./configure
    make
    make check
    make install

If you happen to have any of FLINT, Arb, or ANTIC installed in a non standard
directory you will have to specify the `CPPFLAGS` and `LDFLAGS` variables for
the configure script

    ./configure CPPFLAGS=-I/my/path/include LDFLAGS=-L/my/path/lib

For best performance run `CFLAGS="-O3" CXXFLAGS="-O3" ./configure` instead of
`./configure`.  You might want to add `-g3` to `CFLAGS` and `CXXFLAGS` which
does not hurt performance but gives a better debugging experience. For the best
debugging experience, you might want to replace `-O3` with `-Og` or even `-O0`
but the latter results in poor performance.

If your compiler supports it, you can try to add `-fvisibility=hidden
-fvisibility-inlines-hidden` to your `CXXFLAGS`. This hides internal bits in
the resulting library which have lead to crashes in the past due to conflicting
header-only libraries.

If your linker supports it, you should use `./configure --with-version-script`
to shrink the resulting shared library to an exact curated list of versioned
symbols.

perf works well to profile when you make sure that `CFLAGS` and `CXXFLAGS`
contain `-fno-omit-framepointer`. You can then for example run our test suite
with `perf record --call-graph dwarf make check`. Apart from perf itself there
are several ways to analyze the output,
[hotspot](https://github.com/KDAB/hotspot) might be the most convenient one at
the time of this writing.

For more detailed but generic instructions please refer to the INSTALL file.

## Install with Conda

You can install this package with conda. Download and install
[Miniconda](https://conda.io/miniconda.html), then run

    conda config --add channels conda-forge
    conda create -n eantic -c flatsurf libeantic libeanticxx pyeantic
    conda activate eantic

The latest (experimental) versions for conda are:

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Build](https://img.shields.io/badge/recipe-libeantic-green.svg)](https://anaconda.org/flatsurf/libeantic) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/libeantic.svg)](https://anaconda.org/flatsurf/libeantic) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/libeantic.svg)](https://anaconda.org/flatsurf/libeantic) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/libeantic.svg)](https://anaconda.org/flatsurf/libeantic) |
| [![Build](https://img.shields.io/badge/recipe-pyeantic-green.svg)](https://anaconda.org/flatsurf/pyeantic) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/pyeantic.svg)](https://anaconda.org/flatsurf/pyeantic) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/pyeantic.svg)](https://anaconda.org/flatsurf/pyeantic) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/pyeantic.svg)](https://anaconda.org/flatsurf/pyeantic) |

## Install with your Distribution Tools

Some Versions of E-ANTIC might also be available as part of your
[distribution](https://repology.org/project/e-antic/packages).
 
## Build with Conda Dependencies

To build all of e-antic package, you need a fairly recent C++ compiler and
probably some packages that might not be readily available on your system. If
you don't want to use your distribution's packages, you can provide these
dependencies with conda. Download and install
[Miniconda](https://conda.io/miniconda.html), then run

    conda create -n e-antic-build ccache
    conda env update -n e-antic-build -f libeantic/environment.yml
    conda env create -n e-antic-build -f pyeantic/environment.yml
    conda activate e-antic-build
    export CPPFLAGS="-isystem $CONDA_PREFIX/include"
    export CFLAGS="$CPPFLAGS"
    export LDFLAGS="-L$CONDA_PREFIX/lib -Wl,-rpath-link=$CONDA_PREFIX/lib"
    export CC="ccache cc"
    export CXX="ccache c++"
    git clone --recurse-submodules https://github.com/flatsurf/e-antic.git
    cd e-antic
    ./bootstrap
    ./configure --prefix="$CONDA_PREFIX"
    make
    make check| [![Build](https://img.shields.io/badge/recipe-libeantic-green.svg)](https://anaconda.org/flatsurf/libeantic) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/libeantic.svg)](https://anaconda.org/flatsurf/libeantic) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/libeantic.svg)](https://anaconda.org/flatsurf/libeantic) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/libeantic.svg)](https://anaconda.org/flatsurf/libeantic) |

