/* portae.h - PORTAE DECLARATAE: opertura portarum ex REPOSITORIO
 * (spec project-specs/portae-declaratae-spec.md; ledger …GGMGY94)
 *
 * Quae porta mutationi debetur ex declarationibus in capitibus
 * cursorum (annotatio STML in commentariis scripti) et derivatione
 * computatur - numquam ex tabulario (principium …RBA4W73). Nucleus
 * in C, involucra tenuia (principium …Z2Y555): pythonica solum vocat.
 *
 * ANNOTATIO (in commentario '#' cursoris; materia_annotationes per
 * clientem crustae - '#' intra chordam commentarium non est):
 *
 *   # <porta titulus="credo" tegit="tools/credo_*.py"/>
 *   # <porta manu causa="rete poscit (API vera - centesimae)"/>
 *
 *   titulus  clavis PORTAE quam hoc scriptum currit (plures elementa
 *            si scriptum plures portas currit - argumenta in PORTAE)
 *   manu     porta quam HOMO currit (sine PORTAE); causa necessaria
 *   tegit    exemplaria viarum (exemplar.h) ultra derivationem
 *   currit   programmata quae probationes lancant, derivationi caeca
 *   instrumentum  binaria domus quae haec porta POSSIDET
 *            ('bin/aedilis'): portae aliae quae ea currunt fontes
 *            eorum non hereditant (infrastructura aedificandi, non
 *            subiectum); possessor et 'currit' expressum hereditant.
 *            Possessio bina = recusatio
 *   genus    'praeiudicium' (PRAE-IUDICIUM: proprietas arboris commissae
 *            - ratchet, registrum, generata == regeneratio, verdicta
 *            corporis - SINE codice probando currendo) aut 'probatio'
 *            (ordinarium: codicem currit, mores iudicat). Genus phasim
 *            et regulam fracturae decernit (praeiudicia omnia currunt,
 *            fracturae SIMUL nominantur; probationes primam fracturam
 *            sistunt); sumptus ordinem intra phasim (circulus
 *            commissionis II §II-III). Valor alius = recusatio
 *   gradus   'commissio' (ordinarium: obligatoria in omni commissione)
 *            aut 'fusio' (obligatoria solum in fusione in main; inter
 *            commissiones debitum rami notatur) - gradus-portarum
 *            spec, decretum Frani ...GDFF4J. Valor alius = recusatio
 * Vocabulum CLAUSUM: attributum aliud = recusatio (error scribae).
 *
 * MACHINA PURA: ingressus et egressus per suturam (exemplar
 * aedilis) - probationes plagulas in memoria dant; CLI (bin/fabrica
 * portae, T3) discum et 'git ls-files'.
 */
#ifndef PORTAE_H
#define PORTAE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "stml.h"

/* registrum PORTAE (pythonica/silva.py) ab involucro traditum:
 * clavis -> cursor (via scripti, radici relativa); probationes =
 * directorium suitae (SUITAE: <dir>/probatio_*.c), vacua = porta sine
 * suita */
nomen structura {
    chorda clavis;
    chorda cursor;
    chorda probationes;
} PortaeIngressus;

nomen structura {
          vacuum* datum;
    /* viae OMNES arboris (git ls-files), radici relativae: Xar de
     * chorda; NIHIL = enumeratio fracta. Leguntur solae '.sh'
     * (cursores omnes scripta crustae sunt) */
            Xar* (*plagulas_enumerare) (vacuum* datum,
                Piscina* piscina);
    /* textus plagulae; FALSUM si legi nequit */
            b32  (*plagulam_legere) (vacuum* datum, chorda via,
                                     Piscina* piscina, chorda* textus);
    /* summarium effectuum cursoris (crusta_effectus_derivare: elementum
     * 'effectus'); NIHIL = ignotum (lacuna nominata, non recusatio) */
      StmlNodus* (*effectus) (vacuum* datum, chorda cursor,
                              Piscina* piscina);
    /* litterae chordarum probationis (fons C): contentum sine '"', Xar
     * de chorda; verbum primum viam arboris aut binarium domus nominans
     * dependentia est (programma vocatum, fixum lectum); NIHIL =
     * ignotum. CLI: lexator silvae (commenta
     * absunt, macrones NON expansi - mensuratum 2026-10-12: superset
     * responsi pythonici per arbor.sh -nudum) */
            Xar* (*litterae_probationis) (vacuum* datum, chorda fons,
                                         Piscina* piscina);
    /* clausura fontis C (aedilis): fontes et capita, viae radici
     * relativae, Xar de chorda; NIHIL = ignota (aedilis fractus) */
            Xar* (*clausura_c) (vacuum* datum, chorda fons,
                                Piscina* piscina);
    /* Xar de PortaeIngressus */
    constans Xar* tabula_portarum;
} PortaeSutura;

