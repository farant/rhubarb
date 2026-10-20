#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "stml.h"
#include "insula.h"
#include <stdio.h>

interior i32 actus_numerus = 0;

interior vacuum
actarius_numerare (StmlNodus* radix, chorda via, vacuum* ctx)
{
    (vacuum)radix; (vacuum)via; (vacuum)ctx;
    actus_numerus++;
}

interior vacuum
monstrare (InsulaRepositorium* repo, Piscina* p, constans character* tit)
{
    chorda t;
    t = insula_scribere(repo, p);
    imprimere("%s: versio=%d actus=%d liberi=%d\n  %.*s\n", tit,
        (int)insula_versio(repo), (int)actus_numerus,
        (int)stml_numerus_liberorum(insula_radix(repo)),
        (int)t.mensura, (constans character*)t.datum);
}

integer
principale (vacuum)
{
    Piscina* p;
    InternamentumChorda* in;
    InsulaRepositorium* repo;
    InsulaRamus d;
    b32 r;
    integer k;

    p = piscina_generare_dynamicum("sonda", 65536);
    in = internamentum_creare(p);
    repo = insula_repositorium_creare(p, in, "<hospes/>");
    repo->actarius = actarius_numerare;
    repo->actarius_ctx = NIHIL;
    d = insula_ramus(repo, "libellus", "d");
    monstrare(repo, p, "initium");

    /* elementum non congruens: <alius id="x"/> pro libellus:d */
    per (k = 0; k < 3; k++)
    {
        r = insula_ramum_initiare(&d, "<alius id=\"x\"/>");
        imprimere("initiare(mismatch) #%d -> %d, ramus_nodus=%p\n", k,
            (int)r, (vacuum*)insula_ramus_nodus(&d));
        monstrare(repo, p, "post");
    }
    /* id discrepans */
    r = insula_ramum_initiare(&d, "<libellus id=\"zz\"/>");
    imprimere("initiare(id mismatch) -> %d nodus=%p\n", (int)r,
        (vacuum*)insula_ramus_nodus(&d));
    monstrare(repo, p, "post id");

    /* STML malum */
    r = insula_ramum_initiare(&d, "<libellus id=\"d\"");
    imprimere("initiare(stml malum) -> %d\n", (int)r);
    monstrare(repo, p, "post malum");
    redde 0;
}
