/* css_adaptare.h - CssLexema -> MateriaToken, et ligatio triviorum
 *
 * DUAE RES, CONSULTO SEPARATAE. Adaptatio lexemata VERTIT; ligatio
 * ea in trivia et significantia DISTRIBUIT. Consilium (T8) unam
 * functionem bimodalem petebat - css_adaptare(..., b32
 * spatia_ut_contentum) - quae trivia aut absorberet aut non. Illa
 * forma REGULAM D7 IMPLERE NON POTEST, et ratio structuralis est:
 *
 *   spec par. VI (D7) spatia intra praeludia et valores CONTENTUM
 *   facit, ALIBI trivia. Quae regio praeludium sit CONTEXTU
 *   PARSURAE constat, quem adaptator nondum habet. Modo globali
 *   'non-verbatim', spatium in 'a b { }' iam trivium factum esset
 *   antequam parsator praeludium agnosceret - ergo in nodum
 *   'lexema-servatum' promoveri non posset.
 *
 *   Modus alter (bis adaptare, semel structurae, iterum praeludiis)
 *   DUO lexemata materiae pro octetis IISDEM pareret, et
 *   invariatum 'domini unius' conventio fieret, non structura.
 *
 * Consilium ipsum viam alteram nominat ("or equivalently do not
 * absorb trivia while inside a value", T11 gradus III). Haec est.
 *
 * ERGO INVARIATUM: lexema materiae UNUM pro lexemate CSS UNO,
 * semper. 'Dominus unus' non asseritur sed AEDIFICATUR - nullus
 * octetus bis converti potest quia bis conversus non est.
 *
 * ============================================================
 * REGULA DIVISIONIS, portata ex silva_lexema.c:950-1000
 * ============================================================
 *
 * Silva trivia pendentia ad lexema NOVA_LINEA scindit: quae usque
 * ad primam lineam novam INCLUSIVE stant lexemati PRIORI ut
 * 'post' dantur; cetera SEQUENTI ut 'ante'. Sine linea nova OMNIA
 * priori eunt (commentarium in fine lineae apud lineam suam
 * manet). Sine priore (initium plagulae) omnia sequenti.
 *
 * CSS lexema NOVA_LINEA NON HABET - unum genus SPATIA lineas
 * novas INTRA se absorbet - ergo scissio ad granum LEXEMATIS fieri
 * debet, non ad granum lineae. Divisor est primum trivium quod
 * munus SPATIUM fert ET terminatorem lineae continet; illud et
 * quae sequuntur 'ante' fiunt.
 *
 * DUAE CONSEQUENTIAE, utraque consulta:
 *
 *  I. Silva lineam novam ipsam PRIORI dat (divisio = j + I); nos
 *     totum lexema SEQUENTI (divisio = j). Scindere non possumus,
 *     ergo eligendum est - et indentatio lexematis sequentis pars
 *     maior et legibilior est quam terminator qui nihil legit.
 *
 * II. Commentarium MULTILINEUM divisor NON est, quia silvae
 *     COMMENTUM_CLAUSUM NOVA_LINEA non est. Ergo
 *     'a{} /' '* nota\n plura *' '/ b{}' commentarium apud 'a{}'
 *     relinquit - quod silva quoque facit, et quod oculus legit.
 */

#ifndef CSS_ADAPTARE_H
#define CSS_ADAPTARE_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_token.h"
#include "materia_lexicon.h"


/* ==================================================
 * Adaptatio - UNUM PRO UNO
 * ================================================== */

/* Xar de CssLexema (per valorem) -> Xar de MateriaToken*.
 *
 * Nihil absorbetur, nihil omittitur, nihil fingitur: index i
 * exitus lexema i ingressus est. Valor in FONTEM spectat, sicut
 * CssLexema (fons vivat quamdiu lexemata vivunt).
 *
 * 'forma' caudam frontis nominat; NIHIL = nulla cauda.
 * NIHIL redditur solum si memoria deficit. */
Xar*
css_adaptare (
                       Piscina* piscina,
                           Xar* lexemata_css,
    constans MateriaTokenForma* forma);


/* ==================================================
 * Ligator triviorum
 *
 * Status inter lexemata significantia portandus. Parsator eum
 * tenet quia SOLUS parsator scit quando spatium contentum sit.
 * ================================================== */

nomen structura {
             Piscina* piscina;
    constans MateriaLexiconRatum* lexicon;
                 Xar* pendentia;   /* Xar de MateriaToken* */
        MateriaToken* prior;       /* ultimum significans ligatum */
} CssLigator;

vacuum
css_ligator_incipere (
                      CssLigator* ligator,
                         Piscina* piscina,
    constans MateriaLexiconRatum* lexicon);

/* Trivium differre. Nihil ligatur donec significans adveniat. */
b32
css_ligator_cumulare (
      CssLigator* ligator,
    MateriaToken* trivium);

/* Pendentia inter 'prior' et 'sequens' dividere, regula supra.
 * 'sequens' NIHIL esse non potest - css_lexare lexema FINIS
 * semper ultimum emittit, ergo omnis series triviorum in
 * significante desinit. Ea est ratio cur 'plagula' locum
 * 'cauda:token' ferat (spec par. VII). */
b32
css_ligator_solvere (
      CssLigator* ligator,
    MateriaToken* sequens);

#endif /* CSS_ADAPTARE_H */
