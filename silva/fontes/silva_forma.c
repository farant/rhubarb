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
#include "silva_token.h"
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
    /* EXPRESSIO intra 'redde ...;': 'f(x);' nudum ambiguum est
     * (declaratio 'x' generis 'f' sine prototypo) - valor reditus
     * expressio sola esse potest */
    (vacuum)chorda_aedificator_appendere_literis(a,
        genus == SILVA_FORMA_EXPRESSIO
        ? "vacuum " TITULUS_INVOLUCRI "(vacuum)\n{\n    redde "
        : "vacuum " TITULUS_INVOLUCRI "(vacuum)\n{\n");
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
        _causam_ponere(causa_out, piscina, genus
            == SILVA_FORMA_EXPRESSIO
            ? "forma non parsatur ut expressio C (errores)"
            : "forma non parsatur ut sententia C (errores)");
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

        si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
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
        si (strcmp(silva_nodi_genus_titulus(sententia), "redde")
            != ZEPHYRUM)
        {
            _causam_ponere(causa_out, piscina,
                "forma EXPRESSIO non est expressio");
            redde NIHIL;
        }
        forma->nodus = _nodus_loci(sententia, "valor");
        si (forma->nodus == NIHIL)
        {
            _causam_ponere(causa_out, piscina,
                "forma EXPRESSIO sine expressione");
            redde NIHIL;
        }
    }
    redde forma;
}


/* ==================================================
 * Congruentia (T2)
 * ================================================== */

/* forma_modus VERUM: lacunae formae agnoscuntur (congruentia);
 * FALSUM: aequalitas pura, lacunae ut identificatores ordinarii */
interior b32
_valores_congruunt (
                  Piscina* piscina,
    constans MateriaValor* forma,
    constans MateriaValor* fons,
                      Xar* capturae,
                      b32  forma_modus);

/* captura nominis (NIHIL si nondum) */
interior SilvaCaptura*
_captura (
       Xar* capturae,
    chorda  titulus)
{
    i32 k;

    per (k = ZEPHYRUM; capturae != NIHIL && k < xar_numerus(capturae);
         k++)
    {
        SilvaCaptura* c = (SilvaCaptura*)xar_obtinere(capturae, k);

        si (chorda_aequalis(c->titulus, titulus))
        {
            redde c;
        }
    }
    redde NIHIL;
}

/* lacuna simplex capit nodum fontis, aut iterata aequalitatem poscit */
interior b32
_lacunam_implere (
                    Xar* capturae,
                 chorda  titulus,
    constans SilvaNodus* nodus)
{
    SilvaCaptura* c;

    si (capturae == NIHIL)
    {
        redde VERUM;
    }
    c = _captura(capturae, titulus);
    si (c != NIHIL)
    {
        redde !c->reliquum && silva_nodi_aequales(c->nodus, nodus);
    }
    c = (SilvaCaptura*)xar_addere(capturae);
    si (c == NIHIL)
    {
        redde FALSUM;
    }
    c->titulus   = titulus;
    c->reliquum  = FALSUM;
    c->nodus     = nodus;
    c->elementa  = NIHIL;
    redde VERUM;
}

/* reliquum: elementa fontis [ab, finis) */
interior b32
_reliquum_implere (
                  Piscina* piscina,
                      Xar* capturae,
                   chorda  titulus,
    constans MateriaValor* fons,
                      i32  ab)
{
    SilvaCaptura* c;
             i32  j;

    si (capturae == NIHIL)
    {
        redde VERUM;
    }
    c = (SilvaCaptura*)xar_addere(capturae);
    si (c == NIHIL)
    {
        redde FALSUM;
    }
    c->titulus   = titulus;
    c->reliquum  = VERUM;
    c->nodus     = NIHIL;
    c->elementa  = xar_creare(piscina, (i32)magnitudo(MateriaValor));
    per (j = ab; c->elementa != NIHIL
         && j < materia_valor_lista_numerus(*fons); j++)
    {
        *(MateriaValor*)xar_addere(c->elementa) =
            *materia_valor_lista_obtinere(*fons, j);
    }
    redde c->elementa != NIHIL;
}

