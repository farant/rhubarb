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

integer
principale (
    vacuum)
{
                     b32  praeteritus;
                 Piscina* piscina;
     MateriaDiagnosticum  d;
      MateriaSedesRelata  relata[I];
               character* aurum;
                     i32  aurum_mensura = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("probatio_pictor", 262144);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ex './tools/diagnostica.sh -machina' eiusdem plagulae */
    memset(&d, ZEPHYRUM, magnitudo(d));
    d.gravitas               = (s32)MATERIA_GRAVITAS_ERRATUM;
    d.codex                  = "grex/tok_clausura";
    d.causa                  = "'}' exspectata";
    d.nota                   = "hic exspectatur";
    d.tractus.linea          = (i32)II;
    d.tractus.columna        = (i32)I;
    d.tractus.linea_finis    = (i32)II;
    d.tractus.columna_finis  = (i32)I;
    d.tractus.initium        = (s32)17;
    d.tractus.finis          = (s32)17;
    d.tractus.est_fons       = VERUM;
    memset(relata, ZEPHYRUM, magnitudo(relata));
    relata[ZEPHYRUM].tractus.linea          = (i32)I;
    relata[ZEPHYRUM].tractus.columna        = (i32)IX;
    relata[ZEPHYRUM].tractus.linea_finis    = (i32)II;
    relata[ZEPHYRUM].tractus.columna_finis  = (i32)I;
    relata[ZEPHYRUM].tractus.initium        = (s32)8;
    relata[ZEPHYRUM].tractus.finis          = (s32)17;
    relata[ZEPHYRUM].tractus.est_fons       = VERUM;
    relata[ZEPHYRUM].nota                   = "hic coepit";
    d.relata                                = relata;
    d.numerus_relatorum                     = (i32)I;

    {
        chorda t;

        imprimere("\n--- I. OCTETIM idem ac instrumentum vivum ---\n");
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

    {
        chorda t;

        imprimere("\n--- II. Sine excerpto: caput solum ---\n");
        t = materia_pictor_scribere(piscina, &d, "g.sh", "crusta",
                FONS, (i32)strlen(FONS), FALSUM);
        /* caput + causa = lineae DUAE; excerptum abest */
        CREDO_VERUM (t.mensura > ZEPHYRUM);
        CREDO_VERUM (t.mensura < aurum_mensura);
    }

    {
        chorda t;

        imprimere("\n--- III. Codex cum ':' praefixum NON sumit ---\n");
        d.codex = "materia:ordo-octetorum";
        t = materia_pictor_scribere(piscina, &d, "g.sh", "crusta",
                FONS, (i32)strlen(FONS), FALSUM);
        CREDO_VERUM (t.mensura > ZEPHYRUM);
        /* 'crusta:materia:...' esset si praefixum caeco modo additum */
        CREDO_VERUM (memchr(t.datum, (integer)'[',
            (memoriae_index)t.mensura) != NIHIL);
        CREDO_FALSUM (_continet(t, "crusta:materia:"));
        d.codex = "grex/tok_clausura";
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
