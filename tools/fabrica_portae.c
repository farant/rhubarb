/* fabrica_portae.c - bin/fabrica portae: PORTAE DECLARATAE (plan
 * portae-declaratae T3; nucleus lib/portae.c, caput include/portae.h)
 *
 * Usus (ex radice repositorii):
 *   bin/fabrica portae debitae [-machina] -portae TSV VIA...
 *       quae portae viis mutatis debentur (et quae viae intectae)
 *   bin/fabrica portae tactus [-machina] -portae TSV (-omnes | VIA...)
 *       quas portas quaeque via tangit, rationibus OMNIBUS (-omnes =
 *       viae omnes arboris; comparatio T4)
 *   bin/fabrica portae declaratae [-machina] -portae TSV
 *       tabula portarum declaratarum et opertura derivata
 *   bin/fabrica portae relatio [-machina] -portae TSV [-tempora VIA]
 *       portae per genera unitatum: opertura, sumptus, intecta,
 *       rationes textus et infrastructurae (tools/fabrica_relatio.c)
 *   bin/fabrica portae translatio [-machina] -portae TSV
 *           [-receptum PORTA ARBOR DIGESTUM]... PORTA...
 *       verdictum viride portae ad arborem praesentem portari potest?
 *       (translatio receptorum spec §II): ARBOR praesens (git tree
 *       status OPERIS per indicem temporarium), INSTRUMENTA (digestum
 *       binariorum quae porta currit), TRANSLATIO per portam
 *
 * -portae TSV: registrum ab involucro traditum, linea una per portam
 * 'clavis<TAB>cursor[<TAB>directorium probationum]' (PORTAE et SUITAE
 * pythonicae; C Pythonem numquam parsat). Effusio -machina: contractus
 * in include/portae.h. Exitus: 0 computatum · 2 usus, registrum
 * illegibile aut RECUSATIO declarationum.
 *
 * Sutura vera: viae per 'git ls-files', plagulae per filum, effectus
 * per crusta_effectus (tabula mandatorum semel lecta), litterae
 * probationum per lexatorem silvae amalgamatae (commenta absunt;
 * plagula sine 'system'/'processus_' nihil vocat), clausurae per
 * suturae_clausura_c (aedilis, idem quod gradus fabricae). */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "processus.h"
#include "internamentum.h"
#include "stml.h"
#include "fabrica.h"
#include "portae.h"
#include "crusta_effectus.h"
#include "fabrica_sutura.h"
#include "fabrica_litterae.h"

#include "sigillum.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

nomen structura {
    InternamentumChorda* intern;
                 chorda  radix;      /* absoluta, sine '/' finali */
              StmlNodus* mandata;    /* tabula effectus; NIHIL = legitur
                                      * a crusta ipsa */
} PortaeMundus;

interior constans character*
_cstr (
      chorda  c,
     Piscina* piscina)
{
    redde chorda_ut_cstr(c, piscina);
}

interior Xar*
_enumerare (
      vacuum* datum,
     Piscina* piscina)
{
    hic_manens constans character* constans argumenta[] = {
        "git", "ls-files", NIHIL
    };
        ProcessusResultus  r;
    chorda_fissio_fructus  lineae;
                      Xar* viae;
                      i32  i;

    (vacuum)datum;
    r = processus_exsequi(argumenta, (i32)XXX * M, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde NIHIL;
    }
    viae    = xar_creare(piscina, (i32)magnitudo(chorda));
    lineae  = chorda_fissio(r.effusio, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        si (lineae.elementa[i].mensura > ZEPHYRUM)
        {
            *(chorda*)xar_addere(viae) = lineae.elementa[i];
        }
    }
    redde viae;
}

interior b32
_legere (
      vacuum* datum,
      chorda  via,
     Piscina* piscina,
      chorda* textus)
{
    constans character* v = _cstr(via, piscina);

    (vacuum)datum;
    si (!filum_existit(v))
    {
        redde FALSUM;
    }
    *textus = filum_legere_totum(v, piscina);
    redde VERUM;
}

