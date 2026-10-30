/* xml_coctum.c - Visio cocta xml (vide xml_coctum.h)
 *
 * Coctor unus per documentum: tabulae DTD, acervus coctionis, textus
 * pendens. Diagnostica omnia in c->diagnostica (MateriaDiagnosticum);
 * in fragmentis (textus substitutionis) sedes = lexema referentiae
 * (c->ancora), quia octeti fragmenti in fonte non iacent.
 */

#include "xml_coctum.h"
#include "xml_arbor.h"
#include "xml_lexicon.h"
#include "xml_registrum.h"
#include "xml_nomina.h"
#include "materia_diagnostica.h"
#include "materia_diagnosticum.h"
#include "materia_token.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "utf8.h"
#include <stdlib.h>
#include <string.h>

/* X4: limites expansionis entitatum (mensurati: omnis casus suitae et
 * silvestris intra eos cum margine; billion laughs eos excedit) */
#define XML_PROFUNDITAS_ENTITATUM  LXIV
#define XML_SUMMA_EXPANSA  ((memoriae_index)XVI * MXXIV * MXXIV)

#define XML_NS_XML    "http://www.w3.org/XML/1998/namespace"
#define XML_NS_XMLNS  "http://www.w3.org/2000/xmlns/"


/* ==================================================
 * Typi interni
 * ================================================== */

nomen structura {
                    chorda  titulus;
                       b32  parametri;
                       b32  externa;
                       b32  non_parsata;
                    chorda  valor;         /* textus substitutionis */
     constans MateriaToken* lexema;        /* titulus declarationis */
                       b32  in_expansione; /* recursio */
              MateriaNodus* fragmentum;    /* parsatus SEMEL */
                       b32  fragmentum_sanum;
} Entitas;

nomen structura {
                    chorda elementum;
                    chorda titulus;
                       b32 cdata;         /* typus CDATA */
                       b32 habet_valorem; /* praestitutum ordinarium aut
                                            * #FIXED */
                    chorda  valor;         /* sine citationibus */
     constans MateriaToken* lexema;
} AttributumDeclaratum;

nomen structura {
                  Piscina* piscina;
                      Xar* diagnostica;
    constans MateriaToken* ancora;     /* sedes in fragmento */
           TabulaDispersa* generales;
           TabulaDispersa* parametri;
                      Xar* attributa_declarata;
                      Xar* notationes;
                      b32  spatia;
                      b32  standalone;
                      b32  habet_doctype;
                      b32  dtd_externa;
                      b32  referentiae_parametri;
                      b32  cessatum;    /* par. 5.1 */
                      b32  limes_excessus;
                      i32  profunditas_entitatum;
           memoriae_index  summa_expansa;
                      b32  memoria_defecit;
} Coctor;


/* ==================================================
 * Diagnostica
 * ================================================== */

interior vacuum
_culpa (
                   Coctor* c,
    constans MateriaToken* lexema,
    constans MateriaNodus* nodus,
       constans character* codex,
       constans character* causa)
{
    MateriaDiagnosticum* d;

    d = (MateriaDiagnosticum*)xar_addere(c->diagnostica);
    si (d == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde;
    }
    memset(d, ZEPHYRUM, magnitudo(MateriaDiagnosticum));
    d->gravitas  = (s32)MATERIA_GRAVITAS_ERRATUM;
    d->codex     = codex;
    d->causa     = causa;
    si (c->ancora != NIHIL)
    {
        lexema  = c->ancora;
        nodus   = NIHIL;
    }
    d->nodus   = nodus;
    d->lexema  = lexema;
    si (lexema != NIHIL)
    {
        materia_tractus_lexematis(NIHIL, lexema, &d->tractus);
    }
    alioquin si (   nodus == NIHIL
                 || !materia_tractus_nodi(NIHIL, nodus, &d->tractus))
    {
        d->tractus.initium = (s32)-I;
    }
}


/* ==================================================
 * Adiutores chordarum et nodorum
 * ================================================== */

interior chorda
_chorda (
    constans i8* datum,
            i32  mensura)
{
    chorda c;
    unio { constans i8* c; i8* m; } u;

    u.c        = datum;
    c.datum    = u.m;
    c.mensura  = mensura;
    redde c;
}

interior chorda
_sectio (
    chorda c,
       i32 ab,
       i32 ad)
{
    si (ad > c.mensura)
    {
        ad = c.mensura;
    }
    si (ab > ad)
    {
        ab = ad;
    }
    redde _chorda(c.datum + ab, ad - ab);
}

interior b32
_aequat (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde c.mensura == n && (n == ZEPHYRUM
        || memcmp(c.datum, literae, (size_t)n) == ZEPHYRUM);
}

interior b32
_terminatur (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde c.mensura >= n && memcmp(c.datum + c.mensura - n, literae,
        (size_t)n) == ZEPHYRUM;
}

interior constans MateriaToken*
_token (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    constans MateriaValor* v = &nodus->loci[locus];

    redde v->genus == MATERIA_VALOR_TOKEN ? v->datum.token : NIHIL;
}

interior i32
_numerus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    constans MateriaValor* v = &nodus->loci[locus];

    redde v->genus == MATERIA_VALOR_LISTA
        ? materia_valor_lista_numerus(*v) : ZEPHYRUM;
}

interior constans MateriaValor*
_elementum_listae (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  k)
{
    redde materia_valor_lista_obtinere(nodus->loci[locus], k);
}

/* titulus valide Name totus? */
interior b32
_titulus_validus (
    chorda c)
{
    redde c.mensura > ZEPHYRUM
        && xml_titulum_metiri((constans character*)c.datum,
               (s32)c.mensura, FALSUM) == (s32)c.mensura;
}

interior b32
_spatium_octeti (
    i8 b)
{
    redde b == ' ' || b == '\t' || b == '\n' || b == '\r';
}

/* inter lexemata spatium (trivia) iacet? (octeti eiusdem fontis) */
interior b32
_spatium_inter (
    constans MateriaToken* prior,
    constans MateriaToken* sequens)
{
    redde prior != NIHIL && sequens != NIHIL
        && sequens->byte_offset > prior->byte_offset
            + (s32)prior->valor.mensura;
}

/* lineae normalizatae (CR LF et CR solus -> LF) in aedificatorem */
interior vacuum
_lineas_appendere (
    ChordaAedificator* a,
               chorda  c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] == '\r')
        {
            chorda_aedificator_appendere_character(a, '\n');
            si (i + I < c.mensura && c.datum[i + I] == '\n')
            {
                i++;
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a,
                (character)c.datum[i]);
        }
    }
}

interior chorda
_lineae_purgatae (
     Coctor* c,
     chorda  v)
{
     ChordaAedificator* a;
                   i32  i;

    per (i = ZEPHYRUM; i < v.mensura; i++)
    {
        si (v.datum[i] == '\r')
        {
            frange;
        }
    }
    si (i == v.mensura)
    {
        redde v;
    }
    a = chorda_aedificator_creare(c->piscina, (memoriae_index)v.mensura
        + I);
    si (a == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde v;
    }
    _lineas_appendere(a, v);
    redde chorda_aedificator_finire(a);
}

/* runam in aedificatorem (UTF-8) */
interior vacuum
_runam_appendere (
    ChordaAedificator* a,
                  s32  runa)
{
     i8 b[IV];
    s32 n = utf8_codere(runa, b);
    s32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        chorda_aedificator_appendere_character(a, (character)b[k]);
    }
}

/* referentia characteris '&#N;' aut '&#xH;' (r = textus ab '&');
 * reddit runam aut -I (forma mala), *longitudo = octeti */
interior s32
_referentia_characteris (
    chorda  r,
       i32* longitudo)
{
    i32 j     = II;
    s32 runa  = ZEPHYRUM;
    b32 hex   = FALSUM;
    i32 initium;

    si (r.mensura < IV || r.datum[ZEPHYRUM] != '&' || r.datum[I] != '#')
    {
        redde (s32)-I;
    }
    si (r.datum[j] == 'x')
    {
        hex = VERUM;
        j++;
    }
    initium = j;
    dum (j < r.mensura && r.datum[j] != ';')
    {
        character d = (character)r.datum[j];
              s32 v;

        si (d >= '0' && d <= '9')
        {
            v = (s32)(d - '0');
        }
        alioquin si (hex && d >= 'a' && d <= 'f')
        {
            v = (s32)(d - 'a') + X;
        }
        alioquin si (hex && d >= 'A' && d <= 'F')
        {
            v = (s32)(d - 'A') + X;
        }
        alioquin
        {
            redde (s32)-I;
        }
        runa = runa * (hex ? (s32)XVI : (s32)X) + v;
        si (runa > 0x10FFFF)
        {
            runa = 0x110000;   /* saturatio: extra Char */
        }
        j++;
    }
    si (j == initium || j >= r.mensura)
    {
        redde (s32)-I;
    }
    *longitudo = j + I;
    redde runa;
}

/* referentia '&titulus;' aut '%titulus;' ab r[0]: longitudo aut 0 */
interior i32
_referentia_tituli (
    chorda r)
{
    s32 n;

    si (r.mensura < III)
    {
        redde ZEPHYRUM;
    }
    n = xml_titulum_metiri((constans character*)r.datum + I,
        (s32)r.mensura - I, FALSUM);
    si (   n              == ZEPHYRUM || (i32)n + I >= r.mensura
        || r.datum[n + I] != ';')
    {
        redde ZEPHYRUM;
    }
    redde (i32)n + II;
}

/* entitates praedefinitae: runa aut -I */
interior s32
_entitas_praestituta (
    chorda titulus)
{
    si (_aequat(titulus, "lt"))
    { redde '<';
    }
    si (_aequat(titulus, "gt"))
    { redde '>';
    }
    si (_aequat(titulus, "amp"))
    { redde '&';
    }
    si (_aequat(titulus, "apos"))
    { redde '\'';
    }
    si (_aequat(titulus, "quot"))
    { redde '"';
    }
    redde (s32)-I;
}

/* WFC Entity Declared activa (par. 4.1): sine DTD, aut DTD interna
 * sola sine referentiis parametri, aut standalone="yes" */
interior b32
_ignota_vitium (
    constans Coctor* c)
{
    redde !c->habet_doctype || c->standalone
        || (!c->dtd_externa && !c->referentiae_parametri);
}

/* contentum citationis (VALOR sine citationibus); FALSUM si non
 * clausa */
interior b32
_citatum (
    constans MateriaToken* t,
                   chorda* contentum)
{
    chorda v = t->valor;

    si (   v.mensura < II || v.datum[ZEPHYRUM] != v.datum[v.mensura - I]
        || (v.datum[ZEPHYRUM] != '"' && v.datum[ZEPHYRUM] != '\''))
    {
        *contentum = _sectio(v, I, v.mensura);
        redde FALSUM;
    }
    *contentum = _sectio(v, I, v.mensura - I);
    redde VERUM;
}


/* ==================================================
 * GRADUS II: lexemata singula
 * ================================================== */