/* elementum listae formae lacuna reliqui est? */
interior b32
_est_reliquum (
    constans MateriaValor* v,
                   chorda* titulus)
{
    b32 reliquum = FALSUM;

    redde v != NIHIL && v->genus == MATERIA_VALOR_NODUS
        && silva_lacuna_est(v->datum.nodus, titulus, &reliquum)
        && reliquum;
}

interior b32
_listae_congruunt (
                  Piscina* piscina,
    constans MateriaValor* forma,
    constans MateriaValor* fons,
                      Xar* capturae,
                      b32  forma_modus)
{
    i32 nf = materia_valor_lista_numerus(*forma);
    i32 ns = materia_valor_lista_numerus(*fons);
    i32 i;
    i32 j = ZEPHYRUM;

    per (i = ZEPHYRUM; i < nf; i++)
    {
        constans MateriaValor* p = materia_valor_lista_obtinere(*forma,
            i);
                       chorda titulus;

        /* '$...r' ULTIMUM: reliquum capit (nullum licet) */
        si (forma_modus && _est_reliquum(p, &titulus))
        {
            redde i + I == nf
                && _reliquum_implere(piscina, capturae, titulus, fons,
                j);
        }
        /* comma ante '$...r' ultimum, fons exhaustus: ambo vacua */
        si (   forma_modus && j == ns && i + II == nf
            && p->genus == MATERIA_VALOR_TOKEN
            && _est_reliquum(materia_valor_lista_obtinere(*forma, i
                + I),
                   &titulus))
        {
            redde _reliquum_implere(piscina, capturae, titulus, fons,
                j);
        }
        si (   j >= ns
            || !_valores_congruunt(piscina, p,
                   materia_valor_lista_obtinere(*fons, j), capturae,
                   forma_modus))
        {
            redde FALSUM;
        }
        j++;
    }
    redde j == ns;
}

interior b32
_valores_congruunt (
                  Piscina* piscina,
    constans MateriaValor* forma,
    constans MateriaValor* fons,
                      Xar* capturae,
                      b32  forma_modus)
{
    si (forma == NIHIL || fons == NIHIL)
    {
        redde forma == fons;
    }
    si (   forma_modus && forma->genus == MATERIA_VALOR_NODUS
        && forma->datum.nodus != NIHIL)
    {
        chorda titulus;
           b32 reliquum = FALSUM;

        si (silva_lacuna_est(forma->datum.nodus, &titulus, &reliquum))
        {
            /* '$...' extra listam nihil significat */
            redde !reliquum && fons->genus == MATERIA_VALOR_NODUS
                && fons->datum.nodus != NIHIL
                && _lacunam_implere(capturae, titulus,
                fons->datum.nodus);
        }
    }
    si (forma->genus != fons->genus)
    {
        redde FALSUM;
    }
    commutatio (forma->genus)
    {
        casus MATERIA_VALOR_NIHIL:
            redde VERUM;
        casus MATERIA_VALOR_TOKEN:
            redde forma->datum.token != NIHIL
                && fons->datum.token != NIHIL
                && chorda_aequalis(forma->datum.token->valor,
                       fons->datum.token->valor);
        casus MATERIA_VALOR_INDEX:
            redde forma->datum.index == fons->datum.index;
        casus MATERIA_VALOR_REFERENTIA:
            /* nexus lateralis (identificator -> declaratio): non pars
             * formae */
            redde VERUM;
        casus MATERIA_VALOR_LISTA:
            redde _listae_congruunt(piscina, forma, fons, capturae,
                forma_modus);
        casus MATERIA_VALOR_NODUS:
        {
            constans SilvaNodus* a = forma->datum.nodus;
            constans SilvaNodus* b = fons->datum.nodus;
                            i32  k;

            si (a == NIHIL || b == NIHIL)
            {
                redde a == b;
            }
            si (   a->genus           != b->genus
                || a->numerus_locorum != b->numerus_locorum)
            {
                redde FALSUM;
            }
            per (k = ZEPHYRUM; k < a->numerus_locorum; k++)
            {
                si (!_valores_congruunt(piscina, &a->loci[k],
                    &b->loci[k],
                        capturae, forma_modus))
                {
                    redde FALSUM;
                }
            }
            redde VERUM;
        }
        ordinarius:
            redde FALSUM;
    }
    redde FALSUM;
}

