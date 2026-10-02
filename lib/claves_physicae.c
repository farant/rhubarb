/* claves_physicae.c - Vide claves_physicae.h
 *
 * Tabula macOS: kVK_* (Carbon HIToolbox Events.h, constantes Apple
 * fixae), cum convertere_clavem fenestrae collata. 114 = kVK_Help =
 * positio Insert claviaturae PC. Fn (63) et 52 (vacuus) -> IGNOTUS. */

#include "claves_physicae.h"

EventusCodex
claves_codex_ex_macos (
    s32 codex_virtualis)
{
    commutatio (codex_virtualis)
    {
        casus ZEPHYRUM: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE); /* KeyA */
        casus I: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XVIII); /* KeyS */
        casus II: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + III); /* KeyD */
        casus III: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + V); /* KeyF */
        casus IV: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + VII); /* KeyH */
        casus V: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + VI); /* KeyG */
        casus VI: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XXV); /* KeyZ */
        casus VII: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XXIII); /* KeyX */
        casus VIII: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + II); /* KeyC */
        casus IX: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XXI); /* KeyV */
        casus XI: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + I); /* KeyB */
        casus XII: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XVI); /* KeyQ */
        casus XIII: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XXII); /* KeyW */
        casus XIV: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + IV); /* KeyE */
        casus XV: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XVII); /* KeyR */
        casus XVI: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XXIV); /* KeyY */
        casus XVII: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XIX); /* KeyT */
        casus XVIII: redde (EventusCodex)(EVENTUS_CODEX_NUMERI + I); /* Digit1 */
        casus XIX: redde (EventusCodex)(EVENTUS_CODEX_NUMERI + II); /* Digit2 */
        casus XX: redde (EventusCodex)(EVENTUS_CODEX_NUMERI + III); /* Digit3 */
        casus XXI: redde (EventusCodex)(EVENTUS_CODEX_NUMERI + IV); /* Digit4 */
        casus XXII: redde (EventusCodex)(EVENTUS_CODEX_NUMERI + VI); /* Digit6 */
        casus XXIII: redde (EventusCodex)(EVENTUS_CODEX_NUMERI + V); /* Digit5 */
        casus XXIV: redde (EventusCodex)(EVENTUS_CODEX_AEQUALE);
        casus XXV: redde (EventusCodex)(EVENTUS_CODEX_NUMERI + IX); /* Digit9 */
        casus XXVI: redde (EventusCodex)(EVENTUS_CODEX_NUMERI + VII); /* Digit7 */
        casus XXVII: redde (EventusCodex)(EVENTUS_CODEX_MINUS);
        casus XXVIII: redde (EventusCodex)(EVENTUS_CODEX_NUMERI + VIII); /* Digit8 */
        casus XXIX: redde (EventusCodex)(EVENTUS_CODEX_NUMERI); /* Digit0 */
        casus XXX: redde (EventusCodex)(EVENTUS_CODEX_UNCUS_DEXTER);
        casus XXXI: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XIV); /* KeyO */
        casus XXXII: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XX); /* KeyU */
        casus XXXIII: redde (EventusCodex)(EVENTUS_CODEX_UNCUS_SINISTER);
        casus XXXIV: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE
                         + VIII); /* KeyI */
        casus XXXV: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XV); /* KeyP */
        casus XXXVI: redde (EventusCodex)(EVENTUS_CODEX_REDITUS);
        casus XXXVII: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XI); /* KeyL */
        casus XXXVIII: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE
                           + IX); /* KeyJ */
        casus XXXIX: redde (EventusCodex)(EVENTUS_CODEX_APOSTROPHUS);
        casus XL: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + X); /* KeyK */
        casus XLI: redde (EventusCodex)(EVENTUS_CODEX_PUNCTUM_VIRGULA);
        casus XLII: redde (EventusCodex)(EVENTUS_CODEX_VIRGULA_INVERSA);
        casus XLIII: redde (EventusCodex)(EVENTUS_CODEX_VIRGULA);
        casus XLIV: redde (EventusCodex)(EVENTUS_CODEX_VIRGULA_OBLIQUA);
        casus XLV: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XIII); /* KeyN */
        casus XLVI: redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + XII); /* KeyM */
        casus XLVII: redde (EventusCodex)(EVENTUS_CODEX_PUNCTUM);
        casus XLVIII: redde (EventusCodex)(EVENTUS_CODEX_TABULA);
        casus XLIX: redde (EventusCodex)(EVENTUS_CODEX_SPATIUM);
        casus L: redde (EventusCodex)(EVENTUS_CODEX_GRAVIS);
        casus LI: redde (EventusCodex)(EVENTUS_CODEX_RETRORSUM);
        casus LIII: redde (EventusCodex)(EVENTUS_CODEX_EFFUGIUM);
        casus LIV: redde (EventusCodex)(EVENTUS_CODEX_SUPER_DEXTRUM);
        casus LV: redde (EventusCodex)(EVENTUS_CODEX_SUPER_SINISTRUM);
        casus LVI: redde (EventusCodex)(EVENTUS_CODEX_MAIUSCULA_SINISTRA);
        casus LVII: redde (EventusCodex)(EVENTUS_CODEX_SERA_MAIUSCULARUM);
        casus LVIII: redde (EventusCodex)(EVENTUS_CODEX_ALTERUM_SINISTRUM);
        casus LIX: redde (EventusCodex)(EVENTUS_CODEX_IMPERIUM_SINISTRUM);
        casus LX: redde (EventusCodex)(EVENTUS_CODEX_MAIUSCULA_DEXTRA);
        casus LXI: redde (EventusCodex)(EVENTUS_CODEX_ALTERUM_DEXTRUM);
        casus LXII: redde (EventusCodex)(EVENTUS_CODEX_IMPERIUM_DEXTRUM);
        casus XCVI: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES + IV); /* F5 */
        casus XCVII: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES + V); /* F6 */
        casus XCVIII: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES
                          + VI); /* F7 */
        casus XCIX: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES + II); /* F3 */
        casus C: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES + VII); /* F8 */
        casus CI: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES + VIII); /* F9 */
        casus CIII: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES + X); /* F11 */
        casus CIX: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES + IX); /* F10 */
        casus CXI: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES + XI); /* F12 */
        casus CXIV: redde (EventusCodex)(EVENTUS_CODEX_INSERERE);
        casus CXV: redde (EventusCodex)(EVENTUS_CODEX_DOMUS);
        casus CXVI: redde (EventusCodex)(EVENTUS_CODEX_PAGINA_SURSUM);
        casus CXVII: redde (EventusCodex)(EVENTUS_CODEX_DELERE);
        casus CXVIII: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES
                          + III); /* F4 */
        casus CXIX: redde (EventusCodex)(EVENTUS_CODEX_FINIS);
        casus CXX: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES + I); /* F2 */
        casus CXXI: redde (EventusCodex)(EVENTUS_CODEX_PAGINA_DEORSUM);
        casus CXXII: redde (EventusCodex)(EVENTUS_CODEX_FUNCTIONES); /* F1 */
        casus CXXIII: redde (EventusCodex)(EVENTUS_CODEX_SAGITTA_SINISTRA);
        casus CXXIV: redde (EventusCodex)(EVENTUS_CODEX_SAGITTA_DEXTRA);
        casus CXXV: redde (EventusCodex)(EVENTUS_CODEX_SAGITTA_DEORSUM);
        casus CXXVI: redde (EventusCodex)(EVENTUS_CODEX_SAGITTA_SURSUM);
        ordinarius:
            redde EVENTUS_CODEX_IGNOTUS;
    }
}

