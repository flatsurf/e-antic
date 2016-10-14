/*
    Copyright (C) 2016 Elias Tsigaridas

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

slong _fmpz_poly_remove_content_2exp(fmpz * f, slong len)
{
    slong cont, i, z;


    if (len == 0)
        return 0;

    i = 0;
    while (fmpz_is_zero(f + i))
        i++;

    cont = fmpz_val2(f + i);

    for ( ; (i < len) && cont; i++)
    {
        if (!fmpz_is_zero(f + i))
        {
            z = fmpz_val2(f + i);
            if (z < cont)
                cont = z;
        }
    }

    if (cont == 0)
        return 0;

    for (i = 0; i < len; i++)
        fmpz_fdiv_q_2exp(f + i, f + i, cont);
    
    return cont;
}
