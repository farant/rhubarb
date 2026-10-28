/* veredarius.h - petitio HTTP una per interfaciem (communio-plan-5
 * T1; communio-spec IX, D11)
 *
 * Veredarius (cursus publicus) petitionem fert et responsum refert;
 * quis eum praebet hospitis est: in macOS et in instrumentis
 * veredarius domesticus super http_vocatio (veredarius_domesticus.h),
 * in iOS testa per URLSession (nulla cryptographia in fasciculo), in
 * Android nullus adhuc. Hoc caput solum interfaciem habet - nihil
 * reticuli ligat: qui id includit TLS non trahit.
 *
 * CONTRACTUS:
 * - petitio (url, auctoritas, corpus, mimen) solum intra petere
 *   legitur: vocans eam statim post reditum delere potest;
 * - finis numquam intra petere vocatur; post abrumpere numquam;
 *   aliter semel;
 * - corpus responsi in piscina vocantis (petere) habitat;
 * - causa numquam URL nec auctoritatem continet;
 * - successus = responsum HTTP advenit (quovis statu); FALSUM = rete,
 *   tempus, abruptio - status tum 0. */
#ifndef VEREDARIUS_H
#define VEREDARIUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen enumeratio {
    VEREDARIUS_GET = ZEPHYRUM,
    VEREDARIUS_POST,
    VEREDARIUS_PUT
} VeredariusMethodus;

nomen structura {
     VeredariusMethodus  methodus;
     constans character* url;
     constans character* auctoritas;   /* "Bearer ..." aut NIHIL */
                 chorda  corpus;       /* vacua = sine corpore */
     constans character* mimen;        /* Content-Type si corpus */
                    i32  tempus_ms;    /* 0 = XXX s */
} VeredariusPetitio;

nomen structura {
       b32 successus;
       i32 status;
    chorda corpus;
    chorda causa;
} VeredariusResponsum;

nomen vacuum (*VeredariusFinis)(
                          vacuum* ctx,
    constans VeredariusResponsum* responsum);

nomen structura Veredarius {
    /* signum: manubrium pro abrumpere; FALSUM = petitio non incepta
     * (finis numquam) */
    b32    (*petere)(vacuum* sui, constans VeredariusPetitio* petitio,
                     Piscina* piscina, VeredariusFinis finis,
                     vacuum* ctx, i32* signum);
    /* idempotens; signum vetus aut finitum innocuum */
    vacuum (*abrumpere)(vacuum* sui, i32 signum);
    vacuum* sui;
} Veredarius;

#endif /* VEREDARIUS_H */
