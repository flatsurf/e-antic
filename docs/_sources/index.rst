.. toctree::
   :caption: libeantic
   :maxdepth: 1
   :hidden:

   c_overview
   renf.h — number fields <c_renf>
   renf_elem.h — number field elements <c_renf_elem>

.. toctree::
   :caption: libeanticxx
   :maxdepth: 1
   :hidden:

   cxx_overview
   cxx_renf_class
   cxx_renf_elem_class

.. toctree::
   :maxdepth: 1
   :caption: pyeantic
   :hidden:

   python_overview
   Interface with realalg <python_realalg>
   Interface with SageMath <python_sage>

e-antic : Real Embedded Number Fields
=====================================

e-antic is a C library for exact computation with real embedded algebraic
numbers. It also offers C++ and Python interfaces.

The C library is built on top of `FLINT <https://flintlib.org/>`_ and follows the
same naming conventions.

The main focus of e-antic is on efficiency. For a more versatile C
library for exact real numbers computations we recommend Calcium which is a
part of FLINT.

The development of e-antic is coordinated on GitHub at https://github.com/flatsurf/e-antic.

Installation
------------

The preferred way to install e-antic is to use your package manager (e.g.,
``apt-get`` on Debian or Ubuntu, ``pacman`` on Arch Linux, ``brew`` on MacOS, etc).
See `repology for e-antic <https://repology.org/project/e-antic/packages>`_.

The following describe alternative ways of installing e-antic:

* `Build with pixi for Development <https://github.com/flatsurf/e-antic/#build-and-develop-e-antic-with-pixi>`_
* `Build from the Source Tarball <https://github.com/flatsurf/e-antic/#build-from-the-source-code-repository-or-a-tarball>`_

C Library
---------

* :doc:`Tour of the C Interface <c_overview>`
* :doc:`Number Fields renf.h <c_renf>`
* :doc:`Number Field Elements renf_elem.h <c_renf_elem>`

The following headers extend `FLINT <https://flintlib.org/>`_. They should not
be considered as part of the e-antic API and will be ported to FLINT eventually:

* :doc:`Rational Polynomials fmpq_poly_extra.h <c_fmpq_poly_extra>`
* :doc:`Integral Polynomials fmpz_poly_extra.h <c_fmpz_poly_extra>`

C++ Interface
-------------

* :doc:`Tour of the C++ Interface <cxx_overview>`
* :doc:`Number Fields renf_class.hpp <cxx_renf_class>`
* :doc:`Number Field Elements renf_elem_class.hpp <cxx_renf_elem_class>`

Python Interface
----------------

* :doc:`Tour of the Python Interface <python_overview>`
* :doc:`Interface with realalg <python_realalg>`
* :doc:`Interface with SageMath <python_sage>`

Feedback and Contributions
--------------------------

We are thrilled to learn about your ideas for e-antic, please have a look at
`this section
<https://github.com/flatsurf/e-antic/#feedback-and-contributions>`_ of our
README to reach out to us.

License
-------

e-antic is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License (LGPL) as published by the Free
Software Foundation; either version 3.0 of the License, or (at your option) any
later version. See https://www.gnu.org/licenses/.
