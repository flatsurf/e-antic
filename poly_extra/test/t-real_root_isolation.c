/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"
#include "fmpq_vec.h"

void _slong_vec_print(const slong * vec, slong len)
{
    slong i;
    flint_printf("%wd", len);
    for (i = 0; i < len; ++i)
        flint_printf(" %wd", vec[i]);
}


/*#define DEBUG*/

void check_intervals(
      fmpq * vec, slong len,
      fmpq * exact, slong n_exact,
      fmpz * c_array, slong * k_array, slong n_interval)
{
    slong i,j,k;
    fmpq_t x,y;

    if (n_exact + n_interval != len)
    {
        flint_printf("ERROR:\n");
        flint_printf("found n_exact = %wd and n_interval = %wd but n = %wd\n", n_exact, n_interval, len);
        flint_printf("vec = "); _fmpq_vec_print(vec, len);
        printf("\n");
        abort();
    }

    fmpq_init(x);
    fmpq_init(y);
    i = j = k = 0;
    while ((j < n_exact) || (k < n_interval))
    {
#ifdef DEBUG
        flint_printf("[check_intervals] j = %wd  k = %wd\n", j, k);
#endif
        if (k < n_interval)
        {
            fmpz_set(fmpq_numref(x), c_array + k);
            fmpz_set(fmpq_numref(y), c_array + k);
            fmpz_one(fmpq_denref(x));
            fmpz_one(fmpq_denref(y));
            fmpz_add_ui(fmpq_numref(y), fmpq_numref(y), 1);
            if (k_array[k] > 0)
            {
                fmpq_mul_2exp(x, x, k_array[k]);
                fmpq_mul_2exp(y, y, k_array[k]);
            }
            else if (k_array[k] < 0)
            {
                fmpq_div_2exp(x, x, -k_array[k]);
                fmpq_div_2exp(y, y, -k_array[k]);
            }
        }

#ifdef DEBUG
        if (k < n_interval)
        {
            flint_printf("[check intervals] %wd-th interval is ", k);
            printf("("); fmpq_print(x); printf(","); fmpq_print(y); printf(")\n");
        }

        if (j < n_exact)
        {
            flint_printf("[check intervals] %wd-th exact root is ", j); fmpq_print(exact +j); printf("\n");
        }
#endif

        if ((j < n_exact) && (k < n_interval))
        {
            if ((fmpq_cmp(exact + j, x) == 1) && (fmpq_cmp(y, exact + j) == 1))
            {
                flint_printf("ERROR:\n");
                flint_printf("the %wd-th exact root ", j);
                fmpq_print(exact + j); printf("\n");
                flint_printf("belongs to the %wd-th interval ", k);
                printf("("); fmpq_print(x); printf(","); fmpq_print(y); printf(")");
                printf("\n");
                printf("vec   = "); _fmpq_vec_print(vec, len);
                printf("\nc     = "); _fmpz_vec_print(c_array, n_interval);
                printf("\nk     = "); _slong_vec_print(k_array, n_interval);
                printf("\nexact = "); _fmpq_vec_print(exact, n_exact);
                printf("\n");
                abort();
            }
        }

        if ((j == n_exact) || ((k < n_interval) && (fmpq_cmp(exact + j, y) >= 0)))
        {
            /* check interval */
            if ((fmpq_cmp(x, vec + i) > 0) ||
                 fmpq_cmp(vec + i, y) > 0)
            {
                flint_printf("ERROR:\n");
                flint_printf(" the %wd-th root is vec[%wd] = ", i, i);
                fmpq_print(vec + i);
                flint_printf(" but got %wd-th interval ", k);
                printf("("); fmpq_print(x); printf(","); fmpq_print(y); printf(")");
                printf("\n");
                printf("vec   = "); _fmpq_vec_print(vec, len);
                printf("\nc     = "); _fmpz_vec_print(c_array, n_interval);
                printf("\nk     = "); _slong_vec_print(k_array, n_interval);
                printf("\nexact = "); _fmpq_vec_print(exact, n_exact);
                printf("\n");
                abort();
            }
            k += 1;
            i += 1;
        }
        else
        {
            /* check root */
            if (!fmpq_equal(vec + i, exact + j))
            {
                flint_printf("ERROR:\n");
                flint_printf("the %wd-th root is vec[%wd] = ", i, i);
                fmpq_print(vec + i);
                flint_printf(" but got %wd-th exact root ", j);
                fmpq_print(exact + j);
                printf("\n");
                printf("vec   = "); _fmpq_vec_print(vec, len);
                printf("\nc     = "); _fmpz_vec_print(c_array, n_interval);
                printf("\nk     = "); _slong_vec_print(k_array, n_interval);
                printf("\nexact = "); _fmpq_vec_print(exact, n_exact);
                printf("\n");
                abort();
            }
            j += 1;
            i += 1;
        }
    }

    fmpq_clear(x);
    fmpq_clear(y);
}

