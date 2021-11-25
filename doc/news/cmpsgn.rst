**Changed:**

* We do not claim anymore that `_cmp` functions return -1, 0, 1. Instead we
  only make a statement about the sign of the integer returned. In practice we
  still return -1, 0, 1 for the time being.

**Fixed:**

* We do not assume anymore that `_cmp` functions return -1, 0, 1. FLINT, GMP,
  and Arb only guarantee that these functions return a negative, zero, or
  positive integer. In practice their implementations return -1, 0, 1, however
  in some obscure case on Fedora/i686, this was not the case.
