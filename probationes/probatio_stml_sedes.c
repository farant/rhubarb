/* probatio_stml_sedes.c - tabula sedium scriptoris contra
 * parsatorem
 *
 * DUO ORACULA GENERUM DIVERSORUM: scriptor sedes notat dum
 * scribit (stml_scribere_sedibus); parsator easdem sedes in
 * textu emisso independenter invenit (positus_initium/finis,
 * bd623d46). Scribere -> relegere -> conferre: si tabula et
 * parsator de extensione ullius elementi dissentiunt, alter
 * mentitur - et uterque aliunde venit, ergo error communis
 * fingi non potest.
 *
 * ORDO: tabula post-ordinem clausurae fert (nodus notatur ubi
 * scriptio eius finitur), ergo ambulatio arboris relectae
 * post-ordine eundem ordinem parit - collatio par-ad-par sine
 * quaesitione identitatis.
 *
 * CONSUMENS PRIMUS: inspector nexus (laboratorium 0032) - nodus
 * silvae extensionem C et extensionem STML simul ferat.
 */
#include "latina.h"
#include "stml.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include <stdio.h>

/* Elementa post-ordine colligere (liberi ante parentem, fratres
 * ordine documenti) - idem ordo quo scriptor notat. */
interior vacuum
_elementa_postordine (
    StmlNodus* nodus,
          Xar* exitus)
{
    i32 i;

    si (nodus == NIHIL)
    {
        redde;
    }
    si (nodus->liberi != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(nodus->liberi); i++)
        {
            StmlNodus* liberum;

            liberum = *(StmlNodus**)xar_obtinere(nodus->liberi, i);
            _elementa_postordine(liberum, exitus);
        }
    }
    si (nodus->genus == STML_NODUS_ELEMENTUM)
    {
        StmlNodus** cella;

        cella = xar_addere(exitus);
        si (cella != NIHIL)
        {
            *cella = nodus;
        }
    }
}

/* Circuitus unus: fixturam parsare, cum tabula scribere,
 * relegere, tabulam contra positus parsatoris conferre. */
interior vacuum
_circuitum_probare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
     constans character* fixtura,
                    b32  pulchrum,
                    i32  elementa_exspectata)
{
    StmlResultus  res_a;
    StmlResultus  res_b;
             Xar* tabula;
             Xar* elementa_b;
          chorda  fons;
          chorda  emissum;
             i32  i;

    imprimere("\n--- %s (pulchrum=%d) ---\n", titulus,
              (integer)pulchrum);

    fons   = chorda_ex_literis(fixtura, piscina);
    res_a  = stml_legere(fons, piscina, intern);
    CREDO_VERUM (res_a.successus);
    CREDO_NON_NIHIL (res_a.radix);
    si (!res_a.successus || res_a.radix == NIHIL)
    {
        redde;
    }

    tabula = xar_creare(piscina, magnitudo(StmlSedesNodi));
    CREDO_NON_NIHIL (tabula);

    emissum = stml_scribere_sedibus(res_a.radix, piscina,
                                    pulchrum, tabula);
    CREDO_CHORDA_NON_VACUA (emissum);

    res_b = stml_legere(emissum, piscina, intern);
    CREDO_VERUM (res_b.successus);
    si (!res_b.successus || res_b.radix == NIHIL)
    {
        redde;
    }

    elementa_b = xar_creare(piscina, magnitudo(StmlNodus*));
    _elementa_postordine(res_b.radix, elementa_b);

    CREDO_AEQUALIS_I32 (xar_numerus(tabula),
                        elementa_exspectata);
    CREDO_AEQUALIS_I32 (xar_numerus(elementa_b),
                        elementa_exspectata);
    si (xar_numerus(tabula) != xar_numerus(elementa_b))
    {
        redde;
    }

    per (i = ZEPHYRUM; i < xar_numerus(tabula); i++)
    {
        StmlSedesNodi* nota;
            StmlNodus* par;

        nota  = (StmlSedesNodi*)xar_obtinere(tabula, i);
        par   = *(StmlNodus**)xar_obtinere(elementa_b, i);
        CREDO_NON_NIHIL (nota);
        CREDO_NON_NIHIL (par);
        si (nota == NIHIL || par == NIHIL)
        {
            perge;
        }

        /* extensio scriptoris == extensio parsatoris */
        CREDO_AEQUALIS_I32 (nota->initium, par->positus_initium);
        CREDO_AEQUALIS_I32 (nota->finis,   par->positus_finis);

        /* fetta incipit tago aperienti */
        CREDO_MINOR_I32 (nota->initium, (i32)emissum.mensura);
        CREDO_VERUM (emissum.datum[nota->initium] == (i8)'<');

        /* titulus congruit (fragmenta titulum NIHIL ferre
         * possunt - tunc uterque) */
        si (nota->nodus->titulus != NIHIL && par->titulus != NIHIL)
        {
            CREDO_CHORDA_AEQUALIS (*nota->nodus->titulus,
                                   *par->titulus);
        }
        alioquin
        {
            CREDO_VERUM ((nota->nodus->titulus == NIHIL)
                         == (par->titulus == NIHIL));
        }
    }
}

