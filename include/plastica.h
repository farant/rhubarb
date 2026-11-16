/*
 * plastica.h - plastica: descriptio textualis in statuam
 *
 * Res (puteus, saepes, saxum) describitur plagula STML brevi quae
 * solida simplicia enumerat; plastica_legere eam in Statuam finitam
 * (statua.h) vertit, eandem quam lector GLB dat. Omnia quae statuam
 * pingunt aut condunt eam sine mutatione accipiunt
 * (project-specs/plastica-spec.md IV.1).
 *
 * Nihil de mundo scit: nullum caput mundus_* hic includitur (spec
 * 9a). Nihil de plagulis aut pixelis.
 *
 * FORMA:
 *
 *   <res titulus="well">
 *     <grex locus="0 0 0" gyrus="0">
 *       <capsa titulus="murus" magnitudo="2 1 2" locus="0 0 0"
 *              color="#8a8f94" signum="solidum"/>
 *     </grex>
 *   </res>
 *
 * MENSURAE: unitas una = unitas mundi (persona II unitates alta); y
 * sursum; humus y = 0. Anguli in gradibus.
 *
 * locus="x y z"   centrum BASIS solidi (res in humo sedet);
 *                 ordinarius 0 0 0
 * gyrus="g"       circa axem erectum
 * rotatio="x y z" versio plena, post gyrum (rara)
 * color="#rrggbb" color planus (aut brevis "#rgb"); ordinarius
 *                 albus
 * signum          "solidum" aut "gressibile"; absens = nullum
 *                 (pars ornamenti: gressum non impedit)
 * invisibilis     "verum": pars non pingitur sed signum tenet
 *                 (obex invisus rei tenuis; spec 14a) - materia
 *                 alpha 0, limen excisionis 0.5
 * titulus         nomen partis (in tabula 'partes')
 * textura         genus texturae genitae ("saxum", "tabulae" ...) aut
 *                 titulus texturae manu scriptae; cum semine
 *                 (semen="n") et coloribus (colores="#o #m #c";
 *                 absentes ex 'color' fiunt). Coordinatae per faciem,
 *                 XVI texella per unitatem. Vide plastica_textura.h
 *
 * SOLIDA (omnia: titulus, locus, gyrus, rotatio, color, signum,
 * invisibilis; basis in y = 0 spatii sui):
 *
 *   capsa      magnitudo="x y z"
 *   cylindrus  radius, altitudo; radius_summus (ordinarius = radius;
 *              0 = conus); latera (ordinarius VIII; III .. LXIV)
 *   sphaera    radius AUT magnitudo="x y z" (ellipsoides: saxa,
 *              comae arborum); segmenta circum (ordinarius VI;
 *              III .. XXXII; zonae = segmenta / II, minimum II);
 *              asperitas 0 .. 1 cum semine (semen="n"): radius
 *              cuiusque verticis variatur - vertices communes
 *              valorem eundem tenent, rimae nullae
 *   cuneus     magnitudo="x y z": altitudo plena ad marginem -x,
 *              ad nihilum cadit ad +x (tecta, clivi)
 *   extrusio   puncta="x,z x,z ..." (III .. LXIV), altitudo:
 *              lineamentum in plano humi sursum extrusum. Concavum
 *              licet; ordo punctorum uterque (invertitur si opus);
 *              lineamentum se secans recusatur
 *   tornus     puncta="r,y r,y ..." (II .. LXIV), latera: profilum
 *              circa axem erectum tornatum. r >= 0. Superficies ad
 *              DEXTRAM itineris spectat (r dextrorsum, y sursum):
 *              sursum eundo extrorsum, deorsum eundo introrsum -
 *              sic paries interior situlae fit. Fines non
 *              clauduntur nisi profilum ad r = 0 incipit aut desinit
 *
 * GREGES (omnes: titulus, locus, gyrus, rotatio):
 *
 *   grex       transformatio sola
 *   speculum   axis="x" aut "z": liberi ET imago eorum trans planum
 *              ubi coordinata illa 0 est. Imago vera est (vertices
 *              reflexi, ordo verticum inversus), non scala negativa
  *   series     numerus="n" (I .. CCLVI), gressus="x y z",
 *              gyrus_gressus="g": liberi n vicibus, exemplar k per
 *              k gressus motum et k gyros versum.
 *              semen_gressus="n": exemplar k semini liberorum k * n
  *              addit (saxa aspera alia quaeque; textura genita
 *              quoque alia fit - tegula nova per exemplar).
 *              motus_texturae_gressus="u v": exemplar k texturam
 *              liberorum k * (u v) texellis movet - pannus alius
 *              tegulae EIUSDEM, sine textura nova (saxa muri)
 *              magnitudo_gressus="x y z": exemplar k magnitudini
 *              liberorum k * (x y z) addit (capsa, cuneus, sphaera
 *              cum magnitudine) et altitudini cylindri et
 *              extrusionis k * y - scalae: gradus idem, quisque
 *              altior. Radii et puncta non crescunt. Magnitudo quae
 *              in exemplari non positiva fit recusatur. Series
 *              nidificatae gressus suos addunt
 *   sparsio    numerus="n" (I .. CCLVI), semen="n", area="x z":
 *              liberi n vicibus intra aream circa centrum sparsi;
 *              gyrus_varius="verum" quodque fortuito vertit;
 *              magnitudo_varia="v" (0 .. 1) quodque scala inter
 *              1 - v et 1 + v mutat
 *
  * motus_texturae  "u v" in texellis: coordinatae texturae in omni
 *                 facie solidi tot texellis augentur (ordinarius
 *                 0 0). Coordinatae a medio solidi numerantur: sic
 *                 tegula manu scripta in facie collocatur sine
 *                 ordinibus rotatis (capsa I lata, tegula XVI:
 *                 motus_texturae="8 0")
 *
 * OBEX: <res titulus="..." obex="x z"> obicem invisum solidum addit,
 * x latum et z profundum (unus numerus = quadratus), I unitate
 * altum, in origine rei: pars ULTIMA, titulo "obex". Pro re tenui
 * (palus, truncus) quae per se nullam cellam gressus claudit. Obex
 * alibi aut alter: capsa invisibilis manu scripta, ut ante.
 *
 * TEXTURA MANU SCRIPTA (liber <res>, ante usum; nodum non facit):
 *
 *   <textura titulus="t" latitudo="4" colores="#222 #fc5">
 *     aaaa abba abba aaaa
 *   </textura>
 *
 * Litterae a .. p colores ordine indicant; maxima XVI x XVI
 * (plastica_textura.h).
 *
 * Fortuita ex semine solo (generator integer proprius): semen idem,
 * res eadem in omni machina.
 *
 * RECUSAT praeterea: latera aut segmenta extra fines; profilum aut
 * lineamentum cum punctis nimis paucis, puncto iterato aut r
 * negativo; numerum 0; rem cuius nodi MMMMXCVI aut triangula CC
 * milia excedunt (repetitores nidificati).
 *
 * Solidum unum = nodus unus cum reti uno; normales per faciem
 * (umbratio plana); facies extrorsum spectant.
 *
 * RECUSAT, causa verbis planis Anglicis et linea plagulae: textum
 * qui STML non est; radicem aliam quam <res>; elementum aut
 * attributum ignotum; magnitudinem absentem, non numericam, nullam
 * aut negativam; signum ignotum; texturam ignotam aut male scriptam;
 * rem sine solidis, aut cum plus quam LXIV texturis diversis.
 * Statua recusata NIHIL est - numquam dimidia.
 *
 * DETERMINATA: textus idem statuam eandem dat, octetum pro octeto.
 *
 * FIGURA (radix altera): res cum articulis, quae moveri potest
 * (project-specs/plastica-motus-spec.md IV.1).
 *
 *   <figura titulus="limax">
 *     <articulus titulus="corpus" locus="0 0.4 0">
 *       <sphaera radius="0.5" color="#6c9"/>
 *       <articulus titulus="oculus_sin" locus="0.2 0.5 0.3">
 *         <sphaera radius="0.08" color="#fff"/>
 *       </articulus>
 *     </articulus>
 *   </figura>
 *
 * <figura> ut <res> legitur (solida, greges, texturae manu scriptae,
 * semen), praeter: attributum 'obex' non habet (persona, non res
 * loci), et articulos tenere potest.
 *
 * ARTICULUS: grex nominatus qui nodum UNUM facit; origo eius cardo
 * est (circa quem pars vertitur). Positura quietis = plagula ut
 * scripta.
 *
 *   titulus   necessarius, non vacuus, in plagula unicus
 *   locus     cardo, in spatio parentis; ordinarius 0 0 0
 *   gyrus, rotatio   ut in grege (positura quietis)
 *
 * Articulus sub <figura> ipsa aut sub alio articulo stat: nidus
 * articulorum ossa figurae est. Solida et greges intra articulum
 * cum eo moventur. Maxime LXIV articuli.
 *
 * Elementa <positura>, <cyclus> et <membrum>, liberi <figura>
 * ipsius, hic praetereuntur (plastica_motus.h ea legit); alibi
 * ignota sunt.
 *
 * RECUSAT praeterea, causa et linea: <articulus> in <res>; articulum
 * intra <grex>, <speculum>, <series> aut <sparsio>; titulum
 * absentem, vacuum aut iteratum; articulos plus quam LXIV; 'obex'
 * in <figura>; figuram sine solidis.
 */

