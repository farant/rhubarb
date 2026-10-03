/* probatio_conformitas_terminalis.c - Cursor terminalis tabulae
 * conformitatis (eventus B4b; spec D7)
 *
 * Eadem tabula ac fenestrae (probationes/fixa/eventus/conformitas.stml)
 * - columna terminalis: <terminalis profilum octeti> per scaenam. Sine
 * terminali, sine fenestra: rivus PURUS octetos accipit. Cellula 1x1
 * pixelum: cellula c -> pixelum c-1, ergo expectata muris fenestrae
 * (pixela) valent. Lectio coalita (motus per lectionem coalescit, ut
 * fenestra); post octetos silentium (rivus_moram: ESC solus).
 *
 * Excusationes per FACULTATES quas rivus ipse publicat (B4a). Quae
 * scaenae EXCUSATAE sint hic NOMINATUR - excusatio tacita = porta
 * mortua.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "filum.h"
#include "eventus.h"
#include "eventus_conformitas.h"
#include "rivus_terminalis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define TABULA "probationes/fixa/eventus/conformitas.stml"

/* Visus (textus, viae, exempla) usque ad lectionem proximam vivunt:
 * in piscinam copiantur */
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
    si (e->genus == EVENTUS_DEPOSITIO && e->datum.depositio.viae.mensura
        > ZEPHYRUM)
    {
        i8* nova = (i8*)piscina_allocare(piscina,
            (memoriae_index)e->datum.depositio.viae.mensura);

        memcpy(nova, e->datum.depositio.viae.datum,
            (memoriae_index)e->datum.depositio.viae.mensura);
        e->datum.depositio.viae.datum = nova;
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

interior vacuum
_legere_omnia (
    RivusTerminalis* r,
                Xar* actualia,
            Piscina* piscina)
{
    Eventus e;

    dum (rivus_eventum_coalitum(r, M, &e))
    {
        _onera_copiare(&e, piscina);
        *(Eventus*)xar_addere(actualia) = e;
    }
}

/* Scaenam sub immissione terminali currere: octeti -> rivus -> Eventus,
 * deinde silentium, deinde comparatio. */
interior ConformitasVerdictum
_currere (
        constans ConformitasScaena* scaena,
    constans ConformitasTerminalis* t,
                           Piscina* piscina,
               InternamentumChorda* intern,
                            chorda* diagnosis)
{
    RivusTerminalis* r;
                 i8  modi[RIVUS_MODI_MAXIMUM];
                i32  declarati = RIVUS_MODUS_MUS
                    | RIVUS_MODUS_GLUTINUM;
                i32  traditi  = ZEPHYRUM;
                i32  gyri     = ZEPHYRUM;
                Xar* actualia;

    r         = rivus_creare(piscina, I, I);
    actualia  = xar_creare(piscina, (i32)magnitudo(Eventus));
    si (chorda_aequalis_literis(t->profilum, "kitty"))
    {
        declarati |= RIVUS_MODUS_KITTY;
    }
    (vacuum)rivus_modos_intrare(r, declarati, modi, RIVUS_MODI_MAXIMUM);
    dum (traditi < t->octeti.mensura && gyri < M)
    {
        traditi += rivus_tradere(r, t->octeti.datum + traditi,
            t->octeti.mensura - traditi);
        _legere_omnia(r, actualia, piscina);
        gyri++;
    }
    rivus_moram(r, M);
    _legere_omnia(r, actualia, piscina);
    redde eventus_conformitas_comparare(scaena, actualia, piscina,
        intern,
        diagnosis);
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                 chorda  fons;
                    Xar* tabula;
                    i32  i;
                    i32  cursus              = ZEPHYRUM;
                    i32  excusatae           = ZEPHYRUM;
                    b32  excusatio_nominata  = FALSUM;

    piscina =
        piscina_generare_dynamicum("probatio_conformitas_terminalis",
        M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    fons    = filum_legere_totum(TABULA, piscina);
    tabula  = (fons.mensura > ZEPHYRUM)
        ? eventus_conformitas_legere(chorda_ut_cstr(fons, piscina),
              piscina, intern)
        : NIHIL;
    CREDO_NON_NIHIL (tabula);
    si (tabula == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }

    per (i = ZEPHYRUM; i < xar_numerus(tabula); i++)
    {
        ConformitasScaena* s = (ConformitasScaena*)xar_obtinere(tabula,
            i);
                      i32 k;
                      b32 vetustum  = FALSUM;
                      b32 kitty     = FALSUM;

        imprimere("\n--- %.*s ---\n", (int)s->titulus.mensura,
            (constans character*)s->titulus.datum);
        per (k = ZEPHYRUM; k < xar_numerus(s->terminales); k++)
        {
            ConformitasTerminalis* t = (ConformitasTerminalis*)
                xar_obtinere(s->terminales, k);
                           chorda diagnosis;
             ConformitasVerdictum v;

            vetustum  = vetustum || chorda_aequalis_literis(t->profilum,
                "legacy");
            kitty   = kitty || chorda_aequalis_literis(t->profilum,
                "kitty");
            v = _currere(s, t, piscina, intern, &diagnosis);
            cursus++;
            si (v == CONFORMITAS_EXCUSATA)
            {
                excusatae++;
                si (   chorda_aequalis_literis(s->titulus, "ctrl-i")
                    && chorda_aequalis_literis(t->profilum, "legacy"))
                {
                    excusatio_nominata = VERUM;
                }
            }
            si (v == CONFORMITAS_FRACTA)
            {
                imprimere("FRACTA %.*s [%.*s]\n%.*s",
                    (int)s->titulus.mensura,
                    (constans character*)s->titulus.datum,
                    (int)t->profilum.mensura,
                    (constans character*)t->profilum.datum,
                    (int)diagnosis.mensura,
                    (constans character*)diagnosis.datum);
            }
            CREDO_VERUM (v != CONFORMITAS_FRACTA);
        }
        /* columna terminalis plena: profilum utrumque */
        CREDO_VERUM (vetustum);
        CREDO_VERUM (kitty);
    }

    imprimere("\n--- excusationes nominatae ---\n");
    CREDO_AEQUALIS_I32 (cursus, II * xar_numerus(tabula));
    CREDO_AEQUALIS_I32 (excusatae, I);
    CREDO_VERUM (excusatio_nominata);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
