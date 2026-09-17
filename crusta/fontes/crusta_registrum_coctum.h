/* crusta_registrum_coctum.h
 *
 * Registrum generum COCTUM grammaticae 'crusta' - GENERATUM, NE MANU
 * EDITES. Fons: crusta/grammatica/crusta.registrum.stml
 * (materia/coquere.sh). Genera L, loci CLXXVI.
 */

#ifndef CRUSTA_REGISTRUM_COCTUM_H
#define CRUSTA_REGISTRUM_COCTUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    /* Radix: lista sententiarum ordine octetorum + lexema FINIS
     * (trivia caudalia fert) */
    CRUSTA_GENUS_PROGRAMMA = 0,
    /* ';', signum et, aut linea nova quae sententiam terminat (C6:
     * linea nova alibi trivium est) */
    CRUSTA_GENUS_SEPARATOR,
    /* Totalitas (C12): lexemata quae grammatica ponere non potuit
     * (fi sine if, ')' vaga, ';;' extra case) - octeti manent,
     * sanitas negatur */
    CRUSTA_GENUS_MALUM,
    /* Corpus heredoc ubi octeti iacent (C5): partes ut in gemina,
     * aut pars litteralis una si delimitator citatus; linea
     * delimitatoris; linea nova post eam. Ambo lexemata absentia ad
     * EOF */
    CRUSTA_GENUS_HEREDOC,
    /* Solum cum operatore ET (signum et geminum) aut AUT (virgula
     * gemina) adest (C8): liberi = pipa|imperium, operator, ... */
    CRUSTA_GENUS_CATENA,
    /* Solum cum '|', pipa erroris, '!' aut 'time' adest (C8).
     * praefixa: time, -p, ! ordine octetorum */
    CRUSTA_GENUS_PIPA,
    /* Operator nudus intra listam: ET/AUT in catena, pipae in pipa;
     * '|' inter exemplaria optionis; '/' aut ':' in argumentis
     * expansionis; ';' in cyclo */
    CRUSTA_GENUS_OPERATOR,
    /* Imperium simplex: assignatio | verbum | redirectio ordine
     * octetorum; titulus imperii = verbum primum (derivatum) */
    CRUSTA_GENUS_IMPERIUM,
    /* titulus [subscriptum crudum] = aut += valor; valor absens pro
     * 'a=' */
    CRUSTA_GENUS_ASSIGNATIO,
    /* Valor tabulae '( verbum ... )'; elementum '[i]=v' = assignatio
     * filia */
    CRUSTA_GENUS_TABULATUM,
    /* fd (digiti aut {titulus}, optionale) operator scopus(verbum);
     * corpus = referentia ad heredoc pro operatoribus heredoc solis */
    CRUSTA_GENUS_REDIRECTIO,
    /* ['function'] titulus ['(' ')'] [interiecta] corpus (imperium
     * compositum, redirectiones suas fert); interiecta = corpora
     * heredoc post titulum aut ')' ante corpus (linea nova ibi
     * valida) */
    CRUSTA_GENUS_FUNCTIO,
    /* '{' lista '}' [redirectiones] (C9) */
    CRUSTA_GENUS_GREX,
    /* '(' lista ')' subcrusta [redirectiones] */
    CRUSTA_GENUS_CRUSTULA,
    /* if probatio then liberi rami fi [redirectiones]; rami =
     * elif/else */
    CRUSTA_GENUS_CONDITIO,
    /* elif probatio then liberi (IV loci) aut else liberi (probatio
     * et tok_deinde absentes) */
    CRUSTA_GENUS_RAMUS,
    /* for/select titulus [interiecta] [in verba] separator cursus
     * [redirectiones]; separator = lista: nodus separator (';' aut
     * linea nova quae verba terminat) et corpora heredoc quae ei
     * sequuntur; interiecta = corpora heredoc post titulum ante in
     * (linea nova ibi trivium est) - P6: corpus FreeBSD 'read x
     * <<EOF; for i' + linea nova */
    CRUSTA_GENUS_ITERATIO,
    /* for (( a; b; c )) separator cursus; liberi = expressio,
     * operator(;), expressio, operator(;), expressio - expressiones
     * vacuae absentes; separator = lista ut in iteratione (nodus
     * separator et corpora heredoc) */
    CRUSTA_GENUS_CYCLUS,
    /* while/until probatio cursus [redirectiones] */
    CRUSTA_GENUS_REPETITIO,
    /* do lista done */
    CRUSTA_GENUS_CURSUS,
    /* case verbum [interiecta] in optiones esac [redirectiones];
     * interiecta = corpora heredoc post verbum ante in (linea nova
     * ibi trivium est) - P6: corpus FreeBSD 'read x <<EOF; case $x'
     * + linea nova */
    CRUSTA_GENUS_ELECTIO,
    /* ['('] exemplar | exemplar ')' lista [terminator: ;; aut formae
     * bash]; exemplaria = verbum, operator(|), verbum; terminator
     * absens ante esac */
    CRUSTA_GENUS_OPTIO,
    /* (( expressio )) ut imperium [redirectiones]; post_* = corpora
     * heredoc in lacunis */
    CRUSTA_GENUS_ARITHMETICA,
    /* [[ expressio ]] ut imperium [redirectiones]; expressio =
     * iudicium-* aut verbum nudum; post_* = corpora heredoc in
     * lacunis */
    CRUSTA_GENUS_IUDICIUM,
    /* coproc [TITULUS] imperium (titulus solum cum imperio
     * composito) */
    CRUSTA_GENUS_SOCIUS,
    /* Verbum = cursus partium adiacentium (C10); expansio non
     * exsistit - valor staticus derivatur (crusta_verbum_staticum) */
    CRUSTA_GENUS_VERBUM,
    /* Octeti nudi usque ad octetum specialem proximum */
    CRUSTA_GENUS_PARS_LITTERALIS,
    /* '\x' duo octeti; intra gemina solum quinque effugia */
    CRUSTA_GENUS_PARS_EFFUGIUM,
    /* '\' + linea nova intra verbum (inter verba: trivium LAMINA) */
    CRUSTA_GENUS_PARS_CONTINUATIO,
    /* '...' totum, apices inclusi */
    CRUSTA_GENUS_PARS_SIMPLEX,
    /* Apices gemini cum partibus intra */
    CRUSTA_GENUS_PARS_GEMINA,
    /* $'...' totum (effugia ANSI-C), numquam decodatum in arbore */
    CRUSTA_GENUS_PARS_EFFUGIA,
    /* Sigillum et apices gemini: chorda vertenda, cum partibus ut
     * gemina; numquam versa in arbore */
    CRUSTA_GENUS_PARS_VERSA,
    /* $x $1 $? $@ ... : sigillum (intra backtick '\$') et titulus */
    CRUSTA_GENUS_PARS_PARAMETRUM,
    /* ${ [#|!] titulus [subscriptum] [operator] argumenta };
     * argumenta = verbum, operator(/ aut :), verbum */
    CRUSTA_GENUS_PARS_EXPANSIO,
    /* $( lista ) aut `lista`; liberi = lista programmatis */
    CRUSTA_GENUS_PARS_SUBSTITUTIO,
    /* $(( expressio )) */
    CRUSTA_GENUS_PARS_ARITHMETICA,
    /* Substitutio processus: parenthesis post signum minoris aut
     * maioris, lista intra */
    CRUSTA_GENUS_PARS_PROCESSUS,
    /* ~ aut ~user in initio verbi (praefixum tildae) */
    CRUSTA_GENUS_PARS_DOMUS,
    /* Arithmetica (C11): decimalis, 0x.., 0.., basis#digiti */
    CRUSTA_GENUS_NUMERUS,
    /* titulus [subscriptum crudum] */
    CRUSTA_GENUS_VARIABILIS,
    /* -x !x ~x ++x --x +x */
    CRUSTA_GENUS_PRAEPOSITA,
    /* x++ x-- */
    CRUSTA_GENUS_POSTPOSITA,
    /* sinister operator dexter; etiam assignationes et comma;
     * operator absens = iuxtapositio */
    CRUSTA_GENUS_BINARIA,
    /* probatio ? sinister : dexter */
    CRUSTA_GENUS_TERNARIA,
    /* ( expressio ) */
    CRUSTA_GENUS_INCLUSA,
    /* [[ ]]: -f x ; ! c (operanda verba) */
    CRUSTA_GENUS_IUDICIUM_PRAEPOSITA,
    /* a == b ; a =~ re (dexter: verbum partis unius REGULA);
     * operator absens = iuxtapositio */
    CRUSTA_GENUS_IUDICIUM_BINARIA,
    /* coniunctio ET aut AUT (ET artius ligat) */
    CRUSTA_GENUS_IUDICIUM_CONIUNCTA,
    /* ( c ) */
    CRUSTA_GENUS_IUDICIUM_INCLUSA,

    CRUSTA_GENUS_NUMERUS_GENERUM
} CrustaGenus;

externus constans MateriaRegistrumCoctum CRUSTA_REGISTRUM;

#endif /* CRUSTA_REGISTRUM_COCTUM_H */
