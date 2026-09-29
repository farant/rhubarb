/* briar_fabrica.c - Vide briar_fabrica.h. */

#include "postulata_posix.h"
#include "briar_fabrica.h"
#include "briar_plagulae.h"
#include "briar_arbor.h"
#include "briar_silva.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "sigillum.h"
#include "tabula_dispersa.h"
#include "via.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior chorda
_literae (
               Piscina* piscina,
    constans character* literae)
{
    redde chorda_ex_literis(literae, piscina);
}

interior constans character*
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b,
    constans character* c)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)128);

    chorda_aedificator_appendere_literis(aed, a);
    chorda_aedificator_appendere_literis(aed, b);
    si (c != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, c);
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

/* spatia finalia abscisa */
interior chorda
_detondere (
    chorda c)
{
    dum (   c.mensura > ZEPHYRUM
         && ((character)c.datum[c.mensura - I] == '\n'
            || (character)c.datum[c.mensura - I] == '\r'
            || (character)c.datum[c.mensura - I] == ' '
            || (character)c.datum[c.mensura - I] == '\t'))
    {
        c.mensura = c.mensura - I;
    }
    redde c;
}

/* linea (I-basata) in qua offset iacet */
interior i32
_linea_octeti (
    chorda textus,
       i32 offset)
{
    i32 linea = I;
    i32 i;

    per (i = ZEPHYRUM; i < offset && i < textus.mensura; i++)
    {
        si ((character)textus.datum[i] == '\n')
        {
            linea = linea + I;
        }
    }
    redde linea;
}

interior b32
_silva_chorda_est (
            SilvaChorda  s,
     constans character* literae)
{
    redde (b32)(s.mensura == (insignatus integer)strlen(literae)
        && memcmp(s.datum, literae, (size_t)s.mensura) == ZEPHYRUM);
}

interior vacuum
_recusare (
    BriarFabricaFructus* f,
                Piscina* piscina,
     constans character* causa,
                    i32  linea)
{
    f->successus     = FALSUM;
    f->causa         = _literae(piscina, causa);
    f->linea_causae  = linea;
    f->genitae       = NIHIL;
    f->clausura      = NIHIL;
}

interior vacuum
_genitam_addere (
               Piscina* piscina,
                   Xar* genitae,
    constans character* via,
                chorda  contentum)
{
    BriarPlagula* p = (BriarPlagula*)xar_addere(genitae);

    si (p != NIHIL)
    {
        p->via        = _literae(piscina, via);
        p->contentum  = contentum;
    }
}

interior vacuum
_lineam_appendere (
     ChordaAedificator* a,
                   i32  linea,
    constans character* via)
{
    character b[32];

    /* via NIHIL: textus sine lineis (caput membri pro parsura
     * importantis - silva '#line' recusat; spec par. 3.5) */
    si (via == NIHIL)
    {
        redde;
    }
    sprintf(b, "#line %d \"", (integer)linea);
    chorda_aedificator_appendere_literis(a, b);
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a, "\"\n");
}

b32
briar_directoria_creare (
               Piscina* piscina,
    constans character* via)
{
          i32  m = (i32)strlen(via);
    character* gradus = (character*)piscina_allocare(piscina,
        (memoriae_index)(m + I));
    i32 i;

    per (i = ZEPHYRUM; i < m; i++)
    {
        gradus[i] = via[i];
        si (via[i] == '/' && i > ZEPHYRUM)
        {
            gradus[i] = '\0';
            (vacuum)filum_directorium_creare_si_necesse(gradus);
            gradus[i] = '/';
        }
    }
    gradus[m] = '\0';
    redde filum_directorium_creare_si_necesse(gradus);
}


/* ==================================================
 * Titulus, vexilla, clavis
 * ================================================== */

constans character*
briar_fabrica_titulus (
               Piscina* piscina,
    constans character* via)
{
    constans character* basis = via;
    constans character* p;
             character* t;
                   i32  m;
                   i32  i;

    per (p = via; *p != '\0'; p++)
    {
        si (*p == '/')
        {
            basis = p + I;
        }
    }
    m = (i32)strlen(basis);
    si (m > VIII && strcmp(basis + (m - VIII), ".thistle") == ZEPHYRUM)
    {
        m = m - VIII;
    }
    si (m == ZEPHYRUM)
    {
        redde "thistle";
    }
    t = (character*)piscina_allocare(piscina, (memoriae_index)(m + I));
    per (i = ZEPHYRUM; i < m; i++)
    {
        character c = basis[i];
              b32 litera = (b32)((c >= 'a' && c <= 'z')
                  || (c >= 'A' && c <= 'Z')
                  || (c >= '0' && c <= '9') || c == '_');

        t[i] = litera ? c : '_';
    }
    t[m] = '\0';
    redde t;
}

constans character*
briar_fabrica_vexilla (
    BriarForma forma)
{
    si (forma == BRIAR_FORMA_VITREA)
    {
        redde SILEX_VEXILLA_VITREA " | " SILEX_VEXILLA_VENDITORIA;
    }
    redde SILEX_VEXILLA_COMPILATIONIS;
}

vacuum
briar_fabrica_clavem_computare (
    constans character* stampa,
    constans character* vexilla,
                chorda  octeti,
             character* sigillum_xvii)
{
    SigillumContextus ctx;
             Sigillum s;
            character hex[SIGILLUM_HEX_MENSURA];

    sigillum_incipere(&ctx);
    sigillum_addere(&ctx, stampa, (memoriae_index)strlen(stampa));
    sigillum_addere(&ctx, "\n", (memoriae_index)I);
    sigillum_addere(&ctx, vexilla, (memoriae_index)strlen(vexilla));
    sigillum_addere(&ctx, "\n", (memoriae_index)I);
    sigillum_addere(&ctx, octeti.datum, (memoriae_index)octeti.mensura);
    s = sigillum_finire(&ctx);
    sigillum_hex(&s, hex);
    memcpy(sigillum_xvii, hex, (size_t)16);
    sigillum_xvii[16] = '\0';
}


/* ==================================================
 * Inventarium regionum
 * ================================================== */

nomen structura {
     BriarNexusRes** app;          /* regiones C non-probatio, ordine */
               i32   numerus_app;
     BriarNexusRes*  probatio;     /* aut NIHIL */
     BriarNexusRes*  fenestra;     /* elementum */
     BriarNexusRes*  html;
     BriarNexusRes*  js;
     BriarNexusRes*  css;
} BriarInventarium;

interior b32
_inventarium_colligere (
               Piscina* piscina,
                   Xar* nexus,
      BriarInventarium* inv,
   BriarFabricaFructus* f)
{
    i32 i;
    i32 n = xar_numerus(nexus);

    memset(inv, 0, magnitudo(*inv));
    inv->app = (BriarNexusRes**)piscina_allocare(piscina,
        (memoriae_index)((n + I) * (i32)magnitudo(BriarNexusRes*)));
    per (i = ZEPHYRUM; i < n; i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

                si (   r->genus == BRIAR_NEXUS_REGIO
                    && briar_nexus_titulus_est(r, "c"))
                {
            si (r->linea_erroris > ZEPHYRUM)
            {
                _recusare(f, piscina, chorda_ut_cstr(r->causa, piscina),
                    r->linea_erroris);
                redde FALSUM;
            }
            si (r->est_fragmentum)
            {
                perge;   /* in radicibus contextum (briar_contextus) */
            }
            si (r->silva == NIHIL || r->silva->parsura == NIHIL)
            {
                _recusare(f, piscina, r->causa.mensura > ZEPHYRUM
                    ? chorda_ut_cstr(r->causa, piscina)
                    : "regio C non parsata (briar_silvam_texere ante)",
                    r->linea_erroris > ZEPHYRUM ? r->linea_erroris
                    : r->linea_initium);
                redde FALSUM;
            }
            si (chorda_aequalis_literis(briar_nexus_attributum(r,
                "munus"),
                "probatio"))
            {
                si (inv->probatio != NIHIL)
                {
                    _recusare(f, piscina,
                        "regio probationis iterata (una in plano I)",
                        r->linea_initium - I);
                    redde FALSUM;
                }
                inv->probatio = r;
            }
            alioquin
            {
                inv->app[inv->numerus_app]  = r;
                inv->numerus_app            = inv->numerus_app + I;
            }
                }
        alioquin si (   r->genus == BRIAR_NEXUS_STML
                     && briar_nexus_titulus_est(r, "fenestra"))
                {
            inv->fenestra = r;
                }
        alioquin si (r->genus == BRIAR_NEXUS_REGIO)
                {
            BriarNexusRes** sedes = briar_nexus_titulus_est(r, "html")
                ? &inv->html : briar_nexus_titulus_est(r,
                "js") ? &inv->js
                : briar_nexus_titulus_est(r, "css") ? &inv->css : NIHIL;

            si (sedes != NIHIL)
            {
                si (*sedes != NIHIL)
                {
                    character b[96];

                    sprintf(b, "regio %.*s iterata (prima linea %d)",
                        (integer)r->titulus.mensura,
                        (constans character*)r->titulus.datum,
                        (integer)((*sedes)->linea_initium - I));
                    _recusare(f, piscina, b, r->linea_initium - I);
                    redde FALSUM;
                }
                *sedes = r;
            }
                }
    }
    redde VERUM;
}


