/* silva_emendatio.h - custodia emendationis: textus NOVUS plagulae ANTE
 * scriptionem iudicatur (ex emendare.c extracta, transformare T5; spec
 * project-specs/transformare-spec.md III, N4)
 *
 * Duae portae, ambae sine disco plagulae:
 *   iudicare   - recipe examinis (silva_iudicium) super textum novum:
 *                parsura sine erroribus novis, codex SCOPI exacte
 *                'decrementum' perdit (aut ad nihil), nullus codex
 *                alius crescit, REICE non inducitur;
 *   compilare  - clang -fsyntax-only super textum (plagula temporaria
 *                in $TMPDIR, '-iquote' directorii originis): nomen aut
 *                numerus argumentorum falsus capitur, quod examen non
 *                semper videt.
 * Vocator iudicium ANTE suum tradit (consilium eo iam eguit).
 */
#ifndef SILVA_EMENDATIO_H
#define SILVA_EMENDATIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_iudicium.h"

/* decrementum: codex scopi ad nihil (emendare 85) */
#define EMENDATIO_AD_NIHIL (-I)

nomen structura {
       b32 bene;
    chorda causa;   /* nominata si !bene */
       i32 ante;    /* numerus codicis scopi ante (0 si nullus) */
       i32 post;
} EmendatioVerdictum;

/* codex < 0 = nullus scopus (solum: nihil crescit, nihil fractum) */
EmendatioVerdictum
silva_emendationem_iudicare (
           IudiciumApparatus* app,
                     Piscina* opus,
          constans character* via,
    constans IudiciumFructus* ante,
          constans character* textus_novus,
                         i32  mensura_novi,
                         s32  codex,
                         s32  decrementum);

/* VERUM = clang -fsyntax-only tacet. vexilla = argumenta clang
 * NIHIL-terminata (vexilla domus + radices -I). FALSUM: erratum_out =
 * lineae primae errati clang (aut causa apparatus). */
b32
silva_emendationem_compilare (
                     Piscina* piscina,
         constans character* via,
         constans character* textus,
                        i32  mensura,
    constans character* constans* vexilla,
                     chorda* erratum_out);

#endif /* SILVA_EMENDATIO_H */
