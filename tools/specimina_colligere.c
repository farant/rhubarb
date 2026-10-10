/* specimina_colligere.c - figuras API Anthropic veras colligere: petitio
 * quaeque (corpus JSON ex directorio) mittitur et responsum OMNE in
 * herbarium premitur (iudex 'exemplum'), non solum inexspectatum.
 *
 * NON PORTA: API vera, centesimae pecuniae - manu curritur.
 *
 * Curre per: ./tools/specimina_colligere.sh [-ad <acervus>]
 *            [<directorium petitionum>]
 * Ordinaria: petitiones = plagulae .json in tools/specimina_anthropic (ordine
 * nominum); acervus ~/.rhubarb/herbarium/api.anthropic.com-exempla
 * (seorsum ab acervo inexspectatorum, quem custos vatis implet).
 * Petitio cuius nomen 'clavis_falsa' continet clave FICTA mittitur
 * (figura erroris authenticationis).
 *
 * Clavis: ~/.rhubarb/anthropic.clavis - in processu legitur, in capite
 * x-api-key solo ponitur, numquam imprimitur; herbarium capita
 * petitionis numquam scribit. Exitus 0 = omnes petitiones missae
 * (status quilibet), 1 = defectus retis, 2 = usus aut clavis. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "http.h"
#include "herbarium.h"
#include "iter_directoria.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PETITIONES  "tools/specimina_anthropic"
#define URL         "https://api.anthropic.com/v1/messages"

nomen structura {
    constans character* titulus;   /* petitio currens */
} Collector;

/* iudex: OMNE responsum premitur, causa = nomen petitionis */
interior chorda
_iudex_omnia (
      HttpPetitio* petitio,
    HttpResponsum* responsum,
          Piscina* piscina,
           vacuum* datum)
{
    Collector* c = (Collector*)datum;
    character  causa[CCLVI];

    (vacuum)petitio;
    (vacuum)responsum;
    sprintf(causa, "exemplum %.200s", c->titulus);
    redde chorda_ex_literis(causa, piscina);
}

interior s32
_comparare_nomina (
    constans vacuum* a,
    constans vacuum* b)
{
    redde (s32)strcmp(*(constans character* constans*)a,
                      *(constans character* constans*)b);
}

s32
principale (
       integer   argc,
     character** argv)
{
               Piscina*  p;
    constans character*  domus        = getenv("HOME");
    constans character*  directorium  = PETITIONES;
             character   acervus[DXII];
             character   via[DXII];
                chorda   crudum;
             character*  clavis;
     HerbariumOptiones   o;
             Herbarium*  h;
           HttpVectura   vectura;
             Collector   collector;
   DirectoriumIterator*  it;
  DirectoriumIntroitus*  introitus;
             character** nomina;
                   i32   numerus   = 0;
                   i32   defectus  = 0;
               integer   i;
    constans character* constans campi[II] = { "model", NIHIL };

    p = piscina_generare_dynamicum("specimina_colligere", M * M);
    si (!p || !domus)
    {
        redde II;
    }
    sprintf(acervus, "%s/.rhubarb/herbarium/api.anthropic.com-exempla",
        domus);
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-ad") == 0 && i + I < argc)
        {
            sprintf(acervus, "%.500s", argv[++i]);
        }
        alioquin si (argv[i][0] != '-')
        {
            directorium = argv[i];
        }
        alioquin
        {
            fprintf(stderr, "usus: specimina_colligere [-ad <acervus>]"
                " [<directorium petitionum>]\n");
            redde II;
        }
    }
    sprintf(via, "%s/.rhubarb/anthropic.clavis", domus);
    si (!filum_existit(via))
    {
        fprintf(stderr,
            "specimina_colligere: ~/.rhubarb/anthropic.clavis"
            " abest\n");
        redde II;
    }
    crudum = filum_legere_totum(via, p);
    clavis = chorda_ut_cstr(chorda_praecidere(crudum), p);
    memset(crudum.datum, 0, (size_t)crudum.mensura);

    /* petitiones: *.json ordine nominum */
    nomina = (character**)piscina_allocare(p,
        (memoriae_index)(CCLVI * magnitudo(character*)));
    it = directorium_iterator_aperire(directorium, p);
    dum (it && (introitus = directorium_iterator_proximum(it)) != NIHIL
         && numerus < CCLVI)
    {
        si (   introitus->genus == INTROITUS_FILUM
            && introitus->titulus.mensura > V
            && memcmp(introitus->titulus.datum
                + introitus->titulus.mensura
                   - V, ".json", V) == 0)
        {
            nomina[numerus++] = chorda_ut_cstr(introitus->titulus, p);
        }
    }
    si (it)
    {
        directorium_iterator_claudere(it);
    }
    si (numerus == 0)
    {
        fprintf(stderr, "specimina_colligere: nulla petitio in %s\n",
            directorium);
        redde II;
    }
    qsort(nomina, (size_t)numerus, magnitudo(character*),
        _comparare_nomina);

    o                   = herbarium_optiones_ordinariae();
    o.directorium       = acervus;
    o.campi_petitionis  = campi;
    /* captura deliberata: genus clavis ordinariae (sceleton) multa
     * corpora 200 coniungit - III variantes (ordinarium herbarii)
     * citationes et max_tokens abiecerunt (mensuratum 2026-10-09) */
    o.variantes_maximae  = XXXII;
    o.iudex              = _iudex_omnia;
    o.iudex_datum        = &collector;
    h                    = herbarium_aperire(p, &o);
    si (!h)
    {
        redde II;
    }
    vectura = herbarium_vectura(h, http_vectura_ordinaria());
    fprintf(stderr, "specimina_colligere: %u petitiones -> %s\n",
        (insignatus integer)numerus, acervus);

    per (i = 0; i < (integer)numerus; i++)
    {
         HttpPetitio* petitio;
        HttpResultus  res;
              chorda  corpus;

        sprintf(via, "%s/%s", directorium, nomina[i]);
        corpus             = filum_legere_totum(via, p);
        collector.titulus  = nomina[i];
        petitio            = http_petitio_creare(p, HTTP_POST, URL);
        http_petitio_caput_addere(petitio, "content-type",
            "application/json");
        http_petitio_caput_addere(petitio, "anthropic-version",
            "2023-06-01");
        http_petitio_caput_addere(petitio, "x-api-key",
            strstr(nomina[i],
            "clavis_falsa") ? "sk-ant-clavis-ficta-probatio"
                                              : clavis);
        http_petitio_tempus_ponere(petitio, CXX * M);
        http_petitio_corpus_ponere_chorda(petitio, corpus);
        res = http_vectura_exsequi(vectura, petitio, p);
        si (res.successus && res.responsum)
        {
            printf("%-32s status %u, octeti %u\n", nomina[i],
                (insignatus integer)res.responsum->status,
                (insignatus integer)res.responsum->corpus.mensura);
        }
        alioquin
        {
            printf("%-32s DEFECTUS: %s\n", nomina[i],
                http_error_descriptio(res.error));
            defectus++;
        }
    }
    memset(clavis, 0, strlen(clavis));
    piscina_destruere(p);
    redde defectus > 0 ? I : 0;
}
