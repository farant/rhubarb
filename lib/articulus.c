/* articulus.c - forma canonica et sigillum articuli (navigator N2.1)
 *
 * Forma (linea per elementum; campi "<n>:<octeti>", numeri "<n>;"):
 *   A titulus                         articulus
 *   S titulus ancora                  sectio
 *   K genus; gradus; clavis textus    clausula
 *   N genus; initium; finis; scopus   nota clausulae praecedentis
 *   D titulus                         adiunctum (clausulae K sequuntur)
 * Clausulae sectionis ante adiuncta eius stant, ergo K post D ad
 * adiunctum pertinet. Codificator unus formam aut in aedificatorem
 * aut in sigillum incrementale scribit - duae viae numquam
 * dissentiunt. */
#include "articulus.h"
#include "sigillum.h"
#include "chorda_aedificator.h"

/* scriptor formae: aedificator XOR sigillum */
nomen structura {
    ChordaAedificator* aedificator;
    SigillumContextus* sigillum;
} Scriptor;

interior vacuum
octetos_scribere (
              Scriptor* s,
    constans character* octeti,
                   i32  n)
{
    i32 i;

    si (n == ZEPHYRUM)
    {
        redde;
    }
    si (s->sigillum)
    {
        sigillum_addere(s->sigillum, octeti, (memoriae_index)n);
        redde;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        chorda_aedificator_appendere_character(s->aedificator,
            octeti[i]);
    }
}

/* numerus decimalis (signo si negativus) + terminator */
interior vacuum
numerum_scribere (
     Scriptor* s,
          s64  n,
    character  terminator)
{
    character capsa[XXIV];
          i32 k;
          i64 u;

    k           = XXIV;
    capsa[--k]  = terminator;
    u           = (n < ZEPHYRUM) ? (i64)(-n) : (i64)n;
    fac
    {
        capsa[--k]  = (character)('0' + (s32)(u % X));
        u           /= X;
    } dum (u > ZEPHYRUM);
    si (n < ZEPHYRUM)
    {
        capsa[--k] = '-';
    }
    octetos_scribere(s, capsa + k, XXIV - k);
}

interior vacuum
campum_scribere (
    Scriptor* s,
      chorda  c)
{
    numerum_scribere(s, (s64)c.mensura, ':');
    octetos_scribere(s, (constans character*)c.datum, (i32)c.mensura);
}

interior vacuum
signum_scribere (
     Scriptor* s,
    character  signum)
{
    octetos_scribere(s, &signum, I);
}

interior vacuum
clausulas_scribere (
    Scriptor* s,
         Xar* clausulae)
{
                  i32  i;
                  i32  j;
    ArticulusClausula* k;
        ArticulusNota* n;

    per (i = ZEPHYRUM; clausulae && i < xar_numerus(clausulae); i++)
    {
        k = (ArticulusClausula*)xar_obtinere(clausulae, i);
        signum_scribere(s, 'K');
        numerum_scribere(s, (s64)k->genus, ';');
        numerum_scribere(s, (s64)k->gradus, ';');
        campum_scribere(s, k->clavis);
        campum_scribere(s, k->textus);
        signum_scribere(s, '\n');
        per (j = ZEPHYRUM; k->notae && j < xar_numerus(k->notae); j++)
        {
            n = (ArticulusNota*)xar_obtinere(k->notae, j);
            signum_scribere(s, 'N');
            numerum_scribere(s, (s64)n->genus, ';');
            numerum_scribere(s, (s64)n->initium, ';');
            numerum_scribere(s, (s64)n->finis, ';');
            campum_scribere(s, n->scopus);
            signum_scribere(s, '\n');
        }
    }
}

interior vacuum
articulum_scribere (
              Scriptor* s,
    constans Articulus* a)
{
                   i32  i;
                   i32  j;
       ArticulusSectio* sectio;
    ArticulusAdiunctum* d;

    signum_scribere(s, 'A');
    campum_scribere(s, a->titulus);
    signum_scribere(s, '\n');
    per (i = ZEPHYRUM; a->sectiones && i < xar_numerus(a->sectiones);
         i++)
    {
        sectio = (ArticulusSectio*)xar_obtinere(a->sectiones, i);
        signum_scribere(s, 'S');
        campum_scribere(s, sectio->titulus);
        campum_scribere(s, sectio->ancora);
        signum_scribere(s, '\n');
        clausulas_scribere(s, sectio->clausulae);
        per (j = ZEPHYRUM;
             sectio->adiuncta && j < xar_numerus(sectio->adiuncta); j++)
        {
            d = (ArticulusAdiunctum*)xar_obtinere(sectio->adiuncta, j);
            signum_scribere(s, 'D');
            campum_scribere(s, d->titulus);
            signum_scribere(s, '\n');
            clausulas_scribere(s, d->clausulae);
        }
    }
}

chorda
articulus_canonicum (
    constans Articulus* articulus,
               Piscina* piscina)
{
    Scriptor s;

    s.sigillum    = NIHIL;
    s.aedificator = chorda_aedificator_creare(piscina, (memoriae_index)
        (IV * M));
    si (!s.aedificator)
    {
        redde chorda_ex_literis("", piscina);
    }
    si (articulus)
    {
        articulum_scribere(&s, articulus);
    }
    redde chorda_aedificator_finire(s.aedificator);
}

vacuum
articulus_sigillum (
    constans Articulus* articulus,
             character* effusio)
{
    SigillumContextus ctx;
             Scriptor s;
             Sigillum sg;

    sigillum_incipere(&ctx);
    s.aedificator  = NIHIL;
    s.sigillum     = &ctx;
    si (articulus)
    {
        articulum_scribere(&s, articulus);
    }
    sg = sigillum_finire(&ctx);
    sigillum_hex(&sg, effusio);
}
