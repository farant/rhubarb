/* quaestio.c - quaestio una ad vates (vide caput) */

#include "quaestio.h"
#include "xar.h"
#include "chorda_aedificator.h"
#include "materia_nodus.h"
#include "md_arbor.h"
#include "md_lexicon.h"
#include "md_registrum.h"

#include <stdio.h>
#include <string.h>

/* codex Unicode -> ASCII (NIHIL = '?') */
interior constans character*
ascii_substitutum (
    i32 codex)
{
    si (   codex == 0x2018 || codex == 0x2019 || codex == 0x201A
        || codex == 0x201B || codex == 0x2032)
    {
        redde "'";
    }
    si (   codex == 0x201C || codex == 0x201D || codex == 0x201E
        || codex == 0x2033)
    {
        redde "\"";
    }
    si (   codex == 0x2010 || codex == 0x2011 || codex == 0x2012
        || codex == 0x2013 || codex == 0x2014 || codex == 0x2212)
    {
        redde "-";
    }
    si (codex == 0x2026)
    {
        redde "...";
    }
    si (codex == 0x00A0 || codex == 0x2009 || codex == 0x202F)
    {
        redde " ";
    }
    si (codex == 0x2022)
    {
        redde "*";
    }
    redde NIHIL;
}

/* textus UTF-8 -> ASCII: '\r' tollitur, '\n' manet, imperia cetera ->
 * spatium, multi-octeti per ascii_substitutum aut '?' */
interior chorda
ad_ascii (
    chorda  textus,
   Piscina* piscina)
{
    chorda  c;
        i8* q;
       s32  i;
       s32  n;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    /* maximum: '...' ex III octetis = III - numquam longior */
    q = (i8*)piscina_allocare(piscina, (memoriae_index)(textus.mensura
        + I));
    si (!q)
    {
        redde c;
    }
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < (s32)textus.mensura; i++)
    {
        i8 b = textus.datum[i];

        si (b < 0x80)
        {
            si (b == (i8)'\r')
            {
                perge;
            }
            q[n++] = (b == (i8)'\n' || b == (i8)'\t') ? b
                   : (b < 0x20 || b == 0x7F) ? (i8)' ' : b;
            perge;
        }
        {
            s32 longitudo = (b & 0xE0) == 0xC0 ? II
                          : (b & 0xF0) == 0xE0 ? III
                          : (b & 0xF8) == 0xF0 ? IV : I;
            i32 codex = longitudo == II ? (i32)(b & 0x1F)
                      : longitudo == III ? (i32)(b & 0x0F)
                      : (i32)(b & 0x07);
            constans character* sub;
                           s32  k;

            si (longitudo == I || i + longitudo > (s32)textus.mensura)
            {
                q[n++] = (i8)'?';
                perge;
            }
            per (k = I; k < longitudo; k++)
            {
                codex = (codex << VI) | (i32)(textus.datum[i
                    + k] & 0x3F);
            }
            i    += longitudo - I;
            sub  = ascii_substitutum(codex);
            si (!sub)
            {
                q[n++] = (i8)'?';
                perge;
            }
            per (k = ZEPHYRUM; sub[k]; k++)
            {
                q[n++] = (i8)sub[k];
            }
        }
    }
    c.datum    = q;
    c.mensura  = (i32)n;
    redde c;
}


/* ==================================================
 * Markdown -> lineae logicae (vide caput: forma responsi)
 * ================================================== */

nomen structura {
    ChordaAedificator* a;
                  b32  vacua_pendens;   /* linea vacua in fonte ante */
                  b32  aliquid;         /* linea iam emissa */
} Planum;

/* tabulas ad spatia (columna IV) post columnam datam; redde columnam */
interior s32
expandere (
    ChordaAedificator* a,
               chorda  c,
                  s32  columna)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] == (i8)'\t')
        {
            fac
            {
                (vacuum)chorda_aedificator_appendere_character(a, ' ');
                columna++;
            }
            dum ((columna % IV) != ZEPHYRUM);
        }
        alioquin si (c.datum[i] != (i8)'\n' && c.datum[i] != (i8)'\r')
        {
            (vacuum)chorda_aedificator_appendere_character(a,
                (character)c.datum[i]);
            columna++;
        }
    }
    redde columna;
}

