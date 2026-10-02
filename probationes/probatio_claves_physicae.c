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

    imprimere("\n--- IV. dispositio US basica (B2b) ---\n");
    {
        constans character* signa = "`-=[]\\;',./ ";
        constans character* tituli[XIV];
                       i32  j;

        tituli[0]  = "Backquote";    tituli[1]  = "Minus";
        tituli[2]  = "Equal";        tituli[3]  = "BracketLeft";
        tituli[4]  = "BracketRight"; tituli[5]  = "Backslash";
        tituli[6]  = "Semicolon";    tituli[7]  = "Quote";
        tituli[8]  = "Comma";        tituli[9]  = "Period";
        tituli[10] = "Slash";        tituli[11] = "Space";
        per (j = ZEPHYRUM; j < XII; j++)
        {
            CREDO_VERUM (strcmp(eventus_codex_titulus(
                claves_codex_ex_littera((s32)signa[j])), tituli[j])
                == ZEPHYRUM);
        }
        per (j = ZEPHYRUM; j < XXVI; j++)
        {
            CREDO_VERUM (claves_codex_ex_littera((s32)('a' + j))
                == (EventusCodex)(EVENTUS_CODEX_LITTERAE + j));
        }
        per (j = ZEPHYRUM; j < X; j++)
        {
            CREDO_VERUM (claves_codex_ex_littera((s32)('0' + j))
                == (EventusCodex)(EVENTUS_CODEX_NUMERI + j));
        }
        /* concordia cum macOS: eadem positio */
        CREDO_VERUM (claves_codex_ex_littera('q')
            == claves_codex_ex_macos(XII));
        CREDO_VERUM (claves_codex_ex_littera(';')
            == claves_codex_ex_macos(XLI));
        CREDO_VERUM (claves_codex_ex_littera('A')
            == EVENTUS_CODEX_IGNOTUS);
        CREDO_VERUM (claves_codex_ex_littera(0x444)
            == EVENTUS_CODEX_IGNOTUS);
    }

    imprimere("\n--- V. kitty: claves functionales ---\n");
    {
                       s32  numeri[XIII];
        constans character* tituli[XIII];
                       i32  j;

        numeri[0]  = XXVII;  tituli[0]  = "Escape";
        numeri[1]  = XIII;   tituli[1]  = "Enter";
        numeri[2]  = IX;     tituli[2]  = "Tab";
        numeri[3]  = CXXVII; tituli[3]  = "Backspace";
        numeri[4]  = 57358;  tituli[4]  = "CapsLock";
        numeri[5]  = 57441;  tituli[5]  = "ShiftLeft";
        numeri[6]  = 57447;  tituli[6]  = "ShiftRight";
        numeri[7]  = 57442;  tituli[7]  = "ControlLeft";
        numeri[8]  = 57448;  tituli[8]  = "ControlRight";
        numeri[9]  = 57443;  tituli[9]  = "AltLeft";
        numeri[10] = 57449;  tituli[10] = "AltRight";
        numeri[11] = 57444;  tituli[11] = "MetaLeft";
        numeri[12] = 57450;  tituli[12] = "MetaRight";
        per (j = ZEPHYRUM; j < XIII; j++)
        {
            CREDO_VERUM (strcmp(eventus_codex_titulus(
                claves_codex_ex_kitty(numeri[j])), tituli[j])
                    == ZEPHYRUM);
        }
        /* runae, F13, tabula numerica: sine codice */
        CREDO_VERUM (claves_codex_ex_kitty(XCVII)
            == EVENTUS_CODEX_IGNOTUS);
        CREDO_VERUM (claves_codex_ex_kitty(57376)
            == EVENTUS_CODEX_IGNOTUS);
        CREDO_VERUM (claves_codex_ex_kitty(57399)
            == EVENTUS_CODEX_IGNOTUS);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
