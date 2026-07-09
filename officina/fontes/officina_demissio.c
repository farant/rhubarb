/* officina_demissio.c - Demissio: arbor typata -> medulla (M1b A)
 * Vide .h pro contractu; officina/phase-log.md M1b INTENTIO pro
 * consilio. Chunk A: fistula + expressiones + sententiae; vocatio/
 * accessus/subscriptio -> B, data statica/congeries/chordae -> C
 * (sistere causis nominatis - classificatio est vectis). */

#include "officina_demissio.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================================================
 * Structurae
 * ================================================== */

nomen structura {
    i8                           clavis_octeti[magnitudo(vacuum*)];
    vacuum*                      valor;
} DemissioIntroitus;

nomen structura {
    constans SemanticaSymbolum* symbolum;
    s32                         index_registri;  /* valor AUT
                                                  * inscriptio arcae */
    b32                         est_arca;
} DemissioSedes;

nomen structura {
    b32 directum;               /* VERUM: index = registrum valoris */
    s32 index;                  /* aliter registrum inscriptionis */
    b32 valida;
} DemissioLocus;

nomen structura {
    Piscina*                piscina;
    SilvaSemantica*         sem;
    MedullaModulus*         modulus;
    MedullaFunctio*         functio;
    s32                     bloccus;
    s32                     numerator;    /* nomina bloccorum */
    s32                     frange_finis; /* -I extra */
    s32                     perge_finis;
    TabulaDispersa*         lexemata;     /* SilvaToken* -> symbolum */
    TabulaDispersa*         sedes;        /* symbolum -> DemissioSedes* */
    TabulaDispersa*         capti;        /* symbola inscriptione capta */
} Demissio;

/* prototypa mutuae recursionis */
interior s32 _expressionem (Demissio* d, constans SilvaNodus* nodus);
interior MedullaOperandum _ut_operandum (Demissio* d,
    constans SilvaNodus* nodus);
interior s32 _ut_valor (Demissio* d, constans SilvaNodus* nodus);
interior DemissioLocus _ut_locum (Demissio* d,
    constans SilvaNodus* nodus);
interior vacuum _sententiam (Demissio* d, constans SilvaNodus* nodus);

/* ==================================================
 * Auxilia parva
 * ================================================== */

interior chorda
_ch_de_silva (SilvaChorda s)
{
    chorda c;

    c.datum = s.datum;
    c.mensura = s.mensura;
    redde c;
}

interior chorda
_ch_literis (constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    redde c;
}

interior vacuum*
_sine_constante (constans vacuum* monstrator)
{
    unio { constans vacuum* c; vacuum* m; } u;

    u.c = monstrator;
    redde u.m;
}

interior constans SilvaNodus*
_canonicus (constans SilvaNodus* nodus)
{
    dum (nodus != NIHIL
         && nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS)
    {
        SilvaValor v = silva_c89_ambiguus_canonica(nodus);

        si (v.genus != SILVA_VALOR_NODUS)
        {
            frange;
        }
        nodus = v.datum.nodus;
    }
    redde nodus;
}

interior SilvaChorda
_tok_textus (SilvaValor v)
{
    SilvaChorda vacua;

    si (v.genus == SILVA_VALOR_TOKEN && v.datum.token != NIHIL)
    {
        redde v.datum.token->valor;
    }
    vacua.mensura = 0;
    vacua.datum = NIHIL;
    redde vacua;
}

interior b32
_op_est (SilvaChorda textus, constans character* literis)
{
    i32 longitudo = (i32)strlen(literis);

    redde textus.mensura == longitudo
        && memcmp(textus.datum, literis,
               (memoriae_index)longitudo) == ZEPHYRUM;
}

/* ==================================================
 * Tabulae monstratorum (exemplar typationis M0b)
 * ================================================== */

interior vacuum*
_tabulam_invenire (TabulaDispersa* tabula,
    constans vacuum* monstrator)
{
    i8 octeti[magnitudo(vacuum*)];
    chorda clavis;
    vacuum* valor = NIHIL;

    memcpy(octeti, &monstrator, magnitudo(octeti));
    clavis.datum = octeti;
    clavis.mensura = (i32)magnitudo(octeti);
    si (tabula_dispersa_invenire(tabula, clavis, &valor))
    {
        redde ((DemissioIntroitus*)valor)->valor;
    }
    redde NIHIL;
}

interior vacuum
_tabulam_ponere (Demissio* d, TabulaDispersa* tabula,
    constans vacuum* monstrator, vacuum* valor)
{
    DemissioIntroitus* introitus;
    chorda clavis;
    i8 octeti[magnitudo(vacuum*)];
    vacuum* prior = NIHIL;

    memcpy(octeti, &monstrator, magnitudo(octeti));
    clavis.datum = octeti;
    clavis.mensura = (i32)magnitudo(octeti);
    si (tabula_dispersa_invenire(tabula, clavis, &prior))
    {
        ((DemissioIntroitus*)prior)->valor = valor;
        redde;
    }
    introitus = (DemissioIntroitus*)piscina_allocare(d->piscina,
        (memoriae_index)magnitudo(DemissioIntroitus));
    si (introitus == NIHIL)
    {
        redde;
    }
    memcpy(introitus->clavis_octeti, &monstrator,
        magnitudo(introitus->clavis_octeti));
    introitus->valor = valor;
    clavis.datum = introitus->clavis_octeti;
    clavis.mensura = (i32)magnitudo(introitus->clavis_octeti);
    (vacuum)tabula_dispersa_inserere(tabula, clavis,
        (vacuum*)introitus);
}

/* ==================================================
 * Typi
 * ================================================== */

interior constans TypusC89*
_exutus (constans TypusC89* typus)
{
    dum (typus != NIHIL && typus->genus == TYPUS_C89_QUALIFICATUS)
    {
        typus = typus->datum.qualificatus.internum;
    }
    redde typus;
}

/* MEDULLA_TYPUS_* aut -I (non scalaris) */
interior s32
_typus_medullae (constans TypusC89* typus)
{
    typus = _exutus(typus);
    si (typus == NIHIL)
    {
        redde -I;
    }
    commutatio (typus->genus)
    {
        casus TYPUS_C89_PRIMITIVUS:
        {
            commutatio (typus->datum.primitivum)
            {
                casus PRIMITIVUM_CHARACTER:           redde MEDULLA_TYPUS_S8;
                casus PRIMITIVUM_CHARACTER_SIGNATUM:  redde MEDULLA_TYPUS_S8;
                casus PRIMITIVUM_CHARACTER_INSIGNATUM: redde MEDULLA_TYPUS_I8;
                casus PRIMITIVUM_BREVIS:              redde MEDULLA_TYPUS_S16;
                casus PRIMITIVUM_BREVIS_INSIGNATUM:   redde MEDULLA_TYPUS_I16;
                casus PRIMITIVUM_INTEGER:             redde MEDULLA_TYPUS_S32;
                casus PRIMITIVUM_INTEGER_INSIGNATUM:  redde MEDULLA_TYPUS_I32;
                casus PRIMITIVUM_LONGUS:              redde MEDULLA_TYPUS_S64;
                casus PRIMITIVUM_LONGUS_INSIGNATUM:   redde MEDULLA_TYPUS_I64;
                casus PRIMITIVUM_LONGUS_LONGUS:       redde MEDULLA_TYPUS_S64;
                casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
                    redde MEDULLA_TYPUS_I64;
                casus PRIMITIVUM_FLUITANS:            redde MEDULLA_TYPUS_F32;
                casus PRIMITIVUM_DUPLEX:              redde MEDULLA_TYPUS_F64;
                casus PRIMITIVUM_DUPLEX_LONGUS:       redde MEDULLA_TYPUS_F64;
                ordinarius:                           redde -I;   /* vacuum */
            }
        }
        casus TYPUS_C89_MONSTRATOR:  redde MEDULLA_TYPUS_I64;
        casus TYPUS_C89_ACIES:       redde MEDULLA_TYPUS_I64;
        casus TYPUS_C89_FUNCTIO:     redde MEDULLA_TYPUS_I64;
        casus TYPUS_C89_ENUMERATUS:  redde MEDULLA_TYPUS_S32;
        ordinarius:                  redde -I;
    }
}

interior constans TypusC89*
_typus_finalis (constans Demissio* d, constans SilvaNodus* nodus)
{
    constans TypusC89* conversus = silva_c89_conversio_expressionis(
        d->sem, nodus);

    si (conversus != NIHIL)
    {
        redde conversus;
    }
    redde silva_c89_typus_expressionis(d->sem, nodus);
}

/* magnitudo per formam; -I si forma invalida */
interior s32
_mensura_typi (Demissio* d, constans TypusC89* typus)
{
    TypusC89* mutabilis = (TypusC89*)_sine_constante(
        (constans vacuum*)typus);

    si (typus == NIHIL
        || !silva_c89_formam_computare(d->sem, mutabilis))
    {
        redde -I;
    }
    redde (s32)typus->magnitudo_octetorum;
}

/* ==================================================
 * Nomina + blocci + registra
 * ================================================== */

interior chorda
_titulum_fingere (Demissio* d, constans character* basis)
{
    character littera[XLVIII];
    chorda temporarium;
    s32 longitudo = (s32)sprintf(littera, "%s_%d", basis,
        (int)d->numerator);

    d->numerator++;
    temporarium.datum = (i8*)littera;
    temporarium.mensura = (i32)longitudo;
    redde chorda_transcribere(temporarium, d->piscina);
}

interior s32
_bloccum_novum (Demissio* d, constans character* basis)
{
    redde medulla_bloccum_creare(d->functio,
        _titulum_fingere(d, basis));
}

interior b32
_bloccus_vivus (constans Demissio* d)
{
    constans MedullaBloccus* bloccus = medulla_bloccum_obtinere(
        d->functio, d->bloccus);

    redde bloccus != NIHIL && !bloccus->terminatus;
}

interior vacuum
_vivum (Demissio* d)
{
    si (!_bloccus_vivus(d))
    {
        d->bloccus = _bloccum_novum(d, "insequens");
    }
}