/* ==================================================
 * Partitio unitatum (silva)
 * ================================================== */

nomen structura {
                       i32  linea;    /* .thistle (lineae primae) */
                    chorda  textus;
    constans BriarNexusRes* regio;    /* tabula linearum (contextus) */
                       i32  index;    /* index contextus, linea prima */
} BriarUnitas;

/* linea .thistle lineae k contextus regionis (tabula briar_contextus;
 * formula linearis si abest) */
interior i32
_linea_tabulae (
    constans BriarNexusRes* r,
                       i32  k)
{
    si (r->lineae != NIHIL && k < xar_numerus(r->lineae))
    {
        redde *(i32*)xar_obtinere(r->lineae, k);
    }
    redde r->linea_initium + k;
}

nomen structura {
                Xar* directivae;   /* lineae '#...' */
                Xar* typi;         /* unitates sine obiecto */
                Xar* prototypi;    /* 'caput;' definitionum */
                Xar* corpora;      /* obiecta + definitiones */
                Xar* derivata;     /* chorda: capita derivata */

    BriarUnitas princeps;
            i32 principalia;  /* numerus unitatum 'main' */
            i32 linea_principalis_secundi;
} BriarPartitio;

interior vacuum
_unitatem_addere (
                       Xar* xar,
    constans BriarNexusRes* regio,
                       i32  index,
                    chorda  textus)
{
    BriarUnitas* u = (BriarUnitas*)xar_addere(xar);

    si (u != NIHIL)
    {
        u->linea   = _linea_tabulae(regio, index);
        u->textus  = textus;
        u->regio   = regio;
        u->index   = index;
    }
}

/* intervallum octetorum contextus [initium, finis) unitatis MIXTAE
 * (directivae + codex): directivae eius cum unitate emittuntur, non
 * seorsum (parcum VF42V) */
nomen structura {
    i32 initium;
    i32 finis;
} BriarIntervallum;

interior b32
_intra_intervalla (
    Xar* intervalla,
    i32  positio)
{
    i32 i;

    per (i = ZEPHYRUM; intervalla != NIHIL
        && i < xar_numerus(intervalla); i++)
    {
        constans BriarIntervallum* v =
            (constans BriarIntervallum*)xar_obtinere(intervalla, i);

        si (positio >= v->initium && positio < v->finis)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* quid directivarum unitas silvae fert (coniunctio '#ifndef ...
 * #endif' unitas UNA est, cum directivis suis) */
nomen enumeratio {
    BRIAR_DIRECTIVAE_NULLAE  = 0,   /* codex solus */
    BRIAR_DIRECTIVAE_SOLAE   = 1,   /* lineae '#' solae (et vacuae) */
    BRIAR_DIRECTIVAE_MIXTAE  = 2    /* directivae cum codice (etiam
                                     * commentario) */
} BriarDirectivaeUnitatis;

interior BriarDirectivaeUnitatis
_directivae_unitatis (
    chorda textus)
{
    b32 directiva   = FALSUM;
    b32 alia        = FALSUM;
    b32 continuata  = FALSUM;
    i32 i           = ZEPHYRUM;

    dum (i < textus.mensura)
    {
        i32 p = i;
        i32 f;

        dum (   p < textus.mensura && ((character)textus.datum[p] == ' '
            || (character)textus.datum[p] == '\t'))
        {
            p = p + I;
        }
        f = p;
        dum (f < textus.mensura && (character)textus.datum[f] != '\n')
        {
            f = f + I;
        }
        si (continuata || (p < f && (character)textus.datum[p] == '#'))
        {
            directiva   = VERUM;
            continuata  = (b32)(f > p
                && (character)textus.datum[f - I] == '\\');
        }
        alioquin si (p < f)
        {
            alia = VERUM;
        }
        i = f + I;
    }
    si (!directiva)
    {
        redde BRIAR_DIRECTIVAE_NULLAE;
    }
    redde alia ? BRIAR_DIRECTIVAE_MIXTAE : BRIAR_DIRECTIVAE_SOLAE;
}

/* directivae textuales (lineae quarum character primus non albus '#',
 * cum continuationibus '\') - silva eas consumit, textus eas servat.
 * Directivae intra unitatem MIXTAM (intervalla) omittuntur: unitas eas
 * ipsa fert. */
interior vacuum
_directivas_colligere (
    constans BriarNexusRes* r,
                       Xar* directivae,
                       Xar* intervalla)
{
    chorda c = r->contextus;   /* contextus: fragmenta contexta */
       i32 i = ZEPHYRUM;
       i32 k = ZEPHYRUM;       /* index lineae contextus */

    dum (i < c.mensura)
    {
        i32 initium  = i;
        i32 p        = i;
        i32 finis;

        dum (   p < c.mensura && ((character)c.datum[p] == ' '
            || (character)c.datum[p] == '\t'))
        {
            p = p + I;
        }
        finis = initium;
        dum (finis < c.mensura && (character)c.datum[finis] != '\n')
        {
            finis = finis + I;
        }
        si (p < c.mensura && (character)c.datum[p] == '#')
        {
            i32 f       = finis;
            i32 lineae  = I;

            dum (   f > initium && f < c.mensura
                 && (character)c.datum[f - I] == '\\')
            {
                f = f + I;
                dum (f < c.mensura && (character)c.datum[f] != '\n')
                {
                    f = f + I;
                }
                lineae = lineae + I;
            }
            si (!_intra_intervalla(intervalla, initium))
            {
                _unitatem_addere(directivae, r, k, chorda_sectio(c,
                    initium, f));
            }
            finis  = f;
            k      = k + lineae - I;
        }
        i = (finis < c.mensura) ? finis + I : finis;
        k = k + I;
    }
}

interior constans SemanticaSymbolum*
_symbolum_definitionis (
    constans SilvaSemantica* sem,
        constans SilvaNodus* unitas)
{
    insignatus integer k;

    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(sem); k++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(sem,
            k);

        si (   s->declarans == unitas
            && s->genus     == (integer)SYMBOLUM_FUNCTIO)
        {
            redde s;
        }
    }
    redde NIHIL;
}

interior b32
_unitas_obiectum_declarat (
    constans SilvaSemantica* sem,
                    integer  fons_index,
                    integer  minimum,
                    integer  maximum)
{
    insignatus integer k;

    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(sem); k++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(sem,
            k);

        si (   s->genus               == (integer)SYMBOLUM_VARIABILE
            && s->profunditas         == (insignatus integer)ZEPHYRUM
            && s->lexema              != NIHIL
            && s->lexema->fons_index  == fons_index
            && s->lexema->byte_offset >= minimum
            && s->lexema->byte_offset < maximum)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* capita derivata regionis in xar (dedup, ordo primae visionis) */
interior vacuum
_derivata_addere (
                       Xar* derivata,
    constans BriarNexusRes* r)
{
    i32 k;

    si (r->silva == NIHIL || r->silva->capita_derivata == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(r->silva->capita_derivata); k++)
    {
        chorda c = *(chorda*)xar_obtinere(r->silva->capita_derivata, k);
           i32 i;
           b32 visa = FALSUM;

        per (i = ZEPHYRUM; i < xar_numerus(derivata); i++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(derivata, i), c))
            {
                visa = VERUM;
            }
        }
        si (!visa)
        {
            chorda* cella = (chorda*)xar_addere(derivata);

            *cella = c;
        }
    }
}

/* lineae '#include "x.h"' capitum derivatorum (pro capite genito,
 * unitate probationis, clausura) */
interior chorda
_inclusiones_derivatae (
    Piscina* piscina,
        Xar* derivata)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)256);
    i32 i;

    per (i = ZEPHYRUM; derivata != NIHIL
        && i < xar_numerus(derivata); i++)
    {
        chorda_aedificator_appendere_literis(a, "#include \"");
        chorda_aedificator_appendere_chorda(a,
            *(chorda*)xar_obtinere(derivata, i));
        chorda_aedificator_appendere_literis(a, "\"\n");
    }
    redde chorda_aedificator_finire(a);
}

