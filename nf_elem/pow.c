/*=============================================================================

    This file is part of FLINT.

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

    Copyright (C) 2010 Sebastian Pancratz
    Copyright (C) 2014 William Hart

******************************************************************************/

#include "e-antic/nf_elem.h"

#include <stdlib.h>
#include <gmp.h>
#include "flint/flint.h"
#include "flint/fmpz.h"
#include "flint/fmpz_vec.h"
#include "flint/fmpz_poly.h"

void
_nf_elem_pow(nf_elem_t res, const nf_elem_t a, ulong e, const nf_t nf)
{
    ulong bit = ~((~UWORD(0)) >> 1);
    nf_elem_t v;
    nf_elem_struct * R, * S, * T;

    nf_elem_init(v, nf);

    /*
       Set bits to the bitmask with a 1 one place lower than the msb of e
     */

    while ((bit & e) == UWORD(0))
        bit >>= 1;

    bit >>= 1;

    /*
       Trial run without any polynomial arithmetic to determine the parity
       of the number of swaps;  then set R and S accordingly
     */

    {
        unsigned int swaps = 0U;
        ulong bit2 = bit;
        if ((bit2 & e))
            swaps = ~swaps;
        while (bit2 >>= 1)
            if ((bit2 & e) == UWORD(0))
                swaps = ~swaps;

        if (swaps == 0U)
        {
            R = res;
            S = v;
        }
        else
        {
            R = v;
            S = res;
        }
    }

    /*
       We unroll the first step of the loop, referring to {poly, len}
     */

    nf_elem_mul(R, a, a, nf);
    if ((bit & e))
    {
        nf_elem_mul(S, R, a, nf);
        T = R;
        R = S;
        S = T;
    }

    while ((bit >>= 1))
    {
        if ((bit & e))
        {
            nf_elem_mul(S, R, R, nf);
            nf_elem_mul(R, S, a, nf);
        }
        else
        {
            nf_elem_mul(S, R, R, nf);
            T = R;
            R = S;
            S = T;
        }
    }

    nf_elem_clear(v, nf);
}

void
nf_elem_pow_si(nf_elem_t res, const nf_elem_t a, slong e, const nf_t nf)
{
   nf_elem_t t;

   if (e == WORD(0))
   {
      nf_elem_one(res, nf);
      return;
   }

   if (nf_elem_is_zero(a, nf))
   {
      nf_elem_zero(res, nf);
      return;
   }

   if (nf->flag & NF_LINEAR)
      _fmpq_pow_si(LNF_ELEM_NUMREF(res), LNF_ELEM_DENREF(res),
                   LNF_ELEM_NUMREF(a), LNF_ELEM_DENREF(a), e);
   else
   {
        if (e == WORD(1))
            nf_elem_set(res, a, nf);
        else if (e == WORD(-1))
            nf_elem_inv(res, a, nf);
        else if (e == WORD(2))
            nf_elem_mul(res, a, a, nf);
        else if (e < WORD(0))
        {
            nf_elem_init(t, nf);
            nf_elem_inv(t, a, nf);
            _nf_elem_pow(res, t, (ulong) (-e), nf);
            nf_elem_clear(t, nf);
        }
        else if (res == a)
        {
            nf_elem_init(t, nf);
            _nf_elem_pow(t, a, (ulong) e, nf);
            nf_elem_swap(t, res, nf);
            nf_elem_clear(t, nf);
        }
        else
            _nf_elem_pow(res, a, (ulong) e, nf);
   }
}
