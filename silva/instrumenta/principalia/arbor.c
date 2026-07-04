/* arbor.c - impressor arborum (dev-time, M2a Chunk A)
 *
 * Usus: arbor '<fons>'        e.g. arbor '2+3*4;'
 *       arbor -f <via>        plagulam legere
 *
 * Parsat fontem grammatica c89 et arborem generum imprimit,
 * ambulatione generica per registrum coctum (nomina generum et
 * locorum ex tabulis - nulla scientia grammaticae in impressore).
 * Oculi barrae manualis M2a: quod grammatica aedificat, hoc vides.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_glr.h"
#include "silva_expandere.h"
#include "silva_commissio.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include <stdio.h>
#include <string.h>

interior character*
_plagulam_legere (Piscina* piscina, constans character* via)
{
    FILE* pl;
    character* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL) redde NIHIL;
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM) { fclose(pl); redde NIHIL; }
    mensura = ftell(pl);
    si (mensura < 0L) { fclose(pl); redde NIHIL; }
    rewind(pl);
    buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL) { fclose(pl); redde NIHIL; }
    si (mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    buffer[mensura] = '\0';
    redde buffer;
}

interior vacuum _valorem_imprimere (SilvaValor valor, i32 gradus);

interior vacuum
_recessum_imprimere (i32 gradus)
{
    i32 i;
    per (i = ZEPHYRUM; i < gradus; i++) imprimere("  ");
}

interior vacuum
_nodum_imprimere (constans SilvaNodus* nodus, i32 gradus)
{
    constans SilvaRegistrumCoctum* tabularium = &SILVA_C89_REGISTRUM;
    constans SilvaTabGenus* genus;
    i32 i;

    si (nodus->genus < ZEPHYRUM
        || nodus->genus >= (s32)tabularium->numerus_generum)
    {
        imprimere("<genus ignotum %d>\n", (int)nodus->genus);
        redde;
    }
    genus = &tabularium->genera[nodus->genus];
    imprimere("%s\n", genus->titulus);

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans SilvaTabLocus* locus =
            &tabularium->loci[genus->loci_offset + i];

        si (nodus->loci[i].genus == SILVA_VALOR_NIHIL) perge;
        _recessum_imprimere(gradus + I);
        imprimere("%s: ", locus->titulus);
        _valorem_imprimere(nodus->loci[i], gradus + I);
    }
}

interior vacuum
_valorem_imprimere (SilvaValor valor, i32 gradus)
{
    commutatio (valor.genus)
    {
    casus SILVA_VALOR_NODUS:
        _nodum_imprimere(valor.datum.nodus, gradus);
        frange;
    casus SILVA_VALOR_TOKEN:
        imprimere("%s \"%.*s\"\n",
            silva_lexema_genus_nomen(valor.datum.token->genus),
            (int)valor.datum.token->valor.mensura,
            (constans character*)valor.datum.token->valor.datum);
        frange;
    casus SILVA_VALOR_LISTA:
    {
        i32 numerus = silva_valor_lista_numerus(valor);
        i32 i;

        imprimere("[%d]\n", (int)numerus);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            _recessum_imprimere(gradus + I);
            imprimere("- ");
            si (elem != NIHIL)
            {
                _valorem_imprimere(*elem, gradus + I);
            }
            alioquin
            {
                imprimere("<NIHIL>\n");
            }
        }
        frange;
    }
    casus SILVA_VALOR_INDEX:
        imprimere("index %d\n", (int)valor.datum.index);
        frange;
    ordinarius:
        imprimere("<nihil>\n");
        frange;
    }
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina;
    constans character* fons;
    SilvaParsura* parsura;
    i32 i;
    i32 numerus;

    si (argc < II)
    {
        fprintf(stderr, "usus: arbor '<fons>' | arbor -f <via>\n");
        redde I;
    }

    piscina = piscina_generare_dynamicum("arbor", 33554432);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "arbor: piscina non generata\n");
        redde I;
    }

    si (argc >= III && strcmp(argv[I], "-f") == ZEPHYRUM)
    {
        fons = _plagulam_legere(piscina, argv[II]);
        si (fons == NIHIL)
        {
            fprintf(stderr, "arbor: plagula non lecta: %s\n", argv[II]);
            redde I;
        }
    }
    alioquin
    {
        fons = argv[I];
    }

    /* saltatio oraculi plena (M2b): typedefs plagulae ipsius
     * registrantur et solvunt - barra manualis */
    parsura = silva_c89_parsare(piscina, "arbor.c", fons,
        (i32)strlen(fons), NIHIL);
    si (parsura == NIHIL || !parsura->successus)
    {
        fprintf(stderr, "arbor: parsatio fracta\n");
        redde I;
    }

    imprimere("segmenta %d  errores %d  fusiones %d  ambigui %d\n\n",
        (int)parsura->numerus_segmentorum,
        (int)parsura->numerus_errorum,
        (int)parsura->fusiones,
        (int)xar_numerus(parsura->commissio->ambigui));

    numerus = silva_valor_lista_numerus(parsura->commissio->radix);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(
            parsura->commissio->radix, i);

        imprimere("[%d] ", (int)i);
        si (elem != NIHIL)
        {
            _valorem_imprimere(*elem, ZEPHYRUM);
        }
    }

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
