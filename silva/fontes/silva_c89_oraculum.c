/* silva_c89_oraculum.c - anima semantica grammaticae c89
 * (vide caput; consilium = simulatio X + addendum X10) */

#include "silva_c89_oraculum.h"
#include "xar.h"
#include <string.h>

/* ==================================================
 * Ambulator declaratoris (X5)
 * ================================================== */

SilvaToken*
silva_c89_declaratoris_titulus (constans SilvaNodus* declarator)
{
    constans SilvaNodus* nodus = declarator;
    i32 custodia = ZEPHYRUM;

    dum (nodus != NIHIL && custodia < XXXII)
    {
        SilvaValor internum;

        custodia++;
        commutatio (nodus->genus)
        {
        casus (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS:
        {
            SilvaValor titulus =
                silva_c89_declarator_titulus_tok_titulus(nodus);

            redde (titulus.genus == SILVA_VALOR_TOKEN)
                ? titulus.datum.token : NIHIL;
        }
        casus (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS:
            internum =
                silva_c89_declarator_initiatus_declarator(nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR:
            internum =
                silva_c89_declarator_monstrator_internum(nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI:
            internum = silva_c89_declarator_aciei_internum(nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS:
            internum =
                silva_c89_declarator_functionis_internum(nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_PARENTHESIS:
            internum = silva_c89_parenthesis_internum(nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_CAMPUS:
            internum = silva_c89_campus_declarator(nodus);
            frange;
        ordinarius:
            redde NIHIL;
        }
        nodus = (internum.genus == SILVA_VALOR_NODUS)
            ? internum.datum.nodus : NIHIL;
    }
    redde NIHIL;
}

/* ==================================================
 * Registratio (X1/X2/X4)
 * ================================================== */

/* Lexema TYPEDEF nudum in lista specificatorum? (nota semantica
 * M2.0 - typedef genus proprium NON habet) */
hic_manens b32
_habet_typedef (SilvaValor specificatores)
{
    i32 i;

    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(specificatores); i++)
    {
        SilvaValor* elem =
            silva_valor_lista_obtinere(specificatores, i);

        si (elem != NIHIL && elem->genus == SILVA_VALOR_TOKEN
            && elem->datum.token != NIHIL
            && elem->datum.token->genus == SILVA_LEX_TYPEDEF)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

hic_manens vacuum
_declarationem_registrare (
    constans SilvaNodus* declaratio,
    SilvaOraculum*       oraculum)
{
    SilvaValor declaratores;
    b32 est_typedef = _habet_typedef(
        silva_c89_declaratio_specificatores(declaratio));
    i32 i;

    declaratores = silva_c89_declaratio_declaratores(declaratio);
    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(declaratores); i++)
    {
        SilvaValor* elem =
            silva_valor_lista_obtinere(declaratores, i);
        SilvaToken* titulus;

        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
        {
            perge;  /* signa COMMA interserta */
        }
        titulus = silva_c89_declaratoris_titulus(elem->datum.nodus);
        si (titulus == NIHIL || titulus->byte_offset < ZEPHYRUM)
        {
            perge;
        }
        si (est_typedef)
        {
            silva_oraculum_typum_addere_situ(oraculum,
                titulus->valor, titulus->byte_offset);
        }
        alioquin
        {
            /* trivalens (M0a C): declaratores non-typedef = nomina
             * NON-typorum nota (variabiles/functiones/acies) -
             * lectiones sizeof-typi/conversionis eorum occiduntur */
            (vacuum)silva_oraculum_non_typum_addere(oraculum,
                titulus->valor);
        }
    }
}

hic_manens vacuum
_valorem_registrare (
    SilvaValor     valor,
    SilvaOraculum* oraculum,
    i32            profunditas)
{
    si (profunditas > XXXII) redde;

    si (valor.genus == SILVA_VALOR_LISTA)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                _valorem_registrare(*elem, oraculum,
                    profunditas + I);
            }
        }
        redde;
    }
    si (valor.genus != SILVA_VALOR_NODUS
        || valor.datum.nodus == NIHIL)
    {
        redde;
    }

    commutatio (valor.datum.nodus->genus)
    {
    casus (s32)SILVA_C89_GENUS_DECLARATIO:
        _declarationem_registrare(valor.datum.nodus, oraculum);
        frange;
    casus (s32)SILVA_C89_GENUS_CONDITIONALIS:
        _valorem_registrare(
            silva_c89_conditionalis_rami(valor.datum.nodus),
            oraculum, profunditas + I);
        frange;
    casus (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS:
        _valorem_registrare(
            silva_c89_ramus_sumptus_contentum(valor.datum.nodus),
            oraculum, profunditas + I);
        frange;
    ordinarius:
        /* AMBIGUUS praetermittitur (X4 exclusivum); sententiae
         * et ERROR typos non ferunt */
        frange;
    }
}

vacuum
silva_c89_typos_registrare (
    SilvaValor     radix,
    SilvaOraculum* oraculum)
{
    si (oraculum == NIHIL) redde;
    _valorem_registrare(radix, oraculum, ZEPHYRUM);
}

/* ==================================================
 * Ambulatio praecommissionis INTERTEXTA (sanatio 2026-07-06):
 * registratio et examinatio in UNA ambulatione ordine fluxus.
 * Typus visibilis = iam registratus cum ambiguum attingitur
 * (semantica puncti declarationis ISO; coordinata positionis
 * NULLA - comparatio situm trans fontes falsa erat, 2,382
 * lectiones falso retentae; silva-incrementalitas.md par V).
 * sine_registratione: intra corpora (decisiones 13 - typedef
 * localis non registratur) et intra lectiones ambiguorum (X4
 * exclusivum); examinatio tamen UBIQUE (ordo locorum = ordo
 * fontis, validatio ordinis locorum id spondet).
 * ================================================== */

hic_manens vacuum _percurrere (SilvaValor valor,
    SilvaOraculum* oraculum, b32 sine_registratione,
    i32 profunditas);
hic_manens vacuum _ambiguum_examinare (constans SilvaNodus* ambiguum,
    constans SilvaOraculum* oraculum,
    SilvaResolutioResponsum* responsum);

hic_manens vacuum
_ambiguum_stipare (
    constans SilvaNodus* ambiguum,
    SilvaOraculum*       oraculum)
{
    SilvaResolutioResponsum responsum;

    responsum.victor = -I;
    responsum.discriminans = NIHIL;
    _ambiguum_examinare(ambiguum, oraculum, &responsum);
    (vacuum)silva_oraculum_responsum_ponere(oraculum, ambiguum,
        responsum.victor);
}

hic_manens vacuum
_percurrere (
    SilvaValor     valor,
    SilvaOraculum* oraculum,
    b32            sine_registratione,
    i32            profunditas)
{
    constans SilvaNodus* nodus;
    i32 i;

    si (profunditas > LXIV) redde;

    si (valor.genus == SILVA_VALOR_LISTA)
    {
        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                _percurrere(*elem, oraculum, sine_registratione,
                    profunditas + I);
            }
        }
        redde;
    }
    si (valor.genus != SILVA_VALOR_NODUS
        || valor.datum.nodus == NIHIL)
    {
        redde;
    }
    nodus = valor.datum.nodus;

    commutatio (nodus->genus)
    {
    casus (s32)SILVA_C89_GENUS_AMBIGUUS:
        _ambiguum_stipare(nodus, oraculum);
        /* descende in lectiones: ambigua nidificata verdicta
         * quoque accipiunt (eodem statu oraculi - eadem positio
         * fluxus); registratio intus suppressa (X4) */
        _percurrere(silva_c89_ambiguus_interpretationes(nodus),
            oraculum, VERUM, profunditas + I);
        redde;
    casus (s32)SILVA_C89_GENUS_DECLARATIO:
        si (!sine_registratione)
        {
            _declarationem_registrare(nodus, oraculum);
        }
        frange;  /* loci infra descenduntur (ambigua initiatorum) */
    casus (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS:
        si (!sine_registratione)
        {
            /* nomen functionis = non-typus notus (trivalens) */
            SilvaValor decl_v =
                silva_c89_definitio_functionis_declarator(nodus);

            si (decl_v.genus == SILVA_VALOR_NODUS)
            {
                SilvaToken* titulus = silva_c89_declaratoris_titulus(
                    decl_v.datum.nodus);

                si (titulus != NIHIL
                    && titulus->byte_offset >= ZEPHYRUM)
                {
                    (vacuum)silva_oraculum_non_typum_addere(
                        oraculum, titulus->valor);
                }
            }
        }
        sine_registratione = VERUM;  /* decisiones 13 */
        frange;
    casus (s32)SILVA_C89_GENUS_CORPUS:
        sine_registratione = VERUM;  /* decisiones 13 */
        frange;
    casus (s32)SILVA_C89_GENUS_RAMUS_OMISSUS:
        redde;  /* cruda - lexemata sola */
    ordinarius:
        frange;
    }

    /* descensio generica per locos, ordine fontis */
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans SilvaValor* locus = &nodus->loci[i];

        si (locus->genus == SILVA_VALOR_NODUS
            || locus->genus == SILVA_VALOR_LISTA)
        {
            _percurrere(*locus, oraculum, sine_registratione,
                profunditas + I);
        }
    }
}

vacuum
silva_c89_praecommissio (SilvaValor radix, vacuum* datum)
{
    SilvaOraculum* oraculum = (SilvaOraculum*)datum;

    si (oraculum == NIHIL) redde;
    silva_oraculum_responsa_vacare(oraculum);
    _percurrere(radix, oraculum, FALSUM, ZEPHYRUM);
}

/* ==================================================
 * Resolutor (X6 + X10)
 * ================================================== */

/* Examen lectionis unius */
nomen structura {
    b32 invalidum;        /* combinatio impossibilis (X10) */
    b32 nominatus_adest;
    b32 ignotus_adest;    /* nominatus oraculo NUNC ignotus */
    b32 non_typus_adest;  /* nominatus NON-TYPUS notus (trivalens):
                           * lectio impossibilis - occiditur */
    b32 species_adest;    /* nodus specificatores ferens (politica) */
} ExamenLectionis;

hic_manens vacuum _nodum_examinare (constans SilvaNodus* nodus,
    constans SilvaOraculum* oraculum,
    ExamenLectionis* examen, i32 profunditas);

/* Lista specificatorum: numerare atoma typorum + combinationem
 * probare + nominatos contra oraculum. SANATIO 2026-07-06:
 * visibilitas per ORDINEM ambulationis, non per situm — quaestio
 * est "novit NUNC?" (contenta oraculi in momento ambulationis
 * IPSA sunt copia visibilis; comparatio situm trans fontes falsa
 * erat — silva-incrementalitas.md par V). */
hic_manens vacuum
_specificatores_examinare (
    SilvaValor              specificatores,
    constans SilvaOraculum* oraculum,
    ExamenLectionis*        examen,
    i32                     profunditas)
{
    i32 primitivi = ZEPHYRUM;
    i32 nominati = ZEPHYRUM;
    i32 tags = ZEPHYRUM;
    i32 i;

    (vacuum)profunditas;
    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(specificatores); i++)
    {
        SilvaValor* elem =
            silva_valor_lista_obtinere(specificatores, i);
        SilvaNodus* nodus;

        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
        {
            perge;  /* lexemata (qualificatores/classes) */
        }
        nodus = elem->datum.nodus;
        commutatio (nodus->genus)
        {
        casus (s32)SILVA_C89_GENUS_TYPUS_PRIMITIVUS:
            primitivi++;
            frange;
        casus (s32)SILVA_C89_GENUS_TYPUS_NOMINATUS:
        {
            SilvaValor titulus =
                silva_c89_typus_nominatus_tok_titulus(nodus);

            nominati++;
            examen->nominatus_adest = VERUM;
            si (titulus.genus == SILVA_VALOR_TOKEN
                && !silva_oraculum_typum_novit(oraculum,
                       titulus.datum.token->valor))
            {
                examen->ignotus_adest = VERUM;
                /* trivalens: NON-typus notus (nec typus - umbratio
                 * typum vincere sinit) = lectio impossibilis */
                si (silva_oraculum_non_typum_novit(oraculum,
                        titulus.datum.token->valor))
                {
                    examen->non_typus_adest = VERUM;
                }
            }
            frange;
        }
        casus (s32)SILVA_C89_GENUS_STRUCTURA:
        casus (s32)SILVA_C89_GENUS_UNIO:
        casus (s32)SILVA_C89_GENUS_ENUMERATIO:
            /* membra interiora descensione generica loci
             * visitantur (elementa listae nodi sunt) - descensio
             * explicita HIC duplicaret (exponentialiter in
             * nidificatione) */
            tags++;
            frange;
        ordinarius:
            frange;
        }
    }

    /* X10: nominatus cum quolibet alio typo, tag cum primitivo,
     * tag cum tag = impossibile (C89 3.5.2 combinationes);
     * primitivi plures LICENT (cursus qualificatoribus scissi) */
    si (nominati + tags > I
        || ((nominati + tags) >= I && primitivi >= I))
    {
        examen->invalidum = VERUM;
    }
}

hic_manens vacuum
_nodum_examinare (
    constans SilvaNodus*    nodus,
    constans SilvaOraculum* oraculum,
    ExamenLectionis*        examen,
    i32                     profunditas)
{
    SilvaValor specificatores;
    b32 habet_species = FALSUM;
    i32 i;

    si (nodus == NIHIL || profunditas > LXIV) redde;

    commutatio (nodus->genus)
    {
    casus (s32)SILVA_C89_GENUS_AMBIGUUS:
    {
        /* Furca nidificata: vexilla nominatus/ignotus fluunt ut
         * ante (QUAELIBET lectio); non_typus fluit SOLUM si OMNES
         * lectiones interiores eum ferunt (aliter lectio exterior
         * ob alternativam interiorem NON electam occideretur -
         * contagio falsa, inventa Chunk C per "values") */
        SilvaValor interp =
            silva_c89_ambiguus_interpretationes(nodus);
        i32 m = silva_valor_lista_numerus(interp);
        i32 k;
        b32 omnes_non_typi = (m > ZEPHYRUM) ? VERUM : FALSUM;

        per (k = ZEPHYRUM; k < m; k++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(interp, k);
            ExamenLectionis sub;

            sub.invalidum = FALSUM;
            sub.nominatus_adest = FALSUM;
            sub.ignotus_adest = FALSUM;
            sub.non_typus_adest = FALSUM;
            sub.species_adest = FALSUM;
            si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
            {
                omnes_non_typi = FALSUM;
                perge;
            }
            _nodum_examinare(elem->datum.nodus, oraculum, &sub,
                profunditas + I);
            si (sub.nominatus_adest)
            {
                examen->nominatus_adest = VERUM;
            }
            si (sub.ignotus_adest)
            {
                examen->ignotus_adest = VERUM;
            }
            si (sub.species_adest)
            {
                examen->species_adest = VERUM;
            }
            si (!sub.non_typus_adest)
            {
                omnes_non_typi = FALSUM;
            }
        }
        si (omnes_non_typi)
        {
            examen->non_typus_adest = VERUM;
        }
        redde;
    }
    casus (s32)SILVA_C89_GENUS_DECLARATIO:
        specificatores = silva_c89_declaratio_specificatores(nodus);
        habet_species = VERUM;
        frange;
    casus (s32)SILVA_C89_GENUS_MEMBRUM:
        specificatores = silva_c89_membrum_specificatores(nodus);
        habet_species = VERUM;
        frange;
    casus (s32)SILVA_C89_GENUS_SPECIES_TYPI:
        specificatores =
            silva_c89_species_typi_specificatores(nodus);
        habet_species = VERUM;
        frange;
    casus (s32)SILVA_C89_GENUS_PARAMETRUM:
        specificatores = silva_c89_parametrum_specificatores(nodus);
        habet_species = VERUM;
        frange;
    casus (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS:
        /* M2c Chunk C: lectio K&R falsa ("int f" ut specificatores
         * bini, declarator parenthesi) combinatione invalida est -
         * X10 eam necat et lectio VERA K&R sola superstes fit
         * (ISO 6.9.1: identificatores listae nomina SUNT - regula
         * linguae, resolutio determinata recta) */
        specificatores =
            silva_c89_definitio_functionis_specificatores(nodus);
        habet_species = VERUM;
        frange;
    ordinarius:
        frange;
    }
    si (habet_species)
    {
        examen->species_adest = VERUM;
        _specificatores_examinare(specificatores, oraculum,
            examen, profunditas);
    }

    /* descensio generica per locos (tags intra specificatores
     * iam separatim descensi - iteratio innocua, examen
     * idempotens praeter numerationem listae SUAE, quae per
     * locum semel visitatur) */
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans SilvaValor* valor = &nodus->loci[i];

        si (valor->genus == SILVA_VALOR_NODUS)
        {
            _nodum_examinare(valor->datum.nodus, oraculum,
                examen, profunditas + I);
        }
        alioquin si (valor->genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(*valor); k++)
            {
                SilvaValor* elem =
                    silva_valor_lista_obtinere(*valor, k);

                si (elem != NIHIL
                    && elem->genus == SILVA_VALOR_NODUS)
                {
                    _nodum_examinare(elem->datum.nodus, oraculum,
                        examen, profunditas + I);
                }
            }
        }
    }
}

/* Situs primi lexematis subarboris (byte_offset >= 0) */
hic_manens s32
_situs_primi (constans SilvaNodus* nodus, i32 profunditas)
{
    i32 i;

    si (nodus == NIHIL || profunditas > LXIV) redde -I;

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans SilvaValor* valor = &nodus->loci[i];

        si (valor->genus == SILVA_VALOR_TOKEN
            && valor->datum.token != NIHIL
            && valor->datum.token->byte_offset >= ZEPHYRUM)
        {
            redde valor->datum.token->byte_offset;
        }
        si (valor->genus == SILVA_VALOR_NODUS)
        {
            s32 situs = _situs_primi(valor->datum.nodus,
                profunditas + I);

            si (situs >= ZEPHYRUM) redde situs;
        }
        si (valor->genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(*valor); k++)
            {
                SilvaValor* elem =
                    silva_valor_lista_obtinere(*valor, k);
                s32 situs = -I;

                si (elem == NIHIL) perge;
                si (elem->genus == SILVA_VALOR_TOKEN
                    && elem->datum.token != NIHIL
                    && elem->datum.token->byte_offset >= ZEPHYRUM)
                {
                    situs = elem->datum.token->byte_offset;
                }
                alioquin si (elem->genus == SILVA_VALOR_NODUS)
                {
                    situs = _situs_primi(elem->datum.nodus,
                        profunditas + I);
                }
                si (situs >= ZEPHYRUM) redde situs;
            }
        }
    }
    redde -I;
}