interior b32
_registrum_captum (constans Demissio* d, chorda titulus)
{
    i32 i;
    i32 m = xar_numerus(d->functio->registra);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans chorda* sedes = (constans chorda*)xar_obtinere(
            d->functio->registra, i);

        si (sedes->mensura == titulus.mensura
            && titulus.mensura > ZEPHYRUM
            && memcmp(sedes->datum, titulus.datum,
                   (memoriae_index)titulus.mensura) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior s32
_registrum_unicum (Demissio* d, SilvaChorda titulus_silva)
{
    chorda titulus = _ch_de_silva(titulus_silva);

    si (titulus.mensura == ZEPHYRUM
        || !_registrum_captum(d, titulus))
    {
        redde medulla_registrum_novum(d->functio, titulus);
    }
    {
        character littera[LXIV];
        chorda alterum;
        i32 scriptum = ZEPHYRUM;
        i32 caput = (titulus.mensura < XL) ? titulus.mensura : XL;
        s32 n;

        memcpy(littera, titulus.datum, (memoriae_index)caput);
        scriptum = caput;
        n = (s32)sprintf(littera + scriptum, "_%d",
            (int)d->numerator);
        d->numerator++;
        alterum.datum = (i8*)littera;
        alterum.mensura = scriptum + (i32)n;
        redde medulla_registrum_novum(d->functio,
            chorda_transcribere(alterum, d->piscina));
    }
}

interior s32
_registrum_temporarium (Demissio* d)
{
    chorda vacua;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    redde medulla_registrum_novum(d->functio, vacua);
}

/* ==================================================
 * Emissio
 * ================================================== */

interior s32
_em (Demissio* d, constans SilvaNodus* origo, s32 op, s32 typus,
    s32 typus_secundus, s32 destinatio, MedullaOperandum a,
    MedullaOperandum b, MedullaOperandum c)
{
    MedullaInstructio instructio;

    _vivum(d);
    memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
    instructio.op = op;
    instructio.typus = typus;
    instructio.typus_secundus = typus_secundus;
    instructio.destinatio = destinatio;
    instructio.extra_index = -I;
    instructio.a = a;
    instructio.b = b;
    instructio.c = c;
    instructio.origo = origo;
    (vacuum)medulla_emittere(d->functio, d->bloccus, &instructio);
    redde destinatio;
}

interior vacuum
_sistere (Demissio* d, constans SilvaNodus* origo,
    constans character* causa)
{
    s32 index = medulla_causam_internare(d->modulus,
        _ch_literis(causa));

    (vacuum)_em(d, origo, MEDULLA_OP_SISTERE, MEDULLA_TYPUS_NIHIL,
        MEDULLA_TYPUS_NIHIL, -I, medulla_op_causa(index),
        medulla_op_nihil(), medulla_op_nihil());
}

interior vacuum
_salire_si_vivus (Demissio* d, s32 finis)
{
    si (_bloccus_vivus(d))
    {
        (vacuum)_em(d, NIHIL, MEDULLA_OP_SALIRE, MEDULLA_TYPUS_NIHIL,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_bloccum(finis),
            medulla_op_nihil(), medulla_op_nihil());
    }
}

interior vacuum
_ramus (Demissio* d, constans SilvaNodus* origo, MedullaOperandum
    conditio, s32 verum, s32 falsum)
{
    (vacuum)_em(d, origo, MEDULLA_OP_RAMUS, MEDULLA_TYPUS_NIHIL,
        MEDULLA_TYPUS_NIHIL, -I, conditio,
        medulla_op_bloccum(verum), medulla_op_bloccum(falsum));
}

/* movere operandi in registrum novum (typus datur) */
interior s32
_movere (Demissio* d, constans SilvaNodus* origo, s32 typus,
    s32 destinatio, MedullaOperandum a)
{
    redde _em(d, origo, MEDULLA_OP_MOVERE,
        (typus >= ZEPHYRUM) ? typus : MEDULLA_TYPUS_S64,
        MEDULLA_TYPUS_NIHIL, destinatio, a, medulla_op_nihil(),
        medulla_op_nihil());
}

/* mensurae typorum medullae (octeti) */
interior s32
_octeti_typi_m (s32 typus)
{
    commutatio (typus)
    {
        casus MEDULLA_TYPUS_I8:  redde I;
        casus MEDULLA_TYPUS_S8:  redde I;
        casus MEDULLA_TYPUS_I16: redde II;
        casus MEDULLA_TYPUS_S16: redde II;
        casus MEDULLA_TYPUS_I32: redde IV;
        casus MEDULLA_TYPUS_S32: redde IV;
        casus MEDULLA_TYPUS_F32: redde IV;
        ordinarius:              redde VIII;
    }
}

interior b32
_fluitans_m (s32 typus)
{
    redde typus == (s32)MEDULLA_TYPUS_F32
        || typus == (s32)MEDULLA_TYPUS_F64;
}

/* conversio inter typos C annotatos - familia operationum
 * nominatarum; idem aut solum-signum = registrum immotum
 * (typi in operationibus, non in registris) */
interior s32
_conversionem (Demissio* d, constans SilvaNodus* origo, s32
    registrum_valoris, constans TypusC89* de, constans TypusC89* ad)
{
    s32 mt_de = _typus_medullae(de);
    s32 mt_ad = _typus_medullae(ad);
    s32 op;

    si (mt_de < ZEPHYRUM || mt_ad < ZEPHYRUM)
    {
        redde registrum_valoris;   /* vacuum/aggregatum: immotum */
    }
    si (mt_de == mt_ad)
    {
        redde registrum_valoris;
    }
    si (_fluitans_m(mt_de) && _fluitans_m(mt_ad))
    {
        op = (mt_de == (s32)MEDULLA_TYPUS_F32)
            ? (s32)MEDULLA_OP_DILATARE : (s32)MEDULLA_OP_ARTARE;
    }
    alioquin si (_fluitans_m(mt_de))
    {
        op = MEDULLA_OP_INTEGRARE;
    }
    alioquin si (_fluitans_m(mt_ad))
    {
        op = MEDULLA_OP_FLUITARE;
    }
    alioquin
    {
        s32 o_de = _octeti_typi_m(mt_de);
        s32 o_ad = _octeti_typi_m(mt_ad);

        si (o_de == o_ad)
        {
            redde registrum_valoris;   /* signum solum: octeti idem */
        }
        op = (o_ad > o_de) ? (s32)MEDULLA_OP_EXTENDERE
                           : (s32)MEDULLA_OP_AMPUTARE;
    }
    redde _em(d, origo, op, mt_de, mt_ad, _registrum_temporarium(d),
        medulla_op_registrum(registrum_valoris), medulla_op_nihil(),
        medulla_op_nihil());
}

/* ==================================================
 * Sedes symbolorum
 * ================================================== */

interior DemissioSedes*
_sedem_symboli (constans Demissio* d,
    constans SemanticaSymbolum* symbolum)
{
    redde (DemissioSedes*)_tabulam_invenire(d->sedes,
        (constans vacuum*)symbolum);
}

interior DemissioSedes*
_sedem_creare (Demissio* d, constans SemanticaSymbolum* symbolum,
    s32 index_registri, b32 est_arca)
{
    DemissioSedes* sedes = (DemissioSedes*)piscina_allocare(
        d->piscina, (memoriae_index)magnitudo(DemissioSedes));

    si (sedes == NIHIL)
    {
        redde NIHIL;
    }
    sedes->symbolum = symbolum;
    sedes->index_registri = index_registri;
    sedes->est_arca = est_arca;
    _tabulam_ponere(d, d->sedes, (constans vacuum*)symbolum,
        (vacuum*)sedes);
    redde sedes;
}

/* ==================================================
 * Expressiones
 * ================================================== */

/* valor naturalis identificatoris (ante conversionem nodi) */
interior s32
_identificatorem (Demissio* d, constans SilvaNodus* nodus)
{
    constans SemanticaSymbolum* symbolum = silva_c89_symbolum_nodi(
        d->sem, nodus);
    constans TypusC89* naturalis = silva_c89_typus_expressionis(
        d->sem, nodus);
    DemissioSedes* sedes;

    si (symbolum == NIHIL)
    {
        _sistere(d, nodus, "identificator sine symbolo");
        redde _registrum_temporarium(d);
    }
    si (symbolum->genus == (int)SYMBOLUM_CONSTANS)
    {
        redde _movere(d, nodus, MEDULLA_TYPUS_S32,
            _registrum_temporarium(d),
            medulla_op_immediatum((s64)symbolum->valor));
    }
    sedes = _sedem_symboli(d, symbolum);
    si (sedes != NIHIL)
    {
        si (!sedes->est_arca)
        {
            redde sedes->index_registri;
        }
        /* arca: inscriptio; onus solum si typus scalaris */
        {
            s32 mt = _typus_medullae(naturalis);

            si (mt >= ZEPHYRUM
                && _exutus(naturalis)->genus != TYPUS_C89_ACIES)
            {
                redde _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                    MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                    medulla_op_registrum(sedes->index_registri),
                    medulla_op_nihil(), medulla_op_nihil());
            }
            redde sedes->index_registri;   /* inscriptio (acies/
                                            * aggregatum) */
        }
    }
    /* globale aut functio: locus $symbolum */
    si (symbolum->profunditas > ZEPHYRUM
        && (symbolum->repositio & REPOSITIO_STATICA) != ZEPHYRUM)
    {
        _sistere(d, nodus, "nondum staticum locale (M1b C)");
        redde _registrum_temporarium(d);
    }
    {
        s32 index_symboli = medulla_symbolum_internare(d->modulus,
            _ch_de_silva(symbolum->titulus));
        s32 inscriptio = _em(d, nodus, MEDULLA_OP_LOCUS,
            MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d),
            medulla_op_symbolum(index_symboli), medulla_op_nihil(),
            medulla_op_nihil());
        constans TypusC89* exutus = _exutus(naturalis);
        s32 mt = _typus_medullae(naturalis);

        si (mt >= ZEPHYRUM && exutus != NIHIL
            && exutus->genus != TYPUS_C89_ACIES
            && exutus->genus != TYPUS_C89_FUNCTIO)
        {
            redde _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(inscriptio),
                medulla_op_nihil(), medulla_op_nihil());
        }
        redde inscriptio;
    }
}

/* index (int) -> inscriptio scalata (i64): extendere + multiplicare */
interior s32
_indicem_scalare (Demissio* d, constans SilvaNodus* origo,
    MedullaOperandum index_op, s32 mt_indicis, s32 mensura_elementi)
{
    s32 registrum_indicis;
    s32 latus;

    si (index_op.genus == (s32)MEDULLA_OPERANDUM_IMMEDIATUM)
    {
        /* scalatio in tempore compilationis */
        redde _movere(d, origo, MEDULLA_TYPUS_I64,
            _registrum_temporarium(d),
            medulla_op_immediatum(index_op.datum.immediatum
                * (s64)mensura_elementi));
    }
    registrum_indicis = (index_op.genus
        == (s32)MEDULLA_OPERANDUM_REGISTRUM)
        ? index_op.datum.index
        : _movere(d, origo, mt_indicis, _registrum_temporarium(d),
              index_op);
    latus = registrum_indicis;
    si (_octeti_typi_m(mt_indicis) < VIII)
    {
        latus = _em(d, origo, MEDULLA_OP_EXTENDERE, mt_indicis,
            (mt_indicis == (s32)MEDULLA_TYPUS_I32)
                ? (s32)MEDULLA_TYPUS_I64 : (s32)MEDULLA_TYPUS_S64,
            _registrum_temporarium(d),
            medulla_op_registrum(registrum_indicis),
            medulla_op_nihil(), medulla_op_nihil());
    }
    redde _em(d, origo, MEDULLA_OP_MULTIPLICARE, MEDULLA_TYPUS_I64,
        MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
        medulla_op_registrum(latus),
        medulla_op_immediatum((s64)mensura_elementi),
        medulla_op_nihil());
}

