#include "postulata_posix.h"
#include "latina.h"
#include "filum.h"
#include "piscina.h"
#include "html_arbor.h"
#include "html_registrum.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include <stdio.h>
#include <string.h>

/* html_rotunda.c - sonda (2026-10-19): emit(parse(x)) != x pro initu
 * mutato (probatio_html_totalitas, semen III super aurum briar
 * fragmenta.html). Longitudo eadem, octeti ex CCXCVII ordine diverso.
 * Currere: ./tools/sonda.sh -nudus probationes/fixa/sondae_auditus/
 * html_rotunda.c [via] (ordinarie html_rotunda_minima.html, DCCLXXX
 * octeti; html_rotunda_plena.html = initus totus XV milium). */
integer principale (integer argc, character** argv)
{
    Piscina* p = piscina_generare_dynamicum("rotunda", 1048576);
    chorda fons = filum_legere_totum(argc > 1 ? argv[1] : "probationes/fixa/sondae_auditus/html_rotunda_minima.html", p);
    MateriaNodus* radix = html_arbor_parsare(p, (constans character*)fons.datum, (i32)fons.mensura);
    MateriaScripturaConsilium consilium;
    MateriaScriptura emissa;
    i32 k = 0, a, b;
    si (!radix) { imprimere("NIHIL\n"); redde I; }
    materia_scriptura_consilium_nudum(&consilium, &HTML_REGISTRUM);
    emissa = materia_scribere_nodum(p, radix, &consilium);
    imprimere("fons %u emissa %u successus %d\n", fons.mensura, emissa.textus.mensura, (integer)emissa.successus);
    dum (k < fons.mensura && k < emissa.textus.mensura && fons.datum[k] == emissa.textus.datum[k]) k++;
    imprimere("prima differentia %u\n", k);
    a = k > 60 ? k - 60 : 0;
    b = k + 60;
    imprimere("FONS   [%.*s]\n", (integer)((b < (i32)fons.mensura ? b : (i32)fons.mensura) - a), (constans character*)fons.datum + a);
    imprimere("EMISSA [%.*s]\n", (integer)((b < (i32)emissa.textus.mensura ? b : (i32)emissa.textus.mensura) - a), (constans character*)emissa.textus.datum + a);
    redde 0;
}
