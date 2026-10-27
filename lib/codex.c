/* codex.c - liber EPUB (include/codex.h) */
#include "codex.h"
#include "sarcina.h"
#include "xml.h"
#include "chorda_aedificator.h"
#include "html_lexema.h"
#include "entitates_html.h"
#include "sententia_fissio.h"

#include <string.h>

/* res manifesti: id -> via (in arca, a directorio OPF resoluta) */
nomen structura {
    chorda id;
    chorda via;
    chorda genus;          /* media-type */
    chorda proprietates;   /* properties ("nav" ...) */
} ResManifesti;

/* caput in tabula: introitus indicis -> (index spinae, ancora) */
nomen structura {
    chorda titulus;      /* vacuum = prooemium */
       i32 ordo;         /* index in spina */
    chorda fragmentum;   /* id ancorae; vacuum = initium documenti */
} CaputInternum;

structura Codex {
                 Piscina* piscina;
     InternamentumChorda* intern;
                 Sarcina* arca;
                  chorda  identitas;
                  chorda  titulus;
                  chorda  auctor;
                  chorda  lingua;
                  chorda  directorium;  /* directorium OPF cum '/' */
                     Xar* manifestum;   /* ResManifesti */
                     Xar* ordo;         /* chorda: viae spinae */
                  chorda  index_toc;    /* spine@toc (id NCX) */
                     Xar* capita;       /* CaputInternum */
};

interior chorda
vacua (
    vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior Codex*
recusare (
     constans character* nuntium,
                Piscina* piscina,
                 chorda* vitium)
{
    si (vitium && piscina)
    {
        *vitium = chorda_ex_literis(nuntium, piscina);
    }
    redde NIHIL;
}

/* spatia extrema (spatium, tab, linea) dempta */
interior chorda
purgare (
    chorda c)
{
    dum (   c.mensura > ZEPHYRUM
         && (   c.datum[ZEPHYRUM] == ' ' || c.datum[ZEPHYRUM] == '\t'
             || c.datum[ZEPHYRUM] == '\n' || c.datum[ZEPHYRUM] == '\r'))
    {
        c.datum++;
        c.mensura--;
    }
    dum (   c.mensura > ZEPHYRUM
         && (   c.datum[c.mensura - I] == ' '
             || c.datum[c.mensura - I] == '\t'
             || c.datum[c.mensura - I] == '\n'
             || c.datum[c.mensura - I] == '\r'))
    {
        c.mensura--;
    }
    redde c;
}

/* membrum arcae per viam legere (in piscina data); FALSUM et vitium
 * si abest */
interior b32
membrum_legere (
     constans Codex* c,
             chorda  via,
            Piscina* piscina,
             chorda* exitus,
             chorda* vitium)
{
    constans SarcinaIntroitus* e;

    e = sarcina_invenire(c->arca, via);
    si (!e)
    {
        *vitium =
            chorda_concatenare(chorda_ex_literis("missing entry: ",
            piscina), via, piscina);
        redde FALSUM;
    }
    redde sarcina_legere(c->arca, e, piscina, exitus, vitium);
}

/* XML membri (in piscina data: capita in piscina vocantis, non libri);
 * NIHIL et vitium si non legitur */
interior XmlNodus*
xml_membri (
     constans Codex* c,
             chorda  via,
            Piscina* piscina,
             chorda* vitium)
{
         chorda datum;
    XmlResultus r;

    si (!membrum_legere(c, via, piscina, &datum, vitium))
    {
        redde NIHIL;
    }
    r = xml_legere(datum, piscina, c->intern);
    si (!r.successus || !r.radix)
    {
        *vitium =
            chorda_concatenare(chorda_ex_literis("malformed XML: ",
            piscina), via, piscina);
        redde NIHIL;
    }
    redde r.radix;
}

/* lexemata membri (html_lexema: totalis, numquam frangit); NIHIL et
 * vitium solum si membrum deest aut legi nequit */
interior Xar*
lexemata_membri (
     constans Codex* c,
             chorda  via,
            Piscina* piscina,
             chorda* vitium)
{
    chorda  datum;
       Xar* l;

    si (!membrum_legere(c, via, piscina, &datum, vitium))
    {
        redde NIHIL;
    }
    l = html_lexare(piscina, (constans character*)datum.datum,
        datum.mensura);
    si (!l)
    {
        *vitium = chorda_ex_literis("out of memory", piscina);
    }
    redde l;
}

/* textus primi liberi cum titulo (purgatus); vacuus si abest */
interior chorda
textus_liberi (
    constans Codex* c,
          XmlNodus* parens,
constans character* titulus)
{
    XmlNodus* n;

    n = parens ? xml_invenire_liberum(parens, titulus) : NIHIL;
    redde n ? purgare(xml_textus_internus(n, c->piscina)) : vacua();
}

/* valor hexadecimalis litterae; -1 si non */
interior s32
hexadecimalis (
    i8 x)
{
    si (x >= '0' && x <= '9')
    {
        redde (s32)(x - '0');
    }
    si (x >= 'a' && x <= 'f')
    {
        redde (s32)(x - 'a') + X;
    }
    si (x >= 'A' && x <= 'F')
    {
        redde (s32)(x - 'A') + X;
    }
    redde -I;
}

/* nexus relativus -> via in arca: basis (directorium cum '/') + nexus,
 * %XX decoctum, fragmentum ('#...') demptum */
interior chorda
viam_resolvere (
     constans Codex* c,
             chorda  basis,
             chorda  nexus)
{
        i8* b;
       i32  k;
       i32  i;
       s32  a;
       s32  d;
    chorda  via;

    b = (i8*)piscina_allocare(c->piscina, basis.mensura + nexus.mensura
        + I);
    si (!b)
    {
        redde vacua();
    }
    memcpy(b, basis.datum, (size_t)basis.mensura);
    k = basis.mensura;
    per (i = ZEPHYRUM; i < nexus.mensura && nexus.datum[i] != '#'; i++)
    {
        si (nexus.datum[i] == '%' && i + II < nexus.mensura)
        {
            a = hexadecimalis(nexus.datum[i + I]);
            d = hexadecimalis(nexus.datum[i + II]);
            si (a >= ZEPHYRUM && d >= ZEPHYRUM)
            {
                b[k++]  = (i8)(a * XVI + d);
                i       += II;
                perge;
            }
        }
        b[k++] = nexus.datum[i];
    }
    via.datum    = b;
    via.mensura  = k;
    redde via;
}

/* directorium viae (usque ad '/' ultimum, inclusum) */
interior chorda
directorium_viae (
    chorda via)
{
    chorda d;
       i32 i;

    d.datum    = via.datum;
    d.mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < via.mensura; i++)
    {
        si (via.datum[i] == '/')
        {
            d.mensura = i + I;
        }
    }
    redde d;
}

