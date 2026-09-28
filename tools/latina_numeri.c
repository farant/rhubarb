/* latina_numeri.c - sectionem numerorum Romanorum latina.h generare
 *
 * Fons veritatis UNUS: numerus_romanus_scribere. Sectio inter
 * lineam initii (SIGNUM_INITII) et lineam finis (SIGNUM_FINIS)
 * tota rescribitur: ZEPHYRUM, deinde I - MMMCMXCIX ordine.
 *
 * Usus: latina_numeri <via latina.h> [-scribere]
 *   sine -scribere: comparat - exitus 0 recens, 1 stalum (linea
 *   prima discrepans nominatur), 2 usus/lectio.
 *   -scribere: sectionem in loco rescribit.
 * Porta: tools/generata_probare.sh (V). */

#include "latina.h"
#include "numerus_romanus.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

#define SIGNUM_INITII "/* NUMERI ROMANI - GENERATUM"
#define SIGNUM_FINIS  "/* finis numerorum generatorum */"
#define LATITUDO_NOMINIS XVI

interior chorda
_sectio_generata (
    Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
                                 CXXVIII * M);
                  i32 n;
            character linea[LXIV];

    chorda_aedificator_appendere_literis(aed, SIGNUM_INITII
        " ex numerus_romanus_scribere\n"
        " * (tools/latina_numeri.sh -scribere) - NE MANU MUTES; porta\n"
        " * 'generata' iudicat. Omnes ZEPHYRUM-MMMCMXCIX ordine: numerus\n"
        " * Romanus classicus ad MMMCMXCIX finit - maiores ut expressio\n"
        " * (vinculum ut '* M'): IV * M (4000), IV * MXXIV (4096); vide\n"
        " * numerus_romanus_exprimere. Omne numerale identificator\n"
        " * reservatus est: capita systematis ANTE latina.h includenda\n"
        " * (dns_util.h membra 'MD' et 'MX' habet). */\n");
    sprintf(linea, "#define %-*s %d\n", (integer)LATITUDO_NOMINIS,
            "ZEPHYRUM", ZEPHYRUM);
    chorda_aedificator_appendere_literis(aed, linea);
    per (n = I; n <= MMM + CM + XC + IX; n++)
    {
        chorda c = numerus_romanus_scribere(n, piscina);

        sprintf(linea, "#define %-*.*s %u\n", (integer)LATITUDO_NOMINIS,
                (integer)c.mensura, (constans character*)c.datum, n);
        chorda_aedificator_appendere_literis(aed, linea);
    }
    chorda_aedificator_appendere_literis(aed, SIGNUM_FINIS);
    redde chorda_aedificator_finire(aed);
}

/* Index lineae incipientis cum 'signum' ab 'ab', aut -1 */
interior s32
_lineam_invenire (
                 chorda  textus,
     constans character* signum,
                    i32  ab)
{
    i32 k = ab;
    i32 l = (i32)strlen(signum);

    dum (k + l <= textus.mensura)
    {
        si (   (k == ZEPHYRUM || textus.datum[k - I] == '\n')
            && memcmp(textus.datum + k, signum, (size_t)l) == ZEPHYRUM)
        {
            redde (s32)k;
        }
        k++;
    }
    redde -I;
}

s32
principale (
      integer   argc,
    character** argv)
{
     Piscina* piscina;
      chorda  textus;
      chorda  nova;
      chorda  sectio_vetus;
         s32  initium;
         s32  finis;
         b32  scribere;
         i32  k;

    si (argc < II || argc > III)
    {
        fprintf(stderr, "usus: latina_numeri <latina.h> [-scribere]\n");
        redde II;
    }
    scribere = argc == III && strcmp(argv[II], "-scribere") == ZEPHYRUM;
    piscina = piscina_generare_dynamicum("latina_numeri", M * M);
    textus = filum_legere_totum(argv[I], piscina);
    si (textus.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "latina_numeri: legi nequit: %s\n", argv[I]);
        redde II;
    }
    initium = _lineam_invenire(textus, SIGNUM_INITII, ZEPHYRUM);
    finis = initium < ZEPHYRUM ? -I
            : _lineam_invenire(textus, SIGNUM_FINIS, (i32)initium);
    si (initium < ZEPHYRUM || finis < ZEPHYRUM)
    {
        fprintf(stderr, "latina_numeri: signa sectionis desunt in %s"
                " ('%s' ... '%s')\n", argv[I], SIGNUM_INITII,
                SIGNUM_FINIS);
        redde II;
    }
    finis         += (s32)strlen(SIGNUM_FINIS);
    nova          = _sectio_generata(piscina);
    sectio_vetus  = chorda_sectio(textus, (i32)initium, (i32)finis);

    si (!scribere)
    {
        si (chorda_aequalis(sectio_vetus, nova))
        {
            printf("latina_numeri: %s recens\n", argv[I]);
            redde ZEPHYRUM;
        }
        /* linea prima discrepans */
        per (k = ZEPHYRUM; k < sectio_vetus.mensura && k < nova.mensura
             && sectio_vetus.datum[k] == nova.datum[k]; k++)
        {
        }
        printf("latina_numeri: %s STALUM (octetus %u sectionis) -"
               " ./tools/latina_numeri.sh -scribere\n", argv[I], k);
        redde I;
    }
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            (memoriae_index)(textus.mensura + nova.mensura));

        chorda_aedificator_appendere_chorda(aed,
            chorda_sectio(textus, ZEPHYRUM, (i32)initium));
        chorda_aedificator_appendere_chorda(aed, nova);
        chorda_aedificator_appendere_chorda(aed,
            chorda_sectio(textus, (i32)finis, textus.mensura));
        si (!filum_scribere(argv[I], chorda_aedificator_finire(aed)))
        {
            fprintf(stderr, "latina_numeri: scribi nequit: %s\n",
                argv[I]);
            redde II;
        }
    }
    printf("latina_numeri: %s scriptum (ZEPHYRUM-MMMCMXCIX)\n",
        argv[I]);
    redde ZEPHYRUM;
}
