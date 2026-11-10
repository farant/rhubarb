/* gabc_compositio.h - compositio cantus gabc: GabcCantus -> paginae
 * pictorum positorum (partes neumarum, rectangula, textus, initialis)
 *
 * Spec: project-specs/gabc-spec.md §IV-V, G11, G14, G15. Planum:
 * project-specs/gabc-plan-2.md P1, P3. Purum: pixela logica numerat,
 * mandata nescit (gabc_pictura.h pingit), vicum nescit. Pictum quodque
 * syllabam, elementum, notam unde venit nominat; syllabae et notae
 * capsas habent (illuminatio soni, ictus muris quaerit).
 *
 * USUS:
 *   GabcCompositionisForma forma;
 *   gabc_compositionis_formam_ordinariam(&forma);
 *   forma.latitudo = latitudo_tabulae;
 *   c = gabc_componere(cantus, &forma, piscina);
 *   per (i = c->paginae[p]; i < c->paginae[p + I]; i++)
 *       ... c->picta[i] ...
 *
 * SIGNA: s = spatium, k = max(I, s/VIII), y(p) = linea_ima -
 * (p - III) * s/II (positio p, linea_ima = y lineae imae systematis;
 * lineae systematis in positionibus III, V, ... II*lineae+I).
 *
 * COMPOSITIO NEUMARUM (formae ex tabulis glypharum GregorioRef
 * 6.1.0, appendix A, lectae 2026-11-04). IUXTA = caput sequens ad
 * dextram contiguum (x + capitis_latitudo), linea connectens
 * verticalis k lata in margine communi, a centro ad centrum. SUPRA =
 * caput sequens in eadem columna, linea connectens in margine dextro.
 * Caulis ligaturae: rectangulum k latum in margine sinistro capitis
 * primi, a centro eius deorsum usque ad s/II + s/IV infra centrum
 * notae inferioris (ultra caput, numquam in linea; P4 post visum).
 * Caulis VIRGAE longior: ad medium spatii primum quod spatium saltem
 * infra centrum iacet (nota in spatio: II gradus; in linea: III).
 *   punctum, virga, stropha, oriscus, quilisma, cavum ...: pars una
 *     ex formis notae (GabcNota.formae); W ut quilisma, O ut oriscus
 *     cum caule sinistro.
 *   bivirga, trivirga, distropha, tristropha: partes singulae,
 *     intervallo s/II.
 *   pes: caput inferius, superius SUPRA. pes quilismaticus: quilisma
 *     inferius. epiphonus: superius DEMINUTUM SUPRA.
 *   pes quadratus (q), pes quassus (oriscus inferius): superius IUXTA.
 *   clivis: caulis sinister, inferius IUXTA. cephalicus: inferius
 *     DEMINUTUM IUXTA. clivis ab orisco aut cum '@': sine caule.
 *   torculus: tria capita IUXTA; quilismaticus: quilisma primum;
 *     deminutus: tertium DEMINUTUM; initio debilis (-): primum
 *     DEMINUTUM.
 *   porrectus: caulis sinister, PORRECTUS a prima ad secundam
 *     (parametrum = differentia positionum), tertia SUPRA finem lineae
 *     obliquae (margo dexter capitis = finis obliquae). Differentia
 *     supra V: clivis + caput IUXTA. porrectus flexus: + quarta IUXTA.
 *   torculus resupinus (flexus): caput primum, linea connectens
 *     sursum ad initium obliquae, deinde ut porrectus (flexus).
 *   scandicus: caput, deinde pes IUXTA. salicus: caput, deinde pes
 *     quassus IUXTA (oriscus medius).
 *   climacus, pes subbipunctis/subtripunctis, ancus: pars prior ut
 *     supra (virga, pes, clivis), deinde inclinata quodque gradu
 *     inferius, x += s*III/IV; deminuta ut INCLINATUM_DEMINUTUM.
 *   pressus: capita aequalia IUXTA sine linea.
 *   AUCTUM (< >) et DEMINUTUM (~) notam ultimam mutant.
 *   IGNOTA aut figura sine regula: notae sinistra ad dextram, caput
 *     quodque ex formis suis IUXTA, linea connectens inter capita
 *     inaequalia non inclinata - numquam omittitur.
 *   Lineae additae: positio extra systema -> lineae ad positiones
 *     I, -I ... aut II*lineae+III ... usque ad notam, latitudo
 *     capitis + s/II, centratae; solum ubi nota eget.
 *
 * SIGNA NOTARUM:
 *   punctum morae: ad dextram capitis + s/IV; nota in linea -> in
 *     spatio supra (mora_locus .0 infra, .1 supra); duo puncta:
 *     secundum + s/II.
 *   episema (_): supra caput (infra si _0), s/IV a margine; notae
 *     contiguae eiusdem neumae lineam unam communem habent (nisi _2).
 *   ictus ('): infra caput, s/IV a margine (supra si '1).
 *   r1-r5: supra caput, s/II a margine. r6-r8 (ficta): signum
 *     alterationis notae supra caput.
 *
 * ELEMENTA (intervalla post elementum; s = spatium):
 *   CLAVIS: pars clavis; clavis 'b' (cb3): bemolle in b quae intra
 *     systema iacet (e duabus octava distantibus; ambae -> superior);
 *     clavis secunda (@c4) ad dextram, s/IV. Post clavem s.
 *   ALTERATIO: pars signi in positione, deinde s/IV.
 *   DIVISIO (rectangula k lata, nisi aliter): virgula ` pars VIRGULA
 *     supra lineam summam; minimis ^ a linea summa s/II sursum;
 *     minima , lineam summam transit (s/II supra et infra); minor ;
 *     a medio spatii summi ad medium spatii imi; maior : a linea summa
 *     ad imam; maior punctata :? eadem fracta (s/II plena, s/II
 *     vacua); finalis :: duae maiores, s/II inter eas; dominicana ut
 *     minor (v1); alta (`0 ^0 ,0) supra lineam summam. Ante et post
 *     divisionem s*III/IV.
 *   SPATIUM: ' ' s/IV; /0 s*III/VIII; / et /! s*III/IV; // s*III/II;
 *     /[n] n * s*III/IV (n malum -> /); ! 0. Elementa sine spatio
 *     inter ea: s/IV.
 *   CUSTOS: scriptus (g+) in positione sua; z0 -> positio notae
 *     sequentis. Caulis deorsum si positio >= VIII, aliter sursum.
 *   FRACTIO, SIGNUM_OPACUM: non picta (fractio lineam frangit).
 *
 * SYLLABAE ET TEXTUS:
 *   Textus cantatus FONS_PARVUS (forma.fons_textus), latitudo =
 *   runae * latitudo cellulae. Rubricae (*, †, ℟, ℣, et signum c)
 *   in tinctura RUBRICA; alii stili non picti (fons unus). Translatio
 *   [..] et verbatim non pinguntur (v1); signum alt supra systema,
 *   FONS_PARVUS, sinistra ad notas syllabae.
 *   PUNCTUM ALIGNATIONIS NOTARUM (Gregorius, "Graphical
 *   considerations"): neuma prima unius notae, pes, aut III+ notarum
 *   -> medium capitis primi; II notarum alia -> medium neumae;
 *   porrectus -> medium quadrati imaginarii ab initio eius.
 *   PUNCTUM ALIGNATIONIS TEXTUS: vocalis '{}' si data; aliter cursus
 *   vocalium primus (u post q aut g et i initialis ante vocalem
 *   consonantes sunt: Iesus, qui; diphthongi ae au oe eu, æ œ, ut
 *   una), medium eius; sine vocali medium syllabae.
 *   Puncta duo congruunt; syllaba: x_notarum, x_textus >= cursor.
 *   Cursor sequens = max(finis notarum + s, finis textus + cellula
 *   una si verbum novum sequitur, aliter finis textus). Inter
 *   syllabas eiusdem verbi '-' in medio intervalli pingitur si
 *   intervallum saltem dimidiam cellulam latum est (P4: syllabae
 *   paucis pixelis separatae ut verba legebantur).
 *   Littera prima syllabae primae ad initialem transit (si initialis
 *   adest).
 *
 * SYSTEMATA, LINEAE, PAGINAE:
 *   Margo paginae s undique. Systema: clavis in vigore primum (in
 *   omni systemate repetita), deinde syllabae. Summum systematis =
 *   minimum (linea summa - s, summum picti cuiusvis, textus alt);
 *   textus cantatus summum = max(linea_ima + s, imum picti notarum
 *   + s/IV); imum systematis = imum textus. Inter systemata s.
 *   FRACTIO (G15): z frangit et iustificat, Z frangit sine
 *   iustificatione; z+ custodem cogit, z- vetat. Aliter avide: syllaba
 *   quae non capit in systema novum transit; syllaba quae in systemate
 *   vacuo non capit intra se frangitur ad spatium ultimum quod capit
 *   (non ! nec /!), textus cum parte prima manet; nihil capiens ->
 *   excedit ad dextram (tabula secat), compositio semper finitur.
 *   Custos in fine omnis systematis cui nota sequitur (positio notae
 *   primae systematis sequentis; post clavem eius - clavis nova nihil
 *   mutat quia positio gabc locus in systemate est), s/II post
 *   elementum ultimum, latitudine in mensura systematis computata.
 *   IUSTIFICATIO: systema fractum avide aut per z: latitudo reliqua
 *   inter intervalla syllabarum aequaliter dividitur (residuum a
 *   sinistra). Systema ultimum et Z non iustificantur.
 *   INITIALIS (caput initial-style; absens = I): 0 nulla; I littera
 *   FONS_TITULI scalata ad altitudinem a linea summa ad imum textus
 *   systematis primi (scala integra = altitudo / XVI, min I),
 *   tinctura ATRAMENTUM, ad sinistram; systema primum tantum
 *   intenditur. II: altitudo duorum systematum, duo intenduntur.
 *   Scala ex s et fontibus ante compositionem computatur (non ex
 *   systematis factis), ergo latitudo nota est cum lineae franguntur.
 *   Annotationes (I-II lineae) FONS_PARVUS, RUBRICA, centratae supra
 *   initialem (sine initiali supra clavem). Titulus (name) FONS_TITULI
 *   centratus in summo paginae primae.
 *   Paginae systemata tota tenent; systema quod non capit paginam
 *   novam incipit; systema pagina altius solum in sua pagina
 *   (excedit).
 *
 * CAPSAE: syllabae locus = unio capsarum notarum et textus eius (pars
 * prima si fracta); notae locus = capsa capitis. Sine notis et textu:
 * pagina -1.
 *
 * DATUM NON COPIATUR: textus pictorum in octetos cantus monstrat aut
 * in piscinam compositionis; cantus vivere debet dum compositio
 * vivit.
 */
