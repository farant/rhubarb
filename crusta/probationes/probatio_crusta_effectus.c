/* probatio_crusta_effectus.c - summarium effectuum (effectus-plan.md
 * T3; spec par. IV)
 *
 * Arbor ficta sub crusta/build/ (idioma
 * probatio_crusta_effectus_idiomata): scriptum unum (a/r.sh) classem
 * omnem situum spec par. IV.2 tangit, linea per classem. Quaeque
 * assertio CONTRARIUM quoque fert ubi analysis errare potest: globus
 * citatus non enumeratur; /dev/null et '2>&1' non scribuntur; '<<<' non
 * legitur; exemplar grep/sed situs non est; functio domus mandatum
 * ignotum non est; '$?' '$1' et variabiles a 'read' assignatae ambitus
 * non leguntur; scriptum EXSECUTUM variabiles vocantis non videt.
 *
 * Deinde domus tota: omne .sh arboris summatur (nullum NIHIL) et
 * summarium omne contra effectus.canon sanum est - custos derivae:
 * elementum aut valorem quem analysis fingit canon hic recusat.
 */

#include "postulata_posix.h"

#include "latina.h"
#include "canon.h"
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

/* attributum aequat valorem? (NIHIL valor = attributum abest) */
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

/* linea sedis: 'L:...' */
interior b32
_in_linea (
     StmlNodus* nodus,
           i32  linea)
{
       chorda* s = NIHIL;
    character  prima[XXXII];

    si (nodus != NIHIL)
    {
        s = stml_attributum_capere(nodus, "sedes");
    }

    si (s == NIHIL)
    {
        redde FALSUM;
    }
    sprintf(prima, "%u:", (insignatus integer)linea);
    redde s->mensura > (i32)strlen(prima)
        && memcmp(s->datum, prima, strlen(prima)) == ZEPHYRUM;
}

/* situs omnes (elementa sub processus) in Xar planum */
interior vacuum
_situs_colligere (
             StmlNodus* radix_summarii,
                   Xar* situs,
    constans character* processus_radix)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; radix_summarii->liberi
                       && i < xar_numerus(radix_summarii->liberi); i++)
    {
        StmlNodus* p = *(StmlNodus**)xar_obtinere(
            radix_summarii->liberi, i);

        si (   p->genus != STML_NODUS_ELEMENTUM
            || !chorda_aequalis_literis(*p->titulus, "processus"))
        {
            perge;
        }
        si (   processus_radix != NIHIL
            && !_attributum(p, "radix", processus_radix))
        {
            perge;
        }
        per (j = ZEPHYRUM; p->liberi && j < xar_numerus(p->liberi); j++)
        {
            StmlNodus* s = *(StmlNodus**)xar_obtinere(p->liberi, j);

            si (s->genus == STML_NODUS_ELEMENTUM)
            {
                *(StmlNodus**)xar_addere(situs) = s;
            }
        }
    }
}

/* processus 'radix' fert custodiam datam (NIHIL = sine custodia)? */
interior b32
_processus_custodia (
              StmlNodus* summarium,
     constans character* radix_processus,
     constans character* custodia)
{
    i32 i;

    per (i = ZEPHYRUM; summarium->liberi
                       && i < xar_numerus(summarium->liberi); i++)
    {
        StmlNodus* p = *(StmlNodus**)xar_obtinere(summarium->liberi, i);

        si (   p->genus == STML_NODUS_ELEMENTUM
            && _attributum(p, "radix", radix_processus))
        {
            redde _attributum(p, "custodia", custodia);
        }
    }
    imprimere("    processus absens: %s\n", radix_processus);
    redde FALSUM;
}

/* situs primus generis, viae et lineae (NIHIL / 0 = quaevis) */
interior StmlNodus*
_situs (
                   Xar* situs,
    constans character* elementum,
    constans character* via,
                   i32  linea)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(situs); k++)
    {
        StmlNodus* s = *(StmlNodus**)xar_obtinere(situs, k);

        si (!chorda_aequalis_literis(*s->titulus, elementum))
        {
            perge;
        }
        si (   via != NIHIL && !_attributum(s, "via", via)
            && !_attributum(s, "titulus", via))
        {
            perge;
        }
        si (linea != ZEPHYRUM && !_in_linea(s, linea))
        {
            perge;
        }
        redde s;
    }
    redde NIHIL;
}

/* situs numerare: elementum NIHIL = quodvis */
interior i32
_numerare (
                   Xar* situs,
    constans character* elementum,
                   i32  linea)
{
    i32 k;
    i32 n = ZEPHYRUM;

    per (k = ZEPHYRUM; k < xar_numerus(situs); k++)
    {
        StmlNodus* s = *(StmlNodus**)xar_obtinere(situs, k);

        si (   elementum != NIHIL
            && !chorda_aequalis_literis(*s->titulus, elementum))
        {
            perge;
        }
        si (linea != ZEPHYRUM && !_in_linea(s, linea))
        {
            perge;
        }
        n++;
    }
    redde n;
}

interior vacuum
_imprimere_situs (
    Xar* situs)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(situs); k++)
    {
        StmlNodus* s = *(StmlNodus**)xar_obtinere(situs, k);
           chorda* v = stml_attributum_capere(s, "via");
           chorda* t = stml_attributum_capere(s, "titulus");
           chorda* l = stml_attributum_capere(s, "sedes");

        imprimere("    %.*s %.*s @%.*s\n", (integer)s->titulus->mensura,
            (constans character*)s->titulus->datum,
            v ? (integer)v->mensura : (t ? (integer)t->mensura : 0),
            v ? (constans character*)v->datum
              : (t ? (constans character*)t->datum : ""),
            l ? (integer)l->mensura : 0,
            l ? (constans character*)l->datum : "");
    }
}

