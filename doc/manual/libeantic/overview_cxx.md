# Overview of the C++ interface

There are two main objects in e-antic. Number fields, which are represented by a
[eantic::renf_class](), and their elements, represented by a
[eantic::renf_elem_class](). These objects are provided by the headers
[e-antic/renf_class.hpp]() and [e-antic/renf_elem_class.hpp]() respectively.

To work with e-antic, you first need to create a number field. Typically, by
providing a minimal polynomial and an approximation of one of its roots.

Once a [eantic::renf_class]() has been initialized, its elements can be created
with the various [eantic::renf_elem_class]() contructors. Lifetime of all these
objects is taken care of by smart pointers, so you usually do not need to
worry about allocating and freeing resources manually.

## Example

We construct a totally real field with minimal polynomial $x^3 - 3x + 1$ and
fix its real root which is approximately 0.34.

```cpp
#include <e-antic/renf_class.hpp>

auto K = eantic::renf_class::make("x^3 - 3*x + 1", "x", "0.34 +/- .01");
```

Let `a` be the selected root of this number field:
```cpp
#include <e-antic/renf_elem_class.hpp>

auto a = K->gen();
```

We compute $b = a^2 - 2$ and print its value.

```cpp
auto b = a.pow(2) - 2;

std::cout << b;
// -> (x^2 - 2 ~ -1.8793852)
```

Note that the same example is also explained in detail in the documentation of
the [C interface](../overview_c).
