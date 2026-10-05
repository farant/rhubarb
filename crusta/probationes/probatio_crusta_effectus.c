/* probatio_crusta_effectus.c - summarium effectuum (effectus-plan.md
 * T3; spec par. IV)
 *
 * Arbor ficta sub crusta/build/ (idioma probatio_crusta_fontationes):
 * scriptum unum (a/r.sh) classem omnem situum spec par. IV.2 tangit,
 * linea per classem. Quaeque assertio CONTRARIUM quoque fert ubi
 * analysis errare potest: globus citatus non enumeratur; /dev/null et
 * '2>&1' non scribuntur; '<<<' non legitur; exemplar grep/sed situs non
 * est; functio domus mandatum ignotum non est; '$?' '$1' et variabiles
 * a 'read' assignatae ambitus non leguntur; scriptum EXSECUTUM
 * variabiles vocantis non videt.
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
    "while read -r l; do :; done < <(cat data/p.txt)\n"; /* 28 */

hic_manens constans character* LIB =
    "lib_functio () {\n"
    "    local v=\"$1\"\n"
    "    cat \"$D/data/lib.txt\"\n"
    "}\n";

hic_manens constans character* PUER =
    "#!/bin/bash\n"
    "cat \"$D/data/puer.txt\"\n";

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

    (vacuum)filum_arborem_delere(basis);
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
