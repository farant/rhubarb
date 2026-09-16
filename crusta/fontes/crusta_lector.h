/* crusta_lector.h - Lector crustae: functio (modus, positio) -> lexema
 *
 * DECRETUM 01M2NJ1JR7 (spec crusta-arbor-spec.md par. III): lexator
 * functio pura est modi et positionis - status eius = cursor (situs),
 * acervus regionum, queue heredoc, et tria vexilla continuationis
 * (titulus post sigillum, gradus assignationis) quae pulsum unum
 * solum supervivunt. MODUS a parsatore eligitur pulsu quoque; lexema
 * unum redditur ut MateriaToken (segmentum chordae in fontem, numquam
 * copia), cum cauda privata CrustaCauda (profunditas backtick).
 *
 * REGIONES: corpora heredoc et backtick DELIMITANTUR primum, parsantur
 * secundo (ordo bash): regio limitem 'finis' fert, FINIS intra eam
 * regionem terminat, non fontem. Regio backtick profunditatem 'gravis'
 * fert: intra eam cursus n retroversuum ante '$', '`' aut '\' post d
 * exuitiones r = n >> d retroversa logica dat (bash retroversum semel
 * per gradum exuit); genus lexematis ex octeto LOGICO decernitur,
 * octeti CRUDI in lexemate manent, decodator caudam legit.
 *
 * LINEA NOVA BIS (C6): modus decernit - SEPARATOR_LINEAE in modis qui
 * sententiam aut listam verborum terminant (VERBA ASSIGNATIONES
 * IN_VERBIS), LINEA (trivium) alibi.
 *
 * MODI XIX (spec XV + TABULATUM pro valore tabulae '( )', et
 * EXPANSIO_EXEMPLAR / EXPANSIO_SECTIO pro verbis argumentorum
 * expansionis quae '/' aut ':' terminat - lector modum non scit nisi
 * parsator dicit; et POST_COMPOSITUM post lexema clausurae compositi,
 * ubi bash verba reservata iterum agnoscit sed assignationes non).
 */

#ifndef CRUSTA_LECTOR_H
#define CRUSTA_LECTOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "materia_token.h"
#include "crusta_registrum.h"
#include "crusta_lexicon.h"

nomen enumeratio {
    CRUSTA_MODUS_INITIUM = 0,        /* positio imperii */
    /* post verbum imperii */
    CRUSTA_MODUS_VERBA,
    /* post aedificatorem assignationum */
    CRUSTA_MODUS_ASSIGNATIONES,
    /* intra valorem tabulae ( ... ) */
    CRUSTA_MODUS_TABULATUM,
    /* post for/select/case: verbum unum */
    CRUSTA_MODUS_POST_TITULUM,
    /* lista verborum 'for x in ...' */
    CRUSTA_MODUS_IN_VERBIS,
    /* exemplaria case */
    CRUSTA_MODUS_EXEMPLAR,
    /* intra "..." */
    CRUSTA_MODUS_GEMINA,
    /* intra ${ post titulum */
    CRUSTA_MODUS_EXPANSIO,
    /* argumentum ${x:-...} */
    CRUSTA_MODUS_EXPANSIO_VERBUM,
    /* argumentum ${x/a/b}: '/' terminat */
    CRUSTA_MODUS_EXPANSIO_EXEMPLAR,
    /* argumentum ${x:1:2}: ':' terminat */
    CRUSTA_MODUS_EXPANSIO_SECTIO,
    /* (( )) profunditate ZEPHYRUM */
    CRUSTA_MODUS_ARITHMETICA_SUMMA,
    /* intra ( ) arithmeticae */
    CRUSTA_MODUS_ARITHMETICA_INTRA,
    /* intra [[ ]] */
    CRUSTA_MODUS_IUDICIUM,
    /* operandum dextrum =~ */
    CRUSTA_MODUS_REGULA,
    /* corpus heredoc, delimitator nudus */
    CRUSTA_MODUS_HEREDOC,
    /* corpus heredoc, delimitator citatus */
    CRUSTA_MODUS_HEREDOC_LITTERALE,
    /* post lexema clausurae compositi ('}' ')' '))' ']]' 'fi' 'done'
     * 'esac'): verba reservata agnoscuntur (bash: 'if ((x)) then',
     * '{ { a; } }'), assignationes non, linea nova terminat */
    CRUSTA_MODUS_POST_COMPOSITUM,
    CRUSTA_MODUS_NUMERUS_MODORUM
} CrustaModus;

