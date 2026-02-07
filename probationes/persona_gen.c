/* Auto-generatum ex schemate nuntii */
#include "persona_gen.h"
#include <string.h>

b32
persona_scribere(NuntiumScriptor* scriptor, constans Persona* nuntium)
{
  si (!scriptor || !nuntium) redde FALSUM;

  si (nuntium->id)
  {
    si (!nuntium_scribere_chorda(scriptor, PERSONA_TAG_ID, *nuntium->id))
        redde FALSUM;
  }

  si (!nuntium_scribere_varint(scriptor, PERSONA_TAG_AETAS, nuntium->aetas))
      redde FALSUM;

  si (nuntium->appellatio)
  {
    si (!nuntium_scribere_chorda(scriptor, PERSONA_TAG_APPELLATIO, *nuntium->appellatio))
        redde FALSUM;
  }

  redde VERUM;
}

Persona*
persona_legere(NuntiumLector* lector, Piscina* piscina)
{
  Persona* n;
  i32 tag;
  i32 wire_type;

  si (!lector || !piscina) redde NIHIL;

  n = (Persona*)piscina_allocare(piscina, (memoriae_index)magnitudo(Persona));
  si (!n) redde NIHIL;
  memset(n, 0, magnitudo(Persona));

  dum (nuntium_legere_field(lector, &tag, &wire_type))
  {
    commutatio (tag)
    {
      casus PERSONA_TAG_ID:
      {
        chorda tmp = nuntium_legere_chorda(lector);
        n->id = (chorda*)piscina_allocare(piscina, (memoriae_index)magnitudo(chorda));
        si (n->id)
        {
          n->id->datum = (i8*)piscina_allocare(piscina, (memoriae_index)tmp.mensura);
          si (n->id->datum)
          {
            memcpy(n->id->datum, tmp.datum, (memoriae_index)tmp.mensura);
            n->id->mensura = tmp.mensura;
          }
        }
        frange;
      }
      casus PERSONA_TAG_AETAS:
        n->aetas = nuntium_legere_varint(lector);
        frange;
      casus PERSONA_TAG_APPELLATIO:
      {
        chorda tmp = nuntium_legere_chorda(lector);
        n->appellatio = (chorda*)piscina_allocare(piscina, (memoriae_index)magnitudo(chorda));
        si (n->appellatio)
        {
          n->appellatio->datum = (i8*)piscina_allocare(piscina, (memoriae_index)tmp.mensura);
          si (n->appellatio->datum)
          {
            memcpy(n->appellatio->datum, tmp.datum, (memoriae_index)tmp.mensura);
            n->appellatio->mensura = tmp.mensura;
          }
        }
        frange;
      }
      ordinarius:
        nuntium_saltare_field(lector, wire_type);
        frange;
    }
  }

  si (nuntium_lector_error(lector)) redde NIHIL;
  redde n;
}
