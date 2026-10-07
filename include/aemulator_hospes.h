/* aemulator_hospes.h - hospes emulatoris: nucleus + infans + ansa
 * (aemulator-plan B4)
 *
 * Locus UNUS ubi aemulator et pseudoterminale se tangunt: effusio
 * infantis in nucleum, responsa nuclei et initus vocantis ad
 * infantem, magnitudo ad ambos. Facies (probatio sine capite,
 * fenestra phasis E, vitrea postea) tenues manent: schirmum per
 * aemulator_hospes_aemulator legunt, claves per scribere mittunt,
 * ansam suam possident et pulsare vocant.
 *
 * PURUS: infantem per tabulam Pseudoterminale SOLAM tangit - nulla
 * vocatio systematis hic. Pons memoriae = probatio sine processu;
 * pons posix = concha vera. (Decisio X: wasm-abilis ut nucleus.)
 *
 * MEMORIA: omnia in creatione (nucleus, cauda scribendi, sacculus
 * lectionis); pulsus constans nihil allocat.
 */

#ifndef AEMULATOR_HOSPES_H
#define AEMULATOR_HOSPES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "aemulator.h"
#include "pseudoterminale.h"

nomen structura AemulatorHospes AemulatorHospes;

nomen structura {
    /* nucleus: magnitudo, identitas, effectus. effectus.responsum
     * a hospite OCCUPATUR (responsa ad infantem); campana et titulus
     * ad vocantem transeunt. */
    AemulatorConfiguratio aemulator;
    /* fames prohibenda: octeti ab infante per pulsum ad summum
     * (cat /dev/urandom ansam non devorat). 0 = ordinarius */
                      i32 octeti_per_pulsum;
    /* cauda ad infantem (initus + responsa); 0 = ordinaria.
     * Pars ultima responsis RESERVATUR: initus vocantis eam numquam
     * implet, ergo responsum numquam perit propter pastam magnam. */
                      i32 cauda_capacitas;
} AemulatorHospesConfiguratio;

/* Ordinaria: aemulator_configuratio_initiare; LXIV KiB per pulsum;
 * cauda LXIV KiB (IV KiB responsis reservati). */
vacuum
aemulator_hospes_configuratio_initiare (
    AemulatorHospesConfiguratio* cfg);

/* Hospes infantem POSSIDET (claudere eum claudit). Magnitudo nuclei
 * statim ad infantem mittitur (pt->amplitudo), ut ambo consentiant.
 * NIHIL si cfg malus, pt NIHIL, aut piscina deficit (pt tunc
 * clauditur). */
AemulatorHospes*
aemulator_hospes_creare (
                                 Piscina* piscina,
    constans AemulatorHospesConfiguratio* cfg,
                         Pseudoterminale* pt);

nomen structura {
    i32 lecti;     /* octeti ab infante in nucleum hoc pulsu */
    i32 missi;     /* octeti caudae ad infantem hoc pulsu */
    b32 mutatum;   /* schirmum fortasse mutatum (lecti > 0 aut
                    * amplitudo) - facies repingat */
    b32 finitus;   /* infans exivit ET effusio tota consumpta */
} AemulatorHospesPulsus;

/* Pulsus unus: caudam mittere quantum infans accipit; legere
 * (primum octetum usque ad mora_ms exspectans, deinde quod paratum
 * est usque ad octeti_per_pulsum) et in nucleum scribere; responsa
 * nuclei in caudam; caudam iterum mittere; exitum infantis
 * inspicere. mora_ms ut Pseudoterminale.legere (0 = numquam
 * exspectat, < 0 = sine fine). */
AemulatorHospesPulsus
aemulator_hospes_pulsare (
    AemulatorHospes* h,
                s32  mora_ms);

/* Initus ad infantem (claves iam codificatae, pasta). In caudam
 * ponitur et proximo pulsu mittitur; initus acceptus visum ad imum
 * reducit (decisio XXI). Reddit octetos acceptos (0..n):
 * minus quam n = cauda plena (praeter reservatum), reliquum vocans
 * postea offert. */
i32
aemulator_hospes_scribere (
     AemulatorHospes* h,
         constans i8* octeti,
                 i32  n);

/* Magnitudo: nucleus primum, deinde infans (SIGWINCH). FALSUM si
 * mala aut nucleus deficit (nihil mutatum). */
b32
aemulator_hospes_amplitudo (
    AemulatorHospes* h,
                i32  latitudo,
                i32  altitudo,
                i32  px_latitudo,
                i32  px_altitudo);

/* Visum movere per hospitem (facies aemulatorem constantem tenet):
 * ut aemulator_visum_movere; pulsus proximus 'mutatum' nuntiat. */
vacuum
aemulator_hospes_visum_movere (
    AemulatorHospes* h,
                s32  delta);

/* Schirmum legendum (cellulae, cursor, modi, textus). Valet usque ad
 * pulsum aut amplitudinem proximam. */
constans Aemulator*
aemulator_hospes_aemulator (
    constans AemulatorHospes* h);

/* Descriptor infantis ut ansa faciei in eo exspectet (decisio XIV);
 * -1 si nullus (pons memoriae). */
s32
aemulator_hospes_fossa (
    constans AemulatorHospes* h);

/* VERUM si infans exivit (exitus impletur) */
b32
aemulator_hospes_exitus (
          AemulatorHospes* h,
    PseudoterminaleExitus* exitus);

/* Infantem claudere (pt->claudere). Idempotens; post id pulsare
 * finitus reddit. */
vacuum
aemulator_hospes_claudere (
    AemulatorHospes* h);

#endif /* AEMULATOR_HOSPES_H */
