/* sessio.c - machina sessionis M4b, chunk A (vide sessio.h)
 *
 * Receptum iudicii = examen.c fidele (contextus + systema lexicon +
 * capita praebita + oraculum praeseminatum + BIS-ANALYSIS). Piscinae:
 * app (vita sessionis: turni, relata) + silvae perennis (ctx +
 * systema) + silvae effimera PER OBLATIONEM (parsurae + semantica —
 * destructa ante reditum; disciplina generationum chunk B plena).
 *
 * Duo textus (C3): OSTENSUS (quem usor typavit — artefactum) et
 * INVOLUTUS (sententiae in `void turnus_N(void){…}` — iudicatus et
 * demittendus). Tabula offset linearum per turnum mappat positiones
 * iudicatas in ostensas.
 */

#include "sessio.h"
#include "praeparator.h"

#include "latina.h"
#include "xar.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "silva.h"

#include "officina_medulla.h"
#include "officina_demissio.h"
#include "officina_regio.h"
#include "officina_conexio.h"
#include "officina_machinula.h"

#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define SESSIO_VIA_DOCUMENTI "<sessio>"

/* ==================================================
 * structurae internae
 * ================================================== */

nomen structura {
    chorda textus;       /* finitur '\n'; in piscina sessionis */
    s32    genus;        /* SESSIO_TURNUS_* */
    chorda titulus;      /* declaratum (religatio); vacua nisi */
    chorda involucrum;   /* "turnus_N" si involvendus; vacua nisi */
    i32    lineae;       /* numerus linearum textus */
    /* acta (chunk B): effusio capta replicationis novissimae */
    chorda effusio;
    chorda effusio_erroris;
    b32    effusio_valida;
    /* impressio (chunk C): typus expressionis (involucrum typatum) */
    chorda typus_textus;
    s32    valor_genus;
} TurnusInternus;

/* visus operans per oblationem: turni exsistentes (substitutione
 * applicata) + candidatus appensus */
nomen structura {
    chorda textus;
    s32    genus;
    chorda involucrum;
    chorda typus_textus;        /* involucrum typatum (expressio) */
    i32    lineae;
    i32    ostensum_initium;    /* linea prima ostensa (1-basata) */
    i32    involutum_initium;   /* linea prima TEXTUS in involuto */
    i32    involutum_primus;    /* linea prima spatii pleni */
    i32    involutum_ultimus;   /* linea ultima spatii pleni */
} TurnusVisus;

structura Sessio {
    Piscina*        piscina;
    Praeparatio     praeparatio;       /* ctx + systema (praeparator
                                        * - unitas communis) */
    Xar*            turni;             /* TurnusInternus */
    i32             numerator;         /* involucra turnus_N */
    /* mundus (chunk B): bibliothecae perennes + generatio currens */
    Piscina*        piscina_bibliothecarum;  /* moduli demissi semel */
    Xar*            moduli_bibliothecarum;   /* MedullaModulus* */
    Piscina*        piscina_generationis;    /* mundus currens - per
                                              * turnum demolitur */
    Regio*          regio;
    Conexio*        conexio;
    Machinula*      machinula;
    i32             vexilla_recusationum;
};

/* ==================================================
 * auxilia chordarum et linearum
 * ================================================== */

interior chorda
_chorda_vacua (vacuum)
{
    chorda c;

    c.mensura = ZEPHYRUM;
    c.datum = NIHIL;
    redde c;
}

interior i32
_lineas_numerare (chorda textus)
{
    i32 n = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == (i8)'\n')
        {
            n++;
        }
    }
    redde n;
}

interior b32
_spatium_est (i8 c)
{
    redde (c == (i8)' ' || c == (i8)'\t' || c == (i8)'\n'
        || c == (i8)'\r') ? VERUM : FALSUM;
}

interior chorda
_praecisa (chorda textus)
{
    i32 a = ZEPHYRUM;
    i32 b = textus.mensura;

    dum (a < b && _spatium_est(textus.datum[a]))
    {
        a++;
    }
    dum (b > a && _spatium_est(textus.datum[b - I]))
    {
        b--;
    }
    /* chorda_sectio(s, initium, FINIS) - non mensura! (vitium
     * latens interludii: a>0 caudam praecidebat) */
    redde chorda_sectio(textus, a, b);
}

/* textus turni normalizatus: praecisus + '\n' finali (copia in
 * piscinam) */
interior chorda
_normalizare (Piscina* piscina, chorda textus, b32 cum_semicolo)
{
    chorda nucleus = _praecisa(textus);
    i32 m = nucleus.mensura + (cum_semicolo ? I : ZEPHYRUM) + I;
    i8* datum = piscina_allocare(piscina, (memoriae_index)m);
    chorda fructus;

    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    si (datum == NIHIL)
    {
        redde fructus;
    }
    memcpy(datum, nucleus.datum, (memoriae_index)nucleus.mensura);
    si (cum_semicolo)
    {
        datum[nucleus.mensura] = (i8)';';
    }
    datum[m - I] = (i8)'\n';
    fructus.datum = datum;
    fructus.mensura = m;
    redde fructus;
}

interior b32
_continet (constans i8* datum, i32 mensura, constans character* acus)
{
    i32 acus_m = (i32)strlen(acus);
    i32 k;

    si (acus_m == ZEPHYRUM || mensura < acus_m)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + acus_m <= mensura; k++)
    {
        si (memcmp(datum + k, acus, (memoriae_index)acus_m)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * textus texere (ostensus + involutus) super visus
 * ================================================== */

interior chorda
_ostensum_texere (TurnusVisus* visus, i32 numerus, Piscina* piscina)
{
    i32 summa = ZEPHYRUM;
    i32 k;
    i8* datum;
    i32 cursor = ZEPHYRUM;
    i32 linea = I;
    chorda fructus;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        summa += visus[k].textus.mensura;
    }
    datum = piscina_allocare(piscina,
        (memoriae_index)(summa > ZEPHYRUM ? summa : I));
    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    si (datum == NIHIL)
    {
        redde fructus;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        visus[k].ostensum_initium = linea;
        memcpy(datum + cursor, visus[k].textus.datum,
            (memoriae_index)visus[k].textus.mensura);
        cursor += visus[k].textus.mensura;
        linea += visus[k].lineae;
    }
    fructus.datum = datum;
    fructus.mensura = cursor;
    redde fructus;
}

interior b32
_involvendus (s32 genus)
{
    redde (genus == SESSIO_TURNUS_SENTENTIA
        || genus == SESSIO_TURNUS_EXPRESSIO) ? VERUM : FALSUM;
}

interior chorda
_involutum_texere (TurnusVisus* visus, i32 numerus, Piscina* piscina)
{
    i32 summa = ZEPHYRUM;
    i32 k;
    i8* datum;
    i32 cursor = ZEPHYRUM;
    i32 linea = I;
    chorda fructus;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        summa += visus[k].textus.mensura;
        si (_involvendus(visus[k].genus))
        {
            summa += visus[k].involucrum.mensura
                + visus[k].typus_textus.mensura + LXIV;
        }
    }
    datum = piscina_allocare(piscina,
        (memoriae_index)(summa > ZEPHYRUM ? summa : I));
    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    si (datum == NIHIL)
    {
        redde fructus;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        visus[k].involutum_primus = linea;
        si (_involvendus(visus[k].genus))
        {
            integer scripti;
            b32 typatum = (visus[k].genus == SESSIO_TURNUS_EXPRESSIO
                && visus[k].typus_textus.mensura > ZEPHYRUM)
                ? VERUM : FALSUM;

            si (typatum)
            {
                /* involucrum typatum (chunk C): valor per codex.
                 * `return` in linea propria - mappa linearum pura */
                scripti = sprintf((character*)(datum + cursor),
                    "%.*s %.*s(void)\n{\nreturn\n",
                    (int)visus[k].typus_textus.mensura,
                    (constans character*)visus[k].typus_textus.datum,
                    (int)visus[k].involucrum.mensura,
                    (constans character*)visus[k].involucrum.datum);
                cursor += (i32)scripti;
                linea += III;
            }
            alioquin
            {
                scripti = sprintf((character*)(datum + cursor),
                    "void %.*s(void)\n{\n",
                    (int)visus[k].involucrum.mensura,
                    (constans character*)visus[k].involucrum.datum);
                cursor += (i32)scripti;
                linea += II;
            }
            visus[k].involutum_initium = linea;
            memcpy(datum + cursor, visus[k].textus.datum,
                (memoriae_index)visus[k].textus.mensura);
            cursor += visus[k].textus.mensura;
            linea += visus[k].lineae;
            datum[cursor] = (i8)'}';
            datum[cursor + I] = (i8)'\n';
            cursor += II;
            visus[k].involutum_ultimus = linea;
            linea += I;
        }
        alioquin
        {
            visus[k].involutum_initium = linea;
            memcpy(datum + cursor, visus[k].textus.datum,
                (memoriae_index)visus[k].textus.mensura);
            cursor += visus[k].textus.mensura;
            linea += visus[k].lineae;
            visus[k].involutum_ultimus = linea - I;
        }
    }
    fructus.datum = datum;
    fructus.mensura = cursor;
    redde fructus;
}

/* mappa: linea involuta -> {index turni, linea ostensa} */
interior s32
_turnum_possidentem (constans TurnusVisus* visus, i32 numerus,
    i32 linea_involuta, i32* linea_ostensa_out)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (linea_involuta >= visus[k].involutum_primus
            && linea_involuta <= visus[k].involutum_ultimus)
        {
            i32 delta = linea_involuta - visus[k].involutum_initium;

            si (delta < ZEPHYRUM)
            {
                delta = ZEPHYRUM;
            }
            si (delta >= visus[k].lineae)
            {
                delta = visus[k].lineae > ZEPHYRUM
                    ? visus[k].lineae - I : ZEPHYRUM;
            }
            *linea_ostensa_out = visus[k].ostensum_initium + delta;
            redde (s32)k;
        }
    }
    *linea_ostensa_out = ZEPHYRUM;
    redde -I;
}

/* ==================================================
 * ambulationes arboris (amalgama)
 * ================================================== */

interior SilvaToken* _lexema_primum_valoris (SilvaValor v);

interior SilvaToken*
_lexema_primum_nodi (constans SilvaNodus* n)
{
    insignatus integer k;

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        SilvaToken* t = _lexema_primum_valoris(n->loci[k]);

        si (t != NIHIL)
        {
            redde t;
        }
    }
    redde NIHIL;
}

interior SilvaToken*
_lexema_primum_valoris (SilvaValor v)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_TOKEN:
            redde v.datum.token;
        casus SILVA_VALOR_NODUS:
            redde _lexema_primum_nodi(v.datum.nodus);
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    SilvaToken* t = _lexema_primum_valoris(*elem);

                    si (t != NIHIL)
                    {
                        redde t;
                    }
                }
            }
            redde NIHIL;
        }
        ordinarius:
            redde NIHIL;
    }
}

/* quaerere nodum generis dati (profunditate prima) */
interior constans SilvaNodus* _nodum_generis_valoris (SilvaValor v,
    integer genus);

interior constans SilvaNodus*
_nodum_generis_nodi (constans SilvaNodus* n, integer genus)
{
    insignatus integer k;

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (n->genus == genus)
    {
        redde n;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        constans SilvaNodus* f = _nodum_generis_valoris(n->loci[k],
            genus);

        si (f != NIHIL)
        {
            redde f;
        }
    }
    redde NIHIL;
}

interior constans SilvaNodus*
_nodum_generis_valoris (SilvaValor v, integer genus)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_NODUS:
            redde _nodum_generis_nodi(v.datum.nodus, genus);
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    constans SilvaNodus* f = _nodum_generis_valoris(
                        *elem, genus);

                    si (f != NIHIL)
                    {
                        redde f;
                    }
                }
            }
            redde NIHIL;
        }
        ordinarius:
            redde NIHIL;
    }
}

/* numerare nodos generis dati in subarbore */
interior i32 _numerare_generis_valoris (SilvaValor v, integer genus);

interior i32
_numerare_generis_nodi (constans SilvaNodus* n, integer genus)
{
    i32 summa = ZEPHYRUM;
    insignatus integer k;

    si (n == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (n->genus == genus)
    {
        summa++;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        summa += _numerare_generis_valoris(n->loci[k], genus);
    }
    redde summa;
}

interior i32
_numerare_generis_valoris (SilvaValor v, integer genus)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_NODUS:
            redde _numerare_generis_nodi(v.datum.nodus, genus);
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;
            i32 summa = ZEPHYRUM;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    summa += _numerare_generis_valoris(*elem, genus);
                }
            }
            redde summa;
        }
        ordinarius:
            redde ZEPHYRUM;
    }
}

