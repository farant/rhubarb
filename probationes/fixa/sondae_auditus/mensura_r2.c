/* sonda: diarium legi potest sed truncari non (modus 0444) -
 * mensura_condere successum VERUM refert, conditio altera duplicat */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "mensura.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

integer
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    constans character* diarium = "/tmp/sonda_mensura_r2.diarium";
    Volumen* vol;
    Mensura m;
    MensuraFructus fr;
    Xar* lectae;
    structura stat st;

    (vacuum)chmod(diarium, 0644);
    (vacuum)remove(diarium);
    memset(&m, 0, sizeof(m));
    m.sessio = chorda_ex_literis("s", p);
    m.id = chorda_ex_literis("i1", p);
    m.parens = chorda_ex_literis("", p);
    m.titulus = chorda_ex_literis("t", p);
    m.unitas = chorda_ex_literis("secunda", p);
    m.valor = 1.5;
    m.momentum = (s64)1000;
    imprimere("annotare: %d\n", (integer)mensura_annotare(diarium, &m, p));
    chmod(diarium, 0444);

    vol = volumen_temporarium(p, "sonda_mensura_r2");
    fr = mensura_condere(diarium, vol, p);
    stat(diarium, &st);
    imprimere("prima: successus=%d conditae=%u causa_mensura=%u diarium=%ld octeti\n",
        (integer)fr.successus, (unsigned)fr.conditae, (unsigned)fr.causa.mensura, (long)st.st_size);
    fr = mensura_condere(diarium, vol, p);
    imprimere("altera: successus=%d conditae=%u\n", (integer)fr.successus, (unsigned)fr.conditae);
    lectae = mensura_mensuras_legere(vol, chorda_ex_literis("", p), p);
    imprimere("mensurae in volumine: %d (annotata 1)\n", lectae ? (integer)xar_numerus(lectae) : -1);
    volumen_claudere(vol);
    chmod(diarium, 0644);
    (vacuum)remove(diarium);
    piscina_destruere(p);
    redde 0;
}
