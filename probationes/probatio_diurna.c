/* probatio_diurna.c - acta diurna: taberna actorum (analytica T4).
 *
 * Corpus ponitur et legitur per clavem; corpus idem semel conditur;
 * corpus nimium caudam servat; clavis impura recusatur; corpus mutatum
 * CORRUPTUM nominatur, clavis numquam posita IGNOTUM; clavis fixa
 * notatur; status post reaperturam idem (index in disco, non in
 * memoria). Radix quaeque sub TMPDIR (area membri), in fine deleta.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
#include "filum.h"
#include "iter_directoria.h"
#include "diurna.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* radix temporaria propria sectionis (deleta ante et post) */
interior constans character*
_radix (
    constans character* titulus,
               Piscina* piscina)
{
    constans character* tmp = getenv("TMPDIR");
             character* via = (character*)piscina_allocare(piscina,
                 (memoriae_index)DXII);

    sprintf(via, "%s/probatio_diurna.%ld.%s",
        (tmp != NIHIL
            && tmp[0] != '\0') ? tmp : "/tmp", (longus)getpid(),
        titulus);
    (vacuum)filum_arborem_delere(via);
    redde via;
}

/* plagulae in corpora/<II hex>/ (corpora omnia unius praefixi) */
interior i32
_corpora_numerare (
    constans character* radix,
    constans character* sigillum,
               Piscina* piscina)
{
               character  via[DXII];
     DirectoriumIterator* it;
    DirectoriumIntroitus* introitus;
                     i32  numerus = ZEPHYRUM;

    sprintf(via, "%s/corpora/%.2s", radix, sigillum);
    it = directorium_iterator_aperire(via, piscina);
    si (it == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum ((introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (introitus->genus == INTROITUS_FILUM)
        {
            numerus++;
        }
    }
    directorium_iterator_claudere(it);
    redde numerus;
}

/* I. ponere et legere: introitus scriptus (sigillum LXIV hex, mensura),
 * corpus redditum idem, status ADEST, introitus unus in indice */
interior vacuum
_sectio_circuitus (
    CredoContextus* c)
{
    constans character* radix  = _radix("circuitus", c->piscina);
                Diurna* d      = diurna_aperire(radix, c->piscina);
       DiurnaIntroitus  positum;
          DiurnaStatus  status = DIURNA_IGNOTUM;
                chorda  corpus = chorda_ex_literis("salve\nmunde\n",
                    c->piscina);
                chorda  lectum;
                   Xar* omnes;

    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_NECESSE(diurna_ponere(d, chorda_ex_literis("c1", c->piscina),
        chorda_ex_literis("fumus/a", c->piscina), FALSUM, corpus,
        &positum));
    CREDO_AEQUALIS_S64((s64)strlen(positum.sigillum), (s64)LXIV);
    CREDO_AEQUALIS_S64(positum.mensura, (s64)XII);
    CREDO_AEQUALIS_S64(positum.originalis, (s64)XII);
    lectum = diurna_legere(d, chorda_ex_literis("c1", c->piscina),
        c->piscina, &status);
    CREDO_AEQUALIS_I32((i32)status, (i32)DIURNA_ADEST);
    CREDO_CHORDA_AEQUALIS(lectum, corpus);
    omnes = diurna_introitus(d, c->piscina);
    CREDO_NECESSE_NON_NIHIL(omnes);
    CREDO_AEQUALIS_I32(xar_numerus(omnes), I);
    CREDO_CHORDA_AEQUALIS(((DiurnaIntroitus*)xar_obtinere(omnes,
        ZEPHYRUM))->titulus, chorda_ex_literis("fumus/a", c->piscina));
    CREDO_AEQUALIS_I32((i32)((DiurnaIntroitus*)xar_obtinere(omnes,
        ZEPHYRUM))->forma, (i32)DIURNA_CRUDUM);
    (vacuum)filum_arborem_delere(radix);
}

/* II. corpus idem sub clavibus duabus: sigillum idem, plagula UNA;
 * corpus aliud: plagula altera; claves ambae leguntur */
interior vacuum
_sectio_semel (
    CredoContextus* c)
{
    constans character* radix  = _radix("dedup", c->piscina);
                Diurna* d      = diurna_aperire(radix, c->piscina);
       DiurnaIntroitus  a;
       DiurnaIntroitus  b;
          DiurnaStatus  status = DIURNA_IGNOTUM;
                chorda  quietum = chorda_ex_literis("omnia bene\n",
                    c->piscina);

    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_NECESSE(diurna_ponere(d, chorda_ex_literis("k1", c->piscina),
        chorda_ex_literis("t", c->piscina), FALSUM, quietum, &a));
    CREDO_NECESSE(diurna_ponere(d, chorda_ex_literis("k2", c->piscina),
        chorda_ex_literis("t", c->piscina), FALSUM, quietum, &b));
    CREDO_VERUM(strcmp(a.sigillum, b.sigillum) == ZEPHYRUM);
    CREDO_AEQUALIS_I32(_corpora_numerare(radix, a.sigillum, c->piscina),
        I);
    CREDO_CHORDA_AEQUALIS(diurna_legere(d, chorda_ex_literis("k2",
        c->piscina), c->piscina, &status), quietum);
    CREDO_AEQUALIS_I32((i32)status, (i32)DIURNA_ADEST);
    CREDO_AEQUALIS_I32(xar_numerus(diurna_introitus(d, c->piscina)),
        II);
    (vacuum)filum_arborem_delere(radix);
}

/* III. corpus > DIURNA_CORPUS_MAXIMUM: cauda servatur, linea
 * praecisionis praeponitur, originalis = mensura vera */
interior vacuum
_sectio_cauda (
    CredoContextus* c)
{
    constans character* radix  = _radix("cauda", c->piscina);
                Diurna* d      = diurna_aperire(radix, c->piscina);
       DiurnaIntroitus  positum;
          DiurnaStatus  status = DIURNA_IGNOTUM;
                   i32  n = (i32)DIURNA_CORPUS_MAXIMUM + (i32)MXXIV;
                    i8* octeti = (i8*)piscina_allocare(c->piscina,
                        (memoriae_index)n);
                chorda corpus;
                chorda lectum;
                chorda finis = chorda_ex_literis("CAUDA VERA\n",
                    c->piscina);
                   i32 k;

    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_NECESSE_NON_NIHIL(octeti);
    per (k = ZEPHYRUM; k < n; k++)
    {
        octeti[k] = (i8)('a' + (k % XXVI));
    }
    memcpy(octeti + n - finis.mensura, finis.datum,
        (size_t)finis.mensura);
    corpus = chorda_ex_buffer(octeti, n);
    CREDO_NECESSE(diurna_ponere(d, chorda_ex_literis("magnum",
        c->piscina), chorda_ex_literis("t", c->piscina), FALSUM, corpus,
        &positum));
    CREDO_AEQUALIS_S64(positum.originalis, (s64)n);
    CREDO_VERUM(positum.mensura < (s64)n);
    lectum = diurna_legere(d, chorda_ex_literis("magnum", c->piscina),
        c->piscina, &status);
    CREDO_AEQUALIS_I32((i32)status, (i32)DIURNA_ADEST);
    CREDO_AEQUALIS_S64((s64)lectum.mensura, positum.mensura);
    CREDO_CHORDA_INCIPIT(lectum, chorda_ex_literis("[... ",
        c->piscina));
    CREDO_VERUM(lectum.mensura > finis.mensura && memcmp(lectum.datum
        + lectum.mensura - finis.mensura, finis.datum,
        (size_t)finis.mensura) == ZEPHYRUM);
    (vacuum)filum_arborem_delere(radix);
}

/* IV. clavis impura (TAB, linea nova) aut vacua, titulus impurus:
 * FALSUM, nihil scriptum */
interior vacuum
_sectio_recusatio (
    CredoContextus* c)
{
    constans character* radix   = _radix("recusatio", c->piscina);
                Diurna* d       = diurna_aperire(radix, c->piscina);
                chorda  corpus  = chorda_ex_literis("x\n", c->piscina);

    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_FALSUM(diurna_ponere(d, chorda_ex_literis("a\tb", c->piscina),
        chorda_ex_literis("t", c->piscina), FALSUM, corpus, NIHIL));
    CREDO_FALSUM(diurna_ponere(d, chorda_ex_literis("a\nb", c->piscina),
        chorda_ex_literis("t", c->piscina), FALSUM, corpus, NIHIL));
    CREDO_FALSUM(diurna_ponere(d, chorda_ex_literis("", c->piscina),
        chorda_ex_literis("t", c->piscina), FALSUM, corpus, NIHIL));
    CREDO_FALSUM(diurna_ponere(d, chorda_ex_literis("k", c->piscina),
        chorda_ex_literis("t\tu", c->piscina), FALSUM, corpus, NIHIL));
    CREDO_AEQUALIS_I32(xar_numerus(diurna_introitus(d, c->piscina)),
        ZEPHYRUM);
    (vacuum)filum_arborem_delere(radix);
}

/* V. clavis numquam posita = IGNOTUM; corpus in disco mutatum =
 * CORRUPTUM (sigillum verificatur), numquam contentum falsum */
interior vacuum
_sectio_status (
    CredoContextus* c)
{
    constans character* radix  = _radix("status", c->piscina);
                Diurna* d      = diurna_aperire(radix, c->piscina);
       DiurnaIntroitus  positum;
          DiurnaStatus  status = DIURNA_ADEST;
             character  via[DXII];
                chorda  lectum;

    CREDO_NECESSE_NON_NIHIL(d);
    lectum = diurna_legere(d, chorda_ex_literis("nemo", c->piscina),
        c->piscina, &status);
    CREDO_AEQUALIS_I32((i32)status, (i32)DIURNA_IGNOTUM);
    CREDO_AEQUALIS_I32(lectum.mensura, ZEPHYRUM);
    CREDO_NECESSE(diurna_ponere(d, chorda_ex_literis("c1", c->piscina),
        chorda_ex_literis("t", c->piscina), VERUM,
        chorda_ex_literis("verum\n", c->piscina), &positum));
    sprintf(via, "%s/corpora/%.2s/%s", radix, positum.sigillum,
        positum.sigillum + II);
    CREDO_NECESSE(filum_existit(via));
    CREDO_NECESSE(filum_scribere_literis(via, "falsum\n"));
    lectum = diurna_legere(d, chorda_ex_literis("c1", c->piscina),
        c->piscina, &status);
    CREDO_AEQUALIS_I32((i32)status, (i32)DIURNA_CORRUPTUM);
    CREDO_AEQUALIS_I32(lectum.mensura, ZEPHYRUM);
    (vacuum)filum_arborem_delere(radix);
}

/* VI. figere: introitus fixus notatur; clavis ignota FALSUM */
interior vacuum
_sectio_figere (
    CredoContextus* c)
{
    constans character* radix  = _radix("figere", c->piscina);
                Diurna* d      = diurna_aperire(radix, c->piscina);
                   Xar* omnes;
                   i32  k;
                   b32  fixa    = FALSUM;
                   b32  libera  = FALSUM;

    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_NECESSE(diurna_ponere(d, chorda_ex_literis("f1", c->piscina),
        chorda_ex_literis("t", c->piscina), FALSUM,
        chorda_ex_literis("a\n", c->piscina), NIHIL));
    CREDO_NECESSE(diurna_ponere(d, chorda_ex_literis("f2", c->piscina),
        chorda_ex_literis("t", c->piscina), FALSUM,
        chorda_ex_literis("b\n", c->piscina), NIHIL));
    CREDO_VERUM(diurna_figere(d, chorda_ex_literis("f1", c->piscina)));
    CREDO_FALSUM(diurna_figere(d, chorda_ex_literis("nemo",
        c->piscina)));
    omnes = diurna_introitus(d, c->piscina);
    CREDO_NECESSE_NON_NIHIL(omnes);
    per (k = ZEPHYRUM; k < xar_numerus(omnes); k++)
    {
        DiurnaIntroitus* i = (DiurnaIntroitus*)xar_obtinere(omnes, k);

        si (chorda_aequalis_literis(i->clavis, "f1"))
        {
            fixa = i->fixum;
        }
        si (chorda_aequalis_literis(i->clavis, "f2"))
        {
            libera = !i->fixum;
        }
    }
    CREDO_VERUM(fixa);
    CREDO_VERUM(libera);
    (vacuum)filum_arborem_delere(radix);
}

/* VII. reapertio: manubrium novum eadem videt (index in disco) - et
 * fractum, momentum, titulus servantur */
interior vacuum
_sectio_iterum_aperta (
    CredoContextus* c)
{
    constans character* radix  = _radix("reapertio", c->piscina);
                Diurna* d      = diurna_aperire(radix, c->piscina);
                Diurna* e;
       DiurnaIntroitus  positum;
                   Xar* omnes;
       DiurnaIntroitus* i;
          DiurnaStatus  status = DIURNA_IGNOTUM;

    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_NECESSE(diurna_ponere(d, chorda_ex_literis("r1", c->piscina),
        chorda_ex_literis("porta:radix", c->piscina), VERUM,
        chorda_ex_literis("FRACTA\n", c->piscina), &positum));
    e = diurna_aperire(radix, c->piscina);
    CREDO_NECESSE_NON_NIHIL(e);
    omnes = diurna_introitus(e, c->piscina);
    CREDO_NECESSE_NON_NIHIL(omnes);
    CREDO_NECESSE(xar_numerus(omnes) == I);
    i = (DiurnaIntroitus*)xar_obtinere(omnes, ZEPHYRUM);
    CREDO_VERUM(i->fractum);
    CREDO_AEQUALIS_S64(i->momentum, positum.momentum);
    CREDO_CHORDA_AEQUALIS(i->titulus, chorda_ex_literis("porta:radix",
        c->piscina));
    CREDO_CHORDA_AEQUALIS(diurna_legere(e, chorda_ex_literis("r1",
        c->piscina), c->piscina, &status), chorda_ex_literis("FRACTA\n",
        c->piscina));
    (vacuum)filum_arborem_delere(radix);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ponere et legere", _sectio_circuitus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "corpus idem semel", _sectio_semel, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cauda corporis nimii", _sectio_cauda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "claves impurae recusatae", _sectio_recusatio, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "ignotum et corruptum", _sectio_status, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "figere", _sectio_figere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "reapertio", _sectio_iterum_aperta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("diurna", SECTIONES);
}
