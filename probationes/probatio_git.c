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
 * ORACULUM = 'git show HEAD:include/latina.h' (git ipsum, externum),
 * NON discus. Olim discus: "latina.h numquam mutatur, ergo discus ==
 * CAPUT" - sed porta ANTE commissionem currit, ergo omnis mutatio
 * latina.h (2026-09-24: linea descriptionis) portam fregit et
 * committi non poterat. Arbor laborans nihil ad caput pertinet.
 */

/* <aedilis facultas="repositorium"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "filum.h"
#include "via.h"
#include "git.h"
#include "credo.h"
#include "processus.h"
#include "friatio.h"

#include <stdio.h>
#include <string.h>

/* MUNDUS SUITAE (credo II): repositorium VERUM semel apertum, CAPUT
 * semel resolutum - sectiones LEGUNT (c->suita), numquam mutant.
 * Repositorium in piscina SUITAE vivit (git_aperire allocationes suas
 * in piscina creationis facit, numquam in piscina sectionis). */
nomen structura {
     GitRepositorium* repositorium;
           character  caput[GIT_SHA_HEX_MENSURA];
} MundusGit;

interior b32
_est_sha_hex (
    constans character* sha);

/* massa include/latina.h in CAPITE per git ipsum (oraculum externum);
 * chorda vacua si git deficit */
interior chorda
_massa_per_git (
    Piscina* piscina)
{
     constans character* argumenta[IV];
      ProcessusResultus  r;
                 chorda  vacua;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    argumenta[ZEPHYRUM] = "git";
    argumenta[I] = "show";
    argumenta[II] = "HEAD:include/latina.h";
    argumenta[III] = NIHIL;
    r = processus_exsequi(argumenta, (i32)10000, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde vacua;
    }
    redde r.effusio;
}

/* a + b + c in piscina (viae et contenta arborum synthetica) */
interior constans character*
_serere (
               Piscina* piscina,
    constans character* a,
    constans character* b,
    constans character* c)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)256);

    chorda_aedificator_appendere_literis(aed, a);
    chorda_aedificator_appendere_literis(aed, b);
    chorda_aedificator_appendere_literis(aed, c);
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

interior b32
_est_sha_hex (
    constans character* sha)
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


/* ==================================================
 * FIXURA SUITAE: repositorium et CAPUT. Repositorium absens (arbor
 * operis frigida, 2026-09-28, quaestio …QY4: reliqua SIGSEGV ruebant)
 * -> '(parare suitae)' ABORTA, nulla sectio currit - rubrum clarum
 * ================================================== */

interior vacuum
_mundum_parare (
    CredoContextus* c)
{
    MundusGit* m;

    m = (MundusGit*)piscina_allocare(c->piscina, magnitudo(MundusGit));
    CREDO_NECESSE_NON_NIHIL(m);
    m->repositorium = git_aperire(c->piscina, ".");
    CREDO_NECESSE_NON_NIHIL(m->repositorium);
    CREDO_NECESSE(git_ref_resolvere(m->repositorium, "HEAD", m->caput));
    c->datum = m;
}

interior vacuum
_sectio_apertio (
    CredoContextus* c)
{
    constans MundusGit* m = (constans MundusGit*)c->suita;

    CREDO_VERUM(xar_numerus(m->repositorium->sarcinae) >= (i32)1);

    /* non repositorium -> NIHIL */
    CREDO_NIHIL(git_aperire(c->piscina, "/tmp"));
}