#ifndef GABC_COMPOSITIO_H
#define GABC_COMPOSITIO_H

/* <aedilis corpus="lib/gabc_compositio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "gabc.h"
#include "neumae_ductae.h"

nomen enumeratio {
    GABC_PICTUM_PARS,            /* neumae_ductae: pars + parametrum */
    GABC_PICTUM_RECTANGULUM,     /* linea systematis, addita,
                                  * connectens, caulis, divisio */
    GABC_PICTUM_TEXTUS,          /* cursus textus, fons FONS_* */
    GABC_PICTUM_INITIALIS        /* littera initialis scalata */
} GabcPictiGenus;

nomen enumeratio {
    GABC_TINCTURA_ATRAMENTUM,    /* COLOR_TEXT */
    GABC_TINCTURA_SYSTEMA,       /* lineae systematis et additae:
                                  * COLOR_TEXT_DIM */
    GABC_TINCTURA_RUBRICA        /* rubrum palettae */
} GabcTinctura;

nomen structura {
    GabcPictiGenus genus;
               s32 x;            /* PARS: origo; alia: angulus
                                  * sinister superior */
               s32 y;
        NeumaePars pars;         /* PARS */
               s32 parametrum;
               i32 latitudo;     /* RECTANGULUM, INITIALIS */
               i32 altitudo;
            chorda textus;       /* TEXTUS; INITIALIS: runa una */
               i32 fons;         /* TEXTUS: FONS_*; INITIALIS: scala */
      GabcTinctura tinctura;
               s32 syllaba;      /* index in GabcCantus.syllabae; -1 */
               s32 elementum;    /* index in syllaba.elementa; -1 */
               s32 nota;         /* index in GabcCantus.notae; -1 */
} GabcPictum;

