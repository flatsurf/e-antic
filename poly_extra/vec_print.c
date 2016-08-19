/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

/* NOTE: contrarily to fmpz_fprint, the function fmpq_fprint is declared as
 * void */
int _fmpq_vec_fprint(FILE * file, const fmpq * vec, slong len)
{
    int r;
    slong i;

    r = flint_fprintf(file, "%li", len);
    if ((len > 0) && (r > 0))
    {
        r = fputc(' ', file);
        for (i = 0; (i < len) && (r > 0); i++)
        {
            r = fputc(' ', file);
            if (r > 0)
                fmpq_fprint(file, vec + i);
        }
    }

    return r;
}

void _fmpq_vec_print(const fmpq * vec, slong len)
{
    _fmpq_vec_fprint(stdout, vec, len);
}