interior vacuum
_sectio_resolutio (
    CredoContextus* c)
{
    constans MundusGit* m             = (constans MundusGit*)c->suita;
       GitRepositorium* repositorium  = m->repositorium;
    constans character* caput         = m->caput;
               Piscina* piscina       = c->piscina;

    CREDO_VERUM(_est_sha_hex(caput));

    /* sha plenum per se ipsum */
    {
        character idem_sha[GIT_SHA_HEX_MENSURA];

        /* <tolera codex="CREDO_SUITA_MUTATA" (>lectio git scratch in repositorium->piscina allocat (arena fixurae crescit, status idem) - vitium bibliothecae, parcum 01M5DWCWHCV4Q1Q8Z8767WTA47 */
        CREDO_VERUM(git_ref_resolvere(repositorium, caput,
            idem_sha));
        CREDO_VERUM(strcmp(caput, idem_sha) == 0);
    }

    /* CAPUT ramo alligatum, et ramus ille == CAPUT. Olim 'main' hic
     * pinnatum erat: suite in ramo quovis (arbores operis fratres) et
     * in clone umbrae rubra erat, ubi 'main' ne ref localis quidem
     * est (2026-09-04, 01M17PG01E). Ramus ex <via_git>/HEAD legitur
     * ('ref: refs/heads/X'); CAPUT solutum (sine ramo) assertionem
     * praefixi clare frangit - nulla porta tacita. */
    {
                 character  ramus[GIT_SHA_HEX_MENSURA];
         ChordaAedificator* aed;
                    chorda  caput_textus;
                    chorda  praefixum;
                    chorda  nomen_rami;
                       i32  finis;

        aed = chorda_aedificator_creare(piscina, (memoriae_index)256);
        chorda_aedificator_appendere_literis(aed,
            repositorium->via_git);
        chorda_aedificator_appendere_literis(aed, "/HEAD");
        caput_textus = filum_legere_totum(
            chorda_ut_cstr(chorda_aedificator_finire(aed), piscina),
            piscina);
        praefixum = chorda_ex_literis("ref: refs/heads/", piscina);
        CREDO_VERUM(caput_textus.mensura > praefixum.mensura);
        CREDO_VERUM(chorda_incipit(caput_textus, praefixum));

        /* 'ref: ' abscisum, lineae novae finales abscisae */
        finis = caput_textus.mensura;
        dum (   finis > ZEPHYRUM
             && (caput_textus.datum[finis - I] == '\n'
                || caput_textus.datum[finis - I] == '\r'))
        {
            finis = finis - I;
        }
        nomen_rami = chorda_sectio(caput_textus,
            (i32)strlen("ref: "), finis);
        /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
        CREDO_VERUM(git_ref_resolvere(repositorium,
            chorda_ut_cstr(nomen_rami, piscina), ramus));
        CREDO_VERUM(strcmp(caput, ramus) == 0);
    }

    /* ~N: parentes distincti, catena consistens */
    {
        character primus[GIT_SHA_HEX_MENSURA];
        character tertius[GIT_SHA_HEX_MENSURA];

        /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
        CREDO_VERUM(git_ref_resolvere(repositorium, "HEAD~1",
            primus));
        /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
        CREDO_VERUM(git_ref_resolvere(repositorium, "HEAD~3",
            tertius));
        CREDO_VERUM(strcmp(caput, primus) != 0);
        CREDO_VERUM(strcmp(primus, tertius) != 0);
    }

    /* ignota recusantur */
    {
        character vanum[GIT_SHA_HEX_MENSURA];

        /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
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
        /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
        CREDO_VERUM(git_ref_resolvere(repositorium, breve,
            resolutum));
        CREDO_VERUM(strcmp(caput, resolutum) == 0);
        breve[3] = '\0';
        /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
        CREDO_FALSUM(git_ref_resolvere(repositorium, breve,
            resolutum));
    }
}

/* ARBOR OPERIS (git worktree add; quaestio …QY4, desideratum
 * …JF8ZMD): '.git' ibi PLAGULA est ('gitdir: <via>'), gitdir HEAD
 * proprium tenet et 'commondir' ad directorium commune (objects,
 * refs, packed-refs) monstrat. Structura synthetica in credo_area
 * (credo II T5; olim scratch manu sub TMPDIR, ante build/), git ipso
 * non vocato. git_aperire directoria parentum ascendit: sine '.git'
 * plagula intellecta repositorium VERUM supra inveniret, ergo via_git
 * == gitdir syntheticum asseritur, non apertio sola. */
