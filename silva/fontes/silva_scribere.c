/* silva_scribere.c - Emissio arborum ad octetos, SUPER MATERIAM
 *
 * silva-migratio T9 (2026-09-25): ambulatio, emissio lexematum,
 * deduplicatio invocationum et fusio reinserendorum sunt
 * materia_scribere.c (portatum ex hoc modulo - vide caput eius:
 * semita una, radix == lexema pro FONS). Hic manet quod C89
 * PROPRIUM est:
 *
 *   unci - radix strati 0 per catenam originis (PASTA/CHORDA/API
 *   fractura clara), extentum invocationis functio-similis (per
 *   identitatem, dein per CONTINENTIAM pro lexematibus CHORDA),
 *   valor cum scissuris (silva_lexema_valorem_emittere_in);
 *
 *   plagula tota (fontem) - reinserenda: lineae directivae
 *   consumptae + laminae ramorum non sumptorum + invocationes
 *   vacuae, per fontem filtrata, per offset ordinata, cauda eorum
 *   fusa (reinserenda_finire), dein lexema EOF.
 *
 * Lexemata silvae SUNT MateriaToken (T7b) et nodi MateriaNodus
 * (T8): nulla conversio, unci caudam silvae per accessores legunt.
 */

#include "silva_scribere.h"
#include "silva_lexema.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"


/* Datum uncorum: contextus expansionis (NIHIL licet - tunc lexema
 * expansum quodlibet fractura clara est). */
nomen structura {
    constans SilvaExpansio* expansio;
} SilvaScribereDatum;


/* ==================================================
 * Limes expansionis (Chunk B, sim ⑥ C4/C5)
 * ================================================== */

/* Radix strati 0 cum probatione puritatis: catenam invocationum
 * sequitur. PASTA/CHORDA (radix trans plagulam definitionis errare
 * potest) et API (fons syntheticus) = stratum 0 non recuperabile -
 * deferral nominatum (INTENTIO Phase 5). */
interior SilvaToken*
_radix_probata (
    SilvaToken* token,
           b32* impurum_out)
{
    dum (token != NIHIL)
    {
        commutatio (silva_token_origo(token)->genus)
        {
            casus SILVA_ORIGO_FONS:
                redde token;
            casus SILVA_ORIGO_EXPANSIO:
                token =
                    silva_token_origo(token)->datum.expansio.invocatio;
                frange;
            casus SILVA_ORIGO_CHORDA:
                /* stringificatio (#x): primus = lexema primum
                 * argumenti CRUDI (use-site) - intra extentum
                 * invocationis iacet; quaestio continentiae infra
                 * extentum invenit (Chunk C - corpus solarii
                 * deferral coegit, vectis maximalista) */
                token =
                    silva_token_origo(token)->datum.stringificatio.primus;
                frange;
            casus SILVA_ORIGO_PASTA:
                /* PASTA: invocationem sequi, NON parentes.
                 *
                 * Parens uterque ex CORPORE macri venire potest
                 * ('#define C(a) pre##a' - 'pre' def-site est,
                 * 'a' argumentum substitutum), ergo a parentibus
                 * ad usum semita non semper ducit: id est quod
                 * nota vetus 'radix trans plagulam definitionis
                 * errare potest' dicebat. Campus 'invocatio'
                 * (arma PASTAE additus) semitam rectam dat,
                 * sicut EXPANSIO iam habebat. Sine eo (lexema
                 * vetus) recusatio nominata manet. */
                si (silva_token_origo(token)->datum.pasta.invocatio
                    == NIHIL)
                {
                    *impurum_out = VERUM;
                    redde token;
                }
                token = silva_token_origo(token)->datum.pasta.invocatio;
                frange;
            ordinarius:
                *impurum_out = VERUM;
                redde token;
        }
    }
    *impurum_out = VERUM;
    redde NIHIL;
}

/* Extentum cuius lamina offset radicis CONTINET (pro lexematibus
 * CHORDA: radix = lexema argumenti INTRA invocationem, non lexema
 * nominis - quaestio per identitatem fallit, continentia invenit).
 * Scansio linearis - numeri parvi. */
