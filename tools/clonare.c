/* clonare.c - bin/clonare: paria (fons, destinatio) per clonatio_facere
 * (clonefile, copy-on-write; pythonica S3). Processus unus pro omnibus
 * paribus - silva._clonare_ignorata photographiam ita clonat (XIII s per
 * 'cp -c -R' -> ~III s).
 *
 * Usus:   bin/clonare FONS DESTINATIO [FONS DESTINATIO ...]
 * Exitus: 0 omnia clonata; I par fractum (nominatur, cum errno - ceteri
 *         non tentantur); II usus.
 */

/* plagula provenientiae a tools/clonare_struere.sh generata: binarium
 * '-provenientia' respondet */
/* <aedilis obiectum="build/fabrica/provenientia/clonare.c"/> */

#include "postulata_posix.h"

#include "latina.h"
#include "clonatio.h"
#include "provenientia.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

externus constans ProvenientiaRelatio provenientia_clonare;

s32
principale (
      integer   argc,
    character** argv)
{
    integer i;

    si (provenientia_respondere(argc, argv, &provenientia_clonare))
    {
        redde ZEPHYRUM;
    }
    si (argc < III || (argc - I) % II != ZEPHYRUM)
    {
        fprintf(stderr, "usus: clonare FONS DESTINATIO "
            "[FONS DESTINATIO ...]\n");
        redde II;
    }
    per (i = I; i + I < argc; i += II)
    {
        si (!clonatio_facere(argv[i], argv[i + I]))
        {
            fprintf(stderr, "clonare: %s -> %s: %s\n", argv[i],
                argv[i + I], strerror(errno));
            redde I;
        }
    }
    redde ZEPHYRUM;
}
