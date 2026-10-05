/* effectus.c - Instrumentum summarii effectuum (crusta/effectus.sh)
 *
 * Usus:  effectus <scriptum> [-radix DIR] [-tabula PLAGULA]
 *        effectus -catenae [-radix DIR]
 *        effectus -census [-radix DIR] < index   (T6: census.tsv)
 *        effectus -lintrum <plagula>... [-radix DIR]   (T6: regulae
 *                 crusta/lintrum/effectus/, erratum -> exitus 1)
 *        effectus -observata <scriptum> <liber> [-radix DIR]
 *        effectus -comparare <scriptum> <liber> [-radix DIR]
 *                 [-ante_scripta PLAGULA]
 * Effusio: summarium STML dialecti 'effectus' (effectus.canon);
 * -observata: summarium liberi oraculi (interpositio_macos.c);
 * -comparare: situs observati quos summarium staticum NON tegit,
 * linea una quisque ('elementum\tvia\tmandatum'). Radix ordinaria =
 * directorium operis; tabula mandatorum ex
 * radix/crusta/effectus_mandata.stml.
 *
 * Exitus: 0 sanum (comparare: omnia tecta) | 1 non tecta |
 *         2 usus / scriptum absens / tabula illegibilis. */

#include "postulata_posix.h"

#include "latina.h"
#include "crusta_effectus.h"
#include "crusta_facies.h"
#include "materia_diagnostica.h"
#include "materia_pictor.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VIA_MAXIMA (IV * MXXIV)

interior vacuum
_usus (vacuum)
{
    fprintf(stderr, "usus: effectus <scriptum> [-radix DIR]\n"
        "       effectus -observata <scriptum> <liber> [-radix DIR]\n"
        "       effectus -comparare <scriptum> <liber> [-radix DIR] "
        "[-ante_scripta PLAGULA]\n");
}

interior constans character*
_attributum (
               Piscina* piscina,
             StmlNodus* n,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(n, titulus);

    redde v == NIHIL ? "" : chorda_ut_cstr(*v, piscina);
}


/* ==================================================
 * Lintrum effectuum et catenae (planum T6)
 * ================================================== */

/* contextus: regulae effectus, summaria catenarum - semel per cursum,
 * internamento uno (LEX INTERNAMENTI) */
nomen structura {
                 Piscina* piscina;
     InternamentumChorda* intern;
      constans character* radix;
               StmlNodus* tabula;
          CrustaOptiones  optiones;
                     Xar* catenae;     /* character* */
                     Xar* summaria;  /* StmlNodus*: catenae */
} Contextus;

interior b32
_contextum_parare (
           Contextus* c,
    constans character** causa)
{
             character  directorium[VIA_MAXIMA];
    constans character* ambitus = getenv(CRUSTA_LINTRUM_AMBITUS);
                   i32  k;

    memset(&c->optiones, ZEPHYRUM, magnitudo(c->optiones));
    si (ambitus != NIHIL && ambitus[ZEPHYRUM] != '\0')
    {
        sprintf(directorium, "%s/effectus", ambitus);
    }
    alioquin
    {
        sprintf(directorium, "%s/%s/effectus", c->radix,
            CRUSTA_LINTRUM);
    }
    c->optiones.intern   = c->intern;
    c->optiones.regulae  = crusta_regulae_legere(c->piscina,
        directorium,
        c->intern, causa);
    si (c->optiones.regulae == NIHIL)
    {
        redde FALSUM;
    }
    si (xar_numerus(c->optiones.regulae) == ZEPHYRUM)
    {
        *causa = "regula effectus nulla (lintrum vacuum refutatio est)";
        redde FALSUM;
    }
    c->catenae = crusta_effectus_catenae(c->piscina, c->intern,
        c->radix,
        causa);
    c->summaria = xar_creare(c->piscina, (i32)magnitudo(StmlNodus*));
    si (c->catenae == NIHIL || c->summaria == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(c->catenae); k++)
    {
        StmlNodus* sm = crusta_effectus_derivare(c->piscina, c->intern,
            c->radix, *(character**)xar_obtinere(c->catenae, k),
            c->tabula, causa);

        si (sm == NIHIL)
        {
            redde FALSUM;
        }
        *(StmlNodus**)xar_addere(c->summaria) = sm;
    }
    redde VERUM;
}

