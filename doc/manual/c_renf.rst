renf.h — Real Embedded Number Fields
====================================

.. doxygenfile:: e-antic/renf.h
   :sections: detaileddescription

Memory Layout
-------------

.. doxygentypedef:: renf_t

.. doxygenstruct:: renf
   :members:

Initialization, Allocation, Deallocation
----------------------------------------

.. doxygenfunction:: renf_init
.. doxygenfunction:: renf_init_nth_root_fmpq
.. doxygenfunction:: renf_init_set
.. doxygenfunction:: renf_clear
.. doxygenfunction:: renf_set_embeddings_fmpz_poly

Other Functions
---------------

.. doxygenfunction:: renf_refine_embedding
.. doxygenfunction:: renf_set_immutable
.. doxygenfunction:: renf_equal
.. doxygenfunction:: renf_degree
.. doxygenfunction:: renf_randtest
.. doxygenfunction:: renf_get_str
.. doxygenfunction:: renf_print
