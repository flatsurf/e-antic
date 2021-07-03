/*
    Copyright (C) 2020 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "../e-antic/renf.h"

int renf_set_immutable(renf_t nf, int immutable)
{
    int old = nf->immutable;
    nf->immutable = immutable;
    return old;
}