interior StmlNodus*
_effectus (
      vacuum* datum,
      chorda  cursor,
     Piscina* piscina)
{
           PortaeMundus* m      = (PortaeMundus*)datum;
     constans character* causa  = NIHIL;

    redde crusta_effectus_derivare(piscina, m->intern, _cstr(m->radix,
        piscina), _cstr(cursor, piscina), m->mandata, &causa);
}

interior Xar*
_litterae (
      vacuum* datum,
      chorda  fons,
     Piscina* piscina)
{
    chorda  textus;
       Xar* fructus;

    si (!_legere(datum, fons, piscina, &textus))
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(chorda));
    si (   !chorda_continet(textus, chorda_ex_literis("system",
        piscina))
        && !chorda_continet(textus, chorda_ex_literis("processus_",
               piscina)))
    {
        redde fructus;
    }
    si (!suturae_litteras_colligere(textus, piscina, fructus))
    {
        redde NIHIL;
    }
    redde fructus;
}

interior Xar*
_clausura (
      vacuum* datum,
      chorda  fons,
     Piscina* piscina)
{
    FabricaClausuraC  clausura;
              chorda  causa;
                 Xar* fructus;
                 i32  i;

    (vacuum)datum;
    causa = chorda_ex_literis("", piscina);
    si (!suturae_clausura_c(NIHIL, _cstr(fons, piscina), piscina,
            &clausura, &causa))
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(clausura.fontes); i++)
    {
        *(chorda*)xar_addere(fructus) = *(chorda*)xar_obtinere(
            clausura.fontes, i);
    }
    per (i = ZEPHYRUM; i < xar_numerus(clausura.capita); i++)
    {
        *(chorda*)xar_addere(fructus) = *(chorda*)xar_obtinere(
            clausura.capita, i);
    }
    redde fructus;
}

/* registrum TSV: 'clavis<TAB>cursor[<TAB>probationes]'; NIHIL =
 * illegibile (causa in stderr) */
interior Xar*
_registrum_legere (
    constans character* via,
               Piscina* piscina)
{
                   chorda  textus;
    chorda_fissio_fructus  lineae;
                      Xar* tabula;
                      i32  i;

    si (!filum_existit(via))
    {
        fprintf(stderr, "fabrica portae: registrum absens: %s\n", via);
        redde NIHIL;
    }
    textus = filum_legere_totum(via, piscina);
    tabula = xar_creare(piscina, (i32)magnitudo(PortaeIngressus));
    lineae = chorda_fissio(textus, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        chorda_fissio_fructus  campi;
              PortaeIngressus* p;

        si (lineae.elementa[i].mensura == ZEPHYRUM)
        {
            perge;
        }
        campi = chorda_fissio(lineae.elementa[i], '\t', piscina);
        si (campi.numerus < II || campi.numerus > III)
        {
            fprintf(stderr, "fabrica portae: %s:%u: linea registri "
                "mala (clavis<TAB>cursor[<TAB>probationes])\n", via,
                (insignatus integer)(i + I));
            redde NIHIL;
        }
        p          = (PortaeIngressus*)xar_addere(tabula);
        p->clavis  = campi.elementa[ZEPHYRUM];
        p->cursor  = campi.elementa[I];
        p->probationes  = campi.numerus == III ? campi.elementa[II]
            : chorda_ex_literis("", piscina);
    }
    redde tabula;
}

interior vacuum
_scribere (
    chorda textus)
{
    (vacuum)fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
}


/* ==================================================
 * TRANSLATIO (translatio receptorum spec §II, T2)
 * ================================================== */

/* effusio processus sine lineis novis finalibus; NIHIL-chorda si
 * fractus */
interior b32
_curre_lineam (
    constans character* constans* argumenta,
                           chorda* linea,
                          Piscina* piscina)
{
    ProcessusResultus r = processus_exsequi(argumenta, (i32)LX * M,
        piscina);

    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *linea = r.effusio;
    dum (   linea->mensura > ZEPHYRUM
         && (linea->datum[linea->mensura - I] == '\n'
             || linea->datum[linea->mensura - I] == '\r'))
    {
        linea->mensura--;
    }
    redde VERUM;
}

