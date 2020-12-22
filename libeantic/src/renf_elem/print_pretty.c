/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <string.h>

#include "../e-antic/renf_elem.h"

void renf_elem_print_pretty(renf_elem_t a, const char * var, renf_t nf, slong n, int flag)
{
    char * res = renf_elem_get_str_pretty(a, var, nf, n, flag);
    fputs(res, stdout);
    flint_free(res);
}