/* plagulam unam iudicare; reddit numerum erratorum (-I = refutatio) */
interior s32
_plagulam_effectuum_iudicare (
              Contextus* c,
    constans character* via,
                    b32 machina)
{
                chorda  fons;
                   Xar* summaria;
                   Xar* d;
                   b32  in_catena = FALSUM;
                   i32  k;
                   s32  errata  = ZEPHYRUM;
    constans character* causa   = NIHIL;
             character  absoluta[VIA_MAXIMA];

    sprintf(absoluta, "%s/%s", c->radix, via);
    fons = filum_legere_totum(absoluta, c->piscina);
    si (fons.datum == NIHIL)
    {
        fprintf(stderr, "effectus: %s illegibilis\n", via);
        redde -I;
    }
    summaria = xar_creare(c->piscina, (i32)magnitudo(StmlNodus*));
    per (k = ZEPHYRUM; k < xar_numerus(c->summaria); k++)
    {
        StmlNodus* sm = *(StmlNodus**)xar_obtinere(c->summaria, k);

        si (crusta_effectus_plagulam_tenet(sm, via))
        {
            *(StmlNodus**)xar_addere(summaria)  = sm;
            in_catena                           = VERUM;
        }
    }
    si (!in_catena)
    {
        StmlNodus* sm = crusta_effectus_derivare(c->piscina, c->intern,
            c->radix, via, c->tabula, &causa);

        si (sm == NIHIL)
        {
            fprintf(stderr, "effectus: %s: %s\n", via,
                causa != NIHIL ? causa : "?");
            redde -I;
        }
        *(StmlNodus**)xar_addere(summaria) = sm;
    }
    d = crusta_effectus_diagnostica(c->piscina, via,
        (constans character*)fons.datum, (i32)fons.mensura, summaria,
        in_catena, &c->optiones, &causa);
    si (d == NIHIL)
    {
        fprintf(stderr, "effectus: %s: %s\n", via,
            causa
                != NIHIL ? causa : "diagnostica derivari non possunt");
        redde -I;
    }
    per (k = ZEPHYRUM; k < xar_numerus(d); k++)
    {
        constans MateriaDiagnosticum* x =
            (constans MateriaDiagnosticum*)
            xar_obtinere(d, k);
        chorda t = materia_pictor_scribere(c->piscina, x, via, "crusta",
            (constans character*)fons.datum, (i32)fons.mensura,
            !machina);

        fwrite(t.datum, I, (size_t)t.mensura, stdout);
        si (x->gravitas == (s32)MATERIA_GRAVITAS_ERRATUM)
        {
            errata++;
        }
    }
    redde errata;
}

/* modus lintri aut catenarum inter argumenta (involucrum -radix
 * ante modum ponit); NIHIL = modus alius */
interior constans character*
_modus_lintri_quaerere (
        integer   argc,
      character** argv)
{
    integer i;

    per (i = I; i < argc; i++)
    {
        si (   strcmp(argv[i], "-lintrum") == ZEPHYRUM
            || strcmp(argv[i], "-catenae") == ZEPHYRUM
            || strcmp(argv[i], "-census")  == ZEPHYRUM)
        {
            redde argv[i];
        }
    }
    redde NIHIL;
}

/* attributum situs ut chorda C ('-' si absens) */
interior constans character*
_cella (
               Piscina* piscina,
             StmlNodus* n,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(n, titulus);

    redde v == NIHIL ? "-" : chorda_ut_cstr(*v, piscina);
}

/* numeratio per clavem (Xar de character* + Xar de i32 paralleli) */
interior vacuum
_numerare (
               Piscina* piscina,
                   Xar* claves,
                   Xar* numeri,
    constans character* clavis)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(claves); k++)
    {
        si (strcmp(*(character**)xar_obtinere(claves, k), clavis)
            == ZEPHYRUM)
        {
            (*(i32*)xar_obtinere(numeri, k))++;
            redde;
        }
    }
    *(character**)xar_addere(claves) = chorda_ut_cstr(
        chorda_ex_literis(clavis, piscina), piscina);
    *(i32*)xar_addere(numeri) = I;
}

/* CENSUS (planum T6, spec par. VI.1): situs omnis omnium scriptorum
 * (index ex stdin) in build/effectus/census.tsv - plagula linea
 * elementum per mandatum resolutio classis scripta via lintrum (codices
 * post excusationes). Summa per genus, resolutionem, regulam; et quot
 * excusationes absorbuerunt (cursus sine excusatione comparatus). */
