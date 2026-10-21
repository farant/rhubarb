/* probatio_materia_pictor.c - Compositor textus diagnostici
 * (project-specs/exemplaria-c-spec.md E4)
 *
 * PORTA VERA OCTETIM EST: aurum
 * (probationes/fixa/pictor/aurum.txt) ex instrumento VIVO captum est
 * ANTE migrationem, ergo probat compositorem octetos EOSDEM reddere
 * quos usor heri vidit - non octetos quos ego hodie putem rectos.
 *
 * Diagnosticum manu aedificatur ex TSV vero eiusdem plagulae
 * (diagnostica -machina), ergo numeri hic ficti non sunt.
 */

#include "latina.h"
#include "credo.h"
#include "materia_pictor.h"
#include "materia_diagnostica.h"
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* plagula aurea: "echo a; { echo b\n" - XVII octeti */
hic_manens constans character* FONS = "echo a; { echo b\n";

hic_manens character*
_aurum_legere (
    Piscina* piscina,
        i32* mensura)
{
             FILE* f;
       character* memoria;
    constans character* via =
        "materia/probationes/fixa/pictor/aurum.txt";
           longus longitudo;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (   memoria == NIHIL
        || fread(memoria, I, (size_t)longitudo, f)
               != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    memoria[longitudo]  = '\0';
    *mensura            = (i32)longitudo;
    redde memoria;
}

/* chorda NUL non terminatur, ergo 'strstr' vetitum (examen id
 * statim clamavit) - scansio manualis */
hic_manens b32
_continet (
                 chorda  ubi,
     constans character* quid)
{
    i32 n = (i32)strlen(quid);
    i32 k;

    si (ubi.mensura < n)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + n <= ubi.mensura; k++)
    {
        si (memcmp(ubi.datum + k, quid, (memoriae_index)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Diagnosticum ex './tools/diagnostica.sh -machina' eiusdem plagulae
 * - sine assertione; sectio quaeque suum aedificat (III codicem mutat,
 * ergo status communis nullus). */
interior vacuum
_diagnosticum_parare (
    MateriaDiagnosticum* d,
     MateriaSedesRelata* relata)
{
    memset(d, ZEPHYRUM, magnitudo(*d));
    d->gravitas               = (s32)MATERIA_GRAVITAS_ERRATUM;
    d->codex                  = "grex/tok_clausura";
    d->causa                  = "'}' exspectata";
    d->nota                   = "hic exspectatur";
    d->tractus.linea          = (i32)II;
    d->tractus.columna        = (i32)I;
    d->tractus.linea_finis    = (i32)II;
    d->tractus.columna_finis  = (i32)I;
    d->tractus.initium        = (s32)17;
    d->tractus.finis          = (s32)17;
    d->tractus.est_fons       = VERUM;
    memset(relata, ZEPHYRUM, magnitudo(*relata));
    relata->tractus.linea          = (i32)I;
    relata->tractus.columna        = (i32)IX;
    relata->tractus.linea_finis    = (i32)II;
    relata->tractus.columna_finis  = (i32)I;
    relata->tractus.initium        = (s32)8;
    relata->tractus.finis          = (s32)17;
    relata->tractus.est_fons       = VERUM;
    relata->nota                   = "hic coepit";
    d->relata                      = relata;
    d->numerus_relatorum           = (i32)I;
}


/* ==================================================
 * PROBARE: OCTETIM idem ac instrumentum vivum
 * ================================================== */

interior vacuum
_aurum_conferre (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
     MateriaDiagnosticum  d;
      MateriaSedesRelata  relata[I];
               character* aurum;
                     i32  aurum_mensura = ZEPHYRUM;
                  chorda  t;

    _diagnosticum_parare(&d, relata);
    aurum = _aurum_legere(piscina, &aurum_mensura);
    CREDO_NON_NIHIL (aurum);
    CREDO_VERUM (aurum_mensura > ZEPHYRUM);
    t = materia_pictor_scribere(piscina, &d, "g.sh", "crusta",
            FONS, (i32)strlen(FONS), VERUM);
    CREDO_AEQUALIS_I32 (t.mensura, aurum_mensura);
    si (t.mensura == aurum_mensura)
    {
        CREDO_VERUM (memcmp(t.datum, aurum,
            (memoriae_index)aurum_mensura) == ZEPHYRUM);
    }
    alioquin
    {
        imprimere("  FACTUM: [%.*s]\n", (integer)t.mensura,
            (constans character*)t.datum);
    }
}


/* ==================================================
 * PROBARE: sine excerpto - caput solum
 * ================================================== */

interior vacuum
_sine_excerpto_probare (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
     MateriaDiagnosticum  d;
      MateriaSedesRelata  relata[I];
                     i32  aurum_mensura = ZEPHYRUM;
                  chorda  t;

    _diagnosticum_parare(&d, relata);
    /* mensura auri sine assertione relecta (prima eam asserit) */
    si (_aurum_legere(piscina, &aurum_mensura) == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    t = materia_pictor_scribere(piscina, &d, "g.sh", "crusta",
            FONS, (i32)strlen(FONS), FALSUM);
    /* caput + causa = lineae DUAE; excerptum abest */
    CREDO_VERUM (t.mensura > ZEPHYRUM);
    CREDO_VERUM (t.mensura < aurum_mensura);
}


/* ==================================================
 * PROBARE: codex cum ':' praefixum NON sumit
 * ================================================== */

interior vacuum
_praefixum_codicis_probare (
    CredoContextus* c)
{
                 Piscina* piscina = c->piscina;
     MateriaDiagnosticum  d;
      MateriaSedesRelata  relata[I];
                  chorda  t;

    _diagnosticum_parare(&d, relata);
    d.codex = "materia:ordo-octetorum";
    t = materia_pictor_scribere(piscina, &d, "g.sh", "crusta",
            FONS, (i32)strlen(FONS), FALSUM);
    CREDO_VERUM (t.mensura > ZEPHYRUM);
    /* 'crusta:materia:...' esset si praefixum caeco modo additum */
    CREDO_VERUM (memchr(t.datum, (integer)'[',
        (memoriae_index)t.mensura) != NIHIL);
    CREDO_FALSUM (_continet(t, "crusta:materia:"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "octetim idem ac instrumentum vivum", _aurum_conferre, NIHIL,
        NIHIL,
        NIHIL, NIHIL },
    { "sine excerpto: caput solum", _sine_excerpto_probare, NIHIL,
        NIHIL,
        NIHIL, NIHIL },
    { "codex cum ':' praefixum non sumit", _praefixum_codicis_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("materia_pictor", SECTIONES);
}
