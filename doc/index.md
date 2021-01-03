# e-antic : computing with real embedded number fields

## Overview

e-antic is a C library for exact computation with real embedded algebraic
numbers. It also offers C++ and Python interfaces.

The C library is built on top of [flint](flintlib.org/),
[arb](https://fredrikj.net/arb/) and [antic](https://github.com/wbhart/antic)
and follows the same naming conventions.

The main focus of e-antic is on efficiency. For a more versatile C
library for exact real numbers computations we recommand
[calcium](https://fredrikj.net/calcium/) by Fredrik Johansson.

The development of e-antic is coordinated on [github](https://github.com/flatsurf/e-antic).

## C library

* C installation specifics
* [tour of the C interface](overview_c.md)
* index of the C library

## C++ interface

* C++ installation specifics
* [tour of the C++ interface](overview_cpp.md)
* index of the C++ interface

## Python interface

The Python interface relies on [cppyy](https://cppyy.readthedocs.io/en/latest/)
that provides automatic bindings to the C++ interface. The documentation is
currently independent of the C/C++ documentation and can be found at
[pyeantic documentation](LINK_TO_PYEANTIC_DOCUMENTATION).

