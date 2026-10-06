/* probatio_crusta_effectus_lintrum.c - regulae effectus super summarium
 * (effectus-plan T6; spec par. V)
 *
 * Regulae verae (crusta/lintrum/effectus/) super scripta in memoria
 * scripta (arbor ficta sub crusta/build/). Regula quaeque exemplum
 * POSITIVUM et NEGATIVUM fert; gravitas per catenam (erratum intra,
 * monitum extra); excusatio super functione omnes situs eius tegit, in
 * loco alieno MORTUA nominatur (Review Focus 2).
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
#include <unistd.h>

hic_manens character radix[CCLVI];
hic_manens constans character* r;
hic_manens Piscina* piscina;
hic_manens CrustaOptiones optiones;
hic_manens StmlNodus* tabula;

interior vacuum
_scribere (
    constans character* relativa,
    constans character* contentum)
{
    character  via[DXII];
    character  directorium[DXII];
    character* ultimum;

    sprintf(via, "%s/%s", radix, relativa);
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
    constans character* titulus,
    constans character* contentum,
                    b32 in_catena)
{
             StmlNodus* sm;
                   Xar* summaria;
                   Xar* d;
    constans character* causa = NIHIL;

    _scribere(titulus, contentum);
    sm = crusta_effectus_derivare(piscina, optiones.intern, radix,
        titulus, tabula, &causa);
    CREDO_NON_NIHIL (sm);
    si (sm == NIHIL)
    {
        redde NIHIL;
    }
    summaria = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    *(StmlNodus**)xar_addere(summaria) = sm;
    d = crusta_effectus_diagnostica(piscina, titulus, contentum,
        (i32)strlen(contentum), summaria, in_catena, &optiones, &causa);
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

s32 principale (vacuum)
{
        InternamentumChorda* intern;
                  character  basis[CCLVI];
                  character  via[DXII];
         constans character* causa = NIHIL;
                        Xar* d;
                        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_effectus_lintrum",
        (memoriae_index)XVI * M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    r       = getenv("RHUBARB_RADIX");
    si (r == NIHIL)
    {
        r = ".";
    }
    sprintf(basis, "%s/crusta/build/effectus_lintrum.%ld", r,
        (longus)getpid());
    (vacuum)filum_arborem_delere(basis);
    sprintf(radix, "%s/arbor", basis);
    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.intern = intern;
    sprintf(via, "%s/crusta/lintrum/effectus", r);
    optiones.regulae = crusta_regulae_legere(piscina, via, intern,
        &causa);
    CREDO_NON_NIHIL (optiones.regulae);
    CREDO_VERUM (optiones.regulae != NIHIL
        && xar_numerus(optiones.regulae) == III);
    sprintf(via, "%s/crusta/effectus_mandata.stml", r);
    {
        StmlResultus t = stml_legere(filum_legere_totum(via, piscina),
            piscina, intern);

        CREDO_VERUM (t.successus);
        tabula = t.elementum_radix;
    }
    si (optiones.regulae == NIHIL || tabula == NIHIL)
    {
        credo_imprimere_compendium();
        credo_claudere();
        redde I;
    }

    imprimere("\n--- I. effectus-irresolutum ---\n");
    d = _scriptum_iudicare("a.sh", "f () {\n    cat \"$1/x\"\n}\n",
        VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, ERRATUM), I);
    /* extra catenam irresolutum TACET (Fran 2026-10-05, Q8 emendata:
     * clavem solam tangit; census situm servat) */
    d = _scriptum_iudicare("a.sh", "f () {\n    cat \"$1/x\"\n}\n",
        FALSUM);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, -I), ZEPHYRUM);
    d = _scriptum_iudicare("b.sh", "cat data/x\n", VERUM);
    CREDO_AEQUALIS_I32 (_numerare(d, NIHIL, -I), ZEPHYRUM);
    /* scriptura irresoluta consulto non flagratur */
    d = _scriptum_iudicare("c.sh", "echo x > \"$NESCIO/y\"\n", VERUM);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, -I), ZEPHYRUM);

    imprimere("\n--- II. effectus-build-sine-domino ---\n");
    d = _scriptum_iudicare("d.sh", "cat build/x\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, SINE_DOMINO, MONITUM), I);
    /* extra catenam regulae ceterae monent (non tacent) */
    d = _scriptum_iudicare("d.sh", "cat build/x\n", FALSUM);
    CREDO_AEQUALIS_I32 (_numerare(d, SINE_DOMINO, MONITUM), I);
    d = _scriptum_iudicare("e.sh", "mkdir -p build\necho y > build/x\n"
        "cat build/x\n", VERUM);
    CREDO_AEQUALIS_I32 (_numerare(d, SINE_DOMINO, -I), ZEPHYRUM);

    imprimere("\n--- III. effectus-mandatum-ignotum ---\n");
    d = _scriptum_iudicare("f.sh", "jqx a\neval \"$X\"\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, IGNOTUM, ERRATUM), II);
    d = _scriptum_iudicare("g.sh", "cat a >/dev/null\nhead -1 a\n",
        VERUM);
    CREDO_AEQUALIS_I32 (_numerare(d, IGNOTUM, -I), ZEPHYRUM);

    imprimere("\n--- IV. excusatio super functione (RF 2) ---\n");
    d = _scriptum_iudicare("h.sh",
        "# <tolera codex=\"lint:effectus-irresolutum\" (>sera\n"
        "f () {\n"
        "    cat \"$1/a\"\n"
        "    cat \"$1/b\"\n"
        "    [ -f \"$1/c\" ]\n"
        "}\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, NIHIL, -I), ZEPHYRUM);
    /* excusatio intra functionem super lectione PRIMA: secunda manet */
    d = _scriptum_iudicare("i.sh",
        "f () {\n"
        "    # <tolera codex=\"lint:effectus-irresolutum\" (>sera\n"
        "    cat \"$1/a\"\n"
        "    cat \"$1/b\"\n"
        "}\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, -I), I);
    /* excusatio super linea sine situ: MORTUA nominatur */
    d = _scriptum_iudicare("j.sh",
        "# <tolera codex=\"lint:effectus-irresolutum\" (>nihil tegit\n"
        "cat data/x\n", VERUM);
    _imprimere(d);
    CREDO_AEQUALIS_I32 (_numerare(d, IRRESOLUTUM, -I), ZEPHYRUM);
    CREDO_MAIOR_I32 (_numerare(d, NIHIL, -I), ZEPHYRUM);

    imprimere("\n--- V. catenae ex declarationibus fabricae ---\n");
    {
        Xar* catenae;

        _scribere("fabrica.stml", "<fabrica titulus=\"x\">"
            "<subsystema via=\"s\"/></fabrica>\n");
        _scribere("s/aedificatio.stml",
            "<aedificatio><actio titulus=\"porta_x\" "
            "genus=\"iudicium\">"
            "<ingressus genus=\"fontationes\" via=\"s/r.sh\"/>"
            "<ingressus genus=\"effectus\" via=\"s/q.sh\"/>"
            "<ingressus genus=\"fasciculus\" via=\"s/f.sh\"/>"
            "<exitus via=\"v.txt\" provenientia=\"verdictum\"/>"
            "</actio></aedificatio>\n");
        catenae = crusta_effectus_catenae(piscina, intern, radix,
            &causa);
        CREDO_NON_NIHIL (catenae);
        /* 'fontationes' (retiratum T8) et 'fasciculus' radices non sunt */
        CREDO_AEQUALIS_I32 (catenae ? xar_numerus(catenae) : ZEPHYRUM,
            I);
        si (catenae != NIHIL && xar_numerus(catenae) == I)
        {
            CREDO_VERUM (strcmp(*(character**)xar_obtinere(catenae,
                ZEPHYRUM), "s/q.sh") == ZEPHYRUM);
        }
    }

    (vacuum)filum_arborem_delere(basis);
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
