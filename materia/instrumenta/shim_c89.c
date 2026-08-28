/* shim_c89.c - PORTA: emissorem materiae contra SILVAM IPSAM conferre
 *
 * Arborem C89 a silva parsatam in typos materiae vertit, per
 * materia_scribere emittit, et contra emissionem SILVAE ipsius
 * OCTETIM confert.
 *
 * CUR HOC ANTE MATERIA_ARBOR. Porta phasis I ('shim C89 CCLXXXI/CCLXXXI
 * per materiam') circuitum STML poscit, ergo materia_arbor - V,DLXXIII
 * lineas. Sed dimidium OCTETORUM illius portae iam NUNC parari potest,
 * et id ipsum est quod uncos originis probat (radix, extentum,
 * scissurae). Portatio V,DLXXIII linearum consilio improbato inniti
 * NON debet.
 *
 * ORACULUM SEPARANS: silva ipsa iudicat, non fixtura a me scripta.
 * Fixtura sumptiones meas communicat; silva non.
 *
 * SEDES TEMPORARIA. Hoc instrumentum C89 SCIT - materia nescire debet
 * (M8). Ergo hic vivit, non in probationibus materiae, et phasis V eum
 * retirat cum silva materiam consumat.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_lexema.h"
#include "materia_token.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_arbor.h"
#include "lexicon_c89.h"
#include "silva_arbor.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Cauda lexematis materiae: lexema silvae unde venit */
nomen structura { SilvaToken* silva; } CaudaShim;
hic_manens constans MateriaTokenForma FORMA = { (i32)magnitudo(CaudaShim) };

nomen structura {
    Piscina*                piscina;
    Xar*                    lexemata;   /* MateriaToken* omnia */
    TabulaDispersa*         index;      /* SilvaToken* -> MateriaToken* */
    constans SilvaExpansio* expansio;
} Shim;

hic_manens Shim SHIM;

/* Clavis ex punctatore: octeti eius in piscina servati (chorda
 * stabilem memoriam poscit). Quaestio linearis prior O(n^2) erat
 * et super plagulas veras EXCESSIT - defectus probae, non consilii. */
hic_manens chorda
_clavis (SilvaToken* t)
{
    chorda c;
    SilvaToken** cella = (SilvaToken**)piscina_allocare_ordinatum(
        SHIM.piscina, magnitudo(SilvaToken*), magnitudo(vacuum*));

    *cella = t;
    c.datum   = (i8*)cella;
    c.mensura = (i32)magnitudo(SilvaToken*);
    redde c;
}


/* ---------- conversio registri ---------- */

hic_manens MateriaRegistrumCoctum
_registrum_convertere (Piscina* p, constans SilvaRegistrumCoctum* s)
{
    MateriaRegistrumCoctum r;
    MateriaTabGenus* g = (MateriaTabGenus*)piscina_allocare_ordinatum(p,
        (memoriae_index)s->numerus_generum * magnitudo(MateriaTabGenus),
        magnitudo(vacuum*));
    MateriaTabLocus* l = (MateriaTabLocus*)piscina_allocare_ordinatum(p,
        (memoriae_index)s->numerus_locorum * magnitudo(MateriaTabLocus),
        magnitudo(vacuum*));
    i32 i;

    per (i = ZEPHYRUM; i < s->numerus_generum; i++)
    {
        g[i].titulus      = s->genera[i].titulus;
        g[i].loci_offset  = s->genera[i].loci_offset;
        g[i].loci_numerus = s->genera[i].loci_numerus;
    }
    per (i = ZEPHYRUM; i < s->numerus_locorum; i++)
    {
        l[i].titulus = s->loci[i].titulus;
        l[i].species = s->loci[i].species;
    }
    r.genera = g; r.numerus_generum = s->numerus_generum;
    r.loci   = l; r.numerus_locorum = s->numerus_locorum;
    redde r;
}

/* ---------- conversio lexematum ---------- */

hic_manens MateriaToken*
_quaerere (SilvaToken* s)
{
    vacuum* valor;
    chorda  c;
    SilvaToken* cella[1];

    cella[0] = s;
    c.datum   = (i8*)cella;
    c.mensura = (i32)magnitudo(SilvaToken*);
    si (tabula_dispersa_invenire(SHIM.index, c, &valor))
    {
        redde (MateriaToken*)valor;
    }
    redde NIHIL;
}

