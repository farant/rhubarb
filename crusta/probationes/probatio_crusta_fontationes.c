/* probatio_crusta_fontationes.c - fontationes statice derivatae
 * (fabrica spec 3 par. III.2, T2)
 *
 * Arbor ficta sub crusta/build/: scriptum unum (a/cursor.sh)
 * omnes regulas crusta_fontationes.h tangit. Quaeque assertio
 * CONTRARIUM quoque fert ubi regula errare potest: ambitus exsecutus
 * variabiles vocantis NON videt (z.sh irresolutum, quamvis exstet);
 * 'X=1 cmd' RADIX_DIR non inficit (aliter omnia irresoluta); definitio
 * vacua intra verbum longius NON omittitur (p.sh irresolutum).
 */

#include "postulata_posix.h"

#include "latina.h"
#include "credo.h"
#include "crusta_fontationes.h"
#include "filum.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

hic_manens character basis[CCLVI];
hic_manens character radix[CCLVI];

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

interior constans CrustaFontatio*
_invenire (
                    Xar* f,
    CrustaFontatioGenus  genus,
     constans character* via)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(f); k++)
    {
        constans CrustaFontatio* e = (constans CrustaFontatio*)
            xar_obtinere(f, k);

        si (e->genus == genus && strcmp(e->via, via) == ZEPHYRUM)
        {
            redde e;
        }
    }
    redde NIHIL;
}

interior i32
_numerare (
                    Xar* f,
    CrustaFontatioGenus  genus)
{
    i32 k;
    i32 n = ZEPHYRUM;

    per (k = ZEPHYRUM; k < xar_numerus(f); k++)
    {
        si (((constans CrustaFontatio*)xar_obtinere(f, k))->genus
            == genus)
        {
            n++;
        }
    }
    redde n;
}

hic_manens constans character* CURSOR =
    "#!/bin/bash\n"                                                /* 1 */
    "X_DIR=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")\" && pwd)\"\n" /* 2 */
    "RADIX_DIR=\"$(cd \"$X_DIR/..\" && pwd)\"\n"                    /* 3 */
    "source \"$RADIX_DIR/tools/v.sh\"\n"                            /* 4 */
    "if [ -n \"$Y\" ]; then\n"                                      /* 5 */
    "    . \"$X_DIR/alia.sh\"\n"                                    /* 6 */
    "fi\n"                                                          /* 7 */
    "source \"$NESCIO\"\n"                                          /* 8 */
    "\"$RADIX_DIR/tools/e.sh\" >&2 || exit 1\n"                     /* 9 */
    "\"$RADIX_DIR/bin/inst\" -x\n"                                  /* 10 */
    "\"$RADIX_DIR/bin/absens\"\n"                                   /* 11 */
    "for nomen in a b; do\n"                                        /* 12 */
    "    bin=\"$X_DIR/build/$nomen\"\n"                             /* 13 */
    "    \"$bin\"\n"                                                /* 14 */
    "done\n"                                                        /* 15 */
    "q=\"$RADIX_DIR/tools/q.sh\"\n"                                 /* 16 */
    "g () {\n"                                                      /* 17 */
    "    local q=\"$1\"\n"                                          /* 18 */
    "    source \"$q\"\n"                                           /* 19 */
    "}\n"                                                           /* 20 */
    "source \"$q\"\n"                                               /* 21 */
    "M=\"\"\n"                                                      /* 22 */
    "M=\"$RADIX_DIR/bin/inst\"\n"                                   /* 23 */
    "\"$M\"\n"                                                      /* 24 */
    "P=\"\"\n"                                                      /* 25 */
    "P=\"$RADIX_DIR\"\n"                                            /* 26 */
    "source \"$P/tools/p.sh\"\n"                                    /* 27 */
    "(cd \"$RADIX_DIR\" && bin/inst2)\n"                            /* 28 */
    "tools/rel.sh\n"                                                /* 29 */
    "RADIX_DIR=/alibi true\n"                                       /* 30 */
    "source \"${NESCIO:-$RADIX_DIR/tools/v.sh}\"\n"                 /* 31 */
    "source \"$RADIX_DIR/../extra.sh\"\n"                           /* 32 */
    "source \"$RADIX_DIR/tools/fracta.sh\"\n"                       /* 33 */
    "exec \"$RADIX_DIR/tools/e3.sh\"\n"                             /* 34 */
    "env A=1 \"$RADIX_DIR/tools/e4.sh\"\n"                          /* 35 */
    "bash -x \"$RADIX_DIR/tools/e5.sh\"\n"                          /* 36 */
    "bash -c \"source nihil.sh\"\n"                                 /* 37 */
    "command -v nihil\n"                                            /* 38 */
    "D2=\"$(dirname \"$0\")\"\n"                                    /* 39 */
    "source \"$D2/alia2.sh\"\n"                                     /* 40 */
    "L=\"$(readlink -f \"$X_DIR/../tools\")\"\n"                    /* 41 */
    "source \"$L/l.sh\"\n";                                         /* 42 */

