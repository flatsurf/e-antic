set -exo pipefail

cd libeantic

autoreconf -ivf
export CXXFLAGS="-UNDEBUG -g3 $CXXFLAGS"
export CFLAGS="-UNDEBUG -g3 $CFLAGS"
./configure --prefix="$PREFIX" --without-benchmark
make install