hic_manens MateriaToken*
_token_convertere (SilvaToken* s)
{
    MateriaToken* m;
    i32 i;

    si (s == NIHIL) { redde NIHIL; }
    m = _quaerere(s);
    si (m != NIHIL) { redde m; }

    m = materia_token_creare(SHIM.piscina, &FORMA, (s32)s->genus,
        s->valor, s->byte_offset, s->linea, s->columna, s->fons_index);
    si (m == NIHIL) { redde NIHIL; }
    ((CaudaShim*)materia_token_cauda(m))->silva = s;
    materia_token_initium_lineae_ponere(m, s->initium_lineae);
    *(MateriaToken**)xar_addere(SHIM.lexemata) = m;
    tabula_dispersa_inserere(SHIM.index, _clavis(s), m);

    /* trivia - exacta */
    {
        i32 n = xar_numerus(s->spatia_ante);

        si (n > ZEPHYRUM)
        {
            MateriaToken** ser = (MateriaToken**)piscina_allocare_ordinatum(
                SHIM.piscina, (memoriae_index)n * magnitudo(MateriaToken*),
                magnitudo(vacuum*));
            per (i = ZEPHYRUM; i < n; i++)
            {
                ser[i] = _token_convertere(
                    *(SilvaToken**)xar_obtinere(s->spatia_ante, i));
            }
            materia_token_trivia_ante_ponere(m, SHIM.piscina, ser, n);
        }
        n = xar_numerus(s->spatia_post);
        si (n > ZEPHYRUM)
        {
            MateriaToken** ser = (MateriaToken**)piscina_allocare_ordinatum(
                SHIM.piscina, (memoriae_index)n * magnitudo(MateriaToken*),
                magnitudo(vacuum*));
            per (i = ZEPHYRUM; i < n; i++)
            {
                ser[i] = _token_convertere(
                    *(SilvaToken**)xar_obtinere(s->spatia_post, i));
            }
            materia_token_trivia_post_ponere(m, SHIM.piscina, ser, n);
        }
    }
    redde m;
}

/* ---------- conversio arboris ---------- */

hic_manens MateriaNodus* _nodus_convertere (SilvaNodus* s);

hic_manens MateriaValor
_valor_convertere (SilvaValor v)
{
    commutatio (v.genus)
    {
    casus SILVA_VALOR_NIHIL:  redde materia_valor_nihil();
    casus SILVA_VALOR_INDEX:  redde materia_valor_index(v.datum.index);
    casus SILVA_VALOR_TOKEN:
        redde materia_valor_token(_token_convertere(v.datum.token));
    casus SILVA_VALOR_NODUS:
        redde materia_valor_nodus(_nodus_convertere(v.datum.nodus));
    casus SILVA_VALOR_LISTA:
    {
        MateriaValor lista = materia_valor_lista_nova(SHIM.piscina);
        i32 i;
        i32 n = silva_valor_lista_numerus(v);

        per (i = ZEPHYRUM; i < n; i++)
        {
            SilvaValor* e = silva_valor_lista_obtinere(v, i);

            si (e != NIHIL)
            {
                lista = materia_valor_lista_appendere(SHIM.piscina,
                    lista, _valor_convertere(*e));
            }
        }
        redde lista;
    }
    ordinarius: redde materia_valor_nihil();
    }
}

hic_manens MateriaNodus*
_nodus_convertere (SilvaNodus* s)
{
    MateriaNodus* m;
    i32 i;

    si (s == NIHIL) { redde NIHIL; }
    m = materia_nodus_creare(SHIM.piscina, s->genus, s->numerus_locorum);
    si (m == NIHIL) { redde NIHIL; }
    per (i = ZEPHYRUM; i < s->numerus_locorum; i++)
    {
        m->loci[i] = _valor_convertere(s->loci[i]);
    }
    redde m;
}

/* ---------- unci C89 (portati ex silva_scribere.c) ---------- */

