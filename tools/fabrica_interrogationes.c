/* fabrica_interrogationes.c - bin/fabrica quid -interrogationes:
 * QUAESTIO (piscina communis quaestionum) et PAGINA docs/faq.html
 * (spec unitates-codicis par. VII, U5b gradus 4; lector
 * lib/unitates.c unitates_interrogationes)
 *
 * -quaere TEXTUS: verba quaestionis (minuscula, sine verbis vacuis
 * Anglicis) contra verba quaestionum (III puncta), symbola (II) et
 * responsa (I); verbum quodque semel numeratur, optimo loco.
 * Congruentia = aequalitas, aut praefixum commune IV litterarum saltem
 * quod brevius verbum totum praeter II ultimas tegit: 'pointer'
 * 'pointers' et 'compare' 'comparing' tangit. Ordo: puncta
 * descendentia, deinde ordo lectionis.
 *
 * -symbolum X: interrogationes quarum responsum X ancorat (lectio
 * inversa: hae SUNT documenta symboli).
 *
 * -html VIA: pagina una, DETERMINISTICA (nulla tempora), unitates
 * ordine tituli; signum GENERATUM in linea II post doctype (lex domus).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "filum.h"
#include "unitates.h"
#include "fabrica_sutura.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior vacuum
_scribere (
    chorda c)
{
    fwrite(c.datum, I, (size_t)c.mensura, stdout);
}

interior constans character* constans _VERBA_VACUA[] = {
    "a", "an", "and", "are", "as", "be", "can", "do", "does", "for",
    "from", "how", "i", "in", "into", "is", "it", "me", "my", "of",
    "on", "or", "that", "the", "this", "to", "what", "when", "which",
    "why", "with", NIHIL
};

interior b32
_vacuum_est (
    chorda verbum)
{
    i32 i;

    per (i = ZEPHYRUM; _VERBA_VACUA[i] != NIHIL; i++)
    {
        si (chorda_aequalis_literis(verbum, _VERBA_VACUA[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* textus -> verba minuscula [a-z0-9_]+ (vacua omissa) */
interior Xar*
_verba (
      chorda  textus,
     Piscina* piscina)
{
    Xar* verba = xar_creare(piscina, (i32)magnitudo(chorda));
    i32  i;
    s32  initium = -I;

    per (i = ZEPHYRUM; i <= textus.mensura; i++)
    {
        character ch = i < textus.mensura ? (character)textus.datum[i]
            : ' ';
        b32 litterae = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch
            <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_';

        si (litterae && initium < ZEPHYRUM)
        {
            initium = (s32)i;
        }
        alioquin si (!litterae && initium >= ZEPHYRUM)
        {
            i32  n = i - (i32)initium;
             i8* m = (i8*)piscina_allocare_ordinatum(piscina,
                 (memoriae_index)n, (memoriae_index)I);
               i32 j;
            chorda v;

            per (j = ZEPHYRUM; j < n; j++)
            {
                character c = (character)textus.datum[(i32)initium + j];

                m[j] = (i8)((c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c);
            }
            v.datum    = m;
            v.mensura  = n;
            si (n >= II && !_vacuum_est(v))
            {
                *(chorda*)xar_addere(verba) = v;
            }
            /* identificator: partes quoque ('piscina_allocare' ->
             * piscina, allocare) - 'allocation' partem tangit */
            {
                i32 p0 = ZEPHYRUM;
                i32 k;

                per (k = ZEPHYRUM; k <= n; k++)
                {
                    si (k == n || m[k] == '_')
                    {
                        chorda pars;

                        pars.datum    = m + p0;
                        pars.mensura  = k - p0;
                        si (   p0 > ZEPHYRUM || k < n)
                        {
                            si (   pars.mensura >= II
                                && !_vacuum_est(pars))
                            {
                                *(chorda*)xar_addere(verba) = pars;
                            }
                        }
                        p0 = k + I;
                    }
                }
            }
            initium = -I;
        }
    }
    redde verba;
}

interior b32
_congruit (
    chorda a,
    chorda b)
{
    i32 minima    = a.mensura <= b.mensura ? a.mensura : b.mensura;
    i32 communis  = ZEPHYRUM;

    dum (communis < minima && a.datum[communis] == b.datum[communis])
    {
        communis++;
    }
    /* aequalia, aut praefixum commune IV saltem et brevius sine II
     * ultimis litteris: pointer/pointers, compare/comparing,
     * allocate/allocation, grow/growing */
    redde (communis == minima && a.mensura == b.mensura)
        || (communis >= IV && communis >= minima - II);
}

interior b32
_in_verbis (
    constans Xar* verba,
          chorda  verbum)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(verba); i++)
    {
        si (_congruit(*(chorda*)xar_obtinere(verba, i), verbum))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

nomen structura {
     constans UnitatisInterrogatio* x;
                               i32  puncta;
                               i32  ordo;
} Inventum;

interior s32
_inventa_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans Inventum* p = (constans Inventum*)a;
    constans Inventum* q = (constans Inventum*)b;

    si (p->puncta != q->puncta)
    {
        redde p->puncta > q->puncta ? -I : I;
    }
    redde p->ordo < q->ordo ? -I : (p->ordo > q->ordo ? I : ZEPHYRUM);
}