/* fragmentum nexus (post '#'); vacuum si nullum */
interior chorda
fragmentum_nexus (
    chorda nexus)
{
    i32 i;

    per (i = ZEPHYRUM; i < nexus.mensura; i++)
    {
        si (nexus.datum[i] == '#')
        {
            nexus.datum    += i + I;
            nexus.mensura  -= i + I;
            redde nexus;
        }
    }
    redde vacua();
}

/* spatia (spatium, tab, linea) in unum contracta, extrema dempta */
interior chorda
spatia_contrahere (
     chorda  c,
    Piscina* piscina)
{
        i8* b;
       i32  i;
       i32  k;
       b32  spatium;

    c = purgare(c);
    b = (i8*)piscina_allocare(piscina, c.mensura + I);
    si (!b)
    {
        redde vacua();
    }
    k        = ZEPHYRUM;
    spatium  = FALSUM;
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (   c.datum[i] == ' ' || c.datum[i] == '\t'
            || c.datum[i] == '\n' || c.datum[i] == '\r')
        {
            spatium = VERUM;
            perge;
        }
        si (spatium && k > ZEPHYRUM)
        {
            b[k++] = ' ';
        }
        spatium  = FALSUM;
        b[k++]   = c.datum[i];
    }
    c.datum    = b;
    c.mensura  = k;
    redde c;
}

/* textus omnium descendentium (xml_textus_internus solum liberos
 * directos reddit: navLabel > text) in aedificatorem */
interior vacuum
textum_colligere (
             XmlNodus* n,
    ChordaAedificator* aed)
{
    XmlNodus* l;
         i32  i;

    per (i = ZEPHYRUM; n && i < xml_numerus_liberorum(n); i++)
    {
        l = xml_liberum_ad_indicem(n, i);
        si (l && l->genus == XML_NODUS_TEXTUS && l->valor)
        {
            chorda_aedificator_appendere_chorda(aed, *l->valor);
        }
        alioquin si (l && l->genus == XML_NODUS_ELEMENTUM)
        {
            textum_colligere(l, aed);
        }
    }
}

/* titulus indicis: textus totus, spatia contracta */
interior chorda
titulum_legere (
    constans Codex* c,
          XmlNodus* n)
{
    ChordaAedificator* aed;

    aed = chorda_aedificator_creare(c->piscina, CCLVI);
    si (!aed || !n)
    {
        redde vacua();
    }
    textum_colligere(n, aed);
    redde spatia_contrahere(chorda_aedificator_finire(aed), c->piscina);
}

interior b32
elementum_est (
              XmlNodus* n,
    constans character* titulus)
{
    redde n && n->genus == XML_NODUS_ELEMENTUM && n->titulus
        && chorda_aequalis_literis(*n->titulus, titulus);
}