/* valor NODUS ex nodo constanti (unio: sine abiectione 'constans') */
interior MateriaValor
_valor_nodi (
    constans SilvaNodus* nodus)
{
    MateriaValor v;
    unio {
        constans SilvaNodus* c;
               MateriaNodus* m;
    } u;

    u.c            = nodus;
    v.genus        = MATERIA_VALOR_NODUS;
    v.datum.nodus  = u.m;
    redde v;
}

b32
silva_forma_congruit (
                Piscina* piscina,
    constans SilvaForma* forma,
    constans SilvaNodus* nodus,
                    Xar* capturae)
{
    MateriaValor p;
    MateriaValor f;

    si (forma == NIHIL || forma->nodus == NIHIL || nodus == NIHIL)
    {
        redde FALSUM;
    }
    /* capturae semper servantur: lacuna iterata aequalitatem eius
     * poscit etiam si vocator capturas non vult */
    si (capturae == NIHIL)
    {
        capturae = xar_creare(piscina, (i32)magnitudo(SilvaCaptura));
        si (capturae == NIHIL)
        {
            redde FALSUM;
        }
    }
    xar_vacare(capturae);
    p = _valor_nodi(forma->nodus);
    f = _valor_nodi(nodus);
    redde _valores_congruunt(piscina, &p, &f, capturae, VERUM);
}

b32
silva_nodi_aequales (
    constans SilvaNodus* a,
    constans SilvaNodus* b)
{
    MateriaValor p;
    MateriaValor f;

    p = _valor_nodi(a);
    f = _valor_nodi(b);
    redde _valores_congruunt(NIHIL, &p, &f, NIHIL, FALSUM);
}


/* ==================================================
 * Situs (transformare T3)
 * ================================================== */

/* lectio canonica nodi ambigui (NIHIL si forma deformis) */
interior constans SilvaValor*
_situs_lectio_canonica (
    constans SilvaCommissio* commissio,
        constans SilvaNodus* nodus)
{
    constans SilvaValor* interpretationes;
    constans SilvaValor* canonica;

    si (   commissio->locus_interpretationum >= nodus->numerus_locorum
        || commissio->locus_canonicae        >= nodus->numerus_locorum)
    {
        redde NIHIL;
    }
    interpretationes  = &nodus->loci[commissio->locus_interpretationum];
    canonica          = &nodus->loci[commissio->locus_canonicae];
    si (   interpretationes->genus != MATERIA_VALOR_LISTA
        || canonica->genus         != MATERIA_VALOR_INDEX
        || canonica->datum.index < ZEPHYRUM
        || (i32)canonica->datum.index
            >= materia_valor_lista_numerus(*interpretationes))
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*interpretationes,
        (i32)canonica->datum.index);
}

interior b32
_est_ambiguus (
    constans SilvaCommissio* commissio,
        constans SilvaNodus* nodus)
{
    redde commissio != NIHIL && nodus != NIHIL
        && nodus->genus == commissio->genus_ambigui;
}

/* lexema loci (NIHIL si locus absens aut non TOKEN) */
interior SilvaToken*
_lexema_loci (
    constans SilvaNodus* nodus,
     constans character* titulus)
{
    constans SilvaValor* v = silva_nodi_locus(nodus, titulus);

    redde (v != NIHIL && v->genus == MATERIA_VALOR_TOKEN)
        ? v->datum.token : NIHIL;
}

/* lexema scriptum in plagula principali ipsa (stratum 0) */
interior b32
_fons_principis (
    SilvaToken* lex,
           s32  princeps)
{
    redde lex != NIHIL && silva_token_est_fons(lex)
        && lex->fons_index == princeps && lex->byte_offset >= ZEPHYRUM;
}

