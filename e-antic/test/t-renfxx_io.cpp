/*
    Copyright (C) 2017 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <sstream>
#include <cstdlib>
#include "e-antic/renfxx.h"

using namespace std;

int main(void)
{
    {
        renf_elem_class f;

        // By default, read QQ encapsulated in renf_elem_class
        istringstream is("42");
        is >> f;
        if (f != 42) {
            cerr << "FAIL: C++ read rational" << endl;
            throw 10;
        }
    }

    {
        renf_class nf;

        /* former test */
        istringstream is("minpoly 3  -2 0 1 embedding [1.41 +/- 0.3]");
        is >> nf;
        cout << "field is: " << nf;

        renf_elem_class a(nf);
//        istringstream is2("32 5/7");
//        is2 >> a;
//        cout << "element is: " << a;
    }

    return 0;
}