#define LECTIONES_MAXIMAE XVI

/* Examinatio ambigui sine positione: filtrum X10 + scala
 * superstitis unici / typo-positivae unicae. Vocata ab
 * ambulatione intertexta (verdicta stipanda) atque a recidiva
 * resolutoris (vocator directus sine ambulatione). */
hic_manens vacuum
_ambiguum_examinare (
    constans SilvaNodus*     ambiguum,
    constans SilvaOraculum*  oraculum,
    SilvaResolutioResponsum* responsum)
{
    SilvaValor interpretationes =
        silva_c89_ambiguus_interpretationes(ambiguum);
    ExamenLectionis examina[LECTIONES_MAXIMAE];
    i32 numerus = silva_valor_lista_numerus(interpretationes);
    i32 superstites = ZEPHYRUM;
    s32 superstes_ultimus = -I;
    i32 typo_positivae = ZEPHYRUM;
    s32 typo_positiva_ultima = -I;
    i32 i;

    si (numerus > LECTIONES_MAXIMAE) redde;  /* numquam visum */

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaValor* elem =
            silva_valor_lista_obtinere(interpretationes, i);

        examina[i].invalidum = FALSUM;
        examina[i].nominatus_adest = FALSUM;
        examina[i].ignotus_adest = FALSUM;
        examina[i].non_typus_adest = FALSUM;
        examina[i].species_adest = FALSUM;
        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
        {
            examina[i].invalidum = VERUM;
            perge;
        }
        _nodum_examinare(elem->datum.nodus, oraculum,
            &examina[i], ZEPHYRUM);

        /* trivalens: lectio quae non-typum notum ut typum postulat
         * OCCIDITUR (eadem vi ac combinatio impossibilis X10) */
        si (examina[i].non_typus_adest)
        {
            examina[i].invalidum = VERUM;
        }

        si (!examina[i].invalidum)
        {
            superstites++;
            superstes_ultimus = (s32)i;
            si (examina[i].nominatus_adest
                && !examina[i].ignotus_adest)
            {
                typo_positivae++;
                typo_positiva_ultima = (s32)i;
            }
        }
    }

    /* (b) superstes unicus: ceterae combinatione impossibiles */
    si (superstites == I)
    {
        responsum->victor = superstes_ultimus;
        redde;
    }
    /* (c) lectio typo-positiva unica (typi NUNC noti - ordo
     * ambulationis pro X3) */
    si (superstites > I && typo_positivae == I)
    {
        responsum->victor = typo_positiva_ultima;
        redde;
    }
    /* (d) nullum responsum - retentio */
}

