/* nexus.c - NEXUS CLI (instrumenta prima #4b): quaestio tabulae
 * build/nexus.tsv (nexus_percursus eam generat) - millisecunda,
 * sine silva.
 *
 * Usus: nexus <symbolum> [-omnia]     sedes + usus, per plagulam
 *       nexus -similis <quaestio>     nomina similia (similitudo)
 * -omnia: etiam symbola localia (profunditas > 0); defaltum =
 * scopus fili solus (quaestio "ubi est X" trans plagulas).
 * Nihil inventum -> suggestiones "fortasse" (exemplar emitte).
 *
 * Exitus: 0 inventum | 1 non inventum | 2 tabula deest / usus
 */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "similitudo.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

#define VIA_TABULAE "build/nexus.tsv"
#define LINEAE_MONSTRATAE XX
#define SIMILES_MAXIMI X

nomen structura {
    chorda titulus;
    chorda species;      /* "sedes" | "usus" */
    chorda genus;
    chorda via;
    i32    linea;
    i32    columna;
    i32    profunditas;
} NexusOrdo;

nomen structura {
    chorda via;
    Xar*   ordines;      /* NexusOrdo* (indices in omnes) */
} NexusGrex;

hic_manens i32
_numerus_ex_chorda (chorda c)
{
    i32 valor = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < c.mensura; k++)
    {
        character littera = (character)c.datum[k];

        si (littera < '0' || littera > '9') frange;
        valor = valor * X + (i32)(littera - '0');
    }
    redde valor;
}

