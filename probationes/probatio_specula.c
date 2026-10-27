/* probatio_specula.c - specula plagulae (vicus quies T6c): fossa
 * legenda cum plagula mutatur, quieta post exhaustionem; deletio,
 * recreatio, creatio sub directorio speculato */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "filum.h"
#include "specula.h"
#include "credo.h"
#include <poll.h>
#include <stdio.h>
#include <string.h>

/* fossa legenda (mora: ms exspectandi; 0 = statim) */
interior b32
legenda (
    constans Specula* s,
                 s32  mora)
{
    structura pollfd p;

    p.fd       = (integer)specula_fossa(s);
    p.events   = POLLIN;
    p.revents  = ZEPHYRUM;
    redde poll(&p, I, (integer)mora) == I
        && (p.revents & POLLIN) != ZEPHYRUM;
}

interior vacuum
_sectio_plagula (
    CredoContextus* c)
{
      Specula* s;
    character  via[CCLVI];

    sprintf(via, "%s/liber.jsonl", credo_area(c));
    CREDO_NECESSE(filum_scribere_literis(via, ""));
    s = specula_creare(c->piscina, via);
    CREDO_NECESSE(s);
    CREDO_VERUM(specula_fossa(s) >= ZEPHYRUM);

    CREDO_GRADUS("nihil mutatum: quieta");
    CREDO_FALSUM(legenda(s, ZEPHYRUM));
    CREDO_FALSUM(specula_exhaurire(s));

    CREDO_GRADUS("appendere: legenda; exhausta: quieta");
    CREDO_NECESSE(filum_appendere_literis(via, "a\n"));
    CREDO_VERUM(legenda(s, CC));
    CREDO_VERUM(specula_exhaurire(s));
    CREDO_FALSUM(legenda(s, ZEPHYRUM));
    CREDO_FALSUM(specula_exhaurire(s));

    CREDO_GRADUS("deleta, recreata: utrumque videtur");
    CREDO_NECESSE(filum_delere(via));
    CREDO_VERUM(legenda(s, CC));
    CREDO_VERUM(specula_exhaurire(s));
    CREDO_FALSUM(legenda(s, ZEPHYRUM));
    CREDO_NECESSE(filum_scribere_literis(via, "b\n"));
    CREDO_VERUM(legenda(s, CC));
    CREDO_VERUM(specula_exhaurire(s));

    CREDO_GRADUS("post recreationem: plagula nova speculatur");
    CREDO_NECESSE(filum_appendere_literis(via, "c\n"));
    CREDO_VERUM(legenda(s, CC));
    CREDO_VERUM(specula_exhaurire(s));
    CREDO_FALSUM(legenda(s, ZEPHYRUM));

    CREDO_GRADUS("claudere: fossa -1, iterum licet");
    specula_claudere(s);
    CREDO_AEQUALIS_S32(specula_fossa(s), -I);
    specula_claudere(s);
}

interior vacuum
_sectio_absens (
    CredoContextus* c)
{
      Specula* s;
    character  via[CCLVI];
    character  nusquam[CCLVI];

    CREDO_GRADUS("plagula absens: directorium speculatur");
    sprintf(via, "%s/nondum.jsonl", credo_area(c));
    s = specula_creare(c->piscina, via);
    CREDO_NECESSE(s);
    CREDO_FALSUM(legenda(s, ZEPHYRUM));

    CREDO_GRADUS("creata: legenda; deinde plagula ipsa speculatur");
    CREDO_NECESSE(filum_scribere_literis(via, "a\n"));
    CREDO_VERUM(legenda(s, CC));
    CREDO_VERUM(specula_exhaurire(s));
    CREDO_FALSUM(legenda(s, ZEPHYRUM));
    CREDO_NECESSE(filum_appendere_literis(via, "b\n"));
    CREDO_VERUM(legenda(s, CC));
    CREDO_VERUM(specula_exhaurire(s));
    specula_claudere(s);

    CREDO_GRADUS("directorium parentis absens: NIHIL");
    sprintf(nusquam, "%s/nullum/liber.jsonl", credo_area(c));
    CREDO_NIHIL(specula_creare(c->piscina, nusquam));
    CREDO_NIHIL(specula_creare(c->piscina, NIHIL));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "plagula", _sectio_plagula, NIHIL, NIHIL, NIHIL, NIHIL },
    { "absens", _sectio_absens, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("specula", SECTIONES);
}
