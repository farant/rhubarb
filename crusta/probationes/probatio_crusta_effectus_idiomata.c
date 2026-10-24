/* probatio_crusta_effectus_idiomata.c - idiomata aestimatoris in
 * summario effectuum (effectus-plan T8)
 *
 * Translatio probatio_crusta_fontationes (fabrica spec 3 T2) post
 * fontationes retiratas: idem arbor ficta, idem scriptum CURSOR, eaedem
 * regulae - nunc super situs summarii (fontatio, exsecutio, processus)
 * asserta. Quaeque assertio CONTRARIUM fert ubi regula errare potest:
 * ambitus exsecutus variabiles vocantis NON videt (z.sh irresolutum
 * quamvis exstet); 'X=1 cmd' RADIX_DIR non inficit; definitio vacua
 * intra verbum longius NON omittitur (p.sh irresolutum).
 */

#include "postulata_posix.h"

#include "latina.h"
#include "credo.h"
#include "crusta_effectus.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* MUNDUS: arbor ficta in area contextus (credo_area) et summarium
 * derivatum. Sectiones quaeque suum aedificant (summarium StmlNodus*
 * adiutoribus non constantibus traditur - fixura esse non potest);
 * creatio semel ASSERITUR in parare_suitae. */
nomen structura {
              character  basis[DXII];
              character  radix[DXII];
              character  externum[DXII];
              StmlNodus* tabula;
              StmlNodus* summarium;
     constans character* causa;
} Mundus;

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

interior b32
_attributum (
              StmlNodus* nodus,
     constans character* titulus,
     constans character* valor)
{
    chorda* v;

    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    v = stml_attributum_capere(nodus, titulus);
    si (valor == NIHIL)
    {
        redde v == NIHIL;
    }
    redde v != NIHIL && chorda_aequalis_literis(*v, valor);
}

/* situs primus elementi et viae in processu (NIHIL = quivis) */
interior StmlNodus*
_situs (
              StmlNodus* summarium,
     constans character* processus_radix,
     constans character* elementum,
     constans character* via)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; summarium->liberi
                       && i < xar_numerus(summarium->liberi); i++)
    {
        StmlNodus* p = *(StmlNodus**)xar_obtinere(summarium->liberi, i);

        si (   p->genus != STML_NODUS_ELEMENTUM
            || (processus_radix != NIHIL
                && !_attributum(p, "radix", processus_radix)))
        {
            perge;
        }
        per (j = ZEPHYRUM; p->liberi && j < xar_numerus(p->liberi); j++)
        {
            StmlNodus* s = *(StmlNodus**)xar_obtinere(p->liberi, j);

            si (   s->genus == STML_NODUS_ELEMENTUM
                && chorda_aequalis_literis(*s->titulus, elementum)
                && (via == NIHIL || _attributum(s, "via", via)))
            {
                redde s;
            }
        }
    }
    redde NIHIL;
}

/* situs ignotum primus causa data */
interior StmlNodus*
_ignotum_causae (
              StmlNodus* summarium,
     constans character* causa)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; summarium->liberi
                       && i < xar_numerus(summarium->liberi); i++)
    {
        StmlNodus* p = *(StmlNodus**)xar_obtinere(summarium->liberi, i);

        per (j = ZEPHYRUM; p->liberi && j < xar_numerus(p->liberi); j++)
        {
            StmlNodus* s = *(StmlNodus**)xar_obtinere(p->liberi, j);

            si (   s->genus == STML_NODUS_ELEMENTUM
                && chorda_aequalis_literis(*s->titulus, "ignotum")
                && _attributum(s, "causa", causa))
            {
                redde s;
            }
        }
    }
    redde NIHIL;
}

