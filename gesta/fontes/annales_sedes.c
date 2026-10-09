/* annales_sedes.c - sedes annalium (vide annales_sedes.h): una
 * quaestio 'ubi acta habitant' pro tabulario MCP, via frigida et
 * daemone fori. */

#include "annales_sedes.h"
#include "filum.h"
#include "via.h"
#include <stdlib.h>
#include <string.h>

/* tituli plagularum, ordine AnnaliumPlagula: sedes nova (directorium
 * unum) et legatum (forma arboris vetus) */
interior constans character* constans TITULI_NOVI[] = {
    "tabularium.jsonl", "tabularium.db", "tabula.md", "entities",
    "forum.jsonl", "forum.db"
};
interior constans character* constans TITULI_LEGATI[] = {
    "gesta/annales/tabularium.jsonl", "tabularium.db",
    "gesta/annales/tabula.md", "gesta/annales/entities",
    "gesta/annales/forum.jsonl", "forum.db"
};

/* causa = a + b (si causa datur); FALSUM */
interior b32
_recusare (
                 chorda* causa,
     constans character* a,
                 chorda  b,
                Piscina* piscina)
{
    si (causa != NIHIL)
    {
        *causa = chorda_concatenare(chorda_ex_literis(a, piscina), b,
            piscina);
    }
    redde FALSUM;
}

/* a/b ut chorda */
interior chorda
_iungere (
                 chorda  a,
     constans character* b,
                Piscina* piscina)
{
    chorda partes[II];

    partes[0] = a;
    partes[1] = chorda_ex_literis(b, piscina);
    redde via_iungere(partes, II, piscina);
}

b32
annales_sedem_invenire (
     constans character* radix_arboris,
                Piscina* piscina,
          AnnaliumSedes* sedes,
                 chorda* causa)
{
    constans character* ambitus;
    constans character* domus;

    sedes->radix_arboris = chorda_ex_literis(
        (radix_arboris != NIHIL) ? radix_arboris : ".", piscina);
    ambitus = getenv("RHUBARB_ANNALES");
    si (ambitus != NIHIL && ambitus[0] != '\0')
    {
        si (!filum_directorium_existit(ambitus))
        {
            redde _recusare(causa,
                "RHUBARB_ANNALES directorium non exstans: ",
                chorda_ex_literis(ambitus, piscina), piscina);
        }
        sedes->origo        = ANNALES_EX_AMBITU;
        sedes->directorium  = chorda_ex_literis(ambitus, piscina);
        redde VERUM;
    }
    domus = getenv("HOME");
    si (domus != NIHIL && domus[0] != '\0')
    {
        chorda directorium;

        directorium = _iungere(chorda_ex_literis(domus, piscina),
            ".rhubarb/annales", piscina);
        si (filum_directorium_existit(chorda_ut_cstr(directorium,
                piscina)))
        {
            sedes->origo        = ANNALES_EX_DOMO;
            sedes->directorium  = directorium;
            redde VERUM;
        }
    }
    sedes->origo        = ANNALES_EX_ARBORE;
    sedes->directorium  = sedes->radix_arboris;
    redde VERUM;
}

chorda
annales_via (
    constans AnnaliumSedes* sedes,
           AnnaliumPlagula  plagula,
                   Piscina* piscina)
{
    si (sedes->origo == ANNALES_EX_ARBORE)
    {
        redde _iungere(sedes->radix_arboris, TITULI_LEGATI[plagula],
            piscina);
    }
    redde _iungere(sedes->directorium, TITULI_NOVI[plagula], piscina);
}

b32
annales_custodire (
    constans AnnaliumSedes* sedes,
           AnnaliumPlagula  annales,
           AnnaliumPlagula  scrinium,
                       b32  genesis_licita,
                   Piscina* piscina,
                    chorda* causa)
{
    chorda via_annalium;
    chorda via_scrinii;
       b32 annales_exstant;
       b32 scrinium_exstat;

    via_annalium  = annales_via(sedes, annales, piscina);
    via_scrinii   = annales_via(sedes, scrinium, piscina);
    annales_exstant  = filum_existit(chorda_ut_cstr(via_annalium,
        piscina));
    scrinium_exstat  = filum_existit(chorda_ut_cstr(via_scrinii,
        piscina));
    si (!annales_exstant)
    {
        si (scrinium_exstat)
        {
            redde _recusare(causa,
                "scrinium sine annalibus (veritas absens): ",
                via_annalium, piscina);
        }
        si (!genesis_licita)
        {
            redde _recusare(causa,
                "annales absentes (genesis expressa: -genesis): ",
                via_annalium, piscina);
        }
        redde VERUM;
    }
    si (!scrinium_exstat)
    {
        redde _recusare(causa,
            "annales sine scrinio - restitue per './gesta/frigida.sh"
            " -restituere' (aut scrinium cum annalibus transfer): ",
            via_scrinii, piscina);
    }
    redde VERUM;
}