nomen structura {
    s32 pagina;                  /* -1 = sine loco */
    s32 x;
    s32 y;
    i32 latitudo;
    i32 altitudo;
} GabcLocus;

nomen structura {
    i32 latitudo;                /* paginae, pixelis logicis */
    i32 altitudo;
    i32 spatium;                 /* par, VI..XLVIII */
    i32 fons_textus;             /* FONS_PARVUS */
    i32 fons_tituli;             /* FONS_TITULI */
} GabcCompositionisForma;

nomen structura {
    GabcCompositionisForma forma; /* qua composita est (pictura spatium
                                   * et paginam legit) */
    GabcPictum* picta;           /* omnia, ordine paginarum */
           i32  numerus_pictorum;
           i32* paginae;         /* numerus_paginarum + I: pagina p =
                                  * picta[paginae[p] .. paginae[p+I]) */
           i32  numerus_paginarum;
     GabcLocus* syllabae;        /* per syllabam cantus */
           i32  numerus_syllabarum;
     GabcLocus* notae;           /* per notam cantus */
           i32  numerus_notarum;
} GabcCompositio;

/* spatium VIII, CMLX x DLXXIII, FONS_PARVUS, FONS_TITULI */
vacuum
gabc_compositionis_formam_ordinariam (
    GabcCompositionisForma* forma);

/* NIHIL si memoria deficit aut forma mala (spatium impar aut extra
 * VI..XLVIII, latitudo < XVI * spatium); cantus cum vitiis componitur
 * quantum legitur. Paginae saltem una (vacua si cantus vacuus). */
GabcCompositio*
gabc_componere (
                constans GabcCantus* cantus,
    constans GabcCompositionisForma* forma,
                            Piscina* piscina);

/* syllaba cuius locus punctum continet; aliter syllaba eiusdem
 * systematis proxima in x; aliter -1 */
s32
gabc_compositio_syllaba_ad_punctum (
    constans GabcCompositio* compositio,
                        i32  pagina,
                        s32  x,
                        s32  y);

/* forma canonica (probationes): linea una per paginam, pictum,
 * locum; columnae tabulis separatae (grammatica in
 * probatio_gabc_compositio.c fixa) */
b32
gabc_compositionem_scribere (
    constans GabcCompositio* compositio,
          ChordaAedificator* exitus);

#endif /* GABC_COMPOSITIO_H */
