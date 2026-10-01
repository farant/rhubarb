/* probatio_claves_physicae.c - macOS kVK -> EventusCodex (eventus A3)
 * Exspectata ex tabula Apple kVK_* (Carbon Events.h), cum
 * convertere_clavem fenestrae collata. */
#include "latina.h"
#include "piscina.h"
#include "eventus.h"
#include "eventus_stml.h"
#include "claves_physicae.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
                    s32  kvk;
     constans character* titulus;
} Par;

interior constans Par PARIA[] = {
    {0, "KeyA"}, {1, "KeyS"}, {2, "KeyD"}, {3, "KeyF"}, {4, "KeyH"},
    {5, "KeyG"}, {6, "KeyZ"}, {7, "KeyX"}, {8, "KeyC"}, {9, "KeyV"},
    {11, "KeyB"}, {12, "KeyQ"}, {13, "KeyW"}, {14, "KeyE"},
    {15, "KeyR"}, {16, "KeyY"}, {17, "KeyT"}, {18, "Digit1"},
    {19, "Digit2"}, {20, "Digit3"}, {21, "Digit4"}, {22, "Digit6"},
    {23, "Digit5"}, {24, "Equal"}, {25, "Digit9"}, {26, "Digit7"},
    {27, "Minus"}, {28, "Digit8"}, {29, "Digit0"},
    {30, "BracketRight"}, {31, "KeyO"}, {32, "KeyU"},
    {33, "BracketLeft"}, {34, "KeyI"}, {35, "KeyP"}, {36, "Enter"},
    {37, "KeyL"}, {38, "KeyJ"}, {39, "Quote"}, {40, "KeyK"},
    {41, "Semicolon"}, {42, "Backslash"}, {43, "Comma"},
    {44, "Slash"}, {45, "KeyN"}, {46, "KeyM"}, {47, "Period"},
    {48, "Tab"}, {49, "Space"}, {50, "Backquote"}, {51, "Backspace"},
    {53, "Escape"}, {54, "MetaRight"}, {55, "MetaLeft"},
    {56, "ShiftLeft"}, {57, "CapsLock"}, {58, "AltLeft"},
    {59, "ControlLeft"}, {60, "ShiftRight"}, {61, "AltRight"},
    {62, "ControlRight"}, {96, "F5"}, {97, "F6"}, {98, "F7"},
    {99, "F3"}, {100, "F8"}, {101, "F9"}, {103, "F11"}, {109, "F10"},
    {111, "F12"}, {114, "Insert"}, {115, "Home"}, {116, "PageUp"},
    {117, "Delete"}, {118, "F4"}, {119, "End"}, {120, "F2"},
    {121, "PageDown"}, {122, "F1"}, {123, "ArrowLeft"},
    {124, "ArrowRight"}, {125, "ArrowDown"}, {126, "ArrowUp"}
};
#define PARIUM_NUMERUS ((i32)(magnitudo(PARIA) / magnitudo(PARIA[0])))

s32 principale (vacuum)
{
     Piscina* piscina;
         i32  k;
         b32  visum[CXXVIII];
         b32  omnia_recta    = VERUM;
         b32  omnes_attacti  = VERUM;

    piscina = piscina_generare_dynamicum("probatio_claves_physicae",
        M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I. tabula macOS: paria omnia ---\n");
    CREDO_AEQUALIS_I32 (PARIUM_NUMERUS, LXXXIII);
    memset(visum, ZEPHYRUM, magnitudo(visum));
    per (k = ZEPHYRUM; k < PARIUM_NUMERUS; k++)
    {
        EventusCodex c = claves_codex_ex_macos(PARIA[k].kvk);

        si (strcmp(eventus_codex_titulus(c), PARIA[k].titulus)
            != ZEPHYRUM)
        {
            imprimere("  kVK %u -> %s, exspectatum %s\n",
                (insignatus integer)PARIA[k].kvk,
                eventus_codex_titulus(c),
                PARIA[k].titulus);
            omnia_recta = FALSUM;
        }
        si ((i32)c < CXXVIII)
        {
            visum[c] = VERUM;
        }
    }
    CREDO_VERUM (omnia_recta);

    imprimere("\n--- II. omnis codex attingitur (tabula sine lacuna) ---\n");
    per (k = I; k < (i32)EVENTUS_CODICES_NUMERUS; k++)
    {
        si (!visum[k])
        {
            imprimere("  codex %s nullo kVK attingitur\n",
                eventus_codex_titulus((EventusCodex)k));
            omnes_attacti = FALSUM;
        }
    }
    CREDO_VERUM (omnes_attacti);

    imprimere("\n--- III. ignoti ---\n");
    CREDO_VERUM (claves_codex_ex_macos(52) == EVENTUS_CODEX_IGNOTUS);
    CREDO_VERUM (claves_codex_ex_macos(63) == EVENTUS_CODEX_IGNOTUS);
    CREDO_VERUM (claves_codex_ex_macos(200) == EVENTUS_CODEX_IGNOTUS);
    CREDO_VERUM (claves_codex_ex_macos(-I) == EVENTUS_CODEX_IGNOTUS);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