interior Xar*
_extentum_continens (
    constans SilvaExpansio* expansio,
       constans SilvaToken* radix)
{
    i32 k;

    si (expansio == NIHIL || expansio->extenta == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(expansio->extenta); k++)
    {
        SilvaExtentumInvocationis* extentum =
            (SilvaExtentumInvocationis*)xar_obtinere(
                expansio->extenta, k);
        SilvaToken* primum;
        SilvaToken* ultimum;

        si (   extentum == NIHIL || extentum->lamina == NIHIL
            || xar_numerus(extentum->lamina) == ZEPHYRUM)
        {
            perge;
        }
        primum = *(SilvaToken**)xar_obtinere(extentum->lamina,
            ZEPHYRUM);
        ultimum = *(SilvaToken**)xar_obtinere(extentum->lamina,
            (i32)(xar_numerus(extentum->lamina) - I));
        si (   primum->fons_index == radix->fons_index
            && radix->byte_offset >= primum->byte_offset
            && radix->byte_offset
                < ultimum->byte_offset
                    + (s32)silva_token_longitudo(ultimum))
        {
            redde extentum->lamina;
        }
    }
    redde NIHIL;
}

/* Extentum invocationis functio-similis strati 0 (scansio linearis -
 * numeri parvi; radix comparatur per identitatem monstratoris) */
interior Xar*
_extentum_quaerere (
    constans SilvaExpansio* expansio,
       constans SilvaToken* radix)
{
    i32 k;

    si (expansio == NIHIL || expansio->extenta == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(expansio->extenta); k++)
    {
        SilvaExtentumInvocationis* ext = (SilvaExtentumInvocationis*)
            xar_obtinere(expansio->extenta, k);

        si (ext != NIHIL && ext->invocatio == radix)
        {
            redde ext->lamina;
        }
    }
    redde NIHIL;
}


/* ==================================================
 * Unci materiae
 * ================================================== */

interior MateriaToken*
_radix_unci (
                  vacuum*  datum,
            MateriaToken*  token,
      constans character** causa)
{
    constans SilvaScribereDatum* d =
        (constans SilvaScribereDatum*)datum;
                     SilvaToken* radix;
                            b32  impurum;

    si (silva_token_origo(token)->genus == SILVA_ORIGO_FONS)
    {
        redde token;
    }
    /* Limes expansionis: stratum 0 emittitur, non lexema expansum */
    si (d->expansio == NIHIL)
    {
        *causa = "lexema expansum sine contextu expansionis";
        redde NIHIL;
    }
    impurum  = FALSUM;
    radix    = _radix_probata(token, &impurum);
    si (impurum || radix == NIHIL)
    {
        *causa = "origo pasta/chorda/api - stratum 0 non "
            "recuperabile (deferral nominatum)";
        redde NIHIL;
    }
    redde radix;
}

/* Extentum invocationis functio-similis: per identitatem radicis,
 * dein per continentiam (radix intra invocationem - lexema argumenti
 * per CHORDA/stringificationem). NIHIL = obiectum-simile. */
interior Xar*
_extentum_unci (
                   vacuum* datum,
    constans MateriaToken* radix)
{
    constans SilvaScribereDatum* d =
        (constans SilvaScribereDatum*)datum;
                             Xar* extentum;

    extentum = _extentum_quaerere(d->expansio, radix);
    si (extentum == NIHIL)
    {
        extentum = _extentum_continens(d->expansio, radix);
    }
    redde extentum;
}

interior b32
_valorem_unci (
                   vacuum* datum,
        ChordaAedificator* aed,
    constans MateriaToken* token)
{
    (vacuum)datum;
    silva_lexema_valorem_emittere_in(aed, token);
    redde VERUM;
}

interior vacuum
_consilium_parare (
     MateriaScripturaConsilium* consilium,
             MateriaOrigoUncus* uncus,
            SilvaScribereDatum* datum,
 constans SilvaRegistrumCoctum* tabularium,
        constans SilvaExpansio* expansio,
                           s32  fons_index)
{
    datum->expansio           = expansio;
    uncus->datum              = datum;
    uncus->sedes_quaerere     = NIHIL;
    uncus->radix_quaerere     = _radix_unci;
    uncus->extentum_quaerere  = _extentum_unci;
    materia_scriptura_consilium_nudum(consilium, tabularium);
    consilium->origo             = uncus;
    consilium->valorem_emittere  = _valorem_unci;
    consilium->valorem_datum     = datum;
    consilium->fons_index        = fons_index;
}


/* ==================================================
 * Reinserenda colligere (fontem)
 * ================================================== */

