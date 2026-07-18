/* officina_indicium.c - INDICIUM: scriptor + lector (M3 chunk 3)
 *
 * Scriptor: colligit per-TU dum parsura vivit (acies expansionis
 * plena ex catena originis tokenorum - unio SilvaOrigo publica in
 * amalgamate), iungit ordine CONEXIONIS ad tempus scribendi (clavis
 * = monstrator functionis, exemplar clavium MedullaLineae).
 * Lector: plagula tota in blocum unum, sectiones per directorium
 * genere-indexatum, quaestiones per quaestionem binariam.
 *
 * Vide officina_indicium.h pro forma; officina-m3-spec.md §V pro
 * consilio; worklog pro inventis.
 */

#include "officina_indicium.h"

#include "xar.h"
#include "tabula_dispersa.h"
#include "piscina.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================================================
 * Typi scriptoris (privati)
 * ================================================== */

nomen structura {
    i32 instructio;                /* index planus intra functionem */
    i32 via;
    i32 linea;
    i32 profunditas;
    i32 nomen_macro;
} ScriptorLinea;

nomen structura {
    constans MedullaFunctio* functio;   /* clavis iuncturae */
    i32  titulus;                  /* chorda interna */
    Xar* lineae;                   /* ScriptorLinea valore */
    Xar* variabilia;               /* IndiciumVariabile valore */
    i32  instructiones_numerus;
} ScriptorFunctio;

structura IndiciumScriptor {
    Piscina* piscina;
    Xar* chordae_datum;            /* i8 */
    Xar* chordae_offseta;          /* i32; N+1 introitus */
    TabulaDispersa* chordae_tabula;
    Xar* viae;                     /* i32 (chorda) */
    TabulaDispersa* viae_tabula;
    Xar* functiones;               /* ScriptorFunctio valore */
    TabulaDispersa* functiones_tabula;
};

/* magnitudines elementorum - ORDO == IndiciumSectioGenus EXACTE
 * (asserta magnitudinum probationis derivam figunt) */
interior constans memoriae_index _elementi_magnitudines[] = {
    magnitudo(i8),                 /* CHORDAE_DATA */
    magnitudo(i32),                /* CHORDAE_OFFSETA */
    magnitudo(IndiciumVia),        /* VIAE */
    magnitudo(IndiciumModulus),    /* MODULI */
    magnitudo(IndiciumFunctio),    /* FUNCTIONES */
    magnitudo(i32),                /* LINEAE_INDICES */
    magnitudo(IndiciumLinea),      /* LINEAE_RES */
    magnitudo(i32),                /* RETRO_LINEAE */
    magnitudo(i32),                /* RETRO_INTERVALLA */
    magnitudo(IndiciumSitus),      /* RETRO_SITUS */
    magnitudo(IndiciumVariabile)   /* VARIABILIA */
};

/* ==================================================
 * Auxilia chordarum
 * ================================================== */

interior chorda
_ch_vacua (vacuum)
{
    chorda c;

    c.datum = NIHIL;
    c.mensura = ZEPHYRUM;
    redde c;
}

interior chorda
_ind_ch_de_silva (constans SilvaChorda* s)
{
    chorda c;

    si (s == NIHIL || s->datum == NIHIL)
    {
        redde _ch_vacua();
    }
    c.datum = (i8*)s->datum;
    c.mensura = (i32)s->mensura;
    redde c;
}

interior chorda
_ch_de_literis (constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    redde c;
}

interior b32
_ind_chordae_aequales (chorda a, chorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (b32)(memcmp(a.datum, b.datum,
        (memoriae_index)a.mensura) == 0);
}

/* copia clavis in piscinam (tabula clavem non copiat) */
interior chorda
_clavem_copiare (Piscina* piscina, chorda c)
{
    chorda copia;

    copia.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(c.mensura > ZEPHYRUM ? c.mensura : I));
    copia.mensura = ZEPHYRUM;
    si (copia.datum == NIHIL)
    {
        copia.datum = NIHIL;
        redde copia;
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(copia.datum, c.datum, (memoriae_index)c.mensura);
    }
    copia.mensura = c.mensura;
    redde copia;
}

/* ==================================================
 * Internamentum chordarum (blob + offseta + dedup)
 * ================================================== */

interior i32
_internare (IndiciumScriptor* s, chorda c)
{
    vacuum* valor = NIHIL;
    i32 index;
    i32 i;

    si (c.mensura == ZEPHYRUM || c.datum == NIHIL)
    {
        redde ZEPHYRUM;   /* chorda nihili */
    }
    si (tabula_dispersa_invenire(s->chordae_tabula, c, &valor))
    {
        redde (i32)(memoriae_index)valor;
    }
    index = xar_numerus(s->chordae_offseta) - I;   /* chordae adhuc */
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        i8* locellus = xar_addere(s->chordae_datum);

        si (locellus == NIHIL)
        {
            redde ZEPHYRUM;
        }
        *locellus = c.datum[i];
    }
    {
        i32* finis = xar_addere(s->chordae_offseta);

        si (finis == NIHIL)
        {
            redde ZEPHYRUM;
        }
        *finis = xar_numerus(s->chordae_datum);
    }
    (vacuum)tabula_dispersa_inserere(s->chordae_tabula,
        _clavem_copiare(s->piscina, c),
        (vacuum*)(memoriae_index)index);
    redde index;
}

interior i32
_ind_viam_internare (IndiciumScriptor* s, chorda via)
{
    vacuum* valor = NIHIL;
    i32 index;

    si (via.mensura == ZEPHYRUM || via.datum == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (tabula_dispersa_invenire(s->viae_tabula, via, &valor))
    {
        redde (i32)(memoriae_index)valor;
    }
    index = xar_numerus(s->viae);
    {
        i32* locellus = xar_addere(s->viae);

        si (locellus == NIHIL)
        {
            redde ZEPHYRUM;
        }
        *locellus = _internare(s, via);
    }
    (vacuum)tabula_dispersa_inserere(s->viae_tabula,
        _clavem_copiare(s->piscina, via),
        (vacuum*)(memoriae_index)index);
    redde index;
}

/* ==================================================
 * Scriptor: creatio
 * ================================================== */

IndiciumScriptor*
indicium_scriptor_creare (Piscina* piscina)
{
    IndiciumScriptor* s;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    s = piscina_allocare(piscina, magnitudo(IndiciumScriptor));
    si (s == NIHIL)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(IndiciumScriptor));
    s->piscina = piscina;
    s->chordae_datum = xar_creare(piscina, (i32)magnitudo(i8));
    s->chordae_offseta = xar_creare(piscina, (i32)magnitudo(i32));
    s->chordae_tabula = tabula_dispersa_creare_chorda(piscina, 512);
    s->viae = xar_creare(piscina, (i32)magnitudo(i32));
    s->viae_tabula = tabula_dispersa_creare_chorda(piscina, 64);
    s->functiones = xar_creare(piscina,
        (i32)magnitudo(ScriptorFunctio));
    s->functiones_tabula = tabula_dispersa_creare_chorda(piscina,
        512);
    si (s->chordae_datum == NIHIL || s->chordae_offseta == NIHIL
        || s->chordae_tabula == NIHIL || s->viae == NIHIL
        || s->viae_tabula == NIHIL || s->functiones == NIHIL
        || s->functiones_tabula == NIHIL)
    {
        redde NIHIL;
    }
    /* chorda 0 = nihili: offseta {0, 0} */
    {
        i32* nulla = xar_addere(s->chordae_offseta);
        i32* prima = xar_addere(s->chordae_offseta);

        si (nulla == NIHIL || prima == NIHIL)
        {
            redde NIHIL;
        }
        *nulla = ZEPHYRUM;
        *prima = ZEPHYRUM;
    }
    redde s;
}

