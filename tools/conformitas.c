/* conformitas.c - Cursor tabulae conformitatis contra fenestram
 * (eventus A4; spec D7)
 *
 * Fenestram RETRO aperit (focum non rapit: immissa in caudam
 * applicationis ipsius ponuntur, quam fenestra legit). Per scaenam:
 * caudam vacuare, immissiones ponere, legere donec quiescat, cum
 * expectatis comparare (lib/eventus_conformitas). Onera visus (textus,
 * exempla) in piscinam COPIANTUR dum colliguntur - visus usque ad
 * lectionem proximam solum vivunt.
 *
 * Curre per: ./tools/conformitas.sh [tabula.stml]
 * Exitus: 0 omnes conformes, 1 aliqua non, 2 tabula prava.
 * Murem ne moveas dum currit: motus veri in scaenas tractus intrant.
 */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "internamentum.h"
#include "filum.h"
#include "fenestra.h"
#include "eventus_conformitas.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_onera_copiare (
     Eventus* e,
     Piscina* piscina)
{
    si (e->genus == EVENTUS_TEXTUS && e->datum.textus.contentum.mensura
        > ZEPHYRUM)
    {
        i8* nova = (i8*)piscina_allocare(piscina,
            (memoriae_index)e->datum.textus.contentum.mensura);

        memcpy(nova, e->datum.textus.contentum.datum,
            (memoriae_index)e->datum.textus.contentum.mensura);
        e->datum.textus.contentum.datum = nova;
    }
    si (   e->genus == EVENTUS_MUS_MOTUS
        && e->datum.mus.numerus_exemplorum > ZEPHYRUM)
    {
         memoriae_index  n;
        EventusExemplum* nova;

        n = (memoriae_index)e->datum.mus.numerus_exemplorum
            * magnitudo(EventusExemplum);
        nova = (EventusExemplum*)piscina_allocare_ordinatum(piscina, n,
            VIII);
        memcpy(nova, e->datum.mus.exempla, n);
        e->datum.mus.exempla = nova;
    }
}

/* Legere donec quiescat: post eventum ultimum C ms sine novo, aut D
 * ms in summa. */
interior Xar*
_colligere (
    Fenestra* fenestra,
     Piscina* piscina)
{
        Xar* eventa;
        s64  initium;
        s64  ultimum;
    Eventus  e;

    eventa   = xar_creare(piscina, (i32)magnitudo(Eventus));
    initium  = fenestra_tempus_ms();
    ultimum  = initium;
    dum (   fenestra_tempus_ms() - ultimum < C
         && fenestra_tempus_ms() - initium < D)
    {
        fenestra_expectare_eventus(fenestra, XX);
        dum (fenestra_obtinere_eventus(fenestra, &e))
        {
            _onera_copiare(&e, piscina);
            *(Eventus*)xar_addere(eventa)  = e;
            ultimum                        = fenestra_tempus_ms();
        }
    }
    redde eventa;
}

interior b32
_immittere (
                        Fenestra* fenestra,
    constans ConformitasImmissio* im,
                         Piscina* piscina)
{
    FenestraMusGenus genus;

    si (im->genus == CONFORMITAS_IMMISSIO_CLAVIS)
    {
        redde fenestra_clavem_immittere(fenestra, (i32)im->codex,
            im->modificantes, chorda_ut_cstr(im->characteres, piscina),
            im->depressa);
    }
    si (chorda_aequalis_literis(im->mus_genus, "motus"))
    { genus = FENESTRA_MUS_MOTUS;
    }
    alioquin si (chorda_aequalis_literis(im->mus_genus, "depressio"))
    { genus = FENESTRA_MUS_DEPRESSIO;
    }
    alioquin si (chorda_aequalis_literis(im->mus_genus, "tractus"))
    { genus = FENESTRA_MUS_TRACTUS;
    }
    alioquin si (chorda_aequalis_literis(im->mus_genus, "liberatio"))
    { genus = FENESTRA_MUS_LIBERATIO;
    }
    alioquin si (chorda_aequalis_literis(im->mus_genus,
                 "depressio-dextra"))
    { genus = FENESTRA_MUS_DEPRESSIO_DEXTRA;
    }
    alioquin si (chorda_aequalis_literis(im->mus_genus,
                 "liberatio-dextra"))
    { genus = FENESTRA_MUS_LIBERATIO_DEXTRA;
    }
    alioquin
    { redde FALSUM;
    }
    /* x, y non negativa in tabula (immissio i32) */
    redde fenestra_murem_immittere(fenestra, genus, (i32)im->x,
        (i32)im->y, im->modificantes);
}

