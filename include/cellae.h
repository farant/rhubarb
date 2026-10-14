/* cellae.h - Cellae activationis combinatae (sector x axis) super
 * catalogum quaternionum EXACTUM
 *
 * Machina communis demonstrationum activationis (knotapel D82, D84 et
 * sequentium): catalogus per clausuram, tabula axium, cella summae
 * exacta (regula et paritates), separabilitas functionum paritatis,
 * quaestiones avida et exhaustiva. Planum: project-specs/cellae-plan.md.
 *
 * CATALOGUS: clausura BFS generatorum, ORDINE certo (ordo est pars
 * semanticae: tabulam axium, regulam axis primi maximi et ordinem
 * quaestionis avidae figit):
 *   - elementum 0 = 1 (profunditas 0, torsio 0);
 *   - generatores ordine dato, si nondum adsunt (profunditas 0, torsio
 *     sua);
 *   - gradus r = 1, 2, ...: omne elementum i ante initium gradus, ordine,
 *     per omnem generatorem g ordine: i g, si nondum adest (profunditas
 *     r, torsio i + torsio g);
 *   - aequalitas AD SIGNUM: q et -q eadem rotatio, unum elementum;
 *   - desinit si gradus nihil addit, aut post gradus_maximi gradus
 *     crescentes (gradu 0 incluso); limes: ultra eum nihil additur, etiam
 *     in medio gradu, et catalogus TRUNCATUS notatur (nihil tacite).
 * D82/D84: generatores (g1, g1*, g2, g2*), torsiones (+1, -1, +1, -1),
 * limes MMMMXCVI, gradus maximi XVI.
 *
 * Anulus ORDINATUS postulatur (signum: repraesentans q aut -q per
 * signum partis primae non nullae). Valores in piscina data.
 *
 * Vide lib/cellae.worklog.md.
 */
/* <aedilis corpus="lib/cellae.c"/> */
#ifndef CELLAE_H
#define CELLAE_H

#include "latina.h"
#include "piscina.h"
#include "quaternio.h"

nomen structura Catalogus Catalogus;

/* clausura (vide supra); limes 0 / gradus_maximi 0: sine limite.
 * FALSUM si numerus 0, generator anulo alieno aut sine ordine, aut
 * operatio refutata; exitus non tangitur. */
b32
catalogus_clausura (
    constans Quaternio*  generatores,
          constans s32*  torsiones,
                   i32   numerus,
                   i32   limes,
                   i32   gradus_maximi,
               Piscina*  piscina,
             Catalogus** exitus);

i32
catalogus_numerus (
    constans Catalogus* catalogus);

/* i extra [0, numerus): quaternio cum anulo NIHIL */
Quaternio
catalogus_elementum (
    constans Catalogus* catalogus,
                   i32  i);

/* gradus quo elementum natum est (1 et generatores: 0); -1 extra */
i32
catalogus_profunditas (
    constans Catalogus* catalogus,
                   i32  i);

s32
catalogus_torsio (
    constans Catalogus* catalogus,
                   i32  i);

/* pars realis nulla (Re q = 0: semiangulus 90 gradus) */
b32
catalogus_nullum (
    constans Catalogus* catalogus,
                   i32  i);

/* limes aliquid novum refutavit */
b32
catalogus_truncatus (
    constans Catalogus* catalogus);

/* gradus crescentes, gradu 0 incluso (D82 n_rounds) */
i32
catalogus_gradus (
    constans Catalogus* catalogus);

/* catalogus novus ex indicibus, ORDINE indicum (profunditas, torsio
 * servantur; truncatus hereditur). FALSUM si index extra aut
 * repetitus. */
b32
catalogus_pars (
    constans Catalogus*  catalogus,
          constans i32*  indices,
                   i32   numerus,
               Piscina*  piscina,
             Catalogus** exitus);


/* catalogus ex punctis datis, ORDINE dato (cellae II, T1): catalogi
 * nulli non ex clausura oriuntur. Profunditas, torsio, gradus 0.
 * FALSUM si numerus 0, anulus sine ordine, anuli mixti, punctum
 * nullum, aut punctum REPETITUM ad signum (refutatur, non tacite
 * deletur); exitus non tangitur. Puncta non copiantur: valent dum
 * piscina eorum vivit. Unitas non postulatur. */
b32
catalogus_ex_punctis (
    constans Quaternio*  puncta,
                   i32   numerus,
               Piscina*  piscina,
             Catalogus** exitus);