/* macro exterior: catena originis usque ad lexema fontis */
interior chorda
_macrum_exterius (
    SilvaToken* lex)
{
         chorda  titulus;
     SilvaToken* t       = lex;
            i32  gradus  = ZEPHYRUM;   /* custodia contra cyclos */

    titulus.mensura  = ZEPHYRUM;
    titulus.datum    = NIHIL;
    dum (t != NIHIL && gradus < M)
    {
        SilvaOrigo* o = silva_token_origo(t);
            chorda* n = NIHIL;

        si (o == NIHIL)
        {
            frange;
        }
        commutatio (o->genus)
        {
            casus SILVA_ORIGO_EXPANSIO:
                n  = o->datum.expansio.nomen_macro;
                t  = o->datum.expansio.invocatio;
                frange;
            casus SILVA_ORIGO_PASTA:
                n  = o->datum.pasta.nomen_macro;
                t  = o->datum.pasta.invocatio;
                frange;
            casus SILVA_ORIGO_CHORDA:
                n  = o->datum.stringificatio.nomen_macro;
                t  = o->datum.stringificatio.primus;
                frange;
            ordinarius:
                t = NIHIL;
                frange;
        }
        si (n != NIHIL)
        {
            titulus = *n;
        }
        gradus++;
    }
    redde titulus;
}

/* lexemata valoris numerare (spina canonica), primum servare */
interior vacuum
_lexemata_numerare (
    constans SilvaCommissio*  commissio,
        constans SilvaValor*  v,
                        i32*  numerus,
                 SilvaToken** primum)
{
    i32 k;

    si (v == NIHIL)
    {
        redde;
    }
    si (v->genus == MATERIA_VALOR_TOKEN && v->datum.token != NIHIL)
    {
        si (*numerus == ZEPHYRUM)
        {
            *primum = v->datum.token;
        }
        (*numerus)++;
        redde;
    }
    si (v->genus == MATERIA_VALOR_LISTA)
    {
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*v); k++)
        {
            _lexemata_numerare(commissio,
                materia_valor_lista_obtinere(*v, k), numerus, primum);
        }
        redde;
    }
    si (v->genus != MATERIA_VALOR_NODUS || v->datum.nodus == NIHIL)
    {
        redde;
    }
    si (_est_ambiguus(commissio, v->datum.nodus))
    {
        _lexemata_numerare(commissio,
            _situs_lectio_canonica(commissio, v->datum.nodus), numerus,
            primum);
        redde;
    }
    per (k = ZEPHYRUM; k < v->datum.nodus->numerus_locorum; k++)
    {
        _lexemata_numerare(commissio, &v->datum.nodus->loci[k],
            numerus, primum);
    }
}

/* nodus quem valor vocationis transit sine usu: parentheses, ambigua,
 * conversio '(vacuum)' (valorem abicit) */