interior b32
_regionem_partiri (
                   Piscina* piscina,
    constans BriarNexusRes* r,
             BriarPartitio* part,
       BriarFabricaFructus* f)
{
              SilvaValor  radix = r->silva->parsura->commissio->radix;
                SilvaXar* liberi = NIHIL;
      insignatus integer  numerus = ZEPHYRUM;
      insignatus integer  k;
                 integer  fons_index = r->silva->parsura->fons_princeps;
                     Xar* intervalla = xar_creare(piscina,
                         (i32)magnitudo(BriarIntervallum));

    _derivata_addere(part->derivata, r);

    /* radix commissionis: LISTA unitatum (parsura sana) aut NODUS -
     * utraque forma ambulatur */
    si (radix.genus == SILVA_VALOR_LISTA)
    {
        numerus = silva_valor_lista_numerus(radix);
    }
    alioquin si (radix.genus == SILVA_VALOR_NODUS)
    {
        liberi = silva_nodus_liberi(r->silva->piscina,
            radix.datum.nodus);
        numerus = (liberi
            != NIHIL) ? silva_xar_numerus(liberi) : ZEPHYRUM;
    }
    alioquin
    {
        _recusare(f, piscina, "regio C: arbor silvae sine radice",
            r->linea_initium);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
                constans SilvaNodus* u;
                            integer  minimum = -I;
                            integer  maximum = ZEPHYRUM;
                             chorda  textus;
                                i32  linea;
                                i32  index;

        si (liberi != NIHIL)
        {
            u = *(SilvaNodus**)silva_xar_obtinere(liberi, k);
        }
        alioquin
        {
            SilvaValor* e = silva_valor_lista_obtinere(radix, k);

            si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            u = e->datum.nodus;
        }
        silva_nodus_extensionem(u, fons_index, &minimum, &maximum);
        si (   minimum < ZEPHYRUM
            || minimum < (integer)r->praeludium_octeti)
        {
            perge;   /* syntheticum aut praeludium (exemplar) */
        }
                textus = chorda_sectio(r->textus_silvae, (i32)minimum,
                    (i32)maximum);
        index  = _linea_octeti(r->textus_silvae, (i32)minimum)
            - r->praeludium - I;
        linea  = _linea_tabulae(r, index);
        /* coniunctio directivarum SOLARUM: grex directivarum eam fert
         * ordine fontis; MIXTA: unitas tota semel, directivae eius non
         * seorsum (parcum VF42V - '#define' custodis bis emissus
         * copiam typi celabat) */
        commutatio (_directivae_unitatis(textus))
        {
            casus BRIAR_DIRECTIVAE_SOLAE:
                perge;
            casus BRIAR_DIRECTIVAE_MIXTAE:
            {
                BriarIntervallum* v = (BriarIntervallum*)xar_addere(
                    intervalla);

                si (v != NIHIL)
                {
                    v->initium  = (i32)minimum - r->praeludium_octeti;
                    v->finis    = (i32)maximum - r->praeludium_octeti;
                }
                frange;
            }
            ordinarius:
                frange;
        }
        si (u->genus == (integer)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
        {
            constans SemanticaSymbolum* s = _symbolum_definitionis(
                r->silva->semantica, u);
            SilvaValor corpus =
                silva_c89_definitio_functionis_corpus(u);
               integer initium_corporis  = -I;
               integer finis_corporis    = ZEPHYRUM;

            si (s != NIHIL && _silva_chorda_est(s->titulus, "main"))
            {
                part->principalia = part->principalia + I;
                                si (part->principalia == I)
                                {
                    part->princeps.linea   = linea;
                    part->princeps.textus  = textus;
                    part->princeps.regio   = r;
                    part->princeps.index   = index;
                                }
                alioquin
                                {
                    part->linea_principalis_secundi = linea;
                                }
                perge;
            }
            si (corpus.genus == SILVA_VALOR_NODUS)
            {
                silva_nodus_extensionem(corpus.datum.nodus, fons_index,
                    &initium_corporis, &finis_corporis);
            }
            si (initium_corporis > minimum)
            {
                ChordaAedificator* a =
                    chorda_aedificator_creare(piscina,
                    (memoriae_index)256);

                chorda_aedificator_appendere_chorda(a, _detondere(
                    chorda_sectio(r->textus_silvae, (i32)minimum,
                        (i32)initium_corporis)));
                                chorda_aedificator_appendere_literis(a,
                                    ";");
                _unitatem_addere(part->prototypi, r, index,
                    chorda_aedificator_finire(a));
            }
            _unitatem_addere(part->corpora, r, index, textus);
        }
        alioquin si (_unitas_obiectum_declarat(r->silva->semantica,
                     fons_index, minimum, maximum))
        {
            _unitatem_addere(part->corpora, r, index, textus);
        }
        alioquin
        {
            _unitatem_addere(part->typi, r, index, textus);
        }
    }
    _directivas_colligere(r, part->directivae, intervalla);
    redde VERUM;
}


/* ==================================================
 * Textus geniti
 * ================================================== */

/* textus lineatim cum '#line' ad omnem fracturam cursus tabulae
 * linearum (fragmentum contextum intra corpus functionis lineas suas
 * .thistle nominat); linea prima semper '#line' fert; linea ultima
 * '\n' terminata */
interior vacuum
_textum_mappatum_appendere (
         ChordaAedificator* a,
                    chorda  textus,
    constans BriarNexusRes* regio,
                       i32  index,
        constans character* via)
{
    i32 i      = ZEPHYRUM;
    i32 k      = ZEPHYRUM;
    i32 prior  = ZEPHYRUM;

    dum (i < textus.mensura)
    {
        i32 f = i;
        i32 t = _linea_tabulae(regio, index + k);

        dum (f < textus.mensura && (character)textus.datum[f] != '\n')
        {
            f = f + I;
        }
        si (k == ZEPHYRUM || t != prior + I)
        {
            _lineam_appendere(a, t, via);
        }
        chorda_aedificator_appendere_chorda(a, chorda_sectio(textus, i,
            f));
        chorda_aedificator_appendere_literis(a, "\n");
        prior  = t;
        i      = f + I;
        k      = k + I;
    }
}

interior vacuum
_unitates_appendere (
     ChordaAedificator* a,
                   Xar* unitates,
    constans character* via)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(unitates); i++)
    {
        constans BriarUnitas* u = (constans BriarUnitas*)xar_obtinere(
            unitates, i);

        _textum_mappatum_appendere(a, u->textus, u->regio, u->index,
            via);
    }
}

interior constans character*
_custos (
               Piscina* piscina,
    constans character* titulus)
{
          i32  m = (i32)strlen(titulus);
    character* c = (character*)piscina_allocare(piscina,
        (memoriae_index)(m + 12));
    i32 i;

    per (i = ZEPHYRUM; i < m; i++)
    {
        character x = titulus[i];

        c[i] = (x >= 'a' && x <= 'z') ? (character)(x - 'a' + 'A') : x;
    }
    c[m] = '\0';
    strcat(c, "_REGIONES_H");
    redde c;
}