interior vacuum
_indicem_scribere (
    constans Xar* verba)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(verba); i++)
    {
        si (i > ZEPHYRUM)
        {
            putchar(' ');
        }
        _scribere(*(chorda*)xar_obtinere(verba, i));
    }
}

interior vacuum
_inventum_imprimere (
     constans UnitatisInterrogatio* x,
                               i32  puncta,
                               b32  machina)
{
    si (machina)
    {
        fputs("INTERROGATIO\t", stdout);
        _scribere(x->unitas->titulus);
        putchar('\t');
        _scribere(x->via);
        printf("\t%ld\t", (longus)x->linea);
        _scribere(x->genus);
        putchar('\t');
        _scribere(x->quaestio);
        putchar('\t');
        _scribere(x->responsum);
        putchar('\t');
        _indicem_scribere(x->symbola);
        putchar('\t');
        _indicem_scribere(x->viae);
        putchar('\t');
        _scribere(x->imperium);
        printf("\t%ld\n", (longus)puncta);
        redde;
    }
    printf("  %-8.*s ", (integer)x->unitas->titulus.mensura,
        (constans character*)x->unitas->titulus.datum);
    printf("%-9.*s ", (integer)x->genus.mensura,
        (constans character*)x->genus.datum);
    _scribere(x->quaestio);
    fputs("\n           -> ", stdout);
    _scribere(x->responsum);
    putchar('\n');
    fputs("           ", stdout);
    _scribere(x->via);
    printf(":%ld", (longus)x->linea);
    si (xar_numerus(x->symbola) > ZEPHYRUM)
    {
        fputs("  symbolum: ", stdout);
        _indicem_scribere(x->symbola);
    }
    putchar('\n');
}