interior b32
_valorem_non_adhibet (
    constans SilvaCommissio* commissio,
        constans SilvaNodus* nodus)
{
    constans character* g = silva_nodi_genus_titulus(nodus);

    si (   _est_ambiguus(commissio, nodus)
        || strcmp(g, "parenthesis") == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (strcmp(g, "conversio") == ZEPHYRUM)
    {
                i32  numerus  = ZEPHYRUM;
         SilvaToken* primum   = NIHIL;

        _lexemata_numerare(commissio, silva_nodi_locus(nodus, "typus"),
            &numerus, &primum);
        redde numerus == I && primum != NIHIL
            && chorda_aequalis_literis(primum->valor, "void");
    }
    redde FALSUM;
}

interior constans SilvaNodus*
_consumentem_invenire (
    constans SilvaCommissio* commissio,
        constans SilvaNodus* vocatio)
{
    constans SilvaNodus* p = vocatio->pater;

    dum (p != NIHIL && _valorem_non_adhibet(commissio, p))
    {
        p = p->pater;
    }
    si (   p != NIHIL
        && strcmp(silva_nodi_genus_titulus(p), "declarator-initiatus")
            == ZEPHYRUM)
    {
        constans SilvaNodus* d = p->pater;

        dum (   d != NIHIL
             && strcmp(silva_nodi_genus_titulus(d), "declaratio")
                 != ZEPHYRUM)
        {
            d = d->pater;
        }
        si (d != NIHIL)
        {
            p = d;
        }
    }
    redde p;
}

nomen structura {
    constans SilvaParsura* parsura;
                   chorda  functio;
                      Xar* situs;
} SitusInventio;

interior vacuum
_vocationem_probare (
           SitusInventio* inv,
     constans SilvaNodus* n)
{
    constans SilvaNodus* functio   = _nodus_loci(n, "functio");
                    s32  princeps  = inv->parsura->fons_princeps;
             SilvaToken* lex;
             SilvaToken* radix;
             SilvaToken* apertum;
             SilvaToken* clausum;
             SilvaSitus* s;

    si (   functio == NIHIL
        || strcmp(silva_nodi_genus_titulus(functio),
               "folium-identificator") != ZEPHYRUM)
    {
        redde;
    }
    lex = _lexema_loci(functio, "tok_valor");
    si (lex == NIHIL || !chorda_aequalis(lex->valor, inv->functio))
    {
        redde;
    }
    radix = silva_token_radix(lex);
    si (   radix == NIHIL || radix->fons_index != princeps
        || radix->byte_offset < ZEPHYRUM)
    {
        redde;
    }
    s = (SilvaSitus*)xar_addere(inv->situs);
    si (s == NIHIL)
    {
        redde;
    }
    s->vocatio        = n;
    s->consumens      = NIHIL;
    s->in_macro       = FALSUM;
    s->macro.mensura  = ZEPHYRUM;
    s->macro.datum    = NIHIL;
    s->linea          = radix->linea;
    s->columna        = radix->columna;
    apertum           = _lexema_loci(n, "tok_apertum");
    clausum           = _lexema_loci(n, "tok_clausum");
    si (   !_fons_principis(apertum, princeps)
        || !_fons_principis(clausum, princeps))
    {
        s->in_macro  = VERUM;
        s->macro     = _macrum_exterius(
            _fons_principis(apertum, princeps) ? clausum : apertum);
        redde;
    }
    s->consumens = _consumentem_invenire(inv->parsura->commissio, n);
}

interior vacuum
_situs_ambulare (
          SitusInventio* inv,
    constans SilvaValor* v)
{
    constans SilvaNodus* n;
                    i32  k;

    si (v == NIHIL)
    {
        redde;
    }
    si (v->genus == MATERIA_VALOR_LISTA)
    {
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*v); k++)
        {
            _situs_ambulare(inv, materia_valor_lista_obtinere(*v, k));
        }
        redde;
    }
    si (v->genus != MATERIA_VALOR_NODUS || v->datum.nodus == NIHIL)
    {
        redde;
    }
    n = v->datum.nodus;
    si (_est_ambiguus(inv->parsura->commissio, n))
    {
        _situs_ambulare(inv,
            _situs_lectio_canonica(inv->parsura->commissio, n));
        redde;
    }
    si (strcmp(silva_nodi_genus_titulus(n), "vocatio") == ZEPHYRUM)
    {
        _vocationem_probare(inv, n);
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        _situs_ambulare(inv, &n->loci[k]);
    }
}

