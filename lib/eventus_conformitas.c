/* eventus_conformitas.c - Vide eventus_conformitas.h */

#include "eventus_conformitas.h"
#include "eventus_stml.h"
#include "chorda_aedificator.h"
#include <string.h>


/* ==================================================
 * LEGERE
 * ================================================== */

interior chorda
_attributum (
             StmlNodus* n,
    constans character* t)
{
    chorda* c;
    chorda  vacua;

    c = stml_attributum_capere(n, t);
    si (c)
    {
        redde *c;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

interior s32
_numerus (
             StmlNodus* n,
    constans character* t)
{
    chorda* c;
       s32  v;

    v = ZEPHYRUM;
    c = stml_attributum_capere(n, t);
    si (c)
    {
        chorda_ut_s32(*c, &v);
    }
    redde v;
}

/* 'genera' spatiis divisa -> Xar de s32. FALSUM si titulus ignotus. */
interior b32
_genera_legere (
       chorda  fons,
          Xar* genera,
      Piscina* piscina)
{
    i32 k;
    i32 initium;

    k = ZEPHYRUM;
    dum (k < fons.mensura)
    {
        dum (k < fons.mensura && fons.datum[k] == ' ')
        {
            k++;
        }
        initium = k;
        dum (k < fons.mensura && fons.datum[k] != ' ')
        {
            k++;
        }
        si (k > initium)
        {
                        chorda  verbum;
            constans character* cstr;
               eventus_genus_t  g;

            verbum.datum    = fons.datum + initium;
            verbum.mensura  = k - initium;
            cstr            = chorda_ut_cstr(verbum, piscina);
            g               = eventus_genus_ex_titulo(cstr);
            si (g == EVENTUS_NIHIL)
            {
                redde FALSUM;
            }
            *(s32*)xar_addere(genera) = (s32)g;
        }
    }
    redde VERUM;
}

Xar*
eventus_conformitas_legere (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlResultus  r;
             Xar* scaenae;
             Xar* nodi;
             i32  i;

    r = stml_legere_ex_literis(cstr, piscina, intern);
    si (!r.successus || r.elementum_radix == NIHIL)
    {
        redde NIHIL;
    }
    scaenae  = xar_creare(piscina, (i32)magnitudo(ConformitasScaena));
    nodi     = stml_invenire_omnes_liberos(r.elementum_radix, "scaena",
        piscina);
    per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
    {
               StmlNodus* n = *(StmlNodus**)xar_obtinere(nodi, i);
       ConformitasScaena* s;
               StmlNodus* immissio;
               StmlNodus* expectata;

        s           = (ConformitasScaena*)xar_addere(scaenae);
        s->titulus  = _attributum(n, "titulus");
        s->genera   = xar_creare(piscina, (i32)magnitudo(s32));
        s->immissiones  = xar_creare(piscina,
            (i32)magnitudo(ConformitasImmissio));
        s->expectata    = xar_creare(piscina,
            (i32)magnitudo(StmlNodus*));
        si (!_genera_legere(_attributum(n, "genera"), s->genera,
                piscina))
        {
            redde NIHIL;
        }

        immissio = stml_invenire_liberum(n, "immissio");
        si (immissio)
        {
            i32 j;

            per (j = ZEPHYRUM; j
                < stml_numerus_liberorum(immissio); j++)
            {
                        StmlNodus* m = stml_liberum_ad_indicem(immissio,
                            j);
               ConformitasImmissio* im;

                si (m->genus != STML_NODUS_ELEMENTUM)
                {
                    perge;
                }
                im = (ConformitasImmissio*)xar_addere(s->immissiones);
                memset(im, ZEPHYRUM, magnitudo(ConformitasImmissio));
                im->modificantes  = (i32)_numerus(m, "modificantes");
                im->x             = _numerus(m, "x");
                im->y             = _numerus(m, "y");
                si (chorda_aequalis_literis(*m->titulus, "clavis"))
                {
                    im->genus        = CONFORMITAS_IMMISSIO_CLAVIS;
                    im->codex        = _numerus(m, "codex");
                    im->characteres  = _attributum(m, "characteres");
                    im->depressa     = (b32)(_numerus(m, "depressa")
                        != ZEPHYRUM);
                }
                alioquin
                {
                    im->genus      = CONFORMITAS_IMMISSIO_MUS;
                    im->mus_genus  = _attributum(m, "genus");
                }
            }
        }

        expectata = stml_invenire_liberum(n, "expectata");
        si (expectata)
        {
            Xar* eventa;
            i32  j;

            eventa = stml_invenire_omnes_liberos(expectata, "eventus",
                piscina);
            per (j = ZEPHYRUM; j < xar_numerus(eventa); j++)
            {
                *(StmlNodus**)xar_addere(s->expectata)
                    = *(StmlNodus**)xar_obtinere(eventa, j);
            }
        }
    }
    redde scaenae;
}


/* ==================================================
 * COMPARARE
 * ================================================== */

interior b32
_consideratur (
    constans ConformitasScaena* scaena,
               eventus_genus_t  genus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(scaena->genera); i++)
    {
        si (*(s32*)xar_obtinere(scaena->genera, i) == (s32)genus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_fluxum_appendere (
     ChordaAedificator* a,
    constans character* titulus,
                   Xar* nodi,
               Piscina* piscina)
{
    i32 i;

    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ":\n");
    per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
    {
        StmlNodus* n = *(StmlNodus**)xar_obtinere(nodi, i);

        chorda_aedificator_appendere_literis(a, "  ");
        /* nodus tabulae trivia sua fert (indentatio): praecisus */
        chorda_aedificator_appendere_chorda(a,
            chorda_praecidere(stml_scribere(n, piscina, FALSUM)));
        chorda_aedificator_appendere_literis(a, "\n");
    }
}

b32
eventus_conformitas_comparare (
    constans ConformitasScaena* scaena,
                  constans Xar* actualia,
                       Piscina* piscina,
           InternamentumChorda* intern,
                        chorda* diagnosis)
{
              Xar* considerata;
              Xar* nodi_actuales;
           chorda  scriptum;
     StmlResultus  r;
ChordaAedificator* a;
              i32  i;
              b32  conformis;

    si (diagnosis)
    {
        diagnosis->datum    = NIHIL;
        diagnosis->mensura  = ZEPHYRUM;
    }

    /* I. genera considerata solum */
    considerata = xar_creare(piscina, (i32)magnitudo(Eventus));
    per (i = ZEPHYRUM; i < xar_numerus(actualia); i++)
    {
        constans Eventus* e = (constans Eventus*)xar_obtinere(actualia,
            i);

        si (_consideratur(scaena, e->genus))
        {
            *(Eventus*)xar_addere(considerata) = *e;
        }
    }

    /* II. per scriptorem plagularum: tituli et formae eaedem */
    scriptum = eventus_scribere_stml(considerata, piscina, intern,
        FALSUM);
    r = stml_legere(scriptum, piscina, intern);
    si (!r.successus || r.elementum_radix == NIHIL)
    {
        redde FALSUM;
    }
    nodi_actuales = stml_invenire_omnes_liberos(r.elementum_radix,
        "eventus", piscina);

    /* III. numerus exactus, ordo exactus, attributa subset */
    a          = chorda_aedificator_creare(piscina, CCLVI);
    conformis  = VERUM;
    si (xar_numerus(nodi_actuales) != xar_numerus(scaena->expectata))
    {
        conformis = FALSUM;
        chorda_aedificator_appendere_literis(a, "numerus: expectata ");
        chorda_aedificator_appendere_i32(a,
            xar_numerus(scaena->expectata));
        chorda_aedificator_appendere_literis(a, ", actualia ");
        chorda_aedificator_appendere_i32(a, xar_numerus(nodi_actuales));
        chorda_aedificator_appendere_literis(a, "\n");
    }
    per (i = ZEPHYRUM; conformis && i < xar_numerus(scaena->expectata);
         i++)
    {
        StmlNodus* ex = *(StmlNodus**)xar_obtinere(scaena->expectata,
            i);
        StmlNodus* ac = *(StmlNodus**)xar_obtinere(nodi_actuales, i);
              i32  k;

        per (k = ZEPHYRUM; k < xar_numerus(ex->attributa); k++)
        {
            StmlAttributum* at = (StmlAttributum*)xar_obtinere(
                ex->attributa, k);
                   chorda* valor;

            si (chorda_aequalis_literis(*at->titulus, "tempus"))
            {
                perge;
            }
            valor = stml_attributum_capere(ac,
                chorda_ut_cstr(*at->titulus, piscina));
            si (valor == NIHIL || !chorda_aequalis(*valor, *at->valor))
            {
                conformis = FALSUM;
                chorda_aedificator_appendere_literis(a, "eventus ");
                chorda_aedificator_appendere_i32(a, i);
                chorda_aedificator_appendere_literis(a, ": ");
                chorda_aedificator_appendere_chorda(a, *at->titulus);
                chorda_aedificator_appendere_literis(a,
                    " expectatum \"");
                chorda_aedificator_appendere_chorda(a, *at->valor);
                chorda_aedificator_appendere_literis(a, "\", actuale ");
                si (valor)
                {
                    chorda_aedificator_appendere_literis(a, "\"");
                    chorda_aedificator_appendere_chorda(a, *valor);
                    chorda_aedificator_appendere_literis(a, "\"\n");
                }
                alioquin
                {
                    chorda_aedificator_appendere_literis(a, "absens\n");
                }
                frange;
            }
        }
    }

    si (!conformis && diagnosis)
    {
        _fluxum_appendere(a, "expectata", scaena->expectata, piscina);
        _fluxum_appendere(a, "actualia (considerata)", nodi_actuales,
            piscina);
        *diagnosis = chorda_aedificator_finire(a);
    }
    redde conformis;
}