/* ARBOR STATUS OPERIS: tracta mutata et novae non ignoratae, per
 * indicem TEMPORARIUM (copia indicis veri; GIT_INDEX_FILE per 'env' -
 * ambitus processus exactus est, hereditas ita servatur) - index verus
 * intactus, obiecta in repositorio ut 'git diff <a> <b>' postea
 * valeat. FALSUM cum causa in stderr. */
interior b32
_arborem_status (
    constans PortaeMundus* mundus,
                   chorda* arbor,
                  Piscina* piscina)
{
    hic_manens constans character* constans via_indicis[] = {
        "git", "rev-parse", "--git-path", "index", NIHIL
    };
                 chorda  index;
              character  temporarius[MXXIV];
              character  ambitus[MXXIV + XXXII];
     constans character* addere[VIII];
     constans character* scribere[V];
                 chorda  vacua;
                    b32  bene;

    si (!_curre_lineam(via_indicis, &index, piscina))
    {
        fprintf(stderr, "fabrica portae translatio: 'git rev-parse"
            " --git-path index' fractum\n");
        redde FALSUM;
    }
    /* iuxta indicem verum (in directorio git, numquam in arbore:
     * 'git add -A' eum ipsum aliter adderet ubi build/ non
     * ignoratur) */
    si (index.mensura > ZEPHYRUM && index.datum[ZEPHYRUM] != '/')
    {
        /* repositorium principale: '.git/index' relativum radici */
        ChordaAedificator* r = chorda_aedificator_creare(piscina,
            (memoriae_index)CCLVI);

        chorda_aedificator_appendere_chorda(r, mundus->radix);
        chorda_aedificator_appendere_character(r, '/');
        chorda_aedificator_appendere_chorda(r, index);
        index = chorda_aedificator_finire(r);
    }
    sprintf(temporarius, "%.*s.translatio.%ld",
        (integer)(index.mensura > DCCC ? DCCC : index.mensura),
        (constans character*)index.datum, (longus)getpid());
    /* index absens (repositorium recens): index temporarius vacuus
     * incipit - 'git add -A' eum creat */
    (vacuum)filum_delere(temporarius);
    si (   filum_existit(_cstr(index, piscina))
        && !filum_copiare(_cstr(index, piscina), temporarius))
    {
        fprintf(stderr, "fabrica portae translatio: index copiari"
            " nequit: %s\n", temporarius);
        redde FALSUM;
    }
    sprintf(ambitus, "GIT_INDEX_FILE=%s", temporarius);
    addere[ZEPHYRUM]    = "env";
    addere[I]           = ambitus;
    addere[II]          = "git";
    addere[III]         = "add";
    addere[IV]          = "-A";
    addere[V]           = "--";
    addere[VI]          = ".";
    addere[VII]         = NIHIL;
    scribere[ZEPHYRUM]  = "env";
    scribere[I]         = ambitus;
    scribere[II]        = "git";
    scribere[III]       = "write-tree";
    scribere[IV]        = NIHIL;
    bene = _curre_lineam(addere, &vacua, piscina)
        && _curre_lineam(scribere, arbor, piscina)
        && arbor->mensura > ZEPHYRUM;
    (vacuum)filum_delere(temporarius);
    si (!bene)
    {
        fprintf(stderr, "fabrica portae translatio: arbor status"
            " scribi nequit (git add -A / write-tree)\n");
    }
    redde bene;
}

/* viae mutatae inter arbores (git diff --name-only --no-renames: nomen
 * vetus ET novum); NIHIL = arbor ignota (putata) aut git fractum */
interior Xar*
_viae_mutatae (
      chorda  ab,
      chorda  ad,
     Piscina* piscina)
{
     constans character* argumenta[VII];
      ProcessusResultus  r;
  chorda_fissio_fructus  lineae;
                    Xar* viae;
                    i32  i;

    argumenta[ZEPHYRUM] = "git";
    argumenta[I] = "diff";
    argumenta[II] = "--name-only";
    argumenta[III] = "--no-renames";
    argumenta[IV] = _cstr(ab, piscina);
    argumenta[V] = _cstr(ad, piscina);
    argumenta[VI] = NIHIL;
    r = processus_exsequi(argumenta, (i32)LX * M, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde NIHIL;
    }
    viae    = xar_creare(piscina, (i32)magnitudo(chorda));
    lineae  = chorda_fissio(r.effusio, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        si (lineae.elementa[i].mensura > ZEPHYRUM)
        {
            *(chorda*)xar_addere(viae) = lineae.elementa[i];
        }
    }
    redde viae;
}

