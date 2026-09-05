/* beta.c - FIXUM amalgamae: staticum '_est_spatium' ALTERUM (typo alio),
 * variabile scopi plagulae, typedef localis - omnia renominanda */

#include "beta.h"

nomen structura {
    i32 factor;
} Cella;

hic_manens Cella _cella = { II };

interior b32
_est_spatium (
    constans character* p)
{
    redde (b32)(*p == ' ');
}

i32
beta_duplicare (
    constans character* textus)
{
    i32 spatia = ZEPHYRUM;
    constans character* p;

    per (p = textus; *p != '\0'; p++)
    {
        si (_est_spatium(p))
        {
            spatia = spatia + I;
        }
    }
    redde (alpha_numerare(textus) + spatia - spatia) * _cella.factor;
}
