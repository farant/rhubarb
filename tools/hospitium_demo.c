/* hospitium_demo.c - Demo manualis servi domestici (curl amicus)
 *
 * USUS:  ./bin/hospitium_demo [portus]
 *
 * Pollex manualis (uncia curl):
 *   curl -v http://127.0.0.1:8080/salve
 *   curl -v http://127.0.0.1:8080/res/42
 *   curl -v -d 'corpus probationis' http://127.0.0.1:8080/echo
 *   curl -v -X DELETE http://127.0.0.1:8080/salve      (405 + Allow)
 *   curl -v -H 'Transfer-Encoding: chunked' -d x http://127.0.0.1:8080/echo  (411)
 *   curl -v http://127.0.0.1:8080/static/              (index directorii)
 *   curl -v http://127.0.0.1:8080/fructus              (numeratores)
 *   curl -v http://127.0.0.1:8080/salve http://127.0.0.1:8080/salve
 *       (keep-alive: II petitiones, I connexio - vide "Re-using")
 */

#include "hospitium.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior vacuum
_salve(HospitiumColloquium* colloquium)
{
    chorda corpus = chorda_ex_literis("salve ex hospitio!\n",
                                      colloquium_piscina(colloquium));
    colloquium_respondere(colloquium, CC, "text/plain", corpus);
}

interior vacuum
_resonans(HospitiumColloquium* colloquium)
{
    chorda id = colloquium_param(colloquium, "id");
    colloquium_respondere(colloquium, CC, "text/plain", id);
}

interior vacuum
_echo(HospitiumColloquium* colloquium)
{
    constans HttpPetitioServeri* petitio = colloquium_petitio(colloquium);
    colloquium_respondere(colloquium, CC, "application/octet-stream",
                          petitio->corpus);
}

interior vacuum
_fructus_tractator(HospitiumColloquium* colloquium)
{
    Hospitium* h = (Hospitium*)colloquium_datum(colloquium);
    HospitiumFructus fr = hospitium_fructus(h);
    character textus[DXII];
    chorda corpus;

    sprintf(textus,
        "{\n"
        "  \"connexiones_acceptae\": %ld,\n"
        "  \"connexiones_plenae_recusatae\": %ld,\n"
        "  \"petitiones_tractatae\": %ld,\n"
        "  \"petitiones_reiectae\": %ld,\n"
        "  \"responsa_missa\": %ld,\n"
        "  \"clausae_otio\": %ld,\n"
        "  \"clausae_reliquiis\": %ld\n"
        "}\n",
        (longus)fr.connexiones_acceptae,
        (longus)fr.connexiones_plenae_recusatae,
        (longus)fr.petitiones_tractatae,
        (longus)fr.petitiones_reiectae,
        (longus)fr.responsa_missa,
        (longus)fr.clausae_otio,
        (longus)fr.clausae_reliquiis);

    corpus = chorda_ex_literis(textus, colloquium_piscina(colloquium));
    colloquium_respondere(colloquium, CC, "application/json", corpus);
}

integer
principale(integer argc, character** argv)
{
    Piscina* piscina;
    HospitiumConfiguratio cfg;
    Hospitium* h;

    piscina = piscina_generare_dynamicum("hospitium_demo", IV * M * M);

    memset(&cfg, 0, magnitudo(cfg));
    cfg.portus = (argc > I) ? (i32)atoi(argv[I]) : (i32)(VIII * M + LXXX);
    cfg.acta_accessus = VERUM;
    cfg.acao = VERUM;   /* postura dev - paginae capsula:// petere possunt */

    h = hospitium_creare(piscina, &cfg);
    si (h == NIHIL)
    {
        fprintf(stderr, "hospitium_demo: creatio fallita (portus %d occupatus?)\n",
                (argc > I) ? atoi(argv[I]) : (VIII * M + LXXX));
        redde I;
    }

    hospitium_praebere(h, HTTP_GET, "/salve", _salve, NIHIL);
    hospitium_praebere(h, HTTP_GET, "/res/:id", _resonans, NIHIL);
    hospitium_praebere(h, HTTP_POST, "/echo", _echo, NIHIL);
    hospitium_praebere(h, HTTP_GET, "/fructus", _fructus_tractator, h);
    hospitium_directorium_servire(h, "/static", ".");

    printf("hospitium in portu %u - curl http://127.0.0.1:%u/salve\n",
           hospitium_portus(h), hospitium_portus(h));
    printf("(Ctrl-C ad terminandum)\n");

    hospitium_currere(h);

    redde 0;
}
