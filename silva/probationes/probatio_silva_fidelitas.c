/* probatio_silva_fidelitas.c - Fidelitas octetorum trans corpus laminarum
 *
 * Currit oraculum emittere(lexare(x)) == x super OMNES laminas in
 * probationes/fixa/roundtrip/ - corpus commune generationum priorum
 * (78+ plagulae; quaeque cicatrix defectus arbor v1/v2 aut fons verus
 * rhubarb: latina.h, piscina.h, cursor.c, base64.c, utf8.c ...).
 * Lexator silvae has probationes TRES phases ante scribere accipit.
 *
 * Radix repositorii per RHUBARB_RADIX (compile_probationes.sh eam
 * praebet); defaltum ".." pro cursu manuali ex silva/.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>  /* probatio solum - macOS/POSIX */

#define VIA_MAXIMA 1024

/* Legere plagulam totam; reddit NIHIL in defectu */
interior i8*
_plagulam_legere (Piscina* piscina, constans character* via, i32* mensura_out)
{
    FILE* pl;
    i8* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);

    buffer = (i8*)piscina_allocare(piscina, (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);

    *mensura_out = (i32)mensura;
    redde buffer;
}

/* Probare fidelitatem unius plagulae: emittere(lexare(x)) == x */
interior b32
_fidelitatem_probare (constans character* via)
{
    Piscina* piscina;
    i8* fons;
    Xar* lexemata;
    chorda emissa;
    i32 mensura;
    b32 fidelis;

    piscina = piscina_generare_dynamicum("fidelitas_plagulae", 8388608);
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }

    fons = _plagulam_legere(piscina, via, &mensura);
    si (fons == NIHIL)
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }

    lexemata = silva_lexare(piscina, (constans character*)fons, mensura,
        ZEPHYRUM);
    emissa = silva_lexemata_emittere(piscina, lexemata);

    fidelis = (emissa.mensura == mensura
        && (mensura == ZEPHYRUM
            || memcmp(emissa.datum, fons, (memoriae_index)mensura)
                == ZEPHYRUM)) ? VERUM : FALSUM;

    piscina_destruere(piscina);
    redde fidelis;
}

interior b32
_est_c_vel_h (constans character* titulus)
{
    memoriae_index m;

    m = strlen(titulus);
    si (m < III)
    {
        redde FALSUM;
    }
    si (titulus[m - II] == '.'
        && (titulus[m - I] == 'c' || titulus[m - I] == 'h'))
    {
        redde VERUM;
    }
    redde FALSUM;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    constans character* radix;
    character via_corporis[VIA_MAXIMA];
    character via_plagulae[VIA_MAXIMA];
    DIR* corpus;
    structura dirent* introitus;
    i32 numerus;
    i32 fideles;

    piscina = piscina_generare_dynamicum("probatio_silva_fidelitas", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = "..";
    }
    sprintf(via_corporis, "%s/probationes/fixa/roundtrip", radix);

    imprimere("\n--- Probans fidelitatem corporis laminarum ---\n");
    imprimere("  corpus: %s\n", via_corporis);

    numerus = ZEPHYRUM;
    fideles = ZEPHYRUM;
    corpus = opendir(via_corporis);
    si (corpus == NIHIL)
    {
        imprimere("FRACTA: corpus non apertum: %s\n", via_corporis);
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }

    dum ((introitus = readdir(corpus)) != NIHIL)
    {
        b32 fidelis;

        si (!_est_c_vel_h(introitus->d_name))
        {
            perge;
        }
        sprintf(via_plagulae, "%s/%s", via_corporis, introitus->d_name);

        fidelis = _fidelitatem_probare(via_plagulae);
        numerus++;
        si (fidelis)
        {
            fideles++;
        }
        alioquin
        {
            imprimere("  INFIDELIS: %s\n", introitus->d_name);
        }
        CREDO_VERUM (fidelis);
    }
    closedir(corpus);

    imprimere("  plagulae: %d, fideles: %d\n", numerus, fideles);

    /* corpus integrum inventum (LXXVIII cum scriptum; crescere licet) */
    CREDO_VERUM (numerus >= LXXVIII);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