/* lexemata inlinea in aedificatorem (textus, codex, effugia sine '\');
 * fracturae spatium; delimitatores, caudae nexuum, derivata omittuntur */
interior vacuum
inlinea_colligere (
        ChordaAedificator* a,
    constans MateriaValor* v);

interior vacuum
inlinea_nodi (
        ChordaAedificator* a,
    constans MateriaNodus* n)
{
    i32 i;

    si (!n)
    {
        redde;
    }
    si (   n->genus == (s32)MD_GENUS_FRACTURA_MOLLIS
        || n->genus == (s32)MD_GENUS_FRACTURA_DURA)
    {
        (vacuum)chorda_aedificator_appendere_character(a, ' ');
        redde;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        inlinea_colligere(a, &n->loci[i]);
    }
}

interior vacuum
inlinea_colligere (
        ChordaAedificator* a,
    constans MateriaValor* v)
{
    si (v->genus == MATERIA_VALOR_NODUS)
    {
        inlinea_nodi(a, v->datum.nodus);
    }
    alioquin si (v->genus == MATERIA_VALOR_TOKEN && v->datum.token)
    {
        constans MateriaToken* t = v->datum.token;
        si (   t->genus == (s32)MD_LEX_TEXTUS
            || t->genus == (s32)MD_LEX_GRAVIS
            || t->genus == (s32)MD_LEX_ENS
            || t->genus == (s32)MD_LEX_AUTONEXUS
            || t->genus == (s32)MD_LEX_HTML)
        {
            (vacuum)expandere(a, t->valor, ZEPHYRUM);
        }
        alioquin si (   t->genus == (s32)MD_LEX_EFFUGIUM
                     && t->valor.mensura > I)
        {
            chorda sine;
            sine.datum    = t->valor.datum + I;
            sine.mensura  = t->valor.mensura - I;
            (vacuum)expandere(a, sine, ZEPHYRUM);
        }
    }
    alioquin si (v->genus == MATERIA_VALOR_LISTA)
    {
        i32 m = materia_valor_lista_numerus(*v);
        i32 j;
        per (j = ZEPHYRUM; j < m; j++)
        {
            constans MateriaValor* e = materia_valor_lista_obtinere(*v,
                j);
            si (e)
            {
                inlinea_colligere(a, e);
            }
        }
    }
}

/* praefixa lineae: indentatio -> spatia, marcae puncti -> "- ",
 * numeri et citationis manent; cetera omittuntur */
interior vacuum
praefixa_colligere (
        ChordaAedificator* a,
    constans MateriaValor* v)
{
    si (v->genus == MATERIA_VALOR_TOKEN && v->datum.token)
    {
        constans MateriaToken* t = v->datum.token;
        si (t->genus == (s32)MD_LEX_INDENTATIO)
        {
            (vacuum)expandere(a, t->valor, ZEPHYRUM);
        }
        alioquin si (t->genus == (s32)MD_LEX_MARCA_PUNCTI)
        {
            (vacuum)chorda_aedificator_appendere_literis(a, "- ");
        }
        alioquin si (   t->genus == (s32)MD_LEX_MARCA_NUMERI
                     || t->genus == (s32)MD_LEX_MARCA_CITATIONIS)
        {
            chorda c = chorda_praecidere(t->valor);
            (vacuum)chorda_aedificator_appendere_chorda(a, c);
            (vacuum)chorda_aedificator_appendere_character(a, ' ');
        }
    }
    alioquin si (v->genus == MATERIA_VALOR_LISTA)
    {
        i32 m = materia_valor_lista_numerus(*v);
        i32 j;
        per (j = ZEPHYRUM; j < m; j++)
        {
            constans MateriaValor* e = materia_valor_lista_obtinere(*v,
                j);
            si (e)
            {
                praefixa_colligere(a, e);
            }
        }
    }
}