s32 principale (vacuum)
{
                    Piscina* piscina;
                        Xar* f;
         constans character* causa = NIHIL;
    constans CrustaFontatio* e;
                  character  externum[DXII];
                        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_crusta_fontationes",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    {
        constans character* r = getenv("RHUBARB_RADIX");

        /* arbor ficta sub crusta/build/ (radix ficta: viae ei relativae
         * '/build/' non continent) - per processum unica */
        sprintf(basis, "%s/crusta/build/fontationes_fixa.%ld",
            r != NIHIL ? r : ".", (longus)getpid());
    }
    (vacuum)filum_arborem_delere(basis);
    sprintf(radix, "%s/arbor", basis);
    _scribere("a/cursor.sh", CURSOR);
    _scribere("a/alia.sh", "#!/bin/bash\n:\n");
    _scribere("a/alia2.sh", "#!/bin/bash\n:\n");
    _scribere("tools/v.sh", "source \"$RADIX_DIR/tools/w.sh\"\n");
    _scribere("tools/w.sh", ":\n");
    _scribere("tools/e.sh",
        "#!/bin/bash\nsource \"$RADIX_DIR/tools/z.sh\"\n");
    _scribere("tools/z.sh", ":\n");
    _scribere("tools/q.sh", ":\n");
    _scribere("tools/p.sh", ":\n");
    _scribere("tools/rel.sh", "#!/bin/bash\n:\n");
    _scribere("tools/e3.sh", "#!/bin/bash\n:\n");
    _scribere("tools/e4.sh", "#!/bin/bash\n:\n");
    _scribere("tools/e5.sh", "#!/bin/bash\n:\n");
    _scribere("tools/l.sh", ":\n");
    _scribere("tools/fracta.sh", "if then\n(\n");
    _scribere("bin/inst", "\177ELF binarium\n");
    _scribere("bin/inst2", "\177ELF binarium\n");
    sprintf(externum, "%s/extra.sh", basis);
    (vacuum)filum_scribere_literis(externum, ":\n");

    f = crusta_fontationes_derivare(piscina, radix, "a/cursor.sh",
        &causa);
    CREDO_NON_NIHIL (f);
    si (f == NIHIL)
    {
        imprimere("FRACTA: derivatio: %s\n", causa ? causa : "-");
        redde I;
    }


    /* ========================================================
     * PROBARE: idiomata, fontationes, ambitus communis
     * ======================================================== */

    imprimere("\n--- Probans idiomata et ambitum communem ---\n");
    e = _invenire(f, CRUSTA_FONTATIO_FASCICULUS, "a/cursor.sh");
    CREDO_NON_NIHIL (e);                    /* radix ipsa */
    e = _invenire(f, CRUSTA_FONTATIO_FASCICULUS, "tools/v.sh");
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_VERUM (e->fontatum);
        CREDO_AEQUALIS_I32 (e->linea, IV);
    }
    /* w.sh: v.sh RADIX_DIR vocantis videt (ambitus communis) */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "tools/w.sh"));
    /* intra 'if': superaestimatio */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "a/alia.sh"));
    /* $(dirname "$0") et $(readlink -f ...) */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "a/alia2.sh"));
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "tools/l.sh"));
    /* globalis q extra g: q.sh */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "tools/q.sh"));


    /* ========================================================
     * PROBARE: exsecuta - ambitus novus, involucra, binaria
     * ======================================================== */

    imprimere("\n--- Probans exsecuta ---\n");
    e = _invenire(f, CRUSTA_FONTATIO_FASCICULUS, "tools/e.sh");
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_FALSUM (e->fontatum);
    }
    /* e.sh ambitum NOVUM habet: RADIX_DIR vocantis non videt */
    CREDO_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "tools/z.sh"));
    e = _invenire(f, CRUSTA_FONTATIO_IRRESOLUTUM,
        "\"$RADIX_DIR/tools/z.sh\"");
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(e->plagula,
            piscina), "tools/e.sh");
    }
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "tools/rel.sh"));                  /* relativa: cwd = radix */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "tools/e3.sh"));                   /* exec */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "tools/e4.sh"));                   /* env A=1 */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "tools/e5.sh"));                   /* bash -x */
    CREDO_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS, "nihil.sh"));
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_INSTRUMENTUM,
        "bin/inst"));
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_INSTRUMENTUM,
        "bin/absens"));                    /* sub bin/ nondum structum */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_INSTRUMENTUM,
        "bin/inst2"));                     /* (cd X && bin/inst2) */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_PRODUCTUM,
        "a/build/*"));


    /* ========================================================
     * PROBARE: irresoluta (et quod NON irresolutum)
     * ======================================================== */

    imprimere("\n--- Probans irresoluta ---\n");
    e = _invenire(f, CRUSTA_FONTATIO_IRRESOLUTUM, "\"$NESCIO\"");
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_AEQUALIS_I32 (e->linea, VIII);
        CREDO_VERUM (e->fontatum);
    }
    /* local q="$1" in g: definitio globalis non valet */
    e = _invenire(f, CRUSTA_FONTATIO_IRRESOLUTUM, "\"$q\"");
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_AEQUALIS_I32 (e->linea, XIX);
    }
    /* '"$M"' solum: M="" omittitur -> instrumentum (supra); '"$P/..."':
     * P="" viam mutaret -> irresolutum */
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_IRRESOLUTUM,
        "\"$P/tools/p.sh\""));
    CREDO_NIHIL (_invenire(f, CRUSTA_FONTATIO_FASCICULUS,
        "tools/p.sh"));
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_IRRESOLUTUM,
        "\"${NESCIO:-$RADIX_DIR/tools/v.sh}\""));
    e = _invenire(f, CRUSTA_FONTATIO_IRRESOLUTUM, "parsura non sana");
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(e->plagula,
            piscina), "tools/fracta.sh");
    }
    CREDO_NON_NIHIL (_invenire(f, CRUSTA_FONTATIO_EXTERNUM, externum));
    /* 'RADIX_DIR=/alibi true' non est definitio: alias omnia supra
     * irresoluta essent - numerus irresolutorum exactus */
    CREDO_AEQUALIS_I32 (_numerare(f, CRUSTA_FONTATIO_IRRESOLUTUM), VI);

    (vacuum)filum_arborem_delere(basis);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