interior vacuum
_sectio_arbor_operis (
    CredoContextus* c)
{
    constans MundusGit* m             = (constans MundusGit*)c->suita;
       GitRepositorium* repositorium  = m->repositorium;
               Piscina* piscina       = c->piscina;
    constans character* radix_arborum;
    constans character* communis;
    constans character* gitdir;
    constans character* arbor_duo;
    constans character* arbor_unus;
    constans character* arbores[II];
       GitRepositorium* r;
             character  sha[GIT_SHA_HEX_MENSURA];
                   i32  k;

    radix_arborum = credo_area(c);
    CREDO_NECESSE_NON_NIHIL(radix_arborum);
    communis    = repositorium->via_communis;
    gitdir      = _serere(piscina, radix_arborum, "/duo/gitdir", "");
    arbor_duo   = _serere(piscina, radix_arborum, "/duo/arbor", "");
    arbor_unus  = _serere(piscina, radix_arborum, "/unus", "");
    CREDO_VERUM(filum_directorium_creare_cum_parentibus(gitdir));
    CREDO_VERUM(filum_directorium_creare_cum_parentibus(arbor_duo));
    CREDO_VERUM(filum_directorium_creare_cum_parentibus(arbor_unus));
    /* gitdir: HEAD idem ac arboris huius, commondir absolutum */
    CREDO_VERUM(filum_scribere(_serere(piscina, gitdir, "/HEAD", ""),
        filum_legere_totum(_serere(piscina, repositorium->via_git,
            "/HEAD", ""), piscina)));
    CREDO_VERUM(filum_scribere_literis(_serere(piscina, gitdir,
        "/commondir", ""), _serere(piscina, communis, "\n", "")));
    /* forma I: gitdir RELATIVUS (ad arborem); forma II: absolutus */
    CREDO_VERUM(filum_scribere_literis(_serere(piscina, arbor_unus,
        "/.git", ""), "gitdir: ../duo/gitdir\n"));
    CREDO_VERUM(filum_scribere_literis(_serere(piscina, arbor_duo,
        "/.git", ""), _serere(piscina, "gitdir: ", gitdir, "\n")));
    arbores[ZEPHYRUM]  = arbor_unus;
    arbores[I]         = arbor_duo;
    per (k = ZEPHYRUM; k < II; k++)
    {
        r = git_aperire(piscina, arbores[k]);
        CREDO_NON_NIHIL(r);
        si (r == NIHIL)
        {
            perge;
        }
        CREDO_VERUM(strcmp(r->via_git, gitdir) == ZEPHYRUM);
        CREDO_VERUM(strcmp(r->via_communis, communis) == ZEPHYRUM);
        CREDO_AEQUALIS_I32(xar_numerus(r->sarcinae),
            xar_numerus(repositorium->sarcinae));
        CREDO_VERUM(git_ref_resolvere(r, "HEAD", sha));
        CREDO_VERUM(strcmp(sha, m->caput) == ZEPHYRUM);
    }
}

interior vacuum
_sectio_commissum (
    CredoContextus* c)
{
    constans MundusGit* m = (constans MundusGit*)c->suita;
          GitCommissum  commissum;

    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    commissum = git_commissum_legere(m->repositorium, m->caput,
        c->piscina);
    CREDO_VERUM(commissum.successus);
    CREDO_AEQUALIS_I32(commissum.arbor.mensura, (i32)40);
    CREDO_VERUM(xar_numerus(commissum.parentes) >= (i32)1);
    CREDO_VERUM(commissum.nuntius.mensura > (i32)0);
    /* epocha post MMXX (sanitas temporis) */
    CREDO_VERUM(commissum.tempus > (s64)1577836800);
    CREDO_CHORDA_CONTINET(commissum.auctor,
        chorda_ex_literis("@", c->piscina));
}

interior vacuum
_sectio_arbor (
    CredoContextus* c)
{
    constans MundusGit* m        = (constans MundusGit*)c->suita;
               Piscina* piscina  = c->piscina;
          GitCommissum  commissum;
                   Xar* introitus;
                   b32  lib_arbor      = FALSUM;
                   b32  include_arbor  = FALSUM;
                   b32  claude_massa   = FALSUM;
                   i32  i;

    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    commissum = git_commissum_legere(m->repositorium, m->caput,
        piscina);
    CREDO_NECESSE(commissum.successus);
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    introitus = git_arborem_legere(m->repositorium,
        chorda_ut_cstr(commissum.arbor, piscina), piscina);
    CREDO_NECESSE_NON_NIHIL(introitus);
    CREDO_VERUM(xar_numerus(introitus) > (i32)10);
    per (i = 0; i < xar_numerus(introitus); i = i + 1)
    {
        GitArborIntroitus* e = (GitArborIntroitus*)
            xar_obtinere(introitus, i);

        si (   chorda_aequalis_literis(e->titulus, "lib")
            && e->est_arbor)
        {
            lib_arbor = VERUM;
        }
        si (   chorda_aequalis_literis(e->titulus, "include")
            && e->est_arbor)
        {
            include_arbor = VERUM;
        }
        si (   chorda_aequalis_literis(e->titulus,
                "CLAUDE.md") && !e->est_arbor)
        {
            claude_massa = VERUM;
        }
    }
    CREDO_VERUM(lib_arbor);
    CREDO_VERUM(include_arbor);
    CREDO_VERUM(claude_massa);
}

interior vacuum
_sectio_massa (
    CredoContextus* c)
{
    constans MundusGit* m = (constans MundusGit*)c->suita;

    {
           b32 inventum = FALSUM;
        /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
        chorda massa = git_massam_per_viam(m->repositorium, m->caput,
            "include/latina.h", c->piscina, &inventum);
        chorda discus = _massa_per_git(c->piscina);

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

        /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
        (vacuum)git_massam_per_viam(m->repositorium, m->caput,
            "non/exsistit.c", c->piscina, &inventum);
        CREDO_FALSUM(inventum);
    }
}

