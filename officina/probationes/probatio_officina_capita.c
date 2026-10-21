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
 *
 * Sectio IV: repositio declarationum (columna VIII nexus.tsv). Sedes
 * 'extern' = externus, 'static' = staticus, ceterae '-'.
 * tools/corpus_infixum.sh declarationes externas inter statica non
 * numerat: 'extern char **environ' in lib/processus_posix.c est
 * environ libc, et conflatio (knotapel/archive.sh, D125) id
 * renominavit - nexus fractus (2026-10-12).
 */

#include "postulata_posix.h"
#include "latina.h"
#include "nexus_ordines.h"
#include "praeparator.h"
#include "credo.h"
#include "piscina.h"
#include "xar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

/* sedes profunditatis 0 cum repositione (sectio IV) */
#define SEDES_MAXIMAE  XVI

/* receptor per datum scribit: status sectionis IV proprius (olim
 * statica plagulae - examen 92 in sectione v2) */
nomen structura {
    character titulus[SEDES_MAXIMAE][XXXII];
    character valor[SEDES_MAXIMAE][XVI];
          i32 numerus;
} SedesTabula;

interior vacuum
_sedem_recipere (
                  vacuum* datum,
    constans SilvaChorda* titulus,
      constans character* species,
      constans character* genus,
    constans SilvaChorda* via,
      insignatus integer  linea,
      insignatus integer  columna,
      insignatus integer  profunditas,
      constans character* repositio)
{
    SedesTabula* tabula = (SedesTabula*)datum;

    (vacuum)genus;
    (vacuum)via;
    (vacuum)linea;
    (vacuum)columna;
    si (   strcmp(species, "sedes") != ZEPHYRUM
        || profunditas              != ZEPHYRUM
        || tabula->numerus          >= SEDES_MAXIMAE
        || titulus->mensura         >= XXXII
        || strlen(repositio)        >= XVI)
    {
        redde;
    }
    memcpy(tabula->titulus[tabula->numerus], titulus->datum,
        (memoriae_index)titulus->mensura);
    tabula->titulus[tabula->numerus][titulus->mensura] = '\0';
    strcpy(tabula->valor[tabula->numerus], repositio);
    tabula->numerus++;
}

/* repositio sedis nominatae (prima); NIHIL si nulla */
interior constans character*
_repositio (
     constans SedesTabula* tabula,
       constans character* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < tabula->numerus; k++)
    {
        si (strcmp(tabula->titulus[k], titulus) == ZEPHYRUM)
        {
            redde tabula->valor[k];
        }
    }
    redde NIHIL;
}

/* sedes nominatae cum repositione data (declarationes eiusdem nominis
 * singulae sedes sunt) */
interior i32
_sedes_cum (
     constans SedesTabula* tabula,
       constans character* titulus,
       constans character* valor)
{
    i32 k;
    i32 n = ZEPHYRUM;

    per (k = ZEPHYRUM; k < tabula->numerus; k++)
    {
        si (   strcmp(tabula->titulus[k], titulus) == ZEPHYRUM
            && strcmp(tabula->valor[k], valor)     == ZEPHYRUM)
        {
            n++;
        }
    }
    redde n;
}

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

/* praeludium v1: piscina asserta (assertio ut olim numeratur) */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    CREDO_NON_NIHIL (c->piscina);
}


/* ==================================================
 * PROBARE: I. gradus capitum
 * ================================================== */

interior vacuum
_gradus_probare (
    CredoContextus* c)
{
    (vacuum)c;
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
}


/* ==================================================
 * PROBARE: II. comparator: gradus, deinde ordo canonicus
 * ================================================== */

interior vacuum
_comparatorem_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

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
}


/* ==================================================
 * PROBARE: III. arbor vera (RHUBARB_RADIX)
 * ================================================== */

interior vacuum
_arborem_veram_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

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
}


/* ==================================================
 * PROBARE: IV. repositio declarationum (columna VIII nexus.tsv)
 * ================================================== */

