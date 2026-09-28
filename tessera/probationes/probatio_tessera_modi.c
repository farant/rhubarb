/* probatio_tessera_modi.c - Lex parium modorum (tessera_modi.h)
 *
 * Structuralis, sine terminali: chordae INTRANDI/EXEUNDI parsantur
 * (series "ESC [ ? N[;N...] h|l") et leges asseruntur:
 *   I   omnis modus intratus (h) relinquitur (l) in EXEUNDI
 *   II  nullus modus relinquitur qui intratus non est
 *   III EXEUNDI ordine INVERSO relinquit (acervus: mus ante scrinium)
 *   IV  modi fundamentales adsunt (1049 scrinium alternum, 1000 mus,
 *       1002 tractus, 1006 mus SGR)
 *   V   modi PER QUADRUM (QUADRUM_INITIUM, 2026) numquam in
 *       INTRANDI, semper in EXEUNDI et PRIMI (ruina quadrum apertum
 *       non relinquit); lex II eos excipit
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
         s32  per_quadrum[MODI_MAXIMI];
         s32  relicti_ordinis[MODI_MAXIMI];
         i32  n_intrati;
         i32  n_relicti;
         i32  n_quadri;
         i32  n_ordinis;
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

    /* modi PER QUADRUM (QUADRUM_INITIUM): non in INTRANDI, sed in
     * EXEUNDI relinquendi (ruina inter initium et finem quadri) */
    n_quadri = _modos_colligere(QUADRUM_INITIUM, 'h', per_quadrum);
    CREDO_MAIOR_I32 (n_quadri, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_modos_colligere(QUADRUM_FINIS, 'l', spurii),
        n_quadri);
    n_ordinis = ZEPHYRUM;
    per (j = ZEPHYRUM; j < n_relicti; j++)
    {
        si (!_continet(per_quadrum, n_quadri, relicti[j]))
        {
            relicti_ordinis[n_ordinis++] = relicti[j];
        }
    }

    /* II: nullus relictus sine introitu (modis per quadrum exceptis) */
    per (j = ZEPHYRUM; j < n_relicti; j++)
    {
        b32 legitimus = _continet(intrati, n_intrati, relicti[j])
                     || _continet(per_quadrum, n_quadri, relicti[j]);

        si (!legitimus)
        {
            imprimere("  FRACTA: modus ?%d relictus, numquam intratus\n",
                (integer)relicti[j]);
        }
        CREDO_VERUM (legitimus);
    }

    /* III: ordo inversus (modi intrandi soli) */
    CREDO_AEQUALIS_I32 (n_ordinis, n_intrati);
    si (n_ordinis == n_intrati)
    {
        per (j = ZEPHYRUM; j < n_intrati; j++)
        {
            CREDO_AEQUALIS_S32 (relicti_ordinis[j],
                intrati[n_intrati - I - j]);
        }
    }

    /* V: omnis modus per quadrum in EXEUNDI relinquitur, et PRIMUS
     * (quadrum apertum ante ceteros modos clauditur); numquam in
     * INTRANDI */
    per (j = ZEPHYRUM; j < n_quadri; j++)
    {
        si (!_continet(relicti, n_relicti, per_quadrum[j]))
        {
            imprimere("  FRACTA: modus per quadrum ?%d in EXEUNDI deest "
                "(ruina quadrum apertum relinqueret)\n",
                (integer)per_quadrum[j]);
        }
        CREDO_VERUM (_continet(relicti, n_relicti, per_quadrum[j]));
        CREDO_FALSUM (_continet(intrati, n_intrati, per_quadrum[j]));
        si (j < n_relicti)
        {
            CREDO_VERUM (_continet(per_quadrum, n_quadri, relicti[j]));
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
    CREDO_VERUM (_continet(intrati, n_intrati, MII));
    CREDO_VERUM (_continet(intrati, n_intrati, MVI));

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