/* descendentes cum titulo ordine documenti (praeordine) */
interior vacuum
descendentes_colligere (
              XmlNodus* n,
    constans character* titulus,
                   Xar* exitus)
{
    XmlNodus* l;
         i32  i;

    per (i = ZEPHYRUM; n && i < xml_numerus_liberorum(n); i++)
    {
        l = xml_liberum_ad_indicem(n, i);
        si (!l || l->genus != XML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (elementum_est(l, titulus))
        {
            *(XmlNodus**)xar_addere(exitus) = l;
        }
        descendentes_colligere(l, titulus, exitus);
    }
}

/* tag ex lexematibus html_lexema lectum */
nomen structura {
    chorda titulus;    /* sine '<' aut '</' */
       b32 clausura;   /* </x */
       b32 solus;      /* <x/> */
    chorda id;         /* sine citatione; vacuum si abest */
    chorda classis;
} Tagum;

/* modus elementi aperti in acervo */
nomen enumeratio {
    GRADUS_INLINEUS = ZEPHYRUM,   /* intra lineam */
    GRADUS_CLAUSULA,              /* clausulam aut lineam claudit */
    GRADUS_OMISSUS                /* textus eius non legitur */
} GradusModus;

nomen structura {
         chorda titulus;
    GradusModus modus;
     CodexGenus prior;      /* genus ante elementum */
            b32 claudere;   /* clausulam (non solum lineam) claudit */
} Gradus;

/* ambulatio capitis: status dum lexemata in textum et clausulas
 * vertuntur */
nomen structura {
              Piscina* piscina;
    ChordaAedificator* textus;      /* textus capitis */
                  Xar* clausulae;   /* CodexClausula */
    ChordaAedificator* linea;       /* linea currens, cruda */
    ChordaAedificator* clausula;    /* lineae purgatae, '\n' iunctae */
                  Xar* acervus;     /* Gradus: elementa aperta */
                  i32  omissa;      /* elementa omissa aperta */
           CodexGenus  genus;       /* clausulae currentis */
               chorda  initium;     /* ancora initii in hoc documento */
               chorda  finis;       /* ancora finis in hoc documento */
                  b32  emittere;    /* ancora initii attacta */
                  b32  finitum;     /* ancora finis attacta */
} Ambulatio;

hic_manens constans character* constans OMITTENDA[] = {
    "head", "script", "style", "svg", "title", NIHIL
};

hic_manens constans character* constans VACUA[] = {
    "br", "img", "hr", "meta", "link", "input", "wbr", "area", "base",
    "col", "embed", "param", "source", "track", NIHIL
};

hic_manens constans character* constans INLINEA[] = {
    "a", "abbr", "b", "bdi", "bdo", "big", "cite", "code", "del", "dfn",
    "em", "font", "i", "ins", "kbd", "mark", "q", "s", "samp", "small",
    "span", "strike", "strong", "sub", "sup", "time", "tt", "u", "var",
    NIHIL
};

/* tituli aequales sine casu litterarum */
interior b32
tituli_aequales (
             chorda  a,
     constans    i8* b,
                i32  n)
{
    i32 i;
     i8 x;
     i8 y;

    si (a.mensura != n)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = a.datum[i];
        y = b[i];
        si (x >= 'A' && x <= 'Z')
        {
            x = (i8)(x + ('a' - 'A'));
        }
        si (y >= 'A' && y <= 'Z')
        {
            y = (i8)(y + ('a' - 'A'));
        }
        si (x != y)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
titulus_est_literae (
                 chorda  titulus,
     constans character* literae)
{
    redde tituli_aequales(titulus, (constans i8*)literae,
        (i32)strlen(literae));
}

interior b32
in_tabula (
                           chorda  titulus,
    constans character* constans* tabula)
{
    i32 i;

    per (i = ZEPHYRUM; tabula[i]; i++)
    {
        si (titulus_est_literae(titulus, tabula[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* verbum litterarum in chorda? (sine allocatione) */
interior b32
continet_literas (
                 chorda  c,
     constans character* verbum)
{
    i32 n;
    i32 i;

    n = (i32)strlen(verbum);
    per (i = ZEPHYRUM; n > ZEPHYRUM && i + n <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, verbum, (size_t)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* valor attributi sine citatione */
interior chorda
sine_citatione (
    chorda v)
{
    si (   v.mensura              >= II
        && (v.datum[ZEPHYRUM] == '"' || v.datum[ZEPHYRUM] == '\'')
        && v.datum[v.mensura - I] == v.datum[ZEPHYRUM])
    {
        v.datum++;
        v.mensura -= II;
    }
    redde v;
}

/* tagum ab indice k (TAG_APERTURA aut TAG_CLAUSURA) legere: titulus,
 * id, class, '/>'; redde indicem post tagum (tag truncatus: ad lexema
 * primum alienum) */
interior i32
tagum_legere (
       Xar* lexemata,
       i32  k,
     Tagum* t)
{
    constans HtmlLexema* x;
                 chorda  attributum;
                    i32  i;
                    i32  praefixum;

    x = (constans HtmlLexema*)xar_obtinere(lexemata, k);
    memset(t, ZEPHYRUM, magnitudo(Tagum));
    t->clausura  = x->genus == HTML_LEX_TAG_CLAUSURA;
    praefixum    = t->clausura ? II : I;
    t->titulus   = x->valor;
    si (t->titulus.mensura >= praefixum)
    {
        t->titulus.datum    += praefixum;
        t->titulus.mensura  -= praefixum;
    }
    attributum = vacua();
    per (i = k + I; i < xar_numerus(lexemata); i++)
    {
        x = (constans HtmlLexema*)xar_obtinere(lexemata, i);
        si (x->genus == HTML_LEX_TAG_FINIS)
        {
            redde i + I;
        }
        si (x->genus == HTML_LEX_TAG_FINIS_SOLUS)
        {
            t->solus = VERUM;
            redde i + I;
        }
        si (x->genus == HTML_LEX_ATTRIBUTUM_NOMEN)
        {
            attributum = x->valor;
        }
        alioquin si (x->genus == HTML_LEX_ATTRIBUTUM_VALOR)
        {
            si (titulus_est_literae(attributum, "id"))
            {
                t->id = sine_citatione(x->valor);
            }
            alioquin si (titulus_est_literae(attributum, "class"))
            {
                t->classis = sine_citatione(x->valor);
            }
        }
        alioquin si (   x->genus != HTML_LEX_SPATIA
                     && x->genus != HTML_LEX_AEQUALE
                     && x->genus != HTML_LEX_DELIM
                     && x->genus
                         != HTML_LEX_ATTRIBUTUM_VALOR_IMPERFECTUS)
        {
            redde i;
        }
    }
    redde i;
}

/* h1-h6 */
interior b32
titulus_est (
    constans Tagum* t)
{
    redde t->titulus.mensura == II
        && (t->titulus.datum[ZEPHYRUM] == 'h'
            || t->titulus.datum[ZEPHYRUM] == 'H')
        && t->titulus.datum[I] >= '1' && t->titulus.datum[I] <= '6';
}

/* classis poem / stanza / verse */
interior b32
versus_est (
    constans Tagum* t)
{
    redde continet_literas(t->classis, "poem")
        || continet_literas(t->classis, "stanza")
        || continet_literas(t->classis, "verse");
}

/* numerum elementorum apertorum mutare (numquam infra nihil) */
interior vacuum
numerare (
    i32* n,
    b32  clausura)
{
    si (!clausura)
    {
        (*n)++;
    }
    alioquin si (*n > ZEPHYRUM)
    {
        (*n)--;
    }
}

/* textus visibilis in lexematibus ante elementum cuius id = ancora
 * (ancora vacua: totum)? head/script/style/title praetereuntur; textus
 * solum intra <a> non numeratur (involucra imaginum). *inventa =
 * ancora attacta */
interior b32
textus_visibilis (
        Xar* lexemata,
     chorda  ancora,
        b32* inventa)
{
    constans HtmlLexema* x;
                  Tagum  t;
                    i32  i;
                    i32  omissa;
                    i32  nexus;

    omissa  = ZEPHYRUM;
    nexus   = ZEPHYRUM;
    i       = ZEPHYRUM;
    dum (lexemata && i < xar_numerus(lexemata))
    {
        x = (constans HtmlLexema*)xar_obtinere(lexemata, i);
        si (   x->genus == HTML_LEX_TAG_APERTURA
            || x->genus == HTML_LEX_TAG_CLAUSURA)
        {
            i = tagum_legere(lexemata, i, &t);
            si (   !t.clausura && ancora.mensura > ZEPHYRUM
                && chorda_aequalis(t.id, ancora))
            {
                *inventa = VERUM;
                redde FALSUM;
            }
            si (t.solus || in_tabula(t.titulus, VACUA))
            {
                perge;
            }
            si (in_tabula(t.titulus, OMITTENDA))
            {
                numerare(&omissa, t.clausura);
            }
            alioquin si (titulus_est_literae(t.titulus, "a"))
            {
                numerare(&nexus, t.clausura);
            }
            perge;
        }
        si (   omissa == ZEPHYRUM && nexus == ZEPHYRUM
            && (   x->genus == HTML_LEX_REFERENTIA
                || (   x->genus == HTML_LEX_TEXTUS
                    && purgare(x->valor).mensura > ZEPHYRUM)))
        {
            redde VERUM;
        }
        i++;
    }
    redde FALSUM;
}

/* lineam currentem (spatiis contractis) clausulae addere */
interior vacuum
lineam_claudere (
    Ambulatio* a)
{
    chorda l;

    l = spatia_contrahere(chorda_aedificator_spectare(a->linea),
        a->piscina);
    chorda_aedificator_reset(a->linea);
    si (l.mensura == ZEPHYRUM)
    {
        redde;
    }
    si (chorda_aedificator_longitudo(a->clausula) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a->clausula, '\n');
    }
    chorda_aedificator_appendere_chorda(a->clausula, l);
}

/* clausulam currentem (si non vacua) textui capitis addere */
interior vacuum
clausulam_claudere (
    Ambulatio* a)
{
    CodexClausula* b;
           chorda  t;

    lineam_claudere(a);
    t = chorda_aedificator_spectare(a->clausula);
    si (t.mensura == ZEPHYRUM)
    {
        redde;
    }
    si (chorda_aedificator_longitudo(a->textus) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a->textus, "\n\n");
    }
    b           = (CodexClausula*)xar_addere(a->clausulae);
    b->genus    = a->genus;
    b->initium  = (i32)chorda_aedificator_longitudo(a->textus);
    chorda_aedificator_appendere_chorda(a->textus, t);
    b->finis = (i32)chorda_aedificator_longitudo(a->textus);
    chorda_aedificator_reset(a->clausula);
}

interior vacuum
gradum_addere (
      Ambulatio* a,
         chorda  titulus,
    GradusModus  modus,
            b32  claudere)
{
    Gradus* g;

    g            = (Gradus*)xar_addere(a->acervus);
    g->titulus   = titulus;
    g->modus     = modus;
    g->prior     = a->genus;
    g->claudere  = claudere;
}

/* elementa aperta ab indice j sursum claudere (ultimum primum) */
interior vacuum
acervum_truncare (
    Ambulatio* a,
          i32  j)
{
    Gradus g;

    dum (xar_numerus(a->acervus) > j)
    {
        g = *(Gradus*)xar_obtinere(a->acervus, xar_numerus(a->acervus)
            - I);
        (vacuum)xar_removere_ultimum(a->acervus);
        si (g.modus == GRADUS_OMISSUS)
        {
            numerare(&a->omissa, VERUM);
        }
        alioquin si (g.modus == GRADUS_CLAUSULA)
        {
            si (g.claudere)
            {
                clausulam_claudere(a);
            }
            alioquin
            {
                lineam_claudere(a);
            }
            a->genus = g.prior;
        }
    }
}

/* tag aperiens: ancorae; <br/> in titulo et versu = linea nova, in
 * paragrapho = spatium; elementa clausularum clausulas claudunt (in
 * titulo et versu: lineas); classis versus clausulam propriam semper
 * facit (stanzae separatae). <p> non clausum non nocet: elementum
 * clausulae sequens clausulam iam claudit */
interior vacuum
tagum_aperire (
         Ambulatio* a,
    constans Tagum* t)
{
    CodexGenus novum;
           b32 claudere;

    si (   a->finis.mensura > ZEPHYRUM
        && chorda_aequalis(t->id, a->finis))
    {
        clausulam_claudere(a);
        a->finitum = VERUM;
        redde;
    }
    si (   !a->emittere && a->initium.mensura > ZEPHYRUM
        && chorda_aequalis(t->id, a->initium))
    {
        a->emittere = VERUM;
    }
    si (titulus_est_literae(t->titulus, "br"))
    {
        si (a->genus == CODEX_PARAGRAPHUS)
        {
            chorda_aedificator_appendere_character(a->linea, ' ');
        }
        alioquin
        {
            lineam_claudere(a);
        }
        redde;
    }
    si (t->solus || in_tabula(t->titulus, VACUA))
    {
        redde;
    }
    si (in_tabula(t->titulus, OMITTENDA))
    {
        gradum_addere(a, t->titulus, GRADUS_OMISSUS, FALSUM);
        numerare(&a->omissa, FALSUM);
        redde;
    }
    si (in_tabula(t->titulus, INLINEA))
    {
        gradum_addere(a, t->titulus, GRADUS_INLINEUS, FALSUM);
        redde;
    }
    novum = titulus_est(t) ? CODEX_TITULUS : versus_est(t)
        ? CODEX_VERSUS : a->genus;
    claudere = novum != a->genus || a->genus == CODEX_PARAGRAPHUS
        || versus_est(t);
    si (claudere)
    {
        clausulam_claudere(a);
    }
    alioquin
    {
        lineam_claudere(a);
    }
    gradum_addere(a, t->titulus, GRADUS_CLAUSULA, claudere);
    a->genus = novum;
}

/* tag claudens: elementum apertum eiusdem tituli (et quae supra id
 * aperta manent) clauditur; sine pari ignoratur */
interior vacuum
tagum_claudere (
         Ambulatio* a,
    constans Tagum* t)
{
    s32 j;

    per (j = (s32)xar_numerus(a->acervus) - I; j >= ZEPHYRUM; j--)
    {
        si (tituli_aequales(t->titulus,
                ((constans Gradus*)xar_obtinere(a->acervus,
                (i32)j))->titulus.datum,
                ((constans Gradus*)xar_obtinere(
                a->acervus, (i32)j))->titulus.mensura))
        {
            acervum_truncare(a, (i32)j);
            redde;
        }
    }
}

/* documentum (lexemata) ambulare: textus et referentiae (decoctae)
 * post ancoram initii in lineam; ancora finis ambulationem finit; in
 * fine elementa aperta clauduntur */
interior vacuum
documentum_ambulare (
    Ambulatio* a,
          Xar* lexemata)
{
    constans HtmlLexema* x;
                  Tagum  t;
              character  octeti[VIII];
                    i32  longitudo;
                    s32  post;
                    i32  i;

    i = ZEPHYRUM;
    dum (i < xar_numerus(lexemata) && !a->finitum)
    {
        x = (constans HtmlLexema*)xar_obtinere(lexemata, i);
        si (   x->genus == HTML_LEX_TAG_APERTURA
            || x->genus == HTML_LEX_TAG_CLAUSURA)
        {
            i = tagum_legere(lexemata, i, &t);
            si (t.clausura)
            {
                tagum_claudere(a, &t);
            }
            alioquin
            {
                tagum_aperire(a, &t);
            }
            perge;
        }
        si (a->emittere && a->omissa == ZEPHYRUM)
        {
            si (x->genus == HTML_LEX_TEXTUS)
            {
                chorda_aedificator_appendere_chorda(a->linea, x->valor);
            }
            alioquin si (x->genus == HTML_LEX_REFERENTIA)
            {
                si (entitates_html_decoquere(
                        (constans character*)x->valor.datum, ZEPHYRUM,
                        (s32)x->valor.mensura, FALSUM, octeti,
                        &longitudo, &post))
                {
                    chorda_aedificator_appendere_chorda(a->linea,
                        chorda_ex_buffer((i8*)octeti, longitudo));
                }
                alioquin
                {
                    chorda_aedificator_appendere_chorda(a->linea,
                        x->valor);
                }
            }
        }
        i++;
    }
    acervum_truncare(a, ZEPHYRUM);
    clausulam_claudere(a);
}

/* introitum indicis in tabulam capitum: via (resoluta) -> index spinae;
 * extra spinam aut iteratus praetermittitur */
interior vacuum
introitum_addere (
      Codex* c,
     chorda  titulus,
     chorda  via,
     chorda  fragmentum)
{
    CaputInternum* caput;
              i32  k;
              i32  ordo;

    ordo = xar_numerus(c->ordo);
    per (k = ZEPHYRUM; k < xar_numerus(c->ordo); k++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(c->ordo, k), via))
        {
            ordo = k;
            frange;
        }
    }
    si (ordo == xar_numerus(c->ordo))
    {
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(c->capita); k++)
    {
        caput = (CaputInternum*)xar_obtinere(c->capita, k);
        si (   caput->ordo == ordo && chorda_aequalis(caput->fragmentum,
                fragmentum))
        {
            redde;
        }
    }
    caput              = (CaputInternum*)xar_addere(c->capita);
    caput->titulus     = titulus;
    caput->ordo        = ordo;
    caput->fragmentum  = fragmentum;
}

/* res manifesti per id, aut per verbum in properties (spatiis
 * separatum) */
interior constans ResManifesti*
rem_quaerere (
        constans Codex* c,
    constans character* proprietas,
                chorda* id)
{
      constans ResManifesti* res;
                     chorda  verbum;
                        i32  i;
                        i32  j;
                        i32  a;

    verbum = proprietas ? chorda_ex_literis(proprietas, c->piscina)
        : vacua();
    per (i = ZEPHYRUM; i < xar_numerus(c->manifestum); i++)
    {
        res = (constans ResManifesti*)xar_obtinere(c->manifestum, i);
        si (id && chorda_aequalis(res->id, *id))
        {
            redde res;
        }
        a = ZEPHYRUM;
        per (j = ZEPHYRUM; proprietas && j <= res->proprietates.mensura;
            j++)
        {
            si (   j == res->proprietates.mensura
                || res->proprietates.datum[j] == ' ')
            {
                si (   j - a == verbum.mensura
                    && verbum.mensura > ZEPHYRUM
                    && memcmp(res->proprietates.datum + a, verbum.datum,
                           (size_t)verbum.mensura) == ZEPHYRUM)
                {
                    redde res;
                }
                a = j + I;
            }
        }
    }
    redde NIHIL;
}

/* index: nav (EPUB 3) aut NCX (EPUB 2) -> tabula capitum; prooemium
 * si textus visibilis ante introitum primum; nullus index -> caput
 * unum (titulus libri) */
interior vacuum
capita_struere (
    Codex* c)
{
    constans ResManifesti* res;
                 XmlNodus* radix;
                 XmlNodus* nav;
                 XmlNodus* n;
                      Xar* nodi;
                   chorda* a;
                   chorda  basis;
                   chorda  vitium;
                   chorda  titulus;
            CaputInternum* primum;
            CaputInternum  prooemium;
                      i32  i;
                      b32  inventa;
                      b32  textus;

    c->capita  = xar_creare(c->piscina, (i32)magnitudo(CaputInternum));
    nodi       = xar_creare(c->piscina, (i32)magnitudo(XmlNodus*));
    res        = rem_quaerere(c, "nav", NIHIL);
    radix = res ? xml_membri(c, res->via, c->piscina,
        &vitium) : NIHIL;
    si (radix)
    {
        basis = directorium_viae(res->via);
        descendentes_colligere(radix, "nav", nodi);
        nav = NIHIL;
        per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
        {
            n = *(XmlNodus**)xar_obtinere(nodi, i);
            a = xml_attributum_capere(n, "epub:type");
            si (   !nav
                || (a && chorda_continet(*a, chorda_ex_literis("toc",
                    c->piscina))))
            {
                nav = n;
            }
        }
        nodi = xar_creare(c->piscina, (i32)magnitudo(XmlNodus*));
        descendentes_colligere(nav, "a", nodi);
        per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
        {
            n = *(XmlNodus**)xar_obtinere(nodi, i);
            a = xml_attributum_capere(n, "href");
            si (a)
            {
                introitum_addere(c, titulum_legere(c, n),
                    viam_resolvere(c,
                    basis, *a), fragmentum_nexus(*a));
            }
        }
    }
    alioquin
    {
        res    = c->index_toc.mensura > ZEPHYRUM ? rem_quaerere(c,
            NIHIL,
            &c->index_toc) : NIHIL;
        radix = res ? xml_membri(c, res->via, c->piscina,
            &vitium) : NIHIL;
        basis = res ? directorium_viae(res->via) : vacua();
        descendentes_colligere(radix, "navPoint", nodi);
        per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
        {
            n        = *(XmlNodus**)xar_obtinere(nodi, i);
            titulus  = titulum_legere(c, xml_invenire_liberum(n,
                "navLabel"));
            a = xml_invenire_liberum(n, "content")
                ? xml_attributum_capere(xml_invenire_liberum(n,
                "content"),
                "src") : NIHIL;
            si (a)
            {
                introitum_addere(c, titulus, viam_resolvere(c, basis,
                    *a),
                    fragmentum_nexus(*a));
            }
        }
    }
    si (xar_numerus(c->capita) == ZEPHYRUM)
    {
        si (xar_numerus(c->ordo) > ZEPHYRUM)
        {
            primum              = (CaputInternum*)xar_addere(c->capita);
            primum->titulus     = c->titulus;
            primum->ordo        = ZEPHYRUM;
            primum->fragmentum  = vacua();
        }
        redde;
    }
    /* prooemium: textus visibilis ante introitum primum? */
    primum   = (CaputInternum*)xar_obtinere(c->capita, ZEPHYRUM);
    textus   = FALSUM;
    inventa  = FALSUM;
    per (i = ZEPHYRUM; !textus && i <= primum->ordo; i++)
    {
        si (i == primum->ordo && primum->fragmentum.mensura == ZEPHYRUM)
        {
            frange;   /* introitus = initium documenti */
        }
        textus = textus_visibilis(lexemata_membri(c,
            *(chorda*)xar_obtinere(c->ordo, i), c->piscina, &vitium),
            i == primum->ordo ? primum->fragmentum : vacua(), &inventa);
    }
    si (textus)
    {
        prooemium.titulus     = vacua();
        prooemium.ordo        = ZEPHYRUM;
        prooemium.fragmentum  = vacua();
        (vacuum)xar_addere(c->capita);
        per (i = xar_numerus(c->capita) - I; i > ZEPHYRUM; i--)
        {
            *(CaputInternum*)xar_obtinere(c->capita, i) =
                *(CaputInternum*)xar_obtinere(c->capita, i - I);
        }
        *(CaputInternum*)xar_obtinere(c->capita, ZEPHYRUM) = prooemium;
    }
}

Codex*
codex_aperire (
                 chorda  datum,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* vitium)
{
           Codex* c;
        XmlNodus* radix;
        XmlNodus* n;
        XmlNodus* descriptio;
        XmlNodus* manifestum;
        XmlNodus* spina;
          chorda* via_descriptionis;
          chorda* unicum;
          chorda* a;
             Xar* omnes;
    ResManifesti* res;
             i32  i;
             i32  k;

    si (!piscina || !intern)
    {
        redde recusare("missing argument", piscina, vitium);
    }
    c = (Codex*)piscina_allocare(piscina, magnitudo(Codex));
    si (!c)
    {
        redde recusare("out of memory", piscina, vitium);
    }
    memset(c, ZEPHYRUM, magnitudo(Codex));
    c->piscina  = piscina;
    c->intern   = intern;
    c->arca     = sarcina_aperire(datum, piscina, vitium);
    si (!c->arca)
    {
        redde NIHIL;
    }
    /* container -> rootfile@full-path */
    radix = xml_membri(c, chorda_ex_literis("META-INF/container.xml",
        piscina), piscina, vitium);
    si (!radix)
    {
        redde NIHIL;
    }
    n = xml_invenire_liberum(radix, "rootfiles");
    n = n ? xml_invenire_liberum(n, "rootfile") : NIHIL;
    via_descriptionis = n ? xml_attributum_capere(n,
        "full-path") : NIHIL;
    si (!via_descriptionis || via_descriptionis->mensura == ZEPHYRUM)
    {
        redde recusare("container.xml names no rootfile", piscina,
            vitium);
    }
    /* directorium OPF: usque ad '/' ultimum */
    c->directorium.datum    = via_descriptionis->datum;
    c->directorium.mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < via_descriptionis->mensura; i++)
    {
        si (via_descriptionis->datum[i] == '/')
        {
            c->directorium.mensura = i + I;
        }
    }
    radix = xml_membri(c, *via_descriptionis, piscina, vitium);
    si (!radix)
    {
        redde NIHIL;
    }
    descriptio  = xml_invenire_liberum(radix, "metadata");
    manifestum  = xml_invenire_liberum(radix, "manifest");
    spina       = xml_invenire_liberum(radix, "spine");
    si (!descriptio || !manifestum || !spina)
    {
        redde recusare("package lacks descriptio, manifest or spine",
            piscina, vitium);
    }
    /* identitas: dc:identifier cuius id = unique-identifier */
    unicum  = xml_attributum_capere(radix, "unique-identifier");
    omnes   = xml_invenire_omnes_liberos(descriptio, "dc:identifier",
        piscina);
    per (i = ZEPHYRUM; omnes && i < xar_numerus(omnes); i++)
    {
        n = *(XmlNodus**)xar_obtinere(omnes, i);
        a = xml_attributum_capere(n, "id");
        si (   i == ZEPHYRUM
            || (unicum && a && chorda_aequalis(*a, *unicum)))
        {
            c->identitas = purgare(xml_textus_internus(n, piscina));
        }
    }
    c->titulus  = textus_liberi(c, descriptio, "dc:title");
    c->auctor   = textus_liberi(c, descriptio, "dc:creator");
    c->lingua   = textus_liberi(c, descriptio, "dc:language");
    /* manifestum */
    c->manifestum = xar_creare(piscina, (i32)magnitudo(ResManifesti));
    omnes = xml_invenire_omnes_liberos(manifestum, "item", piscina);
    per (i = ZEPHYRUM; omnes && i < xar_numerus(omnes); i++)
    {
        n = *(XmlNodus**)xar_obtinere(omnes, i);
        a = xml_attributum_capere(n, "href");
        si (!a || !xml_attributum_capere(n, "id"))
        {
            perge;
        }
        res = (ResManifesti*)xar_addere(c->manifestum);
        memset(res, ZEPHYRUM, magnitudo(ResManifesti));
        res->id   = *xml_attributum_capere(n, "id");
        res->via  = viam_resolvere(c, c->directorium, *a);
        si (xml_attributum_capere(n, "media-type"))
        {
            res->genus = *xml_attributum_capere(n, "media-type");
        }
        si (xml_attributum_capere(n, "properties"))
        {
            res->proprietates = *xml_attributum_capere(n, "properties");
        }
    }
    /* spina: idref -> via */
    a = xml_attributum_capere(spina, "toc");
    si (a)
    {
        c->index_toc = *a;
    }
    c->ordo  = xar_creare(piscina, (i32)magnitudo(chorda));
    omnes    = xml_invenire_omnes_liberos(spina, "itemref", piscina);
    per (i = ZEPHYRUM; omnes && i < xar_numerus(omnes); i++)
    {
        a = xml_attributum_capere(*(XmlNodus**)xar_obtinere(omnes, i),
            "idref");
        per (k = ZEPHYRUM; a && k < xar_numerus(c->manifestum); k++)
        {
            res = (ResManifesti*)xar_obtinere(c->manifestum, k);
            si (chorda_aequalis(res->id, *a))
            {
                *(chorda*)xar_addere(c->ordo) = res->via;
                frange;
            }
        }
    }
    capita_struere(c);
    redde c;
}

chorda
codex_identitas (
    constans Codex* c)
{
    redde c ? c->identitas : vacua();
}

chorda
codex_titulus (
    constans Codex* c)
{
    redde c ? c->titulus : vacua();
}

chorda
codex_auctor (
    constans Codex* c)
{
    redde c ? c->auctor : vacua();
}

chorda
codex_lingua (
    constans Codex* c)
{
    redde c ? c->lingua : vacua();
}

i32
codex_numerus_capitum (
    constans Codex* c)
{
    redde c && c->capita ? xar_numerus(c->capita) : ZEPHYRUM;
}

chorda
codex_caput_titulus (
    constans Codex* c,
               i32  k)
{
    si (k >= codex_numerus_capitum(c))
    {
        redde vacua();
    }
    redde ((constans CaputInternum*)xar_obtinere(c->capita,
        k))->titulus;
}

/* caput k: documenta spinae ab ancora capitis k usque ad ancoram
 * capitis sequentis ambulantur; involucra (nihil textus extra <a>)
 * omittuntur; ancora initii non inventa -> documentum totum */
CodexCaput*
codex_caput_legere (
    constans Codex* c,
               i32  k,
           Piscina* piscina,
            chorda* vitium)
{
    constans CaputInternum* a;
    constans CaputInternum* b;
                CodexCaput* caput;
                 Ambulatio  amb;
                       Xar* lexemata;
                       s32  d;
                       s32  ultimum;
                       b32  inventa;

    si (!c || !piscina || k >= codex_numerus_capitum(c))
    {
        redde (CodexCaput*)recusare("no such chapter", piscina, vitium);
    }
    a = (constans CaputInternum*)xar_obtinere(c->capita, k);
    b = k + I < codex_numerus_capitum(c)
        ? (constans CaputInternum*)xar_obtinere(c->capita, k
            + I) : NIHIL;
    si (!b)
    {
        ultimum = (s32)xar_numerus(c->ordo) - I;
    }
    alioquin si (b->fragmentum.mensura > ZEPHYRUM)
    {
        ultimum = (s32)b->ordo;
    }
    alioquin
    {
        ultimum = (s32)b->ordo - I;
    }
    memset(&amb, ZEPHYRUM, magnitudo(amb));
    amb.piscina    = piscina;
    amb.textus     = chorda_aedificator_creare(piscina, MXXIV);
    amb.linea      = chorda_aedificator_creare(piscina, CCLVI);
    amb.clausula   = chorda_aedificator_creare(piscina, MXXIV);
    amb.clausulae  = xar_creare(piscina, (i32)magnitudo(CodexClausula));
    amb.acervus    = xar_creare(piscina, (i32)magnitudo(Gradus));
    amb.genus      = CODEX_PARAGRAPHUS;
    si (   !amb.textus || !amb.linea || !amb.clausula || !amb.clausulae
        || !amb.acervus)
    {
        redde (CodexCaput*)recusare("out of memory", piscina, vitium);
    }
    per (d = (s32)a->ordo; d <= ultimum; d++)
    {
        lexemata = lexemata_membri(c, *(chorda*)xar_obtinere(c->ordo,
            (i32)d), piscina, vitium);
        si (!lexemata)
        {
            redde NIHIL;
        }
        inventa = FALSUM;
        si (!textus_visibilis(lexemata, vacua(), &inventa))
        {
            perge;   /* involucrum imaginis */
        }
        amb.initium = d == (s32)a->ordo ? a->fragmentum : vacua();
        amb.finis = b
            && d == (s32)b->ordo ? b->fragmentum : vacua();
        amb.emittere  = amb.initium.mensura == ZEPHYRUM;
        amb.finitum   = FALSUM;
        documentum_ambulare(&amb, lexemata);
        si (!amb.emittere)
        {
            amb.emittere = VERUM;   /* ancora initii absens */
            documentum_ambulare(&amb, lexemata);
        }
    }
    caput = (CodexCaput*)piscina_allocare(piscina,
        magnitudo(CodexCaput));
    si (!caput)
    {
        redde (CodexCaput*)recusare("out of memory", piscina, vitium);
    }
    caput->titulus    = a->titulus;
    caput->textus     = chorda_aedificator_finire(amb.textus);
    caput->clausulae  = amb.clausulae;
    redde caput;
}

/* spatium (spatium, tab, linea)? */
interior b32
spatium_est (
    i8 x)
{
    redde x == ' ' || x == '\t' || x == '\n' || x == '\r';
}

/* frustum [a, b) textus ut sententiam addere, spatiis extremis
 * demptis; vacuum praetermittitur */
interior vacuum
frustum_addere (
       Xar* exitus,
    chorda  textus,
       i32  a,
       i32  b,
       i32  clausula)
{
    CodexSententia* t;

    dum (a < b && spatium_est(textus.datum[a]))
    {
        a++;
    }
    dum (b > a && spatium_est(textus.datum[b - I]))
    {
        b--;
    }
    si (a >= b)
    {
        redde;
    }
    t            = (CodexSententia*)xar_addere(exitus);
    t->initium   = a;
    t->finis     = b;
    t->clausula  = clausula;
}

/* sententiae ante emendationes: titulus = una, versus = una per
 * lineam, paragraphus per sententia_fissio_indices (offsets a clausula
 * in textum capitis translati) */
interior Xar*
sententias_basis (
    constans CodexCaput* caput,
                Piscina* piscina)
{
                        Xar* x;
     constans CodexClausula* b;
    SententiaIndicesFructus  f;
                        i32  i;
                        i32  j;
                        i32  a;
                        i32  longitudo;

    x = xar_creare(piscina, (i32)magnitudo(CodexSententia));
    per (i = ZEPHYRUM; x && caput->clausulae
        && i < xar_numerus(caput->clausulae); i++)
    {
        b = (constans CodexClausula*)xar_obtinere(caput->clausulae, i);
        si (b->finis > caput->textus.mensura || b->initium > b->finis)
        {
            perge;
        }
        si (b->genus == CODEX_TITULUS)
        {
            frustum_addere(x, caput->textus, b->initium, b->finis, i);
        }
        alioquin si (b->genus == CODEX_VERSUS)
        {
            a = b->initium;
            per (j = b->initium; j <= b->finis; j++)
            {
                si (j == b->finis || caput->textus.datum[j] == '\n')
                {
                    frustum_addere(x, caput->textus, a, j, i);
                    a = j + I;
                }
            }
        }
        alioquin
        {
            longitudo  = b->finis - b->initium;
            f          = sententia_fissio_indices(chorda_sectio(
                caput->textus, b->initium, b->finis), piscina);
            per (j = ZEPHYRUM; f.indices && j < f.numerus; j++)
            {
                si (   f.indices[j].finis   <= longitudo
                    && f.indices[j].initium <= f.indices[j].finis)
                {
                    frustum_addere(x, caput->textus, b->initium
                        + f.indices[j].initium, b->initium
                        + f.indices[j].finis, i);
                }
            }
        }
    }
    redde x;
}

/* fissurae: positio intra sententiam (initium < positio < finis) eam
 * findit; pars nova ad primum octetum non spatium ab positione incipit;
 * ceterae nihil */
interior Xar*
fissuras_applicare (
                        Xar* u,
                     chorda  textus,
    constans CodexEmendatio* e,
                        i32  n,
                    Piscina* piscina)
{
                        Xar* x;
    constans CodexSententia* t;
                        i32  k;
                        i32  j;
                        i32  a;
                        i32  proxima;

    x = xar_creare(piscina, (i32)magnitudo(CodexSententia));
    per (k = ZEPHYRUM; x && k < xar_numerus(u); k++)
    {
        t = (constans CodexSententia*)xar_obtinere(u, k);
        a = t->initium;
        dum (VERUM)
        {
            proxima = t->finis;
            per (j = ZEPHYRUM; j < n; j++)
            {
                si (   e[j].genus == CODEX_FINDERE && e[j].positio > a
                    && e[j].positio < proxima)
                {
                    proxima = e[j].positio;
                }
            }
            si (proxima == t->finis)
            {
                frange;
            }
            frustum_addere(x, textus, a, proxima, t->clausula);
            a = proxima;
        }
        frustum_addere(x, textus, a, t->finis, t->clausula);
    }
    redde x;
}

/* iuncturae: sententia cuius initium = positio cum sequente iungitur,
 * si sequens in eadem clausula est; vincula ante applicationem omnia
 * notantur (ordo non refert, catenae iunguntur) */
interior Xar*
iuncturas_applicare (
                        Xar* u,
    constans CodexEmendatio* e,
                        i32  n,
                    Piscina* piscina)
{
                        Xar* x;
                        b32* vinculum;
    constans CodexSententia* s;
             CodexSententia* t;
                        i32  m;
                        i32  k;
                        i32  j;

    m = xar_numerus(u);
    x = xar_creare(piscina, (i32)magnitudo(CodexSententia));
    si (!x || m == ZEPHYRUM)
    {
        redde x;
    }
    vinculum = (b32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(b32));
    si (!vinculum)
    {
        redde NIHIL;
    }
    memset(vinculum, ZEPHYRUM, (size_t)m * magnitudo(b32));
    per (j = ZEPHYRUM; j < n; j++)
    {
        per (k = ZEPHYRUM; e[j].genus == CODEX_IUNGERE
            && k + I < m; k++)
        {
            s = (constans CodexSententia*)xar_obtinere(u, k);
            si (s->initium == e[j].positio)
            {
                vinculum[k] = s->clausula == ((constans CodexSententia*)
                    xar_obtinere(u, k + I))->clausula;
                frange;
            }
        }
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        s = (constans CodexSententia*)xar_obtinere(u, k);
        si (k > ZEPHYRUM && vinculum[k - I])
        {
            t = (CodexSententia*)xar_obtinere(x, xar_numerus(x) - I);
            t->finis = s->finis;
        }
        alioquin
        {
            *(CodexSententia*)xar_addere(x) = *s;
        }
    }
    redde x;
}

/* sententiae: basis, deinde fissurae omnes, deinde iuncturae omnes */
Xar*
codex_sententiae (
        constans CodexCaput* caput,
    constans CodexEmendatio* emendationes,
                        i32  numerus,
                    Piscina* piscina)
{
    Xar* u;

    si (!caput || !piscina)
    {
        redde NIHIL;
    }
    u = sententias_basis(caput, piscina);
    si (u && emendationes && numerus > ZEPHYRUM)
    {
        u = fissuras_applicare(u, caput->textus, emendationes, numerus,
            piscina);
        u = u ? iuncturas_applicare(u, emendationes, numerus, piscina)
            : NIHIL;
    }
    redde u;
}

/* vestigium ad positionem in textu? */
interior b32
vestigium_congruit (
    chorda textus,
       i32 positio,
    chorda vestigium)
{
    redde positio <= textus.mensura
        && vestigium.mensura <= textus.mensura - positio
        && (   vestigium.mensura == ZEPHYRUM
            || memcmp(textus.datum + positio, vestigium.datum,
                   (size_t)vestigium.mensura) == ZEPHYRUM);
}

/* positio congruens -> ipsa; aliter proxima congruens intra M octetos
 * (aequali distantia: posterior prius - textus ante auctus) */
s32
codex_reperire (
     constans CodexCaput* caput,
                     i32  positio,
                  chorda  vestigium)
{
    i32 d;

    si (!caput)
    {
        redde -I;
    }
    si (vestigium_congruit(caput->textus, positio, vestigium))
    {
        redde (s32)positio;
    }
    per (d = I; d <= M; d++)
    {
        si (   positio + d > positio
            && vestigium_congruit(caput->textus, positio + d,
            vestigium))
        {
            redde (s32)(positio + d);
        }
        si (   d <= positio
            && vestigium_congruit(caput->textus, positio - d,
            vestigium))
        {
            redde (s32)(positio - d);
        }
    }
    redde -I;
}
