/* frigida.c - via scripturae frigida (vide frigida.h) */

#include "frigida.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "json.h"
#include <string.h>

#define FRIGIDA_IMPERIUM "./gesta/frigida.sh"

/* formae verborum: nomina operandorum ordine (NIHIL terminat).
 * operandum primum SEMPER res est. */
nomen structura {
    constans character* vexillum;
    constans character* instrumentum; /* instrumentum machinae */
    constans character* actus;        /* actus gerere; NIHIL = lectio */
    constans character* operanda[IV]; /* claves argumentorum */
                   b32  citanda[IV];  /* in imperio valido "citata" */
} FrigidaForma;

interior constans FrigidaForma FORMAE[] = {
    { "-status",  "gerere", "status",
      { "res", "novus", NIHIL, NIHIL },
      { VERUM, FALSUM, FALSUM, FALSUM } },
    { "-mutatio", "gerere", "mutatio",
      { "res", "clavis", "valor", NIHIL },
      { VERUM, FALSUM, VERUM, FALSUM } },
    { "-nexus",   "gerere", "nexus",
      { "res", "verbum", "alterum", NIHIL },
      { VERUM, FALSUM, VERUM, FALSUM } },
    /* LECTIO (nihil scribit): breviarium rei - linea prima
     * 'Titulus (genus, status)' contractus lectorum est
     * (silva.commissio opus ante portas praeiudicat) */
    { "-res",     "res",    NIHIL,
      { "res", NIHIL, NIHIL, NIHIL },
      { VERUM, FALSUM, FALSUM, FALSUM } },
    /* LECTIO SINE OPERANDO: arbor regionum nominibus solis (forma
     * 'nomina' instrumenti mappa) - quod Fran manu currit */
    { "-mappa",   "mappa",  NIHIL,
      { NIHIL, NIHIL, NIHIL, NIHIL },
      { FALSUM, FALSUM, FALSUM, FALSUM } }
};
#define FORMAE_NUMERUS V

interior constans FrigidaForma*
_formam_invenire (
    constans character* vexillum)
{
    i32 i;

    si (vexillum == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < FORMAE_NUMERUS; i++)
    {
        si (strcmp(vexillum, FORMAE[i].vexillum) == ZEPHYRUM)
        {
            redde &FORMAE[i];
        }
    }
    redde NIHIL;
}

interior b32
_forma_est_nexus (
    constans FrigidaForma* forma)
{
    redde forma != NIHIL && forma->actus != NIHIL
        && strcmp(forma->actus, "nexus") == ZEPHYRUM;
}

b32
frigida_verbum_novit (
    constans character* vexillum)
{
    si (vexillum == NIHIL)
    {
        redde FALSUM;
    }
    redde _formam_invenire(vexillum) != NIHIL
        || strcmp(vexillum, "-actor") == ZEPHYRUM
        || strcmp(vexillum, "-origo") == ZEPHYRUM
        || strcmp(vexillum, "-vis") == ZEPHYRUM;
}

interior chorda
_ch (
    constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l        = litterae != NIHIL ? litterae : "";
    c.datum    = u.m;
    c.mensura  = (i32)strlen(u.l);
    redde c;
}

/* causam novam in indice incipere: "\n  N. " */
interior vacuum
_causam_incipere (
    ChordaAedificator* aed,
                  i32* numerus)
{
    character caput[XXXII];

    (*numerus)++;
    sprintf(caput, "\n  %d. ", (int)*numerus);
    chorda_aedificator_appendere_literis(aed, caput);
}

/* valorem campi ex linea 'SCRIPTURA VALIDA' machinae extrahere:
 * post 'clavis: "' usque ad '"' proximum. NIHIL si abest. (Valor
 * cum '"' interno truncaretur - claves ibi res_id sunt aut verba;
 * tituli crudi rari, et tunc linea machinae ipsa legibilis manet.) */
