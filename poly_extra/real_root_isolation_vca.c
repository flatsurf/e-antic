/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

/*#define DEBUG*/


/* isolate the real roots of pol contained in [0,1] */
/* using VCA (Vincent-Collins-Akritas) method       */
/* the output are arrays of fmpz c and integers k so that the roots belong to */
/* [c/2^k, (c+1)/2^k[ */
/* if exact_roots is NULL only n_exact is updated */
/* similarly if c_array/k_array is NULL only n_intervals is updated (useful to */
/* count the roots) */
void _fmpz_poly_isolate_real_roots_0_1_vca(fmpq * exact_roots, slong * n_exact,
        fmpz * c_array, slong * k_array, slong * n_intervals,
        fmpz * pol, slong len)
{
    fmpz_t c;
    ulong k;
    fmpz * p;
    fmpz * p0;
    ulong i;
    fmpz_t one;
    slong bound;
    slong len0 = len;

    fmpz_init(one);
    fmpz_one(one);
    p0 = p = _fmpz_vec_init(len);
    _fmpz_vec_set(p, pol, len);

    fmpz_init(c);
    fmpz_zero(c);
    k = 0;

    while (1)
    {
#ifdef DEBUG
        if (k > 100)
        {
            printf("[_fmpz_poly_isolate_real_roots_0_1_vca]: get above depth 50!");
            abort();
        }
        flint_printf("[_fmpz_poly_isolate_real_roots_0_1_vca] k = %wu  c = ", k); fmpz_print(c);
        printf("\n[_fmpz_poly_isolate_real_roots_0_1_vca] p = ");
        _fmpz_vec_fprint(stdout, p, len);
        printf("\n");
#endif

        /* check for exact zero */
        while (fmpz_is_zero(p) && len)
        {
#ifdef DEBUG
            printf("[_fmpz_poly_isolate_real_roots_0_1_vca] found exact zero: c = ");
            fmpz_print(c);
            flint_printf(" k = %wd\n", k);
            fflush(stdout);
#endif

            if (exact_roots != NULL)
            {
                fmpz_set(fmpq_numref(exact_roots + *n_exact), c);
                fmpz_one(fmpq_denref(exact_roots + *n_exact));
                fmpq_div_2exp(exact_roots + *n_exact, exact_roots + *n_exact, k);
            }
            (*n_exact)++;
            p++;
            len--;
        }

        /* use Descartes bound */
        bound = _fmpz_poly_descartes_bound_0_1(p, len, 2);
        switch(bound)
        {
            case 2:
#ifdef DEBUG
                printf("[_fmpz_poly_isolate_real_roots_0_1_vca] case 2\n");
#endif
            case WORD_MAX:
#ifdef DEBUG
                printf("[_fmpz_poly_isolate_real_roots_0_1_vca] case WORD_MAX\n");
#endif
                /* unknown: go down */
                k += 1;
                fmpz_mul_2exp(c, c, 1);
                _fmpz_poly_scale_2exp(p, len, -1);
                break;
            case 1:
#ifdef DEBUG
                printf("[_fmpz_poly_isolate_real_roots_0_1_vca] case 1\n");
#endif
                /* got a root! */
                if ((c_array != NULL) && (k_array != NULL))
                {
                    fmpz_set(c_array + *n_intervals, c);
                    k_array[*n_intervals] = -k;
                }
                (*n_intervals)++;
            case 0:
#ifdef DEBUG
                printf("[_fmpz_poly_isolate_real_roots_0_1_vca] case 0\n");
#endif
                /* no root: go up */
                fmpz_add_ui(c, c, 1);
                i = fmpz_val2(c);
#ifdef DEBUG
                flint_printf("[_fmpz_poly_isolate_real_roots_0_1_vca] i = %wd\n", i);
                fflush(stdout);
#endif
                if (k == i)
                {
                    fmpz_clear(c);
                    _fmpz_vec_clear(p0, len0);
                    return;
                }

                /* go to the next node */
                _fmpz_poly_taylor_shift(p, one, len);
                if (i)
                {
                    _fmpz_poly_scale_2exp(p, len, i);
                    fmpz_fdiv_q_2exp(c, c, i);
                    k -= i;
                }
                break;
            default:
                flint_fprintf(stderr, "ERROR: expected 0,1,WORD_MAX as output from descartes_bound but got %wd\n", bound);
                abort();
        }
    }
}