interior b32
_est_monstratorius (constans TypusC89* typus)
{
    constans TypusC89* exutus = _exutus(typus);

    redde exutus != NIHIL
        && (exutus->genus == TYPUS_C89_MONSTRATOR
            || exutus->genus == TYPUS_C89_ACIES);
}

interior s32
_mensura_elementi_monstratoris (Demissio* d,
    constans TypusC89* typus)
{
    constans TypusC89* exutus = _exutus(typus);
    constans TypusC89* elementum = NIHIL;

    si (exutus == NIHIL)
    {
        redde -I;
    }
    si (exutus->genus == TYPUS_C89_MONSTRATOR)
    {
        elementum = exutus->datum.monstrator.internum;
    }
    alioquin si (exutus->genus == TYPUS_C89_ACIES)
    {
        elementum = exutus->datum.acies.elementum;
    }
    redde _mensura_typi(d, elementum);
}

/* && / || : materializatio ingenua per bloccos (0/1) */
interior s32
_brevem (Demissio* d, constans SilvaNodus* nodus, b32 est_et)
{
    SilvaValor sin_v = silva_c89_binarium_sinister(nodus);
    SilvaValor dex_v = silva_c89_binarium_dexter(nodus);
    s32 fructus = _registrum_temporarium(d);
    s32 b_alter = _bloccum_novum(d, est_et ? "et_alter" : "vel_alter");
    s32 b_muta  = _bloccum_novum(d, est_et ? "et_verum" : "vel_zephyrum");
    s32 b_finis = _bloccum_novum(d, est_et ? "et_finis" : "vel_finis");
    s32 primus;

    (vacuum)_movere(d, nodus, MEDULLA_TYPUS_S32, fructus,
        medulla_op_immediatum(est_et ? 0 : 1));
    primus = _ut_valor(d, sin_v.datum.nodus);
    si (est_et)
    {
        _ramus(d, nodus, medulla_op_registrum(primus), b_alter,
            b_finis);
    }
    alioquin
    {
        _ramus(d, nodus, medulla_op_registrum(primus), b_finis,
            b_alter);
    }
    d->bloccus = b_alter;
    {
        s32 secundus = _ut_valor(d, dex_v.datum.nodus);

        si (est_et)
        {
            _ramus(d, nodus, medulla_op_registrum(secundus), b_muta,
                b_finis);
        }
        alioquin
        {
            _ramus(d, nodus, medulla_op_registrum(secundus), b_finis,
                b_muta);
        }
    }
    d->bloccus = b_muta;
    (vacuum)_movere(d, nodus, MEDULLA_TYPUS_S32, fructus,
        medulla_op_immediatum(est_et ? 1 : 0));
    _salire_si_vivus(d, b_finis);
    d->bloccus = b_finis;
    redde fructus;
}

interior s32
_binarium (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor sin_v = silva_c89_binarium_sinister(nodus);
    SilvaValor dex_v = silva_c89_binarium_dexter(nodus);
    SilvaChorda operator = _tok_textus(
        silva_c89_binarium_tok_operator(nodus));
    constans TypusC89* fructus_t = silva_c89_typus_expressionis(
        d->sem, nodus);
    s32 op = -I;
    b32 comparatio = FALSUM;

    si (sin_v.genus != SILVA_VALOR_NODUS
        || dex_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "binarium mancum");
        redde _registrum_temporarium(d);
    }
    si (_op_est(operator, "&&"))
    {
        redde _brevem(d, nodus, VERUM);
    }
    si (_op_est(operator, "||"))
    {
        redde _brevem(d, nodus, FALSUM);
    }
    si      (_op_est(operator, "+"))  { op = MEDULLA_OP_ADDERE; }
    alioquin si (_op_est(operator, "-"))  { op = MEDULLA_OP_SUBTRAHERE; }
    alioquin si (_op_est(operator, "*"))  { op = MEDULLA_OP_MULTIPLICARE; }
    alioquin si (_op_est(operator, "/"))  { op = MEDULLA_OP_DIVIDERE; }
    alioquin si (_op_est(operator, "%"))  { op = MEDULLA_OP_RESIDUUM; }
    alioquin si (_op_est(operator, "&"))  { op = MEDULLA_OP_ET; }
    alioquin si (_op_est(operator, "|"))  { op = MEDULLA_OP_VEL; }
    alioquin si (_op_est(operator, "^"))  { op = MEDULLA_OP_AUT; }
    alioquin si (_op_est(operator, "<<")) { op = MEDULLA_OP_SINISTRORSUM; }
    alioquin si (_op_est(operator, ">>")) { op = MEDULLA_OP_DEXTRORSUM; }
    alioquin si (_op_est(operator, "==")) { op = MEDULLA_OP_AEQUALIS; comparatio = VERUM; }
    alioquin si (_op_est(operator, "!=")) { op = MEDULLA_OP_INAEQUALIS; comparatio = VERUM; }
    alioquin si (_op_est(operator, "<"))  { op = MEDULLA_OP_MINOR; comparatio = VERUM; }
    alioquin si (_op_est(operator, "<=")) { op = MEDULLA_OP_MINOR_AEQUALIS; comparatio = VERUM; }
    alioquin si (_op_est(operator, ">"))  { op = MEDULLA_OP_MAIOR; comparatio = VERUM; }
    alioquin si (_op_est(operator, ">=")) { op = MEDULLA_OP_MAIOR_AEQUALIS; comparatio = VERUM; }
    alioquin
    {
        _sistere(d, nodus, "operator binarii ignotus");
        redde _registrum_temporarium(d);
    }

    /* arithmetica monstratorum: scalatio per formam */
    si ((op == (s32)MEDULLA_OP_ADDERE
         || op == (s32)MEDULLA_OP_SUBTRAHERE) && !comparatio)
    {
        constans TypusC89* sin_ft = _typus_finalis(d,
            _canonicus(sin_v.datum.nodus));
        constans TypusC89* dex_ft = _typus_finalis(d,
            _canonicus(dex_v.datum.nodus));
        b32 sin_m = _est_monstratorius(sin_ft);
        b32 dex_m = _est_monstratorius(dex_ft);

        si (sin_m && dex_m && op == (s32)MEDULLA_OP_SUBTRAHERE)
        {
            /* monstrator - monstrator -> differentia elementorum */
            s32 mensura = _mensura_elementi_monstratoris(d, sin_ft);
            s32 a = _ut_valor(d, sin_v.datum.nodus);
            s32 b = _ut_valor(d, dex_v.datum.nodus);
            s32 crudum;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma elementi ignota");
                redde _registrum_temporarium(d);
            }
            crudum = _em(d, nodus, MEDULLA_OP_SUBTRAHERE,
                MEDULLA_TYPUS_I64, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d), medulla_op_registrum(a),
                medulla_op_registrum(b), medulla_op_nihil());
            redde _em(d, nodus, MEDULLA_OP_DIVIDERE,
                MEDULLA_TYPUS_S64, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_registrum(crudum),
                medulla_op_immediatum((s64)mensura),
                medulla_op_nihil());
        }
        si (sin_m != dex_m)
        {
            /* monstrator ± integer (aut integer + monstrator) */
            constans SilvaNodus* n_monstratoris = sin_m
                ? sin_v.datum.nodus : dex_v.datum.nodus;
            constans SilvaNodus* n_indicis = sin_m
                ? dex_v.datum.nodus : sin_v.datum.nodus;
            constans TypusC89* t_monstratoris = sin_m ? sin_ft
                                                      : dex_ft;
            s32 mensura = _mensura_elementi_monstratoris(d,
                t_monstratoris);
            s32 basis;
            MedullaOperandum index_op;
            s32 scalatum;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma elementi ignota");
                redde _registrum_temporarium(d);
            }
            basis = _ut_valor(d, n_monstratoris);
            index_op = _ut_operandum(d, n_indicis);
            scalatum = _indicem_scalare(d, nodus, index_op,
                _typus_medullae(_typus_finalis(d,
                    _canonicus(n_indicis))), mensura);
            redde _em(d, nodus, op, MEDULLA_TYPUS_I64,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(basis),
                medulla_op_registrum(scalatum), medulla_op_nihil());
        }
    }

    {
        MedullaOperandum a = _ut_operandum(d, sin_v.datum.nodus);
        MedullaOperandum b = _ut_operandum(d, dex_v.datum.nodus);
        s32 suffixus;

        si (comparatio)
        {
            suffixus = _typus_medullae(_typus_finalis(d,
                _canonicus(sin_v.datum.nodus)));
        }
        alioquin
        {
            suffixus = _typus_medullae(fructus_t);
        }
        si (suffixus < ZEPHYRUM)
        {
            _sistere(d, nodus, "typus binarii non scalaris");
            redde _registrum_temporarium(d);
        }
        redde _em(d, nodus, op, suffixus, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d), a, b, medulla_op_nihil());
    }
}