/* HEAD~20: obiecta fere certe in sarcinis (deltae exercitae); omnis
 * lectio sha-verificata - transitus = probatio SHA-1 */
interior vacuum
_sectio_historia (
    CredoContextus* c)
{
    constans MundusGit* m = (constans MundusGit*)c->suita;
             character  vetus[GIT_SHA_HEX_MENSURA];
                   b32  inventum = FALSUM;
                chorda  massa;

    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    CREDO_VERUM(git_ref_resolvere(m->repositorium, "HEAD~20", vetus));
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    massa = git_massam_per_viam(m->repositorium, vetus,
        "include/latina.h", c->piscina, &inventum);
    CREDO_VERUM(inventum);
    CREDO_VERUM(massa.mensura > (i32)1000);

    /* et profundius: catena parentum XL gradus */
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    CREDO_VERUM(git_ref_resolvere(m->repositorium, "HEAD~40", vetus));
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    massa = git_massam_per_viam(m->repositorium, vetus, "CLAUDE.md",
        c->piscina, &inventum);
    CREDO_VERUM(inventum);
    CREDO_VERUM(massa.mensura > (i32)1000);
}

/* catena probationis completa sine sha transcripto: sha promptum ->
 * obiectum eo sha lectum (lectio se ipsam contra sha verificat!) ->
 * octeti == discus. Sha falsum catenam alicubi frangeret. */
interior vacuum
_sectio_sha_per_viam (
    CredoContextus* c)
{
    constans MundusGit* m = (constans MundusGit*)c->suita;
             character  sha_massae[GIT_SHA_HEX_MENSURA];
                   b32  est_arbor = VERUM;
           GitObiectum  obiectum;
                chorda  discus;

    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    CREDO_VERUM(git_sha_per_viam(m->repositorium, m->caput,
        "include/latina.h", c->piscina, sha_massae, &est_arbor));
    CREDO_FALSUM(est_arbor);
    CREDO_VERUM(_est_sha_hex(sha_massae));
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    obiectum = git_obiectum_legere(m->repositorium, sha_massae,
        c->piscina);
    CREDO_VERUM(obiectum.successus);
    CREDO_VERUM(obiectum.genus == GIT_OBIECTUM_MASSA);
    discus = _massa_per_git(c->piscina);
    CREDO_AEQUALIS_I32(obiectum.datum.mensura, discus.mensura);
    CREDO_VERUM(obiectum.datum.mensura == discus.mensura
        && memcmp(obiectum.datum.datum, discus.datum,
               (memoriae_index)discus.mensura) == 0);

    /* directorium: sha redditur, est_arbor VERUM */
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    CREDO_VERUM(git_sha_per_viam(m->repositorium, m->caput, "lib",
        c->piscina, sha_massae, &est_arbor));
    CREDO_VERUM(est_arbor);

    /* via ignota */
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    CREDO_FALSUM(git_sha_per_viam(m->repositorium, m->caput,
        "non/exsistit.c", c->piscina, sha_massae, &est_arbor));
}

/* historia congelata = oraculum immutabile: commissum 4aafde9
 * (bibliotheca git ipsa nata) contra parentem d4a89dd.
 * Exspectata transcripta ex 'git show --name-status 4aafde9':
 *   M  compile_tests_fontes_generata.sh
 *   A  include/git.h
 *   A  lib/git.c
 *   A  lib/git.worklog.md
 *   A  probationes/probatio_git.c
 * (ordo idem: viae ordinatae chorda_comparare) */