/* genus portae (attributum 'genus'; circulus commissionis II T4) */
nomen enumeratio {
    PORTA_PROBATIO = 0,      /* ordinarium */
    PORTA_PRAEIUDICIUM
} PortaGenus;

/* gradus portae (attributum 'gradus'; gradus-portarum T1) */
nomen enumeratio {
    PORTA_COMMISSIO = 0,     /* ordinarium */
    PORTA_FUSIO
} PortaGradus;

/* porta declarata una (elementum <porta> unum) */
nomen structura {
     chorda  via;        /* cursor: scriptum annotatum */
        i32  linea;      /* linea commentarii, I-basata */
     chorda  titulus;    /* clavis PORTAE; vacua si manu */
        b32  manu;
     chorda  causa;      /* manu: cur homo currit; vacua aliter */
        Xar* tegit;      /* chorda: exemplaria viarum */
        Xar* currit;     /* chorda: programmata */
        Xar* instrumenta; /* chorda: binaria domus possessa */
 PortaGenus  genus;      /* PORTA_PROBATIO nisi 'genus' aliter */
PortaGradus  gradus;      /* PORTA_COMMISSIO nisi 'gradus' aliter */
} PortaDeclarata;

/* recusatio una: locus (via vacua = registrum aut fundamentum; linea
 * ZEPHYRUM = sine linea) et causa */
nomen structura {
    chorda via;
       i32 linea;
    chorda causa;
} PortaeRecusatio;

/* Declarationes omnium cursorum. Xar de PortaDeclarata (ordine viarum
 * enumeratarum, deinde commentariorum). NIHIL = RECUSATIO: *causa_out
 * causas OMNES nominat (linea una per causam, 'via:linea: ...') et
 * *recusationes_out (NIHIL licet) easdem structas (Xar de
 * PortaeRecusatio): annotatio <porta> illegibilis, attributum ignotum,
 * nec titulus nec manu, manu sine causa, titulus extra
 * registrum, cursor alienus (registrum clavem alii scripto dat),
 * titulus bis declaratus, clavis registri sine cursore annotato. */
Xar*
portae_declaratas_legere (
    constans PortaeSutura*  sutura,
                  Piscina*  piscina,
                   chorda*  causa_out,
                      Xar** recusationes_out);


/* ==================================================
 * DERIVATIO (plan T2): quae via portam tangit, ex repositorio solo
 * ================================================== */

nomen enumeratio {
    PORTAE_EXEMPLAR_VIA = ZEPHYRUM,  /* via aequalis */
    PORTAE_EXEMPLAR_GLOBUS,          /* '*' '?' - '*' etiam '/' transit
                                      * (directorium_titulus_congruit,
                                      * ut fnmatch pythonicum) */
    PORTAE_EXEMPLAR_PRAEFIXUM        /* via sic incipit */
} PortaeExemplarGenus;

/* genus RATIONIS (unitates-codicis U5, relatio portarum): unde exemplar
 * venit - typatum, ne consumptores textum 'ratio' parsent */
nomen enumeratio {
    PORTAE_RATIO_CURSOR = ZEPHYRUM,  /* scriptum portae ipsum */
    PORTAE_RATIO_TEGIT,              /* 'tegit' annotationis (manu) */
    PORTAE_RATIO_EFFECTUS,           /* lectio/exsecutio/fontatio in
                                      * scripto (crusta effectus) */
    PORTAE_RATIO_SUITA,              /* probatio suitae (directorium) */
    PORTAE_RATIO_NOMINAT,            /* probatio viam litteris
                                      * nominat */
    PORTAE_RATIO_CURRIT,             /* programma quod porta currit */
    PORTAE_RATIO_CURRIT_ALIENUM,     /* 'currit' annotationis
                                      * programmatis quod porta ALIA
                                      * possidet
                                      * (instrumentum=): infrastructura */
    PORTAE_RATIO_INSTRUMENTUM,       /* binarium possessum */
    PORTAE_RATIO_IGNOTA,             /* caeca: debetur, non tegitur */
    PORTAE_RATIONUM_NUMERUS
} PortaeRatioGenus;

nomen structura {
                 chorda exemplar;
    PortaeExemplarGenus genus;
                 chorda ratio;     /* unde: "lectio (clang) in x.sh" */
       PortaeRatioGenus ratio_genus;
} PortaeExemplar;