s32
principale (
    vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    /* elementa: radix, caput, linea, linea, vacuum-elementum,
     * corpus = VI */
    constans character* FIXTURA_ORDINARIA =
        "<radix genus=\"probatio\">\n"
        "  <caput versio=\"2\" activum>titulus</caput>\n"
        "  <corpus>\n"
        "    <linea>prima</linea>\n"
        "    <linea>secunda</linea>\n"
        "    <vacuum-elementum/>\n"
        "  </corpus>\n"
        "</radix>\n";

    /* elementa: intra, #frag1, crudum, curta, par, summa = VI
     * (transclusio genus proprium fert - non numeratur) */
    constans character* FIXTURA_EXOTICA =
        "<summa>\n"
        "  <#frag1><intra>textus</intra></#>\n"
        "  <<#frag1>>\n"
        "  <crudum!>a < b &amp; c</crudum>\n"
        "  <curta>x</>\n"
        "  <par clavis=\"valor\"/>\n"
        "</summa>\n";

    /* elementa: d, e, c, b, a = V. Modo pulchro spina fit
     * ('<a(> <b(>' + c multiplex '((>' stackata) - fixtura prima
     * quae conversionem capturae in porta sedium exercet: ordo
     * post-ordinis notarum captorum hic iudicatur (quaestio
     * 01M0WVG98N - sarcinator vincula dum scribit notabat,
     * prae-ordine). */
    constans character* FIXTURA_SPINAE =
        "<a><b><c><d/><e/></c></b></a>";

    piscina = piscina_generare_dynamicum("probatio_stml_sedes",
                                         1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    _circuitum_probare(piscina, intern, "ordinaria",
                       FIXTURA_ORDINARIA, FALSUM, (i32)VI);
    _circuitum_probare(piscina, intern, "ordinaria",
                       FIXTURA_ORDINARIA, VERUM, (i32)VI);
    _circuitum_probare(piscina, intern, "exotica",
                       FIXTURA_EXOTICA, FALSUM, (i32)VI);
    _circuitum_probare(piscina, intern, "exotica",
                       FIXTURA_EXOTICA, VERUM, (i32)VI);
    _circuitum_probare(piscina, intern, "spinae",
                       FIXTURA_SPINAE, FALSUM, (i32)V);
    _circuitum_probare(piscina, intern, "spinae",
                       FIXTURA_SPINAE, VERUM, (i32)V);

    /* sedes NIHIL = stml_scribere ad octetum; et ANCORA ABSOLUTA:
     * paritas duorum oraculorum consensum probat, extensio manu
     * numerata INTENTIONEM ipsam figit (ne ambo idem falsum
     * concedant). "<radix><a/><b>t</b></radix>" non-pulchre:
     * a=[7,11) b=[11,19) radix=[0,27), post-ordo a,b,radix. */
    {
        StmlResultus  res;
              chorda  fons;
              chorda  a;
              chorda  b;
                 Xar* tabula;

        imprimere("\n--- sedes NIHIL + ancora absoluta ---\n");
        fons = chorda_ex_literis(
            "<radix><a/><b>t</b></radix>", piscina);
        res = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            a = stml_scribere(res.radix, piscina, FALSUM);
            b = stml_scribere_sedibus(res.radix, piscina,
                                      FALSUM, NIHIL);
            CREDO_CHORDA_AEQUALIS (a, b);

            tabula = xar_creare(piscina,
                                magnitudo(StmlSedesNodi));
            (vacuum)stml_scribere_sedibus(res.radix, piscina,
                                          FALSUM, tabula);
            CREDO_AEQUALIS_I32 (xar_numerus(tabula), (i32)III);
            si (xar_numerus(tabula) == (i32)III)
            {
                StmlSedesNodi* n;

                n = (StmlSedesNodi*)xar_obtinere(tabula,
                                                 ZEPHYRUM);
                CREDO_AEQUALIS_I32 (n->initium, (i32)VII);
                CREDO_AEQUALIS_I32 (n->finis,   (i32)XI);
                n = (StmlSedesNodi*)xar_obtinere(tabula, I);
                CREDO_AEQUALIS_I32 (n->initium, (i32)XI);
                CREDO_AEQUALIS_I32 (n->finis,   (i32)XIX);
                n = (StmlSedesNodi*)xar_obtinere(tabula, II);
                CREDO_AEQUALIS_I32 (n->initium, (i32)ZEPHYRUM);
                CREDO_AEQUALIS_I32 (n->finis,   (i32)XXVII);
            }
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    si (credo_omnia_praeterierunt())
    {
        credo_claudere();
        redde ZEPHYRUM;
    }
    credo_claudere();
    redde I;
}
