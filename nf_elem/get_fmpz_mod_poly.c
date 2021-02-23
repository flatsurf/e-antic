/*=============================================================================

    This file is part of ANTIC.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2018 Tommy Hofmann

******************************************************************************/

#include "e-antic/nf_elem.h"

#if (__FLINT_RELEASE >= 20700)
void _nf_elem_get_fmpz_mod_poly(fmpz_mod_poly_t pol, const nf_elem_t a,
                                       const nf_t nf, const fmpz_mod_ctx_t ctx)
#else
void _nf_elem_get_fmpz_mod_poly(fmpz_mod_poly_t pol, const nf_elem_t a, const nf_t nf)
#endif
{
    if (nf_elem_is_zero(a, nf))
    {
#if (__FLINT_RELEASE >= 20700)
        fmpz_mod_poly_zero(pol, ctx);
        
#else
        fmpz_mod_poly_zero(pol);
#endif
        return;
    }
    if (nf->flag & NF_LINEAR)
    {
        {
#if (__FLINT_RELEASE >= 20700)
            fmpz_mod_poly_fit_length(pol, 1, ctx);
        
            fmpz_mod(pol->coeffs + 0, LNF_ELEM_NUMREF(a), ctx->n);
        
#else
            fmpz_mod_poly_fit_length(pol, 1);
            fmpz_mod(pol->coeffs + 0, LNF_ELEM_NUMREF(a), &(pol->p));
#endif
            _fmpz_mod_poly_set_length(pol, 1);
            _fmpz_mod_poly_normalise(pol);

        }
    } else if (nf->flag & NF_QUADRATIC)
    {
#if (__FLINT_RELEASE >= 20700)
        fmpz_mod_poly_fit_length(pol, 3, ctx);
        
        fmpz_mod(pol->coeffs + 0, QNF_ELEM_NUMREF(a), ctx->n);
        fmpz_mod(pol->coeffs + 1, QNF_ELEM_NUMREF(a) + 1, ctx->n);
        fmpz_mod(pol->coeffs + 2, QNF_ELEM_NUMREF(a) + 2, ctx->n);
        
#else
        fmpz_mod_poly_fit_length(pol, 3);
        fmpz_mod(pol->coeffs + 0, QNF_ELEM_NUMREF(a), &(pol->p));
        fmpz_mod(pol->coeffs + 1, QNF_ELEM_NUMREF(a) + 1, &(pol->p));
        fmpz_mod(pol->coeffs + 2, QNF_ELEM_NUMREF(a) + 2, &(pol->p));
#endif
        _fmpz_mod_poly_set_length(pol, 3);
        _fmpz_mod_poly_normalise(pol);
    } else
    {
        slong len = NF_ELEM(a)->length;
        slong i;
#if (__FLINT_RELEASE >= 20700)

        fmpz_mod_poly_fit_length(pol, len, ctx);
        
#else
        fmpz_mod_poly_fit_length(pol, len);
#endif
        for (i = 0; i < len; i++)
#if (__FLINT_RELEASE >= 20700)
            fmpz_mod(pol->coeffs + i, NF_ELEM_NUMREF(a) + i, ctx->n);
        
#else
            fmpz_mod(pol->coeffs + i, NF_ELEM_NUMREF(a) + i, &(pol->p));
#endif
        _fmpz_mod_poly_set_length(pol, len);
        _fmpz_mod_poly_normalise(pol);
    }
}

#if (__FLINT_RELEASE >= 20700)
void nf_elem_get_fmpz_mod_poly_den(fmpz_mod_poly_t pol, const nf_elem_t a,
                              const nf_t nf, int den, const fmpz_mod_ctx_t ctx)
#else
void nf_elem_get_fmpz_mod_poly_den(fmpz_mod_poly_t pol, const nf_elem_t a, const nf_t nf, int den)
#endif
{
#if (__FLINT_RELEASE >= 20700)
    _nf_elem_get_fmpz_mod_poly(pol, a, nf, ctx);
#else
    _nf_elem_get_fmpz_mod_poly(pol, a, nf);
#endif
    if (den)
    {
        if (nf->flag & NF_LINEAR)
#if (__FLINT_RELEASE >= 20700)
            fmpz_mod_poly_scalar_div_fmpz(pol, pol, LNF_ELEM_DENREF(a), ctx);
#else
            fmpz_mod_poly_scalar_div_fmpz(pol, pol, LNF_ELEM_DENREF(a));
#endif
        else if (nf->flag & NF_QUADRATIC)
#if (__FLINT_RELEASE >= 20700)
            fmpz_mod_poly_scalar_div_fmpz(pol, pol, QNF_ELEM_DENREF(a), ctx);
#else
            fmpz_mod_poly_scalar_div_fmpz(pol, pol, QNF_ELEM_DENREF(a));
#endif
        else
#if (__FLINT_RELEASE >= 20700)
            fmpz_mod_poly_scalar_div_fmpz(pol, pol, NF_ELEM_DENREF(a), ctx);
#else
            fmpz_mod_poly_scalar_div_fmpz(pol, pol, NF_ELEM_DENREF(a));
#endif
    }
}

#if (__FLINT_RELEASE >= 20700)
void nf_elem_get_fmpz_mod_poly(fmpz_mod_poly_t pol, const nf_elem_t a,
                                       const nf_t nf, const fmpz_mod_ctx_t ctx)
#else
void nf_elem_get_fmpz_mod_poly(fmpz_mod_poly_t pol, const nf_elem_t a, const nf_t nf)
#endif
{
#if (__FLINT_RELEASE >= 20700)
    nf_elem_get_fmpz_mod_poly_den(pol, a, nf, 1, ctx);
#else
    nf_elem_get_fmpz_mod_poly_den(pol, a, nf, 1);
#endif
}
