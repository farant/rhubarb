/* fabrica_litterae.c - litterae chordarum per silvam amalgamatam
 * (caput tools/fabrica_litterae.h, cur unitas separata ibi) */

#include "fabrica_litterae.h"
#include "silva.h"

b32
suturae_litteras_colligere (
               chorda  textus,
              Piscina* piscina,
                  Xar* fructus)
{
          SilvaPiscina* arena;
              SilvaXar* lexemata;
    insignatus integer  i;

    arena = silva_piscina_generare_dynamicum("portae_litterae",
        (size_t)LXV * MXXIV);
    si (arena == NIHIL)
    {
        redde FALSUM;
    }
    lexemata = silva_lexare(arena, (constans character*)textus.datum,
        textus.mensura, ZEPHYRUM);
    si (lexemata == NIHIL)
    {
        silva_piscina_destruere(arena);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < silva_xar_numerus(lexemata); i++)
    {
        constans SilvaToken* t = *(SilvaToken**)silva_xar_obtinere(
            lexemata, i);

        si (   silva_token_genus(t) == SILVA_LEX_STRING_LIT
            && t->valor.mensura     >= II)
        {
            /* contentum sine '"' in piscinam domus (arena perit) */
            *(chorda*)xar_addere(fructus) = chorda_transcribere(
                chorda_ex_buffer(t->valor.datum + I,
                    (i32)(t->valor.mensura - II)), piscina);
        }
    }
    silva_piscina_destruere(arena);
    redde VERUM;
}
