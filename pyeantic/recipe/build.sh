cd pyeantic

autoreconf -ivf
./configure --prefix="$PREFIX" --without-pytest
make install