#ifndef PLASTICA_H
#define PLASTICA_H

/* <aedilis corpus="lib/plastica.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "statua.h"

#define PLASTICA_ARTICULI_MAXIMI  LXIV

nomen structura {
       b32  successus;
    Statua* statua;   /* finita (statua_finire); NIHIL si recusata */
    chorda* partes;   /* statua->nodorum tituli, ordine nodorum;
                       * vacuus ubi nodus titulum non habet; NIHIL si
                       * recusata. Extra Statuam manet (spec 14b) */
       i32* articuli; /* indices nodorum qui articuli sunt, ordine
                       * plagulae (parens ante liberos); titulus
                       * articuli k = partes[articuli[k]]; NIHIL si
                       * nulli aut recusata */
       i32 articulorum;  /* 0 pro <res> */
       b32 figura;       /* radix <figura> fuit (non <res>) */
    chorda error;    /* causa recusationis, Anglice, sine linea
                       * nova; vacua si successus */
       i32 linea;    /* linea plagulae (ab I); 0 si ignota aut
                       * successus */
} PlasticaFructus;

/*
 * plastica_legere - descriptionem in statuam vertere
 *
 * @textus:  plagula tota (non mutatur; statua nihil in ea tenet)
 * @piscina: statua, tabula partium et nuntius erroris
 *
 * Redde: successus VERUM cum statua finita et partibus, aut FALSUM
 * cum errore et linea
 */
PlasticaFructus
plastica_legere (
     chorda  textus,
    Piscina* piscina);

/*
 * plastica_legere_seminis - ut plastica_legere, semine addito
 *
 * @semen: 0 .. 1000000; omni semini rei additur (asperitas sphaerae,
 *         textura genita, sparsio) - plagula eadem rem aliam eiusdem
 *         generis dat (saxum alia forma, alius pannus texturae).
 *         Semen 0: idem ac plastica_legere
 *
 * Redde: ut plastica_legere; semen extra fines: recusatio, linea 0
 */
PlasticaFructus
plastica_legere_seminis (
     chorda  textus,
        i32  semen,
    Piscina* piscina);

structura StmlNodus;

/*
 * plastica_legere_arboris - ut plastica_legere_seminis, ex arbore
 * STML iam lecta
 *
 * @radix: elementum <res> aut <figura> (stml.h); non mutatur. Lineae
 *         nodorum ut datae in 'linea' redduntur
 *
 * Pro plastica_motus, quae figuram ex ossibus et solidis componit.
 */
PlasticaFructus
plastica_legere_arboris (
    constans structura StmlNodus* radix,
                             i32  semen,
                         Piscina* piscina);

#endif /* PLASTICA_H */