interior chorda
_caput_fingere (
                   Piscina* piscina,
        constans character* titulus,
        constans character* via,
    constans BriarPartitio* part)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);
    constans character* custos = _custos(piscina, titulus);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "_regiones.h - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via != NIHIL ? via
        : "(textus parsurae)");
    chorda_aedificator_appendere_literis(a,
        ": directivae, typi, prototypi regionum */");
    /* textus parsurae (via NIHIL) SINE custode: unitates membrorum
     * in praeludio SEMEL ponuntur, dependentibus primum (briar_silva
     * _parsare) - custos supervacuus. (Non quia semantica intra
     * '#ifndef' caeca sit: planta custodis restituti typum notum
     * reliquit, 2026-09-29.) */
    si (via != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, "\n#ifndef ");
        chorda_aedificator_appendere_literis(a, custos);
        chorda_aedificator_appendere_literis(a, "\n#define ");
        chorda_aedificator_appendere_literis(a, custos);
    }
    /* capita implicita: latina.h + trias vulgaris (stdio/stdlib/string)
     * - plagula thistle scriptum est; imprimere sine stdio.h non
     * compilat */
    chorda_aedificator_appendere_literis(a,
        "\n#include \"latina.h\"\n"
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
                "#include <string.h>\n");
    /* capita DERIVATA ex usu symbolorum (briar_silva) - sine #line */
    chorda_aedificator_appendere_chorda(a,
        _inclusiones_derivatae(piscina, part->derivata));
    _unitates_appendere(a, part->directivae, via);
    _unitates_appendere(a, part->typi, via);
    _unitates_appendere(a, part->prototypi, via);
    si (via != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, "#endif /* ");
        chorda_aedificator_appendere_literis(a, custos);
        chorda_aedificator_appendere_literis(a, " */\n");
    }
    redde chorda_aedificator_finire(a);
}

interior chorda
_corpus_fingere (
                   Piscina* piscina,
        constans character* titulus,
        constans character* via,
    constans BriarPartitio* part)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "_regiones.c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": obiecta et definitiones regionum */\n"
        "#include \"latina.h\"\n#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_regiones.h\"\n");
    _unitates_appendere(a, part->corpora, via);
    redde chorda_aedificator_finire(a);
}

interior chorda
_principem_fingere (
                 Piscina* piscina,
      constans character* titulus,
      constans character* via,
    constans BriarUnitas* princeps)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)2048);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": principale */\n#include \"latina.h\"\n#include \"");
        chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_regiones.h\"\n");
    _textum_mappatum_appendere(a, princeps->textus, princeps->regio,
        princeps->index, via);
    redde chorda_aedificator_finire(a);
}

/* prototypi: 'caput;' functionum regionis probationis (praeter
 * principale) - ut regio principalis, adiutor sine 'staticus' ibi non
 * frangit -Wmissing-prototypes et ordo definitionum liber est (lapide
 * documentation-ideas/016). Caput e fonte sectum 'staticus' servat. */
interior chorda
_probationem_fingere (
                   Piscina* piscina,
        constans character* titulus,
        constans character* via,
    constans BriarNexusRes* probatio,
                       Xar* derivata,
                       Xar* prototypi)
{
        ChordaAedificator* a = chorda_aedificator_creare(piscina,
            (memoriae_index)(probatio->contextus.mensura + 256));

    chorda_aedificator_appendere_literis(a, "/* probatio_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": regio munus=\"probatio\" */\n#include \"latina.h\"\n"
        "#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, "_regiones.h\"\n");
        chorda_aedificator_appendere_chorda(a,
            _inclusiones_derivatae(piscina, derivata));
    _unitates_appendere(a, prototypi, via);
    /* contextus lineatim: fragmenta in probatione contexta */
    _textum_mappatum_appendere(a, probatio->contextus, probatio,
        ZEPHYRUM, via);
    redde chorda_aedificator_finire(a);
}

/* prototypus 'caput;' cum repositione statica (interior, staticus,
 * hic_manens, universalis - latina.h - aut static) initio */
interior b32
_repositio_statica (
    chorda textus)
{
    constans character* verba[VI];
                   i32  i = ZEPHYRUM;
                   i32  k;

    verba[0] = "interior";
    verba[1] = "staticus";
    verba[2] = "hic_manens";
    verba[3] = "universalis";
    verba[4] = "static";
    verba[5] = NIHIL;
    dum (   i < textus.mensura && (textus.datum[i] == ' '
        || textus.datum[i] == '\t' || textus.datum[i] == '\n'))
    {
        i = i + I;
    }
    per (k = ZEPHYRUM; verba[k] != NIHIL; k++)
    {
        i32 m = (i32)strlen(verba[k]);

        si (   i + m < textus.mensura
            && memcmp(textus.datum + i, verba[k], (size_t)m) == ZEPHYRUM
            && (textus.datum[i + m] == ' '
                || textus.datum[i + m] == '\t'
                || textus.datum[i + m] == '\n'))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

BriarMembrumPartitum
briar_membrum_partiri (
                 Piscina* piscina,
                     Xar* nexus,
      constans character* via,
      constans character* titulus)
{
    BriarMembrumPartitum m;
     BriarFabricaFructus f;
           BriarPartitio part;
                     i32 i;

    memset(&m, 0, magnitudo(m));
    memset(&f, 0, magnitudo(f));
    memset(&part, 0, magnitudo(part));
    part.directivae  = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.typi        = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.prototypi   = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.corpora     = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.derivata    = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; nexus != NIHIL && i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* r = (constans BriarNexusRes*)
            xar_obtinere(nexus, i);

        si (!briar_nexus_regio_plana(r))
        {
            perge;
        }
        si (   r->silva == NIHIL || r->silva->parsura == NIHIL
            || !_regionem_partiri(piscina, r, &part, &f))
        {
            m.causa = f.causa.mensura > ZEPHYRUM ? f.causa
                : chorda_ex_literis("bibliotheca: regio membri non"
                    " parsata", piscina);
            m.linea_causae = f.linea_causae > ZEPHYRUM ? f.linea_causae
                : r->linea_initium;
            redde m;
        }
    }
    /* principale membri (si adest) in part.princeps manet: numquam
     * praebetur (spec par. 3.5). Prototypi STATICI caput membri NON
     * intrant: omnis unitas importans declarationem staticam non
     * definitam haberet (-Wunused-function; classis lapide bugs/002) -
     * in capite corporis sui soli stant, ubi definiuntur. */
    {
        BriarPartitio publica   = part;
        BriarPartitio corporis  = part;
                  i32 k;

        publica.prototypi = xar_creare(piscina,
            (i32)magnitudo(BriarUnitas));
        corporis.corpora = xar_creare(piscina,
            (i32)magnitudo(BriarUnitas));
        per (k = ZEPHYRUM; k < xar_numerus(part.prototypi); k++)
        {
            constans BriarUnitas* u = (constans BriarUnitas*)
                xar_obtinere(part.prototypi, k);

            *(BriarUnitas*)xar_addere(_repositio_statica(u->textus)
                ? corporis.corpora : publica.prototypi) = *u;
        }
        per (k = ZEPHYRUM; k < xar_numerus(part.corpora); k++)
        {
            *(BriarUnitas*)xar_addere(corporis.corpora) =
                *(constans BriarUnitas*)xar_obtinere(part.corpora, k);
        }
        m.caput           = _caput_fingere(piscina, titulus, via,
            &publica);
        m.caput_parsurae  = _caput_fingere(piscina, titulus, NIHIL,
            &publica);
        m.corpus          = _corpus_fingere(piscina, titulus, via,
            &corporis);
    }
    m.derivata   = part.derivata;
    m.successus  = VERUM;
    redde m;
}


/* ==================================================
 * Forma vitrea: <fenestra/>, methodi, principale genitum, assets
 * ================================================== */

interior b32
_numerum_legere (
    chorda  c,
       i32* valor)
{
    i32 i;
    i32 v = ZEPHYRUM;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character d = (character)c.datum[i];

        si (d < '0' || d > '9')
        {
            redde FALSUM;
        }
        v = v * X + (i32)(d - '0');
    }
    *valor = v;
    redde VERUM;
}

/* methodus="nomen": functio 'nomen' in regione definita et cum
 * exemplari tractatoris compatibilis; recusatio nominat lineam tagi */
