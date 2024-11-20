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
built on top of [ANTIC](https://github.com/wbhart/antic). It aims to provide
the fastest possible exact arithmetic operations and comparisons.

Source tarballs can be downloaded at https://github.com/flatsurf/e-antic/releases.

This repository contains two related projects:

* **libeantic** a C/C++ library
* **pyeantic** a Python wrapper for **libeantic**

The dependencies are:

 - either [FLINT 2.6, 2.7, 2.8, or 2.9](https://flintlib.org), [Arb](https://arblib.org/), and [ANTIC](https://github.com/wbhart/antic),
 - or [FLINT 3.x](https://flintlib.org);
 - [Boost](https://www.boost.org/) for the C++ library
 - [cppyy](https://cppyy.readthedocs.io/en/latest/) for the Python wrapper

## Build and Develop e-antic with pixi

If you have cloned the source repository, make sure to pull in all the
third-party header-only libraries by running:

```
git submodule update --init
```

If you are distributing e-antic or you are a purist who *only* wants to
interact with autotools directly, then you should skip to the [next
section](#build-from-the-source-code-repository-or-a-tarball). Otherwise, we
strongly recommend that you install [pixi](https://pixi.sh) and then use the
following commands:

* `pixi run test` build e-antic and run the libeantic and pyeantic test suites
* `pixi run sage` build e-antic and spawn SageMath with the local libeantic and pyeantic installed
* `pixi run doc` to build and preview the documentation
* `pixi run compile-commands` to generate a `compile_commands.json` that your IDE might be able to use to make sense of this project

<details>
<summary>What is pixi?</summary>

pixi is a tool based on
[conda](https://en.wikipedia.org/wiki/Conda_(package_manager)) &
[conda-forge](https://conda-forge.org) for developers so that we can all use
the same workflows in the same defined environments.

pixi allows us to ship a very opinionated setup to developers of e-antic,
namely a number of opinionated scripts with corresponding tested (and
opinionated) dependencies.

This makes the whole development experience much more reliable and
reproducible, e.g., the CI on GitHub Pull Requests runs with the exact same
setup, so if something fails there, you can just run the CI command to get
exactly the same behavior locally.
</details>

<details>
<summary>How do I use pixi?</summary>

If you have not used pixi before, the most relevant pixi command is:

```sh
pixi run TASK
```

Run `pixi task list` to see the available tasks.

All tasks are defined in the `pixi.toml` file and most are used somewhere in
our GitHub Continuous Integration setup, see .github/workflows/.
</details>

<details>
<summary>Why don't we add all things to the Makefiles but use pixi tasks?</summary>

Packagers do prefer a system that is as minimalistic as possible. Any
opinionated bit in the build system, such as setting compiler flags, usually
needs to be patched out by software distributions. That's why our Makefiles are
trying to follow the autoconfiscated standards as closely as possible. And
essentially all that pixi does is to call these Makefiles without you having to
figure out how everything works in detail.
</details>

<details>
<summary>Can I use configure & make with pixi?</summary>

More experienced developers may not want to use these tasks. You can also just
use the curated list of dependencies that pixi provides and drop into a shell
with these dependencies installed. For example, to run the libeantic test suite
directly, you could do:

```sh
pixi shell -e dev
./bootstrap
cd libeantic
./configure
make check
```

Note that the following section contains more details about this `configure &&
make` workflow that might be of interest to you.
</details>

## Build from the Source Code Repository or a Tarball

If you have cloned the source directory and you decided not to use pixi, you
will need to setup the configure script and Makefile using autotools. That is

    git submodule update --init
    ./bootstrap

If you obtained a tarball of the sources or if the preceding step
worked, you can now run

    ./configure
    make
    make check  # to run our test suite
    make install  # to install into /usr/local

If you happen to have any of FLINT, Arb, or ANTIC installed in a non standard
directory, you will have to specify the `CPPFLAGS` and `LDFLAGS` variables for
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

## How to Cite this Project

If you have used this project in the preparation of a publication, please cite
it as described [on our zenodo page](https://doi.org/10.5281/zenodo.5166953).

## Acknowledgements

* Julian Rüth's contributions to this project have been supported by the Simons
  Foundation Investigator grant of Alex Eskin.

## Maintainers

* [@saraedum](https://github.com/saraedum)
* [@videlec](https://github.com/videlec)
