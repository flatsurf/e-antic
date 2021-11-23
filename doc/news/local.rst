**Changed:**

* Header file `e-antic.h` is now called `local.h`. There is still a header file
  `e-antic.h` that includes all of e-antic's C interface, so in particular
  `local.h`.
* Header file `renfxx_fwd.hpp` is now called `forward.hpp`. The old header file
  `renfxx_fwd.hpp` can still be used.
* Header file `renfxx.h` is now called `e-antic.hpp`. The old header file
  `renfxx.h` can still be used.

**Fixed:**

* Updated installation instructions for installation from conda-forge. The
  e-antic package there has been renamed to libeantic. The package `e-antic`
  still exists but is now essentially an alias for `libeantic` and `pyeantic`.
