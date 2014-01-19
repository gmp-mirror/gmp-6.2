/* gmp_printf -- formatted output.

Copyright 2001 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see https://www.gnu.org/licenses/.  */

#include <stdarg.h>
#include <stdio.h>

#include "gmp.h"
#include "gmp-impl.h"


int
gmp_printf (const char *fmt, ...)
{
  va_list  ap;
  int      ret;

  va_start (ap, fmt);

  ret = __gmp_doprnt (&__gmp_fprintf_funs, stdout, fmt, ap);
  va_end (ap);
  return ret;
}
