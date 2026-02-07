/* Auto-generatum ex schemate nuntii */
#ifndef PERSONA_GEN_H
#define PERSONA_GEN_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "nuntium.h"

#define PERSONA_TAG_ID 1
#define PERSONA_TAG_AETAS 2
#define PERSONA_TAG_APPELLATIO 3

nomen structura {
    chorda* id;
    i64     aetas;
    chorda* appellatio;
} Persona;

b32 persona_scribere(NuntiumScriptor* scriptor, constans Persona* nuntium);
Persona* persona_legere(NuntiumLector* lector, Piscina* piscina);

#endif /* PERSONA_GEN_H */
