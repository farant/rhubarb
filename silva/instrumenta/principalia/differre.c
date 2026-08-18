/* differre.c - differentia semantica unitatum C89 (instrumenta)
 *
 * Quaestio: QUID mutatum est inter duas versiones plagulae C89 -
 * in unitatibus quibus cogitas (functiones, typi, macra, data),
 * non in lineis nudis. Lexemate fundatum (silva_lexare +
 * silva_unitates): etiam plagulae quae non parsantur differuntur.
 *
 * Algorithmus:
 *   1. lexare ambas, unitates scandere, spatia byte PARTIRI:
 *      initium unitatis = trivia praecedentia lexematis primi
 *      (commentarium ducens ad unitatem sequentem pertinet, regula
 *      attachmenti lexatoris); primum ab 0, ultimum ad finem -
 *      omnis byte unitati uni pertinet.
 *   2. sigillum spatii cuiusque, identitates internatae ->
 *      differentia_seriei super UNITATES: nucleus idem qui lineas
 *      differt, granularitate tertia (unitas / linea / lexema).
 *   3. IDEM = immotae (tacent). DELETA+ADDITA sigillo aequali =
 *      MOTA (contentum idem, sede alia - quod differentia linearum
 *      structuraliter caeca est); titulo aequali = MUTATA; ceterae
 *      REMOTA / ADDITA.
 *   4. MUTATA classificantur: substantiva (lexemata codicis
 *      differunt) / documentaria (commenta sola differunt) /
 *      cosmetica (spatia alba sola). Textus = differentia linearum
 *      spatii ipsius (contextus III) - homo lineas legit, lexemata
 *      classificationi serviunt.
 *
 * Usus:   ./silva/differre.sh <vetus.c> <novum.c> [-machina]
 *         ./silva/differre.sh -git <via> [ref_vetus] [ref_novum]
 *           modus git NATIVUS (bibliotheca git, non subprocessus):
 *           ref_vetus ordinarius CAPUT; ref_novum absens = discus.
 *           Plagula ad ref absens = latus vacuum (honestum).
 *         ./silva/differre.sh -commissum [ref | vetus novus]
 *           commissum TOTUM in symbolis ('git log --stat' gradu
 *           unitatum): sine refs = CAPUT contra parentem; ref una
 *           = ea contra parentem; duae = arbor contra arborem.
 *           Plagulae C unitatim (ordines soli, sine corporibus -
 *           corpus petis per '-git <via> <vetus> <novus>');
 *           ceterae summa linearum; binariae notatae.
 *         ./silva/differre.sh -historia <via> <titulus> [ref]
 *           historia symboli ("quando functio X mutata est - et
 *           QUOMODO"): ambulatio parentum primorum a ref (CAPUT
 *           ordinarius), cribrum duplici gradu - sha massae
 *           plagulae primum (plagula intacta = zerum parsationis),
 *           sigillum spatiorum deinde. Unitates OMNES titulo
 *           congruentes una identitate vehuntur (prototypum +
 *           definitio simul). Eventa MUTATA/ADDITA/REMOTA cum
 *           classificatione et corpore unificato; -summa =
 *           ordines eventuum soli.
 * Exitus: 0 = cucurrit (differentia fractura NON est) |
 *         2 = usus malus aut plagula illegibilis
 * -machina: TSV genus, titulus, status, classificatio, +N, -M
 *           (sine textu; par fistulis - nexus.sh, vocantes);
 *           modo -commissum columna VIae praefigitur et plagulae
 *           non-C ordines 'plagula' accipiunt; modo -historia:
 *           sha, dies, status, classificatio, +N, -M, subiectum
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "sigillum.h"
#include "filum.h"
#include "differentia.h"
#include "git.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_unitates.h"
#include "silva_differre.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

/* machina differentiae (latus/paria/emissio/classificatio) in
 * silva_differre.{h,c} extracta 2026-08-18 - silex eandem fert.
 * Hic manent: modi (plagulae/git/commissum/historia) = fontes
 * contentorum, et CLI. */

interior b32
_latus_parare (Piscina* piscina, InternamentumChorda* intern,
    constans character* via, SilvaDifferreLatus* latus);

