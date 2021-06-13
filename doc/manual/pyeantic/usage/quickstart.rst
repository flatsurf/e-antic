Quickstart
==========

The Python interface is automatically generated from the C++ code using
`cppyy <https://cppyy.readthedocs.io/en/latest/>`_. In particular, all classes,
functions and methods available from C++ are available from Python. We
reproduce here the example from the `C++ overview <../../libeantic/overview_cxx>`_.

::

    >>> import pyeantic
    >>> K = pyeantic.eantic.renf_class.make("a^3 - 3*a + 1", "a", "0.34 +/- 0.01", 64)
    >>> a = K.gen()
    >>> b = a * a - 2
    >>> print(b)
    (a^2 - 2 ~ -1.8793852)

The Python interface also provides conversions from `Sagemath <https://www.sagemath.org>`_, see :ref:`sage_interface` and the `realalg <https://github.com/MarkCBell/realalg>`_ library, see :ref:`realalg_interface`.

