/* chronica.h - CHRONICA in latere vici (genus "chronica"; spec
 * project-specs/chronica-spec.md C3). Manifestum
 * apps/chronica/organum.stml; fontes (chronica_fontes.h) a vico
 * dati, communes omnibus lateribus; status visus in ramo.
 *
 * VISUS ANNI: in summo omnis paginae annus magnus (FONS_TITULI):
 * "1446 BC", "AD 1274". Infra eventa anni ordine tabulae: descriptio
 * (paragraphus), deinde linea parva obscura - dies ("15 Jul"; mensis
 * solus "Jul"), genus, personae (", "), locus, " · " iuncta, vacua
 * omissa. Paginae per lector_paginare (fractiones domus: descriptio
 * longa in paginam sequentem fluit, numquam praecisa). Linea ima:
 * "< Timeline - AD 1274 - 1/2 >" (sagittae anni).
 * CLAVES: h / l = annus prior / proximus CUM EVENTIS; j / k = pagina
 * proxima / prior intra annum; sagittae idem (sinistra / dextra =
 * h / l, deorsum / sursum = j / k). In fine: nihil.
 * MUS: ictus in sagitta pedis sinistra / dextra = h / l; ictus in
 * dimidio superiore / inferiore paginae = k / j.
 * POSITIO: annus visus in praesentia voluminis ("chronica/annus")
 * quotiens mutatur; apertio sine argumento eum restituit; numquam
 * visus: annus cum eventis hodie proximus. Annus (restitutus, datus,
 * aut mutatione vacuus factus) sine eventis: proximus cum eventis
 * (aequalitas: prior), pagina 0. Pagina ultra finem: ultima.
 * MUTATIO: tabula in omni compositione legitur (chronica_tabula: sine
 * mutatione eadem, nihil reaedificatur).
 * VACUA: nulla eventa - "Write <event year=\"1274\">...</event> in a
 * note"; pes "Timeline".
 * VISUS FLUMINIS (C3b; ORDINARIUS): eventa omnium annorum in ordine
 * continuo, paginae plenae (sine fractione inter annos). Columna
 * sinistra (quinta pars) annum magnum monstrat ad lineam primam
 * eventus primi anni; annus in paginam sequentem fluens in summo eius
 * iteratur. Columna dextra (quattuor quintae) descriptiones et
 * lineas parvas. j / k paginae; h / l ad paginam ubi annus prior /
 * proximus incipit (annus proximus in eadem pagina incipiens
 * praeteritur). Positio: annus in summo paginae (communis cum visu
 * anni) et pagina inter paginas ab anno eius incipientes. Pes
 * "Timeline · 1446 BC – AD 1274 · 1/9" (annus unus: "Timeline · AD
 * 1250 · 2/9"). v visum vertit (flumen <-> annus).
 * LOCUS: "chronica/flumen/1274", "chronica/annus/-1446"; titulus loci
 * ut pes. Comes (Cmd+T) anni sui est. Argumentum nudum ("1274", "")
 * flumen aperit.
 * ELECTIO (C4a; visus ambo): Tab rem proximam paginae eligit -
 * eventum (descriptio), deinde personas eius ordine, deinde locum;
 * post ultimam paginae prima paginae sequentis (flumen: trans annos;
 * visus anni: intra annum), S-Tab retro. Res ad paginam pertinet ubi
 * initium eius stat. Electa inversa pingitur (etiam per lineas
 * fluens); pes "-> note: Notes · Thomas Aquinas" (eventus: titulus
 * libri), "-> person: Moses", "-> place: Egypt". Esc electionem
 * tollit (nulla: latus prius per vicum). Motus paginae aut anni
 * (j k h l, sagittae, ictus) eam tollit. Electa mutatione deleta =
 * nulla (pes ordinarius).
 * MUS: ictus in textu eventus, personae aut loci eum eligit (punctum
 * locale -> linea -> columna -> offset); ictus extra textum in
 * dimidio superiore / inferiore paginam vertit ut prius. (C4b: Enter
 * aut ictus in electo domum aperit; C5: visus personae et loci.) */

#ifndef CHRONICA_H
#define CHRONICA_H

/* <aedilis corpus="lib/chronica.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "volumen.h"
#include "fasti.h"
#include "chronica_fontes.h"

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
          ChronicaFontes* fontes;        /* a vico */
                 Volumen* volumen;       /* praesentia positionis */
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* figurae_ctx;   /* privatum: cache paginarum */
    /* hodie (annus prope quem prima apertio); vicus: hodie_vici,
     * probationes fictum. NIHIL aut FALSUM = annus primus */
                     b32  (*hodie)(vacuum* ctx, Dies* hodie);
                  vacuum* hodie_ctx;
} ChronicaMontatio;

/* montare in ramo <chronica id="..."> (manifestum infixum);
 * superficies latitudo x altitudo (0 = ordinaria) */
b32
chronica_montare (
       ChronicaMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
     constans character* id,
         ChronicaFontes* fontes,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
        StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = ChronicaMontatio* */
Componens*
chronica_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* argumentum: "" = positio servata; "1274", "-1446" (annus; sine
 * eventis: proximus) - visus fluminis; semita "chronica/annus/<n>",
 * "chronica/flumen/<n>". Malum (0, non numerus): FALSUM, nihil
 * mutatum. */
b32
chronica_argumentum_ponere (
       ChronicaMontatio* m,
     constans character* argumentum);

/* facies.semitam_secuta (historia, $via, restitutio) */
b32
chronica_semitam_secuta (
    vacuum* ctx);

/* facies.locus */
chorda
chronica_locus (
     vacuum* ctx,
    Piscina* piscina);

/* facies.locus_titulus */
chorda
chronica_locus_titulus (
     vacuum* ctx,
    Piscina* piscina);

#endif /* CHRONICA_H */