vacuum
silva_c89_resolutor (
    constans SilvaNodus*     ambiguum,
    constans SilvaOraculum*  oraculum,
    vacuum*                  datum,
    SilvaResolutioResponsum* responsum)
{
    s32 victor;

    (vacuum)datum;
    si (silva_oraculum_responsum_quaerere(oraculum, ambiguum,
            &victor))
    {
        /* verdictum praecomputatum ambulationis; victor -1 =
         * retentio DECISA */
        si (victor >= ZEPHYRUM)
        {
            responsum->victor = victor;
        }
        redde;
    }
    /* recidiva: vocator directus sine ambulatione praecommissionis
     * (omnes typi noti visibiles - positio nulla) */
    _ambiguum_examinare(ambiguum, oraculum, responsum);
}

/* ==================================================
 * Vista declarationum (Chunk D)
 * ================================================== */

hic_manens constans character*
_genus_titulus (s32 genus)
{
    si (genus < ZEPHYRUM
        || genus >= (s32)SILVA_C89_REGISTRUM.numerus_generum)
    {
        redde "";
    }
    redde SILVA_C89_REGISTRUM.genera[genus].titulus;
}

/* Ordinem implere ex lexemate tituli (aut ex nodo primo) */
hic_manens vacuum
_vistam_implere (
    SilvaDeclaratioVista* vista,
    constans SilvaNodus*  nodus_ordinis,
    SilvaToken*           titulus)
{
    vista->genus = _genus_titulus(nodus_ordinis->genus);
    vista->est_typedef = FALSUM;
    si (titulus != NIHIL)
    {
        vista->titulus = titulus->valor;
        vista->linea = (s32)titulus->linea;
        vista->situs = titulus->byte_offset;
        vista->fons_index = titulus->fons_index;
    }
    alioquin
    {
        vista->titulus.mensura = ZEPHYRUM;
        vista->titulus.datum = NIHIL;
        vista->linea = -I;
        vista->situs = _situs_primi(nodus_ordinis, ZEPHYRUM);
        vista->fons_index = -I;
    }
}

