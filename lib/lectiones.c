/* lectiones.c - liber lectionum (fabrica plan 2 T1). Vide lectiones.h.
 *
 * Variabilis ambitus omni vocatione legitur (vilis): probationes eam
 * ponunt et tollunt, et descriptor apertus viae NOVAE non servit.
 * Descriptor per processum servatur dum via eadem manet; filius post
 * fork descriptorem patris heredat - O_APPEND lineas integras servat.
 */
#include "postulata_posix.h"
#include "lectiones.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


hic_manens  integer  _descriptor = -I;
hic_manens character _via_aperta[IV * MXXIV];

interior character
_littera (
    LectioGenus genus)
{
    commutatio (genus)
    {
        casus LECTIO_LEGIT:       redde 'L';
        casus LECTIO_ABSENS:      redde 'A';
        casus LECTIO_EXSTAT:      redde 'X';
        casus LECTIO_ENUMERAVIT:  redde 'D';
        casus LECTIO_SCRIPSIT:    redde 'S';
        casus LECTIO_AMBITUS:     redde 'E';
    }
    redde '?';
}

vacuum
lectiones_notare (
                 LectioGenus  genus,
          constans character* via)
{
    constans character* liber;
             character  linea[IV * MXXIV + IV];
        memoriae_index  mensura_viae;

    liber = getenv("FABRICA_LECTIONES");
    si (liber == NIHIL || liber[0] == '\0')
    {
        /* absens = CLAUSUM: descriptor servatus plagulam interea
         * deletam (eadem via, inodus alius) scriberet - probatio III
         * lineas omnes ita amisit */
        si (_descriptor >= 0)
        {
            (vacuum)close(_descriptor);
            _descriptor = -I;
        }
        redde;
    }
    si (via == NIHIL)
    {
        redde;
    }
    si (_descriptor < 0 || strcmp(liber, _via_aperta) != 0)
    {
        si (_descriptor >= 0)
        {
            (vacuum)close(_descriptor);
            _descriptor = -I;
        }
        si (strlen(liber) >= magnitudo(_via_aperta))
        {
            redde;
        }
        _descriptor = open(liber, O_WRONLY | O_APPEND | O_CREAT, 0644);
        si (_descriptor < 0)
        {
            redde;
        }
        strcpy(_via_aperta, liber);
    }
    mensura_viae = strlen(via);
    si (mensura_viae + IV > magnitudo(linea))
    {
        redde;   /* linea numquam scinditur */
    }
    linea[0] = _littera(genus);
    linea[I] = '\t';
    memcpy(linea + II, via, mensura_viae);
    linea[II + mensura_viae] = '\n';
    (vacuum)write(_descriptor, linea, mensura_viae + III);
}

constans character*
lectiones_ambitus (
         constans character* titulus)
{
    constans character* valor;
             character  linea[IV * MXXIV];

    si (titulus == NIHIL)
    {
        redde NIHIL;
    }
    valor = getenv(titulus);
    si (strlen(titulus) + (valor != NIHIL ? strlen(valor) : 0) + II
            >= magnitudo(linea))
    {
        redde valor;   /* nimis longum: non notatur, ut via longa */
    }
    strcpy(linea, titulus);
    si (valor != NIHIL)
    {
        strcat(linea, "\t");
        strcat(linea, valor);
    }
    lectiones_notare(LECTIO_AMBITUS, linea);
    redde valor;
}
