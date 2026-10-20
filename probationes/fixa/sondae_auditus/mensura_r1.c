/* sonda: argv longior quam MENSURA_LINEA_MAXIMA (4096) in linea initii -
 * mensura_condere lineam per fgets scindit, cursus perditur */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "mensura.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

interior CursusInitium
_initium (constans character* argv, Piscina* p)
{
    CursusInitium i;
    memset(&i, 0, sizeof(i));
    i.id = chorda_ex_literis("c1", p);
    i.parens = chorda_ex_literis("", p);
    i.sessio = chorda_ex_literis("s", p);
    i.titulus = chorda_ex_literis("t", p);
    i.argv = chorda_ex_literis(argv, p);
    i.vocator = chorda_ex_literis("homo", p);
    i.commissum = chorda_ex_literis("abc", p);
    i.ramus = chorda_ex_literis("r", p);
    i.arbor = chorda_ex_literis("/a", p);
    i.machina = chorda_ex_literis("m", p);
    i.initium = (s64)1000;
    redde i;
}

integer
principale (integer argc, character** argv_p)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    constans character* diarium = "/tmp/sonda_mensura_r1.diarium";
    Volumen* vol;
    CursusInitium ini;
    CursusFinis fin;
    MensuraFructus fr;
    Xar* cursus;
    character* longum;
    structura stat st;
    integer n = (argc > 1) ? atoi(argv_p[1]) : 5000;

    (vacuum)remove(diarium);
    longum = (character*)malloc((size_t)n + 1);
    memset(longum, 'a', (size_t)n);
    longum[n] = '\0';
    ini = _initium(longum, p);
    memset(&fin, 0, sizeof(fin));
    fin.id = chorda_ex_literis("c1", p);
    fin.momentum = (s64)2000;
    fin.acta = chorda_ex_literis("", p);

    imprimere("incipere: %d\n", (integer)mensura_cursum_incipere(diarium, &ini, p));
    imprimere("finire: %d\n", (integer)mensura_cursum_finire(diarium, &fin, p));
    stat(diarium, &st);
    imprimere("diarium ante: %ld octeti\n", (long)st.st_size);

    vol = volumen_temporarium(p, "sonda_mensura_r1");
    fr = mensura_condere(diarium, vol, p);
    imprimere("successus=%d conditae=%u causa=%.*s\n", (integer)fr.successus,
        (unsigned)fr.conditae, (integer)fr.causa.mensura,
        fr.causa.datum ? (constans character*)fr.causa.datum : "");
    stat(diarium, &st);
    imprimere("diarium post: %ld octeti\n", (long)st.st_size);
    cursus = mensura_cursus_legere(vol, p);
    imprimere("cursus lecti: %d\n", cursus ? (integer)xar_numerus(cursus) : -1);
    si (cursus && xar_numerus(cursus) > 0)
    {
        Cursus* c = (Cursus*)xar_obtinere(cursus, 0);
        imprimere("argv mensura: %u status %d\n", (unsigned)c->initium.argv.mensura, (integer)c->status);
    }
    volumen_claudere(vol);
    (vacuum)remove(diarium);
    free(longum);
    piscina_destruere(p);
    redde 0;
}