hic_manens SilvaToken*
_radix_silvae (SilvaToken* t, b32* impurum)
{
    dum (t != NIHIL)
    {
        commutatio (t->origo.genus)
        {
        casus SILVA_ORIGO_FONS:      redde t;
        casus SILVA_ORIGO_EXPANSIO:
            t = t->origo.datum.expansio.invocatio; frange;
        casus SILVA_ORIGO_CHORDA:
            t = t->origo.datum.stringificatio.primus; frange;
        casus SILVA_ORIGO_PASTA:
            si (t->origo.datum.pasta.invocatio == NIHIL)
            { *impurum = VERUM; redde t; }
            t = t->origo.datum.pasta.invocatio; frange;
        ordinarius: *impurum = VERUM; redde t;
        }
    }
    *impurum = VERUM;
    redde NIHIL;
}

hic_manens MateriaToken*
_radix_quaerere (vacuum* datum, MateriaToken* token,
                 constans character** causa)
{
    SilvaToken* s;
    SilvaToken* r;
    b32 impurum = FALSUM;

    (vacuum)datum;
    s = ((CaudaShim*)materia_token_cauda(token))->silva;
    si (s->origo.genus == SILVA_ORIGO_FONS) { redde token; }
    si (SHIM.expansio == NIHIL)
    {
        *causa = "lexema expansum sine contextu expansionis";
        redde NIHIL;
    }
    r = _radix_silvae(s, &impurum);
    si (impurum || r == NIHIL)
    {
        *causa = "origo pasta/chorda/api - stratum 0 non "
                 "recuperabile (deferral nominatum)";
        redde NIHIL;
    }
    redde _token_convertere(r);
}

/* SEDES: materia hinc discit an lexema DERIVATUM sit. Sine hoc unco
 * omne lexema 'origo sua' videtur et scriptor sedem PORTATAM omittit
 * (silva eam lexemati non-FONS scribit, quia sedes eius DEF-SITE est,
 * in plagula alia, ergo ex hoc fluxu derivari NEQUIT). */
hic_manens vacuum
_sedes_quaerere (vacuum* datum, constans MateriaToken* token,
                 MateriaSedes* sedes)
{
    SilvaToken* s = ((CaudaShim*)materia_token_cauda(token))->silva;

    (vacuum)datum;
    sedes->byte_offset = token->byte_offset;
    sedes->linea       = token->linea;
    sedes->columna     = token->columna;
    sedes->fons_index  = token->fons_index;
    sedes->est_fons    = (b32)(s->origo.genus == SILVA_ORIGO_FONS);
}

hic_manens Xar*
_extentum_quaerere (vacuum* datum, constans MateriaToken* radix)
{
    SilvaToken* r;
    Xar* lamina = NIHIL;
    i32 k;

    (vacuum)datum;
    r = ((CaudaShim*)materia_token_cauda(radix))->silva;
    si (SHIM.expansio == NIHIL || SHIM.expansio->extenta == NIHIL)
    { redde NIHIL; }

    per (k = ZEPHYRUM; k < xar_numerus(SHIM.expansio->extenta); k++)
    {
        SilvaExtentumInvocationis* e = (SilvaExtentumInvocationis*)
            xar_obtinere(SHIM.expansio->extenta, k);

        si (e != NIHIL && e->invocatio == r) { lamina = e->lamina; frange; }
    }
    si (lamina == NIHIL)
    {
        /* continentia (via CHORDA) */
        per (k = ZEPHYRUM; k < xar_numerus(SHIM.expansio->extenta); k++)
        {
            SilvaExtentumInvocationis* e = (SilvaExtentumInvocationis*)
                xar_obtinere(SHIM.expansio->extenta, k);
            SilvaToken* pri; SilvaToken* ult;

            si (e == NIHIL || e->lamina == NIHIL
                || xar_numerus(e->lamina) == ZEPHYRUM) { perge; }
            pri = *(SilvaToken**)xar_obtinere(e->lamina, ZEPHYRUM);
            ult = *(SilvaToken**)xar_obtinere(e->lamina,
                (i32)(xar_numerus(e->lamina) - I));
            si (pri->fons_index == r->fons_index
                && r->byte_offset >= pri->byte_offset
                && r->byte_offset < ult->byte_offset + (s32)ult->longitudo)
            { lamina = e->lamina; frange; }
        }
    }
    si (lamina == NIHIL) { redde NIHIL; }

    /* laminam silvae in laminam materiae vertere */
    {
        Xar* m = xar_creare(SHIM.piscina, magnitudo(MateriaToken*));

        per (k = ZEPHYRUM; k < xar_numerus(lamina); k++)
        {
            *(MateriaToken**)xar_addere(m) = _token_convertere(
                *(SilvaToken**)xar_obtinere(lamina, k));
        }
        redde m;
    }
}

