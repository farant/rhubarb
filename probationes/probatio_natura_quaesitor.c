/* ==================================================
 * probatio_natura_quaesitor.c - porta nativitatis bin/natura
 *
 * OCCASIO (2026-08-08): candidatus 'mensuratio' liber visus est
 * quia inspectio per 'head -c' truncata tag speciei absciderat.
 * Instrumentum ideo SENTENTIAM reddit, non lineas solas - et
 * haec probatio culpam illam ipsam plantat: nomen quod SOLUM ut
 * species exsistit OCCUPATUM nuntiari debet.
 *
 * Mos probationis: probatio_natura_glossae (processus_exsequi
 * super binarium, assertiones in effusione).
 * ================================================== */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "filum.h"
#include "processus.h"
#include <stdio.h>

interior ProcessusResultus _currere(constans character* arg1,
                                    constans character* arg2,
                                    constans character* arg3,
                                    Piscina* piscina);
interior b32 _continet_literis(chorda effusio,
                               constans character* literae,
                               Piscina* piscina);

interior ProcessusResultus
_currere(
    constans character*  arg1,
    constans character*  arg2,
    constans character*  arg3,
    Piscina*             piscina)
{
    constans character* argumenta[VI];
    i32                 n;

    n = ZEPHYRUM;
    argumenta[n++] = "bin/natura";
    si (arg1) { argumenta[n++] = arg1; }
    si (arg2) { argumenta[n++] = arg2; }
    si (arg3) { argumenta[n++] = arg3; }
    argumenta[n] = NIHIL;

    redde processus_exsequi(argumenta, 60000, piscina);
}

interior b32
_continet_literis(
    chorda               effusio,
    constans character*  literae,
    Piscina*             piscina)
{
    redde chorda_continet(effusio,
                          chorda_ex_literis(literae, piscina));
}