/* digestum binariorum quae porta currit (opertura->binaria, 'bin/x'):
 * via + contentum per binarium, absens nominatum; NIHIL opertura =
 * vacuum */
interior chorda
_digestum_binariorum (
    constans PortaeOpertura* o,
                    Piscina* piscina)
{
    SigillumContextus ctx;
             Sigillum s;
            character hex[SIGILLUM_HEX_MENSURA];
                  i32 i;

    sigillum_incipere(&ctx);
    per (i = ZEPHYRUM; o != NIHIL && o->binaria != NIHIL
         && i < xar_numerus(o->binaria); i++)
    {
        chorda b = ((constans PortaeExemplar*)xar_obtinere(o->binaria,
            i))->exemplar;
        constans character* v = _cstr(b, piscina);

        sigillum_addere(&ctx, b.datum, (memoriae_index)b.mensura);
        sigillum_addere(&ctx, "\n", (memoriae_index)I);
        si (filum_existit(v))
        {
            chorda c = filum_legere_totum(v, piscina);

            sigillum_addere(&ctx, c.datum, (memoriae_index)c.mensura);
        }
        alioquin
        {
            sigillum_addere(&ctx, "\001absens", (memoriae_index)VII);
        }
    }
    s = sigillum_finire(&ctx);
    sigillum_hex(&s, hex);
    redde chorda_ex_literis(hex, piscina);
}

/* receptum unum (-receptum PORTA ARBOR DIGESTUM) */
nomen structura {
    chorda porta;
    chorda arbor;
    chorda digestum;
} PortaeReceptum;

interior constans PortaeReceptum*
_receptum_portae (
    constans Xar* recepta,
          chorda  porta)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(recepta); i++)
    {
        constans PortaeReceptum* r = (constans PortaeReceptum*)
            xar_obtinere(recepta, i);

        si (chorda_aequalis(r->porta, porta))
        {
            redde r;
        }
    }
    redde NIHIL;
}

/* translationes portarum quaesitarum: per arborem recepti SEMEL
 * computatae (tactus et clausurae una vocatione), ordine quaesitarum;
 * sine recepto NULLUM (praeter NUMQUAM et non declaratas); digestum
 * mutatum vertit PORTAT in INSTRUMENTA */
