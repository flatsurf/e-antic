Tour of the C Interface
=======================

There are two main objects in e-antic: number fields, which are represented by a
:cpp:type:`renf_t`, and their elements, represented by a
:cpp:type:`renf_elem_t`. Functions that operate on number fields are defined in
the header :doc:`e-antic/renf.h <c_renf>` and have the prefix ``renf_``.
Functions that operate on elements of number fields are defined in the header
:doc:`e-antic/renf_elem.h <c_renf_elem>` and have the prefix ``renf_elem_``.

To work with e-antic, you first need to create a number field. Typically, you
will use the function :cpp:func:`renf_init` to create a number field from
its defining rational polynomial as a FLINT :c:type:`fmpq_poly_t` and an
approximation of one of its real roots as an :c:type:`arb_t`.

Once a `renf_t` has been initialized, its elements can be created with
:cpp:func:`renf_elem_init`. Eventually, use :cpp:func:`renf_elem_clear` to free
the memory taken by these elements, and use :cpp:func:`renf_clear` to free the
memory taken by the number field itself.

Example
-------

We want to construct a totally real field with minimal polynomial :math:`x^3 - 3x + 1`, represented by an :c:type:`fmpq_poly_t`. We include the appropriate headers
and define the polynomial:

.. code-block:: c

    #include <e-antic/fmpq_poly_extra.h>

    fmpq_poly_t poly;
    fmpq_poly_init(poly);
    fmpq_poly_set_str_pretty(poly, "a^3 - 3*a + 1", "a");

We fix one of its roots, which is approximately 0.34:

.. code-block:: c

    #include <flint/flint.h>
    #if __FLINT_RELEASE < 30000
    #include <arb.h>
    #else
    #include <flint/arb.h>
    #endif

    arb_t emb;
    arb_init(emb);
    arb_set_str(emb, "0.34 +/- 0.01", 32);

We construct the embedded number field and let :math:`a` be its generator:

.. code-block:: c

    #include <e-antic/renf.h>
    #include <e-antic/renf_elem.h>

    renf_t nf;
    renf_elem_t a, b;

    renf_init(nf, poly, emb, 64);

    renf_elem_init(a, nf);
    renf_elem_gen(a, nf);

We deallocate the embedding and the defining polynomial, as we will no longer need them:

.. code-block:: c

    fmpq_poly_clear(poly);
    arb_clear(emb);

We compute :math:`b = a^2 - 2` and print its value:

.. code-block:: c

    renf_elem_init(b, nf);

    renf_elem_mul(b, a, a, nf);
    renf_elem_sub_ui(b, b, 2, nf);

    renf_elem_print_pretty(b, "a", nf, 32, EANTIC_STR_ALG);
    // -> a^2 - 2

Finally, we deallocate the number field and its elements:

.. code-block:: c

    renf_elem_clear(a, nf);
    renf_elem_clear(b, nf);
    renf_clear(nf);

Note
----

The same example is also explained in detail in the documentation of
the :doc:`C++ interface <cxx_overview>`.
