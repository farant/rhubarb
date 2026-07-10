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
    s32                         index_symboli_moduli;  /* staticum
                                                        * locale ->
                                                        * datum; -I */
} DemissioSedes;

nomen structura {
    b32 directum;               /* VERUM: index = registrum valoris */
    s32 index;                  /* aliter registrum inscriptionis */
    b32 valida;
} DemissioLocus;

nomen structura {
    constans SemanticaSymbolum* symbolum;
    constans SilvaNodus*        initiator;   /* NIHIL licet (mensura
                                              * completa acierum) */
} DemissioLocale;

nomen structura {
    Piscina*                piscina;
    SilvaSemantica*         sem;
    MedullaModulus*         modulus;
    MedullaFunctio*         functio;
    s32                     bloccus;
    s32                     numerator;    /* nomina bloccorum */
    s32                     frange_finis; /* -I extra */
    s32                     perge_finis;
    s32                     reditus_index;   /* registrum parametri
                                              * reditus aggregati
                                              * (conventio C7); -I */
    s32                     reditus_mensura;
    s32                     numerator_datorum;  /* chorda_N/zeri_N/
                                                 * congeries_N */
    SilvaPiscina*           piscina_silvae;  /* transitoria (decodere;
                                              * octeti statim copiati) */
    TabulaDispersa*         lexemata;     /* SilvaToken* -> symbolum */
    TabulaDispersa*         sedes;        /* symbolum -> DemissioSedes* */
    TabulaDispersa*         capti;        /* symbola inscriptione capta */
    chorda                  stirps;       /* "lib/chorda.c" ->
                                           * "chorda_c" (praefixum
                                           * staticorum plagulae) */
} Demissio;

/* prototypa mutuae recursionis */
interior s32 _mensura_elementi_monstratoris (Demissio* d,
    constans TypusC89* typus);
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

        si (v.genus == SILVA_VALOR_NODUS)
        {
            nodus = v.datum.nodus;
            perge;
        }
        si (v.genus == SILVA_VALOR_INDEX)
        {
            /* canonica = INDEX in interpretationes (species loci
             * INDEX - inventum fusoris: genus 54 residuum) */
            SilvaValor interpretationes =
                silva_c89_ambiguus_interpretationes(nodus);
            SilvaValor* electa;

            si (v.datum.index < ZEPHYRUM)
            {
                frange;
            }
            electa = silva_valor_lista_obtinere(interpretationes,
                (unsigned int)v.datum.index);
            si (electa != NIHIL
                && electa->genus == SILVA_VALOR_NODUS)
            {
                nodus = electa->datum.nodus;
                perge;
            }
        }
        frange;
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

/* octeti LE (deterministici - machinula LE) */
interior vacuum
_octetos_integri (s64 valor, i8* octeti, s32 mensura)
{
    s32 k;

    per (k = ZEPHYRUM; k < mensura; k++)
    {
        octeti[k] = (i8)((valor >> (k * VIII)) & 0xff);
    }
}

/* aestimator fluitantium staticus (aestimator publicus integralis
 * solum est): folium fluitans/integer, -, +, parenthesis, conversio */
