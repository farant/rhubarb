/* navigator_paginae.c - articulus in paginas (navigator N3.1; vide
 * caput). Textus per aedificatorem crescit; offset clausulae = mensura
 * textus ante eam. In paragraphis et indicibus '\n' (ex <br>) fit
 * spatium (mensura eadem, offsets notarum manent): lector_paginare
 * lineas novas solum in titulis et versibus tractat. */
#include "navigator_paginae.h"
#include "chorda_aedificator.h"
#include "url.h"
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


/* ==================================================
 * Index nexuum (N3.1b, spec V.19)
 * ================================================== */

/* "https://hospes/via..." -> hospes; via in *via (vacua si nulla).
 * FALSUM si sine "://" */
interior b32
hospitem_et_viam (
     chorda  url,
     chorda* hospes,
     chorda* via)
{
    i32 i;
    i32 a = ZEPHYRUM;

    per (i = ZEPHYRUM; i + II < url.mensura; i++)
    {
        si (   url.datum[i]      == ':' && url.datum[i + I] == '/'
            && url.datum[i + II] == '/')
        {
            a = i + III;
            frange;
        }
    }
    si (a == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = a; i < url.mensura && url.datum[i] != '/'; i++)
    {
    }
    *hospes  = chorda_sectio(url, a, i);
    *via     = chorda_sectio(url, i, url.mensura);
    redde VERUM;
}

/* URL sine #ancora */
interior chorda
sine_ancora (
    chorda url)
{
    i32 i;

    per (i = ZEPHYRUM; i < url.mensura; i++)
    {
        si (url.datum[i] == '#')
        {
            redde chorda_sectio(url, ZEPHYRUM, i);
        }
    }
    redde url;
}

/* linea indicis: titulus in hospite articuli (/wiki/T), aliter
 * "hospes: via decodicata"; scopus sine "://" ut est */
interior chorda
lineam_nexus (
        chorda  scopus,
        chorda  hospes_articuli,
       Piscina* piscina)
{
                           chorda hospes;
                           chorda via;
                           chorda r;
                              i32 i;
    hic_manens constans character praefixum[] = "/wiki/";

    scopus = sine_ancora(scopus);
    si (!hospitem_et_viam(scopus, &hospes, &via))
    {
        redde chorda_transcribere(scopus, piscina);
    }
    si (   chorda_aequalis(hospes, hospes_articuli)
        && chorda_incipit(via, chorda_ex_literis(praefixum, piscina))
        && via.mensura > (i32)strlen(praefixum))
    {
        r = url_viam_decodificare(chorda_sectio(via,
            (i32)strlen(praefixum), via.mensura), piscina);
        per (i = ZEPHYRUM; i < r.mensura; i++)
        {
            si (r.datum[i] == '_')
            {
                r.datum[i] = ' ';
            }
        }
        redde r;
    }
    redde chorda_concatenare(chorda_concatenare(hospes,
        chorda_ex_literis(": ", piscina), piscina),
        url_viam_decodificare(via, piscina), piscina);
}

/* fluxus secundus: "Links" + linea per scopum distinctum */
interior vacuum
indicem_nexuum_facere (
    NavigatorTextus* t,
             chorda  fons,
            Piscina* piscina)
{
    ChordaAedificator* aed;
                  Xar* visi;
               chorda  hospes_articuli;
               chorda  via;
                  i32  mensura = ZEPHYRUM;
                  i32  i;
                  i32  j;

    t->nexus.titulus = chorda_ex_literis("Links", piscina);
    t->nexus.clausulae = xar_creare(piscina,
        magnitudo(CodexClausula));
    t->notae_nexuum = xar_creare(piscina,
        magnitudo(ArticulusNota));
    t->nexus.textus.datum = NIHIL;
    t->nexus.textus.mensura = ZEPHYRUM;
    visi = xar_creare(piscina, (i32)magnitudo(chorda));
    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(IV * MXXIV));
    si (!t->nexus.clausulae || !t->notae_nexuum || !visi || !aed)
    {
        redde;
    }
    hospes_articuli.datum    = NIHIL;
    hospes_articuli.mensura  = ZEPHYRUM;
    (vacuum)hospitem_et_viam(fons, &hospes_articuli, &via);
    per (i = ZEPHYRUM; i < xar_numerus(t->notae); i++)
    {
        constans ArticulusNota* n = (constans ArticulusNota*)
            xar_obtinere(t->notae, i);
                         chorda  clavis;
                         chorda  linea;
                  CodexClausula* k;
                  ArticulusNota* m;

        si (   n->genus          != ARTICULUS_NEXUS
            || n->scopus.mensura == ZEPHYRUM)
        {
            perge;
        }
        clavis = sine_ancora(n->scopus);
        per (j = ZEPHYRUM; j < xar_numerus(visi); j++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(visi, j),
                clavis))
            {
                frange;
            }
        }
        si (j < xar_numerus(visi))
        {
            perge;
        }
        *(chorda*)xar_addere(visi) = clavis;
        si (mensura == ZEPHYRUM)
        {
            /* titulus "Links" ante lineam primam */
            chorda_aedificator_appendere_literis(aed, "Links");
            k = (CodexClausula*)xar_addere(t->nexus.clausulae);
            k->genus = CODEX_TITULUS;
            k->initium = ZEPHYRUM;
            k->finis = V;
            mensura = V;
        }
        linea = lineam_nexus(n->scopus, hospes_articuli, piscina);
        chorda_aedificator_appendere_literis(aed, "\n\n");
        mensura += II;
        chorda_aedificator_appendere_chorda(aed, linea);
        k           = (CodexClausula*)xar_addere(t->nexus.clausulae);
        k->genus    = CODEX_PARAGRAPHUS;
        k->initium  = mensura;
        k->finis    = mensura + linea.mensura;
        m           = (ArticulusNota*)xar_addere(t->notae_nexuum);
        m->genus    = ARTICULUS_NEXUS;
        m->initium  = mensura;
        m->finis    = mensura + linea.mensura;
        m->scopus   = n->scopus;
        mensura     += linea.mensura;
    }
    si (mensura > ZEPHYRUM)
    {
        t->nexus.textus = chorda_aedificator_finire(aed);
    }
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
    indicem_nexuum_facere(t, articulus ? articulus->fons
        : chorda_ex_literis("", piscina), piscina);
    redde t;
}