Xar*
silva_situs_invenire (
                   Piscina* piscina,
     constans SilvaParsura* parsura,
                    chorda  functio)
{
    SitusInventio inv;

    si (   piscina            == NIHIL || parsura == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    inv.parsura  = parsura;
    inv.functio  = functio;
    inv.situs    = xar_creare(piscina, (i32)magnitudo(SilvaSitus));
    si (inv.situs == NIHIL)
    {
        redde NIHIL;
    }
    _situs_ambulare(&inv, &parsura->commissio->radix);
    redde inv.situs;
}

/* substitutio scripturae: octeti [initium, finis) -> textus */
nomen structura {
                    s32  initium;
                    s32  finis;
     constans character* substitutio;
} FormaeSubstitutio;

nomen structura {
     constans SilvaCommissio* commissio;
         constans SilvaNodus* vocatio;
           ChordaAedificator* clavis;
                         Xar* substitutiones;
                     Piscina* piscina;
                         s32  princeps;
                         i32  lacunae;
                         b32  corpus_ultimum;  /* '...' contiguum */
} FormaeScriptor;

interior constans character*
_loci_titulus (
    constans SilvaNodus* nodus,
                    i32  k)
{
    constans MateriaTabGenus* g;

    si (   nodus->genus < ZEPHYRUM
        || nodus->genus >= (s32)SILVA_C89_REGISTRUM.numerus_generum)
    {
        redde "";
    }
    g = &SILVA_C89_REGISTRUM.genera[nodus->genus];
    si (k >= g->loci_numerus)
    {
        redde "";
    }
    redde SILVA_C89_REGISTRUM.loci[g->loci_offset + k].titulus;
}

/* locus corporis sententiae compositae (in forma '...') */
interior b32
_locus_corporis (
    constans SilvaNodus* nodus,
                    i32  k)
{
    constans character* g = silva_nodi_genus_titulus(nodus);
    constans character* l;

    si (   strcmp(g, "si")         != ZEPHYRUM
        && strcmp(g, "dum")        != ZEPHYRUM
        && strcmp(g, "per")        != ZEPHYRUM
        && strcmp(g, "fac-dum")    != ZEPHYRUM
        && strcmp(g, "commutatio") != ZEPHYRUM)
    {
        redde FALSUM;
    }
    l = _loci_titulus(nodus, k);
    redde strcmp(l, "consequens") == ZEPHYRUM
        || strcmp(l, "tok_alioquin") == ZEPHYRUM
        || strcmp(l, "alioquin") == ZEPHYRUM
        || strcmp(l, "corpus") == ZEPHYRUM;
}

interior vacuum
_clavi_addere (
          FormaeScriptor* fs,
      constans character* textus,
                     i32  mensura)
{
    chorda iam = chorda_aedificator_spectare(fs->clavis);
       i32 k;

    /* 'genus|lex lex ...': spatium inter lexemata, non post '|' */
    si (iam.mensura > ZEPHYRUM && iam.datum[iam.mensura - I] != '|')
    {
        (vacuum)chorda_aedificator_appendere_character(fs->clavis, ' ');
    }
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        (vacuum)chorda_aedificator_appendere_character(fs->clavis,
            textus[k]);
    }
}

/* substitutionem notare (extentum ignotum = scriptura cruda) */
interior vacuum
_substitutionem_notare (
          FormaeScriptor* fs,
     constans SilvaValor* v,
      constans character* substitutio,
                     b32  extendere)
{
    FormaeSubstitutio* o;
                  s32  min = -I;
                  s32  max = ZEPHYRUM;

    silva_valor_extensionem(*v, fs->princeps, &min, &max);
    si (min < ZEPHYRUM)
    {
        redde;
    }
    si (extendere && xar_numerus(fs->substitutiones) > ZEPHYRUM)
    {
        o = (FormaeSubstitutio*)xar_obtinere(fs->substitutiones,
            xar_numerus(fs->substitutiones) - I);
        si (o != NIHIL && max > o->finis)
        {
            o->finis = max;
        }
        redde;
    }
    o = (FormaeSubstitutio*)xar_addere(fs->substitutiones);
    si (o != NIHIL)
    {
        o->initium      = min;
        o->finis        = max;
        o->substitutio  = substitutio;
    }
}

interior constans character*
_lacunae_titulus (
    Piscina* piscina,
        i32  numerus)
{
    character  inversa[XII];
    character* t = (character*)piscina_allocare(piscina, XVI);
          i32  n = ZEPHYRUM;
          i32  k = ZEPHYRUM;

    si (t == NIHIL)
    {
        redde "$?";
    }
    fac
    {
        inversa[n++]  = (character)('0' + (integer)(numerus % X));
        numerus       /= X;
    } dum (numerus > ZEPHYRUM && n < XII);
    t[k++] = '$';
    dum (n > ZEPHYRUM)
    {
        t[k++] = inversa[--n];
    }
    t[k] = '\0';
    redde t;
}

interior vacuum
_formam_ambulare (
          FormaeScriptor* fs,
     constans SilvaValor* v);

