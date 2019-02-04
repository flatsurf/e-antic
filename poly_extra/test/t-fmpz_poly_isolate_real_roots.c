/*
    Copyright (C) 2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/poly_extra.h>
#include <stdio.h>

/* check that a (= approximation of polynomial root) is in between */
/* c 2^k and (c+1) 2^k (c and k are produced by root isolation).   */
int check_isolation(fmpz * c, slong k, fmpq_t a)
{
    fmpq_t r1, r2;

    fmpq_init(r1);
    fmpq_init(r2);

    fmpz_set(fmpq_numref(r1), c);
    fmpz_one(fmpq_denref(r1));
    fmpq_set(r2, r1);
    fmpq_add_si(r2, r2, 1);
    if (k > 0)
    {
        fmpq_mul_2exp(r1, r1, k);
        fmpq_mul_2exp(r2, r2, k);
    }
    else if (k < 0)
    {
        fmpq_div_2exp(r1, r1, -k);
        fmpq_div_2exp(r2, r2, -k);
    }
    if (fmpq_cmp(r1, a) >= 0 || fmpq_cmp(r2, a) <= 0)
    {
        fprintf(stderr, "a  = "); fmpq_fprint(stderr, a); fprintf(stderr, "\n");
        fprintf(stderr, "r1 = "); fmpq_fprint(stderr, r1); fprintf(stderr, "\n");
        fprintf(stderr, "r2 = "); fmpq_fprint(stderr, r2); fprintf(stderr, "\n");
        return 1;
    }

    fmpq_clear(r1);
    fmpq_clear(r2);

    return 0;
}

int main(void)
{
    fmpz_poly_t p;
    fmpq * exact_roots;
    fmpz * c_array;
    slong * k_array;
    slong n_exact, n_interval;
    fmpq_t a;

    fmpq_init(a);
    fmpz_poly_init(p);
    exact_roots = _fmpq_vec_init(5);
    c_array = _fmpz_vec_init(5);
    k_array = (slong *) flint_malloc(5 * sizeof(slong));

    /* -1705*x^2 - 7650*x - 3297 */
    /* roots: -4.0038 and -0.4829 */
    fmpz_poly_set_coeff_si(p, 0, -3297);
    fmpz_poly_set_coeff_si(p, 1, -7650);
    fmpz_poly_set_coeff_si(p, 2, -1705);

    fmpz_poly_isolate_real_roots(exact_roots, &n_exact,
            c_array, k_array, &n_interval, p);

    if (n_exact != 0 || n_interval != 2)
    {
        fprintf(stderr, "wrong number of isolated roots\n");
        return 1;
    }

    fmpq_set_si(a, -2497397708202708803, 623751320421068880); /* approx of root 1 */
    if (check_isolation(c_array, k_array[0], a))
    {
        fprintf(stderr, "Failed root1 of poly1\n");
        return 1;
    }

    fmpq_set_si(a, -83382880964438876, 172646812034399449); /* approx of root 2 */
    if (check_isolation(c_array + 1, k_array[1], a))
    {
        fprintf(stderr, "Failed root2 of poly1\n");
        return 1;
    }

    /* x^2 - 7650*x - 13297 */
    /* roots: -1.7377 and 7651.7377 */
    fmpz_poly_set_coeff_si(p, 0, -13297);
    fmpz_poly_set_coeff_si(p, 1, -7650);
    fmpz_poly_set_coeff_si(p, 2, 1);

    n_exact = n_interval = 0;
    fmpz_poly_isolate_real_roots(exact_roots, &n_exact,
            c_array, k_array, &n_interval, p);

    if (n_exact != 0 || n_interval != 2)
    {
        fprintf(stderr, "wrong number of isolated roots\n");
        return 1;
    }

    fmpq_set_si(a, -1729553040654133611, 995268582037528766);
    if (check_isolation(c_array, k_array[0], a))
    {
        fprintf(stderr, "Failed root1 of poly2\n");
        return 1;
    }
    fmpq_set_si(a, 1148746836908268129, 150128881916767);
    if (check_isolation(c_array + 1, k_array[1], a))
    {
        fprintf(stderr, "Failed root2 of poly2\n");
        return 1;
    }

    /* x^2 - 1505*x + 566255 */
    /* roots: 751.381, 753.618 */
    fmpz_poly_set_coeff_si(p, 0, 566255);
    fmpz_poly_set_coeff_si(p, 1, -1505);
    fmpz_poly_set_coeff_si(p, 2, 1);

    n_exact = n_interval = 0;
    fmpz_poly_isolate_real_roots(exact_roots, &n_exact,
            c_array, k_array, &n_interval, p);

    if (n_exact != 0 || n_interval != 2)
    {
        fprintf(stderr, "wrong number of isolated roots\n");
        return 1;
    }

    fmpq_set_si(a, 606001226841264272, 806515533049393);
    if (check_isolation(c_array, k_array[0], a))
    {
        fprintf(stderr, "Failed root1 of poly2\n");
        return 1;
    }
    fmpq_set_si(a, 607804650398072193, 806515533049393);
    if (check_isolation(c_array + 1, k_array[1], a))
    {
        fprintf(stderr, "Failed root2 of poly2\n");
        return 1;
    }

    /* 146434129 * x^2 - 134751 * x + 31 */
    /* roots: 0.0004601002 and 0.0004601155 */
    fmpz_poly_set_coeff_si(p, 0, 31);
    fmpz_poly_set_coeff_si(p, 1, -134751);
    fmpz_poly_set_coeff_si(p, 2, 146434129);

    n_exact = n_interval = 0;
    fmpz_poly_isolate_real_roots(exact_roots, &n_exact,
            c_array, k_array, &n_interval, p);

    if (n_exact != 0 || n_interval != 2)
    {
        fprintf(stderr, "wrong number of isolated roots\n");
        return 1;
    }

    fmpq_set_si(a, 370174771560547, 804552393201742478);
    if (check_isolation(c_array, k_array[0], a))
    {
        fprintf(stderr, "Failed root1 of poly2\n");
        return 1;
    }
    fmpq_set_si(a, 442898231808688, 962580472320952871);
    if (check_isolation(c_array + 1, k_array[1], a))
    {
        fprintf(stderr, "Failed root2 of poly2\n");
        return 1;
    }

    _fmpq_vec_clear(exact_roots, 5);
    _fmpz_vec_clear(c_array, 5);
    flint_free(k_array);
    fmpq_clear(a);
    fmpz_poly_clear(p);

    return 0;
}
