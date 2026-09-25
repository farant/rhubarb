/* silva_frons.c - FRONS C89 super materiam (phasis V, T6b)
 *
 * E shim (materia/instrumenta/shim_c89.c) promotus: conversio arboris
 * silvae in typos materiae et unci C89 scriptorum materiae. Codex
 * conversionis et uncorum IDEM est ac in shim (portatio verbatim);
 * mutationes solae: status globalis 'SHIM' -> contextus 'SilvaFrons'
 * (per 'datum' uncis traditus), tabula frontis omnes IX campos fert
 * (vexilla domus), '_radix_silvae' (numquam vocatum) sublatum.
 * Probatio: materia/shim_probare.sh -stml (octeti, STML, circuitus bis,
 * comparator) contra silvam ipsam; porta oraculi.
 */

#include "silva_frons.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"
#include "stml.h"
#include "silva_token.h"
#include "silva_tabulae_c89.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "silva_lexicon_c89.h"
#include <string.h>

/* Cauda lexematis materiae = DATUM FRONTIS C89.
 *
 * SYMMETRICA CONSULTO. Prius 'SilvaToken* silva' sola erat, quod
 * semitae CONVERSIONIS sufficiebat sed lectioni non: lexema ex
 * documento lectum lexema silvae unde veniret NON habet. Uncus
 * scripturae qui '->silva' legit ergo circuitum claudere non
 * posset.
 *
 * Nunc cauda id fert quod DOCUMENTUM fert, et utraque semita eam
 * implet - conversio ex silva, lectio ex STML. Uncus scripturae
 * caudam solam legit, silvam numquam. Haec est forma frontis
 * C89 (phasis V, in shim nata). */
nomen structura {
      SilvaToken* silva;        /* conversio; NIHIL in lectione */
             s32  origo_genus;  /* SilvaOrigoGenus; FONS = 0 */
    MateriaToken* primus;
    MateriaToken* secundus;
    MateriaToken* tertius;
          chorda* nomen_macro;
             b32  def_adest;
             s32  def_f;
             s32  def_l;
             s32  def_c;
             Xar* extentum;     /* Xar de MateriaToken*; NIHIL */
             Xar* scissurae;    /* Xar de SilvaScissura; NIHIL */
              i8  standard;
} SilvaFronsCauda;

#define CAUDA(t) ((SilvaFronsCauda*)materia_token_cauda(t))

hic_manens Xar*
_extentum_laminam_silvae (
             SilvaFrons* frons,
    constans SilvaToken* invocatio);
hic_manens constans MateriaTokenForma FORMA =
    { (i32)magnitudo(SilvaFronsCauda) };

/* Contextus per plagulam (opacus in capite). Unci eum per 'datum'
 * accipiunt; conversio eum explicite fert - nullus status globalis
 * (in shim 'SHIM' globalis erat). */
structura SilvaFrons {
                    Piscina* piscina;
                        Xar* lexemata;    /* MateriaToken* */
             TabulaDispersa* index;       /* silva -> materia */
     constans SilvaExpansio* expansio;
     MateriaRegistrumCoctum  tabularium;  /* C89, semel */
          MateriaOrigoUncus  uncus;       /* datum = frons */
        MateriaLexiconRatum  lexratum;
         constans character* causa;

    /* FRONS SUPER LEXEMATA SILVAE (T10b): unci caudam silvae ipsam
     * legunt, sine conversione. Tabula frontis per frontem (non
     * statica) quia unci contextum per 'datum' poscunt - extenta
     * LECTA in contextu vivunt (lectio subarboris expansionem non
     * habet; lector silvae vetus ideo 'extentum sine expansione'
     * recusabat, CI nodos). */
          MateriaArborFrons  frons_silvae;
          MateriaOrigoUncus  uncus_silvae;
                        Xar* extenta_lecta;  /* SilvaExtentumInvocationis */
};

/* Clavis ex punctatore: octeti eius in piscina servati (chorda
 * stabilem memoriam poscit). Quaestio linearis prior O(n^2) erat
 * et super plagulas veras EXCESSIT - defectus probae, non consilii. */
hic_manens chorda
_clavis (
    SilvaFrons* frons,
    SilvaToken* t)
{
        chorda   c;
    SilvaToken** cella = (SilvaToken**)piscina_allocare_ordinatum(
        frons->piscina, magnitudo(SilvaToken*), magnitudo(vacuum*));

    *cella     = t;
    c.datum    = (i8*)cella;
    c.mensura  = (i32)magnitudo(SilvaToken*);
    redde c;
}