/* opertura portae declaratae unius. exemplaria = viae quas porta
 * DIRECTE tegit: cursor ipse; situs effectuum CONTENTI (lectio
 * exsecutio fontatio - probatio et enumeratio tempus/existentiam
 * solum inspiciunt; classis arbor, resolutio plena;
 * custoditi praetereuntur - aedificatores '[ -x P ] || S' extra
 * catenam verdicti); scripta quae probationes vocant; tegit
 * annotationis. fontes_c = fontes C quorum CLAUSURAE (aedilis, T3)
 * tegunt - fons ipse et quae includit: probationes suitae, fontes quos
 * cursor compilat (situs .c, globi .c in viis arboris expansi), fontes
 * introitus programmatum quae probationes aut annotatio 'currit'
 * vocant (nomen ambiguum = candidati OMNES). ignota = CAECA (effectus
 * ignotum, litterae ignotae, 'currit' irresolutum): porta omni viae
 * mutatae DEBETUR, viam tamen non tegit (INTECTUM manet). notae =
 * informationes (binarium vocatum sine fonte in arbore, e.g. fixa
 * probationum 'bin/inst') - nihil debent. LACUNA NOTA: '-I'
 * directoria (tabula mandatorum effectus ea non notat) - 'tegit'
 * annotationis ea portat. */
nomen structura {
    constans PortaDeclarata* declaratio;
                        Xar* exemplaria;  /* PortaeExemplar */
                        Xar* fontes_c;    /* PortaeExemplar (VIA) */
                        Xar* ignota;      /* chorda: caeca */
                        Xar* notae;       /* chorda */
                        Xar* binaria;     /* PortaeExemplar (VIA):
                                           * binaria arboris 'bin/x'
                                           * quae porta CURRIT -
                                           * exsecutio cursoris aut
                                           * probationis, instrumentum
                                           * possessum, 'currit
                                           * tools/<b>.c' -> bin/<b>
                                           * (candidatus; fabrica
                                           * ignota praeterit).
                                           * circulus commissionis II
                                           * T2: praevolatus ea sanat */
} PortaeOpertura;

/* Operturae declarationum omnium (Xar de PortaeOpertura, ordine
 * declaratarum; duplicata in eadem opertura semel, ratio prima).
 * NIHIL = memoria deficit aut enumeratio fracta. */
Xar*
portae_operturas_derivare (
    constans PortaeSutura* sutura,
             constans Xar* declaratae,
                  Piscina* piscina);


/* ==================================================
 * DEBITA (plan T3): quae portae viis mutatis debentur
 * ================================================== */

/* debitum unum. porta = titulus, aut via cursoris si manu (homo
 * currit: nominatur, numquam curritur). causa = ratio prima, '(+N
 * aliae)' si plures */
nomen structura {
             chorda porta;
             chorda causa;
                b32 manu;
    constans Xar*   binaria;   /* operturae (PortaeExemplar); NIHIL licet */
} PortaeDebitum;

/* tactus unus: via mutata portam tangit, rationes OMNES (Xar de
 * chorda). porta = titulus, aut via cursoris (manu sine registro) */
nomen structura {
                     chorda  via;
                     chorda  porta;
                        b32  manu;
                        Xar* rationes;
    constans PortaeOpertura* opertura;
                        i32  directa;     /* (I << PortaeRatioGenus) per
                                           * exemplar quod viam ipsam
                                           * tegit; IGNOTA si caeca */
                        i32 clausurae;   /* idem per fontem C cuius
                                           * clausura viam continet */
} PortaeTactus;

/* Tactus viarum mutatarum (Xar de PortaeTactus, ordine operturarum
 * deinde viarum) - quaestio 'quid haec mutatio tangit et cur', et
 * fundamentum debitorum (portae_debita_computare eos colligit).
 * *intecta_out ut infra. NIHIL = memoria deficit. */
Xar*
portae_tactus_computare (
    constans PortaeSutura*  sutura,
             constans Xar*  operturae,
             constans Xar*  viae,
                      Xar** intecta_out,
                  Piscina*  piscina);

/* Debita viarum mutatarum (Xar de chorda, radici relativae). Ordo:
 * portae registri ordine registri, deinde manu ordine operturarum.
 * Via debet si exemplar eam tegit, si fons_c est, si (via .c .h .m)
 * in clausura fontis_c est - clausura ignota = DEBETUR - aut si
 * opertura caeca est (ignota: debetur, non tegitur). *intecta_out
 * (NIHIL licet): viae quas nulla porta tegit. Clausurae solum si via
 * mutata C est, semel per fontem. NIHIL = memoria deficit. */
