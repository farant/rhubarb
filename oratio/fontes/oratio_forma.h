/* oratio_forma.h - Forma textus paragraphi (T6b, spec decisiones XXIII-XXV)
 *
 * Stratum FORMAE ante lectorem sententiarum, ab eo DISCRETUM: cursus
 * per elementa paragraphi (cum caudis, post cursum elementorum, ante
 * cursum sententiarum) INDICIA numerat - lineas, longitudines, hiatus,
 * initia et fines linearum - et REGULA prima tabulae cuius condiciones
 * omnes tenent formam nominat: prosa | versus | titulus | tabula |
 * index. Forma ut locus INDEX in paragrapho deponitur (lex deprimendi);
 * lector eam consulit: versus/tabula/index = LINEA unitas; titulus =
 * unitas UNA; prosa = regula sententiarum v1.
 *
 * REGULAE SUNT DATA (ORATIO_REGULAE_FORMAE): ordo = praecedentia,
 * condicio = (indicium, genus, valor); casus speciales ut ORDINES
 * accumulantur cum causa nominata, numquam ut rami codicis.
 * Nulla regula tenens = PROSA: ambiguitas ad iunctionem vergit
 * (decisio XXIV: scissio falsa cogitationem frangit, iunctio falsa
 * molestia levis).
 *
 * INDICIUM PRIMARIUM: fractura VOLUNTARIA - linea finita quamquam
 * verbum primum lineae sequentis in eam cepisset (longitudo + I +
 * verbum <= latitudo involucri). Involucrum avidum eam numquam parit;
 * versus, tabulae, indices eam fere semper. Latitudo = linea
 * longissima, lineis cum elemento >= XXIV octetorum (url, via) EXCLUSIS
 * ne lexema infrangibile latitudinem fingat.
 *
 * Longitudo lineae = a columna 0 (indentatione inclusa) ad octetum
 * ultimum PARTIUM elementi ultimi (spatia finalia exclusa); LINEA vel
 * LINEA_CR in cauda elementi lineam finit.
 */

#ifndef ORATIO_FORMA_H
#define ORATIO_FORMA_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_nodus.h"
#include "oratio_registrum.h"

/* Indicia paragraphi ex arbore numerata. Per centum: linearum, nisi
 * voluntariae_pct = per centum FRACTURARUM (linearum cum successore). */
nomen structura {
    i32 lineae;
    i32 elementa;
    i32 latitudo;              /* involucri (vide supra) */
    i32 longitudo_maxima;
    i32 longitudo_minima;
    i32 longitudo_media;
    i32 indentatio_maxima;
    i32 indentatio_minima;
    i32 fracturae;             /* lineae - I */
    i32 voluntariae;           /* fracturae ubi verbum sequens cepisset */
    i32 terminales;            /* lineae in . ! ? ... finitae (claudentibus exclusis) */
    i32 interpunctae;          /* lineae in interpunctione quavis finitae */
    i32 capitales;             /* lineae littera capitali ASCII incipientes */
    i32 capitales_omnes;       /* lineae quarum litterae ASCII omnes capitales */
    i32 notatae;               /* lineae nota incipientes (numerus, signum listae,
                                  littera + punctum) aut numero paginae finitae */
    i32 columnatae;            /* lineae cum hiatu interno (>= II spatia post
                                  vocabulum/numerum, tabula, aut '|' bis) */
    i32 terminales_internae;   /* candidati finis NON in fine lineae */

    i32 voluntariae_pct;
    i32 terminales_pct;
    i32 interpunctae_pct;
    i32 capitales_pct;
    i32 capitales_omnes_pct;
    i32 notatae_pct;
    i32 columnatae_pct;
} OratioIndicia;

nomen enumeratio {
    ORATIO_INDICIUM_NIHIL = 0,          /* terminator condicionum */
    ORATIO_INDICIUM_LINEAE,
    ORATIO_INDICIUM_ELEMENTA,
    ORATIO_INDICIUM_LATITUDO,
    ORATIO_INDICIUM_LONGITUDO_MEDIA,
    ORATIO_INDICIUM_INDENTATIO_DIVERSA, /* maxima - minima */
    ORATIO_INDICIUM_TERMINALES_INTERNAE,
    ORATIO_INDICIUM_VOLUNTARIAE_PCT,
    ORATIO_INDICIUM_TERMINALES_PCT,
    ORATIO_INDICIUM_INTERPUNCTAE_PCT,
    ORATIO_INDICIUM_CAPITALES_PCT,
    ORATIO_INDICIUM_CAPITALES_OMNES_PCT,
    ORATIO_INDICIUM_NOTATAE_PCT,
    ORATIO_INDICIUM_COLUMNATAE_PCT,

    ORATIO_INDICIUM_NUMERUS_INDICIORUM
} OratioIndicium;

nomen enumeratio {
    ORATIO_CONDICIO_MINIMUM = 0,        /* indicium >= valor */
    ORATIO_CONDICIO_MAXIMUM,            /* indicium <= valor */
    ORATIO_CONDICIO_AEQUALIS            /* indicium == valor */
} OratioCondicioGenus;

nomen structura {
         OratioIndicium indicium;
    OratioCondicioGenus genus;
                    i32 valor;
} OratioCondicio;

#define ORATIO_CONDICIONES_MAXIMAE 6

nomen structura {
           OratioForma  forma;
    constans character* titulus;                                  /* nomen regulae */
        OratioCondicio  condiciones[ORATIO_CONDICIONES_MAXIMAE];  /* NIHIL terminat */
} OratioRegulaFormae;

externus constans OratioRegulaFormae ORATIO_REGULAE_FORMAE[];
externus constans i32                ORATIO_REGULAE_FORMAE_NUMERUS;

/* Indicia ex elementis paragraphi (Xar de MateriaNodus*: vocabulum |
 * interpunctio | numerus, caudis iam appensis) et offset octeti ubi
 * linea prima incipit (indentatione inclusa). */
vacuum
oratio_forma_indicia (
              Xar* elementa,
              s32  initium_lineae,
    OratioIndicia* indicia);

/* Regula prima tenens; *regula (si datum) eam nominat, NIHIL = nulla
 * (prosa). */
OratioForma
oratio_forma_iudicare (
         constans OratioIndicia*  indicia,
    constans OratioRegulaFormae** regula);

/* Valor indicii nominati (ZEPHYRUM pro NIHIL). */
i32
oratio_forma_indicium (
    constans OratioIndicia* indicia,
            OratioIndicium  quod);

constans character*
oratio_forma_titulus (
    OratioForma forma);

constans character*
oratio_forma_indicii_titulus (
    OratioIndicium quod);

/* Forma paragraphi ex arbore (locus INDEX); prosa si absens. */
OratioForma
oratio_forma_paragraphi (
    constans MateriaNodus* paragraphus);

/* An cauda elementi lineam novam ferat (linea finit). */
b32
oratio_forma_linea_finit (
    constans MateriaNodus* elementum);

/* Elementa paragraphi iam constructi (sententiae concatenatae) et
 * initium lineae primae - ut indicia post parsuram recomputentur
 * (instrumentum formae.sh, porta). NIHIL = memoria. */
Xar*
oratio_forma_elementa (
                  Piscina* piscina,
    constans MateriaNodus* paragraphus);

s32
oratio_forma_initium_lineae (
    constans MateriaNodus* paragraphus);

#endif /* ORATIO_FORMA_H */
