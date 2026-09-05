/* oratio_vocabularia.c - Vocabularia orationis: onerator unus (T16) */

#include "oratio_vocabularia.h"
#include "oratio_glossarium.h"
#include <stdio.h>
#include <string.h>

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    f                = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (memoria == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde FALSUM;
    }
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = (i8*)memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

b32
oratio_vocabularia_onerare (
                     Piscina* piscina,
          constans character* radix,
           OratioVocabularia* exitus,
    OratioVocabulariumVitium* vitium)
{
               character  via[1024];
                  chorda  fons;
    OratioVocabulariumLa* la;
        OratioGlossarium* gl;
    OratioVocabulariumEn* en;

    exitus->la = NIHIL;
    exitus->en = NIHIL;
    memset(vitium, ZEPHYRUM, magnitudo(*vitium));
    si (radix == NIHIL)
    {
        radix = ".";
    }
    sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
    si (!_plagulam_legere(piscina, via, &fons))
    {
        vitium->plagula  = "oratio/vocabularium/la.bin";
        vitium->causa    = "plagula absens";
        redde FALSUM;
    }
    la = oratio_vocabularium_la_onerare(piscina, fons, vitium);
    si (la == NIHIL)
    {
        redde FALSUM;
    }
    sprintf(via, "%s/oratio/glossarium.stml", radix);
    si (!_plagulam_legere(piscina, via, &fons))
    {
        vitium->plagula  = "oratio/glossarium.stml";
        vitium->causa    = "plagula absens";
        redde FALSUM;
    }
    gl = oratio_glossarium_legere(piscina, fons, vitium);
    si (gl == NIHIL)
    {
        redde FALSUM;
    }
    oratio_vocabularium_la_glossarium_ponere(la, gl);
    sprintf(via, "%s/oratio/vocabularium/en/mobypos.txt", radix);
    si (!_plagulam_legere(piscina, via, &fons))
    {
        vitium->plagula  = "oratio/vocabularium/en/mobypos.txt";
        vitium->causa    = "plagula absens";
        redde FALSUM;
    }
    en = oratio_vocabularium_en_onerare(piscina, fons, vitium);
    si (en == NIHIL)
    {
        redde FALSUM;
    }
    exitus->la = la;
    exitus->en = en;
    redde VERUM;
}
