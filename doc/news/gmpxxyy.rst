**Performance:**

* Improved conversion from vectors of rational coefficients to `renf_elem_class` by using a faster code path in FLINT.

* Improved conversion from SageMath number fields to pyeantic `RealEmbeddedNumberField`. This **adds a dependency** of pyeantic on gmpxxyy.