/* scissurae: valorem lexematis cum laminis reinsertis */
hic_manens b32
_valorem_emittere (vacuum* datum, ChordaAedificator* aed,
                   constans MateriaToken* token)
{
    SilvaToken* s;

    (vacuum)datum;
    s = ((CaudaShim*)materia_token_cauda(token))->silva;
    si (s->scissurae == NIHIL)
    {
        chorda_aedificator_appendere_chorda(aed, token->valor);
        redde VERUM;
    }
    {
        i32 i, prius = ZEPHYRUM;

        per (i = ZEPHYRUM; i < xar_numerus(s->scissurae); i++)
        {
            SilvaScissura* sc = (SilvaScissura*)xar_obtinere(s->scissurae, i);

            chorda_aedificator_appendere_chorda(aed,
                chorda_sectio(token->valor, prius, (i32)sc->offset));
            chorda_aedificator_appendere_literis(aed,
                sc->crlf ? "\\\r\n" : "\\\n");
            prius = (i32)sc->offset;
        }
        chorda_aedificator_appendere_chorda(aed,
            chorda_sectio(token->valor, prius, token->valor.mensura));
    }
    redde VERUM;
}


/* ---------- frons C89 pro arbore (GRADATIM) ----------
 * Gradus I: 'standard' solum. Origo et scissurae CONSULTO absunt -
 * probatio dicat quantum absit, ne CCCXL lineae caeco portentur. */

hic_manens b32
_attributa_ornare (vacuum* datum, MateriaArborScriptor* st,
                   StmlNodus* elementum, constans MateriaToken* lexema)
{
    SilvaToken* s = ((CaudaShim*)materia_token_cauda(lexema))->silva;

    (vacuum)datum;
    si (s->standard != (i8)SILVA_STANDARD_C89)
    {
        redde materia_arbor_attributum_numeri(st, elementum, "standard",
            (i32)s->standard);
    }
    redde VERUM;
}

hic_manens vacuum
_origo_numerare (vacuum* datum, constans MateriaToken* lexema,
                 vacuum (*numerare)(vacuum*, constans MateriaToken*),
                 vacuum* ctx)
{
    SilvaToken* s = ((CaudaShim*)materia_token_cauda(lexema))->silva;

    (vacuum)datum;
    commutatio (s->origo.genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        si (s->origo.datum.expansio.invocatio != NIHIL)
        { numerare(ctx, _token_convertere(s->origo.datum.expansio.invocatio)); }
        frange;
    casus SILVA_ORIGO_PASTA:
        si (s->origo.datum.pasta.sinister != NIHIL)
        { numerare(ctx, _token_convertere(s->origo.datum.pasta.sinister)); }
        si (s->origo.datum.pasta.dexter != NIHIL)
        { numerare(ctx, _token_convertere(s->origo.datum.pasta.dexter)); }
        si (s->origo.datum.pasta.invocatio != NIHIL)
        { numerare(ctx, _token_convertere(s->origo.datum.pasta.invocatio)); }
        frange;
    casus SILVA_ORIGO_CHORDA:
        si (s->origo.datum.stringificatio.primus != NIHIL)
        { numerare(ctx, _token_convertere(s->origo.datum.stringificatio.primus)); }
        frange;
    ordinarius: frange;
    }
}


/* Gradus II: scissurae + catena originis nestata.
 * Portatum ex silva_arbor.c _origo_scribere/_extentum_scribere;
 * uncus 'liberos_ornare' scriptorem lexematis RE-INTRAT per
 * materia_arbor_lexema_scribere, ergo machina fragmentorum una
 * manet et identitas per transclusiones servatur. */

hic_manens Xar*
_extentum_laminam_silvae (constans SilvaToken* invocatio)
{
    i32 k;

    si (   SHIM.expansio == NIHIL || SHIM.expansio->extenta == NIHIL
        || invocatio == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(SHIM.expansio->extenta); k++)
    {
        SilvaExtentumInvocationis* e = (SilvaExtentumInvocationis*)
            xar_obtinere(SHIM.expansio->extenta, k);

        si (e != NIHIL && e->invocatio == invocatio) { redde e->lamina; }
    }
    redde NIHIL;
}