s32
principale (
          s32   argc,
    character** argv)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
      constans character* via;
                  chorda  fons;
                     Xar* tabula;
    FenestraConfiguratio  configuratio;
                Fenestra* fenestra;
                     i32  i;
                     i32  conformes = ZEPHYRUM;

    via = (argc > I) ? argv[I]
                     : "probationes/fixa/eventus/conformitas.stml";
    piscina = piscina_generare_dynamicum("conformitas", 16777216);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "conformitas: piscina deest\n");
        redde II;
    }
    intern  = internamentum_creare(piscina);
    fons    = filum_legere_totum(via, piscina);
    tabula  = (fons.mensura > ZEPHYRUM)
        ? eventus_conformitas_legere(chorda_ut_cstr(fons, piscina),
              piscina, intern)
        : NIHIL;
    si (tabula == NIHIL)
    {
        fprintf(stderr, "conformitas: tabula prava aut absens: %s\n",
            via);
        redde II;
    }

    memset(&configuratio, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    configuratio.titulus   = "conformitas - eventus A4";
    configuratio.x         = C;
    configuratio.y         = C;
    configuratio.latitudo  = DC;
    configuratio.altitudo  = CD;
    configuratio.vexilla   = FENESTRA_ORDINARIA | FENESTRA_RETRO;
    fenestra               = fenestra_creare(piscina, &configuratio);
    si (fenestra == NIHIL)
    {
        fprintf(stderr, "conformitas: fenestra deest\n");
        redde II;
    }
    (vacuum)_colligere(fenestra, piscina);   /* initium: facultates */

    per (i = ZEPHYRUM; i < xar_numerus(tabula); i++)
    {
        ConformitasScaena* s = (ConformitasScaena*)xar_obtinere(tabula,
            i);
                      Xar* actualia;
                   chorda  diagnosis;
                      i32  k;
                      b32  bene       = VERUM;
     ConformitasVerdictum  verdictum  = CONFORMITAS_FRACTA;

        (vacuum)_colligere(fenestra, piscina);           /* vacuare */
        per (k = ZEPHYRUM; k < xar_numerus(s->immissiones); k++)
        {
            si (!_immittere(fenestra,
                    (ConformitasImmissio*)xar_obtinere(s->immissiones,
                        k), piscina))
            {
                bene = FALSUM;
            }
        }
        actualia = _colligere(fenestra, piscina);
        si (bene)
        {
            verdictum = eventus_conformitas_comparare(s, actualia,
                piscina, intern, &diagnosis);
            /* EXCUSATA = conformis per facultatem declaratam */
            bene = (b32)(verdictum != CONFORMITAS_FRACTA);
        }
        alioquin
        {
            diagnosis = chorda_ex_literis("immissio recusata\n",
                piscina);
        }
        printf("%s %.*s\n", (verdictum == CONFORMITAS_EXCUSATA)
            ? "EXCUSATA " : bene ? "CONFORMIS" : "FRACTA   ",
            (int)s->titulus.mensura,
            (constans character*)s->titulus.datum);
        si (!bene)
        {
            printf("%.*s", (int)diagnosis.mensura,
                (constans character*)diagnosis.datum);
        }
        fflush(stdout);
        si (bene)
        {
            conformes++;
        }
    }
    printf("conformitas: %u / %u scaenae conformes\n",
        (unsigned)conformes, (unsigned)xar_numerus(tabula));
    fenestra_destruere(fenestra);
    redde (conformes == xar_numerus(tabula)) ? ZEPHYRUM : I;
}