void fmpz_poly_from_fmpq_roots(fmpz_poly_t p, const fmpq * vec, slong n)
{
    fmpz_poly_t q;
    slong i;

    fmpz_poly_init(q);
    fmpz_poly_one(p);
    for (i = 0; i < n; i++)
    {
        if (fmpq_is_zero(vec + i))
        {
            fmpz_poly_set_coeff_si(q, 0, 0);
            fmpz_poly_set_coeff_si(q, 1, 1);
        }
        else
        {
            fmpz_poly_set_coeff_fmpz(q, 0, fmpq_numref(vec + i));
            fmpz_neg(fmpq_poly_numref(q), fmpq_poly_numref(q));
            fmpz_poly_set_coeff_fmpz(q, 1, fmpq_denref(vec + i));
        }
        fmpz_poly_mul(p, p, q);
    }
    fmpz_poly_clear(q);
}


int main(void)
{
    int iter;

    FLINT_TEST_INIT(state);

    printf("t-isolate_real_roots....");
    fflush(stdout);

    for (iter = 0; iter < 200; iter++)
    {
        fmpq vec[30];
        fmpz c_array[30];
        slong k_array[30];
        fmpq exact_array[30];
        fmpz_poly_t p,q;
        slong n = n_randint(state, 30);  /* real roots            */
        slong nc = n_randint(state, 30); /* pair of complex roots */
        slong i;
        slong n_exact, n_interval;

        if (n + nc == 0) continue;

        for(i = 0; i < 30; ++i)
        {
            fmpq_init(vec + i);
            fmpz_init(c_array + i);
            fmpq_init(exact_array + i);
        }

        _fmpq_vec_randtest_uniq_sorted(vec, state, n, 100);

#ifdef DEBUG
        printf("\n=============== NEW POLY =====================\n");
        flint_printf("n = %wd\n", n);
        printf("vec = ");
        _fmpq_vec_print(vec, n);
        printf("\n");
        fflush(stdout);
#endif

        fmpz_poly_init(p);
        fmpz_poly_from_fmpq_roots(p, vec, n);

        fmpz_poly_init(q);
        fmpz_poly_randtest_no_real_root(q, state, nc, 100);
        fmpz_poly_mul(p, p, q);

#ifdef DEBUG
        printf("p = "); fmpz_poly_print_pretty(p, "x"); printf("\n");
        fflush(stdout);
#endif
        fmpz_poly_isolate_real_roots(exact_array, &n_exact, c_array, k_array, &n_interval, p);

        check_intervals(vec, n, exact_array, n_exact, c_array, k_array, n_interval);

        fmpz_poly_clear(p);
        for(i = 0; i < 30; ++i)
        {
            fmpq_clear(vec + i);
            fmpz_clear(c_array + i);
            fmpq_clear(exact_array + i);
        }
    }

    FLINT_TEST_CLEANUP(state);

    printf("PASS\n");
    return 0;
}
