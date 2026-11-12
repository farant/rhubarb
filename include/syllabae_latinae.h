/* syllabae_latinae.h - verbum Latinum -> syllabae et accentus
 *
 * Spec: project-specs/psalmodia-spec.md III. Planum:
 * project-specs/psalmodia-plan-2.md T1-T2. Mensura regularum:
 * lib/syllabae_latinae.worklog.md. Stratum: chorda et piscina solum -
 * psalmos, tonos, gabc nescit.
 *
 * USUS:
 *   VerbumLatinum v;
 *   si (!syllabae_latinae_dividere(verbum, piscina, &v)) ... memoria;
 *   per (i = ZEPHYRUM; i < v.numerus; i++)
 *       ... verbum.datum + v.syllabae[i].initium,
 *           v.syllabae[i].finis - v.syllabae[i].initium ...
 *   si (v.accentus >= ZEPHYRUM) ... syllaba accentus ...
 *
 * NUMERUS ET FORMA: aptator numerum syllabarum et accentum solum
 * requirit; ubi consonantes inter vocales cadunt hyphenos tantum
 * mutat, numerum numquam. Numerus ex vocalibus pendet (regulae V
 * infra), forma ex consonantibus (regulae C). Forma = libri nostri,
 * AR 1912 (editio Vaticana), non GregoBase, qui stilos tres miscet.
 * Numeri inter uncos: (GregoBase) aut [AR 1912, hyphenis qui in
 * OCR supersunt].
 *
 * REGULAE V (nuclei; numerus):
 *   - vocales: a e i o u y æ œ, maiusculae, cum acuto (á ǽ,
 *     praecompositae aut U+0301 sequente), cum diaeresi (ë);
 *   - ae oe au diphthongi (2260:251, 112:75, 3840:17 - divisa
 *     omnia nomina hebraica aut graeca sunt: Is-ra-el, No-e, Sa-ul);
 *     diaeresis hiatum facit (Is-ra-ël, 76:0);
 *   - ceterae vocales contiguae hiatum faciunt: De-us (3812:27),
 *     cu-i (64:28), hu-ic (16:0), me-i, tu-a, su-á-vis (44:0);
 *     exceptiones eu: heu ceu seu neu, eu-ge, pneu-, Eu- + consonans
 *     nisi eunt- (Eu-cha-rí-sti-a, e-un-tes);
 *   - i consonans: inter vocales (al-le-lú-ia, e-ius, cu-ius; 4079:10)
 *     et initio ante vocalem (Ie-sus, iam; 413:13), nisi initio ii
 *     (i-is-dem); j semper consonans;
 *   - u consonans post q (qu 1961:3) et post ng ante vocalem
 *     (sán-gui-ne, lin-gua; [43:0]); alibi vocalis (ar-gu-it).
 *
 * REGULAE C (forma; consonantes inter nucleos):
 *   - consonans una cum sequente (Dó-mi-nus); ch ph th rh qu et
 *     ngu (u consonans) pro consonante una; x quoque (di-xi,
 *     surre-xí-sti [200:3]);
 *   - plures: syllaba sequens catervam longissimam accipit quae
 *     syllabam incipere potest; ceterae cum priore. Incipere
 *     possunt: muta + liquida (br cr dr fr gr pr tr bl cl fl gl pl,
 *     chr phr thr chl phl; tr [78:0], cl [17:0]); s + muta (+
 *     liquida): sc sp st sch scr spl spr str (st [205:3], str
 *     [55:0]); gn [34:0], ct [92:0], pt [19:0], mn [13:3], ps [16:0],
 *     sm [1:0]. Ergo ju-stí-ti-a, San-ctus, pro-pté-re-a, o-mnes,
 *     As-sum-ptió-nis, sed us-que (s|qu [4:0]), ter-ra, ex-ten-do;
 *   - praefixum + vocalis initio verbi: hyphenus post praefixum
 *     (ab ad ex in ob per red sub trans): in-imí-cus [49:3],
 *     ex-áu-di [46:3], red-émp-tor [36:2], per-én-nis [40:15],
 *     ab-í-it [12:2], ad-o-rá-re [6:5]; verba integra sic-ut
 *     [77:2], sic-u-ti, et-é-nim. Non composita (regula communis):
 *     abyss-, Abel, Adam, adip-, adeps, Adonai, reduc-, redux-,
 *     redupl-.
 *
 * HEBRAICA: nomina hebraica sine diaeresi regulas V sequuntur
 * (Israel -> Is-rael). Aptator (VOCES_HEBRAICAE) aut textus ea
 * hyphenis cogit; haec bibliotheca indicem nominum non tenet.
 *
 * ACCENTUS: acutus (praecompositus aut U+0301) syllabam suam
 * notat, notatus VERUM; acuti plures -> ultimus (Dominúsque). Sine
 * acuto: syllabae duae -> prima (Deus 0), una -> -1, tres aut plures
 * -> -1; notatus FALSUM. Diaeresis accentus non est.
 *
 * COGERE: '-' inter litteras syllabas dividit (Is-ra-el,
 * E-u-o-u-a-e) et in nulla syllaba stat; regulae intra partes
 * valent. '-' alibi = interpunctio.
 *
 * SPATIA: syllabae verbum ordine tegunt (praeter '-' cogentes);
 * interpunctio initialis syllabae primae, finalis ultimae, media
 * priori (Dómino, -> ultima "no,"). initium < finis, ordine
 * crescente, intra verbum. Verbum = textus unius verbi; spatium
 * interpunctio est (vocans verba dividit).
 *
 * TOTALITAS: octeti quilibet -> exitus validus. Verbum sine vocali
 * -> numerus 0, accentus -1 (litterae sine vocali, ut "Ps", in
 * syllabam nullam cadunt). UTF-8 malum = interpunctio. FALSUM solum
 * si memoria deficit. Syllabae in piscina; verbum non copiatur
 * (spatia in verbum vocantis monstrant).
 */

#ifndef SYLLABAE_LATINAE_H
#define SYLLABAE_LATINAE_H

/* <aedilis corpus="lib/syllabae_latinae.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"


/* syllaba = octeti [initium, finis) verbi dati */
nomen structura {
    i32 initium;
    i32 finis;
} SyllabaLatina;

nomen structura {
    SyllabaLatina* syllabae;
              i32  numerus;
              s32  accentus;  /* index syllabae; -1 = nullus */
              b32  notatus;   /* VERUM si acutus eum dedit */
} VerbumLatinum;


/* verbum -> syllabae et accentus (vide caput); FALSUM solum si
 * memoria deficit */
b32
syllabae_latinae_dividere (
           chorda  verbum,
          Piscina* piscina,
    VerbumLatinum* exitus);

/* VERUM si ad octetum positio vocalis incipit (omnis forma:
 * maiuscula, acuta, diaeresi, æ œ, U+0301/U+0308 sequente);
 * longitudo (si non NIHIL) octetos eius accipit. Regulas V non
 * applicat: i et u semper vocales hic. */
b32
syllabae_latinae_vocalis (
    chorda  verbum,
       i32  positio,
       i32* longitudo);

#endif /* SYLLABAE_LATINAE_H */
