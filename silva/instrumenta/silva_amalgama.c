/* silva_amalgama.c - Implementatio machinae amalgamationis
 *
 * Extracta ex principalia/amalgamator.c (Phase 7 Chunk C tesserae):
 * mechanismus solus - manifesta in plagulis principalibus vivunt.
 * Lexatore silvae tokenizat (bootstrap dev-time). PORTA extractionis:
 * silva.c byte-identicum ante et post.
 */
#include "silva_amalgama.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include <stdio.h>
#include <string.h>

#define VIA_MAXIMA 1024

/* ==================================================
 * Status amalgamatoris
 * ================================================== */

nomen structura {
    Piscina*            piscina;
    ChordaAedificator*  aed;
    constans character* radix;
    Xar*                systematis;  /* chorda - includenda <...> */
    TabulaDispersa*     statica;     /* titulus -> character* (via) */
    b32                 successus;
    constans AmalgamaManifestum* manifestum;
} Amalgamator;

/* ==================================================
 * Auxiliares
 * ================================================== */

interior i8*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl;
    i8* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);

    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_out = (i32)mensura;
    redde buffer;
}

interior character*
_literis_figere (Piscina* piscina, constans character* literis)
{
    memoriae_index m = strlen(literis) + I;
    character* fixum = (character*)piscina_allocare(piscina, m);

    si (fixum != NIHIL)
    {
        memcpy(fixum, literis, m);
    }
    redde fixum;
}

interior b32
_chorda_est (chorda c, constans character* literis)
{
    si (c.mensura != (i32)strlen(literis))
    {
        redde FALSUM;
    }
    si (c.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(c.datum, literis, (memoriae_index)c.mensura)
        == ZEPHYRUM) ? VERUM : FALSUM;
}

interior SilvaToken*
_ad (Xar* lexemata, i32 i)
{
    redde *(SilvaToken**)xar_obtinere(lexemata, i);
}

interior vacuum
_trivia_emittere (Amalgamator* am, Xar* spatia)
{
    i32 i;

    si (spatia == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(spatia); i++)
    {
        chorda_aedificator_appendere_chorda(am->aed,
            (*(SilvaToken**)xar_obtinere(spatia, i))->valor);
    }
}

/* Emittere lexema, renominatum si tabulae congruunt.
 * NB scissurae hic ignorantur (valor mundus emittitur) - laminae
 * intra lexemata in bibliothecis vendicatis non exsistunt, et textus
 * mundus semper compilat. */
interior vacuum
_lexema_emittere (Amalgamator* am, SilvaToken* t, b32 sine_ante)
{
    i32 k;

    si (!sine_ante)
    {
        _trivia_emittere(am, t->spatia_ante);
    }

    si (t->genus == SILVA_LEX_IDENTIFICATOR)
    {
        per (k = ZEPHYRUM; k < am->manifestum->numerus_typorum; k++)
        {
            si (_chorda_est(t->valor,
                    am->manifestum->typi_exacti[k].vetus))
            {
                chorda_aedificator_appendere_literis(am->aed,
                    am->manifestum->typi_exacti[k].novum);
                _trivia_emittere(am, t->spatia_post);
                redde;
            }
        }
        per (k = ZEPHYRUM; k < am->manifestum->numerus_praefixorum;
             k++)
        {
            i32 m = (i32)strlen(
                am->manifestum->praefixa_functionum[k].vetus);

            si (t->valor.mensura > m
                && memcmp(t->valor.datum,
                       am->manifestum->praefixa_functionum[k].vetus,
                       (memoriae_index)m) == ZEPHYRUM)
            {
                chorda cauda;

                chorda_aedificator_appendere_literis(am->aed,
                    am->manifestum->praefixa_functionum[k].novum);
                cauda.datum = t->valor.datum + m;
                cauda.mensura = t->valor.mensura - m;
                chorda_aedificator_appendere_chorda(am->aed, cauda);
                _trivia_emittere(am, t->spatia_post);
                redde;
            }
        }
    }

    chorda_aedificator_appendere_chorda(am->aed, t->valor);
    _trivia_emittere(am, t->spatia_post);
}

/* ==================================================
 * Scansio unitatum
 * ================================================== */

interior i32
_lineam_finire (Xar* lexemata, i32 i, i32 n)
{
    i32 j = i + I;

    dum (j < n)
    {
        SilvaToken* t = _ad(lexemata, j);

        si (t->initium_lineae || t->genus == SILVA_LEX_EOF)
        {
            frange;
        }
        j++;
    }
    redde j;
}