interior vacuum
_sectio_differentia (
    CredoContextus* c)
{
    constans MundusGit* m             = (constans MundusGit*)c->suita;
       GitRepositorium* repositorium  = m->repositorium;
               Piscina* piscina       = c->piscina;
             character  sha_parentis[GIT_SHA_HEX_MENSURA];
             character  sha_nati[GIT_SHA_HEX_MENSURA];
          GitCommissum  parens;
          GitCommissum  natus;
                   Xar* mutatae;
          GitViaMutata* via_mutata;

    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    CREDO_VERUM(git_ref_resolvere(repositorium, "d4a89dd",
        sha_parentis));
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    CREDO_VERUM(git_ref_resolvere(repositorium, "4aafde9", sha_nati));
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    parens  = git_commissum_legere(repositorium, sha_parentis, piscina);
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    natus   = git_commissum_legere(repositorium, sha_nati, piscina);
    CREDO_NECESSE(parens.successus);
    CREDO_NECESSE(natus.successus);

    /* arbor contra se ipsam - differentia nulla */
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    mutatae = git_arbores_differre(repositorium,
        chorda_ut_cstr(parens.arbor, piscina),
        chorda_ut_cstr(parens.arbor, piscina), piscina);
    CREDO_NECESSE_NON_NIHIL(mutatae);
    CREDO_AEQUALIS_I32(xar_numerus(mutatae), (i32)0);

    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    mutatae = git_arbores_differre(repositorium,
        chorda_ut_cstr(parens.arbor, piscina),
        chorda_ut_cstr(natus.arbor, piscina), piscina);
    CREDO_NECESSE_NON_NIHIL(mutatae);
    CREDO_NECESSE(xar_numerus(mutatae) == (i32)5);

    via_mutata = (GitViaMutata*)xar_obtinere(mutatae, 0);
    CREDO_VERUM(chorda_aequalis_literis(via_mutata->via,
        "compile_tests_fontes_generata.sh"));
    CREDO_VERUM(via_mutata->genus == GIT_VIA_MUTATA);
    CREDO_AEQUALIS_I32(via_mutata->sha_vetus.mensura, (i32)40);
    CREDO_AEQUALIS_I32(via_mutata->sha_novus.mensura, (i32)40);

    via_mutata = (GitViaMutata*)xar_obtinere(mutatae, 1);
    CREDO_VERUM(chorda_aequalis_literis(via_mutata->via,
        "include/git.h"));
    CREDO_VERUM(via_mutata->genus == GIT_VIA_ADDITA);
    CREDO_AEQUALIS_I32(via_mutata->sha_vetus.mensura, (i32)0);
    CREDO_AEQUALIS_I32(via_mutata->sha_novus.mensura, (i32)40);

    via_mutata = (GitViaMutata*)xar_obtinere(mutatae, 2);
    CREDO_VERUM(chorda_aequalis_literis(via_mutata->via, "lib/git.c"));
    CREDO_VERUM(via_mutata->genus == GIT_VIA_ADDITA);

    via_mutata = (GitViaMutata*)xar_obtinere(mutatae, 3);
    CREDO_VERUM(chorda_aequalis_literis(via_mutata->via,
        "lib/git.worklog.md"));
    CREDO_VERUM(via_mutata->genus == GIT_VIA_ADDITA);

    via_mutata = (GitViaMutata*)xar_obtinere(mutatae, 4);
    CREDO_VERUM(chorda_aequalis_literis(via_mutata->via,
        "probationes/probatio_git.c"));
    CREDO_VERUM(via_mutata->genus == GIT_VIA_ADDITA);

    /* directio inversa: ADDITA fiunt REMOTA, symmetria */
    /* <tolera codex="CREDO_SUITA_MUTATA" (>idem: arena repositorii (parcum ...TA47) */
    mutatae = git_arbores_differre(repositorium,
        chorda_ut_cstr(natus.arbor, piscina),
        chorda_ut_cstr(parens.arbor, piscina), piscina);
    CREDO_NECESSE_NON_NIHIL(mutatae);
    CREDO_NECESSE(xar_numerus(mutatae) == (i32)5);
    via_mutata = (GitViaMutata*)xar_obtinere(mutatae, 1);
    CREDO_VERUM(chorda_aequalis_literis(via_mutata->via,
        "include/git.h"));
    CREDO_VERUM(via_mutata->genus == GIT_VIA_REMOTA);
    CREDO_AEQUALIS_I32(via_mutata->sha_vetus.mensura, (i32)40);
    CREDO_AEQUALIS_I32(via_mutata->sha_novus.mensura, (i32)0);
}


/* ==================================================
 * INDEX (fabrica-plan-8 G2)
 * ================================================== */

/* ORACULUM: 'git ls-files -s -z' (git ipsum) contra indicem verum
 * arboris huius - introitus quisque "<modus> <sha> <gradus>\t<via>"
 * idem et ordine eodem (XII milia et ultra) */
