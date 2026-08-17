/* probatio_git.c - lector repositorii git contra repositorium
 * VERUM (rhubarb ipsum - suite e radice currit).
 *
 * Oraculum alterius generis: git_massam_per_viam(CAPUT,
 * "include/latina.h") octetatim contra plagulam in disco - via
 * tota (zlib + sarcinae + deltae + arbores) probatur contra
 * veritatem externam. Insuper OMNIS lectio se ipsam verificat
 * (sha recomputatum in git_obiectum_legere), ergo quisque
 * transitus = probatio SHA-1 contra obiecta vera.
 *
 * NB: latina.h electa quia numquam mutatur - discus == CAPUT
 * semper. Si umquam mutabitur in arbore laborante immunda,
 * assertio octetorum honeste clamabit.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "git.h"
#include "credo.h"

#include <stdio.h>
#include <string.h>

interior b32
_est_sha_hex (constans character* sha);

interior b32
_est_sha_hex (constans character* sha)
{
    i32 i;

    per (i = 0; i < 40; i = i + 1)
    {
        character c = sha[i];
        b32 hexus = (c >= '0' && c <= '9')
            || (c >= 'a' && c <= 'f');

        si (!hexus)
        {
            redde FALSUM;
        }
    }
    redde sha[40] == '\0';
}

s32 principale (vacuum)
{
    b32              praeteritus;
    Piscina*         piscina;
    GitRepositorium* repositorium;
    character        caput[GIT_SHA_HEX_MENSURA];

    piscina = piscina_generare_dynamicum("probatio_git", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans apertionem ---\n");

    repositorium = git_aperire(piscina, ".");
    CREDO_NON_NIHIL(repositorium);
    CREDO_VERUM(xar_numerus(repositorium->sarcinae) >= (i32)1);

    /* non repositorium -> NIHIL */
    CREDO_NIHIL(git_aperire(piscina, "/tmp"));

    imprimere("\n--- Probans resolutionem refum ---\n");

    CREDO_VERUM(git_ref_resolvere(repositorium, "HEAD", caput));
    CREDO_VERUM(_est_sha_hex(caput));

    /* sha plenum per se ipsum */
    {
        character idem_sha[GIT_SHA_HEX_MENSURA];

        CREDO_VERUM(git_ref_resolvere(repositorium, caput,
            idem_sha));
        CREDO_VERUM(strcmp(caput, idem_sha) == 0);
    }

    /* ramus principalis == CAPUT (suite in main currit) */
    {
        character ramus[GIT_SHA_HEX_MENSURA];

        CREDO_VERUM(git_ref_resolvere(repositorium, "main",
            ramus));
        CREDO_VERUM(strcmp(caput, ramus) == 0);
    }

    /* ~N: parentes distincti, catena consistens */
    {
        character primus[GIT_SHA_HEX_MENSURA];
        character tertius[GIT_SHA_HEX_MENSURA];

        CREDO_VERUM(git_ref_resolvere(repositorium, "HEAD~1",
            primus));
        CREDO_VERUM(git_ref_resolvere(repositorium, "HEAD~3",
            tertius));
        CREDO_VERUM(strcmp(caput, primus) != 0);
        CREDO_VERUM(strcmp(primus, tertius) != 0);
    }

    /* ignota recusantur */
    {
        character vanum[GIT_SHA_HEX_MENSURA];

        CREDO_FALSUM(git_ref_resolvere(repositorium,
            "ramus/qui/non/est", vanum));
    }

    /* sha breve: praefixum VIII resolvitur ad CAPUT; praefixum
     * III recusatur (minimum IV) */
    {
        character breve[9];
        character resolutum[GIT_SHA_HEX_MENSURA];

        memcpy(breve, caput, 8);
        breve[8] = '\0';
        CREDO_VERUM(git_ref_resolvere(repositorium, breve,
            resolutum));
        CREDO_VERUM(strcmp(caput, resolutum) == 0);
        breve[3] = '\0';
        CREDO_FALSUM(git_ref_resolvere(repositorium, breve,
            resolutum));
    }

    imprimere("\n--- Probans commissum ---\n");

    {
        GitCommissum commissum = git_commissum_legere(repositorium,
            caput, piscina);

        CREDO_VERUM(commissum.successus);
        CREDO_AEQUALIS_I32(commissum.arbor.mensura, (i32)40);
        CREDO_VERUM(xar_numerus(commissum.parentes) >= (i32)1);
        CREDO_VERUM(commissum.nuntius.mensura > (i32)0);
        /* epocha post MMXX (sanitas temporis) */
        CREDO_VERUM(commissum.tempus > (s64)1577836800);
        CREDO_CHORDA_CONTINET(commissum.auctor,
            chorda_ex_literis("@", piscina));

        imprimere("\n--- Probans arborem ---\n");

        {
            Xar* introitus = git_arborem_legere(repositorium,
                chorda_ut_cstr(commissum.arbor, piscina), piscina);
            b32  lib_arbor = FALSUM;
            b32  include_arbor = FALSUM;
            b32  claude_massa = FALSUM;
            i32  i;

            CREDO_NON_NIHIL(introitus);
            CREDO_VERUM(xar_numerus(introitus) > (i32)10);
            per (i = 0; i < xar_numerus(introitus); i = i + 1)
            {
                GitArborIntroitus* e = (GitArborIntroitus*)
                    xar_obtinere(introitus, i);

                si (chorda_aequalis_literis(e->titulus, "lib")
                    && e->est_arbor)
                {
                    lib_arbor = VERUM;
                }
                si (chorda_aequalis_literis(e->titulus, "include")
                    && e->est_arbor)
                {
                    include_arbor = VERUM;
                }
                si (chorda_aequalis_literis(e->titulus,
                        "CLAUDE.md") && !e->est_arbor)
                {
                    claude_massa = VERUM;
                }
            }
            CREDO_VERUM(lib_arbor);
            CREDO_VERUM(include_arbor);
            CREDO_VERUM(claude_massa);
        }
    }

    imprimere("\n--- Probans massam per viam (oraculum disci) ---\n");

    {
        b32    inventum = FALSUM;
        chorda massa = git_massam_per_viam(repositorium, caput,
            "include/latina.h", piscina, &inventum);
        chorda discus = filum_legere_totum("include/latina.h",
            piscina);

        CREDO_VERUM(inventum);
        CREDO_VERUM(massa.mensura > (i32)0);
        CREDO_AEQUALIS_I32(massa.mensura, discus.mensura);
        CREDO_VERUM(massa.mensura == discus.mensura
            && memcmp(massa.datum, discus.datum,
                   (memoriae_index)massa.mensura) == 0);
    }

    /* via ignota */
    {
        b32 inventum = VERUM;

        (vacuum)git_massam_per_viam(repositorium, caput,
            "non/exsistit.c", piscina, &inventum);
        CREDO_FALSUM(inventum);
    }

    imprimere("\n--- Probans historiam profundam (sarcinae) ---\n");

    /* HEAD~20: obiecta fere certe in sarcinis (deltae exercitae);
     * omnis lectio sha-verificata - transitus = probatio SHA-1 */
    {
        character vetus[GIT_SHA_HEX_MENSURA];
        b32       inventum = FALSUM;
        chorda    massa;

        CREDO_VERUM(git_ref_resolvere(repositorium, "HEAD~20",
            vetus));
        massa = git_massam_per_viam(repositorium, vetus,
            "include/latina.h", piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_VERUM(massa.mensura > (i32)1000);

        /* et profundius: catena parentum XL gradus */
        CREDO_VERUM(git_ref_resolvere(repositorium, "HEAD~40",
            vetus));
        massa = git_massam_per_viam(repositorium, vetus,
            "CLAUDE.md", piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_VERUM(massa.mensura > (i32)1000);
    }

    imprimere("\n--- Probans differentiam arborum ---\n");

    /* historia congelata = oraculum immutabile: commissum 4aafde9
     * (bibliotheca git ipsa nata) contra parentem d4a89dd.
     * Exspectata transcripta ex 'git show --name-status 4aafde9':
     *   M  compile_tests_fontes_generata.sh
     *   A  include/git.h
     *   A  lib/git.c
     *   A  lib/git.worklog.md
     *   A  probationes/probatio_git.c
     * (ordo idem: viae ordinatae chorda_comparare) */
    {
        character    sha_parentis[GIT_SHA_HEX_MENSURA];
        character    sha_nati[GIT_SHA_HEX_MENSURA];
        GitCommissum parens;
        GitCommissum natus;
        Xar*         mutatae;

        CREDO_VERUM(git_ref_resolvere(repositorium, "d4a89dd",
            sha_parentis));
        CREDO_VERUM(git_ref_resolvere(repositorium, "4aafde9",
            sha_nati));
        parens = git_commissum_legere(repositorium, sha_parentis,
            piscina);
        natus = git_commissum_legere(repositorium, sha_nati,
            piscina);
        CREDO_VERUM(parens.successus);
        CREDO_VERUM(natus.successus);

        /* arbor contra se ipsam - differentia nulla */
        mutatae = git_arbores_differre(repositorium,
            chorda_ut_cstr(parens.arbor, piscina),
            chorda_ut_cstr(parens.arbor, piscina), piscina);
        CREDO_NON_NIHIL(mutatae);
        CREDO_AEQUALIS_I32(xar_numerus(mutatae), (i32)0);

        mutatae = git_arbores_differre(repositorium,
            chorda_ut_cstr(parens.arbor, piscina),
            chorda_ut_cstr(natus.arbor, piscina), piscina);
        CREDO_NON_NIHIL(mutatae);
        CREDO_AEQUALIS_I32(xar_numerus(mutatae), (i32)5);
        {
            GitViaMutata* m = (GitViaMutata*)xar_obtinere(mutatae,
                0);

            CREDO_VERUM(chorda_aequalis_literis(m->via,
                "compile_tests_fontes_generata.sh"));
            CREDO_VERUM(m->genus == GIT_VIA_MUTATA);
            CREDO_AEQUALIS_I32(m->sha_vetus.mensura, (i32)40);
            CREDO_AEQUALIS_I32(m->sha_novus.mensura, (i32)40);

            m = (GitViaMutata*)xar_obtinere(mutatae, 1);
            CREDO_VERUM(chorda_aequalis_literis(m->via,
                "include/git.h"));
            CREDO_VERUM(m->genus == GIT_VIA_ADDITA);
            CREDO_AEQUALIS_I32(m->sha_vetus.mensura, (i32)0);
            CREDO_AEQUALIS_I32(m->sha_novus.mensura, (i32)40);

            m = (GitViaMutata*)xar_obtinere(mutatae, 2);
            CREDO_VERUM(chorda_aequalis_literis(m->via,
                "lib/git.c"));
            CREDO_VERUM(m->genus == GIT_VIA_ADDITA);

            m = (GitViaMutata*)xar_obtinere(mutatae, 3);
            CREDO_VERUM(chorda_aequalis_literis(m->via,
                "lib/git.worklog.md"));
            CREDO_VERUM(m->genus == GIT_VIA_ADDITA);

            m = (GitViaMutata*)xar_obtinere(mutatae, 4);
            CREDO_VERUM(chorda_aequalis_literis(m->via,
                "probationes/probatio_git.c"));
            CREDO_VERUM(m->genus == GIT_VIA_ADDITA);
        }

        /* directio inversa: ADDITA fiunt REMOTA, symmetria */
        mutatae = git_arbores_differre(repositorium,
            chorda_ut_cstr(natus.arbor, piscina),
            chorda_ut_cstr(parens.arbor, piscina), piscina);
        CREDO_NON_NIHIL(mutatae);
        CREDO_AEQUALIS_I32(xar_numerus(mutatae), (i32)5);
        {
            GitViaMutata* m = (GitViaMutata*)xar_obtinere(mutatae,
                1);

            CREDO_VERUM(chorda_aequalis_literis(m->via,
                "include/git.h"));
            CREDO_VERUM(m->genus == GIT_VIA_REMOTA);
            CREDO_AEQUALIS_I32(m->sha_vetus.mensura, (i32)40);
            CREDO_AEQUALIS_I32(m->sha_novus.mensura, (i32)0);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
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