/* Unitas suprema: [i, finis). titulus_out = identificator ante '('
 * primum (functio) aut ante '='/'[' aut ultimus ante ';' (datum,
 * typus) aut post structura/unio/enumeratio (definitio tag) aut in
 * '(*IDENT)' (typedef indicis functionis). est_definitio_out:
 * corpus functionis {...} habet. */
interior i32
_unitatem_finire (Xar* lexemata, i32 i, i32 n, chorda* titulus_out,
    b32* est_functio_out, b32* est_statica_out, b32* est_definitio_out,
    b32* est_typedef_out, b32* est_tag_def_out)
{
    i32 pp = ZEPHYRUM;
    i32 pb = ZEPHYRUM;
    b32 vidit_clausam = FALSUM;
    b32 est_typedef = FALSUM;
    b32 titulus_fixus = FALSUM;
    chorda ultimus;
    SilvaLexemaGenus prius = SILVA_LEX_EOF;
    SilvaLexemaGenus prius_prius = SILVA_LEX_EOF;
    i32 j;

    titulus_out->datum = NIHIL;
    titulus_out->mensura = ZEPHYRUM;
    ultimus.datum = NIHIL;
    ultimus.mensura = ZEPHYRUM;
    *est_functio_out = FALSUM;
    *est_statica_out = FALSUM;
    *est_definitio_out = FALSUM;
    *est_tag_def_out = FALSUM;

    {
        SilvaToken* primum = _ad(lexemata, i);

        si (primum->genus == SILVA_LEX_STATIC)
        {
            *est_statica_out = VERUM;
        }
        alioquin si (primum->genus == SILVA_LEX_IDENTIFICATOR
            && (_chorda_est(primum->valor, "interior")
                || _chorda_est(primum->valor, "hic_manens")
                || _chorda_est(primum->valor, "staticus")
                || _chorda_est(primum->valor, "universalis")))
        {
            *est_statica_out = VERUM;
        }
        si (primum->genus == SILVA_LEX_TYPEDEF
            || (primum->genus == SILVA_LEX_IDENTIFICATOR
                && _chorda_est(primum->valor, "nomen")))
        {
            est_typedef = VERUM;
        }

        /* Definitio tag (structura X {...};) - titulus = tag ipse */
        si (!est_typedef
            && (primum->genus == SILVA_LEX_STRUCT
                || primum->genus == SILVA_LEX_UNION
                || primum->genus == SILVA_LEX_ENUM
                || (primum->genus == SILVA_LEX_IDENTIFICATOR
                    && (_chorda_est(primum->valor, "structura")
                        || _chorda_est(primum->valor, "unio")
                        || _chorda_est(primum->valor, "enumeratio"))))
            && i + I < n)
        {
            SilvaToken* secundum = _ad(lexemata, i + I);

            si (secundum->genus == SILVA_LEX_IDENTIFICATOR)
            {
                *titulus_out = secundum->valor;
                titulus_fixus = VERUM;
                *est_tag_def_out = VERUM;
            }
        }
    }
    *est_typedef_out = est_typedef;

    per (j = i; j < n; j++)
    {
        SilvaToken* t = _ad(lexemata, j);

        commutatio (t->genus)
        {
            casus SILVA_LEX_EOF:
                redde j;
            casus SILVA_LEX_PAREN_APERTA:
                si (pp == ZEPHYRUM && pb == ZEPHYRUM && !est_typedef
                    && !titulus_fixus && ultimus.datum != NIHIL)
                {
                    *titulus_out = ultimus;
                    *est_functio_out = VERUM;
                    titulus_fixus = VERUM;
                }
                pp++;
                frange;
            casus SILVA_LEX_PAREN_CLAUSA:
                pp--;
                si (pp == ZEPHYRUM && pb == ZEPHYRUM)
                {
                    vidit_clausam = VERUM;
                }
                frange;
            casus SILVA_LEX_BRACE_APERTA:
                si (pb == ZEPHYRUM && pp == ZEPHYRUM && vidit_clausam
                    && *est_functio_out)
                {
                    *est_definitio_out = VERUM;
                }
                pb++;
                frange;
            casus SILVA_LEX_BRACE_CLAUSA:
                pb--;
                si (pb == ZEPHYRUM && *est_definitio_out)
                {
                    redde j + I;
                }
                frange;
            casus SILVA_LEX_SEMICOLON:
                si (pp == ZEPHYRUM && pb == ZEPHYRUM)
                {
                    si (titulus_out->datum == NIHIL)
                    {
                        *titulus_out = ultimus;
                    }
                    redde j + I;
                }
                frange;
            casus SILVA_LEX_ASSIGNATIO:
            casus SILVA_LEX_QUADRA_APERTA:
                si (pp == ZEPHYRUM && pb == ZEPHYRUM && !titulus_fixus
                    && ultimus.datum != NIHIL)
                {
                    *titulus_out = ultimus;
                    titulus_fixus = VERUM;
                }
                frange;
            casus SILVA_LEX_IDENTIFICATOR:
                /* typedef indicis functionis: nomen ... (*IDENT)(...) */
                si (est_typedef && !titulus_fixus
                    && prius == SILVA_LEX_STAR
                    && prius_prius == SILVA_LEX_PAREN_APERTA)
                {
                    *titulus_out = t->valor;
                    titulus_fixus = VERUM;
                }
                ultimus = t->valor;
                frange;
            ordinarius:
                frange;
        }
        prius_prius = prius;
        prius = t->genus;
    }
    redde n;
}

