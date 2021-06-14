# Overview of the C interface

There are two main objects in e-antic. Number fields, which are represented by a
[renf_t](), and their elements, represented by a [renf_elem_t](). Functions
that operate on number fields are defined in the header [e-antic/renf.h]() and
have the prefix `renf_`. Functions that operate on elements of number fields
are defined in the header [e-antic/renf_elem.h]() and have the prefix
`renf_elem_`.

To work with e-antic, you first need to create a number field. Typically, you
will use the function [renf_init]() to create a number field from its defining
rational polynomial as a FLINT [fmpq_poly_t]() and an
approximation of one of its real roots as an [arb_t]().

Once a [renf_t]() has been initialized, its elements can be created with
[renf_elem_init](). Eventually, use [renf_elem_clear]() to free the memory
taken by these elements, and use [renf_clear]() to free the memory taken
by the number field itself.

## Example

We want to construct a totally real field with minimal polynomial $x^3 - 3x +
1$, represented by an `fmpq_poly_t`. We include the appropriate headers
and define the polynomial.

```c
#include <e-antic/fmpq_poly_extra.h>

fmpq_poly_t poly;
fmpq_poly_init(poly);
fmpq_poly_set_str_pretty(poly, "a^3 - 3*a + 1", "a");
```

We fix one of its roots which is approximately 0.34.

```c
#include <arb.h>

arb_t emb;
arb_init(emb);
arb_set_str(emb, "0.34 +/- 0.01", 32);
```

We construct the embedded number field and let $a$ be its generator.

```c
#include <e-antic/renf.h>
#include <e-antic/renf_elem.h>

renf_t nf;
renf_elem_t a, b;

/* initialize and set the real ball emb to approximately 0.34 */
arb_init(emb);
arb_set_str(emb, "0.34 +/- 0.01", 32);

renf_init(nf, poly, emb, 64);

renf_elem_init(a, nf);
renf_elem_gen(a, nf);
```

We deallocate the embedding and the defining polynomial, as we will no longer need them.

```c
fmpq_poly_clear(poly);
arb_clear(emb);
```

We compute $b = a^2 - 2$ and print its value.

```c
renf_elem_init(b, nf);

renf_elem_mul(b, a, a, nf);
renf_elem_sub_ui(b, b, 2, nf);

renf_elem_print_pretty(b, "a", nf, 32, EANTIC_STR_ALG);
// -> a^2 - 2
```

Finally, we deallocate the number field and its elements.

```c
renf_elem_clear(a, nf);
renf_elem_clear(b, nf);
renf_clear(nf);
```