interior constans character*
_campum_extrahere (
               Piscina* pn,
    constans character* textus,
    constans character* clavis)
{
    constans character* linea = strstr(textus, "SCRIPTURA VALIDA:");
    constans character* initium;
    constans character* finis;
             character* fructus;
        memoriae_index  mensura;

    si (linea == NIHIL)
    {
        redde NIHIL;
    }
    initium = strstr(linea, clavis);
    si (initium == NIHIL)
    {
        redde NIHIL;
    }
    initium  += strlen(clavis);
    finis    = strchr(initium, '"');
    si (finis == NIHIL)
    {
        redde NIHIL;
    }
    mensura = (memoriae_index)(finis - initium);
    fructus = (character*)piscina_allocare(pn, mensura + I);
    si (fructus == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(fructus, initium, mensura);
    fructus[mensura] = '\0';
    redde fructus;
}

/* IMPERIUM VALIDUM: linea imperii pasta-parata - optiones VOCANTIS
 * servatae, operanda data aut locus-tenens '<clavis>' ubi desunt */
interior vacuum
_imperium_validum_appendere (
        ChordaAedificator*  aed,
       constans character*  actor,
       constans character*  origo,
                      b32   vis,
    constans FrigidaForma*  forma,
       constans character** operanda)
{
    i32 i;

    chorda_aedificator_appendere_literis(aed,
        "\nIMPERIUM VALIDUM: " FRIGIDA_IMPERIUM);
    si (vis)
    {
        chorda_aedificator_appendere_literis(aed, " -vis");
    }
    si (actor != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, " -actor \"");
        chorda_aedificator_appendere_literis(aed, actor);
        chorda_aedificator_appendere_literis(aed, "\"");
    }
    si (origo != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, " -origo \"");
        chorda_aedificator_appendere_literis(aed, origo);
        chorda_aedificator_appendere_literis(aed, "\"");
    }
    chorda_aedificator_appendere_literis(aed, " ");
    chorda_aedificator_appendere_literis(aed, forma->vexillum);
    per (i = ZEPHYRUM; i < IV && forma->operanda[i] != NIHIL; i++)
    {
        chorda_aedificator_appendere_literis(aed, " ");
        si (operanda[i] == NIHIL || operanda[i][0] == '\0')
        {
            chorda_aedificator_appendere_literis(aed, "<");
            chorda_aedificator_appendere_literis(aed,
                forma->operanda[i]);
            chorda_aedificator_appendere_literis(aed, ">");
        }
        alioquin si (operanda[i][0] == '<')
        {
            /* locus-tenens machinae ('<a | b>') verbatim */
            chorda_aedificator_appendere_literis(aed, operanda[i]);
        }
        alioquin si (forma->citanda[i])
        {
            chorda_aedificator_appendere_literis(aed, "\"");
            chorda_aedificator_appendere_literis(aed, operanda[i]);
            chorda_aedificator_appendere_literis(aed, "\"");
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(aed, operanda[i]);
        }
    }
}

/* vocationem 'gerere' per machinam mittere; textus responsi in
 * *textus, redditum = isError machinae (VERUM = recusatum). JSON per
 * aedificatorem componitur - valores cum '"' RECTE effugiuntur. */
