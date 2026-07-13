/* praeparator.h - praeparatio contextus silvae COMMUNIS (familia
 * oneratorum app-lateris; interludium post-M4b: migratio geminorum)
 *
 * Quattuor officia quae omnis onerator duplicabat: ① lectio
 * plagularum; ② systema texere (ISO [+POSIX] [+latina]) + lexicon +
 * parsura + semantica systematis; ③ capita repositorii praebere
 * (ambulatio dirent, basename primus-vincit); ④ receptum
 * bis-analysis (oraculum praeseminatum + parsura cum contextu +
 * clausura + recanonicare + semantica auctoritativa).
 *
 * Consumptores: sessio, vindex_onerator. Fontes-latus (cursor/
 * interpretare/examen/percursus) migrat tactu proximo cuiusque.
 * Classis vitii canalis-macrorum (sessio.worklog 2026-07-11) in
 * exemplaribus divergentibus vivebat - haec unitas eam sepelit.
 *
 * C89 vanilla in capite - hospites amalgamata consumunt. */

#ifndef PRAEPARATOR_H
#define PRAEPARATOR_H

#include "piscina.h"
#include "silva.h"

typedef struct {
    const char* radix;     /* radix repositorii (viae systematis +
                            * ambulatio capitum); NULL = sine
                            * systemate et capitibus */
    int cum_posix;         /* systema += silva/fontes/systema_posix.h */
    int cum_latina;        /* systema += include/latina.h - POST
                            * systema (ORDO PORTANS: size_t ante
                            * memoriae_index; lexicon = canalis
                            * macrorum solum, typedefs per parsuram
                            * systematis fluunt) */
    int sine_capitibus;    /* ambulatio capitum omissa */
} PraeparatorConfiguratio;

typedef struct {
    SilvaPiscina*   piscina;           /* ctx + systema (perennis) */
    SilvaContextus* ctx;
    SilvaParsura*   systema_parsura;   /* NULL si radix NULL */
    SilvaSemantica* systema_semantica;
} Praeparatio;

/* Praeparat contextum. piscina_capitum = arena textuum capitum et
 * systematis (vocantis; vita >= praeparationis). 0 = infrastructura
 * deest (praeparatio tunc semi-structa - destruere tutum). */
int praeparator_praeparare(Praeparatio* praeparatio,
    Piscina* piscina_capitum, const PraeparatorConfiguratio* cfg);
void praeparator_destruere(Praeparatio* praeparatio);

/* Plagulam totam in piscinam legere (NUL appenso - textus etiam
 * chorda cruda usabilis). NULL si illegibilis. */
char* praeparator_plagulam_legere(Piscina* piscina, const char* via,
    unsigned int* mensura_out);

/* Receptum bis-analysis: parsura cum contextu + semantica cum
 * systemate + clausura oraculi (augere + vacare + recanonicare) +
 * semantica iterum (auctoritativa). Effimera = piscina vocantis
 * (arbores/oraculum/semantica in eam; fons vivere debet quamdiu
 * arbores). NULL = apparatus fractus. Politica errorum syntaxis =
 * VOCANTIS (parsura_out->numerus_errorum inspice). */
SilvaSemantica* praeparator_analysare(const Praeparatio* praeparatio,
    SilvaPiscina* effimera, const char* via, const char* fons,
    unsigned int mensura, SilvaParsura** parsura_out);

#endif /* PRAEPARATOR_H */