interior integer
_modus_census (
    Contextus* c)
{
    ChordaAedificator* tsv = chorda_aedificator_creare(c->piscina,
        (memoriae_index)M * M);
                  Xar* claves = xar_creare(c->piscina,
                             (i32)magnitudo(character*));
                  Xar* numeri = xar_creare(c->piscina,
                      (i32)magnitudo(i32));
            character linea[VIA_MAXIMA];
            character via_census[VIA_MAXIMA];
                  i32 plagulae  = ZEPHYRUM;
                  i32 situs     = ZEPHYRUM;
                  i32 excusata  = ZEPHYRUM;
                  i32 k;

    chorda_aedificator_appendere_literis(tsv, "# effectus census "
        "(GENERATUM: ./crusta/effectus.sh -census) - plagula linea "
        "elementum per mandatum resolutio classis scripta via "
        "lintrum\n");
    dum (fgets(linea, (integer)magnitudo(linea), stdin) != NIHIL)
    {
        memoriae_index  n = strlen(linea);
                   Xar* summaria;
                   Xar* d;
                   Xar* d_crudum;
                   b32  in_catena = FALSUM;
                chorda  fons;
             character  absoluta[VIA_MAXIMA * II];
        CrustaOptiones  crudae;
    constans character* causa = NIHIL;
                   i32  i;
                   i32  j;

        dum (   n > ZEPHYRUM && (linea[n - I] == '\n'
                              || linea[n - I] == '\r'))
        {
            linea[--n] = '\0';
        }
        si (n == ZEPHYRUM)
        {
            perge;
        }
        sprintf(absoluta, "%s/%s", c->radix, linea);
        fons = filum_legere_totum(absoluta, c->piscina);
        si (fons.datum == NIHIL)
        {
            perge;
        }
        summaria = xar_creare(c->piscina, (i32)magnitudo(StmlNodus*));
        per (i = ZEPHYRUM; i < xar_numerus(c->summaria); i++)
        {
            StmlNodus* sm = *(StmlNodus**)xar_obtinere(c->summaria, i);

            si (crusta_effectus_plagulam_tenet(sm, linea))
            {
                *(StmlNodus**)xar_addere(summaria)  = sm;
                in_catena                           = VERUM;
            }
        }
        si (!in_catena)
        {
            StmlNodus* sm = crusta_effectus_derivare(c->piscina,
                c->intern, c->radix, linea, c->tabula, &causa);

            si (sm == NIHIL)
            {
                perge;
            }
            *(StmlNodus**)xar_addere(summaria) = sm;
        }
        d = crusta_effectus_diagnostica(c->piscina, linea,
            (constans character*)fons.datum, (i32)fons.mensura,
            summaria,
            in_catena, &c->optiones, &causa);
        crudae                   = c->optiones;
        crudae.sine_excusatione  = VERUM;
        d_crudum = crusta_effectus_diagnostica(c->piscina, linea,
            (constans character*)fons.datum, (i32)fons.mensura,
            summaria,
            in_catena, &crudae, &causa);
        si (   d != NIHIL && d_crudum != NIHIL
            && xar_numerus(d_crudum) > xar_numerus(d))
        {
            excusata += (i32)(xar_numerus(d_crudum) - xar_numerus(d));
        }
        plagulae++;
        /* situs huius plagulae, sedibus non iteratis */
        {
            Xar* visa = xar_creare(c->piscina,
                (i32)magnitudo(character*));

            per (i = ZEPHYRUM; i < xar_numerus(summaria); i++)
            {
                 StmlNodus* sm;
                       i32  q;

                sm = *(StmlNodus**)xar_obtinere(summaria, i);

                per (q = ZEPHYRUM; sm->liberi
                                   && q < xar_numerus(sm->liberi); q++)
                {
                    StmlNodus* pr =
                        *(StmlNodus**)xar_obtinere(sm->liberi,
                        q);

                    per (j = ZEPHYRUM; pr->liberi != NIHIL
                                       && j < xar_numerus(pr->liberi);
                         j++)
                    {
                        StmlNodus* s = *(StmlNodus**)xar_obtinere(
                            pr->liberi, j);
                        constans character* octeti;
                        constans character* el;
                                 character  clavis[VIA_MAXIMA];
                                 character  inventa[CCLVI];
                                       s32  initium;
                                       i32  w;
                                       b32  iteratum = FALSUM;

                        si (   s->genus != STML_NODUS_ELEMENTUM
                            || strcmp(_cella(c->piscina, s, "plagula"),
                                   linea) != ZEPHYRUM)
                        {
                            perge;
                        }
                        el = chorda_ut_cstr(*s->titulus, c->piscina);
                        octeti = _cella(c->piscina, s, "octeti");
                        sprintf(clavis, "%s %s", el, octeti);
                        per (w = ZEPHYRUM; w < xar_numerus(visa); w++)
                        {
                            si (strcmp(*(character**)xar_obtinere(visa,
                                    w), clavis) == ZEPHYRUM)
                            {
                                iteratum = VERUM;
                            }
                        }
                        si (iteratum)
                        {
                            perge;
                        }
                        *(character**)xar_addere(visa) = chorda_ut_cstr(
                            chorda_ex_literis(clavis, c->piscina),
                            c->piscina);
                        situs++;
                        initium = (s32)strtol(octeti, NIHIL, X);
                        inventa[ZEPHYRUM] = '\0';
                        per (w = ZEPHYRUM; d != NIHIL
                                           && w < xar_numerus(d); w++)
                        {
                            constans MateriaDiagnosticum* x =
                                (constans MateriaDiagnosticum*)
                                xar_obtinere(d, w);

                            si (   x->tractus.initium == initium
                                && x->codex           != NIHIL
                                && strlen(inventa) + strlen(x->codex)
                                    + II
                                   < magnitudo(inventa))
                            {
                                si (inventa[ZEPHYRUM] != '\0')
                                {
                                    strcat(inventa, ",");
                                }
                                strcat(inventa, x->codex);
                                _numerare(c->piscina, claves, numeri,
                                    x->codex);
                            }
                        }
                        _numerare(c->piscina, claves, numeri, el);
                        _numerare(c->piscina, claves, numeri,
                            _cella(c->piscina, s, "resolutio"));
                        sprintf(clavis,
                            "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
                            linea, _cella(c->piscina, s, "sedes"), el,
                            _cella(c->piscina, s, "per"),
                            _cella(c->piscina, s, "mandatum"),
                            _cella(c->piscina, s, "resolutio"),
                            _cella(c->piscina, s, "classis"),
                            _cella(c->piscina, s, "scripta_in_ambitu"),
                            inventa[ZEPHYRUM] != '\0' ? inventa : "-");
                        chorda_aedificator_appendere_literis(tsv,
                            clavis);
                        (vacuum)j;
                    }
                }
            }
        }
    }
    sprintf(via_census, "%s/build/effectus", c->radix);
    (vacuum)filum_directorium_creare_cum_parentibus(via_census);
    sprintf(via_census, "%s/build/effectus/census.tsv", c->radix);
    (vacuum)filum_scribere(via_census, chorda_aedificator_finire(tsv));
    imprimere("census: plagulae %u, situs %u, excusata %u -> "
        "build/effectus/census.tsv\n", (insignatus integer)plagulae,
        (insignatus integer)situs, (insignatus integer)excusata);
    per (k = ZEPHYRUM; k < xar_numerus(claves); k++)
    {
        imprimere("  %6u  %s\n",
            (insignatus integer)*(i32*)xar_obtinere(numeri, k),
            *(character**)xar_obtinere(claves, k));
    }
    redde ZEPHYRUM;
}

