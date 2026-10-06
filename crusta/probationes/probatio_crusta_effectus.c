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
    "Y=b\n"                                               /* 10 */
    "cat \"$Y\"\n"                                        /* 11 */
    "A=(x y)\n"                                           /* 12 */
    "cat \"$A\"\n"                                        /* 13 */
    "for g in p q; do cat \"$g\"; done\n"                 /* 14 */
    "cat \"$((I + II))\"\n"                               /* 15 */
    "cat \"$NESCIO/data/z\"\n"                            /* 16 */
    "O=\"\"\n"                                             /* 17 */
    "for o in a b; do O=\"$O $o\"; done\n"                 /* 18 */
    "cat $O\n";                                          /* 19 */

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
            "puer", "redirectio", NIHIL
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
            CREDO_VERUM (_causa_lineae(cs, XIV, "ansa_read"));
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

    (vacuum)filum_arborem_delere(basis);
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