/* ==================================================
 * Activatio: tabula axium et cella summae (semantica D82/D84
 * combined_cell, exacte)
 *
 * Tabula: axes partium vectoriarum catalogi, ordine primae apparitionis,
 * antipodes idem; axes_maximae (0 omnes) secat et TRUNCATAM notat (D82:
 * D). Cella summae s = (a, v) ad k, nd = numerus axium:
 *   - s = 0: (k - 1)(nd + 1) + nd;
 *   - v = 0: sector 0, axis nd (cella nd);
 *   - a = 0 (nulla): sector floor(k/2); k par: paritas cum k/2 - 1;
 *   - aliter: sector = #{m < k/2 : tan^2 h >= tan^2(m pi/k)}, tan^2 h =
 *     |v|^2 / a^2; paritas si aequalitas ad aliquod m (sector m vel
 *     m - 1);
 *   - axis: primus maximus (v.u)^2 / |u|^2 (quaternio_proximi);
 *     paritas: omnes maximi.
 * cella = sector (nd + 1) + axis; regula = sector floor, axis primus;
 * ligatae = omnes combinationes (regula inclusa).
 *
 * v1: corpus QUADRATICUM ordinatum (Q(sqrt d)): tan^2 h radix
 * polynomii t^2 - Tr t + N, termini tan^2(m pi/k) radices S_k(u) = sum
 * C(k, 2l+1) (-1)^l u^l per POSITIONEM (radices). Aliter FALSUM.
 * Memoria cellarum per punctum exactum (s et -s idem) in piscina
 * activationis.
 * ================================================== */

nomen structura Activatio Activatio;

nomen structura {
             i32  regula;
             i32  numerus;     /* ligatae, regula inclusa */
    constans i32* ligatae;     /* valida dum activatio vivit */
} Cella;

#define CELLAE_CELLA_D82  0    /* cella combinata (supra) */
#define CELLAE_CELLA_D94  1    /* cella phasis (infra) */

/* CELLA PHASIS (D94, D119 exacte; cellae II T2). Summa s = (a, v), h =
 * acos(a/|s|) in [0, pi], angulus rotationis 2h in k sectores:
 *   - s = 0: (k - 1)(nd + 1) + nd (ut D82);
 *   - v = 0: a > 0 sector 0, a < 0 sector k - 1 (h = pi), axis nd - a <
 *     0 ergo eadem cella ac summa nulla (D119);
 *   - aliter: sector = #{m in 1..k-1 : cos h <= cos(m pi/k)}; in termino
 *     (aequalitas ad m) regula m, paritas {m - 1, m}. a = 0: sector
 *     floor(k/2), k par paritas cum k/2 - 1 (ut D82). Axis ut D82.
 * NULLA plicatio: s et -s cellas diversas habent (D82 plicat). Via: tan^2
 * h = |v|^2/a^2 radicibus S_k per positionem comparatur (ut D82),
 * terminis per signum a reflexis (m = k - m'). Activationes D94 a
 * quaestionibus antipodalibus (activatio_separat, _exhaustiva, _avida)
 * refutantur; activatio_cella et activatio_iudica eas accipiunt. */

/* FALSUM si corpus non quadraticum ordinatum, aut catalogus sine axe.
 * = activatio_creare(catalogus, NIHIL, 0, CELLAE_CELLA_D82, ...) */
b32
activatio_ex_catalogo (
    constans Catalogus*  catalogus,
                   i32   axes_maximae,
               Piscina*  piscina,
             Activatio** exitus);

/* activatio super 'catalogus' (cuius summas iudicat) cum tabula axium
 * EXPRESSA (cellae II): partes vectoriae 'axes' ordine dato,
 * antipodes et multipla idem (primus manet); NIHIL = tabula catalogi
 * ipsius (ut activatio_ex_catalogo). Sic copia una tabula alterius
 * iudicari potest (activatio_axis). FALSUM si axis anulo alieno aut
 * parte vectoria nulla, numerus_axium 0 cum axibus, aut cella ignota. */
b32
activatio_creare (
    constans Catalogus*  catalogus,
    constans Quaternio*  axes,
                   i32   numerus_axium,
                   i32   cella,
                   i32   axes_maximae,
               Piscina*  piscina,
             Activatio** exitus);

/* axis i tabulae, quaternio PURUS (0, v); i extra [0, axes): quaternio
 * cum anulo NIHIL. Valet dum piscina activationis vivit. */
Quaternio
activatio_axis (
    constans Activatio* activatio,
                   i32  i);

i32
activatio_axes (
    constans Activatio* activatio);

/* axes_maximae axem novum refutavit */
b32
activatio_truncata (
    constans Activatio* activatio);

/* k >= 1; summa in anulo catalogi; FALSUM aliter */
b32
activatio_cella (
     Activatio* activatio,
     Quaternio  summa,
           i32  k,
         Cella* exitus);


