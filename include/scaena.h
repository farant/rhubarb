/*
 * scaena.h - scaena: prospectus stantes cum rebus tangendis
 *
 * Modus ludi primus: pictura stans (graphis.h), sub ea descriptio,
 * et res in pictura quas tangere licet - quaeque textum dat et
 * fortasse exitum in prospectum alium. Haec bibliotheca plagulam
 * scaenarum legit, picturas componit, omnia probat, et prospectum
 * in massam octetorum vertit quam volumen servat et latus legit.
 * Nihil de volumine aut de vico scit
 * (project-specs/graphis-spec.md 20-24).
 *
 * PLAGULA SCAENARUM (prospectus plures in una licent):
 *
 *   <scaenae>
 *     <prospectus titulus="hut-hearth" pictura="casa_focus">
 *       <descriptio>A low room. The fire is lit.</descriptio>
 *       <res titulus="hearth">A stone hearth. Something simmers.</res>
 *       <res titulus="turn" exitus="hut-door">You turn round.</res>
 *     </prospectus>
 *   </scaenae>
 *
 *   prospectus  titulus: nomen quo exitus eum petunt et quo in
 *               volumine stat (I .. LXIV litterae ex a-z, 0-9,
 *               '-' et '_'). pictura: nomen picturae, quam vocans
 *               reddit (ut partes graphidis: a-z, 0-9, '_')
 *   descriptio  una, non vacua: textus sub pictura semper visus
 *   res         titulus: titulus regionis in pictura. Textus eius
 *               tangenti ostenditur (non vacuus). exitus: titulus
 *               prospectus alius in hac plagula, aut "mundus"
 *               (foras; in hoc gradu textus solus)
 *
 * Textus: spatia et lineae novae in spatium unum contrahuntur,
 * margines tolluntur. Longitudo summa: descriptio MMXLVIII, res DXII.
 *
 * CONVENIENTIA picturae et scaenae, utrimque: omnis regio picturae
 * rem habet, omnis res regionem. Res ordine regionum picturae
 * redduntur (res[n - I] regioni n tabulae dominorum respondet),
 * quocumque ordine scriptae sunt.
 *
 * RECUSAT, cum plagula et linea: STML malum; radicem aliam quam
 * <scaenae>; plagulam sine prospectu; elementum aut attributum
 * ignotum; titulum prospectus malum aut iteratum; picturam non
 * inventam aut recusatam (tum plagula et linea picturae aut partis
 * eius); descriptionem absentem, iteratam aut vacuam; rem sine
 * titulo, sine textu aut iteratam; rem cuius regio in pictura non
 * est; regionem picturae sine re; exitum in prospectum qui in
 * plagula non est; exitum in prospectum ipsum; textum nimis longum.
 *
 * MASSA (scaena_scribere / scaena_legere): verbum "SCAENA01", deinde
 * campi numerati ordine fixo, numeri XXXII bitorum ordine magno:
 * titulus, descriptio, numerus rerum, res quaeque (titulus, textus,
 * exitus), indices picturae (CCCLX x CCXL octeti), domini (totidem).
 * Prospectus idem, octeti idem.
 */

#ifndef SCAENA_H
#define SCAENA_H

/* <aedilis corpus="lib/scaena.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "graphis.h"

#define SCAENA_EXITUS_MUNDUS       "mundus"
#define SCAENA_TITULUS_MAXIMUS     LXIV
#define SCAENA_DESCRIPTIO_MAXIMA   (II * MXXIV)
#define SCAENA_TEXTUS_MAXIMUS      DXII
#define SCAENA_PROSPECTUS_MAXIMI   LXIV

nomen structura {
    chorda titulus;     /* titulus regionis */
    chorda textus;
    chorda exitus;      /* vacuus = nullus */
} ScaenaRes;

nomen structura {
       chorda  titulus;
       chorda  descriptio;
           i8* indices;   /* palettae, GRAPHIS_LATITUDO x
                           * GRAPHIS_ALTITUDO */
           i8* domini;    /* 0 = nullus; n = res[n - I] */
    ScaenaRes* res;
          i32  rerum;
} ScaenaProspectus;

nomen structura {
                 b32  successus;
    ScaenaProspectus* prospectus;  /* NIHIL si recusata */
                 i32  numerus;
              chorda  error;       /* Anglice, sine linea nova */
              chorda  plagula;     /* nomen picturae aut partis in
                                    * qua recusatio nata est; vacua
                                    * = plagula scaenarum ipsa */
                 i32 linea;
} ScaenaFructus;

/*
 * scaena_struere - plagulam scaenarum in prospectus componere
 *
 * @textus:    plagula scaenarum tota
 * @picturae:  picturam nomine reddit (plagula <pictura>); chorda
 *             vacua = non inventa
 * @partes:    partes picturarum reddit (graphis.h); NIHIL = nullae
 * @contextus: utrique fonti traditur
 * @piscina:   prospectus et nuntius
 *
 * Redde: successus VERUM cum omnibus prospectibus, ordine plagulae;
 * aut FALSUM cum errore, plagula et linea - nihil dimidium
 */
ScaenaFructus
scaena_struere (
         chorda  textus,
    GraphisFons  picturae,
    GraphisFons  partes,
         vacuum* contextus,
        Piscina* piscina);

/*
 * scaena_scribere - prospectum in massam vertere
 *
 * Redde: massa tota; chorda vacua (datum NIHIL) si prospectus aut
 * piscina NIHIL aut memoria deficit
 */
chorda
scaena_scribere (
    constans ScaenaProspectus* prospectus,
                      Piscina* piscina);

/*
 * scaena_legere - massam in prospectum relegere
 *
 * @massa:   ut scaena_scribere eam dedit (non mutatur; prospectus
 *           nihil in ea tenet)
 * @exitus:  impletur
 * @piscina: campi prospectus
 *
 * Redde: VERUM; FALSUM (exitus non fidendus) si verbum primum aliud
 * est, massa brevior quam campi dicunt, campus fines superat, octeti
 * supersunt, aut dominus numerum rerum excedit
 */
b32
scaena_legere (
              chorda  massa,
    ScaenaProspectus* exitus,
             Piscina* piscina);

#endif /* SCAENA_H */