interior b32
_gerere_mittere (
                Tabularium*  t,
                   Piscina*  pn,
     constans FrigidaForma*  forma,
        constans character** operanda,
        constans character*  actor,
        constans character*  origo,
                       b32   vis,
        constans character** textus)
{
    JsonValor* radix      = json_objectum_creare(pn);
    JsonValor* parametra  = json_objectum_creare(pn);
    JsonValor* argumenta  = json_objectum_creare(pn);
         FILE* captura    = tmpfile();
          b32  recusatum  = VERUM;
          i32  i;

    *textus = "responsum machinae legi non potuit";
    si (captura == NIHIL)
    {
        redde VERUM;
    }
    si (forma->actus != NIHIL)
    {
        json_objectum_ponere(argumenta, "actus",
            json_chorda_creare_literis(pn, forma->actus));
        si (vis)
        {
            json_objectum_ponere(argumenta, "vis",
                json_chorda_creare_literis(pn, "verum"));
        }
    }
    alioquin si (strcmp(forma->instrumentum, "mappa") == ZEPHYRUM)
    {
        /* lectio mappae: nomina sola (ut 'tree') */
        json_objectum_ponere(argumenta, "forma",
            json_chorda_creare_literis(pn, "nomina"));
    }
    alioquin
    {
        /* lectio: breviarium (forma plena datum crudum effundit) */
        json_objectum_ponere(argumenta, "breviter",
            json_chorda_creare_literis(pn, "verum"));
    }
    per (i = ZEPHYRUM; i < IV && forma->operanda[i] != NIHIL; i++)
    {
        json_objectum_ponere(argumenta, forma->operanda[i],
            json_chorda_creare_literis(pn, operanda[i]));
    }
    si (forma->actus != NIHIL)
    {
        json_objectum_ponere(argumenta, "actor",
            json_chorda_creare_literis(pn, actor));
        json_objectum_ponere(argumenta, "origo",
            json_chorda_creare_literis(pn, origo));
    }
    json_objectum_ponere(parametra, "name",
        json_chorda_creare_literis(pn, forma->instrumentum));
    json_objectum_ponere(parametra, "arguments", argumenta);
    json_objectum_ponere(radix, "jsonrpc",
        json_chorda_creare_literis(pn, "2.0"));
    json_objectum_ponere(radix, "id", json_integer_creare(pn,
        (s64)I));
    json_objectum_ponere(radix, "method",
        json_chorda_creare_literis(pn, "tools/call"));
    json_objectum_ponere(radix, "params", parametra);
    (vacuum)tabularium_tractare(t, pn, json_scribere(radix, pn),
        captura);
    {
                  long  mensura;
             character* crudum;
          JsonResultus  r;

        fseek(captura, 0L, SEEK_END);
        mensura = ftell(captura);
        fseek(captura, 0L, SEEK_SET);
        crudum = (character*)piscina_allocare(pn,
            (memoriae_index)(mensura > 0L ? mensura + 1L : I));
        si (   crudum != NIHIL && mensura > 0L
            && fread(crudum, I, (memoriae_index)mensura, captura)
                == (memoriae_index)mensura)
        {
            chorda corpus;
            unio { character* l; i8* m; } u;

            crudum[mensura]  = '\0';
            u.l              = crudum;
            corpus.datum     = u.m;
            corpus.mensura   = (i32)mensura;
            r                = json_legere(corpus, pn);
            si (r.successus && json_est_objectum(r.radix))
            {
                JsonValor* fructus = json_objectum_capere(r.radix,
                    "result");
                JsonValor* error_v = json_objectum_capere(r.radix,
                    "error");

                si (fructus != NIHIL && json_est_objectum(fructus))
                {
                    JsonValor* vitium = json_objectum_capere(
                        fructus, "isError");
                    JsonValor* contentum = json_objectum_capere(
                        fructus, "content");

                    recusatum = vitium != NIHIL
                        && json_est_boolean(vitium)
                        && json_ad_boolean(vitium);
                    si (   contentum != NIHIL
                        && json_est_tabulatum(contentum)
                        && json_tabulatum_numerus(contentum)
                            > ZEPHYRUM)
                    {
                        JsonValor* primum = json_tabulatum_obtinere(
                            contentum, ZEPHYRUM);
                        JsonValor* textus_valor = primum != NIHIL
                            ? json_objectum_capere(primum, "text")
                            : NIHIL;

                        si (   textus_valor != NIHIL
                            && json_est_chorda(textus_valor))
                        {
                               chorda  c = json_ad_chorda(textus_valor);
                            character* copia = (character*)
                                piscina_allocare(pn,
                                    (memoriae_index)c.mensura + I);

                            si (copia != NIHIL)
                            {
                                memcpy(copia, c.datum,
                                    (memoriae_index)c.mensura);
                                copia[c.mensura]  = '\0';
                                *textus           = copia;
                            }
                        }
                    }
                }
                alioquin si (error_v != NIHIL)
                {
                    *textus = "machina errorem protocolli reddidit";
                }
            }
        }
    }
    fclose(captura);
    redde recusatum;
}