/* ==================================================
 * Separabilitas functionis paritatis (XOR)
 *
 * Pondera n (indices catalogi), quisque binas ingressiones (+q, -q):
 * summa larvae = sum c_i q_i, c_i in {-1, 0, +1}, paritas = numerus c_i
 * non nullorum mod 2. Separat si nulla cella summas paritatis
 * diversae continet. 3^n vectores, plicati per +-: s et -s eandem cellam
 * et paritatem habent.
 *   REGULA:     cellae regulae;
 *   ROBUSTA:    omnis resolutio paritatum (cellae ligatae omnes);
 *   POSSIBILIS: aliqua resolutio, per PUNCTUM exactum (idem punctum
 *               eandem cellam; punctum paritatibus diversis numquam);
 *               SAT (DPLL) cum limite decisionum;
 *   FIXA(s, u): sector superior (s) aut inferior, axis ultima (u) aut
 *               prima - FIXA(1, 0) = REGULA.
 * ================================================== */

#define CELLAE_REGULA          0
#define CELLAE_ROBUSTA         1
#define CELLAE_POSSIBILIS      2
#define CELLAE_FIXA(s, u)      (III + II * (s) + (u))

/* *exitus verdictum; FALSUM si n 0 aut > VII, index extra, k < 1, modus
 * ignotus, aut limes decisionum SAT (indecisum: exitus non tangitur) */
b32
activatio_separat (
       Activatio* activatio,
    constans i32* pondera,
             i32  n,
             i32  k,
             i32  modus,
             b32* exitus);


/* ==================================================
 * Encodatio signata (D94, D119-D123; cellae II T3)
 *
 * Pondera n (indices catalogi), larva in [0, 2^n): summa = sum +-q_i,
 * +q_i si bitum i larvae, -q_i aliter (nullus coefficiens 0). Valor
 * desideratus tabula[larva] in {0, 1}. Separat ad k si nulla cella
 * larvas valoris diversi continet; verdicta per k unita (aliquod k
 * scalae transit). Cellae D82 et D94 ambae accipiuntur (D82 plicat s et
 * -s). Modi ut supra:
 *   regula:     cellae regulae;
 *   robusta:    omnis resolutio paritatum;
 *   possibilis: aliqua resolutio (per punctum exactum; D94: s et -s
 *               puncta diversa); quaeritur solum ubi robusta deficit et
 *               nondum possibilis.
 * ================================================== */

/* n maximum encodationis signatae (D94/D119 usque ad N = VIII);
 * quaestio
 * antipodalis CELLAE_N_MAXIMUS (VII) servat */
#define CELLAE_N_SIGNATA_MAXIMA  VIII

nomen structura {
    b32 regula;
    b32 robusta;
    b32 possibilis;
    b32 ligata;      /* aliqua larva, aliquod k: cella cum paritate */
    b32 indecisa;    /* limes SAT attactus, possibilis non probata:
                      * possibilis FALSUM ergo ignotum, non verdictum */
    b32 concursus;   /* duae larvae valoris diversi, summa exacte eadem
                      * (D82: aut opposita): nulla cella ullo k eas
                      * distinguit, ergo nullus modus transit. Completum
                      * (paria omnia), non per quaestionem SAT; 0 si
                      * possibilis. Concursus indecisam tollit. */
} Iudicium;

/* *exitus verdicta; FALSUM si n 0 aut > CELLAE_N_SIGNATA_MAXIMA, index
 * extra aut iteratus,
 * scala vacua aut k < 1, tabula valore > 1 (exitus non tangitur) */
b32
activatio_iudica (
       Activatio* activatio,
    constans i32* pondera,
             i32  n,
    constans i32* scala_k,
             i32  numerus_k,
     constans i8* tabula,
        Iudicium* exitus);

/* tabulae veritatis 2^n (D94): paritas (XOR), et (AND: larva plena),
 * maior (MAJ: plus quam n/2 bita). NIHIL si n 0 aut >
 * CELLAE_N_SIGNATA_MAXIMA. */
constans i8*
cellae_paritas (
         i32  n,
     Piscina* piscina);

constans i8*
cellae_et (
         i32  n,
     Piscina* piscina);

constans i8*
cellae_maior (
         i32  n,
     Piscina* piscina);


/* ==================================================
 * Quaestiones et capacitas
 *
 * NUMERUS VERUS a SERVATIS separatur (D84: MAX_WIN XXXII pro capacitate
 * legebatur): numerus[n] = copiae n ponderum transeuntes (exhaustiva:
 * omnes; avida: a quaestione inventae, ubi 'victores_maximi' pars
 * SEMANTICAE quaestionis est, non numeri); servati usque ad
 * limes_servandi, cum k quo transierunt. maximum = 2 n maxima cum
 * numerus > 0, n >= III (XOR6 ad minimum, ut D82/D84), 0 si nulla.
 * ================================================== */

#define CELLAE_N_MAXIMUS  VII

