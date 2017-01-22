#include "arb.h"

void arb_add_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_add(a, b, x, prec);
    arb_clear(x);
}

void arb_sub_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_sub(a, b, x, prec);
    arb_clear(x);
}

void arb_mul_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_mul(a, b, x, prec);
    arb_clear(x);
}

void arb_div_fmpq(arb_t a, const arb_t b, const fmpq_t c, slong prec)
{
    arb_t x;
    arb_init(x);
    arb_set_fmpq(x, c, prec);
    arb_div(a, b, x, prec);
    arb_clear(x);
}