/* omnia lexemata valoris (sine derivatis) - fons crudus */
interior vacuum
crudum_colligere (
        ChordaAedificator* a,
    constans MateriaValor* v)
{
    si (v->genus == MATERIA_VALOR_TOKEN && v->datum.token)
    {
        si (v->datum.token->genus != (s32)MD_LEX_DERIVATUM)
        {
            (vacuum)chorda_aedificator_appendere_chorda(a,
                v->datum.token->valor);
        }
    }
    alioquin si (v->genus == MATERIA_VALOR_NODUS && v->datum.nodus)
    {
        i32 i;
        per (i = ZEPHYRUM; i < v->datum.nodus->numerus_locorum; i++)
        {
            crudum_colligere(a, &v->datum.nodus->loci[i]);
        }
    }
    alioquin si (v->genus == MATERIA_VALOR_LISTA)
    {
        i32 m = materia_valor_lista_numerus(*v);
        i32 j;
        per (j = ZEPHYRUM; j < m; j++)
        {
            constans MateriaValor* e = materia_valor_lista_obtinere(*v,
                j);
            si (e)
            {
                crudum_colligere(a, e);
            }
        }
    }
}

/* linea logica emittere: linea vacua ante si fons eam habuit */
interior vacuum
lineam_emittere (
         Planum* pl,
         chorda  linea,
            b32  verbatim)
{
    si (pl->vacua_pendens && pl->aliquid)
    {
        (vacuum)chorda_aedificator_appendere_character(pl->a, '\n');
    }
    pl->vacua_pendens = FALSUM;
    si (verbatim)
    {
        (vacuum)chorda_aedificator_appendere_character(pl->a, '\t');
    }
    (vacuum)chorda_aedificator_appendere_chorda(pl->a, linea);
    (vacuum)chorda_aedificator_appendere_character(pl->a, '\n');
    pl->aliquid = VERUM;
}

/* textus lineae: spatia continua ad unum, extrema demptis (praefixa
 * ante dimissa: spatia ducentia eius manent) */
interior chorda
textum_purgare (
     chorda  praefixa,
     chorda  textus,
    Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(praefixa.mensura + textus.mensura + IV));
    i32 i;
    b32 spatium = FALSUM;
    b32 initium = VERUM;

    (vacuum)chorda_aedificator_appendere_chorda(a, praefixa);
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        i8 c = textus.datum[i];
        si (c == (i8)' ' || c == (i8)'\t' || c == (i8)'\n')
        {
            spatium = !initium;
            perge;
        }
        si (spatium)
        {
            (vacuum)chorda_aedificator_appendere_character(a, ' ');
            spatium = FALSUM;
        }
        (vacuum)chorda_aedificator_appendere_character(a, (character)c);
        initium = FALSUM;
    }
    redde chorda_aedificator_finire(a);
}

interior vacuum
blocum_planare (
                   Planum* pl,
    constans MateriaNodus* n,
                  Piscina* piscina);

interior vacuum
valorem_planare (
                   Planum* pl,
    constans MateriaValor* v,
                  Piscina* piscina)
{
    si (v->genus == MATERIA_VALOR_NODUS)
    {
        blocum_planare(pl, v->datum.nodus, piscina);
    }
    alioquin si (v->genus == MATERIA_VALOR_LISTA)
    {
        i32 m = materia_valor_lista_numerus(*v);
        i32 j;
        per (j = ZEPHYRUM; j < m; j++)
        {
            constans MateriaValor* e = materia_valor_lista_obtinere(*v,
                j);
            si (e)
            {
                valorem_planare(pl, e, piscina);
            }
        }
    }
}