/* characteres: UTF-8 et Char (par. 2.2) */
interior vacuum
_characteres (
                   Coctor* c,
    constans MateriaToken* t)
{
    constans i8* p      = t->valor.datum;
    constans i8* finis  = p + t->valor.mensura;

    dum (p < finis)
    {
        s32 runa = utf8_decodere(&p, finis);

        si (runa < ZEPHYRUM)
        {
            _culpa(c, t, NIHIL, XML_CODEX_UTF8,
                "sequentia UTF-8 invalida");
            redde;
        }
        si (!xml_runa_licita(runa))
        {
            _culpa(c, t, NIHIL, XML_CODEX_CHARACTER,
                "character extra Char (XML 1.0 par. 2.2)");
            redde;
        }
    }
}

interior vacuum
_lexema_iudicare (
                   Coctor* c,
    constans MateriaToken* t)
{
    chorda v = t->valor;

    commutatio (t->genus)
    {
        casus XML_LEX_TEXTUS:
        casus XML_LEX_REFERENTIA:
        casus XML_LEX_TITULUS:
        casus XML_LEX_TAG_APERTURA:
        casus XML_LEX_TAG_CLAUSURA:
        casus XML_LEX_DECLARATIO:
        casus XML_LEX_REFERENTIA_PARAMETRI:
        casus XML_LEX_VERBUM_PRAEDEFINITUM:
        casus XML_LEX_DELIM:
            _characteres(c, t);
            redde;
        casus XML_LEX_VALOR:
        {
            chorda contentum;

            _characteres(c, t);
            si (!_citatum(t, &contentum))
            {
                _culpa(c, t, NIHIL, XML_CODEX_ATTRIBUTUM_FORMA,
                    "citatio non clausa");
            }
            redde;
        }
        casus XML_LEX_COMMENTARIUM:
        {
            chorda intus;
               i32 k;

            _characteres(c, t);
            si (v.mensura < VII || !_terminatur(v, "-->"))
            {
                _culpa(c, t, NIHIL, XML_CODEX_COMMENTARIUM,
                    "commentarium non clausum ('-->' deest)");
                redde;
            }
            intus = _sectio(v, IV, v.mensura - III);
            per (k = ZEPHYRUM; k + I < intus.mensura; k++)
            {
                si (intus.datum[k] == '-' && intus.datum[k + I] == '-')
                {
                    _culpa(c, t, NIHIL, XML_CODEX_COMMENTARIUM,
                        "'--' intra commentarium vetitum");
                    redde;
                }
            }
            si (   intus.mensura > ZEPHYRUM
                && intus.datum[intus.mensura - I] == '-')
            {
                _culpa(c, t, NIHIL, XML_CODEX_COMMENTARIUM,
                    "commentarium in '--->' desinere nequit");
            }
            redde;
        }
        casus XML_LEX_CDATA:
            _characteres(c, t);
            si (v.mensura < XII || !_terminatur(v, "]]>"))
            {
                _culpa(c, t, NIHIL, XML_CODEX_CDATA,
                    "sectio CDATA non clausa (']]>' deest)");
            }
            redde;
        casus XML_LEX_INSTRUCTIO:
        {
            s32 n;
            i32 post;

            _characteres(c, t);
            si (v.mensura < IV || !_terminatur(v, "?>"))
            {
                _culpa(c, t, NIHIL, XML_CODEX_INSTRUCTIO,
                    "instructio non clausa ('?>' deest)");
                redde;
            }
            n = xml_titulum_metiri((constans character*)v.datum + II,
                (s32)v.mensura - IV, FALSUM);
            si (n == ZEPHYRUM)
            {
                _culpa(c, t, NIHIL, XML_CODEX_INSTRUCTIO,
                    "scopus instructionis deest");
                redde;
            }
            si (   n == (s32)III
                && (v.datum[II] == 'x' || v.datum[II] == 'X')
                && (v.datum[III] == 'm' || v.datum[III] == 'M')
                && (v.datum[IV] == 'l' || v.datum[IV] == 'L'))
            {
                _culpa(c, t, NIHIL, XML_CODEX_INSTRUCTIO,
                    "scopus 'xml' reservatus (declaratio XML solum ad"
                    " initium)");
                redde;
            }
            post = (i32)n + II;
            si (   post < v.mensura - II
                && !_spatium_octeti(v.datum[post]))
            {
                _culpa(c, t, NIHIL, XML_CODEX_INSTRUCTIO,
                    "spatium inter scopum et data deest");
            }
            si (c->spatia)
            {
                s32 k;

                per (k = ZEPHYRUM; k < n; k++)
                {
                    si (v.datum[II + k] == ':')
                    {
                        _culpa(c, t, NIHIL, XML_CODEX_SPATIUM_NOMINUM,
                            "scopus instructionis ':' ferre nequit"
                            " (Namespaces 1.0)");
                        frange;
                    }
                }
            }
            redde;
        }
        ordinarius:
            redde;
    }
}

/* lexemata omnia arboris (ambulatio iterativa) */
interior vacuum
_lexemata_iudicare (
                   Coctor* c,
    constans MateriaNodus* radix)
{
    Xar* acervus = xar_creare(c->piscina,
                       (i32)magnitudo(constans MateriaNodus*));

    si (acervus == NIHIL || radix == NIHIL)
    {
        redde;
    }
    *(constans MateriaNodus**)xar_addere(acervus) = radix;
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        constans MateriaNodus* n =
            *(constans MateriaNodus**)xar_obtinere(
            acervus, xar_numerus(acervus) - I);
                          i32 l;

        xar_removere_ultimum(acervus);
        per (l = ZEPHYRUM; l < n->numerus_locorum; l++)
        {
            constans MateriaValor* v = &n->loci[l];

            si (v->genus == MATERIA_VALOR_TOKEN)
            {
                _lexema_iudicare(c, v->datum.token);
            }
            alioquin si (v->genus == MATERIA_VALOR_NODUS)
            {
                *(constans MateriaNodus**)xar_addere(acervus) =
                    v->datum.nodus;
            }
            alioquin si (v->genus == MATERIA_VALOR_LISTA)
            {
                i32 k;
                i32 m = materia_valor_lista_numerus(*v);

                per (k = ZEPHYRUM; k < m; k++)
                {
                    constans MateriaValor* e =
                        materia_valor_lista_obtinere(*v, k);

                    si (e->genus == MATERIA_VALOR_TOKEN)
                    {
                        _lexema_iudicare(c, e->datum.token);
                    }
                    alioquin si (e->genus == MATERIA_VALOR_NODUS)
                    {
                        *(constans MateriaNodus**)xar_addere(acervus) =
                            e->datum.nodus;
                    }
                }
            }
        }
    }
}


/* ==================================================
 * Declaratio XML (par. 2.8)
 * ================================================== */