/* campum proximum usque ad tab/novissimam secare; cursorem movet */
hic_manens chorda
_campum_secare (character* textus, i32 mensura,
    i32* cursor)
{
    chorda campus;
    i32 initium = *cursor;
    i32 k = initium;

    dum (k < mensura && textus[k] != '\t' && textus[k] != '\n')
    {
        k++;
    }
    campus.datum = (i8*)(textus + initium);
    campus.mensura = (i32)(k - initium);
    *cursor = (k < mensura && textus[k] == '\t') ? k + I : k;
    redde campus;
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina;
    constans character* quaestio = NIHIL;
    b32 omnia = FALSUM;
    b32 modus_similis = FALSUM;
    FILE* pl;
    long mensura_l;
    i32 mensura;
    character* textus;
    Xar* congruentes;      /* NexusOrdo */
    Xar* tituli_distincti; /* chorda (pro similis/fortasse) */
    TabulaDispersa* tituli_visi;
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            omnia = VERUM;
        }
        alioquin si (strcmp(argv[k], "-similis") == ZEPHYRUM)
        {
            modus_similis = VERUM;
        }
        alioquin si (quaestio == NIHIL)
        {
            quaestio = argv[k];
        }
    }
    si (quaestio == NIHIL)
    {
        fprintf(stderr, "usus: nexus <symbolum> [-omnia] |"
            " nexus -similis <quaestio>\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("nexus", 8388608);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "nexus: piscina deest\n");
        redde II;
    }

    pl = fopen(VIA_TABULAE, "rb");
    si (pl == NIHIL)
    {
        fprintf(stderr, "nexus: %s deest - genera per"
            " ./silva/nexus.sh -renovare\n", VIA_TABULAE);
        redde II;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l <= 0L)
    {
        fclose(pl);
        fprintf(stderr, "nexus: tabula vacua\n");
        redde II;
    }
    mensura = (i32)mensura_l;
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura);
    si (textus == NIHIL
        || fread(textus, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        fprintf(stderr, "nexus: tabula non lecta\n");
        redde II;
    }
    fclose(pl);

    congruentes = xar_creare(piscina, magnitudo(NexusOrdo));
    tituli_distincti = xar_creare(piscina, magnitudo(chorda));
    tituli_visi = tabula_dispersa_creare_chorda(piscina, DXII);
    si (congruentes == NIHIL || tituli_distincti == NIHIL
        || tituli_visi == NIHIL)
    {
        redde II;
    }

    /* tabulam percurrere */
    {
        i32 cursor = ZEPHYRUM;
        chorda quaestio_ch = chorda_ex_literis(quaestio, piscina);

        dum (cursor < mensura)
        {
            NexusOrdo ordo;
            chorda campus;

            si (textus[cursor] == '#')
            {
                dum (cursor < mensura && textus[cursor] != '\n')
                {
                    cursor++;
                }
                si (cursor < mensura) cursor++;
                perge;
            }

            ordo.titulus = _campum_secare(textus, mensura, &cursor);
            ordo.species = _campum_secare(textus, mensura, &cursor);
            ordo.genus = _campum_secare(textus, mensura, &cursor);
            ordo.via = _campum_secare(textus, mensura, &cursor);
            campus = _campum_secare(textus, mensura, &cursor);
            ordo.linea = _numerus_ex_chorda(campus);
            campus = _campum_secare(textus, mensura, &cursor);
            ordo.columna = _numerus_ex_chorda(campus);
            campus = _campum_secare(textus, mensura, &cursor);
            ordo.profunditas = _numerus_ex_chorda(campus);
            dum (cursor < mensura && textus[cursor] != '\n')
            {
                cursor++;
            }
            si (cursor < mensura) cursor++;

            si (ordo.titulus.mensura == ZEPHYRUM) perge;

            /* nomina distincta (similis/fortasse) */
            si (!tabula_dispersa_continet(tituli_visi,
                    ordo.titulus))
            {
                chorda* novum;

                (vacuum)tabula_dispersa_inserere(tituli_visi,
                    ordo.titulus, NIHIL);
                novum = (chorda*)xar_addere(tituli_distincti);
                si (novum != NIHIL) *novum = ordo.titulus;
            }

            si (modus_similis) perge;
            si (!omnia && ordo.profunditas > ZEPHYRUM) perge;
            si (chorda_aequalis(ordo.titulus, quaestio_ch))
            {
                NexusOrdo* novus =
                    (NexusOrdo*)xar_addere(congruentes);

                si (novus != NIHIL) *novus = ordo;
            }
        }
    }

    /* modus -similis: nomina simillima cum numeris */
    si (modus_similis)
    {
        SimilitudoFructus fructus[SIMILES_MAXIMI];
        i32 numerus = xar_numerus(tituli_distincti);
        chorda* acies = (chorda*)piscina_allocare(piscina,
            (memoriae_index)(numerus > ZEPHYRUM ? numerus : I)
                * magnitudo(chorda));
        i32 n = ZEPHYRUM;
        i32 f;

        si (acies != NIHIL)
        {
            i32 c;

            per (c = ZEPHYRUM; c < numerus; c++)
            {
                acies[c] = *(chorda*)xar_obtinere(tituli_distincti,
                    c);
            }
            n = similitudo_optima(
                chorda_ex_literis(quaestio, piscina), acies,
                numerus, fructus, SIMILES_MAXIMI);
        }
        si (n == ZEPHYRUM)
        {
            imprimere("nexus: nihil simile '%s'\n", quaestio);
            redde I;
        }
        per (f = ZEPHYRUM; f < n; f++)
        {
            imprimere("%.*s\n",
                (int)acies[fructus[f].index].mensura,
                (constans character*)acies[fructus[f].index].datum);
        }
        redde ZEPHYRUM;
    }

    si (xar_numerus(congruentes) == ZEPHYRUM)
    {
        fprintf(stderr, "nexus: symbolum non inventum: %s%s\n",
            quaestio, omnia ? "" : " (localia? proba -omnia)");
        /* fortasse (exemplar emitte: quaestio decurtata donec
         * candidati appareant) */
        {
            SimilitudoFructus fructus[V];
            i32 numerus = xar_numerus(tituli_distincti);
            chorda* acies = (chorda*)piscina_allocare(piscina,
                (memoriae_index)(numerus > ZEPHYRUM
                    ? numerus : I) * magnitudo(chorda));
            i32 n = ZEPHYRUM;

            si (acies != NIHIL && numerus > ZEPHYRUM)
            {
                chorda q = chorda_ex_literis(quaestio, piscina);
                i32 c;

                per (c = ZEPHYRUM; c < numerus; c++)
                {
                    acies[c] = *(chorda*)xar_obtinere(
                        tituli_distincti, c);
                }
                n = similitudo_optima_decurtata(q, acies, numerus,
                    fructus, V);
            }
            si (n > ZEPHYRUM)
            {
                i32 f;

                fprintf(stderr, "fortasse:\n");
                per (f = ZEPHYRUM; f < n; f++)
                {
                    fprintf(stderr, "  %.*s\n",
                        (int)acies[fructus[f].index].mensura,
                        (constans character*)
                            acies[fructus[f].index].datum);
                }
            }
        }
        redde I;
    }

    /* effusio: sedes primum, tum usus per plagulam gregati */
    {
        i32 m;
        i32 usus_summa = ZEPHYRUM;
        Xar* greges = xar_creare(piscina, magnitudo(NexusGrex));

        si (greges == NIHIL) redde II;

        per (m = ZEPHYRUM; m < xar_numerus(congruentes); m++)
        {
            NexusOrdo* ordo =
                (NexusOrdo*)xar_obtinere(congruentes, m);

            si (chorda_aequalis_literis(ordo->species, "sedes"))
            {
                imprimere("SEDES %.*s:%u:%u  %.*s",
                    (int)ordo->via.mensura,
                    (constans character*)ordo->via.datum,
                    ordo->linea, ordo->columna,
                    (int)ordo->genus.mensura,
                    (constans character*)ordo->genus.datum);
                si (ordo->profunditas > ZEPHYRUM)
                {
                    imprimere("  [localis, prof %u]",
                        ordo->profunditas);
                }
                imprimere("\n");
            }
            alioquin
            {
                i32 g;
                NexusGrex* grex = NIHIL;

                usus_summa++;
                per (g = ZEPHYRUM; g < xar_numerus(greges); g++)
                {
                    NexusGrex* candidatus =
                        (NexusGrex*)xar_obtinere(greges, g);

                    si (chorda_aequalis(candidatus->via,
                            ordo->via))
                    {
                        grex = candidatus;
                        frange;
                    }
                }
                si (grex == NIHIL)
                {
                    grex = (NexusGrex*)xar_addere(greges);
                    si (grex == NIHIL) perge;
                    grex->via = ordo->via;
                    grex->ordines = xar_creare(piscina,
                        magnitudo(i32));
                }
                si (grex->ordines != NIHIL)
                {
                    i32* index = (i32*)xar_addere(grex->ordines);

                    si (index != NIHIL) *index = m;
                }
            }
        }

        si (usus_summa > ZEPHYRUM)
        {
            imprimere("USUS %u in %u plagulis:\n",
                usus_summa, xar_numerus(greges));
            per (m = ZEPHYRUM; m < xar_numerus(greges); m++)
            {
                NexusGrex* grex = (NexusGrex*)xar_obtinere(greges,
                    m);
                i32 n = grex->ordines != NIHIL
                    ? xar_numerus(grex->ordines) : ZEPHYRUM;
                i32 u;

                imprimere("  %.*s (%u):",
                    (int)grex->via.mensura,
                    (constans character*)grex->via.datum, n);
                per (u = ZEPHYRUM;
                     u < n && u < LINEAE_MONSTRATAE; u++)
                {
                    i32* index = (i32*)xar_obtinere(grex->ordines,
                        u);
                    NexusOrdo* ordo = (NexusOrdo*)xar_obtinere(
                        congruentes, *index);

                    imprimere(" %u", ordo->linea);
                }
                si (n > LINEAE_MONSTRATAE)
                {
                    /* numquam trunca tacita */
                    imprimere(" +%u reliquae",
                        n - LINEAE_MONSTRATAE);
                }
                imprimere("\n");
            }
        }
    }
    redde ZEPHYRUM;
}
