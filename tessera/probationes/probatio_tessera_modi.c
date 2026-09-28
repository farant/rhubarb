/* probatio_tessera_modi.c - Lex parium modorum (tessera_modi.h)
 *
 * Structuralis, sine terminali: chordae INTRANDI/EXEUNDI parsantur
 * (series "ESC [ ? N[;N...] h|l") et leges asseruntur:
 *   I   omnis modus intratus (h) relinquitur (l) in EXEUNDI
 *   II  nullus modus relinquitur qui intratus non est
 *   III EXEUNDI ordine INVERSO relinquit (acervus: mus ante scrinium)
 *   IV  modi fundamentales adsunt (1049 scrinium alternum, 1000 mus,
 *       1006 mus SGR)
 * Cursor (?25) exceptus: exitus eum ostendit consulto.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "tessera_modi.h"
#include <stdio.h>
#include <string.h>

#define MODI_MAXIMI XVI
#define MODUS_CURSORIS XXV

/* Modos privatos cum finali data colligere, ordine apparitionis */
interior i32
_modos_colligere (
     constans character* s,
              character  finalis,
                    s32* modi)
{
    i32 n = ZEPHYRUM;
    i32 k = ZEPHYRUM;

    dum (s[k] != '\0')
    {
        si (s[k] == '\033' && s[k + I] == '[' && s[k + II] == '?')
        {
            s32 numeri[MODI_MAXIMI];
            i32 m      = ZEPHYRUM;
            s32 valor  = ZEPHYRUM;
            i32 j;

            k += III;
            dum ((s[k] >= '0' && s[k] <= '9') || s[k] == ';')
            {
                si (s[k] == ';')
                {
                    si (m < MODI_MAXIMI)
                    {
                        numeri[m++] = valor;
                    }
                    valor = ZEPHYRUM;
                }
                alioquin
                {
                    valor = valor * X + (s32)(s[k] - '0');
                }
                k++;
            }
            si (m < MODI_MAXIMI)
            {
                numeri[m++] = valor;
            }
            si (s[k] == finalis)
            {
                per (j = ZEPHYRUM; j < m; j++)
                {
                    si (numeri[j] != MODUS_CURSORIS && n < MODI_MAXIMI)
                    {
                        modi[n++] = numeri[j];
                    }
                }
            }
            si (s[k] != '\0')
            {
                k++;
            }
            perge;
        }
        k++;
    }
    redde n;
}

interior b32
_continet (
    constans s32* modi,
             i32  n,
             s32  modus)
{
    i32 j;

    per (j = ZEPHYRUM; j < n; j++)
    {
        si (modi[j] == modus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
         s32  intrati[MODI_MAXIMI];
         s32  relicti[MODI_MAXIMI];
         s32  spurii[MODI_MAXIMI];
         i32  n_intrati;
         i32  n_relicti;
         i32  j;

    piscina = piscina_generare_dynamicum("probatio_tessera_modi",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    n_intrati = _modos_colligere(INTRANDI, 'h', intrati);
    n_relicti = _modos_colligere(EXEUNDI, 'l', relicti);

    imprimere("\n--- intrati:");
    per (j = ZEPHYRUM; j < n_intrati; j++)
    {
        imprimere(" %d", (integer)intrati[j]);
    }
    imprimere("\n--- relicti:");
    per (j = ZEPHYRUM; j < n_relicti; j++)
    {
        imprimere(" %d", (integer)relicti[j]);
    }
    imprimere("\n");

    /* I: omnis intratus relinquitur */
    per (j = ZEPHYRUM; j < n_intrati; j++)
    {
        si (!_continet(relicti, n_relicti, intrati[j]))
        {
            imprimere("  FRACTA: modus ?%d intratus, numquam relictus\n",
                (integer)intrati[j]);
        }
        CREDO_VERUM (_continet(relicti, n_relicti, intrati[j]));
    }

    /* II: nullus relictus sine introitu */
    per (j = ZEPHYRUM; j < n_relicti; j++)
    {
        si (!_continet(intrati, n_intrati, relicti[j]))
        {
            imprimere("  FRACTA: modus ?%d relictus, numquam intratus\n",
                (integer)relicti[j]);
        }
        CREDO_VERUM (_continet(intrati, n_intrati, relicti[j]));
    }

    /* III: ordo inversus */
    CREDO_AEQUALIS_I32 (n_relicti, n_intrati);
    si (n_relicti == n_intrati)
    {
        per (j = ZEPHYRUM; j < n_intrati; j++)
        {
            CREDO_AEQUALIS_S32 (relicti[j], intrati[n_intrati - I - j]);
        }
    }

    /* EXEUNDI nullum modum INTRAT, INTRANDI nullum RELINQUIT (cursore
     * excepto) */
    CREDO_AEQUALIS_I32 (_modos_colligere(EXEUNDI, 'h', spurii),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_modos_colligere(INTRANDI, 'l', spurii),
        ZEPHYRUM);

    /* IV: modi fundamentales */
    CREDO_VERUM (_continet(intrati, n_intrati, MXLIX));
    CREDO_VERUM (_continet(intrati, n_intrati, M));
    CREDO_VERUM (_continet(intrati, n_intrati, MVI));

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