/* linea radicis (positio fontis) lexematis primi elementi */
interior b32
_elementum_positio (constans SilvaNodus* elementum,
    integer* fons_index_out, i32* linea_out)
{
    SilvaToken* t = _lexema_primum_nodi(elementum);
    SilvaToken* radix;

    si (t == NIHIL)
    {
        redde FALSUM;
    }
    radix = silva_token_radix(t);
    si (radix == NIHIL)
    {
        radix = t;
    }
    *fons_index_out = radix->fons_index;
    *linea_out = (i32)radix->linea;
    redde VERUM;
}

/* ==================================================
 * classificatio
 * ================================================== */

interior s32
_genus_turni_ex_nodo (constans SilvaNodus* n)
{
    commutatio (n->genus)
    {
        casus SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS:
            redde SESSIO_TURNUS_DEFINITIO;
        casus SILVA_C89_GENUS_DECLARATIO:
            redde SESSIO_TURNUS_DECLARATIO;
        casus SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS:
            redde SESSIO_TURNUS_EXPRESSIO;
        ordinarius:
            redde SESSIO_TURNUS_SENTENTIA;
    }
}

/* ==================================================
 * redditor typorum (chunk C): TypusC89 -> textus latinus C-stili.
 * FALSUM = irreddibilis (acies/functio/error/aggregata nuda) -
 * impressio tunc tacet (recusatio nominata).
 * ================================================== */

interior b32
_typum_scribere (constans TypusC89* t, character* b, i32* cursor,
    i32 capacitas)
{
    constans character* nomen_p = NIHIL;

    si (t == NIHIL || *cursor + LXIV >= capacitas)
    {
        redde FALSUM;
    }
    commutatio (t->genus)
    {
        casus TYPUS_C89_PRIMITIVUS:
            commutatio (t->datum.primitivum)
            {
                casus PRIMITIVUM_VACUUM: nomen_p = "vacuum"; frange;
                casus PRIMITIVUM_CHARACTER:
                    nomen_p = "character"; frange;
                casus PRIMITIVUM_CHARACTER_SIGNATUM:
                    nomen_p = "signatus character"; frange;
                casus PRIMITIVUM_CHARACTER_INSIGNATUM:
                    nomen_p = "insignatus character"; frange;
                casus PRIMITIVUM_BREVIS: nomen_p = "brevis"; frange;
                casus PRIMITIVUM_BREVIS_INSIGNATUM:
                    nomen_p = "insignatus brevis"; frange;
                casus PRIMITIVUM_INTEGER: nomen_p = "integer"; frange;
                casus PRIMITIVUM_INTEGER_INSIGNATUM:
                    nomen_p = "insignatus integer"; frange;
                casus PRIMITIVUM_LONGUS: nomen_p = "longus"; frange;
                casus PRIMITIVUM_LONGUS_INSIGNATUM:
                    nomen_p = "insignatus longus"; frange;
                casus PRIMITIVUM_LONGUS_LONGUS:
                    nomen_p = "longus longus"; frange;
                casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
                    nomen_p = "insignatus longus longus"; frange;
                casus PRIMITIVUM_FLUITANS:
                    nomen_p = "fluitans"; frange;
                casus PRIMITIVUM_DUPLEX: nomen_p = "duplex"; frange;
                casus PRIMITIVUM_DUPLEX_LONGUS:
                    nomen_p = "duplex longus"; frange;
                ordinarius: redde FALSUM;
            }
            *cursor += (i32)sprintf(b + *cursor, "%s", nomen_p);
            redde VERUM;
        casus TYPUS_C89_MONSTRATOR:
            si (!_typum_scribere(t->datum.monstrator.internum, b,
                    cursor, capacitas))
            {
                redde FALSUM;
            }
            *cursor += (i32)sprintf(b + *cursor, "*");
            redde VERUM;
        casus TYPUS_C89_QUALIFICATUS:
            si (t->datum.qualificatus.quales
                & (insignatus integer)QUALIS_CONSTANS)
            {
                *cursor += (i32)sprintf(b + *cursor, "constans ");
            }
            redde _typum_scribere(t->datum.qualificatus.internum,
                b, cursor, capacitas);
        casus TYPUS_C89_STRUCTURA:
        casus TYPUS_C89_UNIO:
            si (t->datum.tag.titulus.mensura == ZEPHYRUM
                || *cursor + (i32)t->datum.tag.titulus.mensura + XVI
                    >= capacitas)
            {
                redde FALSUM;
            }
            *cursor += (i32)sprintf(b + *cursor, "%s %.*s",
                t->genus == TYPUS_C89_STRUCTURA ? "structura"
                    : "unio",
                (int)t->datum.tag.titulus.mensura,
                (constans character*)t->datum.tag.titulus.datum);
            redde VERUM;
        casus TYPUS_C89_ENUMERATUS:
            *cursor += (i32)sprintf(b + *cursor, "enumeratio");
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

interior chorda
_typum_reddere (constans TypusC89* t, Piscina* piscina)
{
    character buffer[CCLVI];
    i32 cursor = ZEPHYRUM;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (t == NIHIL
        || !_typum_scribere(t, buffer, &cursor, (i32)CCLVI))
    {
        redde vacua;
    }
    redde chorda_ex_literis(buffer, piscina);
}

interior constans TypusC89*
_qualibus_nudatum (constans TypusC89* t)
{
    dum (t != NIHIL && t->genus == TYPUS_C89_QUALIFICATUS)
    {
        t = t->datum.qualificatus.internum;
    }
    redde t;
}

interior s32
_valor_genus_ex_typo (constans TypusC89* t)
{
    t = _qualibus_nudatum(t);
    si (t == NIHIL)
    {
        redde SESSIO_VALOR_NULLUS;
    }
    commutatio (t->genus)
    {
        casus TYPUS_C89_PRIMITIVUS:
            commutatio (t->datum.primitivum)
            {
                casus PRIMITIVUM_VACUUM:
                    redde SESSIO_VALOR_NULLUS;
                casus PRIMITIVUM_FLUITANS:
                    redde SESSIO_VALOR_FLUITANS_32;
                casus PRIMITIVUM_DUPLEX:
                casus PRIMITIVUM_DUPLEX_LONGUS:
                    redde SESSIO_VALOR_FLUITANS_64;
                casus PRIMITIVUM_CHARACTER_INSIGNATUM:
                casus PRIMITIVUM_BREVIS_INSIGNATUM:
                casus PRIMITIVUM_INTEGER_INSIGNATUM:
                casus PRIMITIVUM_LONGUS_INSIGNATUM:
                casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
                    redde SESSIO_VALOR_INSIGNATUS;
                ordinarius:
                    redde SESSIO_VALOR_SIGNATUS;
            }
        casus TYPUS_C89_ENUMERATUS:
            redde SESSIO_VALOR_SIGNATUS;
        casus TYPUS_C89_STRUCTURA:
        casus TYPUS_C89_UNIO:
            redde SESSIO_VALOR_AGGREGATUM;
        casus TYPUS_C89_MONSTRATOR:
        {
            constans TypusC89* interior_t = _qualibus_nudatum(
                t->datum.monstrator.internum);

            si (interior_t != NIHIL
                && interior_t->genus == TYPUS_C89_PRIMITIVUS
                && (interior_t->datum.primitivum
                        == PRIMITIVUM_CHARACTER
                    || interior_t->datum.primitivum
                        == PRIMITIVUM_CHARACTER_SIGNATUM
                    || interior_t->datum.primitivum
                        == PRIMITIVUM_CHARACTER_INSIGNATUM))
            {
                redde SESSIO_VALOR_MONSTRATOR_CHARACTERUM;
            }
            redde SESSIO_VALOR_MONSTRATOR;
        }
        ordinarius:
            redde SESSIO_VALOR_NULLUS;
    }
}

/* titulus declaratus (religatio): definitio -> titulus functionis;
 * declaratio -> titulus declaratoris initiati UNICI (multi aut
 * nulli -> vacua) */
interior chorda
_titulum_declaratum (constans SilvaNodus* elementum, Piscina* piscina)
{
    constans SilvaNodus* sedes = NIHIL;
    constans SilvaNodus* titulus_nodus;
    SilvaToken* t;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;

    si (elementum->genus == SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
    {
        sedes = elementum;
    }
    alioquin si (elementum->genus == SILVA_C89_GENUS_DECLARATIO)
    {
        i32 initiati = _numerare_generis_nodi(elementum,
            SILVA_C89_GENUS_DECLARATOR_INITIATUS);

        si (initiati == I)
        {
            sedes = _nodum_generis_nodi(elementum,
                SILVA_C89_GENUS_DECLARATOR_INITIATUS);
        }
        alioquin si (initiati == ZEPHYRUM
            && _numerare_generis_nodi(elementum,
                SILVA_C89_GENUS_DECLARATOR_TITULUS) == I)
        {
            /* declaratio nuda (`chorda s;`): titulus unicus -
             * religatio + echo etiam sine initiatore */
            sedes = elementum;
        }
        alioquin
        {
            redde vacua;
        }
    }
    si (sedes == NIHIL)
    {
        redde vacua;
    }
    titulus_nodus = _nodum_generis_nodi(sedes,
        SILVA_C89_GENUS_DECLARATOR_TITULUS);
    si (titulus_nodus == NIHIL)
    {
        redde vacua;
    }
    t = _lexema_primum_nodi(titulus_nodus);
    si (t == NIHIL || t->valor.datum == NIHIL)
    {
        redde vacua;
    }
    {
        chorda cruda;

        cruda.datum = (i8*)(vacuum*)(memoriae_index)(constans vacuum*)
            t->valor.datum;
        cruda.mensura = (i32)t->valor.mensura;
        redde chorda_transcribere(cruda, piscina);
    }
}

/* ==================================================
 * custos initiatoris globalis (chunk B, experimentum B6):
 * demissio initiatorem non-constantem TACITE zephyrat - vocationes
 * et lectiones valoris variabilium reiciuntur ad limen. Licent:
 * constantes, &variabilis (adressa), magnitudo (inaestimata),
 * congeries {…} (v0 clementia). Refinamentum nominatum: &a[i]
 * cum i variabili per clementiam maiorum transit.
 * ================================================== */

interior b32
_sub_maiore_generis (constans SilvaNodus* nodus,
    constans SilvaNodus* radix, integer genus_quaesitum)
{
    constans SilvaNodus* n = nodus->pater;

    dum (n != NIHIL)
    {
        si (n->genus == genus_quaesitum)
        {
            redde VERUM;
        }
        si (n == radix)
        {
            frange;
        }
        n = n->pater;
    }
    redde FALSUM;
}

interior b32
_sub_adressa (constans SilvaNodus* nodus, constans SilvaNodus* radix)
{
    constans SilvaNodus* n = nodus->pater;

    dum (n != NIHIL)
    {
        si (n->genus == SILVA_C89_GENUS_UNARIUM)
        {
            SilvaValor op = silva_c89_unarium_tok_operator(n);

            si (op.genus == SILVA_VALOR_TOKEN
                && op.datum.token != NIHIL
                && op.datum.token->valor.mensura == I
                && op.datum.token->valor.datum[ZEPHYRUM] == '&')
            {
                redde VERUM;
            }
        }
        si (n == radix)
        {
            frange;
        }
        n = n->pater;
    }
    redde FALSUM;
}

/* VERUM = initiator illegalis (nuntius_out impletur) */
interior b32 _initiator_arbor_illegalis (SilvaSemantica* sem,
    SilvaValor v, constans SilvaNodus* radix,
    constans character** nuntius_out);

interior b32
_initiator_nodus_illegalis (SilvaSemantica* sem,
    constans SilvaNodus* n, constans SilvaNodus* radix,
    constans character** nuntius_out)
{
    insignatus integer k;

    si (n == NIHIL)
    {
        redde FALSUM;
    }
    si (n->genus == SILVA_C89_GENUS_VOCATIO
        && !_sub_maiore_generis(n, radix,
            SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS))
    {
        *nuntius_out = "vocatio in initiatore globali - divide"
            " declarationem et assignationem";
        redde VERUM;
    }
    si (n->genus == SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        constans SemanticaSymbolum* sym = silva_c89_symbolum_nodi(
            sem, n);

        si (sym != NIHIL
            && (sym->genus == (int)SYMBOLUM_VARIABILE
                || sym->genus == (int)SYMBOLUM_PARAMETRUM)
            && !_sub_adressa(n, radix)
            && !_sub_maiore_generis(n, radix,
                SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS))
        {
            *nuntius_out = "lectio variabilis in initiatore"
                " globali - divide declarationem et assignationem";
            redde VERUM;
        }
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        si (_initiator_arbor_illegalis(sem, n->loci[k], radix,
                nuntius_out))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_initiator_arbor_illegalis (SilvaSemantica* sem, SilvaValor v,
    constans SilvaNodus* radix, constans character** nuntius_out)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_NODUS:
            redde _initiator_nodus_illegalis(sem, v.datum.nodus,
                radix, nuntius_out);
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL && _initiator_arbor_illegalis(sem,
                        *elem, radix, nuntius_out))
                {
                    redde VERUM;
                }
            }
            redde FALSUM;
        }
        ordinarius:
            redde FALSUM;
    }
}

