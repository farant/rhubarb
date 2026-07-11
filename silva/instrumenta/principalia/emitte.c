/* emitte.c - EMITTE (instrumenta prima #2): definitio symboli
 * octetim exacta ex UNA plagula, cum capite [via:linea-linea]
 * (pin sim-2 ep.1: effusio citabilis sine relectione).
 *
 * Usus: ./silva/emitte.sh <plagula> <symbolum>
 *
 * Quid emittitur: OMNE elementum gradus summi quod symbolum LIGAT
 * (declarator/enumerator/tag - loci "tok_titulus", inventarium
 * mechanicum ex SILVA_C89_REGISTRUM ut in censore). Ambulatio
 * congruentiae corpora functionum PRAETERMITTIT (usus intra
 * corpora = opus nexus, non emitte). Elementa trans bracchia
 * conditionalia sumpta et spinas canonicas ambiguorum visitantur;
 * bracchia omissa (blobs lexematum) non parsantur - v1 quaestio.
 *
 * Extentus = lineae fontis integrae a linea lexematis primi ad
 * lineam lexematis ultimi (lexemata per radicem originis ad
 * plagulam principem reducta) - effusio EST octeti fontis ipsius.
 *
 * Exitus: 0 inventum | 1 non inventum | 2 sine arbore / usus
 */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "similitudo.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens i32 mensura_maxima = 4194304;  /* -omnia = sine tecto */

/* genera ligaminis: loci tok_titulus horum generum symbolum LIGANT
 * (typus-nominatus et accessus = USUS, exclusi ut in censore) */