hic_manens b32
_extentum_ornare (MateriaArborScriptor* st, StmlNodus* parens, Xar* lamina)
{
    StmlNodus* elem;
          i32  k;

    elem = stml_elementum_creare(materia_arbor_scriptor_piscina(st),
        materia_arbor_scriptor_intern(st), "extentum");
    si (elem == NIHIL)
    {
        materia_arbor_scriptor_recusare(st,
            "elementum extenti creari non potuit");
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(lamina); k++)
    {
        SilvaToken* t = *(SilvaToken**)xar_obtinere(lamina, k);
        StmlNodus*  scriptum;

        si (t == NIHIL) { perge; }
        scriptum = materia_arbor_lexema_scribere(st, _token_convertere(t));
        si (scriptum == NIHIL) { redde FALSUM; }
        si (!stml_liberum_addere(elem, scriptum))
        {
            materia_arbor_scriptor_recusare(st,
                "lexema in extentum addi non potuit");
            redde FALSUM;
        }
    }
    si (!stml_liberum_addere(parens, elem))
    {
        materia_arbor_scriptor_recusare(st,
            "extentum in originem addi non potuit");
        redde FALSUM;
    }
    redde VERUM;
}

hic_manens b32
_liberos_ornare (vacuum* datum, MateriaArborScriptor* st,
                 StmlNodus* elementum, constans MateriaToken* lexema)
{
    SilvaToken*  s = ((CaudaShim*)materia_token_cauda(lexema))->silva;
    Piscina*     p = materia_arbor_scriptor_piscina(st);
    InternamentumChorda* in = materia_arbor_scriptor_intern(st);
    constans character* tag;
    chorda*      titulus_macro;
    SilvaToken*  primus;
    SilvaToken*  secundus;
    SilvaToken*  tertius;
    SilvaToken*  definitio;
    StmlNodus*   elem;
    StmlNodus*   scriptum;
    i32          i;

    (vacuum)datum;

    /* --- scissurae --- */
    si (s->scissurae != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(s->scissurae); i++)
        {
            SilvaScissura* sc = (SilvaScissura*)xar_obtinere(s->scissurae, i);
            StmlNodus*     es;

            si (sc == NIHIL || sc->offset < ZEPHYRUM)
            {
                materia_arbor_scriptor_recusare(st, "scissura corrupta");
                redde FALSUM;
            }
            es = stml_elementum_creare(p, in, "scissura");
            si (   es == NIHIL
                || !materia_arbor_attributum_numeri(st, es, "offset",
                        (i32)sc->offset))
            {
                materia_arbor_scriptor_recusare(st,
                    "scissura scribi non potuit");
                redde FALSUM;
            }
            si (sc->crlf)
            {
                stml_attributum_boolean_addere(es, p, in, "crlf");
            }
            si (!stml_liberum_addere(elementum, es))
            {
                materia_arbor_scriptor_recusare(st,
                    "scissura addi non potuit");
                redde FALSUM;
            }
        }
    }

    /* --- origo nestata --- */
    si (s->origo.genus == SILVA_ORIGO_FONS) { redde VERUM; }

    primus = secundus = tertius = definitio = NIHIL;
    commutatio (s->origo.genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        tag = "expansio";
        titulus_macro = s->origo.datum.expansio.nomen_macro;
        primus        = s->origo.datum.expansio.invocatio;
        definitio     = s->origo.datum.expansio.corpus;
        frange;
    casus SILVA_ORIGO_PASTA:
        tag = "pasta";
        titulus_macro = s->origo.datum.pasta.nomen_macro;
        primus        = s->origo.datum.pasta.sinister;
        secundus      = s->origo.datum.pasta.dexter;
        tertius       = s->origo.datum.pasta.invocatio;
        frange;
    casus SILVA_ORIGO_CHORDA:
        tag = "stringificatio";
        titulus_macro = s->origo.datum.stringificatio.nomen_macro;
        primus        = s->origo.datum.stringificatio.primus;
        frange;
    casus SILVA_ORIGO_API:
        tag = "api";
        titulus_macro = s->origo.datum.api.nomen_macro;
        frange;
    ordinarius:
        materia_arbor_scriptor_recusare(st, "genus originis ignotum");
        redde FALSUM;
    }

    elem = stml_elementum_creare(p, in, tag);
    si (elem == NIHIL)
    {
        materia_arbor_scriptor_recusare(st,
            "elementum originis creari non potuit");
        redde FALSUM;
    }
    si (titulus_macro != NIHIL && titulus_macro->mensura > ZEPHYRUM)
    {
        stml_attributum_addere_chorda(elem, p, in, "macro", *titulus_macro);
    }
    si (definitio != NIHIL)
    {
        /* DEF-SITE per REFERENTIAM, numquam inlinatum: aliter quaeque
         * plagula latina.h utens lexemata latina.h COPIARET. */
        materia_arbor_attributum_numeri(st, elem, "def-f",
            (i32)definitio->fons_index);
        materia_arbor_attributum_numeri(st, elem, "def-l", definitio->linea);
        materia_arbor_attributum_numeri(st, elem, "def-c", definitio->columna);
    }
    si (primus != NIHIL)
    {
        scriptum = materia_arbor_lexema_scribere(st,
            _token_convertere(primus));
        si (scriptum == NIHIL) { redde FALSUM; }
        si (!stml_liberum_addere(elem, scriptum))
        {
            materia_arbor_scriptor_recusare(st,
                "invocatio in originem addi non potuit");
            redde FALSUM;
        }
        /* EXTENTUM semel per invocationem: si transclusio, invocatio
         * iam scripta est et extentum cum ea. */
        si (   scriptum->genus != STML_NODUS_TRANSCLUSIO
            && s->origo.genus  == SILVA_ORIGO_EXPANSIO)
        {
            Xar* lamina = _extentum_laminam_silvae(primus);

            si (lamina != NIHIL && xar_numerus(lamina) > I
                && !_extentum_ornare(st, elem, lamina))
            {
                redde FALSUM;
            }
        }
    }
    si (secundus != NIHIL)
    {
        scriptum = materia_arbor_lexema_scribere(st,
            _token_convertere(secundus));
        si (scriptum == NIHIL) { redde FALSUM; }
        si (!stml_liberum_addere(elem, scriptum))
        {
            materia_arbor_scriptor_recusare(st,
                "dexter in originem addi non potuit");
            redde FALSUM;
        }
    }
    si (tertius != NIHIL)
    {
        scriptum = materia_arbor_lexema_scribere(st,
            _token_convertere(tertius));
        si (scriptum == NIHIL) { redde FALSUM; }
        si (!stml_liberum_addere(elem, scriptum))
        {
            materia_arbor_scriptor_recusare(st,
                "invocatio pastae addi non potuit");
            redde FALSUM;
        }
        si (scriptum->genus != STML_NODUS_TRANSCLUSIO)
        {
            Xar* lamina = _extentum_laminam_silvae(tertius);

            si (lamina != NIHIL && xar_numerus(lamina) > I
                && !_extentum_ornare(st, elem, lamina))
            {
                redde FALSUM;
            }
        }
    }
    si (!stml_liberum_addere(elementum, elem))
    {
        materia_arbor_scriptor_recusare(st,
            "origo in lexema addi non potuit");
        redde FALSUM;
    }
    redde VERUM;
}