interior b32
_in_indice (chorda titulus, constans character* constans* index)
{
    i32 k;

    si (titulus.datum == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; index[k] != NIHIL; k++)
    {
        si (_chorda_est(titulus, index[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_in_servandis (chorda titulus,
    constans character* constans* servanda)
{
    i32 k;

    si (servanda == NIHIL)
    {
        redde VERUM;
    }
    si (titulus.datum == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; servanda[k] != NIHIL; k++)
    {
        si (_chorda_est(titulus, servanda[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Processio plagulae
 * ================================================== */

interior vacuum
_includendum_systematis_subferre (Amalgamator* am, Xar* lexemata,
    i32 i, i32 lf)
{
    ChordaAedificator* titulus_aed;
    chorda titulus;
    i32 j;
    b32 novum = VERUM;

    si (i + II >= lf
        || _ad(lexemata, i + II)->genus != SILVA_LEX_MINOR)
    {
        redde;  /* "proiectum.h" - cadit sine sublatione */
    }
    titulus_aed = chorda_aedificator_creare(am->piscina, XXXII);
    per (j = i + III; j < lf; j++)
    {
        SilvaToken* u = _ad(lexemata, j);

        si (u->genus == SILVA_LEX_MAIOR)
        {
            frange;
        }
        chorda_aedificator_appendere_chorda(titulus_aed, u->valor);
    }
    titulus = chorda_aedificator_finire(titulus_aed);
    per (j = ZEPHYRUM; j < xar_numerus(am->systematis); j++)
    {
        chorda* c = (chorda*)xar_obtinere(am->systematis, j);

        si (c->mensura == titulus.mensura
            && (titulus.mensura == ZEPHYRUM
                || memcmp(c->datum, titulus.datum,
                       (memoriae_index)c->mensura) == ZEPHYRUM))
        {
            novum = FALSUM;
            frange;
        }
    }
    si (novum)
    {
        chorda* locus = (chorda*)xar_addere(am->systematis);

        si (locus != NIHIL)
        {
            *locus = titulus;
        }
    }
}

interior vacuum
_plagulam_processare (Amalgamator* am, constans AmalgamaPlagula* pl)
{
    character via[VIA_MAXIMA];
    character* via_fixa;
    i8* fons;
    i32 mensura;
    Xar* lexemata;
    i32 n;
    i32 i;

    sprintf(via, "%s/%s", am->radix, pl->via);
    fons = _plagulam_legere(am->piscina, via, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "amalgamator: plagula non lecta: %s\n", via);
        am->successus = FALSUM;
        redde;
    }
    via_fixa = _literis_figere(am->piscina, pl->via);

    chorda_aedificator_appendere_literis(am->aed,
        "\n/* ================= ex ");
    chorda_aedificator_appendere_literis(am->aed, pl->via);
    chorda_aedificator_appendere_literis(am->aed,
        " ================= */\n");

    lexemata = silva_lexare(am->piscina, (constans character*)fons,
        mensura, ZEPHYRUM);
    n = xar_numerus(lexemata);
    i = ZEPHYRUM;

    dum (i < n)
    {
        SilvaToken* t = _ad(lexemata, i);

        si (t->genus == SILVA_LEX_EOF)
        {
            _trivia_emittere(am, t->spatia_ante);
            frange;
        }

        /* Directivae: includenda systematis sublata, proiecti remota;
         * ceterae (custodiae, defines) verbatim renominatae */
        si (t->genus == SILVA_LEX_CANCELLUM && t->initium_lineae)
        {
            i32 lf = _lineam_finire(lexemata, i, n);
            b32 est_includendum = FALSUM;

            si (i + I < lf)
            {
                SilvaToken* nomen_dir = _ad(lexemata, i + I);

                si (nomen_dir->genus == SILVA_LEX_IDENTIFICATOR
                    && _chorda_est(nomen_dir->valor, "include"))
                {
                    est_includendum = VERUM;
                }
            }
            si (est_includendum)
            {
                _includendum_systematis_subferre(am, lexemata, i, lf);
                i = lf;
                perge;
            }
            {
                i32 j;

                per (j = i; j < lf; j++)
                {
                    _lexema_emittere(am, _ad(lexemata, j), FALSUM);
                }
            }
            i = lf;
            perge;
        }

        /* Unitas suprema */
        {
            chorda titulus;
            b32 est_functio;
            b32 est_statica;
            b32 est_definitio;
            b32 est_typedef;
            b32 est_tag_def;
            b32 inicere;
            i32 finis = _unitatem_finire(lexemata, i, n, &titulus,
                &est_functio, &est_statica, &est_definitio,
                &est_typedef, &est_tag_def);
            i32 j;

            si (!_in_servandis(titulus, pl->servanda))
            {
                i = finis;  /* unitas exclusa cadit (S44) */
                perge;
            }
            si (pl->excludenda != NIHIL
                && _in_indice(titulus, pl->excludenda))
            {
                i = finis;  /* inusitata demissa (S44 finis) */
                perge;
            }

            /* CADENDA: typi quos silva.h possidet cadunt ex capitibus
             * (definitio una per TU). Typedef unitates per titulum;
             * definitiones tag solum si silva.h structuram PLENE
             * possidet. */
            si (!pl->est_corpus
                && ((est_typedef
                        && _in_indice(titulus,
                               am->manifestum->cadenda_typedef))
                    || (est_tag_def
                        && _in_indice(titulus,
                               am->manifestum->cadenda_definitio))))
            {
                i = finis;
                perge;
            }

            /* S41: definitiones staticae duplicatae trans corpora =
             * error in UNO TU (vendicatae iniectae + interiores
             * silvae aeque examinantur) */
            si (pl->est_corpus && est_functio && est_definitio
                && titulus.datum != NIHIL
                && (est_statica || pl->est_vendicata))
            {
                vacuum* valor_prior = NIHIL;

                si (tabula_dispersa_invenire(am->statica, titulus,
                        &valor_prior))
                {
                    fprintf(stderr, "amalgamator: definitio duplicata "
                        "'%.*s' (%s et %s)\n",
                        (int)titulus.mensura,
                        (constans character*)titulus.datum,
                        (character*)valor_prior, pl->via);
                    am->successus = FALSUM;
                }
                alioquin
                {
                    chorda clavis;
                    i8* datum_fixum = (i8*)piscina_allocare(
                        am->piscina,
                        (memoriae_index)(titulus.mensura > ZEPHYRUM
                            ? titulus.mensura : I));

                    si (datum_fixum != NIHIL
                        && titulus.mensura > ZEPHYRUM)
                    {
                        memcpy(datum_fixum, titulus.datum,
                            (memoriae_index)titulus.mensura);
                    }
                    clavis.datum = datum_fixum;
                    clavis.mensura = titulus.mensura;
                    tabula_dispersa_inserere(am->statica, clavis,
                        (vacuum*)via_fixa);
                }
            }

            /* static inicere in functiones vendicatas nondum staticas
             * (S44); publicae vendicatae (NON_STATICA) et functiones
             * silvae (API - extern) intactae */
            inicere = (pl->est_vendicata && est_functio && !est_statica
                && !_in_indice(titulus,
                       am->manifestum->non_statica)) ? VERUM : FALSUM;
            si (inicere)
            {
                SilvaToken* primum = _ad(lexemata, i);

                _trivia_emittere(am, primum->spatia_ante);
                chorda_aedificator_appendere_literis(am->aed,
                    "static ");
                _lexema_emittere(am, primum, VERUM);
                per (j = i + I; j < finis; j++)
                {
                    _lexema_emittere(am, _ad(lexemata, j), FALSUM);
                }
            }
            alioquin
            {
                per (j = i; j < finis; j++)
                {
                    _lexema_emittere(am, _ad(lexemata, j), FALSUM);
                }
            }
            i = finis;
        }
    }
}
/* ==================================================
 * Datum latinum (optionale - silva): plagulae datum ex
 * include/latina.h emittuntur (series octetorum - limes 509
 * characterum C89). Viae/custos/tituli ex manifesto; contentum
 * byte-identicum emissioni pristinae.
 * ================================================== */

interior constans character*
_basis_viae (constans character* via)
{
    constans character* solidus = strrchr(via, '/');

    redde (solidus != NIHIL) ? (solidus + I) : via;
}

interior b32
_latina_datum_emittere (
    constans character*          radix,
    constans i8*                 textus,
    i32                          mensura,
    constans AmalgamaManifestum* m)
{
    character via[VIA_MAXIMA];
    FILE* pl;
    i32 k;

    sprintf(via, "%s/%s", radix, m->latina_datum_via_capitis);
    pl = fopen(via, "wb");
    si (pl == NIHIL)
    {
        fprintf(stderr, "amalgamator: %s non apertum\n", via);
        redde FALSUM;
    }
    fprintf(pl, "%s", m->latina_datum_prooemium);
    fprintf(pl,
        "\n"
        "#ifndef %s\n"
        "#define %s\n"
        "\n"
        "#include \"latina.h\"\n"
        "\n"
        "externus constans character %s[];\n"
        "externus constans i32       %s;\n"
        "\n"
        "#endif /* %s */\n",
        m->latina_datum_custos, m->latina_datum_custos,
        m->latina_datum_titulus_textus,
        m->latina_datum_titulus_mensurae,
        m->latina_datum_custos);
    fclose(pl);

    sprintf(via, "%s/%s", radix, m->latina_datum_via_corporis);
    pl = fopen(via, "wb");
    si (pl == NIHIL)
    {
        fprintf(stderr, "amalgamator: %s non apertum\n", via);
        redde FALSUM;
    }
    fprintf(pl,
        "/* %s - GENERATUM ex include/latina.h - NE MANU MUTES */\n"
        "\n"
        "#include \"%s\"\n"
        "\n"
        "constans character %s[] = {\n",
        _basis_viae(m->latina_datum_via_corporis),
        _basis_viae(m->latina_datum_via_capitis),
        m->latina_datum_titulus_textus);
    per (k = ZEPHYRUM; k < mensura; k += XII)
    {
        i32 finis = (k + XII < mensura) ? (k + XII) : mensura;
        i32 j;

        fprintf(pl, "    ");
        per (j = k; j < finis; j++)
        {
            fprintf(pl, (j == k) ? "%d" : ", %d", (int)textus[j]);
        }
        fprintf(pl, (finis < mensura) ? ",\n" : "\n");
    }
    fprintf(pl,
        "};\n"
        "\n"
        "constans i32 %s = %d;\n",
        m->latina_datum_titulus_mensurae, (int)mensura);
    fclose(pl);
    redde VERUM;
}


/* ==================================================
 * Currere - fluxus totus (olim principale silvae)
 * ================================================== */

b32
silva_amalgama_currere (
    Piscina*                     piscina,
    constans character*          radix,
    constans character*          via_exitus,
    constans AmalgamaManifestum* manifestum)
{
    Amalgamator am;
    character via_latinae[VIA_MAXIMA];
    i8* latina;
    i32 mensura_latinae;
    chorda corpus;
    i32 k;

    si (piscina == NIHIL || radix == NIHIL || via_exitus == NIHIL
        || manifestum == NIHIL)
    {
        redde FALSUM;
    }
    am.piscina = piscina;
    am.aed = chorda_aedificator_creare(piscina, 262144);
    am.radix = radix;
    am.systematis = xar_creare(piscina, magnitudo(chorda));
    am.statica = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    am.successus = VERUM;
    am.manifestum = manifestum;

    /* latina.h verbatim (numquam renominata - S43) */
    sprintf(via_latinae, "%s/include/latina.h", radix);
    latina = _plagulam_legere(piscina, via_latinae, &mensura_latinae);
    si (latina == NIHIL)
    {
        fprintf(stderr, "amalgamator: latina.h non lecta\n");
        redde FALSUM;
    }

    /* Passus 0: datum latinum (si manifestum id petit) */
    si (manifestum->latina_datum_via_capitis != NIHIL)
    {
        si (!_latina_datum_emittere(radix, latina, mensura_latinae,
                manifestum))
        {
            redde FALSUM;
        }
    }

    /* Corpus primum aedificatur (includenda systematis inter
     * processionem colliguntur), prooemium postea praeponitur */
    per (k = ZEPHYRUM; k < manifestum->numerus_capitum_vendicatorum;
         k++)
    {
        _plagulam_processare(&am, &manifestum->capita_vendicata[k]);
    }

    /* Assertiones derivae (manifesti; NIHIL licet) */
    si (manifestum->assertiones != NIHIL)
    {
        chorda_aedificator_appendere_literis(am.aed,
            manifestum->assertiones);
    }

    per (k = ZEPHYRUM; k < manifestum->numerus_capitum_propriorum;
         k++)
    {
        _plagulam_processare(&am, &manifestum->capita_propria[k]);
    }
    per (k = ZEPHYRUM;
         k < manifestum->numerus_corporum_vendicatorum; k++)
    {
        _plagulam_processare(&am, &manifestum->corpora_vendicata[k]);
    }
    per (k = ZEPHYRUM; k < manifestum->numerus_corporum_propriorum;
         k++)
    {
        _plagulam_processare(&am, &manifestum->corpora_propria[k]);
    }
    corpus = chorda_aedificator_finire(am.aed);

    si (!am.successus)
    {
        redde FALSUM;
    }

    {
        ChordaAedificator* totum =
            chorda_aedificator_creare(piscina, 262144);
        chorda latina_chorda;

        chorda_aedificator_appendere_literis(totum,
            manifestum->prooemium);
        per (k = ZEPHYRUM; k < xar_numerus(am.systematis); k++)
        {
            chorda* c = (chorda*)xar_obtinere(am.systematis, k);

            chorda_aedificator_appendere_literis(totum, "#include <");
            chorda_aedificator_appendere_chorda(totum, *c);
            chorda_aedificator_appendere_literis(totum, ">\n");
        }
        /* caput manu scriptum VERBATIM (vanilla C89 - S43) */
        {
            character via_capitis[VIA_MAXIMA];
            i8* caput;
            i32 mensura_capitis;
            chorda caput_chorda;

            sprintf(via_capitis, "%s/%s", radix,
                manifestum->via_capitis);
            caput = _plagulam_legere(piscina, via_capitis,
                &mensura_capitis);
            si (caput == NIHIL)
            {
                fprintf(stderr,
                    "amalgamator: caput non lectum: %s\n",
                    via_capitis);
                redde FALSUM;
            }
            chorda_aedificator_appendere_literis(totum,
                "\n/* ================= ");
            chorda_aedificator_appendere_literis(totum,
                _basis_viae(manifestum->via_capitis));
            chorda_aedificator_appendere_literis(totum,
                " (verbatim) ================= */\n");
            caput_chorda.datum = caput;
            caput_chorda.mensura = mensura_capitis;
            chorda_aedificator_appendere_chorda(totum, caput_chorda);
        }
        chorda_aedificator_appendere_literis(totum,
            "\n/* ================= ex include/latina.h "
            "================= */\n");
        latina_chorda.datum = latina;
        latina_chorda.mensura = mensura_latinae;
        chorda_aedificator_appendere_chorda(totum, latina_chorda);
        chorda_aedificator_appendere_chorda(totum, corpus);

        {
            chorda exitus = chorda_aedificator_finire(totum);
            FILE* pl = fopen(via_exitus, "wb");

            si (pl == NIHIL)
            {
                fprintf(stderr,
                    "amalgamator: exitus non apertus: %s\n",
                    via_exitus);
                redde FALSUM;
            }
            si (exitus.mensura > ZEPHYRUM
                && fwrite(exitus.datum, I,
                       (memoriae_index)exitus.mensura, pl)
                    != (memoriae_index)exitus.mensura)
            {
                fprintf(stderr, "amalgamator: scriptio fracta\n");
                fclose(pl);
                redde FALSUM;
            }
            fclose(pl);
            imprimere("amalgamator: %s scriptum (%d octeti, "
                "%d includenda systematis)\n",
                via_exitus, (int)exitus.mensura,
                (int)xar_numerus(am.systematis));
        }
    }
    redde VERUM;
}
