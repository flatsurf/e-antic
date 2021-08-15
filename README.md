<p align="center">
    <img alt="logo" src="https://github.com/flatsurf/e-antic/raw/master/logo.svg?sanitize=true" width="300px">
</p>

<h1><p align="center">e-antic</p></h1>

<p align="center">
  <img src="https://img.shields.io/badge/License-LGPL_3.0_or_later-blue.svg" alt="License: LGPL 3.0 or later">
  <a href="https://github.com/flatsurf/e-antic/actions/workflows/test.yml"><img src="https://github.com/flatsurf/e-antic/actions/workflows/test.yml/badge.svg" alt="Test"></a>
  <a href="https://codecov.io/gh/flatsurf/e-antic"><img src="https://codecov.io/gh/flatsurf/e-antic/branch/master/graph/badge.svg" alt="Coverage"></a>
  <a href="https://doi.org/10.5281/zenodo.5166953"><img src="https://zenodo.org/badge/DOI/10.5281/zenodo.5166953.svg" alt="DOI 10.5281/zenodo.5166953"></a>
</p>

<p align="center">(Real Embedded) Algebraic Number Theory</p>
<hr>

e-antic is a C/C++/Python library to deal with real embedded number fields
built on top of [ANTIC](https://github.com/wbhart/antic). Its aim is to have as
fast as possible exact arithmetic operations and comparisons.

Source tarballs can be downloaded at https://github.com/flatsurf/e-antic/releases.

This repository contains two related projects:

* **libeantic** a C/C++ library
* **pyeantic** a Python wrapper for **libeantic**

The dependencies are:

 - [FLINT 2.6 or 2.7](https://flintlib.org)
 - [Arb](https://arblib.org/)
 - [ANTIC](https://github.com/wbhart/antic)
 - [Boost](https://www.boost.org/) for the C++ library
 - [cppyy](https://cppyy.readthedocs.io/en/latest/) for the Python wrapper

## Build from the Source Code Repository or a Tarball

If you have cloned the source directory you will need to setup the
configure script and Makefile using autotools. That is

    git submodule update --init
    ./bootstrap

If you obtained a tarball of the sources or if the preceding step
worked, you just have to do

    ./configure
    make
    make check  # to run our test suite
    make install  # to install into /usr/local

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

## Installation

See [our documentation](https://flatsurf.github.io/e-antic/libeantic/#installation)
for installation instructions.

## Run with binder in the Cloud

You can try out the projects in this repository in a very limited environment
online by clicking the following links:

* **libeantic** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/flatsurf/e-antic/master?filepath=binder%2FSample.libeantic.ipynb)
* **pyeantic** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/flatsurf/e-antic/master?filepath=binder%2FSample.pyeantic.ipynb)

## Build with conda-forge Dependencies

To build all of e-antic, you need a fairly recent C++ compiler and probably
some packages that might not be readily available on your system. If you don't
want to use your distribution's packages, you can use these dependencies from
[conda-forge](https://conda-forge.org). Download and install
[Mambaforge](https://github.com/conda-forge/miniforge#mambaforge), then run

    mamba create -n e-antic-build ccache
    mamba env update -n e-antic-build -f libeantic/environment.yml
    mamba env update -n e-antic-build -f pyeantic/environment.yml
    mamba env update -n e-antic-build -f doc/environment.yml
    conda activate e-antic-build
    export CC="ccache cc"  # to speed up future compilation
    export CXX="ccache c++"  # to speed up future compilation
    git clone --recurse-submodules https://github.com/flatsurf/e-antic.git
    cd e-antic
    ./bootstrap
    ./configure --prefix="$CONDA_PREFIX"
    make
    make check  # to run our test suite
    make html  # to build the documentation

## How to Cite this Project

If you have used this project in the preparation of a publication, please cite
it as described [on our zenodo page](https://doi.org/10.5281/zenodo.5166953).

## Acknowledgements

* Julian Rüth's contributions to this project have been supported by the Simons
  Foundation Investigator grant of Alex Eskin.

## Maintainers

* [@saraedum](https://github.com/saraedum)
* [@videlec](https://github.com/videlec)
