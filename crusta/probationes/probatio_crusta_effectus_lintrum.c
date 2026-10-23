/* probatio_crusta_effectus_lintrum.c - regulae effectus super summarium
 * (effectus-plan T6; spec par. V)
 *
 * Regulae verae (crusta/lintrum/effectus/) super scripta in memoria
 * scripta (arbor ficta in area sectionis, credo_area). Regula quaeque
 * exemplum POSITIVUM et NEGATIVUM fert; gravitas per catenam (erratum
 * intra, monitum extra); excusatio super functione omnes situs eius
 * tegit, in loco alieno MORTUA nominatur (Review Focus 2).
 */

#include "postulata_posix.h"

#include "latina.h"
#include "credo.h"
#include "crusta_effectus.h"
#include "crusta_facies.h"
#include "filum.h"
#include "internamentum.h"
#include "materia_diagnostica.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MUNDUS SECTIONIS: regulae et tabula mandatorum per sectionem lectae
 * (tabula StmlNodus* non constans vocatis traditur - fixura esse non
 * potest), arbor ficta in area sectionis propria (credo_area). Lectio
 * semel ASSERITUR in parare_suitae. */
nomen structura {
           Piscina* piscina;
         character  radix[DXII];
    CrustaOptiones  optiones;
         StmlNodus* tabula;
} Mundus;

interior constans character*
_radix_viae (vacuum)
{
    constans character* radix_viae = getenv("RHUBARB_RADIX");

    redde radix_viae != NIHIL ? radix_viae : ".";
}

interior vacuum
_scribere (
         constans Mundus* mundus,
      constans character* relativa,
      constans character* contentum)
{
    character  via[DXII];
    character  directorium[DXII];
    character* ultimum;

    sprintf(via, "%s/%s", mundus->radix, relativa);
    strcpy(directorium, via);
    ultimum = strrchr(directorium, '/');
    si (ultimum != NIHIL)
    {
        *ultimum = '\0';
        (vacuum)filum_directorium_creare_cum_parentibus(directorium);
    }
    (vacuum)filum_scribere_literis(via, contentum);
}

/* scriptum scribere, summarium derivare, regulas applicare */
interior Xar*
_scriptum_iudicare (
                Mundus* mundus,
    constans character* titulus,
    constans character* contentum,
                    b32 in_catena)
{
             StmlNodus* sm;
                   Xar* summaria;
                   Xar* d;
    constans character* causa = NIHIL;

    _scribere(mundus, titulus, contentum);
    sm = crusta_effectus_derivare(mundus->piscina,
        mundus->optiones.intern, mundus->radix, titulus, mundus->tabula,
        &causa);
    CREDO_NON_NIHIL (sm);
    si (sm == NIHIL)
    {
        redde NIHIL;
    }
    summaria = xar_creare(mundus->piscina, (i32)magnitudo(StmlNodus*));
    *(StmlNodus**)xar_addere(summaria) = sm;
    d = crusta_effectus_diagnostica(mundus->piscina, titulus, contentum,
        (i32)strlen(contentum), summaria, in_catena, &mundus->optiones,
        &causa);
    CREDO_NON_NIHIL (d);
    si (d == NIHIL)
    {
        imprimere("  CAUSA: %s\n", causa ? causa : "?");
    }
    redde d;
}

/* diagnostica codicis (NIHIL = quodvis) et gravitatis (-I = quaevis) */
interior i32
_numerare (
                    Xar* d,
     constans character* codex,
                    s32  gravitas)
{
    i32 n = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; d != NIHIL && k < xar_numerus(d); k++)
    {
        constans MateriaDiagnosticum* x =
            (constans MateriaDiagnosticum*)
            xar_obtinere(d, k);

        si (   codex != NIHIL
            && (x->codex == NIHIL
            || strcmp(x->codex, codex) != ZEPHYRUM))
        {
            perge;
        }
        si (gravitas >= ZEPHYRUM && x->gravitas != gravitas)
        {
            perge;
        }
        n++;
    }
    redde n;
}

interior vacuum
_imprimere (
    Xar* d)
{
    i32 k;

    per (k = ZEPHYRUM; d != NIHIL && k < xar_numerus(d); k++)
    {
        constans MateriaDiagnosticum* x =
            (constans MateriaDiagnosticum*)
            xar_obtinere(d, k);

        imprimere("    %s (gravitas %d) @%d\n",
            x->codex ? x->codex : "?", (integer)x->gravitas,
            (integer)x->tractus.initium);
    }
}

#define IRRESOLUTUM "lint:effectus-irresolutum"
#define SINE_DOMINO "lint:effectus-build-sine-domino"
#define IGNOTUM     "lint:effectus-mandatum-ignotum"
#define ERRATUM     ((s32)MATERIA_GRAVITAS_ERRATUM)
#define MONITUM     ((s32)MATERIA_GRAVITAS_MONITUM)