interior vacuum
_sectio_index (
    CredoContextus* c)
{
     constans MundusGit* m        = (constans MundusGit*)c->suita;
                Piscina* piscina  = c->piscina;
     constans character* argumenta[V];
      ProcessusResultus  r;
                    Xar* index;
                    i32  i;
                    i32  initium;
                    i32  k;
                    b32  discordia;

    index = git_indicem_legere(m->repositorium, piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    argumenta[ZEPHYRUM] = "git";
    argumenta[I] = "ls-files";
    argumenta[II] = "-s";
    argumenta[III] = "-z";
    argumenta[IV] = NIHIL;
    r = processus_exsequi(argumenta, (i32)30000, piscina);
    CREDO_NECESSE(r.successus && r.codex_exitus == ZEPHYRUM);
    CREDO_VERUM(xar_numerus(index) > (i32)M);
    initium    = ZEPHYRUM;
    k          = ZEPHYRUM;
    discordia  = FALSUM;
    per (i = ZEPHYRUM; i < r.effusio.mensura; i++)
    {
        constans GitIndexIntroitus* e;
                         character  linea[CCLVI];
                            chorda  oraculum;
                            chorda  nostrum;

        si (r.effusio.datum[i] != '\0')
        {
            perge;
        }
        oraculum  = chorda_sectio(r.effusio, initium, i);
        initium   = i + I;
        si (k >= xar_numerus(index))
        {
            k++;
            perge;
        }
        e = (constans GitIndexIntroitus*)xar_obtinere(index, k);
        sprintf(linea, "%06o %s %d\t", (insignatus integer)e->modus,
            e->sha, (integer)e->gradus);
        nostrum = chorda_concatenare(chorda_ex_literis(linea, piscina),
            e->via, piscina);
        si (!discordia && !chorda_aequalis(nostrum, oraculum))
        {
            discordia = VERUM;
            imprimere("  index: %.*s\n  git:   %.*s\n",
                (integer)nostrum.mensura, (constans character*)
                nostrum.datum, (integer)oraculum.mensura,
                (constans character*)oraculum.datum);
        }
        k++;
    }
    CREDO_AEQUALIS_I32(k, xar_numerus(index));
    CREDO_FALSUM(discordia);
}

/* introitus ficti in indicem (forma git: ctime, mtime, dev, ino,
 * modus, uid, gid, mensura, sha, vexilla [, vexilla extensa], via,
 * NUL ad multiplum VIII) */
interior vacuum
_be32_scribere (
    ChordaAedificator* a,
                  i32  valor)
{
    (vacuum)chorda_aedificator_appendere_character(a,
        (character)((valor >> 24) & 0xFF));
    (vacuum)chorda_aedificator_appendere_character(a,
        (character)((valor >> 16) & 0xFF));
    (vacuum)chorda_aedificator_appendere_character(a,
        (character)((valor >> 8) & 0xFF));
    (vacuum)chorda_aedificator_appendere_character(a,
        (character)(valor & 0xFF));
}

interior vacuum
_introitum_fingere (
      ChordaAedificator* a,
     constans character* via,
                    i32  modus,
                    i32  gradus,
                    b32  extensus,
                    i32  mensura,
                    i32  signum_sha)
{
    i32 longitudo = (i32)strlen(via);
    i32 octeti;
    i32 k;

    _be32_scribere(a, ZEPHYRUM);           /* ctime s */
    _be32_scribere(a, ZEPHYRUM);           /* ctime ns */
    _be32_scribere(a, (i32)1700000000);    /* mtime s */
    _be32_scribere(a, (i32)123456789);     /* mtime ns */
    _be32_scribere(a, ZEPHYRUM);           /* dev */
    _be32_scribere(a, ZEPHYRUM);           /* ino */
    _be32_scribere(a, modus);
    _be32_scribere(a, ZEPHYRUM);           /* uid */
    _be32_scribere(a, ZEPHYRUM);           /* gid */
    _be32_scribere(a, mensura);
    per (k = ZEPHYRUM; k < XX; k++)
    {
        (vacuum)chorda_aedificator_appendere_character(a,
            (character)((signum_sha + k) & 0xFF));
    }
    k = (gradus << XII)
        | (longitudo < (i32)0xFFF ? longitudo : (i32)0xFFF);
    si (extensus)
    {
        k |= (i32)0x4000;
    }
    (vacuum)chorda_aedificator_appendere_character(a,
        (character)((k >> 8) & 0xFF));
    (vacuum)chorda_aedificator_appendere_character(a,
        (character)(k & 0xFF));
    octeti = LXII;
    si (extensus)
    {
        /* intent-to-add (git add -N) */
        (vacuum)chorda_aedificator_appendere_character(a,
            (character)0x20);
        (vacuum)chorda_aedificator_appendere_character(a, (character)0);
        octeti += II;
    }
    (vacuum)chorda_aedificator_appendere_literis(a, via);
    octeti += longitudo;
    per (k = ((octeti + VIII) & ~(i32)VII) - octeti; k > ZEPHYRUM; k--)
    {
        (vacuum)chorda_aedificator_appendere_character(a, (character)0);
    }
}

/* caput + corpus + trailer SHA-1 -> '<gitdir>/index'; repositorium
 * fictum cuius via_git = gitdir */
interior b32
_indicem_ponere (
     constans character* gitdir,
                 chorda  corpus,
                    i32  versio,
                    i32  numerus,
                    b32  digestum_falsum,
                Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, CCLVI);
                   i8  digestum[SHA1_DIGEST_MENSURA];
               chorda  totum;
                  i32  k;

    (vacuum)chorda_aedificator_appendere_literis(a, "DIRC");
    _be32_scribere(a, versio);
    _be32_scribere(a, numerus);
    (vacuum)chorda_aedificator_appendere_chorda(a, corpus);
    totum = chorda_aedificator_spectare(a);
    sha1_friare(totum.datum, totum.mensura, digestum);
    si (digestum_falsum)
    {
        digestum[ZEPHYRUM] = (i8)(digestum[ZEPHYRUM] ^ 0x01);
    }
    per (k = ZEPHYRUM; k < SHA1_DIGEST_MENSURA; k++)
    {
        (vacuum)chorda_aedificator_appendere_character(a,
            (character)digestum[k]);
    }
    redde filum_scribere(_serere(piscina, gitdir, "/index", ""),
        chorda_aedificator_finire(a));
}