interior integer
_modus_lintri (
                integer   argc,
              character** argv,
     constans character*  radix,
     constans character*  via_tabulae)
{
             Contextus  c;
    constans character* causa = NIHIL;
               integer  i;
                   s32  errata   = ZEPHYRUM;
                   b32  fractum  = FALSUM;

    c.piscina = piscina_generare_dynamicum("effectus_lintrum",
        (memoriae_index)LXIV * M * M);
    si (c.piscina == NIHIL)
    {
        redde II;
    }
    c.intern  = internamentum_creare(c.piscina);
    c.radix   = radix;
    c.tabula  = NIHIL;
    si (via_tabulae != NIHIL)
    {
        StmlResultus r = stml_legere(filum_legere_totum(via_tabulae,
            c.piscina), c.piscina, c.intern);

        c.tabula = r.successus ? r.elementum_radix : NIHIL;
    }
    si (!_contextum_parare(&c, &causa))
    {
        fprintf(stderr, "effectus: %s\n", causa != NIHIL ? causa : "?");
        piscina_destruere(c.piscina);
        redde II;
    }
    si (strcmp(_modus_lintri_quaerere(argc, argv), "-census")
        == ZEPHYRUM)
    {
        integer exitus = _modus_census(&c);

        piscina_destruere(c.piscina);
        redde exitus;
    }
    si (strcmp(_modus_lintri_quaerere(argc, argv), "-catenae")
        == ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i < (integer)xar_numerus(c.catenae); i++)
        {
            imprimere("%s\n",
                *(character**)xar_obtinere(c.catenae, (i32)i));
        }
        piscina_destruere(c.piscina);
        redde ZEPHYRUM;
    }
    per (i = I; i < argc; i++)
    {
        s32 n;

        si (   strcmp(argv[i], "-radix")  == ZEPHYRUM
            || strcmp(argv[i], "-tabula") == ZEPHYRUM)
        {
            i++;   /* valor iam lectus */
            perge;
        }
        si (argv[i][ZEPHYRUM] == '-')
        {
            perge;   /* modus ipse */
        }
        n = _plagulam_effectuum_iudicare(&c, argv[i], FALSUM);
        si (n < ZEPHYRUM)
        {
            fractum = VERUM;
        }
        alioquin
        {
            errata += n;
        }
    }
    piscina_destruere(c.piscina);
    si (fractum)
    {
        redde II;
    }
    redde errata > ZEPHYRUM ? I : ZEPHYRUM;
}