interior b32
_latus_parare (Piscina* piscina, InternamentumChorda* intern,
    constans character* via, SilvaDifferreLatus* latus)
{
    si (!filum_existit(via))
    {
        fprintf(stderr, "differre: plagula non exsistit: %s\n",
            via);
        redde FALSUM;
    }
    redde silva_differre_latus_ex_textu(piscina, intern,
        filum_legere_totum(via, piscina), via, latus);
}

interior b32
_via_est_c (chorda via);

interior b32
_via_est_c (chorda via)
{
    character ultima;

    si (via.mensura < 2 || via.datum[via.mensura - 2] != '.')
    {
        redde FALSUM;
    }
    ultima = (character)via.datum[via.mensura - 1];
    redde ultima == 'c' || ultima == 'h';
}

/* massa e git per sha (chorda hex 40); vacua si sha vacua aut
 * obiectum non massa */
interior chorda
_massam_ex_git (GitRepositorium* repositorium, chorda sha,
    Piscina* piscina);

interior chorda
_massam_ex_git (GitRepositorium* repositorium, chorda sha,
    Piscina* piscina)
{
    chorda      vacua;
    GitObiectum obiectum;

    vacua.datum = NIHIL;
    vacua.mensura = 0;
    si (sha.mensura != 40)
    {
        redde vacua;
    }
    obiectum = git_obiectum_legere(repositorium,
        chorda_ut_cstr(sha, piscina), piscina);
    si (!obiectum.successus
        || obiectum.genus != GIT_OBIECTUM_MASSA)
    {
        redde vacua;
    }
    redde obiectum.datum;
}

interior b32
_est_binaria (chorda textus);

interior b32
_est_binaria (chorda textus)
{
    redde textus.mensura > 0
        && memchr(textus.datum, 0,
               (memoriae_index)textus.mensura) != NIHIL;
}

/* --------------------------------------------------
 * modus -historia: symbolum per commissa sequi
 * -------------------------------------------------- */

nomen structura {
    b32    inventa;       /* unitas ulla titulo data */
    chorda textus;        /* spatia concatenata (piscina AMBULATIONIS) */
    chorda sigillum_hex;  /* identitas contenti (64 hex) */
} HistoriaLatus;

/* massam parsare piscina BREVI (deleta ante reditum - memoria
 * plana trans ambulationem totam), spatia unitatum titulo
 * congruentium in piscinam ambulationis concatenare. Unitates
 * OMNES titulo (prototypum + definitio una identitate).
 * sha_massae NIHIL = plagula ad commissum absens (latus vacuum
 * honestum). FALSUM = defectus verus. */
interior b32
_historia_latus_legere (GitRepositorium* repositorium,
    constans character* sha_massae, constans character* titulus,
    Piscina* ambulationis, HistoriaLatus* exitus);

interior b32
_historia_latus_legere (GitRepositorium* repositorium,
    constans character* sha_massae, constans character* titulus,
    Piscina* ambulationis, HistoriaLatus* exitus)
{
    Piscina*             temporaria;
    InternamentumChorda* intern_brevis;
    GitObiectum          obiectum;
    SilvaDifferreLatus   latus;
    ChordaAedificator*   aed;
    i32                  k;

    exitus->inventa = FALSUM;
    exitus->textus.datum = NIHIL;
    exitus->textus.mensura = 0;
    exitus->sigillum_hex = exitus->textus;
    si (sha_massae == NIHIL)
    {
        redde VERUM;
    }
    temporaria = piscina_generare_dynamicum("differre_historia",
        1048576);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    obiectum = git_obiectum_legere(repositorium, sha_massae,
        temporaria);
    intern_brevis = internamentum_creare(temporaria);
    si (!obiectum.successus
        || obiectum.genus != GIT_OBIECTUM_MASSA
        || intern_brevis == NIHIL
        || !silva_differre_latus_ex_textu(temporaria,
               intern_brevis, obiectum.datum, "historia", &latus))
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    aed = chorda_aedificator_creare(ambulationis,
        (memoriae_index)256);
    si (aed == NIHIL)
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    per (k = 0; k < latus.numerus; k = k + 1)
    {
        constans SilvaUnitas* u = (constans SilvaUnitas*)
            xar_obtinere(latus.unitates, k);

        si (chorda_aequalis_literis(u->titulus, titulus))
        {
            chorda_aedificator_appendere_chorda(aed,
                silva_differre_spatium(&latus, k));
            exitus->inventa = VERUM;
        }
    }
    piscina_destruere(temporaria);
    si (!exitus->inventa)
    {
        redde VERUM;
    }
    exitus->textus = chorda_aedificator_finire(aed);
    {
        Sigillum  sig;
        character hex[SIGILLUM_HEX_MENSURA];
        i8*       datum = (i8*)piscina_allocare(ambulationis,
            (memoriae_index)64);

        si (datum == NIHIL)
        {
            redde FALSUM;
        }
        sig = sigillum_computare(
            (constans vacuum*)exitus->textus.datum,
            (memoriae_index)exitus->textus.mensura);
        sigillum_hex(&sig, hex);
        memcpy(datum, hex, (memoriae_index)64);
        exitus->sigillum_hex = chorda_ex_buffer(datum, 64);
    }
    redde VERUM;
}