hic_manens constans MateriaArborFrons FRONS_C89 = {
    NIHIL,
    _origo_numerare,
    _attributa_ornare,
    _liberos_ornare,
    NIHIL,   /* attributa_legere */
    NIHIL,   /* liberum_legere */
    NIHIL,   /* cursorem_movere */
    NIHIL    /* perficere */
};

/* Nodum ex valore radicis eruere (radix commissionis VALOR est) */
hic_manens SilvaNodus*
_nodum_radicis (SilvaValor v)
{
    si (v.genus == SILVA_VALOR_NODUS) { redde v.datum.nodus; }
    si (v.genus == SILVA_VALOR_LISTA)
    {
        i32 i;
        per (i = ZEPHYRUM; i < silva_valor_lista_numerus(v); i++)
        {
            SilvaValor* e = silva_valor_lista_obtinere(v, i);

            si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
            { redde e->datum.nodus; }
        }
    }
    redde NIHIL;
}

/* ---------- proba ---------- */

hic_manens i32 PROBATAE = 0;
hic_manens i32 FRACTAE  = 0;
hic_manens i32 STML_IDEM = 0;
hic_manens i32 STML_DISPAR = 0;
hic_manens b32 STML_AGERE = FALSUM;
hic_manens b32 STML_VERBOSE = FALSUM;