s32
suturae_interrogationes_quaerere (
    constans UnitatesInterrogationes* res,
                  constans character* textus,
                  constans character* symbolum,
                                 b32  machina,
                             Piscina* piscina)
{
     Xar* quaesita = textus != NIHIL ? _verba(chorda_ex_literis(textus,
         piscina), piscina) : NIHIL;
     Xar* inventa  = xar_creare(piscina, (i32)magnitudo(Inventum));
     i32  i;
     i32  j;

    si (quaesita != NIHIL && xar_numerus(quaesita) == ZEPHYRUM)
    {
        fprintf(stderr, "quid: quaestio sine verbis ('%s')\n", textus);
        redde II;
    }
    per (i = ZEPHYRUM; i < xar_numerus(res->interrogationes); i++)
    {
        constans UnitatisInterrogatio* x = (constans
            UnitatisInterrogatio*)xar_obtinere(res->interrogationes, i);
                                   i32 puncta = ZEPHYRUM;

        si (symbolum != NIHIL)
        {
            per (j = ZEPHYRUM; j < xar_numerus(x->symbola); j++)
            {
                si (chorda_aequalis_literis(*(chorda*)xar_obtinere(
                        x->symbola, j), symbolum))
                {
                    puncta = I;
                }
            }
        }
        alioquin
        {
            Xar* q = _verba(x->quaestio, piscina);
            Xar* r = _verba(x->responsum, piscina);
            Xar* s = xar_creare(piscina, (i32)magnitudo(chorda));

            /* symbola minuscula ut verba (xar_addere unum verbum est:
             * '_' litteris adnumeratur) */
            per (j = ZEPHYRUM; j < xar_numerus(x->symbola); j++)
            {
                Xar* partes = _verba(*(chorda*)xar_obtinere(x->symbola,
                    j), piscina);
                i32 k;

                per (k = ZEPHYRUM; k < xar_numerus(partes); k++)
                {
                    *(chorda*)xar_addere(s) = *(chorda*)xar_obtinere(
                        partes, k);
                }
            }
            per (j = ZEPHYRUM; j < xar_numerus(quaesita); j++)
            {
                chorda v = *(chorda*)xar_obtinere(quaesita, j);

                si (_in_verbis(q, v))
                {
                    puncta += III;
                }
                alioquin si (_in_verbis(s, v))
                {
                    puncta += II;
                }
                alioquin si (_in_verbis(r, v))
                {
                    puncta += I;
                }
            }
        }
        si (puncta > ZEPHYRUM)
        {
            Inventum* n = (Inventum*)xar_addere(inventa);

            n->x       = x;
            n->puncta  = puncta;
            n->ordo    = i;
        }
    }
    xar_ordinare(inventa, _inventa_comparare);
    per (i = ZEPHYRUM; i < xar_numerus(inventa); i++)
    {
        constans Inventum* n = (constans Inventum*)xar_obtinere(inventa,
            i);

        si (!machina && i >= X)
        {
            printf("  ... et %ld alia (-machina: omnia)\n",
                (longus)(xar_numerus(inventa) - X));
            frange;
        }
        _inventum_imprimere(n->x, n->puncta, machina);
    }
    si (xar_numerus(inventa) == ZEPHYRUM)
    {
        si (!machina)
        {
            puts("nihil congruit");
        }
        redde I;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * PAGINA docs/faq.html
 * ================================================== */

interior vacuum
_effugere (
    ChordaAedificator* a,
               chorda  c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character ch = (character)c.datum[i];

        si (ch == '&')
        {
            chorda_aedificator_appendere_literis(a, "&amp;");
        }
        alioquin si (ch == '<')
        {
            chorda_aedificator_appendere_literis(a, "&lt;");
        }
        alioquin si (ch == '>')
        {
            chorda_aedificator_appendere_literis(a, "&gt;");
        }
        alioquin si (ch == '"')
        {
            chorda_aedificator_appendere_literis(a, "&quot;");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, ch);
        }
    }
}

interior s32
_unitates_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans Unitas* p = *(constans Unitas* constans*)a;
    constans Unitas* q = *(constans Unitas* constans*)b;
                i32  n = p->titulus.mensura < q->titulus.mensura
                    ? p->titulus.mensura : q->titulus.mensura;
          integer c = memcmp(p->titulus.datum, q->titulus.datum,
              (size_t)n);

    si (c != ZEPHYRUM)
    {
        redde c < ZEPHYRUM ? -I : I;
    }
    redde p->titulus.mensura < q->titulus.mensura ? -I
        : (p->titulus.mensura > q->titulus.mensura ? I : ZEPHYRUM);
}

/* fons unitatis: bibliotheca -> caput stemmatis ('stemma include/x.h'),
 * directorium -> via sua */
interior chorda
_fons_unitatis (
    constans Unitas* x)
{
    constans character* praefixum  = "stemma ";
                   i32  n          = (i32)strlen(praefixum);

    si (   x->fons == UNITAS_BIBLIOTHECAE
        && x->declaratio.mensura > n
        && memcmp(x->declaratio.datum, praefixum, (size_t)n)
            == ZEPHYRUM)
    {
        redde chorda_sectio(x->declaratio, n, x->declaratio.mensura);
    }
    redde x->via;
}

interior vacuum
_ancoras_scribere (
                     ChordaAedificator* a,
         constans UnitatisInterrogatio* x)
{
    i32 i;
    b32 primum = VERUM;

    si (xar_numerus(x->symbola) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "symbolum");
        per (i = ZEPHYRUM; i < xar_numerus(x->symbola); i++)
        {
            chorda_aedificator_appendere_literis(a, " <code>");
            _effugere(a, *(chorda*)xar_obtinere(x->symbola, i));
            chorda_aedificator_appendere_literis(a, "</code>");
        }
        primum = FALSUM;
    }
    si (xar_numerus(x->viae) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a,
            primum ? "via" : " · via");
        per (i = ZEPHYRUM; i < xar_numerus(x->viae); i++)
        {
            chorda v = *(chorda*)xar_obtinere(x->viae, i);

            chorda_aedificator_appendere_literis(a, " <a href=\"../");
            _effugere(a, v);
            chorda_aedificator_appendere_literis(a, "\"><code>");
            _effugere(a, v);
            chorda_aedificator_appendere_literis(a, "</code></a>");
        }
        primum = FALSUM;
    }
    si (x->imperium.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a,
            primum ? "imperium <code>"
            : " · imperium <code>");
        _effugere(a, x->imperium);
        chorda_aedificator_appendere_literis(a, "</code>");
    }
}