/* dies ex epocha commissoris; exitus >= 16 octeti */
interior vacuum
_diem_scribere (s64 tempus, character* exitus);

interior vacuum
_diem_scribere (s64 tempus, character* exitus)
{
    time_t         t = (time_t)tempus;
    structura tm*  fractum = gmtime(&t);

    si (fractum == NIHIL
        || strftime(exitus, (memoriae_index)16, "%Y-%m-%d",
               fractum) == 0)
    {
        strcpy(exitus, "0000-00-00");
    }
}

/* linea prima nuntii commissi */
interior chorda
_subiectum (chorda nuntius);

interior chorda
_subiectum (chorda nuntius)
{
    i32 k;

    per (k = 0; k < nuntius.mensura; k = k + 1)
    {
        si (nuntius.datum[k] == '\n')
        {
            frange;
        }
    }
    redde chorda_ex_buffer(nuntius.datum, k);
}

/* eventum unum historiae emittere: vetus = latus parentis, novus
 * = latus commissi mutantis */
interior vacuum
_historia_eventum_emittere (Piscina* piscina,
    constans character* sha_commissi,
    constans GitCommissum* commissum, constans character* status,
    constans HistoriaLatus* vetus, constans HistoriaLatus* novus,
    b32 machina, b32 summa_modus, constans character* titulus);

interior vacuum
_historia_eventum_emittere (Piscina* piscina,
    constans character* sha_commissi,
    constans GitCommissum* commissum, constans character* status,
    constans HistoriaLatus* vetus, constans HistoriaLatus* novus,
    b32 machina, b32 summa_modus, constans character* titulus)
{
    DifferentiaSumma    summa;
    constans character* classificatio = "-";
    character           dies[16];
    chorda              subiectum = _subiectum(commissum->nuntius);

    summa = silva_differre_summa_textuum(piscina, vetus->textus,
        novus->textus);
    si (strcmp(status, "MUTATA") == 0)
    {
        classificatio = silva_differre_classificare_textus(
            piscina, vetus->textus, novus->textus);
    }
    _diem_scribere(commissum->tempus, dies);
    si (machina)
    {
        imprimere("%.7s\t%s\t%s\t%s\t+%d\t-%d\t", sha_commissi,
            dies, status, classificatio, (integer)summa.additae,
            (integer)summa.deletae);
        silva_differre_titulum_imprimere(subiectum);
        imprimere("\n");
        redde;
    }
    imprimere("== %.7s %s  %s", sha_commissi, dies, status);
    si (strcmp(status, "MUTATA") == 0)
    {
        imprimere("  [%s]", classificatio);
    }
    imprimere("  +%d -%d\n   ", (integer)summa.additae,
        (integer)summa.deletae);
    silva_differre_titulum_imprimere(subiectum);
    imprimere("\n");
    si (!summa_modus)
    {
        DifferentiaLinearum* d = differentia_linearum(piscina,
            vetus->textus, novus->textus);

        si (d != NIHIL)
        {
            chorda textus = differentia_unificata(piscina, d,
                silva_differre_titulum_cstr(piscina, "a/",
                    chorda_ex_literis(titulus, piscina)),
                silva_differre_titulum_cstr(piscina, "b/",
                    chorda_ex_literis(titulus, piscina)),
                (i32)3);

            imprimere("%.*s", (integer)textus.mensura,
                (constans character*)textus.datum);
        }
    }
    imprimere("\n");
}

