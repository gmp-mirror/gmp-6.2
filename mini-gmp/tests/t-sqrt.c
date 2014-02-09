/*

Copyright 2012, 2014, Free Software Foundation, Inc.

This file is part of the GNU MP Library test suite.

The GNU MP Library test suite is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

The GNU MP Library test suite is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
the GNU MP Library test suite.  If not, see https://www.gnu.org/licenses/.  */

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "testutils.h"

#define MAXBITS 400
#define COUNT 10000

static void
dump (const char *label, const mpz_t x)
{
  char *buf = mpz_get_str (NULL, 16, x);
  fprintf (stderr, "%s: %s\n", label, buf);
  testfree (buf);
}

/* Called when s is supposed to be floor(sqrt(u)), and r = u - s^2 */
static int
sqrtrem_valid_p (const mpz_t u, const mpz_t s, const mpz_t r)
{
  mpz_t t;

  mpz_init (t);
  mpz_mul (t, s, s);
  mpz_sub (t, u, t);
  if (mpz_sgn (t) < 0 || mpz_cmp (t, r) != 0)
    {
      mpz_clear (t);
      return 0;
    }
  mpz_add_ui (t, s, 1);
  mpz_mul (t, t, t);
  if (mpz_cmp (t, u) <= 0)
    {
      mpz_clear (t);
      return 0;
    }

  mpz_clear (t);
  return 1;
}

void
testmain (int argc, char **argv)
{
  unsigned i;
  mpz_t u, s, r;

  mpz_init (u);
  mpz_init (s);
  mpz_init (r);

  mpz_set_si (u, -1);
  if (mpz_perfect_square_p (u))
    {
      fprintf (stderr, "mpz_perfect_square_p failed on -1.\n");
      abort ();
    }

  mpz_set_ui (u, 0);
  if (!mpz_perfect_square_p (u))
    {
      fprintf (stderr, "mpz_perfect_square_p failed on 0.\n");
      abort ();
    }

  for (i = 0; i < COUNT; i++)
    {
      mini_rrandomb (u, MAXBITS);
      mpz_sqrtrem (s, r, u);

      if (!sqrtrem_valid_p (u, s, r))
	{
	  fprintf (stderr, "mpz_sqrtrem failed:\n");
	  dump ("u", u);
	  dump ("sqrt", s);
	  dump ("rem", r);
	  abort ();
	}

      if (mpz_sgn (r) == 0) {
	mpz_neg (u, u);
	mpz_sub_ui (u, u, 1);
      }

      if ((mpz_sgn (u) <= 0 || (i & 1)) ?
	  mpz_perfect_square_p (u) :
	  mpn_perfect_square_p (mpz_limbs_read (u), mpz_size (u)))
	{
	  fprintf (stderr, "mp%s_perfect_square_p failed on non square:\n",
		   (mpz_sgn (u) <= 0 || (i & 1)) ? "z" : "n");
	  dump ("u", u);
	  abort ();
	}

      mpz_mul (u, s, s);
      if (!((mpz_sgn (u) <= 0 || (i & 1)) ?
	    mpz_perfect_square_p (u) :
	    mpn_perfect_square_p (mpz_limbs_read (u), mpz_size (u))))
	{
	  fprintf (stderr, "mp%s_perfect_square_p failed on square:\n",
		   (mpz_sgn (u) <= 0 || (i & 1)) ? "z" : "n");
	  dump ("u", u);
	  abort ();
	}

    }
  mpz_clear (u);
  mpz_clear (s);
  mpz_clear (r);
}