/* mundum sectionis aedificare SINE assertione (creatio semel in
 * parare_suitae asserta); FALSUM = lectio fracta */
interior b32
_mundum_parare (
    CredoContextus* c,
            Mundus* mundus)
{
     constans character* area   = credo_area(c);
     constans character* causa  = NIHIL;
              character  via[DXII];
           StmlResultus  t;

    si (area == NIHIL)
    {
        redde FALSUM;
    }
    mundus->piscina = c->piscina;
    sprintf(mundus->radix, "%s/arbor", area);
    memset(&mundus->optiones, ZEPHYRUM, magnitudo(mundus->optiones));
    mundus->optiones.intern = internamentum_creare(c->piscina);
    sprintf(via, "%s/crusta/lintrum/effectus", _radix_viae());
    mundus->optiones.regulae = crusta_regulae_legere(c->piscina, via,
        mundus->optiones.intern, &causa);
    sprintf(via, "%s/crusta/effectus_mandata.stml", _radix_viae());
    t = stml_legere(filum_legere_totum(via, c->piscina), c->piscina,
        mundus->optiones.intern);
    mundus->tabula = t.elementum_radix;
    redde mundus->optiones.regulae != NIHIL && mundus->tabula != NIHIL;
}

/* lectio regularum et tabulae semel asserta (ut olim ante sectiones) */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
     InternamentumChorda* intern  = internamentum_creare(c->piscina);
      constans character* causa   = NIHIL;
                     Xar* regulae;
               character  via[DXII];

    sprintf(via, "%s/crusta/lintrum/effectus", _radix_viae());
    regulae = crusta_regulae_legere(c->piscina, via, intern, &causa);
    CREDO_NON_NIHIL (regulae);
    CREDO_VERUM (regulae != NIHIL && xar_numerus(regulae) == III);
    sprintf(via, "%s/crusta/effectus_mandata.stml", _radix_viae());
    {
        StmlResultus t = stml_legere(filum_legere_totum(via,
            c->piscina),
            c->piscina, intern);

        CREDO_VERUM (t.successus);
    }
}

interior vacuum
_irresolutum_probare (
    CredoContextus* c)
{
    Mundus  mundus;
       Xar* d;

    si (!_mundum_parare(c, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    d = _scriptum_iudicare(&mundus, "a.sh",
        "f () {\n    cat \"$1/x\"\n}\n",
        VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, ERRATUM), I);
    /* extra catenam irresolutum TACET (Fran 2026-10-05, Q8 emendata:
     * clavem solam tangit; census situm servat) */
    d = _scriptum_iudicare(&mundus, "a.sh",
        "f () {\n    cat \"$1/x\"\n}\n",
        FALSUM);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, -I), ZEPHYRUM);
    d = _scriptum_iudicare(&mundus, "b.sh", "cat data/x\n", VERUM);
    CREDO_AEQUALIS_I32 (_numerare(d, NIHIL, -I), ZEPHYRUM);
    /* scriptura irresoluta consulto non flagratur */
    d = _scriptum_iudicare(&mundus, "c.sh", "echo x > \"$NESCIO/y\"\n",
        VERUM);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, -I), ZEPHYRUM);
}