/* custos: declaratio globalis cum initiatore illegali?
 * initiatus_out (licet NIHIL) = nodus pro scissione automatica */
interior b32
_initiatorem_probare (SilvaSemantica* sem,
    constans SilvaNodus* elementum, constans character** nuntius_out,
    constans SilvaNodus** initiatus_out)
{
    constans SilvaNodus* initiatus;
    SilvaValor initiator;

    *nuntius_out = NIHIL;
    si (initiatus_out != NIHIL)
    {
        *initiatus_out = NIHIL;
    }
    si (elementum->genus != SILVA_C89_GENUS_DECLARATIO)
    {
        redde FALSUM;
    }
    initiatus = _nodum_generis_nodi(elementum,
        SILVA_C89_GENUS_DECLARATOR_INITIATUS);
    si (initiatus == NIHIL)
    {
        redde FALSUM;
    }
    si (initiatus_out != NIHIL)
    {
        *initiatus_out = initiatus;
    }
    initiator = silva_c89_declarator_initiatus_initiator(initiatus);
    si (initiator.genus != SILVA_VALOR_NODUS
        || initiator.datum.nodus == NIHIL)
    {
        redde FALSUM;   /* sine initiatore (aut lista aliena) */
    }
    si (initiator.datum.nodus->genus == SILVA_C89_GENUS_CONGERIES)
    {
        redde FALSUM;   /* congeries {…} - clementia v0 */
    }
    redde _initiator_arbor_illegalis(sem, initiator,
        initiator.datum.nodus, nuntius_out);
}

/* ==================================================
 * receptum iudicii (examen fidele)
 * ================================================== */

interior SilvaSemantica*
_iudicare (Sessio* s, SilvaPiscina* effimera, constans character* via,
    constans character* fons, i32 mensura, SilvaParsura** parsura_out)
{
    redde praeparator_analysare(&s->praeparatio, effimera, via,
        fons, (insignatus integer)mensura, parsura_out);
}

/* ==================================================
 * diagnostica colligere
 * ================================================== */

nomen structura {
    Xar* xar;            /* SessioDiagnosticum */
    b32  reice;
} Collectio;

interior chorda
_chordam_ex_silva (constans SilvaChorda* sc, Piscina* piscina)
{
    chorda cruda;

    si (sc == NIHIL || sc->datum == NIHIL || sc->mensura == ZEPHYRUM)
    {
        cruda.mensura = ZEPHYRUM;
        cruda.datum = NIHIL;
        redde cruda;
    }
    cruda.datum = (i8*)(vacuum*)(memoriae_index)(constans vacuum*)
        sc->datum;
    cruda.mensura = (i32)sc->mensura;
    redde chorda_transcribere(cruda, piscina);
}

interior b32
_via_documenti_est (constans SilvaChorda* via)
{
    constans character* exspectata = SESSIO_VIA_DOCUMENTI;
    memoriae_index m = strlen(exspectata);

    si (via == NIHIL || via->datum == NIHIL
        || (memoriae_index)via->mensura != m)
    {
        redde FALSUM;
    }
    redde memcmp(via->datum, exspectata, m) == ZEPHYRUM
        ? VERUM : FALSUM;
}

interior vacuum
_diagnosticum_addere (Collectio* c, Piscina* piscina, chorda via,
    i32 linea, i32 columna, s32 severitas, s32 codex,
    b32 provisionale, b32 extra_turnum, s32 turnus_vetus,
    constans character* nuntius)
{
    SessioDiagnosticum* d = xar_addere(c->xar);

    si (d == NIHIL)
    {
        redde;
    }
    d->via = via;
    d->linea = linea;
    d->columna = columna;
    d->severitas = severitas;
    d->codex = codex;
    d->provisionale = provisionale;
    d->extra_turnum = extra_turnum;
    d->turnus_vetus = turnus_vetus;
    d->nuntius = chorda_ex_literis(nuntius, piscina);
}

/* colligere diagnostica semantica + syntaxis in visum novum
 * mappata; regula C2: acceptatio = documentum totum, ostensio =
 * intervallum novum + violationes extra (extra_turnum notatae) */
interior vacuum
_colligere (Sessio* s, Collectio* c, SilvaParsura* parsura,
    SilvaSemantica* sem, TurnusVisus* visus, i32 visus_numerus,
    i32 index_novi)
{
    chorda via_doc = chorda_ex_literis(SESSIO_VIA_DOCUMENTI,
        s->piscina);

    /* syntaxis: nodi erroris in radice */
    si (parsura->numerus_errorum > ZEPHYRUM)
    {
        SilvaValor radix = parsura->commissio->radix;
        insignatus integer n = silva_valor_lista_numerus(radix);
        insignatus integer k;

        c->reice = VERUM;
        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(radix, k);
            SilvaToken* t;
            SilvaToken* rad;
            i32 linea_ostensa = ZEPHYRUM;
            s32 possessor;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS
                || v->datum.nodus == NIHIL
                || v->datum.nodus->genus != SILVA_C89_GENUS_ERROR)
            {
                perge;
            }
            t = _lexema_primum_valoris(
                silva_c89_error_tokens(v->datum.nodus));
            si (t == NIHIL)
            {
                t = _lexema_primum_nodi(v->datum.nodus);
            }
            si (t == NIHIL)
            {
                _diagnosticum_addere(c, s->piscina, via_doc,
                    ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, -I, FALSUM,
                    FALSUM, -I, "error syntaxis");
                perge;
            }
            rad = silva_token_radix(t);
            si (rad == NIHIL)
            {
                rad = t;
            }
            possessor = _turnum_possidentem(visus, visus_numerus,
                (i32)rad->linea, &linea_ostensa);
            _diagnosticum_addere(c, s->piscina, via_doc,
                linea_ostensa, (i32)rad->columna, ZEPHYRUM, -I,
                FALSUM,
                (possessor >= ZEPHYRUM && possessor != (s32)index_novi)
                    ? VERUM : FALSUM,
                (possessor >= ZEPHYRUM && possessor != (s32)index_novi)
                    ? possessor : -I,
                "error syntaxis");
        }
    }

    /* semantica */
    {
        insignatus integer m = silva_c89_diagnostica_numerus(sem);
        insignatus integer k;

        per (k = ZEPHYRUM; k < m; k++)
        {
            constans SemanticaDiagnosticum* d =
                silva_c89_diagnosticum_per_indicem(sem, k);
            b32 violat;
            b32 in_documento;
            s32 possessor = -I;
            i32 linea_ostensa = ZEPHYRUM;
            b32 extra;

            si (d == NIHIL)
            {
                perge;
            }
            violat = (d->severitas == (int)ZEPHYRUM
                && !d->provisionale) ? VERUM : FALSUM;
            si (violat)
            {
                c->reice = VERUM;
            }
            in_documento = _via_documenti_est(&d->via);
            si (in_documento)
            {
                possessor = _turnum_possidentem(visus, visus_numerus,
                    (i32)d->linea, &linea_ostensa);
            }
            extra = (in_documento && possessor >= ZEPHYRUM
                && possessor != (s32)index_novi) ? VERUM : FALSUM;

            /* regula ostensionis: intervallum novum semper;
             * violationes ubique; cetera extra intervallum tacent
             * (re-emissiones historicae) */
            si (!violat && !(in_documento
                    && possessor == (s32)index_novi))
            {
                perge;
            }
            _diagnosticum_addere(c, s->piscina,
                in_documento ? via_doc
                    : _chordam_ex_silva(&d->via, s->piscina),
                in_documento ? linea_ostensa : (i32)d->linea,
                (i32)d->columna, (s32)d->severitas, (s32)d->codex,
                d->provisionale ? VERUM : FALSUM, extra,
                extra ? possessor : -I, d->causa);
        }
    }
}

/* ==================================================
 * mundus (chunk B): generationes + replicatio + captura
 * ================================================== */

interior chorda
_capturam_legere (FILE* pl, Piscina* piscina)
{
    long mensura_l;
    chorda fructus;
    i8* datum;

    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    si (pl == NIHIL)
    {
        redde fructus;
    }
    fflush(pl);
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    si (mensura_l <= 0L)
    {
        redde fructus;
    }
    fseek(pl, 0L, SEEK_SET);
    datum = piscina_allocare(piscina, (memoriae_index)mensura_l);
    si (datum == NIHIL
        || fread(datum, I, (memoriae_index)mensura_l, pl)
            != (memoriae_index)mensura_l)
    {
        redde fructus;
    }
    fructus.datum = datum;
    fructus.mensura = (i32)mensura_l;
    redde fructus;
}

/* ordo demolitionis RIGIDUS (C6/C8): ansae programmatis clausae ->
 * regio soluta (basis fixa!) -> piscina generationis destructa */
interior vacuum
_generationem_demoliri (Sessio* s)
{
    si (s->machinula != NIHIL)
    {
        machinula_ansas_claudere(s->machinula);
    }
    si (s->regio != NIHIL)
    {
        regio_destruere(s->regio);
    }
    si (s->piscina_generationis != NIHIL)
    {
        piscina_destruere(s->piscina_generationis);
    }
    s->piscina_generationis = NIHIL;
    s->regio = NIHIL;
    s->conexio = NIHIL;
    s->machinula = NIHIL;
}

/* mundum aedificare in piscina generationis currenti: bibliothecae
 * (perennes) + modulus documenti -> nexus -> machinula */
interior b32
_mundum_aedificare (Sessio* s, MedullaModulus* modulus_documenti)
{
    i32 k;

    s->regio = regio_generare(s->piscina_generationis);
    si (s->regio == NIHIL)
    {
        redde FALSUM;
    }
    s->conexio = conexio_creare(s->piscina_generationis, s->regio);
    si (s->conexio == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(s->moduli_bibliothecarum);
        k++)
    {
        si (!conexio_modulum_addere(s->conexio,
                *(MedullaModulus**)xar_obtinere(
                    s->moduli_bibliothecarum, k)))
        {
            redde FALSUM;
        }
    }
    si (modulus_documenti != NIHIL
        && !conexio_modulum_addere(s->conexio, modulus_documenti))
    {
        redde FALSUM;
    }
    si (!conexio_nectere(s->conexio))
    {
        redde FALSUM;
    }
    s->machinula = machinula_creare(s->piscina_generationis,
        s->conexio, s->regio);
    si (s->machinula == NIHIL)
    {
        redde FALSUM;
    }
    machinula_recusationes_ponere(s->machinula,
        s->vexilla_recusationum);
    redde VERUM;
}

/* replicatio: turni involuti ordine, machinula UNA (persistentia
 * globalium trans currere = designata M2, probata hic). Captura per
 * turnum; capturae/capturae_err NIHIL = modus abiciendi
 * (restitutio). VERUM = replicatio viridis. */
