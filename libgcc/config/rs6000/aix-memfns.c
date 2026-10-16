/* Copyright (C) 2026 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

/* Private memcpy/memmove/bcopy for the AIX shared libgcc (shr.o).

   The shared libgcc links as "@shlib_objs@ -lc"; if nothing in
   @shlib_objs@ already defines these, the link pulls IBM libc's
   moveeq_64.o, whose memmove is a bare branch-absolute instruction
   with no function descriptor, causing a SIGILL through a bogus
   glink stub at run time.  Defining them here keeps moveeq_64.o out.
   They are not in libgcc's export map, so ordinary code still gets
   libc's optimized routines; only libgcc's own internal references
   are satisfied by these.  */

#include <stddef.h>

void *
memcpy (void *dest, const void *src, size_t n)
{
  unsigned char *d = (unsigned char *) dest;
  const unsigned char *s = (const unsigned char *) src;
  while (n--)
    *d++ = *s++;
  return dest;
}

void *
memmove (void *dest, const void *src, size_t n)
{
  unsigned char *d = (unsigned char *) dest;
  const unsigned char *s = (const unsigned char *) src;
  if (d < s)
    while (n--)
      *d++ = *s++;
  else
    {
      d += n;
      s += n;
      while (n--)
	*--d = *--s;
    }
  return dest;
}

void
bcopy (const void *src, void *dest, size_t n)
{
  memmove (dest, src, n);
}