/* Positio cursoris, reponenda (reversio $(( ). */
nomen structura {
    s32 cursor;
    /* I-basata */
    i32 linea;
    /* offset initii lineae currentis */
    s32 linea_initium;
    b32 titulus_exspectatur;   /* post PARAMETRUM_SIGILLUM */
    /* I post titulum, II post subscriptum */
    i32 assignationis_gradus;
    /* offset '$((' ab aedificatore RECUSATI (reversio: ut '$(' '('
     * relegendum); -I nullus */
    s32 arithmetica_recusata;
} CrustaSitus;

nomen structura {
    s32 initium;
    s32 finis;
    /* profunditas backtick; ZEPHYRUM = nulla */
    i32 gravis;
} CrustaRegio;

nomen structura {
    chorda delimitator;   /* sine apicibus (valor staticus verbi) */
       b32 citatus;       /* delimitator citatus: corpus litterale */
       b32 tabulae;       /* '<<-': tabulae ducentes exuendae */
} CrustaHeredocPetitio;

/* Cauda privata lexematis (materia_token_cauda) */
nomen structura {
    i32 gravis;
} CrustaCauda;

nomen structura {
                    Piscina* piscina;
         constans character* fons;
                        i32  mensura;
                CrustaSitus  situs;
                        /* CrustaRegio per valorem */
                        Xar* regiones;
                        /* CrustaHeredocPetitio */
                        Xar* heredoca;
                        /* index petitionis proximae */
                        i32 heredoca_caput;
                        /* linea delimitatoris regionis heredoc apertae;
                         * -I absens */
                         s32  delimitator_ab;
                         s32  delimitator_ad;
           MateriaTokenForma  forma;
    constans CrustaDialectus* dialectus;
} CrustaLector;

/* FALSUM = memoria deficit. */
b32
crusta_lector_incipere (
                CrustaLector* lector,
                     Piscina* piscina,
          constans character* fons,
                         i32  mensura,
    constans CrustaDialectus* dialectus);

/* Lexema proximum in modo dato. FINIS (vacuum) in fine fontis AUT
 * regionis intimae; NIHIL = memoria deficit. */
MateriaToken*
crusta_lector_proximum (
     CrustaLector* lector,
      CrustaModus  modus);

CrustaSitus
crusta_lector_situs (
    constans CrustaLector* lector);

vacuum
crusta_lector_situm_reponere (
     CrustaLector* lector,
      CrustaSitus  situs);

/* Regio [cursor, finis) profunditate 'gravis'. */
b32
crusta_lector_regionem_aperire (
    CrustaLector* lector,
             s32  finis,
             i32  gravis);

/* Regionem intimam claudere; cursor ad finem eius ponitur. */
vacuum
crusta_lector_regionem_claudere (
    CrustaLector* lector);

/* Offset initii cursus clausurae backtick profunditatis 'gravis'
 * (>= I) ab 'ab' intra regionem intimam; -I si nulla (regio tunc ad
 * finem currit). */
s32
crusta_lector_gravem_quaerere (
    constans CrustaLector* lector,
                      s32  ab,
                      i32  gravis);

/* Cauda lexematis crustae; NIHIL si lexema alienum. */
constans CrustaCauda*
crusta_lector_cauda (
    constans MateriaToken* token);

/* Petitionem heredoc in queue ponere (aedificator post verbum
 * delimitatoris). FALSUM = memoria. */
b32
crusta_lector_heredoc_petere (
    CrustaLector* lector,
          chorda  delimitator,
             b32  citatus,
             b32  tabulae);

b32
crusta_lector_heredoca_pendent (
    constans CrustaLector* lector);

/* Petitionem proximam sumere, lineam delimitatoris quaerere, regionem
 * corporis [cursor, initium lineae delimitatoris) aperire. FALSUM =
 * queue vacua. */
b32
crusta_lector_heredoc_aperire (
            CrustaLector* lector,
    CrustaHeredocPetitio* exitus);

/* Post FINIS regionis: regionem claudit; lexema lineae delimitatoris
 * (tabulae servatae), NIHIL si corpus ad EOF cucurrit. */
MateriaToken*
crusta_lector_heredoc_delimitator (
    CrustaLector* lector);

/* Linea nova post lineam delimitatoris ut SEPARATOR_LINEAE; NIHIL ad
 * EOF. */
MateriaToken*
crusta_lector_heredoc_finis (
    CrustaLector* lector);

#endif /* CRUSTA_LECTOR_H */
