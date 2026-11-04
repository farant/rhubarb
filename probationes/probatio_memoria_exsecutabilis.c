/* probatio_memoria_exsecutabilis.c - probationes memoriae
 * exsecutabilis (scribere, sigillare, vocare; rescribere sine codice
 * stalo; locus optatus; furca)
 *
 * Verba manu: movz w0,#42 = 0x52800540; movz w0,#7 = 0x528000E0;
 * movz w0,#9 = 0x52800120; ret = 0xD65F03C0. Functio per memcpy ex
 * vacuum* obtinetur (C89 -pedantic conversionem obiecti in functionem
 * vetat).
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "memoria_exsecutabilis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define VERBUM_RESPONSUM   0x52800540
#define VERBUM_VII    0x528000E0
#define VERBUM_IX     0x52800120
#define VERBUM_REDDE  0xD65F03C0
#define PAGINA        0x4000
#define LOCUS_OPTATUS 0x280000000000UL

nomen s32 (*FunctioNativa)(vacuum);

interior s32
_vocare (
    vacuum* locus)
{
    FunctioNativa functio;

    memcpy(&functio, &locus, magnitudo(functio));
    redde functio();
}

/* scribit 'verbum; ret' ad basim et sigillat */
interior b32
_codicem_scribere (
    MemoriaExsecutabilis* m,
                     i32  verbum)
{
    i32* verba;

    verba = (i32*)memoria_exsecutabilis_basis(m);
    memoria_exsecutabilis_aperire(m);
    verba[0] = verbum;
    verba[1] = VERBUM_REDDE;
    redde memoria_exsecutabilis_sigillare(m, ZEPHYRUM, VIII);
}


    /* ==================================================
     * PROBARE: scribere, sigillare, vocare
     * ================================================== */

interior vacuum
_sectio_vocare (
    CredoContextus* c)
{
    MemoriaExsecutabilis* m;
                  chorda  causa;

    causa.mensura = ZEPHYRUM;
    m = memoria_exsecutabilis_reservare(c->piscina, ZEPHYRUM, PAGINA,
        &causa);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_AEQUALIS_I64 ((i64)memoria_exsecutabilis_mensura(m),
        (i64)PAGINA);
    CREDO_VERUM (_codicem_scribere(m, VERBUM_RESPONSUM));
    CREDO_AEQUALIS_S32 (_vocare(memoria_exsecutabilis_basis(m)),
        XLII);

    /* intervallum extra regionem: FALSUM */
    memoria_exsecutabilis_aperire(m);
    CREDO_FALSUM (memoria_exsecutabilis_sigillare(m, PAGINA, IV));
    memoria_exsecutabilis_aperire(m);
    CREDO_FALSUM (memoria_exsecutabilis_sigillare(m, ZEPHYRUM,
        PAGINA + I));
    memoria_exsecutabilis_aperire(m);
    CREDO_FALSUM (memoria_exsecutabilis_sigillare(m, IV,
        ~(memoriae_index)ZEPHYRUM));
    memoria_exsecutabilis_aperire(m);
    CREDO_VERUM (memoria_exsecutabilis_sigillare(m, ZEPHYRUM,
        PAGINA));
    CREDO_AEQUALIS_S32 (_vocare(memoria_exsecutabilis_basis(m)),
        XLII);
    memoria_exsecutabilis_liberare(m);
}


    /* ==================================================
     * PROBARE: rescribere (nullus codex stalus)
     * ================================================== */

interior vacuum
_sectio_rescribere (
    CredoContextus* c)
{
    MemoriaExsecutabilis* m;
                  chorda  causa;
                     s32  i;
                     s32  errata;

    m = memoria_exsecutabilis_reservare(c->piscina, ZEPHYRUM, PAGINA,
        &causa);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_VERUM (_codicem_scribere(m, VERBUM_RESPONSUM));
    CREDO_AEQUALIS_S32 (_vocare(memoria_exsecutabilis_basis(m)),
        XLII);

    /* eadem sedes, codex novus: omnis vocatio VII */
    CREDO_VERUM (_codicem_scribere(m, VERBUM_VII));
    errata = ZEPHYRUM;
    per (i = ZEPHYRUM; i < 200000; i++)
    {
        si (_vocare(memoria_exsecutabilis_basis(m)) != VII)
        {
            errata++;
        }
    }
    CREDO_AEQUALIS_S32 (errata, ZEPHYRUM);

    /* rescriptiones alternae: quaeque vocatio codicem ultimum videt */
    errata = ZEPHYRUM;
    per (i = ZEPHYRUM; i < 20000; i++)
    {
        b32 par;

        par = (i % II) == ZEPHYRUM;
        _codicem_scribere(m, par ? VERBUM_VII : VERBUM_IX);
        si (_vocare(memoria_exsecutabilis_basis(m)) != (par ? VII : IX))
        {
            errata++;
        }
    }
    si (errata != ZEPHYRUM)
    {
        imprimere("\n  rescriptiones alternae: %d stalae",
            (integer)errata);
    }
    CREDO_AEQUALIS_S32 (errata, ZEPHYRUM);
    memoria_exsecutabilis_liberare(m);
}


    /* ==================================================
     * PROBARE: locus optatus (refusio, nulla sedes mota)
     * ================================================== */

