/* silva_frons.c - FRONS C89 super materiam (phasis V)
 *
 * Unci C89 quibus scriptor et lector arboris MATERIAE lexemata SILVAE
 * ipsa tractant: SilvaToken EST MateriaToken (T7b), SilvaNodus EST
 * MateriaNodus (T8) - nihil vertitur. Cauda silvae (origo, standard,
 * scissurae) per accessores legitur et scribitur.
 *
 * HISTORIA. In shim natus (materia/instrumenta/shim_c89.c) ut
 * emissorem materiae contra silvam iudicaret, T6b promotus, ubi
 * arborem silvae in typos materiae VERTEBAT cum cauda propria
 * (SilvaFronsCauda). T10b unci super caudam silvae IPSAM additi et
 * contra scriptorem silvae veterem probati (XI,DCCXI nodi); T10c
 * silva tota eis utitur, et conversio cum shim recessit (2026-09-25).
 */

#include "silva_frons.h"
#include "chorda.h"
#include "xar.h"
#include "chorda_aedificator.h"
#include "stml.h"
#include "silva_token.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "silva_lexicon_c89.h"
#include <string.h>

/* Contextus per plagulam (opacus in capite). Unci eum per 'datum'
 * accipiunt - nullus status globalis. */
structura SilvaFrons {
                   Piscina* piscina;
    constans SilvaExpansio* expansio;
       MateriaLexiconRatum  lexratum;
        constans character* causa;

    /* Tabula frontis per frontem (non statica) quia unci contextum
     * per 'datum' poscunt - extenta LECTA in contextu vivunt (lectio
     * subarboris expansionem non habet; lector silvae vetus ideo
     * 'extentum sine expansione' recusabat, CI nodos). */
          MateriaArborFrons  frons_silvae;
          MateriaOrigoUncus  uncus_silvae;
                        Xar* extenta_lecta;  /* SilvaExtentumInvocationis */
};

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

/* Radix strati 0 lexematis DERIVATI per catenam originis (primus
 * gradus ab origine eius - lexema FONS vocans ipse tractat); NIHIL +
 * causa si irrecuperabilis. Nucleus communis uncorum radicis
 * (emissio) et sedis (T14): monstratores catenae non-constantes sunt,
 * ergo lexema constans accipitur sine cast. */
hic_manens SilvaToken*
_silvae_radix_derivati (
     constans SilvaToken*  token,
      constans character** causa)
{
    SilvaOrigo* o = silva_token_origo(token);
           i32  custodia;

    per (custodia = ZEPHYRUM; custodia < (i32)CCLVI; custodia++)
    {
        SilvaToken* proximum;

        commutatio (o->genus)
        {
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
        o = silva_token_origo(proximum);
        si (o->genus == SILVA_ORIGO_FONS)
        {
            redde proximum;
        }
    }
    *causa = "catena originis nimis longa";
    redde NIHIL;
}

hic_manens MateriaToken*
_silvae_radix_quaerere (
                vacuum*  datum,
          MateriaToken*  token,
    constans character** causa)
{
    (vacuum)datum;
    si (silva_token_origo(token)->genus == SILVA_ORIGO_FONS)
    {
        redde token;
    }
    redde _silvae_radix_derivati(token, causa);
}

/* Sedes EFFICAX (contractus materia_nodus.h: extensio, puritas,
 * geometria, commentarium): lexema fontis = sedes sua; lexema
 * DERIVATUM = sedes RADICIS eius (invocatio in strato 0 - ubi in
 * plagula apparet), est_fons tamen FALSUM (puritas, ordo octetorum
 * id praetereunt). Radix irrecuperabilis (pasta, chorda, api) =
 * sine sede (-I): ab extensionibus omittitur, non fingitur - ut
 * silva_nodus_extensionem 'synthetica omissa'.
 *
 * Ante T14 sedes DEF-SITE reddebatur (octeti corporis macro in
 * latina.h): extensio nodi a lexemate derivato incipientis in
 * plagulam ALIAM cadebat (materia_tractus_conferre fontem primi
 * sequitur) - diagnostica C89 in lineis latinae nominabantur. */
hic_manens vacuum
_silvae_sedes_quaerere (
                   vacuum* datum,
    constans MateriaToken* token,
             MateriaSedes* sedes)
{
    constans MateriaToken* radix;
       constans character* causa = NIHIL;

    (vacuum)datum;
    sedes->est_fons = (b32)(silva_token_origo(token)->genus
        == SILVA_ORIGO_FONS);
    radix = sedes->est_fons
        ? token
        : _silvae_radix_derivati(token, &causa);
    si (radix == NIHIL)
    {
        sedes->byte_offset  = (s32)-I;
        sedes->linea        = ZEPHYRUM;
        sedes->columna      = ZEPHYRUM;
        sedes->fons_index   = token->fons_index;
        redde;
    }
    sedes->byte_offset  = radix->byte_offset;
    sedes->linea        = radix->linea;
    sedes->columna      = radix->columna;
    sedes->fons_index   = radix->fons_index;
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
    memset(frons, ZEPHYRUM, magnitudo(*frons));
    frons->piscina   = piscina;
    frons->expansio  = expansio;
    redde frons;
}

constans MateriaOrigoUncus*
silva_frons_uncus (
    constans SilvaFrons* frons)
{
    redde (frons != NIHIL) ? &frons->uncus_silvae : NIHIL;
}

constans character*
silva_frons_causa (
    constans SilvaFrons* frons)
{
    redde (frons != NIHIL) ? frons->causa : NIHIL;
}