interior b32
_methodum_probare (
                   Piscina* piscina,
    constans BriarNexusRes* r,
                    chorda  methodus,
       BriarFabricaFructus* f)
{
       constans SilvaSemantica* sem       = r->silva->semantica;
    constans SemanticaSymbolum* functio   = NIHIL;
    constans SemanticaSymbolum* exemplar  = NIHIL;
            insignatus integer  k;
                     character  b[160];
                           i32  linea_tagi = r->linea_initium - I;

    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(sem); k++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(sem,
            k);

        si (s->profunditas != (insignatus integer)ZEPHYRUM)
        {
            perge;
        }
        si (   s->genus == (integer)SYMBOLUM_FUNCTIO
            && s->titulus.mensura
                == (insignatus integer)methodus.mensura
            && memcmp(s->titulus.datum, methodus.datum,
                (size_t)methodus.mensura) == ZEPHYRUM
            && silva_c89_definitio_functionis_corpus(s->declarans).genus
                != SILVA_VALOR_NIHIL)
        {
            functio = s;
        }
        si (_silva_chorda_est(s->titulus, "briar_tractator_exemplar"))
        {
            exemplar = s;
        }
    }
    si (functio == NIHIL)
    {
        sprintf(b, "methodus '%.*s' (linea %d): functio non definita in"
            " regione", (integer)methodus.mensura,
            (constans character*)methodus.datum, (integer)linea_tagi);
        _recusare(f, piscina, b, linea_tagi);
        redde FALSUM;
    }
    si (   exemplar               == NIHIL
        || exemplar->typus->genus != (integer)TYPUS_C89_MONSTRATOR
        || !silva_c89_typi_compatibiles(
            exemplar->typus->datum.monstrator.internum, functio->typus))
    {
        sprintf(b,
            "methodus '%.*s' (linea %d): signatura aliena; exspectata"
            " JsonValor* f(JsonValor*, Piscina*, vacuum*, chorda*)",
            (integer)methodus.mensura,
            (constans character*)methodus.datum,
            (integer)linea_tagi);
        _recusare(f, piscina, b, linea_tagi);
        redde FALSUM;
    }
    redde VERUM;
}

interior chorda
_principem_vitreum_fingere (
                      Piscina* piscina,
           constans character* titulus,
           constans character* via,
    constans BriarInventarium* inv,
          BriarFabricaFructus* f)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);
    chorda fenestrae_titulus = briar_nexus_attributum(inv->fenestra,
        "titulus");
    chorda latitudo = briar_nexus_attributum(inv->fenestra,
        "latitudo");
    chorda altitudo = briar_nexus_attributum(inv->fenestra,
        "altitudo");
          i32 numerus;
          i32 i;
    character b[96];

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": fenestra vitrea, methodi regionum praebitae.\n"
        " * Gyrus atrii hic (vide include/atrium.h); status usoris"
        " NIHIL (plan 2). */\n"
        "#include \"latina.h\"\n"
        "#include \"piscina.h\"\n"
        "#include \"chorda.h\"\n"
        "#include \"json.h\"\n"
        "#include \"atrium.h\"\n"
        "#include \"internuntius.h\"\n"
        "#include \"capsula_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".h\"\n#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "_regiones.h\"\n"
        "#include <stdio.h>\n"
        "#include <string.h>\n"
        "\n"
        "s32\n"
        "principale (integer argc, character** argv)\n"
        "{\n"
        "    Piscina*            piscina;\n"
        "    AtriumConfiguratio  figura;\n"
        "    Atrium*             atrium;\n"
        "    chorda              causa;\n"
        "\n"
        "    piscina = piscina_generare_dynamicum(\"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\", 16777216);\n"
        "    si (piscina == NIHIL)\n"
        "    {\n"
        "        redde I;\n"
        "    }\n"
        "    memset(&figura, 0, magnitudo(figura));\n"
        "    figura.titulus  = \"");
    si (fenestrae_titulus.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, fenestrae_titulus);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, titulus);
    }
    chorda_aedificator_appendere_literis(a, "\";\n");
    si (latitudo.mensura > ZEPHYRUM)
    {
        si (!_numerum_legere(latitudo, &numerus))
        {
            sprintf(b, "<fenestra> (linea %d): latitudo non numerus",
                (integer)inv->fenestra->linea_initium);
            _recusare(f, piscina, b, inv->fenestra->linea_initium);
            redde _vacua();
        }
        sprintf(b, "    figura.latitudo = %d;\n", (integer)numerus);
        chorda_aedificator_appendere_literis(a, b);
    }
    si (altitudo.mensura > ZEPHYRUM)
    {
        si (!_numerum_legere(altitudo, &numerus))
        {
            sprintf(b, "<fenestra> (linea %d): altitudo non numerus",
                (integer)inv->fenestra->linea_initium);
            _recusare(f, piscina, b, inv->fenestra->linea_initium);
            redde _vacua();
        }
        sprintf(b, "    figura.altitudo = %d;\n", (integer)numerus);
        chorda_aedificator_appendere_literis(a, b);
    }
    chorda_aedificator_appendere_literis(a,
        "    figura.capsula  = &capsula_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ";\n"
        "    figura.visio    = \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ".visio.html\";\n"
        "    atrium_vexilla_legere(&figura, argc, argv);\n"
        "\n"
        "    atrium = atrium_creare(piscina, &figura, &causa);\n"
        "    si (atrium == NIHIL)\n"
        "    {\n"
        "        imprimere(\"FRACTA: %.*s\\n\","
        " (integer)causa.mensura,\n"
        "            (constans character*)causa.datum);\n"
        "        redde I;\n"
        "    }\n");
    per (i = ZEPHYRUM; i < inv->numerus_app; i++)
    {
        chorda methodus = briar_nexus_attributum(inv->app[i],
            "methodus");

        si (methodus.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (!_methodum_probare(piscina, inv->app[i], methodus, f))
        {
            redde _vacua();
        }
        chorda_aedificator_appendere_literis(a,
            "    (vacuum)internuntius_praebere("
            "atrium_internuntius(atrium),\n"
            "        \"");
        chorda_aedificator_appendere_chorda(a, methodus);
        chorda_aedificator_appendere_literis(a, "\", ");
        chorda_aedificator_appendere_chorda(a, methodus);
        chorda_aedificator_appendere_literis(a, ", NIHIL);\n");
    }
    chorda_aedificator_appendere_literis(a,
        "\n"
        "    imprimere(\"[");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "] fenestra aperta\\n\");\n"
        "    si (atrium_portus(atrium) != ZEPHYRUM)\n"
        "    {\n"
        "        imprimere(\"[");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "] imperium: http://127.0.0.1:%d/imperium\\n\",\n"
        "            (integer)atrium_portus(atrium));\n"
        "    }\n"
        "    fflush(stdout);\n"
        "    atrium_monstrare(atrium);\n"
        "    dum (atrium_currendum(atrium))\n"
        "    {\n"
        "        si (atrium_gressus(atrium)"
        " & (i32)ATRIUM_ACTUM_VISIO)\n"
        "        {\n"
        "            imprimere(\"[");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "] visio aperta\\n\");\n"
        "            fflush(stdout);\n"
        "        }\n"
        "    }\n"
        "    atrium_destruere(atrium);\n"
        "    piscina_destruere(piscina);\n"
        "    redde ZEPHYRUM;\n"
        "}\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_toml_fingere (
                      Piscina* piscina,
           constans character* titulus,
    constans BriarInventarium* inv)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)512);

    chorda_aedificator_appendere_literis(a,
        "# Capsula frontis (a briar genita) - aedificare.sh eam omni"
        " aedificatione regenerat\n\n");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_files = [\"index.html\"");
    si (inv->js != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, ", \"");
        chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, ".js\"");
    }
    si (inv->css != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, ", \"");
        chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, ".css\"");
    }
    chorda_aedificator_appendere_literis(a, "]\n");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_compress = false\n");
    redde chorda_aedificator_finire(a);
}