interior vacuum
_declarationes_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
        constans character* fons =
            "extern char **alibi;\n"
            "static int proprius;\n"
            "int publicus;\n"
            "extern int posterius;\n"
            "int posterius = 3;\n"
            "extern int functio_alibi(void);\n"
            "int specimen(void) { return proprius + publicus"
            " + posterius + functio_alibi() + (alibi != 0); }\n";
                    SedesTabula tabula;
        PraeparatorConfiguratio cfg;
                    Praeparatio praeparatio;
                   SilvaPiscina* effimera;
                   SilvaParsura* parsura = NIHIL;
                 SilvaSemantica* sem;
                       Piscina*  piscina_ordinum;

        tabula.numerus = ZEPHYRUM;
        memset(&cfg, ZEPHYRUM, magnitudo(PraeparatorConfiguratio));
        cfg.radix = NIHIL;   /* sine systemate: fons sibi sufficit */
        CREDO_VERUM (praeparator_praeparare(&praeparatio, piscina, &cfg)
            != ZEPHYRUM);
        effimera =
            silva_piscina_generare_dynamicum("probatio_repositio",
            4194304);
        piscina_ordinum =
            piscina_generare_dynamicum("ordines_repositio",
            65536);
        CREDO_NON_NIHIL (effimera);
        CREDO_NON_NIHIL (piscina_ordinum);
        sem = praeparator_analysare(&praeparatio, effimera,
            "lib/specimen_repositionis.c", fons,
            (insignatus integer)strlen(fons), &parsura);
        CREDO_NON_NIHIL (sem);
        si (   sem             != NIHIL && parsura != NIHIL
            && piscina_ordinum != NIHIL)
        {
            nexus_ordines_fundere(parsura, sem, piscina_ordinum,
                _sedem_recipere, &tabula);
        }
        imprimere("  sedes %d: alibi %s, proprius %s, publicus %s,"
            " specimen %s\n", (integer)tabula.numerus,
            _repositio(&tabula, "alibi")
                ? _repositio(&tabula, "alibi") : "(nihil)",
            _repositio(&tabula, "proprius")
                ? _repositio(&tabula, "proprius") : "(nihil)",
            _repositio(&tabula, "publicus")
                ? _repositio(&tabula, "publicus") : "(nihil)",
            _repositio(&tabula, "specimen")
                ? _repositio(&tabula, "specimen") : "(nihil)");
        CREDO_VERUM (_repositio(&tabula, "alibi") != NIHIL
            && strcmp(_repositio(&tabula, "alibi"), "externus")
                == ZEPHYRUM);
        CREDO_VERUM (_repositio(&tabula, "proprius") != NIHIL
            && strcmp(_repositio(&tabula, "proprius"), "staticus")
                == ZEPHYRUM);
        CREDO_VERUM (_repositio(&tabula, "publicus") != NIHIL
            && strcmp(_repositio(&tabula, "publicus"), "-")
                == ZEPHYRUM);
        CREDO_VERUM (_repositio(&tabula, "specimen") != NIHIL
            && strcmp(_repositio(&tabula, "specimen"), "-")
                == ZEPHYRUM);
        /* extern deinde definitio in eadem TU: sedes duae, declaratio
         * externa et definitio simplex (haec manet statica in tabula
         * corpus_infixum si nullum caput eam fert) */
        CREDO_AEQUALIS_I32 (_sedes_cum(&tabula, "posterius",
            "externus"), I);
        CREDO_AEQUALIS_I32 (_sedes_cum(&tabula, "posterius", "-"),
            I);
        /* prototypus functionis extern: externus (non statica) */
        CREDO_AEQUALIS_I32 (_sedes_cum(&tabula, "functio_alibi",
            "externus"), I);
        CREDO_AEQUALIS_I32 (_sedes_cum(&tabula, "functio_alibi",
            "-"), ZEPHYRUM);
        si (effimera != NIHIL) silva_piscina_destruere(effimera);
        si (piscina_ordinum
            != NIHIL) piscina_destruere(piscina_ordinum);
        praeparator_destruere(&praeparatio);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. gradus", _gradus_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II. comparator", _comparatorem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "III. arbor vera", _arborem_veram_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "IV. repositio declarationum", _declarationes_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    imprimere("=== probatio_officina_capita ===\n");
    redde credo_suitam_currere_cum("officina_capita", SECTIONES,
        _suitam_parare, NIHIL);
}