/* ---------- conversio registri ---------- */
hic_manens MateriaRegistrumCoctum
_registrum_convertere (
                          Piscina* p,
    constans SilvaRegistrumCoctum* s)
{
    MateriaRegistrumCoctum r;
           MateriaTabGenus* g;
           MateriaTabLocus* l;
                       i32  i;

    g = (MateriaTabGenus*)piscina_allocare_ordinatum(p,
        (memoriae_index)s->numerus_generum * magnitudo(MateriaTabGenus),
        magnitudo(vacuum*));
    l = (MateriaTabLocus*)piscina_allocare_ordinatum(p,
        (memoriae_index)s->numerus_locorum * magnitudo(MateriaTabLocus),
        magnitudo(vacuum*));

    per (i = ZEPHYRUM; i < s->numerus_generum; i++)
    {
        g[i].titulus       = s->genera[i].titulus;
        g[i].loci_offset   = s->genera[i].loci_offset;
        g[i].loci_numerus  = s->genera[i].loci_numerus;
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
_quaerere (
    SilvaFrons* frons,
    SilvaToken* s)
{
        vacuum* valor;
        chorda  c;
    SilvaToken* cella[1];

    cella[0]   = s;
    c.datum    = (i8*)cella;
    c.mensura  = (i32)magnitudo(SilvaToken*);
    si (tabula_dispersa_invenire(frons->index, c, &valor))
    {
        redde (MateriaToken*)valor;
    }
    redde NIHIL;
}

hic_manens MateriaToken*
_token_convertere (
    SilvaFrons* frons,
    SilvaToken* s)
{
    MateriaToken* m;
             i32  i;

    si (s == NIHIL)
    { redde NIHIL;
    }
    m = _quaerere(frons, s);
    si (m != NIHIL)
    { redde m;
    }

    m = materia_token_creare(frons->piscina, &FORMA, (s32)s->genus,
        s->valor, s->byte_offset, s->linea, s->columna, s->fons_index);
    si (m == NIHIL)
    { redde NIHIL;
    }
    {
        SilvaFronsCauda* cd = CAUDA(m);

        cd->silva        = s;
        cd->origo_genus  = (s32)silva_token_origo(s)->genus;
        cd->standard     = silva_token_standard(s);
        cd->scissurae    = silva_token_scissurae(s);
        commutatio (silva_token_origo(s)->genus)
        {
        casus SILVA_ORIGO_EXPANSIO:
            cd->nomen_macro =
                silva_token_origo(s)->datum.expansio.nomen_macro;
            si (silva_token_origo(s)->datum.expansio.corpus != NIHIL)
            {
                SilvaToken* d =
                    silva_token_origo(s)->datum.expansio.corpus;

                cd->def_adest  = VERUM;
                cd->def_f      = d->fons_index;
                cd->def_l      = (s32)d->linea;
                cd->def_c      = (s32)d->columna;
            }
            frange;
        casus SILVA_ORIGO_PASTA:
            cd->nomen_macro =
                silva_token_origo(s)->datum.pasta.nomen_macro; frange;
        casus SILVA_ORIGO_CHORDA:
            cd->nomen_macro =
                silva_token_origo(s)->datum.stringificatio.nomen_macro;
            frange;
        casus SILVA_ORIGO_API:
            cd->nomen_macro =
                silva_token_origo(s)->datum.api.nomen_macro; frange;
        ordinarius: frange;
        }
    }
    materia_token_initium_lineae_ponere(m,
        silva_token_initium_lineae(s));
    *(MateriaToken**)xar_addere(frons->lexemata) = m;
    tabula_dispersa_inserere(frons->index, _clavis(frons, s), m);

    /* trivia - exacta */
    {
        i32 n = silva_token_ante_numerus(s);

        si (n > ZEPHYRUM)
        {
            MateriaToken** ser =
                (MateriaToken**)piscina_allocare_ordinatum(
                frons->piscina,
                (memoriae_index)n * magnitudo(MateriaToken*),
                magnitudo(vacuum*));
            per (i = ZEPHYRUM; i < n; i++)
            {
                ser[i] = _token_convertere(frons,
                    silva_token_ante(s, i));
            }
            materia_token_trivia_ante_ponere(m, frons->piscina, ser, n);
        }
        n = silva_token_post_numerus(s);
        si (n > ZEPHYRUM)
        {
            MateriaToken** ser =
                (MateriaToken**)piscina_allocare_ordinatum(
                frons->piscina,
                (memoriae_index)n * magnitudo(MateriaToken*),
                magnitudo(vacuum*));
            per (i = ZEPHYRUM; i < n; i++)
            {
                ser[i] = _token_convertere(frons,
                    silva_token_post(s, i));
            }
            materia_token_trivia_post_ponere(m, frons->piscina, ser, n);
        }
    }

    /* Catena originis POST insertionem in indicem: lexema catenam
     * suam per se ipsum attingere potest (recursio infinita aliter). */
    {
        SilvaFronsCauda* cd = CAUDA(m);

        commutatio (silva_token_origo(s)->genus)
        {
        casus SILVA_ORIGO_EXPANSIO:
            cd->primus =
                _token_convertere(frons,
                silva_token_origo(s)->datum.expansio.invocatio);
            frange;
        casus SILVA_ORIGO_PASTA:
            cd->primus   =
                _token_convertere(frons,
                silva_token_origo(s)->datum.pasta.sinister);
            cd->secundus =
                _token_convertere(frons,
                silva_token_origo(s)->datum.pasta.dexter);
            cd->tertius  =
                _token_convertere(frons,
                silva_token_origo(s)->datum.pasta.invocatio);
            frange;
        casus SILVA_ORIGO_CHORDA:
            cd->primus =
                _token_convertere(frons,
                silva_token_origo(s)->datum.stringificatio.primus);
            frange;
        ordinarius: frange;
        }
        si (cd->primus != NIHIL || cd->tertius != NIHIL)
        {
            SilvaToken* anc = (silva_token_origo(s)->genus
                == SILVA_ORIGO_PASTA)
                ? silva_token_origo(s)->datum.pasta.invocatio
                : ((silva_token_origo(s)->genus == SILVA_ORIGO_EXPANSIO)
                    ? silva_token_origo(s)->datum.expansio.invocatio : NIHIL);
            Xar* lam = _extentum_laminam_silvae(frons, anc);

            si (lam != NIHIL && xar_numerus(lam) > I)
            {
                i32 k;

                cd->extentum = xar_creare(frons->piscina,
                    magnitudo(MateriaToken*));
                per (k = ZEPHYRUM; k < xar_numerus(lam); k++)
                {
                    *(MateriaToken**)xar_addere(cd->extentum) =
                        _token_convertere(frons,
                            *(SilvaToken**)xar_obtinere(lam, k));
                }
            }
        }
    }
    redde m;
}

/* ---------- conversio arboris ---------- */
hic_manens MateriaNodus*
_nodus_convertere (
    SilvaFrons* frons,
    SilvaNodus* s);

hic_manens MateriaValor
_valor_convertere (
    SilvaFrons* frons,
    SilvaValor  v)
{
    commutatio (v.genus)
    {
    casus SILVA_VALOR_NIHIL:  redde materia_valor_nihil();
    casus SILVA_VALOR_INDEX:  redde materia_valor_index(v.datum.index);
    casus SILVA_VALOR_TOKEN:
        redde materia_valor_token(_token_convertere(frons,
            v.datum.token));
    casus SILVA_VALOR_NODUS:
        redde materia_valor_nodus(_nodus_convertere(frons,
            v.datum.nodus));
    casus SILVA_VALOR_LISTA:
    {
        MateriaValor lista = materia_valor_lista_nova(frons->piscina);
                 i32 i;
                 i32 n = silva_valor_lista_numerus(v);

        per (i = ZEPHYRUM; i < n; i++)
        {
            SilvaValor* e = silva_valor_lista_obtinere(v, i);

            si (e != NIHIL)
            {
                lista = materia_valor_lista_appendere(frons->piscina,
                    lista, _valor_convertere(frons, *e));
            }
        }
        redde lista;
    }
    ordinarius: redde materia_valor_nihil();
    }
}

hic_manens MateriaNodus*
_nodus_convertere (
    SilvaFrons* frons,
    SilvaNodus* s)
{
    MateriaNodus* m;
             i32  i;

    si (s == NIHIL)
    { redde NIHIL;
    }
    m = materia_nodus_creare(frons->piscina, s->genus,
        s->numerus_locorum);
    si (m == NIHIL)
    { redde NIHIL;
    }
    per (i = ZEPHYRUM; i < s->numerus_locorum; i++)
    {
        m->loci[i] = _valor_convertere(frons, s->loci[i]);

        /* PATER: politica RECONSTRUCTIONIS materiae, non speculum
         * silvae. Documentum patrem NON fert - uterque latus eum
         * reficit, et politicae DIFFERUNT: commissio silvae
         * bracchia ambigui non canonica SINE patre relinquit
         * (artificium ambulationis spinae), lector materiae
         * (_patres_figere) arborem TOTAM parentat. Speculari
         * silvam hic LXXVIII plagulas capitum (nodi AMBIGUI
         * retenti) falso divergentes dedit. Divergentia politicae
         * ad phasim V NOMINATA (phase-log): silva in materiam
         * migrans semel decernat. Oraculum arboris ambas vias
         * primo tactu invenit: CCXXXIV, deinde LXXVIII. */
        si (   m->loci[i].genus       == MATERIA_VALOR_NODUS
            && m->loci[i].datum.nodus != NIHIL)
        {
            m->loci[i].datum.nodus->pater = m;
        }
        alioquin si (m->loci[i].genus == MATERIA_VALOR_LISTA)
        {
            i32 j;
            i32 n = materia_valor_lista_numerus(m->loci[i]);

            per (j = ZEPHYRUM; j < n; j++)
            {
                MateriaValor* f =
                    materia_valor_lista_obtinere(m->loci[i], j);

                si (   f              != NIHIL
                    && f->genus       == MATERIA_VALOR_NODUS
                    && f->datum.nodus != NIHIL)
                {
                    f->datum.nodus->pater = m;
                }
            }
        }
    }
    redde m;
}

/* ---------- unci C89 (portati ex silva_scribere.c) ---------- */
/* Radix per CAUDAM, non per silvam - ergo eadem via lexemata
 * conversa et lexemata LECTA tractat. */
hic_manens MateriaToken*
_radix_quaerere (
                             vacuum*  datum,
                       MateriaToken*  token,
                 constans character** causa)
{
    MateriaToken* t = token;

    (vacuum)datum;
    per (;;)
    {
           SilvaFronsCauda* cd = CAUDA(t);
        MateriaToken* proximum;

        si (cd->origo_genus == (s32)SILVA_ORIGO_FONS)
        { redde t;
        }

        commutatio (cd->origo_genus)
        {
        casus SILVA_ORIGO_EXPANSIO: proximum = cd->primus;  frange;
        casus SILVA_ORIGO_CHORDA:   proximum = cd->primus;  frange;
        casus SILVA_ORIGO_PASTA:    proximum = cd->tertius; frange;
        ordinarius:                 proximum = NIHIL;       frange;
        }
        si (proximum == NIHIL)
        {
            *causa = "origo pasta/chorda/api - stratum 0 non "
                     "recuperabile (deferral nominatum)";
            redde NIHIL;
        }
        t = proximum;
    }
}

/* SEDES: materia hinc discit an lexema DERIVATUM sit. Sine hoc unco
 * omne lexema 'origo sua' videtur et scriptor sedem PORTATAM omittit
 * (silva eam lexemati non-FONS scribit, quia sedes eius DEF-SITE est,
 * in plagula alia, ergo ex hoc fluxu derivari NEQUIT). */
hic_manens vacuum
_sedes_quaerere (
                       vacuum* datum,
        constans MateriaToken* token,
                 MateriaSedes* sedes)
{
    SilvaFronsCauda* cd = CAUDA(token);

    (vacuum)datum;
    sedes->byte_offset  = token->byte_offset;
    sedes->linea        = token->linea;
    sedes->columna      = token->columna;
    sedes->fons_index   = token->fons_index;
    sedes->est_fons = (b32)(cd->origo_genus
        == (s32)SILVA_ORIGO_FONS);
}

/* Extentum per CAUDAM. Cauda RADICIS eum non fert - lexema DERIVATUM
 * eum fert (invocatio eius) - ergo per lexemata omnia quaerendum est
 * quorum radix haec sit. Scansio linearis; numeri parvi. */
hic_manens Xar*
_extentum_quaerere (
                   vacuum* datum,
    constans MateriaToken* radix)
{
    SilvaFrons* frons = (SilvaFrons*)datum;
           i32  i;

    /* lectio: extentum in cauda invocationis ipsius (vide
     * _extentum_legere); conversio: per lexemata derivata */
    si (CAUDA(radix)->extentum != NIHIL)
    {
        redde CAUDA(radix)->extentum;
    }
    per (i = ZEPHYRUM; i < xar_numerus(frons->lexemata); i++)
    {
        MateriaToken* m = *(MateriaToken**)xar_obtinere(frons->lexemata,
            i);
           SilvaFronsCauda* cd = CAUDA(m);

        si (cd->extentum == NIHIL)
        { perge;
        }
        si (   cd->primus  == radix
            || cd->tertius == radix)
        {
            redde cd->extentum;
        }
    }
    redde NIHIL;
}

/* scissurae: valorem lexematis cum laminis reinsertis */
hic_manens b32
_valorem_emittere (
                                  vacuum* datum,
                       ChordaAedificator* aed,
                   constans MateriaToken* token)
{
    SilvaFronsCauda* s;

    (vacuum)datum;
    s = CAUDA(token);
    si (s->scissurae == NIHIL)
    {
        chorda_aedificator_appendere_chorda(aed, token->valor);
        redde VERUM;
    }
    {
        i32 i, prius = ZEPHYRUM;

        per (i = ZEPHYRUM; i < xar_numerus(s->scissurae); i++)
        {
            SilvaScissura* sc =
                (SilvaScissura*)xar_obtinere(s->scissurae, i);

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
_attributa_ornare (
                      vacuum* datum,
        MateriaArborScriptor* st,
                   StmlNodus* elementum,
       constans MateriaToken* lexema)
{
    SilvaFronsCauda* cd = CAUDA(lexema);

    (vacuum)datum;
    si (cd->standard != (i8)SILVA_STANDARD_C89)
    {
        redde materia_arbor_attributum_numeri(st, elementum, "standard",
            (i32)cd->standard);
    }
    redde VERUM;
}

hic_manens vacuum
_origo_numerare (
    vacuum* datum,
    constans MateriaToken* lexema,
                 vacuum (*numerare)(vacuum*, constans MateriaToken*),
                 vacuum* ctx)
{
    SilvaFronsCauda* cd = CAUDA(lexema);

    (vacuum)datum;
    si (cd->primus != NIHIL)
    { numerare(ctx, cd->primus);
    }
    si (cd->secundus != NIHIL)
    { numerare(ctx, cd->secundus);
    }
    si (cd->tertius != NIHIL)
    { numerare(ctx, cd->tertius);
    }
}


/* Gradus II: scissurae + catena originis nestata.
 * Portatum ex silva_arbor.c _origo_scribere/_extentum_scribere;
 * uncus 'liberos_ornare' scriptorem lexematis RE-INTRAT per
 * materia_arbor_lexema_scribere, ergo machina fragmentorum una
 * manet et identitas per transclusiones servatur. */

hic_manens Xar*
_extentum_laminam_silvae (
             SilvaFrons* frons,
    constans SilvaToken* invocatio)
{
    i32 k;

    si (   frons->expansio == NIHIL || frons->expansio->extenta == NIHIL
        || invocatio       == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(frons->expansio->extenta); k++)
    {
        SilvaExtentumInvocationis* e = (SilvaExtentumInvocationis*)
            xar_obtinere(frons->expansio->extenta, k);

        si (e != NIHIL && e->invocatio == invocatio)
        { redde e->lamina;
        }
    }
    redde NIHIL;
}

hic_manens b32
_extentum_ornare (
    MateriaArborScriptor* st,
               StmlNodus* parens,
                     Xar* lamina)
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
        MateriaToken* t = *(MateriaToken**)xar_obtinere(lamina, k);
           StmlNodus* scriptum;

        si (t == NIHIL)
        { perge;
        }
        scriptum = materia_arbor_lexema_scribere(st, t);
        si (scriptum == NIHIL)
        { redde FALSUM;
        }
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
_liberos_ornare (
                    vacuum* datum,
      MateriaArborScriptor* st,
                 StmlNodus* elementum,
     constans MateriaToken* lexema)
{
              SilvaFronsCauda* cd   = CAUDA(lexema);
                      Piscina* p  =
                          materia_arbor_scriptor_piscina(st);
    InternamentumChorda* in     = materia_arbor_scriptor_intern(st);
     constans character* tag;
              StmlNodus* elem;
              StmlNodus* scriptum;
                    i32  i;

    (vacuum)datum;

    /* --- scissurae --- */
    si (cd->scissurae != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(cd->scissurae); i++)
        {
            SilvaScissura* sc =
                (SilvaScissura*)xar_obtinere(cd->scissurae, i);
                StmlNodus* es;

            si (sc == NIHIL || sc->offset < ZEPHYRUM)
            {
                materia_arbor_scriptor_recusare(st,
                    "scissura corrupta");
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
    si (cd->origo_genus == (s32)SILVA_ORIGO_FONS)
    { redde VERUM;
    }

    commutatio (cd->origo_genus)
    {
    casus SILVA_ORIGO_EXPANSIO:      tag = "expansio";       frange;
    casus SILVA_ORIGO_PASTA:         tag = "pasta";          frange;
    casus SILVA_ORIGO_CHORDA:        tag = "stringificatio"; frange;
    casus SILVA_ORIGO_API:           tag = "api";            frange;
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
    si (cd->nomen_macro != NIHIL && cd->nomen_macro->mensura > ZEPHYRUM)
    {
        stml_attributum_addere_chorda(elem, p, in, "macro",
            *cd->nomen_macro);
    }
    si (cd->def_adest)
    {
        /* DEF-SITE per REFERENTIAM, numquam inlinatum: aliter quaeque
         * plagula latina.h utens lexemata latina.h COPIARET. */
        materia_arbor_attributum_numeri(st, elem, "def-f",
            (i32)cd->def_f);
        materia_arbor_attributum_numeri(st, elem, "def-l",
            (i32)cd->def_l);
        materia_arbor_attributum_numeri(st, elem, "def-c",
            (i32)cd->def_c);
    }
    si (cd->primus != NIHIL)
    {
        scriptum = materia_arbor_lexema_scribere(st, cd->primus);
        si (scriptum == NIHIL)
        { redde FALSUM;
        }
        si (!stml_liberum_addere(elem, scriptum))
        {
            materia_arbor_scriptor_recusare(st,
                "invocatio in originem addi non potuit");
            redde FALSUM;
        }
        /* EXTENTUM semel per invocationem: si transclusio, invocatio
         * iam scripta est et extentum cum ea. */
        si (   scriptum->genus != STML_NODUS_TRANSCLUSIO
            && cd->origo_genus == (s32)SILVA_ORIGO_EXPANSIO
            && cd->extentum    != NIHIL && xar_numerus(cd->extentum) > I
            && !_extentum_ornare(st, elem, cd->extentum))
        {
            redde FALSUM;
        }
    }
    si (cd->secundus != NIHIL)
    {
        scriptum = materia_arbor_lexema_scribere(st, cd->secundus);
        si (scriptum == NIHIL)
        { redde FALSUM;
        }
        si (!stml_liberum_addere(elem, scriptum))
        {
            materia_arbor_scriptor_recusare(st,
                "dexter in originem addi non potuit");
            redde FALSUM;
        }
    }
    si (cd->tertius != NIHIL)
    {
        scriptum = materia_arbor_lexema_scribere(st, cd->tertius);
        si (scriptum == NIHIL)
        { redde FALSUM;
        }
        si (!stml_liberum_addere(elem, scriptum))
        {
            materia_arbor_scriptor_recusare(st,
                "invocatio pastae addi non potuit");
            redde FALSUM;
        }
        si (   scriptum->genus != STML_NODUS_TRANSCLUSIO
            && cd->extentum    != NIHIL && xar_numerus(cd->extentum) > I
            && !_extentum_ornare(st, elem, cd->extentum))
        {
            redde FALSUM;
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

/* ---------- frons C89: LECTIO ---------- */
hic_manens b32
_attributa_legere (
                               vacuum* datum,
                   MateriaArborLector* lector,
                   constans StmlNodus* elementum,
                         MateriaToken* lexema)
{
    SilvaFronsCauda* cd = CAUDA(lexema);
             chorda* a;
                i32  n;

    (vacuum)datum;
    cd->standard = (i8)SILVA_STANDARD_C89;
    a = stml_attributum_capere((StmlNodus*)(size_t)elementum,
        "standard");
    si (a != NIHIL)
    {
        si (!materia_arbor_numerus_ex_chorda(a, &n))
        {
            redde materia_arbor_lector_recusare(lector,
                "standard non numerus", elementum->linea);
        }
        cd->standard = (i8)n;
    }
    redde VERUM;
}

hic_manens b32
_extentum_legere (
                       MateriaArborLector* lector,
                                StmlNodus* elementum,
                             MateriaToken* invocatio,
                          SilvaFronsCauda* cd)
{
    Xar* lamina;
    i32  cursor;
    i32  numerus;

    si (invocatio == NIHIL)
    {
        redde materia_arbor_lector_recusare(lector,
            "extentum sine invocatione", elementum->linea);
    }
    lamina  = xar_creare(materia_arbor_lector_piscina(lector),
        magnitudo(MateriaToken*));
    numerus = stml_numerus_liberorum(elementum);
    per (cursor = ZEPHYRUM; cursor < numerus; cursor++)
    {
           StmlNodus* liberum = stml_liberum_ad_indicem(elementum,
               cursor);
        MateriaToken* lectum;

        si (liberum == NIHIL)
        { perge;
        }
        si (   liberum->genus != STML_NODUS_ELEMENTUM
            && liberum->genus != STML_NODUS_TRANSCLUSIO)
        { perge;
        }
        lectum = materia_arbor_lexema_legere(lector, liberum, NIHIL);
        si (lectum == NIHIL)
        { redde FALSUM;
        }
        *(MateriaToken**)xar_addere(lamina) = lectum;
    }

    /* LEXEMA PRIMUM LAMINAE *EST* INVOCATIO - IDENTITAS, non
     * aequalitas. Scriptor nomen BIS scribit (semel in 'expansio',
     * semel ut caput laminae), ergo lectio DUO OBIECTA valore pari
     * pareret; emissor autem extentum per IDENTITATEM MONSTRATORIS
     * quaerit, et sic invocatio sedem numquam rectam acciperet et
     * emissio eam SILENTER OMITTERET. Silva id mensuravit: IV
     * plagulae, octeti invocationis absentes dum extenta ipsa recte
     * numerarentur. NUMERUS PAR IDENTITATEM NON PROBAT. */
    si (xar_numerus(lamina) > ZEPHYRUM)
    {
        *(MateriaToken**)xar_obtinere(lamina, ZEPHYRUM) = invocatio;
    }
    cd->extentum = lamina;
    /* ET IN CAUDA INVOCATIONIS (T10b, 2026-09-25): uncus extenti
     * per RADICEM quaerit, et lexemata LECTA in frons->lexemata non
     * stant (uncis lectionis frons non traditur) - ergo extentum
     * lectum numquam inveniebatur, cursor '(3)\n' non ambulabat, et
     * lexema post invocationem functio-similem initium_lineae (et
     * sedes) amittebat: XX nodi lib/arbor2_glr_tabula.c, latentes
     * dum shim nodum PRIMUM solum probabat. Radix FONS est, ergo
     * scriptor hoc campum in ea numquam scribit. */
    si (CAUDA(invocatio)->extentum == NIHIL)
    {
        CAUDA(invocatio)->extentum = lamina;
    }
    redde VERUM;
}

hic_manens s32
_liberum_legere (
                             vacuum* datum,
                 MateriaArborLector* lector,
                 constans StmlNodus* liberum,
                       MateriaToken* lexema)
{
    SilvaFronsCauda* cd = CAUDA(lexema);
          StmlNodus* el = (StmlNodus*)(size_t)liberum;
             chorda* a;
                s32  genus;
                i32  cursor;
                i32  numerus;

    (vacuum)datum;
    si (liberum->titulus == NIHIL)
    { redde (s32)MATERIA_LECTIO_IGNOTUM;
    }

    /* --- scissura --- */
    si (chorda_aequalis_literis(*liberum->titulus, "scissura"))
    {
        SilvaScissura sc;
                  i32 offset;

        a = stml_attributum_capere(el, "offset");
        si (!materia_arbor_numerus_ex_chorda(a, &offset))
        {
            materia_arbor_lector_recusare(lector,
                "scissura sine offset",
                liberum->linea);
            redde (s32)MATERIA_LECTIO_FRACTUM;
        }
        sc.offset  = (s32)offset;
        sc.crlf    = stml_attributum_habet(el, "crlf");
        si (cd->scissurae == NIHIL)
        {
            cd->scissurae = xar_creare(
                materia_arbor_lector_piscina(lector),
                magnitudo(SilvaScissura));
        }
        *(SilvaScissura*)xar_addere(cd->scissurae) = sc;
        redde (s32)MATERIA_LECTIO_ACCEPTUM;
    }

    /* --- origo --- */
    si (chorda_aequalis_literis(*liberum->titulus, "expansio"))
    { genus = (s32)SILVA_ORIGO_EXPANSIO;
    }
    alioquin si (chorda_aequalis_literis(*liberum->titulus, "pasta"))
    { genus = (s32)SILVA_ORIGO_PASTA;
    }
    alioquin si (chorda_aequalis_literis(*liberum->titulus,
                     "stringificatio"))
    { genus = (s32)SILVA_ORIGO_CHORDA;
    }
    alioquin si (chorda_aequalis_literis(*liberum->titulus, "api"))
    { genus = (s32)SILVA_ORIGO_API;
    }
    alioquin
    { redde (s32)MATERIA_LECTIO_IGNOTUM;
    }

    cd->origo_genus = genus;
    cd->nomen_macro = stml_attributum_capere(el, "macro");

    a = stml_attributum_capere(el, "def-l");
    si (a != NIHIL)
    {
        i32 n = ZEPHYRUM;

        cd->def_adest = VERUM;
        (vacuum)materia_arbor_numerus_ex_chorda(a, &n);
        cd->def_l  = (s32)n;
        n          = ZEPHYRUM;
        a          = stml_attributum_capere(el, "def-f");
        si (a != NIHIL)
        { (vacuum)materia_arbor_numerus_ex_chorda(a, &n);
        }
        cd->def_f  = (s32)n;
        n          = ZEPHYRUM;
        a          = stml_attributum_capere(el, "def-c");
        si (a != NIHIL)
        { (vacuum)materia_arbor_numerus_ex_chorda(a, &n);
        }
        cd->def_c = (s32)n;
    }

    numerus = stml_numerus_liberorum(el);
    per (cursor = ZEPHYRUM; cursor < numerus; cursor++)
    {
           StmlNodus* n_lib = stml_liberum_ad_indicem(el, cursor);
        MateriaToken* lectum;

        si (n_lib == NIHIL)
        { perge;
        }
        si (   n_lib->genus != STML_NODUS_ELEMENTUM
            && n_lib->genus != STML_NODUS_TRANSCLUSIO)
        { perge;
        }

        /* EXTENTUM post invocationem stat, ergo 'primus'/'tertius'
         * iam noti sunt. */
        si (   n_lib->genus   == STML_NODUS_ELEMENTUM
            && n_lib->titulus != NIHIL
            && chorda_aequalis_literis(*n_lib->titulus, "extentum"))
        {
            si (!_extentum_legere(lector, n_lib,
                     (genus == (s32)SILVA_ORIGO_PASTA)
                         ? cd->tertius : cd->primus, cd))
            {
                redde (s32)MATERIA_LECTIO_FRACTUM;
            }
            perge;
        }
        lectum = materia_arbor_lexema_legere(lector, n_lib, NIHIL);
        si (lectum == NIHIL)
        { redde (s32)MATERIA_LECTIO_FRACTUM;
        }
        si (cd->primus == NIHIL)
        { cd->primus   = lectum;
        }
        alioquin si (cd->secundus == NIHIL)
        { cd->secundus = lectum;
        }
        alioquin si (cd->tertius == NIHIL)
        { cd->tertius  = lectum;
        }
    }
    redde (s32)MATERIA_LECTIO_ACCEPTUM;
}

/* Cursor per valorem CUM laminis reinsertis: sedes eas numerare
 * debent, aliter omnia post lexema lamina-ferens labuntur. */
hic_manens b32
_cursorem_movere (
                                 vacuum* datum,
                     MateriaArborCursor* c,
                  constans MateriaToken* lexema)
{
    SilvaFronsCauda* cd = CAUDA(lexema);
                i32  i;
                i32  s_idx;
                i32  n_sc;

    (vacuum)datum;
    si (cd->scissurae == NIHIL)
    { redde FALSUM;
    }
    n_sc   = xar_numerus(cd->scissurae);
    s_idx  = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= lexema->valor.mensura; i++)
    {
        dum (s_idx < n_sc)
        {
            SilvaScissura* sc = (SilvaScissura*)xar_obtinere(
                cd->scissurae, s_idx);

            si (sc == NIHIL || sc->offset != (s32)i)
            { frange;
            }
            c->offset += sc->crlf ? III : II;
            c->linea++;
            c->columna = I;
            s_idx++;
        }
        si (i == lexema->valor.mensura)
        { frange;
        }
        si ((character)lexema->valor.datum[i] == '\n')
        { c->linea++; c->columna = I;
        }
        alioquin
        { c->columna++;
        }
        c->offset++;
    }
    redde VERUM;
}

hic_manens constans MateriaArborFrons FRONS_C89 = {
    NIHIL,
    _origo_numerare,
    _attributa_ornare,
    _liberos_ornare,
    _attributa_legere,
    _liberum_legere,
    _cursorem_movere,
    NIHIL,   /* perficere: patres materia ipsa figit */
    NIHIL    /* nodum_ornare: C89 nodis attributa non addit */
};

/* Nodum ex valore radicis eruere (radix commissionis VALOR est) */
SilvaNodus*
silva_frons_nodus_radicis (
    SilvaValor v)
{
    si (v.genus == SILVA_VALOR_NODUS)
    { redde v.datum.nodus;
    }
    si (v.genus == SILVA_VALOR_LISTA)
    {
        i32 i;
        per (i = ZEPHYRUM; i < silva_valor_lista_numerus(v); i++)
        {
            SilvaValor* e = silva_valor_lista_obtinere(v, i);

            si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
            { redde e->datum.nodus;
            }
        }
    }
    redde NIHIL;
}


/* ==================================================
 * FRONS SUPER LEXEMATA SILVAE (silva-migratio T10b)
 *
 * Idem officium ac unci supra (conversio) sed super caudam SILVAE:
 * SilvaToken EST MateriaToken (T7b), SilvaNodus EST MateriaNodus
 * (T8) - nihil vertendum. Unci supra lexemata CONVERSA (cauda
 * SilvaFronsCauda) servant usque ad T10c, cum conversio recedit.
 * Portatio fidelis: semantica eadem, campi ex SilvaOrigo per
 * accessores silvae.
 * ================================================== */

/* Extentum invocationis per IDENTITATEM (sicut silva_arbor.c
 * _extentum_laminam_quaerere): parsurae extenta, deinde lecta. */
hic_manens Xar*
_silvae_extentum (
             SilvaFrons* frons,
    constans SilvaToken* invocatio)
{
    Xar* fontes[II];
    i32  f;
    i32  k;

    si (frons == NIHIL || invocatio == NIHIL)
    { redde NIHIL;
    }
    fontes[0] = (frons->expansio != NIHIL) ? frons->expansio->extenta
                                           : NIHIL;
    fontes[I] = frons->extenta_lecta;
    per (f = ZEPHYRUM; f < (i32)II; f++)
    {
        si (fontes[f] == NIHIL)
        { perge;
        }
        per (k = ZEPHYRUM; k < xar_numerus(fontes[f]); k++)
        {
            constans SilvaExtentumInvocationis* e =
                (constans SilvaExtentumInvocationis*)xar_obtinere(
                    fontes[f], k);

            si (e != NIHIL && e->invocatio == invocatio)
            { redde e->lamina;
            }
        }
    }
    redde NIHIL;
}

hic_manens MateriaToken*
_silvae_radix_quaerere (
                vacuum*  datum,
          MateriaToken*  token,
    constans character** causa)
{
    SilvaToken* t = token;
           i32  custodia;

    (vacuum)datum;
    per (custodia = ZEPHYRUM; custodia < (i32)CCLVI; custodia++)
    {
        SilvaOrigo* o = silva_token_origo(t);
        SilvaToken* proximum;

        commutatio (o->genus)
        {
        casus SILVA_ORIGO_FONS:
            redde t;
        casus SILVA_ORIGO_EXPANSIO:
            proximum = o->datum.expansio.invocatio; frange;
        casus SILVA_ORIGO_CHORDA:
            proximum = o->datum.stringificatio.primus; frange;
        casus SILVA_ORIGO_PASTA:
            proximum = o->datum.pasta.invocatio; frange;
        ordinarius:
            proximum = NIHIL; frange;
        }
        si (proximum == NIHIL)
        {
            *causa = "origo pasta/chorda/api - stratum 0 non "
                     "recuperabile (deferral nominatum)";
            redde NIHIL;
        }
        t = proximum;
    }
    *causa = "catena originis nimis longa";
    redde NIHIL;
}

hic_manens vacuum
_silvae_sedes_quaerere (
                   vacuum* datum,
    constans MateriaToken* token,
             MateriaSedes* sedes)
{
    (vacuum)datum;
    sedes->byte_offset  = token->byte_offset;
    sedes->linea        = token->linea;
    sedes->columna      = token->columna;
    sedes->fons_index   = token->fons_index;
    sedes->est_fons     = (b32)(silva_token_origo(token)->genus
        == SILVA_ORIGO_FONS);
}

hic_manens Xar*
_silvae_extentum_quaerere (
                   vacuum* datum,
    constans MateriaToken* radix)
{
    redde _silvae_extentum((SilvaFrons*)datum, radix);
}

/* ---------- scriptura ---------- */

hic_manens vacuum
_silvae_origo_numerare (
                   vacuum* datum,
    constans MateriaToken* lexema,
                   vacuum (*numerare)(vacuum*, constans MateriaToken*),
                   vacuum* ctx)
{
    SilvaOrigo* o = silva_token_origo(lexema);

    (vacuum)datum;
    commutatio (o->genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        si (o->datum.expansio.invocatio != NIHIL)
        { numerare(ctx, o->datum.expansio.invocatio);
        }
        frange;
    casus SILVA_ORIGO_PASTA:
        si (o->datum.pasta.sinister != NIHIL)
        { numerare(ctx, o->datum.pasta.sinister);
        }
        si (o->datum.pasta.dexter != NIHIL)
        { numerare(ctx, o->datum.pasta.dexter);
        }
        si (o->datum.pasta.invocatio != NIHIL)
        { numerare(ctx, o->datum.pasta.invocatio);
        }
        frange;
    casus SILVA_ORIGO_CHORDA:
        si (o->datum.stringificatio.primus != NIHIL)
        { numerare(ctx, o->datum.stringificatio.primus);
        }
        frange;
    ordinarius:
        frange;
    }
}

hic_manens b32
_silvae_attributa_ornare (
                      vacuum* datum,
        MateriaArborScriptor* st,
                   StmlNodus* elementum,
       constans MateriaToken* lexema)
{
    (vacuum)datum;
    si (silva_token_standard(lexema) != (i8)SILVA_STANDARD_C89)
    {
        redde materia_arbor_attributum_numeri(st, elementum, "standard",
            (i32)silva_token_standard(lexema));
    }
    redde VERUM;
}

/* Lexema catenae scribere et in elementum originis addere */
hic_manens b32
_silvae_catenam_addere (
    MateriaArborScriptor*  st,
               StmlNodus*  elem,
   constans MateriaToken*  lexema,
      constans character*  causa,
               StmlNodus** scriptum)
{
    *scriptum = materia_arbor_lexema_scribere(st, lexema);
    si (*scriptum == NIHIL)
    { redde FALSUM;
    }
    si (!stml_liberum_addere(elem, *scriptum))
    {
        materia_arbor_scriptor_recusare(st, causa);
        redde FALSUM;
    }
    redde VERUM;
}

hic_manens b32
_silvae_liberos_ornare (
                    vacuum* datum,
      MateriaArborScriptor* st,
                 StmlNodus* elementum,
     constans MateriaToken* lexema)
{
                SilvaFrons* frons  = (SilvaFrons*)datum;
                SilvaOrigo* o      = silva_token_origo(lexema);
                       Xar* sc     = silva_token_scissurae(lexema);
                   Piscina* p      = materia_arbor_scriptor_piscina(st);
       InternamentumChorda* in     = materia_arbor_scriptor_intern(st);
        constans character* tag;
                    chorda* macro = NIHIL;
                 StmlNodus* elem;
                 StmlNodus* scriptum;
                       i32  i;

    /* --- scissurae --- */
    si (sc != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(sc); i++)
        {
            SilvaScissura* s = (SilvaScissura*)xar_obtinere(sc, i);
                StmlNodus* es;

            si (s == NIHIL || s->offset < ZEPHYRUM)
            {
                materia_arbor_scriptor_recusare(st,
                    "scissura corrupta");
                redde FALSUM;
            }
            es = stml_elementum_creare(p, in, "scissura");
            si (   es == NIHIL
                || !materia_arbor_attributum_numeri(st, es, "offset",
                        (i32)s->offset))
            {
                materia_arbor_scriptor_recusare(st,
                    "scissura scribi non potuit");
                redde FALSUM;
            }
            si (s->crlf)
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
    commutatio (o->genus)
    {
    casus SILVA_ORIGO_FONS:
        redde VERUM;
    casus SILVA_ORIGO_EXPANSIO:
        tag = "expansio";       macro = o->datum.expansio.nomen_macro;
        frange;
    casus SILVA_ORIGO_PASTA:
        tag = "pasta";          macro = o->datum.pasta.nomen_macro;
        frange;
    casus SILVA_ORIGO_CHORDA:
        tag = "stringificatio"; macro =
                                    o->datum.stringificatio.nomen_macro;
        frange;
    casus SILVA_ORIGO_API:
        tag = "api";            macro = o->datum.api.nomen_macro;
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
    si (macro != NIHIL && macro->mensura > ZEPHYRUM)
    {
        stml_attributum_addere_chorda(elem, p, in, "macro", *macro);
    }
    /* DEF-SITE per REFERENTIAM, numquam inlinatum */
    si (   o->genus                 == SILVA_ORIGO_EXPANSIO
        && o->datum.expansio.corpus != NIHIL)
    {
        constans SilvaToken* d = o->datum.expansio.corpus;

        materia_arbor_attributum_numeri(st, elem, "def-f",
            (i32)d->fons_index);
        materia_arbor_attributum_numeri(st, elem, "def-l",
            (i32)d->linea);
        materia_arbor_attributum_numeri(st, elem, "def-c",
            (i32)d->columna);
    }

    commutatio (o->genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        si (o->datum.expansio.invocatio != NIHIL)
        {
            Xar* lamina;

            si (!_silvae_catenam_addere(st, elem,
                    o->datum.expansio.invocatio,
                    "invocatio in originem addi non potuit", &scriptum))
            { redde FALSUM;
            }
            /* EXTENTUM semel per invocationem: transclusio = iam
             * scriptum cum invocatione sua. */
            lamina = _silvae_extentum(frons,
                o->datum.expansio.invocatio);
            si (   scriptum->genus != STML_NODUS_TRANSCLUSIO
                && lamina          != NIHIL && xar_numerus(lamina) > I
                && !_extentum_ornare(st, elem, lamina))
            {
                redde FALSUM;
            }
        }
        frange;
    casus SILVA_ORIGO_PASTA:
        si (   o->datum.pasta.sinister != NIHIL
            && !_silvae_catenam_addere(st, elem,
            o->datum.pasta.sinister,
                   "invocatio in originem addi non potuit", &scriptum))
        { redde FALSUM;
        }
        si (   o->datum.pasta.dexter != NIHIL
            && !_silvae_catenam_addere(st, elem, o->datum.pasta.dexter,
                   "dexter in originem addi non potuit", &scriptum))
        { redde FALSUM;
        }
        si (o->datum.pasta.invocatio != NIHIL)
        {
            Xar* lamina;

            si (!_silvae_catenam_addere(st, elem,
                    o->datum.pasta.invocatio,
                    "invocatio pastae addi non potuit", &scriptum))
            { redde FALSUM;
            }
            lamina = _silvae_extentum(frons, o->datum.pasta.invocatio);
            si (   scriptum->genus != STML_NODUS_TRANSCLUSIO
                && lamina          != NIHIL && xar_numerus(lamina) > I
                && !_extentum_ornare(st, elem, lamina))
            {
                redde FALSUM;
            }
        }
        frange;
    casus SILVA_ORIGO_CHORDA:
        si (   o->datum.stringificatio.primus != NIHIL
            && !_silvae_catenam_addere(st, elem,
                   o->datum.stringificatio.primus,
                   "invocatio in originem addi non potuit", &scriptum))
        { redde FALSUM;
        }
        frange;
    ordinarius:
        frange;
    }
    si (!stml_liberum_addere(elementum, elem))
    {
        materia_arbor_scriptor_recusare(st,
            "origo in lexema addi non potuit");
        redde FALSUM;
    }
    redde VERUM;
}

/* ---------- lectio ---------- */

hic_manens b32
_silvae_attributa_legere (
                  vacuum* datum,
      MateriaArborLector* lector,
      constans StmlNodus* elementum,
            MateriaToken* lexema)
{
    chorda* a;
       i32  n;

    (vacuum)datum;
    /* LONGITUDO = valor, sicut lector silvae vetus (silva_token_ex_
     * fonte + _lexema_ponere): scissurae non numerantur */
    silva_token_longitudo_ponere(lexema, lexema->valor.mensura);
    silva_token_standard_ponere(lexema, (i8)SILVA_STANDARD_C89);
    a = stml_attributum_capere((StmlNodus*)(size_t)elementum,
        "standard");
    si (a != NIHIL)
    {
        si (!materia_arbor_numerus_ex_chorda(a, &n))
        {
            redde materia_arbor_lector_recusare(lector,
                "standard non numerus", elementum->linea);
        }
        silva_token_standard_ponere(lexema, (i8)n);
    }
    redde VERUM;
}

/* Extentum lectum: lamina [nomen..')'], lexema PRIMUM est invocatio
 * ipsa (IDENTITAS - vide _extentum_legere supra); in extenta lecta
 * frontis registratur, ut uncus extenti id inveniat. */
hic_manens b32
_silvae_extentum_legere (
             SilvaFrons* frons,
     MateriaArborLector* lector,
              StmlNodus* elementum,
             SilvaToken* invocatio)
{
                          Xar* lamina;
    SilvaExtentumInvocationis* cella;
                          i32  cursor;
                          i32  numerus;

    si (invocatio == NIHIL)
    {
        redde materia_arbor_lector_recusare(lector,
            "extentum sine invocatione", elementum->linea);
    }
    lamina   = xar_creare(frons->piscina, magnitudo(SilvaToken*));
    numerus  = stml_numerus_liberorum(elementum);
    per (cursor = ZEPHYRUM; lamina != NIHIL && cursor < numerus;
         cursor++)
    {
        StmlNodus* liberum = stml_liberum_ad_indicem(elementum, cursor);
       SilvaToken* lectum;

        si (   liberum == NIHIL
            || (   liberum->genus != STML_NODUS_ELEMENTUM
                && liberum->genus != STML_NODUS_TRANSCLUSIO))
        { perge;
        }
        lectum = materia_arbor_lexema_legere(lector, liberum, NIHIL);
        si (lectum == NIHIL)
        { redde FALSUM;
        }
        *(SilvaToken**)xar_addere(lamina) = lectum;
    }
    si (lamina == NIHIL)
    {
        redde materia_arbor_lector_recusare(lector,
            "lamina extenti creari non potuit", elementum->linea);
    }
    si (xar_numerus(lamina) > ZEPHYRUM)
    {
        *(SilvaToken**)xar_obtinere(lamina, ZEPHYRUM) = invocatio;
    }
    si (frons->extenta_lecta == NIHIL)
    {
        frons->extenta_lecta = xar_creare(frons->piscina,
            magnitudo(SilvaExtentumInvocationis));
    }
    cella = (frons->extenta_lecta != NIHIL)
        ? (SilvaExtentumInvocationis*)xar_addere(frons->extenta_lecta)
        : NIHIL;
    si (cella == NIHIL)
    {
        redde materia_arbor_lector_recusare(lector,
            "extentum addi non potuit", elementum->linea);
    }
    memset(cella, ZEPHYRUM, magnitudo(*cella));
    cella->invocatio  = invocatio;
    cella->lamina     = lamina;
    redde VERUM;
}

hic_manens s32
_silvae_liberum_legere (
                 vacuum* datum,
     MateriaArborLector* lector,
     constans StmlNodus* liberum,
           MateriaToken* lexema)
{
      SilvaFrons* frons  = (SilvaFrons*)datum;
       StmlNodus* el     = (StmlNodus*)(size_t)liberum;
      SilvaOrigo* o;
          chorda* a;
 SilvaOrigoGenus  genus;
      SilvaToken* primus     = NIHIL;
      SilvaToken* secundus   = NIHIL;
      SilvaToken* tertius    = NIHIL;
      SilvaToken* definitio  = NIHIL;
             i32  cursor;
             i32  numerus;

    si (liberum->titulus == NIHIL)
    { redde (s32)MATERIA_LECTIO_IGNOTUM;
    }

    /* --- scissura --- */
    si (chorda_aequalis_literis(*liberum->titulus, "scissura"))
    {
        SilvaScissura  sc;
                  i32  offset;
                  Xar* series = silva_token_scissurae(lexema);

        a = stml_attributum_capere(el, "offset");
        si (!materia_arbor_numerus_ex_chorda(a, &offset))
        {
            materia_arbor_lector_recusare(lector,
                "scissura sine offset", liberum->linea);
            redde (s32)MATERIA_LECTIO_FRACTUM;
        }
        sc.offset  = (s32)offset;
        sc.crlf    = stml_attributum_habet(el, "crlf");
        si (series == NIHIL)
        {
            series = xar_creare(frons->piscina,
                magnitudo(SilvaScissura));
            silva_token_scissurae_ponere(lexema, series);
        }
        *(SilvaScissura*)xar_addere(series) = sc;
        redde (s32)MATERIA_LECTIO_ACCEPTUM;
    }

    /* --- origo --- */
    si (chorda_aequalis_literis(*liberum->titulus, "expansio"))
    { genus = SILVA_ORIGO_EXPANSIO;
    }
    alioquin si (chorda_aequalis_literis(*liberum->titulus, "pasta"))
    { genus = SILVA_ORIGO_PASTA;
    }
    alioquin si (chorda_aequalis_literis(*liberum->titulus,
                     "stringificatio"))
    { genus = SILVA_ORIGO_CHORDA;
    }
    alioquin si (chorda_aequalis_literis(*liberum->titulus, "api"))
    { genus = SILVA_ORIGO_API;
    }
    alioquin
    { redde (s32)MATERIA_LECTIO_IGNOTUM;
    }

    numerus = stml_numerus_liberorum(el);
    per (cursor = ZEPHYRUM; cursor < numerus; cursor++)
    {
        StmlNodus* n_lib = stml_liberum_ad_indicem(el, cursor);
       SilvaToken* lectum;

        si (   n_lib == NIHIL
            || (   n_lib->genus != STML_NODUS_ELEMENTUM
                && n_lib->genus != STML_NODUS_TRANSCLUSIO))
        { perge;
        }
        si (   n_lib->genus   == STML_NODUS_ELEMENTUM
            && n_lib->titulus != NIHIL
            && chorda_aequalis_literis(*n_lib->titulus, "extentum"))
        {
            si (!_silvae_extentum_legere(frons, lector, n_lib,
                     (genus == SILVA_ORIGO_PASTA) ? tertius : primus))
            {
                redde (s32)MATERIA_LECTIO_FRACTUM;
            }
            perge;
        }
        lectum = materia_arbor_lexema_legere(lector, n_lib, NIHIL);
        si (lectum == NIHIL)
        { redde (s32)MATERIA_LECTIO_FRACTUM;
        }
        si (primus == NIHIL)
        { primus = lectum;
        }
        alioquin si (secundus == NIHIL)
        { secundus = lectum;
        }
        alioquin si (tertius == NIHIL)
        { tertius = lectum;
        }
    }

    /* DEF-SITE: lexema SYNTHETICUM ex referentia (sicut silva_arbor.c
     * _origo_legere) - fontem et lineam fert, sedem veram non */
    a = stml_attributum_capere(el, "def-l");
    si (a != NIHIL)
    {
           i32 linea    = ZEPHYRUM;
           i32 fons     = ZEPHYRUM;
           i32 columna  = ZEPHYRUM;
        chorda vacua;

        (vacuum)materia_arbor_numerus_ex_chorda(a, &linea);
        a = stml_attributum_capere(el, "def-f");
        si (a != NIHIL)
        { (vacuum)materia_arbor_numerus_ex_chorda(a, &fons);
        }
        a = stml_attributum_capere(el, "def-c");
        si (a != NIHIL)
        { (vacuum)materia_arbor_numerus_ex_chorda(a, &columna);
        }
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        definitio = silva_token_ex_fonte(frons->piscina,
            SILVA_LEX_IDENTIFICATOR, vacua, (s32)-I, linea, columna,
            (s32)fons);
    }

    a         = stml_attributum_capere(el, "macro");
    o         = silva_token_origo(lexema);
    o->genus  = genus;
    commutatio (genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        o->datum.expansio.invocatio    = primus;
        o->datum.expansio.corpus       = definitio;
        o->datum.expansio.nomen_macro  = a;
        o->datum.expansio.caecatio     = NIHIL;
        frange;
    casus SILVA_ORIGO_PASTA:
        o->datum.pasta.sinister     = primus;
        o->datum.pasta.dexter       = secundus;
        o->datum.pasta.invocatio    = tertius;
        o->datum.pasta.nomen_macro  = a;
        o->datum.pasta.caecatio     = NIHIL;
        frange;
    casus SILVA_ORIGO_CHORDA:
        o->datum.stringificatio.primus       = primus;
        o->datum.stringificatio.nomen_macro  = a;
        frange;
    casus SILVA_ORIGO_API:
        o->datum.api.nomen_macro = a;
        frange;
    ordinarius:
        frange;
    }
    redde (s32)MATERIA_LECTIO_ACCEPTUM;
}

/* Cursor per valorem CUM laminis reinsertis (sicut silva_arbor.c
 * _lexema_ponere) */
hic_manens b32
_silvae_cursorem_movere (
                 vacuum* datum,
     MateriaArborCursor* c,
  constans MateriaToken* lexema)
{
    Xar* sc = silva_token_scissurae(lexema);
    i32  i;
    i32  s_idx;
    i32  n_sc;

    (vacuum)datum;
    si (sc == NIHIL)
    { redde FALSUM;
    }
    n_sc   = xar_numerus(sc);
    s_idx  = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= lexema->valor.mensura; i++)
    {
        dum (s_idx < n_sc)
        {
            SilvaScissura* s = (SilvaScissura*)xar_obtinere(sc, s_idx);

            si (s == NIHIL || s->offset != (s32)i)
            { frange;
            }
            c->offset += s->crlf ? III : II;
            c->linea++;
            c->columna = I;
            s_idx++;
        }
        si (i == lexema->valor.mensura)
        { frange;
        }
        si ((character)lexema->valor.datum[i] == '\n')
        { c->linea++; c->columna = I;
        }
        alioquin
        { c->columna++;
        }
        c->offset++;
    }
    redde VERUM;
}

/* LONGITUDO triviorum et catenarum: lector materiae trivia SINE unco
 * creat (cauda zephyrata). Lector silvae vetus OMNE lexema per
 * silva_token_ex_fonte creabat (longitudo = valor) - hic idem post
 * lectionem. */
hic_manens vacuum
_silvae_longitudo_lexematis (
    SilvaToken* t,
           i32  profunditas);

hic_manens vacuum
_silvae_longitudo_catenae (
    SilvaToken* t,
           i32  profunditas)
{
    SilvaOrigo* o;

    si (t == NIHIL || profunditas > (i32)LXIV)
    { redde;
    }
    o = silva_token_origo(t);
    commutatio (o->genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        _silvae_longitudo_lexematis(o->datum.expansio.invocatio,
            profunditas + I);
        _silvae_longitudo_lexematis(o->datum.expansio.corpus,
            profunditas + I);
        frange;
    casus SILVA_ORIGO_PASTA:
        _silvae_longitudo_lexematis(o->datum.pasta.sinister,
            profunditas + I);
        _silvae_longitudo_lexematis(o->datum.pasta.dexter,
            profunditas + I);
        _silvae_longitudo_lexematis(o->datum.pasta.invocatio,
            profunditas + I);
        frange;
    casus SILVA_ORIGO_CHORDA:
        _silvae_longitudo_lexematis(o->datum.stringificatio.primus,
            profunditas + I);
        frange;
    ordinarius:
        frange;
    }
}

hic_manens vacuum
_silvae_longitudo_lexematis (
    SilvaToken* t,
           i32  profunditas)
{
    i32 i;

    si (t == NIHIL)
    { redde;
    }
    silva_token_longitudo_ponere(t, t->valor.mensura);
    per (i = ZEPHYRUM; i < silva_token_ante_numerus(t); i++)
    {
        SilvaToken* tr = silva_token_ante(t, i);

        si (tr != NIHIL)
        { silva_token_longitudo_ponere(tr, tr->valor.mensura);
        }
    }
    per (i = ZEPHYRUM; i < silva_token_post_numerus(t); i++)
    {
        SilvaToken* tr = silva_token_post(t, i);

        si (tr != NIHIL)
        { silva_token_longitudo_ponere(tr, tr->valor.mensura);
        }
    }
    _silvae_longitudo_catenae(t, profunditas);
}

hic_manens vacuum
_silvae_longitudo_valoris (
    MateriaValor v)
{
    i32 i;

    commutatio (v.genus)
    {
    casus MATERIA_VALOR_TOKEN:
        _silvae_longitudo_lexematis(v.datum.token, ZEPHYRUM);
        frange;
    casus MATERIA_VALOR_NODUS:
        si (v.datum.nodus != NIHIL)
        {
            per (i = ZEPHYRUM; i < v.datum.nodus->numerus_locorum; i++)
            { _silvae_longitudo_valoris(v.datum.nodus->loci[i]);
            }
        }
        frange;
    casus MATERIA_VALOR_LISTA:
        per (i = ZEPHYRUM; i < materia_valor_lista_numerus(v); i++)
        {
            MateriaValor* e = materia_valor_lista_obtinere(v, i);

            si (e != NIHIL)
            { _silvae_longitudo_valoris(*e);
            }
        }
        frange;
    ordinarius:
        frange;
    }
}

vacuum
silva_frons_longitudines_figere (
    SilvaFrons* frons,
    SilvaValor  valor)
{
    i32 k;

    _silvae_longitudo_valoris(valor);
    si (frons == NIHIL || frons->extenta_lecta == NIHIL)
    { redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(frons->extenta_lecta); k++)
    {
        SilvaExtentumInvocationis* e = (SilvaExtentumInvocationis*)
            xar_obtinere(frons->extenta_lecta, k);
                               i32 j;

        per (j = ZEPHYRUM; e != NIHIL && e->lamina != NIHIL
            && j < xar_numerus(e->lamina); j++)
        {
            _silvae_longitudo_lexematis(
                *(SilvaToken**)xar_obtinere(e->lamina, j), ZEPHYRUM);
        }
    }
}

hic_manens b32
_silvae_perficere (
                vacuum* datum,
    MateriaArborLector* lector,
          MateriaNodus* radix)
{
    (vacuum)lector;
    silva_frons_longitudines_figere((SilvaFrons*)datum,
        materia_valor_nodus(radix));
    redde VERUM;
}

vacuum
silva_frons_extenta_lecta_ponere (
    SilvaFrons* frons,
           Xar* extenta)
{
    si (frons != NIHIL)
    {
        frons->extenta_lecta = extenta;
    }
}

b32
silva_frons_arborem_silvae_parare (
                       SilvaFrons* frons,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* grammatica,
              InternamentumChorda* intern,
            MateriaArborConsilium* consilium)
{
    MateriaLexIudicium iudicium;

    si (frons == NIHIL || consilium == NIHIL || tabularium == NIHIL)
    { redde FALSUM;
    }
    si (!materia_lexicon_ratum_facere(&frons->lexratum, &LEXICON_C89,
            &iudicium))
    {
        frons->causa = materia_lexicon_vitium_nomen(
            (MateriaLexVitium)frons->lexratum.ratum);
        redde FALSUM;
    }
    frons->uncus_silvae.datum              = frons;
    frons->uncus_silvae.sedes_quaerere     = _silvae_sedes_quaerere;
    frons->uncus_silvae.radix_quaerere     = _silvae_radix_quaerere;
    frons->uncus_silvae.extentum_quaerere  = _silvae_extentum_quaerere;

    memset(&frons->frons_silvae, ZEPHYRUM,
        magnitudo(frons->frons_silvae));
    frons->frons_silvae.datum             = frons;
    frons->frons_silvae.origo_numerare    = _silvae_origo_numerare;
    frons->frons_silvae.attributa_ornare  = _silvae_attributa_ornare;
    frons->frons_silvae.liberos_ornare    = _silvae_liberos_ornare;
    frons->frons_silvae.attributa_legere  = _silvae_attributa_legere;
    frons->frons_silvae.liberum_legere    = _silvae_liberum_legere;
    frons->frons_silvae.cursorem_movere   = _silvae_cursorem_movere;
    frons->frons_silvae.perficere         = _silvae_perficere;

    materia_arbor_consilium_nudum(consilium, tabularium,
        &frons->lexratum,
        grammatica);
    consilium->intern  = intern;
    consilium->origo   = &frons->uncus_silvae;
    consilium->frons   = &frons->frons_silvae;
    consilium->forma   = *silva_token_forma();
    redde VERUM;
}

/* ---------- superficies publica ---------- */
SilvaFrons*
silva_frons_creare (
                   Piscina* piscina,
    constans SilvaExpansio* expansio)
{
    SilvaFrons* frons;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    frons = (SilvaFrons*)piscina_allocare_ordinatum(piscina,
        magnitudo(SilvaFrons), magnitudo(vacuum*));
    si (frons == NIHIL)
    {
        redde NIHIL;
    }
    frons->piscina   = piscina;
    frons->lexemata  = xar_creare(piscina, magnitudo(MateriaToken*));
    frons->index     = tabula_dispersa_creare_chorda(piscina, 4096);
    frons->expansio  = expansio;
    frons->tabularium = _registrum_convertere(piscina,
        &SILVA_C89_REGISTRUM);
    frons->causa          = NIHIL;
    frons->extenta_lecta  = NIHIL;

    frons->uncus.datum              = frons;
    frons->uncus.sedes_quaerere     = _sedes_quaerere;
    frons->uncus.radix_quaerere     = _radix_quaerere;
    frons->uncus.extentum_quaerere  = _extentum_quaerere;

    si (frons->lexemata == NIHIL || frons->index == NIHIL)
    {
        redde NIHIL;
    }
    redde frons;
}

MateriaValor
silva_frons_valorem_convertere (
     SilvaFrons* frons,
     SilvaValor  valor)
{
    redde _valor_convertere(frons, valor);
}

MateriaNodus*
silva_frons_nodum_convertere (
    SilvaFrons* frons,
    SilvaNodus* nodus)
{
    redde _nodus_convertere(frons, nodus);
}

vacuum
silva_frons_scripturam_parare (
                   SilvaFrons* frons,
    MateriaScripturaConsilium* consilium)
{
    materia_scriptura_consilium_nudum(consilium, &frons->tabularium);
    consilium->origo             = &frons->uncus;
    consilium->valorem_emittere  = _valorem_emittere;
}

b32
silva_frons_arborem_parare (
               SilvaFrons* frons,
    MateriaArborConsilium* consilium)
{
    MateriaLexIudicium iudicium;

    si (!materia_lexicon_ratum_facere(&frons->lexratum, &LEXICON_C89,
            &iudicium))
    {
        frons->causa = materia_lexicon_vitium_nomen(
            (MateriaLexVitium)frons->lexratum.ratum);
        redde FALSUM;
    }
    materia_arbor_consilium_nudum(consilium, &frons->tabularium,
        &frons->lexratum, "c89");
    consilium->origo = &frons->uncus;
    consilium->frons = &FRONS_C89;
    consilium->forma = FORMA;
    redde VERUM;
}

i32
silva_frons_lexemata_numerus (
    constans SilvaFrons* frons)
{
    si (frons == NIHIL || frons->lexemata == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(frons->lexemata);
}

constans character*
silva_frons_causa (
    constans SilvaFrons* frons)
{
    redde (frons != NIHIL) ? frons->causa : NIHIL;
}