/* argumentum vocationis scopi: lacuna in clave et scriptura */
interior vacuum
_argumentum_scribere (
          FormaeScriptor* fs,
     constans SilvaValor* v)
{
    constans character* t;

    si (v->genus != MATERIA_VALOR_NODUS)
    {
        _formam_ambulare(fs, v);   /* comma */
        redde;
    }
    fs->lacunae++;
    t = _lacunae_titulus(fs->piscina, fs->lacunae);
    _clavi_addere(fs, t, (i32)strlen(t));
    fs->corpus_ultimum = FALSUM;
    _substitutionem_notare(fs, v, t, FALSUM);
}

interior vacuum
_formam_ambulare (
          FormaeScriptor* fs,
     constans SilvaValor* v)
{
    constans SilvaNodus* n;
                    i32  k;

    si (v == NIHIL)
    {
        redde;
    }
    si (v->genus == MATERIA_VALOR_TOKEN)
    {
        si (v->datum.token != NIHIL)
        {
            _clavi_addere(fs,
                (constans character*)v->datum.token->valor.datum,
                v->datum.token->valor.mensura);
            fs->corpus_ultimum = FALSUM;
        }
        redde;
    }
    si (v->genus == MATERIA_VALOR_LISTA)
    {
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*v); k++)
        {
            _formam_ambulare(fs, materia_valor_lista_obtinere(*v, k));
        }
        redde;
    }
    si (v->genus != MATERIA_VALOR_NODUS || v->datum.nodus == NIHIL)
    {
        redde;
    }
    n = v->datum.nodus;
    si (_est_ambiguus(fs->commissio, n))
    {
        _formam_ambulare(fs, _situs_lectio_canonica(fs->commissio, n));
        redde;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        constans SilvaValor* l = &n->loci[k];

        si (   n                                        == fs->vocatio
            && strcmp(_loci_titulus(n, k), "argumenta") == ZEPHYRUM)
        {
            si (l->genus == MATERIA_VALOR_LISTA)
            {
                i32 m;

                per (m = ZEPHYRUM; m < materia_valor_lista_numerus(*l);
                     m++)
                {
                    _argumentum_scribere(fs,
                        materia_valor_lista_obtinere(*l, m));
                }
            }
            alioquin si (l->genus == MATERIA_VALOR_NODUS)
            {
                _argumentum_scribere(fs, l);
            }
            perge;
        }
        si (_locus_corporis(n, k))
        {
            si (l->genus == MATERIA_VALOR_NIHIL)
            {
                perge;
            }
            si (!fs->corpus_ultimum)
            {
                _clavi_addere(fs, "...", III);
            }
            _substitutionem_notare(fs, l, "...", fs->corpus_ultimum);
            fs->corpus_ultimum = VERUM;
            perge;
        }
        _formam_ambulare(fs, l);
    }
}

interior b32
_scripturae_spatium_est (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
        || c == '\v';
}

/* scriptura: octeti [a, b) fontis, substitutionibus factis,
 * commentariis omissis, spatiis contractis; litterae intactae */
interior vacuum
_scripturam_scribere (
      ChordaAedificator* s,
     constans character* fons,
                    s32  a,
                    s32  b,
                    Xar* series)
{
    s32 i         = a;
    i32 j         = ZEPHYRUM;
    b32 spatium   = FALSUM;
    b32 scriptum  = FALSUM;

    dum (i < b)
    {
          constans FormaeSubstitutio* o = NIHIL;
                           character  c = fons[i];

        dum (j < xar_numerus(series))
        {
            o = (constans FormaeSubstitutio*)xar_obtinere(series, j);
            si (o->initium >= i)
            {
                frange;
            }
            j++;
            o = NIHIL;
        }
        si (o != NIHIL && o->initium == i)
        {
            si (spatium && scriptum)
            {
                (vacuum)chorda_aedificator_appendere_character(s, ' ');
            }
            (vacuum)chorda_aedificator_appendere_literis(s,
                o->substitutio);
            scriptum  = VERUM;
            spatium   = FALSUM;
            i         = o->finis > i ? o->finis : i + I;
            j++;
            perge;
        }
        si (   _scripturae_spatium_est(c) || (c == '\\' && i + I < b
            && fons[i + I] == '\n'))
        {
            spatium = VERUM;
            i++;
            perge;
        }
        si (c == '/' && i + I < b && fons[i + I] == '*')
        {
            i += II;
            dum (i + I < b && !(fons[i] == '*' && fons[i + I] == '/'))
            {
                i++;
            }
            i        += II;
            spatium  = VERUM;
            perge;
        }
        si (c == '/' && i + I < b && fons[i + I] == '/')
        {
            dum (i < b && fons[i] != '\n')
            {
                i++;
            }
            spatium = VERUM;
            perge;
        }
        si (spatium && scriptum)
        {
            (vacuum)chorda_aedificator_appendere_character(s, ' ');
        }
        spatium   = FALSUM;
        scriptum  = VERUM;
        (vacuum)chorda_aedificator_appendere_character(s, c);
        i++;
        si (c == '"' || c == '\'')
        {
            dum (i < b && fons[i] != c && fons[i] != '\n')
            {
                si (fons[i] == '\\' && i + I < b)
                {
                    (vacuum)chorda_aedificator_appendere_character(s,
                        fons[i]);
                    i++;
                }
                (vacuum)chorda_aedificator_appendere_character(s,
                    fons[i]);
                i++;
            }
            si (i < b && fons[i] == c)
            {
                (vacuum)chorda_aedificator_appendere_character(s, c);
                i++;
            }
        }
    }
}