interior vacuum
_sectio_index_fictus (
    CredoContextus* c)
{
            constans MundusGit* m =
                (constans MundusGit*)c->suita;
                       Piscina* piscina = c->piscina;
            constans character* gitdir;
            constans character* arbor;
               GitRepositorium  fictum;
               GitRepositorium* r;
             ChordaAedificator* a;
                        chorda  duo;
                           Xar* index;
    constans GitIndexIntroitus* e;

    gitdir  = _serere(piscina, credo_area(c), "/gd", "");
    arbor   = _serere(piscina, credo_area(c), "/arbor", "");
    CREDO_NECESSE(filum_directorium_creare_cum_parentibus(gitdir));
    CREDO_NECESSE(filum_directorium_creare_cum_parentibus(arbor));
    fictum          = *m->repositorium;
    fictum.via_git  = gitdir;

    /* absens: Xar vacuum, non NIHIL */
    index = git_indicem_legere(&fictum, piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    CREDO_AEQUALIS_I32(xar_numerus(index), ZEPHYRUM);

    /* II: duo introitus, campi omnes */
    a = chorda_aedificator_creare(piscina, CCLVI);
    _introitum_fingere(a, "a.c", (i32)0100644, ZEPHYRUM, FALSUM, V,
        0x10);
    _introitum_fingere(a, "dir/b", (i32)0100755, ZEPHYRUM, FALSUM,
        (i32)70000, 0x20);
    duo = chorda_aedificator_finire(a);
    CREDO_NECESSE(_indicem_ponere(gitdir, duo, II, II, FALSUM,
        piscina));
    index = git_indicem_legere(&fictum, piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    CREDO_NECESSE(xar_numerus(index) == II);
    e = (constans GitIndexIntroitus*)xar_obtinere(index, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(e->via, "a.c");
    CREDO_AEQUALIS_I32(e->modus, (i32)0100644);
    CREDO_AEQUALIS_I32(e->gradus, ZEPHYRUM);
    CREDO_VERUM(e->mensura == (s64)V);
    CREDO_VERUM(e->tempus_ns == (s64)1700000000 * (s64)1000000000
        + (s64)123456789);
    CREDO_VERUM(strcmp(e->sha,
        "101112131415161718191a1b1c1d1e1f20212223") == ZEPHYRUM);
    e = (constans GitIndexIntroitus*)xar_obtinere(index, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(e->via, "dir/b");
    CREDO_AEQUALIS_I32(e->modus, (i32)0100755);
    CREDO_VERUM(e->mensura == (s64)70000);

    /* III: introitus extensus (git add -N) - II octeti plus; si non
     * omittuntur, via et introitus sequens male leguntur */
    a = chorda_aedificator_creare(piscina, CCLVI);
    _introitum_fingere(a, "novum.c", (i32)0100644, ZEPHYRUM, VERUM,
        ZEPHYRUM, 0x30);
    _introitum_fingere(a, "zeta", (i32)0100644, ZEPHYRUM, FALSUM, I,
        0x40);
    CREDO_NECESSE(_indicem_ponere(gitdir, chorda_aedificator_finire(a),
        III, II, FALSUM, piscina));
    index = git_indicem_legere(&fictum, piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    CREDO_NECESSE(xar_numerus(index) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans GitIndexIntroitus*)
        xar_obtinere(index, ZEPHYRUM))->via, "novum.c");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans GitIndexIntroitus*)
        xar_obtinere(index, I))->via, "zeta");

    /* gradus conflictus 1-3 */
    a = chorda_aedificator_creare(piscina, CCLVI);
    _introitum_fingere(a, "c", (i32)0100644, I, FALSUM, I, 1);
    _introitum_fingere(a, "c", (i32)0100644, II, FALSUM, I, 2);
    _introitum_fingere(a, "c", (i32)0100644, III, FALSUM, I, 3);
    CREDO_NECESSE(_indicem_ponere(gitdir, chorda_aedificator_finire(a),
        II, III, FALSUM, piscina));
    index = git_indicem_legere(&fictum, piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    CREDO_NECESSE(xar_numerus(index) == III);
    CREDO_AEQUALIS_I32(((constans GitIndexIntroitus*)xar_obtinere(
        index, II))->gradus, III);

    /* extensio (TREE) ante trailer praeteritur */
    a = chorda_aedificator_creare(piscina, CCLVI);
    (vacuum)chorda_aedificator_appendere_chorda(a, duo);
    (vacuum)chorda_aedificator_appendere_literis(a, "TREE");
    _be32_scribere(a, IV);
    (vacuum)chorda_aedificator_appendere_literis(a, "xyzw");
    CREDO_NECESSE(_indicem_ponere(gitdir, chorda_aedificator_finire(a),
        II, II, FALSUM, piscina));
    index = git_indicem_legere(&fictum, piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    CREDO_AEQUALIS_I32(xar_numerus(index), II);

    /* RECUSATIONES: IV, trailer falsus, numerus ultra corpus, signum */
    CREDO_NECESSE(_indicem_ponere(gitdir, duo, IV, II, FALSUM,
        piscina));
    CREDO_NIHIL(git_indicem_legere(&fictum, piscina));
    CREDO_NECESSE(_indicem_ponere(gitdir, duo, II, II, VERUM, piscina));
    CREDO_NIHIL(git_indicem_legere(&fictum, piscina));
    CREDO_NECESSE(_indicem_ponere(gitdir, duo, II, III, FALSUM,
        piscina));
    CREDO_NIHIL(git_indicem_legere(&fictum, piscina));
    CREDO_NECESSE(filum_scribere_literis(_serere(piscina, gitdir,
        "/index", ""), "DIRX\0\0\0\2\0\0\0\0"));
    CREDO_NIHIL(git_indicem_legere(&fictum, piscina));

    /* ARBOR OPERIS: index gitdir PROPRII legitur, non communis */
    CREDO_NECESSE(_indicem_ponere(gitdir, duo, II, II, FALSUM,
        piscina));
    CREDO_NECESSE(filum_scribere(_serere(piscina, gitdir, "/HEAD", ""),
        filum_legere_totum(_serere(piscina, m->repositorium->via_git,
            "/HEAD", ""), piscina)));
    CREDO_NECESSE(filum_scribere_literis(_serere(piscina, gitdir,
        "/commondir", ""), _serere(piscina,
        m->repositorium->via_communis, "\n", "")));
    CREDO_NECESSE(filum_scribere_literis(_serere(piscina, arbor,
        "/.git", ""), _serere(piscina, "gitdir: ", gitdir, "\n")));
    r = git_aperire(piscina, arbor);
    CREDO_NECESSE_NON_NIHIL(r);
    index = git_indicem_legere(r, piscina);
    CREDO_NECESSE_NON_NIHIL(index);
    CREDO_AEQUALIS_I32(xar_numerus(index), II);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "apertionem", _sectio_apertio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "resolutionem refum", _sectio_resolutio, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "arborem operis ('.git' plagula, commondir)",
        _sectio_arbor_operis,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "commissum", _sectio_commissum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "indicem (oraculum git ls-files -s)", _sectio_index, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "indicem fictum (II, III, gradus, extensio, recusationes, arbor "
      "operis)", _sectio_index_fictus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arborem", _sectio_arbor, NIHIL, NIHIL, NIHIL, NIHIL },
    { "massam per viam (oraculum disci)", _sectio_massa, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "historiam profundam (sarcinae)", _sectio_historia, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "sha per viam (cribrum sine contento)", _sectio_sha_per_viam,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "differentiam arborum", _sectio_differentia, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("git", SECTIONES, _mundum_parare,
        NIHIL);
}