interior vacuum
_reinserendum_addere (
           Xar** reinserenda,
       Piscina*  piscina,
           s32   fons_index,
           Xar*  lamina)
{
             SilvaToken* primum;
    MateriaReinserendum* r;

    si (lamina == NIHIL || xar_numerus(lamina) == ZEPHYRUM)
    {
        redde;
    }
    primum = *(SilvaToken**)xar_obtinere(lamina, ZEPHYRUM);
    si (   fons_index         >= ZEPHYRUM
        && primum->fons_index != fons_index)
    {
        redde;
    }
    si (*reinserenda == NIHIL)
    {
        *reinserenda = xar_creare(piscina,
            magnitudo(MateriaReinserendum));
    }
    r = (MateriaReinserendum*)xar_addere(*reinserenda);
    si (r != NIHIL)
    {
        r->offset = primum->byte_offset;
        r->lamina = lamina;
    }
}

/* Arbor regionum recursive: regiones NON textae lineas structurales
 * suas (rami directiva + directiva_finis - β, sim ⑦ C2) et laminas
 * crudas reinserendis dant; regiones TEXTAE omnia ex ARBORE emittunt
 * (dominus unus) - sed filiae semper visitantur (regio degradata
 * intra textam sua adhuc possidet reinserendis). */
interior vacuum
_regiones_colligere (
        Xar** reinserenda,
    Piscina*  piscina,
        s32   fons_index,
        Xar*  regiones)
{
    i32 i;

    si (regiones == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(regiones); i++)
    {
        SilvaRegio* regio = *(SilvaRegio**)xar_obtinere(regiones, i);
               i32  j;

        si (regio == NIHIL) perge;
        si (!regio->est_texta && regio->rami != NIHIL)
        {
            per (j = ZEPHYRUM; j < xar_numerus(regio->rami); j++)
            {
                SilvaRamus* ramus = *(SilvaRamus**)xar_obtinere(
                    regio->rami, j);

                si (ramus == NIHIL) perge;
                si (ramus->directiva != NIHIL)
                {
                    _reinserendum_addere(reinserenda, piscina,
                        fons_index, ramus->directiva);
                }
                si (ramus->lexemata_cruda != NIHIL)
                {
                    _reinserendum_addere(reinserenda, piscina,
                        fons_index, ramus->lexemata_cruda);
                }
            }
        }
        si (!regio->est_texta && regio->directiva_finis != NIHIL)
        {
            _reinserendum_addere(reinserenda, piscina, fons_index,
                regio->directiva_finis);
        }
        _regiones_colligere(reinserenda, piscina, fons_index,
            regio->filiae);
    }
}


/* ==================================================
 * API
 * ================================================== */

interior SilvaScriptura
_silvae_scriptura_fracta (
    constans character* causa)
{
    SilvaScriptura s;

    s.successus       = FALSUM;
    s.textus.datum    = NIHIL;
    s.textus.mensura  = ZEPHYRUM;
    s.causa           = causa;
    s.sedes           = NIHIL;
    redde s;
}

SilvaScriptura
silva_scribere_valorem (
                          Piscina* piscina,
                       SilvaValor  valor,
    constans SilvaRegistrumCoctum* tabularium,
           constans SilvaExpansio* expansio)
{
    MateriaScripturaConsilium consilium;
            MateriaOrigoUncus uncus;
           SilvaScribereDatum datum;

    _consilium_parare(&consilium, &uncus, &datum, tabularium, expansio,
        (s32)-I);
    redde materia_scribere_valorem(piscina, valor, &consilium);
}

SilvaScriptura
silva_scribere_nodum (
                          Piscina* piscina,
              constans SilvaNodus* nodus,
    constans SilvaRegistrumCoctum* tabularium,
           constans SilvaExpansio* expansio)
{
    MateriaScripturaConsilium consilium;
            MateriaOrigoUncus uncus;
           SilvaScribereDatum datum;

    _consilium_parare(&consilium, &uncus, &datum, tabularium, expansio,
        (s32)-I);
    redde materia_scribere_nodum(piscina, nodus, &consilium);
}

/* Lexema EOF plagulae (trivia caudae): plagulae inclusae EOF suum in
 * includenda retinent (fluxus reliquorum id abicit - sim ⑥ C6).
 * NIHIL = nullum, aut plagulae alienae. */