/* ==================================================
 * Collectio: acies expansionis (catena originis)
 * ================================================== */

/* lexema primum verum subarboris (par demissionis _ind_lexema_primum -
 * exscriptum consulto: demissio internum tenet; nota in worklog) */
interior constans SilvaToken*
_ind_lexema_primum (constans SilvaNodus* nodus, i32 profunditas)
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
            constans SilvaToken* lexema = _ind_lexema_primum(
                v->datum.nodus, profunditas + I);

            si (lexema != NIHIL)
            {
                redde lexema;
            }
        }
    }
    redde NIHIL;
}

interior constans SilvaToken*
_praedecessor_sedis_usus (constans SilvaToken* t)
{
    commutatio (t->origo.genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        redde t->origo.datum.expansio.invocatio;
    casus SILVA_ORIGO_PASTA:
        redde t->origo.datum.pasta.sinister;
    casus SILVA_ORIGO_CHORDA:
        redde t->origo.datum.stringificatio.primus;
    ordinarius:
        redde NIHIL;   /* FONS / API = radix */
    }
}

interior constans SilvaChorda*
_nomen_brachii (constans SilvaToken* t)
{
    commutatio (t->origo.genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        redde t->origo.datum.expansio.nomen_macro;
    casus SILVA_ORIGO_PASTA:
        redde t->origo.datum.pasta.nomen_macro;
    casus SILVA_ORIGO_CHORDA:
        redde t->origo.datum.stringificatio.nomen_macro;
    casus SILVA_ORIGO_API:
        redde t->origo.datum.api.nomen_macro;
    ordinarius:
        redde NIHIL;
    }
}

#define CATENA_MAXIMA 16

nomen structura {
    i32 initium;                   /* in sf->lineae */
    i32 numerus;
} AciesPrior;

interior vacuum
_aciem_colligere (IndiciumScriptor* s, ScriptorFunctio* sf,
    constans SilvaParsura* parsura, i32 index_planus,
    constans structura SilvaNodus* origo, AciesPrior* prior)
{
    constans SilvaToken* catena[CATENA_MAXIMA];
    ScriptorLinea novae[CATENA_MAXIMA];
    i32 numerus = ZEPHYRUM;
    i32 k;
    constans SilvaToken* t;

    t = _ind_lexema_primum(origo, ZEPHYRUM);
    si (t == NIHIL || parsura->expansio == NIHIL)
    {
        redde;
    }
    dum (t != NIHIL && numerus < (i32)CATENA_MAXIMA)
    {
        catena[numerus] = t;
        numerus++;
        t = _praedecessor_sedis_usus(t);
    }
    /* radix prima (profunditas 0 = invocatio) */
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans SilvaToken* gradus = catena[numerus - I - k];
        constans SilvaChorda* via_s = silva_fons_via(
            parsura->expansio, gradus->fons_index);

        novae[k].instructio = index_planus;
        novae[k].via = _ind_viam_internare(s, _ind_ch_de_silva(via_s));
        novae[k].linea = (i32)gradus->linea;
        novae[k].profunditas = k;
        novae[k].nomen_macro = _internare(s,
            _ind_ch_de_silva(_nomen_brachii(gradus)));
    }
    /* dedup contra aciem emissam priorem: eadem acies (praeter
     * instructio) = intervallum extenditur, nihil emittitur */
    si (prior->numerus == numerus && numerus > ZEPHYRUM)
    {
        b32 eadem = VERUM;

        per (k = ZEPHYRUM; k < numerus; k++)
        {
            constans ScriptorLinea* p = (constans ScriptorLinea*)
                xar_obtinere(sf->lineae, prior->initium + k);

            si (p->via != novae[k].via || p->linea != novae[k].linea
                || p->profunditas != novae[k].profunditas
                || p->nomen_macro != novae[k].nomen_macro)
            {
                eadem = FALSUM;
                frange;
            }
        }
        si (eadem)
        {
            redde;
        }
    }
    prior->initium = xar_numerus(sf->lineae);
    prior->numerus = numerus;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        ScriptorLinea* locellus = xar_addere(sf->lineae);

        si (locellus == NIHIL)
        {
            redde;
        }
        *locellus = novae[k];
    }
}

/* ==================================================
 * Collectio: variabilia (registra nominata x symbola semanticae)
 * ================================================== */

