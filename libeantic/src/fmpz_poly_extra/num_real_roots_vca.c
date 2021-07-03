/*
   Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/fmpz_poly_extra.h"

slong fmpz_poly_num_real_roots_vca(fmpz_poly_t pol)
{
    slong n_exact=0, n_interval=0;
    fmpz_poly_isolate_real_roots(NULL, &n_exact, NULL, NULL, &n_interval, pol);
    return n_exact + n_interval;
}

