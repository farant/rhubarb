/* silva_forma.c - formae: fragmenta C cum lacunis (transformare T1)
 *
 * Fragmentum -> lacunae in nomina reservata -> involucrum functionis
 * -> parsura cum lexico latino -> nodus expressionis aut sententiae.
 * Vide silva_forma.h.
 */
#include "silva_forma.h"
#include "silva_contextus.h"
#include "silva_c89_oraculum.h"
#include "silva_commissio.h"
#include "silva_tabulae_c89.h"
#include "materia_nodus.h"
#include "chorda_aedificator.h"
#include <string.h>

#define PRAEFIXUM_LACUNAE   "__trf_"
#define PRAEFIXUM_RELIQUI   "__trfv_"
#define TITULUS_INVOLUCRI   "__trf_forma"


/* ==================================================
 * Registrum: loci et genera per nomen
 * ================================================== */

constans character*
silva_nodi_genus_titulus (
    constans SilvaNodus* nodus)
{
    si (   nodus        == NIHIL || nodus->genus < ZEPHYRUM
        || nodus->genus >= (s32)SILVA_C89_REGISTRUM.numerus_generum)
    {
        redde "";
    }
    redde SILVA_C89_REGISTRUM.genera[nodus->genus].titulus;
}

constans SilvaValor*
silva_nodi_locus (
    constans SilvaNodus* nodus,
     constans character* titulus)
{
    constans MateriaTabGenus* g;
                         i32  k;

    si (   nodus == NIHIL || titulus == NIHIL || nodus->genus < ZEPHYRUM
        || nodus->genus >= (s32)SILVA_C89_REGISTRUM.numerus_generum)
    {
        redde NIHIL;
    }
    g = &SILVA_C89_REGISTRUM.genera[nodus->genus];
    per (k = ZEPHYRUM; k < g->loci_numerus
        && k < nodus->numerus_locorum;
         k++)
    {
        si (strcmp(SILVA_C89_REGISTRUM.loci[g->loci_offset + k].titulus,
                titulus) == ZEPHYRUM)
        {
            redde &nodus->loci[k];
        }
    }
    redde NIHIL;
}

/* nodus loci (NIHIL si locus absens aut non NODUS) */
interior constans SilvaNodus*
_nodus_loci (
    constans SilvaNodus* nodus,
     constans character* titulus)
{
    constans SilvaValor* v = silva_nodi_locus(nodus, titulus);

    redde (v != NIHIL && v->genus == MATERIA_VALOR_NODUS)
        ? v->datum.nodus : NIHIL;
}


/* ==================================================
 * Lacunae
 * ================================================== */