integer
principale (
      integer   argc,
    character** argv)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
               StmlNodus* summarium;
                  chorda  textus;
               character  radix[VIA_MAXIMA];
      constans character* modus        = NIHIL;
      constans character* scriptum     = NIHIL;
      constans character* liber        = NIHIL;
      constans character* ante_via     = NIHIL;
      constans character* via_tabulae  = NIHIL;
               StmlNodus* tabula       = NIHIL;
      constans character* causa        = NIHIL;
                 integer  i;
                     i32  k;

    radix[ZEPHYRUM] = '\0';
    /* modi lintri et catenarum: argumenta sua (plagulae plures) */
    si (_modus_lintri_quaerere(argc, argv) != NIHIL)
    {
        constans character* tabula_via = NIHIL;

        per (i = I; i + I < argc; i++)
        {
            si (   strcmp(argv[i], "-radix") == ZEPHYRUM
                && strlen(argv[i + I]) < magnitudo(radix))
            {
                strcpy(radix, argv[i + I]);
            }
            si (strcmp(argv[i], "-tabula") == ZEPHYRUM)
            {
                tabula_via = argv[i + I];
            }
        }
        si (   radix[ZEPHYRUM]                 == '\0'
            && getcwd(radix, magnitudo(radix)) == NIHIL)
        {
            redde II;
        }
        redde _modus_lintri(argc, argv, radix, tabula_via);
    }
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-radix") == ZEPHYRUM && i + I < argc)
        {
            i++;
            si (strlen(argv[i]) >= magnitudo(radix))
            {
                fprintf(stderr, "effectus: radix nimis longa\n");
                redde II;
            }
            strcpy(radix, argv[i]);
        }
        alioquin si (   strcmp(argv[i], "-ante_scripta") == ZEPHYRUM
                     && i + I < argc)
        {
            ante_via = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-tabula") == ZEPHYRUM
                     && i + I < argc)
        {
            via_tabulae = argv[++i];
        }
        alioquin si (   modus == NIHIL && scriptum == NIHIL
                     && (strcmp(argv[i], "-observata") == ZEPHYRUM
                         || strcmp(argv[i], "-comparare") == ZEPHYRUM))
        {
            modus = argv[i];
        }
        alioquin si (scriptum == NIHIL)
        {
            scriptum = argv[i];
        }
        alioquin si (modus != NIHIL && liber == NIHIL)
        {
            liber = argv[i];
        }
        alioquin
        {
            _usus();
            redde II;
        }
    }
    si (scriptum == NIHIL || (modus != NIHIL && liber == NIHIL))
    {
        _usus();
        redde II;
    }
    si (   radix[ZEPHYRUM]                 == '\0'
        && getcwd(radix, magnitudo(radix)) == NIHIL)
    {
        fprintf(stderr, "effectus: directorium operis ignotum\n");
        redde II;
    }
    k = (i32)strlen(radix);
    dum (k > I && radix[k - I] == '/')
    {
        radix[k - I] = '\0';
        k--;
    }
    piscina = piscina_generare_dynamicum("crusta_effectus",
        (memoriae_index)XVI * M * M);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "effectus: memoria deficit\n");
        redde II;
    }
    intern = internamentum_creare(piscina);
    si (via_tabulae != NIHIL)
    {
        StmlResultus r = stml_legere(filum_legere_totum(via_tabulae,
            piscina), piscina, intern);

        si (!r.successus)
        {
            fprintf(stderr, "effectus: tabula '%s' illegibilis\n",
                via_tabulae);
            piscina_destruere(piscina);
            redde II;
        }
        tabula = r.elementum_radix;
    }

    si (modus == NIHIL)
    {
        summarium = crusta_effectus_derivare(piscina, intern, radix,
            scriptum, tabula, &causa);
        si (summarium == NIHIL)
        {
            fprintf(stderr, "effectus: %s: %s\n", scriptum,
                causa != NIHIL ? causa : "memoria deficit");
            piscina_destruere(piscina);
            redde II;
        }
        textus = stml_scribere(summarium, piscina, VERUM);
        fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
        imprimere("\n");
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    /* oraculum: liber -> observata; comparare: contra staticum */
    {
         chorda  datum = filum_legere_totum(liber, piscina);
            Xar* ante = xar_creare(piscina, (i32)magnitudo(character*));
      StmlNodus* observatum;
      StmlNodus* staticum;
            Xar* non_tecta;
            Xar* explicata;

        si (datum.datum == NIHIL)
        {
            fprintf(stderr, "effectus: liber '%s' illegibilis\n",
                liber);
            piscina_destruere(piscina);
            redde II;
        }
        observatum = crusta_effectus_observata(piscina, intern, radix,
            scriptum, datum, tabula, ante, &causa);
        si (observatum == NIHIL)
        {
            fprintf(stderr, "effectus: observata: %s\n",
                causa != NIHIL ? causa : "memoria deficit");
            piscina_destruere(piscina);
            redde II;
        }
        si (ante_via != NIHIL)
        {
            ChordaAedificator* ca = chorda_aedificator_creare(piscina,
                (memoriae_index)MXXIV);

            per (k = ZEPHYRUM; k < xar_numerus(ante); k++)
            {
                chorda_aedificator_appendere_literis(ca,
                    *(character**)xar_obtinere(ante, k));
                chorda_aedificator_appendere_character(ca, '\n');
            }
            (vacuum)filum_scribere(ante_via,
                chorda_aedificator_finire(ca));
        }
        si (strcmp(modus, "-observata") == ZEPHYRUM)
        {
            textus = stml_scribere(observatum, piscina, VERUM);
            fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
            imprimere("\n");
            piscina_destruere(piscina);
            redde ZEPHYRUM;
        }
        staticum = crusta_effectus_derivare(piscina, intern, radix,
            scriptum, tabula, &causa);
        si (staticum == NIHIL)
        {
            fprintf(stderr, "effectus: %s: %s\n", scriptum,
                causa != NIHIL ? causa : "memoria deficit");
            piscina_destruere(piscina);
            redde II;
        }
        explicata = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
        non_tecta = crusta_effectus_non_tecta(piscina, staticum,
            observatum, explicata);
        per (k = ZEPHYRUM; k < xar_numerus(explicata); k++)
        {
            StmlNodus* o = *(StmlNodus**)xar_obtinere(explicata, k);

            imprimere("ignotum\t%.*s\t%s\t%s\n",
                (integer)o->titulus->mensura,
                (constans character*)o->titulus->datum,
                _attributum(piscina, o, "via"),
                _attributum(piscina, o, "mandatum"));
        }
        per (k = ZEPHYRUM; non_tecta && k < xar_numerus(non_tecta); k++)
        {
            StmlNodus* o = *(StmlNodus**)xar_obtinere(non_tecta, k);

            imprimere("%.*s\t%s\t%s\n", (integer)o->titulus->mensura,
                (constans character*)o->titulus->datum,
                _attributum(piscina, o, "via"),
                _attributum(piscina, o, "mandatum"));
        }
        imprimere("effectus: observata non tecta %u, per ignota "
            "explicata %u, ante scripta %u\n",
            non_tecta ? (insignatus integer)xar_numerus(non_tecta) : 0U,
            (insignatus integer)xar_numerus(explicata),
            (insignatus integer)xar_numerus(ante));
        k = non_tecta != NIHIL && xar_numerus(non_tecta) == ZEPHYRUM
            ? ZEPHYRUM : I;
        piscina_destruere(piscina);
        redde (integer)k;
    }
}