hic_manens constans character* RADIX_SCRIPTI =
    "#!/bin/bash\n" /* 1 */
    /* 2 */
    "D=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")/..\" && pwd)\"\n"
    "source \"$D/a/lib.sh\"\n" /* 3 */
    "x=\"$(cat \"$D/data/f.txt\")\"\n" /* 4 */
    "mapfile -t L < \"$D/data/lista.txt\"\n" /* 5 */
    "[ -f build/gen.h ] && echo hi > build/out.txt\n" /* 6 */
    "for f in src/*.c; do :; done\n" /* 7 */
    "echo \"src/*.c\" > /dev/null 2>&1\n" /* 8 */
    "cat <<< \"nihil\"\n" /* 9 */
    "[[ -d src && build/a.o -nt src/a.c ]]\n" /* 10 */
    "sort -o build/s.txt data/f.txt\n" /* 11 */
    "sed -i '' 's/a/b/' data/g.txt\n" /* 12 */
    "grep -e pat data/f.txt\n" /* 13 */
    "grep pat2 data/h.txt\n" /* 14 */
    "lib_functio\n" /* 15 */
    "jqx foo\n" /* 16 */
    "eval \"$CMD\"\n" /* 17 */
    "echo \"$HOME\" \"$?\" \"$1\"\n" /* 18 */
    "X=1 true\n" /* 19 */
    "read -r R < data/r.txt\n" /* 20 */
    "echo \"$R\"\n" /* 21 */
    "cp data/f.txt build/copia.txt\n" /* 22 */
    "\"$D/a/puer.sh\"\n" /* 23 */
    "cd \"$NESCIO_DIR\" && cat x.txt\n" /* 24 */
    "(cd build && cat rel.txt)\n" /* 25 */
    "cat build/clausurae/*.lst\n" /* 26 */
    "echo x > \"build/clausurae/$n.lst\"\n" /* 27 */
    "while read -r l; do :; done < <(cat data/p.txt)\n"  /* 28 */
    "cd lib\n"                                            /* 29 */
    "cat q.txt\n"                                         /* 30 */
    "cd ..\n"                                             /* 31 */
    "[ -x bin/inst ] || ./a/struere.sh\n"                 /* 32 */
    "./a/communis.sh\n"                                   /* 33 */
    "bin/compilator -c src/a.c -o build/x.o\n";           /* 34 */

hic_manens constans character* LIB =
    "lib_functio () {\n"
    "    local v=\"$1\"\n"
    "    cat \"$D/data/lib.txt\"\n"
    "}\n";

/* aedificator custoditus: currit solum si bin/inst abest */
hic_manens constans character* STRUERE =
    "#!/bin/bash\n"
    "for o in $OBJS; do cat \"$o\"; done\n"
    "./a/nepos.sh\n";

/* nepos: per aedificatorem custoditum SOLUM attingitur */
hic_manens constans character* NEPOS =
    "#!/bin/bash\n"
    "cat data/nepos.txt\n";

/* communis: et custodite (ab struere? non) et libere attingitur */
hic_manens constans character* COMMUNIS =
    "#!/bin/bash\n"
    "cat data/communis.txt\n";

hic_manens constans character* PUER =
    "#!/bin/bash\n"
    "cat \"$D/data/puer.txt\"\n";

/* causae (effectus-plan-2 T1): situs irresolutus unus per causam,
 * linea per causam; numeri linearum in commentariis */
hic_manens constans character* CAUSAE =
    "#!/bin/bash\n"                                       /* 1 */
    "f () {\n"                                            /* 2 */
    "    cat \"$1/x\"\n"                                  /* 3 */
    "}\n"                                                 /* 4 */
    "while read -r l; do cat \"$l\"; done < data/r.txt\n" /* 5 */
    "cat \"$(git rev-parse x)\"\n"                        /* 6 */
    "cat \"$NESCIO/y\"\n"                                 /* 7 */
    "cat \"${X/a/b}\"\n"                                  /* 8 */
    "Y=a\n"                                               /* 9 */
    "Y=\"$(mktemp)\"\n"                                    /* 10 */
    "cat \"$Y\"\n"                                        /* 11 */
    "A=(x y)\n"                                           /* 12 */
    "cat \"$A\"\n"                                        /* 13 */
    "for g in p q; do cat \"$g\"; done\n"                 /* 14 */
    "cat \"$((I + II))\"\n"                               /* 15 */
    "cat \"$NESCIO/data/z\"\n"                            /* 16 */
    "O=\"\"\n"                                             /* 17 */
    "for o in a b; do O=\"$O $o\"; done\n"                 /* 18 */
    "cat $O\n";                                          /* 19 */

/* temporaria (effectus-plan-2 T3): viae sub directorio mktemp */
hic_manens constans character* TEMPORARIA =
    "#!/bin/bash\n"                                       /* 1 */
    "T=\"$(mktemp -d)\"\n"                                /* 2 */
    "echo x > \"$T/a\"\n"                                 /* 3 */
    "cat \"$T/a\"\n"                                      /* 4 */
    "R=\"$T/r\"\n"                                        /* 5 */
    "cat \"$R\"\n"                                        /* 6 */
    "F=$(mktemp)\n"                                       /* 7 */
    "echo y > \"$F\"\n"                                   /* 8 */
    "G=\"$(mktemp /tmp/probatio.XXXXXX)\"\n"              /* 9 */
    "cat \"$G\"\n"                                        /* 10 */
    "f () {\n"                                            /* 11 */
    "    local L\n"                                       /* 12 */
    "    L=\"$(mktemp -d)\"\n"                            /* 13 */
    "    cat \"$L/q\"\n"                                  /* 14 */
    "}\n"                                                 /* 15 */
    "X=/tmp/x\n"                                          /* 16 */
    "cat \"$X\"\n"                                        /* 17 */
    "Y=\"$(date)\"\n"                                     /* 18 */
    "cat \"$Y\"\n"                                        /* 19 */
    "cat \"$T/$NESCIO\"\n"                                /* 20 */
    "export T\n"                                          /* 21 */
    "./a/filius_t.sh\n";                                  /* 22 */

/* filius variabiles vocantis non videt: T ambitus, non temporaria */
hic_manens constans character* FILIUS_T =
    "#!/bin/bash\n"
    "cat \"$T/z\"\n";

/* tabulata (effectus-plan-2 T4): optiones per tabulatum non leguntur;
 * elementa quae viae sunt leguntur */
hic_manens constans character* TABULATA =
    "#!/bin/bash\n"                                       /* 1 */
    "source a/vexilla_ficta.sh\n"                         /* 2 */
    "declare -a F=(\"-std=c89\" \"-I$R/include\")\n"       /* 3 */
    "F+=(-O2)\n"                                          /* 4 */
    "clang \"${F[@]}\" -c src/a.c -o build/b.o\n"          /* 5 */
    "S=(lib/a.c lib/b.c)\n"                               /* 6 */
    "clang \"${S[@]}\" -o build/x\n"                       /* 7 */
    "G=(\"${VEXILLA_FICTA[@]}\")\n"                        /* 8 */
    "G+=(-o \"$OUT\")\n"                                   /* 9 */
    "clang \"${G[@]}\" src/c.c\n"                          /* 10 */
    "clang -I\"$R/inc\" src/d.c\n";                         /* 11 */

hic_manens constans character* VEXILLA_FICTA =
    "declare -a VEXILLA_FICTA=(\n"
    "    \"-pedantic\"\n"
    "    \"-Wall\"\n"
    ")\n";

