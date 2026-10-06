/* probatio_ludus_tessera_pons.c - octeti modorum et pons super
 * terminalis (A1)
 *
 * I.   Compositio exacta (MUS + GLUTINUM): intrandi = ?1049h + modi
 *      rivi; exeundi = ?2026l + exitus rivi inversi + ?1049l 0m ?25h.
 * II.  Lex parium super modos OMNES: omne ?Nh suum ?Nl habet, kitty
 *      >31u suum <u; ?2026l primum; ?1049l post exitum rivi ultimum;
 *      capacitas tenet; iterum compositum (modi iam intrati) FALSUM.
 * III. Pons: fossa 1 in plagulam temporariam versa - intrare/egredi
 *      nihil scribunt, scribere octetos ad fossam 1 fert, legere -1,
 *      amplitudo = terminalis_amplitudo, resumptum NIHIL.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "ludus_tessera_pons.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

interior b32
_aequat (
           constans i8* octeti,
                   i32  mensura,
    constans character* exspectatum)
{
    b32 bona = (mensura == (i32)strlen(exspectatum)
        && memcmp(octeti, exspectatum, (memoriae_index)mensura)
        == ZEPHYRUM) ? VERUM : FALSUM;

    si (!bona)
    {
        imprimere("  FRACTA: '%.*s' != '%s'\n", (int)mensura,
            (constans character*)octeti, exspectatum);
    }
    redde bona;
}

/* Positio primae occurrentiae literarum, aut -1 */
interior s32
_positio (
           constans i8* octeti,
                   i32  mensura,
    constans character* literae)
{
    i32 n = (i32)strlen(literae);
    i32 k;

    per (k = ZEPHYRUM; k + n <= mensura; k++)
    {
        si (memcmp(octeti + k, literae, (memoriae_index)n) == ZEPHYRUM)
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* Omne "ESC[?Nh" in intrandis suum "ESC[?Nl" in exeundis habet */
interior b32
_paria (
    constans i8* intrandi,
            i32  mensura_intrandi,
    constans i8* exeundi,
            i32  mensura_exeundi)
{
    i32 k;

    per (k = ZEPHYRUM; k + III < mensura_intrandi; k++)
    {
        si (   intrandi[k]      == 0x1B && intrandi[k + I] == '['
            && intrandi[k + II] == '?')
        {
            character par[XVI];
                  i32 j = k + III;
                  i32 n = ZEPHYRUM;

            dum (   j < mensura_intrandi && intrandi[j] >= '0'
                 && intrandi[j] <= '9' && n < X)
            {
                par[III + n] = (character)intrandi[j];
                j++;
                n++;
            }
            si (j >= mensura_intrandi || intrandi[j] != 'h')
            {
                perge;
            }
            par[ZEPHYRUM]  = (character)0x1B;
            par[I]         = '[';
            par[II]        = '?';
            par[III + n]   = 'l';
            par[IV + n]    = '\0';
            si (_positio(exeundi, mensura_exeundi, par) < ZEPHYRUM)
            {
                imprimere("  FRACTA: '%s' sine pari\n", par + I);
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
                 i8  intrandi[LUDUS_TESSERA_MODI_MAXIMI];
                 i8  exeundi[LUDUS_TESSERA_MODI_MAXIMI];
                i32  mi;
                i32  me;

    piscina = piscina_generare_dynamicum("probatio_ludus_tessera_pons",
        262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I. compositio exacta (MUS + GLUTINUM) ---\n");
    {
        RivusTerminalis* rivus = rivus_creare(piscina, VI, VIII);

        CREDO_VERUM (ludus_tessera_modos_componere(rivus,
            RIVUS_MODUS_MUS | RIVUS_MODUS_GLUTINUM, intrandi, &mi,
            exeundi, &me));
        CREDO_VERUM (_aequat(intrandi, mi,
            "\033[?1049h\033[?1000h\033[?1002h\033[?1006h\033[?2004h"));
        CREDO_VERUM (_aequat(exeundi, me,
            "\033[?2026l\033[?2004l\033[?1006l\033[?1002l\033[?1000l"
            "\033[?1049l\033[0m\033[?25h"));
        /* iterum: modi iam intrati */
        CREDO_FALSUM (ludus_tessera_modos_componere(rivus,
            RIVUS_MODUS_MUS, intrandi, &mi, exeundi, &me));
        CREDO_FALSUM (ludus_tessera_modos_componere(NIHIL,
            RIVUS_MODUS_MUS, intrandi, &mi, exeundi, &me));
    }

    imprimere("\n--- II. lex parium, modi omnes ---\n");
    {
        RivusTerminalis* rivus = rivus_creare(piscina, VI, VIII);
                    s32  p_mille_quadraginta_novem;

        CREDO_VERUM (ludus_tessera_modos_componere(rivus,
            RIVUS_MODUS_MUS | RIVUS_MODUS_SUPER | RIVUS_MODUS_GLUTINUM
            | RIVUS_MODUS_FOCUS | RIVUS_MODUS_KITTY, intrandi, &mi,
            exeundi, &me));
        CREDO_VERUM (mi <= LUDUS_TESSERA_MODI_MAXIMI);
        CREDO_VERUM (me <= LUDUS_TESSERA_MODI_MAXIMI);
        CREDO_VERUM (_paria(intrandi, mi, exeundi, me));
        CREDO_VERUM (_positio(intrandi, mi, "\033[>31u") >= ZEPHYRUM);
        CREDO_VERUM (_positio(exeundi, me, "\033[<u") >= ZEPHYRUM);
        /* ordo exeundi: ?2026l primum; ?1049l post omnes exitus rivi */
        CREDO_AEQUALIS_S32 (_positio(exeundi, me, "\033[?2026l"),
            ZEPHYRUM);
        p_mille_quadraginta_novem = _positio(exeundi, me,
            "\033[?1049l");
        CREDO_VERUM (p_mille_quadraginta_novem
            > _positio(exeundi, me, "\033[?1000l"));
        CREDO_VERUM (p_mille_quadraginta_novem
            > _positio(exeundi, me, "\033[<u"));
        CREDO_VERUM (p_mille_quadraginta_novem
            > _positio(exeundi, me, "\033[?1004l"));
        CREDO_VERUM (_positio(intrandi, mi, "\033[?1049h") == ZEPHYRUM);
    }

    imprimere("\n--- III. pons super terminalis ---\n");
    {
                TesseraPons pons;
                  character via[] = "/tmp/ludus_tessera_ponsXXXXXX";
                        s32 fossa;
                        s32 servata;
                         i8 lecti[XVI];
                        i32 latitudo = ZEPHYRUM;
                        i32 altitudo = ZEPHYRUM;
        TerminalisAmplitudo amplitudo;
                        s32 n;
                        b32 bona_intrare;
                        b32 bona_egredi;
                        b32 bona_scribere;
                        b32 bona_amplitudo;
                        b32 bona_terminalis;

        ludus_tessera_pontem_initiare(&pons);
        CREDO_VERUM (pons.datum == NIHIL);
        CREDO_VERUM (pons.resumptum == NIHIL);
        CREDO_AEQUALIS_S32 (pons.legere(pons.datum, lecti, XVI,
            ZEPHYRUM), -I);

        fflush(stdout);
        fossa    = (s32)mkstemp(via);
        servata  = (s32)dup(I);
        CREDO_VERUM (fossa >= ZEPHYRUM && servata >= ZEPHYRUM);
        /* intra fenestram redirectionis NULLA assertio: credo in
         * stdout scribit et in plagulam caderet - eventa in locales,
         * assertiones post restitutionem */
        (vacuum)dup2(fossa, I);
        bona_intrare  = pons.intrare(pons.datum);
        bona_egredi   = pons.egredi(pons.datum);
        bona_scribere   = pons.scribere(pons.datum,
            (constans i8*)"abc", III);
        bona_amplitudo  = pons.amplitudo(pons.datum, &latitudo,
            &altitudo);
        bona_terminalis = terminalis_amplitudo(&amplitudo);
        (vacuum)dup2(servata, I);
        (vacuum)close(servata);
        CREDO_VERUM (bona_intrare);
        CREDO_VERUM (bona_egredi);
        CREDO_VERUM (bona_scribere);
        CREDO_VERUM (bona_amplitudo);
        CREDO_VERUM (bona_terminalis);

        (vacuum)lseek(fossa, ZEPHYRUM, SEEK_SET);
        n = (s32)read(fossa, lecti, XVI);
        (vacuum)close(fossa);
        (vacuum)unlink(via);
        /* intrare/egredi nihil; scribere solum "abc" */
        CREDO_VERUM (_aequat(lecti, (i32)(n > ZEPHYRUM ? n : ZEPHYRUM),
            "abc"));
        CREDO_AEQUALIS_I32 (latitudo, (i32)amplitudo.columnae);
        CREDO_AEQUALIS_I32 (altitudo, (i32)amplitudo.lineae);
        CREDO_VERUM (latitudo > ZEPHYRUM && altitudo > ZEPHYRUM);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