interior b32
_processus_est (
              StmlNodus* summarium,
     constans character* processus_radix)
{
    i32 i;

    per (i = ZEPHYRUM; summarium->liberi
                       && i < xar_numerus(summarium->liberi); i++)
    {
        StmlNodus* p = *(StmlNodus**)xar_obtinere(summarium->liberi, i);

        si (   p->genus == STML_NODUS_ELEMENTUM
            && _attributum(p, "radix", processus_radix))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* fontationes et exsecutiones irresolutae (et plagulae non sanae) */
interior i32
_irresoluta_numerare (
    StmlNodus* summarium)
{
    i32 i;
    i32 j;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; summarium->liberi
                       && i < xar_numerus(summarium->liberi); i++)
    {
        StmlNodus* p = *(StmlNodus**)xar_obtinere(summarium->liberi, i);

        per (j = ZEPHYRUM; p->liberi && j < xar_numerus(p->liberi); j++)
        {
            StmlNodus* s = *(StmlNodus**)xar_obtinere(p->liberi, j);

            si (s->genus != STML_NODUS_ELEMENTUM)
            {
                perge;
            }
            si (   (chorda_aequalis_literis(*s->titulus, "fontatio")
                    || chorda_aequalis_literis(*s->titulus,
                    "exsecutio"))
                && _attributum(s, "resolutio", "nulla"))
            {
                n++;
            }
            si (   chorda_aequalis_literis(*s->titulus, "ignotum")
                && _attributum(s, "causa", "parsura non sana"))
            {
                n++;
            }
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

interior constans character*
_radix_viae (vacuum)
{
    constans character* radix_viae = getenv("RHUBARB_RADIX");

    redde radix_viae != NIHIL ? radix_viae : ".";
}

/* arborem fictam scribere et summarium derivare SINE assertione;
 * FALSUM = area absens aut derivatio fracta (mundus->causa) */
interior b32
_mundum_parare (
    CredoContextus* c,
            Mundus* mundus)
{
                  Piscina* piscina  = c->piscina;
      InternamentumChorda* intern   = internamentum_creare(piscina);
       constans character* area     = credo_area(c);
                character  via_tabulae[DXII];

    mundus->tabula     = NIHIL;
    mundus->summarium  = NIHIL;
    mundus->causa      = NIHIL;
    si (area == NIHIL)
    {
        redde FALSUM;
    }
    sprintf(mundus->basis, "%s", area);
    sprintf(mundus->radix, "%s/arbor", mundus->basis);
    _scribere(mundus, "a/cursor.sh", CURSOR);
    _scribere(mundus, "a/alia.sh", "#!/bin/bash\n:\n");
    _scribere(mundus, "a/alia2.sh", "#!/bin/bash\n:\n");
    _scribere(mundus, "tools/v.sh",
        "source \"$RADIX_DIR/tools/w.sh\"\n");
    _scribere(mundus, "tools/w.sh", ":\n");
    _scribere(mundus, "tools/e.sh",
        "#!/bin/bash\nsource \"$RADIX_DIR/tools/z.sh\"\n");
    _scribere(mundus, "tools/z.sh", ":\n");
    _scribere(mundus, "tools/q.sh", ":\n");
    _scribere(mundus, "tools/p.sh", ":\n");
    _scribere(mundus, "tools/rel.sh", "#!/bin/bash\n:\n");
    _scribere(mundus, "tools/e3.sh", "#!/bin/bash\n:\n");
    _scribere(mundus, "tools/e4.sh", "#!/bin/bash\n:\n");
    _scribere(mundus, "tools/e5.sh", "#!/bin/bash\n:\n");
    _scribere(mundus, "tools/l.sh", ":\n");
    _scribere(mundus, "tools/fracta.sh", "if then\n(\n");
    _scribere(mundus, "bin/inst", "\177ELF binarium\n");
    _scribere(mundus, "bin/inst2", "\177ELF binarium\n");
    sprintf(mundus->externum, "%s/extra.sh", mundus->basis);
    (vacuum)filum_scribere_literis(mundus->externum, ":\n");
    sprintf(via_tabulae, "%s/crusta/effectus_mandata.stml",
        _radix_viae());
    {
        StmlResultus t = stml_legere(filum_legere_totum(via_tabulae,
            piscina), piscina, intern);

        mundus->tabula = t.successus ? t.elementum_radix : NIHIL;
    }
    mundus->summarium = crusta_effectus_derivare(piscina, intern,
        mundus->radix, "a/cursor.sh", mundus->tabula, &mundus->causa);
    redde mundus->summarium != NIHIL;
}

/* creatio mundi semel asserta (ut olim ante sectiones), mundo proprio
 * et non communicato */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    Mundus mundus;

    (vacuum)_mundum_parare(c, &mundus);
    CREDO_NON_NIHIL (mundus.tabula);
    CREDO_NON_NIHIL (mundus.summarium);
    si (mundus.summarium == NIHIL)
    {
        imprimere("FRACTA: derivatio: %s\n",
            mundus.causa ? mundus.causa : "-");
    }
}

interior vacuum
_idiomata_probare (
    CredoContextus* c)
{
       Mundus  mundus;
    StmlNodus* s;

    si (!_mundum_parare(c, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    CREDO_VERUM (_processus_est(mundus.summarium, "a/cursor.sh"));
    s = _situs(mundus.summarium, "a/cursor.sh", "fontatio",
        "tools/v.sh");
    CREDO_VERUM (_attributum(s, "resolutio", "plena"));
    CREDO_VERUM (_attributum(s, "sedes", NIHIL) == FALSUM);
    /* w.sh: v.sh RADIX_DIR vocantis videt (ambitus communis) */
    CREDO_NON_NIHIL (_situs(mundus.summarium, "a/cursor.sh", "fontatio",
        "tools/w.sh"));
    /* intra 'if': superaestimatio */
    CREDO_NON_NIHIL (_situs(mundus.summarium, NIHIL, "fontatio",
        "a/alia.sh"));
    /* $(dirname "$0") et $(readlink -f ...) */
    CREDO_NON_NIHIL (_situs(mundus.summarium, NIHIL, "fontatio",
        "a/alia2.sh"));
    CREDO_NON_NIHIL (_situs(mundus.summarium, NIHIL, "fontatio",
        "tools/l.sh"));
    /* globalis q extra g: q.sh */
    CREDO_NON_NIHIL (_situs(mundus.summarium, NIHIL, "fontatio",
        "tools/q.sh"));
}

interior vacuum
_exsecuta_probare (
    CredoContextus* c)
{
       Mundus  mundus;
    StmlNodus* s;

    si (!_mundum_parare(c, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    CREDO_NON_NIHIL (_situs(mundus.summarium, "a/cursor.sh",
        "exsecutio",
        "tools/e.sh"));
    CREDO_VERUM (_processus_est(mundus.summarium, "tools/e.sh"));
    /* e.sh ambitum NOVUM habet: RADIX_DIR vocantis non videt */
    CREDO_NIHIL (_situs(mundus.summarium, NIHIL, "fontatio",
        "tools/z.sh"));
    s = _situs(mundus.summarium, "tools/e.sh", "fontatio",
        "&quot;$RADIX_DIR/tools/z.sh&quot;");
    CREDO_VERUM (_attributum(s, "resolutio", "nulla"));
    CREDO_VERUM (_attributum(s, "plagula", "tools/e.sh"));
    CREDO_NON_NIHIL (_situs(mundus.summarium, NIHIL, "exsecutio",
        "tools/rel.sh"));
    /* exec, env, bash -x */
    CREDO_VERUM (_processus_est(mundus.summarium, "tools/e3.sh"));
    CREDO_VERUM (_processus_est(mundus.summarium, "tools/e4.sh"));
    CREDO_VERUM (_processus_est(mundus.summarium, "tools/e5.sh"));
    CREDO_NIHIL (_situs(mundus.summarium, NIHIL, "exsecutio",
        "nihil.sh"));
    s = _situs(mundus.summarium, NIHIL, "exsecutio", "bin/inst");
    CREDO_VERUM (_attributum(s, "classis", "instrumentum_domus"));
    s = _situs(mundus.summarium, NIHIL, "exsecutio", "bin/absens");
    CREDO_VERUM (_attributum(s, "classis", "instrumentum_domus"));
    CREDO_NON_NIHIL (_situs(mundus.summarium, NIHIL, "exsecutio",
        "bin/inst2"));
    /* ansa 'for nomen in a b' (effectus-plan-2 T5): membra duo
     * resoluta, non praefixum a/build/ */
    s = _situs(mundus.summarium, NIHIL, "exsecutio", "a/build/a");
    CREDO_VERUM (_attributum(s, "resolutio", "plena"));
    CREDO_VERUM (_attributum(s, "classis", "build"));
    CREDO_NON_NIHIL (_situs(mundus.summarium, NIHIL, "exsecutio",
        "a/build/b"));
}

interior vacuum
_irresoluta_probare (
    CredoContextus* c)
{
       Mundus  mundus;
    StmlNodus* s;

    si (!_mundum_parare(c, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    s = _situs(mundus.summarium, NIHIL, "fontatio",
        "&quot;$NESCIO&quot;");
    CREDO_VERUM (_attributum(s, "resolutio", "nulla"));
    /* local q="$1" in g: definitio globalis non valet */
    s = _situs(mundus.summarium, NIHIL, "fontatio", "&quot;$q&quot;");
    CREDO_VERUM (_attributum(s, "resolutio", "nulla"));
    /* P="" deinde P="$RADIX_DIR": ordo (effectus-plan-3 T2)
     * assignationem posteriorem solam attingere scit - resolutum (ante
     * slice 3 unio cum "" irresolutum dabat) */
    s = _situs(mundus.summarium, NIHIL, "fontatio", "tools/p.sh");
    CREDO_VERUM (_attributum(s, "resolutio", "plena"));
    /* ${NESCIO:-...} (effectus-plan-2 T6, A4): NESCIO in ambitu non
     * definita -> valor praedefinitus resolutus; clavis lineam
     * 'ambitus NESCIO' per ambitus_lectio tenet */
    s = _situs(mundus.summarium, NIHIL, "fontatio", "tools/v.sh");
    CREDO_VERUM (_attributum(s, "resolutio", "plena"));
    s = _ignotum_causae(mundus.summarium, "parsura non sana");
    CREDO_VERUM (_attributum(s, "plagula", "tools/fracta.sh"));
    s = _situs(mundus.summarium, NIHIL, "fontatio", mundus.externum);
    CREDO_VERUM (_attributum(s, "classis", "externa"));
    /* 'RADIX_DIR=/alibi true' non est definitio: aliter omnia supra
     * irresoluta essent - numerus irresolutorum exactus */
    CREDO_AEQUALIS_I32 (_irresoluta_numerare(mundus.summarium), IV);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. idiomata et ambitus communis",
        _idiomata_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "II. exsecuta: ambitus novus, involucra, binaria",
        _exsecuta_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "III. irresoluta (et quod NON irresolutum)",
        _irresoluta_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("crusta_effectus_idiomata",
        SECTIONES, _suitam_parare, NIHIL);
}