s32 principale (integer argc, character** argv)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    SilvaDifferreLatus   a;
    SilvaDifferreLatus   b;
    constans character*  via_a = NIHIL;
    constans character*  via_b = NIHIL;
    constans character*  positio_tertia = NIHIL;
    b32                  machina = FALSUM;
    b32                  git_modus = FALSUM;
    b32                  commissum_modus = FALSUM;
    b32                  historia_modus = FALSUM;
    b32                  summa_modus = FALSUM;
    Xar*                 paria;
    i32                  immotae = 0;
    i32                  additae_totae = 0;
    i32                  deletae_totae = 0;
    i32                  k;
    integer              arg;

    per (arg = 1; arg < argc; arg = arg + 1)
    {
        si (strcmp(argv[arg], "-machina") == 0)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[arg], "-git") == 0)
        {
            git_modus = VERUM;
        }
        alioquin si (strcmp(argv[arg], "-commissum") == 0)
        {
            commissum_modus = VERUM;
        }
        alioquin si (strcmp(argv[arg], "-historia") == 0)
        {
            historia_modus = VERUM;
        }
        alioquin si (strcmp(argv[arg], "-summa") == 0)
        {
            summa_modus = VERUM;
        }
        alioquin si (via_a == NIHIL)
        {
            via_a = argv[arg];
        }
        alioquin si (via_b == NIHIL)
        {
            via_b = argv[arg];
        }
        alioquin si (positio_tertia == NIHIL)
        {
            positio_tertia = argv[arg];
        }
    }
    {
        i32 modi = (git_modus ? (i32)1 : (i32)0)
            + (commissum_modus ? (i32)1 : (i32)0)
            + (historia_modus ? (i32)1 : (i32)0);
        b32 usus_malus = modi > (i32)1;

        si (!usus_malus)
        {
            si (historia_modus || (!git_modus && !commissum_modus))
            {
                usus_malus = via_a == NIHIL || via_b == NIHIL;
            }
            alioquin si (git_modus)
            {
                usus_malus = via_a == NIHIL;
            }
        }
        si (usus_malus)
        {
            fprintf(stderr, "usus: differre <vetus.c> <novum.c>"
                " [-machina]\n"
                "      differre -git <via> [ref_vetus]"
                " [ref_novum] [-machina]\n"
                "      differre -commissum [ref | vetus novus]"
                " [-machina]\n"
                "      differre -historia <via> <titulus> [ref]"
                " [-summa] [-machina]\n");
            redde II;
        }
    }

    piscina = piscina_generare_dynamicum("differre", 1048576);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "differre: piscina generari non potuit\n");
        redde II;
    }
    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde II;
    }
    si (historia_modus)
    {
        /* ambulatio parentum primorum, cribrum duplici gradu:
         * sha massae plagulae (gratis), sigillum spatiorum
         * (parsatio solum ubi plagula mutata) */
        GitRepositorium*    repositorium = git_aperire(piscina,
            ".");
        constans character* ref = positio_tertia != NIHIL
            ? positio_tertia : "HEAD";
        character           sha_currentis[GIT_SHA_HEX_MENSURA];
        GitCommissum        commissum_currens;
        character           sha_massae_currentis[
            GIT_SHA_HEX_MENSURA];
        b32                 habet_currens = FALSUM;
        HistoriaLatus       latus_currens;
        i32                 eventa = 0;

        si (repositorium == NIHIL)
        {
            fprintf(stderr, "differre: non in repositorio git\n");
            redde II;
        }
        si (!git_ref_resolvere(repositorium, ref, sha_currentis))
        {
            fprintf(stderr, "differre: ref non resolutum: %s\n",
                ref);
            redde II;
        }
        commissum_currens = git_commissum_legere(repositorium,
            sha_currentis, piscina);
        si (!commissum_currens.successus)
        {
            fprintf(stderr, "differre: commissum legi non"
                " potuit\n");
            redde II;
        }
        {
            b32 est_arbor = FALSUM;

            sha_massae_currentis[0] = '\0';
            habet_currens = git_sha_per_viam(repositorium,
                sha_currentis, via_a, piscina,
                sha_massae_currentis, &est_arbor) && !est_arbor;
        }
        si (!_historia_latus_legere(repositorium,
            habet_currens ? sha_massae_currentis : NIHIL, via_b,
            piscina, &latus_currens))
        {
            fprintf(stderr, "differre: latus legi non potuit ad"
                " %.7s\n", sha_currentis);
            redde II;
        }
        si (!machina)
        {
            imprimere("historia symboli '%s' in %s (a %.7s)\n\n",
                via_b, via_a, sha_currentis);
            si (!latus_currens.inventa)
            {
                imprimere("(symbolum ad %.7s non exsistit -"
                    " ambulatio pergit, fortasse olim fuit)\n\n",
                    sha_currentis);
            }
        }

        dum (VERUM)
        {
            character     sha_parentis[GIT_SHA_HEX_MENSURA];
            GitCommissum  commissum_parentis;
            character     sha_massae_parentis[
                GIT_SHA_HEX_MENSURA];
            b32           habet_parens = FALSUM;
            HistoriaLatus latus_parentis;
            b32           plagula_mutata;

            si (xar_numerus(commissum_currens.parentes) == 0)
            {
                /* radix historiae: symbolum praesens = natum hic */
                si (latus_currens.inventa)
                {
                    HistoriaLatus vacuum_latus;

                    vacuum_latus.inventa = FALSUM;
                    vacuum_latus.textus.datum = NIHIL;
                    vacuum_latus.textus.mensura = 0;
                    vacuum_latus.sigillum_hex = vacuum_latus.textus;
                    _historia_eventum_emittere(piscina,
                        sha_currentis, &commissum_currens,
                        "ADDITA", &vacuum_latus, &latus_currens,
                        machina, summa_modus, via_b);
                    eventa = eventa + 1;
                }
                frange;
            }
            memcpy(sha_parentis, chorda_ut_cstr(
                *(chorda*)xar_obtinere(
                    commissum_currens.parentes, 0), piscina),
                (memoriae_index)41);
            commissum_parentis = git_commissum_legere(
                repositorium, sha_parentis, piscina);
            si (!commissum_parentis.successus)
            {
                fprintf(stderr, "differre: commissum legi non"
                    " potuit: %.7s\n", sha_parentis);
                redde II;
            }
            {
                b32 est_arbor = FALSUM;

                sha_massae_parentis[0] = '\0';
                habet_parens = git_sha_per_viam(repositorium,
                    sha_parentis, via_a, piscina,
                    sha_massae_parentis, &est_arbor) && !est_arbor;
            }
            plagula_mutata = habet_currens != habet_parens
                || (habet_currens
                       && strcmp(sha_massae_currentis,
                              sha_massae_parentis) != 0);
            si (!plagula_mutata)
            {
                /* cribrum gradus primi: plagula intacta */
                latus_parentis = latus_currens;
            }
            alioquin
            {
                si (!_historia_latus_legere(repositorium,
                    habet_parens ? sha_massae_parentis : NIHIL,
                    via_b, piscina, &latus_parentis))
                {
                    fprintf(stderr, "differre: latus legi non"
                        " potuit ad %.7s\n", sha_parentis);
                    redde II;
                }
                /* cribrum gradus secundi: sigilla spatiorum */
                si (latus_currens.inventa != latus_parentis.inventa
                    || (latus_currens.inventa
                           && !chorda_aequalis(
                                  latus_currens.sigillum_hex,
                                  latus_parentis.sigillum_hex)))
                {
                    constans character* status =
                        !latus_parentis.inventa ? "ADDITA"
                        : !latus_currens.inventa ? "REMOTA"
                        : "MUTATA";

                    _historia_eventum_emittere(piscina,
                        sha_currentis, &commissum_currens, status,
                        &latus_parentis, &latus_currens, machina,
                        summa_modus, via_b);
                    eventa = eventa + 1;
                }
            }
            /* descendere */
            memcpy(sha_currentis, sha_parentis,
                (memoriae_index)41);
            commissum_currens = commissum_parentis;
            memcpy(sha_massae_currentis, sha_massae_parentis,
                (memoriae_index)41);
            habet_currens = habet_parens;
            latus_currens = latus_parentis;
        }
        si (!machina)
        {
            imprimere("historia: %d eventa\n", (integer)eventa);
        }
        redde ZEPHYRUM;
    }
    si (commissum_modus)
    {
        /* commissum TOTUM: arbores differuntur (shas subarborum -
         * contentum tangitur solum ubi mutatum), plagulae C
         * unitatim, ceterae summa linearum */
        GitRepositorium* repositorium = git_aperire(piscina, ".");
        character        sha_veteris[GIT_SHA_HEX_MENSURA];
        character        sha_novi[GIT_SHA_HEX_MENSURA];
        GitCommissum     vetus_commissum;
        GitCommissum     novum_commissum;
        Xar*             mutatae;
        i32              paria_totae = 0;

        si (repositorium == NIHIL)
        {
            fprintf(stderr, "differre: non in repositorio git\n");
            redde II;
        }
        si (via_a != NIHIL && via_b != NIHIL)
        {
            /* refs duae: arbor contra arborem */
            si (!git_ref_resolvere(repositorium, via_a,
                sha_veteris))
            {
                fprintf(stderr, "differre: ref non resolutum:"
                    " %s\n", via_a);
                redde II;
            }
            si (!git_ref_resolvere(repositorium, via_b, sha_novi))
            {
                fprintf(stderr, "differre: ref non resolutum:"
                    " %s\n", via_b);
                redde II;
            }
        }
        alioquin
        {
            /* nulla aut una: commissum contra parentem primum */
            constans character* ref = via_a != NIHIL ? via_a
                : "HEAD";

            si (!git_ref_resolvere(repositorium, ref, sha_novi))
            {
                fprintf(stderr, "differre: ref non resolutum:"
                    " %s\n", ref);
                redde II;
            }
            novum_commissum = git_commissum_legere(repositorium,
                sha_novi, piscina);
            si (!novum_commissum.successus
                || xar_numerus(novum_commissum.parentes) == 0)
            {
                fprintf(stderr, "differre: commissum sine"
                    " parente: %s\n", ref);
                redde II;
            }
            memcpy(sha_veteris, chorda_ut_cstr(
                *(chorda*)xar_obtinere(novum_commissum.parentes,
                    0), piscina), (memoriae_index)41);
        }
        vetus_commissum = git_commissum_legere(repositorium,
            sha_veteris, piscina);
        novum_commissum = git_commissum_legere(repositorium,
            sha_novi, piscina);
        si (!vetus_commissum.successus
            || !novum_commissum.successus)
        {
            fprintf(stderr, "differre: commissum legi non"
                " potuit\n");
            redde II;
        }
        mutatae = git_arbores_differre(repositorium,
            chorda_ut_cstr(vetus_commissum.arbor, piscina),
            chorda_ut_cstr(novum_commissum.arbor, piscina),
            piscina);
        si (mutatae == NIHIL)
        {
            fprintf(stderr, "differre: arbores differri non"
                " potuerunt\n");
            redde II;
        }
        si (!machina)
        {
            imprimere("commissum %.7s contra %.7s: %d plagulae"
                " mutatae\n\n", sha_veteris, sha_novi,
                (integer)xar_numerus(mutatae));
        }
        per (k = 0; k < xar_numerus(mutatae); k = k + 1)
        {
            GitViaMutata* m = (GitViaMutata*)xar_obtinere(mutatae,
                k);
            constans character* via_cstr = chorda_ut_cstr(m->via,
                piscina);
            constans character* status_plagulae =
                m->genus == GIT_VIA_ADDITA ? "ADDITA"
                : m->genus == GIT_VIA_REMOTA ? "REMOTA"
                : "MUTATA";
            chorda textus_vetus = _massam_ex_git(repositorium,
                m->sha_vetus, piscina);
            chorda textus_novus = _massam_ex_git(repositorium,
                m->sha_novus, piscina);
            b32 binaria = _est_binaria(textus_vetus)
                || _est_binaria(textus_novus);

            si (_via_est_c(m->via) && !binaria)
            {
                SilvaDifferreLatus la;
                SilvaDifferreLatus lb;
                Xar*               paria_plagulae;

                si (!silva_differre_latus_ex_textu(piscina,
                        intern, textus_vetus, via_cstr, &la)
                    || !silva_differre_latus_ex_textu(piscina,
                           intern, textus_novus, via_cstr, &lb))
                {
                    redde II;
                }
                paria_plagulae = silva_differre_paria(piscina,
                    &la, &lb, &immotae);
                si (paria_plagulae == NIHIL)
                {
                    redde II;
                }
                si (!machina)
                {
                    imprimere("== %s  %s\n", via_cstr,
                        status_plagulae);
                }
                silva_differre_paria_emittere(piscina, &la, &lb,
                    paria_plagulae, machina,
                    machina ? via_cstr : NIHIL, FALSUM,
                    &additae_totae, &deletae_totae);
                paria_totae = paria_totae
                    + xar_numerus(paria_plagulae);
                si (!machina)
                {
                    imprimere("\n");
                }
            }
            alioquin
            {
                DifferentiaSumma summa;

                summa.additae = 0;
                summa.deletae = 0;
                si (!binaria)
                {
                    summa = silva_differre_summa_textuum(piscina,
                        textus_vetus, textus_novus);
                }
                additae_totae = additae_totae
                    + (i32)summa.additae;
                deletae_totae = deletae_totae
                    + (i32)summa.deletae;
                si (machina)
                {
                    imprimere(
                        "%s\tplagula\t-\t%s\t%s\t+%d\t-%d\n",
                        via_cstr, status_plagulae,
                        binaria ? "binaria" : "-",
                        (integer)summa.additae,
                        (integer)summa.deletae);
                }
                alioquin si (binaria)
                {
                    imprimere("== %s  %s  (binaria)\n\n",
                        via_cstr, status_plagulae);
                }
                alioquin
                {
                    imprimere("== %s  %s  +%d -%d\n\n", via_cstr,
                        status_plagulae, (integer)summa.additae,
                        (integer)summa.deletae);
                }
            }
        }
        si (!machina)
        {
            imprimere("differre: %d plagulae, %d paria unitatum"
                " (+%d -%d)\n", (integer)xar_numerus(mutatae),
                (integer)paria_totae, (integer)additae_totae,
                (integer)deletae_totae);
        }
        redde ZEPHYRUM;
    }
    si (git_modus)
    {
        /* latera e bibliotheca git NATIVA - nullus subprocessus */
        GitRepositorium*    repositorium = git_aperire(piscina,
            ".");
        character           sha[GIT_SHA_HEX_MENSURA];
        constans character* ref_vetus = via_b != NIHIL ? via_b
            : "HEAD";
        chorda              textus_vetus;
        chorda              textus_novum;
        b32                 inventum = FALSUM;

        si (repositorium == NIHIL)
        {
            fprintf(stderr, "differre: non in repositorio git\n");
            redde II;
        }
        si (!git_ref_resolvere(repositorium, ref_vetus, sha))
        {
            fprintf(stderr, "differre: ref non resolutum: %s\n",
                ref_vetus);
            redde II;
        }
        textus_vetus = git_massam_per_viam(repositorium, sha,
            via_a, piscina, &inventum);
        si (!inventum)
        {
            textus_vetus.datum = NIHIL;
            textus_vetus.mensura = 0;
        }
        si (positio_tertia != NIHIL)
        {
            si (!git_ref_resolvere(repositorium, positio_tertia,
                sha))
            {
                fprintf(stderr, "differre: ref non resolutum:"
                    " %s\n", positio_tertia);
                redde II;
            }
            inventum = FALSUM;
            textus_novum = git_massam_per_viam(repositorium, sha,
                via_a, piscina, &inventum);
            si (!inventum)
            {
                textus_novum.datum = NIHIL;
                textus_novum.mensura = 0;
            }
        }
        alioquin si (filum_existit(via_a))
        {
            textus_novum = filum_legere_totum(via_a, piscina);
        }
        alioquin
        {
            textus_novum.datum = NIHIL;
            textus_novum.mensura = 0;
        }
        si (!silva_differre_latus_ex_textu(piscina, intern,
                textus_vetus, via_a, &a)
            || !silva_differre_latus_ex_textu(piscina, intern,
                   textus_novum, via_a, &b))
        {
            redde II;
        }
    }
    alioquin si (!_latus_parare(piscina, intern, via_a, &a)
        || !_latus_parare(piscina, intern, via_b, &b))
    {
        redde II;
    }

    paria = silva_differre_paria(piscina, &a, &b, &immotae);
    si (paria == NIHIL)
    {
        fprintf(stderr, "differre: differentia computari non"
            " potuit\n");
        redde II;
    }

    /* emissio */
    si (xar_numerus(paria) == 0)
    {
        si (!machina)
        {
            imprimere("differre: nulla differentia (%d unitates"
                " immotae)\n", (integer)immotae);
        }
        redde ZEPHYRUM;
    }
    silva_differre_paria_emittere(piscina, &a, &b, paria, machina,
        NIHIL, VERUM, &additae_totae, &deletae_totae);
    si (!machina)
    {
        imprimere("differre: %d paria (+%d -%d), %d unitates"
            " immotae\n", (integer)xar_numerus(paria),
            (integer)additae_totae, (integer)deletae_totae,
            (integer)immotae);
    }
    redde ZEPHYRUM;
}
