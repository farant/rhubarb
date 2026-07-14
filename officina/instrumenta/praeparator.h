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
#include "tabula_dispersa.h"
#include "xar.h"
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

/* tempus capitis ad onus commemoratum (percursus staleness) */
typedef struct {
    const char* via;       /* via absoluta (copia in piscina) */
    long        tempus;    /* mtime ad tempus onerationis */
} PraeparatorCaputTempus;

typedef struct {
    SilvaPiscina*   piscina;           /* ctx + systema (perennis) */
    SilvaContextus* ctx;
    SilvaParsura*   systema_parsura;   /* NULL si radix NULL */
    SilvaSemantica* systema_semantica;
    TabulaDispersa* viae_capitum;      /* basename (chorda) -> via
                                        * absoluta (char*); ex
                                        * ambulatione capitum; NULL
                                        * si sine_capitibus. Pro
                                        * URIs saltuum in capita
                                        * (legatus definitio). */
    Xar*            tempora_capitum;   /* PraeparatorCaputTempus -
                                        * omnia capita onerata
                                        * (systema + repositorium);
                                        * pro caput_stalum */
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

/* Tempus modificationis plagulae (secunda epochae); 0 = deest aut
 * illegibilis. Sedes POSIX (sys/stat.h) consulto HIC continetur -
 * eadem lex qua dirent (classis POSIX in una plagula). */
long praeparator_tempus_plagulae(const char* via);

/* Percursus staleness (gradus I legis aetatum, spec-v2 mcp-legati):
 * omnia capita onerata re-stantur; via primi cuius mtime a
 * commemorato DIFFERT (identitas, non limen - retro quoque capit,
 * e.g. git checkout) redditur, NULL = omnia recentia. Capita NOVA
 * (post onus creata) non videntur - inventio per iudicium-fallens
 * parcum nominatum. */
const char* praeparator_caput_stalum(const Praeparatio* praeparatio);

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
