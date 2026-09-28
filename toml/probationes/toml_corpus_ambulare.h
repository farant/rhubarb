/* toml_corpus_ambulare.h - Ambulatio corporum toml pro portis
 *
 * Adiumentum probationum (cursor plagulas probationum non-probatio_ ut
 * adiumenta compilat): tres fontes, ordine -
 *   TOML_CORPUS_TOML_TEST  index files-toml-1.0.0 (.toml: valida ET
 *                          invalida; via relativa 'valid/...' datur)
 *   TOML_CORPUS_DOMUS      build/toml_corpus.lst (a cursore scripta)
 *   TOML_CORPUS_SILVESTRIA silvestria.manifestum: plagulae VIVAE;
 *                          sigillo mutato aut absente OMITTUNTUR CUM
 *                          NUMERO (decisio Frani, manifestum solum)
 * Visor plagulam quamque accipit; piscina 'opus' post quamque
 * vacatur (vide numquam ultra visorem servet).
 */

#ifndef TOML_CORPUS_AMBULARE_H
#define TOML_CORPUS_AMBULARE_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

nomen enumeratio {
    TOML_CORPUS_TOML_TEST = 0,
    TOML_CORPUS_DOMUS,
    TOML_CORPUS_SILVESTRIA,
    TOML_CORPUS_NUMERUS_FONTIUM
} TomlCorpusFons;

nomen vacuum (*TomlCorpusVisor)(vacuum* datum, TomlCorpusFons fons,
    constans character* via, chorda textus, Piscina* opus);

nomen structura {
    i32 plagulae[TOML_CORPUS_NUMERUS_FONTIUM];
    i32 omissae;          /* silvestres sigillo mutatae aut absentes */
    b32 indices_lecti;    /* index toml-test et lista domus adsunt */
} TomlCorpusNumeri;

/* radix = radix repositorii (RHUBARB_RADIX aut "."). */
vacuum
toml_corpus_ambulare (
                 Piscina* piscina,
                 Piscina* opus,
      constans character* radix,
         TomlCorpusVisor  visor,
                  vacuum* datum,
        TomlCorpusNumeri* numeri);

#endif /* TOML_CORPUS_AMBULARE_H */
