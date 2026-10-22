/* horreum.c - bin/horreum: cliens S3 ex linea mandatorum super
 * horreum_cliens (horreum-plan T6). Profilum = titulus
 * (~/.rhubarb/horreum/TITULUS.clavis) aut via (continet '/').
 *
 * Usus:
 *   horreum -profilum P ponere CLAVIS FILUM [-genus CT] [-solum-si-abest]
 *   horreum -profilum P capere CLAVIS FILUM
 *   horreum -profilum P inspicere CLAVIS
 *   horreum -profilum P enumerare [PRAEFIXUM] [-delimitator D]
 *   horreum -profilum P delere CLAVIS...
 *   horreum -profilum P praesignare GET|PUT CLAVIS [-secundae N]
 *   horreum -profilum P pendentes [PRAEFIXUM] [-abicere]
 *   horreum -profilum P probare [-praefixum P] [-responsa DIR]
 *
 * -vestigium VIA (mandato cuivis): quisque conatus HTTP (iterationes et
 * HEAD comprehensa) in VIA appenditur ut transcriptio
 * (horreum_observationem_describere: Authorization tectum, corpus
 * longum abbreviatum). Filum 0600 fit: corpora petitionum continet.
 *
 * Numquam secretum scribit; URL praesignata solum a 'praesignare'.
 * Exitus: 0 bene; I operatio defecit (probare: gradus non-capacitatis
 * defecit aut aliquid relictum); II usus aut profilum. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fasti.h"
#include "horreum.h"
#include "horreum_cliens.h"
#include "horreum_probatio.h"
#include "filum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HORREUM_SECUNDAE_ORDINARIAE MMMDC

interior vacuum
_usus (vacuum)
{
    fprintf(stderr,
        "usus: horreum -profilum P MANDATUM ...\n"
        "  ponere CLAVIS FILUM [-genus CT] [-solum-si-abest]\n"
        "  capere CLAVIS FILUM\n"
        "  inspicere CLAVIS\n"
        "  enumerare [PRAEFIXUM] [-delimitator D]\n"
        "  delere CLAVIS...\n"
        "  praesignare GET|PUT CLAVIS [-secundae N]\n"
        "  pendentes [PRAEFIXUM] [-abicere]\n"
        "  probare [-praefixum P] [-responsa DIR]\n"
        "-vestigium VIA: conatus omnes HTTP in VIA appenduntur\n"
        "profilum: titulus (~/.rhubarb/horreum/TITULUS.clavis) aut via\n");
}

nomen structura {
     constans character* via;
                    b32  defectus;
} Vestigium;

/* observator: transcriptio conatus in filum (piscina propria, ne
 * piscina mandati per conatus crescat) */
interior vacuum
_vestigium_scribere (
                        vacuum* datum,
    constans HorreumObservatio* o)
{
    Vestigium* v = (Vestigium*)datum;
      Piscina* q = piscina_generare_dynamicum("horreum_vestigium",
                       (memoriae_index)(XVI * MXXIV));

    si (!filum_appendere(v->via, horreum_observationem_describere(o,
        q)))
    {
        v->defectus = VERUM;
    }
    piscina_destruere(q);
}

interior Vestigium vestigium;

interior chorda
_l (
    constans character* l,
               Piscina* p)
{
    redde chorda_ex_literis(l, p);
}

interior vacuum
_scribere (
    chorda s)
{
    fwrite(s.datum, I, (size_t)s.mensura, stdout);
}

interior s32
_error (
       constans character* mandatum,
    constans HorreumError* e)
{
    fprintf(stderr, "horreum %s: %.*s", mandatum,
        (integer)e->nuntius.mensura,
        (constans character*)e->nuntius.datum);
    si (e->codex.mensura > ZEPHYRUM)
    {
        fprintf(stderr, " [%.*s %d]", (integer)e->codex.mensura,
            (constans character*)e->codex.datum, (integer)e->status);
    }
    fprintf(stderr, "\n");
    redde I;
}

