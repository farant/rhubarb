/* probatio_iteratio.c - iteratio plagulae notatae (eventus A6b; spec
 * D6): CRUDA contra SEMANTICA super dispositionem motam. Sessio toy
 * notatur; dispositio XV pixela deorsum movetur; cruda b1 amittit
 * (divergentia nuntiata), semantica statum finalem reddit. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "filum.h"
#include "eventus_stml.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
     InsulaRepositorium* repo;
            Dispensator* d;
             ManusLudus* m;
              ToyStatus  toy;
} Sessio;

/* Fixura suitae: sessio A semel notatur (notata, status_a ->
 * c->suita); sectiones LEGUNT solum, sessiones suas creant. */
nomen structura {
    Sessio  a;
       Xar* eventus;
       Xar* notata;
    chorda  status_a;
} Mundus;

interior vacuum
sessio_creare (
                 Sessio* s,
                Piscina* piscina,
    InternamentumChorda* intern,
                    s32  translatio_y)
{
    ActioRegistrum* reg;

    memset(s, ZEPHYRUM, magnitudo(Sessio));
    s->toy.translatio_y = translatio_y;
    s->repo = insula_repositorium_creare(piscina, intern,
        "<insula/>");
    reg = actio_registrum_creare(piscina, intern);
    toy_registrare(reg, &s->toy);
    s->d = dispensator_creare(piscina, intern, s->repo, reg,
        toy_componere, &s->toy, CCC);
    s->m = manus_ludus_creare(piscina, s->d);
}

/* status finalis: insulae ambae + focus */
interior chorda
status (
     Sessio* s,
    Piscina* piscina)
{
    chorda c;

    c = chorda_concatenare(
        insula_scribere(s->repo, piscina),
        insula_scribere(s->repo, piscina), piscina);
    redde chorda_concatenare(c, dispensator_focus(s->d), piscina);
}

interior ManusDivergentia*
divergentia (
    Xar* x,
    i32  i)
{
    redde (ManusDivergentia*)xar_obtinere(x, i);
}

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                 Mundus* o;
                 chorda  fons;
                    i32  i;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    o        = (Mundus*)piscina_allocare(piscina, magnitudo(Mundus));
    sessio_creare(&o->a, piscina, intern, ZEPHYRUM);
    fons = filum_legere_totum("probationes/pictor/toy.eventus.stml",
        piscina);
    o->eventus = eventus_legere_stml(chorda_ut_cstr(fons, piscina),
        piscina, intern);
    o->notata = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
    dispensator_notarium_ponere(o->a.d, o->notata);
    per (i = ZEPHYRUM; i < xar_numerus(o->eventus); i++)
    {
        dispensator_tractare(o->a.d,
            (constans Eventus*)xar_obtinere(o->eventus, i));
    }
    o->status_a  = status(&o->a, piscina);
    c->datum     = o;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    constans Mundus* o = (constans Mundus*)c->suita;

    CREDO_NON_NIHIL(o->eventus);
    CREDO_NON_NIHIL(insula_attributum(o->a.repo,
        "numerus"));
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
         constans Mundus* o = (constans Mundus*)c->suita;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  Sessio  b;
                     Xar* div;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    sessio_creare(&b, piscina, intern, ZEPHYRUM);
    div = xar_creare(piscina, (i32)magnitudo(ManusDivergentia));
    CREDO_AEQUALIS_I32(manus_ludus_iterare(b.m, o->notata,
        MANUS_ITERATIO_SEMANTICA, div), ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis(status(&b, piscina), o->status_a));
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
         constans Mundus* o = (constans Mundus*)c->suita;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  Sessio  b;
                     Xar* div;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    sessio_creare(&b, piscina, intern, XV);
    div = xar_creare(piscina, (i32)magnitudo(ManusDivergentia));
    CREDO_AEQUALIS_I32(manus_ludus_iterare(b.m, o->notata,
        MANUS_ITERATIO_CRUDA, div), II);
    CREDO_AEQUALIS_I32(xar_numerus(div), II);
    CREDO_AEQUALIS_I32(divergentia(div, ZEPHYRUM)->index, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(divergentia(div, ZEPHYRUM)
        ->scopus_notatus, "b1");
    CREDO_CHORDA_AEQUALIS_LITERIS(divergentia(div, ZEPHYRUM)
        ->scopus_crudus, "radix");
    CREDO_AEQUALIS_I32(divergentia(div, I)->index, I);
    /* ictus in b1 periit: numerus abest - status differt */
    CREDO_NIHIL(insula_attributum(b.repo, "numerus"));
    CREDO_FALSUM(chorda_aequalis(status(&b, piscina), o->status_a));
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
         constans Mundus* o = (constans Mundus*)c->suita;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  Sessio  b;
                     Xar* div;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    sessio_creare(&b, piscina, intern, XV);
    div = xar_creare(piscina, (i32)magnitudo(ManusDivergentia));
    /* divergentiae eaedem NUNTIANTUR (positio cruda b1 non tangit) */
    CREDO_AEQUALIS_I32(manus_ludus_iterare(b.m, o->notata,
        MANUS_ITERATIO_SEMANTICA, div), II);
    CREDO_VERUM(chorda_aequalis(status(&b, piscina), o->status_a));
    CREDO_NON_NIHIL(insula_attributum(b.repo,
        "numerus"));
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
         constans Mundus* o = (constans Mundus*)c->suita;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  Sessio  b;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    sessio_creare(&b, piscina, intern, XV);
    CREDO_AEQUALIS_I32(manus_ludus_iterare(b.m, o->notata,
        MANUS_ITERATIO_CRUDA, NIHIL), II);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "A. sessio notata (dispositio ordinaria)", _sectio_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "B. semantica, eadem dispositio", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "C. cruda, dispositio mota XV: b1 amissus", _sectio_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "D. semantica, mota XV: status idem", _sectio_quarta, NIHIL,
        NIHIL,
        NIHIL, NIHIL },
    { "E. divergentiae NIHIL licet", _sectio_quinta, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("iteratio", SECTIONES,
        _suitam_parare, NIHIL);
}
