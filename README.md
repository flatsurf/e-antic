E-ANTIC — (Real Embedded) Algebraic Number Theory
=================================================

E-ANTIC is a C/C++ library to deal with real embedded number fields built on
top of ANTIC (https://github.com/wbhart/antic). Its aim is to have as fast
as possible exact arithmetic operations and comparisons.

Source tarballs can be downloaded at https://github.com/videlec/e-antic/releases.

The dependencies are:

 - flint 2.6 (see http://flintlib.org).
 - arb (see http://arblib.org/)
 - antic (see https://github.com/wbhart/antic)

If you have cloned the source directory you will need to setup the
configure script and Makefile using autotools. That is

    $ git submodule update --init
    $ ./bootstrap.sh

If you obtained a tarball of the sources or if the preceding step
worked, you just have to do

    $ ./configure
    $ make
    $ make check
    $ make install

If you happen to have any of flint, arb, or antic installed in a non standard
directory you will have to specify the `CPPFLAGS` and `LDFLAGS` variables for
the configure script

    $ ./configure CPPFLAGS=-I/my/path/include LDFLAGS=-L/my/path/lib

For more detailed but generic instructions please refer to the INSTALL file.