interior vacuum
blocum_planare (
                   Planum* pl,
    constans MateriaNodus* n,
                  Piscina* piscina)
{
     ChordaAedificator* pa;
     ChordaAedificator* ta;
                   i32  i;

    si (!n)
    {
        redde;
    }
    commutatio (n->genus)
    {
        casus MD_GENUS_LINEA_VACUA:
            pl->vacua_pendens = VERUM;
            redde;
        casus MD_GENUS_DEFINITIO_NEXUS:
        casus MD_GENUS_PRAEFATIO:
            redde;
        casus MD_GENUS_PARAGRAPHUS:
        casus MD_GENUS_CAPITULUM:
            pa = chorda_aedificator_creare(piscina,
                (memoriae_index)XXXII);
            ta = chorda_aedificator_creare(piscina,
                (memoriae_index)CXXVIII);
            praefixa_colligere(pa, &n->loci[n->genus
                == (s32)MD_GENUS_PARAGRAPHUS
                ? (i32)MD_PARAGRAPHUS_PRAEFIXA : (i32)MD_CAPITULUM_PRAEFIXA]);
            si (n->genus == (s32)MD_GENUS_CAPITULUM)
            {
                (vacuum)chorda_aedificator_appendere_literis(pa, "# ");
            }
            inlinea_colligere(ta, &n->loci[n->genus
                == (s32)MD_GENUS_PARAGRAPHUS
                ? (i32)MD_PARAGRAPHUS_INLINEA : (i32)MD_CAPITULUM_INLINEA]);
            lineam_emittere(pl,
                textum_purgare(chorda_aedificator_finire(pa),
                chorda_aedificator_finire(ta), piscina), FALSUM);
            redde;
        casus MD_GENUS_SAEPTUM:
        {
            constans MateriaValor* lineae =
                &n->loci[(i32)MD_SAEPTUM_LINEAE];
            i32 m = lineae->genus == MATERIA_VALOR_LISTA
                ? materia_valor_lista_numerus(*lineae) : ZEPHYRUM;
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*lineae, j);
                constans MateriaNodus* l;
                                  s32  columna = ZEPHYRUM;

                si (!e || e->genus != MATERIA_VALOR_NODUS)
                {
                    perge;
                }
                l = e->datum.nodus;
                ta = chorda_aedificator_creare(piscina,
                    (memoriae_index)CXXVIII);
                /* indentatio PROPRIA codicis sola (non continentium) */
                {
                    constans MateriaValor* pr =
                        &l->loci[(i32)MD_LINEA_PRAEFIXA];
                    i32 pm = pr->genus == MATERIA_VALOR_LISTA
                        ? materia_valor_lista_numerus(*pr) : ZEPHYRUM;
                    i32 k;
                    per (k = ZEPHYRUM; k < pm; k++)
                    {
                        constans MateriaValor* x =
                            materia_valor_lista_obtinere(*pr, k);
                        si (   x && x->genus == MATERIA_VALOR_TOKEN
                            && x->datum.token
                            && x->datum.token->genus
                                == (s32)MD_LEX_INDENTATIO_CODICIS)
                        {
                            columna = expandere(ta,
                                x->datum.token->valor, columna);
                        }
                    }
                    si (   pr->genus == MATERIA_VALOR_TOKEN
                        && pr->datum.token
                        && pr->datum.token->genus
                            == (s32)MD_LEX_INDENTATIO_CODICIS)
                    {
                        columna = expandere(ta, pr->datum.token->valor,
                            columna);
                    }
                }
                {
                    ChordaAedificator* ca =
                        chorda_aedificator_creare(piscina,
                        (memoriae_index)CXXVIII);
                    crudum_colligere(ca,
                        &l->loci[(i32)MD_LINEA_CONTENTUM]);
                    (vacuum)expandere(ta, chorda_aedificator_finire(ca),
                        columna);
                }
                lineam_emittere(pl, chorda_aedificator_finire(ta),
                    VERUM);
            }
            redde;
        }
        casus MD_GENUS_DOCUMENTUM:
        casus MD_GENUS_CITATIO:
        casus MD_GENUS_LISTA:
        casus MD_GENUS_ELEMENTUM:
            per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
            {
                valorem_planare(pl, &n->loci[i], piscina);
            }
            redde;
        ordinarius:
        {
            /* tabulae, html, divisiones: fons crudus, linea per lineam,
             * verbatim */
            chorda c;
               i32 a;
               i32 k;

            ta = chorda_aedificator_creare(piscina,
                (memoriae_index)CCLVI);
            per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
            {
                crudum_colligere(ta, &n->loci[i]);
            }
            c = chorda_aedificator_finire(ta);
            a = ZEPHYRUM;
            per (k = ZEPHYRUM; k <= c.mensura; k++)
            {
                si (k == c.mensura || c.datum[k] == (i8)'\n')
                {
                               chorda  linea;
                    ChordaAedificator* la =
                        chorda_aedificator_creare(piscina,
                        (memoriae_index)(k - a + XVI));
                    linea.datum    = c.datum + a;
                    linea.mensura  = k - a;
                    (vacuum)expandere(la, linea, ZEPHYRUM);
                    linea = chorda_aedificator_finire(la);
                    si (linea.mensura > ZEPHYRUM)
                    {
                        lineam_emittere(pl, linea, VERUM);
                    }
                    a = k + I;
                }
            }
            redde;
        }
    }
}

