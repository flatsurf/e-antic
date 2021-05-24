# e-antic : computing with real embedded number fields

## Overview

e-antic is a C library for exact computation with real embedded algebraic
numbers. It also offers C++ and Python interfaces.

The C library is built on top of [flint](https://flintlib.org/),
[arb](https://fredrikj.net/arb/) and [antic](https://github.com/wbhart/antic)
and follows the same naming conventions.

The main focus of e-antic is on efficiency. For a more versatile C
library for exact real numbers computations we recommand
[calcium](https://fredrikj.net/calcium/) by Fredrik Johansson.

The development of e-antic is coordinated on [github](https://github.com/flatsurf/e-antic).

## Installation

The prefered way to install e-antic is to use your package manager (e.g.
`apt-get` on Debian or Ubuntu, `pacman` on archlinux, `brew` on MacOS, etc).
See [repology for e-antic](https://repology.org/project/e-antic/packages).

The following describe alternative ways of installing e-antic

* [Install with conda](install_in_conda.md)
* [Building from source](https://github.com/flatsurf/e-antic/#build-from-the-source-code-repository-or-a-tarball)
* [Conda environment to build from source](https://github.com/flatsurf/e-antic/#build-with-conda-dependencies)

## C library

* [tour of the C interface](overview_c)
* [number fields `renf.h`](doc_renf_h)
* [number field elements `renf_elem.h`](doc_renf_elem_h)

The following headers extend [flint](http://flintlib.org/). They should not
be considered as part of the e-antic API and will be ported to flint in the
near future.

* [rational polynomials `fmpq_poly_extra.h`](libeantic/doc_fmpq_poly_extra) (extending flint)
* [integral polynomials `fmpz_poly_extra.h`](libeantic/doc_fmpz_poly_extra) (extending flint)

## C++ interface

* [tour of the C++ interface](overview_cxx.md)
* [number fields `renf_class.hpp`](doc_renf_class_hpp)
* [number field elements `renf_elem_class.hpp`](doc_renf_elem_class_hpp)

## Python interface

The Python interface relies on [cppyy](https://cppyy.readthedocs.io/en/latest/)
that provides automatic bindings to the C++ interface. The documentation of the
Python interface can be found [here](/pyeantic).