interior s32
_incrementum (Demissio* d, constans SilvaNodus* nodus,
    constans SilvaNodus* internum, b32 est_additio, b32 est_praefixum)
{
    DemissioLocus locus = _ut_locum(d, internum);
    constans TypusC89* naturalis = silva_c89_typus_expressionis(
        d->sem, _canonicus(internum));
    s32 mt = _typus_medullae(naturalis);
    s64 passus = I;
    s32 currens;
    s32 novus;

    si (!locus.valida || mt < ZEPHYRUM)
    {
        _sistere(d, nodus, "incrementum sine loco");
        redde _registrum_temporarium(d);
    }
    si (_est_monstratorius(naturalis))
    {
        s32 mensura = _mensura_elementi_monstratoris(d, naturalis);

        si (mensura <= ZEPHYRUM)
        {
            _sistere(d, nodus, "forma elementi ignota");
            redde _registrum_temporarium(d);
        }
        passus = (s64)mensura;
    }
    si (locus.directum)
    {
        currens = locus.index;
    }
    alioquin
    {
        currens = _em(d, nodus, MEDULLA_OP_LEGERE, mt,
            MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
            medulla_op_registrum(locus.index), medulla_op_nihil(),
            medulla_op_nihil());
    }
    /* valor vetus servandus pro postfixo */
    si (!est_praefixum)
    {
        s32 vetus = _movere(d, nodus, mt, _registrum_temporarium(d),
            medulla_op_registrum(currens));

        novus = _em(d, nodus, est_additio ? (s32)MEDULLA_OP_ADDERE
                                          : (s32)MEDULLA_OP_SUBTRAHERE,
            mt, MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
            medulla_op_registrum(currens),
            medulla_op_immediatum(passus), medulla_op_nihil());
        si (locus.directum)
        {
            (vacuum)_movere(d, nodus, mt, locus.index,
                medulla_op_registrum(novus));
        }
        alioquin
        {
            (vacuum)_em(d, nodus, MEDULLA_OP_SCRIBERE, mt,
                MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_registrum(locus.index),
                medulla_op_registrum(novus), medulla_op_nihil());
        }
        redde vetus;
    }
    novus = _em(d, nodus, est_additio ? (s32)MEDULLA_OP_ADDERE
                                      : (s32)MEDULLA_OP_SUBTRAHERE,
        mt, MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
        medulla_op_registrum(currens), medulla_op_immediatum(passus),
        medulla_op_nihil());
    si (locus.directum)
    {
        (vacuum)_movere(d, nodus, mt, locus.index,
            medulla_op_registrum(novus));
    }
    alioquin
    {
        (vacuum)_em(d, nodus, MEDULLA_OP_SCRIBERE, mt,
            MEDULLA_TYPUS_NIHIL, -I,
            medulla_op_registrum(locus.index),
            medulla_op_registrum(novus), medulla_op_nihil());
    }
    redde novus;
}

interior s32
_unarium (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaChorda operator = _tok_textus(
        silva_c89_unarium_tok_operator(nodus));
    SilvaValor internum_v = silva_c89_unarium_internum(nodus);
    constans SilvaNodus* internum;

    si (internum_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "unarium mancum");
        redde _registrum_temporarium(d);
    }
    internum = internum_v.datum.nodus;

    si (_op_est(operator, "-"))
    {
        s32 mt = _typus_medullae(silva_c89_typus_expressionis(
            d->sem, nodus));
        MedullaOperandum a = _ut_operandum(d, internum);

        redde _em(d, nodus, MEDULLA_OP_NEGARE,
            (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_S32,
            MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d), a,
            medulla_op_nihil(), medulla_op_nihil());
    }
    si (_op_est(operator, "+"))
    {
        redde _ut_valor(d, internum);
    }
    si (_op_est(operator, "~"))
    {
        s32 mt = _typus_medullae(silva_c89_typus_expressionis(
            d->sem, nodus));
        MedullaOperandum a = _ut_operandum(d, internum);

        redde _em(d, nodus, MEDULLA_OP_COMPLEMENTUM,
            (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_S32,
            MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d), a,
            medulla_op_nihil(), medulla_op_nihil());
    }
    si (_op_est(operator, "!"))
    {
        s32 mt = _typus_medullae(_typus_finalis(d,
            _canonicus(internum)));
        MedullaOperandum a = _ut_operandum(d, internum);

        redde _em(d, nodus, MEDULLA_OP_AEQUALIS,
            (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_S32,
            MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d), a,
            medulla_op_immediatum(0), medulla_op_nihil());
    }
    si (_op_est(operator, "&"))
    {
        DemissioLocus locus = _ut_locum(d, internum);

        si (!locus.valida || locus.directum)
        {
            _sistere(d, nodus, "inscriptio sine loco");
            redde _registrum_temporarium(d);
        }
        redde locus.index;
    }
    si (_op_est(operator, "*"))
    {
        s32 inscriptio = _ut_valor(d, internum);
        constans TypusC89* naturalis = silva_c89_typus_expressionis(
            d->sem, nodus);
        s32 mt = _typus_medullae(naturalis);
        constans TypusC89* exutus = _exutus(naturalis);

        si (mt >= ZEPHYRUM && exutus != NIHIL
            && exutus->genus != TYPUS_C89_ACIES
            && exutus->genus != TYPUS_C89_FUNCTIO)
        {
            redde _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(inscriptio),
                medulla_op_nihil(), medulla_op_nihil());
        }
        redde inscriptio;   /* aggregatum/acies/functio: inscriptio */
    }
    si (_op_est(operator, "++"))
    {
        redde _incrementum(d, nodus, internum, VERUM, VERUM);
    }
    si (_op_est(operator, "--"))
    {
        redde _incrementum(d, nodus, internum, FALSUM, VERUM);
    }
    _sistere(d, nodus, "operator unarii ignotus");
    redde _registrum_temporarium(d);
}

interior s32
_assignationem (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor sin_v = silva_c89_assignatio_sinister(nodus);
    SilvaValor dex_v = silva_c89_assignatio_dexter(nodus);
    SilvaChorda operator = _tok_textus(
        silva_c89_assignatio_tok_operator(nodus));
    DemissioLocus locus;
    constans SilvaNodus* sinister;
    constans TypusC89* sin_naturalis;
    s32 mt;
    s32 fons;

    si (sin_v.genus != SILVA_VALOR_NODUS
        || dex_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "assignatio manca");
        redde _registrum_temporarium(d);
    }
    sinister = _canonicus(sin_v.datum.nodus);
    sin_naturalis = silva_c89_typus_expressionis(d->sem, sinister);
    mt = _typus_medullae(sin_naturalis);
    locus = _ut_locum(d, sin_v.datum.nodus);
    si (!locus.valida || mt < ZEPHYRUM)
    {
        _sistere(d, nodus, "assignatio sine loco scalari");
        (vacuum)_ut_operandum(d, dex_v.datum.nodus);
        redde _registrum_temporarium(d);
    }

    si (_op_est(operator, "="))
    {
        fons = _ut_valor(d, dex_v.datum.nodus);
    }
    alioquin
    {
        /* compositum: conversiones implicatae iam annotatae in
         * utroque operando (M0b) */
        s32 op = -I;
        s32 currens;
        s32 commune_reg;
        constans TypusC89* commune_t = _typus_finalis(d, sinister);
        s32 mt_commune = _typus_medullae(commune_t);

        si      (_op_est(operator, "+="))  { op = MEDULLA_OP_ADDERE; }
        alioquin si (_op_est(operator, "-="))  { op = MEDULLA_OP_SUBTRAHERE; }
        alioquin si (_op_est(operator, "*="))  { op = MEDULLA_OP_MULTIPLICARE; }
        alioquin si (_op_est(operator, "/="))  { op = MEDULLA_OP_DIVIDERE; }
        alioquin si (_op_est(operator, "%="))  { op = MEDULLA_OP_RESIDUUM; }
        alioquin si (_op_est(operator, "&="))  { op = MEDULLA_OP_ET; }
        alioquin si (_op_est(operator, "|="))  { op = MEDULLA_OP_VEL; }
        alioquin si (_op_est(operator, "^="))  { op = MEDULLA_OP_AUT; }
        alioquin si (_op_est(operator, "<<=")) { op = MEDULLA_OP_SINISTRORSUM; }
        alioquin si (_op_est(operator, ">>=")) { op = MEDULLA_OP_DEXTRORSUM; }
        alioquin
        {
            _sistere(d, nodus, "operator assignationis ignotus");
            redde _registrum_temporarium(d);
        }
        si (locus.directum)
        {
            currens = locus.index;
        }
        alioquin
        {
            currens = _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(locus.index),
                medulla_op_nihil(), medulla_op_nihil());
        }
        commune_reg = _conversionem(d, nodus, currens,
            sin_naturalis, commune_t);
        si (_est_monstratorius(sin_naturalis)
            && (op == (s32)MEDULLA_OP_ADDERE
                || op == (s32)MEDULLA_OP_SUBTRAHERE))
        {
            /* p += n : scalatio */
            s32 mensura = _mensura_elementi_monstratoris(d,
                sin_naturalis);
            MedullaOperandum index_op = _ut_operandum(d,
                dex_v.datum.nodus);
            s32 scalatum;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma elementi ignota");
                redde _registrum_temporarium(d);
            }
            scalatum = _indicem_scalare(d, nodus, index_op,
                _typus_medullae(_typus_finalis(d,
                    _canonicus(dex_v.datum.nodus))), mensura);
            fons = _em(d, nodus, op, MEDULLA_TYPUS_I64,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(commune_reg),
                medulla_op_registrum(scalatum), medulla_op_nihil());
        }
        alioquin
        {
            MedullaOperandum b = _ut_operandum(d, dex_v.datum.nodus);
            s32 crudus = _em(d, nodus, op,
                (mt_commune >= ZEPHYRUM) ? mt_commune : mt,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(commune_reg), b,
                medulla_op_nihil());

            fons = _conversionem(d, nodus, crudus, commune_t,
                sin_naturalis);
        }
    }

    si (locus.directum)
    {
        (vacuum)_movere(d, nodus, mt, locus.index,
            medulla_op_registrum(fons));
    }
    alioquin
    {
        (vacuum)_em(d, nodus, MEDULLA_OP_SCRIBERE, mt,
            MEDULLA_TYPUS_NIHIL, -I,
            medulla_op_registrum(locus.index),
            medulla_op_registrum(fons), medulla_op_nihil());
    }
    redde fons;
}

interior s32
_ternarium (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor cond_v = silva_c89_ternarius_conditio(nodus);
    SilvaValor verum_v = silva_c89_ternarius_verum(nodus);
    SilvaValor falsum_v = silva_c89_ternarius_falsum(nodus);
    s32 mt = _typus_medullae(silva_c89_typus_expressionis(d->sem,
        nodus));
    s32 fructus = _registrum_temporarium(d);
    s32 b_verum = _bloccum_novum(d, "tern_verum");
    s32 b_falsum = _bloccum_novum(d, "tern_falsum");
    s32 b_finis = _bloccum_novum(d, "tern_finis");
    s32 conditio;

    si (mt < ZEPHYRUM)
    {
        mt = MEDULLA_TYPUS_S64;   /* monstratores compositi etc. */
    }
    conditio = _ut_valor(d, cond_v.datum.nodus);
    _ramus(d, nodus, medulla_op_registrum(conditio), b_verum,
        b_falsum);
    d->bloccus = b_verum;
    (vacuum)_movere(d, nodus, mt, fructus,
        _ut_operandum(d, verum_v.datum.nodus));
    _salire_si_vivus(d, b_finis);
    d->bloccus = b_falsum;
    (vacuum)_movere(d, nodus, mt, fructus,
        _ut_operandum(d, falsum_v.datum.nodus));
    _salire_si_vivus(d, b_finis);
    d->bloccus = b_finis;
    redde fructus;
}

