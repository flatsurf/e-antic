Tour of the C++ Interface
=========================

There are two main objects in e-antic: number fields, which are represented by a
:cpp:type:`eantic::renf_class`, and their elements, represented by a
:cpp:type:`eantic::renf_elem_class`. These objects are provided by the headers
:doc:`e-antic/renf_class.hpp <cxx_renf_class>` and
:doc:`e-antic/renf_elem_class.hpp <cxx_renf_elem_class>`, respectively.

To work with e-antic, you first need to create a number field, typically by
providing a minimal polynomial and an approximation of one of its roots.

Once a :cpp:type:`eantic::renf_class` has been initialized, its elements can be created
with the various constructors. The lifetime of all these objects is managed by
smart pointers, so you usually do not need to worry about allocating and
freeing resources manually.

Example
-------

We construct a totally real field with the minimal polynomial :math:`x^3 - 3x + 1` and
fix its real root, which is approximately 0.34:

.. code-block:: cpp

    #include <e-antic/renf_class.hpp>

    auto K = eantic::renf_class::make("x^3 - 3*x + 1", "x", "0.34 +/- .01");

Let `a` be the selected root of this number field:

.. code-block:: cpp

    #include <e-antic/renf_elem_class.hpp>

    auto a = K->gen();

We compute :math:`b = a^2 - 2` and print its value:

.. code-block:: cpp

    auto b = a.pow(2) - 2;

    std::cout << b;
    // -> (x^2 - 2 ~ -1.8793852)

Note
----

The same example is also explained in detail in the documentation of
the :doc:`C interface <c_overview>`.
