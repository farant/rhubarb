/* materia_nodus.h - Nodus uniformis + valor signatus
 *
 * Nodus = caput fixum + series locorum signatorum per genus. NULLA
 * unio per genus (arbor2 cuique nodo pretium bracchi maximi
 * imputabat). Genus index in registrum est, non typus C - eadem
 * ratio qua materia_lexicon genera lexematum tractat.
 *
 * ============================================================
 * SEDES: SUTURA QUAM SPEC NON PRAEVIDIT
 * ============================================================
 *
 * materia-spec.md par. II tabulam fert dicentem
 *   silva_nodus -> silva_token   CLEAN
 * quod de INCLUSIONIBUS verum est (nodus solum token includit) et
 * de SEMANTICA falsum. Mensuratum 2026-08-27: quinque familiae
 * quaestionum in silva_nodus.c catenam ORIGINIS ambulant -
 *
 *   silva_valor_extensionem       extensio byte
 *   _extensionem_lineis_valoris   extensio linea/columna
 *   silva_valor_est_fons_purus    puritas fontis
 *   _sedes_colligere              geometria fida
 *   _lexema_primum_valoris        commentarium ducens
 *
 * - quia extensio lexematis EXPANSI non est sedes lexematis sed
 * sedes INVOCATIONIS eius. Phasis 0.1 unam intrusionem nuclei
 * nominavit (ArborCursor.expansio); haec SECUNDA est, in modulo
 * quem spec 'mundum' vocavit.
 *
 * ERROR IDEM QUEM CENSUS CONIUNCTIONIS FECIT: mensura quaestioni
 * ANGUSTIORI respondit quam conclusio ex ea tracta. Inclusiones
 * mensuratae, semantica conclusa.
 *
 * REMEDIUM: uncus UNUS. Omnes quinque idem rogant - "quae est
 * sedes EFFICAX huius lexematis". Frons sine derivatione uncum
 * NIHIL relinquit et sedes lexematis ipsius respondetur; id pro
 * CSS et HTML rectum est, ubi omne lexema origo sua est.
 */

#ifndef MATERIA_NODUS_H
#define MATERIA_NODUS_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_token.h"


/* ==================================================
 * Species loci - forma unius loci in nodo
 * ================================================== */

nomen enumeratio {
    MATERIA_LOCUS_NODUS = 0,
    MATERIA_LOCUS_TOKEN,
    MATERIA_LOCUS_LISTA_NODUS,
    MATERIA_LOCUS_LISTA_TOKEN,
    MATERIA_LOCUS_LISTA_MIXTA,
    MATERIA_LOCUS_INDEX,

    MATERIA_LOCUS_NUMERUS_SPECIERUM
} MateriaLocusSpecies;


/* ==================================================
 * Valor signatus
 * ================================================== */

nomen structura MateriaNodus MateriaNodus;

nomen enumeratio {
    MATERIA_VALOR_NIHIL = 0,
    MATERIA_VALOR_NODUS,
    MATERIA_VALOR_TOKEN,
    MATERIA_VALOR_LISTA,
    MATERIA_VALOR_INDEX
} MateriaValorGenus;

/* Prospectus listae: valor listae {repositorium, mensura} est, non
 * Xar nudus. Repositorium append-only; prospectus mensuram SUAM
 * fert, ergo furca quae ultra prospectum meum scripsit me non
 * laedit. NUMQUAM xar_numerus in repositorio lege - mensuram
 * prospectus semper adhibe. */
nomen structura {
    Xar* xar;
    i32  mensura;
} MateriaListaProspectus;

nomen structura {
    MateriaValorGenus genus;
    unio {
        MateriaNodus*          nodus;
        MateriaToken*          token;
        MateriaListaProspectus lista;
        s32                    index;
    } datum;
} MateriaValor;


/* ==================================================
 * Nodus
 * ================================================== */

structura MateriaNodus {
    s32           genus;            /* index in registrum generum */
    i32           numerus_locorum;
    MateriaValor* loci;
    MateriaNodus* pater;            /* post-acceptum SOLUM */
};