interior b32
_replicare (Sessio* s, TurnusVisus* visus, i32 numerus,
    i32 index_novi, chorda* capturae, chorda* capturae_err,
    Xar* mutati, SessioRelatum* r)
{
    i32 k;
    i32 n_recordorum = xar_numerus(s->turni);

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        FILE* effusio_pl;
        FILE* erroris_pl;
        MachinulaExitus exitus;
        chorda capta;
        chorda capta_err;

        si (visus[k].involucrum.mensura == ZEPHYRUM)
        {
            perge;   /* declarationes/definitiones non exsecuntur */
        }
        effusio_pl = tmpfile();
        erroris_pl = tmpfile();
        machinula_ansam_ponere(s->machinula, I, effusio_pl);
        machinula_ansam_ponere(s->machinula, II, erroris_pl);
        exitus = machinula_currere(s->machinula,
            visus[k].involucrum);
        machinula_ansam_ponere(s->machinula, I, NIHIL);
        machinula_ansam_ponere(s->machinula, II, NIHIL);
        capta = _capturam_legere(effusio_pl, s->piscina);
        capta_err = _capturam_legere(erroris_pl, s->piscina);
        si (effusio_pl != NIHIL)
        {
            fclose(effusio_pl);
        }
        si (erroris_pl != NIHIL)
        {
            fclose(erroris_pl);
        }
        si (capturae != NIHIL)
        {
            capturae[k] = capta;
            capturae_err[k] = capta_err;
        }
        si (exitus.genus != (s32)MACHINULA_BENE)
        {
            /* halitus: replicatio fracta (C8) */
            si (r != NIHIL)
            {
                r->halitus_genus = exitus.genus;
                r->halitus_codex = exitus.codex;
                r->halitus_nuntius = chorda_transcribere(
                    exitus.nuntius, s->piscina);
                /* index solum si turnus VETUS fregit (novus =
                 * ipse - suffixum "(turnus N)" tunc taceat) */
                r->halitus_turnus = ((s32)k == (s32)index_novi)
                    ? -I : (s32)k;
            }
            redde FALSUM;
        }
        si (r != NIHIL && (s32)k == (s32)index_novi)
        {
            /* valor turni novi (chunk C): codex = valor redditus
             * involucri typati (figurae f32/f64 - reinterpretatio
             * hospitis-lateris per valor_genus) */
            r->valor = exitus.codex;
            r->valor_validus = VERUM;
            si (r->halitus_genus == -I
                && exitus.codex != (s64)ZEPHYRUM
                && visus[k].typus_textus.mensura == ZEPHYRUM)
            {
                /* exit(n) in turno novo (involucro void): BENE +
                 * codex - relatum */
                r->halitus_genus = (s32)MACHINULA_BENE;
                r->halitus_codex = exitus.codex;
            }
        }
        /* historia mutata: comparatio contra actas conditas */
        si (mutati != NIHIL && k != index_novi && k < n_recordorum)
        {
            TurnusInternus* t = xar_obtinere(s->turni, k);

            si (t->effusio_valida
                && (t->effusio.mensura != capta.mensura
                    || (capta.mensura > ZEPHYRUM
                        && memcmp(t->effusio.datum, capta.datum,
                            (memoriae_index)capta.mensura)
                            != ZEPHYRUM)))
            {
                s32* locellus = xar_addere(mutati);

                si (locellus != NIHIL)
                {
                    *locellus = (s32)k;
                }
            }
        }
    }
    redde VERUM;
}

/* mundum ex recordis restituere (post reiectionem replicationis C8:
 * determinismus reaedificationem veterem correctam facit) */
interior vacuum
_mundum_restituere (Sessio* s)
{
    i32 n = xar_numerus(s->turni);
    TurnusVisus* visus;
    chorda involutum;
    SilvaPiscina* effimera;
    SilvaParsura* parsura = NIHIL;
    SilvaSemantica* sem;
    MedullaModulus* modulus_doc;
    i32 k;

    si (n == ZEPHYRUM)
    {
        redde;   /* documentum vacuum - sine mundo licet */
    }
    visus = piscina_allocare(s->piscina,
        (memoriae_index)n * magnitudo(TurnusVisus));
    si (visus == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        TurnusInternus* t = xar_obtinere(s->turni, k);

        visus[k].textus = t->textus;
        visus[k].genus = t->genus;
        visus[k].involucrum = t->involucrum;
        visus[k].typus_textus = t->typus_textus;
        visus[k].lineae = t->lineae;
    }
    (vacuum)_ostensum_texere(visus, n, s->piscina);
    involutum = _involutum_texere(visus, n, s->piscina);
    si (involutum.datum == NIHIL)
    {
        redde;
    }
    effimera = silva_piscina_generare_dynamicum("sessio_restitutio",
        268435456);
    si (effimera == NIHIL)
    {
        redde;
    }
    sem = _iudicare(s, effimera, SESSIO_VIA_DOCUMENTI,
        (constans character*)involutum.datum, involutum.mensura,
        &parsura);
    si (sem == NIHIL || parsura == NIHIL)
    {
        silva_piscina_destruere(effimera);
        redde;
    }
    s->piscina_generationis = piscina_generare_dynamicum(
        "sessio_generatio", 67108864);
    si (s->piscina_generationis == NIHIL)
    {
        silva_piscina_destruere(effimera);
        redde;
    }
    modulus_doc = demissio_currere(s->piscina_generationis, parsura,
        sem, chorda_ex_literis(SESSIO_VIA_DOCUMENTI, s->piscina));
    silva_piscina_destruere(effimera);
    si (modulus_doc == NIHIL || !_mundum_aedificare(s, modulus_doc))
    {
        _generationem_demoliri(s);
        redde;
    }
    (vacuum)_replicare(s, visus, n, n + I, NIHIL, NIHIL, NIHIL,
        NIHIL);
}

/* bibliothecam demittere (perenniter) - receptum oneratoris */
interior b32
_bibliothecam_demittere (Sessio* s, constans character* radix,
    constans character* via)
{
    character via_plena[1024];
    insignatus integer mensura = ZEPHYRUM;
    character* fons;
    SilvaPiscina* effimera;
    SilvaParsura* parsura = NIHIL;
    SilvaSemantica* sem;
    MedullaModulus* modulus;

    si (strlen(radix) + strlen(via) + II >= magnitudo(via_plena))
    {
        redde FALSUM;
    }
    sprintf(via_plena, "%s/%s", radix, via);
    fons = praeparator_plagulam_legere(s->piscina, via_plena,
        &mensura);
    si (fons == NIHIL)
    {
        redde FALSUM;
    }
    effimera = silva_piscina_generare_dynamicum("sessio_bibliotheca",
        268435456);
    si (effimera == NIHIL)
    {
        redde FALSUM;
    }
    sem = _iudicare(s, effimera, via, fons, (i32)mensura,
        &parsura);
    si (sem == NIHIL || parsura == NIHIL
        || parsura->numerus_errorum > ZEPHYRUM)
    {
        silva_piscina_destruere(effimera);
        redde FALSUM;
    }
    modulus = demissio_currere(s->piscina_bibliothecarum, parsura,
        sem, chorda_ex_literis(via, s->piscina));
    silva_piscina_destruere(effimera);
    si (modulus == NIHIL)
    {
        redde FALSUM;
    }
    {
        vacuum** locellus = xar_addere(s->moduli_bibliothecarum);

        si (locellus == NIHIL)
        {
            redde FALSUM;
        }
        *locellus = modulus;
    }
    redde VERUM;
}

/* ==================================================
 * API: creare / destruere
 * ================================================== */

Sessio*
sessio_creare (Piscina* piscina, constans SessioConfiguratio* cfg)
{
    Sessio* s;

    si (piscina == NIHIL || cfg == NIHIL)
    {
        redde NIHIL;
    }
    s = piscina_allocare(piscina, magnitudo(Sessio));
    si (s == NIHIL)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(Sessio));
    s->piscina = piscina;
    s->numerator = I;
    s->turni = xar_creare(piscina, (i32)magnitudo(TurnusInternus));
    si (s->turni == NIHIL)
    {
        redde NIHIL;
    }
    {
        PraeparatorConfiguratio pcfg;

        memset(&pcfg, ZEPHYRUM, magnitudo(PraeparatorConfiguratio));
        pcfg.radix = cfg->radix;
        pcfg.cum_posix = cfg->cum_posix ? I : ZEPHYRUM;
        pcfg.cum_latina = I;   /* turni latine loquuntur */
        pcfg.sine_capitibus = cfg->sine_capitibus ? I : ZEPHYRUM;
        si (!praeparator_praeparare(&s->praeparatio, piscina,
                &pcfg))
        {
            redde NIHIL;
        }
    }
    s->piscina_bibliothecarum = piscina_generare_dynamicum(
        "sessio_bibliothecae", 67108864);
    s->moduli_bibliothecarum = xar_creare(piscina,
        (i32)magnitudo(vacuum*));
    si (s->piscina_bibliothecarum == NIHIL
        || s->moduli_bibliothecarum == NIHIL)
    {
        redde NIHIL;
    }
    s->vexilla_recusationum = cfg->sine_recusationibus
        ? (i32)ZEPHYRUM
        : (i32)(MACHINULA_RECUSARE_SCRIPTURAS
            | MACHINULA_RECUSARE_TEMPUS | MACHINULA_RECUSARE_INITUM);
    si (cfg->radix != NIHIL)
    {
        /* bibliothecae: demissae semel, moduli perennes */
        {
            s32 k;

            per (k = ZEPHYRUM; k < cfg->plagulae_numerus; k++)
            {
                si (!_bibliothecam_demittere(s, cfg->radix,
                        cfg->plagulae[k]))
                {
                    redde NIHIL;
                }
            }
        }
    }
    redde s;
}

vacuum
sessio_destruere (Sessio* s)
{
    si (s == NIHIL)
    {
        redde;
    }
    _generationem_demoliri(s);
    si (s->piscina_bibliothecarum != NIHIL)
    {
        piscina_destruere(s->piscina_bibliothecarum);
        s->piscina_bibliothecarum = NIHIL;
    }
    praeparator_destruere(&s->praeparatio);
}

/* ==================================================
 * API: inspectio
 * ================================================== */

i32
sessio_turni_numerus (constans Sessio* s)
{
    redde s == NIHIL ? ZEPHYRUM : xar_numerus(s->turni);
}

chorda
sessio_turnus_textus (constans Sessio* s, i32 index)
{
    chorda vacua;
    TurnusInternus* t;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (s == NIHIL)
    {
        redde vacua;
    }
    t = xar_obtinere(s->turni, index);
    redde t == NIHIL ? vacua : t->textus;
}

s32
sessio_turnus_genus (constans Sessio* s, i32 index)
{
    TurnusInternus* t;

    si (s == NIHIL)
    {
        redde -I;
    }
    t = xar_obtinere(s->turni, index);
    redde t == NIHIL ? -I : t->genus;
}

chorda
sessio_turnus_nomen (constans Sessio* s, i32 index)
{
    chorda vacua;
    TurnusInternus* t;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (s == NIHIL)
    {
        redde vacua;
    }
    t = xar_obtinere(s->turni, index);
    redde t == NIHIL ? vacua : t->titulus;
}

chorda
sessio_turnus_effusio (constans Sessio* s, i32 index)
{
    chorda vacua;
    TurnusInternus* t;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (s == NIHIL)
    {
        redde vacua;
    }
    t = xar_obtinere(s->turni, index);
    redde (t == NIHIL || !t->effusio_valida) ? vacua : t->effusio;
}

/* ==================================================
 * chunk D: via #! + emissores
 * ================================================== */

/* extensio octetorum elementi in fonte principe (min/max trans
 * lexemata fontis principis; tokens synthetici/lexici omissi) */
interior vacuum _extensionem_valoris (SilvaValor v, integer princeps,
    s32* minimum, s32* maximum);

interior vacuum
_extensionem_nodi (constans SilvaNodus* n, integer princeps,
    s32* minimum, s32* maximum)
{
    insignatus integer k;

    si (n == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        _extensionem_valoris(n->loci[k], princeps, minimum, maximum);
    }
}

interior vacuum
_extensionem_valoris (SilvaValor v, integer princeps, s32* minimum,
    s32* maximum)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_TOKEN:
            si (v.datum.token != NIHIL)
            {
                /* RADIX originis: lexemata expansa synthetica sunt
                 * (byte_offset -1) - sedes invocationis in fonte
                 * principe per catenam originis (macros latinae!) */
                SilvaToken* radix_t = silva_token_radix(
                    v.datum.token);

                si (radix_t == NIHIL)
                {
                    radix_t = v.datum.token;
                }
                si (radix_t->fons_index == princeps
                    && radix_t->byte_offset >= ZEPHYRUM)
                {
                    s32 a = (s32)radix_t->byte_offset;
                    s32 b = a + (s32)radix_t->longitudo;

                    si (*minimum < (s32)ZEPHYRUM || a < *minimum)
                    {
                        *minimum = a;
                    }
                    si (b > *maximum)
                    {
                        *maximum = b;
                    }
                }
            }
            frange;
        casus SILVA_VALOR_NODUS:
            _extensionem_nodi(v.datum.nodus, princeps, minimum,
                maximum);
            frange;
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    _extensionem_valoris(*elem, princeps, minimum,
                        maximum);
                }
            }
            frange;
        }
        ordinarius:
            frange;
    }
}

nomen structura {
    i32 initium;
    i32 finis;
} SegmentumScripti;

