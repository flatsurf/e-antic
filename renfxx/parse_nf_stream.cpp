/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "e-antic/poly_extra.h"
#include "e-antic/renfxx.h"

/* read a number field from a string */
/* formatting "minpoly LEN coeff0 coeff1 ... coeffl embedding [mid +/- rad]" */
std::istream& parse_nf_stream(fmpq_poly_t minpoly, arb_t emb, std::istream& in)
{
    char c;
    int err;
    std::string s;
    std::string t;

    in >> s;
    if (s != "minpoly")
        throw std::ios_base::failure("Error in reading number field: expected keyword minpoly");

    /* go to the next non-space character */
    c = in.peek();
    while (c == ' ')
    {
        in.get(c);
        c = in.peek();
    }

    /* read the polynomial */
    while(true)
    {
        c = in.peek();
        if (c == 'e')
            break;
        in.get(c);
        t += c;
    }
    while (t.back() == ' ') t.pop_back();
    err = fmpq_poly_set_str_magic(minpoly, t.c_str());
    if (err)
        throw std::ios_base::failure("Error in reading minimal polynomial");

    in >> s;
    if (s != "embedding")
        throw std::ios_base::failure("Error in reading number field: expected keyword embedding");

    /* ignore spaces */
    c = in.peek();
    while (c == ' ')
    {
        in.get(c);
        c = in.peek();
    }

    /* read embedding */
    t.clear();
    c = in.peek();
    if (c == '[')
    {
        while(c != ']' and c != '\0')
        {
            in.get(c);
            t += c;
            if (c == ']')
                break;
        }
    }
    else
    {
        in >> t;
    }

    int error = arb_set_str(emb, t.c_str(), 10);
    if (error)
        throw std::ios_base::failure("Error in reading number field: bad formatting of embedding " + t);

    return in;
}