/* unio clausurarum per viam (dedup, ordo primae visionis) */
interior Xar*
_clausuras_fundere (
    Piscina* piscina,
        Xar* a,
        Xar* b,
        Xar* c)
{
               Xar* omnes;
    TabulaDispersa* viae;
               Xar* fontes[3];
               i32  j;

    omnes  = xar_creare(piscina, (i32)magnitudo(SilexRes));
    viae   = tabula_dispersa_creare_chorda(piscina, 64);

    fontes[0] = a;
    fontes[1] = b;
    fontes[2] = c;
    per (j = ZEPHYRUM; j < III; j++)
    {
        i32 i;

        si (fontes[j] == NIHIL)
        {
            perge;
        }
        per (i = ZEPHYRUM; i < xar_numerus(fontes[j]); i++)
        {
            SilexRes* r = (SilexRes*)xar_obtinere(fontes[j], i);

            si (tabula_dispersa_continet(viae, r->via))
            {
                perge;
            }
            tabula_dispersa_inserere(viae, r->via, (vacuum*)r);
            {
                SilexRes* cella = (SilexRes*)xar_addere(omnes);

                si (cella != NIHIL)
                {
                    *cella = *r;
                }
            }
        }
    }
    redde omnes;
}


/* ==================================================
 * Fabricare
 * ================================================== */

vacuum
briar_optiones_plagulae (
                 Piscina* piscina,
      constans SilexFons* fons,
      constans character* via,
    BriarFabricaOptiones* optiones)
{
    chorda plena = via_absoluta(chorda_ex_literis(via, piscina),
        piscina);

    optiones->via_thistle   = plena.mensura > ZEPHYRUM
        ? chorda_ut_cstr(plena, piscina) : via;
    optiones->fons_titulus  = fons->titulus;
    optiones->stampa        = fons->titulus;
}

BriarFabricaFructus
briar_fabricare (
                          Piscina* piscina,
            constans MateriaNodus* documentum,
                              Xar* nexus,
               constans SilexFons* fons,
    constans BriarFabricaOptiones* optiones,
                           chorda  octeti)
{
    redde briar_fabricare_cum_membris(piscina, documentum, nexus, fons,
        optiones, octeti, NIHIL);
}

/* lineae '#include "<m>_regiones.h"' membrorum tolluntur: clausura
 * corporis sola capita CORPORIS videt (membra genita sunt) */
interior chorda
_capita_membrorum_tollere (
    Piscina* piscina,
     chorda  textus,
        Xar* membra)
{
    ChordaAedificator* a;
                  i32  i = ZEPHYRUM;

    si (membra == NIHIL || xar_numerus(membra) == ZEPHYRUM)
    {
        redde textus;
    }
    a = chorda_aedificator_creare(piscina,
        (memoriae_index)(textus.mensura + 16));
    dum (i < textus.mensura)
    {
        i32 f = i;
        i32 k;
        b32 membri = FALSUM;
     chorda linea;

        dum (f < textus.mensura && textus.datum[f] != '\n')
        {
            f = f + I;
        }
        linea = chorda_sectio(textus, i, f);
        per (k = ZEPHYRUM; k < xar_numerus(membra) && !membri; k++)
        {
            constans BriarMembrum* m = (constans BriarMembrum*)
                xar_obtinere(membra, k);

            membri = chorda_aequalis(linea, chorda_ex_literis(
                _texere(piscina, "#include \"", m->titulus,
                "_regiones.h\""), piscina));
        }
        si (!membri)
        {
            chorda_aedificator_appendere_chorda(a, linea);
            chorda_aedificator_appendere_literis(a, "\n");
        }
        i = f + I;
    }
    redde chorda_aedificator_finire(a);
}

/* sedes prima nominis publici: 'via:linea' + linea si in RADICE */
nomen structura {
    chorda sedes;
       i32 linea_radicis;    /* ZEPHYRUM si membri */
} NominisSedes;

/* nomen publicum bis in aedificatione (radix + membra): refutatio
 * ANTE clang cum sedibus ambabus (spec par. 3.5); linea causae est
 * radicis si radix in pari est */
