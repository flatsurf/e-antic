renf_elem.h — Embedded Number Field Elements
============================================

Memory Layout
-------------

.. doxygentypedef:: renf_elem_t

.. doxygenstruct:: renf_elem
   :members:

.. doxygenfunction:: renf_elem_init
.. doxygenfunction:: renf_elem_clear
.. doxygenfunction:: renf_elem_swap

Setters
-------
.. doxygenfunction:: renf_elem_zero
.. doxygenfunction:: renf_elem_one
.. doxygenfunction:: renf_elem_gen

.. doxygenfunction:: renf_elem_set_si
.. doxygenfunction:: renf_elem_set_ui
.. doxygenfunction:: renf_elem_set
.. doxygenfunction:: renf_elem_set_fmpz
.. doxygenfunction:: renf_elem_set_mpz
.. doxygenfunction:: renf_elem_set_fmpq
.. doxygenfunction:: renf_elem_set_mpq
.. doxygenfunction:: renf_elem_set_fmpq_poly
.. doxygenfunction:: renf_elem_set_nf_elem


Embedding Refinement
--------------------

.. doxygenfunction:: renf_elem_set_evaluation

Properties and Conversion
-------------------------

.. doxygenfunction:: renf_elem_is_zero
.. doxygenfunction:: renf_elem_is_one
.. doxygenfunction:: renf_elem_is_integer
.. doxygenfunction:: renf_elem_is_rational
.. doxygenfunction:: renf_elem_sgn
.. doxygenfunction:: renf_elem_floor
.. doxygenfunction:: renf_elem_ceil

Floating point approximations
-----------------------------

.. doxygenfunction:: renf_elem_get_arb
.. doxygenfunction:: renf_elem_get_d

Comparisons
-----------

.. doxygengroup:: renf_elem_cmp

String Conversion and Printing
------------------------------

.. doxygenfunction:: renf_elem_get_str_pretty
.. doxygenfunction:: renf_elem_print_pretty

Randomization
-------------

.. doxygenfunction:: renf_elem_randtest

Unary operations
----------------

.. doxygenfunction:: renf_elem_neg
.. doxygenfunction:: renf_elem_inv

Binary operations
-----------------

.. doxygengroup:: renf_elem_binop
.. doxygenfunction:: renf_elem_fdiv

Other functions
---------------

.. doxygenfunction:: renf_elem_get_cfrac
.. doxygenfunction:: renf_elem_check_embedding
.. doxygenfunction:: renf_elem_relative_condition_number_2exp