/* ansae et exemplaria (effectus-plan-2 T5; spec-2 par. II, IV, V.1) */
hic_manens constans character* ANSAE =
    "#!/bin/bash\n"                                          /* 1 */
    "for f in toml css; do cat \"$f/x\"; done\n"              /* 2 */
    "for f in lib/*.c; do cat \"$f\"; done\n"                 /* 3 */
    "B=build/q\n"                                            /* 4 */
    "for f in lib/*.c; do o=\"$B/$(basename \"$f\" .c).o\"; "
        "cat \"$o\"; done\n"                                  /* 5 */
    "for f in lib/*.c; do cat \"${f%.c}.h\"; done\n"          /* 6 */
    "for f in lib/*.c; do cat \"${f##*/}\"; done\n"           /* 7 */
    "X=\"lib/*.c\"\n"                                        /* 8 */
    "cat \"$X\"\n"                                           /* 9 */
    "cat $X\n"                                               /* 10 */
    "for f in a b c d e f g h i j k l m n o p q; do "
        "cat \"d/$f\"; done\n"                                /* 11 */
    "for a in a1 a2 a3 a4 a5 a6 a7 a8 a9 a10 a11 a12 a13 a14 a15 "
        "a16; "
        "do for b in b1 b2 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 b13 b14 "
        "b15 b16; do "
        "cat \"e/$a/$b\"; done; done\n"                       /* 12 */
    "CF=\"-O2 -Wall\"\n"                                     /* 13 */
    "clang $CF src/m.c\n"                                    /* 14 */
    "for s in a/m1.sh a/m2.sh; do source \"$s\"; done\n"     /* 15 */
    "for c in u v; do cp -r \"src/$c\" build/; done\n";       /* 16 */

hic_manens constans character* M1 = "cat data/m1.txt\n";
hic_manens constans character* M2 = "cat data/m2.txt\n";

/* valores praedefiniti (effectus-plan-2 T6, A4) */
hic_manens constans character* PRAEDEFINITUM =
    "#!/bin/bash\n"                                       /* 1 */
    "cat \"${FS:-build/fs}/x\"\n"                          /* 2 */
    "G=a\n"                                               /* 3 */
    "cat \"${G:-build/g}/y\"\n"                            /* 4 */
    "cat \"${H-data/h}/z\"\n"                              /* 5 */
    "cat \"${1:-data/u}/w\"\n"                             /* 6 */
    "E=\"\"\n"                                            /* 7 */
    "cat \"${E:-data/e}/v\"\n";                            /* 8 */

/* subsumptio (effectus-plan-3 T1): summarium VETUS et NOVUM ficta */
hic_manens constans character* SUMMARIUM_VETUS =
    "<effectus lingua=\"bash\" radix=\"r.sh\">"
    "<processus radix=\"r.sh\">"
    "<lectio via=\"a/x\" forma=\"via\" resolutio=\"plena\" "
        "plagula=\"r.sh\" sedes=\"1:1-1:5\" octeti=\"0-4\"/>"
    "<lectio via=\"lib/*.c\" forma=\"globus\" resolutio=\"plena\" "
        "plagula=\"r.sh\" sedes=\"2:1-2:5\" octeti=\"6-10\"/>"
    "<lectio via=\"d/\" forma=\"praefixum\" resolutio=\"partialis\" "
        "plagula=\"r.sh\" sedes=\"3:1-3:5\" octeti=\"12-16\"/>"
    "<lectio via=\"$X\" forma=\"via\" resolutio=\"nulla\" "
        "plagula=\"r.sh\" sedes=\"4:1-4:5\" octeti=\"18-22\"/>"
    "</processus></effectus>";

/* NOVUM: membra quae VETUS tegit (aequalis, globus, praefixum,
 * irresolutum) */
hic_manens constans character* SUMMARIUM_BONUM =
    "<effectus lingua=\"bash\" radix=\"r.sh\">"
    "<processus radix=\"r.sh\">"
    "<lectio via=\"a/x\" forma=\"via\" resolutio=\"plena\" "
        "plagula=\"r.sh\" sedes=\"1:1-1:5\" octeti=\"0-4\"/>"
    "<lectio via=\"lib/a.c\" forma=\"via\" resolutio=\"plena\" "
        "plagula=\"r.sh\" sedes=\"2:1-2:5\" octeti=\"6-10\"/>"
    "<lectio via=\"d/q.txt\" forma=\"via\" resolutio=\"plena\" "
        "plagula=\"r.sh\" sedes=\"3:1-3:5\" octeti=\"12-16\"/>"
    "<lectio via=\"z\" forma=\"via\" resolutio=\"plena\" "
        "plagula=\"r.sh\" sedes=\"4:1-4:5\" octeti=\"18-22\"/>"
    "</processus></effectus>";

/* NOVUM: membrum extra valorem veterem (a/y), et regressio (praefixum
 * ubi vetus via exacta erat) - ambo nominantur */
hic_manens constans character* SUMMARIUM_MALUM =
    "<effectus lingua=\"bash\" radix=\"r.sh\">"
    "<processus radix=\"r.sh\">"
    "<lectio via=\"a/y\" forma=\"via\" resolutio=\"plena\" "
        "plagula=\"r.sh\" sedes=\"1:1-1:5\" octeti=\"0-4\"/>"
    "<lectio via=\"src/a.c\" forma=\"via\" resolutio=\"plena\" "
        "plagula=\"r.sh\" sedes=\"2:1-2:5\" octeti=\"6-10\"/>"
    "<lectio via=\"d/q.txt\" forma=\"via\" resolutio=\"plena\" "
        "plagula=\"r.sh\" sedes=\"3:1-3:5\" octeti=\"12-16\"/>"
    "<lectio via=\"a/\" forma=\"praefixum\" resolutio=\"partialis\" "
        "plagula=\"r.sh\" sedes=\"1:1-1:5\" octeti=\"0-4\"/>"
    "</processus></effectus>";

/* causa situs lectionis in linea (NIHIL = situs absens) */
interior b32
_causa_lineae (
                   Xar* situs,
                   i32  linea,
    constans character* causa)
{
    StmlNodus* s = _situs(situs, "lectio", NIHIL, linea);

    si (s == NIHIL)
    {
        imprimere("    linea %u: lectio absens\n",
            (insignatus integer)linea);
        redde FALSUM;
    }
    si (!_attributum(s, "causa", causa))
    {
        chorda* c = stml_attributum_capere(s, "causa");

        imprimere("    linea %u: causa '%.*s', exspectata '%s'\n",
            (insignatus integer)linea, c ? (integer)c->mensura : 0,
            c ? (constans character*)c->datum : "", causa);
        redde FALSUM;
    }
    redde VERUM;
}

