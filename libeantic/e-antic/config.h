/*
    Copyright (C) 2024 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

// e-antic/config.h is a redirect to e-antic/config.generated.h.
// This file exists because config.h is generated in the build path and does
// not exist in the source path so we cannot include e-antic/config.h with its
// relative path (at least not during VPATH builds.)

// Note that config.h (and config.generated.h) should only be used at build
// time. These files should not be installed (ideally) see
// https://www.sourceware.org/autobook/autobook/autobook_66.html

// All source files .c, .cxx should include config.h even if they do not use
// any of its macros.
// By including config.h, all files get rebuild when ./configure regenerates
// the config.generated.h, i.e., in particular when the compile flags changed.

#include "config.generated.h"
