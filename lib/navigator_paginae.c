/* navigator_paginae.c - articulus in paginas (navigator N3.1; vide
 * caput). Textus per aedificatorem crescit; offset clausulae = mensura
 * textus ante eam. In paragraphis et indicibus '\n' (ex <br>) fit
 * spatium (mensura eadem, offsets notarum manent): lector_paginare
 * lineas novas solum in titulis et versibus tractat. */
#include "navigator_paginae.h"
#include "chorda_aedificator.h"
#include <string.h>

#define PUNCTUM "\xE2\x80\xA2 "   /* U+2022 + spatium */

nomen structura {
    NavigatorTextus* t;
  ChordaAedificator* aed;
                i32  mensura;    /* octeti scripti */
            Piscina* piscina;
} Scriptor;

interior vacuum
octetos_addere (
                Scriptor* s,
      constans character* octeti,
                     i32  n)
{
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        chorda_aedificator_appendere_character(s->aed, octeti[i]);
    }
    s->mensura += n;
}

/* clausulam addere: separator, praefixum, textus (linea nova ->
 * spatium nisi titulus); notae translatae. Textus vacuus omittitur. */
interior vacuum
clausulam_scribere (
                 Scriptor* s,
               CodexGenus  genus,
       constans character* praefixum,
                   chorda  textus,
                      Xar* notae)
{
    CodexClausula* k;
              i32  initium;
              i32  basis;
              i32  i;
        character  c;

    si (textus.mensura == ZEPHYRUM)
    {
        redde;
    }
    si (s->mensura > ZEPHYRUM)
    {
        octetos_addere(s, "\n\n", II);
    }
    initium = s->mensura;
    octetos_addere(s, praefixum, (i32)strlen(praefixum));
    basis = s->mensura;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        c = (character)textus.datum[i];
        si (c == '\n' && genus != CODEX_TITULUS)
        {
            c = ' ';
        }
        octetos_addere(s, &c, I);
    }
    k = (CodexClausula*)xar_addere(s->t->caput.clausulae);
    si (k)
    {
        k->genus    = genus;
        k->initium  = initium;
        k->finis    = s->mensura;
    }
    per (i = ZEPHYRUM; notae && i < xar_numerus(notae); i++)
    {
        constans ArticulusNota* n = (constans ArticulusNota*)
            xar_obtinere(notae, i);
                 ArticulusNota* m;

        si (n->finis <= n->initium || n->finis > textus.mensura)
        {
            perge;
        }
        m = (ArticulusNota*)xar_addere(s->t->notae);
        si (m)
        {
            *m          = *n;
            m->initium  = basis + n->initium;
            m->finis    = basis + n->finis;
        }
    }
}

/* "  " per gradum ultra primum, deinde punctum */
interior constans character*
praefixum_indicis (
         s32  gradus,
     Piscina* piscina)
{
    character* p;
          s32  spatia;
          s32  i;

    spatia = gradus > I ? II * (gradus - I) : ZEPHYRUM;
    p = (character*)piscina_allocare(piscina,
        (memoriae_index)(spatia + (s32)strlen(PUNCTUM) + I));
    per (i = ZEPHYRUM; i < spatia; i++)
    {
        p[i] = ' ';
    }
    strcpy(p + spatia, PUNCTUM);
    redde p;
}

NavigatorTextus*
navigator_textum_facere (
    constans Articulus* articulus,
               Piscina* piscina)
{
    NavigatorTextus* t;
           Scriptor  s;
                i32  i;
                i32  j;

    t = (NavigatorTextus*)piscina_allocare(piscina,
        magnitudo(NavigatorTextus));
    si (!t)
    {
        redde NIHIL;
    }
    t->caput.titulus   = articulus ? articulus->titulus
        : chorda_ex_literis("", piscina);
    t->caput.clausulae  = xar_creare(piscina, magnitudo(CodexClausula));
    t->notae            = xar_creare(piscina, magnitudo(ArticulusNota));
    s.t                 = t;
    s.aed     = chorda_aedificator_creare(piscina, (memoriae_index)
        (XVI * MXXIV));
    s.mensura = ZEPHYRUM;
    s.piscina = piscina;
    si (!s.aed || !t->caput.clausulae || !t->notae)
    {
        redde NIHIL;
    }
    si (articulus)
    {
        clausulam_scribere(&s, CODEX_TITULUS, "", articulus->titulus,
            NIHIL);
        per (i = ZEPHYRUM; articulus->sectiones
            && i < xar_numerus(articulus->sectiones); i++)
        {
            constans ArticulusSectio* sectio =
                (constans ArticulusSectio*)
                xar_obtinere(articulus->sectiones, i);

            clausulam_scribere(&s, CODEX_TITULUS, "", sectio->titulus,
                NIHIL);
            per (j = ZEPHYRUM; sectio->clausulae
                && j < xar_numerus(sectio->clausulae); j++)
            {
                constans ArticulusClausula* k =
                    (constans ArticulusClausula*)xar_obtinere(
                    sectio->clausulae, j);

                commutatio (k->genus)
                {
                    casus ARTICULUS_TITULUS:
                        clausulam_scribere(&s, CODEX_TITULUS, "",
                            k->textus, k->notae);
                        frange;
                    casus ARTICULUS_PARAGRAPHUS:
                    casus ARTICULUS_CITATIO:
                        clausulam_scribere(&s, CODEX_PARAGRAPHUS, "",
                            k->textus, k->notae);
                        frange;
                    casus ARTICULUS_INDEX:
                        clausulam_scribere(&s, CODEX_PARAGRAPHUS,
                            praefixum_indicis(k->gradus, piscina),
                            k->textus, k->notae);
                        frange;
                    ordinarius:
                        frange;   /* FIGURA (N5), PAR (adiuncta) */
                }
            }
        }
    }
    t->caput.textus = chorda_aedificator_finire(s.aed);
    redde t;
}