void fmpz_poly_isolate_real_roots(fmpq * exact_roots, slong * n_exact, fmpz * c_array, slong * k_array, slong * n_interval, fmpz_poly_t pol)
{
    slong i, k, n_neg, tmp, len, n_zeros, n_neg_exact;
    fmpz * p;

    n_neg = n_zeros = n_neg_exact = *n_exact = *n_interval = 0;
    len = pol->length;

#ifdef DEBUG
    printf("[fmpz_poly_isolate_real_roots] ************************\n");
    printf("[fmpz_poly_isolate_real_roots] pol = ");
    fmpz_poly_print_pretty(pol, "x");
    printf("\n");
#endif

    if (fmpz_poly_is_zero(pol))
    {
        fprintf(stderr, "ERROR (fmpz_poly_isolate_real_roots): zero polynomial\n");
        abort();
    }

    /* compute the number zero roots */
    /* (they will be inserted after the negative ones) */
    for (n_zeros = 0; (n_zeros < len) && fmpz_is_zero(pol->coeffs + n_zeros); n_zeros++);
    len -= n_zeros;
    p = _fmpz_vec_init(len);
    _fmpz_vec_set(p, pol->coeffs + n_zeros, len);

#ifdef DEBUG
    flint_printf("[fmpz_poly_isolate_real_roots] done treating zeros\n");
    flint_printf("[fmpz_poly_isolate_real_roots] n_zeros = %wd\n", n_zeros);
    flint_printf("[fmpz_poly_isolate_real_roots] n_neg = %wd\n", n_neg);
    flint_printf("[fmpz_poly_isolate_real_roots] n_neg_exact = %wd\n", n_neg_exact);
    flint_printf("[fmpz_poly_isolate_real_roots] n_exact = %wd\n", *n_exact);
    flint_printf("[fmpz_poly_isolate_real_roots] n_interval = %wd\n", *n_interval);
    flint_printf("[fmpz_poly_isolate_real_roots] len = %wd\n", len);
    flint_printf("[fmpz_poly_isolate_real_roots] p = "); _fmpz_vec_print(p, len); printf("\n");
    fflush(stdout);
#endif

    /* negative roots (use P(-x)) */
    for (i = 1; i < len; i += 2) fmpz_neg(p + i, p + i);
#ifdef DEBUG
    printf("[fmpz_poly_isolate_real_roots] P(-x) = "); _fmpz_vec_fprint(stdout, p, len); printf("\n");
    fflush(stdout);
#endif
    k = _fmpz_poly_positive_root_upper_bound_2exp(p, len);
    if (k != WORD_MIN)
    {
#ifdef DEBUG
        flint_printf("[fmpz_poly_isolate_real_roots] k = %wd\n", k);
        fflush(stdout);
#endif
        _fmpz_poly_scale_2exp(p, len, k);
#ifdef DEBUG
        printf("[fmpz_poly_isolate_real_roots] rescaled: "); _fmpz_vec_fprint(stdout, p, len); printf("\n");
        fflush(stdout);
#endif
        _fmpz_poly_isolate_real_roots_0_1_vca(exact_roots, n_exact, c_array, k_array, n_interval, p, len);
        n_neg = *n_interval;
        n_neg_exact = *n_exact;
#ifdef DEBUG
        flint_printf("[fmpz_poly_isolate_real_roots] got %wd interval and %wd exact roots\n", n_neg, n_neg_exact);
#endif
        if ((c_array != NULL) && (k_array != NULL))
        {
            for (i = 0; i < *n_interval; i++)
            {
                fmpz_add_ui(c_array + i, c_array + i, 1);
                fmpz_neg(c_array + i, c_array + i);
                k_array[i] += k;
            }
            for (i = 0; i < *n_interval / 2; i++)
            {
                fmpz_swap(c_array + i, c_array + *n_interval - i - 1);
                tmp = k_array[i];
                k_array[i] = k_array[*n_interval - i - 1];
                k_array[*n_interval - i - 1] = tmp;
            }
        }

        if (exact_roots != NULL)
        {
            for (i = 0; i < n_neg_exact; i++)
            {
#ifdef DEBUG
                flint_printf("rescaling %wd-th exact root\n ", i);
                fmpq_print(exact_roots + i);
                printf(" --> ");
#endif
                fmpq_neg(exact_roots + i, exact_roots + i);
                if (k > 0)
                    fmpq_mul_2exp(exact_roots + i, exact_roots + i, k);
                else if (k < 0)
                    fmpq_div_2exp(exact_roots + i, exact_roots + i, -k);
#ifdef DEBUG
                fmpq_print(exact_roots + i);
                printf("\n");
#endif
            }
            for (i = 0; i < n_neg_exact/2; i++)
            {
                fmpq_swap(exact_roots + i, exact_roots + *n_exact - i - 1);
            }
        }
    }
    else
        n_neg = 0;

    /* insert zero roots */
    if (exact_roots != NULL)
    {
        for (i = *n_exact; i < *n_exact+n_zeros; i++) fmpq_zero(exact_roots + i);
    }
    *n_exact += n_zeros;

#ifdef DEBUG
    flint_printf("[fmpz_poly_isolate_real_roots] done treating negative and zeros\n");
    flint_printf("[fmpz_poly_isolate_real_roots] n_zeros = %wd\n", n_zeros);
    flint_printf("[fmpz_poly_isolate_real_roots] n_neg = %wd\n", n_neg);
    flint_printf("[fmpz_poly_isolate_real_roots] n_neg_exact = %wd\n", n_neg_exact);
    flint_printf("[fmpz_poly_isolate_real_roots] n_exact = %wd\n", *n_exact);
    flint_printf("[fmpz_poly_isolate_real_roots] n_interval = %wd\n", *n_interval);
    fflush(stdout);
#endif

    /* positive roots */
    _fmpz_vec_set(p, pol->coeffs + n_zeros, len);
#ifdef DEBUG
    printf("[fmpz_poly_isolate_real_roots] P(x) = "); _fmpz_vec_fprint(stdout, p, len); printf("\n");
#endif
    k = _fmpz_poly_positive_root_upper_bound_2exp(p, len);
    if (k != WORD_MIN)
    {
#ifdef DEBUG
        flint_printf("[fmpz_poly_isolate_real_roots] k = %wd\n", k);
#endif
        _fmpz_poly_scale_2exp(p, len, k);
#ifdef DEBUG
        flint_printf("[fmpz_poly_isolate_real_roots] after scaling p = ");
        _fmpz_vec_print(p, len);
        printf("\n");
        fflush(stdout);
#endif
        _fmpz_poly_isolate_real_roots_0_1_vca(exact_roots, n_exact, c_array, k_array, n_interval, p, len);

        if ((c_array != NULL) && (k_array != NULL))
        {
            for (i = n_neg; i < *n_interval; i++)
                k_array[i] += k;
        }

        if (exact_roots != NULL)
        {
            for (i = n_neg_exact + n_zeros; i < *n_exact; i++)
            {
                if (k > 0)
                    fmpq_mul_2exp(exact_roots + i, exact_roots + i, k);
                else if (k < 0)
                    fmpq_div_2exp(exact_roots + i, exact_roots + i, -k);
            }
        }
    }

#ifdef DEBUG
    flint_printf("[fmpz_poly_isolate_real_roots] done treating positive\n");
    flint_printf("[fmpz_poly_isolate_real_roots] n_zeros = %wd\n", n_zeros);
    flint_printf("[fmpz_poly_isolate_real_roots] n_neg = %wd\n", n_neg);
    flint_printf("[fmpz_poly_isolate_real_roots] n_neg_exact = %wd\n", n_neg_exact);
    flint_printf("[fmpz_poly_isolate_real_roots] n_exact = %wd\n", *n_exact);
    flint_printf("[fmpz_poly_isolate_real_roots] n_interval = %wd\n", *n_interval);
    fflush(stdout);
#endif
}

slong fmpz_poly_num_real_roots_vca(fmpz_poly_t pol)
{
    slong n_exact, n_interval;
    n_exact = n_interval = 0;
    fmpz_poly_isolate_real_roots(NULL, &n_exact, NULL, NULL, &n_interval, pol);
    return n_exact + n_interval;
}