nomen structura {
    i32 pondera[CELLAE_N_MAXIMUS];   /* indices catalogi, crescentes */
    i32 n;
    i32 k;                           /* k primus scalae qui transit */
} Victor;

nomen structura {
        i64  numerus[CELLAE_N_MAXIMUS + I];   /* index n */
        i32  servati[CELLAE_N_MAXIMUS + I];
     Victor* victores[CELLAE_N_MAXIMUS + I];
        i32  maximum;
} Capacitas;

nomen structura {
    constans i32* scala_k;            /* ordine (avida: primus transiens) */
             i32  numerus_k;
             i32  modus;              /* CELLAE_REGULA, ... */
             i32  n_maximum;          /* <= CELLAE_N_MAXIMUS */
             i32  victores_maximi;    /* avida: sistit post tot per n
                                       * (D82/D84 XXXII) */
             i32 limes_triplicum;    /* avida: triplices ex primis tot
                                       * indicibus (D82/D84 C) */
             i32 cellae_maximae;     /* k omittitur si k (axes + 1) >
                                       * hoc (D82/D84 MAX_ACT: XVI M
                                       * CCCLXXXIV); 0 nullum */
             i32  limes_servandi;     /* victores servati per n; 0 nulli */
    constans i32* indices;            /* pondera permissa (crescentia);
                                       * NIHIL = totus catalogus */
             i32 numerus_indicum;
} Quaestio;

/* exhaustiva: omnes copiae n = 1 .. n_maximum ponderum permissorum, per
 * gradus (separabilitas deorsum clausa ad k fixum), unio super scalam.
 * Catalogus aut indices <= LXIV. FALSUM si refutatum aut SAT indecisum.
 * Victores in piscina. */
b32
activatio_exhaustiva (
             Activatio* activatio,
     constans Quaestio* quaestio,
               Piscina* piscina,
             Capacitas* exitus);

/* avida (D82/D84 find_capacity): n = 3 per triplices crescentes ex primis
 * limes_triplicum ponderibus permissis, primus k scalae qui transit,
 * sistit post victores_maximi; deinde n + 1: omnis victor gradus n
 * ordine, omne pondus permissum ordine (non in victore, insertum
 * ordine, non duplicatum), primus k qui transit; gradus solum si prior
 * victores habet; usque ad n_maximum. */
b32
activatio_avida (
             Activatio* activatio,
     constans Quaestio* quaestio,
               Piscina* piscina,
             Capacitas* exitus);


/* ==================================================
 * Filtrum fluitans certum et statistica
 *
 * In activatio_separat (et quaestionibus) summa primum per intervalla
 * duplicium aestimatur (elementa et termini correcte rotundati, omnis
 * operatio ultra errorem suum dilatata): si intervalla a != 0, v != 0,
 * tan^2 h inter terminos STRICTE et axem unum STRICTE maximum
 * probant, cella decisa est et sine paritate (paritas aequalitas exacta
 * est, quam intervallum numquam excludit). Aliter via exacta (axes
 * candidatae solae si filtrum eas angustavit). Sine iactura: verdicta
 * eadem ac sine filtro (probatur).
 * ================================================== */

nomen structura {
    i64 filtro;      /* cellae a filtro decisae */
    i64 exacte;      /* cellae per viam exactam */
    i64 ligatae;     /* exactae cum paritate (plus quam una cella) */
    i64 imagine;     /* summae per imaginem integram nominatae */
    i64 algebraice;  /* summae per numeros algebraicos nominatae */
    i64 puncta;      /* puncta exacta in memoria activationis */
} CellaeStatistica;

/* filtrum (ordinarie VERUM) */
vacuum
activatio_filtrum (
     Activatio* activatio,
           b32  utere);

/* IMAGO INTEGRA (cellae II T4b): ad creationem pars j elementi =
 * (p + q alpha)/D, alpha generator corporis, D denominator communis,
 * p, q integri |p|, |q| <= 2^40. Summa tum additiones integrorum et
 * punctum exactum per integros nominatur (valor idem sse integri
 * idem); numeri algebraici solum pro puncto novo. Coefficiens maior aut
 * D extra s64: nulla imago, via algebraica (statistica: imagine 0).
 * activatio_cella summam repraesentabilem (omnis coefficiens x D
 * integer in s64) per imaginem quoque nominat: idem valor, idem
 * punctum. Ordinarie VERUM; FALSUM viam algebraicam priorem reddit
 * (probationis causa: verdicta eadem). Commutatio inter vocationes
 * licet; idem valor tum puncta duo habere potest (unum per nomen),
 * cellis eisdem (ambae exactae). */
vacuum
activatio_imago (
     Activatio* activatio,
           b32  utere);

vacuum
activatio_statistica (
    constans Activatio* activatio,
      CellaeStatistica* exitus);

#endif /* CELLAE_H */
