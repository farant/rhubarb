/* vitrea_servus.h - transportus ALTER applicationis vitreae
 *
 * Eadem capsula, idem registrum methodorum, super HTTP loco
 * WebKit. Non est portatio vitreae: est SECUNDA IMPLEMENTATIO
 * SUTURAE MISSORIS quam internuntius.h ab initio nominavit
 * ("de WebKit NIHIL scit... idem involucrum postea super tcp aut
 * stdio currere posset"). Fenestra et servus sunt duo transportus
 * unius applicationis, non duae applicationes.
 *
 * USUS (applicatio methodos suas semel scribit):
 *
 *   interior vacuum
 *   _methodos_praebere (Internuntius* inx, InternuntiusModus modus,
 *       vacuum* datum)
 *   {
 *       Forum* f = (Forum*)datum;
 *       (vacuum)internuntius_praebere(inx, "res_legere", _legere, f);
 *       si (modus == INTERNUNTIUS_MODUS_LOCALIS)
 *       {
 *           (vacuum)internuntius_praebere(inx, "fumus_modus",
 *               _fumus, f);   (harnesium: fenestrae solum)
 *       }
 *   }
 *
 *   modus fenestrae: inx = internuntius_creare(p, vitrea_missor, v);
 *                    _methodos_praebere(inx, LOCALIS, &forum);
 *   modus servi:     figura.praebitor = _methodos_praebere;
 *                    vitrea_servus_creare(p, &figura);
 *
 * PAGINA IMMUTATA: sicut vitrea 'window.internuntius' scripto
 * documenti-initii iniicit, servus gemellum suum (fetch-basatum)
 * in <head> inserit dum paginam ex capsula servit. Applicatio
 * transportum suum NON videt neque in C neque in JS.
 *
 * ASSETA: via una per filum capsulae, in creatione registrata.
 * Superficies servita = contenta capsulae EX CONSTRUCTIONE -
 * nulla congruentia generalis, ergo nulla traversalis semitae
 * possibilis (vias petitionis numquam in systema filorum vertimus).
 *
 * PULSUS NULLI (limes notus): internuntius_eventum_mittere ->
 * 'audire' super HTTP petitionis-responsi non transit. Applicatio
 * quae pulsibus utitur super hunc transportum TACITE non
 * laboraret - ergo 'audire' huius scripti culpam CLAMAT loco
 * silentii. Emendatio nominata: SSE (/eventus + EventSource).
 *
 * FILUM UNUM: hospitium filo uno currit et tractator ad finem
 * currit ante petitionem proximam - ergo sedes una responsi tuta
 * est. Si id umquam mutaverit, haec sedes prima franget.
 *
 * PLATFORMA: C89 purum super hospitium - WebKit NIHIL. Haec
 * plagula in Linux aedificat ubi vitrea_macos.m non potest.
 */

#ifndef VITREA_SERVUS_H
#define VITREA_SERVUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "capsula.h"
#include "internuntius.h"

nomen structura VitreaServus VitreaServus;

/* Configuratio - 0/NIHIL = defalta */
nomen structura {
    Capsula*              capsula;         /* asseta applicationis */
    constans character*   via_initialis;   /* NIHIL = "index.html" */
    InternuntiusPraebitor praebitor;       /* registratio methodorum */
    vacuum*               praebitor_datum;
    /* NIHIL = "127.0.0.1": loopback de industria. Expositio in
     * reticulum ACTUS EXPLICITUS sit ("0.0.0.0"), numquam quod
     * per neglegentiam accidit. */
    constans character*   hospes;
    /* Tessera secreta (chorda hexadecimalis, >= XXXII characteres).
     *
     * PORTA NATIVITATIS: si 'hospes' datur (id est, expositio ultra
     * loopback) et haec abest, vitrea_servus_creare NIHIL reddit et
     * causam nominat. Non monitum - REFUSIO. Forum sine tessera
     * expositum casus esse non debet, sed impossibile.
     *
     * Bibliotheca UNDE venerit nescit: lima, arca, environmentum.
     * Ea sutura est per quam arca (01KYAMMMF58F) postea succedet
     * sine ulla mutatione huius codicis. */
    constans character*   tessera;
    i32                   portus;          /* 0 = auto-selectus */
    b32                   acta_accessus;   /* linea per petitionem */
} VitreaServusConfiguratio;

/* Creare: hospitium ligare + internuntium creare + praebitorem
 * vocare (modo PUBLICO) + vias assetorum registrare.
 * Redde NIHIL si ligatio aut praebitio fallit. */
VitreaServus*
vitrea_servus_creare (
    Piscina*                              piscina,
    constans VitreaServusConfiguratio*    configuratio);

/* Gressus in ansa usque ad sistere */
vacuum
vitrea_servus_currere (VitreaServus* servus);

/* Gyrus unus (pro ansa hospitis externa - e.g. applicatio quae
 * fenestram ET servum simul gerit). Redde FALSUM post sistere. */
b32
vitrea_servus_gressus (
    VitreaServus* servus,
    s32           ms_maximae);

vacuum
vitrea_servus_sistere (VitreaServus* servus);

vacuum
vitrea_servus_destruere (VitreaServus* servus);

/* Portus ligatus (utilis cum portus 0 = auto) */
i32
vitrea_servus_portus (constans VitreaServus* servus);

/* Internuntius interior - pro applicatione quae pulsus mittere
 * aut registrum inspicere velit */
Internuntius*
vitrea_servus_internuntius (VitreaServus* servus);

#endif /* VITREA_SERVUS_H */
