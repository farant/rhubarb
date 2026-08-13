/*
 * machina.h - Identitas et nota HUIUS machinae
 *
 * NOTA DE NOMINE: vexillum `-machina` per repositorium "machinae
 * legibile" significat. Hic "machina" machinam IPSAM significat -
 * duo sensus eiusdem vocis, sicut Anglice "machine ID" et
 * "machine-readable" coexistunt.
 *
 * ==========================================================
 * CUR HAEC BIBLIOTHECA
 * ==========================================================
 *
 * Mensurae quae machinam suam non nominant inter se conferri NON
 * possunt. Tempus probationum, mensurae schirmatis, exemplaria
 * visualia - omnia haec a machina pendent. Duo cursus in machinis
 * diversis comparati strepitum pariunt qui mutationem simulat.
 *
 * ==========================================================
 * RECUSAT, NON DEGENERAT
 * ==========================================================
 *
 * Si identitas deprehendi non potest, chorda VACUA redditur -
 * NUMQUAM constans ut "ignota". Ratio: identitas constans mensuras
 * OMNIUM machinarum in unum vas funderet, id est vitium ipsum quod
 * haec bibliotheca vetare debet. Officium quod tacite degenerat
 * operari VIDETUR dum datum corrumpit - pessimum genus fracturae,
 * quia nemo quaerit.
 *
 * ==========================================================
 * OPACITAS
 * ==========================================================
 *
 * Identitas est SHA-256 fontis stabilis, ad XVI litteras hex
 * truncatum. Fons ipse (UUID ferramenti aut /etc/machine-id)
 * numquam emittitur: sigillum unam viam it, ergo identitas in
 * plagulis communibus aut in git tuto iacere potest.
 */

#ifndef MACHINA_H
#define MACHINA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* XVI litterae hex = LXIV bita entropiae - collisio inter machinas
 * paucas impossibilis, et in tabulis legibile manet */
#define MACHINA_IDENTITAS_LONGITUDO XVI

/* ============================================================
 * Typi
 * ============================================================ */

/*
 * MachinaNota - Identitas PLUS condiciones grossae
 *
 * Identitas sola dicit "eadem capsa"; haec dicunt "eaedem
 * condiciones". Versio systematis sola tempora decima parte movere
 * potest, ergo cum figura saliet scire voles quid mutatum sit.
 *
 * @valida: si FALSUM, identitas VACUA est et causa dicit cur
 */
nomen structura {
    b32    valida;
    chorda identitas;      /* XVI litterae hex, aut vacua */
    chorda systema;        /* e.g. "Darwin 23.6.0" */
    chorda architectura;   /* e.g. "arm64" */
    i32    nuclei;         /* numerus nucleorum agentium */
    chorda causa;          /* si !valida */
} MachinaNota;

/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * machina_identitas_ex_fonte - Identitatem ex fonte quolibet fingere
 *
 * NUCLEUS PURUS: platformam non tangit, ergo probari potest.
 * Fontes diversi identitates diversas parere DEBENT - id est tota
 * ratio cur haec functio seorsum exposita sit. Officium quod
 * constantem redderet omnes probationes stabilitatis praeteriret;
 * sola probatio quae fontes DUOS confert id deprehendit.
 *
 * Redde: XVI litterae hex, aut chorda vacua si fons vacuus est.
 */
chorda
machina_identitas_ex_fonte (constans chorda fons, Piscina* piscina);

/*
 * machina_identitas - Identitas huius machinae
 *
 * Scala fontium (primus qui respondet vincit):
 *   I.  macOS: gethostuuid()
 *   II. Linux: /etc/machine-id
 *
 * Neuter praesto? Chorda VACUA - vide RECUSAT supra.
 *
 * Stabilis per cursus, per reboot, per renovationes systematis.
 * Nullam plagulam status scribit, ergo nec perire nec divergere
 * potest.
 */
chorda
machina_identitas (Piscina* piscina);

/*
 * machina_nota - Identitas cum condicionibus grossis
 */
MachinaNota
machina_nota (Piscina* piscina);

#endif /* MACHINA_H */
