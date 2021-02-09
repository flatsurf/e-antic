# Overview of the C interface

The main structures for number field and number field elements are respectively
[renf_t BROKEN LINK](renf_t) and [renf_elem_t BROKEN LINK](renf_elem_t). These are
pointers to C struct and can be safely used as function arguments.
Functions that operate on them respectively start by
`renf_` and `renf_elem_`.

To initialize a number field, the most generic function is
[renf_init BROKEN LINK](renf_init) which requires number fields a [rational
flint polynomial (`fmpq_poly_t`)](http://flintlib.org/sphinx/fmpq_poly.html)
and a [arb real ball (`arb_t`)](https://fredrikj.net/arb/arb.html). To
deallocate use [renf_free BROKEN LINK](renf_free).

For initialization and deallocation of number field elements use
[renf_elem_init BROKEN LINK](renf_elem_init) and
[renf_elem_free BROKEN LINK](renf_elem_free).
```c
#include <e-antic.h>

int main() {
  fmpq_poly_t poly;
  arb_t emb;
  renf_t nf;
  renf_elem_t a, b;

  /* initialize and set the polynomial poly to x^3 - 4x + 1 */
  fmpq_poly_init(poly);
  fmpq_poly_set_str_pretty(poly, "a^3 - 3*a + 1", "a");

  /* initialize and set the real ball emb to approximately 0.34 */
  arb_init(emb);
  arb_set_str(emb, "0.34 +/- 0.01", 32);

  /* initialize the number field nf with poly and emb */
  renf_init(nf, poly, emb, 64);

  /* initialize a and b */
  renf_elem_init(a, nf);
  renf_elem_init(b, nf);

  /* sets a to the generator and b to a^2 - 2 */
  renf_elem_gen(a, nf);
  renf_elem_mul(b, a, a, nf);
  renf_elem_sub_ui(b, b, 2, nf);

  /* prints b */
  renf_elem_print_pretty(b, "a", nf, 32, EANTIC_STR_ALG);

  /* deallocation */
  fmpq_poly_free(p);
  arb_free(emb);
  renf_elem_free(a);
  renf_elem_free(b);
  renf_free(nf);

  return 0;
}
```
