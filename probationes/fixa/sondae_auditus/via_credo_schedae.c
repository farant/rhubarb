#include "postulata_posix.h"
#include "latina.h"
#include "via.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

s32 principale (vacuum)
{
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_via", IV * MXXIV);
    credo_aperire(piscina);

    /* A ======================================================
     * PROBARE: via_normalizare - ".." iterati in via relativa
     * ======================================================== */
    {
        imprimere("\n--- Probans via_normalizare (.. iterati) ---\n");

        CREDO_CHORDA_AEQUALIS_LITERIS(via_normalizare(
            chorda_ex_literis("../..", piscina), piscina), "../..");
        CREDO_CHORDA_AEQUALIS_LITERIS(via_normalizare(
            chorda_ex_literis("../../a", piscina), piscina), "../../a");
        CREDO_CHORDA_AEQUALIS_LITERIS(via_normalizare(
            chorda_ex_literis("../../..", piscina), piscina), "../../..");
        CREDO_CHORDA_AEQUALIS_LITERIS(via_normalizare(
            chorda_ex_literis("x/../../..", piscina), piscina), "../..");
        CREDO_CHORDA_AEQUALIS_LITERIS(via_normalizare(
            chorda_ex_literis("a/../../../b", piscina), piscina),
            "../../b");
        /* ".." servatum post partem normalem: "../x/.." -> ".." */
        CREDO_CHORDA_AEQUALIS_LITERIS(via_normalizare(
            chorda_ex_literis("../x/..", piscina), piscina), "..");
    }

    /* B ======================================================
     * PROBARE: via_existit - octetus nullus intra viam
     * ======================================================== */
    {
        character  buf[XVI];
           chorda  via_nul;

        imprimere("\n--- Probans via_existit (octetus nullus) ---\n");

        /* "/" semper existit; "/\0nusquam_zzz" numquam */
        memcpy(buf, "/\0nusquam_zzz", XIII);
        via_nul.datum   = (i8*)buf;
        via_nul.mensura = XIII;
        CREDO_FALSUM(via_existit(via_nul));
    }

    /* C ======================================================
     * PROBARE: via_extensio / via_nomen_radix - dotfile cum
     * extensione
     * ======================================================== */
    {
        imprimere("\n--- Probans via_extensio (dotfile cum extensione) ---\n");

        CREDO_CHORDA_AEQUALIS_LITERIS(via_extensio(
            chorda_ex_literis(".bashrc.bak", piscina), piscina), ".bak");
        CREDO_CHORDA_AEQUALIS_LITERIS(via_nomen_radix(
            chorda_ex_literis(".bashrc.bak", piscina), piscina),
            ".bashrc");
        CREDO_CHORDA_AEQUALIS_LITERIS(via_extensio(
            chorda_ex_literis("a/.b.c", piscina), piscina), ".c");
        CREDO_CHORDA_AEQUALIS_LITERIS(via_nomen_radix(
            chorda_ex_literis("a/.b.c", piscina), piscina), ".b");
        /* dotfile purum manet sine extensione */
        CREDO_CHORDA_VACUA(via_extensio(
            chorda_ex_literis(".bashrc", piscina), piscina));
    }

    /* D ======================================================
     * PROBARE: via_absoluta - directorium currens deletum
     * ======================================================== */
    {
        character  origo[IV * MXXIV];
        character  modulus[XXXII];
        character* basis;
           chorda  fructus;

        imprimere("\n--- Probans via_absoluta (cwd deletum) ---\n");

        CREDO_NECESSE_NON_NIHIL(getcwd(origo, magnitudo(origo)));
        memcpy(modulus, "/tmp/via_cwd_XXXXXX", XX);
        basis = mkdtemp(modulus);
        CREDO_NECESSE_NON_NIHIL(basis);
        CREDO_NECESSE(chdir(basis) == ZEPHYRUM);
        CREDO_NECESSE(rmdir(basis) == ZEPHYRUM);

        fructus = via_absoluta(chorda_ex_literis("foo/../bar", piscina),
            piscina);

        CREDO_NECESSE(chdir(origo) == ZEPHYRUM);

        /* aut via absoluta aut error clarus (mensura 0) - numquam
         * via relativa reddita quasi successus */
        CREDO_VERUM(   fructus.mensura == ZEPHYRUM
                    || via_est_absoluta(fructus));
    }

    /* E ======================================================
     * PROBARE: via_directorium_currens - cwd longior 4096
     * ======================================================== */
    {
        character  origo[IV * MXXIV];
        character  modulus[XXXII];
        character  segmentum[CCI];
        character* basis;
              s32  profunditas;
           chorda  cwd;

        imprimere("\n--- Probans via_directorium_currens (cwd longus) ---\n");

        CREDO_NECESSE_NON_NIHIL(getcwd(origo, magnitudo(origo)));
        memcpy(modulus, "/tmp/via_longa_XXXXXX", XXII);
        basis = mkdtemp(modulus);
        CREDO_NECESSE_NON_NIHIL(basis);
        CREDO_NECESSE(chdir(basis) == ZEPHYRUM);
        memset(segmentum, 'd', CC);
        segmentum[CC] = '\0';
        per (profunditas = ZEPHYRUM; profunditas < XXV; profunditas++)
        {
            CREDO_NECESSE(mkdir(segmentum, 0755) == ZEPHYRUM);
            CREDO_NECESSE(chdir(segmentum) == ZEPHYRUM);
        }

        cwd = via_directorium_currens(piscina);

        per (profunditas = ZEPHYRUM; profunditas < XXV; profunditas++)
        {
            CREDO_NECESSE(chdir("..") == ZEPHYRUM);
            CREDO_NECESSE(rmdir(segmentum) == ZEPHYRUM);
        }
        CREDO_NECESSE(chdir(origo) == ZEPHYRUM);
        CREDO_NECESSE(rmdir(basis) == ZEPHYRUM);

        /* ~25 * 201 + basis > 5000 octeti */
        CREDO_MAIOR_I32(cwd.mensura, (i32)(V * M));
        CREDO_VERUM(via_est_absoluta(cwd));
    }

    /* F ======================================================
     * PROBARE: extractio - fructus ex piscina, non sectio
     * ingressus
     * ======================================================== */
    {
        chorda ingressus;
        chorda dir;
        chorda nom;
        chorda rad;
        chorda ext;

        imprimere("\n--- Probans extractio (fructus non aliat) ---\n");

        ingressus = chorda_ex_literis("/usr/lib/foo.txt", piscina);
        dir = via_directorium(ingressus, piscina);
        nom = via_nomen(ingressus, piscina);
        rad = via_nomen_radix(ingressus, piscina);
        ext = via_extensio(ingressus, piscina);

        memset(ingressus.datum, 'Q', (size_t)ingressus.mensura);

        CREDO_CHORDA_AEQUALIS_LITERIS(dir, "/usr/lib");
        CREDO_CHORDA_AEQUALIS_LITERIS(nom, "foo.txt");
        CREDO_CHORDA_AEQUALIS_LITERIS(rad, "foo");
        CREDO_CHORDA_AEQUALIS_LITERIS(ext, ".txt");
    }

    /* G ======================================================
     * PROBARE: via_normalizare - via vacua -> "."
     * ======================================================== */
    {
        chorda vacua;

        imprimere("\n--- Probans via_normalizare (vacua) ---\n");

        vacua.datum   = NIHIL;
        vacua.mensura = ZEPHYRUM;
        CREDO_CHORDA_AEQUALIS_LITERIS(via_normalizare(vacua, piscina), ".");
        CREDO_CHORDA_AEQUALIS_LITERIS(via_normalizare(
            chorda_ex_literis("", piscina), piscina), ".");
    }

    /* H ======================================================
     * PROBARE: via_nomen / via_extensio / via_directorium -
     * mensura supra 2^31 (i32 insignatus)
     * ======================================================== */
    {
        memoriae_index  amplitudo;
                    i8* regio;
                 chorda c_magna;

        imprimere("\n--- Probans extractio (mensura > 2^31) ---\n");

        amplitudo = (memoriae_index)2147483648UL + XVI;
        regio = (i8*)mmap(NIHIL, (size_t)amplitudo,
            PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        CREDO_NECESSE(regio != (i8*)MAP_FAILED);

        memcpy(regio, "/dir/", V);
        memcpy(regio + amplitudo - IX, "/name.txt", IX);
        c_magna.datum   = regio;
        c_magna.mensura = (i32)amplitudo;

        CREDO_CHORDA_AEQUALIS_LITERIS(via_nomen(c_magna, piscina),
            "name.txt");
        CREDO_CHORDA_AEQUALIS_LITERIS(via_extensio(c_magna, piscina),
            ".txt");
        CREDO_VERUM(via_directorium(c_magna, piscina).mensura
            == (i32)(amplitudo - IX));

        munmap(regio, (size_t)amplitudo);
    }

    /* I ======================================================
     * PROBARE: via_iungere - separator utrimque non duplicatur
     * ======================================================== */
    {
        chorda partes[II];

        imprimere("\n--- Probans via_iungere (separator utrimque) ---\n");

        partes[ZEPHYRUM] = chorda_ex_literis("foo/", piscina);
        partes[I]        = chorda_ex_literis("/bar", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(via_iungere(partes, II, piscina),
            "foo/bar");

        partes[ZEPHYRUM] = chorda_ex_literis("/", piscina);
        partes[I]        = chorda_ex_literis("/", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(via_iungere(partes, II, piscina),
            "/");
    }

    /* J ======================================================
     * PROBARE: via_normalizare - allocationes non per partem
     * ======================================================== */
    {
                 i8  buf[MM];
                i32  idx;
             chorda  longa;
             chorda  fructus;
     memoriae_index  ante;
     memoriae_index  delta;

        imprimere("\n--- Probans via_normalizare (allocationes) ---\n");

        per (idx = ZEPHYRUM; idx < MM; idx += II)
        {
            buf[idx]      = 'a';
            buf[idx + I]  = '/';
        }
        longa.datum   = buf;
        longa.mensura = MM;

        ante    = piscina_numerus_allocationum(piscina);
        fructus = via_normalizare(longa, piscina);
        delta   = piscina_numerus_allocationum(piscina) - ante;

        CREDO_AEQUALIS_I32(fructus.mensura, (i32)(MM - I));
        /* M partes; hodie ~MM allocationes (".", ".." per partem) */
        CREDO_MINOR_I32((i32)delta, C);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
