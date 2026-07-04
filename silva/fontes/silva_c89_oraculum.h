/* silva_c89_oraculum.h - anima semantica grammaticae c89
 * (M2b Chunk C; consilium = simulatio X, project-specs/
 * silva-simulatio-10.md)
 *
 * Primus modulus grammaticae-proprius manu scriptus. Quattuor
 * partes, unus possessor sequentiae:
 *
 *   1. REGISTRATIO (X1/X2/X4/X5): uncus praecommissionis ambulat
 *      radicem NONDUM commissam, declarationes typedef detegit
 *      (lexema TYPEDEF in specificatoribus - nota semantica
 *      M2.0), nomina declaratorum in oraculum POSITIONALE
 *      inscribit (situs = byte_offset lexematis nominis).
 *      Elementa AMBIGUA praetermittuntur (politica exclusiva
 *      X4); rami sumpti conditionalium visitantur.
 *
 *   2. RESOLUTOR (X6 + X10): per interpretationes -
 *      (a) FILTRUM COMBINATIONIS (X10, necatio dura): lectio
 *          cuius lista specificatorum combinationem impossibilem
 *          fert (nominatus + quidquam typi, tag + primitivus,
 *          tag + tag) NUMQUAM vera esse potest - cadit.
 *      (b) superstes UNICUS -> victor (ceterae impossibiles).
 *      (c) aliter: lectio typo-positiva UNICA (nominatus adest,
 *          omnes noti AD SITUM - X3) -> victor.
 *      (d) aliter: nullum responsum - AMBIGUUS retinetur (pin
 *          quaestionum joculatoriarum).
 *
 *   3. POLITICA (X8): spina canonica = lectio expressionis pro
 *      superstitibus (recanonicare; involucrum manet). Ambigua
 *      eiusdem generis (ambae declarationes) ordinem motoris
 *      retinent - politica nihil dicit.
 *
 *   4. INVOLUCRUM: silva_c89_parsare = vocatio una (registratio
 *      per uncum + resolutio ad commissionem + politica post).
 *
 * Ambulationes genus+loci SOLUM legunt (pater praecommissioni
 * absens - S27).
 */

#ifndef SILVA_C89_ORACULUM_H
#define SILVA_C89_ORACULUM_H

#include "latina.h"
#include "piscina.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "silva_tabulae_c89.h"

/* Ligamen plenum grammaticae c89 CUM unco registrationis - hoc
 * adhibe, nisi machinationem propriam requiris */
externus constans SilvaGrammatica SILVA_C89_GRAMMATICA;

/* Lexema tituli declaratoris (catena monstrator/aciei/functionis/
 * parenthesis/initiatus persecuta usque ad titulum; NIHIL si
 * abstractus vel forma ignota). Idem ambulator quo vista
 * declarationum utetur (X5). */
SilvaToken*
silva_c89_declaratoris_titulus (constans SilvaNodus* declarator);

/* Registratio typorum: declarationes typedef in radice (etiam
 * intra ramos sumptos) -> oraculum, cum situ. Elementa ambigua
 * praetermissa (X4). */
vacuum
silva_c89_typos_registrare (
    SilvaValor     radix,
    SilvaOraculum* oraculum);

/* Uncus praecommissionis (datum = SilvaOraculum* mutabile) */
vacuum
silva_c89_praecommissio (SilvaValor radix, vacuum* datum);

/* Resolutor verus (X6+X10) - forma SilvaResolutor */
vacuum
silva_c89_resolutor (
    constans SilvaNodus*     ambiguum,
    constans SilvaOraculum*  oraculum,
    vacuum*                  datum,
    SilvaResolutioResponsum* responsum);

/* Politica spinae canonicae super superstites; numerum versorum
 * reddit */
i32
silva_c89_politicam_imponere (
    SilvaCommissio* commissio,
    SilvaOraculum*  oraculum);

/* ==================================================
 * Vista declarationum (M2b Chunk D) - ordines TOC pro
 * saltuario/solario: UNUS ordo per declaratorem (int a, b; =
 * duo), UNUS per declarationem nudam (tag = titulus). Elementa
 * ambigua per lectionem CANONICAM; rami sumpti visitantur.
 * Exemplar vistarum additionum II: structurae leves, per
 * indicem, sine allocatione.
 * ================================================== */

nomen structura {
    constans character* genus;    /* titulus generis ordinis
                                   * (kebab, e registro): declarator-
                                   * functionis, structura, ... */
    chorda              titulus;  /* nomen declaratum; mensura 0 =
                                   * anonymum/abstractum */
    s32                 linea;    /* lexematis nominis (aut primi) */
    s32                 situs;    /* byte_offset eiusdem */
} SilvaDeclaratioVista;

/* Numerus ordinum vistae per radicem parsurae */
i32
silva_c89_declarationes_numerus (constans SilvaParsura* parsura);

/* Ordo vistae ad indicem; FALSUM extra fines */
b32
silva_c89_declaratio_vista (
    constans SilvaParsura* parsura,
    i32                    index,
    SilvaDeclaratioVista*  vista);

/* Subscriptio functionis (M2c Chunk C): specificatores +
 * declarator octetim ex arbore (sine corpore, sine
 * declarationibus-kr) - textus ordinis TOC. Ordo vistae ad
 * indicem definitio-functionis sit; aliter fractura clara. */
SilvaScriptura
silva_c89_functionis_subscriptio (
    Piscina*               piscina,
    constans SilvaParsura* parsura,
    i32                    index);

/* Vocatio una: parsare + registrare + resolvere + politica.
 * oraculum NIHIL licet (vacuum intus creatur); oraculum datum
 * typos plagulae ACCIPIT (praeonera typos externos ante). */
SilvaParsura*
silva_c89_parsare (
    Piscina*            piscina,
    constans character* via,
    constans character* fons,
    i32                 mensura,
    SilvaOraculum*      oraculum);

#endif /* SILVA_C89_ORACULUM_H */