interior b32
_nomina_duplicata (
                Piscina* piscina,
                    Xar* nexus,
     constans character* via_radicis,
                    Xar* membra,
    BriarFabricaFructus* f)
{
    TabulaDispersa* visa = tabula_dispersa_creare_chorda(piscina, 128);
               s32  k;

    per (k = -I; k < (s32)xar_numerus(membra); k++)
    {
        constans BriarMembrum* m = k < ZEPHYRUM ? NIHIL
            : (constans BriarMembrum*)xar_obtinere(membra, (i32)k);
        constans character* via = m == NIHIL ? via_radicis : m->via;
                       Xar* nomina = m == NIHIL
                           ? briar_silva_nomina_publica(piscina,
                           nexus) : m->nomina;
                       i32 j;

        per (j = ZEPHYRUM; nomina != NIHIL && j < xar_numerus(nomina);
            j++)
        {
            constans BriarNomenPublicum* n =
                (constans BriarNomenPublicum*)
                xar_obtinere(nomina, j);
                             vacuum* prior = NIHIL;
                          character  b[32];

            si (!tabula_dispersa_invenire(visa, n->titulus, &prior))
            {
                ChordaAedificator* sedes = chorda_aedificator_creare(
                    piscina, (memoriae_index)128);

                chorda_aedificator_appendere_literis(sedes, via);
                sprintf(b, ":%d", (integer)n->linea);
                chorda_aedificator_appendere_literis(sedes, b);
                {
                    NominisSedes* c = (NominisSedes*)piscina_allocare(
                        piscina,
                        (memoriae_index)magnitudo(NominisSedes));

                    c->sedes          =
                        chorda_aedificator_finire(sedes);
                    c->linea_radicis  = k < ZEPHYRUM ? n->linea
                        : ZEPHYRUM;
                    tabula_dispersa_inserere(visa, n->titulus,
                        (vacuum*)c);
                }
                perge;
            }
            {
                ChordaAedificator* a =
                    chorda_aedificator_creare(piscina,
                    (memoriae_index)256);

                chorda_aedificator_appendere_literis(a, "nomen '");
                chorda_aedificator_appendere_chorda(a, n->titulus);
                chorda_aedificator_appendere_literis(a,
                    "' in duabus plagulis: ");
                chorda_aedificator_appendere_chorda(a,
                    ((NominisSedes*)prior)->sedes);
                chorda_aedificator_appendere_literis(a, ", ");
                chorda_aedificator_appendere_literis(a, via);
                sprintf(b, ":%d", (integer)n->linea);
                chorda_aedificator_appendere_literis(a, b);
                chorda_aedificator_appendere_literis(a,
                    " - nomen publicum unum per aedificationem;"
                    " alterum renomina aut 'interior' fac");
                f->causa        = chorda_aedificator_finire(a);
                f->linea_causae = k < ZEPHYRUM ? n->linea
                    : ((NominisSedes*)prior)->linea_radicis;
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

BriarFabricaFructus
briar_fabricare_cum_membris (
                          Piscina* piscina,
            constans MateriaNodus* documentum,
                              Xar* nexus,
               constans SilexFons* fons,
    constans BriarFabricaOptiones* optiones,
                           chorda  octeti,
                              Xar* membra)
{
        BriarFabricaFructus   f;
           BriarInventarium   inv;
              BriarPartitio   part;
                        i32   i;
         constans character*  via;
         constans character** fontes_app;
         constans character** fontes_prob;
                        i32   numerus_fontium;
                        i32   numerus_membrorum = membra != NIHIL
                            ? xar_numerus(membra) : ZEPHYRUM;
                     chorda  contenta_membrorum;
                        Xar* derivata_probationis;
              BriarPartitio  part_prob;
                     chorda  inclusiones_derivatae;
                     chorda  inclusiones_probationis;


    memset(&f, 0, magnitudo(f));
    si (   piscina == NIHIL || documentum == NIHIL || nexus == NIHIL
        || fons    == NIHIL || optiones == NIHIL)
    {
        _recusare(&f, piscina, "argumenta nulla", ZEPHYRUM);
        redde f;
    }
    via        = optiones->via_thistle;
    f.titulus  = briar_fabrica_titulus(piscina, via);
    si (!_inventarium_colligere(piscina, nexus, &inv, &f))
    {
        redde f;
    }
    f.regiones_c      = inv.numerus_app;
    f.probatio_adest  = (b32)(inv.probatio != NIHIL);

    memset(&part, 0, magnitudo(part));
    part.directivae = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.typi = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.prototypi = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
        part.corpora = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.derivata = xar_creare(piscina, (i32)magnitudo(chorda));

    per (i = ZEPHYRUM; i < inv.numerus_app; i++)
    {
        si (!_regionem_partiri(piscina, inv.app[i], &part, &f))
        {
            redde f;
        }
    }

    /* regula principalis */
    si (part.principalia > I)
    {
        character b[96];

        sprintf(b, "duo principalia: lineae %d et %d",
            (integer)part.princeps.linea,
            (integer)part.linea_principalis_secundi);
        _recusare(&f, piscina, b, part.linea_principalis_secundi);
        redde f;
    }
    si (part.principalia == I && inv.fenestra != NIHIL)
    {
        character b[96];

        sprintf(b,
            "<fenestra> (linea %d) et principale (linea %d): unum"
            " elige",
            (integer)inv.fenestra->linea_initium,
            (integer)part.princeps.linea);
        _recusare(&f, piscina, b, inv.fenestra->linea_initium);
        redde f;
    }
    si (part.principalia == ZEPHYRUM && inv.fenestra == NIHIL)
    {
        _recusare(&f, piscina,
            "nec principale in regione C nec <fenestra/>: nihil"
            " currendum",
            ZEPHYRUM);
        redde f;
    }
    f.forma = (part.principalia
        == I) ? BRIAR_FORMA_PLANA : BRIAR_FORMA_VITREA;

        /* regio probationis partita ut principales: capita derivata
         * (unitas sua ea includit) et prototypi adiutorum eius */
    derivata_probationis = xar_creare(piscina, (i32)magnitudo(chorda));
    memset(&part_prob, 0, magnitudo(part_prob));
    part_prob.directivae = xar_creare(piscina,
        (i32)magnitudo(BriarUnitas));
    part_prob.typi = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part_prob.prototypi = xar_creare(piscina,
        (i32)magnitudo(BriarUnitas));
    part_prob.corpora = xar_creare(piscina,
        (i32)magnitudo(BriarUnitas));
    part_prob.derivata = derivata_probationis;
    si (   inv.probatio != NIHIL
        && !_regionem_partiri(piscina, inv.probatio, &part_prob, &f))
    {
        redde f;
    }
    inclusiones_derivatae = _inclusiones_derivatae(piscina,
        part.derivata);
    inclusiones_probationis = _inclusiones_derivatae(piscina,
        derivata_probationis);
    f.genitae = xar_creare(piscina, (i32)magnitudo(BriarPlagula));
    /* caput in include/: -Iinclude ordinum id omnibus unitatibus
     * praebet (probationes/ quoque), fontes/ soli non */
    _genitam_addere(piscina, f.genitae,
        _texere(piscina, "include/", f.titulus, "_regiones.h"),
        _caput_fingere(piscina, f.titulus, via, &part));
    _genitam_addere(piscina, f.genitae,
        _texere(piscina, "fontes/", f.titulus, "_regiones.c"),
        _corpus_fingere(piscina, f.titulus, via, &part));
    si (inv.probatio != NIHIL)
    {
        _genitam_addere(piscina, f.genitae,
            _texere(piscina, "probationes/probatio_", f.titulus, ".c"),
                        _probationem_fingere(piscina, f.titulus, via,
                        inv.probatio,
                derivata_probationis, part_prob.prototypi));
    }

    /* membra (bibliotheca, spec par. 3.5): nomina publica unica per
     * aedificationem; unitates eorum in proiecto; fontes eorum in
     * ordinibus; textus eorum in clausura (capitibus membrorum
     * sublatis - genita sunt, corpus ea non novit) */
    si (   numerus_membrorum > ZEPHYRUM
        && !_nomina_duplicata(piscina, nexus, via, membra, &f))
    {
        redde f;
    }
    numerus_fontium = II + numerus_membrorum;
    fontes_app = (constans character**)piscina_allocare(piscina,
        (memoriae_index)((numerus_fontium + I)
            * (i32)magnitudo(constans character*)));
    fontes_prob = (constans character**)piscina_allocare(piscina,
        (memoriae_index)((numerus_fontium + I)
            * (i32)magnitudo(constans character*)));
    fontes_app[0] = _texere(piscina, "fontes/", f.titulus, ".c");
    fontes_app[1] = _texere(piscina, "fontes/", f.titulus,
        "_regiones.c");
    fontes_prob[0] = _texere(piscina, "probationes/probatio_",
        f.titulus,
        ".c");
    fontes_prob[1] = fontes_app[1];
    {
        ChordaAedificator* cm = chorda_aedificator_creare(piscina,
            (memoriae_index)4096);

        per (i = ZEPHYRUM; i < numerus_membrorum; i++)
        {
            constans BriarMembrum* m = (constans BriarMembrum*)
                xar_obtinere(membra, i);
            constans character* h = _texere(piscina, "include/",
                m->titulus, "_regiones.h");
            constans character* c = _texere(piscina, "fontes/",
                m->titulus, "_regiones.c");

            _genitam_addere(piscina, f.genitae, h, m->caput);
            _genitam_addere(piscina, f.genitae, c, m->corpus);
            fontes_app[II + i]   = c;
            fontes_prob[II + i]  = c;
            chorda_aedificator_appendere_chorda(cm,
                _capita_membrorum_tollere(piscina, m->caput, membra));
            chorda_aedificator_appendere_chorda(cm,
                _capita_membrorum_tollere(piscina, m->corpus, membra));
        }
        contenta_membrorum = chorda_aedificator_finire(cm);
    }
    inclusiones_derivatae = _capita_membrorum_tollere(piscina,
        inclusiones_derivatae, membra);
    inclusiones_probationis = _capita_membrorum_tollere(piscina,
        inclusiones_probationis, membra);

    si (f.forma == BRIAR_FORMA_PLANA)
    {
        chorda* contenta;
           i32  n = ZEPHYRUM;

        _genitam_addere(piscina, f.genitae, fontes_app[0],
            _principem_fingere(piscina, f.titulus, via,
            &part.princeps));
        /* clausura: regiones omnes (probatio inclusa - credo.h) */
                        contenta = (chorda*)piscina_allocare(piscina,
                            (memoriae_index)((inv.numerus_app + V)
                            * (i32)magnitudo(chorda)));
                per (i = ZEPHYRUM; i < inv.numerus_app; i++)
                {
            contenta[n]  = inv.app[i]->contextus;
            n            = n + I;
                }
        si (inv.probatio != NIHIL)
        {
            contenta[n]  = inv.probatio->contextus;
            n            = n + I;
        }
                contenta[n]  = inclusiones_derivatae;
        contenta[n + I]      = inclusiones_probationis;
        /* latina.h SEMPER: plagulae genitae eam includunt; clausura
         * vacua (scriptum libc solum) eam aliter non ferret */
        contenta[n + II] = _literae(piscina,
            "#include \"latina.h\"\n");
        contenta[n + III]  = contenta_membrorum;
        n                  = n + IV;
        f.clausura = silex_clausuram_e_contentis(piscina, fons,
            contenta, n);
        si (f.clausura == NIHIL)
        {
            _recusare(&f, piscina, "clausura bibliothecarum fracta",
                ZEPHYRUM);
            redde f;
        }
        /* clausura data: fontes lib expliciti (.c et .m) + frameworks
         * si Objective-C - globus bibliothecarum clausuram vacuam et
         * fenestram nativam fallebat (2026-09-05) */
        _genitam_addere(piscina, f.genitae, "aedificare.sh",
            silex_ordinem_fingere(piscina, f.titulus, fontes_app,
                numerus_fontium,
                f.clausura));
        si (inv.probatio != NIHIL)
        {
            _genitam_addere(piscina, f.genitae, "probare.sh",
                silex_ordinem_probandi_fingere(piscina, f.titulus,
                    fontes_prob, numerus_fontium, f.clausura));
        }
    }
    alioquin
    {
        chorda princeps = _principem_vitreum_fingere(piscina,
            f.titulus,
            via, &inv, &f);
        chorda  instrumentum;
        chorda* contenta_app;
        chorda* contenta_prob;
           Xar* clausura_app;
           Xar* clausura_instrumenti;
           Xar* clausura_prob  = NIHIL;
           b32  inventum       = FALSUM;
           i32  n;

        si (f.causa.mensura > ZEPHYRUM)
        {
            redde f;   /* recusatio methodi / attributi */
        }
        si (inv.html == NIHIL)
        {
            _recusare(&f, piscina,
                "<fenestra/> sine regione html: nihil monstrandum",
                inv.fenestra->linea_initium);
            redde f;
        }
        _genitam_addere(piscina, f.genitae, fontes_app[0], princeps);
        _genitam_addere(piscina, f.genitae, "assets/index.html",
            inv.html->contentum);
        si (inv.js != NIHIL)
        {
            _genitam_addere(piscina, f.genitae,
                _texere(piscina, "assets/", f.titulus, ".js"),
                inv.js->contentum);
        }
        si (inv.css != NIHIL)
        {
            _genitam_addere(piscina, f.genitae,
                _texere(piscina, "assets/", f.titulus, ".css"),
                inv.css->contentum);
        }
        _genitam_addere(piscina, f.genitae,
            _texere(piscina, "assets/", f.titulus, ".toml"),
            _toml_fingere(piscina, f.titulus, &inv));
        instrumentum = silex_fons_legere(fons,
            "tools/capsula_generare.c",
            piscina, &inventum);
        si (!inventum)
        {
            _recusare(&f, piscina,
                "tools/capsula_generare.c in fonte silicis deest",
                ZEPHYRUM);
            redde f;
        }
        _genitam_addere(piscina, f.genitae,
            "instrumenta/capsula_generare.c",
            instrumentum);

                contenta_app = (chorda*)piscina_allocare(piscina,
                    (memoriae_index)((inv.numerus_app + IV)
                    * (i32)magnitudo(chorda)));
        contenta_prob = (chorda*)piscina_allocare(piscina,
            (memoriae_index)((inv.numerus_app + V)
                * (i32)magnitudo(chorda)));
                per (i = ZEPHYRUM; i < inv.numerus_app; i++)
                {
            contenta_app[i]   = inv.app[i]->contextus;
            contenta_prob[i]  = inv.app[i]->contextus;
                }
                contenta_app[inv.numerus_app]  = princeps;
        contenta_app[inv.numerus_app + I]      = inclusiones_derivatae;
        contenta_app[inv.numerus_app + II]     = contenta_membrorum;
        clausura_app = silex_clausuram_e_contentis(piscina, fons,
            contenta_app, inv.numerus_app + III);
        clausura_instrumenti = silex_clausuram_e_contentis(piscina,
            fons,
            &instrumentum, I);
        n = inv.numerus_app;
        si (inv.probatio != NIHIL)
        {
                        contenta_prob[n]  = inv.probatio->contextus;
            contenta_prob[n + I]          = inclusiones_derivatae;
            contenta_prob[n + II]         = inclusiones_probationis;
            contenta_prob[n + III]        = contenta_membrorum;
            n                             = n + IV;
            clausura_prob = silex_clausuram_e_contentis(piscina, fons,
                contenta_prob, n);
        }
        si (   clausura_app == NIHIL || clausura_instrumenti == NIHIL
            || (inv.probatio != NIHIL && clausura_prob == NIHIL))
        {
            _recusare(&f, piscina, "clausura bibliothecarum fracta",
                ZEPHYRUM);
            redde f;
        }
        _genitam_addere(piscina, f.genitae, "aedificare.sh",
            silex_ordinem_vitreum_fingere(piscina, f.titulus,
            fontes_app, numerus_fontium,
                clausura_app, clausura_instrumenti,
                optiones->fons_titulus));
        si (inv.probatio != NIHIL)
        {
            _genitam_addere(piscina, f.genitae, "probare.sh",
                silex_ordinem_probandi_vitreum_fingere(piscina,
                f.titulus,
                    fontes_prob, numerus_fontium, clausura_prob,
                    optiones->fons_titulus));
        }
        f.clausura = _clausuras_fundere(piscina, clausura_app,
            clausura_instrumenti, clausura_prob);
    }

    briar_fabrica_clavem_computare(optiones->stampa,
        briar_fabrica_vexilla(f.forma), octeti, f.sigillum);
    f.successus = VERUM;
    redde f;
}


/* ==================================================
 * Visio (par. 4.9)
 * ================================================== */

b32
briar_visionem_addere (
                Piscina* piscina,
    BriarFabricaFructus* fructus,
                 chorda  pagina)
{
    constans character* via_toml;
                   i32  i;
                   i32  k;

    si (   piscina == NIHIL || fructus == NIHIL || !fructus->successus
        || fructus->forma != BRIAR_FORMA_VITREA
        || pagina.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    via_toml = _texere(piscina, "assets/", fructus->titulus, ".toml");
    per (i = ZEPHYRUM; i < xar_numerus(fructus->genitae); i++)
    {
        BriarPlagula* p = (BriarPlagula*)xar_obtinere(fructus->genitae,
            i);

        si (!chorda_aequalis_literis(p->via, via_toml))
        {
            perge;
        }
        /* lista _files (_toml_fingere) ']' PRIMO clauditur */
        per (k = ZEPHYRUM; k < p->contentum.mensura; k++)
        {
            ChordaAedificator* a;

            si ((character)p->contentum.datum[k] != ']')
            {
                perge;
            }
            a = chorda_aedificator_creare(piscina,
                (memoriae_index)(p->contentum.mensura + 64));
            chorda_aedificator_appendere_chorda(a,
                chorda_sectio(p->contentum, ZEPHYRUM, k));
            chorda_aedificator_appendere_literis(a, ", \"");
            chorda_aedificator_appendere_literis(a, fructus->titulus);
            chorda_aedificator_appendere_literis(a, ".visio.html\"");
            chorda_aedificator_appendere_chorda(a,
                chorda_sectio(p->contentum, k, p->contentum.mensura));
            p->contentum = chorda_aedificator_finire(a);
            _genitam_addere(piscina, fructus->genitae,
                _texere(piscina, "assets/", fructus->titulus,
                ".visio.html"), pagina);
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Scriptor
 * ================================================== */

interior b32
_plagulam_scribere (
               Piscina* piscina,
    constans character* radix,
                chorda  via,
                chorda  contentum,
                chorda* causa)
{
    constans character* plena = _texere(piscina, radix, "/",
        chorda_ut_cstr(via, piscina));
                chorda parens = via_directorium(_literae(piscina,
                    plena),
                    piscina);

    (vacuum)briar_directoria_creare(piscina, chorda_ut_cstr(parens,
        piscina));
    si (!filum_scribere(plena, contentum))
    {
        *causa = _literae(piscina, _texere(piscina, "non scripta: ",
            plena,
            NIHIL));
        redde FALSUM;
    }
    redde VERUM;
}

b32
briar_fabricam_scribere (
                         Piscina* piscina,
    constans BriarFabricaFructus* fructus,
              constans character* radix,
                          chorda* causa)
{
    i32 i;

    *causa = _vacua();
    si (fructus == NIHIL || !fructus->successus || radix == NIHIL)
    {
        *causa = _literae(piscina, "fructus non sanus");
        redde FALSUM;
    }
    si (!briar_directoria_creare(piscina, radix))
    {
        *causa = _literae(piscina, _texere(piscina,
            "directorium non creatum: ", radix, NIHIL));
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->clausura); i++)
    {
        constans SilexRes* r = (constans SilexRes*)xar_obtinere(
            fructus->clausura, i);

        si (!_plagulam_scribere(piscina, radix, r->via, r->contentum,
            causa))
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->genitae); i++)
    {
        constans BriarPlagula* p = (constans BriarPlagula*)xar_obtinere(
            fructus->genitae, i);

        si (!_plagulam_scribere(piscina, radix, p->via, p->contentum,
            causa))
        {
            redde FALSUM;
        }
        si (   p->contentum.mensura > II
            && memcmp(p->contentum.datum, "#!", (size_t)II) == ZEPHYRUM)
        {
            (vacuum)chmod(_texere(piscina, radix, "/",
                chorda_ut_cstr(p->via, piscina)), (mode_t)0755);
        }
    }
    redde VERUM;
}