interior vacuum
_sine_domino_probare (
    CredoContextus* c)
{
    Mundus  mundus;
       Xar* d;

    si (!_mundum_parare(c, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    d = _scriptum_iudicare(&mundus, "d.sh", "cat build/x\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, SINE_DOMINO, MONITUM), I);
    /* extra catenam regulae ceterae monent (non tacent) */
    d = _scriptum_iudicare(&mundus, "d.sh", "cat build/x\n", FALSUM);
    CREDO_AEQUALIS_I32 (_numerare(d, SINE_DOMINO, MONITUM), I);
    d = _scriptum_iudicare(&mundus, "e.sh",
        "mkdir -p build\necho y > build/x\n"
        "cat build/x\n", VERUM);
    CREDO_AEQUALIS_I32 (_numerare(d, SINE_DOMINO, -I), ZEPHYRUM);
}

interior vacuum
_ignotum_probare (
    CredoContextus* c)
{
    Mundus  mundus;
       Xar* d;

    si (!_mundum_parare(c, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    d = _scriptum_iudicare(&mundus, "f.sh", "jqx a\neval \"$X\"\n",
        VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, IGNOTUM, ERRATUM), II);
    d = _scriptum_iudicare(&mundus, "g.sh",
        "cat a >/dev/null\nhead -1 a\n",
        VERUM);
    CREDO_AEQUALIS_I32 (_numerare(d, IGNOTUM, -I), ZEPHYRUM);
}

interior vacuum
_excusationem_probare (
    CredoContextus* c)
{
    Mundus  mundus;
       Xar* d;

    si (!_mundum_parare(c, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    d = _scriptum_iudicare(&mundus, "h.sh",
        "# <tolera codex=\"lint:effectus-irresolutum\" (>sera\n"
        "f () {\n"
        "    cat \"$1/a\"\n"
        "    cat \"$1/b\"\n"
        "    [ -f \"$1/c\" ]\n"
        "}\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, NIHIL, -I), ZEPHYRUM);
    /* excusatio intra functionem super lectione PRIMA: secunda manet */
    d = _scriptum_iudicare(&mundus, "i.sh",
        "f () {\n"
        "    # <tolera codex=\"lint:effectus-irresolutum\" (>sera\n"
        "    cat \"$1/a\"\n"
        "    cat \"$1/b\"\n"
        "}\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, -I), I);
    /* excusatio super linea sine situ: MORTUA nominatur */
    d = _scriptum_iudicare(&mundus, "j.sh",
        "# <tolera codex=\"lint:effectus-irresolutum\" (>nihil tegit\n"
        "cat data/x\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, -I), ZEPHYRUM);
    CREDO_MAIOR_I32 (_numerare(d, NIHIL, -I), ZEPHYRUM);
}

interior vacuum
_membra_probare (
    CredoContextus* c)
{
    Mundus  mundus;
       Xar* d;

    (vacuum)c;
    si (!_mundum_parare(c, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    /* RF 2: build-sine-domino per viam (membra duo -> monita duo) */
    d = _scriptum_iudicare(&mundus, "k.sh",
        "for f in x1 x2; do cat \"build/n/$f\"; done\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, SINE_DOMINO, MONITUM), II);
    /* irresolutum per verbum: unum */
    d = _scriptum_iudicare(&mundus, "l.sh",
        "for f in $(ls); do cat \"$f\"; done\n", VERUM);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, -I), I);
    /* Q8 continentia: scriptor build/w/ *.o lectorem build/w/a*.o
     * tegit; lector latior (build/ *) non tegitur */
    d = _scriptum_iudicare(&mundus, "m.sh",
        "for f in src/*.c; do\n"
        "    clang -c \"$f\" -o \"build/w/$(basename \"$f\" .c).o\"\n"
        "done\n"
        "cat build/w/a*.o\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, SINE_DOMINO, -I), ZEPHYRUM);
    d = _scriptum_iudicare(&mundus, "n.sh",
        "for f in src/*.c; do\n"
        "    clang -c \"$f\" -o \"build/w/$(basename \"$f\" .c).o\"\n"
        "done\n"
        "cat build/*\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, SINE_DOMINO, MONITUM), I);
}

interior vacuum
_catenas_probare (
    CredoContextus* c)
{
                Mundus  mundus;
    constans character* causa = NIHIL;

    si (!_mundum_parare(c, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        Xar* catenae;

        _scribere(&mundus, "fabrica.stml", "<fabrica titulus=\"x\">"
            "<subsystema via=\"s\"/></fabrica>\n");
        _scribere(&mundus, "s/aedificatio.stml",
            "<aedificatio><actio titulus=\"porta_x\" "
            "genus=\"iudicium\">"
            "<ingressus genus=\"fontationes\" via=\"s/r.sh\"/>"
            "<ingressus genus=\"effectus\" via=\"s/q.sh\"/>"
            "<ingressus genus=\"fasciculus\" via=\"s/f.sh\"/>"
            "<exitus via=\"v.txt\" provenientia=\"verdictum\"/>"
            "</actio></aedificatio>\n");
        catenae = crusta_effectus_catenae(c->piscina,
            mundus.optiones.intern, mundus.radix, &causa);
        CREDO_NON_NIHIL (catenae);
        /* 'fontationes' (retiratum T8) et 'fasciculus' radices non
         * sunt */
        CREDO_AEQUALIS_I32 (catenae ? xar_numerus(catenae) : ZEPHYRUM,
            I);
        si (catenae != NIHIL && xar_numerus(catenae) == I)
        {
            CREDO_VERUM (strcmp(*(character**)xar_obtinere(catenae,
                ZEPHYRUM), "s/q.sh") == ZEPHYRUM);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. effectus-irresolutum",
        _irresolutum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II. effectus-build-sine-domino",
        _sine_domino_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "III. effectus-mandatum-ignotum",
        _ignotum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IV. excusatio super functione (RF 2)",
        _excusationem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IV b. membra et exemplaria (plan-2 T5)",
        _membra_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "V. catenae ex declarationibus fabricae",
        _catenas_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("crusta_effectus_lintrum", SECTIONES,
        _suitam_parare, NIHIL);
}
