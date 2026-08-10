#ifndef CANON_H
#define CANON_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "tabula_dispersa.h"
#include "stml.h"

/* ==================================================
 * canon - schema STML generale (gradus II)
 *
 * Domus quinque dialectos STML scribit (natura, librarium,
 * grammatica, aedilis, quaestiones) et UNA sola custodem habebat.
 * Hoc fundamentum ceteris dat.
 *
 * CANON IPSE STML EST (se hospitans): plagula *.canon dialectum
 * describit; registrum in radice (canones.registrum) extensiones
 * ad canones ligat.
 *
 * QUID CANON PRAESTAT (gradus II - 'schema'):
 *   - vocabularium CLAUSUM: elementa et attributa ignota = vitium
 *   - cardinalitas liberorum (minimum/maximum per genus)
 *   - genera attributorum (nomen/numerus/veritas/dies/electio/textus)
 *   - necessitas attributorum
 *   - textus ubi non licet (spatium album semper licet)
 *   - unicitas nominum super genera elementorum plura
 *
 * QUID NON PRAESTAT, CONSULTO:
 *   - resolutionem citationum (gradus III) - clavis quae rem
 *     exsistentem nominet
 *   - validitatem contra MODELLUM COMPUTATUM (gradus IV) -
 *     hereditas, graphus, quidquid arborem interpretari poscit
 *   - co-occurrentias ('ad AUT a', 'optio si genus=electio')
 * Ea vocantis sunt. Canon fundamentum praestat, non sapientiam;
 * lingua expressionum hic NON invenitur, quia illa via schemata
 * in linguas programmandi vertit (XSD testis).
 *
 * CANON INFIXUS (2026-08-06): documentum canonem proprium ut
 * LIBERUM PRIMUM radicis ferre potest - <radix><canon>...</canon>
 * contentum...</radix>. Mos subsidii interni DTD, sine entitatibus
 * (unde DTD periit). Nomen 'canon' ut liberum primum radicis ideo
 * RESERVATUM est per dialectos omnes: iudex id semper contractum
 * legit, numquam contentum - liberum 'canon' ALIBI positum
 * vocabularium dialecti ordinarium manet.
 *
 * PRAECEDENTIA: AMBO IUDICANT (decretum Fran 2026-08-06). Canon
 * registri = pavimentum domus, quod documentum deponere non potest;
 * canon infixus addere solum potest. Unde documentum contractum
 * suum clam debilitare nequit (vitium 'schemaLocation' XSD -
 * documentum de schemate suo mentiens). Infixus ipse contra
 * canonem canonum (canon.canon) iudicatur: infixus fractus
 * clamat, non tacite iners fit.
 * ================================================== */

/* ==================================================
 * Genera valorum attributorum
 * ================================================== */

nomen enumeratio {
    CANON_GENUS_TEXTUS     = I,   /* quidlibet */
    CANON_GENUS_NOMEN      = II,  /* identificator: [a-zA-Z0-9_*] */
    CANON_GENUS_NUMERUS    = III, /* cifrae, signum optionale */
    CANON_GENUS_VERITAS    = IV,  /* verum | falsum */
    CANON_GENUS_DIES       = V,   /* yyyy | yyyy-mm | yyyy-mm-dd */
    CANON_GENUS_ELECTIO    = VI,  /* ex <optio> enumeratis */
    /* nomen + lineola: nomina kebab ('definitio-functionis').
     * COACTUM dialecto tertio (grammatica) - eius nomina lineolis
     * iunguntur, quae 'nomen' vetat; sine hoc genere grammatica
     * describi non potuit. Superset nominis consulto: unum genus
     * 'verbum sine spatiis' latius, non genus per dialectum. */
    CANON_GENUS_COMPOSITUM = VII,
    /* signa (spec canon-referentia): valores se nuntiant.
     * identitas  = '#' + corpus compositum (nomen proprium - et
     *              declaratio et referentia, congruentia verbatim)
     * referentia = '#' aut '.' + corpus compositum ('.' = genus e
     *              vocabulario; valor nudus VITIUM est) */
    CANON_GENUS_IDENTITAS  = VIII,
    CANON_GENUS_REFERENTIA = IX,
    /* titulus elementi: compositum + punctum UNUM ducens optionale
     * ('.species' = elementum generum generans - arbor porphyriana
     * 2026-08-10). Nomina elementorum/liberorum/intra sola; nomina
     * attributorum composita manent (speculum lexatoris stml). */
    CANON_GENUS_TITULUS    = X
} CanonGenusValoris;

/* ==================================================
 * Diagnostica
 * ================================================== */

