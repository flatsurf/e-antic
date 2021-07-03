# e-antic : Computing with Real Embedded Number Fields

e-antic is a C library for exact computation with real embedded algebraic
numbers. It also offers C++ and Python interfaces.

The C library is built on top of [FLINT](https://flintlib.org/),
[Arb](https://fredrikj.net/arb/) and [antic](https://github.com/wbhart/antic)
and follows the same naming conventions.

The main focus of e-antic is on efficiency. For a more versatile C
library for exact real numbers computations we recommend
[Calcium](https://fredrikj.net/calcium/) by Fredrik Johansson.

The development of e-antic is coordinated on [github](https://github.com/flatsurf/e-antic).

## Installation

The preferred way to install e-antic is to use your package manager (e.g.
`apt-get` on Debian or Ubuntu, `pacman` on Arch Linux, `brew` on MacOS, etc).
See [repology for e-antic](https://repology.org/project/e-antic/packages).

The following describe alternative ways of installing e-antic.

* [Install with Conda](install_in_conda)
* [Build from Source](https://github.com/flatsurf/e-antic/#build-from-the-source-code-repository-or-a-tarball)
* [Conda Environment to Build from Source](https://github.com/flatsurf/e-antic/#build-with-conda-dependencies)

## C Library

* [Tour of the C Interface](overview_c)
* [Number Fields `renf.h`](doc_renf_h)
* [Number Field Elements `renf_elem.h`](doc_renf_elem_h)

The following headers extend [FLINT](https://flintlib.org/). They should not
be considered as part of the e-antic API and will be ported to FLINT
eventually.

* [Rational Polynomials `fmpq_poly_extra.h`](doc_fmpq_poly_extra_h)
* [Integral Polynomials `fmpz_poly_extra.h`](doc_fmpz_poly_extra_h)

## C++ Interface

* [Tour of the C++ Interface](overview_cxx)
* [Number Fields `renf_class.hpp`](doc_renf_class_hpp)
* [Number Field Elements `renf_elem_class.hpp`](doc_renf_elem_class_hpp)

## Python Interface

The Python interface relies on [cppyy](https://cppyy.readthedocs.io/en/latest/)
exposing automatic bindings to the C++ interface. The documentation of the
Python interface can be found [here](/e-antic/pyeantic).
