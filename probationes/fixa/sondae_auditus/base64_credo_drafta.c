#include "postulata_posix.h"
#include "latina.h"
#include "base64.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

/* Codificat n_octeti ex regione mappata (paginae zero); refutatio
 * {NIHIL, 0} exspectatur - aliter _exit(III). */
interior vacuum
_codificare_magnum_refutari (
    i32 n_octeti)
{
     Piscina* p;
      vacuum* regio;
      chorda  codificata;

    regio = mmap(NIHIL, (size_t)n_octeti, PROT_READ,
        MAP_ANON | MAP_PRIVATE, -1, 0);
    si (regio == MAP_FAILED)
    {
        redde;
    }
    p           = piscina_generare_dynamicum("base64_magnum", 65536);
    codificata  = base64_codificare((constans i8*)regio, n_octeti, p);
    si (codificata.datum != NIHIL || codificata.mensura != 0)
    {
        _exit(III);
    }
    piscina_destruere(p);
    munmap(regio, (size_t)n_octeti);
}

/* CREDO_NON_RUIT non sufficit: sub ASan ruina in exitum I
 * convertitur (WIFEXITED verum). Exitum ZEPHYRUM ipsum exigimus. */
interior b32
_filius_refutat (
    i32 n_octeti)
{
    CredoProcessusFructus  cpf;
                      int  status;

    cpf = credo_processus_incipere();
    si (cpf.in_filio)
    {
        _codificare_magnum_refutari(n_octeti);
        credo_processus_filium_finire();
    }
    si (cpf.pid <= 0 || waitpid(cpf.pid, &status, 0) != cpf.pid)
    {
        redde FALSUM;
    }
    redde (b32)(WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

interior vacuum
probatio_codificare_magnum(Piscina* piscina)
{
    (vacuum)piscina;

    printf("--- Probans codificare ingressum magnum (> 3 GiB) ---\n");

    /* longitudo vera 4294967296: ad 0 involvitur, piscina NIHIL reddit */
    CREDO_VERUM(_filius_refutat((i32)0xBFFFFFFEUL));

    /* longitudo vera 4294967300: ad 4 involvitur, alveus transcribitur */
    CREDO_VERUM(_filius_refutat((i32)0xC0000001UL));

    printf("\n");
}

interior vacuum
probatio_longitudo_codificata_extrema(Piscina* piscina)
{
    (vacuum)piscina;

    printf("--- Probans longitudo codificata extrema ---\n");

    /* maxima quae capit: 4 * ceil(0xBFFFFFFD / 3) = 0xFFFFFFFC */
    CREDO_AEQUALIS_I32(base64_longitudo_codificata((i32)0xBFFFFFFDUL),
        (i32)0xFFFFFFFCUL);

    /* longitudo vera i32 excedit: 0 (refutatio), numquam involutio */
    CREDO_AEQUALIS_I32(base64_longitudo_codificata((i32)0xBFFFFFFEUL), 0);
    CREDO_AEQUALIS_I32(base64_longitudo_codificata((i32)0xC0000001UL), 0);
    CREDO_AEQUALIS_I32(base64_longitudo_codificata((i32)0xFFFFFFFCUL), 0);
    CREDO_AEQUALIS_I32(base64_longitudo_codificata((i32)0xFFFFFFFDUL), 0);
    CREDO_AEQUALIS_I32(base64_longitudo_codificata((i32)0xFFFFFFFFUL), 0);

    printf("\n");
}

interior vacuum
probatio_longitudo_decodificata_brevis(Piscina* piscina)
{
    printf("--- Probans longitudo decodificata (ingressus brevis) ---\n");

    /* (mensura / 4) * 3 == 0: padding subtrahi non potest */
    CREDO_AEQUALIS_I32(base64_longitudo_decodificata(
        chorda_ex_literis("=", piscina)), 0);
    CREDO_AEQUALIS_I32(base64_longitudo_decodificata(
        chorda_ex_literis("==", piscina)), 0);
    CREDO_AEQUALIS_I32(base64_longitudo_decodificata(
        chorda_ex_literis("A=", piscina)), 0);
    CREDO_AEQUALIS_I32(base64_longitudo_decodificata(
        chorda_ex_literis("Q==", piscina)), 0);

    printf("\n");
}

interior vacuum
probatio_padding_medius(Piscina* piscina)
{
    Base64Fructus fructus;

    printf("--- Probans padding in quaterno non ultimo ---\n");

    /* RFC 4648 par. 3.3: '=' solum in fine datorum */
    fructus = base64_decodificare(chorda_ex_literis("Zg==Zg==", piscina),
        piscina);
    CREDO_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, 0);

    fructus = base64_decodificare(chorda_ex_literis("Zg==Zm9v", piscina),
        piscina);
    CREDO_NIHIL(fructus.datum);

    fructus = base64_decodificare(chorda_ex_literis("Zm8=Zm9v", piscina),
        piscina);
    CREDO_NIHIL(fructus.datum);

    fructus = base64_decodificare(chorda_ex_literis("AA==AAAA", piscina),
        piscina);
    CREDO_NIHIL(fructus.datum);

    /* imperium: padding finalis adhuc accipitur */
    fructus = base64_decodificare(chorda_ex_literis("Zm9vZg==", piscina),
        piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, IV);
    CREDO_VERUM(memcmp(fructus.datum, "foof", IV) == 0);

    printf("\n");
}

interior vacuum
probatio_padding_discontinuus(Piscina* piscina)
{
    Base64Fructus fructus;

    printf("--- Probans '=' tertio loco sine '=' quarto ---\n");

    fructus = base64_decodificare(chorda_ex_literis("Zg=A", piscina),
        piscina);
    CREDO_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, 0);

    fructus = base64_decodificare(chorda_ex_literis("AA=A", piscina),
        piscina);
    CREDO_NIHIL(fructus.datum);

    fructus = base64_decodificare(chorda_ex_literis("QU=D", piscina),
        piscina);
    CREDO_NIHIL(fructus.datum);

    fructus = base64_decodificare(chorda_ex_literis("Zm9vZg=A", piscina),
        piscina);
    CREDO_NIHIL(fructus.datum);

    printf("\n");
}

interior vacuum
probatio_vacua_successus(Piscina* piscina)
{
               i8  vacuus[I];
           chorda  ingressus;
    Base64Fructus  fructus;

    (vacuum)piscina;

    printf("--- Probans decodificatio vacua (RFC 4648 par. 10) ---\n");

    /* "" -> "": successus, non error - datum non-NIHIL, mensura 0 */
    vacuus[0]          = 0;
    ingressus.datum    = vacuus;
    ingressus.mensura  = 0;
    fructus = base64_decodificare(ingressus, piscina);
    CREDO_NON_NIHIL(fructus.datum);
    CREDO_AEQUALIS_I32(fructus.mensura, 0);

    printf("\n");
}

integer
principale (vacuum)
{
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_base64", LXIV * M);
    credo_aperire(piscina);

    probatio_codificare_magnum(piscina);
    probatio_longitudo_codificata_extrema(piscina);
    probatio_longitudo_decodificata_brevis(piscina);
    probatio_padding_medius(piscina);
    probatio_padding_discontinuus(piscina);
    probatio_vacua_successus(piscina);

    credo_imprimere_compendium();
    piscina_destruere(piscina);
    redde 0;
}
