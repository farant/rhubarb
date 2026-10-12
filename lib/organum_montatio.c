/* organum_montatio.c - organum in ramo montare
 * (include/organum_montatio.h). Ex pictor_applicatio.c (S2-S5c)
 * generalizatum, consumptor secundus scriba (S6). */

#include "organum_montatio.h"
#include "filum.h"

#include <stdio.h>
#include <string.h>

/* radix + "/" + via (radix NIHIL aut vacua: via sola) */
interior constans character*
via_plena (
               Piscina* p,
    constans character* radix,
    constans character* via)
{
    chorda c;

    si (!radix || radix[ZEPHYRUM] == '\0')
    {
        redde via;
    }
    c = chorda_concatenare(chorda_ex_literis(radix, p),
        chorda_ex_literis("/", p), p);
    c = chorda_concatenare(c, chorda_ex_literis(via, p), p);
    redde chorda_ut_cstr(c, p);
}

/* manifestum unum; NIHIL si deest aut malum (vitia in stderr) */
interior Organum*
manifestum_legere (
                Piscina* p,
    InternamentumChorda* in,
     constans character* radix,
     constans character* via,
     constans character* praefixum)
{
     chorda  fons;
    Organum* o;
        Xar* vitia;
        i32  i;

    fons = filum_legere_totum(via_plena(p, radix, via), p);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "%s: manifestum abest: %s\n", praefixum, via);
        redde NIHIL;
    }
    o = organum_legere(fons, p, in, &vitia);
    per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
    {
        chorda* v;

        v = (chorda*)xar_obtinere(vitia, i);
        fprintf(stderr, "%s: %s: %.*s\n", praefixum, via,
            (int)v->mensura,
                (constans character*)v->datum);
    }
    redde o;
}

Organum*
organum_legere_viam (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* radix,
     constans character* via,
     constans character* praefixum)
{
    Organum* o;
    Organum* basis;

    si (!piscina || !intern || !via)
    {
        redde NIHIL;
    }
    si (!praefixum)
    {
        praefixum = "organum";
    }
    o     = manifestum_legere(piscina, intern, radix, via, praefixum);
    basis = o ? manifestum_legere(piscina, intern, radix,
        "apps/organum_commune.stml", praefixum) : NIHIL;
    si (!o || !basis || !organum_basim_addere(o, basis))
    {
        redde NIHIL;
    }
    redde o;
}

b32
organum_ramum_parare (
        constans Organum* organum,
    constans InsulaRamus* ramus,
         StatusSystemata* systemata,
      constans character* attributa_addita,
      constans character* clavis,
                 Piscina* piscina,
     InternamentumChorda* intern)
{
        Canon* canon;
       chorda  causa;
       chorda  elementum;
    StmlNodus* domini;

    si (!organum || !ramus || !piscina || !intern)
    {
        redde FALSUM;
    }
    /* canon PRIMUM (hospes elementum montatum numquam videt) */
    causa.mensura = ZEPHYRUM;
    causa.datum = NIHIL;
    canon = organum_canonem_generare(organum, piscina, intern, &causa);
    si (!canon)
    {
        fprintf(stderr, "%.*s: canon generatus: %.*s\n",
            (int)organum_genus(organum).mensura,
            (constans character*)organum_genus(organum).datum,
            (int)causa.mensura, (constans character*)causa.datum);
        redde FALSUM;
    }
    /* S7a: radix (hospes) canonem radicis repositorii accipit - visio
     * radicis montationes delegat; canon rami arborem totam
     * iudicaret */
    si (chorda_vacua(ramus->elementum))
    {
        insula_ponere_canonem(ramus->repo, canon);
    }
    alioquin
    {
        insula_ramus_canonem_ponere(ramus, canon);
    }

    /* elementum initiale: <genus [id] absentes addita/> (S4) */
    elementum = chorda_concatenare(chorda_ex_literis("<", piscina),
        organum_genus(organum), piscina);
    si (ramus->id.mensura > ZEPHYRUM)
    {
        elementum = chorda_concatenare(elementum,
            chorda_ex_literis(" id=\"", piscina), piscina);
        elementum = chorda_concatenare(elementum, ramus->id, piscina);
        elementum = chorda_concatenare(elementum,
            chorda_ex_literis("\"", piscina), piscina);
    }
    elementum = chorda_concatenare(elementum,
        organum_initium(organum, piscina), piscina);
    si (attributa_addita)
    {
        elementum = chorda_concatenare(elementum,
            chorda_ex_literis(attributa_addita, piscina), piscina);
    }
    elementum = chorda_concatenare(elementum,
        chorda_ex_literis("/>", piscina), piscina);
    si (!insula_ramum_initiare(ramus, chorda_ut_cstr(elementum,
            piscina)))
    {
        fprintf(stderr, "%.*s: elementum initiale: %.*s\n",
            (int)organum_genus(organum).mensura,
            (constans character*)organum_genus(organum).datum,
            (int)insula_causa(ramus->repo).mensura,
            (constans character*)insula_causa(ramus->repo).datum);
        redde FALSUM;
    }

    /* S3: status servatus restituitur (post initiationem, ante
     * dominos - scriptura anonyma), ramus deinde servatur */
    si (systemata && systemata->conservatio && clavis)
    {
        (vacuum)conservatio_ramum_addere(systemata->conservatio, ramus,
            organum, clavis);
    }

    /* domini POST initiationem, ex manifesto generati */
    domini = organum_dominos_generare(organum, piscina, intern);
    si (domini)
    {
        (vacuum)insula_ramus_dominos_legere(ramus, domini);
    }

    /* S5c: dimissio - campi popover rami */
    si (systemata && systemata->dimissio)
    {
        (vacuum)dimissio_ramum_addere(systemata->dimissio, ramus,
            organum);
    }
    redde VERUM;
}