s32
sessio_scriptum_offerre (Sessio* s, chorda textus,
    SessioRelatum* relatum_out)
{
    i8* copia;
    SilvaPiscina* effimera;
    SilvaParsura* parsura = NIHIL;
    SilvaSemantica* sem;
    Xar* segmenta;
    s32 recepti = ZEPHYRUM;
    SessioRelatum relatum_ultimum;

    memset(&relatum_ultimum, ZEPHYRUM,
        magnitudo(SessioRelatum));
    relatum_ultimum.verdictum = SESSIO_APPARATUS;
    si (s == NIHIL || textus.mensura == ZEPHYRUM)
    {
        salta apparatus;
    }

    /* copia mutabilis; linea #! spatiis obliterata (\n servato) */
    copia = piscina_allocare(s->piscina,
        (memoriae_index)textus.mensura);
    si (copia == NIHIL)
    {
        salta apparatus;
    }
    memcpy(copia, textus.datum, (memoriae_index)textus.mensura);
    si (textus.mensura >= II && copia[ZEPHYRUM] == (i8)'#'
        && copia[I] == (i8)'!')
    {
        i32 k;

        per (k = ZEPHYRUM; k < textus.mensura
            && copia[k] != (i8)'\n'; k++)
        {
            copia[k] = (i8)' ';
        }
    }

    /* parsura findendi (fines elementorum; iudicium in oblatione) */
    effimera = silva_piscina_generare_dynamicum("sessio_scriptum",
        268435456);
    si (effimera == NIHIL)
    {
        salta apparatus;
    }
    sem = _iudicare(s, effimera, "<scriptum>",
        (constans character*)copia, textus.mensura, &parsura);
    (vacuum)sem;
    si (parsura == NIHIL)
    {
        silva_piscina_destruere(effimera);
        salta apparatus;
    }
    segmenta = xar_creare(s->piscina,
        (i32)magnitudo(SegmentumScripti));
    si (segmenta == NIHIL)
    {
        silva_piscina_destruere(effimera);
        salta apparatus;
    }

    /* extensiones elementorum principis */
    {
        SilvaValor radix = parsura->commissio->radix;
        insignatus integer n = silva_valor_lista_numerus(radix);
        insignatus integer k;

        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(radix, k);
            s32 minimum = -I;
            s32 maximum = ZEPHYRUM;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS
                || v->datum.nodus == NIHIL)
            {
                perge;
            }
            _extensionem_valoris(*v, parsura->fons_princeps,
                &minimum, &maximum);
            si (minimum >= (s32)ZEPHYRUM && maximum > minimum)
            {
                SegmentumScripti* seg = xar_addere(segmenta);

                si (seg != NIHIL)
                {
                    seg->initium = (i32)minimum;
                    seg->finis = (i32)maximum;
                }
            }
        }
    }
    silva_piscina_destruere(effimera);

    /* lineae directivarum EXTRA extensiones (scanner textus cum
     * statu commentorum; directivae intra elementa cum eis vehuntur) */
    {
        b32 in_commento = FALSUM;
        i32 k = ZEPHYRUM;

        dum (k < textus.mensura)
        {
            i32 linea_initium = k;
            i32 linea_finis;
            i32 j;
            b32 directiva = FALSUM;

            /* finis lineae */
            linea_finis = k;
            dum (linea_finis < textus.mensura
                && copia[linea_finis] != (i8)'\n')
            {
                linea_finis++;
            }
            si (linea_finis < textus.mensura)
            {
                linea_finis++;   /* \n inclusum */
            }
            si (!in_commento)
            {
                j = linea_initium;
                dum (j < linea_finis && (copia[j] == (i8)' '
                    || copia[j] == (i8)'\t'))
                {
                    j++;
                }
                si (j < linea_finis && copia[j] == (i8)'#')
                {
                    directiva = VERUM;
                }
            }
            /* status commentorum trans lineam (grossus: chordae
             * non trans lineas in C89) */
            per (j = linea_initium; j < linea_finis; j++)
            {
                si (in_commento)
                {
                    si (copia[j] == (i8)'*' && j + I < linea_finis
                        && copia[j + I] == (i8)'/')
                    {
                        in_commento = FALSUM;
                        j++;
                    }
                }
                alioquin si (copia[j] == (i8)'/'
                    && j + I < linea_finis
                    && copia[j + I] == (i8)'*')
                {
                    in_commento = VERUM;
                    j++;
                }
            }
            si (directiva)
            {
                /* extra omnes extensiones elementorum? */
                i32 n_seg = xar_numerus(segmenta);
                i32 si_index;
                b32 intra = FALSUM;

                per (si_index = ZEPHYRUM; si_index < n_seg;
                    si_index++)
                {
                    SegmentumScripti* seg = xar_obtinere(segmenta,
                        si_index);

                    si (linea_initium < seg->finis
                        && linea_finis > seg->initium)
                    {
                        intra = VERUM;
                        frange;
                    }
                }
                si (!intra)
                {
                    SegmentumScripti* seg = xar_addere(segmenta);

                    si (seg != NIHIL)
                    {
                        seg->initium = linea_initium;
                        seg->finis = linea_finis;
                    }
                }
            }
            k = linea_finis;
        }
    }

    /* ordinare per initium (insertio - segmenta pauca) et offerre */
    {
        i32 n = xar_numerus(segmenta);
        i32 a;
        i32 b;

        per (a = I; a < n; a++)
        {
            per (b = a; b > ZEPHYRUM; b--)
            {
                SegmentumScripti* praecedens = xar_obtinere(
                    segmenta, b - I);
                SegmentumScripti* hic_seg = xar_obtinere(segmenta,
                    b);

                si (praecedens->initium > hic_seg->initium)
                {
                    SegmentumScripti temp = *praecedens;

                    *praecedens = *hic_seg;
                    *hic_seg = temp;
                }
                alioquin
                {
                    frange;
                }
            }
        }
        per (a = ZEPHYRUM; a < n; a++)
        {
            SegmentumScripti* seg = xar_obtinere(segmenta, a);
            chorda pars;

            pars.datum = copia + seg->initium;
            pars.mensura = seg->finis - seg->initium;
            relatum_ultimum = sessio_turnum_offerre(s, pars);
            si (relatum_ultimum.verdictum != SESSIO_ACCEPTUM)
            {
                si (relatum_out != NIHIL)
                {
                    *relatum_out = relatum_ultimum;
                }
                redde recepti;
            }
            recepti++;
        }
    }
    si (relatum_out != NIHIL)
    {
        *relatum_out = relatum_ultimum;
    }
    redde recepti;

apparatus:
    si (relatum_out != NIHIL)
    {
        *relatum_out = relatum_ultimum;
    }
    redde -I;
}

b32
sessio_functionem_currere (Sessio* s, constans character* titulus,
    s64* codex_out)
{
    MachinulaExitus exitus;

    si (codex_out != NIHIL)
    {
        *codex_out = ZEPHYRUM;
    }
    si (s == NIHIL || s->machinula == NIHIL || titulus == NIHIL)
    {
        redde FALSUM;
    }
    exitus = machinula_currere(s->machinula,
        chorda_ex_literis(titulus, s->piscina));
    si (codex_out != NIHIL)
    {
        *codex_out = exitus.codex;
    }
    redde exitus.genus == (s32)MACHINULA_BENE ? VERUM : FALSUM;
}

chorda
sessio_documentum_strictum (constans Sessio* s, Piscina* piscina)
{
    i32 n;
    i32 k;
    TurnusVisus* visus;
    chorda involutum;
    ChordaAedificator* a;
    b32 principale_adest = FALSUM;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (s == NIHIL || piscina == NIHIL)
    {
        redde vacua;
    }
    n = xar_numerus(s->turni);
    visus = piscina_allocare(piscina,
        (memoriae_index)(n > ZEPHYRUM ? n : I)
            * magnitudo(TurnusVisus));
    si (visus == NIHIL)
    {
        redde vacua;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        TurnusInternus* t = xar_obtinere(s->turni, k);

        visus[k].textus = t->textus;
        visus[k].genus = t->genus;
        visus[k].involucrum = t->involucrum;
        visus[k].typus_textus = t->typus_textus;
        visus[k].lineae = t->lineae;
        si (t->genus == SESSIO_TURNUS_DEFINITIO
            && chorda_aequalis_literis(t->titulus, "principale"))
        {
            principale_adest = VERUM;
        }
    }
    (vacuum)_ostensum_texere(visus, n, piscina);
    involutum = _involutum_texere(visus, n, piscina);

    a = chorda_aedificator_creare(piscina, DXII);
    si (a == NIHIL)
    {
        redde vacua;
    }
    (vacuum)chorda_aedificator_appendere_literis(a,
        "#include \"latina.h\"\n\n");
    si (involutum.mensura > ZEPHYRUM)
    {
        (vacuum)chorda_aedificator_appendere_chorda(a, involutum);
    }
    si (!principale_adest)
    {
        (vacuum)chorda_aedificator_appendere_literis(a,
            "\ninteger principale(vacuum)\n{\n");
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (visus[k].involucrum.mensura > ZEPHYRUM)
            {
                (vacuum)chorda_aedificator_appendere_literis(a,
                    "    ");
                (vacuum)chorda_aedificator_appendere_chorda(a,
                    visus[k].involucrum);
                (vacuum)chorda_aedificator_appendere_literis(a,
                    "();\n");
            }
        }
        (vacuum)chorda_aedificator_appendere_literis(a,
            "    redde 0;\n}\n");
    }
    redde chorda_aedificator_finire(a);
}

interior constans character*
_halitus_titulus (s32 genus)
{
    commutatio (genus)
    {
        casus (s32)MACHINULA_BENE:      redde "BENE";
        casus (s32)MACHINULA_SISTERE:   redde "SISTERE";
        casus (s32)MACHINULA_DECIPULA:  redde "DECIPULA";
        casus (s32)MACHINULA_VITIUM:    redde "VITIUM";
        casus (s32)MACHINULA_PAUSA:     redde "PAUSA";
        casus (s32)MACHINULA_RECUSATIO: redde "RECUSATIO";
        ordinarius:                     redde "IGNOTUM";
    }
}

interior constans character*
_severitas_titulus (s32 severitas)
{
    commutatio (severitas)
    {
        casus ZEPHYRUM: redde "violatio";
        casus I:        redde "suspectum";
        casus II:       redde "domesticum";
        ordinarius:     redde "infra";
    }
}

/* praevisus char* limitatus ex memoria regionis (chunk C) */
interior vacuum
_praevisum_appendere (Sessio* s, ChordaAedificator* a, s64 valor)
{
    constans insignatus character* p;
    i32 k;

    si (s->regio == NIHIL || valor == (s64)ZEPHYRUM)
    {
        redde;
    }
    p = (constans insignatus character*)(memoriae_index)valor;
    si (!regio_continet(s->regio, (constans vacuum*)p)
        || !regio_continet(s->regio, (constans vacuum*)(p + LXIII)))
    {
        redde;
    }
    (vacuum)chorda_aedificator_appendere_literis(a, " \"");
    per (k = ZEPHYRUM; k < LXIV; k++)
    {
        si (p[k] == (insignatus character)'\0')
        {
            frange;
        }
        (vacuum)chorda_aedificator_appendere_character(a,
            (p[k] >= XXXII && p[k] <= CXXVI && p[k] != (insignatus
                character)'"')
                ? (character)p[k] : '.');
    }
    (vacuum)chorda_aedificator_appendere_literis(a,
        k >= LXIV ? "\xE2\x80\xA6\"" : "\"");
}

