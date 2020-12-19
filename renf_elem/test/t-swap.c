/*
    Copyright (C) 2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../../e-antic/renf_elem.h"

int main(void)
{
    renf_t nf1, nf2, nf3;
    renf_elem_t a1, a2, a3, b1, b2, b3;
    fmpq_t r1, r2, r3;
    int i;

    fmpq_init(r1);
    fmpq_init(r2);
    fmpq_init(r3);

    fmpq_set_si(r1, 2, 3);
    fmpq_set_si(r2, 5, 4);
    fmpq_set_si(r3, 7, 1);

    renf_init_nth_root_fmpq(nf1, r1, 1, 64);
    renf_init_nth_root_fmpq(nf2, r2, 2, 128);
    renf_init_nth_root_fmpq(nf3, r3, 5, 128);

    renf_elem_init(a1, nf1);
    renf_elem_init(a2, nf2);
    renf_elem_init(a3, nf3);

    /* set: a1 to be gen(nf1)^10 and a2 to gen(nf2)^10 */
    renf_elem_gen(a1, nf1);
    renf_elem_gen(a2, nf2);
    renf_elem_gen(a3, nf3);
    for (i = 0; i < 10; i++)
    {
        renf_elem_mul(a1, a1, a1, nf1);
        renf_elem_mul(a2, a2, a2, nf2);
        renf_elem_mul(a3, a3, a3, nf3);
    }

    renf_elem_init(b1, nf1);
    renf_elem_init(b2, nf2);
    renf_elem_init(b3, nf3);
    renf_elem_set(b1, a1, nf1);
    renf_elem_set(b2, a2, nf2);
    renf_elem_set(b3, a3, nf3);

    renf_elem_swap(b1, b2); /* b1 = a2, b2 = a1*/
    renf_elem_swap(b2, b3); /* b3 = a1, b2 = a3*/

    renf_elem_check_embedding(b1, nf2, 256);
    renf_elem_check_embedding(b2, nf3, 256);
    renf_elem_check_embedding(b3, nf1, 256);
    if (!renf_elem_equal(b1, a2, nf2) ||
        !renf_elem_equal(b2, a3, nf3) ||
        !renf_elem_equal(b3, a1, nf1))
    {
        fprintf(stderr, "swap failed\n");
        return 1;
    }

    fmpq_clear(r1);
    fmpq_clear(r2);
    fmpq_clear(r3);
    renf_elem_clear(a1, nf1);
    renf_elem_clear(a2, nf2);
    renf_elem_clear(a3, nf3);
    renf_elem_clear(b1, nf2);
    renf_elem_clear(b2, nf3);
    renf_elem_clear(b3, nf1);
    renf_clear(nf1);
    renf_clear(nf2);
    renf_clear(nf3);
}