s32 principale (vacuum)
{
         b32  praeteritus;
    Piscina*  piscina;

    piscina = piscina_generare_dynamicum("probatio_natura_quaesitor",
                                         8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * 0. instrumentum paratur
     * ======================================================== */
    {
        imprimere("\n--- 0. instrumentum ---\n");

        si (!filum_existit("bin/natura"))
        {
            constans character* struere[II];
            ProcessusResultus   rs;

            imprimere("  (bin/natura abest - struo semel)\n");
            struere[ZEPHYRUM] = "./tools/natura_struere.sh";
            struere[I]        = NIHIL;
            rs = processus_exsequi(struere, 300000, piscina);
            si (!rs.successus || rs.codex_exitus != ZEPHYRUM)
            {
                imprimere("  structor fefellit (codex %u)\n",
                          rs.codex_exitus);
            }
        }
        CREDO_VERUM (filum_existit("bin/natura"));
    }

    /* ========================================================
     * I. CULPA PLANTATA: nomen quod SOLUM ut species exsistit
     *
     * Haec est causa instrumenti. Si haec assertio rubet,
     * custos collisionum mortuus est et nomina occupata libera
     * videbuntur - exacte quod 2026-08-08 accidit.
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- I. culpa plantata (species latens) ---\n");

        r = _currere("quaere", "mensuratio", NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, (i32)ZEPHYRUM);
        CREDO_VERUM (_continet_literis(r.effusio, "OCCUPATUM",
                                       piscina));
        /* genus continens NOMINATUR - sine eo lector nescit ubi
         * nomen sedeat */
        CREDO_VERUM (_continet_literis(r.effusio, "fundamentum",
                                       piscina));
    }

    /* ========================================================
     * II. nomen vere liberum: SENTENTIA contraria
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- II. nomen liberum ---\n");

        r = _currere("quaere", "zzz-nomen-quod-non-est", NIHIL,
                     piscina);
        CREDO_VERUM (r.successus);
        /* quaestio sine congruentia RESPONSUM est, non defectus */
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, (i32)ZEPHYRUM);
        CREDO_VERUM (_continet_literis(r.effusio, "LIBERUM",
                                       piscina));
        CREDO_VERUM (!_continet_literis(r.effusio, "OCCUPATUM",
                                        piscina));
    }

    /* ========================================================
     * III. genus nomine ipso quoque OCCUPATUM nuntiatur
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- III. genus nomine ipso ---\n");

        r = _currere("quaere", "praeceptum", NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (_continet_literis(r.effusio, "OCCUPATUM",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio, "iudicium",
                                       piscina));
    }

    /* ========================================================
     * IV. apparatus hereditatem fert (trans exemplaria)
     *
     * deceptio sub defectus_documenti sedet: contentum et
     * in_opere a parente veniunt, ideo [hereditarium].
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- IV. apparatus cum hereditate ---\n");

        r = _currere("apparatus", "deceptio", NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, (i32)ZEPHYRUM);
        CREDO_VERUM (_continet_literis(r.effusio, "contentum",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio, "in_opere",
                                       piscina));
        /* auctor hereditatis NOMINATUR (non solum "hereditarium"
         * nudum): lector scit UBI membrum definitum sit */
        CREDO_VERUM (_continet_literis(
            r.effusio, "[a documentatio.defectus_documenti]",
            piscina));
    }

    /* ========================================================
     * V. maiores trans exemplaria (documentatio -> fabricatio)
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- V. maiores trans exemplaria ---\n");

        r = _currere("maiores", "ambulatio_lectoris", NIHIL,
                     piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (_continet_literis(r.effusio, "fabricatio",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio, "probatio",
                                       piscina));
    }

    /* ========================================================
     * VI. DISCIPLINA EXITUUM: 2 = NIHIL CURSUM, non salus
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- VI. disciplina exituum ---\n");

        /* genus ignotum */
        r = _currere("apparatus", "zzz-genus-nullum", NIHIL,
                     piscina);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, (i32)II);

        /* mandatum ignotum */
        r = _currere("fabulari", "quidlibet", NIHIL, piscina);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, (i32)II);

        /* radix sine exemplaribus - PORTA VACUA fiduciam
         * fabricaret nisi hic rubesceret */
        r = _currere("index", "-radix", "/tmp", piscina);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, (i32)II);
    }

    /* ========================================================
     * VII. modus machinae: SENTENTIA linea propria
     *
     * Custos truncationis: sententia ULTIMA linea est, ideo
     * etiam 'tail -1' responsum servat.
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- VII. modus machinae ---\n");

        r = _currere("quaere", "mensuratio", "-machina", piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (_continet_literis(r.effusio,
                                       "SENTENTIA\tOCCUPATUM",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio, "RES\tiudicium",
                                       piscina));
    }

    /* ========================================================
     * VIII. UMBRAE: tertius status sententiae
     *
     * Umbra nomen TENET nec describitur. Ideo nec OCCUPATUM
     * (nullum ens id fert) nec LIBERUM (adhibetur): condere eo
     * nomine umbram IMPLET. Sententia binaria hic mentiretur.
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- VIII. umbrae (sententia triplex) ---\n");

        r = _currere("quaere", "typus", NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, (i32)ZEPHYRUM);
        CREDO_VERUM (_continet_literis(r.effusio, "UMBRATUM",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio, "superfectum",
                                       piscina));
        /* umbra eadem multis locis citatur - SEMEL nuntianda */
        CREDO_AEQUALIS_I32 (
            chorda_numerare_occurrentia(
                r.effusio,
                chorda_ex_literis("UMBRA [NOMEN IPSUM]", piscina)),
            (i32)I);
    }

    /* ========================================================
     * IX. APPARATUS DIVES (munus a natura_quaere.sh migratum):
     * optiones electionis, status machinae, necessitas partium,
     * petitum relationis, catena in arbore, species.
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- IX. apparatus dives ---\n");

        r = _currere("apparatus", "planta.planta", NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, (i32)ZEPHYRUM);

        /* forma 'modulus.genus' quoque valet (memoria manuum) */
        CREDO_VERUM (_continet_literis(r.effusio, "/planta/planta",
                                       piscina));
        /* optiones electionis NOMINANTUR */
        CREDO_VERUM (_continet_literis(r.effusio,
                                       "herba|frutex|arbor",
                                       piscina));
        /* necessitas partium */
        CREDO_VERUM (_continet_literis(r.effusio, "[NECESSARIA]",
                                       piscina));
        /* machina statuum cum statibus */
        CREDO_VERUM (_continet_literis(r.effusio, "quo statu",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio, "germinans",
                                       piscina));
        /* catena hereditatis trans exemplaria */
        CREDO_VERUM (_continet_literis(r.effusio, "IN ARBORE",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
                                       "corporalia.vivens",
                                       piscina));
    }

    /* ========================================================
     * IXb. species enumerantur (genus quod eas RECTA habet -
     * plantae species sub sub-generibus sedent, non sub ipsa)
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- IXb. species enumeratae ---\n");

        r = _currere("apparatus", "observatio", NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (_continet_literis(r.effusio, "QUAE SPECIES SIT",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio, "symptoma",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio, "testimonium",
                                       piscina));
    }

    /* ========================================================
     * X. petitum relationis NOMINATUR (non solum nomen relationis)
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- X. petita relationum ---\n");

        r = _currere("apparatus", "ambulatio_lectoris", NIHIL,
                     piscina);
        CREDO_VERUM (r.successus);
        /* clausura restricta: opus_scriptum nominatum */
        CREDO_VERUM (_continet_literis(r.effusio, "exercet ->",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
                                       "opus_scriptum.opus_scriptum",
                                       piscina));
        /* clausura aperta legitur '(quidlibet)', non '*' nudum */
        CREDO_VERUM (_continet_literis(r.effusio, "(quidlibet)",
                                       piscina));
        /* multiplex notatur */
        CREDO_VERUM (_continet_literis(r.effusio, "[multiplex]",
                                       piscina));
    }

    /* ========================================================
     * XI. TAXINOMIA NIDIFICATA (2026-08-08): species speciem
     * continere potest. Ante hoc graphus id planabat, ergo
     * 'intra isbn' dici non poterat - res ad genus ambiens
     * solum referebatur.
     * ======================================================== */
    {
        ProcessusResultus r;

        imprimere("\n--- XI. taxinomia nidificata ---\n");

        /* species INTRA speciem */
        r = _currere("quaere", "isbn_10", NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (_continet_literis(r.effusio, "intra isbn",
                                       piscina));
        /* utraque catena dicitur: continens ET genus */
        CREDO_VERUM (_continet_literis(
            r.effusio, "in genere schema_identificandi", piscina));

        /* cultivar intra speciem, arbore inclusa */
        r = _currere("apparatus", "malus", NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_VERUM (_continet_literis(r.effusio, "malus_domestica",
                                       piscina));
        CREDO_VERUM (_continet_literis(r.effusio, "> granny_smith",
                                       piscina));
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
