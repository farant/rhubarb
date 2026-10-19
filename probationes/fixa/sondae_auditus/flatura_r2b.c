/* sonda flatura r2b: git_obiectum_legere obiecti sarcinae cuius rivus
 * zlib > 64KB (fenestra prima) - fenestra numquam crescit quia
 * flatura INCOMPLETUS numquam reddit */
#include "latina.h"
#include "piscina.h"
#include "git.h"
#include <stdio.h>

integer
principale (integer argc, character** argv)
{
    Piscina*         piscina;
    GitRepositorium* repo;
    GitObiectum      ob;

    si (argc < 4)
    {
        redde 2;
    }
    piscina = piscina_generare_dynamicum("sonda", 65536);
    repo = git_aperire(piscina, argv[1]);
    imprimere("aperire: %s\n", repo ? "ok" : "NIHIL");
    si (repo == NIHIL)
    {
        redde 1;
    }
    ob = git_obiectum_legere(repo, argv[2], piscina);
    imprimere("parva (1000 octeti): successus=%d mensura=%d\n",
        (int)ob.successus, (int)ob.datum.mensura);
    ob = git_obiectum_legere(repo, argv[3], piscina);
    imprimere("magna (200000 octeti): successus=%d mensura=%d\n",
        (int)ob.successus, (int)ob.datum.mensura);
    piscina_destruere(piscina);
    redde 0;
}