/* Tag primum in specificatoribus (structura/unio/enumeratio) */
hic_manens constans SilvaNodus*
_tag_specificatorum (SilvaValor specificatores)
{
    i32 i;

    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(specificatores); i++)
    {
        SilvaValor* elem =
            silva_valor_lista_obtinere(specificatores, i);

        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        commutatio (elem->datum.nodus->genus)
        {
        casus (s32)SILVA_C89_GENUS_STRUCTURA:
        casus (s32)SILVA_C89_GENUS_UNIO:
        casus (s32)SILVA_C89_GENUS_ENUMERATIO:
            redde elem->datum.nodus;
        ordinarius:
            frange;
        }
    }
    redde NIHIL;
}

/* Ambulatio vistarum: numerat ordines; si quaesitum >= 0 et
 * attingitur, vistam implet et VERUM reddit */
hic_manens b32
_vistas_ambulare (
    SilvaValor            valor,
    i32*                  numerator,
    s32                   quaesitum,
    SilvaDeclaratioVista* vista,
    constans SilvaNodus** nodus_out,
    i32                   profunditas)
{
    si (profunditas > XXXII) redde FALSUM;

    si (valor.genus == SILVA_VALOR_LISTA)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL
                && _vistas_ambulare(*elem, numerator, quaesitum,
                       vista, nodus_out, profunditas + I))
            {
                redde VERUM;
            }
        }
        redde FALSUM;
    }
    si (valor.genus != SILVA_VALOR_NODUS
        || valor.datum.nodus == NIHIL)
    {
        redde FALSUM;
    }

    commutatio (valor.datum.nodus->genus)
    {
    casus (s32)SILVA_C89_GENUS_DECLARATIO:
    {
        constans SilvaNodus* declaratio = valor.datum.nodus;
        SilvaValor declaratores =
            silva_c89_declaratio_declaratores(declaratio);
        i32 numerus_decl =
            silva_valor_lista_numerus(declaratores);

        si (numerus_decl == ZEPHYRUM)
        {
            /* nuda: ordo unus, tag = titulus */
            constans SilvaNodus* tag = _tag_specificatorum(
                silva_c89_declaratio_specificatores(declaratio));

            si (tag != NIHIL)
            {
                si ((s32)*numerator == quaesitum && vista != NIHIL)
                {
                    SilvaValor tok;

                    commutatio (tag->genus)
                    {
                    casus (s32)SILVA_C89_GENUS_STRUCTURA:
                        tok = silva_c89_structura_tok_titulus(tag);
                        frange;
                    casus (s32)SILVA_C89_GENUS_UNIO:
                        tok = silva_c89_unio_tok_titulus(tag);
                        frange;
                    ordinarius:
                        tok = silva_c89_enumeratio_tok_titulus(tag);
                        frange;
                    }
                    _vistam_implere(vista, tag,
                        (tok.genus == SILVA_VALOR_TOKEN)
                            ? tok.datum.token : NIHIL);
                    vista->est_typedef = _habet_typedef(
                        silva_c89_declaratio_specificatores(
                            declaratio));
                    si (nodus_out != NIHIL) *nodus_out = declaratio;
                    redde VERUM;
                }
                (*numerator)++;
            }
            redde FALSUM;
        }
        {
            i32 i;

            per (i = ZEPHYRUM; i < numerus_decl; i++)
            {
                SilvaValor* elem =
                    silva_valor_lista_obtinere(declaratores, i);

                si (elem == NIHIL
                    || elem->genus != SILVA_VALOR_NODUS)
                {
                    perge;  /* signa COMMA */
                }
                si ((s32)*numerator == quaesitum && vista != NIHIL)
                {
                    _vistam_implere(vista, elem->datum.nodus,
                        silva_c89_declaratoris_titulus(
                            elem->datum.nodus));
                    vista->est_typedef = _habet_typedef(
                        silva_c89_declaratio_specificatores(
                            declaratio));
                    si (nodus_out != NIHIL)
                    {
                        *nodus_out = declaratio;
                    }
                    redde VERUM;
                }
                (*numerator)++;
            }
        }
        redde FALSUM;
    }
    casus (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS:
    {
        /* FUNCTIONES (M2c Chunk C): ordo unus per definitionem,
         * titulus per catenam declaratoris (X5) */
        constans SilvaNodus* definitio = valor.datum.nodus;

        si ((s32)*numerator == quaesitum && vista != NIHIL)
        {
            SilvaValor declarator =
                silva_c89_definitio_functionis_declarator(
                    definitio);

            _vistam_implere(vista, definitio,
                (declarator.genus == SILVA_VALOR_NODUS)
                    ? silva_c89_declaratoris_titulus(
                          declarator.datum.nodus)
                    : NIHIL);
            si (nodus_out != NIHIL) *nodus_out = definitio;
            redde VERUM;
        }
        (*numerator)++;
        redde FALSUM;
    }
    casus (s32)SILVA_C89_GENUS_CONDITIONALIS:
        redde _vistas_ambulare(
            silva_c89_conditionalis_rami(valor.datum.nodus),
            numerator, quaesitum, vista, nodus_out, profunditas + I);
    casus (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS:
        redde _vistas_ambulare(
            silva_c89_ramus_sumptus_contentum(valor.datum.nodus),
            numerator, quaesitum, vista, nodus_out, profunditas + I);
    casus (s32)SILVA_C89_GENUS_AMBIGUUS:
    {
        /* lectio canonica sola (spina) */
        SilvaValor interps = silva_c89_ambiguus_interpretationes(
            valor.datum.nodus);
        SilvaValor canonica = silva_c89_ambiguus_canonica(
            valor.datum.nodus);
        SilvaValor* electa;

        si (canonica.genus != SILVA_VALOR_INDEX) redde FALSUM;
        electa = silva_valor_lista_obtinere(interps,
            (i32)canonica.datum.index);
        si (electa == NIHIL) redde FALSUM;
        redde _vistas_ambulare(*electa, numerator, quaesitum,
            vista, nodus_out, profunditas + I);
    }
    ordinarius:
        redde FALSUM;
    }
}

i32
silva_c89_declarationes_numerus (constans SilvaParsura* parsura)
{
    i32 numerator = ZEPHYRUM;

    si (parsura == NIHIL || parsura->commissio == NIHIL)
    {
        redde ZEPHYRUM;
    }
    _vistas_ambulare(parsura->commissio->radix, &numerator, -I,
        NIHIL, NIHIL, ZEPHYRUM);
    redde numerator;
}

b32
silva_c89_declaratio_vista (
    constans SilvaParsura* parsura,
    i32                    index,
    SilvaDeclaratioVista*  vista)
{
    i32 numerator = ZEPHYRUM;

    si (parsura == NIHIL || parsura->commissio == NIHIL
        || vista == NIHIL)
    {
        redde FALSUM;
    }
    redde _vistas_ambulare(parsura->commissio->radix, &numerator,
        (s32)index, vista, NIHIL, ZEPHYRUM);
}

SilvaScriptura
silva_c89_functionis_subscriptio (
    Piscina*               piscina,
    constans SilvaParsura* parsura,
    i32                    index)
{
    SilvaScriptura fructus;
    SilvaDeclaratioVista vista;
    constans SilvaNodus* nodus = NIHIL;
    i32 numerator = ZEPHYRUM;

    fructus.successus = FALSUM;
    fructus.textus.mensura = ZEPHYRUM;
    fructus.textus.datum = NIHIL;
    fructus.causa = "ordo non inventus";
    fructus.sedes = NIHIL;

    si (piscina == NIHIL || parsura == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde fructus;
    }
    si (!_vistas_ambulare(parsura->commissio->radix, &numerator,
            (s32)index, &vista, &nodus, ZEPHYRUM)
        || nodus == NIHIL)
    {
        redde fructus;
    }
    si (nodus->genus != (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
    {
        fructus.causa = "ordo non functionis";
        fructus.sedes = nodus;
        redde fructus;
    }

    /* specificatores (absunt in forma int-implicita) + declarator,
     * SINE corpore et declarationibus-kr - subscriptio octetim ex
     * arbore (trivia intacta, spatia fontis manent) */
    {
        SilvaScriptura pars_spec;
        SilvaScriptura pars_decl;
        SilvaValor specificatores =
            silva_c89_definitio_functionis_specificatores(nodus);
        SilvaValor declarator =
            silva_c89_definitio_functionis_declarator(nodus);
        b32 habet_spec =
            (specificatores.genus == SILVA_VALOR_LISTA
             && silva_valor_lista_numerus(specificatores)
                > ZEPHYRUM);

        pars_spec.successus = VERUM;
        pars_spec.textus.mensura = ZEPHYRUM;
        pars_spec.textus.datum = NIHIL;
        si (habet_spec)
        {
            pars_spec = silva_scribere_valorem(piscina,
                specificatores, &SILVA_C89_REGISTRUM,
                parsura->expansio);
            si (!pars_spec.successus) redde pars_spec;
        }
        pars_decl = silva_scribere_valorem(piscina, declarator,
            &SILVA_C89_REGISTRUM, parsura->expansio);
        si (!pars_decl.successus) redde pars_decl;

        fructus.textus.mensura = pars_spec.textus.mensura
            + pars_decl.textus.mensura;
        fructus.textus.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)(fructus.textus.mensura > ZEPHYRUM
                ? fructus.textus.mensura : (i32)I));
        si (fructus.textus.datum == NIHIL)
        {
            fructus.textus.mensura = ZEPHYRUM;
            fructus.causa = "piscina exhausta";
            redde fructus;
        }
        si (pars_spec.textus.mensura > ZEPHYRUM)
        {
            memcpy(fructus.textus.datum, pars_spec.textus.datum,
                (size_t)pars_spec.textus.mensura);
        }
        si (pars_decl.textus.mensura > ZEPHYRUM)
        {
            memcpy(fructus.textus.datum + pars_spec.textus.mensura,
                pars_decl.textus.datum,
                (size_t)pars_decl.textus.mensura);
        }
        fructus.successus = VERUM;
        fructus.causa = NIHIL;
        fructus.sedes = NIHIL;
    }

    /* Trivia PRAECEDENTIA tondere (albispatia + commenta) -
     * subscriptio textus ORDINIS est, non textus roundtrip;
     * lexema primum trivia praecedentia ferre potest (post
     * directivam / initio segmenti commenta PRORSUM adhaerent -
     * inventum M2c: vexillum "/ * ==== * /" titulos ordinum
     * implebat) */
    {
        i32 a = ZEPHYRUM;

        dum (a < fructus.textus.mensura)
        {
            i8 c = fructus.textus.datum[a];

            si (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            {
                a++;
                perge;
            }
            si (c == '/' && a + I < fructus.textus.mensura
                && fructus.textus.datum[a + I] == '*')
            {
                i32 b = a + II;

                dum (b + I < fructus.textus.mensura
                    && !(fructus.textus.datum[b] == '*'
                         && fructus.textus.datum[b + I] == '/'))
                {
                    b++;
                }
                si (b + I < fructus.textus.mensura)
                {
                    a = b + II;
                    perge;
                }
            }
            frange;
        }
        fructus.textus.datum += a;
        fructus.textus.mensura -= a;
    }
    redde fructus;
}

/* ==================================================
 * Politica spinae canonicae (X8) + involucrum
 * ================================================== */

hic_manens vacuum
_politica_expressionis (
    constans SilvaNodus*     ambiguum,
    constans SilvaOraculum*  oraculum,
    vacuum*                  datum,
    SilvaResolutioResponsum* responsum)
{
    SilvaValor interpretationes =
        silva_c89_ambiguus_interpretationes(ambiguum);
    i32 sine_specie = ZEPHYRUM;
    s32 ultima = -I;
    i32 i;

    (vacuum)oraculum;
    (vacuum)datum;
    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(interpretationes); i++)
    {
        SilvaValor* elem =
            silva_valor_lista_obtinere(interpretationes, i);
        ExamenLectionis examen;

        examen.invalidum = FALSUM;
        examen.nominatus_adest = FALSUM;
        examen.ignotus_adest = FALSUM;
        examen.species_adest = FALSUM;
        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        _nodum_examinare(elem->datum.nodus, NIHIL,
            &examen, ZEPHYRUM);
        si (!examen.species_adest)
        {
            sine_specie++;
            ultima = (s32)i;
        }
    }
    /* lectio PURE expressionis unica -> canonica; ambigua
     * eiusdem generis ordinem motoris retinent */
    si (sine_specie == I)
    {
        responsum->victor = ultima;
    }
}

i32
silva_c89_politicam_imponere (
    SilvaCommissio* commissio,
    SilvaOraculum*  oraculum)
{
    redde (i32)silva_recanonicare(commissio, oraculum,
        _politica_expressionis, NIHIL);
}

constans SilvaGrammatica SILVA_C89_GRAMMATICA = {
    &SILVA_C89_TABULA,
    &SILVA_C89_REGISTRUM,
    silva_c89_construere,
    silva_c89_ambiguum_fabricare,
    silva_c89_praecommissio
};

SilvaParsura*
silva_c89_parsare (
    Piscina*            piscina,
    constans character* via,
    constans character* fons,
    i32                 mensura,
    SilvaOraculum*      oraculum)
{
    SilvaParsura* parsura;

    si (oraculum == NIHIL)
    {
        oraculum = silva_oraculum_creare(piscina);
        si (oraculum == NIHIL) redde NIHIL;
    }
    parsura = silva_parsare(piscina, via, fons, mensura,
        &SILVA_C89_GRAMMATICA, oraculum, silva_c89_resolutor,
        (vacuum*)oraculum);
    si (parsura != NIHIL && parsura->commissio != NIHIL)
    {
        silva_c89_politicam_imponere(parsura->commissio, oraculum);
    }
    redde parsura;
}

SilvaParsura*
silva_c89_parsare_cum_contextu (
    Piscina*                 piscina,
    constans SilvaContextus* contextus,
    constans character*      via,
    constans character*      fons,
    i32                      mensura,
    SilvaOraculum*           oraculum)
{
    SilvaParsura* parsura;

    si (oraculum == NIHIL)
    {
        oraculum = silva_oraculum_creare(piscina);
        si (oraculum == NIHIL) redde NIHIL;
    }
    parsura = silva_parsare_cum_contextu(piscina, contextus, via,
        fons, mensura, &SILVA_C89_GRAMMATICA, oraculum,
        silva_c89_resolutor, (vacuum*)oraculum);
    si (parsura != NIHIL && parsura->commissio != NIHIL)
    {
        silva_c89_politicam_imponere(parsura->commissio, oraculum);
    }
    redde parsura;
}