b32
silva_situs_forma (
                  Piscina* piscina,
    constans SilvaParsura* parsura,
      constans SilvaSitus* situs,
       constans character* fons,
                      i32  mensura,
                   chorda* clavis_out,
                   chorda* scriptura_out)
{
       FormaeScriptor  fs;
    ChordaAedificator* scriptura;
         MateriaValor  radix;
                  s32  min = -I;
                  s32  max = ZEPHYRUM;
                  i32  i;
                  i32  k;

    si (   situs            == NIHIL || situs->in_macro
        || situs->consumens == NIHIL
        || parsura          == NIHIL || fons == NIHIL)
    {
        redde FALSUM;
    }
    silva_nodus_extensionem(situs->consumens, parsura->fons_princeps,
        &min, &max);
    si (min < ZEPHYRUM || max > (s32)mensura)
    {
        redde FALSUM;
    }
    fs.commissio       = parsura->commissio;
    fs.vocatio         = situs->vocatio;
    fs.piscina         = piscina;
    fs.princeps        = parsura->fons_princeps;
    fs.lacunae         = ZEPHYRUM;
    fs.corpus_ultimum  = FALSUM;
    fs.clavis          = chorda_aedificator_creare(piscina, CXXVIII);
    fs.substitutiones = xar_creare(piscina,
        (i32)magnitudo(FormaeSubstitutio));
    scriptura = chorda_aedificator_creare(piscina, CXXVIII);
    si (   fs.clavis == NIHIL || fs.substitutiones == NIHIL
        || scriptura == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)chorda_aedificator_appendere_literis(fs.clavis,
        silva_nodi_genus_titulus(situs->consumens));
    (vacuum)chorda_aedificator_appendere_character(fs.clavis, '|');
    radix = _valor_nodi(situs->consumens);
    _formam_ambulare(&fs, &radix);

    /* substitutiones ordine fontis (insertio simplex: paucae) */
    per (i = I; i < xar_numerus(fs.substitutiones); i++)
    {
        FormaeSubstitutio o =
            *(FormaeSubstitutio*)xar_obtinere(fs.substitutiones,
            i);

        k = i;
        dum (   k > ZEPHYRUM
             && ((FormaeSubstitutio*)xar_obtinere(fs.substitutiones, k
                 - I))
                    ->initium > o.initium)
        {
            *(FormaeSubstitutio*)xar_obtinere(fs.substitutiones, k) =
                *(FormaeSubstitutio*)xar_obtinere(fs.substitutiones, k
                    - I);
            k--;
        }
        *(FormaeSubstitutio*)xar_obtinere(fs.substitutiones, k) = o;
    }
    _scripturam_scribere(scriptura, fons, min, max, fs.substitutiones);

    *clavis_out     = chorda_aedificator_finire(fs.clavis);
    *scriptura_out  = chorda_aedificator_finire(scriptura);
    redde VERUM;
}