interior SilvaToken*
_lexema_finis (
    constans SilvaParsura* parsura,
                      s32  fons_index)
{
    si (   parsura->lexema_finis != NIHIL
        && (fons_index < ZEPHYRUM
            || parsura->lexema_finis->fons_index == fons_index))
    {
        redde parsura->lexema_finis;
    }
    si (fons_index >= ZEPHYRUM && parsura->expansio != NIHIL)
    {
        SilvaFons* fons = (SilvaFons*)xar_obtinere(
            parsura->expansio->fontes, (i32)fons_index);
        vacuum* valor;

        si (   fons != NIHIL && fons->via != NIHIL
            && tabula_dispersa_invenire(parsura->expansio->includenda,
                   *fons->via, &valor))
        {
            SilvaIncludendum* incl = (SilvaIncludendum*)valor;

            si (   incl != NIHIL && incl->lexemata != NIHIL
                && xar_numerus(incl->lexemata) > ZEPHYRUM)
            {
                SilvaToken* ultimum = *(SilvaToken**)xar_obtinere(
                    incl->lexemata,
                    (i32)(xar_numerus(incl->lexemata) - I));

                si (   ultimum             != NIHIL
                    && ultimum->genus      == SILVA_LEX_EOF
                    && ultimum->fons_index == fons_index)
                {
                    redde ultimum;
                }
            }
        }
    }
    redde NIHIL;
}

SilvaScriptura
silva_scribere_fontem (
                          Piscina* piscina,
            constans SilvaParsura* parsura,
    constans SilvaRegistrumCoctum* tabularium,
                              s32  fons_index)
{
    MateriaScripturaConsilium  consilium;
            MateriaOrigoUncus  uncus;
           SilvaScribereDatum  datum;
                          Xar* reinserenda = NIHIL;
                   SilvaToken* finis;
               SilvaScriptura  s;

    si (   piscina == NIHIL || parsura == NIHIL || tabularium == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde _silvae_scriptura_fracta("argumenta nulla");
    }

    /* Reinserenda: lineae directivae consumptae + rami non sumpti
     * (per fontem filtrata, per offset ordinata) */
    si (parsura->directivae != NIHIL)
    {
        i32 i;

        per (i = ZEPHYRUM; i < xar_numerus(parsura->directivae); i++)
        {
            _reinserendum_addere(&reinserenda, piscina, fons_index,
                *(Xar**)xar_obtinere(parsura->directivae, i));
        }
    }
    si (parsura->expansio != NIHIL)
    {
        _regiones_colligere(&reinserenda, piscina, fons_index,
            parsura->expansio->regiones);
    }
    /* INVOCATIONES VACUAE: expansio quae ZERO lexemata peperit
     * nullum lexema arboris relinquit quod eam monstret, ergo
     * ambulatio eam numquam invenit. Lamina eius reinserendis
     * danda est, sicut linea directivae consumpta - utraque enim
     * octetos tegit quos arbor NON fert.
     *
     * Sine hoc octeti SILENTER pereunt successu nuntiato: 'V(x)'
     * ex effusione evanescebat dum silva 'successus=1' diceret.
     * Vulnus in ipso oraculo fidelitatis, corpore non inventum
     * (nulla ex CLIV plagulis lib macrum vacuum habet) sed casu
     * adversario. */
    si (   parsura->expansio          != NIHIL
        && parsura->expansio->extenta != NIHIL)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < xar_numerus(parsura->expansio->extenta); i++)
        {
            SilvaExtentumInvocationis* ext =
                (SilvaExtentumInvocationis*)xar_obtinere(
                    parsura->expansio->extenta, i);

            si (ext != NIHIL && ext->vacua)
            {
                _reinserendum_addere(&reinserenda, piscina, fons_index,
                    ext->lamina);
            }
        }
    }
    si (reinserenda != NIHIL)
    {
        xar_ordinare(reinserenda, materia_reinserenda_comparare);
    }

    _consilium_parare(&consilium, &uncus, &datum, tabularium,
        parsura->expansio, fons_index);
    consilium.reinserenda         = reinserenda;
    consilium.reinserenda_finire  = VERUM;
    s = materia_scribere_valorem(piscina, parsura->commissio->radix,
        &consilium);

    /* EOF POST caudam reinserendorum (ordo silvae: fusio OMNIUM, dein
     * EOF). Lexema FONS est plagulae petitae - emissio sua sola. */
    finis = _lexema_finis(parsura, fons_index);
    si (s.successus && finis != NIHIL)
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            s.textus.mensura + (i32)CXXVIII);

        si (aed == NIHIL)
        {
            redde _silvae_scriptura_fracta("memoria deficit");
        }
        chorda_aedificator_appendere_chorda(aed, s.textus);
        silva_lexema_emittere_in(aed, finis);
        s.textus = chorda_aedificator_finire(aed);
    }
    redde s;
}