/* titulus -> ~/.rhubarb/horreum/TITULUS.clavis */
interior constans character*
_via_tabellae (
    constans character* tabella,
               Piscina* p)
{
    constans character* domus = getenv("HOME");
     ChordaAedificator* a;

    si (strchr(tabella, '/') || !domus)
    {
        redde tabella;
    }
    a = chorda_aedificator_creare(p, CCLVI);
    chorda_aedificator_appendere_literis(a, domus);
    chorda_aedificator_appendere_literis(a, "/.rhubarb/horreum/");
    chorda_aedificator_appendere_literis(a, tabella);
    chorda_aedificator_appendere_literis(a, ".clavis");
    redde chorda_ut_cstr(chorda_aedificator_finire(a), p);
}

/* valor optionis '-nomen VALOR' in argv[ab..]; NIHIL si abest */
interior constans character*
_optio (
               integer   argc,
             character** argv,
               integer   ab,
    constans character*  nomen_optionis)
{
    integer i;

    per (i = ab; i + I < argc; i++)
    {
        si (strcmp(argv[i], nomen_optionis) == ZEPHYRUM)
        {
            redde argv[i + I];
        }
    }
    redde NIHIL;
}

interior b32
_vexillum (
               integer   argc,
             character** argv,
               integer   ab,
    constans character*  nomen_vexilli)
{
    integer i;

    per (i = ab; i < argc; i++)
    {
        si (strcmp(argv[i], nomen_vexilli) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* argumentum positionale k-tum (optionibus et valoribus saltatis) */
interior constans character*
_positio (
       integer   argc,
     character** argv,
       integer   ab,
       integer   k)
{
    integer i;
    integer n = ZEPHYRUM;

    per (i = ab; i < argc; i++)
    {
        si (argv[i][ZEPHYRUM] == '-' && argv[i][I] != '\0')
        {
            si (   strcmp(argv[i], "-solum-si-abest") != ZEPHYRUM
                && strcmp(argv[i], "-abicere")        != ZEPHYRUM)
            {
                i++;
            }
            perge;
        }
        si (n == k)
        {
            redde argv[i];
        }
        n++;
    }
    redde NIHIL;
}

interior vacuum
_obiectum_scribere (
    constans HorreumObjectum* ob,
                     Piscina* p)
{
    i32 i;

    imprimere("clavis\t");
    _scribere(ob->clavis);
    imprimere("\nmensura\t%ld\netag\t", (longus)ob->mensura);
    _scribere(ob->etag);
    si (ob->mutatum > ZEPHYRUM)
    {
        imprimere("\nmutatum\t");
        _scribere(fasti_ad_iso(fasti_ex_unix(ob->mutatum), p));
    }
    si (ob->genus_contenti.mensura > ZEPHYRUM)
    {
        imprimere("\ngenus\t");
        _scribere(ob->genus_contenti);
    }
    per (i = ZEPHYRUM; i < ob->meta_numerus; i++)
    {
        imprimere("\nmeta\t");
        _scribere(ob->meta[i].titulus);
        imprimere("=");
        _scribere(ob->meta[i].valor);
    }
    imprimere("\n");
}

interior s32
_enumerare (
       Horreum*  h,
        chorda   situla,
       integer   argc,
     character** argv,
       Piscina*  p)
{
    constans character* praefixum = _positio(argc, argv, IV, ZEPHYRUM);
    constans character* delimitator = _optio(argc, argv, IV,
        "-delimitator");
       HorreumEnumerandi en;
           HorreumPagina pg;
            HorreumError e;
                     i32 i;

    memset(&en, ZEPHYRUM, magnitudo(en));
    si (praefixum)
    {
        en.praefixum = _l(praefixum, p);
    }
    si (delimitator)
    {
        en.delimitator = _l(delimitator, p);
    }
    fac
    {
        si (!horreum_enumerare(h, situla, &en, &pg, &e, p))
        {
            redde _error("enumerare", &e);
        }
        per (i = ZEPHYRUM; i < pg.praefixa_numerus; i++)
        {
            imprimere("PRAEFIXUM\t");
            _scribere(pg.praefixa_communia[i]);
            imprimere("\n");
        }
        per (i = ZEPHYRUM; i < pg.obiecta_numerus; i++)
        {
            imprimere("%ld\t", (longus)pg.obiecta[i].mensura);
            _scribere(pg.obiecta[i].clavis);
            imprimere("\n");
        }
        en.signum_continuationis = pg.signum_proximum;
    }
    dum (pg.truncata && pg.signum_proximum.mensura > ZEPHYRUM);
    redde ZEPHYRUM;
}

interior s32
_pendentes (
       Horreum*  h,
        chorda   situla,
       integer   argc,
     character** argv,
       Piscina*  p)
{
    constans character* praefixum = _positio(argc, argv, IV, ZEPHYRUM);
    HorreumOnusPendens* onera;
                   i32  numerus;
                   i32  i;
          HorreumError  e;
                   b32  abicere = _vexillum(argc, argv, IV, "-abicere");
                   s32  exitus = ZEPHYRUM;

    si (!horreum_mp_pendentes(h, situla, _l(praefixum ? praefixum : "",
        p),
            &onera, &numerus, &e, p))
    {
        redde _error("pendentes", &e);
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        _scribere(onera[i].id_oneris);
        imprimere("\t");
        _scribere(onera[i].clavis);
        si (abicere)
        {
            si (horreum_mp_abicere(h, situla, onera[i].clavis,
                    onera[i].id_oneris, &e, p))
            {
                imprimere("\tabiectum");
            }
            alioquin
            {
                exitus = _error("pendentes -abicere", &e);
            }
        }
        imprimere("\n");
    }
    redde exitus;
}

interior s32
_probare (
       Horreum*  h,
       integer   argc,
     character** argv,
       Piscina*  p)
{
       HorreumProbandi  o;
       HorreumProbatio  r;
          HorreumError  e;
                   i32  i;
                   s32  exitus = ZEPHYRUM;
    constans character* praefixum = _optio(argc, argv, IV,
        "-praefixum");

    memset(&o, ZEPHYRUM, magnitudo(o));
    si (praefixum)
    {
        o.praefixum = _l(praefixum, p);
    }
    o.responsa = _optio(argc, argv, IV, "-responsa");
    si (vestigium.via)
    {
        o.observator          = _vestigium_scribere;
        o.observatoris_datum  = &vestigium;
    }
    si (!horreum_probare(h, &o, &r, &e, p))
    {
        redde _error("probare", &e);
    }
    per (i = ZEPHYRUM; i < r.numerus; i++)
    {
        constans HorreumProbatioLinea* l = &r.lineae[i];

        imprimere("%-16s %-13s ", l->titulus,
            horreum_verdictum_nomen(l->verdictum));
        _scribere(l->causa);
        imprimere("\n");
        si (   l->capacitas == ZEPHYRUM
            && l->verdictum != HORREUM_PROBATUM)
        {
            exitus = I;
        }
    }
    imprimere("praefixum         ");
    _scribere(r.praefixum);
    imprimere("\nclavis difficilis ");
    _scribere(r.clavis_difficilis);
    imprimere("\ncaput petitionis  ");
    _scribere(r.caput_petitionis.mensura > ZEPHYRUM ? r.caput_petitionis
        : _l("(nullum)", p));
    imprimere("\nrelicta           %ld\n", (longus)r.relicta);
    redde exitus;
}

interior s32
_mandatum (
       Horreum*  h,
       integer   argc,
     character** argv,
       Piscina*  p)
{
     constans character* m         = argv[III];
     constans character* primum    = _positio(argc, argv, IV, ZEPHYRUM);
     constans character* secundum  = _positio(argc, argv, IV, I);
                 chorda  situla    = horreum_situla_ordinaria(h);
        HorreumObjectum  ob;
           HorreumError  e;
         HorreumPonendi  ponendi;
                integer  i;

    si (strcmp(m, "ponere") == ZEPHYRUM && primum && secundum)
    {
        memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
        si (_optio(argc, argv, IV, "-genus"))
        {
            ponendi.genus_contenti = _l(_optio(argc, argv, IV,
                "-genus"),
                p);
        }
        ponendi.solum_si_abest = _vexillum(argc, argv, IV,
            "-solum-si-abest");
        si (!horreum_ponere_ex_filo(h, situla, _l(primum, p), secundum,
                &ponendi, NIHIL, &ob, &e, p))
        {
            redde _error("ponere", &e);
        }
        imprimere("etag\t");
        _scribere(ob.etag);
        imprimere("\n");
        redde ZEPHYRUM;
    }
    si (strcmp(m, "capere") == ZEPHYRUM && primum && secundum)
    {
        si (!horreum_capere_in_filum(h, situla, _l(primum, p), secundum,
                &ob, &e, p))
        {
            redde _error("capere", &e);
        }
        imprimere("%ld\t", (longus)ob.mensura);
        _scribere(ob.etag);
        imprimere("\n");
        redde ZEPHYRUM;
    }
    si (strcmp(m, "inspicere") == ZEPHYRUM && primum)
    {
        si (!horreum_inspicere(h, situla, _l(primum, p), &ob, &e, p))
        {
            redde _error("inspicere", &e);
        }
        ob.clavis = _l(primum, p);
        _obiectum_scribere(&ob, p);
        redde ZEPHYRUM;
    }
    si (strcmp(m, "enumerare") == ZEPHYRUM)
    {
        redde _enumerare(h, situla, argc, argv, p);
    }
    si (strcmp(m, "delere") == ZEPHYRUM && primum)
    {
        /* claves per _positio: optiones (-vestigium VIA) numquam
         * claves fiunt (recensio I1) */
        constans character* clavis;

        per (i = ZEPHYRUM;
             (clavis = _positio(argc, argv, IV, i)) != NIHIL; i++)
        {
            si (!horreum_delere(h, situla, _l(clavis, p), &e, p))
            {
                redde _error("delere", &e);
            }
            imprimere("deletum\t%s\n", clavis);
        }
        redde ZEPHYRUM;
    }
    si (strcmp(m, "praesignare") == ZEPHYRUM && primum && secundum)
    {
        constans character* s = _optio(argc, argv, IV, "-secundae");
                    chorda  url;
              HttpMethodus  methodus;

        si (strcmp(primum, "GET") == ZEPHYRUM)
        {
            methodus = HTTP_GET;
        }
        alioquin si (strcmp(primum, "PUT") == ZEPHYRUM)
        {
            methodus = HTTP_PUT;
        }
        alioquin
        {
            _usus();
            redde II;
        }
        url = horreum_praesignare(h, methodus, situla, _l(secundum, p),
            s ? (i32)atol(s) : HORREUM_SECUNDAE_ORDINARIAE,
            chorda_ex_buffer(NIHIL, ZEPHYRUM), (MomentumSec)time(NIHIL),
            &e,
            p);
        si (url.mensura == ZEPHYRUM)
        {
            redde _error("praesignare", &e);
        }
        _scribere(url);
        imprimere("\n");
        redde ZEPHYRUM;
    }
    si (strcmp(m, "pendentes") == ZEPHYRUM)
    {
        redde _pendentes(h, situla, argc, argv, p);
    }
    si (strcmp(m, "probare") == ZEPHYRUM)
    {
        redde _probare(h, argc, argv, p);
    }
    _usus();
    redde II;
}

s32
principale (
      integer   argc,
    character** argv)
{
                    Piscina* p;
                    Horreum* h;
               HorreumError  e;
                        s32  exitus;
    HorreumClientisOptiones  co;

    si (argc < IV || strcmp(argv[I], "-profilum") != ZEPHYRUM)
    {
        _usus();
        redde II;
    }
    p = piscina_generare_dynamicum("horreum", (memoriae_index)(LXIV
        * MXXIV));
    h = horreum_ex_tabella(_via_tabellae(argv[II], p), &e, p);
    si (!h)
    {
        _error("profilum", &e);
        piscina_destruere(p);
        redde II;
    }
    memset(&co, ZEPHYRUM, magnitudo(co));
    vestigium.via = _optio(argc, argv, IV, "-vestigium");
    si (vestigium.via)
    {
        /* corpora petitionum (usque IV KiB) in vestigio stant - filum
         * 0600 ANTE primam scripturam (recensio M8) */
        si (   !filum_appendere_firmiter(vestigium.via,
                    chorda_ex_buffer(NIHIL, ZEPHYRUM))
            || !filum_modum_ponere(vestigium.via, 0600))
        {
            fprintf(stderr, "horreum: vestigium %s parari nequit\n",
                vestigium.via);
            horreum_claudere(h);
            piscina_destruere(p);
            redde II;
        }
        co.observator          = _vestigium_scribere;
        co.observatoris_datum  = &vestigium;
    }
    horreum_cliens_parare(h, &co, p);
    exitus = _mandatum(h, argc, argv, p);
    si (vestigium.defectus)
    {
        fprintf(stderr, "horreum: vestigium %s scribi nequit\n",
            vestigium.via);
    }
    horreum_claudere(h);
    piscina_destruere(p);
    redde exitus;
}