chorda
sessio_relatum_formare (Sessio* s, constans SessioRelatum* r,
    Piscina* piscina)
{
    ChordaAedificator* a;
    character buffer[CXXVIII];
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (s == NIHIL || r == NIHIL || piscina == NIHIL)
    {
        redde vacua;
    }
    a = chorda_aedificator_creare(piscina, CCLVI);
    si (a == NIHIL)
    {
        redde vacua;
    }

    /* echo scissionis: declaratio + assignatio ostensae */
    si (r->divisum && r->verdictum == SESSIO_ACCEPTUM)
    {
        (vacuum)chorda_aedificator_appendere_literis(a,
            "\xE2\x89\xA1 divisum: ");
        (vacuum)chorda_aedificator_appendere_chorda(a,
            r->nomen_declaratum);
        (vacuum)chorda_aedificator_appendere_literis(a,
            " declaratum, tum assignatum\n");
    }
    /* echo reparationis */
    si (r->reparatum && r->textus.mensura > ZEPHYRUM)
    {
        (vacuum)chorda_aedificator_appendere_literis(a,
            "\xE2\x80\xA6 ");
        (vacuum)chorda_aedificator_appendere_chorda(a,
            _praecisa(r->textus));
        (vacuum)chorda_aedificator_appendere_character(a, '\n');
    }

    /* effusio capta (programma loquitur primum) */
    si (r->effusio.mensura > ZEPHYRUM)
    {
        (vacuum)chorda_aedificator_appendere_chorda(a, r->effusio);
    }
    si (r->effusio_erroris.mensura > ZEPHYRUM)
    {
        (vacuum)chorda_aedificator_appendere_chorda(a,
            r->effusio_erroris);
    }

    si (r->verdictum == SESSIO_ACCEPTUM)
    {
        /* echo declarationis: nomen : typus */
        si ((r->genus == SESSIO_TURNUS_DECLARATIO
                || r->genus == SESSIO_TURNUS_DEFINITIO)
            && r->nomen_declaratum.mensura > ZEPHYRUM
            && r->typus_textus.mensura > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_chorda(a,
                r->nomen_declaratum);
            (vacuum)chorda_aedificator_appendere_literis(a, " : ");
            (vacuum)chorda_aedificator_appendere_chorda(a,
                r->typus_textus);
            (vacuum)chorda_aedificator_appendere_character(a, '\n');
        }
        /* aggregatum: typus notus, impressio parcata (nota honesta) */
        si (r->genus == SESSIO_TURNUS_EXPRESSIO
            && r->valor_genus == SESSIO_VALOR_AGGREGATUM
            && r->typus_textus.mensura > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_literis(a,
                "aggregatum : ");
            (vacuum)chorda_aedificator_appendere_chorda(a,
                r->typus_textus);
            (vacuum)chorda_aedificator_appendere_literis(a,
                " (impressio parcata)\n");
        }
        /* valor : typus */
        si (r->genus == SESSIO_TURNUS_EXPRESSIO && r->valor_validus
            && r->valor_genus != SESSIO_VALOR_NULLUS
            && r->valor_genus != SESSIO_VALOR_AGGREGATUM)
        {
            commutatio (r->valor_genus)
            {
                casus SESSIO_VALOR_SIGNATUS:
                    sprintf(buffer, "%lld", (long long)r->valor);
                    frange;
                casus SESSIO_VALOR_INSIGNATUS:
                    sprintf(buffer, "%llu",
                        (insignatus long long)(unsigned long long)
                            r->valor);
                    frange;
                casus SESSIO_VALOR_FLUITANS_32:
                {
                    unio { i32 i; fluitans f; } u;

                    u.i = (i32)(insignatus long long)r->valor;
                    sprintf(buffer, "%g", (duplex)u.f);
                    frange;
                }
                casus SESSIO_VALOR_FLUITANS_64:
                {
                    unio { s64 s; duplex d; } u;

                    u.s = r->valor;
                    sprintf(buffer, "%g", u.d);
                    frange;
                }
                ordinarius:
                    sprintf(buffer, "0x%llx",
                        (unsigned long long)r->valor);
                    frange;
            }
            (vacuum)chorda_aedificator_appendere_literis(a, buffer);
            si (r->valor_genus
                == SESSIO_VALOR_MONSTRATOR_CHARACTERUM)
            {
                _praevisum_appendere(s, a, r->valor);
            }
            (vacuum)chorda_aedificator_appendere_literis(a, " : ");
            (vacuum)chorda_aedificator_appendere_chorda(a,
                r->typus_textus);
            (vacuum)chorda_aedificator_appendere_character(a, '\n');
        }
        /* exit(n) */
        si (r->halitus_genus == (s32)MACHINULA_BENE
            && r->halitus_codex != (s64)ZEPHYRUM)
        {
            sprintf(buffer, "[exitus %lld]\n",
                (long long)r->halitus_codex);
            (vacuum)chorda_aedificator_appendere_literis(a, buffer);
        }
        /* historia mutata (notitia quieta) */
        {
            s32 k;

            per (k = ZEPHYRUM; k < r->turni_mutati_numerus; k++)
            {
                sprintf(buffer, "turnus %d: effusio mutata\n",
                    (int)r->turni_mutati[k]);
                (vacuum)chorda_aedificator_appendere_literis(a,
                    buffer);
            }
        }
    }
    alioquin
    {
        /* diagnostica */
        s32 k;

        per (k = ZEPHYRUM; k < r->diagnostica_numerus; k++)
        {
            constans SessioDiagnosticum* d = &r->diagnostica[k];

            (vacuum)chorda_aedificator_appendere_chorda(a, d->via);
            sprintf(buffer, ":%d:%d: [%s] ", (int)d->linea,
                (int)d->columna, _severitas_titulus(d->severitas));
            (vacuum)chorda_aedificator_appendere_literis(a, buffer);
            (vacuum)chorda_aedificator_appendere_chorda(a,
                d->nuntius);
            si (d->extra_turnum)
            {
                sprintf(buffer, " (turnus %d aegrotat)",
                    (int)d->turnus_vetus);
                (vacuum)chorda_aedificator_appendere_literis(a,
                    buffer);
            }
            (vacuum)chorda_aedificator_appendere_character(a, '\n');
        }
        /* halitus replicationis */
        si (r->halitus_genus >= ZEPHYRUM
            && r->halitus_genus != (s32)MACHINULA_BENE)
        {
            sprintf(buffer, "[halitus %s] ",
                _halitus_titulus(r->halitus_genus));
            (vacuum)chorda_aedificator_appendere_literis(a, buffer);
            (vacuum)chorda_aedificator_appendere_chorda(a,
                r->halitus_nuntius);
            si (r->halitus_turnus >= ZEPHYRUM)
            {
                sprintf(buffer, " (turnus %d)",
                    (int)r->halitus_turnus);
                (vacuum)chorda_aedificator_appendere_literis(a,
                    buffer);
            }
            (vacuum)chorda_aedificator_appendere_character(a, '\n');
        }
    }
    redde chorda_aedificator_finire(a);
}

chorda
sessio_documentum (constans Sessio* s, Piscina* piscina)
{
    i32 n;
    i32 k;
    TurnusVisus* visus;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (s == NIHIL || piscina == NIHIL)
    {
        redde vacua;
    }
    n = xar_numerus(s->turni);
    si (n == ZEPHYRUM)
    {
        redde vacua;
    }
    visus = piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(TurnusVisus));
    si (visus == NIHIL)
    {
        redde vacua;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        TurnusInternus* t = xar_obtinere(s->turni, k);

        visus[k].textus = t->textus;
        visus[k].genus = t->genus;
        visus[k].involucrum = t->involucrum;
        visus[k].typus_textus = t->typus_textus;
        visus[k].lineae = t->lineae;
    }
    redde _ostensum_texere(visus, n, piscina);
}

/* ==================================================
 * API: probatio integritatis (librae) - scanner manualis
 * ================================================== */

b32
sessio_initus_completus (Sessio* s, chorda initus)
{
    i32 rotundae = ZEPHYRUM;   /* ( ) */
    i32 quadrae = ZEPHYRUM;    /* [ ] */
    i32 librae = ZEPHYRUM;     /* { } */
    b32 in_chorda = FALSUM;
    b32 in_charactere = FALSUM;
    b32 in_commento = FALSUM;
    b32 in_commento_lineae = FALSUM;
    i32 k;

    (vacuum)s;
    per (k = ZEPHYRUM; k < initus.mensura; k++)
    {
        i8 c = initus.datum[k];
        i8 proximus = (k + I < initus.mensura)
            ? initus.datum[k + I] : (i8)'\0';

        si (in_commento_lineae)
        {
            si (c == (i8)'\n')
            {
                in_commento_lineae = FALSUM;
            }
            perge;
        }
        si (in_commento)
        {
            si (c == (i8)'*' && proximus == (i8)'/')
            {
                in_commento = FALSUM;
                k++;
            }
            perge;
        }
        si (in_chorda)
        {
            si (c == (i8)'\\')
            {
                k++;
            }
            alioquin si (c == (i8)'"')
            {
                in_chorda = FALSUM;
            }
            perge;
        }
        si (in_charactere)
        {
            si (c == (i8)'\\')
            {
                k++;
            }
            alioquin si (c == (i8)'\'')
            {
                in_charactere = FALSUM;
            }
            perge;
        }
        commutatio ((integer)c)
        {
            casus '/':
                si (proximus == (i8)'*')
                {
                    in_commento = VERUM;
                    k++;
                }
                alioquin si (proximus == (i8)'/')
                {
                    in_commento_lineae = VERUM;
                    k++;
                }
                frange;
            casus '"':  in_chorda = VERUM;     frange;
            casus '\'': in_charactere = VERUM; frange;
            casus '(':  rotundae++;            frange;
            casus ')':  rotundae--;            frange;
            casus '[':  quadrae++;             frange;
            casus ']':  quadrae--;             frange;
            casus '{':  librae++;              frange;
            casus '}':  librae--;              frange;
            ordinarius:                        frange;
        }
    }
    redde (rotundae <= ZEPHYRUM && quadrae <= ZEPHYRUM
        && librae <= ZEPHYRUM && !in_chorda && !in_charactere
        && !in_commento) ? VERUM : FALSUM;
}

/* ==================================================
 * API: turnum offerre
 * ================================================== */

interior SessioRelatum
_relatum_vacuum (vacuum)
{
    SessioRelatum r;

    memset(&r, ZEPHYRUM, magnitudo(SessioRelatum));
    r.verdictum = SESSIO_APPARATUS;
    r.genus = -I;
    r.turnus_substitutus = -I;
    r.turnus_index = -I;
    r.halitus_genus = -I;
    r.halitus_turnus = -I;
    redde r;
}

/* visum operans construere: turni exsistentes (cum substitutione
 * facultativa) + candidatus (si index_substituti < 0) */
interior TurnusVisus*
_visum_construere (Sessio* s, Piscina* piscina, chorda candidatus,
    s32 genus_candidati, chorda involucrum_candidati,
    chorda typus_candidati, s32 index_substituti, i32* numerus_out,
    i32* index_novi_out)
{
    i32 n = xar_numerus(s->turni);
    i32 summa = (index_substituti >= ZEPHYRUM) ? n : n + I;
    TurnusVisus* visus = piscina_allocare(piscina,
        (memoriae_index)summa * magnitudo(TurnusVisus));
    i32 k;

    si (visus == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        TurnusInternus* t = xar_obtinere(s->turni, k);

        si ((s32)k == index_substituti)
        {
            visus[k].textus = candidatus;
            visus[k].genus = genus_candidati;
            visus[k].involucrum = involucrum_candidati;
            visus[k].typus_textus = typus_candidati;
            visus[k].lineae = _lineas_numerare(candidatus);
        }
        alioquin
        {
            visus[k].textus = t->textus;
            visus[k].genus = t->genus;
            visus[k].involucrum = t->involucrum;
            visus[k].typus_textus = t->typus_textus;
            visus[k].lineae = t->lineae;
        }
    }
    si (index_substituti < ZEPHYRUM)
    {
        visus[n].textus = candidatus;
        visus[n].genus = genus_candidati;
        visus[n].involucrum = involucrum_candidati;
        visus[n].typus_textus = typus_candidati;
        visus[n].lineae = _lineas_numerare(candidatus);
        *index_novi_out = n;
    }
    alioquin
    {
        *index_novi_out = (i32)index_substituti;
    }
    *numerus_out = summa;
    redde visus;
}

/* involucrum novum eligere (collisione vitata per textum ostensum) */
interior chorda
_involucrum_eligere (Sessio* s, chorda ostensum)
{
    character tentamen[XXXII];

    per (;;)
    {
        sprintf(tentamen, "turnus_%d", (int)s->numerator);
        s->numerator++;
        si (!_continet(ostensum.datum, ostensum.mensura, tentamen))
        {
            redde chorda_ex_literis(tentamen, s->piscina);
        }
    }
}