s32
suturae_interrogationes_paginam_scribere (
    constans UnitatesInterrogationes* res,
                  constans character* via,
                             Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(XVI * M));
                  Xar* unitates = xar_creare(piscina,
                      (i32)magnitudo(Unitas*));
                  Xar* genera = xar_creare(piscina,
                      (i32)magnitudo(UnitatesNumerus));
                  i32 n = xar_numerus(res->interrogationes);
                  i32 i;
                  i32 j;

    si (xar_numerus(res->recusationes) > ZEPHYRUM)
    {
        fprintf(stderr, "quid: %ld vitia interrogationum - pagina non "
            "scripta (bin/fabrica quid -interrogationes)\n",
            (longus)xar_numerus(res->recusationes));
        redde I;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans UnitatisInterrogatio* x = (constans
            UnitatisInterrogatio*)xar_obtinere(res->interrogationes, i);
                                   b32  visa  = FALSUM;
                       UnitatesNumerus* g     = NIHIL;

        per (j = ZEPHYRUM; j < xar_numerus(unitates) && !visa; j++)
        {
            visa = *(constans Unitas**)xar_obtinere(unitates, j)
                == x->unitas;
        }
        si (!visa)
        {
            *(constans Unitas**)xar_addere(unitates) = x->unitas;
        }
        per (j = ZEPHYRUM; j < xar_numerus(genera) && g == NIHIL; j++)
        {
            UnitatesNumerus* h = (UnitatesNumerus*)xar_obtinere(genera,
                j);

            si (chorda_aequalis(h->clavis, x->genus))
            {
                g = h;
            }
        }
        si (g == NIHIL)
        {
            g           = (UnitatesNumerus*)xar_addere(genera);
            g->clavis   = x->genus;
            g->numerus  = ZEPHYRUM;
        }
        g->numerus++;
    }
    xar_ordinare(unitates, _unitates_comparare);

    chorda_aedificator_appendere_literis(a,
        "<!doctype html>\n"
        "<!-- GENERATUM: bin/fabrica quid -interrogationes -html"
        " (tools/interrogationes_pagina.sh) - noli manu mutare;"
        " quaestiones in lib/S.interrogationes.stml -->\n"
        "<html lang=\"en\">\n<head>\n<meta charset=\"utf-8\">\n"
        "<meta name=\"viewport\" content=\"width=device-width,"
        " initial-scale=1\">\n<title>Interrogationes</title>\n"
        "<link rel=\"stylesheet\" href=\"stilus.css\">\n</head>\n"
        "<body>\n\n"
        "<p class=\"nota\"><a href=\"index.html\">Rhubarb</a></p>\n"
        "<h1>Interrogationes</h1>\n"
        "<p class=\"subtitulus\">Questions the code answers: how do I,"
        " can I, why, what is</p>\n\n");
    chorda_aedificator_appendere_literis(a,
        "<p>Each unit declares the questions it answers in"
        " <code>lib/S.interrogationes.stml</code> (canon"
        " <code>interrogationes.canon</code>). Every answer is anchored"
        " to a symbol, a path or a command, and the gate"
        " <code>interrogationes</code> turns red when an anchor dies."
        " From the terminal: <code>bin/fabrica quid -interrogationes"
        " -quaere \"how do I ...\"</code>, or <code>-symbolum"
        " xar_addere</code> for the questions that name a"
        " symbol.</p>\n\n");
    chorda_aedificator_appendere_literis(a, "<p class=\"nota\">");
    chorda_aedificator_appendere_i32(a, n);
    chorda_aedificator_appendere_literis(a, n == I ? " question in "
        : " questions in ");
    chorda_aedificator_appendere_i32(a, xar_numerus(unitates));
    chorda_aedificator_appendere_literis(a, xar_numerus(unitates) == I
        ? " unit" : " units");
    per (j = ZEPHYRUM; j < xar_numerus(genera); j++)
    {
        constans UnitatesNumerus* g = (constans UnitatesNumerus*)
            xar_obtinere(genera, j);

        chorda_aedificator_appendere_literis(a, " · ");
        _effugere(a, g->clavis);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_i32(a, g->numerus);
    }
    chorda_aedificator_appendere_literis(a, "</p>\n<ul>\n");
    per (j = ZEPHYRUM; j < xar_numerus(unitates); j++)
    {
        constans Unitas* u = *(constans Unitas**)xar_obtinere(unitates,
            j);
                    i32 k = ZEPHYRUM;

        per (i = ZEPHYRUM; i < n; i++)
        {
            si (((constans UnitatisInterrogatio*)xar_obtinere(
                    res->interrogationes, i))->unitas == u)
            {
                k++;
            }
        }
        chorda_aedificator_appendere_literis(a, "<li><a href=\"#");
        _effugere(a, u->titulus);
        chorda_aedificator_appendere_literis(a, "\">");
        _effugere(a, u->titulus);
        chorda_aedificator_appendere_literis(a,
            "</a> <span class=\"nota\">");
        chorda_aedificator_appendere_i32(a, k);
        chorda_aedificator_appendere_literis(a, "</span></li>\n");
    }
    chorda_aedificator_appendere_literis(a, "</ul>\n");

    per (j = ZEPHYRUM; j < xar_numerus(unitates); j++)
    {
        constans Unitas* u = *(constans Unitas**)xar_obtinere(unitates,
            j);
                 chorda fons   = _fons_unitatis(u);
                    i32 ordo   = ZEPHYRUM;
                    b32 prima  = VERUM;

        chorda_aedificator_appendere_literis(a, "\n<h2 id=\"");
        _effugere(a, u->titulus);
        chorda_aedificator_appendere_literis(a, "\">");
        _effugere(a, u->titulus);
        chorda_aedificator_appendere_literis(a, "</h2>\n");
        per (i = ZEPHYRUM; i < n; i++)
        {
            constans UnitatisInterrogatio* x = (constans
                UnitatisInterrogatio*)xar_obtinere(res->interrogationes,
                i);

            si (x->unitas != u)
            {
                perge;
            }
            si (prima)
            {
                chorda_aedificator_appendere_literis(a,
                    "<p class=\"nota\">");
                si (fons.mensura > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_literis(a,
                        "<a href=\"../");
                    _effugere(a, fons);
                    chorda_aedificator_appendere_literis(a,
                        "\"><code>");
                    _effugere(a, fons);
                    chorda_aedificator_appendere_literis(a,
                        "</code></a> · ");
                }
                chorda_aedificator_appendere_literis(a,
                    "<a href=\"../");
                _effugere(a, x->via);
                chorda_aedificator_appendere_literis(a, "\"><code>");
                _effugere(a, x->via);
                chorda_aedificator_appendere_literis(a,
                    "</code></a></p>\n<ol class=\"quaestiones\">\n");
                prima = FALSUM;
            }
            ordo++;
            chorda_aedificator_appendere_literis(a, "<li id=\"");
            _effugere(a, u->titulus);
            chorda_aedificator_appendere_character(a, '-');
            chorda_aedificator_appendere_i32(a, ordo);
            chorda_aedificator_appendere_literis(a,
                "\"><span class=\"status\">");
            _effugere(a, x->genus);
            chorda_aedificator_appendere_literis(a, "</span>");
            _effugere(a, x->quaestio);
            chorda_aedificator_appendere_literis(a,
                "\n<div class=\"responsum\">");
            _effugere(a, x->responsum);
            chorda_aedificator_appendere_literis(a,
                "<br><span class=\"nota\">");
            _ancoras_scribere(a, x);
            chorda_aedificator_appendere_literis(a,
                "</span></div></li>\n");
        }
        chorda_aedificator_appendere_literis(a, "</ol>\n");
    }
    chorda_aedificator_appendere_literis(a, "\n</body>\n</html>\n");

    si (!filum_scribere(via, chorda_aedificator_finire(a)))
    {
        fprintf(stderr, "quid: %s scribi nequit\n", via);
        redde II;
    }
    redde ZEPHYRUM;
}