interior b32
_temporarium (chorda titulus)
{
    i32 i;

    si (titulus.mensura < II || titulus.datum[ZEPHYRUM] != 't')
    {
        redde FALSUM;
    }
    per (i = I; i < titulus.mensura; i++)
    {
        si (titulus.datum[i] < '0' || titulus.datum[i] > '9')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_intra_nodum (constans SilvaNodus* nodus,
    constans structura SilvaNodus* maior)
{
    dum (nodus != NIHIL)
    {
        si (nodus == maior)
        {
            redde VERUM;
        }
        nodus = nodus->pater;
    }
    redde FALSUM;
}

interior constans character*
_primitivi_nomen (integer primitivum)
{
    interior constans character* constans NOMINA[] = {
        "void", "char", "signed char", "unsigned char",
        "short", "unsigned short", "int", "unsigned int",
        "long", "unsigned long", "long long",
        "unsigned long long", "float", "double", "long double"
    };

    si (primitivum < 0
        || primitivum >= (integer)(magnitudo(NOMINA)
               / magnitudo(NOMINA[0])))
    {
        redde "?";
    }
    redde NOMINA[primitivum];
}

interior vacuum
_scripturae_adde (character* cella, i32* cursor, i32 capacitas,
    constans character* textus, i32 mensura)
{
    i32 i;

    per (i = ZEPHYRUM; i < mensura && *cursor + I < capacitas; i++)
    {
        cella[*cursor] = textus[i];
        (*cursor)++;
    }
    cella[*cursor] = '\0';
}

interior vacuum
_typum_scribere (constans TypusC89* typus, character* cella,
    i32* cursor, i32 capacitas, i32 profunditas)
{
    si (typus == NIHIL || profunditas > VIII)
    {
        _scripturae_adde(cella, cursor, capacitas, "?", I);
        redde;
    }
    commutatio (typus->genus)
    {
    casus TYPUS_C89_PRIMITIVUS:
    {
        constans character* n = _primitivi_nomen(
            typus->datum.primitivum);

        _scripturae_adde(cella, cursor, capacitas, n,
            (i32)strlen(n));
        frange;
    }
    casus TYPUS_C89_MONSTRATOR:
        _typum_scribere(typus->datum.monstrator.internum, cella,
            cursor, capacitas, profunditas + I);
        _scripturae_adde(cella, cursor, capacitas, "*", I);
        frange;
    casus TYPUS_C89_ACIES:
    {
        character numeri[XXXII];

        _typum_scribere(typus->datum.acies.elementum, cella,
            cursor, capacitas, profunditas + I);
        si (typus->datum.acies.numerus >= 0)
        {
            sprintf(numeri, "[%d]",
                (integer)typus->datum.acies.numerus);
        }
        alioquin
        {
            sprintf(numeri, "[]");
        }
        _scripturae_adde(cella, cursor, capacitas, numeri,
            (i32)strlen(numeri));
        frange;
    }
    casus TYPUS_C89_FUNCTIO:
        _typum_scribere(typus->datum.functio.reditus, cella,
            cursor, capacitas, profunditas + I);
        _scripturae_adde(cella, cursor, capacitas, " ()", III);
        frange;
    casus TYPUS_C89_STRUCTURA:
    casus TYPUS_C89_UNIO:
    {
        constans character* genus_nomen =
            (typus->genus == (integer)TYPUS_C89_STRUCTURA)
                ? "struct " : "union ";

        _scripturae_adde(cella, cursor, capacitas, genus_nomen,
            (i32)strlen(genus_nomen));
        si (typus->datum.tag.titulus.mensura > 0U)
        {
            _scripturae_adde(cella, cursor, capacitas,
                (constans character*)typus->datum.tag.titulus.datum,
                (i32)typus->datum.tag.titulus.mensura);
        }
        alioquin
        {
            _scripturae_adde(cella, cursor, capacitas, "?", I);
        }
        frange;
    }
    casus TYPUS_C89_ENUMERATUS:
        _scripturae_adde(cella, cursor, capacitas, "enum ", V);
        si (typus->datum.enumeratus.titulus.mensura > 0U)
        {
            _scripturae_adde(cella, cursor, capacitas,
                (constans character*)
                    typus->datum.enumeratus.titulus.datum,
                (i32)typus->datum.enumeratus.titulus.mensura);
        }
        alioquin
        {
            _scripturae_adde(cella, cursor, capacitas, "?", I);
        }
        frange;
    casus TYPUS_C89_QUALIFICATUS:
        si (typus->datum.qualificatus.quales
            & (insignatus integer)QUALIS_CONSTANS)
        {
            _scripturae_adde(cella, cursor, capacitas, "const ",
                VI);
        }
        si (typus->datum.qualificatus.quales
            & (insignatus integer)QUALIS_VOLATILIS)
        {
            _scripturae_adde(cella, cursor, capacitas,
                "volatile ", IX);
        }
        _typum_scribere(typus->datum.qualificatus.internum, cella,
            cursor, capacitas, profunditas + I);
        frange;
    ordinarius:
        _scripturae_adde(cella, cursor, capacitas, "?", I);
        frange;
    }
}

interior s32
_tag_de_typo (constans TypusC89* typus)
{
    si (typus == NIHIL)
    {
        redde (s32)INDICIUM_TYPUS_IGNOTUS;
    }
    si (typus->genus == (integer)TYPUS_C89_QUALIFICATUS)
    {
        redde _tag_de_typo(typus->datum.qualificatus.internum);
    }
    commutatio (typus->genus)
    {
    casus TYPUS_C89_MONSTRATOR:
    casus TYPUS_C89_ACIES:
    casus TYPUS_C89_FUNCTIO:
        redde MEDULLA_TYPUS_I64;   /* inscriptiones = verba */
    casus TYPUS_C89_ENUMERATUS:
        redde MEDULLA_TYPUS_S32;
    casus TYPUS_C89_PRIMITIVUS:
        commutatio (typus->datum.primitivum)
        {
        casus PRIMITIVUM_CHARACTER:            /* arm64 Apple:
                                                * char signatus */
        casus PRIMITIVUM_CHARACTER_SIGNATUM:
            redde MEDULLA_TYPUS_S8;
        casus PRIMITIVUM_CHARACTER_INSIGNATUM:
            redde MEDULLA_TYPUS_I8;
        casus PRIMITIVUM_BREVIS:
            redde MEDULLA_TYPUS_S16;
        casus PRIMITIVUM_BREVIS_INSIGNATUM:
            redde MEDULLA_TYPUS_I16;
        casus PRIMITIVUM_INTEGER:
            redde MEDULLA_TYPUS_S32;
        casus PRIMITIVUM_INTEGER_INSIGNATUM:
            redde MEDULLA_TYPUS_I32;
        casus PRIMITIVUM_LONGUS:
        casus PRIMITIVUM_LONGUS_LONGUS:
            redde MEDULLA_TYPUS_S64;
        casus PRIMITIVUM_LONGUS_INSIGNATUM:
        casus PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM:
            redde MEDULLA_TYPUS_I64;
        casus PRIMITIVUM_FLUITANS:
            redde MEDULLA_TYPUS_F32;
        casus PRIMITIVUM_DUPLEX:
        casus PRIMITIVUM_DUPLEX_LONGUS:
            redde MEDULLA_TYPUS_F64;
        ordinarius:
            redde (s32)INDICIUM_TYPUS_IGNOTUS;
        }
    ordinarius:
        redde (s32)INDICIUM_TYPUS_IGNOTUS;
    }
}

interior constans SemanticaSymbolum*
_symbolum_functionis (SilvaSemantica* sem,
    constans structura SilvaNodus* functionis_nodus, chorda titulus)
{
    insignatus integer numerus;
    insignatus integer i;

    si (sem == NIHIL || functionis_nodus == NIHIL)
    {
        redde NIHIL;
    }
    numerus = silva_c89_symbola_numerus(sem);
    per (i = 0U; i < numerus; i++)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_per_indicem(sem, i);

        si (symbolum == NIHIL)
        {
            perge;
        }
        si (symbolum->genus != (integer)SYMBOLUM_VARIABILE
            && symbolum->genus != (integer)SYMBOLUM_PARAMETRUM)
        {
            perge;
        }
        si (symbolum->profunditas == 0U)
        {
            perge;   /* scopus fili = datum, non registrum */
        }
        si (!_ind_chordae_aequales(titulus,
                _ind_ch_de_silva(&symbolum->titulus)))
        {
            perge;
        }
        si (!_intra_nodum(symbolum->declarans, functionis_nodus))
        {
            perge;
        }
        redde symbolum;   /* congruentia prima (umbrae: lacuna
                           * honesta documentata) */
    }
    redde NIHIL;
}

interior vacuum
_variabilia_colligere (IndiciumScriptor* s, ScriptorFunctio* sf,
    constans MedullaFunctio* functio, SilvaSemantica* sem)
{
    i32 numerus_registrorum = xar_numerus(functio->registra);
    i32 numerus_parametrorum = xar_numerus(functio->parametra);
    i32 r;

    per (r = ZEPHYRUM; r < numerus_registrorum; r++)
    {
        constans chorda* titulus = (constans chorda*)xar_obtinere(
            functio->registra, r);
        IndiciumVariabile v;
        i32 p;

        si (_temporarium(*titulus))
        {
            perge;
        }
        v.titulus = _internare(s, *titulus);
        v.index_registri = r;
        v.typus_medulla = (s32)INDICIUM_TYPUS_IGNOTUS;
        v.typus_scriptus = ZEPHYRUM;
        per (p = ZEPHYRUM; p < numerus_parametrorum; p++)
        {
            constans MedullaParametrum* parametrum =
                (constans MedullaParametrum*)xar_obtinere(
                    functio->parametra, p);

            si (parametrum->index == (s32)r)
            {
                v.typus_medulla = parametrum->typus;
                frange;
            }
        }
        {
            constans SemanticaSymbolum* symbolum =
                _symbolum_functionis(sem, functio->origo,
                    *titulus);

            si (symbolum != NIHIL && symbolum->typus != NIHIL)
            {
                character littera[CXXVIII];
                i32 cursor = ZEPHYRUM;

                littera[ZEPHYRUM] = '\0';
                _typum_scribere(symbolum->typus, littera, &cursor,
                    (i32)magnitudo(littera), ZEPHYRUM);
                v.typus_scriptus = _internare(s,
                    _ch_de_literis(littera));
                si (v.typus_medulla == (s32)INDICIUM_TYPUS_IGNOTUS)
                {
                    v.typus_medulla = _tag_de_typo(symbolum->typus);
                }
            }
        }
        {
            IndiciumVariabile* locellus = xar_addere(sf->variabilia);

            si (locellus != NIHIL)
            {
                *locellus = v;
            }
        }
    }
}

/* ==================================================
 * Collectio: modulus
 * ================================================== */

b32
indicium_modulum_colligere (IndiciumScriptor* s,
    constans MedullaModulus* modulus,
    constans SilvaParsura* parsura, SilvaSemantica* sem)
{
    i32 numerus_functionum;
    i32 f;

    si (s == NIHIL || modulus == NIHIL || parsura == NIHIL)
    {
        redde FALSUM;
    }
    numerus_functionum = xar_numerus(modulus->functiones);
    per (f = ZEPHYRUM; f < numerus_functionum; f++)
    {
        constans MedullaFunctio* functio =
            *(MedullaFunctio**)xar_obtinere(modulus->functiones,
                (i32)f);
        ScriptorFunctio* sf;
        AciesPrior prior;
        i32 index_planus = ZEPHYRUM;
        i32 b;
        i32 numerus_bloccorum;

        /* aperire */
        {
            i8* clavis_octeti = piscina_allocare(s->piscina,
                magnitudo(vacuum*));
            chorda clavis;
            i32 sf_index = xar_numerus(s->functiones);

            sf = xar_addere(s->functiones);
            si (sf == NIHIL || clavis_octeti == NIHIL)
            {
                redde FALSUM;
            }
            memset(sf, ZEPHYRUM, magnitudo(ScriptorFunctio));
            sf->functio = functio;
            sf->titulus = _internare(s, functio->titulus);
            sf->lineae = xar_creare(s->piscina,
                (i32)magnitudo(ScriptorLinea));
            sf->variabilia = xar_creare(s->piscina,
                (i32)magnitudo(IndiciumVariabile));
            si (sf->lineae == NIHIL || sf->variabilia == NIHIL)
            {
                redde FALSUM;
            }
            memcpy(clavis_octeti, &functio, magnitudo(vacuum*));
            clavis.datum = clavis_octeti;
            clavis.mensura = (i32)magnitudo(vacuum*);
            (vacuum)tabula_dispersa_inserere(s->functiones_tabula,
                clavis, (vacuum*)(memoriae_index)sf_index);
        }
        prior.initium = ZEPHYRUM;
        prior.numerus = ZEPHYRUM;
        numerus_bloccorum = xar_numerus(functio->blocci);
        per (b = ZEPHYRUM; b < numerus_bloccorum; b++)
        {
            constans MedullaBloccus* bloccus =
                medulla_bloccum_obtinere(functio, (s32)b);
            i32 n = xar_numerus(bloccus->instructiones);
            i32 k;

            per (k = ZEPHYRUM; k < n; k++)
            {
                constans MedullaInstructio* instructio =
                    (constans MedullaInstructio*)xar_obtinere(
                        bloccus->instructiones, (i32)k);

                si (instructio->origo != NIHIL)
                {
                    _aciem_colligere(s, sf, parsura, index_planus,
                        instructio->origo, &prior);
                }
                index_planus++;
            }
        }
        sf->instructiones_numerus = index_planus;
        si (sem != NIHIL)
        {
            _variabilia_colligere(s, sf, functio, sem);
        }
    }
    redde VERUM;
}

/* ==================================================
 * Scribere: iunctura ordine conexionis + retro + plagula
 * ================================================== */

nomen structura {
    i32 linea;
    i32 functio;
    i32 instructio;
} RetroTriplum;

interior integer
_tripla_comparare (constans vacuum* a, constans vacuum* b)
{
    constans RetroTriplum* ta = (constans RetroTriplum*)a;
    constans RetroTriplum* tb = (constans RetroTriplum*)b;

    si (ta->linea != tb->linea)
    {
        redde (ta->linea < tb->linea) ? -1 : 1;
    }
    si (ta->functio != tb->functio)
    {
        redde (ta->functio < tb->functio) ? -1 : 1;
    }
    si (ta->instructio != tb->instructio)
    {
        redde (ta->instructio < tb->instructio) ? -1 : 1;
    }
    redde 0;
}

interior i32
_instructiones_numerare (constans MedullaFunctio* functio)
{
    i32 summa = ZEPHYRUM;
    i32 b;
    i32 numerus_bloccorum;

    si (functio == NIHIL || functio->blocci == NIHIL)
    {
        redde ZEPHYRUM;
    }
    numerus_bloccorum = xar_numerus(functio->blocci);
    per (b = ZEPHYRUM; b < numerus_bloccorum; b++)
    {
        constans MedullaBloccus* bloccus =
            medulla_bloccum_obtinere(functio, (s32)b);

        summa += xar_numerus(bloccus->instructiones);
    }
    redde summa;
}

interior vacuum*
_xar_in_tabulam (Piscina* piscina, constans Xar* xar,
    memoriae_index elementi_octeti, i32* numerus_out)
{
    i32 numerus = xar_numerus(xar);
    vacuum* tabula;

    *numerus_out = numerus;
    si (numerus == ZEPHYRUM)
    {
        redde NIHIL;
    }
    tabula = piscina_allocare(piscina,
        (memoriae_index)numerus * elementi_octeti);
    si (tabula == NIHIL)
    {
        *numerus_out = ZEPHYRUM;
        redde NIHIL;
    }
    (vacuum)xar_copiare_ad_tabulam(xar, tabula, ZEPHYRUM, numerus);
    redde tabula;
}

b32
indicium_scribere (IndiciumScriptor* s, constans Conexio* conexio,
    constans character* via)
{
    Xar* moduli_res;
    Xar* functiones_res;
    Xar* lineae_indices;
    Xar* lineae_res;
    Xar* variabilia_res;
    Xar* viae_res;
    Xar* retro_lineae;
    Xar* retro_intervalla;
    Xar* retro_situs;
    Xar* tripla_per_viam;          /* Xar* valore, per viam */
    i32 numerus_functionum;
    i32 numerus_modulorum;
    i32 numerus_viarum;
    i32 f;
    i32 m;
    i32 v;

    si (s == NIHIL || conexio == NIHIL || via == NIHIL)
    {
        redde FALSUM;
    }
    moduli_res = xar_creare(s->piscina,
        (i32)magnitudo(IndiciumModulus));
    functiones_res = xar_creare(s->piscina,
        (i32)magnitudo(IndiciumFunctio));
    lineae_indices = xar_creare(s->piscina, (i32)magnitudo(i32));
    lineae_res = xar_creare(s->piscina,
        (i32)magnitudo(IndiciumLinea));
    variabilia_res = xar_creare(s->piscina,
        (i32)magnitudo(IndiciumVariabile));
    viae_res = xar_creare(s->piscina, (i32)magnitudo(IndiciumVia));
    retro_lineae = xar_creare(s->piscina, (i32)magnitudo(i32));
    retro_intervalla = xar_creare(s->piscina, (i32)magnitudo(i32));
    retro_situs = xar_creare(s->piscina,
        (i32)magnitudo(IndiciumSitus));
    tripla_per_viam = xar_creare(s->piscina, (i32)magnitudo(Xar*));
    si (moduli_res == NIHIL || functiones_res == NIHIL
        || lineae_indices == NIHIL || lineae_res == NIHIL
        || variabilia_res == NIHIL || viae_res == NIHIL
        || retro_lineae == NIHIL || retro_intervalla == NIHIL
        || retro_situs == NIHIL || tripla_per_viam == NIHIL)
    {
        redde FALSUM;
    }

    /* moduli (ordo conexionis) */
    numerus_modulorum = (i32)conexio_numerus_modulorum(conexio);
    per (m = ZEPHYRUM; m < numerus_modulorum; m++)
    {
        constans MedullaModulus* modulus =
            conexio_modulum_obtinere(conexio, (s32)m);
        IndiciumModulus* locellus = xar_addere(moduli_res);

        si (locellus == NIHIL)
        {
            redde FALSUM;
        }
        locellus->titulus = (modulus != NIHIL)
            ? _internare(s, modulus->titulus) : ZEPHYRUM;
    }

    /* sinus triplorum per viam (parati) */
    numerus_viarum = xar_numerus(s->viae);
    per (v = ZEPHYRUM; v < numerus_viarum; v++)
    {
        Xar** locellus = xar_addere(tripla_per_viam);

        si (locellus == NIHIL)
        {
            redde FALSUM;
        }
        *locellus = xar_creare(s->piscina,
            (i32)magnitudo(RetroTriplum));
        si (*locellus == NIHIL)
        {
            redde FALSUM;
        }
    }

    /* functiones (ordo conexionis; iunctura per monstratorem) */
    numerus_functionum = (i32)conexio_numerus_functionum(conexio);
    per (f = ZEPHYRUM; f < numerus_functionum; f++)
    {
        constans ConexioFunctioNexa* nexa =
            conexio_functionem_obtinere(conexio, (s32)f);
        IndiciumFunctio ifu;
        constans ScriptorFunctio* sf = NIHIL;

        memset(&ifu, ZEPHYRUM, magnitudo(IndiciumFunctio));
        ifu.modulus = (i32)nexa->modulus_index;
        {
            i8 octeti[magnitudo(vacuum*)];
            chorda clavis;
            vacuum* valor = NIHIL;

            memcpy(octeti, &nexa->functio, magnitudo(octeti));
            clavis.datum = octeti;
            clavis.mensura = (i32)magnitudo(octeti);
            si (tabula_dispersa_invenire(s->functiones_tabula,
                    clavis, &valor))
            {
                sf = (constans ScriptorFunctio*)xar_obtinere(
                    s->functiones, (i32)(memoriae_index)valor);
            }
        }
        ifu.lineae_indices_primus = xar_numerus(lineae_indices);
        ifu.lineae_res_primus = xar_numerus(lineae_res);
        ifu.variabilia_prima = xar_numerus(variabilia_res);
        si (sf != NIHIL)
        {
            i32 n = xar_numerus(sf->lineae);
            i32 k;

            ifu.titulus = sf->titulus;
            ifu.instructiones_numerus = sf->instructiones_numerus;
            ifu.lineae_numerus = n;
            per (k = ZEPHYRUM; k < n; k++)
            {
                constans ScriptorLinea* sl =
                    (constans ScriptorLinea*)xar_obtinere(
                        sf->lineae, k);
                i32* index_locellus = xar_addere(lineae_indices);
                IndiciumLinea* res_locellus = xar_addere(lineae_res);

                si (index_locellus == NIHIL
                    || res_locellus == NIHIL)
                {
                    redde FALSUM;
                }
                *index_locellus = sl->instructio;
                res_locellus->via = sl->via;
                res_locellus->linea = sl->linea;
                res_locellus->profunditas = sl->profunditas;
                res_locellus->nomen_macro = sl->nomen_macro;
                /* radix -> triplum retro */
                si (sl->profunditas == ZEPHYRUM)
                {
                    Xar** sinus = (Xar**)xar_obtinere(
                        tripla_per_viam, sl->via);
                    RetroTriplum* triplum = xar_addere(*sinus);

                    si (triplum == NIHIL)
                    {
                        redde FALSUM;
                    }
                    triplum->linea = sl->linea;
                    triplum->functio = f;
                    triplum->instructio = sl->instructio;
                }
            }
            {
                i32 k2 = xar_numerus(sf->variabilia);
                i32 j;

                ifu.variabilia_numerus = k2;
                per (j = ZEPHYRUM; j < k2; j++)
                {
                    IndiciumVariabile* locellus =
                        xar_addere(variabilia_res);

                    si (locellus == NIHIL)
                    {
                        redde FALSUM;
                    }
                    *locellus = *(IndiciumVariabile*)xar_obtinere(
                        sf->variabilia, j);
                }
            }
        }
        alioquin
        {
            ifu.titulus = _internare(s, nexa->functio->titulus);
            ifu.instructiones_numerus =
                _instructiones_numerare(nexa->functio);
            ifu.lineae_numerus = ZEPHYRUM;
        }
        /* introitus claudens (semper) */
        {
            i32* claudens = xar_addere(lineae_indices);

            si (claudens == NIHIL)
            {
                redde FALSUM;
            }
            *claudens = ifu.instructiones_numerus;
        }
        {
            IndiciumFunctio* locellus = xar_addere(functiones_res);

            si (locellus == NIHIL)
            {
                redde FALSUM;
            }
            *locellus = ifu;
        }
    }

    /* retro: per viam, ordinata */
    per (v = ZEPHYRUM; v < numerus_viarum; v++)
    {
        Xar** sinus = (Xar**)xar_obtinere(tripla_per_viam, v);
        i32 n;
        RetroTriplum* tabula;
        IndiciumVia iv;
        i32 k;
        i32 linea_prior = (i32)-I;   /* valor impossibilis */

        iv.titulus = *(i32*)xar_obtinere(s->viae, v);
        iv.retro_lineae_primus = xar_numerus(retro_lineae);
        iv.retro_intervalla_primus = xar_numerus(retro_intervalla);
        iv.retro_numerus = ZEPHYRUM;
        tabula = (RetroTriplum*)_xar_in_tabulam(s->piscina, *sinus,
            magnitudo(RetroTriplum), &n);
        si (n > ZEPHYRUM)
        {
            qsort(tabula, (memoriae_index)n,
                magnitudo(RetroTriplum), _tripla_comparare);
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (tabula[k].linea != linea_prior)
            {
                i32* linea_locellus = xar_addere(retro_lineae);
                i32* intervallum = xar_addere(retro_intervalla);

                si (linea_locellus == NIHIL || intervallum == NIHIL)
                {
                    redde FALSUM;
                }
                *linea_locellus = tabula[k].linea;
                *intervallum = xar_numerus(retro_situs);
                iv.retro_numerus++;
                linea_prior = tabula[k].linea;
            }
            {
                IndiciumSitus* situs = xar_addere(retro_situs);

                si (situs == NIHIL)
                {
                    redde FALSUM;
                }
                situs->functio = tabula[k].functio;
                situs->instructio = tabula[k].instructio;
            }
        }
        {
            i32* clausura = xar_addere(retro_intervalla);

            si (clausura == NIHIL)
            {
                redde FALSUM;
            }
            *clausura = xar_numerus(retro_situs);
        }
        {
            IndiciumVia* locellus = xar_addere(viae_res);

            si (locellus == NIHIL)
            {
                redde FALSUM;
            }
            *locellus = iv;
        }
    }

    /* plagula: caput + directorium + sectiones (8-ordinatae) */
    {
        vacuum* sectio_data[INDICIUM_SECTIO_NUMERUS];
        i32 sectio_numeri[INDICIUM_SECTIO_NUMERUS];
        IndiciumCaput caput;
        IndiciumSectio directorium[INDICIUM_SECTIO_NUMERUS];
        i32 cursor;
        i32 g;
        FILE* plagula;

        sectio_data[INDICIUM_SECTIO_CHORDAE_DATA] =
            _xar_in_tabulam(s->piscina, s->chordae_datum,
                magnitudo(i8),
                &sectio_numeri[INDICIUM_SECTIO_CHORDAE_DATA]);
        sectio_data[INDICIUM_SECTIO_CHORDAE_OFFSETA] =
            _xar_in_tabulam(s->piscina, s->chordae_offseta,
                magnitudo(i32),
                &sectio_numeri[INDICIUM_SECTIO_CHORDAE_OFFSETA]);
        sectio_data[INDICIUM_SECTIO_VIAE] =
            _xar_in_tabulam(s->piscina, viae_res,
                magnitudo(IndiciumVia),
                &sectio_numeri[INDICIUM_SECTIO_VIAE]);
        sectio_data[INDICIUM_SECTIO_MODULI] =
            _xar_in_tabulam(s->piscina, moduli_res,
                magnitudo(IndiciumModulus),
                &sectio_numeri[INDICIUM_SECTIO_MODULI]);
        sectio_data[INDICIUM_SECTIO_FUNCTIONES] =
            _xar_in_tabulam(s->piscina, functiones_res,
                magnitudo(IndiciumFunctio),
                &sectio_numeri[INDICIUM_SECTIO_FUNCTIONES]);
        sectio_data[INDICIUM_SECTIO_LINEAE_INDICES] =
            _xar_in_tabulam(s->piscina, lineae_indices,
                magnitudo(i32),
                &sectio_numeri[INDICIUM_SECTIO_LINEAE_INDICES]);
        sectio_data[INDICIUM_SECTIO_LINEAE_RES] =
            _xar_in_tabulam(s->piscina, lineae_res,
                magnitudo(IndiciumLinea),
                &sectio_numeri[INDICIUM_SECTIO_LINEAE_RES]);
        sectio_data[INDICIUM_SECTIO_RETRO_LINEAE] =
            _xar_in_tabulam(s->piscina, retro_lineae,
                magnitudo(i32),
                &sectio_numeri[INDICIUM_SECTIO_RETRO_LINEAE]);
        sectio_data[INDICIUM_SECTIO_RETRO_INTERVALLA] =
            _xar_in_tabulam(s->piscina, retro_intervalla,
                magnitudo(i32),
                &sectio_numeri[INDICIUM_SECTIO_RETRO_INTERVALLA]);
        sectio_data[INDICIUM_SECTIO_RETRO_SITUS] =
            _xar_in_tabulam(s->piscina, retro_situs,
                magnitudo(IndiciumSitus),
                &sectio_numeri[INDICIUM_SECTIO_RETRO_SITUS]);
        sectio_data[INDICIUM_SECTIO_VARIABILIA] =
            _xar_in_tabulam(s->piscina, variabilia_res,
                magnitudo(IndiciumVariabile),
                &sectio_numeri[INDICIUM_SECTIO_VARIABILIA]);

        cursor = (i32)(magnitudo(IndiciumCaput)
            + (memoriae_index)INDICIUM_SECTIO_NUMERUS
                * magnitudo(IndiciumSectio));
        cursor = (cursor + VII) & ~(i32)VII;
        per (g = ZEPHYRUM; g < (i32)INDICIUM_SECTIO_NUMERUS; g++)
        {
            i32 octeti = (i32)((memoriae_index)sectio_numeri[g]
                * _elementi_magnitudines[g]);

            directorium[g].offset = cursor;
            directorium[g].magnitudo_octetorum = octeti;
            cursor = (cursor + octeti + VII) & ~(i32)VII;
        }
        memcpy(caput.magica, INDICIUM_MAGICA,
            magnitudo(caput.magica));
        caput.versio = (i32)INDICIUM_VERSIO;
        caput.numerus_sectionum = (i32)INDICIUM_SECTIO_NUMERUS;

        plagula = fopen(via, "wb");
        si (plagula == NIHIL)
        {
            redde FALSUM;
        }
        {
            b32 bene = VERUM;
            i32 scriptum = (i32)magnitudo(IndiciumCaput)
                + (i32)((memoriae_index)INDICIUM_SECTIO_NUMERUS
                    * magnitudo(IndiciumSectio));
            interior constans i8 ZERI[VIII] = { 0, 0, 0, 0,
                0, 0, 0, 0 };

            bene = bene && fwrite(&caput, magnitudo(IndiciumCaput),
                I, plagula) == (memoriae_index)I;
            bene = bene && fwrite(directorium,
                magnitudo(IndiciumSectio),
                (memoriae_index)INDICIUM_SECTIO_NUMERUS, plagula)
                    == (memoriae_index)INDICIUM_SECTIO_NUMERUS;
            per (g = ZEPHYRUM; bene
                && g < (i32)INDICIUM_SECTIO_NUMERUS; g++)
            {
                /* solum ordinatio: zeri usque ad offset */
                dum (bene && scriptum < directorium[g].offset)
                {
                    i32 restat = directorium[g].offset - scriptum;
                    i32 gradus = (restat < (i32)VIII)
                        ? restat : (i32)VIII;

                    bene = fwrite(ZERI, I, (memoriae_index)gradus,
                        plagula) == (memoriae_index)gradus;
                    scriptum += gradus;
                }
                si (bene
                    && directorium[g].magnitudo_octetorum
                        > ZEPHYRUM)
                {
                    bene = fwrite(sectio_data[g], I,
                        (memoriae_index)
                            directorium[g].magnitudo_octetorum,
                        plagula) == (memoriae_index)
                            directorium[g].magnitudo_octetorum;
                    scriptum += directorium[g].magnitudo_octetorum;
                }
            }
            si (fclose(plagula) != 0)
            {
                bene = FALSUM;
            }
            redde bene;
        }
    }
}

/* ==================================================
 * Lector
 * ================================================== */

structura IndiciumLector {
    constans i8* datum;
    i32 mensura;
    constans i8* sectio_datum[INDICIUM_SECTIO_NUMERUS];
    i32 sectio_numerus[INDICIUM_SECTIO_NUMERUS];
};

IndiciumLector*
indicium_aperire (Piscina* piscina, constans character* via)
{
    FILE* plagula;
    long mensura_l;
    i32 mensura;
    i8* datum;
    IndiciumLector* lector;
    constans IndiciumCaput* caput;
    constans IndiciumSectio* directorium;
    i32 g;

    si (piscina == NIHIL || via == NIHIL)
    {
        redde NIHIL;
    }
    plagula = fopen(via, "rb");
    si (plagula == NIHIL)
    {
        redde NIHIL;
    }
    fseek(plagula, 0L, SEEK_END);
    mensura_l = ftell(plagula);
    fseek(plagula, 0L, SEEK_SET);
    si (mensura_l < (long)(magnitudo(IndiciumCaput)
            + (memoriae_index)INDICIUM_SECTIO_NUMERUS
                * magnitudo(IndiciumSectio)))
    {
        fclose(plagula);
        redde NIHIL;
    }
    mensura = (i32)mensura_l;
    datum = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (datum == NIHIL
        || fread(datum, I, (memoriae_index)mensura, plagula)
            != (memoriae_index)mensura)
    {
        fclose(plagula);
        redde NIHIL;
    }
    fclose(plagula);

    caput = (constans IndiciumCaput*)(constans vacuum*)datum;
    si (memcmp(caput->magica, INDICIUM_MAGICA,
            magnitudo(caput->magica)) != 0
        || caput->versio != (i32)INDICIUM_VERSIO
        || caput->numerus_sectionum
            != (i32)INDICIUM_SECTIO_NUMERUS)
    {
        redde NIHIL;   /* magica aliena aut versio - dure reiecta */
    }
    directorium = (constans IndiciumSectio*)(constans vacuum*)
        (datum + magnitudo(IndiciumCaput));
    lector = piscina_allocare(piscina, magnitudo(IndiciumLector));
    si (lector == NIHIL)
    {
        redde NIHIL;
    }
    lector->datum = datum;
    lector->mensura = mensura;
    per (g = ZEPHYRUM; g < (i32)INDICIUM_SECTIO_NUMERUS; g++)
    {
        constans IndiciumSectio* sectio = &directorium[g];

        /* clausulae "< 0" mortuae remotae (2026-07-17): agri i32
         * formati; summa in memoriae_index (64-bit) sine volutione
         * - cohibitio summae contra mensuram valores QUOSVIS capit */
        si ((memoriae_index)sectio->offset
                + (memoriae_index)sectio->magnitudo_octetorum
                > (memoriae_index)mensura
            || (_elementi_magnitudines[g] > 0U
                && (memoriae_index)sectio->magnitudo_octetorum
                    % _elementi_magnitudines[g] != 0U))
        {
            redde NIHIL;
        }
        lector->sectio_datum[g] = datum + sectio->offset;
        lector->sectio_numerus[g] =
            (i32)((memoriae_index)sectio->magnitudo_octetorum
                / _elementi_magnitudines[g]);
    }
    redde lector;
}

chorda
indicium_chorda (constans IndiciumLector* lector, i32 index)
{
    constans i32* offseta;
    i32 numerus;

    si (lector == NIHIL)
    {
        redde _ch_vacua();
    }
    offseta = (constans i32*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_CHORDAE_OFFSETA];
    numerus = lector->sectio_numerus[
        INDICIUM_SECTIO_CHORDAE_OFFSETA] - I;
    si (index >= numerus)
    {
        redde _ch_vacua();
    }
    {
        chorda c;
        unio { constans i8* c; i8* m; } u;

        u.c = lector->sectio_datum[INDICIUM_SECTIO_CHORDAE_DATA]
            + offseta[index];
        c.datum = u.m;
        c.mensura = offseta[index + I] - offseta[index];
        redde c;
    }
}

chorda
indicium_via_chorda (constans IndiciumLector* lector, i32 via_index)
{
    constans IndiciumVia* viae;

    si (lector == NIHIL
        || via_index >= lector->sectio_numerus[INDICIUM_SECTIO_VIAE])
    {
        redde _ch_vacua();
    }
    viae = (constans IndiciumVia*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_VIAE];
    redde indicium_chorda(lector, viae[via_index].titulus);
}

i32
indicium_functiones_numerus (constans IndiciumLector* lector)
{
    si (lector == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde lector->sectio_numerus[INDICIUM_SECTIO_FUNCTIONES];
}

constans IndiciumFunctio*
indicium_functio (constans IndiciumLector* lector, i32 index)
{
    si (lector == NIHIL || index
        >= lector->sectio_numerus[INDICIUM_SECTIO_FUNCTIONES])
    {
        redde NIHIL;
    }
    redde (constans IndiciumFunctio*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_FUNCTIONES] + index;
}

s32
indicium_functionem_quaerere (constans IndiciumLector* lector,
    chorda titulus)
{
    i32 numerus;
    i32 f;

    si (lector == NIHIL)
    {
        redde -I;
    }
    numerus = lector->sectio_numerus[INDICIUM_SECTIO_FUNCTIONES];
    per (f = ZEPHYRUM; f < numerus; f++)
    {
        constans IndiciumFunctio* ifu = indicium_functio(lector, f);

        si (_ind_chordae_aequales(titulus,
                indicium_chorda(lector, ifu->titulus)))
        {
            redde (s32)f;
        }
    }
    redde -I;
}

i32
indicium_lineas_de_instructione (constans IndiciumLector* lector,
    i32 functio_index, i32 instructio,
    constans IndiciumLinea** acies_out)
{
    constans IndiciumFunctio* ifu;
    constans i32* indices;
    constans IndiciumLinea* res;
    i32 n;
    i32 imus;
    i32 summus;
    i32 primus;
    i32 numerus;

    *acies_out = NIHIL;
    ifu = indicium_functio(lector, functio_index);
    si (ifu == NIHIL || ifu->lineae_numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    indices = (constans i32*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_LINEAE_INDICES]
        + ifu->lineae_indices_primus;
    res = (constans IndiciumLinea*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_LINEAE_RES]
        + ifu->lineae_res_primus;
    n = ifu->lineae_numerus;
    si (instructio < indices[ZEPHYRUM]
        || instructio >= indices[n])   /* [n] = claudens */
    {
        redde ZEPHYRUM;
    }
    /* quaestio binaria: maximus i cum indices[i] <= instructio */
    imus = ZEPHYRUM;
    summus = n - I;
    dum (imus < summus)
    {
        i32 medius = imus + (summus - imus + I) / II;

        si (indices[medius] <= instructio)
        {
            imus = medius;
        }
        alioquin
        {
            summus = medius - I;
        }
    }
    /* acies acervata: introitus consecutivi eodem indice */
    primus = imus;
    dum (primus > ZEPHYRUM
        && indices[primus - I] == indices[imus])
    {
        primus--;
    }
    numerus = I;
    dum (primus + numerus < n
        && indices[primus + numerus] == indices[primus])
    {
        numerus++;
    }
    *acies_out = res + primus;
    redde numerus;
}

i32
indicium_situs_de_linea (constans IndiciumLector* lector,
    chorda via, i32 linea, constans IndiciumSitus** acies_out)
{
    constans IndiciumVia* viae;
    i32 numerus_viarum;
    i32 v;

    *acies_out = NIHIL;
    si (lector == NIHIL)
    {
        redde ZEPHYRUM;
    }
    viae = (constans IndiciumVia*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_VIAE];
    numerus_viarum = lector->sectio_numerus[INDICIUM_SECTIO_VIAE];
    per (v = ZEPHYRUM; v < numerus_viarum; v++)
    {
        constans i32* lineae;
        constans i32* intervalla;
        i32 imus;
        i32 summus;

        si (!_ind_chordae_aequales(via,
                indicium_chorda(lector, viae[v].titulus)))
        {
            perge;
        }
        si (viae[v].retro_numerus == ZEPHYRUM)
        {
            redde ZEPHYRUM;
        }
        lineae = (constans i32*)(constans vacuum*)
            lector->sectio_datum[INDICIUM_SECTIO_RETRO_LINEAE]
            + viae[v].retro_lineae_primus;
        intervalla = (constans i32*)(constans vacuum*)
            lector->sectio_datum[INDICIUM_SECTIO_RETRO_INTERVALLA]
            + viae[v].retro_intervalla_primus;
        imus = ZEPHYRUM;
        summus = viae[v].retro_numerus - I;
        dum (imus < summus)
        {
            i32 medius = imus + (summus - imus) / II;

            si (lineae[medius] < linea)
            {
                imus = medius + I;
            }
            alioquin
            {
                summus = medius;
            }
        }
        si (lineae[imus] != linea)
        {
            redde ZEPHYRUM;   /* congruentia exacta v0 */
        }
        *acies_out = (constans IndiciumSitus*)(constans vacuum*)
            lector->sectio_datum[INDICIUM_SECTIO_RETRO_SITUS]
            + intervalla[imus];
        redde intervalla[imus + I] - intervalla[imus];
    }
    redde ZEPHYRUM;
}

i32
indicium_variabilia_functionis (constans IndiciumLector* lector,
    i32 functio_index, constans IndiciumVariabile** acies_out)
{
    constans IndiciumFunctio* ifu;

    *acies_out = NIHIL;
    ifu = indicium_functio(lector, functio_index);
    si (ifu == NIHIL || ifu->variabilia_numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    *acies_out = (constans IndiciumVariabile*)(constans vacuum*)
        lector->sectio_datum[INDICIUM_SECTIO_VARIABILIA]
        + ifu->variabilia_prima;
    redde ifu->variabilia_numerus;
}