interior s32
_translationem_agere (
    constans PortaeMundus* mundus,
    constans PortaeSutura* sutura,
             constans Xar* operturae,
             constans Xar* quaesitae,
             constans Xar* recepta,
                      b32  machina,
                  Piscina* piscina)
{
                      chorda  arbor;
            PortaeTranslatio* fructus;
                         Xar* ordinatae;
                         Xar* facta;    /* b32 per quaesitam */
           ChordaAedificator* a;
                         i32  i;
                         i32  j;

    si (!_arborem_status(mundus, &arbor, piscina))
    {
        redde II;
    }
    fructus = (PortaeTranslatio*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(PortaeTranslatio)
        * (memoriae_index)(xar_numerus(quaesitae) + I)));
    facta = xar_creare(piscina, (i32)magnitudo(b32));
    si (fructus == NIHIL || facta == NIHIL)
    {
        redde II;
    }
    per (i = ZEPHYRUM; i < xar_numerus(quaesitae); i++)
    {
        *(b32*)xar_addere(facta) = FALSUM;
    }
    /* per arborem recepti (sine recepto: viae nullae) */
    per (i = ZEPHYRUM; i < xar_numerus(quaesitae); i++)
    {
                         chorda porta = *(constans chorda*)
                             xar_obtinere(quaesitae, i);
        constans PortaeReceptum* r = _receptum_portae(recepta, porta);
                            Xar* grex;
                            Xar* indices;
                            Xar* viae;
                            Xar* tr;

        si (*(b32*)xar_obtinere(facta, i))
        {
            perge;
        }
        grex     = xar_creare(piscina, (i32)magnitudo(chorda));
        indices  = xar_creare(piscina, (i32)magnitudo(i32));
        per (j = i; j < xar_numerus(quaesitae); j++)
        {
            constans PortaeReceptum* alterum;

            alterum = _receptum_portae(recepta, *(constans chorda*)
                xar_obtinere(quaesitae, j));
            si (   !*(b32*)xar_obtinere(facta, j)
                && ((r == NIHIL && alterum == NIHIL)
                    || (r != NIHIL && alterum != NIHIL
                        && chorda_aequalis(r->arbor, alterum->arbor))))
            {
                *(chorda*)xar_addere(grex)     = *(constans chorda*)
                    xar_obtinere(quaesitae, j);
                *(i32*)xar_addere(indices)     = j;
                *(b32*)xar_obtinere(facta, j)  = VERUM;
            }
        }
        viae = r == NIHIL ? xar_creare(piscina, (i32)magnitudo(chorda))
            : _viae_mutatae(r->arbor, arbor, piscina);
        si (viae == NIHIL)
        {
            /* arbor recepti ignota (putata): DEBITA, numquam
             * portatur */
            per (j = ZEPHYRUM; j < xar_numerus(grex); j++)
            {
                PortaeTranslatio* t =
                    &fructus[*(i32*)xar_obtinere(indices,
                    j)];

                t->porta     = *(chorda*)xar_obtinere(grex, j);
                t->genus     = PORTAE_TRANSLATIO_DEBITA;
                t->opertura  = NIHIL;
                a = chorda_aedificator_creare(piscina,
                    (memoriae_index)LXIV);
                chorda_aedificator_appendere_literis(a,
                    "arbor recepti ignota: ");
                chorda_aedificator_appendere_chorda(a, r->arbor);
                t->causa = chorda_aedificator_finire(a);
            }
            perge;
        }
        tr = portae_translationes_computare(sutura, operturae, viae,
            grex,
            piscina);
        si (tr == NIHIL)
        {
            fprintf(stderr, "fabrica portae translatio: computari"
                " nequit\n");
            redde II;
        }
        per (j = ZEPHYRUM; j < xar_numerus(tr); j++)
        {
            PortaeTranslatio* t = &fructus[*(i32*)xar_obtinere(indices,
                j)];

            *t = *(constans PortaeTranslatio*)xar_obtinere(tr, j);
            si (r == NIHIL && t->genus == PORTAE_TRANSLATIO_PORTAT)
            {
                t->genus = PORTAE_TRANSLATIO_NULLUM;
                t->causa = chorda_ex_literis("receptum nullum",
                    piscina);
            }
            alioquin si (   r        != NIHIL
                         && t->genus == PORTAE_TRANSLATIO_PORTAT
                         && !chorda_aequalis(_digestum_binariorum(
                                t->opertura, piscina),
                                _receptum_portae(recepta, t->porta)
                                ->digestum))
            {
                t->genus = PORTAE_TRANSLATIO_INSTRUMENTA;
                t->causa = chorda_ex_literis("binaria quae porta currit"
                    " mutata post receptum", piscina);
            }
        }
    }
    ordinatae = xar_creare(piscina, (i32)magnitudo(PortaeTranslatio));
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    chorda_aedificator_appendere_literis(a, machina ? "ARBOR\t"
        : "arbor  ");
    chorda_aedificator_appendere_chorda(a, arbor);
    chorda_aedificator_appendere_character(a, '\n');
    per (i = ZEPHYRUM; i < xar_numerus(quaesitae); i++)
    {
        *(PortaeTranslatio*)xar_addere(ordinatae) = fructus[i];
        si (fructus[i].opertura != NIHIL)
        {
            chorda_aedificator_appendere_literis(a, machina
                ? "INSTRUMENTA\t" : "instrumenta  ");
            chorda_aedificator_appendere_chorda(a,
                portae_campum_effugere(
                fructus[i].porta, piscina));
            chorda_aedificator_appendere_character(a, machina ? '\t'
                : ' ');
            chorda_aedificator_appendere_chorda(a, _digestum_binariorum(
                fructus[i].opertura, piscina));
            chorda_aedificator_appendere_character(a, '\n');
        }
    }
    _scribere(chorda_aedificator_finire(a));
    _scribere(portae_translationes_scribere(ordinatae, machina,
        piscina));
    redde ZEPHYRUM;
}