integer
frigida_currere (
    constans TabulariumConfiguratio*  cfg,
                            integer   argc,
                          character** argv,
                               FILE*  effusio,
                               FILE*  errores)
{
                  Piscina* pn;
        ChordaAedificator* index;
       constans character* actor_datus  = NIHIL;
       constans character* origo_data   = NIHIL;
                      b32  vis          = FALSUM;
       constans character* vexillum     = NIHIL;
    constans FrigidaForma* forma        = NIHIL;
       constans character* operanda[IV];
       constans character* textus_machinae  = NIHIL;
               Tabularium* t                = NIHIL;
                      i32  causae           = ZEPHYRUM;
                      i32  expectata        = ZEPHYRUM;
                      i32  data             = ZEPHYRUM;
                  integer  k                = I;
                  integer  exitus;
                      i32  i;

    pn = piscina_generare_dynamicum("frigida", 67108864);
    si (pn == NIHIL)
    {
        fprintf(errores, "frigida: piscina creari non potuit\n");
        redde FRIGIDA_EXITUS_RECUSATUM;
    }
    index = chorda_aedificator_creare(pn, DXII);
    per (i = ZEPHYRUM; i < IV; i++)
    {
        operanda[i] = NIHIL;
    }

    /* optiones praeviae (-actor, -origo): valor deest = causa, et
     * pergimus - cetera quoque iudicanda sunt */
    dum (   k < argc && (   strcmp(argv[k], "-actor") == ZEPHYRUM
                      || strcmp(argv[k], "-origo") == ZEPHYRUM
                      || strcmp(argv[k], "-vis") == ZEPHYRUM))
    {
        b32 est_actor = strcmp(argv[k], "-actor") == ZEPHYRUM;

        si (strcmp(argv[k], "-vis") == ZEPHYRUM)
        {
            /* vexillum sine valore: lex 'scribe, ne obsta' */
            vis  = VERUM;
            k    += I;
            perge;
        }
        si (k + I < argc && argv[k + I][0] != '-')
        {
            si (est_actor)
            {
                actor_datus = argv[k + I];
            }
            alioquin
            {
                origo_data = argv[k + I];
            }
            k += II;
        }
        alioquin
        {
            _causam_incipere(index, &causae);
            chorda_aedificator_appendere_literis(index, "optio ");
            chorda_aedificator_appendere_literis(index, argv[k]);
            chorda_aedificator_appendere_literis(index,
                ": valor deest");
            k += I;
        }
    }

    /* verbum */
    si (k < argc)
    {
        vexillum  = argv[k];
        forma     = _formam_invenire(vexillum);
        k++;
    }
    si (vexillum == NIHIL)
    {
        _causam_incipere(index, &causae);
        chorda_aedificator_appendere_literis(index,
            "verbum deest (-status | -mutatio | -nexus | -res |"
            " -mappa)");
    }
    alioquin si (forma == NIHIL)
    {
        _causam_incipere(index, &causae);
        chorda_aedificator_appendere_literis(index,
            "vexillum ignotum '");
        chorda_aedificator_appendere_literis(index, vexillum);
        chorda_aedificator_appendere_literis(index,
            "' (nota: -status | -mutatio | -nexus | -res | -mappa)");
    }

    /* operanda */
    si (forma != NIHIL)
    {
        dum (expectata < IV && forma->operanda[expectata] != NIHIL)
        {
            expectata++;
        }
        dum (k < argc && data < expectata)
        {
            operanda[data] = argv[k];
            data++;
            k++;
        }
        /* operanda absentia: PRO VINCULO machina 'verbum deest' et
         * 'alterum deest' ipsa refert (cum regulis suis, in indice
         * uno) - hic sola RES absens nominatur, ne causae
         * duplicentur. Formae ceterae hic totae iudicantur. */
        per (i = data; i < expectata; i++)
        {
            si (   _forma_est_nexus(forma)
                && i > ZEPHYRUM)
            {
                perge;
            }
            _causam_incipere(index, &causae);
            chorda_aedificator_appendere_literis(index,
                "operandum <");
            chorda_aedificator_appendere_literis(index,
                forma->operanda[i]);
            chorda_aedificator_appendere_literis(index, "> deest");
        }
        si (k < argc)
        {
            _causam_incipere(index, &causae);
            chorda_aedificator_appendere_literis(index,
                "operanda superflua:");
            dum (k < argc)
            {
                chorda_aedificator_appendere_literis(index, " '");
                chorda_aedificator_appendere_literis(index, argv[k]);
                chorda_aedificator_appendere_literis(index, "'");
                k++;
            }
        }
    }

    /* machina: pro VINCULO praeiudicium LECTIONIS SOLIUS semper
     * currit (si res data est), ut causae machinae cum causis
     * lineae imperii in responso UNO stent */
    si (forma != NIHIL && data > ZEPHYRUM)
    {
        t = tabularium_creare(pn, cfg);
        si (t == NIHIL || !tabularium_se_initiare(t))
        {
            fprintf(errores, "frigida: tabularium aperiri non potuit"
                " (ex radice repositorii curre)\n");
            tabularium_claudere(t);
            piscina_destruere(pn);
            redde FRIGIDA_EXITUS_RECUSATUM;
        }
        si (_forma_est_nexus(forma))
        {
            textus_machinae = tabularium_nexum_praeiudicare(t, pn,
                _ch(operanda[ZEPHYRUM]), _ch(operanda[I]),
                _ch(operanda[II]));
        }
        alioquin si (   forma->actus                   != NIHIL
                     && strcmp(forma->actus, "status") == ZEPHYRUM)
        {
            /* transitio contra machinam generis cum LEGALIBUS
             * PROXIMIS - imperium validum eos fert; -vis scribit
             * tamen */
            textus_machinae = tabularium_statum_praeiudicare(t, pn,
                _ch(operanda[ZEPHYRUM]), _ch(operanda[I]), vis);
        }
    }

    si (causae > ZEPHYRUM || textus_machinae != NIHIL)
    {
           ChordaAedificator* aed = chorda_aedificator_creare(pn, M);
          constans character* recta[IV];

        per (i = ZEPHYRUM; i < IV; i++)
        {
            recta[i] = operanda[i];
        }
        chorda_aedificator_appendere_literis(aed,
            "frigida RECUSATA - nihil scriptum.");
        si (causae > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed,
                "\nCausae lineae imperii:");
            chorda_aedificator_appendere_chorda(aed,
                chorda_aedificator_finire(index));
        }
        si (textus_machinae != NIHIL)
        {
            chorda_aedificator_appendere_literis(aed,
                "\nCausae machinae:\n");
            chorda_aedificator_appendere_literis(aed,
                textus_machinae);
            /* scriptura valida machinae in lineam imperii versa
             * (partes iam commutatae, verbum iam canonicum) */
            recta[ZEPHYRUM] = _campum_extrahere(pn, textus_machinae,
                "res: \"");
            si (_forma_est_nexus(forma))
            {
                recta[I] = _campum_extrahere(pn, textus_machinae,
                    "verbum: \"");
                recta[II] = _campum_extrahere(pn, textus_machinae,
                    "alterum: \"");
            }
            alioquin
            {
                /* status: '<a | b>' aut status unus legalis */
                recta[I] = _campum_extrahere(pn, textus_machinae,
                    "novus: \"");
            }
        }
        si (forma != NIHIL)
        {
            _imperium_validum_appendere(aed, actor_datus, origo_data,
                vis, forma, recta);
            si (   !vis && forma->actus != NIHIL
                && strcmp(forma->actus, "status")            == ZEPHYRUM
                && textus_machinae                           != NIHIL
                && strstr(textus_machinae, "extra machinam") != NIHIL)
            {
                chorda_aedificator_appendere_literis(aed,
                    "\n  aut, statum extra machinam TAMEN scribere:"
                    " -vis ante verbum");
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(aed,
                "\nFORMAE VALIDAE:"
                "\n  " FRIGIDA_IMPERIUM " [-actor A] [-origo O]"
                " -status \"<res>\" <novus>"
                "\n  " FRIGIDA_IMPERIUM " [-actor A] [-origo O]"
                " -mutatio \"<res>\" <clavis> \"<valor>\""
                "\n  " FRIGIDA_IMPERIUM " [-actor A] [-origo O]"
                " -nexus \"<res>\" <verbum> \"<alterum>\""
                "\n  " FRIGIDA_IMPERIUM " -res \"<res>\""
                "   (lectio - nihil scribit)"
                "\n  " FRIGIDA_IMPERIUM " -mappa"
                "   (lectio - arbor regionum, nomina sola)");
        }
        {
            chorda nuntius = chorda_aedificator_finire(aed);

            fwrite(nuntius.datum, I, (memoriae_index)nuntius.mensura,
                errores);
            fputc('\n', errores);
        }
        exitus = causae > ZEPHYRUM
            ? FRIGIDA_EXITUS_USUS : FRIGIDA_EXITUS_RECUSATUM;
        tabularium_claudere(t);
        piscina_destruere(pn);
        redde exitus;
    }

    /* scriptura (aut lectio) per IANUAM UNAM. Forma sine operando
     * (-mappa) mundum supra non aperuit - hic aperitur. */
    si (t == NIHIL)
    {
        t = tabularium_creare(pn, cfg);
        si (t == NIHIL || !tabularium_se_initiare(t))
        {
            fprintf(errores, "frigida: tabularium aperiri non potuit"
                " (ex radice repositorii curre)\n");
            tabularium_claudere(t);
            piscina_destruere(pn);
            redde FRIGIDA_EXITUS_RECUSATUM;
        }
    }
    {
        constans character* textus = NIHIL;
                       b32  recusatum;

        recusatum = _gerere_mittere(t, pn, forma, operanda,
            actor_datus != NIHIL ? actor_datus : "fran",
            origo_data != NIHIL ? origo_data : "frigida", vis,
            &textus);

        fprintf(recusatum ? errores : effusio, "%s\n", textus);
        exitus = recusatum
            ? FRIGIDA_EXITUS_RECUSATUM : FRIGIDA_EXITUS_SCRIPTUM;
    }
    tabularium_claudere(t);
    piscina_destruere(pn);
    redde exitus;
}