nomen enumeratio {
    CANON_ELEMENTUM_IGNOTUM   = I,
    CANON_ATTRIBUTUM_IGNOTUM  = II,
    CANON_ATTRIBUTUM_DEEST    = III,
    CANON_VALOR_MALUS         = IV,
    CANON_LIBERUM_ILLICITUM   = V,
    CANON_LIBERI_PAUCI        = VI,
    CANON_LIBERI_MULTI        = VII,
    CANON_TEXTUS_ILLICITUS    = VIII,
    CANON_NOMEN_BIS           = IX,
    CANON_RADIX_MALA          = X,
    /* textus TYPATUS (2026-08-06): textus="numerus" etc. - textus
     * licet ET generi congruere debet. Vacuus praeteritur:
     * necessitas textus res lectoris est, non canonis. */
    CANON_TEXTUS_MALUS        = XI,
    /* CITATIO (2026-08-06): clavis-relatio INTRA documentum (mos
     * xs:key/keyref - quod unum XSD recte fecit). Valor attributi
     * valorem clavis alicubi declaratum aequare debet. Cum intra=
     * scopus per INSTANTIAM est (transitus statum machinae SUAE
     * citat). Trans plagulas = gradus III, oneratoris manet. */
    CANON_CITATIO_IRRITA      = XII,
    /* SIGNA (spec canon-referentia): '.genus' extra indicem ad=
     * citationis - vocabularium generum, sine clavibus iudicatum */
    CANON_VOCABULUM_IGNOTUM   = XIII,
    /* FINES (spec fines): valor formae SANAE extra minimum/maximum
     * declaratum (inclusiva, numerus solus). Forma mala manet
     * VALOR_MALUS - diagnosticum hoc finem violatum nominat. */
    CANON_VALOR_EXTRA_FINES   = XIV
} CanonVitiumGenus;

nomen structura {
    CanonVitiumGenus  genus;
    StmlNodus*        nodus;      /* ubi (potest NIHIL) */
    chorda*           elementum;  /* quod elementum */
    chorda*           detail;     /* attributum/liberum/valor */
    i32               numerus;    /* observatum (cardinalitas) */
    i32               limes;      /* postulatum (cardinalitas) */
} CanonVitium;

/* ==================================================
 * Canon oneratus
 * ================================================== */

nomen structura {
                Piscina*  piscina;
    InternamentumChorda*  intern;
                 chorda*  dialectus;
                 chorda*  versio;
         TabulaDispersa*  elementa;    /* nomen -> CanonElementum* */
                    Xar*  unicitates;  /* Xar de CanonUnicitas* */
                    Xar*  citationes;  /* Xar de CanonCitatio* */
                    Xar*  tituli;      /* Xar de chorda* - tituli
                                        * elementorum, pro
                                        * suggestione ('an X?') */
                 chorda*  radix;       /* nomen elementi radicis */
} Canon;

/* ==================================================
 * Lectio
 * ================================================== */

/* Canonem ex fonte STML legere.
 * Redde NIHIL si canon ipse male formatus est (nuntius in causa). */
Canon*
canon_legere(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              causa);

/* Canonem ex elemento <canon> IAM PARSATO struere - via infixi
 * (subtree in documento vivente). canon_legere = parsare + hoc. */
Canon*
canon_ex_nodo(
    StmlNodus*           elementum,
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              causa);

/* Canonem infixum invenire: liberum PRIMUM elementare radicis,
 * si titulo 'canon' - aliter NIHIL. Positio ipsa regula est:
 * <canon> serior in documento contentum est, non contractus. */
StmlNodus*
canon_infixum_invenire(
    StmlNodus* elementum_radix);

/* ==================================================
 * Iudicium
 * ================================================== */

/* Documentum contra canonem iudicare.
 * radix = nodus DOCUMENTI (resultus.radix) aut elementum radicis.
 * Redde: Xar de CanonVitium (vacuum = sanum) */
Xar*
canon_iudicare(
    Canon*     canon,
    StmlNodus* radix,
    Piscina*   piscina);

/* Nuntius legibilis pro genere vitii */
constans character*
canon_nuntius(
    CanonVitiumGenus genus);

/* ==================================================
 * Registrum - extensio ad canonem
 *
 * Forma plagulae (canones.registrum in radice), DUAE claves:
 *   .genera<TAB>natura/natura.canon      per EXTENSIONEM
 *   <aedilis><TAB>aedilis.canon          per ELEMENTUM RADICIS
 *
 * CUR DUAE: extensio contractum nominat sine plagula aperienda -
 * ratio qua '.genera' ab '.stml' distinctum est. SED '.stml'
 * QUATTUOR dialectos fert (librarium, grammatica, aedilis,
 * quaestiones), ergo illis extensio nihil dicit. Elementum
 * radicis dialectum SEMPER nominat, quia documentum se ipsum
 * profitetur.
 * Radix VINCIT cum utraque adest: specialius vincit.
 * (Inventum dialecto secundo describendo: '.stml -> aedilis'
 * tres alios dialectos falso iudicasset.)
 * ================================================== */

/* Canonem per EXTENSIONEM viae invenire.
 * Redde: via canonis (ex piscina) aut chorda vacua si nullus. */
chorda
canon_registrum_quaerere(
    chorda               catalogus,
    constans character*  via_plagulae,
    Piscina*             piscina);

/* Canonem per ELEMENTUM RADICIS invenire (clavis '<nomen>').
 * Redde: via canonis aut chorda vacua si nullus. */
chorda
canon_registrum_quaerere_radice(
    chorda            catalogus,
    constans chorda*  radix,
    Piscina*          piscina);

#endif /* CANON_H */
