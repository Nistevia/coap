/* $Id: fprintbuf.c,v 2.0 2005-10-04 14:54:49 luis Exp $
 * AUTHOR: Luis Colorado <licolorado@indra.es>
 * DATE: 7.10.92.
 * DESC: muestra un buffer de datos en hexadecimal y ASCII.
 */

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include "fprintbuf.h"

static char *id = "$Id: fprintbuf.c,v 2.0 2005-10-04 14:54:49 luis Exp $\n";

#define 	TAM_REG		16

size_t
fprintbuf(
        FILE           *f,      /* fichero de salida */
        off_t           off,    /* initial offset */
	    size_t          t,      /* tamano del buffer */
	    const uint8_t  *b,      /* puntero al buffer */
	    const char     *fmt,    /* rotulo de cabecera */
                        ...)
{
	size_t i;
	uint8_t c;
	va_list lista;
	size_t escritos = 0;

	if (fmt)
		escritos += fprintf (f, "<OFFSET> : ");
	va_start (lista, fmt);
	escritos += vfprintf (f, fmt, lista);
	va_end (lista);
	escritos += fprintf (f, "\n");
	while (t > 0) {
		escritos += fprintf (f, "%08lx : ", off);
		for (i = 0; i < TAM_REG; i++) {
			if (t > 0)
				escritos += fprintf (f, "%02x ", *b);
			else escritos += fprintf (f, "   ");
			off++;
			t--;
			b++;
		}
		escritos += fprintf (f, ": ");
		t += TAM_REG;
		b -= TAM_REG;
		off -= TAM_REG;
		for (i = 0; i < TAM_REG; i++) {
			c = *b++;
			if (t > 0)
				if (isprint (c))
					escritos += fprintf (f, "%c", c);
				else	escritos += fprintf (f, ".");
			else break;
			off++;
			t--;
		}
		escritos += fprintf (f, "\n");
	}
	escritos += fprintf (f, "%08lx\n", off);

	return escritos;
} /* fprintbuf */

/* $Id: fprintbuf.c,v 2.0 2005-10-04 14:54:49 luis Exp $ */
