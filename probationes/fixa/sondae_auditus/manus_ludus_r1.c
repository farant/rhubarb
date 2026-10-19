/* sonda: causa stala post premere fractum */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "ludus_toy.h"
#include <stdio.h>
#include <string.h>

interior vacuum
monstrare (constans character* tit, ManusLudus* m)
{
    chorda c = manus_ludus_causa(m);
    imprimere("%s -> causa '%.*s'\n", tit, (integer)c.mensura,
              c.datum ? (constans character*)c.datum : "");
}

integer
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
             ManusLudus* m;
                Punctum  pt[I];
                    b32  r;

    piscina = piscina_generare_dynamicum("sonda", 65536);
    intern  = internamentum_creare(piscina);
    repo    = insula_repositorium_creare(piscina, intern, "<insula/>");
    reg     = actio_registrum_creare(piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    m = manus_ludus_creare(piscina, d);

    /* A: selector malus, deinde premere selectoris boni sed absentis */
    r = manus_ludus_premere(m, "b1");
    imprimere("premere(\"b1\") = %d\n", (integer)r);
    monstrare("  post selector malus", m);
    r = manus_ludus_premere(m, "#nemo");
    imprimere("premere(\"#nemo\") = %d\n", (integer)r);
    monstrare("  post #nemo (expectatum: 'premere: selector nihil invenit')", m);

    /* B: mundus novus - trahere fractum, deinde premere absentis */
    m = manus_ludus_creare(piscina, d);
    pt[ZEPHYRUM].x = ZEPHYRUM; pt[ZEPHYRUM].y = ZEPHYRUM;
    r = manus_ludus_trahere(m, "#nemo", pt, I);
    imprimere("trahere(\"#nemo\") = %d\n", (integer)r);
    monstrare("  post trahere", m);
    r = manus_ludus_premere(m, "#alius_nemo");
    imprimere("premere(\"#alius_nemo\") = %d\n", (integer)r);
    monstrare("  post premere (causa nominat trahere?)", m);

    /* C: premere successus post fractum - causa manet */
    r = manus_ludus_premere(m, "#b1");
    imprimere("premere(\"#b1\") = %d\n", (integer)r);
    monstrare("  post successum", m);
    redde ZEPHYRUM;
}