interior b32
_versio_valida (
    chorda v)
{
    i32 k;

    si (   v.mensura < III || v.datum[ZEPHYRUM] != '1'
        || v.datum[I] != '.')
    {
        redde FALSUM;
    }
    per (k = II; k < v.mensura; k++)
    {
        si (v.datum[k] < '0' || v.datum[k] > '9')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* EncName ::= [A-Za-z] ([A-Za-z0-9._] | '-')* */
interior b32
_codificatio_valida (
    chorda v)
{
    i32 k;

    si (v.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < v.mensura; k++)
    {
        character d = (character)v.datum[k];
              b32 littera;

        littera = (d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z');

        si (k == ZEPHYRUM ? !littera
            : !(littera || (d >= '0' && d <= '9') || d == '.'
                || d == '_'
                || d == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_codificatio_utf8 (
    chorda v)
{
    character b[XVI];
          i32 k;

    si (v.mensura >= (i32)XVI)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < v.mensura; k++)
    {
        character d = (character)v.datum[k];

        b[k] = (d >= 'A' && d <= 'Z') ? (character)(d - 'A' + 'a') : d;
    }
    b[k] = '\0';
    redde strcmp(b, "utf-8") == ZEPHYRUM
        || strcmp(b, "us-ascii") == ZEPHYRUM
        || strcmp(b, "ascii") == ZEPHYRUM;
}

interior vacuum
_declarationem_xml_iudicare (
                   Coctor* c,
    constans MateriaNodus* d)
{
    i32 n = _numerus(d, (i32)XML_DECLARATIO_XML_ATTRIBUTA);
    i32 k;
    /* gradus: 0 version, 1 encoding, 2 standalone */
                      i32  gradus = ZEPHYRUM;
    constans MateriaToken* prior = _token(d,
        (i32)XML_DECLARATIO_XML_TOK_APERTURA);

    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaNodus* a = _elementum_listae(d,
            (i32)XML_DECLARATIO_XML_ATTRIBUTA, k)->datum.nodus;
        constans MateriaToken* t;
        constans MateriaToken* v;
                       chorda  valor;

        si (a->genus != (s32)XML_GENUS_ATTRIBUTUM)
        {
            redde;   /* malum: derivatum */
        }
        t = _token(a, (i32)XML_ATTRIBUTUM_TOK_TITULUS);
        v = _token(a, (i32)XML_ATTRIBUTUM_TOK_VALOR);
        si (   v == NIHIL || _token(a, (i32)XML_ATTRIBUTUM_TOK_AEQUALE)
            == NIHIL)
        {
            redde;   /* absentia: derivata */
        }
        si (!_spatium_inter(prior, t))
        {
            _culpa(c, t, a, XML_CODEX_DECLARATIO_XML,
                "spatium ante pseudo-attributum deest");
            redde;
        }
        prior = v;
        si (!_citatum(v, &valor))
        {
            redde;   /* lexema iudicat */
        }
        si (_aequat(t->valor, "version") && gradus == ZEPHYRUM)
        {
            si (!_versio_valida(valor))
            {
                _culpa(c, v, a, XML_CODEX_DECLARATIO_XML,
                    "versio mala (VersionNum '1.' [0-9]+)");
            }
            gradus = I;
        }
        alioquin si (_aequat(t->valor, "encoding") && gradus == I)
        {
            si (!_codificatio_valida(valor))
            {
                _culpa(c, v, a, XML_CODEX_DECLARATIO_XML,
                    "nomen codificationis malum (EncName)");
            }
            alioquin si (!_codificatio_utf8(valor))
            {
                _culpa(c, v, a, XML_CODEX_CODIFICATIO,
                    "codificatio non UTF-8 non sustinetur (spec X8)");
            }
            gradus = II;
        }
        alioquin si (   _aequat(t->valor, "standalone") && gradus >= I
                     && gradus <= II)
        {
            si (_aequat(valor, "yes"))
            {
                c->standalone = VERUM;
            }
            alioquin si (!_aequat(valor, "no"))
            {
                _culpa(c, v, a, XML_CODEX_DECLARATIO_XML,
                    "standalone 'yes' aut 'no' esse debet");
            }
            gradus = III;
        }
        alioquin
        {
            _culpa(c, t, a, XML_CODEX_DECLARATIO_XML,
                "pseudo-attributum ignotum aut ordine alieno (version,"
                " encoding, standalone)");
            redde;
        }
    }
    si (gradus == ZEPHYRUM)
    {
        _culpa(c, _token(d, (i32)XML_DECLARATIO_XML_TOK_APERTURA), d,
            XML_CODEX_DECLARATIO_XML, "version deest");
    }
}


/* ==================================================
 * DOCTYPE: id externum (par. 2.8, 4.2.2)
 * ================================================== */

/* PubidLiteral: characteres PubidChar */
interior b32
_pubid_validum (
    chorda v)
{
    i32 k;

    per (k = ZEPHYRUM; k < v.mensura; k++)
    {
        si (!xml_runa_pubid((s32)(insignatus character)v.datum[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Cursor lexematum (partes declarationis, id externum)
 * ================================================== */

nomen structura {
    constans MateriaNodus* nodus;
                      i32  locus;
                      i32  numerus;
                      i32  k;
    constans MateriaToken* prior;
} Cursor;

interior vacuum
_cursor (
                   Cursor* u,
    constans MateriaNodus* nodus,
                      i32  locus,
    constans MateriaToken* prior)
{
    u->nodus    = nodus;
    u->locus    = locus;
    u->numerus  = _numerus(nodus, locus);
    u->k        = ZEPHYRUM;
    u->prior    = prior;
}

interior constans MateriaToken*
_videre (
    constans Cursor* u)
{
    redde u->k < u->numerus
        ? _elementum_listae(u->nodus, u->locus, u->k)->datum.token
        : NIHIL;
}

interior constans MateriaToken*
_capere (
    Cursor* u)
{
    constans MateriaToken* t = _videre(u);

    si (t != NIHIL)
    {
        u->k++;
        u->prior = t;
    }
    redde t;
}

interior b32
_est (
    constans MateriaToken* t,
                      s32  genus,
       constans character* literae)
{
    redde t != NIHIL && t->genus == genus
        && (literae == NIHIL || _aequat(t->valor, literae));
}

/* lexema proximum, cum spatio ante (exigitur) */
interior constans MateriaToken*
_capere_post_spatium (
    Cursor* u)
{
    constans MateriaToken* t = _videre(u);

    si (t == NIHIL || !_spatium_inter(u->prior, t))
    {
        redde NIHIL;
    }
    redde _capere(u);
}


/* ==================================================
 * Id externum: SYSTEM 's' | PUBLIC 'p' 's' (NOTATION: PUBLIC 'p')
 * ================================================== */

nomen structura {
    chorda publicum;
    chorda systema;
       b32 habet_publicum;
       b32 habet_systema;
} IdExternum;

/* verbum clavis iam visum (SYSTEM aut PUBLIC) in u->videre; FALSUM =
 * forma mala (diagnosticum vocantis) */
interior b32
_id_externum (
                   Coctor* c,
                   Cursor* u,
                      b32  publicum_solum,
               IdExternum* id)
{
    constans MateriaToken* t = _capere(u);
    constans MateriaToken* l;

    memset(id, ZEPHYRUM, magnitudo(*id));
    si (_est(t, (s32)XML_LEX_TITULUS, "SYSTEM"))
    {
        l = _capere_post_spatium(u);
        si (   !_est(l, (s32)XML_LEX_VALOR, NIHIL)
            || !_citatum(l, &id->systema))
        {
            redde FALSUM;
        }
        id->habet_systema = VERUM;
        redde VERUM;
    }
    si (!_est(t, (s32)XML_LEX_TITULUS, "PUBLIC"))
    {
        redde FALSUM;
    }
    l = _capere_post_spatium(u);
    si (   !_est(l, (s32)XML_LEX_VALOR, NIHIL)
        || !_citatum(l, &id->publicum))
    {
        redde FALSUM;
    }
    si (!_pubid_validum(id->publicum))
    {
        _culpa(c, l, NIHIL, XML_CODEX_DECLARATIO,
            "identificator publicus characterem extra PubidChar fert");
        redde FALSUM;
    }
    id->habet_publicum  = VERUM;
    l                   = _videre(u);
    si (_est(l, (s32)XML_LEX_VALOR, NIHIL))
    {
        si (!_spatium_inter(u->prior, l))
        {
            redde FALSUM;
        }
        (vacuum)_capere(u);
        si (!_citatum(l, &id->systema))
        {
            redde FALSUM;
        }
        id->habet_systema = VERUM;
        redde VERUM;
    }
    redde publicum_solum;
}


/* ==================================================
 * Valores: entitatis (ad declarationem) et attributi (par. 3.3.3)
 * ================================================== */

/* EntityValue: characterum referentiae expanduntur, entitatum
 * relinquuntur (praeteritae), '%' parametri in parte interna vetitum
 * (WFC: PEs in Internal Subset); lineae normantur. FALSUM = vitium. */
interior b32
_valorem_entitatis (
                   Coctor* c,
    constans MateriaToken* t,
                   chorda  crudus,
                   chorda* exitus)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
                               (memoriae_index)crudus.mensura + I);
                   i32 i;

    si (a == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < crudus.mensura; i++)
    {
        i8 b = crudus.datum[i];

        si (b == '%')
        {
            si (_referentia_tituli(_sectio(crudus, i, crudus.mensura))
                == ZEPHYRUM)
            {
                _culpa(c, t, NIHIL, XML_CODEX_REFERENTIA,
                    "'%' sine referentia parametri in valore"
                    " entitatis");
            }
            alioquin
            {
                _culpa(c, t, NIHIL, XML_CODEX_PARAMETRUM_INTRA,
                    "referentia parametri intra declarationem in parte"
                    " interna (WFC: PEs in Internal Subset)");
            }
            c->referentiae_parametri = VERUM;
            redde FALSUM;
        }
        si (b == '&')
        {
            chorda r          = _sectio(crudus, i, crudus.mensura);
               i32 longitudo  = ZEPHYRUM;
               s32 runa;

            si (crudus.mensura > i + I && crudus.datum[i + I] == '#')
            {
                runa = _referentia_characteris(r, &longitudo);
                si (runa < ZEPHYRUM || !xml_runa_licita(runa))
                {
                    _culpa(c, t, NIHIL, XML_CODEX_REFERENTIA,
                        "referentia characteris mala aut extra Char");
                    redde FALSUM;
                }
                _runam_appendere(a, runa);
                i += longitudo - I;
                perge;
            }
            longitudo = _referentia_tituli(r);
            si (longitudo == ZEPHYRUM)
            {
                _culpa(c, t, NIHIL, XML_CODEX_REFERENTIA,
                    "'&' sine referentia in valore entitatis");
                redde FALSUM;
            }
            chorda_aedificator_appendere_chorda(a, _sectio(r, ZEPHYRUM,
                longitudo));
            i += longitudo - I;
            perge;
        }
        si (b == '\r')
        {
            chorda_aedificator_appendere_character(a, '\n');
            si (i + I < crudus.mensura && crudus.datum[i + I] == '\n')
            {
                i++;
            }
            perge;
        }
        chorda_aedificator_appendere_character(a, (character)b);
    }
    *exitus = chorda_aedificator_finire(a);
    redde VERUM;
}

interior Entitas*
_entitas (
    Coctor* c,
       b32  parametri,
    chorda  titulus)
{
    vacuum* v = NIHIL;

    TabulaDispersa* tabula = parametri ? c->parametri : c->generales;

    si (tabula_dispersa_invenire(tabula, titulus, &v))
    {
        redde (Entitas*)v;
    }
    redde NIHIL;
}

/* Normalizatio valoris attributi (par. 3.3.3) in aedificatorem:
 * referentiae characterum runam dant (non normatam), entitates
 * internae recursive, S -> ' ' (CR LF fontis documenti -> ' ' unum).
 * '<' vetitum (WFC: No < in Attribute Values), entitates externae
 * (WFC: No External Entity References) et non parsatae vetitae.
 * FALSUM = vitium (iam nuntiatum). */
interior b32
_valorem_normare (
                   Coctor* c,
    constans MateriaToken* t,
                   chorda  crudus,
                      b32  fons_documenti,
       ChordaAedificator* a)
{
    i32 i;

    per (i = ZEPHYRUM; i < crudus.mensura; i++)
    {
        i8 b = crudus.datum[i];

        si (b == '<')
        {
            _culpa(c, t, NIHIL, XML_CODEX_MINOR_IN_VALORE,
                "'<' in valore attributi (WFC: No < in Attribute"
                " Values)");
            redde FALSUM;
        }
        si (b == '&')
        {
            chorda  r          = _sectio(crudus, i, crudus.mensura);
               i32  longitudo  = ZEPHYRUM;
               s32  runa;
            chorda  titulus;
           Entitas* e;

            si (crudus.mensura > i + I && crudus.datum[i + I] == '#')
            {
                runa = _referentia_characteris(r, &longitudo);
                si (runa < ZEPHYRUM || !xml_runa_licita(runa))
                {
                    _culpa(c, t, NIHIL, XML_CODEX_REFERENTIA,
                        "referentia characteris mala aut extra Char");
                    redde FALSUM;
                }
                _runam_appendere(a, runa);
                i += longitudo - I;
                perge;
            }
            longitudo = _referentia_tituli(r);
            si (longitudo == ZEPHYRUM)
            {
                _culpa(c, t, NIHIL, XML_CODEX_REFERENTIA,
                    "'&' sine referentia in valore attributi");
                redde FALSUM;
            }
            i        += longitudo - I;
            titulus  = _sectio(r, I, longitudo - I);
            runa     = _entitas_praestituta(titulus);
            si (runa >= ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a,
                    (character)runa);
                perge;
            }
            e = _entitas(c, FALSUM, titulus);
            si (e == NIHIL)
            {
                si (_ignota_vitium(c))
                {
                    _culpa(c, t, NIHIL, XML_CODEX_ENTITAS_IGNOTA,
                        "entitas non declarata (WFC: Entity Declared)");
                    redde FALSUM;
                }
                perge;
            }
            si (e->externa || e->non_parsata)
            {
                _culpa(c, t, NIHIL,
                    e->non_parsata ? XML_CODEX_NON_PARSATA
                    : XML_CODEX_EXTERNA_IN_VALORE,
                    "entitas externa aut non parsata in valore"
                    " attributi (WFC: No External Entity References)");
                redde FALSUM;
            }
            si (e->in_expansione)
            {
                _culpa(c, t, NIHIL, XML_CODEX_RECURSIO,
                    "entitas se ipsam refert (WFC: No Recursion)");
                redde FALSUM;
            }
            si (   c->profunditas_entitatum >= XML_PROFUNDITAS_ENTITATUM
                || c->summa_expansa + (memoriae_index)e->valor.mensura
                    > XML_SUMMA_EXPANSA)
            {
                si (!c->limes_excessus)
                {
                    _culpa(c, t, NIHIL, XML_CODEX_LIMES,
                        "expansio entitatum limitem excedit (spec X4)");
                }
                c->limes_excessus = VERUM;
                redde FALSUM;
            }
            c->summa_expansa += (memoriae_index)e->valor.mensura;
            c->profunditas_entitatum++;
            e->in_expansione = VERUM;
            {
                b32 bene = _valorem_normare(c, t, e->valor, FALSUM, a);

                e->in_expansione = FALSUM;
                c->profunditas_entitatum--;
                si (!bene)
                {
                    redde FALSUM;
                }
            }
            perge;
        }
        si (b == ' ' || b == '\t' || b == '\n' || b == '\r')
        {
            chorda_aedificator_appendere_character(a, ' ');
            si (   fons_documenti && b == '\r' && i + I < crudus.mensura
                && crudus.datum[i + I] == '\n')
            {
                i++;
            }
            perge;
        }
        chorda_aedificator_appendere_character(a, (character)b);
    }
    redde VERUM;
}

/* spatia contracta et ab extremis ablata (typi non CDATA) */
interior chorda
_contrahere (
     Coctor* c,
     chorda  v)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
                               (memoriae_index)v.mensura + I);
                   i32 i;
                   b32 spatium = FALSUM;

    si (a == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde v;
    }
    per (i = ZEPHYRUM; i < v.mensura; i++)
    {
        si (v.datum[i] == ' ')
        {
            spatium = VERUM;
            perge;
        }
        si (spatium && chorda_aedificator_longitudo(a) > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        spatium = FALSUM;
        chorda_aedificator_appendere_character(a,
            (character)v.datum[i]);
    }
    redde chorda_aedificator_finire(a);
}


/* ==================================================
 * Declarationes DTD (par. 3.2, 3.3, 4.2, 4.7)
 * ================================================== */

interior b32
_titulus_lexematis (
    constans MateriaToken* t)
{
    redde _est(t, (s32)XML_LEX_TITULUS, NIHIL)
        && _titulus_validus(t->valor);
}

interior b32
_signum (
    constans MateriaToken* t,
                character  d)
{
    redde t != NIHIL && t->genus == (s32)XML_LEX_DELIM
        && t->valor.mensura == I && t->valor.datum[ZEPHYRUM] == (i8)d;
}

/* signum frequentiae ('?' '*' '+') statim post (sine spatio) */
interior vacuum
_frequentia (
    Cursor* u)
{
    constans MateriaToken* t = _videre(u);

    si (   (_signum(t, '?') || _signum(t, '*') || _signum(t, '+'))
        && !_spatium_inter(u->prior, t))
    {
        (vacuum)_capere(u);
    }
}

interior b32
_grex (
    Cursor* u,
       i32  profunditas);

/* cp ::= (Name | choice | seq) ('?' | '*' | '+')? */
interior b32
_particula (
    Cursor* u,
       i32  profunditas)
{
    constans MateriaToken* t = _capere(u);

    si (_titulus_lexematis(t))
    {
        _frequentia(u);
        redde VERUM;
    }
    si (_signum(t, '(') && profunditas < (i32)CXXVIII)
    {
        si (!_grex(u, profunditas + I))
        {
            redde FALSUM;
        }
        _frequentia(u);
        redde VERUM;
    }
    redde FALSUM;
}

/* post '(': cp ( (',' cp)* | ('|' cp)+ ) ')' - separator unus */
interior b32
_grex (
    Cursor* u,
       i32  profunditas)
{
    character separator = '\0';

    si (!_particula(u, profunditas))
    {
        redde FALSUM;
    }
    dum (VERUM)
    {
        constans MateriaToken* t = _capere(u);

        si (_signum(t, ')'))
        {
            redde VERUM;
        }
        si (!_signum(t, ',') && !_signum(t, '|'))
        {
            redde FALSUM;
        }
        si (separator == '\0')
        {
            separator = (character)t->valor.datum[ZEPHYRUM];
        }
        alioquin si (separator != (character)t->valor.datum[ZEPHYRUM])
        {
            redde FALSUM;
        }
        si (!_particula(u, profunditas))
        {
            redde FALSUM;
        }
    }
}

/* contentspec post '(' : Mixed aut children */
interior b32
_exemplar (
    Cursor* u)
{
    constans MateriaToken* t       = _videre(u);
                      i32  tituli  = ZEPHYRUM;

    si (!_est(t, (s32)XML_LEX_VERBUM_PRAEDEFINITUM, "#PCDATA"))
    {
        si (!_grex(u, I))
        {
            redde FALSUM;
        }
        _frequentia(u);
        redde VERUM;
    }
    (vacuum)_capere(u);
    dum (VERUM)
    {
        t = _capere(u);
        si (_signum(t, ')'))
        {
            constans MateriaToken* s = _videre(u);

            si (_signum(s, '*') && !_spatium_inter(t, s))
            {
                (vacuum)_capere(u);
                redde VERUM;
            }
            redde tituli == ZEPHYRUM;
        }
        si (!_signum(t, '|') || !_titulus_lexematis(_capere(u)))
        {
            redde FALSUM;
        }
        tituli++;
    }
}

interior b32
_declaratio_elementi (
                   Coctor* c,
                   Cursor* u)
{
    constans MateriaToken* t;

    (vacuum)c;
    t = _capere_post_spatium(u);
    si (!_titulus_lexematis(t))
    {
        redde FALSUM;
    }
    t = _capere_post_spatium(u);
    si (   _est(t, (s32)XML_LEX_TITULUS, "EMPTY")
        || _est(t, (s32)XML_LEX_TITULUS, "ANY"))
    {
        redde _videre(u) == NIHIL;
    }
    si (!_signum(t, '(') || !_exemplar(u))
    {
        redde FALSUM;
    }
    redde _videre(u) == NIHIL;
}

hic_manens constans character* constans TYPI_ATTRIBUTORUM[] = {
    "CDATA", "ID", "IDREF", "IDREFS", "ENTITY", "ENTITIES", "NMTOKEN",
    "NMTOKENS", NIHIL
};

/* '(' S? x (S? '|' S? x)* S? ')' - x Name (NOTATION) aut Nmtoken */
interior b32
_enumeratio (
    Cursor* u,
       b32  tituli)
{
    dum (VERUM)
    {
        constans MateriaToken* t = _capere(u);

        si (   !_est(t, (s32)XML_LEX_TITULUS, NIHIL)
            || (tituli && !_titulus_validus(t->valor)))
        {
            redde FALSUM;
        }
        t = _capere(u);
        si (_signum(t, ')'))
        {
            redde VERUM;
        }
        si (!_signum(t, '|'))
        {
            redde FALSUM;
        }
    }
}

interior b32
_declaratio_attributorum (
                   Coctor* c,
                   Cursor* u)
{
    constans MateriaToken* elementum = _capere_post_spatium(u);

    si (!_titulus_lexematis(elementum))
    {
        redde FALSUM;
    }
    dum (_videre(u) != NIHIL)
    {
        constans MateriaToken* titulus = _capere_post_spatium(u);
        constans MateriaToken* typus;
        constans MateriaToken* t;
         AttributumDeclaratum  ad;
                          i32  k;
                          b32  notus = FALSUM;

        si (!_titulus_lexematis(titulus))
        {
            redde FALSUM;
        }
        memset(&ad, ZEPHYRUM, magnitudo(ad));
        typus = _capere_post_spatium(u);
        si (_est(typus, (s32)XML_LEX_TITULUS, "NOTATION"))
        {
            t = _capere_post_spatium(u);
            si (!_signum(t, '(') || !_enumeratio(u, VERUM))
            {
                redde FALSUM;
            }
        }
        alioquin si (_signum(typus, '('))
        {
            si (!_enumeratio(u, FALSUM))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            per (k = ZEPHYRUM; TYPI_ATTRIBUTORUM[k] != NIHIL; k++)
            {
                si (_est(typus, (s32)XML_LEX_TITULUS,
                    TYPI_ATTRIBUTORUM[k]))
                {
                    notus = VERUM;
                }
            }
            si (!notus)
            {
                redde FALSUM;
            }
            ad.cdata = _est(typus, (s32)XML_LEX_TITULUS, "CDATA");
        }
        t = _capere_post_spatium(u);
        si (   _est(t, (s32)XML_LEX_VERBUM_PRAEDEFINITUM, "#REQUIRED")
            || _est(t, (s32)XML_LEX_VERBUM_PRAEDEFINITUM, "#IMPLIED"))
        {
            ad.habet_valorem = FALSUM;
        }
        alioquin
        {
            si (_est(t, (s32)XML_LEX_VERBUM_PRAEDEFINITUM, "#FIXED"))
            {
                t = _capere_post_spatium(u);
            }
            si (   !_est(t, (s32)XML_LEX_VALOR, NIHIL)
                || !_citatum(t, &ad.valor))
            {
                redde FALSUM;
            }
            ad.habet_valorem = VERUM;
            /* valor praestitutus iam nunc iudicatur (entitates ante
             * declaratae, '<', externae) */
            {
                ChordaAedificator* a = chorda_aedificator_creare(
                                           c->piscina, LXIV);

                si (a == NIHIL)
                {
                    c->memoria_defecit = VERUM;
                    redde FALSUM;
                }
                si (!_valorem_normare(c, t, ad.valor, VERUM, a))
                {
                    /* nuntiatum; declaratio forma bona */
                    redde VERUM;
                }
            }
        }
        si (!c->cessatum)
        {
            b32 iam = FALSUM;

            per (k = ZEPHYRUM; k < xar_numerus(c->attributa_declarata);
                 k++)
            {
                AttributumDeclaratum* p = (AttributumDeclaratum*)
                    xar_obtinere(c->attributa_declarata, k);

                si (   chorda_aequalis(p->elementum, elementum->valor)
                    && chorda_aequalis(p->titulus, titulus->valor))
                {
                    iam = VERUM;
                }
            }
            si (!iam)
            {
                AttributumDeclaratum* p = (AttributumDeclaratum*)
                    xar_addere(c->attributa_declarata);

                si (p == NIHIL)
                {
                    c->memoria_defecit = VERUM;
                    redde FALSUM;
                }
                *p            = ad;
                p->elementum  = elementum->valor;
                p->titulus    = titulus->valor;
                p->lexema     = t;
            }
        }
    }
    redde VERUM;
}

interior b32
_declaratio_entitatis (
                   Coctor* c,
                   Cursor* u)
{
    constans MateriaToken* t = _capere_post_spatium(u);
    constans MateriaToken* titulus;
                  Entitas  e;
                      b32  parametri = FALSUM;

    memset(&e, ZEPHYRUM, magnitudo(e));
    si (_signum(t, '%'))
    {
        parametri  = VERUM;
        t          = _capere_post_spatium(u);
    }
    titulus = t;
    si (!_titulus_lexematis(titulus))
    {
        redde FALSUM;
    }
    si (c->spatia)
    {
        i32 k;

        per (k = ZEPHYRUM; k < titulus->valor.mensura; k++)
        {
            si (titulus->valor.datum[k] == ':')
            {
                _culpa(c, titulus, NIHIL, XML_CODEX_SPATIUM_NOMINUM,
                    "titulus entitatis ':' ferre nequit (Namespaces"
                    " 1.0)");
                frange;
            }
        }
    }
    t = _videre(u);
    si (t == NIHIL || !_spatium_inter(u->prior, t))
    {
        redde FALSUM;
    }
    si (_est(t, (s32)XML_LEX_VALOR, NIHIL))
    {
        chorda crudus;

        (vacuum)_capere(u);
        si (!_citatum(t, &crudus))
        {
            redde FALSUM;
        }
        si (!_valorem_entitatis(c, t, crudus, &e.valor))
        {
            redde VERUM;   /* nuntiatum */
        }
    }
    alioquin
    {
        IdExternum id;

        si (!_id_externum(c, u, FALSUM, &id))
        {
            redde FALSUM;
        }
        e.externa  = VERUM;
        t          = _videre(u);
        si (_est(t, (s32)XML_LEX_TITULUS, "NDATA"))
        {
            si (parametri || !_spatium_inter(u->prior, t))
            {
                redde FALSUM;
            }
            (vacuum)_capere(u);
            si (!_titulus_lexematis(_capere_post_spatium(u)))
            {
                redde FALSUM;
            }
            e.non_parsata = VERUM;
        }
    }
    si (_videre(u) != NIHIL)
    {
        redde FALSUM;
    }
    si (!c->cessatum && _entitas(c, parametri, titulus->valor) == NIHIL)
    {
        Entitas* nova = (Entitas*)piscina_allocare(c->piscina,
                            (memoriae_index)magnitudo(Entitas));

        si (nova == NIHIL)
        {
            c->memoria_defecit = VERUM;
            redde FALSUM;
        }
        *nova            = e;
        nova->titulus    = titulus->valor;
        nova->parametri  = parametri;
        nova->lexema     = titulus;
        si (!tabula_dispersa_inserere(parametri ? c->parametri
            : c->generales, titulus->valor, nova))
        {
            c->memoria_defecit = VERUM;
        }
    }
    redde VERUM;
}

interior b32
_declaratio_notationis (
                   Coctor* c,
                   Cursor* u)
{
     constans MateriaToken* titulus = _capere_post_spatium(u);
                IdExternum  id;

    si (!_titulus_lexematis(titulus))
    {
        redde FALSUM;
    }
    si (   _videre(u) == NIHIL || !_spatium_inter(u->prior, _videre(u))
        || !_id_externum(c, u, VERUM, &id) || _videre(u) != NIHIL)
    {
        redde FALSUM;
    }
    si (c->spatia)
    {
        i32 k;

        per (k = ZEPHYRUM; k < titulus->valor.mensura; k++)
        {
            si (titulus->valor.datum[k] == ':')
            {
                _culpa(c, titulus, NIHIL, XML_CODEX_SPATIUM_NOMINUM,
                    "titulus notationis ':' ferre nequit (Namespaces"
                    " 1.0)");
                frange;
            }
        }
    }
    {
        XmlNotatio* n = (XmlNotatio*)xar_addere(c->notationes);
               i32  k;

        per (k = ZEPHYRUM; k + I < xar_numerus(c->notationes); k++)
        {
            si (chorda_aequalis(((XmlNotatio*)xar_obtinere(
                c->notationes, k))->titulus, titulus->valor))
            {
                xar_removere_ultimum(c->notationes);
                redde VERUM;
            }
        }
        si (n == NIHIL)
        {
            c->memoria_defecit = VERUM;
            redde FALSUM;
        }
        n->titulus         = titulus->valor;
        n->publicum        = id.publicum;
        n->systema         = id.systema;
        n->habet_publicum  = id.habet_publicum;
        n->habet_systema   = id.habet_systema;
    }
    redde VERUM;
}

interior vacuum
_declarationem_iudicare (
                   Coctor* c,
    constans MateriaNodus* d)
{
    constans MateriaToken* aperta = _token(d,
        (i32)XML_DECLARATIO_TOK_APERTURA);
                   Cursor u;
                      b32 bene;
                      i32 k;

    si (aperta == NIHIL || _token(d, (i32)XML_DECLARATIO_TOK_CLAUSURA)
        == NIHIL)
    {
        redde;   /* absentia: derivata */
    }
    _cursor(&u, d, (i32)XML_DECLARATIO_PARTES, aperta);
    /* referentiae parametri intra declarationem: vetitae in parte
     * interna */
    per (k = ZEPHYRUM; k < u.numerus; k++)
    {
        constans MateriaToken* t = _elementum_listae(d,
            (i32)XML_DECLARATIO_PARTES, k)->datum.token;

        si (t->genus == (s32)XML_LEX_REFERENTIA_PARAMETRI)
        {
            c->referentiae_parametri = VERUM;
            _culpa(c, t, d, XML_CODEX_PARAMETRUM_INTRA,
                "referentia parametri intra declarationem in parte"
                " interna (WFC: PEs in Internal Subset)");
            redde;
        }
    }
    si (_aequat(aperta->valor, "<!ELEMENT"))
    {
        bene = _declaratio_elementi(c, &u);
    }
    alioquin si (_aequat(aperta->valor, "<!ATTLIST"))
    {
        bene = _declaratio_attributorum(c, &u);
    }
    alioquin si (_aequat(aperta->valor, "<!ENTITY"))
    {
        bene = _declaratio_entitatis(c, &u);
    }
    alioquin
    {
        bene = _declaratio_notationis(c, &u);
    }
    si (!bene)
    {
        constans MateriaToken* t = u.k < u.numerus ? _videre(&u)
            : (u.prior != NIHIL ? u.prior : aperta);

        _culpa(c, t, d, XML_CODEX_DECLARATIO,
            "declaratio forma mala (XML 1.0 par. 3.2, 3.3, 4.2, 4.7)");
    }
}

interior vacuum
_partem_iudicare (
                   Coctor* c,
    constans MateriaNodus* pars,
                      i32  profunditas);

/* referentia parametri inter declarationes */
interior vacuum
_parametrum_tractare (
                   Coctor* c,
    constans MateriaToken* t,
                      i32  profunditas)
{
       chorda  titulus = _sectio(t->valor, I, t->valor.mensura - I);
      Entitas* e;

    c->referentiae_parametri  = VERUM;
    e                         = _entitas(c, VERUM, titulus);
    si (e == NIHIL)
    {
        si (c->standalone)
        {
            _culpa(c, t, NIHIL, XML_CODEX_ENTITAS_IGNOTA,
                "entitas parametri non declarata (WFC: Entity"
                " Declared)");
        }
        redde;
    }
    si (e->externa)
    {
        /* par. 5.1: externa non lecta - declarationes sequentes
         * entitatum et attributorum non tractantur */
        si (!c->standalone)
        {
            c->cessatum = VERUM;
        }
        redde;
    }
    si (e->in_expansione)
    {
        _culpa(c, t, NIHIL, XML_CODEX_RECURSIO,
            "entitas parametri se ipsam refert (WFC: No Recursion)");
        redde;
    }
    si (profunditas >= (i32)XML_PROFUNDITAS_ENTITATUM)
    {
        _culpa(c, t, NIHIL, XML_CODEX_LIMES,
            "expansio entitatum limitem excedit (spec X4)");
        redde;
    }
    {
          XmlParsura r;
        MateriaNodus* fragmentum;
        constans MateriaToken* ancora = c->ancora;

        fragmentum = xml_arbor_parsare_fragmentum(c->piscina,
            (constans character*)e->valor.datum, (s32)e->valor.mensura,
            XML_FRAGMENTUM_PARS_INTERNA, &r);
        si (fragmentum == NIHIL)
        {
            c->memoria_defecit = VERUM;
            redde;
        }
        si (c->ancora == NIHIL)
        {
            c->ancora = t;
        }
        si (!r.sana)
        {
            _culpa(c, t, NIHIL, XML_CODEX_CONTENTUM_ENTITATIS,
                "textus substitutionis entitatis parametri"
                " declarationes integras non fert");
        }
        _lexemata_iudicare(c, fragmentum);
        e->in_expansione = VERUM;
        _partem_iudicare(c, fragmentum, profunditas + I);
        e->in_expansione  = FALSUM;
        c->ancora         = ancora;
    }
}

interior vacuum
_partem_iudicare (
                   Coctor* c,
    constans MateriaNodus* pars,
                      i32  profunditas)
{
    i32 n = _numerus(pars, (i32)XML_PARS_INTERNA_LIBERI);
    i32 k;

    per (k = ZEPHYRUM; k < n && !c->memoria_defecit; k++)
    {
        constans MateriaNodus* f = _elementum_listae(pars,
            (i32)XML_PARS_INTERNA_LIBERI, k)->datum.nodus;

        si (f->genus == (s32)XML_GENUS_DECLARATIO)
        {
            _declarationem_iudicare(c, f);
        }
        alioquin si (f->genus == (s32)XML_GENUS_REFERENTIA_PARAMETRI)
        {
            _parametrum_tractare(c, _token(f, (i32)XML_LEXEMA_TOK),
                profunditas);
        }
    }
}

interior vacuum
_typum_iudicare (
                   Coctor* c,
    constans MateriaNodus* d,
                   chorda* titulus_typi)
{
    constans MateriaToken* aperta  = _token(d,
        (i32)XML_DOCTYPE_TOK_APERTURA);
    constans MateriaToken* titulus = _token(d,
        (i32)XML_DOCTYPE_TOK_TITULUS);
                   Cursor u;

    c->habet_doctype = VERUM;
    si (titulus == NIHIL)
    {
        redde;   /* absentia: derivata */
    }
    *titulus_typi = titulus->valor;
    si (!_spatium_inter(aperta, titulus))
    {
        _culpa(c, titulus, d, XML_CODEX_DOCTYPE,
            "spatium post '<!DOCTYPE' deest");
    }
    _cursor(&u, d, (i32)XML_DOCTYPE_ID_EXTERNUM, titulus);
    si (u.numerus > ZEPHYRUM)
    {
        IdExternum id;

        c->dtd_externa = VERUM;
        si (   !_spatium_inter(titulus, _videre(&u))
            || !_id_externum(c, &u, FALSUM, &id)
            || _videre(&u) != NIHIL)
        {
            _culpa(c, u.prior, d, XML_CODEX_DOCTYPE,
                "id externum forma mala (SYSTEM 's' | PUBLIC 'p' 's')");
        }
    }
    si (d->loci[XML_DOCTYPE_PARS_INTERNA].genus == MATERIA_VALOR_NODUS)
    {
        _partem_iudicare(c,
            d->loci[XML_DOCTYPE_PARS_INTERNA].datum.nodus,
            ZEPHYRUM);
    }
}


/* ==================================================
 * GRADUS IV: contentum
 * ================================================== */

nomen structura {
    constans MateriaNodus* nodus;     /* documentum, elementum, aut
                                       * documentum fragmenti */
                      i32  locus;     /* locus liberorum */
                      i32  index;
                XmlCoctus* coctus;    /* parens coctus */
                  Entitas* entitas;   /* expansio (in_expansione) */
    constans MateriaToken* ancora;    /* ancora priori restituenda */
                      b32  radix;     /* gradus documenti */
                      b32  fragmentum; /* intra textum substitutionis:
                                        * lineae iam normatae */
} GradusCoctionis;

nomen structura {
                Coctor* c;
                   Xar* acervus;     /* GradusCoctionis */
     ChordaAedificator* textus;       /* textus pendens */
             XmlCoctus* parens_textus;
                   b32  cdata;
} Coquina;

interior XmlCoctus*
_coctus (
                   Coctor* c,
           XmlCoctusGenus  genus,
                XmlCoctus* parens,
    constans MateriaNodus* nodus)
{
    XmlCoctus* x = (XmlCoctus*)piscina_allocare(c->piscina,
                       (memoriae_index)magnitudo(XmlCoctus));

    si (x == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde NIHIL;
    }
    memset(x, ZEPHYRUM, magnitudo(XmlCoctus));
    x->genus   = genus;
    x->parens  = parens;
    x->nodus   = nodus;
    si (parens != NIHIL)
    {
        si (parens->liberi == NIHIL)
        {
            parens->liberi = xar_creare(c->piscina,
                (i32)magnitudo(XmlCoctus*));
        }
        si (parens->liberi == NIHIL)
        {
            c->memoria_defecit = VERUM;
            redde x;
        }
        *(XmlCoctus**)xar_addere(parens->liberi) = x;
    }
    redde x;
}

/* textus pendens in nodum textus parentis sui */
interior vacuum
_textum_fundere (
    Coquina* q)
{
    XmlCoctus* t;

    si (q->textus == NIHIL)
    {
        redde;
    }
    si (   chorda_aedificator_longitudo(q->textus) > ZEPHYRUM
        && q->parens_textus != NIHIL)
    {
        t = _coctus(q->c, XML_COCTUS_TEXTUS, q->parens_textus, NIHIL);
        si (t != NIHIL)
        {
            t->textus  = chorda_aedificator_finire(q->textus);
            t->cdata   = q->cdata;
        }
    }
    q->textus         = NIHIL;
    q->parens_textus  = NIHIL;
    q->cdata          = FALSUM;
}

interior ChordaAedificator*
_textus (
      Coquina* q,
    XmlCoctus* parens)
{
    si (q->textus != NIHIL && q->parens_textus != parens)
    {
        _textum_fundere(q);
    }
    si (q->textus == NIHIL)
    {
        q->textus = chorda_aedificator_creare(q->c->piscina, LXIV);
        q->parens_textus = parens;
        si (q->textus == NIHIL)
        {
            q->c->memoria_defecit = VERUM;
        }
    }
    redde q->textus;
}

interior AttributumDeclaratum*
_declaratum (
     Coctor* c,
     chorda  elementum,
     chorda  titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(c->attributa_declarata); k++)
    {
        AttributumDeclaratum* p = (AttributumDeclaratum*)xar_obtinere(
            c->attributa_declarata, k);

        si (   chorda_aequalis(p->elementum, elementum)
            && chorda_aequalis(p->titulus, titulus))
        {
            redde p;
        }
    }
    redde NIHIL;
}

interior vacuum
_attributa_coquere (
                   Coctor* c,
    constans MateriaNodus* e,
                XmlCoctus* x)
{
    i32 n = _numerus(e, (i32)XML_ELEMENTUM_ATTRIBUTA);
    i32 k;

    x->attributa = xar_creare(c->piscina,
        (i32)magnitudo(XmlAttributumCoctum));
    si (x->attributa == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaNodus* a = _elementum_listae(e,
            (i32)XML_ELEMENTUM_ATTRIBUTA, k)->datum.nodus;
        constans MateriaToken* t;
        constans MateriaToken* v;
        constans MateriaToken* prior;
                       chorda  crudus;
            ChordaAedificator* aed;
          XmlAttributumCoctum* ac;
         AttributumDeclaratum* d;
                          i32  j;

        si (a->genus != (s32)XML_GENUS_ATTRIBUTUM)
        {
            perge;
        }
        t = _token(a, (i32)XML_ATTRIBUTUM_TOK_TITULUS);
        v = _token(a, (i32)XML_ATTRIBUTUM_TOK_VALOR);
        si (t == NIHIL || v == NIHIL)
        {
            perge;
        }
        /* spatium ante attributum (post '<titulus' aut valorem
         * priorem) */
        prior = k == ZEPHYRUM ? _token(e,
            (i32)XML_ELEMENTUM_TOK_APERTURA)
            : NIHIL;
        si (k > ZEPHYRUM)
        {
            constans MateriaNodus* pa = _elementum_listae(e,
                (i32)XML_ELEMENTUM_ATTRIBUTA, k - I)->datum.nodus;

            si (pa->genus == (s32)XML_GENUS_ATTRIBUTUM)
            {
                prior = _token(pa, (i32)XML_ATTRIBUTUM_TOK_VALOR);
            }
        }
        si (prior != NIHIL && !_spatium_inter(prior, t))
        {
            _culpa(c, t, a, XML_CODEX_ATTRIBUTUM_FORMA,
                "spatium ante attributum deest");
        }
        (vacuum)_citatum(v, &crudus);
        aed = chorda_aedificator_creare(c->piscina,
            (memoriae_index)crudus.mensura + I);
        si (aed == NIHIL)
        {
            c->memoria_defecit = VERUM;
            redde;
        }
        si (!_valorem_normare(c, v, crudus, VERUM, aed))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(x->attributa); j++)
        {
            si (chorda_aequalis(((XmlAttributumCoctum*)xar_obtinere(
                x->attributa, j))->titulus, t->valor))
            {
                _culpa(c, t, a, XML_CODEX_ATTRIBUTUM_BIS,
                    "attributum bis (WFC: Unique Att Spec)");
                frange;
            }
        }
        si (j < xar_numerus(x->attributa))
        {
            perge;
        }
        ac = (XmlAttributumCoctum*)xar_addere(x->attributa);
        si (ac == NIHIL)
        {
            c->memoria_defecit = VERUM;
            redde;
        }
        memset(ac, ZEPHYRUM, magnitudo(*ac));
        ac->titulus  = t->valor;
        ac->localis  = t->valor;
        ac->valor    = chorda_aedificator_finire(aed);
        ac->nodus    = a;
        d            = _declaratum(c, x->titulus, t->valor);
        si (d != NIHIL && !d->cdata)
        {
            ac->valor = _contrahere(c, ac->valor);
        }
    }
    /* praestituta ex <!ATTLIST> */
    per (k = ZEPHYRUM; k < xar_numerus(c->attributa_declarata); k++)
    {
        AttributumDeclaratum* d = (AttributumDeclaratum*)xar_obtinere(
            c->attributa_declarata, k);
                         i32  j;
           ChordaAedificator* aed;
         XmlAttributumCoctum* ac;

        si (   !d->habet_valorem || !chorda_aequalis(d->elementum,
            x->titulus))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(x->attributa); j++)
        {
            si (chorda_aequalis(((XmlAttributumCoctum*)xar_obtinere(
                x->attributa, j))->titulus, d->titulus))
            {
                frange;
            }
        }
        si (j < xar_numerus(x->attributa))
        {
            perge;
        }
        aed = chorda_aedificator_creare(c->piscina,
            (memoriae_index)d->valor.mensura + I);
        si (aed == NIHIL)
        {
            c->memoria_defecit = VERUM;
            redde;
        }
        si (!_valorem_normare(c, d->lexema, d->valor, VERUM, aed))
        {
            perge;
        }
        ac = (XmlAttributumCoctum*)xar_addere(x->attributa);
        si (ac == NIHIL)
        {
            c->memoria_defecit = VERUM;
            redde;
        }
        memset(ac, ZEPHYRUM, magnitudo(*ac));
        ac->titulus       = d->titulus;
        ac->localis       = d->titulus;
        ac->valor         = chorda_aedificator_finire(aed);
        ac->praestitutum  = VERUM;
        si (!d->cdata)
        {
            ac->valor = _contrahere(c, ac->valor);
        }
    }
}

interior vacuum
_impellere_coctionem (
                  Coquina* q,
    constans MateriaNodus* nodus,
                      i32  locus,
                XmlCoctus* coctus,
                  Entitas* entitas,
                      b32  radix,
                      b32  fragmentum)
{
    GradusCoctionis* g = (GradusCoctionis*)xar_addere(q->acervus);

    si (g == NIHIL)
    {
        q->c->memoria_defecit = VERUM;
        redde;
    }
    g->nodus       = nodus;
    g->locus       = locus;
    g->index       = ZEPHYRUM;
    g->coctus      = coctus;
    g->entitas     = entitas;
    g->ancora      = q->c->ancora;
    g->radix       = radix;
    g->fragmentum  = fragmentum;
}

/* referentia in contento (entitas generalis aut characteris) */
interior vacuum
_referentiam_coquere (
                  Coquina* q,
    constans MateriaToken* t,
               XmlCoctus* parens,
                      b32  radix)
{
      Coctor* c = q->c;
      chorda  r = t->valor;
         s32  runa;
         i32  longitudo = ZEPHYRUM;
      chorda  titulus;
     Entitas* e;

    si (r.mensura > I && r.datum[I] == '#')
    {
        runa = _referentia_characteris(r, &longitudo);
        si (runa < ZEPHYRUM || !xml_runa_licita(runa))
        {
            _culpa(c, t, NIHIL, XML_CODEX_CHARACTER,
                "referentia ad characterem extra Char (WFC: Legal"
                " Character)");
            redde;
        }
        si (!radix)
        {
            _runam_appendere(_textus(q, parens), runa);
        }
        redde;
    }
    titulus  = _sectio(r, I, r.mensura - I);
    runa     = _entitas_praestituta(titulus);
    si (runa >= ZEPHYRUM)
    {
        si (!radix)
        {
            chorda_aedificator_appendere_character(_textus(q, parens),
                (character)runa);
        }
        redde;
    }
    e = _entitas(c, FALSUM, titulus);
    si (e == NIHIL)
    {
        si (_ignota_vitium(c))
        {
            _culpa(c, t, NIHIL, XML_CODEX_ENTITAS_IGNOTA,
                "entitas non declarata (WFC: Entity Declared)");
        }
        redde;
    }
    si (e->non_parsata)
    {
        _culpa(c, t, NIHIL, XML_CODEX_NON_PARSATA,
            "entitas non parsata in contento (WFC: Parsed Entity)");
        redde;
    }
    si (e->externa)
    {
        redde;   /* X3: numquam legitur */
    }
    si (e->in_expansione)
    {
        _culpa(c, t, NIHIL, XML_CODEX_RECURSIO,
            "entitas se ipsam refert (WFC: No Recursion)");
        redde;
    }
    si (   c->limes_excessus
        || xar_numerus(q->acervus)
            > (i32)XML_PROFUNDITAS_ENTITATUM * MXXIV
        || c->profunditas_entitatum >= XML_PROFUNDITAS_ENTITATUM
        || c->summa_expansa + (memoriae_index)e->valor.mensura
            > XML_SUMMA_EXPANSA)
    {
        si (!c->limes_excessus)
        {
            _culpa(c, t, NIHIL, XML_CODEX_LIMES,
                "expansio entitatum limitem excedit (spec X4)");
        }
        c->limes_excessus = VERUM;
        redde;
    }
    c->summa_expansa += (memoriae_index)e->valor.mensura;
    si (e->fragmentum == NIHIL)
    {
        /* parsatio SEMEL per entitatem (billion laughs: expansio
         * quaeque arborem novam parsabat, II GiB) */
                   XmlParsura  rel;
        constans MateriaToken* ancora = c->ancora;

        e->fragmentum = xml_arbor_parsare_fragmentum(c->piscina,
            (constans character*)e->valor.datum, (s32)e->valor.mensura,
            XML_FRAGMENTUM_CONTENTUM, &rel);
        si (e->fragmentum == NIHIL)
        {
            c->memoria_defecit = VERUM;
            redde;
        }
        e->fragmentum_sanum = rel.sana;
        si (c->ancora == NIHIL)
        {
            c->ancora = t;
        }
        si (!rel.sana)
        {
            _culpa(c, t, NIHIL, XML_CODEX_CONTENTUM_ENTITATIS,
                "textus substitutionis entitatis contentum"
                " aequilibratum non est");
        }
        _lexemata_iudicare(c, e->fragmentum);
        c->ancora = ancora;
    }
    si (!e->fragmentum_sanum)
    {
        redde;
    }
    {
        e->in_expansione = VERUM;
        c->profunditas_entitatum++;
        _impellere_coctionem(q, e->fragmentum,
            (i32)XML_DOCUMENTUM_LIBERI,
            parens, e, radix, VERUM);
        si (c->ancora == NIHIL)
        {
            c->ancora = t;
        }
    }
}

/* instructio cocta ex lexemate (contentum aut pars interna) */
interior vacuum
_instructionem_coquere (
                   Coctor* c,
    constans MateriaNodus* f,
                XmlCoctus* parens)
{
     constans MateriaToken* t = _token(f, (i32)XML_LEXEMA_TOK);
                 XmlCoctus* x;
                       s32  n;
                       i32  j;

    si (t->valor.mensura < IV || !_terminatur(t->valor, "?>"))
    {
        redde;
    }
    n = xml_titulum_metiri((constans character*)t->valor.datum + II,
        (s32)t->valor.mensura - IV, FALSUM);
    x = _coctus(c, XML_COCTUS_INSTRUCTIO, parens, f);
    si (x == NIHIL)
    {
        redde;
    }
    x->titulus  = _sectio(t->valor, II, II + (i32)n);
    j           = II + (i32)n;
    dum (   j < t->valor.mensura - II
         && _spatium_octeti(t->valor.datum[j]))
    {
        j++;
    }
    x->textus = _lineae_purgatae(c, _sectio(t->valor, j,
        t->valor.mensura - II));
}

interior vacuum
_contentum_coquere (
                   Coctor* c,
    constans MateriaNodus* documentum,
                XmlCoctus* radix_cocta,
                      s32* locus_typi)
{
    Coquina q;

    memset(&q, ZEPHYRUM, magnitudo(q));
    q.c = c;
    q.acervus = xar_creare(c->piscina, (i32)magnitudo(GradusCoctionis));
    si (q.acervus == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde;
    }
    _impellere_coctionem(&q, documentum, (i32)XML_DOCUMENTUM_LIBERI,
        radix_cocta, NIHIL, VERUM, FALSUM);
    dum (xar_numerus(q.acervus) > ZEPHYRUM && !c->memoria_defecit)
    {
        GradusCoctionis* g = (GradusCoctionis*)xar_obtinere(q.acervus,
            xar_numerus(q.acervus) - I);
        constans MateriaNodus* f;
        constans MateriaToken* t;

        si (g->index >= _numerus(g->nodus, g->locus))
        {
            /* gradus finitus */
            si (g->entitas != NIHIL)
            {
                g->entitas->in_expansione = FALSUM;
                c->profunditas_entitatum--;
            }
            si (g->entitas == NIHIL)
            {
                _textum_fundere(&q);
            }
            c->ancora = g->ancora;
            xar_removere_ultimum(q.acervus);
            perge;
        }
        f = _elementum_listae(g->nodus, g->locus,
            g->index)->datum.nodus;
        g->index++;
        commutatio (f->genus)
        {
            casus XML_GENUS_TEXTUS:
                t = _token(f, (i32)XML_LEXEMA_TOK);
                si (g->radix)
                {
                    frange;
                }
                si (g->fragmentum)
                {
                    chorda_aedificator_appendere_chorda(_textus(&q,
                        g->coctus), t->valor);
                }
                alioquin
                {
                    _lineas_appendere(_textus(&q, g->coctus), t->valor);
                }
                frange;
            casus XML_GENUS_CDATA:
                t = _token(f, (i32)XML_LEXEMA_TOK);
                si (   !g->radix && t->valor.mensura >= XII
                    && _terminatur(t->valor, "]]>"))
                {
                    chorda intus = _sectio(t->valor, IX,
                        t->valor.mensura - III);

                    si (g->fragmentum)
                    {
                        chorda_aedificator_appendere_chorda(_textus(&q,
                            g->coctus), intus);
                    }
                    alioquin
                    {
                        _lineas_appendere(_textus(&q, g->coctus),
                            intus);
                    }
                    q.cdata = VERUM;
                }
                frange;
            casus XML_GENUS_REFERENTIA:
                _referentiam_coquere(&q, _token(f, (i32)XML_LEXEMA_TOK),
                    g->coctus, g->radix);
                frange;
            casus XML_GENUS_COMMENTARIUM:
            {
                XmlCoctus* x;

                t = _token(f, (i32)XML_LEXEMA_TOK);
                _textum_fundere(&q);
                si (   t->valor.mensura >= VII
                    && _terminatur(t->valor, "-->"))
                {
                    x = _coctus(c, XML_COCTUS_COMMENTARIUM, g->coctus,
                        f);
                    si (x != NIHIL)
                    {
                        x->textus = _lineae_purgatae(c,
                            _sectio(t->valor,
                            IV, t->valor.mensura - III));
                    }
                }
                frange;
            }
            casus XML_GENUS_INSTRUCTIO:
                _textum_fundere(&q);
                _instructionem_coquere(c, f, g->coctus);
                frange;
            casus XML_GENUS_DOCTYPE:
                /* instructiones partis internae liberi documenti sunt
                 * (ante DOCTYPE in forma canonica) */
                si (   g->radix && *locus_typi < ZEPHYRUM
                    && f->loci[XML_DOCTYPE_PARS_INTERNA].genus
                        == MATERIA_VALOR_NODUS)
                {
                    constans MateriaNodus* pi =
                        f->loci[XML_DOCTYPE_PARS_INTERNA].datum.nodus;
                    i32 j;

                    per (j = ZEPHYRUM; j < _numerus(pi,
                         (i32)XML_PARS_INTERNA_LIBERI); j++)
                    {
                        constans MateriaNodus* h = _elementum_listae(pi,
                            (i32)XML_PARS_INTERNA_LIBERI,
                            j)->datum.nodus;

                        si (h->genus == (s32)XML_GENUS_INSTRUCTIO)
                        {
                            _instructionem_coquere(c, h, g->coctus);
                        }
                    }
                }
                si (g->radix && *locus_typi < ZEPHYRUM)
                {
                    *locus_typi = ZEPHYRUM;
                    si (g->coctus->liberi != NIHIL)
                    {
                        *locus_typi =
                            (s32)xar_numerus(g->coctus->liberi);
                    }
                }
                frange;
            casus XML_GENUS_ELEMENTUM:
            {
                            XmlCoctus* x;
                constans MateriaToken* finis;

                _textum_fundere(&q);
                x = _coctus(c, XML_COCTUS_ELEMENTUM, g->coctus, f);
                si (x == NIHIL)
                {
                    frange;
                }
                t = _token(f, (i32)XML_ELEMENTUM_TOK_APERTURA);
                x->titulus = _sectio(t->valor, I, t->valor.mensura);
                x->localis = x->titulus;
                _attributa_coquere(c, f, x);
                finis = _token(f, (i32)XML_ELEMENTUM_TOK_FINIS);
                si (   finis        != NIHIL
                    && finis->genus != (s32)XML_LEX_TAG_FINIS_SOLUS)
                {
                    _impellere_coctionem(&q, f,
                        (i32)XML_ELEMENTUM_LIBERI,
                        x, NIHIL, FALSUM, g->fragmentum);
                }
                frange;
            }
            ordinarius:
                /* doctype, declaratio-xml, signum-ordinis, malum */
                frange;
        }
    }
    _textum_fundere(&q);
}


/* ==================================================
 * GRADUS V: spatia nominum (Namespaces 1.0)
 * ================================================== */

nomen structura {
    chorda praefixum;   /* mensura 0 = spatium ordinarium */
    chorda uri;
} Ligatio;

/* QName: NCName (':' NCName)? - FALSUM si ':' plures aut partes
 * vacuae */
interior b32
_qname (
    chorda  titulus,
    chorda* praefixum,
    chorda* localis)
{
    i32 k;
    s32 colon = (s32)-I;

    per (k = ZEPHYRUM; k < titulus.mensura; k++)
    {
        si (titulus.datum[k] == ':')
        {
            si (colon >= ZEPHYRUM)
            {
                redde FALSUM;
            }
            colon = (s32)k;
        }
    }
    si (colon < ZEPHYRUM)
    {
        praefixum->datum    = NIHIL;
        praefixum->mensura  = ZEPHYRUM;
        *localis            = titulus;
        redde VERUM;
    }
    *praefixum  = _sectio(titulus, ZEPHYRUM, (i32)colon);
    *localis    = _sectio(titulus, (i32)colon + I, titulus.mensura);
    redde praefixum->mensura > ZEPHYRUM && localis->mensura > ZEPHYRUM
        && xml_titulum_metiri((constans character*)localis->datum,
               (s32)localis->mensura, FALSUM) == (s32)localis->mensura;
}

interior b32
_ligatio (
        Xar* ligationes,
     chorda  praefixum,
     chorda* uri)
{
    s32 k;

    per (k = (s32)xar_numerus(ligationes) - I; k >= ZEPHYRUM; k--)
    {
        Ligatio* l = (Ligatio*)xar_obtinere(ligationes, (i32)k);

        si (chorda_aequalis(l->praefixum, praefixum))
        {
            *uri = l->uri;
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_culpa_spatii (
                Coctor* c,
    constans XmlCoctus* x,
    constans character* causa)
{
    _culpa(c, NIHIL, x->nodus, XML_CODEX_SPATIUM_NOMINUM, causa);
}

interior vacuum
_spatia_solvere (
       Coctor* c,
    XmlCoctus* radix)
{
    /* acervus: (coctus, index liberi, numerus ligationum ante) */
    nomen structura {
         XmlCoctus* x;
               i32  index;
               i32  ligationes;
    } GradusSpatii;

    Xar* acervus = xar_creare(c->piscina,
        (i32)magnitudo(GradusSpatii));
    Xar* ligationes = xar_creare(c->piscina, (i32)magnitudo(Ligatio));
    GradusSpatii* g;

    si (acervus == NIHIL || ligationes == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde;
    }
    {
        Ligatio* l = (Ligatio*)xar_addere(ligationes);

        l->praefixum  = chorda_ex_literis("xml", c->piscina);
        l->uri        = chorda_ex_literis(XML_NS_XML, c->piscina);
    }
    g              = (GradusSpatii*)xar_addere(acervus);
    g->x           = radix;
    g->index       = (i32)-I;
    g->ligationes  = xar_numerus(ligationes);
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        XmlCoctus* x;
              i32  k;

        g = (GradusSpatii*)xar_obtinere(acervus, xar_numerus(acervus)
            - I);
        x = g->x;
        si (g->index == (i32)-I && x->genus == XML_COCTUS_ELEMENTUM)
        {
            chorda praefixum;
            chorda localis;
            chorda uri;

            /* declarationes xmlns primum */
            per (k = ZEPHYRUM; x->attributa != NIHIL
                 && k < xar_numerus(x->attributa); k++)
            {
                XmlAttributumCoctum* a = (XmlAttributumCoctum*)
                    xar_obtinere(x->attributa, k);
                Ligatio* l;

                si (!_qname(a->titulus, &praefixum, &localis))
                {
                    _culpa_spatii(c, x,
                        "titulus attributi QName non est"
                        " (Namespaces 1.0)");
                    perge;
                }
                si (   praefixum.mensura == ZEPHYRUM
                    && _aequat(localis, "xmlns"))
                {
                    l = (Ligatio*)xar_addere(ligationes);
                    l->praefixum.datum = NIHIL;
                    l->praefixum.mensura = ZEPHYRUM;
                    l->uri = a->valor;
                    si (   _aequat(a->valor, XML_NS_XML)
                        || _aequat(a->valor, XML_NS_XMLNS))
                    {
                        _culpa_spatii(c, x, "spatium ordinarium ad URI"
                            " reservatum ligari nequit");
                    }
                    a->spatium = chorda_ex_literis(XML_NS_XMLNS,
                        c->piscina);
                }
                alioquin si (_aequat(praefixum, "xmlns"))
                {
                    si (_aequat(localis, "xmlns"))
                    {
                        _culpa_spatii(c, x, "praefixum 'xmlns'"
                            " declarari nequit");
                    }
                    alioquin si (_aequat(localis, "xml")
                                 != _aequat(a->valor, XML_NS_XML))
                    {
                        _culpa_spatii(c, x,
                            "praefixum 'xml' et URI eius"
                            " solum inter se ligantur");
                    }
                    alioquin si (_aequat(a->valor, XML_NS_XMLNS))
                    {
                        _culpa_spatii(c, x, "URI spatii xmlns ligari"
                            " nequit");
                    }
                    si (a->valor.mensura == ZEPHYRUM)
                    {
                        _culpa_spatii(c, x, "praefixum ad URI vacuum"
                            " ligari nequit (Namespaces 1.0)");
                    }
                    l             = (Ligatio*)xar_addere(ligationes);
                    l->praefixum  = localis;
                    l->uri        = a->valor;
                    a->spatium = chorda_ex_literis(XML_NS_XMLNS,
                        c->piscina);
                }
            }
            /* elementum */
            si (!_qname(x->titulus, &praefixum, &localis))
            {
                _culpa_spatii(c, x, "titulus elementi QName non est"
                    " (Namespaces 1.0)");
            }
            alioquin
            {
                x->localis = localis;
                si (_ligatio(ligationes, praefixum, &uri))
                {
                    x->spatium = uri;
                }
                alioquin si (praefixum.mensura > ZEPHYRUM)
                {
                    _culpa_spatii(c, x, "praefixum non declaratum (NSC:"
                        " Prefix Declared)");
                }
            }
            /* attributa: praefixa solvuntur; nomina expansa unica */
            per (k = ZEPHYRUM; x->attributa != NIHIL
                 && k < xar_numerus(x->attributa); k++)
            {
                XmlAttributumCoctum* a = (XmlAttributumCoctum*)
                    xar_obtinere(x->attributa, k);
                i32 j;

                si (!_qname(a->titulus, &praefixum, &localis))
                {
                    perge;
                }
                a->localis = localis;
                si (   praefixum.mensura == ZEPHYRUM
                    || _aequat(praefixum, "xmlns"))
                {
                    perge;
                }
                si (!_ligatio(ligationes, praefixum, &a->spatium))
                {
                    _culpa_spatii(c, x, "praefixum attributi non"
                        " declaratum (NSC: Prefix Declared)");
                    perge;
                }
                per (j = ZEPHYRUM; j < k; j++)
                {
                    XmlAttributumCoctum* b = (XmlAttributumCoctum*)
                        xar_obtinere(x->attributa, j);

                    si (   b->spatium.mensura > ZEPHYRUM
                        && !_aequat(b->spatium, XML_NS_XMLNS)
                        && chorda_aequalis(b->spatium, a->spatium)
                        && chorda_aequalis(b->localis, a->localis))
                    {
                        _culpa_spatii(c, x, "attributa idem nomen"
                            " expansum ferunt (NSC: Attributes"
                            " Unique)");
                    }
                }
            }
        }
        g->index++;
        si (x->liberi != NIHIL && g->index < xar_numerus(x->liberi))
        {
            XmlCoctus* f = *(XmlCoctus**)xar_obtinere(x->liberi,
                g->index);
                   i32 n = xar_numerus(ligationes);

            si (f->genus == XML_COCTUS_ELEMENTUM)
            {
                GradusSpatii* h = (GradusSpatii*)xar_addere(acervus);

                si (h == NIHIL)
                {
                    c->memoria_defecit = VERUM;
                    redde;
                }
                h->x           = f;
                h->index       = (i32)-I;
                h->ligationes  = n;
            }
            perge;
        }
        /* ligationes huius elementi dimittuntur */
        xar_truncare(ligationes, g->ligationes);
        xar_removere_ultimum(acervus);
    }
}


/* ==================================================
 * Ordo diagnosticorum et publica
 * ================================================== */

nomen structura {
    MateriaDiagnosticum d;
                    i32 ordo;
} Ordinandum;

interior integer
_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans Ordinandum* x = (constans Ordinandum*)a;
    constans Ordinandum* y = (constans Ordinandum*)b;

    si (x->d.tractus.initium != y->d.tractus.initium)
    {
        redde x->d.tractus.initium < y->d.tractus.initium ? -I : I;
    }
    redde x->ordo < y->ordo ? -I : (x->ordo > y->ordo ? I : ZEPHYRUM);
}

interior Xar*
_ordinare (
    Piscina* piscina,
        Xar* a,
        Xar* b)
{
           i32  summa = ZEPHYRUM;
           i32  k;
           i32  m = ZEPHYRUM;
    Ordinandum* omnia;
           Xar* fontes[II];
           Xar* exitus = xar_creare(piscina,
                            (i32)magnitudo(MateriaDiagnosticum));

    fontes[ZEPHYRUM]  = a;
    fontes[I]         = b;
    per (k = ZEPHYRUM; k < II; k++)
    {
        summa += fontes[k] != NIHIL ? xar_numerus(fontes[k]) : ZEPHYRUM;
    }
    si (exitus == NIHIL || summa == ZEPHYRUM)
    {
        redde exitus;
    }
    omnia = (Ordinandum*)piscina_allocare(piscina,
        (memoriae_index)summa * (memoriae_index)magnitudo(Ordinandum));
    si (omnia == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < II; k++)
    {
        i32 j;

        per (j = ZEPHYRUM; fontes[k] != NIHIL
             && j < xar_numerus(fontes[k]); j++)
        {
            omnia[m].d = *(MateriaDiagnosticum*)xar_obtinere(fontes[k],
                j);
            omnia[m].ordo = m;
            m++;
        }
    }
    qsort(omnia, (size_t)summa, magnitudo(Ordinandum), _comparare);
    per (k = ZEPHYRUM; k < summa; k++)
    {
        MateriaDiagnosticum* d =
            (MateriaDiagnosticum*)xar_addere(exitus);

        si (d == NIHIL)
        {
            redde NIHIL;
        }
        *d = omnia[k].d;
    }
    redde exitus;
}

XmlCoctum
xml_coquere (
               Piscina* piscina,
    constans character* fons,
                   s32  mensura,
                   b32  spatia)
{
       XmlCoctum exitus;
          Coctor c;
      XmlParsura r;
    MateriaNodus* arbor;
             Xar* derivata;
       XmlCoctus* documentum;
             i32  k;
             i32  n;

    memset(&exitus, ZEPHYRUM, magnitudo(exitus));
    memset(&c, ZEPHYRUM, magnitudo(c));
    si (piscina == NIHIL)
    {
        redde exitus;
    }
    c.piscina  = piscina;
    c.spatia   = spatia;
    c.diagnostica          = xar_creare(piscina,
        (i32)magnitudo(MateriaDiagnosticum));
    c.generales = tabula_dispersa_creare_chorda(piscina,
        LXIV);
    c.parametri = tabula_dispersa_creare_chorda(piscina,
        XVI);
    c.attributa_declarata  = xar_creare(piscina,
        (i32)magnitudo(AttributumDeclaratum));
    c.notationes = xar_creare(piscina,
        (i32)magnitudo(XmlNotatio));
    exitus.notationes = c.notationes;
    si (   c.diagnostica == NIHIL || c.generales == NIHIL
        || c.parametri   == NIHIL || c.attributa_declarata == NIHIL
        || c.notationes  == NIHIL)
    {
        redde exitus;
    }

    /* I: arbor et derivata */
    arbor         = xml_arbor_parsare(piscina, fons, mensura, &r);
    exitus.arbor  = arbor;
    si (arbor == NIHIL)
    {
        redde exitus;
    }
    derivata = materia_diagnostica_derivare(piscina, arbor,
        &XML_REGISTRUM, &XML_DIAGNOSTICA, NIHIL, r.diagnostica);

    /* II: lexemata, declaratio XML */
    _lexemata_iudicare(&c, arbor);
    n = _numerus(arbor, (i32)XML_DOCUMENTUM_LIBERI);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaNodus* f = _elementum_listae(arbor,
            (i32)XML_DOCUMENTUM_LIBERI, k)->datum.nodus;

        si (f->genus == (s32)XML_GENUS_DECLARATIO_XML)
        {
            _declarationem_xml_iudicare(&c, f);
        }
    }

    /* III: DTD */
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaNodus* f = _elementum_listae(arbor,
            (i32)XML_DOCUMENTUM_LIBERI, k)->datum.nodus;

        si (f->genus == (s32)XML_GENUS_DOCTYPE)
        {
            _typum_iudicare(&c, f, &exitus.titulus_typi);
            frange;
        }
    }

    /* IV: contentum; V: spatia */
    documentum = _coctus(&c, XML_COCTUS_DOCUMENTUM, NIHIL, arbor);
    si (documentum != NIHIL)
    {
        exitus.locus_typi = (s32)-I;
        _contentum_coquere(&c, arbor, documentum, &exitus.locus_typi);
        si (spatia)
        {
            per (k = ZEPHYRUM; documentum->liberi != NIHIL
                 && k < xar_numerus(documentum->liberi); k++)
            {
                XmlCoctus* f = *(XmlCoctus**)xar_obtinere(
                    documentum->liberi, k);

                si (f->genus == XML_COCTUS_ELEMENTUM)
                {
                    _spatia_solvere(&c, f);
                }
            }
        }
    }
    si (c.memoria_defecit)
    {
        redde exitus;
    }
    exitus.diagnostica    = _ordinare(piscina, derivata, c.diagnostica);
    exitus.bene_formatum  = exitus.diagnostica != NIHIL;
    per (k = ZEPHYRUM; exitus.diagnostica != NIHIL
         && k < xar_numerus(exitus.diagnostica); k++)
    {
        si (((MateriaDiagnosticum*)xar_obtinere(exitus.diagnostica, k))
                ->gravitas == (s32)MATERIA_GRAVITAS_ERRATUM)
        {
            exitus.bene_formatum = FALSUM;
        }
    }
    exitus.documentum = exitus.bene_formatum ? documentum : NIHIL;
    redde exitus;
}