/* ==================================================
 * Sedes - ubi lexema in fonte REVERA est
 * ================================================== */

nomen structura {
    s32 byte_offset;   /* -I = inscibilis (synthetica sine ancora) */
    i32 linea;
    i32 columna;
    s32 fons_index;
    b32 est_fons;      /* VERUM = stratum 0, nulla derivatio */
} MateriaSedes;

/* Uncus originis. NIHIL (aut campus NIHIL) = lingua derivationem
 * non habet; sedes lexematis ipsius respondetur, est_fons VERUM.
 * Frons C89 hunc catenam originis ad radicem ambulare implet. */
nomen structura {
    vacuum* datum;

    /* Sedes efficax - quaestiones nodi (extensio, puritas,
     * geometria, commentarium). NIHIL = lexema origo sua est. */
    vacuum (*sedes_quaerere)(vacuum*, constans MateriaToken*,
                             MateriaSedes*);

    /* Radix EMISSIONIS: quod lexema loco huius octetos dat. Idem
     * lexema reddere = 'se ipsum emittit' (casus omnium linguarum
     * sine derivatione). NIHIL + causa = recuperari non potest.
     * NIHIL uncus = radix semper lexema ipsum. */
    MateriaToken* (*radix_quaerere)(vacuum*, MateriaToken*,
                                    constans character** causa);

    /* Lamina octetorum quam radix TOTA occupat, cum radix plus
     * quam se ipsam tegit (C89: invocatio functio-similis cum
     * argumentis). NIHIL = radix se solam tegit. */
    Xar* (*extentum_quaerere)(vacuum*, constans MateriaToken*);
} MateriaOrigoUncus;

/* Sedes efficax lexematis. Uncum adhibet si adest; aliter campos
 * lexematis reddit. NUMQUAM fallit - sedes semper impletur. */
vacuum
materia_sedes_tokeni (
    constans MateriaOrigoUncus* uncus,
         constans MateriaToken* token,
                  MateriaSedes* sedes);


/* ==================================================
 * Constructores valorum
 * ================================================== */

MateriaValor materia_valor_nihil (vacuum);
MateriaValor materia_valor_nodus (MateriaNodus* nodus);
MateriaValor materia_valor_token (MateriaToken* token);
MateriaValor materia_valor_index (s32 index);
MateriaValor materia_valor_lista (Xar* lista);
MateriaValor materia_valor_lista_nova (Piscina* piscina);

/* Appendere PURUM: prospectus alieni numquam laeduntur. Si
 * prospectus ad finem vivum repositorii stat, in loco appendit
 * (O(1), casus communis); aliter praefixum in repositorium recens
 * copiat. Prospectum NOVUM reddit. */
MateriaValor
materia_valor_lista_appendere (
      Piscina* piscina,
    MateriaValor lista,
    MateriaValor elementum);

i32           materia_valor_lista_numerus  (MateriaValor lista);
MateriaValor* materia_valor_lista_obtinere (MateriaValor lista, i32 index);


/* ==================================================
 * Nodus
 * ================================================== */

MateriaNodus*
materia_nodus_creare (
    Piscina* piscina,
         s32 genus,
         i32 numerus_locorum);

/* Signum valoris contra speciem loci probatur, semel-tantum
 * scribere imponitur. FALSUM in violatione (et nihil scribitur) -
 * errores in prima constructione apparent, non ut arbores
 * corruptae tribus operibus post. */
b32
materia_nodus_ponere (
        MateriaNodus* nodus,
                  i32 locus,
         MateriaValor valor,
  MateriaLocusSpecies species);

b32
materia_nodus_appendere (
             Piscina* piscina,
        MateriaNodus* nodus,
                  i32 locus,
         MateriaValor valor,
  MateriaLocusSpecies species);

b32
materia_valor_congruit (
         MateriaValor valor,
  MateriaLocusSpecies species);

/* Liberi nodales: loci NODUS + elementa NODUS listarum, ordine
 * locorum. Tabulis non eget - valores signati sunt. */
Xar*
materia_nodus_liberi (
                Piscina* piscina,
    constans MateriaNodus* nodus);

#endif /* MATERIA_NODUS_H */