SessioRelatum
sessio_turnum_offerre (Sessio* s, chorda initus)
{
    SessioRelatum r = _relatum_vacuum();
    SilvaPiscina* effimera = NIHIL;
    Collectio collectio;
    chorda candidatus;
    chorda praecisus;
    s32 genus = -I;
    chorda titulus_decl;
    chorda involucrum;
    chorda typus_textus;
    s32 valor_genus = SESSIO_VALOR_NULLUS;
    s32 index_substituti = -I;
    b32 reparatum = FALSUM;

    titulus_decl.mensura = ZEPHYRUM;
    titulus_decl.datum = NIHIL;
    involucrum.mensura = ZEPHYRUM;
    involucrum.datum = NIHIL;
    typus_textus.mensura = ZEPHYRUM;
    typus_textus.datum = NIHIL;
    candidatus.mensura = ZEPHYRUM;
    candidatus.datum = NIHIL;

    si (s == NIHIL)
    {
        redde r;
    }
    collectio.xar = xar_creare(s->piscina,
        (i32)magnitudo(SessioDiagnosticum));
    collectio.reice = FALSUM;
    si (collectio.xar == NIHIL)
    {
        redde r;
    }

    praecisus = _praecisa(initus);
    si (praecisus.mensura == ZEPHYRUM)
    {
        r.verdictum = SESSIO_REIECTUM;
        _diagnosticum_addere(&collectio, s->piscina,
            chorda_ex_literis(SESSIO_VIA_DOCUMENTI, s->piscina),
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, -I, FALSUM, FALSUM, -I,
            "turnus vacuus");
        salta finis;
    }

    /* directiva: sine involucro, sine reparatione, sine
     * classificatione (elementa capitum inclusa multa licent) */
    si (praecisus.datum[ZEPHYRUM] == (i8)'#')
    {
        genus = SESSIO_TURNUS_DIRECTIVA;
        candidatus = _normalizare(s->piscina, initus, FALSUM);
        si (candidatus.datum == NIHIL)
        {
            salta finis;
        }
    }
    alioquin
    {
        /* classificatio: parsura textus OSTENSI + candidati nudi
         * (grammatica liberalis); reparatio `;` si syntaxis fracta */
        candidatus = _normalizare(s->piscina, initus, FALSUM);
        si (candidatus.datum == NIHIL)
        {
            salta finis;
        }
        effimera = silva_piscina_generare_dynamicum(
            "sessio_classificatio", 268435456);
        si (effimera == NIHIL)
        {
            salta finis;
        }
        {
            Piscina* piscina_visus = s->piscina;
            i32 visus_numerus = ZEPHYRUM;
            i32 index_novi = ZEPHYRUM;
            TurnusVisus* visus = _visum_construere(s, piscina_visus,
                candidatus, SESSIO_TURNUS_DECLARATIO, involucrum,
                typus_textus, -I, &visus_numerus, &index_novi);
            chorda ostensum;
            SilvaParsura* parsura = NIHIL;
            SilvaSemantica* sem;
            i32 initium_novi;

            si (visus == NIHIL)
            {
                salta finis;
            }
            ostensum = _ostensum_texere(visus, visus_numerus,
                piscina_visus);
            si (ostensum.datum == NIHIL)
            {
                salta finis;
            }
            initium_novi = visus[index_novi].ostensum_initium;
            sem = _iudicare(s, effimera, SESSIO_VIA_DOCUMENTI,
                (constans character*)ostensum.datum,
                ostensum.mensura, &parsura);
            si (sem == NIHIL || parsura == NIHIL)
            {
                salta finis;
            }

            /* reparatio C1: syntaxis fracta + candidatus sine
             * terminatore -> tentamen cum `;` */
            si (parsura->numerus_errorum > ZEPHYRUM)
            {
                i8 ultimus = praecisus.datum[praecisus.mensura - I];

                si (ultimus != (i8)';' && ultimus != (i8)'}')
                {
                    chorda reparatus = _normalizare(s->piscina,
                        initus, VERUM);
                    TurnusVisus* visus2;
                    chorda ostensum2;
                    SilvaParsura* parsura2 = NIHIL;
                    SilvaSemantica* sem2;

                    si (reparatus.datum == NIHIL)
                    {
                        salta finis;
                    }
                    visus2 = _visum_construere(s, piscina_visus,
                        reparatus, SESSIO_TURNUS_DECLARATIO,
                        involucrum, typus_textus, -I, &visus_numerus,
                        &index_novi);
                    si (visus2 == NIHIL)
                    {
                        salta finis;
                    }
                    ostensum2 = _ostensum_texere(visus2,
                        visus_numerus, piscina_visus);
                    sem2 = _iudicare(s, effimera,
                        SESSIO_VIA_DOCUMENTI,
                        (constans character*)ostensum2.datum,
                        ostensum2.mensura, &parsura2);
                    si (sem2 != NIHIL && parsura2 != NIHIL
                        && parsura2->numerus_errorum == ZEPHYRUM)
                    {
                        candidatus = reparatus;
                        reparatum = VERUM;
                        visus = visus2;
                        ostensum = ostensum2;
                        parsura = parsura2;
                        sem = sem2;
                        initium_novi =
                            visus[index_novi].ostensum_initium;
                    }
                }
            }

            si (parsura->numerus_errorum > ZEPHYRUM)
            {
                /* syntaxis manet fracta: REICE cum extensionibus
                 * errorum (classificatio = textus ostensus, lineae
                 * iam ostensae) */
                r.verdictum = SESSIO_REIECTUM;
                _colligere(s, &collectio, parsura, sem, visus,
                    visus_numerus, index_novi);
                collectio.reice = VERUM;
                salta finis;
            }

            /* elementa nova numerare (fons princeps + intervallum
             * novum) et genus capere */
            {
                SilvaValor radix = parsura->commissio->radix;
                insignatus integer n_rad =
                    silva_valor_lista_numerus(radix);
                insignatus integer k;
                constans SilvaNodus* elementum_novum = NIHIL;
                i32 numerus_novorum = ZEPHYRUM;

                per (k = ZEPHYRUM; k < n_rad; k++)
                {
                    SilvaValor* v = silva_valor_lista_obtinere(radix,
                        k);
                    integer fons_index;
                    i32 linea;

                    si (v == NIHIL || v->genus != SILVA_VALOR_NODUS
                        || v->datum.nodus == NIHIL)
                    {
                        perge;
                    }
                    si (!_elementum_positio(v->datum.nodus,
                            &fons_index, &linea))
                    {
                        perge;
                    }
                    si (fons_index != parsura->fons_princeps
                        || linea < initium_novi)
                    {
                        perge;
                    }
                    numerus_novorum++;
                    si (elementum_novum == NIHIL)
                    {
                        elementum_novum = v->datum.nodus;
                    }
                }
                si (numerus_novorum == ZEPHYRUM
                    || elementum_novum == NIHIL)
                {
                    r.verdictum = SESSIO_REIECTUM;
                    _diagnosticum_addere(&collectio, s->piscina,
                        chorda_ex_literis(SESSIO_VIA_DOCUMENTI,
                            s->piscina),
                        initium_novi, I, ZEPHYRUM, -I, FALSUM,
                        FALSUM, -I, "turnus sine elemento");
                    collectio.reice = VERUM;
                    salta finis;
                }
                si (numerus_novorum > I)
                {
                    r.verdictum = SESSIO_REIECTUM;
                    _diagnosticum_addere(&collectio, s->piscina,
                        chorda_ex_literis(SESSIO_VIA_DOCUMENTI,
                            s->piscina),
                        initium_novi, I, ZEPHYRUM, -I, FALSUM,
                        FALSUM, -I,
                        "turnus multi-elementorum - involve { }");
                    collectio.reice = VERUM;
                    salta finis;
                }
                genus = _genus_turni_ex_nodo(elementum_novum);
                si (genus == SESSIO_TURNUS_DECLARATIO
                    || genus == SESSIO_TURNUS_DEFINITIO)
                {
                    titulus_decl = _titulum_declaratum(
                        elementum_novum, s->piscina);
                }
                /* chunk C: typatio (redditor - dum sem CLASSIFICATIONIS
                 * vivit; TypusC89* in effimeram monstrat, textus
                 * SOLUS superstes) */
                si (genus == SESSIO_TURNUS_EXPRESSIO)
                {
                    SilvaValor expr_v =
                        silva_c89_sententia_expressionis_expressio(
                            elementum_novum);

                    si (expr_v.genus == SILVA_VALOR_NODUS
                        && expr_v.datum.nodus != NIHIL)
                    {
                        constans TypusC89* t =
                            silva_c89_typus_expressionis(sem,
                                expr_v.datum.nodus);

                        valor_genus = _valor_genus_ex_typo(t);
                        si (valor_genus != SESSIO_VALOR_NULLUS)
                        {
                            typus_textus = _typum_reddere(t,
                                s->piscina);
                            si (typus_textus.mensura == ZEPHYRUM)
                            {
                                valor_genus = SESSIO_VALOR_NULLUS;
                            }
                        }
                    }
                    /* aggregata: typus OSTENDITUR, involucrum VOID
                     * manet (%reditus parca) - typus_involucri infra */
                }
                alioquin si ((genus == SESSIO_TURNUS_DECLARATIO
                        || genus == SESSIO_TURNUS_DEFINITIO)
                    && titulus_decl.mensura > ZEPHYRUM)
                {
                    /* echo declarationis: nexus symbolorum sedes
                     * USUS solas notat (declaratores absunt) -
                     * parsura-proba cum usu synthetico appenso
                     * (`nomen;`) typum declaratum dat */
                    i32 m_proba = ostensum.mensura
                        + titulus_decl.mensura + IV;
                    i8* textus_probae = piscina_allocare(s->piscina,
                        (memoriae_index)m_proba);

                    si (textus_probae != NIHIL)
                    {
                        SilvaParsura* parsura_probae = NIHIL;
                        SilvaSemantica* sem_probae;
                        i32 cursor_probae = ostensum.mensura;

                        memcpy(textus_probae, ostensum.datum,
                            (memoriae_index)ostensum.mensura);
                        memcpy(textus_probae + cursor_probae,
                            titulus_decl.datum,
                            (memoriae_index)titulus_decl.mensura);
                        cursor_probae += titulus_decl.mensura;
                        textus_probae[cursor_probae] = (i8)';';
                        textus_probae[cursor_probae + I] = (i8)'\n';
                        cursor_probae += II;
                        sem_probae = _iudicare(s, effimera,
                            SESSIO_VIA_DOCUMENTI,
                            (constans character*)textus_probae,
                            cursor_probae, &parsura_probae);
                        si (sem_probae != NIHIL
                            && parsura_probae != NIHIL)
                        {
                            SilvaValor radix_probae =
                                parsura_probae->commissio->radix;
                            insignatus integer n_probae =
                                silva_valor_lista_numerus(
                                    radix_probae);
                            SilvaValor* ultimum = NIHIL;
                            insignatus integer kp;

                            /* ultimum elementum PRINCIPIS (radix
                             * elementa lexici quoque continet!) */
                            per (kp = ZEPHYRUM; kp < n_probae; kp++)
                            {
                                SilvaValor* vp =
                                    silva_valor_lista_obtinere(
                                        radix_probae, kp);
                                integer fons_p;
                                i32 linea_p;

                                si (vp != NIHIL
                                    && vp->genus == SILVA_VALOR_NODUS
                                    && vp->datum.nodus != NIHIL
                                    && _elementum_positio(
                                        vp->datum.nodus, &fons_p,
                                        &linea_p)
                                    && fons_p
                                        == parsura_probae->fons_princeps)
                                {
                                    ultimum = vp;
                                }
                            }
                            {
                                si (ultimum != NIHIL
                                    && ultimum->genus
                                        == SILVA_VALOR_NODUS
                                    && ultimum->datum.nodus != NIHIL
                                    && ultimum->datum.nodus->genus
                                        == (integer)
                                    SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS)
                                {
                                    SilvaValor expr_v =
                            silva_c89_sententia_expressionis_expressio(
                                        ultimum->datum.nodus);

                                    si (expr_v.genus
                                            == SILVA_VALOR_NODUS
                                        && expr_v.datum.nodus
                                            != NIHIL)
                                    {
                                        constans TypusC89* t =
                                        silva_c89_typus_expressionis(
                                            sem_probae,
                                            expr_v.datum.nodus);
                                        constans TypusC89* nudus =
                                            _qualibus_nudatum(t);

                                        si (nudus != NIHIL
                                            && nudus->genus
                                            == TYPUS_C89_FUNCTIO)
                                        {
                                            typus_textus =
                                                chorda_ex_literis(
                                                "functio",
                                                s->piscina);
                                        }
                                        alioquin
                                        {
                                            typus_textus =
                                                _typum_reddere(t,
                                                s->piscina);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                si (genus == SESSIO_TURNUS_DECLARATIO)
                {
                    constans character* nuntius_init = NIHIL;
                    constans SilvaNodus* initiatus_nodus = NIHIL;

                    si (_initiatorem_probare(sem, elementum_novum,
                            &nuntius_init, &initiatus_nodus))
                    {
                        /* SCISSIO AUTOMATICA (interludium M4b):
                         * `chorda s = f(...)` -> `chorda s;` +
                         * `s = f(...);` - exemplar REPL naturale;
                         * C89 initiatorem non-constantem vetat,
                         * machina dividit pro usore */
                        SilvaValor tok_op = (initiatus_nodus != NIHIL)
                            ? silva_c89_declarator_initiatus_tok_operator(
                                initiatus_nodus)
                            : silva_c89_declarator_initiatus_tok_operator(
                                NIHIL);
                        SilvaToken* op_radix = NIHIL;

                        si (tok_op.genus == SILVA_VALOR_TOKEN
                            && tok_op.datum.token != NIHIL)
                        {
                            op_radix = silva_token_radix(
                                tok_op.datum.token);
                            si (op_radix == NIHIL)
                            {
                                op_radix = tok_op.datum.token;
                            }
                        }
                        si (op_radix != NIHIL
                            && op_radix->byte_offset >= ZEPHYRUM
                            && titulus_decl.mensura > ZEPHYRUM)
                        {
                            i32 initium_candidati = ostensum.mensura
                                - candidatus.mensura;
                            i32 scissura =
                                (i32)op_radix->byte_offset
                                - initium_candidati;

                            si (scissura > ZEPHYRUM
                                && scissura < candidatus.mensura)
                            {
                                chorda pars_decl;
                                chorda pars_assign;
                                ChordaAedificator* aed;
                                SessioRelatum r_decl;

                                pars_decl = _normalizare(s->piscina,
                                    chorda_sectio(candidatus,
                                        ZEPHYRUM, scissura), VERUM);
                                aed = chorda_aedificator_creare(
                                    s->piscina, CXXVIII);
                                si (pars_decl.datum == NIHIL
                                    || aed == NIHIL)
                                {
                                    salta finis;
                                }
                                (vacuum)
                                chorda_aedificator_appendere_chorda(
                                    aed, titulus_decl);
                                (vacuum)
                                chorda_aedificator_appendere_literis(
                                    aed, " =");
                                (vacuum)
                                chorda_aedificator_appendere_chorda(
                                    aed, chorda_sectio(candidatus,
                                        scissura
                                        + (i32)op_radix->longitudo,
                                        candidatus.mensura));
                                pars_assign =
                                    chorda_aedificator_finire(aed);

                                silva_piscina_destruere(effimera);
                                effimera = NIHIL;
                                r_decl = sessio_turnum_offerre(s,
                                    pars_decl);
                                si (r_decl.verdictum
                                    != SESSIO_ACCEPTUM)
                                {
                                    redde r_decl;
                                }
                                r = sessio_turnum_offerre(s,
                                    pars_assign);
                                r.divisum = VERUM;
                                r.nomen_declaratum =
                                    r_decl.nomen_declaratum;
                                redde r;
                            }
                        }
                        /* scissio non potuit: reiectio ut prius */
                        r.verdictum = SESSIO_REIECTUM;
                        _diagnosticum_addere(&collectio, s->piscina,
                            chorda_ex_literis(SESSIO_VIA_DOCUMENTI,
                                s->piscina),
                            initium_novi, I, ZEPHYRUM, -I, FALSUM,
                            FALSUM, -I, nuntius_init);
                        collectio.reice = VERUM;
                        salta finis;
                    }
                }
            }
        }
        silva_piscina_destruere(effimera);
        effimera = NIHIL;
    }

    /* religatio (substitutio in loco): titulus declaratus unicus
     * congruens turno priori declaranti/definienti */
    si (titulus_decl.mensura > ZEPHYRUM)
    {
        i32 n = xar_numerus(s->turni);
        i32 k;

        per (k = ZEPHYRUM; k < n; k++)
        {
            TurnusInternus* t = xar_obtinere(s->turni, k);

            si ((t->genus == SESSIO_TURNUS_DECLARATIO
                    || t->genus == SESSIO_TURNUS_DEFINITIO)
                && t->titulus.mensura > ZEPHYRUM
                && chorda_aequalis(t->titulus, titulus_decl))
            {
                index_substituti = (s32)k;
                frange;
            }
        }
    }

    /* iudicium: textus involutus documenti totius */
    {
        i32 visus_numerus = ZEPHYRUM;
        i32 index_novi = ZEPHYRUM;
        TurnusVisus* visus;
        chorda involutum;
        SilvaParsura* parsura = NIHIL;
        SilvaSemantica* sem;

        si (_involvendus(genus))
        {
            chorda ostensum_praesens = sessio_documentum(s,
                s->piscina);

            involucrum = _involucrum_eligere(s,
                ostensum_praesens.datum == NIHIL
                    ? candidatus : ostensum_praesens);
        }
        {
            chorda typus_involucri = typus_textus;

            si (valor_genus == SESSIO_VALOR_AGGREGATUM
                || valor_genus == SESSIO_VALOR_NULLUS)
            {
                typus_involucri.mensura = ZEPHYRUM;
                typus_involucri.datum = NIHIL;
            }
            visus = _visum_construere(s, s->piscina, candidatus,
                genus, involucrum, typus_involucri,
                index_substituti, &visus_numerus, &index_novi);
        }
        si (visus == NIHIL)
        {
            salta finis;
        }
        /* ostensum primum texere (implet ostensum_initium) */
        (vacuum)_ostensum_texere(visus, visus_numerus, s->piscina);
        involutum = _involutum_texere(visus, visus_numerus,
            s->piscina);
        si (involutum.datum == NIHIL)
        {
            salta finis;
        }
        effimera = silva_piscina_generare_dynamicum("sessio_iudicium",
            268435456);
        si (effimera == NIHIL)
        {
            salta finis;
        }
        sem = _iudicare(s, effimera, SESSIO_VIA_DOCUMENTI,
            (constans character*)involutum.datum, involutum.mensura,
            &parsura);
        si (sem == NIHIL || parsura == NIHIL)
        {
            salta finis;
        }
        _colligere(s, &collectio, parsura, sem, visus, visus_numerus,
            index_novi);

        si (collectio.reice)
        {
            r.verdictum = SESSIO_REIECTUM;
            salta finis;
        }

        /* ACCIPE iudicio: demissio (effimera adhuc viva - fenestra
         * collige-ante-destrue), tum mundus + replicatio (chunk B) */
        {
            MedullaModulus* modulus_doc;
            Piscina* generatio_nova;
            chorda* capturae;
            chorda* capturae_err;
            Xar* mutati;
            b32 viridis;

            generatio_nova = piscina_generare_dynamicum(
                "sessio_generatio", 67108864);
            si (generatio_nova == NIHIL)
            {
                salta finis;
            }
            modulus_doc = demissio_currere(generatio_nova, parsura,
                sem, chorda_ex_literis(SESSIO_VIA_DOCUMENTI,
                    s->piscina));
            si (modulus_doc == NIHIL)
            {
                piscina_destruere(generatio_nova);
                r.verdictum = SESSIO_REIECTUM;
                _diagnosticum_addere(&collectio, s->piscina,
                    chorda_ex_literis(SESSIO_VIA_DOCUMENTI,
                        s->piscina),
                    visus[index_novi].ostensum_initium, I, ZEPHYRUM,
                    -I, FALSUM, FALSUM, -I,
                    "demissio fracta (initiator non constans?"
                    " divide declarationem et assignationem)");
                salta finis;
            }
            silva_piscina_destruere(effimera);
            effimera = NIHIL;

            /* mundus novus (iudicium ANTE demolitionem factum, C8) */
            _generationem_demoliri(s);
            s->piscina_generationis = generatio_nova;
            si (!_mundum_aedificare(s, modulus_doc))
            {
                _generationem_demoliri(s);
                _mundum_restituere(s);
                salta finis;
            }

            capturae = piscina_allocare(s->piscina,
                (memoriae_index)visus_numerus * magnitudo(chorda));
            capturae_err = piscina_allocare(s->piscina,
                (memoriae_index)visus_numerus * magnitudo(chorda));
            mutati = xar_creare(s->piscina, (i32)magnitudo(s32));
            si (capturae == NIHIL || capturae_err == NIHIL
                || mutati == NIHIL)
            {
                salta finis;
            }
            memset(capturae, ZEPHYRUM,
                (memoriae_index)visus_numerus * magnitudo(chorda));
            memset(capturae_err, ZEPHYRUM,
                (memoriae_index)visus_numerus * magnitudo(chorda));

            viridis = _replicare(s, visus, visus_numerus,
                (i32)index_novi, capturae, capturae_err, mutati, &r);
            r.exsecutum = VERUM;
            si (!viridis)
            {
                /* C8: turnus reicitur, mundus vetus restituitur
                 * (determinismus restitutionem correctam facit) */
                r.verdictum = SESSIO_REIECTUM;
                _generationem_demoliri(s);
                _mundum_restituere(s);
                salta finis;
            }

            /* committere (appensio aut substitutio) */
            si (index_substituti >= ZEPHYRUM)
            {
                TurnusInternus* t = xar_obtinere(s->turni,
                    (i32)index_substituti);

                t->textus = candidatus;
                t->genus = genus;
                t->titulus = titulus_decl;
                t->typus_textus = (valor_genus
                        == SESSIO_VALOR_AGGREGATUM
                    || valor_genus == SESSIO_VALOR_NULLUS)
                    ? _chorda_vacua() : typus_textus;
                t->valor_genus = valor_genus;
                t->lineae = _lineas_numerare(candidatus);
                /* involucrum immotum (declarationes numquam
                 * involutae) */
                r.turnus_index = index_substituti;
                r.substitutus = VERUM;
                r.turnus_substitutus = index_substituti;
            }
            alioquin
            {
                TurnusInternus* t = xar_addere(s->turni);

                si (t == NIHIL)
                {
                    salta finis;
                }
                memset(t, ZEPHYRUM, magnitudo(TurnusInternus));
                t->textus = candidatus;
                t->genus = genus;
                t->titulus = titulus_decl;
                t->involucrum = involucrum;
                t->typus_textus = (valor_genus
                        == SESSIO_VALOR_AGGREGATUM
                    || valor_genus == SESSIO_VALOR_NULLUS)
                    ? _chorda_vacua() : typus_textus;
                t->valor_genus = valor_genus;
                t->lineae = _lineas_numerare(candidatus);
                r.turnus_index = (s32)(xar_numerus(s->turni) - I);
            }

            /* actas condere (omnes turni involuti) */
            {
                i32 k;

                per (k = ZEPHYRUM; k < visus_numerus; k++)
                {
                    TurnusInternus* t = xar_obtinere(s->turni, k);

                    si (t != NIHIL
                        && t->involucrum.mensura > ZEPHYRUM)
                    {
                        t->effusio = capturae[k];
                        t->effusio_erroris = capturae_err[k];
                        t->effusio_valida = VERUM;
                    }
                }
            }
            r.effusio = capturae[index_novi];
            r.effusio_erroris = capturae_err[index_novi];
            {
                i32 n_mut = xar_numerus(mutati);

                si (n_mut > ZEPHYRUM)
                {
                    s32* ordo = piscina_allocare(s->piscina,
                        (memoriae_index)n_mut * magnitudo(s32));
                    i32 k;

                    si (ordo != NIHIL)
                    {
                        per (k = ZEPHYRUM; k < n_mut; k++)
                        {
                            ordo[k] = *(s32*)xar_obtinere(mutati, k);
                        }
                        r.turni_mutati = ordo;
                        r.turni_mutati_numerus = (s32)n_mut;
                    }
                }
            }
            r.verdictum = SESSIO_ACCEPTUM;
        }
    }

finis:
    si (effimera != NIHIL)
    {
        silva_piscina_destruere(effimera);
    }
    r.genus = genus;
    r.reparatum = reparatum;
    r.textus = candidatus;
    r.nomen_declaratum = titulus_decl;
    r.typus_textus = typus_textus;
    r.valor_genus = valor_genus;
    /* diagnostica in ordinem contiguum copiare */
    {
        i32 n = xar_numerus(collectio.xar);

        si (n > ZEPHYRUM)
        {
            SessioDiagnosticum* ordo = piscina_allocare(s->piscina,
                (memoriae_index)n * magnitudo(SessioDiagnosticum));
            i32 k;

            si (ordo != NIHIL)
            {
                per (k = ZEPHYRUM; k < n; k++)
                {
                    SessioDiagnosticum* d = xar_obtinere(
                        collectio.xar, k);

                    ordo[k] = *d;
                }
                r.diagnostica = ordo;
                r.diagnostica_numerus = (s32)n;
            }
        }
    }
    redde r;
}