hic_manens vacuum
_probare (constans character* titulus, constans character* fons)
{
    Piscina* p = piscina_generare_dynamicum("shim", 1 << 20);
    SilvaParsura* parsura;
    SilvaScriptura sil;
    MateriaScriptura mat;
    MateriaValor  mradix;
    MateriaScripturaConsilium consilium;
    MateriaOrigoUncus uncus;
    MateriaRegistrumCoctum reg;

    parsura = silva_c89_parsare(p, "proba.c", fons, (i32)strlen(fons), NIHIL);
    si (parsura == NIHIL || !parsura->successus)
    {
        imprimere("  %-28s PARSURA FRACTA\n", titulus);
        FRACTAE++; piscina_destruere(p); redde;
    }

    SHIM.piscina  = p;
    SHIM.lexemata = xar_creare(p, magnitudo(MateriaToken*));
    SHIM.index    = tabula_dispersa_creare_chorda(p, 4096);
    SHIM.expansio = parsura->expansio;

    sil = silva_scribere_valorem(p, parsura->commissio->radix,
        &SILVA_C89_REGISTRUM, parsura->expansio);

    reg    = _registrum_convertere(p, &SILVA_C89_REGISTRUM);
    mradix = _valor_convertere(parsura->commissio->radix);

    uncus.datum             = NIHIL;
    uncus.sedes_quaerere    = _sedes_quaerere;
    uncus.radix_quaerere    = _radix_quaerere;
    uncus.extentum_quaerere = _extentum_quaerere;

    materia_scriptura_consilium_nudum(&consilium, &reg);
    consilium.origo            = &uncus;
    consilium.valorem_emittere = _valorem_emittere;

    mat = materia_scribere_valorem(p, mradix, &consilium);

    si (sil.successus != mat.successus)
    {
        imprimere("  %-28s SUCCESSUS DISPAR (silva %d, materia %d: %s)\n",
            titulus, (int)sil.successus, (int)mat.successus,
            mat.causa ? mat.causa : "-");
        FRACTAE++;
    }
    alioquin si (!sil.successus)
    {
        imprimere("  %-28s ambo recusant (%s) OK\n", titulus, sil.causa);
        PROBATAE++;
    }
    alioquin si (sil.textus.mensura != mat.textus.mensura
        || memcmp(sil.textus.datum, mat.textus.datum,
                  (size_t)sil.textus.mensura) != 0)
    {
        imprimere("  %-28s OCTETI DISPARES (silva %d, materia %d)\n",
            titulus, (int)sil.textus.mensura, (int)mat.textus.mensura);
        FRACTAE++;
    }
    alioquin
    {
        imprimere("  %-28s %4d octeti, %3d lexemata  IDEM\n", titulus,
            (int)sil.textus.mensura, (int)xar_numerus(SHIM.lexemata));
        PROBATAE++;
    }
    /* --- STML: scriptor materiae contra scriptorem silvae --- */
    si (STML_AGERE)
    {
        SilvaNodus* nodus_radicis = _nodum_radicis(parsura->commissio->radix);

        si (nodus_radicis != NIHIL)
        {
            SilvaArborScriptura sa;
            MateriaArborScriptura ma;
            MateriaArborConsilium ac;
            MateriaNodus* mnodus;
            MateriaLexiconRatum lexratum;
            MateriaLexIudicium iud;

            sa = silva_arbor_scribere_nodum(p, nodus_radicis,
                &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NIHIL);
            mnodus = _nodus_convertere(nodus_radicis);

            si (!materia_lexicon_ratum_facere(&lexratum, &LEXICON_C89, &iud))
            {
                imprimere("  %-28s LEXICON RECUSATUM (%s)\n", titulus,
                    materia_lexicon_vitium_nomen(
                        (MateriaLexVitium)lexratum.ratum));
                STML_DISPAR++;
            }
            alioquin
            {
                materia_arbor_consilium_nudum(&ac, &reg, &lexratum, "c89");
                ac.origo = &uncus;
                ac.frons = &FRONS_C89;
                ma = materia_arbor_scribere_nodum(p, mnodus, &ac);

                si (sa.successus != ma.successus)
                {
                    imprimere("  %-28s STML successus dispar (s%d m%d: %s)\n",
                        titulus, (int)sa.successus, (int)ma.successus,
                        ma.causa ? ma.causa : "-");
                    STML_DISPAR++;
                }
                alioquin si (!sa.successus) { STML_IDEM++; }
                alioquin si (sa.textus.mensura != ma.textus.mensura
                    || memcmp(sa.textus.datum, ma.textus.datum,
                              (size_t)sa.textus.mensura) != 0)
                {
                    imprimere("  %-28s STML DISPAR (silva %d, materia %d)\n",
                        titulus, (int)sa.textus.mensura,
                        (int)ma.textus.mensura);
                    si (STML_VERBOSE)
                    {
                        FILE* f1 = fopen("/tmp/shim_silva.stml", "wb");
                        FILE* f2 = fopen("/tmp/shim_materia.stml", "wb");

                        si (f1) { fwrite(sa.textus.datum, 1,
                            (size_t)sa.textus.mensura, f1); fclose(f1); }
                        si (f2) { fwrite(ma.textus.datum, 1,
                            (size_t)ma.textus.mensura, f2); fclose(f2); }
                        STML_VERBOSE = FALSUM;
                    }
                    STML_DISPAR++;
                }
                alioquin { STML_IDEM++; }
            }
        }
    }

    piscina_destruere(p);
}