interior vacuum
_sectio_locus (
    CredoContextus* c)
{
    MemoriaExsecutabilis* m;
    MemoriaExsecutabilis* secunda;
                  chorda  causa;

    causa.mensura = ZEPHYRUM;
    m = memoria_exsecutabilis_reservare(c->piscina, LOCUS_OPTATUS,
        PAGINA * IV, &causa);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_AEQUALIS_I64 ((i64)(memoriae_index)
        memoria_exsecutabilis_basis(m), (i64)LOCUS_OPTATUS);
    CREDO_VERUM (_codicem_scribere(m, VERBUM_RESPONSUM));
    CREDO_AEQUALIS_S32 (_vocare(memoria_exsecutabilis_basis(m)),
        XLII);

    /* sedes occupata: refusio cum causa */
    causa.mensura = ZEPHYRUM;
    secunda = memoria_exsecutabilis_reservare(c->piscina,
        LOCUS_OPTATUS, PAGINA, &causa);
    CREDO_NIHIL (secunda);
    CREDO_VERUM (causa.mensura > ZEPHYRUM);

    /* liberata, iterum datur */
    memoria_exsecutabilis_liberare(m);
    secunda = memoria_exsecutabilis_reservare(c->piscina,
        LOCUS_OPTATUS, PAGINA, &causa);
    CREDO_NECESSE_NON_NIHIL (secunda);
    CREDO_AEQUALIS_I64 ((i64)(memoriae_index)
        memoria_exsecutabilis_basis(secunda), (i64)LOCUS_OPTATUS);
    memoria_exsecutabilis_liberare(secunda);

    /* mensura non paginalis: refusio */
    causa.mensura = ZEPHYRUM;
    CREDO_NIHIL (memoria_exsecutabilis_reservare(c->piscina,
        ZEPHYRUM, 0x1000, &causa));
    CREDO_VERUM (causa.mensura > ZEPHYRUM);
    causa.mensura = ZEPHYRUM;
    CREDO_NIHIL (memoria_exsecutabilis_reservare(c->piscina,
        ZEPHYRUM, ZEPHYRUM, &causa));
    CREDO_VERUM (causa.mensura > ZEPHYRUM);
}


    /* ==================================================
     * PROBARE: furca (codex in filio vivit)
     * ================================================== */

interior vacuum
_sectio_furca (
    CredoContextus* c)
{
    MemoriaExsecutabilis* m;
                  chorda  causa;
                   pid_t  filius;
                 integer  status;

    m = memoria_exsecutabilis_reservare(c->piscina, ZEPHYRUM, PAGINA,
        &causa);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_VERUM (_codicem_scribere(m, VERBUM_RESPONSUM));
    filius = fork();
    CREDO_NECESSE (filius >= ZEPHYRUM);
    si (filius == ZEPHYRUM)
    {
        _exit(_vocare(memoria_exsecutabilis_basis(m)));
    }
    status = ZEPHYRUM;
    CREDO_VERUM (waitpid(filius, &status, ZEPHYRUM) == filius);
    CREDO_VERUM (WIFEXITED(status));
    CREDO_AEQUALIS_S32 ((s32)WEXITSTATUS(status), XLII);
    CREDO_AEQUALIS_S32 (_vocare(memoria_exsecutabilis_basis(m)),
        XLII);
    memoria_exsecutabilis_liberare(m);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "scribere sigillare vocare", _sectio_vocare, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "rescribere",    _sectio_rescribere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "locus optatus", _sectio_locus,      NIHIL, NIHIL, NIHIL, NIHIL },
    { "furca",         _sectio_furca,      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("memoria_exsecutabilis", SECTIONES);
}
