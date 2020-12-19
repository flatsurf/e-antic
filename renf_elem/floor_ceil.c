/*
    Copyright (C) 2016-2019 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "../e-antic/renf_elem.h"


#define KIND floor
#define ARF_INT arf_floor
#define FMPZ_DIV fmpz_fdiv_q
#include "rounding_template.h"
#undef ARF_INT
#undef FMPZ_DIV
#undef KIND

#define KIND ceil
#define ARF_INT  arf_ceil
#define FMPZ_DIV fmpz_cdiv_q
#include "rounding_template.h"
#undef ARF_INT
#undef FMPZ_DIV
#undef KIND