b32
silva_lacuna_est (
    constans SilvaNodus* nodus,
                 chorda* titulus_out,
                    b32* reliquum_out)
{
    constans SilvaValor* v;
                 chorda  t;
                    i32  lp = (i32)strlen(PRAEFIXUM_LACUNAE);
                    i32  lr = (i32)strlen(PRAEFIXUM_RELIQUI);

    si (strcmp(silva_nodi_genus_titulus(nodus), "folium-identificator")
        != ZEPHYRUM)
    {
        redde FALSUM;
    }
    v = silva_nodi_locus(nodus, "tok_valor");
    si (   v              == NIHIL || v->genus != MATERIA_VALOR_TOKEN
        || v->datum.token == NIHIL)
    {
        redde FALSUM;
    }
    t = v->datum.token->valor;
    si (   t.mensura > lr
        && memcmp(t.datum, PRAEFIXUM_RELIQUI, (size_t)lr) == ZEPHYRUM)
    {
        si (titulus_out != NIHIL)
        {
            *titulus_out = chorda_sectio(t, lr, t.mensura);
        }
        si (reliquum_out != NIHIL)
        {
            *reliquum_out = VERUM;
        }
        redde VERUM;
    }
    si (   t.mensura > lp
        && memcmp(t.datum, PRAEFIXUM_LACUNAE, (size_t)lp) == ZEPHYRUM)
    {
        si (titulus_out != NIHIL)
        {
            *titulus_out = chorda_sectio(t, lp, t.mensura);
        }
        si (reliquum_out != NIHIL)
        {
            *reliquum_out = FALSUM;
        }
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_initium_nominis (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

interior b32
_pars_nominis (
    character c)
{
    redde _initium_nominis(c) || (c >= '0' && c <= '9');
}

interior b32
_causam_ponere (
                chorda* causa_out,
               Piscina* piscina,
    constans character* nuntius)
{
    si (causa_out != NIHIL)
    {
        *causa_out = chorda_ex_literis(nuntius, piscina);
    }
    redde FALSUM;
}

/* octeti textus[ab, ad) in aedificatorem */
interior vacuum
_octetos_appendere (
     ChordaAedificator* a,
    constans character* textus,
                   i32  ab,
                   i32  ad)
{
    i32 k;

    per (k = ab; k < ad; k++)
    {
        (vacuum)chorda_aedificator_appendere_character(a, textus[k]);
    }
}

/* '$x' -> '__trf_x', '$...r' -> '__trfv_r' extra litteras chordarum,
 * characterum et commentaria; lacunae in tabulam (unicae). FALSUM +
 * causa si lacuna deformis aut idem nomen bis forma diversa. */
interior b32
_lacunas_reponere (
               Piscina* piscina,
    constans character* textus,
                   i32  mensura,
     ChordaAedificator* a,
                   Xar* lacunae,
                chorda* causa_out)
{
    i32 i = ZEPHYRUM;

    dum (i < mensura)
    {
        character c = textus[i];

        si (c == '"' || c == '\'')
        {
            /* littera: usque ad clausuram eandem (effugia '\') */
            i32 j = i + I;

            dum (j < mensura && textus[j] != c)
            {
                j += (textus[j] == '\\' && j + I < mensura) ? II : I;
            }
            j = (j < mensura) ? j + I : j;
            _octetos_appendere(a, textus, i, j);
            i = j;
            perge;
        }
        si (c == '/' && i + I < mensura && textus[i + I] == '*')
        {
            i32 j = i + II;

            dum (   j + I < mensura
                 && !(textus[j] == '*' && textus[j + I] == '/'))
            {
                j++;
            }
            j = (j + I < mensura) ? j + II : mensura;
            _octetos_appendere(a, textus, i, j);
            i = j;
            perge;
        }
        si (c == '$')
        {
                   b32 reliquum = FALSUM;
                   i32 initium;
                chorda titulus;
                   i32 k;
                   b32 inventa = FALSUM;

            i++;
            si (   i + II < mensura && textus[i] == '.'
                && textus[i + I] == '.' && textus[i + II] == '.')
            {
                reliquum  = VERUM;
                i         += III;
            }
            initium = i;
            si (i >= mensura || !_initium_nominis(textus[i]))
            {
                redde _causam_ponere(causa_out, piscina, reliquum
                    ? "lacuna '$...' sine nomine"
                    : "lacuna '$' sine nomine");
            }
            dum (i < mensura && _pars_nominis(textus[i]))
            {
                i++;
            }
            titulus.mensura  = i - initium;
            titulus.datum    = (i8*)piscina_allocare(piscina,
                (memoriae_index)titulus.mensura);
            si (titulus.datum == NIHIL)
            {
                redde FALSUM;
            }
            memcpy(titulus.datum, textus + initium,
                (size_t)titulus.mensura);
            per (k = ZEPHYRUM; k < xar_numerus(lacunae); k++)
            {
                SilvaLacuna* l = (SilvaLacuna*)xar_obtinere(lacunae, k);

                si (chorda_aequalis(l->titulus, titulus))
                {
                    si (l->reliquum != reliquum)
                    {
                        redde _causam_ponere(causa_out, piscina,
                            "lacuna eadem ut '$' et '$...' adhibita");
                    }
                    inventa = VERUM;
                }
            }
            si (!inventa)
            {
                SilvaLacuna* l = (SilvaLacuna*)xar_addere(lacunae);

                si (l == NIHIL)
                {
                    redde FALSUM;
                }
                l->titulus   = titulus;
                l->reliquum  = reliquum;
            }
            (vacuum)chorda_aedificator_appendere_literis(a, reliquum
                ? PRAEFIXUM_RELIQUI : PRAEFIXUM_LACUNAE);
            (vacuum)chorda_aedificator_appendere_chorda(a, titulus);
            perge;
        }
        (vacuum)chorda_aedificator_appendere_character(a, c);
        i++;
    }
    redde VERUM;
}


/* ==================================================
 * Parsura fragmenti
 * ================================================== */

/* definitio involucri inter elementa radicis (ultima): radix
 * commissionis LISTA est (directivae, declarationes latinae,
 * definitio, cauda) */
interior constans SilvaNodus*
_involucrum_invenire (
    SilvaValor radix)
{
    constans SilvaNodus* inventum = NIHIL;
                    i32  i;

    si (radix.genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < materia_valor_lista_numerus(radix); i++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(radix,
            i);

        si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
            && strcmp(silva_nodi_genus_titulus(e->datum.nodus),
                   "definitio-functionis") == ZEPHYRUM)
        {
            inventum = e->datum.nodus;
        }
    }
    redde inventum;
}

SilvaForma*
silva_formam_parsare (
                Piscina* piscina,
     constans character* textus,
                    i32  mensura,
        SilvaFormaGenus  genus,
                 chorda* causa_out)
{
       ChordaAedificator* a;
                     Xar* lacunae;
          SilvaContextus* ctx;
            SilvaParsura* parsura;
                  chorda  involutum;
     constans SilvaNodus* definitio;
     constans SilvaNodus* corpus;
     constans SilvaValor* elementa;
     constans SilvaNodus* sententia = NIHIL;
                     i32  i;
                     i32  numerus = ZEPHYRUM;
              SilvaForma* forma;

    si (textus == NIHIL || mensura <= ZEPHYRUM)
    {
        _causam_ponere(causa_out, piscina, "forma vacua");
        redde NIHIL;
    }
    a = chorda_aedificator_creare(piscina,
        (memoriae_index)CCLVI);
    lacunae = xar_creare(piscina, (i32)magnitudo(SilvaLacuna));
    si (a == NIHIL || lacunae == NIHIL)
    {
        redde NIHIL;
    }
    (vacuum)chorda_aedificator_appendere_literis(a,
        "vacuum " TITULUS_INVOLUCRI "(vacuum)\n{\n");
    si (!_lacunas_reponere(piscina, textus, mensura, a, lacunae,
            causa_out))
    {
        redde NIHIL;
    }
    (vacuum)chorda_aedificator_appendere_literis(a,
        genus == SILVA_FORMA_EXPRESSIO ? ";\n}\n" : "\n}\n");
    involutum  = chorda_aedificator_finire(a);
    ctx        = silva_contextus_creare(piscina);
    si (ctx == NIHIL || !silva_contextus_latinam_addere(ctx))
    {
        _causam_ponere(causa_out, piscina, "contextus latinus deest");
        redde NIHIL;
    }
    parsura = silva_c89_parsare_cum_contextu(piscina, ctx, "<forma>",
        (constans character*)involutum.datum, involutum.mensura, NIHIL);
    si (   parsura == NIHIL || parsura->commissio == NIHIL
        || parsura->numerus_errorum > ZEPHYRUM)
    {
        _causam_ponere(causa_out, piscina,
            "forma non parsatur ut C (errores in fragmento)");
        redde NIHIL;
    }
    definitio  = _involucrum_invenire(parsura->commissio->radix);
    corpus     = _nodus_loci(definitio, "corpus");
    elementa   = silva_nodi_locus(corpus, "elementa");
    si (elementa == NIHIL || elementa->genus != MATERIA_VALOR_LISTA)
    {
        _causam_ponere(causa_out, piscina,
            "forma: corpus involucri deest");
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < materia_valor_lista_numerus(*elementa); i++)
    {
        constans MateriaValor* e =
            materia_valor_lista_obtinere(*elementa,
            i);

        /* sententia vacua (';' involucri post expressionem non
         * expressionem) non numeratur */
        si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
            && strcmp(silva_nodi_genus_titulus(e->datum.nodus),
                   "sententia-vacua") != ZEPHYRUM)
        {
            sententia = e->datum.nodus;
            numerus++;
        }
    }
    si (numerus != I)
    {
        _causam_ponere(causa_out, piscina, numerus == ZEPHYRUM
            ? "forma vacua (nulla sententia)"
            : "forma plures sententias habet - una sola licet");
        redde NIHIL;
    }
    forma = (SilvaForma*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaForma));
    si (forma == NIHIL)
    {
        redde NIHIL;
    }
    forma->genus    = genus;
    forma->parsura  = parsura;
    forma->lacunae  = lacunae;
    forma->nodus    = sententia;
    si (genus == SILVA_FORMA_EXPRESSIO)
    {
        si (strcmp(silva_nodi_genus_titulus(sententia),
                "sententia-expressionis") != ZEPHYRUM)
        {
            _causam_ponere(causa_out, piscina,
                "forma EXPRESSIO non est expressio");
            redde NIHIL;
        }
        forma->nodus = _nodus_loci(sententia, "expressio");
        si (forma->nodus == NIHIL)
        {
            _causam_ponere(causa_out, piscina,
                "forma EXPRESSIO sine expressione");
            redde NIHIL;
        }
    }
    redde forma;
}
