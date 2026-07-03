/* saltuarius_nexus.h - Pons silvae (Phase B: lexatio sola)
 *
 * Possidet: indicem latinorum (aedificatum LEXANDO
 * silva_latina_textus SILVA IPSA - exemplar probationis custodis),
 * constructorem tabulae CLASSIS-PER-OCTETUM (clavis consilii:
 * u8 classis per octetum plagulae, impleta uno ambulatu fluxus
 * crudi - lexemata multilinearia [commenta!] lineas continuationis
 * GRATIS colorant, sine indice lexematum per lineam), et
 * classificationem degradationis (.c/.h colorata, alia textus
 * merus).
 *
 * Phase C hic crescet: contextus, praebenda, cache parsurarum.
 */

#ifndef SALTUARIUS_NEXUS_H
#define SALTUARIUS_NEXUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "silva.h"

nomen enumeratio {
    SALT_CLASSIS_PLANUM = 0,
    SALT_CLASSIS_VERBUM,      /* verbum C89 (genus silvae!) */
    SALT_CLASSIS_LATINA,      /* nomen latina.h - classis propria */
    SALT_CLASSIS_COMMENTUM,
    SALT_CLASSIS_CHORDA,      /* string + character lit */
    SALT_CLASSIS_NUMERUS,
    SALT_CLASSIS_DIRECTIVA,   /* # + verbum directivae */
    SALT_CLASSIS_IGNOTUM      /* OCTETUS_IGNOTUS */
} SaltuariusClassis;

nomen structura {
    Piscina*        piscina;   /* persistens */
    TabulaDispersa* latina;    /* nomina ex silva_latina_textus */
    i32             numerus_latinorum;
} SaltuariusNexus;

/* Creare nexum: index latinorum aedificatur lexando
 * silva_latina_textus (dogfood; claves in textum STATICUM amalgamae
 * spectant - aliae aeternae, transcriptio non necessaria) */
SaltuariusNexus*
saltuarius_nexus_creare (Piscina* persistens);

/* Estne nomen latinum? */
b32
saltuarius_nexus_est_latinum (constans SaltuariusNexus* nexus,
    chorda titulus);

/* Meretne via colorationem? (.c / .h) */
b32
saltuarius_nexus_est_fons_c (chorda via);

/* Tabulam classium implere: classis_out[k] pro textus.datum[k]
 * (tabula a vocatore allocata, mensura = textus.mensura; PLANUM
 * praeimpletum hic). arena_silvae: lexemata temporaria eius sunt
 * (vocator possidet - contractus vitae lexematum non nos tangit,
 * tabula octetos non lexemata retinet). */
vacuum
saltuarius_nexus_classificare (constans SaltuariusNexus* nexus,
    SilvaPiscina* arena_silvae, chorda textus, i8* classis_out);

#endif /* SALTUARIUS_NEXUS_H */