interior b32
_fluitantem_aestimare (Demissio* d, constans SilvaNodus* nodus,
    f64* valor_fructus)
{
    s64 integrale = 0;

    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS)
    {
        SilvaChorda textus = _tok_textus(
            silva_c89_folium_fluitans_tok_valor(nodus));
        character littera[XL];

        si (textus.mensura == ZEPHYRUM || textus.mensura >= XL)
        {
            redde FALSUM;
        }
        memcpy(littera, textus.datum,
            (memoriae_index)textus.mensura);
        littera[textus.mensura] = '\0';
        *valor_fructus = strtod(littera, NIHIL);
        redde VERUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _fluitantem_aestimare(d, v.datum.nodus,
                   valor_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
    {
        SilvaValor v = silva_c89_conversio_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _fluitantem_aestimare(d, v.datum.nodus,
                   valor_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
    {
        SilvaChorda operator = _tok_textus(
            silva_c89_unarium_tok_operator(nodus));
        SilvaValor v = silva_c89_unarium_internum(nodus);
        f64 internum;

        si (v.genus != SILVA_VALOR_NODUS
            || !_fluitantem_aestimare(d, v.datum.nodus, &internum))
        {
            redde FALSUM;
        }
        si (_op_est(operator, "-"))
        {
            *valor_fructus = -internum;
            redde VERUM;
        }
        si (_op_est(operator, "+"))
        {
            *valor_fructus = internum;
            redde VERUM;
        }
        redde FALSUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_BINARIUM)
    {
        /* initiatores tabularum fluitantium (1.0/3.0 - inventum
         * fusoris, 47 moduli) */
        SilvaValor sin_v = silva_c89_binarium_sinister(nodus);
        SilvaValor dex_v = silva_c89_binarium_dexter(nodus);
        SilvaChorda operator = _tok_textus(
            silva_c89_binarium_tok_operator(nodus));
        f64 a;
        f64 b;

        si (sin_v.genus != SILVA_VALOR_NODUS
            || dex_v.genus != SILVA_VALOR_NODUS
            || !_fluitantem_aestimare(d, sin_v.datum.nodus, &a)
            || !_fluitantem_aestimare(d, dex_v.datum.nodus, &b))
        {
            redde FALSUM;
        }
        si (_op_est(operator, "+"))
        {
            *valor_fructus = a + b;
            redde VERUM;
        }
        si (_op_est(operator, "-"))
        {
            *valor_fructus = a - b;
            redde VERUM;
        }
        si (_op_est(operator, "*"))
        {
            *valor_fructus = a * b;
            redde VERUM;
        }
        si (_op_est(operator, "/") && b != 0.0)
        {
            *valor_fructus = a / b;
            redde VERUM;
        }
        redde FALSUM;
    }
    si (silva_c89_constans_aestimare(d->sem, nodus, &integrale))
    {
        *valor_fructus = (f64)integrale;
        redde VERUM;
    }
    redde FALSUM;
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

/* stirps moduli: "lib/chorda.c" -> "chorda_c" (vocabularium §II).
 * Inventum M2a in primo contactu nexus mundi: sine praefixo statica
 * plagulae + data anonyma trans modulos collidunt (M1b eas videre
 * non potuit - demissio solitaria numquam collidit). */
interior chorda
_stirpem_computare (Piscina* piscina, chorda titulus_moduli)
{
    i8 littera[LXIV];
    i32 initium = ZEPHYRUM;
    i32 scriptum = ZEPHYRUM;
    i32 i;
    chorda s;

    per (i = ZEPHYRUM; i < titulus_moduli.mensura; i++)
    {
        si (titulus_moduli.datum[i] == (i8)'/')
        {
            initium = i + I;
        }
    }
    per (i = initium; i < titulus_moduli.mensura
        && scriptum < (i32)(LXIV - I); i++)
    {
        i8 c = titulus_moduli.datum[i];
        b32 idoneus = (b32)((c >= (i8)'a' && c <= (i8)'z')
            || (c >= (i8)'A' && c <= (i8)'Z')
            || (c >= (i8)'0' && c <= (i8)'9')
            || c == (i8)'_');

        littera[scriptum] = idoneus ? c : (i8)'_';
        scriptum++;
    }
    s.datum = littera;
    s.mensura = scriptum;
    redde chorda_transcribere(s, piscina);
}

/* titulus symboli globalis - CANALIS UNICUS definitionis ET
 * referentiarum (aliter nexus frangitur): statica plagulae
 * (profunditas 0 + REPOSITIO_STATICA) praefixum stirpis accipiunt
 * ($stirps.titulus); externa titulum nudum servant. */
interior chorda
_titulum_symboli (Demissio* d, constans SemanticaSymbolum* symbolum)
{
    chorda titulus = _ch_de_silva(symbolum->titulus);

    si (symbolum->profunditas > ZEPHYRUM
        || (symbolum->repositio & REPOSITIO_STATICA) == ZEPHYRUM)
    {
        redde titulus;
    }
    {
        i8 littera[CXXVIII];
        chorda plenus;
        i32 caput_s = (d->stirps.mensura < XL)
            ? d->stirps.mensura : XL;
        i32 caput_t = (titulus.mensura < LXXX)
            ? titulus.mensura : LXXX;

        memcpy(littera, d->stirps.datum, (memoriae_index)caput_s);
        littera[caput_s] = (i8)'.';
        memcpy(littera + caput_s + I, titulus.datum,
            (memoriae_index)caput_t);
        plenus.datum = littera;
        plenus.mensura = caput_s + I + caput_t;
        redde chorda_transcribere(plenus, d->piscina);
    }
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

/* membrum tag per titulum -> offset + typus (forma computata) */
interior b32
_membrum_invenire (Demissio* d, constans TypusC89* typus,
    SilvaChorda titulus, s32* offset_fructus,
    constans TypusC89** typus_fructus)
{
    constans TypusC89* exutus = _exutus(typus);
    i32 i;

    si (exutus == NIHIL
        || (exutus->genus != TYPUS_C89_STRUCTURA
            && exutus->genus != TYPUS_C89_UNIO)
        || !exutus->datum.tag.completa)
    {
        redde FALSUM;
    }
    si (_mensura_typi(d, exutus) < ZEPHYRUM)
    {
        redde FALSUM;   /* forma incomputabilis */
    }
    per (i = ZEPHYRUM; i < (i32)exutus->datum.tag.numerus_membrorum;
         i++)
    {
        constans TypusC89Membrum* membrum =
            &exutus->datum.tag.membra[i];

        si (membrum->titulus.mensura == titulus.mensura
            && titulus.mensura > ZEPHYRUM
            && memcmp(membrum->titulus.datum, titulus.datum,
                   (memoriae_index)titulus.mensura) == ZEPHYRUM)
        {
            *offset_fructus = (s32)membrum->offset;
            *typus_fructus = membrum->typus;
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_est_aggregatum (constans TypusC89* typus)
{
    constans TypusC89* exutus = _exutus(typus);

    redde exutus != NIHIL
        && (exutus->genus == TYPUS_C89_STRUCTURA
            || exutus->genus == TYPUS_C89_UNIO);
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
    sedes->index_symboli_moduli = -I;
    _tabulam_ponere(d, d->sedes, (constans vacuum*)symbolum,
        (vacuum*)sedes);
    redde sedes;
}

/* ==================================================
 * Data statica (M1b C): imagines + relocationes.
 * Relocatio ADDITIVA: locellus = inscriptio symboli + octeti
 * priores (addendum in imagine).
 * ================================================== */

/* mensura completa: acies incompleta (x[] = ...) ab initiatore
 * completur - parca semanticae "initiatores non probantur"
 * (inventum fusoris: hic_manens i8 lit[] = "...") */
interior s32
_mensura_completa (Demissio* d, constans TypusC89* typus,
    constans SilvaNodus* initiator)
{
    constans TypusC89* exutus = _exutus(typus);
    s32 mensura = _mensura_typi(d, typus);

    si (mensura > ZEPHYRUM || exutus == NIHIL
        || exutus->genus != TYPUS_C89_ACIES
        || exutus->datum.acies.numerus >= ZEPHYRUM
        || initiator == NIHIL)
    {
        redde mensura;
    }
    initiator = _canonicus(initiator);
    {
        s32 mensura_elementi = _mensura_typi(d,
            exutus->datum.acies.elementum);

        si (mensura_elementi <= ZEPHYRUM || initiator == NIHIL)
        {
            redde -I;
        }
        si (initiator->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
        {
            SilvaChorda octeti;

            si (d->piscina_silvae == NIHIL
                || !silva_c89_chorda_decodere(d->piscina_silvae,
                       initiator, &octeti))
            {
                redde -I;
            }
            redde ((s32)octeti.mensura + I) * mensura_elementi;
        }
        si (initiator->genus == (s32)SILVA_C89_GENUS_CONGERIES)
        {
            SilvaValor elementa = silva_c89_congeries_elementa(
                initiator);
            i32 i;
            i32 m = (i32)silva_valor_lista_numerus(elementa);
            s32 numerus = ZEPHYRUM;

            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(elementa,
                    (unsigned int)i);

                si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
                {
                    numerus++;
                }
            }
            si (numerus == ZEPHYRUM)
            {
                redde -I;
            }
            redde numerus * mensura_elementi;
        }
    }
    redde -I;
}

/* ordinatio (acies incompleta: elementi) */
interior s32
_ordinatio_typi (Demissio* d, constans TypusC89* typus)
{
    constans TypusC89* exutus = _exutus(typus);

    si (exutus == NIHIL)
    {
        redde I;
    }
    si (exutus->genus == TYPUS_C89_ACIES
        && exutus->datum.acies.numerus < ZEPHYRUM)
    {
        exutus = _exutus(exutus->datum.acies.elementum);
        si (exutus == NIHIL)
        {
            redde I;
        }
    }
    si (_mensura_typi(d, exutus) <= ZEPHYRUM)
    {
        redde I;
    }
    redde (s32)exutus->ordinatio;
}

/* nota sine contextu functionis: causa internata = signum
 * classificatum moduli (fusor eas numerat) */
interior vacuum
_notare (Demissio* d, constans SilvaNodus* nodus,
    constans character* causa)
{
    si (d->functio != NIHIL)
    {
        _sistere(d, nodus, causa);
    }
    alioquin
    {
        (vacuum)medulla_causam_internare(d->modulus,
            _ch_literis(causa));
    }
}

interior MedullaDatum*
_datum_moduli (constans Demissio* d, s32 index_symboli)
{
    constans MedullaSymbolum* symbolum = medulla_symbolum_obtinere(
        d->modulus, index_symboli);
    MedullaDatum** locus_dati;

    si (symbolum == NIHIL
        || symbolum->genus != (s32)MEDULLA_SYMBOLUM_DATUM)
    {
        redde NIHIL;
    }
    locus_dati = (MedullaDatum**)xar_obtinere_s(d->modulus->data,
        symbolum->index);
    redde (locus_dati != NIHIL) ? *locus_dati : NIHIL;
}

interior MedullaDatum*
_datum_invenire_aut_creare (Demissio* d, chorda titulus,
    s32 mensura, s32 ordinatio)
{
    s32 index_symboli = medulla_symbolum_internare(d->modulus,
        titulus);
    constans MedullaSymbolum* symbolum;

    si (index_symboli < ZEPHYRUM)
    {
        redde NIHIL;
    }
    symbolum = medulla_symbolum_obtinere(d->modulus, index_symboli);
    si (symbolum->genus == (s32)MEDULLA_SYMBOLUM_DATUM)
    {
        redde _datum_moduli(d, index_symboli);
    }
    si (symbolum->genus != (s32)MEDULLA_SYMBOLUM_EXTERNUM)
    {
        redde NIHIL;   /* functio eodem titulo */
    }
    redde medulla_datum_creare(d->modulus, titulus, (i32)mensura,
        (i32)ordinatio);
}

/* data anonyma per se privata moduli sunt -> praefixum stirpis
 * semper ($stirps.chorda_N; inventum M2a: sine eo chorda_0 in
 * omnibus modulis collidit) */
interior chorda
_titulum_dati (Demissio* d, constans character* basis)
{
    character littera[CXXVIII];
    chorda temporarium;
    s32 longitudo = (s32)sprintf(littera, "%.*s.%s_%d",
        (int)((d->stirps.mensura < XL) ? d->stirps.mensura : XL),
        (constans character*)d->stirps.datum, basis,
        (int)d->numerator_datorum);

    d->numerator_datorum++;
    temporarium.datum = (i8*)littera;
    temporarium.mensura = (i32)longitudo;
    redde chorda_transcribere(temporarium, d->piscina);
}

/* chorda litteralis -> datum anonymum (octeti decodati + NUL;
 * mensura data si maior). -I in errore. */
interior s32
_datum_chordae_mensurae (Demissio* d, constans SilvaNodus* nodus,
    s32 mensura_data)
{
    SilvaChorda octeti;
    chorda titulus;
    MedullaDatum* datum;
    s32 mensura;
    chorda visus;

    si (d->piscina_silvae == NIHIL
        || !silva_c89_chorda_decodere(d->piscina_silvae, nodus,
               &octeti))
    {
        redde -I;
    }
    mensura = (mensura_data > (s32)octeti.mensura)
        ? mensura_data : (s32)octeti.mensura + I;   /* + NUL */
    titulus = _titulum_dati(d, "chorda");
    datum = medulla_datum_creare(d->modulus, titulus, (i32)mensura,
        I);
    si (datum == NIHIL)
    {
        redde -I;
    }
    visus.datum = octeti.datum;
    visus.mensura = octeti.mensura;
    si (visus.mensura > ZEPHYRUM)
    {
        (vacuum)medulla_datum_scribere(datum, ZEPHYRUM, visus.datum,
            visus.mensura);
    }
    redde medulla_symbolum_internare(d->modulus, titulus);
}

interior s32
_datum_chordae (Demissio* d, constans SilvaNodus* nodus)
{
    redde _datum_chordae_mensurae(d, nodus, ZEPHYRUM);
}

/* locus staticus: identificator globalis / functio, cum catena
 * accessus/subscriptionis constantis -> symbolum + addendum */
interior b32
_locum_staticum (Demissio* d, constans SilvaNodus* nodus,
    s32* symbolum_fructus, s64* addendum_fructus)
{
    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _locum_staticum(d, v.datum.nodus, symbolum_fructus,
                   addendum_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_nodi(d->sem, nodus);

        si (symbolum == NIHIL || symbolum->profunditas > ZEPHYRUM)
        {
            redde FALSUM;   /* localia non statice */
        }
        *symbolum_fructus = medulla_symbolum_internare(d->modulus,
            _titulum_symboli(d, symbolum));
        *addendum_fructus = 0;
        redde *symbolum_fructus >= ZEPHYRUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_ACCESSUS)
    {
        SilvaValor basis_v = silva_c89_accessus_basis(nodus);
        SilvaChorda operator = _tok_textus(
            silva_c89_accessus_tok_operator(nodus));
        SilvaChorda titulus = _tok_textus(
            silva_c89_accessus_tok_titulus(nodus));
        s32 offset = ZEPHYRUM;
        constans TypusC89* typus_membri = NIHIL;

        si (basis_v.genus != SILVA_VALOR_NODUS
            || !_op_est(operator, "."))
        {
            redde FALSUM;
        }
        si (!_locum_staticum(d, basis_v.datum.nodus,
                symbolum_fructus, addendum_fructus))
        {
            redde FALSUM;
        }
        si (!_membrum_invenire(d, silva_c89_typus_expressionis(
                d->sem, _canonicus(basis_v.datum.nodus)), titulus,
                &offset, &typus_membri))
        {
            redde FALSUM;
        }
        *addendum_fructus += (s64)offset;
        redde VERUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_SUBSCRIPTIO)
    {
        SilvaValor basis_v = silva_c89_subscriptio_basis(nodus);
        SilvaValor index_v = silva_c89_subscriptio_index(nodus);
        s64 index = 0;
        s32 mensura;

        si (basis_v.genus != SILVA_VALOR_NODUS
            || index_v.genus != SILVA_VALOR_NODUS
            || !silva_c89_constans_aestimare(d->sem,
                   index_v.datum.nodus, &index))
        {
            redde FALSUM;
        }
        si (!_locum_staticum(d, basis_v.datum.nodus,
                symbolum_fructus, addendum_fructus))
        {
            redde FALSUM;
        }
        mensura = _mensura_elementi_monstratoris(d,
            _typus_finalis(d, _canonicus(basis_v.datum.nodus)));
        si (mensura <= ZEPHYRUM)
        {
            redde FALSUM;
        }
        *addendum_fructus += index * (s64)mensura;
        redde VERUM;
    }
    redde FALSUM;
}

/* constans inscriptionis pro imagine statica: &..., functio,
 * acies (lapsus), chorda litteralis */
interior b32
_inscriptio_statica (Demissio* d, constans SilvaNodus* nodus,
    s32* symbolum_fructus, s64* addendum_fructus)
{
    nodus = _canonicus(nodus);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        SilvaValor v = silva_c89_parenthesis_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _inscriptio_statica(d, v.datum.nodus,
                   symbolum_fructus, addendum_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
    {
        SilvaValor v = silva_c89_conversio_internum(nodus);

        redde v.genus == SILVA_VALOR_NODUS
            && _inscriptio_statica(d, v.datum.nodus,
                   symbolum_fructus, addendum_fructus);
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
    {
        s32 index_symboli = _datum_chordae(d, nodus);

        si (index_symboli < ZEPHYRUM)
        {
            redde FALSUM;
        }
        *symbolum_fructus = index_symboli;
        *addendum_fructus = 0;
        redde VERUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
    {
        SilvaChorda operator = _tok_textus(
            silva_c89_unarium_tok_operator(nodus));
        SilvaValor v = silva_c89_unarium_internum(nodus);

        redde _op_est(operator, "&")
            && v.genus == SILVA_VALOR_NODUS
            && _locum_staticum(d, v.datum.nodus, symbolum_fructus,
                   addendum_fructus);
    }
    /* identificator functionis / aciei (lapsus) */
    redde _locum_staticum(d, nodus, symbolum_fructus,
        addendum_fructus);
}

/* imaginem staticam scribere (recursiva); FALSUM = inaestimabilis
 * (vocans notat) */
interior b32
_imaginem_scribere (Demissio* d, MedullaDatum* datum, s32 offset,
    constans TypusC89* typus, constans SilvaNodus* initiator)
{
    constans TypusC89* exutus = _exutus(typus);

    initiator = _canonicus(initiator);
    si (exutus == NIHIL || initiator == NIHIL)
    {
        redde FALSUM;
    }
    si (initiator->genus == (s32)SILVA_C89_GENUS_CONGERIES)
    {
        SilvaValor elementa = silva_c89_congeries_elementa(
            initiator);
        i32 i;
        i32 m = (i32)silva_valor_lista_numerus(elementa);
        s32 a = ZEPHYRUM;
        b32 bene = VERUM;

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(elementa,
                (unsigned int)i);
            constans TypusC89* finis = NIHIL;
            s32 offset_elementi = offset;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            si (exutus->genus == TYPUS_C89_ACIES)
            {
                s32 mensura = _mensura_typi(d,
                    exutus->datum.acies.elementum);

                si (mensura <= ZEPHYRUM)
                {
                    redde FALSUM;
                }
                finis = exutus->datum.acies.elementum;
                offset_elementi = offset + a * mensura;
            }
            alioquin si (exutus->genus == TYPUS_C89_STRUCTURA)
            {
                si (_mensura_typi(d, exutus) < ZEPHYRUM
                    || a >= (s32)exutus->datum.tag.numerus_membrorum)
                {
                    redde FALSUM;
                }
                finis = exutus->datum.tag.membra[a].typus;
                offset_elementi = offset
                    + (s32)exutus->datum.tag.membra[a].offset;
            }
            alioquin si (exutus->genus == TYPUS_C89_UNIO)
            {
                si (a > ZEPHYRUM)
                {
                    frange;   /* membrum primum solum */
                }
                si (_mensura_typi(d, exutus) < ZEPHYRUM
                    || exutus->datum.tag.numerus_membrorum
                        == ZEPHYRUM)
                {
                    redde FALSUM;
                }
                finis = exutus->datum.tag.membra[ZEPHYRUM].typus;
            }
            alioquin
            {
                finis = exutus;   /* scalaris uncis vestitus */
            }
            si (!_imaginem_scribere(d, datum, offset_elementi,
                    finis, v->datum.nodus))
            {
                bene = FALSUM;
            }
            a++;
        }
        redde bene;
    }
    /* chorda in aciem characterum */
    si (initiator->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA
        && exutus->genus == TYPUS_C89_ACIES)
    {
        SilvaChorda octeti;

        si (d->piscina_silvae == NIHIL
            || !silva_c89_chorda_decodere(d->piscina_silvae,
                   initiator, &octeti))
        {
            redde FALSUM;
        }
        si (octeti.mensura > ZEPHYRUM)
        {
            redde medulla_datum_scribere(datum, (i32)offset,
                octeti.datum, octeti.mensura);
        }
        redde VERUM;
    }
    /* folium scalare */
    {
        s32 mt = _typus_medullae(exutus);
        s32 mensura = _mensura_typi(d, exutus);
        i8 octeti[VIII];
        s64 valor = 0;

        si (mt < ZEPHYRUM || mensura <= ZEPHYRUM
            || mensura > VIII)
        {
            redde FALSUM;
        }
        si (_fluitans_m(mt))
        {
            f64 fluitans_valor = 0.0;

            si (!_fluitantem_aestimare(d, initiator,
                    &fluitans_valor))
            {
                redde FALSUM;
            }
            si (mt == (s32)MEDULLA_TYPUS_F32)
            {
                unio { f32 v; i8 o[IV]; } u;

                u.v = (f32)fluitans_valor;
                memcpy(octeti, u.o, IV);
            }
            alioquin
            {
                unio { f64 v; i8 o[VIII]; } u;

                u.v = fluitans_valor;
                memcpy(octeti, u.o, VIII);
            }
            redde medulla_datum_scribere(datum, (i32)offset, octeti,
                (i32)mensura);
        }
        si (silva_c89_constans_aestimare(d->sem, initiator, &valor))
        {
            _octetos_integri(valor, octeti, mensura);
            redde medulla_datum_scribere(datum, (i32)offset, octeti,
                (i32)mensura);
        }
        /* constans inscriptionis (relocatio additiva) */
        si (exutus->genus == TYPUS_C89_MONSTRATOR
            || mensura == VIII)
        {
            s32 index_symboli = -I;
            s64 addendum = 0;

            si (_inscriptio_statica(d, initiator, &index_symboli,
                    &addendum))
            {
                _octetos_integri(addendum, octeti, VIII);
                (vacuum)medulla_datum_scribere(datum, (i32)offset,
                    octeti, VIII);
                redde medulla_relocationem_addere(datum,
                    (i32)offset, index_symboli);
            }
        }
        /* constans nulla monstratoris: NULL = ((void*)0) - aestimator
         * publicus conversiones non-integrales recusat (inventum
         * fusoris, 46 moduli). Involucra exuta, nucleus aestimatur. */
        {
            constans SilvaNodus* nucleus = initiator;
            s32 custos = ZEPHYRUM;

            dum (nucleus != NIHIL && custos < XVI)
            {
                custos++;
                nucleus = _canonicus(nucleus);
                si (nucleus->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
                {
                    SilvaValor v = silva_c89_conversio_internum(
                        nucleus);

                    nucleus = (v.genus == SILVA_VALOR_NODUS)
                        ? v.datum.nodus : NIHIL;
                    perge;
                }
                si (nucleus->genus
                    == (s32)SILVA_C89_GENUS_PARENTHESIS)
                {
                    SilvaValor v = silva_c89_parenthesis_internum(
                        nucleus);

                    nucleus = (v.genus == SILVA_VALOR_NODUS)
                        ? v.datum.nodus : NIHIL;
                    perge;
                }
                frange;
            }
            si (nucleus != NIHIL
                && silva_c89_constans_aestimare(d->sem, nucleus,
                       &valor))
            {
                _octetos_integri(valor, octeti, mensura);
                redde medulla_datum_scribere(datum, (i32)offset,
                    octeti, (i32)mensura);
            }
        }
        redde FALSUM;
    }
}

/* probatio puritatis: initiator totus statice aestimabilis? (sine
 * effectibus - chordae aestimabiles censentur) */
interior b32
_constans_est (Demissio* d, constans TypusC89* typus,
    constans SilvaNodus* initiator)
{
    constans TypusC89* exutus = _exutus(typus);

    initiator = _canonicus(initiator);
    si (exutus == NIHIL || initiator == NIHIL)
    {
        redde FALSUM;
    }
    si (initiator->genus == (s32)SILVA_C89_GENUS_CONGERIES)
    {
        SilvaValor elementa = silva_c89_congeries_elementa(
            initiator);
        i32 i;
        i32 m = (i32)silva_valor_lista_numerus(elementa);
        s32 a = ZEPHYRUM;

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(elementa,
                (unsigned int)i);
            constans TypusC89* finis = NIHIL;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            si (exutus->genus == TYPUS_C89_ACIES)
            {
                finis = exutus->datum.acies.elementum;
            }
            alioquin si (exutus->genus == TYPUS_C89_STRUCTURA)
            {
                si (_mensura_typi(d, exutus) < ZEPHYRUM
                    || a >= (s32)exutus->datum.tag.numerus_membrorum)
                {
                    redde FALSUM;
                }
                finis = exutus->datum.tag.membra[a].typus;
            }
            alioquin si (exutus->genus == TYPUS_C89_UNIO)
            {
                si (a > ZEPHYRUM)
                {
                    frange;
                }
                si (exutus->datum.tag.numerus_membrorum == ZEPHYRUM)
                {
                    redde FALSUM;
                }
                finis = exutus->datum.tag.membra[ZEPHYRUM].typus;
            }
            alioquin
            {
                finis = exutus;
            }
            si (!_constans_est(d, finis, v->datum.nodus))
            {
                redde FALSUM;
            }
            a++;
        }
        redde VERUM;
    }
    si (initiator->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
    {
        redde VERUM;
    }
    {
        s32 mt = _typus_medullae(exutus);
        s64 valor = 0;
        f64 fluitans_valor = 0.0;

        si (mt < ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (_fluitans_m(mt))
        {
            redde _fluitantem_aestimare(d, initiator,
                &fluitans_valor);
        }
        si (silva_c89_constans_aestimare(d->sem, initiator, &valor))
        {
            redde VERUM;
        }
        /* &globalis, functio, chorda - resolutio sine creatione
         * TANTUM per probationem locorum: identificator/&-catena
         * (chordae supra iam VERUM) */
        si (exutus->genus == TYPUS_C89_MONSTRATOR)
        {
            constans SilvaNodus* probandus = initiator;

            si (probandus->genus == (s32)SILVA_C89_GENUS_UNARIUM)
            {
                SilvaValor v = silva_c89_unarium_internum(
                    probandus);

                si (_op_est(_tok_textus(
                        silva_c89_unarium_tok_operator(probandus)),
                        "&")
                    && v.genus == SILVA_VALOR_NODUS)
                {
                    s32 ignotum_s = -I;
                    s64 ignotum_a = 0;

                    redde _locum_staticum(d, v.datum.nodus,
                        &ignotum_s, &ignotum_a);
                }
                redde FALSUM;
            }
            {
                s32 ignotum_s = -I;
                s64 ignotum_a = 0;

                redde _locum_staticum(d, probandus, &ignotum_s,
                    &ignotum_a);
            }
        }
        redde FALSUM;
    }
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
    si (sedes != NIHIL && sedes->index_symboli_moduli >= ZEPHYRUM)
    {
        /* staticum locale: datum moduli */
        s32 inscriptio = _em(d, nodus, MEDULLA_OP_LOCUS,
            MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d),
            medulla_op_symbolum(sedes->index_symboli_moduli),
            medulla_op_nihil(), medulla_op_nihil());
        s32 mt = _typus_medullae(naturalis);

        si (mt >= ZEPHYRUM
            && _exutus(naturalis)->genus != TYPUS_C89_ACIES)
        {
            redde _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                medulla_op_registrum(inscriptio),
                medulla_op_nihil(), medulla_op_nihil());
        }
        redde inscriptio;
    }
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
        _sistere(d, nodus, "staticum ante declarationem");
        redde _registrum_temporarium(d);
    }
    {
        s32 index_symboli = medulla_symbolum_internare(d->modulus,
            _titulum_symboli(d, symbolum));
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
    si (locus.valida && !locus.directum && mt < ZEPHYRUM
        && _est_aggregatum(sin_naturalis) && _op_est(operator, "="))
    {
        /* assignatio aggregati -> copia (valor = inscriptio) */
        s32 mensura = _mensura_typi(d, sin_naturalis);
        s32 fons;

        si (mensura <= ZEPHYRUM)
        {
            _sistere(d, nodus, "forma aggregati ignota");
            redde _registrum_temporarium(d);
        }
        fons = _ut_valor(d, dex_v.datum.nodus);
        (vacuum)_em(d, nodus, MEDULLA_OP_COPIA, MEDULLA_TYPUS_NIHIL,
            MEDULLA_TYPUS_NIHIL, -I,
            medulla_op_registrum(locus.index),
            medulla_op_registrum(fons),
            medulla_op_immediatum((s64)mensura));
        redde locus.index;
    }
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

interior s32
_vocationem (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor functio_v = silva_c89_vocatio_functio(nodus);
    SilvaValor argumenta_v = silva_c89_vocatio_argumenta(nodus);
    constans SilvaNodus* nf;
    constans TypusC89* typus_vocati;
    constans TypusC89* reditus;
    MedullaOperandum vocatus;
    MedullaOperandum argumenta[LXIV];
    s32 numerus_argumentorum = ZEPHYRUM;
    b32 reditus_aggregatus = FALSUM;
    s32 arca_reditus = -I;
    s32 mt_reditus;

    si (functio_v.genus != SILVA_VALOR_NODUS)
    {
        _sistere(d, nodus, "vocatio sine vocato");
        redde _registrum_temporarium(d);
    }
    nf = _canonicus(functio_v.datum.nodus);

    /* typus functionis (monstrator ad functionem exuitur) */
    typus_vocati = _exutus(_typus_finalis(d, nf));
    si (typus_vocati != NIHIL
        && typus_vocati->genus == TYPUS_C89_MONSTRATOR)
    {
        typus_vocati = _exutus(
            typus_vocati->datum.monstrator.internum);
    }
    si (typus_vocati == NIHIL
        || typus_vocati->genus != TYPUS_C89_FUNCTIO)
    {
        _sistere(d, nodus, "vocatio sine typo functionis");
        redde _registrum_temporarium(d);
    }
    reditus = _exutus(typus_vocati->datum.functio.reditus);
    mt_reditus = _typus_medullae(reditus);
    reditus_aggregatus = _est_aggregatum(reditus);

    /* vocatus: $symbolum directum si identificator functionis;
     * aliter valor (monstrator functionis) */
    vocatus = medulla_op_nihil();
    si (nf->genus == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_nodi(d->sem, nf);
        constans TypusC89* typus_symboli = (symbolum != NIHIL)
            ? _exutus(symbolum->typus) : NIHIL;

        si (typus_symboli != NIHIL
            && typus_symboli->genus == TYPUS_C89_FUNCTIO)
        {
            vocatus = medulla_op_symbolum(
                medulla_symbolum_internare(d->modulus,
                    _titulum_symboli(d, symbolum)));
        }
    }
    si (vocatus.genus == (s32)MEDULLA_OPERANDUM_NIHIL)
    {
        vocatus = medulla_op_registrum(_ut_valor(d,
            functio_v.datum.nodus));
    }

    /* reditus aggregatus: arca destinationis = argumentum primum
     * EXPLICITUM (conventio C7 - demissio semantica visibilia
     * facit) */
    si (reditus_aggregatus)
    {
        s32 mensura = _mensura_typi(d, reditus);

        si (mensura <= ZEPHYRUM)
        {
            _sistere(d, nodus, "forma reditus ignota");
            redde _registrum_temporarium(d);
        }
        arca_reditus = _em(d, nodus, MEDULLA_OP_ARCA,
            MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d),
            medulla_op_immediatum((s64)mensura),
            medulla_op_immediatum(
                (s64)_exutus(reditus)->ordinatio),
            medulla_op_nihil());
        argumenta[numerus_argumentorum] =
            medulla_op_registrum(arca_reditus);
        numerus_argumentorum++;
    }

    /* argumenta (conversiones/promotiones variadicae iam
     * annotatae; virgulae intersertae praetermissae) */
    {
        i32 i;
        i32 m = (i32)silva_valor_lista_numerus(argumenta_v);

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(argumenta_v,
                (unsigned int)i);
            constans SilvaNodus* argumentum;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            si (numerus_argumentorum >= (s32)LXIV)
            {
                _sistere(d, nodus, "argumenta nimia");
                frange;
            }
            argumentum = v->datum.nodus;
            si (_est_aggregatum(_typus_finalis(d,
                    _canonicus(argumentum))))
            {
                /* valore: copia in arcam temporariam vocantis */
                constans TypusC89* typus_argumenti = _exutus(
                    _typus_finalis(d, _canonicus(argumentum)));
                s32 mensura = _mensura_typi(d, typus_argumenti);
                s32 fons;
                s32 temporaria;

                si (mensura <= ZEPHYRUM)
                {
                    _sistere(d, nodus, "forma argumenti ignota");
                    perge;
                }
                fons = _ut_valor(d, argumentum);
                temporaria = _em(d, nodus, MEDULLA_OP_ARCA,
                    MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                    _registrum_temporarium(d),
                    medulla_op_immediatum((s64)mensura),
                    medulla_op_immediatum(
                        (s64)typus_argumenti->ordinatio),
                    medulla_op_nihil());
                (vacuum)_em(d, nodus, MEDULLA_OP_COPIA,
                    MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                    medulla_op_registrum(temporaria),
                    medulla_op_registrum(fons),
                    medulla_op_immediatum((s64)mensura));
                argumenta[numerus_argumentorum] =
                    medulla_op_registrum(temporaria);
            }
            alioquin
            {
                argumenta[numerus_argumentorum] = _ut_operandum(d,
                    argumentum);
            }
            numerus_argumentorum++;
        }
    }

    /* emissio (stiva UNO ictu appensa - vocationes nidificatae in
     * argumentis stivam propriam iam appenderunt) */
    {
        MedullaInstructio instructio;
        s32 destinatio = -I;
        s32 typus = MEDULLA_TYPUS_NIHIL;

        si (!reditus_aggregatus && mt_reditus >= ZEPHYRUM)
        {
            destinatio = _registrum_temporarium(d);
            typus = mt_reditus;
        }
        _vivum(d);
        memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
        instructio.op = MEDULLA_OP_VOCARE;
        instructio.typus = typus;
        instructio.typus_secundus = MEDULLA_TYPUS_NIHIL;
        instructio.destinatio = destinatio;
        instructio.a = vocatus;
        instructio.b = medulla_op_nihil();
        instructio.c = medulla_op_nihil();
        instructio.extra_index = medulla_operanda_addere(d->functio,
            argumenta, numerus_argumentorum);
        instructio.extra_numerus = numerus_argumentorum;
        instructio.origo = nodus;
        (vacuum)medulla_emittere(d->functio, d->bloccus,
            &instructio);
        si (reditus_aggregatus)
        {
            redde arca_reditus;
        }
        si (destinatio >= ZEPHYRUM)
        {
            redde destinatio;
        }
        redde _registrum_temporarium(d);   /* vacuum */
    }
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

            /* NON redde directum: conversio annotata HUIUS nodi
             * (cauda functionis) applicanda - (s64)(a*b) casus
             * pecuniae sputnik (inventum M2d) */
            fructus = _expressionem(d, v.datum.nodus);
            frange;
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
        {
            /* NON per aestimatorem: scopi post analysim clausi -
             * expressiones locales ibi non solvuntur (inventum
             * fusoris). Typus internae in typationibus UNdecayed
             * (constructione naturalis) -> mensura per formam. */
            SilvaValor internum_v =
                silva_c89_magnitudo_expressionis_internum(nodus);
            constans TypusC89* typus_interni =
                (internum_v.genus == SILVA_VALOR_NODUS)
                ? silva_c89_typus_expressionis(d->sem,
                      _canonicus(internum_v.datum.nodus)) : NIHIL;
            s32 mensura = _mensura_typi(d, typus_interni);

            si (mensura > ZEPHYRUM)
            {
                fructus = _movere(d, nodus, MEDULLA_TYPUS_I64,
                    _registrum_temporarium(d),
                    medulla_op_immediatum((s64)mensura));
            }
            alioquin
            {
                _sistere(d, nodus, "magnitudo inaestimabilis");
                fructus = _registrum_temporarium(d);
            }
            frange;
        }
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
            fructus = _vocationem(d, nodus);
            frange;
        }
        casus (s32)SILVA_C89_GENUS_ACCESSUS:
        casus (s32)SILVA_C89_GENUS_SUBSCRIPTIO:
        {
            DemissioLocus locus = _ut_locum(d, nodus);

            si (!locus.valida)
            {
                redde _registrum_temporarium(d);   /* iam sistit */
            }
            {
                constans TypusC89* exutus = _exutus(naturalis);
                s32 mt = _typus_medullae(naturalis);

                si (mt >= ZEPHYRUM && exutus != NIHIL
                    && exutus->genus != TYPUS_C89_ACIES
                    && exutus->genus != TYPUS_C89_FUNCTIO)
                {
                    fructus = _em(d, nodus, MEDULLA_OP_LEGERE, mt,
                        MEDULLA_TYPUS_NIHIL,
                        _registrum_temporarium(d),
                        medulla_op_registrum(locus.index),
                        medulla_op_nihil(), medulla_op_nihil());
                }
                alioquin
                {
                    fructus = locus.index;   /* inscriptio */
                }
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHORDA:
        {
            s32 index_symboli = _datum_chordae(d, nodus);

            si (index_symboli < ZEPHYRUM)
            {
                _sistere(d, nodus, "chorda indecodabilis");
                redde _registrum_temporarium(d);
            }
            fructus = _em(d, nodus, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());
            frange;
        }
        casus (s32)SILVA_C89_GENUS_CONGERIES:
        {
            _sistere(d, nodus, "nondum congeries (M1b C)");
            redde _registrum_temporarium(d);
        }
        ordinarius:
        {
            character causa[LXIV];

            sprintf(causa, "expressio generis ignoti (%d)",
                (int)nodus->genus);
            _sistere(d, nodus, causa);   /* internatur - copia */
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
        si (sedes != NIHIL
            && sedes->index_symboli_moduli >= ZEPHYRUM)
        {
            /* staticum locale */
            locus.directum = FALSUM;
            locus.index = _em(d, nodus, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(sedes->index_symboli_moduli),
                medulla_op_nihil(), medulla_op_nihil());
            locus.valida = VERUM;
            redde locus;
        }
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
            redde locus;   /* staticum ante declarationem? */
        }
        /* globale */
        {
            s32 index_symboli = medulla_symbolum_internare(
                d->modulus, _titulum_symboli(d, symbolum));

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
    si (nodus->genus == (s32)SILVA_C89_GENUS_SUBSCRIPTIO)
    {
        SilvaValor basis_v = silva_c89_subscriptio_basis(nodus);
        SilvaValor index_v = silva_c89_subscriptio_index(nodus);
        constans SilvaNodus* basis_n;
        constans SilvaNodus* index_n;
        constans TypusC89* basis_ft;
        s32 mensura;
        s32 basis_registrum;
        MedullaOperandum index_op;
        s32 scalatum;

        si (basis_v.genus != SILVA_VALOR_NODUS
            || index_v.genus != SILVA_VALOR_NODUS)
        {
            redde locus;
        }
        basis_n = basis_v.datum.nodus;
        index_n = index_v.datum.nodus;
        basis_ft = _typus_finalis(d, _canonicus(basis_n));
        si (!_est_monstratorius(basis_ft))
        {
            /* subscriptio commutativa: i[a] */
            constans SilvaNodus* commutatum = basis_n;

            basis_n = index_n;
            index_n = commutatum;
            basis_ft = _typus_finalis(d, _canonicus(basis_n));
        }
        mensura = _mensura_elementi_monstratoris(d, basis_ft);
        si (mensura <= ZEPHYRUM)
        {
            _sistere(d, nodus, "forma elementi ignota");
            redde locus;
        }
        basis_registrum = _ut_valor(d, basis_n);
        index_op = _ut_operandum(d, index_n);
        scalatum = _indicem_scalare(d, nodus, index_op,
            _typus_medullae(_typus_finalis(d, _canonicus(index_n))),
            mensura);
        locus.directum = FALSUM;
        locus.index = _em(d, nodus, MEDULLA_OP_ADDERE,
            MEDULLA_TYPUS_I64, MEDULLA_TYPUS_NIHIL,
            _registrum_temporarium(d),
            medulla_op_registrum(basis_registrum),
            medulla_op_registrum(scalatum), medulla_op_nihil());
        locus.valida = VERUM;
        redde locus;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_ACCESSUS)
    {
        SilvaValor basis_v = silva_c89_accessus_basis(nodus);
        SilvaChorda operator = _tok_textus(
            silva_c89_accessus_tok_operator(nodus));
        SilvaChorda titulus = _tok_textus(
            silva_c89_accessus_tok_titulus(nodus));
        constans TypusC89* typus_tag;
        s32 inscriptio_basis;
        s32 offset = ZEPHYRUM;
        constans TypusC89* typus_membri = NIHIL;

        si (basis_v.genus != SILVA_VALOR_NODUS)
        {
            redde locus;
        }
        si (_op_est(operator, "->"))
        {
            constans TypusC89* basis_ft = _exutus(_typus_finalis(d,
                _canonicus(basis_v.datum.nodus)));

            si (basis_ft == NIHIL
                || basis_ft->genus != TYPUS_C89_MONSTRATOR)
            {
                _sistere(d, nodus,
                    "accessus sagittae sine monstratore");
                redde locus;
            }
            typus_tag = basis_ft->datum.monstrator.internum;
            inscriptio_basis = _ut_valor(d, basis_v.datum.nodus);
        }
        alioquin
        {
            DemissioLocus locus_basis = _ut_locum(d,
                basis_v.datum.nodus);

            si (locus_basis.valida && !locus_basis.directum)
            {
                inscriptio_basis = locus_basis.index;
            }
            alioquin si (_est_aggregatum(_typus_finalis(d,
                    _canonicus(basis_v.datum.nodus))))
            {
                /* rvalor aggregatus (fructus vocationis, ...):
                 * valor aggregati EST inscriptio eius (conventio -
                 * vide _vocationem, _assignationem) */
                inscriptio_basis = _ut_valor(d,
                    basis_v.datum.nodus);
            }
            alioquin
            {
                _sistere(d, nodus,
                    "accessus sine inscriptione basis");
                redde locus;
            }
            typus_tag = silva_c89_typus_expressionis(d->sem,
                _canonicus(basis_v.datum.nodus));
        }
        si (!_membrum_invenire(d, typus_tag, titulus, &offset,
                &typus_membri))
        {
            _sistere(d, nodus, "accessus structurae incompletae");
            redde locus;
        }
        (vacuum)typus_membri;
        locus.directum = FALSUM;
        locus.index = (offset == ZEPHYRUM) ? inscriptio_basis
            : _em(d, nodus, MEDULLA_OP_ADDERE, MEDULLA_TYPUS_I64,
                  MEDULLA_TYPUS_NIHIL, _registrum_temporarium(d),
                  medulla_op_registrum(inscriptio_basis),
                  medulla_op_immediatum((s64)offset),
                  medulla_op_nihil());
        locus.valida = VERUM;
        redde locus;
    }
    redde locus;
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

/* congeries localis: imagine statica + copia si constans;
 * aliter zeri + copia + scripturae elementorum */
interior vacuum
_congeriem_localem (Demissio* d, s32 inscriptio,
    constans TypusC89* typus, constans SilvaNodus* congeries)
{
    constans TypusC89* exutus = _exutus(typus);
    SilvaValor elementa = silva_c89_congeries_elementa(congeries);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(elementa);
    s32 a = ZEPHYRUM;

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(elementa,
            (unsigned int)i);
        constans TypusC89* finis = NIHIL;
        s32 offset = ZEPHYRUM;
        constans SilvaNodus* elementum;

        si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        elementum = _canonicus(v->datum.nodus);
        si (exutus->genus == TYPUS_C89_ACIES)
        {
            s32 mensura = _mensura_typi(d,
                exutus->datum.acies.elementum);

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, congeries, "forma elementi ignota");
                redde;
            }
            finis = exutus->datum.acies.elementum;
            offset = a * mensura;
        }
        alioquin si (exutus->genus == TYPUS_C89_STRUCTURA)
        {
            si (_mensura_typi(d, exutus) < ZEPHYRUM
                || a >= (s32)exutus->datum.tag.numerus_membrorum)
            {
                _sistere(d, congeries, "congeries ultra membra");
                redde;
            }
            finis = exutus->datum.tag.membra[a].typus;
            offset = (s32)exutus->datum.tag.membra[a].offset;
        }
        alioquin si (exutus->genus == TYPUS_C89_UNIO)
        {
            si (a > ZEPHYRUM)
            {
                frange;
            }
            finis = exutus->datum.tag.membra[ZEPHYRUM].typus;
        }
        alioquin
        {
            finis = exutus;
        }
        {
            s32 sedes_elementi = (offset == ZEPHYRUM) ? inscriptio
                : _em(d, elementum, MEDULLA_OP_ADDERE,
                      MEDULLA_TYPUS_I64, MEDULLA_TYPUS_NIHIL,
                      _registrum_temporarium(d),
                      medulla_op_registrum(inscriptio),
                      medulla_op_immediatum((s64)offset),
                      medulla_op_nihil());

            si (elementum->genus == (s32)SILVA_C89_GENUS_CONGERIES)
            {
                _congeriem_localem(d, sedes_elementi, finis,
                    elementum);
            }
            alioquin
            {
                s32 mt = _typus_medullae(finis);

                si (mt < ZEPHYRUM)
                {
                    _sistere(d, elementum,
                        "elementum congeriei non scalare");
                }
                alioquin
                {
                    MedullaOperandum fons = _ut_operandum(d,
                        elementum);

                    (vacuum)_em(d, elementum, MEDULLA_OP_SCRIBERE,
                        mt, MEDULLA_TYPUS_NIHIL, -I,
                        medulla_op_registrum(sedes_elementi), fons,
                        medulla_op_nihil());
                }
            }
        }
        a++;
    }
}

/* initiator localis non scalaris (congeries / chorda in aciem) */
interior vacuum
_initiatorem_aggregatum (Demissio* d, DemissioSedes* sedes,
    constans SemanticaSymbolum* symbolum,
    constans SilvaNodus* initiator)
{
    s32 mensura = _mensura_completa(d, symbolum->typus, initiator);
    constans SilvaNodus* canonicus = _canonicus(initiator);

    si (mensura <= ZEPHYRUM || !sedes->est_arca)
    {
        _sistere(d, initiator, "forma aggregati ignota");
        redde;
    }
    /* chorda in aciem characterum: datum plenum + copia */
    si (canonicus->genus == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA)
    {
        s32 index_symboli = _datum_chordae_mensurae(d, canonicus,
            mensura);

        si (index_symboli < ZEPHYRUM)
        {
            _sistere(d, initiator, "chorda indecodabilis");
            redde;
        }
        {
            s32 fons = _em(d, initiator, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());

            (vacuum)_em(d, initiator, MEDULLA_OP_COPIA,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_registrum(sedes->index_registri),
                medulla_op_registrum(fons),
                medulla_op_immediatum((s64)mensura));
        }
        redde;
    }
    si (canonicus->genus != (s32)SILVA_C89_GENUS_CONGERIES)
    {
        _sistere(d, initiator, "initiator aggregati ignotus");
        redde;
    }
    si (_constans_est(d, symbolum->typus, canonicus))
    {
        /* imago statica integra + copia una */
        chorda titulus = _titulum_dati(d, "congeries");
        MedullaDatum* datum = medulla_datum_creare(d->modulus,
            titulus, (i32)mensura,
            (i32)_exutus(symbolum->typus)->ordinatio);

        si (datum != NIHIL
            && _imaginem_scribere(d, datum, ZEPHYRUM,
                   symbolum->typus, canonicus))
        {
            s32 index_symboli = medulla_symbolum_internare(
                d->modulus, titulus);
            s32 fons = _em(d, initiator, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());

            (vacuum)_em(d, initiator, MEDULLA_OP_COPIA,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_registrum(sedes->index_registri),
                medulla_op_registrum(fons),
                medulla_op_immediatum((s64)mensura));
            redde;
        }
        /* cadit ad viam infra */
    }
    /* zeri + copia (C89: membra intacta zephyrum) + scripturae */
    {
        chorda titulus = _titulum_dati(d, "zeri");
        MedullaDatum* datum = medulla_datum_creare(d->modulus,
            titulus, (i32)mensura, I);

        si (datum != NIHIL)
        {
            s32 index_symboli = medulla_symbolum_internare(
                d->modulus, titulus);
            s32 fons = _em(d, initiator, MEDULLA_OP_LOCUS,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                _registrum_temporarium(d),
                medulla_op_symbolum(index_symboli),
                medulla_op_nihil(), medulla_op_nihil());

            (vacuum)_em(d, initiator, MEDULLA_OP_COPIA,
                MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                medulla_op_registrum(sedes->index_registri),
                medulla_op_registrum(fons),
                medulla_op_immediatum((s64)mensura));
        }
    }
    _congeriem_localem(d, sedes->index_registri, symbolum->typus,
        canonicus);
}

/* staticum locale: datum moduli nomine presso $functio.titulus */
interior vacuum
_staticum_locale (Demissio* d, constans SemanticaSymbolum* symbolum,
    constans SilvaNodus* nodus, constans SilvaNodus* initiator)
{
    character littera[CXXVIII];
    chorda titulus;
    i32 scriptum = ZEPHYRUM;
    i32 caput_f;
    i32 caput_s;
    MedullaDatum* datum;
    s32 mensura = _mensura_completa(d, symbolum->typus, initiator);
    s32 index_symboli;

    si (mensura <= ZEPHYRUM)
    {
        _sistere(d, nodus, "forma statici localis ignota");
        redde;
    }
    caput_f = (d->functio->titulus.mensura < XL)
        ? (i32)d->functio->titulus.mensura : XL;
    caput_s = (symbolum->titulus.mensura < XL)
        ? (i32)symbolum->titulus.mensura : XL;
    memcpy(littera, d->functio->titulus.datum,
        (memoriae_index)caput_f);
    scriptum = caput_f;
    littera[scriptum] = '.';
    scriptum++;
    memcpy(littera + scriptum, symbolum->titulus.datum,
        (memoriae_index)caput_s);
    scriptum += caput_s;
    titulus.datum = (i8*)littera;
    titulus.mensura = scriptum;
    titulus = chorda_transcribere(titulus, d->piscina);

    /* collisio (statica eiusdem nominis in scopis fratribus) ->
     * suffixum */
    index_symboli = medulla_symbolum_internare(d->modulus, titulus);
    si (medulla_symbolum_obtinere(d->modulus, index_symboli)->genus
        != (s32)MEDULLA_SYMBOLUM_EXTERNUM)
    {
        character alterum[CXXVIII];
        chorda secunda;
        s32 n = (s32)sprintf(alterum, "%.*s_%d", (int)scriptum,
            littera, (int)d->numerator_datorum);

        d->numerator_datorum++;
        secunda.datum = (i8*)alterum;
        secunda.mensura = (i32)n;
        titulus = chorda_transcribere(secunda, d->piscina);
        index_symboli = medulla_symbolum_internare(d->modulus,
            titulus);
    }
    datum = medulla_datum_creare(d->modulus, titulus, (i32)mensura,
        (i32)_ordinatio_typi(d, symbolum->typus));
    si (datum == NIHIL)
    {
        _sistere(d, nodus, "staticum locale sine dato");
        redde;
    }
    si (initiator != NIHIL
        && !_imaginem_scribere(d, datum, ZEPHYRUM, symbolum->typus,
               initiator))
    {
        _notare(d, nodus, "initiator staticus inaestimabilis");
    }
    {
        DemissioSedes* sedes = _sedem_creare(d, symbolum, -I,
            VERUM);

        si (sedes != NIHIL)
        {
            sedes->index_symboli_moduli = index_symboli;
        }
    }
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
            _staticum_locale(d, symbolum, nodus, initiator);
            perge;
        }
        sedes = _sedem_symboli(d, symbolum);
        si (sedes == NIHIL || initiator == NIHIL)
        {
            perge;
        }
        si (_canonicus(initiator)->genus
                == (s32)SILVA_C89_GENUS_CONGERIES
            || (_canonicus(initiator)->genus
                    == (s32)SILVA_C89_GENUS_FOLIUM_CHORDA
                && _exutus(symbolum->typus) != NIHIL
                && _exutus(symbolum->typus)->genus
                    == TYPUS_C89_ACIES))
        {
            _initiatorem_aggregatum(d, sedes, symbolum, initiator);
            perge;
        }
        {
            s32 mt = _typus_medullae(symbolum->typus);
            MedullaOperandum fons;

            si (mt < ZEPHYRUM && _est_aggregatum(symbolum->typus)
                && sedes->est_arca)
            {
                /* initiator aggregati: copia ex inscriptione */
                s32 mensura = _mensura_typi(d, symbolum->typus);
                s32 origo_valoris;

                si (mensura <= ZEPHYRUM)
                {
                    _sistere(d, nodus, "forma aggregati ignota");
                    perge;
                }
                origo_valoris = _ut_valor(d, initiator);
                (vacuum)_em(d, nodus, MEDULLA_OP_COPIA,
                    MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL, -I,
                    medulla_op_registrum(sedes->index_registri),
                    medulla_op_registrum(origo_valoris),
                    medulla_op_immediatum((s64)mensura));
                perge;
            }
            fons = _ut_operandum(d, initiator);
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

            si (valor_v.genus == SILVA_VALOR_NODUS
                && d->reditus_index >= ZEPHYRUM)
            {
                /* reditus aggregatus: copia in destinationem
                 * (conventio C7), redde sine valore */
                s32 fons = _ut_valor(d, valor_v.datum.nodus);

                si (d->reditus_mensura > ZEPHYRUM)
                {
                    (vacuum)_em(d, nodus, MEDULLA_OP_COPIA,
                        MEDULLA_TYPUS_NIHIL, MEDULLA_TYPUS_NIHIL,
                        -I,
                        medulla_op_registrum(d->reditus_index),
                        medulla_op_registrum(fons),
                        medulla_op_immediatum(
                            (s64)d->reditus_mensura));
                }
                alioquin
                {
                    _sistere(d, nodus, "forma reditus ignota");
                }
            }
            alioquin si (valor_v.genus == SILVA_VALOR_NODUS)
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
            character causa[LXIV];

            sprintf(causa, "sententia generis ignoti (%d)",
                (int)nodus->genus);
            _sistere(d, nodus, causa);
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
            constans SilvaNodus* elementum;
            constans SilvaNodus* initiator = NIHIL;
            SilvaToken* lexema;
            constans SemanticaSymbolum* symbolum;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            elementum = _canonicus(v->datum.nodus);
            si (elementum->genus
                == (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
            {
                SilvaValor init_v =
                    silva_c89_declarator_initiatus_initiator(
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
            si (symbolum != NIHIL
                && symbolum->genus != (int)SYMBOLUM_TYPEDEF
                && (symbolum->repositio & REPOSITIO_STATICA)
                    == ZEPHYRUM
                && symbolum->profunditas > ZEPHYRUM)
            {
                DemissioLocale* locale =
                    (DemissioLocale*)xar_addere(locales);

                si (locale != NIHIL)
                {
                    locale->symbolum = symbolum;
                    locale->initiator = initiator;
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
        _titulum_symboli(d, symbolum),
        (mt_reditus >= ZEPHYRUM) ? mt_reditus : MEDULLA_TYPUS_NIHIL,
        typus_functionis->datum.functio.est_variadica);
    si (d->functio == NIHIL)
    {
        redde;   /* definitio duplex */
    }
    d->numerator = ZEPHYRUM;
    d->frange_finis = -I;
    d->perge_finis = -I;
    d->reditus_index = -I;
    d->reditus_mensura = -I;
    d->sedes = tabula_dispersa_creare_chorda(d->piscina, CCLVI);
    d->capti = tabula_dispersa_creare_chorda(d->piscina, CCLVI);
    locales = xar_creare(d->piscina,
        (i32)magnitudo(DemissioLocale));

    /* praecursus: capti + locales (ante residentiam) */
    _praecursum(d, corpus_v.datum.nodus, locales);

    /* reditus aggregatus: parametrum destinationis EXPLICITUM
     * primum (conventio C7) */
    si (reditus_aggregatus)
    {
        s32 mensura = _mensura_typi(d, reditus);

        d->reditus_index = medulla_parametrum_addere(d->functio,
            _ch_literis("reditus"), MEDULLA_TYPUS_I64);
        d->reditus_mensura = mensura;
    }

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
                /* aggregatum valore: registrum parametri IAM
                 * inscriptionem copiae vocantis tenet (conventio) -
                 * sedes vertitur, nulla arca nova */
                sedes = _sedem_symboli(d, p_symbolum);
                si (sedes != NIHIL)
                {
                    sedes->est_arca = VERUM;
                }
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
        constans DemissioLocale* locale =
            (constans DemissioLocale*)xar_obtinere(locales, i);
        constans SemanticaSymbolum* symbolum_locale;

        si (locale == NIHIL)
        {
            perge;
        }
        symbolum_locale = locale->symbolum;
        si (_sedem_symboli(d, symbolum_locale) != NIHIL)
        {
            perge;   /* iam creatum */
        }
        si (_arcam_symboli(d, symbolum_locale))
        {
            s32 mensura = _mensura_completa(d,
                symbolum_locale->typus, locale->initiator);
            s32 ordinatio;
            s32 inscriptio;

            si (mensura <= ZEPHYRUM)
            {
                _sistere(d, nodus, "forma localis ignota");
                perge;
            }
            ordinatio = _ordinatio_typi(d, symbolum_locale->typus);
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

/* data globalia: definitiones (initiatae aut tentativae) ->
 * MedullaDatum; declarationes purae (extern sine initiatore) et
 * prototypa praetermissa */
interior vacuum
_data_globalia (Demissio* d, constans SilvaNodus* nodus)
{
    SilvaValor declaratores = silva_c89_declaratio_declaratores(
        nodus);
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(declaratores);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(declaratores,
            (unsigned int)i);
        constans SilvaNodus* elementum;
        constans SilvaNodus* initiator = NIHIL;
        SilvaToken* lexema;
        constans SemanticaSymbolum* symbolum;
        MedullaDatum* datum;
        s32 mensura;

        si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        elementum = _canonicus(v->datum.nodus);
        si (elementum->genus
            == (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
        {
            SilvaValor init_v =
                silva_c89_declarator_initiatus_initiator(elementum);

            si (init_v.genus == SILVA_VALOR_NODUS)
            {
                initiator = init_v.datum.nodus;
            }
        }
        lexema = silva_c89_declaratoris_titulus(elementum);
        symbolum = (lexema != NIHIL)
            ? _symbolum_lexematis(d, lexema) : NIHIL;
        si (symbolum == NIHIL
            || symbolum->genus != (int)SYMBOLUM_VARIABILE
            || symbolum->profunditas > ZEPHYRUM)
        {
            perge;   /* typedef/functio/enumerator/prototypum */
        }
        si (initiator == NIHIL
            && (symbolum->repositio & REPOSITIO_EXTERNA)
                != ZEPHYRUM)
        {
            perge;   /* declaratio pura */
        }
        mensura = _mensura_completa(d, symbolum->typus, initiator);
        si (mensura <= ZEPHYRUM)
        {
            _notare(d, nodus, "forma dati globalis ignota");
            perge;
        }
        datum = _datum_invenire_aut_creare(d,
            _titulum_symboli(d, symbolum), mensura,
            _ordinatio_typi(d, symbolum->typus));
        si (datum == NIHIL)
        {
            _notare(d, nodus, "datum globale non creatum");
            perge;
        }
        si (initiator != NIHIL
            && !_imaginem_scribere(d, datum, ZEPHYRUM,
                   symbolum->typus, initiator))
        {
            _notare(d, nodus, "initiator staticus inaestimabilis");
        }
    }
}

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
    si (nodus->genus == (s32)SILVA_C89_GENUS_DECLARATIO)
    {
        _data_globalia(d, nodus);
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
    d.stirps = _stirpem_computare(piscina, titulus_moduli);
    d.modulus = medulla_modulum_creare(piscina, titulus_moduli);
    d.bloccus = -I;
    d.frange_finis = -I;
    d.perge_finis = -I;
    d.reditus_index = -I;
    d.reditus_mensura = -I;
    si (d.modulus == NIHIL)
    {
        redde NIHIL;
    }
    /* piscina transitoria pro decodere (octeti statim in imagines
     * copiantur) */
    d.piscina_silvae = silva_piscina_generare_dynamicum(
        "demissio_chordae", 8388608);
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
    si (d.piscina_silvae != NIHIL)
    {
        silva_piscina_destruere(d.piscina_silvae);
    }
    redde d.modulus;
}

/* ==================================================
 * Distillatio linearum (M2a) - vide caput
 * ================================================== */

/* lexema primum verum subarboris (byte_offset -1 = syntheticum -
 * praetermittitur; fusor _linea_nodi cognatum) */
interior constans SilvaToken*
_lexema_primum (constans SilvaNodus* nodus, i32 profunditas)
{
    i32 i;

    si (nodus == NIHIL || profunditas > XXXII)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < (i32)nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (v->genus == SILVA_VALOR_TOKEN && v->datum.token != NIHIL
            && v->datum.token->byte_offset != -I)
        {
            redde v->datum.token;
        }
        si (v->genus == SILVA_VALOR_NODUS)
        {
            constans SilvaToken* lexema = _lexema_primum(
                v->datum.nodus, profunditas + I);

            si (lexema != NIHIL)
            {
                redde lexema;
            }
        }
    }
    redde NIHIL;
}

interior vacuum
_lineam_colligere (MedullaLineae* lineae,
    constans SilvaParsura* parsura,
    constans structura SilvaNodus* origo)
{
    constans SilvaToken* lexema;
    chorda via;

    si (origo == NIHIL
        || medulla_lineam_quaerere(lineae, origo, NIHIL, NIHIL))
    {
        redde;
    }
    lexema = _lexema_primum((constans SilvaNodus*)origo, ZEPHYRUM);
    si (lexema == NIHIL)
    {
        redde;
    }
    via.datum = NIHIL;
    via.mensura = ZEPHYRUM;
    si (parsura->expansio != NIHIL)
    {
        constans SilvaChorda* via_s = silva_fons_via(
            parsura->expansio, lexema->fons_index);

        si (via_s != NIHIL)
        {
            via = _ch_de_silva(*via_s);
        }
    }
    (vacuum)medulla_lineam_ponere(lineae, origo, via,
        (i32)lexema->linea);
}

MedullaLineae*
demissio_lineas_colligere (Piscina* piscina,
    constans MedullaModulus* modulus,
    constans SilvaParsura* parsura)
{
    MedullaLineae* lineae;
    i32 f;
    i32 numerus_functionum;
    i32 numerus_datorum;

    si (piscina == NIHIL || modulus == NIHIL || parsura == NIHIL)
    {
        redde NIHIL;
    }
    lineae = medulla_lineas_creare(piscina);
    si (lineae == NIHIL)
    {
        redde NIHIL;
    }

    numerus_functionum = xar_numerus(modulus->functiones);
    per (f = ZEPHYRUM; f < numerus_functionum; f++)
    {
        constans MedullaFunctio* functio =
            *(MedullaFunctio**)xar_obtinere(modulus->functiones,
                (i32)f);
        i32 b;
        i32 numerus_bloccorum = xar_numerus(functio->blocci);

        _lineam_colligere(lineae, parsura, functio->origo);
        per (b = ZEPHYRUM; b < numerus_bloccorum; b++)
        {
            constans MedullaBloccus* bloccus =
                (constans MedullaBloccus*)xar_obtinere(
                    functio->blocci, (i32)b);
            i32 n = xar_numerus(bloccus->instructiones);
            i32 k;

            per (k = ZEPHYRUM; k < n; k++)
            {
                constans MedullaInstructio* instructio =
                    (constans MedullaInstructio*)xar_obtinere(
                        bloccus->instructiones, (i32)k);

                _lineam_colligere(lineae, parsura,
                    instructio->origo);
            }
        }
    }
    numerus_datorum = xar_numerus(modulus->data);
    per (f = ZEPHYRUM; f < numerus_datorum; f++)
    {
        constans MedullaDatum* datum =
            *(MedullaDatum**)xar_obtinere(modulus->data, (i32)f);

        _lineam_colligere(lineae, parsura, datum->origo);
    }
    redde lineae;
}