Xar*
portae_debita_computare (
    constans PortaeSutura*  sutura,
             constans Xar*  operturae,
             constans Xar*  viae,
                      Xar** intecta_out,
                  Piscina*  piscina);


/* ==================================================
 * EFFUSIO (plan T3) - CONTRACTUS MACHINAE DECLARATUS
 *
 * Lineae TSV, genus in campo primo; campi effugiuntur: '\' -> '\\',
 * tabula -> '\t', linea nova -> '\n', CR -> '\r' (nihil aliud):
 *
 *   portae debitae:
 *     DEBITUM    <porta>  <0|1 manu>  <causa>
 *     BINARIUM   <porta>  <bin/x>     <ratio>   (post DEBITUM suum;
 *                portae non manu solae - binaria quae currit,
 *                circulus commissionis II T2)
 *     INTECTUM   <via>
 *     RECUSATIO  <via>    <linea|0>   <causa>
 *   portae tactus:
 *     TANGIT     <via>    <porta>     <0|1 manu>  <ratio>   (una per
 *                rationem)
 *     INTECTUM   <via>
 *   portae declaratae:
 *     PORTA      <porta>  <0|1 manu>  <via>  <linea>  <causa manu>
 *     INSTRUMENTUM <porta> <binarium possessum>
 *     GENUS      <porta>  praeiudicium   (solum si non ordinarium)
 *     GRADUS     <porta>  fusio          (solum si non ordinarium)
 *     TEGIT      <porta>  <via|globus|praefixum>  <exemplar>  <ratio>
 *     CLAUSURA   <porta>  <fons>      <ratio>
 *     IGNOTUM    <porta>  <textus>
 *     NOTA       <porta>  <textus>
 *     RECUSATIO  <via>    <linea|0>   <causa>
 *
 * Forma hominis (machina FALSUM): PORTAE DEBITAE (N viae) / MANU (homo
 * currit) / INTECTA (nulla porta) - forma silva.portae_debitae_relatio.
 * ================================================== */

/* campus TSV effugitus (contractus supra) */
chorda
portae_campum_effugere (
      chorda  campus,
     Piscina* piscina);

/* debitae: debita, intecta (NIHIL licet), recusationes (non NIHIL =
 * solae scribuntur); viae_numerus pro titulo hominis */
chorda
portae_debita_scribere (
    constans Xar* debita,
    constans Xar* intecta,
    constans Xar* recusationes,
             i32  viae_numerus,
             b32  machina,
         Piscina* piscina);

/* tactus: lineae TANGIT (rationes omnes) et INTECTUM; homo per viam */
chorda
portae_tactus_scribere (
    constans Xar* tactus,
    constans Xar* intecta,
             b32  machina,
         Piscina* piscina);

/* declaratae: operturae (NIHIL licet si recusationes) et recusationes
 * (NIHIL licet) */
chorda
portae_declaratas_scribere (
    constans Xar* operturae,
    constans Xar* recusationes,
             b32  machina,
         Piscina* piscina);


/* ==================================================
 * ORDO PORTARUM (circulus commissionis II T5)
 *
 * Praeiudicia ANTE probationes (genus phasim decernit); intra genus
 * sumptu ascendente: mediana millesimarum ultimarum X cursuum SANORUM
 * in 'tempora' (TSV build/portae/tempora.tsv: tempus, 'porta',
 * titulus, secundae, cucurrit 1, rc 0); porta SINE historia PRIMA
 * (nova aut modo mutata plerumque); par = ordo nominum. Porta non
 * declarata = probatio. Purum: textus temporum a vocante.
 * ================================================== */

nomen structura {
        chorda porta;
    PortaGenus genus;
   PortaGradus gradus;
           s32 millesimae;   /* mediana; -I = sine historia */
           i32 cursus;       /* cursus sani numerati (<= X) */
} PortaeOrdo;

/* Xar de PortaeOrdo, ordine currendi. nomina = Xar de chorda (titulus
 * portae). NIHIL = memoria deficit. */
Xar*
portae_ordinare (
     constans Xar* declaratae,
     constans Xar* nomina,
           chorda  tempora,
          Piscina* piscina);

/* ORDO <porta> <praeiudicium|probatio> <millesimae|-> <commissio|fusio>
 * per lineam (machina); homo: 'ordo portarum:' + praeiudicia +
 * probationes (gradus fusio: ' [fusio]' post nomen) */
chorda
portae_ordinem_scribere (
    constans Xar* ordo,
             b32  machina,
         Piscina* piscina);

#endif /* PORTAE_H */