/* dispensator: valor naturalis + conversio annotata applicata */
interior s32
_expressionem (Demissio* d, constans SilvaNodus* nodus)
{
    constans TypusC89* naturalis;
    constans TypusC89* conversus;
    s32 fructus;

    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde _registrum_temporarium(d);
    }
    naturalis = silva_c89_typus_expressionis(d->sem, nodus);
    conversus = silva_c89_conversio_expressionis(d->sem, nodus);

    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_PARENTHESIS:
        {
            SilvaValor v = silva_c89_parenthesis_internum(nodus);

            redde _expressionem(d, v.datum.nodus);
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR:
        {
            fructus = _identificatorem(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_BINARIUM:
        {
            fructus = _binarium(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_UNARIUM:
        {
            fructus = _unarium(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_POSTCREMENTUM:
        {
            SilvaValor internum_v = silva_c89_postcrementum_internum(
                nodus);
            SilvaChorda operator = _tok_textus(
                silva_c89_postcrementum_tok_operator(nodus));

            fructus = _incrementum(d, nodus, internum_v.datum.nodus,
                _op_est(operator, "++"), FALSUM);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_ASSIGNATIO:
        {
            fructus = _assignationem(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_TERNARIUS:
        {
            fructus = _ternarium(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_VIRGULA:
        {
            SilvaValor sin_v = silva_c89_virgula_sinister(nodus);
            SilvaValor dex_v = silva_c89_virgula_dexter(nodus);

            (vacuum)_ut_operandum(d, sin_v.datum.nodus);
            fructus = _ut_valor(d, dex_v.datum.nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_CONVERSIO:
        {
            SilvaValor internum_v = silva_c89_conversio_internum(
                nodus);
            constans SilvaNodus* internum = _canonicus(
                internum_v.datum.nodus);
            s32 valor = _ut_valor(d, internum);

            fructus = _conversionem(d, nodus, valor,
                _typus_finalis(d, internum), naturalis);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS:
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI:
        {
            s64 valor = 0;

            si (silva_c89_constans_aestimare(d->sem, nodus, &valor))
            {
                fructus = _movere(d, nodus, MEDULLA_TYPUS_I64,
                    _registrum_temporarium(d),
                    medulla_op_immediatum(valor));
            }
            alioquin
            {
                _sistere(d, nodus, "magnitudo inaestimabilis");
                fructus = _registrum_temporarium(d);
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_INTEGER:
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER:
        casus (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS:
        {
            MedullaOperandum op = _ut_operandum(d, nodus);
            s32 mt = _typus_medullae(_typus_finalis(d, nodus));

            redde _movere(d, nodus, mt, _registrum_temporarium(d),
                op);   /* conversio iam in valore */
        }
        casus (s32)SILVA_C89_GENUS_VOCATIO:
        {
            _sistere(d, nodus, "nondum vocatio (M1b B)");
            redde _registrum_temporarium(d);
        }
        casus (s32)SILVA_C89_GENUS_ACCESSUS:
        {
            _sistere(d, nodus, "nondum accessus (M1b B)");
            redde _registrum_temporarium(d);
        }
        casus (s32)SILVA_C89_GENUS_SUBSCRIPTIO:
        {
            _sistere(d, nodus, "nondum subscriptio (M1b B)");
            redde _registrum_temporarium(d);
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHORDA:
        {
            _sistere(d, nodus, "nondum chorda litteralis (M1b C)");
            redde _registrum_temporarium(d);
        }
        casus (s32)SILVA_C89_GENUS_CONGERIES:
        {
            _sistere(d, nodus, "nondum congeries (M1b C)");
            redde _registrum_temporarium(d);
        }
        ordinarius:
        {
            _sistere(d, nodus, "expressio generis ignoti");
            redde _registrum_temporarium(d);
        }
    }
    /* conversio scopi annotata (lapsus/UAC/assignatio/...) */
    si (conversus != NIHIL)
    {
        fructus = _conversionem(d, nodus, fructus, naturalis,
            conversus);
    }
    redde fructus;
}

interior MedullaOperandum
_ut_operandum (Demissio* d, constans SilvaNodus* nodus)
{
    constans SilvaNodus* canonicus = _canonicus(nodus);

    si (canonicus == NIHIL)
    {
        redde medulla_op_immediatum(0);
    }
    si (canonicus->genus == (s32)SILVA_C89_GENUS_FOLIUM_INTEGER
        || canonicus->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER)
    {
        s64 valor = 0;

        si (silva_c89_constans_aestimare(d->sem, canonicus, &valor))
        {
            constans TypusC89* finalis = _typus_finalis(d,
                canonicus);
            s32 mt = _typus_medullae(finalis);

            si (_fluitans_m(mt))
            {
                redde medulla_op_immediatum_f((f64)valor);
            }
            redde medulla_op_immediatum(valor);
        }
    }
    si (canonicus->genus == (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS)
    {
        SilvaValor tok_v = silva_c89_folium_fluitans_tok_valor(
            canonicus);
        SilvaChorda textus = _tok_textus(tok_v);

        si (textus.mensura > ZEPHYRUM
            && textus.mensura < XL)
        {
            character littera[XL];

            memcpy(littera, textus.datum,
                (memoriae_index)textus.mensura);
            littera[textus.mensura] = '\0';
            redde medulla_op_immediatum_f(strtod(littera, NIHIL));
        }
    }
    redde medulla_op_registrum(_expressionem(d, nodus));
}

interior s32
_ut_valor (Demissio* d, constans SilvaNodus* nodus)
{
    MedullaOperandum op = _ut_operandum(d, nodus);

    si (op.genus == (s32)MEDULLA_OPERANDUM_REGISTRUM)
    {
        redde op.datum.index;
    }
    redde _movere(d, _canonicus(nodus),
        _typus_medullae(_typus_finalis(d, _canonicus(nodus))),
        _registrum_temporarium(d), op);
}

interior DemissioLocus
_ut_locum (Demissio* d, constans SilvaNodus* nodus)
{
    DemissioLocus locus;

    locus.directum = FALSUM;
    locus.index = -I;
    locus.valida = FALSUM;
    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde locus;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        redde _ut_locum(d, v.datum.nodus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_nodi(d->sem, nodus);
        DemissioSedes* sedes;

        si (symbolum == NIHIL)
        {
            redde locus;
        }
        sedes = _sedem_symboli(d, symbolum);
        si (sedes != NIHIL)
        {
            locus.directum = !sedes->est_arca;
            locus.index = sedes->index_registri;
            locus.valida = VERUM;
            redde locus;
        }
        si (symbolum->profunditas > ZEPHYRUM
            && (symbolum->repositio & REPOSITIO_STATICA)
                != ZEPHYRUM)
        {
            redde locus;   /* staticum locale: M1b C */
        }
        /* globale */
        {
            s32 index_symboli = medulla_symbolum_internare(
                d->modulus, _ch_de_silva(symbolum->titulus));

            locus.directum = FALSUM;
            locus.index = _em(d, nodus, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());
            locus.valida = VERUM;
            redde locus;
        }
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
    {
        SilvaChorda operator = _tok_textus(
            silva_c89_unarium_tok_operator(nodus));

        si (_op_est(operator, "*"))
        {
            SilvaValor internum_v = silva_c89_unarium_internum(
                nodus);

            locus.directum = FALSUM;
            locus.index = _ut_valor(d, internum_v.datum.nodus);
            locus.valida = VERUM;
            redde locus;
        }
    }
    redde locus;   /* accessus/subscriptio -> M1b B */
}

/* ==================================================
 * Sententiae
 * ================================================== */

interior vacuum
_lista_sententiarum (Demissio* d, SilvaValor lista)
{
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(lista);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(lista, i);

        si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
        {
            _sententiam(d, v->datum.nodus);
        }
    }
}

interior constans SemanticaSymbolum*
_symbolum_lexematis (constans Demissio* d, SilvaToken* lexema)
{
    redde (constans SemanticaSymbolum*)_tabulam_invenire(
        d->lexemata, (constans vacuum*)lexema);
}

interior vacuum
_declarationem (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor declaratores = silva_c89_declaratio_declaratores(
        nodus);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(declaratores);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(declaratores, i);
        constans SilvaNodus* elementum;
        constans SilvaNodus* initiator = NIHIL;
        SilvaToken* lexema;
        constans SemanticaSymbolum* symbolum;
        DemissioSedes* sedes;

        si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        elementum = _canonicus(v->datum.nodus);
        si (elementum->genus
            == (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
        {
            SilvaValor init_v = silva_c89_declarator_initiatus_initiator(
                elementum);

            si (init_v.genus == SILVA_VALOR_NODUS)
            {
                initiator = init_v.datum.nodus;
            }
        }
        lexema = silva_c89_declaratoris_titulus(elementum);
        si (lexema == NIHIL)
        {
            perge;
        }
        symbolum = _symbolum_lexematis(d, lexema);
        si (symbolum == NIHIL
            || symbolum->genus == (int)SYMBOLUM_TYPEDEF)
        {
            perge;
        }
        si ((symbolum->repositio & REPOSITIO_STATICA) != ZEPHYRUM)
        {
            _sistere(d, nodus, "nondum staticum locale (M1b C)");
            perge;
        }
        sedes = _sedem_symboli(d, symbolum);
        si (sedes == NIHIL || initiator == NIHIL)
        {
            perge;
        }
        si (_canonicus(initiator)->genus
            == (s32)SILVA_C89_GENUS_CONGERIES)
        {
            _sistere(d, nodus, "nondum congeries localis (M1b C)");
            perge;
        }
        {
            s32 mt = _typus_medullae(symbolum->typus);
            MedullaOperandum fons = _ut_operandum(d, initiator);

            si (mt < ZEPHYRUM)
            {
                _sistere(d, nodus,
                    "initiator aggregati sine congerie");
                perge;
            }
            si (sedes->est_arca)
            {
                (vacuum)_em(d, nodus, MEDULLA_OP_SCRIBERE, mt,
                    MEDULLA_TYPUS_NIHIL, -I,
                    medulla_op_registrum(sedes->index_registri),
                    fons, medulla_op_nihil());
            }
            alioquin
            {
                (vacuum)_movere(d, nodus, mt, sedes->index_registri,
                    fons);
            }
        }
    }
}

interior s32
_bloccum_tituli (Demissio* d, SilvaChorda titulus)
{
    character littera[LXIV];
    chorda quaesitum;
    i32 caput = (titulus.mensura < LX) ? titulus.mensura : LX;
    i32 i;
    i32 m;

    littera[ZEPHYRUM] = 'l';
    littera[I] = '_';
    memcpy(littera + II, titulus.datum, (memoriae_index)caput);
    quaesitum.datum = (i8*)littera;
    quaesitum.mensura = caput + II;

    m = xar_numerus(d->functio->blocci);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans MedullaBloccus* bloccus =
            (constans MedullaBloccus*)xar_obtinere(
                d->functio->blocci, i);

        si (bloccus->titulus.mensura == quaesitum.mensura
            && memcmp(bloccus->titulus.datum, quaesitum.datum,
                   (memoriae_index)quaesitum.mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde medulla_bloccum_creare(d->functio,
        chorda_transcribere(quaesitum, d->piscina));
}

interior vacuum
_commutationem (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor discrimen_v = silva_c89_commutatio_discrimen(nodus);
    SilvaValor corpus_v = silva_c89_commutatio_corpus(nodus);
    constans SilvaNodus* corpus;
    SilvaValor elementa;
    s32 discrimen;
    s32 mt = _typus_medullae(_typus_finalis(d,
        _canonicus(discrimen_v.datum.nodus)));
    s32 b_finis;
    s32 b_ordinarius = -I;
    s32 frange_prior = d->frange_finis;
    i32 i;
    i32 m;

    si (corpus_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "commutatio sine corpore");
        redde;
    }
    corpus = _canonicus(corpus_v.datum.nodus);
    si (corpus->genus != (s32)SILVA_C89_GENUS_CORPUS)
    {
        _sistere(d, nodus, "corpus commutationis non compositum");
        redde;
    }
    elementa = silva_c89_corpus_elementa(corpus);
    m = (i32)silva_valor_lista_numerus(elementa);
    discrimen = _ut_valor(d, discrimen_v.datum.nodus);
    b_finis = _bloccum_novum(d, "comm_finis");
    d->frange_finis = b_finis;

    /* transitus primus: bloccos casuum creare + dispensationem
     * emittere (catena comparationum - tabula saliendi parcata) */
    {
        Xar* blocci_casuum = xar_creare(d->piscina,
            (i32)magnitudo(s32));

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(elementa, i);
            constans SilvaNodus* elementum;
            s32* sedes_blocci;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            elementum = _canonicus(v->datum.nodus);
            si (elementum->genus == (s32)SILVA_C89_GENUS_CASUS)
            {
                s64 valor = 0;
                s32 b_casus = _bloccum_novum(d, "comm_casus");
                SilvaValor valor_v = silva_c89_casus_valor(
                    elementum);
                s32 probatum;

                sedes_blocci = (s32*)xar_addere(blocci_casuum);
                *sedes_blocci = b_casus;
                si (!silva_c89_constans_aestimare(d->sem,
                        valor_v.datum.nodus, &valor))
                {
                    _sistere(d, elementum, "casus inaestimabilis");
                    perge;
                }
                probatum = _em(d, elementum, MEDULLA_OP_AEQUALIS,
                    (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_S32,
                    MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                    medulla_op_registrum(discrimen),
                    medulla_op_immediatum(valor),
                    medulla_op_nihil());
                {
                    s32 b_proximus = _bloccum_novum(d,
                        "comm_proba");

                    _ramus(d, elementum,
                        medulla_op_registrum(probatum), b_casus,
                        b_proximus);
                    d->bloccus = b_proximus;
                }
            }
            alioquin si (elementum->genus
                == (s32)SILVA_C89_GENUS_ORDINARIUS)
            {
                b_ordinarius = _bloccum_novum(d, "comm_ordinarius");
                sedes_blocci = (s32*)xar_addere(blocci_casuum);
                *sedes_blocci = b_ordinarius;
            }
            alioquin
            {
                sedes_blocci = (s32*)xar_addere(blocci_casuum);
                *sedes_blocci = -I;
            }
        }
        _salire_si_vivus(d, (b_ordinarius >= ZEPHYRUM)
            ? b_ordinarius : b_finis);

        /* transitus alter: corpora ordine fontis (perlapsus) */
        {
            i32 a = ZEPHYRUM;

            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(elementa,
                    i);
                constans SilvaNodus* elementum;

                si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
                {
                    perge;
                }
                elementum = _canonicus(v->datum.nodus);
                si (elementum->genus == (s32)SILVA_C89_GENUS_CASUS
                    || elementum->genus
                        == (s32)SILVA_C89_GENUS_ORDINARIUS)
                {
                    s32* b = (s32*)xar_obtinere(blocci_casuum,
                        (i32)a);

                    a++;
                    si (b == NIHIL || *b < ZEPHYRUM)
                    {
                        perge;
                    }
                    _salire_si_vivus(d, *b);   /* perlapsus */
                    d->bloccus = *b;
                    si (elementum->genus
                        == (s32)SILVA_C89_GENUS_CASUS)
                    {
                        _lista_sententiarum(d,
                            silva_c89_casus_sententiae(elementum));
                    }
                    alioquin
                    {
                        _lista_sententiarum(d,
                            silva_c89_ordinarius_sententiae(
                                elementum));
                    }
                }
                alioquin
                {
                    a++;
                    _sententiam(d, elementum);
                }
            }
        }
    }
    _salire_si_vivus(d, b_finis);
    d->bloccus = b_finis;
    d->frange_finis = frange_prior;
}

interior vacuum
_sententiam (Demissio* d, constans SilvaNodus* nodus)
{
    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde;
    }
    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_CORPUS:
        {
            _lista_sententiarum(d, silva_c89_corpus_elementa(nodus));
            redde;
        }
        casus (s32)SILVA_C89_GENUS_SENTENTIA_VACUA:
        {
            redde;
        }
        casus (s32)SILVA_C89_GENUS_DECLARATIO:
        {
            _declarationem(d, nodus);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS:
        {
            SilvaValor v = silva_c89_sententia_expressionis_expressio(
                nodus);

            si (v.genus == SILVA_VALOR_NODUS)
            {
                (vacuum)_ut_operandum(d, v.datum.nodus);
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_SI:
        {
            SilvaValor cond_v = silva_c89_si_conditio(nodus);
            SilvaValor cons_v = silva_c89_si_consequens(nodus);
            SilvaValor alio_v = silva_c89_si_alioquin(nodus);
            b32 habet_alioquin = (alio_v.genus == SILVA_VALOR_NODUS);
            s32 b_verum = _bloccum_novum(d, "si_verum");
            s32 b_falsum = habet_alioquin
                ? _bloccum_novum(d, "si_falsum") : -I;
            s32 b_finis = _bloccum_novum(d, "si_finis");
            s32 conditio = _ut_valor(d, cond_v.datum.nodus);

            _ramus(d, nodus, medulla_op_registrum(conditio),
                b_verum, habet_alioquin ? b_falsum : b_finis);
            d->bloccus = b_verum;
            si (cons_v.genus == SILVA_VALOR_NODUS)
            {
                _sententiam(d, cons_v.datum.nodus);
            }
            _salire_si_vivus(d, b_finis);
            si (habet_alioquin)
            {
                d->bloccus = b_falsum;
                _sententiam(d, alio_v.datum.nodus);
                _salire_si_vivus(d, b_finis);
            }
            d->bloccus = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_DUM:
        {
            s32 b_proba = _bloccum_novum(d, "dum_proba");
            s32 b_corpus = _bloccum_novum(d, "dum_corpus");
            s32 b_finis = _bloccum_novum(d, "dum_finis");
            s32 frange_prior = d->frange_finis;
            s32 perge_prior = d->perge_finis;

            _salire_si_vivus(d, b_proba);
            d->bloccus = b_proba;
            {
                SilvaValor cond_v = silva_c89_dum_conditio(nodus);
                s32 conditio = _ut_valor(d, cond_v.datum.nodus);

                _ramus(d, nodus, medulla_op_registrum(conditio),
                    b_corpus, b_finis);
            }
            d->bloccus = b_corpus;
            d->frange_finis = b_finis;
            d->perge_finis = b_proba;
            {
                SilvaValor corpus_v = silva_c89_dum_corpus(nodus);

                si (corpus_v.genus == SILVA_VALOR_NODUS)
                {
                    _sententiam(d, corpus_v.datum.nodus);
                }
            }
            _salire_si_vivus(d, b_proba);
            d->frange_finis = frange_prior;
            d->perge_finis = perge_prior;
            d->bloccus = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_FAC_DUM:
        {
            s32 b_corpus = _bloccum_novum(d, "fac_corpus");
            s32 b_proba = _bloccum_novum(d, "fac_proba");
            s32 b_finis = _bloccum_novum(d, "fac_finis");
            s32 frange_prior = d->frange_finis;
            s32 perge_prior = d->perge_finis;

            _salire_si_vivus(d, b_corpus);
            d->bloccus = b_corpus;
            d->frange_finis = b_finis;
            d->perge_finis = b_proba;
            {
                SilvaValor corpus_v = silva_c89_fac_dum_corpus(
                    nodus);

                si (corpus_v.genus == SILVA_VALOR_NODUS)
                {
                    _sententiam(d, corpus_v.datum.nodus);
                }
            }
            _salire_si_vivus(d, b_proba);
            d->bloccus = b_proba;
            {
                SilvaValor cond_v = silva_c89_fac_dum_conditio(
                    nodus);
                s32 conditio = _ut_valor(d, cond_v.datum.nodus);

                _ramus(d, nodus, medulla_op_registrum(conditio),
                    b_corpus, b_finis);
            }
            d->frange_finis = frange_prior;
            d->perge_finis = perge_prior;
            d->bloccus = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_PER:
        {
            SilvaValor clausula_v = silva_c89_per_clausula(nodus);
            constans SilvaNodus* clausula = (clausula_v.genus
                == SILVA_VALOR_NODUS)
                ? _canonicus(clausula_v.datum.nodus) : NIHIL;
            s32 b_proba = _bloccum_novum(d, "per_proba");
            s32 b_corpus = _bloccum_novum(d, "per_corpus");
            s32 b_passus = _bloccum_novum(d, "per_passus");
            s32 b_finis = _bloccum_novum(d, "per_finis");
            s32 frange_prior = d->frange_finis;
            s32 perge_prior = d->perge_finis;

            si (clausula != NIHIL)
            {
                SilvaValor initium_v = silva_c89_per_clausula_initium(
                    clausula);

                si (initium_v.genus == SILVA_VALOR_NODUS)
                {
                    constans SilvaNodus* initium = _canonicus(
                        initium_v.datum.nodus);

                    si (initium->genus
                        == (s32)SILVA_C89_GENUS_DECLARATIO)
                    {
                        _declarationem(d, initium);   /* C99-forma
                                                       * tolerata */
                    }
                    alioquin
                    {
                        (vacuum)_ut_operandum(d, initium);
                    }
                }
            }
            _salire_si_vivus(d, b_proba);
            d->bloccus = b_proba;
            {
                SilvaValor cond_v;

                cond_v.genus = SILVA_VALOR_NIHIL;
                si (clausula != NIHIL)
                {
                    cond_v = silva_c89_per_clausula_conditio(
                        clausula);
                }
                si (cond_v.genus == SILVA_VALOR_NODUS)
                {
                    s32 conditio = _ut_valor(d, cond_v.datum.nodus);

                    _ramus(d, nodus, medulla_op_registrum(conditio),
                        b_corpus, b_finis);
                }
                alioquin
                {
                    _salire_si_vivus(d, b_corpus);
                }
            }
            d->bloccus = b_corpus;
            d->frange_finis = b_finis;
            d->perge_finis = b_passus;
            {
                SilvaValor corpus_v = silva_c89_per_corpus(nodus);

                si (corpus_v.genus == SILVA_VALOR_NODUS)
                {
                    _sententiam(d, corpus_v.datum.nodus);
                }
            }
            _salire_si_vivus(d, b_passus);
            d->bloccus = b_passus;
            si (clausula != NIHIL)
            {
                SilvaValor passus_v = silva_c89_per_clausula_passus(
                    clausula);

                si (passus_v.genus == SILVA_VALOR_NODUS)
                {
                    (vacuum)_ut_operandum(d, passus_v.datum.nodus);
                }
            }
            _salire_si_vivus(d, b_proba);
            d->frange_finis = frange_prior;
            d->perge_finis = perge_prior;
            d->bloccus = b_finis;
            redde;
        }
        casus (s32)SILVA_C89_GENUS_COMMUTATIO:
        {
            _commutationem(d, nodus);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_TITULATUM:
        {
            SilvaChorda titulus = _tok_textus(
                silva_c89_titulatum_tok_titulus(nodus));
            s32 b = _bloccum_tituli(d, titulus);
            SilvaValor sententia_v = silva_c89_titulatum_sententia(
                nodus);

            _salire_si_vivus(d, b);
            d->bloccus = b;
            si (sententia_v.genus == SILVA_VALOR_NODUS)
            {
                _sententiam(d, sententia_v.datum.nodus);
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_SALTA:
        {
            SilvaChorda titulus = _tok_textus(
                silva_c89_salta_tok_destinatio(nodus));
            s32 b = _bloccum_tituli(d, titulus);

            _salire_si_vivus(d, b);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_FRANGE:
        {
            si (d->frange_finis >= ZEPHYRUM)
            {
                _salire_si_vivus(d, d->frange_finis);
            }
            alioquin
            {
                _sistere(d, nodus, "frange extra ansam");
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_PERGE:
        {
            si (d->perge_finis >= ZEPHYRUM)
            {
                _salire_si_vivus(d, d->perge_finis);
            }
            alioquin
            {
                _sistere(d, nodus, "perge extra ansam");
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_REDDE:
        {
            SilvaValor valor_v = silva_c89_redde_valor(nodus);
            MedullaOperandum a = medulla_op_nihil();

            si (valor_v.genus == SILVA_VALOR_NODUS)
            {
                a = _ut_operandum(d, valor_v.datum.nodus);
            }
            (vacuum)_em(d, nodus, MEDULLA_OP_REDDE,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I, a,
                medulla_op_nihil(), medulla_op_nihil());
            redde;
        }
        casus (s32)SILVA_C89_GENUS_CONDITIONALIS:
        {
            SilvaValor rami = silva_c89_conditionalis_rami(nodus);
            i32 i;
            i32 m = (i32)silva_valor_lista_numerus(rami);

            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(rami, i);

                si (v != NIHIL && v->genus == SILVA_VALOR_NODUS
                    && v->datum.nodus->genus
                        == (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS)
                {
                    _lista_sententiarum(d,
                        silva_c89_ramus_sumptus_contentum(
                            v->datum.nodus));
                }
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_ERROR:
        {
            redde;   /* corpus sine erroribus; robustitas */
        }
        ordinarius:
        {
            _sistere(d, nodus, "sententia generis ignoti");
            redde;
        }
    }
}

/* ==================================================
 * Praecursus: symbola inscriptione capta + locales colligere
 * ================================================== */

interior vacuum
_praecursum (Demissio* d, constans SilvaNodus* nodus, Xar* locales)
{
    i32 i;

    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_DECLARATIO)
    {
        SilvaValor declaratores = silva_c89_declaratio_declaratores(
            nodus);
        i32 m = (i32)silva_valor_lista_numerus(declaratores);
        i32 k;

        per (k = ZEPHYRUM; k < m; k++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(declaratores,
                k);
            SilvaToken* lexema;
            constans SemanticaSymbolum* symbolum;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            lexema = silva_c89_declaratoris_titulus(
                _canonicus(v->datum.nodus));
            si (lexema == NIHIL)
            {
                perge;
            }
            symbolum = _symbolum_lexematis(d, lexema);
            si (symbolum != NIHIL
                && symbolum->genus != (int)SYMBOLUM_TYPEDEF
                && (symbolum->repositio & REPOSITIO_STATICA)
                    == ZEPHYRUM
                && symbolum->profunditas > ZEPHYRUM)
            {
                constans SemanticaSymbolum** sedes =
                    (constans SemanticaSymbolum**)xar_addere(
                        locales);

                si (sedes != NIHIL)
                {
                    *sedes = symbolum;
                }
            }
        }
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
    {
        SilvaChorda operator = _tok_textus(
            silva_c89_unarium_tok_operator(nodus));

        si (_op_est(operator, "&"))
        {
            constans SilvaNodus* basis = NIHIL;
            SilvaValor v = silva_c89_unarium_internum(nodus);

            si (v.genus == SILVA_VALOR_NODUS)
            {
                basis = _canonicus(v.datum.nodus);
            }
            dum (basis != NIHIL)
            {
                si (basis->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
                {
                    v = silva_c89_parenthesis_internum(basis);
                }
                alioquin si (basis->genus
                    == (s32)SILVA_C89_GENUS_SUBSCRIPTIO)
                {
                    v = silva_c89_subscriptio_basis(basis);
                }
                alioquin si (basis->genus
                    == (s32)SILVA_C89_GENUS_ACCESSUS
                    && _op_est(_tok_textus(
                           silva_c89_accessus_tok_operator(basis)),
                           "."))
                {
                    v = silva_c89_accessus_basis(basis);
                }
                alioquin
                {
                    frange;
                }
                basis = (v.genus == SILVA_VALOR_NODUS)
                    ? _canonicus(v.datum.nodus) : NIHIL;
            }
            si (basis != NIHIL && basis->genus
                == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
            {
                constans SemanticaSymbolum* symbolum =
                    silva_c89_symbolum_nodi(d->sem, basis);

                si (symbolum != NIHIL
                    && symbolum->profunditas > ZEPHYRUM)
                {
                    _tabulam_ponere(d, d->capti,
                        (constans vacuum*)symbolum,
                        _sine_constante((constans vacuum*)symbolum));
                }
            }
        }
    }
    /* recursio generalis per loci (NODUS + elementa listarum) */
    per (i = ZEPHYRUM; i < (i32)nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (v->genus == SILVA_VALOR_NODUS)
        {
            _praecursum(d, v->datum.nodus, locales);
        }
        alioquin si (v->genus == SILVA_VALOR_LISTA)
        {
            i32 k;
            i32 m = (i32)silva_valor_lista_numerus(*v);

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* e = silva_valor_lista_obtinere(*v,
                    (unsigned int)k);

                si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
                {
                    _praecursum(d, e->datum.nodus, locales);
                }
            }
        }
    }
}

/* ==================================================
 * Functio
 * ================================================== */

interior constans SilvaNodus*
_declarator_functionis_invenire (constans SilvaNodus* declarator)
{
    s32 custos = ZEPHYRUM;

    declarator = _canonicus(declarator);
    dum (declarator != NIHIL && custos < XXXII)
    {
        SilvaValor v;

        custos++;
        commutatio (declarator->genus)
        {
            casus (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS:
            {
                redde declarator;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR:
            {
                v = silva_c89_declarator_monstrator_internum(
                    declarator);
                frange;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS:
            {
                v = silva_c89_declarator_initiatus_declarator(
                    declarator);
                frange;
            }
            casus (s32)SILVA_C89_GENUS_PARENTHESIS:
            {
                v = silva_c89_parenthesis_internum(declarator);
                frange;
            }
            casus (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI:
            {
                v = silva_c89_declarator_aciei_internum(declarator);
                frange;
            }
            ordinarius:
            {
                redde NIHIL;
            }
        }
        declarator = (v.genus == SILVA_VALOR_NODUS)
            ? _canonicus(v.datum.nodus) : NIHIL;
    }
    redde NIHIL;
}

interior b32
_arcam_symboli (Demissio* d, constans SemanticaSymbolum* symbolum)
{
    constans TypusC89* exutus = _exutus(symbolum->typus);
    s32 mt = _typus_medullae(symbolum->typus);

    si (_tabulam_invenire(d->capti, (constans vacuum*)symbolum)
        != NIHIL)
    {
        redde VERUM;
    }
    si (mt < ZEPHYRUM)
    {
        redde VERUM;   /* aggregatum */
    }
    si (exutus != NIHIL && exutus->genus == TYPUS_C89_ACIES)
    {
        redde VERUM;
    }
    redde FALSUM;
}

interior vacuum
_functionem (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor declarator_v = silva_c89_definitio_functionis_declarator(
        nodus);
    SilvaValor corpus_v = silva_c89_definitio_functionis_corpus(nodus);
    SilvaToken* lexema;
    constans SemanticaSymbolum* symbolum;
    constans TypusC89* typus_functionis;
    constans TypusC89* reditus = NIHIL;
    s32 mt_reditus;
    b32 reditus_aggregatus = FALSUM;
    Xar* locales;
    i32 i;
    i32 m;

    si (declarator_v.genus != SILVA_VALOR_NODUS
        || corpus_v.genus != SILVA_VALOR_NODUS)
    {
        redde;
    }
    lexema = silva_c89_declaratoris_titulus(
        _canonicus(declarator_v.datum.nodus));
    symbolum = (lexema != NIHIL) ? _symbolum_lexematis(d, lexema)
                                 : NIHIL;
    si (symbolum == NIHIL)
    {
        redde;
    }
    typus_functionis = _exutus(symbolum->typus);
    si (typus_functionis == NIHIL
        || typus_functionis->genus != TYPUS_C89_FUNCTIO)
    {
        redde;
    }
    reditus = _exutus(typus_functionis->datum.functio.reditus);
    mt_reditus = _typus_medullae(reditus);
    si (mt_reditus < ZEPHYRUM && reditus != NIHIL
        && (reditus->genus == TYPUS_C89_STRUCTURA
            || reditus->genus == TYPUS_C89_UNIO))
    {
        reditus_aggregatus = VERUM;
    }
    d->functio = medulla_functionem_creare(d->modulus,
        _ch_de_silva(symbolum->titulus),
        (mt_reditus >= ZEPHYRUM) ? mt_reditus : MEDULLA_TYPUS_NIHIL,
        typus_functionis->datum.functio.est_variadica);
    si (d->functio == NIHIL)
    {
        redde;   /* definitio duplex */
    }
    d->numerator = ZEPHYRUM;
    d->frange_finis = -I;
    d->perge_finis = -I;
    d->sedes = tabula_dispersa_creare_chorda(d->piscina, CCLVI);
    d->capti = tabula_dispersa_creare_chorda(d->piscina, CCLVI);
    locales = xar_creare(d->piscina,
        (i32)magnitudo(SemanticaSymbolum*));

    /* praecursus: capti + locales (ante residentiam) */
    _praecursum(d, corpus_v.datum.nodus, locales);

    /* parametra: registra prima (ordo signaturae); symbola
     * servata pro arca post initium */
    {
        constans SilvaNodus* df = _declarator_functionis_invenire(
            declarator_v.datum.nodus);
        SilvaValor parametra;
        Xar* symbola_parametrorum = xar_creare(d->piscina,
            (i32)magnitudo(SemanticaSymbolum*));

        si (df != NIHIL)
        {
            parametra = silva_c89_declarator_functionis_parametra(
                df);
            m = (i32)silva_valor_lista_numerus(parametra);
            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(
                    parametra, i);
                SilvaValor pd;
                SilvaToken* p_lexema;
                constans SemanticaSymbolum* p_symbolum;
                s32 index;

                si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
                {
                    perge;
                }
                pd = silva_c89_parametrum_declarator(
                    _canonicus(v->datum.nodus));
                si (pd.genus != SILVA_VALOR_NODUS)
                {
                    perge;   /* (void) aut abstractum */
                }
                p_lexema = silva_c89_declaratoris_titulus(
                    _canonicus(pd.datum.nodus));
                p_symbolum = (p_lexema != NIHIL)
                    ? _symbolum_lexematis(d, p_lexema) : NIHIL;
                si (p_symbolum == NIHIL)
                {
                    perge;
                }
                {
                    s32 mt = _typus_medullae(p_symbolum->typus);
                    constans SemanticaSymbolum** sedes_p;

                    index = medulla_parametrum_addere(d->functio,
                        _ch_de_silva(p_symbolum->titulus),
                        (mt >= ZEPHYRUM) ? mt : MEDULLA_TYPUS_I64);
                    (vacuum)_sedem_creare(d, p_symbolum, index,
                        FALSUM);
                    sedes_p = (constans SemanticaSymbolum**)
                        xar_addere(symbola_parametrorum);
                    si (sedes_p != NIHIL)
                    {
                        *sedes_p = p_symbolum;
                    }
                }
            }
        }

        /* bloccus primus */
        d->bloccus = _bloccum_novum(d, "initium");
        si (reditus_aggregatus)
        {
            _sistere(d, nodus, "nondum reditus aggregatus (M1b B)");
        }

        /* parametra capta inscriptione aut aggregata -> arca +
         * scribere valoris incipientis */
        m = xar_numerus(symbola_parametrorum);
        per (i = ZEPHYRUM; i < m; i++)
        {
            constans SemanticaSymbolum** sedes_p =
                (constans SemanticaSymbolum**)xar_obtinere(
                    symbola_parametrorum, i);
            constans SemanticaSymbolum* p_symbolum;
            DemissioSedes* sedes;
            s32 mt;

            si (sedes_p == NIHIL)
            {
                perge;
            }
            p_symbolum = *sedes_p;
            si (!_arcam_symboli(d, p_symbolum))
            {
                perge;
            }
            mt = _typus_medullae(p_symbolum->typus);
            si (mt < ZEPHYRUM)
            {
                _sistere(d, nodus,
                    "nondum parametrum aggregatum (M1b B)");
                perge;
            }
            sedes = _sedem_symboli(d, p_symbolum);
            {
                s32 mensura = _mensura_typi(d, p_symbolum->typus);
                s32 inscriptio = _registrum_temporarium(d);

                si (mensura <= ZEPHYRUM || sedes == NIHIL)
                {
                    perge;
                }
                (vacuum)_em(d, NIHIL, MEDULLA_OP_ARCA,
                    MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                    inscriptio,
                    medulla_op_immediatum((s64)mensura),
                    medulla_op_immediatum((s64)_exutus(
                        p_symbolum->typus)->ordinatio),
                    medulla_op_nihil());
                (vacuum)_em(d, NIHIL, MEDULLA_OP_SCRIBERE, mt,
                    MEDULLA_TYPUS_NIHIL, -I,
                    medulla_op_registrum(inscriptio),
                    medulla_op_registrum(sedes->index_registri),
                    medulla_op_nihil());
                sedes->index_registri = inscriptio;
                sedes->est_arca = VERUM;
            }
        }
    }

    /* locales: sedes + arca in initio */
    m = xar_numerus(locales);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaSymbolum** sedes_symboli =
            (constans SemanticaSymbolum**)xar_obtinere(locales, i);
        constans SemanticaSymbolum* symbolum_locale;

        si (sedes_symboli == NIHIL)
        {
            perge;
        }
        symbolum_locale = *sedes_symboli;
        si (_sedem_symboli(d, symbolum_locale) != NIHIL)
        {
            perge;   /* iam creatum */
        }
        si (_arcam_symboli(d, symbolum_locale))
        {
            s32 mensura = _mensura_typi(d, symbolum_locale->typus);
            s32 ordinatio;
            s32 inscriptio;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma localis ignota");
                perge;
            }
            ordinatio = (s32)_exutus(symbolum_locale->typus)
                ->ordinatio;
            inscriptio = _registrum_unicum(d,
                symbolum_locale->titulus);
            (vacuum)_em(d, NIHIL, MEDULLA_OP_ARCA,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                inscriptio, medulla_op_immediatum((s64)mensura),
                medulla_op_immediatum((s64)ordinatio),
                medulla_op_nihil());
            (vacuum)_sedem_creare(d, symbolum_locale, inscriptio,
                VERUM);
        }
        alioquin
        {
            s32 index = _registrum_unicum(d,
                symbolum_locale->titulus);

            (vacuum)_sedem_creare(d, symbolum_locale, index,
                FALSUM);
        }
    }

    /* parametra capta inscriptione: arca + copia valoris */
    {
        i32 n = xar_numerus(d->functio->parametra);

        per (i = ZEPHYRUM; i < (i32)n; i++)
        {
            /* invenire symbolum parametri cuius sedes directa est
             * sed captum est */
            SilvaValor pd_ignotum;

            pd_ignotum.genus = SILVA_VALOR_NIHIL;
            (vacuum)pd_ignotum;
        }
    }

    /* corpus */
    _sententiam(d, corpus_v.datum.nodus);

    /* casus decidens: reditus implicitus */
    si (_bloccus_vivus(d))
    {
        si (mt_reditus >= ZEPHYRUM)
        {
            (vacuum)_em(d, NIHIL, MEDULLA_OP_REDDE,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_immediatum(0), medulla_op_nihil(),
                medulla_op_nihil());
        }
        alioquin
        {
            (vacuum)_em(d, NIHIL, MEDULLA_OP_REDDE,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_nihil(), medulla_op_nihil(),
                medulla_op_nihil());
        }
    }
    d->functio = NIHIL;
}

/* ==================================================
 * Radix
 * ================================================== */

interior vacuum
_radicis_elementum (Demissio* d, constans SilvaNodus* nodus)
{
    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
    {
        _functionem(d, nodus);
        redde;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_CONDITIONALIS)
    {
        SilvaValor rami = silva_c89_conditionalis_rami(nodus);
        i32 i;
        i32 m = (i32)silva_valor_lista_numerus(rami);

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(rami, i);

            si (v != NIHIL && v->genus == SILVA_VALOR_NODUS
                && v->datum.nodus->genus
                    == (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS)
            {
                SilvaValor contentum =
                    silva_c89_ramus_sumptus_contentum(
                        v->datum.nodus);
                i32 k;
                i32 n = (i32)silva_valor_lista_numerus(contentum);

                per (k = ZEPHYRUM; k < n; k++)
                {
                    SilvaValor* e = silva_valor_lista_obtinere(
                        contentum, (unsigned int)k);

                    si (e != NIHIL
                        && e->genus == SILVA_VALOR_NODUS)
                    {
                        _radicis_elementum(d, e->datum.nodus);
                    }
                }
            }
        }
        redde;
    }
    /* declarationes (data statica M1b C), cetera: praetermissa */
}

MedullaModulus*
demissio_currere (Piscina* piscina, constans SilvaParsura* parsura,
    SilvaSemantica* sem, chorda titulus_moduli)
{
    Demissio d;
    i32 i;
    i32 m;

    si (piscina == NIHIL || parsura == NIHIL || sem == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    memset(&d, ZEPHYRUM, magnitudo(Demissio));
    d.piscina = piscina;
    d.sem = sem;
    d.modulus = medulla_modulum_creare(piscina, titulus_moduli);
    d.bloccus = -I;
    d.frange_finis = -I;
    d.perge_finis = -I;
    si (d.modulus == NIHIL)
    {
        redde NIHIL;
    }
    /* tabula lexematum: lexema declarationis -> symbolum (sedes
     * declaratorum; usus per symbolum_nodi) */
    d.lexemata = tabula_dispersa_creare_chorda(piscina, CCLVI);
    si (d.lexemata == NIHIL)
    {
        redde NIHIL;
    }
    m = (i32)silva_c89_symbola_numerus(sem);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_per_indicem(sem, (unsigned int)i);

        si (symbolum != NIHIL && symbolum->lexema != NIHIL)
        {
            _tabulam_ponere(&d, d.lexemata,
                (constans vacuum*)symbolum->lexema,
                _sine_constante((constans vacuum*)symbolum));
        }
    }

    /* radix */
    {
        SilvaValor radix = parsura->commissio->radix;
        i32 n = (i32)silva_valor_lista_numerus(radix);

        per (i = ZEPHYRUM; i < n; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(radix,
                (unsigned int)i);

            si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
            {
                _radicis_elementum(&d, v->datum.nodus);
            }
        }
    }
    redde d.modulus;
}