hic_manens b32
_genus_ligat (s32 genus)
{
    redde (genus == (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS
        || genus == (s32)SILVA_C89_GENUS_ENUMERATOR
        || genus == (s32)SILVA_C89_GENUS_STRUCTURA
        || genus == (s32)SILVA_C89_GENUS_UNIO
        || genus == (s32)SILVA_C89_GENUS_ENUMERATIO
        || genus == (s32)SILVA_C89_GENUS_TITULATUM)
        ? VERUM : FALSUM;
}

/* tag definiens? structura/unio/enumeratio ligant solum cum membra
 * habent (lista aliqua non vacua) - "structura Xar* x;" = USUS
 * (inventum vectis: quaerens Xar etiam XarIterator emittebat) */
hic_manens b32
_tag_definit (constans SilvaNodus* nodus)
{
    i32 k;

    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        si (nodus->loci[k].genus == SILVA_VALOR_LISTA
            && silva_valor_lista_numerus(nodus->loci[k])
                > ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* nomen scriptum congruit? valor lexematis (expansus) AUT valor
 * radicis originis (scriptus) - "principale" expansum "main" est,
 * quaerens alterutrum invenit (inventum vectis: probationes) */
hic_manens b32
_nomen_congruit (SilvaToken* t, constans character* symbolum)
{
    SilvaToken* radix;

    si (chorda_aequalis_literis(t->valor, symbolum)) redde VERUM;
    radix = silva_token_radix(t);
    redde (radix != NIHIL && radix != t
        && chorda_aequalis_literis(radix->valor, symbolum))
        ? VERUM : FALSUM;
}

/* genera perspicua: continentia, non elementa (per ea descenditur
 * sine elemento figendo) */
hic_manens b32
_genus_perspicuum (s32 genus)
{
    redde (genus == (s32)SILVA_C89_GENUS_CONDITIONALIS
        || genus == (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS)
        ? VERUM : FALSUM;
}

/* --------------------------------------------------
 * ambulatio congruentiae: series laborum {valor, elementum}
 * -------------------------------------------------- */

nomen structura {
    SilvaValor  valor;
    SilvaNodus* elementum;   /* elementum gradus summi continens */
} GradusSeriei;

hic_manens vacuum
_seriem_addere (Xar* series, SilvaValor v, SilvaNodus* elementum)
{
    GradusSeriei* gradus = (GradusSeriei*)xar_addere(series);

    si (gradus != NIHIL)
    {
        gradus->valor = v;
        gradus->elementum = elementum;
    }
}

/* elementa congruentia colligere (dedup per monstratorem);
 * nomina = OMNIA nomina ligata (chordae, forma scripta) pro
 * suggestionibus "fortasse" similitudinis si nihil inventum */
hic_manens vacuum
_congruentia_quaerere (constans SilvaParsura* parsura,
    constans character* symbolum, Xar* congruentia, Xar* nomina,
    Piscina* piscina)
{
    Xar* series = xar_creare(piscina, magnitudo(GradusSeriei));
    i32 cursor = ZEPHYRUM;

    si (series == NIHIL) redde;
    _seriem_addere(series, parsura->commissio->radix, NIHIL);

    dum (cursor < xar_numerus(series))
    {
        GradusSeriei gradus =
            *(GradusSeriei*)xar_obtinere(series, cursor);
        SilvaValor v = gradus.valor;

        cursor++;

        si (v.genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(v); k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    _seriem_addere(series, *elem,
                        gradus.elementum);
                }
            }
            perge;
        }
        si (v.genus != SILVA_VALOR_NODUS || v.datum.nodus == NIHIL)
        {
            perge;
        }

        {
            SilvaNodus* nodus = v.datum.nodus;
            SilvaNodus* elementum = gradus.elementum;
            i32 k;

            si (nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS)
            {
                SilvaValor interp =
                    silva_c89_ambiguus_interpretationes(nodus);
                SilvaValor canonica =
                    silva_c89_ambiguus_canonica(nodus);

                si (canonica.genus == SILVA_VALOR_INDEX)
                {
                    SilvaValor* lectio = silva_valor_lista_obtinere(
                        interp, (i32)canonica.datum.index);

                    si (lectio != NIHIL)
                    {
                        _seriem_addere(series, *lectio, elementum);
                    }
                }
                perge;
            }
            si (nodus->genus == (s32)SILVA_C89_GENUS_ERROR
                || nodus->genus
                    == (s32)SILVA_C89_GENUS_RAMUS_OMISSUS)
            {
                perge;
            }

            /* elementum figere: primus nodus non-perspicuus sub
             * radice = elementum gradus summi */
            si (elementum == NIHIL
                && !_genus_perspicuum(nodus->genus))
            {
                elementum = nodus;
            }

            /* congruentia: loci tok_titulus generum ligantium
             * (tag sine membris = usus, non ligamen) */
            si (_genus_ligat(nodus->genus)
                && nodus->genus >= ZEPHYRUM
                && nodus->genus
                    < (s32)SILVA_C89_REGISTRUM.numerus_generum
                && ((nodus->genus
                        != (s32)SILVA_C89_GENUS_STRUCTURA
                     && nodus->genus != (s32)SILVA_C89_GENUS_UNIO
                     && nodus->genus
                        != (s32)SILVA_C89_GENUS_ENUMERATIO)
                    || _tag_definit(nodus)))
            {
                constans SilvaTabGenus* tg =
                    &SILVA_C89_REGISTRUM.genera[nodus->genus];

                per (k = ZEPHYRUM;
                     k < nodus->numerus_locorum
                     && k < tg->loci_numerus; k++)
                {
                    constans SilvaTabLocus* tl =
                        &SILVA_C89_REGISTRUM.loci[tg->loci_offset
                            + k];

                    si (tl->species == (s32)SILVA_LOCUS_TOKEN
                        && strcmp(tl->titulus, "tok_titulus")
                            == ZEPHYRUM
                        && nodus->loci[k].genus == SILVA_VALOR_TOKEN
                        && nodus->loci[k].datum.token != NIHIL)
                    {
                        SilvaToken* lexema =
                            nodus->loci[k].datum.token;

                        /* nomen colligere (forma scripta) */
                        si (nomina != NIHIL)
                        {
                            SilvaToken* radix =
                                silva_token_radix(lexema);
                            chorda* nom =
                                (chorda*)xar_addere(nomina);

                            si (nom != NIHIL)
                            {
                                *nom = (radix != NIHIL)
                                    ? radix->valor : lexema->valor;
                            }
                        }

                        si (_nomen_congruit(lexema, symbolum)
                            && elementum != NIHIL)
                        {
                            i32 d;
                            b32 iam = FALSUM;

                            per (d = ZEPHYRUM;
                                 d < xar_numerus(congruentia); d++)
                            {
                                si (*(SilvaNodus**)xar_obtinere(
                                        congruentia, d)
                                    == elementum)
                                {
                                    iam = VERUM;
                                    frange;
                                }
                            }
                            si (!iam)
                            {
                                SilvaNodus** novus = (SilvaNodus**)
                                    xar_addere(congruentia);

                                si (novus != NIHIL)
                                {
                                    *novus = elementum;
                                }
                            }
                        }
                    }
                }
            }

            /* descende - corpora functionum PRAETERMISSA (usus
             * intra corpora = opus nexus) */
            si (nodus->genus
                == (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS
                && nodus->genus
                    < (s32)SILVA_C89_REGISTRUM.numerus_generum)
            {
                constans SilvaTabGenus* tg =
                    &SILVA_C89_REGISTRUM.genera[nodus->genus];

                per (k = ZEPHYRUM;
                     k < nodus->numerus_locorum
                     && k < tg->loci_numerus; k++)
                {
                    constans SilvaTabLocus* tl =
                        &SILVA_C89_REGISTRUM.loci[tg->loci_offset
                            + k];

                    si (strcmp(tl->titulus, "corpus") != ZEPHYRUM)
                    {
                        _seriem_addere(series, nodus->loci[k],
                            elementum);
                    }
                }
            }
            alioquin
            {
                per (k = ZEPHYRUM;
                     k < nodus->numerus_locorum; k++)
                {
                    _seriem_addere(series, nodus->loci[k],
                        elementum);
                }
            }
        }
    }
}

/* --------------------------------------------------
 * extentus: lineae fontis integrae subarboris (lexemata per
 * radicem originis ad plagulam principem reducta)
 * -------------------------------------------------- */

nomen structura {
    b32 validus;
    s32 initium;        /* offset lexematis primi (linea in
                         * impressore retrorsum quaeritur) */
    s32 finis;          /* offset post lexema ultimum */
    i32 linea_prima;
    i32 linea_ultima;
} Extentus;

hic_manens Extentus
_extentum_metiri (constans SilvaParsura* parsura,
    SilvaNodus* elementum, Piscina* piscina)
{
    Extentus ext;
    Xar* series = xar_creare(piscina, magnitudo(SilvaValor));
    i32 cursor = ZEPHYRUM;

    ext.validus = FALSUM;
    ext.initium = ZEPHYRUM;
    ext.finis = ZEPHYRUM;
    ext.linea_prima = ZEPHYRUM;
    ext.linea_ultima = ZEPHYRUM;

    si (series == NIHIL) redde ext;
    {
        SilvaValor v;
        SilvaValor* radix = (SilvaValor*)xar_addere(series);

        si (radix == NIHIL) redde ext;
        v.genus = SILVA_VALOR_NODUS;
        v.datum.nodus = elementum;
        *radix = v;
    }

    dum (cursor < xar_numerus(series))
    {
        SilvaValor v = *(SilvaValor*)xar_obtinere(series, cursor);

        cursor++;

        si (v.genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(v); k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);
                SilvaValor* novus;

                si (elem == NIHIL) perge;
                novus = (SilvaValor*)xar_addere(series);
                si (novus != NIHIL) *novus = *elem;
            }
            perge;
        }
        si (v.genus == SILVA_VALOR_TOKEN && v.datum.token != NIHIL)
        {
            SilvaToken* radix_lex = silva_token_radix(v.datum.token);

            si (radix_lex != NIHIL
                && radix_lex->fons_index == parsura->fons_princeps
                && radix_lex->byte_offset >= ZEPHYRUM)
            {
                s32 finis_lex = radix_lex->byte_offset
                    + (s32)radix_lex->longitudo;

                si (!ext.validus
                    || radix_lex->byte_offset < ext.initium)
                {
                    ext.initium = radix_lex->byte_offset;
                    ext.linea_prima = radix_lex->linea;
                }
                si (!ext.validus || finis_lex > ext.finis)
                {
                    ext.finis = finis_lex;
                    ext.linea_ultima = radix_lex->linea;
                }
                ext.validus = VERUM;
            }
            perge;
        }
        si (v.genus != SILVA_VALOR_NODUS || v.datum.nodus == NIHIL)
        {
            perge;
        }
        {
            constans SilvaNodus* nodus = v.datum.nodus;
            i32 k;

            per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
            {
                SilvaValor* novus = (SilvaValor*)xar_addere(series);

                si (novus != NIHIL) *novus = nodus->loci[k];
            }
        }
    }
    redde ext;
}

/* -------------------------------------------------- */

/* praeparatio capitum: exemplar percursus/censor (inclusio vera) */
hic_manens b32
_praetermittendum (constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

hic_manens vacuum
_caput_praebere (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via,
    constans character* titulus)
{
    FILE* pl;
    long mensura_l;
    i32 mensura;
    character* textus;
    chorda clavis;

    clavis = chorda_ex_literis(titulus, piscina);
    si (tabula_dispersa_continet(visa, clavis)) redde;

    pl = fopen(via, "rb");
    si (pl == NIHIL) redde;
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde;
    }
    mensura = (i32)mensura_l;

    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (textus == NIHIL || (mensura > ZEPHYRUM
        && fread(textus, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        redde;
    }
    fclose(pl);

    si (silva_contextus_praebere(ctx, titulus, textus, mensura))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
    }
}

hic_manens vacuum
_capita_praeparare (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL) redde;
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (_praetermittendum(introitus->d_name)) perge;
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);

        si (introitus->d_type == DT_DIR)
        {
            _capita_praeparare(ctx, piscina, visa, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                _caput_praebere(ctx, piscina, visa, via_plena,
                    introitus->d_name);
            }
        }
    }
    closedir(dir);
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina_ctx;
    Piscina* piscina;
    SilvaContextus* ctx;
    constans character* via = NIHIL;
    constans character* symbolum = NIHIL;
    FILE* pl;
    i8* fons;
    long mensura_l;
    i32 mensura;
    SilvaParsura* parsura;
    Xar* congruentia;
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            mensura_maxima = ZEPHYRUM;
        }
        alioquin si (via == NIHIL)
        {
            via = argv[k];
        }
        alioquin si (symbolum == NIHIL)
        {
            symbolum = argv[k];
        }
    }
    si (via == NIHIL || symbolum == NIHIL)
    {
        fprintf(stderr, "usus: emitte <plagula> <symbolum>  "
            "(ex radice repositorii)\n");
        redde II;
    }

    piscina_ctx = piscina_generare_dynamicum("emitte_ctx", 8388608);
    si (piscina_ctx == NIHIL)
    {
        fprintf(stderr, "emitte: piscina deest\n");
        redde II;
    }
    ctx = silva_contextus_creare(piscina_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "emitte: contextus deest\n");
        redde II;
    }
    si (mensura_maxima == ZEPHYRUM)
    {
        ctx->fines.lexemata = ZEPHYRUM;
    }
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL)
        {
            fprintf(stderr, "emitte: tabula deest\n");
            redde II;
        }
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        fprintf(stderr, "emitte: plagula non lecta: %s\n", via);
        redde II;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde II;
    }
    mensura = (i32)mensura_l;
    si (mensura_maxima > ZEPHYRUM && mensura > mensura_maxima)
    {
        fprintf(stderr, "emitte: plagula super tectum %.1f MB "
            "(-omnia tollit)\n", (duplex)mensura / 1048576.0);
        redde II;
    }

    piscina = piscina_generare_dynamicum("emitte", 8388608);
    si (piscina == NIHIL)
    {
        fclose(pl);
        redde II;
    }
    fons = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        fprintf(stderr, "emitte: plagula non lecta: %s\n", via);
        redde II;
    }
    fclose(pl);

    parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
        (constans character*)fons, mensura, NIHIL);
    si (parsura == NIHIL || !parsura->successus)
    {
        fprintf(stderr, "emitte: sine arbore: %s\n", via);
        redde II;
    }

    congruentia = xar_creare(piscina, magnitudo(SilvaNodus*));
    si (congruentia == NIHIL) redde II;
    {
        Xar* nomina = xar_creare(piscina, magnitudo(chorda));

        _congruentia_quaerere(parsura, symbolum, congruentia,
            nomina, piscina);

        si (xar_numerus(congruentia) == ZEPHYRUM)
        {
            fprintf(stderr, "emitte: symbolum non inventum: "
                "%s in %s\n", symbolum, via);
            /* fortasse: suggestiones similitudinis (desideratum
             * primum - vectis huius ipsius instrumenti eam
             * poposcit: symbolum male coniectum = circuitus grep) */
            si (nomina != NIHIL
                && xar_numerus(nomina) > ZEPHYRUM)
            {
                SimilitudoFructus fructus[V];
                i32 n = ZEPHYRUM;

                /* xar segmentatum - candidatos in aciem continuam
                 * copiare (similitudo aciem expectat) */
                i32 numerus = xar_numerus(nomina);
                chorda* acies = (chorda*)piscina_allocare(piscina,
                    (memoriae_index)numerus
                        * magnitudo(chorda));

                si (acies != NIHIL)
                {
                    chorda quaestio =
                        chorda_ex_literis(symbolum, piscina);
                    i32 c;

                    per (c = ZEPHYRUM; c < numerus; c++)
                    {
                        acies[c] = *(chorda*)xar_obtinere(nomina,
                            c);
                    }
                    /* nomen totum male coniectum subsequentia
                     * nullius est - quaestionem a dextra decurtare
                     * donec candidati appareant (quod homo in fzf
                     * delendo facit; inventum die natali similitudinis) */
                    dum (quaestio.mensura >= III)
                    {
                        n = similitudo_optima(quaestio, acies,
                            numerus, fructus, V);
                        si (n > ZEPHYRUM) frange;
                        quaestio.mensura--;
                    }
                }
                si (n > ZEPHYRUM)
                {
                    i32 f;

                    fprintf(stderr, "fortasse:\n");
                    per (f = ZEPHYRUM; f < n; f++)
                    {
                        chorda nom = acies[fructus[f].index];
                        b32 iam = FALSUM;
                        i32 g;

                        per (g = ZEPHYRUM; g < f; g++)
                        {
                            si (chorda_aequalis(nom,
                                    acies[fructus[g].index]))
                            {
                                iam = VERUM;
                                frange;
                            }
                        }
                        si (iam) perge;
                        fprintf(stderr, "  %.*s\n",
                            (int)nom.mensura,
                            (constans character*)nom.datum);
                    }
                }
            }
            redde I;
        }
    }

    {
        i32 m;

        per (m = ZEPHYRUM; m < xar_numerus(congruentia); m++)
        {
            SilvaNodus* elementum =
                *(SilvaNodus**)xar_obtinere(congruentia, m);
            Extentus ext = _extentum_metiri(parsura, elementum,
                piscina);
            s32 finis_lineae;

            si (!ext.validus) perge;

            /* initium ad initium lineae retrahere */
            dum (ext.initium > ZEPHYRUM
                && fons[ext.initium - I] != '\n')
            {
                ext.initium--;
            }
            /* finem ad finem lineae extendere (novissima inclusa) */
            finis_lineae = ext.finis;
            dum (finis_lineae < (s32)mensura
                && fons[finis_lineae] != '\n')
            {
                finis_lineae++;
            }
            si (finis_lineae < (s32)mensura) finis_lineae++;

            si (m > ZEPHYRUM) imprimere("\n");
            imprimere("[%s:%u-%u]\n", via,
                ext.linea_prima, ext.linea_ultima);
            fwrite(fons + ext.initium, I,
                (memoriae_index)(finis_lineae - ext.initium),
                stdout);
        }
    }
    redde ZEPHYRUM;
}