hic_manens vacuum
_probare_plagulam (constans character* via)
{
    FILE* f = fopen(via, "rb");
    character* buf;
    longus n;

    si (f == NIHIL) { imprimere("  %-28s APERIRI NON POTUIT\n", via); FRACTAE++; redde; }
    fseek(f, 0, SEEK_END); n = ftell(f); fseek(f, 0, SEEK_SET);
    buf = (character*)malloc((size_t)n + 1);
    si (fread(buf, 1, (size_t)n, f) != (size_t)n) { fclose(f); free(buf); FRACTAE++; redde; }
    buf[n] = '\0';
    fclose(f);
    _probare(via, buf);
    free(buf);
}

s32 principale (integer argc, character** argv)
{
    {
        integer i;
        per (i = 1; i < argc; i++)
        {
            si (strcmp(argv[i], "-stml") == 0) { STML_AGERE = VERUM; }
            si (strcmp(argv[i], "-v") == 0) { STML_VERBOSE = VERUM; }
        }
    }
    imprimere("\n=== SHIM: materia_scribere contra silva_scribere ===\n\n");

    _probare("declaratio nuda",     "int x;\n");
    _probare("functio",             "int f(int a)\n{\n    return a + 1;\n}\n");
    _probare("commenta et trivia",  "/* c */\nint  x ;  /* d */\n");
    _probare("macro obiectum",      "#define N 4\nint a[N];\n");
    _probare("macro functio",       "#define M(a,b) ((a)+(b))\nint x = M(1,2);\n");
    _probare("macro nidificata",    "#define A 1\n#define B (A+A)\nint y = B;\n");
    _probare("directiva et codex",  "#define X\nint p;\n#undef X\nint q;\n");
    _probare("structura",           "struct S { int a; char* b; };\n");
    _probare("lamina intra lexema", "int ab\\\ncd;\n");
    _probare("conditionalis",       "#if 1\nint u;\n#else\nint v;\n#endif\n");

    si (argc > 1)
    {
        integer i;

        per (i = 1; i < argc; i++)
        {
            si (strcmp(argv[i], "-stml") == 0 || strcmp(argv[i], "-v") == 0) { perge; }
            _probare_plagulam(argv[i]);
        }
    }
    si (STML_AGERE)
    {
        imprimere("  STML: idem %d, dispar %d\n", (int)STML_IDEM,
            (int)STML_DISPAR);
    }

    imprimere("\n  probatae %d, fractae %d\n\n", (int)PROBATAE, (int)FRACTAE);
    redde (FRACTAE == 0) ? ZEPHYRUM : I;
}
