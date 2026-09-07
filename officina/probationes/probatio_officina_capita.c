/* probatio_officina_capita.c - Gradus praebitionis capitum (unitas
 * communis nexus_ordines, sweep + legatus)
 *
 * Capita eiusdem basename in expansore PRIMUS-VINCIT sedent; ergo
 * ordo quo praebentur lex est, non fortuna litterarum. Lex
 * (2026-09-07, quaestio 01M1TD1FMFT3): include/ primum (capita
 * publica domus), cetera canonice, fixturae ('fixa' membrum viae)
 * ultimae - fixtura briar latina.h DECURTATUM ante include/ cadebat
 * (strcmp: 'b' < 'i') et omni TU praebebatur: silva_formator.c XIX
 * errores, sedes definitionum VI perditae, tabula 1.10M -> 0.96M.
 *
 * Sectio III arborem VERAM ambulat (RHUBARB_RADIX): inter omnia
 * capita repositorii, ordine comparatoris, primum 'latina.h' et
 * primum 'postulata_posix.h' sub include/ sedent - custos contra
 * fixturam quamlibet futuram.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "nexus_ordines.h"
#include "credo.h"
#include "piscina.h"
#include "xar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

interior constans character*
_radix (vacuum)
{
    constans character* r = getenv("RHUBARB_RADIX");

    redde r != NIHIL ? r : ".";
}

interior b32
_praetermittendum (
    constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* capita sub radice colligere: viae RADICI-RELATIVAE in piscinam */
interior vacuum
_capita_colligere (
               Piscina* piscina,
                   Xar* capita,
    constans character* radix,
    constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL)
    {
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL)
    {
             character via_plena[1024];
        memoriae_index m;

        si (   introitus->d_name[ZEPHYRUM] == '.'
            || _praetermittendum(introitus->d_name))
        {
            perge;
        }
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);
        si (introitus->d_type == DT_DIR)
        {
            _capita_colligere(piscina, capita, radix, via_plena);
            perge;
        }
        m = strlen(introitus->d_name);
        si (   m >= III && introitus->d_name[m - II] == '.'
            && introitus->d_name[m - I] == 'h')
        {
            constans character* relativa = via_plena
                + strlen(radix) + I;
                     character*  copia;
            constans character** sedes;

            copia = (character*)piscina_allocare(piscina,
                strlen(relativa) + I);
            si (copia == NIHIL)
            {
                perge;
            }
            strcpy(copia, relativa);
            sedes = (constans character**)xar_addere(capita);
            si (sedes != NIHIL)
            {
                *sedes = copia;
            }
        }
    }
    closedir(dir);
}

/* primum caput basename dati ordine dato */
interior constans character*
_primum_basename (
                   Xar* capita,
    constans character* basis)
{
    i32 n = xar_numerus(capita);
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        constans character* v =
            *(constans character**)xar_obtinere(capita, k);
        constans character* b = strrchr(v, '/');

        b = (b != NIHIL) ? b + I : v;
        si (strcmp(b, basis) == ZEPHYRUM)
        {
            redde v;
        }
    }
    redde NIHIL;
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    imprimere("=== probatio_officina_capita ===\n");
    piscina = piscina_generare_dynamicum("probatio_capita", 4194304);
        credo_aperire(piscina);
    CREDO_NON_NIHIL (piscina);

    /* I. gradus */
    imprimere("\n--- I. gradus ---\n");
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus("include/latina.h"), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus("./include/latina.h"), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus("silva/fontes/silva_token.h"), I);
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus("includes/x.h"), I);
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus("prefixa/x.h"), I);
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus("lib/fixatio/x.h"), I);
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus(
            "briar/probationes/fixa/amalgama/fabrica/include/latina.h"),
        II);
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus(
            "probationes/fixa/roundtrip/latina.h"), II);
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus("fixa/x.h"), II);
    CREDO_AEQUALIS_I32 (
        nexus_ordines_capitis_gradus("./fixa/x.h"), II);
    CREDO_AEQUALIS_I32 (nexus_ordines_capitis_gradus(NIHIL), I);
    CREDO_AEQUALIS_I32 (nexus_ordines_capitis_gradus(""), I);

    /* II. comparator: gradus, deinde ordo canonicus */
    imprimere("\n--- II. comparator ---\n");
    {
        constans character* fixtura =
            "briar/probationes/fixa/amalgama/fabrica/include/latina.h";
        constans character* publicum  = "include/latina.h";
        constans character* fontes    = "silva/fontes/silva_token.h";
        constans character* aliud     = "aa/x.h";
                       Xar* capita = xar_creare(piscina,
                           (i32)magnitudo(constans character*));
        constans character** sedes;

        CREDO_NON_NIHIL (capita);
        sedes   = (constans character**)xar_addere(capita);
        *sedes  = fixtura;
        sedes   = (constans character**)xar_addere(capita);
        *sedes  = fontes;
        sedes   = (constans character**)xar_addere(capita);
        *sedes  = publicum;
        sedes   = (constans character**)xar_addere(capita);
        *sedes  = aliud;
        xar_ordinare(capita, nexus_ordines_capita_comparare);
        CREDO_AEQUALIS_PTR (
            *(constans character**)xar_obtinere(capita, ZEPHYRUM),
            publicum);
        CREDO_AEQUALIS_PTR (
            *(constans character**)xar_obtinere(capita, I), aliud);
        CREDO_AEQUALIS_PTR (
            *(constans character**)xar_obtinere(capita, II), fontes);
        CREDO_AEQUALIS_PTR (
            *(constans character**)xar_obtinere(capita, III), fixtura);
        CREDO_MINOR_S32 (
            nexus_ordines_capita_comparare(&publicum, &fixtura),
            ZEPHYRUM);
        CREDO_MAIOR_S32 (
            nexus_ordines_capita_comparare(&fixtura, &aliud), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (
            nexus_ordines_capita_comparare(&aliud, &aliud), ZEPHYRUM);
    }

    /* III. arbor vera: primum latina.h / postulata_posix.h ordine
     * comparatoris sub include/ */
    imprimere("\n--- III. arbor vera ---\n");
    {
        Xar* capita = xar_creare(piscina,
            (i32)magnitudo(constans character*));
        constans character* primum;

        CREDO_NON_NIHIL (capita);
        _capita_colligere(piscina, capita, _radix(), _radix());
        CREDO_MAIOR_I32 (xar_numerus(capita), C);
        xar_ordinare(capita, nexus_ordines_capita_comparare);
        primum = _primum_basename(capita, "latina.h");
        CREDO_NON_NIHIL (primum);
        CREDO_VERUM (primum != NIHIL
            && strcmp(primum, "include/latina.h") == ZEPHYRUM);
        primum = _primum_basename(capita, "postulata_posix.h");
        CREDO_NON_NIHIL (primum);
        CREDO_VERUM (primum != NIHIL
            && strcmp(primum, "include/postulata_posix.h") == ZEPHYRUM);
        imprimere("  capita %d, primum latina.h = %s\n",
            (int)xar_numerus(capita), primum != NIHIL
                ? _primum_basename(capita, "latina.h") : "(nihil)");
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