EventusCodex
claves_codex_ex_littera (
    s32 runa)
{
    si (runa >= 'a' && runa <= 'z')
    {
        redde (EventusCodex)(EVENTUS_CODEX_LITTERAE + (runa - 'a'));
    }
    si (runa >= '0' && runa <= '9')
    {
        redde (EventusCodex)(EVENTUS_CODEX_NUMERI + (runa - '0'));
    }
    commutatio (runa)
    {
        casus '`':  redde EVENTUS_CODEX_GRAVIS;
        casus '-':  redde EVENTUS_CODEX_MINUS;
        casus '=':  redde EVENTUS_CODEX_AEQUALE;
        casus '[':  redde EVENTUS_CODEX_UNCUS_SINISTER;
        casus ']':  redde EVENTUS_CODEX_UNCUS_DEXTER;
        casus '\\': redde EVENTUS_CODEX_VIRGULA_INVERSA;
        casus ';':  redde EVENTUS_CODEX_PUNCTUM_VIRGULA;
        casus '\'': redde EVENTUS_CODEX_APOSTROPHUS;
        casus ',':  redde EVENTUS_CODEX_VIRGULA;
        casus '.':  redde EVENTUS_CODEX_PUNCTUM;
        casus '/':  redde EVENTUS_CODEX_VIRGULA_OBLIQUA;
        casus ' ':  redde EVENTUS_CODEX_SPATIUM;
        ordinarius: redde EVENTUS_CODEX_IGNOTUS;
    }
}

/* kitty 'functional key definitions' (kitty doc keyboard-protocol;
 * Ghostty src/input/kitty.zig, MIT, pin 12752b2) */
EventusCodex
claves_codex_ex_kitty (
    s32 numerus)
{
    commutatio (numerus)
    {
        casus XXVII:  redde EVENTUS_CODEX_EFFUGIUM;
        casus XIII:   redde EVENTUS_CODEX_REDITUS;
        casus IX:     redde EVENTUS_CODEX_TABULA;
        casus CXXVII: redde EVENTUS_CODEX_RETRORSUM;
        casus 57358:  redde EVENTUS_CODEX_SERA_MAIUSCULARUM;
        casus 57441:  redde EVENTUS_CODEX_MAIUSCULA_SINISTRA;
        casus 57447:  redde EVENTUS_CODEX_MAIUSCULA_DEXTRA;
        casus 57442:  redde EVENTUS_CODEX_IMPERIUM_SINISTRUM;
        casus 57448:  redde EVENTUS_CODEX_IMPERIUM_DEXTRUM;
        casus 57443:  redde EVENTUS_CODEX_ALTERUM_SINISTRUM;
        casus 57449:  redde EVENTUS_CODEX_ALTERUM_DEXTRUM;
        casus 57444:  redde EVENTUS_CODEX_SUPER_SINISTRUM;
        casus 57450:  redde EVENTUS_CODEX_SUPER_DEXTRUM;
        ordinarius:   redde EVENTUS_CODEX_IGNOTUS;
    }
}