interior vacuum
_usus (vacuum)
{
    fprintf(stderr,
        "usus: bin/fabrica portae debitae [-machina] -portae TSV "
        "VIA...\n"
        "      bin/fabrica portae tactus [-machina] -portae TSV "
        "(-omnes | VIA...)\n"
        "      bin/fabrica portae declaratae [-machina] -portae TSV\n"
        "      bin/fabrica portae ordo [-machina] -portae TSV"
        " [-tempora VIA] PORTA...\n"
        "      bin/fabrica portae relatio [-machina] -portae TSV"
        " [-tempora VIA]\n"
        "      bin/fabrica portae translatio [-machina] -portae TSV"
        " [-receptum PORTA ARBOR DIGESTUM]... PORTA...\n");
}

s32
suturae_portae (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
               PortaeMundus  mundus;
               PortaeSutura  sutura;
         constans character* via_registri  = NIHIL;
                        b32  machina       = FALSUM;
                        b32  debitae;
                        b32  tactus;
                        b32  ordo;
                        b32  relatio;
                        b32  translatio;
                        Xar* recepta;
         constans character* via_temporum  = "build/portae/tempora.tsv";
                        b32  omnes         = FALSUM;
                        Xar* viae;
                        Xar* declaratae;
                        Xar* recusationes = NIHIL;
                        Xar* operturae;
                        Xar* debita;
                        Xar* intecta = NIHIL;
                     chorda  causa;
                     chorda  tabula_mandatorum;
                        s32  a;

    si (   argc < III || (strcmp(argv[2], "debitae") != 0
        && strcmp(argv[2], "tactus") != 0
        && strcmp(argv[2], "declaratae") != 0
        && strcmp(argv[2], "ordo") != 0
        && strcmp(argv[2], "relatio") != 0
        && strcmp(argv[2], "translatio") != 0))
    {
        _usus();
        redde II;
    }
    debitae     = strcmp(argv[2], "debitae") == 0;
    tactus      = strcmp(argv[2], "tactus") == 0;
    ordo        = strcmp(argv[2], "ordo") == 0;
    relatio     = strcmp(argv[2], "relatio") == 0;
    translatio  = strcmp(argv[2], "translatio") == 0;
    viae        = xar_creare(piscina, (i32)magnitudo(chorda));
    recepta     = xar_creare(piscina, (i32)magnitudo(PortaeReceptum));
    per (a = III; a < argc; a++)
    {
        si (strcmp(argv[a], "-machina") == 0)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[a], "-portae") == 0 && a + I < argc)
        {
            via_registri = argv[++a];
        }
        alioquin si (tactus && strcmp(argv[a], "-omnes") == 0)
        {
            omnes = VERUM;
        }
        alioquin si (   (ordo || relatio)
                     && strcmp(argv[a], "-tempora") == 0
                     && a + I < argc)
        {
            via_temporum = argv[++a];
        }
        alioquin si (   translatio && strcmp(argv[a], "-receptum") == 0
                     && a + III < argc)
        {
            PortaeReceptum* r = (PortaeReceptum*)xar_addere(recepta);

            r->porta     = chorda_ex_literis(argv[a + I], piscina);
            r->arbor     = chorda_ex_literis(argv[a + II], piscina);
            r->digestum  = chorda_ex_literis(argv[a + III], piscina);
            a            += III;
        }
        alioquin si (   (debitae || tactus || ordo || translatio)
                     && argv[a][0] != '-')
        {
            *(chorda*)xar_addere(viae) = chorda_ex_literis(argv[a],
                piscina);
        }
        alioquin
        {
            _usus();
            redde II;
        }
    }
    si (via_registri == NIHIL)
    {
        _usus();
        redde II;
    }
    mundus.intern   = internamentum_creare(piscina);
    mundus.radix    = suturae_radix_absoluta(piscina);
    mundus.mandata  = NIHIL;
    tabula_mandatorum =
        filum_legere_totum("crusta/effectus_mandata.stml",
        piscina);
    si (tabula_mandatorum.mensura > ZEPHYRUM)
    {
        StmlResultus r = stml_legere(tabula_mandatorum, piscina,
            mundus.intern);

        mundus.mandata = r.successus ? r.elementum_radix : NIHIL;
    }
    sutura.datum                 = &mundus;
    sutura.plagulas_enumerare    = _enumerare;
    sutura.plagulam_legere       = _legere;
    sutura.effectus              = _effectus;
    sutura.litterae_probationis  = _litterae;
    sutura.clausura_c            = _clausura;
    sutura.tabula_portarum      = _registrum_legere(via_registri,
        piscina);
    si (sutura.tabula_portarum == NIHIL)
    {
        redde II;
    }
    causa = chorda_ex_literis("", piscina);
    declaratae = portae_declaratas_legere(&sutura, piscina, &causa,
        &recusationes);
    si (declaratae == NIHIL)
    {
        _scribere(debitae
            || tactus ? portae_debita_scribere(NIHIL, NIHIL,
            recusationes, ZEPHYRUM, machina, piscina)
            : portae_declaratas_scribere(NIHIL, recusationes, machina,
                piscina));
        redde II;
    }
    /* ORDO (circulus commissionis II T5): declarationes solae - genus
     * ex annotatione, sumptus ex tempora.tsv; derivatio non poscitur.
     * Tempora absentia = omnes sine historia (ordo nominum servatur). */
    si (ordo)
    {
        chorda tempora = filum_legere_totum(via_temporum, piscina);

        _scribere(portae_ordinem_scribere(portae_ordinare(declaratae,
            viae, tempora, piscina), machina, piscina));
        redde ZEPHYRUM;
    }
    operturae = portae_operturas_derivare(&sutura, declaratae, piscina);
    si (operturae == NIHIL)
    {
        fprintf(stderr, "fabrica portae: derivatio fracta (enumeratio "
            "'git ls-files' aut memoria)\n");
        redde II;
    }
    si (translatio)
    {
        redde _translationem_agere(&mundus, &sutura, operturae, viae,
            recepta, machina, piscina);
    }
    si (relatio)
    {
        Xar* omnes_viae = _enumerare(&mundus, piscina);

        si (omnes_viae == NIHIL)
        {
            fprintf(stderr, "fabrica portae: 'git ls-files' fractum\n");
            redde II;
        }
        redde suturae_portae_relatio(&sutura, declaratae, operturae,
            omnes_viae, filum_legere_totum(via_temporum, piscina),
            machina, piscina);
    }
    si (!debitae && !tactus)
    {
        _scribere(portae_declaratas_scribere(operturae, NIHIL, machina,
            piscina));
        redde ZEPHYRUM;
    }
    si (omnes)
    {
        viae = _enumerare(&mundus, piscina);
        si (viae == NIHIL)
        {
            fprintf(stderr, "fabrica portae: 'git ls-files' fractum\n");
            redde II;
        }
    }
    si (tactus)
    {
        debita = portae_tactus_computare(&sutura, operturae, viae,
            &intecta, piscina);
        si (debita == NIHIL)
        {
            fprintf(stderr, "fabrica portae: tactus computari "
                "nequeunt\n");
            redde II;
        }
        _scribere(portae_tactus_scribere(debita, intecta, machina,
            piscina));
        redde ZEPHYRUM;
    }
    debita = portae_debita_computare(&sutura, operturae, viae, &intecta,
        piscina);
    si (debita == NIHIL)
    {
        fprintf(stderr, "fabrica portae: debita computari nequeunt\n");
        redde II;
    }
    _scribere(portae_debita_scribere(debita, intecta, NIHIL,
        xar_numerus(viae), machina, piscina));
    redde ZEPHYRUM;
}