/* textus markdown -> lineae logicae (sine '\n' finali) */
interior chorda
markdown_planare (
     chorda  textus,
    Piscina* piscina)
{
    Planum pl;
    MateriaNodus* radix;
    chorda c;

    pl.a              = chorda_aedificator_creare(piscina,
        (memoriae_index)(textus.mensura * II + XVI));
    pl.vacua_pendens  = FALSUM;
    pl.aliquid        = FALSUM;
    radix = md_arbor_parsare(piscina, chorda_ut_cstr(textus, piscina),
        textus.mensura);
    si (!radix)
    {
        redde textus;
    }
    blocum_planare(&pl, radix, piscina);
    c = chorda_aedificator_finire(pl.a);
    dum (c.mensura > ZEPHYRUM && c.datum[c.mensura - I] == (i8)'\n')
    {
        c.mensura--;
    }
    redde c;
}

interior QuaestioResponsum
errorem_reddere (
    constans character* causa,
               Piscina* piscina)
{
    QuaestioResponsum r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.error = chorda_ex_literis(causa, piscina);
    redde r;
}

QuaestioResponsum
quaestio_rogare (
        Vates* vates,
       chorda  exemplar,
       chorda  quaestio,
          s32  characteres,
          s32  latitudo,
      Piscina* piscina)
{
     QuaestioResponsum  r;
          VatesPetitio* petitio;
          VatesNuntius* nuntius;
        VatesResponsum* responsum;
             character  systema[CCCLXXXIV];
                chorda  textus;
                   i32  k;

    si (!piscina)
    {
        memset(&r, ZEPHYRUM, magnitudo(r));
        redde r;
    }
    si (!vates)
    {
        redde errorem_reddere("nullus vates", piscina);
    }
    si (quaestio.mensura <= ZEPHYRUM)
    {
        redde errorem_reddere("quaestio vacua", piscina);
    }
    si (characteres <= ZEPHYRUM)
    {
        redde errorem_reddere("spatium nullum", piscina);
    }
    petitio = vates_petitio_creare(piscina, exemplar.mensura > ZEPHYRUM
        ? exemplar : chorda_ex_literis(QUAESTIO_EXEMPLAR, piscina));
    /* signa ~ characteres / IV; margo pro verbis longis */
    vates_petitio_maxima_ponere(petitio, characteres / III + LXIV);
    sprintf(systema, "Answer in plain ASCII text only: no Markdown, no "
        "lists, no headings. The whole answer must fit in at most %d "
        "characters; it will be shown on a fixed-width page %d columns "
        "wide. Be direct.", (integer)characteres, (integer)latitudo);
    vates_systema_addere(petitio, chorda_ex_literis(systema, piscina),
        FALSUM);
    nuntius = vates_nuntium_addere(petitio, VATES_USOR);
    (vacuum)vates_textum_addere(nuntius, quaestio);
    responsum = vates_mittere(vates, petitio,
        chorda_ex_literis("quaestio",
        piscina), piscina);
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.usus = responsum->usus;
    si (!responsum->successus)
    {
        r.error =
            chorda_ex_literis(vates_error_descriptio(responsum->error),
            piscina);
        si (responsum->error_nuntius.mensura > ZEPHYRUM)
        {
            r.error = chorda_concatenare(r.error, chorda_concatenare(
                chorda_ex_literis(": ", piscina),
                responsum->error_nuntius,
                piscina), piscina);
        }
        redde r;
    }
    textus.datum    = NIHIL;
    textus.mensura  = ZEPHYRUM;
    per (k = ZEPHYRUM; responsum->bloci
        && k < xar_numerus(responsum->bloci);
         k++)
    {
        /* bloci in Xar ut structurae (non indices - ut probatio_vates
         * legit), quamvis caput "VatesBlocus*" dicat */
        VatesBlocus* b = (VatesBlocus*)xar_obtinere(responsum->bloci,
            k);
        si (b && b->genus == VATES_TEXTUS)
        {
            textus = chorda_concatenare(textus, b->textus, piscina);
        }
    }
    si (textus.mensura <= ZEPHYRUM)
    {
        r.error = chorda_ex_literis(responsum->causa_finis
            == VATES_FINIS_RECUSATIO ? "recusatio" : "responsum vacuum",
            piscina);
        redde r;
    }
    r.textus     = markdown_planare(ad_ascii(textus, piscina), piscina);
    r.successus  = VERUM;
    redde r;
}
