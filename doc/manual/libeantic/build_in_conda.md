# Setup conda environment to build from source

To build all of e-antic package, you need a fairly recent C++ compiler and
probably some packages that might not be readily available on your system. If
you don't want to use your distribution's packages, you can provide these
dependencies with conda. Download and install
[Miniconda](https://conda.io/miniconda.html), then run

```console
    conda create -n e-antic-build ccache
    conda env update -n e-antic-build -f libeantic/environment.yml
    conda env update -n e-antic-build -f pyeantic/environment.yml
    conda env update -n e-antic-build -f doc/environment.yml
    conda activate e-antic-build
    export CPPFLAGS="-isystem $CONDA_PREFIX/include"
    export CFLAGS="$CPPFLAGS"
    export LDFLAGS="-L$CONDA_PREFIX/lib -Wl,-rpath-link=$CONDA_PREFIX/lib"
    export CC="ccache cc"
    export CXX="ccache c++"
    git clone --recurse-submodules https://github.com/flatsurf/e-antic.git
    cd e-antic
    ./bootstrap
    ./configure --prefix="$CONDA_PREFIX"
    make
    make check
    make html # to build the documentation
```
