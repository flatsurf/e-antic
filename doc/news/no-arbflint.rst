**Added:**

* Added `renf_class::construction()` which produces the parameters that can be used to create a `renf_class` with `renf_class::make()`.

**Fixed:**

* Removed dependency of `renfxx_cereal.h` on arb and FLINT, i.e., one does not need to `-larb -lflint` anymore when using cereal.
