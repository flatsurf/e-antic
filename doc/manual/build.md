# Install e-antic from source

If you have cloned the source directory you will need to setup the
configure script and Makefile using autotools. That is

```console
    git submodule update --init
    ./bootstrap
```

If you obtained a tarball of the sources or if the preceding step
worked, you just have to do

```console
    ./configure
    make
    make check
    make install
```

If you happen to have any of FLINT, Arb, or ANTIC installed in a non standard
directory you will have to specify the `CPPFLAGS` and `LDFLAGS` variables for
the configure script

```console
    ./configure CPPFLAGS=-I/my/path/include LDFLAGS=-L/my/path/lib
```

For best performance run `CFLAGS="-O3" CXXFLAGS="-O3" ./configure` instead of
`./configure`.  You might want to add `-g3` to `CFLAGS` and `CXXFLAGS` which
does not hurt performance but gives a better debugging experience. For the best
debugging experience, you might want to replace `-O3` with `-Og` or even `-O0`
but the latter results in poor performance.

If your compiler supports it, you can try to add `-fvisibility=hidden
-fvisibility-inlines-hidden` to your `CXXFLAGS`. This hides internal bits in
the resulting library which have lead to crashes in the past due to conflicting
header-only libraries.

If your linker supports it, you should use `./configure --with-version-script`
to shrink the resulting shared library to an exact curated list of versioned
symbols.

perf works well to profile when you make sure that `CFLAGS` and `CXXFLAGS`
contain `-fno-omit-framepointer`. You can then for example run our test suite
with `perf record --call-graph dwarf make check`. Apart from perf itself there
are several ways to analyze the output,
[hotspot](https://github.com/KDAB/hotspot) might be the most convenient one at
the time of this writing.

For more detailed but generic instructions please refer to the INSTALL file.