s32 principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
               StmlNodus* summarium;
                     Xar* situs;
                     Xar* situs_puer;
      constans character* causa = NIHIL;
               StmlNodus* s;
                     b32  praeteritus;
      constans character* r;

    piscina = piscina_generare_dynamicum("probatio_crusta_effectus",
        (memoriae_index)LXIV * M * M);
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
    sprintf(basis, "%s/crusta/build/effectus_fixa.%ld", r,
        (longus)getpid());
    (vacuum)filum_arborem_delere(basis);
    sprintf(radix, "%s/arbor", basis);
    _scribere("a/r.sh", RADIX_SCRIPTI);
    _scribere("a/lib.sh", LIB);
    _scribere("a/puer.sh", PUER);
    _scribere("a/struere.sh", STRUERE);
    _scribere("a/nepos.sh", NEPOS);
    _scribere("a/communis.sh", COMMUNIS);
    _scribere("data/f.txt", "f\n");
    _scribere("src/a.c", "int a;\n");
    /* tabula mandatorum vera (arbor ficta eam non habet) */
    {
        character via_tabulae[DXII];

        sprintf(via_tabulae, "%s/crusta/effectus_mandata.stml", r);
        _scribere("crusta/effectus_mandata.stml",
            (constans character*)filum_legere_totum(via_tabulae,
                piscina).datum);
    }

    summarium = crusta_effectus_derivare(piscina, intern, radix,
        "a/r.sh", NIHIL, &causa);
    CREDO_NON_NIHIL (summarium);
    si (summarium == NIHIL)
    {
        imprimere("  CAUSA: %s\n", causa ? causa : "?");
        credo_imprimere_compendium();
        credo_claudere();
        redde I;
    }
    situs       = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    situs_puer  = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    _situs_colligere(summarium, situs, "a/r.sh");
    _situs_colligere(summarium, situs_puer, "a/puer.sh");
    imprimere("\n--- situs processus a/r.sh (%u) ---\n",
        (insignatus integer)xar_numerus(situs));
    _imprimere_situs(situs);

    imprimere("\n--- I. fontatio, lectio (mandatum, redirectio) ---\n");
    s = _situs(situs, "fontatio", "a/lib.sh", III);
    CREDO_VERUM (_attributum(s, "resolutio", "plena"));
    CREDO_VERUM (_attributum(s, "classis", "arbor"));
    CREDO_VERUM (_attributum(s, "sedes", "3:8-3:21"));
    CREDO_VERUM (_attributum(s, "octeti", "72-85"));
    s = _situs(situs, "lectio", "data/f.txt", IV);
    CREDO_VERUM (_attributum(s, "per", "mandatum"));
    CREDO_VERUM (_attributum(s, "mandatum", "cat"));
    CREDO_VERUM (_attributum(s, "classis", "arbor"));
    s = _situs(situs, "lectio", "data/lista.txt", V);
    CREDO_VERUM (_attributum(s, "per", "redirectio"));
    CREDO_VERUM (_attributum(s, "operator", "<"));
    /* lib.sh in ambitu radicis: $D resolvitur */
    s = _situs(situs, "lectio", "data/lib.txt", ZEPHYRUM);
    CREDO_VERUM (_attributum(s, "plagula", "a/lib.sh"));
    CREDO_VERUM (_attributum(s, "resolutio", "plena"));

    imprimere("\n--- II. probatio, scriptura, globus ---\n");
    s = _situs(situs, "probatio", "build/gen.h", VI);
    CREDO_VERUM (_attributum(s, "operator", "-f"));
    CREDO_VERUM (_attributum(s, "classis", "build"));
    s = _situs(situs, "scriptura", "build/out.txt", VI);
    CREDO_VERUM (_attributum(s, "per", "redirectio"));
    CREDO_VERUM (_attributum(s, "operator", ">"));
    /* contrarium: '>' scribit, non legit */
    CREDO_NIHIL (_situs(situs, "lectio", "build/out.txt", ZEPHYRUM));
    CREDO_AEQUALIS_I32 (_numerare(situs, NIHIL, VI), II);
    s = _situs(situs, "enumeratio", "src/", VII);
    CREDO_VERUM (_attributum(s, "forma", "globus"));
    CREDO_VERUM (_attributum(s, "exemplar", "*.c"));
    /* contraria: globus citatus, /dev/null, 2>&1, <<< */
    CREDO_AEQUALIS_I32 (_numerare(situs, NIHIL, VIII), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerare(situs, NIHIL, IX), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerare(situs, "enumeratio", ZEPHYRUM), II);

    imprimere("\n--- III. [[ ]] ---\n");
    s = _situs(situs, "probatio", "src", X);
    CREDO_VERUM (_attributum(s, "operator", "-d"));
    s = _situs(situs, "probatio", "build/a.o", X);
    CREDO_VERUM (_attributum(s, "operator", "-nt"));
    s = _situs(situs, "probatio", "src/a.c", X);
    CREDO_VERUM (_attributum(s, "operator", "-nt"));

    imprimere("\n--- IV. tabula mandatorum ---\n");
    s = _situs(situs, "scriptura", "build/s.txt", XI);
    CREDO_VERUM (_attributum(s, "mandatum", "sort"));
    CREDO_NON_NIHIL (_situs(situs, "lectio", "data/f.txt", XI));
    CREDO_NON_NIHIL (_situs(situs, "lectio", "data/g.txt", XII));
    CREDO_NON_NIHIL (_situs(situs, "scriptura", "data/g.txt", XII));
    CREDO_AEQUALIS_I32 (_numerare(situs, NIHIL, XII), II);
    CREDO_NON_NIHIL (_situs(situs, "lectio", "data/f.txt", XIII));
    CREDO_AEQUALIS_I32 (_numerare(situs, NIHIL, XIII), I);
    CREDO_NON_NIHIL (_situs(situs, "lectio", "data/h.txt", XIV));
    CREDO_AEQUALIS_I32 (_numerare(situs, NIHIL, XIV), I);
    CREDO_NON_NIHIL (_situs(situs, "lectio", "data/f.txt", XXII));
    CREDO_NON_NIHIL (_situs(situs, "scriptura", "build/copia.txt",
        XXII));

    imprimere("\n--- V. functio domus, ignota ---\n");
    CREDO_AEQUALIS_I32 (_numerare(situs, NIHIL, XV), ZEPHYRUM);
    s = _situs(situs, "ignotum", NIHIL, XVI);
    CREDO_VERUM (_attributum(s, "mandatum", "jqx"));
    CREDO_VERUM (_attributum(s, "causa", "mandatum ignotum"));
    s = _situs(situs, "ignotum", NIHIL, XVII);
    CREDO_VERUM (_attributum(s, "mandatum", "eval"));

    imprimere("\n--- VI. ambitus ---\n");
    s = _situs(situs, "ambitus_lectio", "CMD", XVII);
    CREDO_VERUM (_attributum(s, "assignatum", "falsum"));
    CREDO_NON_NIHIL (_situs(situs, "ambitus_lectio", "HOME", XVIII));
    CREDO_AEQUALIS_I32 (_numerare(situs, "ambitus_lectio", XVIII), I);
    CREDO_NIHIL (_situs(situs, "ambitus_lectio", "R", ZEPHYRUM));
    CREDO_NIHIL (_situs(situs, "ambitus_lectio", "D", ZEPHYRUM));
    CREDO_NIHIL (_situs(situs, "ambitus_lectio", "L", ZEPHYRUM));
    CREDO_NON_NIHIL (_situs(situs, "lectio", "data/r.txt", XX));

    imprimere("\n--- VII. processus novus (a/puer.sh) ---\n");
    s = _situs(situs, "exsecutio", "a/puer.sh", XXIII);
    CREDO_VERUM (_attributum(s, "classis", "arbor"));
    _imprimere_situs(situs_puer);
    s = _situs(situs_puer, "lectio", NIHIL, II);
    CREDO_VERUM (_attributum(s, "resolutio", "nulla"));
    CREDO_NON_NIHIL (_situs(situs_puer, "ambitus_lectio", "D", II));

    imprimere("\n--- VIII. cwd ---\n");
    s = _situs(situs, "lectio", NIHIL, XXIV);
    CREDO_VERUM (_attributum(s, "resolutio", "nulla"));
    CREDO_VERUM (_attributum(s, "causa", "cwd ignotum"));
    s = _situs(situs, "lectio", "build/rel.txt", XXV);
    CREDO_VERUM (_attributum(s, "scripta_in_ambitu", "falsum"));
    /* 'cd build' (linea 25, in catena) probat 'build' - non
     * 'build/build': cd argumento suo cwd non mutat */
    s = _situs(situs, "probatio", "build", XXV);
    CREDO_VERUM (_attributum(s, "operator", "cd"));

    imprimere("\n--- IX. scripta_in_ambitu ---\n");
    s = _situs(situs, "scriptura", "build/clausurae/", XXVII);
    CREDO_VERUM (_attributum(s, "forma", "praefixum"));
    CREDO_VERUM (_attributum(s, "resolutio", "partialis"));
    s = _situs(situs, "lectio", "build/clausurae/*.lst", XXVI);
    CREDO_VERUM (_attributum(s, "forma", "globus"));
    CREDO_VERUM (_attributum(s, "scripta_in_ambitu", "verum"));

    imprimere("\n--- IX b. redirectio ex processu ---\n");
    /* '< <(cat x)': redirectio ipsa nihil legit; cat intus legit */
    CREDO_NON_NIHIL (_situs(situs, "lectio", "data/p.txt", XXVIII));
    CREDO_AEQUALIS_I32 (_numerare(situs, "lectio", XXVIII), I);

    imprimere("\n--- IX c. cd in summo gradu ---\n");
    /* 'cd lib' summi gradus: probatio 'lib' (non 'lib/lib'); situs
     * sequens in lib/ */
    s = _situs(situs, "probatio", "lib", XXIX);
    CREDO_VERUM (_attributum(s, "operator", "cd"));
    CREDO_NON_NIHIL (_situs(situs, "lectio", "lib/q.txt", XXX));

    imprimere("\n--- IX d. aedificator custoditus ---\n");
    /* '[ -x bin/inst ] || ./a/struere.sh': exsecutio custodita;
     * processus struere et nepos (solum per eum) custoditi; communis
     * (libere) non; plagulam_tenet processus custoditos praeterit */
    s = _situs(situs, "exsecutio", "a/struere.sh", XXXII);
    CREDO_VERUM (_attributum(s, "custodia", "bin/inst"));
    s = _situs(situs, "exsecutio", "a/communis.sh", XXXIII);
    CREDO_VERUM (_attributum(s, "custodia", NIHIL));
    CREDO_VERUM (_processus_custodia(summarium, "a/struere.sh",
        "bin/inst"));
    CREDO_VERUM (_processus_custodia(summarium, "a/nepos.sh",
        "bin/inst"));
    CREDO_VERUM (_processus_custodia(summarium, "a/communis.sh",
        NIHIL));
    CREDO_FALSUM (crusta_effectus_plagulam_tenet(summarium,
        "a/nepos.sh"));
    CREDO_VERUM (crusta_effectus_plagulam_tenet(summarium,
        "a/communis.sh"));

    imprimere("\n--- IX e. binarium domus cum ordine tabulae ---\n");
    /* bin/compilator: exsecutio (provenientia) ET ordo 'compilator'
     * tabulae - '-o' scribitur; lectiones per librum lectionum
     * vestigantur, ergo hic nullae */
    CREDO_NON_NIHIL (_situs(situs, "exsecutio", "bin/compilator",
        XXXIV));
    s = _situs(situs, "scriptura", "build/x.o", XXXIV);
    CREDO_VERUM (_attributum(s, "mandatum", "compilator"));
    CREDO_AEQUALIS_I32 (_numerare(situs, "lectio", XXXIV), ZEPHYRUM);

    imprimere("\n--- X. canon super summarium ---\n");
    {
        character  via_canonis[DXII];
            Canon* canon;
           chorda  causa_canonis;
              Xar* vitia;

        sprintf(via_canonis, "%s/effectus.canon", r);
        canon = canon_legere(filum_legere_totum(via_canonis, piscina),
            piscina, intern, &causa_canonis);
        CREDO_NON_NIHIL (canon);
        vitia = canon ? canon_iudicare(canon, summarium,
            piscina) : NIHIL;
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 (vitia ? xar_numerus(vitia) : I, ZEPHYRUM);

        /* XI. domus tota: omne .sh summatur, canon sanum */
        imprimere("\n--- XI. domus tota ---\n");
        {
            character via_indicis[DXII];
               chorda index;
                  i32 summata  = ZEPHYRUM;
                  i32 fracta   = ZEPHYRUM;
                  i32 vitiosa  = ZEPHYRUM;
                  i32 a        = ZEPHYRUM;

            sprintf(via_indicis, "%s/build/crusta_corpus.lst", r);
            index = filum_legere_totum(via_indicis, piscina);
            CREDO_MAIOR_I32 (index.mensura, ZEPHYRUM);
            dum (a < index.mensura)
            {
                 character  scriptum[DXII];
                       i32  b = a;
                 StmlNodus* sm;

                dum (b < index.mensura && index.datum[b] != '\n')
                {
                    b++;
                }
                si (   b > a && b - a < (i32)DXII
                    && !(b - a > VIII
                         && memcmp(index.datum + a, "oracula/", VIII)
                            == ZEPHYRUM))
                {
                    memcpy(scriptum, index.datum + a, (size_t)(b - a));
                    scriptum[b - a] = '\0';
                    sm = crusta_effectus_derivare(piscina, intern, r,
                        scriptum, NIHIL, &causa);
                    si (sm == NIHIL)
                    {
                        imprimere("  NIHIL: %s (%s)\n", scriptum,
                            causa ? causa : "?");
                        fracta++;
                    }
                    alioquin
                    {
                        Xar* v = canon_iudicare(canon, sm, piscina);

                        si (v == NIHIL || xar_numerus(v) > ZEPHYRUM)
                        {
                            imprimere("  canon: %s (%u)\n", scriptum,
                                v ? (insignatus integer)xar_numerus(v)
                                  : 0);
                            vitiosa++;
                        }
                        summata++;
                    }
                }
                a = b + I;
            }
            imprimere("  summata %u, NIHIL %u, canone vitiosa %u\n",
                (insignatus integer)summata, (insignatus integer)fracta,
                (insignatus integer)vitiosa);
            CREDO_MAIOR_I32 (summata, CC);
            CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (vitiosa, ZEPHYRUM);
        }
    }

    /* XII. oraculum: summaria statica fixorum contra AURAS observatas
     * (crusta/effectus_oraculum.sh -scribere; lex C14: nihil hic
     * spawnatur). Observatum omne tegi debet; pinna situs certos
     * fert, ne aura vacua 'omnia tecta' mentiatur. */
    imprimere("\n--- XII. oraculum: fixa contra auras ---\n");
    {
        constans character* fixa[] = {
            "ante", "cd", "fontatio", "globus", "mandatum", "probatio",
            "puer", "redirectio", "temporaria", NIHIL
        };
        /* elementum, via quae in aura observata esse debent */
        constans character* pinnae[] = {
            "redirectio", "lectio", "data/a.txt",
            "redirectio", "scriptura", "build/o.txt",
            "mandatum", "lectio", "data/b.txt",     /* head: argv SIP */
            "mandatum", "scriptura", "build/s.txt", /* sort -o */
            "probatio", "probatio", "data/absens.txt",
            "globus", "enumeratio", "data",
            "fontatio", "lectio", "lib/m.sh",
            "puer", "lectio", "data/p.txt",   /* filius: shebang */
            "cd", "probatio", "lib",
            NIHIL
        };
           character via_fixorum[DXII];
           character via_tabulae[DXII];
        StmlResultus tabula;
                 i32 i;

        sprintf(via_fixorum, "%s/crusta/probationes/fixa/effectus/"
            "oraculum", r);
        sprintf(via_tabulae, "%s/crusta/effectus_mandata.stml", r);
        tabula = stml_legere(filum_legere_totum(via_tabulae, piscina),
            piscina, intern);
        CREDO_VERUM (tabula.successus);
        per (i = ZEPHYRUM; fixa[i] != NIHIL; i++)
        {
               character  scriptum[LXIV];
               character  via_aurae[DXII];
            StmlResultus  aura;
               StmlNodus* staticum;
                     Xar* non_tecta;
                     Xar* explicata;
                     Xar* observati;
                     i32  k;

            sprintf(scriptum, "%s.sh", fixa[i]);
            sprintf(via_aurae, "%s/crusta/probationes/fixa/effectus/"
                "oraculum_aura/%s.stml", r, fixa[i]);
            aura = stml_legere(filum_legere_totum(via_aurae, piscina),
                piscina, intern);
            CREDO_VERUM (aura.successus);
            staticum = crusta_effectus_derivare(piscina, intern,
                via_fixorum, scriptum, tabula.elementum_radix, &causa);
            CREDO_NON_NIHIL (staticum);
            si (!aura.successus || staticum == NIHIL)
            {
                perge;
            }
            explicata = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
            non_tecta = crusta_effectus_non_tecta(piscina, staticum,
                aura.elementum_radix, explicata);
            /* fixa omnia resolvuntur: nec explicata per ignota */
            CREDO_AEQUALIS_I32 (xar_numerus(explicata), ZEPHYRUM);
            observati = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
            _situs_colligere(aura.elementum_radix, observati, NIHIL);
            imprimere("  %s: observata %u, non tecta %u\n", fixa[i],
                (insignatus integer)xar_numerus(observati),
                non_tecta ? (insignatus integer)xar_numerus(non_tecta)
                          : 0U);
            si (non_tecta != NIHIL)
            {
                _imprimere_situs(non_tecta);
            }
            CREDO_NON_NIHIL (non_tecta);
            CREDO_AEQUALIS_I32 (non_tecta ? xar_numerus(non_tecta) : I,
                ZEPHYRUM);
            /* temporaria (T3): nomina fortuita, ergo pinna per classem
             * - scriptura bash, lectio cat (argv), probatio [ -s ],
             * rm */
            si (strcmp(fixa[i], "temporaria") == ZEPHYRUM)
            {
                i32 temporariae = ZEPHYRUM;

                per (k = ZEPHYRUM; k < xar_numerus(observati); k++)
                {
                    si (_attributum(*(StmlNodus**)xar_obtinere(
                            observati, k), "classis", "temporaria"))
                    {
                        temporariae++;
                    }
                }
                CREDO_AEQUALIS_I32 (temporariae, V);
            }
            per (k = ZEPHYRUM; pinnae[k] != NIHIL; k += III)
            {
                si (strcmp(pinnae[k], fixa[i]) == ZEPHYRUM)
                {
                    StmlNodus* x = _situs(observati, pinnae[k + I],
                        pinnae[k + II], ZEPHYRUM);

                    si (x == NIHIL)
                    {
                        imprimere("    pinna absens: %s %s\n",
                            pinnae[k + I], pinnae[k + II]);
                    }
                    CREDO_NON_NIHIL (x);
                }
            }
        }
    }

    /* XIII. causae (effectus-plan-2 T1): omnis situs irresolutus
     * causam suam NOMINAT, numquam 'valor ignotus' */
    imprimere("\n--- XIII. causae ---\n");
    _scribere("a/causae.sh", CAUSAE);
    {
        StmlNodus* sm = crusta_effectus_derivare(piscina, intern, radix,
            "a/causae.sh", NIHIL, &causa);
              Xar* cs = xar_creare(piscina, (i32)magnitudo(StmlNodus*));

        CREDO_NON_NIHIL (sm);
        si (sm != NIHIL)
        {
            _situs_colligere(sm, cs, "a/causae.sh");
            _imprimere_situs(cs);
            CREDO_VERUM (_causa_lineae(cs, III, "argumentum"));
            CREDO_VERUM (_causa_lineae(cs, V, "ansa_read"));
            CREDO_VERUM (_causa_lineae(cs, VI, "substitutio"));
            CREDO_VERUM (_causa_lineae(cs, VII, "ambitus"));
            CREDO_VERUM (_causa_lineae(cs, VIII, "operator"));
            CREDO_VERUM (_causa_lineae(cs, XI, "discordia"));
            CREDO_VERUM (_causa_lineae(cs, XIII, "tabulatum"));
            /* T5: 'for g in p q' membra duo resoluta (non ansa_read);
             * Y=a cum Y=$(mktemp): discordia vera (temporaria mixta) */
            CREDO_NON_NIHIL (_situs(cs, "lectio", "p", XIV));
            CREDO_NON_NIHIL (_situs(cs, "lectio", "q", XIV));
            CREDO_VERUM (_causa_lineae(cs, XV, "operator"));
            /* accumulatio in ansa: cyclus, non profunditas */
            CREDO_VERUM (_causa_lineae(cs, XIX, "recursio"));
            /* praefixum partiale quoque causam fert (resolutio non
             * plena); textus manet */
            s = _situs(cs, "lectio", NIHIL, XVI);
            CREDO_VERUM (_attributum(s, "causa", "ambitus"));
            /* contrarium: situs plenus causam non fert */
            s = _situs(cs, "lectio", "data/r.txt", V);
            CREDO_VERUM (_attributum(s, "resolutio", "plena"));
            CREDO_VERUM (_attributum(s, "causa", NIHIL));
        }
    }

    /* XIV. temporaria (effectus-plan-2 T3; spec-2 par. V.5, A1): via =
     * cauda sub objecto mktemp ('.' = ipsum), attributum temporaria =
     * plagula:linea:columna substitutionis creantis */
    imprimere("\n--- XIV. temporaria ---\n");
    _scribere("a/temporaria.sh", TEMPORARIA);
    _scribere("a/filius_t.sh", FILIUS_T);
    {
        StmlNodus* sm = crusta_effectus_derivare(piscina, intern, radix,
            "a/temporaria.sh", NIHIL, &causa);
              Xar* ts = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
              Xar* fs = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
           chorda* tt;

        CREDO_NON_NIHIL (sm);
        si (sm != NIHIL)
        {
            _situs_colligere(sm, ts, "a/temporaria.sh");
            _situs_colligere(sm, fs, "a/filius_t.sh");
            _imprimere_situs(ts);
            s = _situs(ts, "scriptura", "a", III);
            CREDO_VERUM (_attributum(s, "classis", "temporaria"));
            CREDO_VERUM (_attributum(s, "resolutio", "plena"));
            CREDO_VERUM (_attributum(s, "temporaria",
                "a/temporaria.sh:2:4"));
            s = _situs(ts, "lectio", "a", IV);
            CREDO_VERUM (_attributum(s, "classis", "temporaria"));
            CREDO_VERUM (_attributum(s, "temporaria",
                "a/temporaria.sh:2:4"));
            /* scriptura in eodem objecto temporario: scripta */
            CREDO_VERUM (_attributum(s, "scripta_in_ambitu", "verum"));
            /* R="$T/r": objectum idem (A1) */
            s = _situs(ts, "lectio", "r", VI);
            CREDO_VERUM (_attributum(s, "temporaria",
                "a/temporaria.sh:2:4"));
            /* mktemp sine -d, cum exemplari: objectum ipsum '.' */
            s = _situs(ts, "scriptura", ".", VIII);
            CREDO_VERUM (_attributum(s, "temporaria",
                "a/temporaria.sh:7:3"));
            s = _situs(ts, "lectio", ".", X);
            CREDO_VERUM (_attributum(s, "temporaria",
                "a/temporaria.sh:9:4"));
            /* in functione, 'local' */
            s = _situs(ts, "lectio", "q", XIV);
            CREDO_VERUM (_attributum(s, "classis", "temporaria"));
            /* CONTRARIA: nomen fixum non temporaria; $(date) ignotum */
            s = _situs(ts, "lectio", "/tmp/x", XVII);
            CREDO_VERUM (_attributum(s, "classis", "externa"));
            CREDO_VERUM (_attributum(s, "temporaria", NIHIL));
            s = _situs(ts, "lectio", NIHIL, XIX);
            CREDO_VERUM (_attributum(s, "resolutio", "nulla"));
            CREDO_VERUM (_attributum(s, "causa", "substitutio"));
            /* cauda ignota: temporaria tamen, partialis */
            s = _situs(ts, "lectio", NIHIL, XX);
            CREDO_VERUM (_attributum(s, "classis", "temporaria"));
            CREDO_VERUM (_attributum(s, "resolutio", "partialis"));
            CREDO_VERUM (_attributum(s, "via", "./"));
            /* filius: T ex ambitu, irresolutum */
            s = _situs(fs, "lectio", NIHIL, II);
            CREDO_VERUM (_attributum(s, "resolutio", "nulla"));
            CREDO_VERUM (_attributum(s, "classis", NIHIL));
            tt = s != NIHIL ? stml_attributum_capere(s, "temporaria")
                            : NIHIL;
            CREDO_NIHIL (tt);
        }
    }

    /* XV. tabulata (effectus-plan-2 T4; spec-2 par. III, V.2) */
    imprimere("\n--- XV. tabulata ---\n");
    _scribere("a/tabulata.sh", TABULATA);
    _scribere("a/vexilla_ficta.sh", VEXILLA_FICTA);
    {
        StmlNodus* sm = crusta_effectus_derivare(piscina, intern, radix,
            "a/tabulata.sh", NIHIL, &causa);
              Xar* ts = xar_creare(piscina, (i32)magnitudo(StmlNodus*));

        CREDO_NON_NIHIL (sm);
        si (sm != NIHIL)
        {
            _situs_colligere(sm, ts, "a/tabulata.sh");
            _imprimere_situs(ts);
            /* optiones per tabulatum (et tabulatum fontatum, et +=):
             * lectio sola fons, scriptura sola -o */
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", V), I);
            CREDO_NON_NIHIL (_situs(ts, "lectio", "src/a.c", V));
            CREDO_NON_NIHIL (_situs(ts, "scriptura", "build/b.o", V));
            CREDO_AEQUALIS_I32 (_numerare(ts, NIHIL, V), II);
            /* RF 6: elementa viae lectiones fiunt; sedes = verbum
             * expansionis in imperio, non definitio */
            s = _situs(ts, "lectio", "lib/a.c", VII);
            CREDO_NON_NIHIL (s);
            CREDO_VERUM (_attributum(s, "sedes", "7:7-7:16"));
            CREDO_NON_NIHIL (_situs(ts, "lectio", "lib/b.c", VII));
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", VII), II);
            /* RF 5: '-o "$OUT"' in additione: scriptura (irresoluta),
             * numquam lectio; tabulatum ex tabulato fontato */
            s = _situs(ts, "scriptura", NIHIL, X);
            CREDO_VERUM (_attributum(s, "resolutio", "nulla"));
            CREDO_NON_NIHIL (_situs(ts, "lectio", "src/c.c", X));
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", X), I);
            /* optio cum valore adnexo non litterali ('-I"$R/inc"')
             * optio est, non plagula '-I...' (vitium ante T4) */
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", XI), I);
            CREDO_NON_NIHIL (_situs(ts, "lectio", "src/d.c", XI));
        }
    }

    /* XVI. ansae, exemplaria, operatores (effectus-plan-2 T5) */
    imprimere("\n--- XVI. ansae et exemplaria ---\n");
    _scribere("a/ansae.sh", ANSAE);
    _scribere("a/m1.sh", M1);
    _scribere("a/m2.sh", M2);
    {
        StmlNodus* sm = crusta_effectus_derivare(piscina, intern, radix,
            "a/ansae.sh", NIHIL, &causa);
              Xar* ts = xar_creare(piscina, (i32)magnitudo(StmlNodus*));

        CREDO_NON_NIHIL (sm);
        si (sm != NIHIL)
        {
            _situs_colligere(sm, ts, "a/ansae.sh");
            _imprimere_situs(ts);
            /* lista litteralis: membrum quodque situs suus, sedes
             * una */
            CREDO_NON_NIHIL (_situs(ts, "lectio", "toml/x", II));
            CREDO_NON_NIHIL (_situs(ts, "lectio", "css/x", II));
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", II), II);
            /* globus in lista: exemplar, etiam citatum in usu */
            s = _situs(ts, "lectio", "lib/*.c", III);
            CREDO_VERUM (_attributum(s, "forma", "globus"));
            s = _situs(ts, "lectio", "build/q/*.o", V);
            CREDO_VERUM (_attributum(s, "forma", "globus"));
            s = _situs(ts, "lectio", "lib/*.h", VI);
            CREDO_VERUM (_attributum(s, "forma", "globus"));
            s = _situs(ts, "lectio", "*.c", VII);
            CREDO_VERUM (_attributum(s, "forma", "globus"));
            /* RF 1 CONTRARIUM: "$X" cum X globum litteralem tenente
             * plagula litteralis est; $X nudus a bash expanditur */
            s = _situs(ts, "lectio", "lib/*.c", IX);
            CREDO_VERUM (_attributum(s, "forma", "via"));
            s = _situs(ts, "lectio", "lib/*.c", X);
            CREDO_VERUM (_attributum(s, "forma", "globus"));
            /* terminus XVI (A2): XVII membra -> praefixum, discordia */
            s = _situs(ts, "lectio", "d/", XI);
            CREDO_VERUM (_attributum(s, "resolutio", "partialis"));
            CREDO_VERUM (_attributum(s, "causa", "discordia"));
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", XI), I);
            /* RF 3: XVI x XVI productum -> terminus, non area fracta */
            s = _situs(ts, "lectio", NIHIL, XII);
            CREDO_VERUM (_attributum(s, "resolutio", "partialis"));
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", XII), I);
            /* scalaris nudus cum spatiis: verba (optiones), non
             * plagula */
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", XIV), I);
            CREDO_NON_NIHIL (_situs(ts, "lectio", "src/m.c", XIV));
            /* fontatio per membra: ambo processus sequuntur */
            CREDO_NON_NIHIL (_situs(ts, "fontatio", "a/m1.sh", XV));
            CREDO_NON_NIHIL (_situs(ts, "fontatio", "a/m2.sh", XV));
            CREDO_NON_NIHIL (_situs(ts, "lectio", "data/m1.txt",
                ZEPHYRUM));
            CREDO_NON_NIHIL (_situs(ts, "lectio", "data/m2.txt",
                ZEPHYRUM));
            /* recursio (cp -r) per membra: praefixa ambo */
            s = _situs(ts, "lectio", "src/u/", XVI);
            CREDO_VERUM (_attributum(s, "forma", "praefixum"));
            s = _situs(ts, "lectio", "src/v/", XVI);
            CREDO_VERUM (_attributum(s, "forma", "praefixum"));
        }
    }

    /* XVII. praedefinita (effectus-plan-2 T6; spec-2 A4) */
    imprimere("\n--- XVII. praedefinita ---\n");
    _scribere("a/praedefinita.sh", PRAEDEFINITUM);
    {
        StmlNodus* sm = crusta_effectus_derivare(piscina, intern, radix,
            "a/praedefinita.sh", NIHIL, &causa);
              Xar* ts = xar_creare(piscina, (i32)magnitudo(StmlNodus*));

        CREDO_NON_NIHIL (sm);
        si (sm != NIHIL)
        {
            _situs_colligere(sm, ts, "a/praedefinita.sh");
            _imprimere_situs(ts);
            /* FS nusquam definita: valor praedefinitus + ambitus FS */
            s = _situs(ts, "lectio", "build/fs/x", II);
            CREDO_VERUM (_attributum(s, "resolutio", "plena"));
            CREDO_NON_NIHIL (_situs(ts, "ambitus_lectio", "FS", II));
            /* G definita: unio (a, build/g) */
            CREDO_NON_NIHIL (_situs(ts, "lectio", "a/y", IV));
            CREDO_NON_NIHIL (_situs(ts, "lectio", "build/g/y", IV));
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", IV), II);
            /* ${H-d} idem */
            CREDO_NON_NIHIL (_situs(ts, "lectio", "data/h/z", V));
            /* CONTRARIUM: argumentum ignotum manet */
            s = _situs(ts, "lectio", NIHIL, VI);
            CREDO_VERUM (_attributum(s, "resolutio", "nulla"));
            CREDO_VERUM (_attributum(s, "causa", "argumentum"));
            /* ':-' vacuum ut absens: E="" -> data/e solum */
            CREDO_NON_NIHIL (_situs(ts, "lectio", "data/e/v", VIII));
            CREDO_AEQUALIS_I32 (_numerare(ts, "lectio", VIII), I);
        }
    }

    /* XVIII. subsumptio (effectus-plan-3 T1; spec-3 par. VIII): omnis
     * situs NOVI a situ VETERIS eiusdem plagulae, sedis, elementi
     * tegitur - aliter defectus (membrum novum aut regressio) */
    imprimere("\n--- XVIII. subsumptio ---\n");
    {
        StmlResultus v = stml_legere(chorda_ex_literis(SUMMARIUM_VETUS,
            piscina), piscina, intern);
        StmlResultus b = stml_legere(chorda_ex_literis(SUMMARIUM_BONUM,
            piscina), piscina, intern);
        StmlResultus m = stml_legere(chorda_ex_literis(SUMMARIUM_MALUM,
            piscina), piscina, intern);
                 Xar* r;

        CREDO_VERUM (v.successus && b.successus && m.successus);
        si (v.successus && b.successus && m.successus)
        {
            r = crusta_effectus_subsumptio(piscina, v.elementum_radix,
                b.elementum_radix);
            CREDO_NON_NIHIL (r);
            CREDO_AEQUALIS_I32 (r ? xar_numerus(r) : I, ZEPHYRUM);
            r = crusta_effectus_subsumptio(piscina, v.elementum_radix,
                m.elementum_radix);
            CREDO_NON_NIHIL (r);
            si (r != NIHIL)
            {
                _imprimere_situs(r);
            }
            /* a/y (extra), src/a.c (extra globum), a/ (regressio) */
            CREDO_AEQUALIS_I32 (r ? xar_numerus(r) : ZEPHYRUM, III);
            CREDO_NON_NIHIL (r ? _situs(r, "lectio", "a/y", ZEPHYRUM)
                               : NIHIL);
            CREDO_NON_NIHIL (r ? _situs(r, "lectio", "a/", ZEPHYRUM)
                               : NIHIL);
            /* idem contra se: omnia subsumpta */
            r = crusta_effectus_subsumptio(piscina, v.elementum_radix,
                v.elementum_radix);
            CREDO_AEQUALIS_I32 (r ? xar_numerus(r) : I, ZEPHYRUM);
        }
    }

    (vacuum)filum_arborem_delere(basis);
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
