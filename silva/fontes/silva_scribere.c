/* silva_scribere.c - Emissio arborum ad octetos (Phase 5 Chunk A+B)
 *
 * Ambulatio generica: loci ordine layout (generator ordinem imposuit -
 * validatio ordinis locorum), valores signati dispensant. Primitivum
 * lexematis = silva_lexema_emittere_in (UNA implementatio fidelitatis
 * cum fluxu lexatoris - scissurae, trivia verbatim).
 *
 * Chunk B - reconstructio strati 0 (simulatio ⑥):
 *   limes expansionis: radix probata (catena invocationum ad stratum
 *   0; PASTA/CHORDA/API = fractura clara), dedup per intervalla
 *   octetorum emissorum (fons_ultimus + emissum_usque), extenta
 *   functio-similium ex expansione;
 *   reinserenda: lineae directivae consumptae + laminae ramorum non
 *   sumptorum, per fontem filtratae, per offset ordinatae, ante
 *   quamque unitatem strati 0 fusae - etiam INTRA extenta (directiva
 *   intra argumenta invocationis).
 */

#include "silva_scribere.h"
#include "silva_lexema.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include <string.h>

#define SILVA_SCRIBERE_OMNIA 0x7FFFFFFF

/* ==================================================
 * Reinserendum - lamina per offset reinserenda
 * ================================================== */

nomen structura {
    s32  offset;
    Xar* lamina;    /* Xar de SilvaToken* */
} SilvaReinserendum;

/* ==================================================
 * Status scriptoris
 * ================================================== */

nomen structura {
    ChordaAedificator*             aed;
    constans SilvaRegistrumCoctum* tabularium;
    constans SilvaExpansio*        expansio;   /* NIHIL licet */
    s32                            genus_ambigui;
    i32                            locus_interpretationum;
    i32                            locus_canonicae;
    s32                            fons_index;   /* -1 = quaelibet */
    Xar*                           reinserenda;  /* ordinata; NIHIL */
    i32                            reinserenda_index;
    s32                            fons_ultimus; /* -2 = nulla adhuc */
    s32                            emissum_usque;
    b32                            successus;
    constans character*            causa;
    constans SilvaNodus*           sedes;
} SilvaScriptor;

interior vacuum _valorem_scribere (SilvaScriptor* st, SilvaValor valor);
interior vacuum _nodum_scribere (SilvaScriptor* st,
    constans SilvaNodus* nodus);

interior vacuum
_fractura (SilvaScriptor* st, constans character* causa,
    constans SilvaNodus* sedes)
{
    si (st->successus)
    {
        st->successus = FALSUM;
        st->causa = causa;
        st->sedes = sedes;
    }
}

/* Forma ambigui ex registro PER NOMEN (exemplar commissionis).
 * Reddit FALSUM si registrum formam non fert (generator eam imponit -
 * absentia = tabulae corruptae aut manu confectae). */
interior b32
_formam_ambigui_invenire (SilvaScriptor* st)
{
    i32 g;

    st->genus_ambigui = -I;
    st->locus_interpretationum = ZEPHYRUM;
    st->locus_canonicae = ZEPHYRUM;

    per (g = ZEPHYRUM; g < st->tabularium->numerus_generum; g++)
    {
        constans SilvaTabGenus* def = &st->tabularium->genera[g];

        si (def->titulus != NIHIL
            && strcmp(def->titulus, "ambiguus") == ZEPHYRUM)
        {
            i32 k;
            s32 interp = -I;
            s32 canon = -I;

            per (k = ZEPHYRUM; k < def->loci_numerus; k++)
            {
                constans SilvaTabLocus* locus =
                    &st->tabularium->loci[def->loci_offset + k];

                si (locus->titulus == NIHIL) perge;
                si (strcmp(locus->titulus, "interpretationes") == ZEPHYRUM)
                {
                    interp = (s32)k;
                }
                si (strcmp(locus->titulus, "canonica") == ZEPHYRUM)
                {
                    canon = (s32)k;
                }
            }
            si (interp < ZEPHYRUM || canon < ZEPHYRUM)
            {
                redde FALSUM;
            }
            st->genus_ambigui = (s32)g;
            st->locus_interpretationum = (i32)interp;
            st->locus_canonicae = (i32)canon;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Limes expansionis (Chunk B, sim ⑥ C4/C5)
 * ================================================== */

/* Radix strati 0 cum probatione puritatis: catenam invocationum
 * sequitur. PASTA/CHORDA (radix trans plagulam definitionis errare
 * potest) et API (fons syntheticus) = stratum 0 non recuperabile -
 * deferral nominatum (INTENTIO Phase 5). */
interior SilvaToken*
_radix_probata (SilvaToken* token, b32* impurum_out)
{
    dum (token != NIHIL)
    {
        commutatio (token->origo.genus)
        {
            casus SILVA_ORIGO_FONS:
                redde token;
            casus SILVA_ORIGO_EXPANSIO:
                token = token->origo.datum.expansio.invocatio;
                frange;
            casus SILVA_ORIGO_CHORDA:
                /* stringificatio (#x): primus = lexema primum
                 * argumenti CRUDI (use-site) - intra extentum
                 * invocationis iacet; quaestio continentiae infra
                 * extentum invenit (Chunk C - corpus solarii
                 * deferral coegit, vectis maximalista) */
                token = token->origo.datum.stringificatio.primus;
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
                si (token->origo.datum.pasta.invocatio == NIHIL)
                {
                    *impurum_out = VERUM;
                    redde token;
                }
                token = token->origo.datum.pasta.invocatio;
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
_extentum_continens (constans SilvaExpansio* expansio,
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

        si (extentum == NIHIL || extentum->lamina == NIHIL
            || xar_numerus(extentum->lamina) == ZEPHYRUM)
        {
            perge;
        }
        primum = *(SilvaToken**)xar_obtinere(extentum->lamina,
            ZEPHYRUM);
        ultimum = *(SilvaToken**)xar_obtinere(extentum->lamina,
            (i32)(xar_numerus(extentum->lamina) - I));
        si (primum->fons_index == radix->fons_index
            && radix->byte_offset >= primum->byte_offset
            && radix->byte_offset
                < ultimum->byte_offset + (s32)ultimum->longitudo)
        {
            redde extentum->lamina;
        }
    }
    redde NIHIL;
}

/* Extentum invocationis functio-similis strati 0 (scansio linearis -
 * numeri parvi; radix comparatur per identitatem monstratoris) */
interior Xar*
_extentum_quaerere (constans SilvaExpansio* expansio,
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

/* Lamina verbatim (sine fusione reinserendorum) */
interior vacuum
_laminam_emittere (SilvaScriptor* st, Xar* lamina)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(lamina); k++)
    {
        silva_lexema_emittere_in(st->aed,
            *(SilvaToken**)xar_obtinere(lamina, k));
    }
}

/* Fundere reinserenda quorum offset < ante */
interior vacuum
_reinserenda_fundere (SilvaScriptor* st, s32 ante)
{
    si (st->reinserenda == NIHIL)
    {
        redde;
    }
    dum (st->reinserenda_index < xar_numerus(st->reinserenda))
    {
        SilvaReinserendum* r = (SilvaReinserendum*)xar_obtinere(
            st->reinserenda, st->reinserenda_index);

        si (r == NIHIL || r->offset >= ante)
        {
            frange;
        }
        _laminam_emittere(st, r->lamina);
        st->reinserenda_index++;
    }
}

/* ==================================================
 * Emissio lexematis
 * ================================================== */

interior vacuum
_lexema_scribere (SilvaScriptor* st, SilvaToken* token)
{
    SilvaToken* radix;
    b32 impurum;

    si (!st->successus || token == NIHIL)
    {
        redde;
    }

    si (token->origo.genus == SILVA_ORIGO_FONS)
    {
        si (st->fons_index >= ZEPHYRUM
            && token->fons_index != st->fons_index)
        {
            /* plagulae alienae (limes includendi): octeti eius
             * reconstructioni SUAE pertinent; linea #include hic
             * reinseritur */
            redde;
        }
        _reinserenda_fundere(st, token->byte_offset);
        silva_lexema_emittere_in(st->aed, token);
        st->fons_ultimus = token->fons_index;
        st->emissum_usque = token->byte_offset + (s32)token->longitudo;
        redde;
    }

    /* Limes expansionis: stratum 0 emittitur, non lexema expansum */
    si (st->expansio == NIHIL)
    {
        _fractura(st, "lexema expansum sine contextu expansionis",
            NIHIL);
        redde;
    }
    impurum = FALSUM;
    radix = _radix_probata(token, &impurum);
    si (impurum || radix == NIHIL)
    {
        _fractura(st, "origo pasta/chorda/api - stratum 0 non "
            "recuperabile (deferral nominatum)", NIHIL);
        redde;
    }
    si (st->fons_index >= ZEPHYRUM
        && radix->fons_index != st->fons_index)
    {
        redde;  /* expansio in plagula aliena */
    }

    /* intra unitatem iam emissam? (corpus multi-lexematis eiusdem
     * invocationis; lexemata ex argumentis intra extentum) */
    si (radix->fons_index == st->fons_ultimus
        && radix->byte_offset < st->emissum_usque)
    {
        redde;
    }

    {
        Xar* extentum = _extentum_quaerere(st->expansio, radix);

        si (extentum == NIHIL)
        {
            /* radix intra invocationem (lexema argumenti - via
             * CHORDA/stringificatio): extentum per continentiam */
            extentum = _extentum_continens(st->expansio, radix);
        }
        si (extentum != NIHIL && xar_numerus(extentum) > ZEPHYRUM)
        {
            /* invocatio functio-similis: [nomen..')'] lexematim -
             * fusio reinserendorum INTRA extentum manet possibilis
             * (directiva intra argumenta) */
            SilvaToken* t = NIHIL;
            i32 k;

            per (k = ZEPHYRUM; k < xar_numerus(extentum); k++)
            {
                t = *(SilvaToken**)xar_obtinere(extentum, k);
                _reinserenda_fundere(st, t->byte_offset);
                silva_lexema_emittere_in(st->aed, t);
            }
            st->fons_ultimus = radix->fons_index;
            st->emissum_usque = t->byte_offset + (s32)t->longitudo;
        }
        alioquin
        {
            /* obiectum-simile: lexema invocationis solum */
            _reinserenda_fundere(st, radix->byte_offset);
            silva_lexema_emittere_in(st->aed, radix);
            st->fons_ultimus = radix->fons_index;
            st->emissum_usque = radix->byte_offset
                + (s32)radix->longitudo;
        }
    }
}

/* ==================================================
 * Ambulatio
 * ================================================== */

interior vacuum
_ambiguum_scribere (SilvaScriptor* st, constans SilvaNodus* nodus)
{
    SilvaValor interp;
    SilvaValor canonica;
    SilvaValor* electa;

    si ((i32)st->locus_interpretationum >= nodus->numerus_locorum
        || (i32)st->locus_canonicae >= nodus->numerus_locorum)
    {
        _fractura(st, "forma ambigui extra loci nodi", nodus);
        redde;
    }
    interp = nodus->loci[st->locus_interpretationum];
    canonica = nodus->loci[st->locus_canonicae];

    si (interp.genus != SILVA_VALOR_LISTA
        || canonica.genus != SILVA_VALOR_INDEX)
    {
        _fractura(st, "forma ambigui corrupta", nodus);
        redde;
    }
    si (canonica.datum.index < ZEPHYRUM
        || canonica.datum.index
            >= (s32)silva_valor_lista_numerus(interp))
    {
        _fractura(st, "canonica extra fines interpretationum", nodus);
        redde;
    }
    electa = silva_valor_lista_obtinere(interp,
        (i32)canonica.datum.index);
    si (electa == NIHIL)
    {
        _fractura(st, "interpretatio canonica deest", nodus);
        redde;
    }
    _valorem_scribere(st, *electa);
}

interior vacuum
_nodum_scribere (SilvaScriptor* st, constans SilvaNodus* nodus)
{
    i32 k;

    si (!st->successus || nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus < ZEPHYRUM
        || nodus->genus >= (s32)st->tabularium->numerus_generum)
    {
        _fractura(st, "genus ignotum", nodus);
        redde;
    }

    /* AMBIGUUS: sola interpretatio canonica - emissio omnium
     * interpretationum octetos duplicaret (lexemata communia) */
    si (nodus->genus == st->genus_ambigui)
    {
        _ambiguum_scribere(st, nodus);
        redde;
    }

    /* Genericus: loci ordine layout. ERROR gratis (locus unicus
     * lista-token); CONDITIONALIS cum Phase 7 veniet. */
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        _valorem_scribere(st, nodus->loci[k]);
    }
}

interior vacuum
_valorem_scribere (SilvaScriptor* st, SilvaValor valor)
{
    si (!st->successus)
    {
        redde;
    }
    commutatio (valor.genus)
    {
        casus SILVA_VALOR_NIHIL:
        casus SILVA_VALOR_INDEX:
            frange;
        casus SILVA_VALOR_TOKEN:
            _lexema_scribere(st, valor.datum.token);
            frange;
        casus SILVA_VALOR_NODUS:
            _nodum_scribere(st, valor.datum.nodus);
            frange;
        casus SILVA_VALOR_LISTA:
        {
            i32 i;
            i32 n = silva_valor_lista_numerus(valor);

            per (i = ZEPHYRUM; i < n; i++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

                si (elem != NIHIL)
                {
                    _valorem_scribere(st, *elem);
                }
            }
            frange;
        }
        ordinarius:
            _fractura(st, "signum valoris ignotum", NIHIL);
            frange;
    }
}

/* ==================================================
 * Reinserenda colligere (fontem)
 * ================================================== */

interior vacuum
_reinserendum_addere (SilvaScriptor* st, Piscina* piscina, Xar* lamina)
{
    SilvaToken* primum;
    SilvaReinserendum* r;

    si (lamina == NIHIL || xar_numerus(lamina) == ZEPHYRUM)
    {
        redde;
    }
    primum = *(SilvaToken**)xar_obtinere(lamina, ZEPHYRUM);
    si (st->fons_index >= ZEPHYRUM
        && primum->fons_index != st->fons_index)
    {
        redde;
    }
    si (st->reinserenda == NIHIL)
    {
        st->reinserenda = xar_creare(piscina,
            magnitudo(SilvaReinserendum));
    }
    r = (SilvaReinserendum*)xar_addere(st->reinserenda);
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
_regiones_colligere (SilvaScriptor* st, Piscina* piscina, Xar* regiones)
{
    i32 i;

    si (regiones == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(regiones); i++)
    {
        SilvaRegio* regio = *(SilvaRegio**)xar_obtinere(regiones, i);
        i32 j;

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
                    _reinserendum_addere(st, piscina, ramus->directiva);
                }
                si (ramus->lexemata_cruda != NIHIL)
                {
                    _reinserendum_addere(st, piscina,
                        ramus->lexemata_cruda);
                }
            }
        }
        si (!regio->est_texta && regio->directiva_finis != NIHIL)
        {
            _reinserendum_addere(st, piscina, regio->directiva_finis);
        }
        _regiones_colligere(st, piscina, regio->filiae);
    }
}

interior s32
_reinserenda_comparare (constans vacuum* a, constans vacuum* b)
{
    constans SilvaReinserendum* ra = (constans SilvaReinserendum*)a;
    constans SilvaReinserendum* rb = (constans SilvaReinserendum*)b;

    si (ra->offset < rb->offset) redde -I;
    si (ra->offset > rb->offset) redde I;
    redde ZEPHYRUM;
}

/* ==================================================
 * Compositio fructus
 * ================================================== */

interior SilvaScriptura
_scriptura_fracta (constans character* causa)
{
    SilvaScriptura s;

    s.successus = FALSUM;
    s.textus.datum = NIHIL;
    s.textus.mensura = ZEPHYRUM;
    s.causa = causa;
    s.sedes = NIHIL;
    redde s;
}

interior b32
_scriptor_parare (SilvaScriptor* st, Piscina* piscina,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaExpansio* expansio, s32 fons_index)
{
    st->aed = chorda_aedificator_creare(piscina, 1024);
    st->tabularium = tabularium;
    st->expansio = expansio;
    st->fons_index = fons_index;
    st->reinserenda = NIHIL;
    st->reinserenda_index = ZEPHYRUM;
    st->fons_ultimus = -II;
    st->emissum_usque = ZEPHYRUM;
    st->successus = VERUM;
    st->causa = NIHIL;
    st->sedes = NIHIL;
    si (st->aed == NIHIL)
    {
        redde FALSUM;
    }
    redde _formam_ambigui_invenire(st);
}

interior SilvaScriptura
_scriptura_finire (SilvaScriptor* st)
{
    SilvaScriptura s;

    s.successus = st->successus;
    s.causa = st->causa;
    s.sedes = st->sedes;
    si (st->successus)
    {
        s.textus = chorda_aedificator_finire(st->aed);
    }
    alioquin
    {
        s.textus.datum = NIHIL;
        s.textus.mensura = ZEPHYRUM;
    }
    redde s;
}

/* ==================================================
 * API
 * ================================================== */

SilvaScriptura
silva_scribere_valorem (
    Piscina*                       piscina,
    SilvaValor                     valor,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaExpansio*        expansio)
{
    SilvaScriptor st;

    si (piscina == NIHIL || tabularium == NIHIL)
    {
        redde _scriptura_fracta("argumenta nulla");
    }
    si (!_scriptor_parare(&st, piscina, tabularium, expansio, -I))
    {
        redde _scriptura_fracta("tabularium sine forma ambigui");
    }
    _valorem_scribere(&st, valor);
    redde _scriptura_finire(&st);
}

SilvaScriptura
silva_scribere_nodum (
    Piscina*                       piscina,
    constans SilvaNodus*           nodus,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaExpansio*        expansio)
{
    SilvaScriptor st;

    si (piscina == NIHIL || nodus == NIHIL || tabularium == NIHIL)
    {
        redde _scriptura_fracta("argumenta nulla");
    }
    si (!_scriptor_parare(&st, piscina, tabularium, expansio, -I))
    {
        redde _scriptura_fracta("tabularium sine forma ambigui");
    }
    _nodum_scribere(&st, nodus);
    redde _scriptura_finire(&st);
}

SilvaScriptura
silva_scribere_fontem (
    Piscina*                       piscina,
    constans SilvaParsura*         parsura,
    constans SilvaRegistrumCoctum* tabularium,
    s32                            fons_index)
{
    SilvaScriptor st;

    si (piscina == NIHIL || parsura == NIHIL || tabularium == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde _scriptura_fracta("argumenta nulla");
    }
    si (!_scriptor_parare(&st, piscina, tabularium, parsura->expansio,
            fons_index))
    {
        redde _scriptura_fracta("tabularium sine forma ambigui");
    }

    /* Reinserenda: lineae directivae consumptae + rami non sumpti
     * (per fontem filtrata, per offset ordinata) */
    si (parsura->directivae != NIHIL)
    {
        i32 i;

        per (i = ZEPHYRUM; i < xar_numerus(parsura->directivae); i++)
        {
            _reinserendum_addere(&st, piscina,
                *(Xar**)xar_obtinere(parsura->directivae, i));
        }
    }
    si (parsura->expansio != NIHIL)
    {
        _regiones_colligere(&st, piscina, parsura->expansio->regiones);
    }
    si (st.reinserenda != NIHIL)
    {
        xar_ordinare(st.reinserenda, _reinserenda_comparare);
    }

    _valorem_scribere(&st, parsura->commissio->radix);
    _reinserenda_fundere(&st, SILVA_SCRIBERE_OMNIA);

    /* Trivia caudae plagulae: lexema EOF. Plagulae inclusae EOF suum
     * in includenda retinent (fluxus reliquorum id abicit - sim ⑥
     * C6). */
    si (parsura->lexema_finis != NIHIL
        && (fons_index < ZEPHYRUM
            || parsura->lexema_finis->fons_index == fons_index))
    {
        _lexema_scribere(&st, parsura->lexema_finis);
    }
    alioquin si (fons_index >= ZEPHYRUM && parsura->expansio != NIHIL)
    {
        SilvaFons* fons = (SilvaFons*)xar_obtinere(
            parsura->expansio->fontes, (i32)fons_index);
        vacuum* valor;

        si (fons != NIHIL && fons->via != NIHIL
            && tabula_dispersa_invenire(parsura->expansio->includenda,
                   *fons->via, &valor))
        {
            SilvaIncludendum* incl = (SilvaIncludendum*)valor;

            si (incl != NIHIL && incl->lexemata != NIHIL
                && xar_numerus(incl->lexemata) > ZEPHYRUM)
            {
                SilvaToken* ultimum = *(SilvaToken**)xar_obtinere(
                    incl->lexemata,
                    (i32)(xar_numerus(incl->lexemata) - I));

                si (ultimum != NIHIL
                    && ultimum->genus == SILVA_LEX_EOF)
                {
                    _lexema_scribere(&st, ultimum);
                }
            }
        }
    }

    redde _scriptura_finire(&st);
}
