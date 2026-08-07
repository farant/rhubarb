/* tools/natura_glossae.c - census glossarum (spec glossae par. 5)
 *
 * Copertura documentationis per corpus generum: 'la' per
 * <definitio> exsistentem, ceterae linguae per <glossa lingua=>.
 * Relatio ordinaria numquam obstat (index operum est dum corpus
 * rubet); -porta obstat. Denominator = genera propria (non
 * species/umbrae/individua).
 *
 * Divisio laboris (spec par. 3): canon FORMAM iudicat (lingua
 * necessaria - natura.canon), hic COPERTURA et GEMINA.
 *
 * -html VIA: encyclopaedia et lacunae FUSAE - plagula una sine ope
 * externa, DETERMINISTICA (nulla tempora: porta output confert,
 * numquam tempora - -probare crustae byte confert).
 *
 * NB 'glossae' in natura_examen/visus aliud est (sententia prima
 * definitionis, glossae.txt) - stratum vetus, non hoc.
 *
 * Exitus: 0 relatio/porta-serena, 1 porta cum lacunis, 2 NIHIL
 * CURSUM (argumenta mala, corpus vacuum) - numquam sanitas tacita.
 */
#include "postulata_posix.h"

#include "latina.h"
#include "natura.h"
#include "filum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>

#define EXTENSIO           ".genera"
#define EXTENSIO_LONGA     VII
#define NG_LINGUAE_MAXIMAE VIII
#define NG_CODEX_LONGUS    VIII
#define NG_COLUMNA         XXIV

nomen structura {
    character  codices[NG_LINGUAE_MAXIMAE][NG_CODEX_LONGUS];
    i32        numerus;
} NgLinguae;

interior b32    _extensionem_habet(constans chorda* titulus);
interior vacuum _stirpem_scribere(constans chorda* titulus,
                                  character* exitus, i32 capacitas);
interior b32    _linguas_legere(constans character* lista,
                                NgLinguae* linguae);
interior i32    _genus_linguae_quot(NaturaGenus* g,
                                    constans character* codex);
interior vacuum _columnam_scribere(FILE* f, constans chorda* t,
                                   i32 latitudo);
interior i32*   _ordo_exemplarium(NaturaBibliotheca* bib,
                                  Piscina* piscina);
interior vacuum _html_textum_scribere(FILE* f, chorda t);
interior vacuum _lineam_scribere(FILE* f, NaturaGenus* g);
interior StmlNodus* _glossam_invenire(NaturaGenus* g,
                                      constans character* codex);
interior StmlNodus* _definitio_elementi(StmlNodus* radix_canonis,
                                        constans chorda* petitum,
                                        constans chorda* intra);
interior vacuum _valorem_fictum_scribere(FILE* f,
                                         StmlNodus* def_attributi,
                                         Piscina* piscina);
interior vacuum _syntaxin_scribere(FILE* f,
                                   StmlNodus* radix_canonis,
                                   StmlNodus* def, i32 gradus,
                                   Piscina* piscina);
interior b32    _paginam_scribere(NaturaBibliotheca* bib,
                                  constans NgLinguae* linguae,
                                  StmlNodus* radix_canonis,
                                  constans character* via,
                                  Piscina* piscina);

interior b32
_extensionem_habet(
    constans chorda*  titulus)
{
    si (titulus->mensura <= (i32)EXTENSIO_LONGA)
    {
        redde FALSUM;
    }
    redde (b32)(memcmp(titulus->datum +
                    (titulus->mensura - (i32)EXTENSIO_LONGA),
                    EXTENSIO, (size_t)EXTENSIO_LONGA) == ZEPHYRUM);
}

interior vacuum
_stirpem_scribere(
    constans chorda*  titulus,
    character*        exitus,
    i32               capacitas)
{
    i32 longitudo;
    i32 i;

    longitudo = titulus->mensura - (i32)EXTENSIO_LONGA;
    si (longitudo >= capacitas)
    {
        longitudo = capacitas - I;
    }
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        exitus[i] = (character)titulus->datum[i];
    }
    exitus[longitudo] = '\0';
}

/* 'la,en,fr' -> codices; FALSUM si vacuus/longus/nimii */
interior b32
_linguas_legere(
    constans character*  lista,
    NgLinguae*           linguae)
{
    i32 n;
    i32 j;

    linguae->numerus = ZEPHYRUM;
    n = ZEPHYRUM;
    j = ZEPHYRUM;
    dum (VERUM)
    {
        character c;

        c = lista[n];
        si (c == ',' || c == '\0')
        {
            si (j == ZEPHYRUM ||
                linguae->numerus >= (i32)NG_LINGUAE_MAXIMAE)
            {
                redde FALSUM;
            }
            linguae->codices[linguae->numerus][j] = '\0';
            linguae->numerus++;
            j = ZEPHYRUM;
            si (c == '\0')
            {
                frange;
            }
        }
        alioquin
        {
            si (j >= (i32)(NG_CODEX_LONGUS - I))
            {
                redde FALSUM;
            }
            linguae->codices[linguae->numerus][j] = c;
            j++;
        }
        n++;
    }
    redde VERUM;
}

/* quot vices genus linguam ferat: 'la' = definitio AUT differentia
 * exsistens (0/1) - sub-genera differentia sola consulto utuntur
 * (definitio = genus proximum + differentia; mos natura_examen,
 * qui glossam derivatam eodem cadente petit), ceterae = glossae
 * congruentes numeratae (>1 = GEMINA) */
interior i32
_genus_linguae_quot(
    NaturaGenus*         g,
    constans character*  codex)
{
    i32 numerus;
    i32 i;
    i32 quot;

    si (strcmp(codex, "la") == ZEPHYRUM)
    {
        redde (stml_invenire_liberum(g->nodus, "definitio") ||
               stml_invenire_liberum(g->nodus, "differentia"))
                  ? I : ZEPHYRUM;
    }

    quot    = ZEPHYRUM;
    numerus = stml_numerus_liberorum(g->nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        chorda*    lingua;

        l = stml_liberum_ad_indicem(g->nodus, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*l->titulus, "glossa"))
        {
            perge;
        }
        lingua = stml_attributum_capere(l, "lingua");
        si (lingua && chorda_aequalis_literis(*lingua, codex))
        {
            quot++;
        }
    }
    redde quot;
}

interior vacuum
_columnam_scribere(
    FILE*             f,
    constans chorda*  t,
    i32               latitudo)
{
    i32 i;

    fprintf(f, "%.*s", (integer)t->mensura,
            (constans character*)t->datum);
    per (i = t->mensura; i < latitudo; i++)
    {
        putc(' ', f);
    }
}

/* indices exemplarium ordine alphabetico stirpium - ordo lectionis
 * directorii systematis plagularum est (arbitrarius ET inter
 * machinas instabilis); relatio et pagina ambae hoc ordine
 * scribuntur, unde output vere deterministicum fit */
interior i32*
_ordo_exemplarium(
    NaturaBibliotheca*  bib,
    Piscina*            piscina)
{
    i32* ordo;
    i32  n;
    i32  i;
    i32  j;

    n    = xar_numerus(bib->exemplaria);
    ordo = (i32*)piscina_allocare(piscina,
               (i32)((size_t)n * magnitudo(i32)));
    per (i = ZEPHYRUM; i < n; i++)
    {
        ordo[i] = i;
    }

    per (i = I; i < n; i++)
    {
        i32 clavis;

        clavis = ordo[i];
        j = i;
        dum (j > ZEPHYRUM)
        {
            NaturaExemplar* a;
            NaturaExemplar* b;
            i32             minima;
            integer         ordo_bytium;

            a = *(NaturaExemplar**)xar_obtinere(bib->exemplaria,
                                                ordo[j - I]);
            b = *(NaturaExemplar**)xar_obtinere(bib->exemplaria,
                                                clavis);
            minima = a->stirps->mensura < b->stirps->mensura
                         ? a->stirps->mensura : b->stirps->mensura;
            ordo_bytium = memcmp(a->stirps->datum, b->stirps->datum,
                                 (size_t)minima);
            si (ordo_bytium < ZEPHYRUM ||
                (ordo_bytium == ZEPHYRUM &&
                 a->stirps->mensura <= b->stirps->mensura))
            {
                frange;
            }
            ordo[j] = ordo[j - I];
            j--;
        }
        ordo[j] = clavis;
    }
    redde ordo;
}

/* textus in html: <, >, & evasa; cetera verbatim (UTF-8 transit) */
interior vacuum
_html_textum_scribere(
    FILE*   f,
    chorda  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c;

        c = (character)t.datum[i];
        si (c == '<')          { fputs("&lt;", f); }
        alioquin si (c == '>') { fputs("&gt;", f); }
        alioquin si (c == '&') { fputs("&amp;", f); }
        alioquin               { putc(c, f); }
    }
}

/* catena parentum a radice ad genus: 'vivens &rarr; planta' */
interior vacuum
_lineam_scribere(
    FILE*         f,
    NaturaGenus*  g)
{
    NaturaGenus* catena[XXXII];
    i32          n;
    i32          i;
    NaturaGenus* cursor;

    n = ZEPHYRUM;
    per (cursor = g; cursor && n < (i32)XXXII;
         cursor = cursor->parens)
    {
        catena[n] = cursor;
        n++;
    }
    per (i = n; i > ZEPHYRUM; i--)
    {
        _html_textum_scribere(f, *catena[i - I]->titulus);
        si (i > I)
        {
            fputs(" &rarr; ", f);
        }
    }
}

/* glossam primam linguae datae reddere, vel NIHIL */
interior StmlNodus*
_glossam_invenire(
    NaturaGenus*         g,
    constans character*  codex)
{
    i32 numerus;
    i32 i;

    numerus = stml_numerus_liberorum(g->nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        chorda*    lingua;

        l = stml_liberum_ad_indicem(g->nodus, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*l->titulus, "glossa"))
        {
            perge;
        }
        lingua = stml_attributum_capere(l, "lingua");
        si (lingua && chorda_aequalis_literis(*lingua, codex))
        {
            redde l;
        }
    }
    redde NIHIL;
}

/* definitionem elementi in canone cocto invenire: intra= parentem
 * IMMEDIATUM nominat (eventum intra="historia"), ergo congruentia
 * intra praefertur, definitio sine intra cadens est. Canon plagula
 * STML est - lectio recta, sine lib/canon.c. */
interior StmlNodus*
_definitio_elementi(
    StmlNodus*        radix_canonis,
    constans chorda*  petitum,
    constans chorda*  intra)
{
    StmlNodus* planum;
    i32        numerus;
    i32        i;

    planum  = NIHIL;
    numerus = stml_numerus_liberorum(radix_canonis);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        chorda*    titulus_n;
        chorda*    intra_n;

        l = stml_liberum_ad_indicem(radix_canonis, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*l->titulus, "elementum"))
        {
            perge;
        }
        titulus_n = stml_attributum_capere(l, "nomen");
        si (!titulus_n || !chorda_aequalis(*titulus_n, *petitum))
        {
            perge;
        }
        intra_n = stml_attributum_capere(l, "intra");
        si (intra_n)
        {
            si (intra && chorda_aequalis(*intra_n, *intra))
            {
                redde l;
            }
        }
        alioquin si (!planum)
        {
            planum = l;
        }
    }
    redde planum;
}

/* valor fictus pro genere attributi: electio optiones veras
 * monstrat (usque ad IV), cetera formam generis */
interior vacuum
_valorem_fictum_scribere(
    FILE*       f,
    StmlNodus*  def_attributi,
    Piscina*    piscina)
{
    chorda* genus_v;

    genus_v = stml_attributum_capere(def_attributi, "genus");
    si (!genus_v)
    {
        fputs("...", f);
        redde;
    }
    si (chorda_aequalis_literis(*genus_v, "electio"))
    {
        i32 numerus;
        i32 i;
        i32 scriptae;

        scriptae = ZEPHYRUM;
        numerus  = stml_numerus_liberorum(def_attributi);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(def_attributi, i);
            si (!l || l->genus != STML_NODUS_ELEMENTUM ||
                !chorda_aequalis_literis(*l->titulus, "optio"))
            {
                perge;
            }
            si (scriptae == IV)
            {
                fputs("|...", f);
                frange;
            }
            si (scriptae > ZEPHYRUM)
            {
                putc('|', f);
            }
            _html_textum_scribere(f,
                stml_textus_normalizatus(l, piscina));
            scriptae++;
        }
        si (scriptae == ZEPHYRUM)
        {
            fputs("...", f);
        }
    }
    alioquin si (chorda_aequalis_literis(*genus_v, "identitas"))
    {
        fputs("#nomen", f);
    }
    alioquin si (chorda_aequalis_literis(*genus_v, "referentia"))
    {
        fputs("#res|.genus", f);
    }
    alioquin si (chorda_aequalis_literis(*genus_v, "numerus"))
    {
        putc('0', f);
    }
    alioquin si (chorda_aequalis_literis(*genus_v, "veritas"))
    {
        fputs("verum|falsum", f);
    }
    alioquin si (chorda_aequalis_literis(*genus_v, "dies"))
    {
        fputs("AAAA-MM-DD", f);
    }
    alioquin
    {
        fputs("...", f);
    }
}

/* exemplum syntacticum tagi e definitione canonis: attributa cum
 * valoribus fictis, liberi nidificati (profunditas II), textus
 * '...'. Effusio iam evasa ('lt;' litteralis) - intra <pre> it. */
interior vacuum
_syntaxin_scribere(
    FILE*       f,
    StmlNodus*  radix_canonis,
    StmlNodus*  def,
    i32         gradus,
    Piscina*    piscina)
{
    chorda* titulus_t;
    b32     textus_habet;
    i32     n_attributa;
    i32     n_liberorum;
    i32     numerus;
    i32     i;
    i32     scripta;

    titulus_t = stml_attributum_capere(def, "nomen");
    si (!titulus_t)
    {
        redde;
    }
    textus_habet = stml_attributum_capere(def, "textus") != NIHIL;

    n_attributa = ZEPHYRUM;
    n_liberorum = ZEPHYRUM;
    numerus     = stml_numerus_liberorum(def);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;

        l = stml_liberum_ad_indicem(def, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (chorda_aequalis_literis(*l->titulus, "attributum"))
        {
            n_attributa++;
        }
        alioquin si (chorda_aequalis_literis(*l->titulus,
                                             "liberum"))
        {
            n_liberorum++;
        }
    }

    per (i = ZEPHYRUM; i < gradus * II; i++)
    {
        putc(' ', f);
    }
    fputs("&lt;", f);
    _html_textum_scribere(f, *titulus_t);

    scripta = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        chorda*    titulus_a;

        l = stml_liberum_ad_indicem(def, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*l->titulus, "attributum"))
        {
            perge;
        }
        titulus_a = stml_attributum_capere(l, "nomen");
        si (!titulus_a)
        {
            perge;
        }
        si (n_attributa > III && scripta > ZEPHYRUM)
        {
            i32 j;

            fputs("\n", f);
            per (j = ZEPHYRUM; j < gradus * II + IV; j++)
            {
                putc(' ', f);
            }
        }
        alioquin
        {
            putc(' ', f);
        }
        _html_textum_scribere(f, *titulus_a);
        fputs("=\"", f);
        _valorem_fictum_scribere(f, l, piscina);
        putc('"', f);
        scripta++;
    }

    si (textus_habet)
    {
        fputs("&gt;...&lt;/", f);
        _html_textum_scribere(f, *titulus_t);
        fputs("&gt;\n", f);
        redde;
    }
    si (n_liberorum == ZEPHYRUM || gradus >= II)
    {
        fputs("/&gt;\n", f);
        redde;
    }

    fputs("&gt;\n", f);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        chorda*    petitum;
        StmlNodus* def_l;

        l = stml_liberum_ad_indicem(def, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*l->titulus, "liberum"))
        {
            perge;
        }
        petitum = stml_attributum_capere(l, "nomen");
        si (!petitum)
        {
            perge;
        }
        def_l = _definitio_elementi(radix_canonis, petitum,
                                    titulus_t);
        si (def_l)
        {
            _syntaxin_scribere(f, radix_canonis, def_l,
                               gradus + I, piscina);
        }
        alioquin
        {
            i32 j;

            per (j = ZEPHYRUM; j < (gradus + I) * II; j++)
            {
                putc(' ', f);
            }
            fputs("&lt;", f);
            _html_textum_scribere(f, *petitum);
            fputs("/&gt;\n", f);
        }
    }
    per (i = ZEPHYRUM; i < gradus * II; i++)
    {
        putc(' ', f);
    }
    fputs("&lt;/", f);
    _html_textum_scribere(f, *titulus_t);
    fputs("&gt;\n", f);
}

/* encyclopaedia et lacunae FUSAE (spec glossae par. 6): plagula
 * una sine ope externa. DETERMINISTICA - nulla tempora, ergo
 * -probare crustae byte conferre potest. Lacuna VISIBILIS
 * ('deest') pagina index operum est; soluta e pagina evanescit. */
interior b32
_paginam_scribere(
    NaturaBibliotheca*   bib,
    constans NgLinguae*  linguae,
    StmlNodus*           radix_canonis,
    constans character*  via,
    Piscina*             piscina)
{
    FILE* f;
    i32*  ordo;
    i32   m;
    i32   g_i;
    i32   l_i;

    f = fopen(via, "w");
    si (!f)
    {
        redde FALSUM;
    }
    ordo = _ordo_exemplarium(bib, piscina);

    fputs("<!DOCTYPE html>\n"
          "<html lang=\"la\"><head><meta charset=\"utf-8\">\n"
          "<title>Glossae - documentatio generum</title>\n"
          "<style>\n"
          " body { font-family: Menlo, monospace; margin: 2rem auto;\n"
          "        max-width: 72rem; background: #14120f;\n"
          "        color: #d8d0c0; line-height: 1.5; }\n"
          " h1 { color: #e8c878; font-size: 1.4rem; }\n"
          " h2 { color: #b8a878; border-bottom: 1px solid #3a352c;\n"
          "      padding-bottom: .3rem; margin-top: 2rem;\n"
          "      font-size: 1.1rem; }\n"
          " h3 { color: #d8d0c0; margin-bottom: .1rem;\n"
          "      font-size: 1rem; }\n"
          " .linea { color: #8a8272; font-size: .85rem;\n"
          "          margin: 0 0 .3rem 0; }\n"
          " .def { color: #c8bfa8; margin: .2rem 0; }\n"
          " .glossa { margin: .2rem 0; }\n"
          " .glossa b { color: #e8c878; margin-right: .5rem; }\n"
          " .deest { color: #c86060; margin: .2rem 0; }\n"
          " .deest b { margin-right: .5rem; }\n"
          " .numeri { color: #8a8272; }\n"
          " .numeri b { color: #e8c878; }\n"
          " .tabula { color: #8a8272; line-height: 1.9; }\n"
          " .tabula a { color: #b8a878; text-decoration: none;\n"
          "             margin-right: .6rem; }\n"
          " .tabula a:hover { color: #e8c878; }\n"
          " details.syn { margin: .1rem 0 .8rem 0; }\n"
          " details.syn summary { color: #8a8272;\n"
          "                       cursor: pointer;\n"
          "                       font-size: .85rem; }\n"
          " details.syn pre { background: #1c1914;\n"
          "                   border: 1px solid #3a352c;\n"
          "                   padding: .6rem .8rem;\n"
          "                   overflow-x: auto; color: #c8bfa8;\n"
          "                   font-size: .85rem;\n"
          "                   line-height: 1.45;\n"
          "                   margin: .3rem 0 0 0; }\n"
          "</style></head><body>\n"
          "<h1>GLOSSAE - documentatio generum</h1>\n", f);

    /* ---- compendium ---- */
    fputs("<p class=\"numeri\">", f);
    per (l_i = ZEPHYRUM; l_i < linguae->numerus; l_i++)
    {
        i32 habentia;
        i32 omnia;

        habentia = ZEPHYRUM;
        omnia    = ZEPHYRUM;
        per (g_i = ZEPHYRUM; g_i < xar_numerus(bib->genera_omnia);
             g_i++)
        {
            NaturaGenus* g;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia,
                                             g_i);
            omnia++;
            si (_genus_linguae_quot(g,
                    linguae->codices[l_i]) >= (i32)I)
            {
                habentia++;
            }
        }
        si (l_i > ZEPHYRUM)
        {
            fputs(" &middot; ", f);
        }
        fprintf(f, "%s <b>%u/%u</b>", linguae->codices[l_i],
                habentia, omnia);
    }
    fputs("</p>\n", f);

    /* ---- tabula saltuum: modulus quisque ancoram fert ---- */
    fputs("<p class=\"tabula\">", f);
    per (m = ZEPHYRUM; m < xar_numerus(bib->exemplaria); m++)
    {
        NaturaExemplar* ex;
        b32             genera_habet;

        ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria,
                                             ordo[m]);
        genera_habet = FALSUM;
        per (g_i = ZEPHYRUM; g_i < xar_numerus(bib->genera_omnia);
             g_i++)
        {
            NaturaGenus* g;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia,
                                             g_i);
            si (chorda_aequalis(*g->modulus, *ex->stirps))
            {
                genera_habet = VERUM;
                frange;
            }
        }
        si (!genera_habet)
        {
            perge;
        }
        fputs("<a href=\"#", f);
        fprintf(f, "%.*s", (integer)ex->stirps->mensura,
                (constans character*)ex->stirps->datum);
        fputs("\">", f);
        _html_textum_scribere(f, *ex->stirps);
        fputs("</a>\n", f);
    }
    fputs("</p>\n", f);

    /* ---- exemplaria ---- */
    per (m = ZEPHYRUM; m < xar_numerus(bib->exemplaria); m++)
    {
        NaturaExemplar* ex;
        i32             omnia_moduli;
        i32             habentia[NG_LINGUAE_MAXIMAE];

        ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria,
                                             ordo[m]);
        omnia_moduli = ZEPHYRUM;
        per (l_i = ZEPHYRUM; l_i < linguae->numerus; l_i++)
        {
            habentia[l_i] = ZEPHYRUM;
        }
        per (g_i = ZEPHYRUM; g_i < xar_numerus(bib->genera_omnia);
             g_i++)
        {
            NaturaGenus* g;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia,
                                             g_i);
            si (!chorda_aequalis(*g->modulus, *ex->stirps))
            {
                perge;
            }
            omnia_moduli++;
            per (l_i = ZEPHYRUM; l_i < linguae->numerus; l_i++)
            {
                si (_genus_linguae_quot(g,
                        linguae->codices[l_i]) >= (i32)I)
                {
                    habentia[l_i]++;
                }
            }
        }
        si (omnia_moduli == ZEPHYRUM)
        {
            perge;
        }

        fputs("<h2 id=\"", f);
        fprintf(f, "%.*s", (integer)ex->stirps->mensura,
                (constans character*)ex->stirps->datum);
        fputs("\">", f);
        _html_textum_scribere(f, *ex->stirps);
        fputs(" <span class=\"numeri\">", f);
        per (l_i = ZEPHYRUM; l_i < linguae->numerus; l_i++)
        {
            si (l_i > ZEPHYRUM)
            {
                fputs(" &middot; ", f);
            }
            fprintf(f, "%s %u/%u", linguae->codices[l_i],
                    habentia[l_i], omnia_moduli);
        }
        fputs("</span></h2>\n", f);

        per (g_i = ZEPHYRUM; g_i < xar_numerus(bib->genera_omnia);
             g_i++)
        {
            NaturaGenus* g;
            StmlNodus*   def;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia,
                                             g_i);
            si (!chorda_aequalis(*g->modulus, *ex->stirps))
            {
                perge;
            }

            fputs("<h3>", f);
            _html_textum_scribere(f, *g->titulus);
            fputs("</h3>\n<p class=\"linea\">", f);
            _lineam_scribere(f, g);
            fputs("</p>\n", f);

            per (l_i = ZEPHYRUM; l_i < linguae->numerus; l_i++)
            {
                constans character* codex;

                codex = linguae->codices[l_i];
                si (strcmp(codex, "la") == ZEPHYRUM)
                {
                    def = stml_invenire_liberum(g->nodus,
                                                "definitio");
                    si (!def)
                    {
                        def = stml_invenire_liberum(g->nodus,
                                                    "differentia");
                    }
                    si (def)
                    {
                        fputs("<p class=\"def\"><b>la</b> ", f);
                        _html_textum_scribere(f,
                            stml_textus_normalizatus(def,
                                                     piscina));
                        fputs("</p>\n", f);
                    }
                    alioquin
                    {
                        fputs("<p class=\"deest\"><b>la</b> "
                              "&#9888; deest</p>\n", f);
                    }
                }
                alioquin
                {
                    StmlNodus* gl;

                    gl = _glossam_invenire(g, codex);
                    si (gl)
                    {
                        fprintf(f,
                            "<p class=\"glossa\"><b>%s</b> ",
                            codex);
                        _html_textum_scribere(f,
                            stml_textus_normalizatus(gl,
                                                     piscina));
                        fputs("</p>\n", f);
                    }
                    alioquin
                    {
                        fprintf(f,
                            "<p class=\"deest\"><b>%s</b> "
                            "&#9888; deest</p>\n", codex);
                    }
                }
            }

            /* syntaxis: exemplum canonicum tagi (definitio in
             * canone cocto kebab stat, titulus generis snake) */
            si (radix_canonis)
            {
                character  kebab[CCLVI];
                chorda     petitum;
                StmlNodus* def_g;
                i32        k;
                i32        longitudo;

                longitudo = g->titulus->mensura;
                si (longitudo >= (i32)magnitudo(kebab))
                {
                    longitudo = (i32)magnitudo(kebab) - I;
                }
                per (k = ZEPHYRUM; k < longitudo; k++)
                {
                    character c;

                    c = (character)g->titulus->datum[k];
                    kebab[k] = c == '_' ? '-' : c;
                }
                kebab[longitudo] = '\0';
                petitum = chorda_ex_literis(kebab, piscina);

                def_g = _definitio_elementi(radix_canonis,
                                            &petitum, NIHIL);
                si (def_g)
                {
                    fputs("<details class=\"syn\"><summary>"
                          "syntaxis</summary>\n<pre>", f);
                    _syntaxin_scribere(f, radix_canonis, def_g,
                                       ZEPHYRUM, piscina);
                    fputs("</pre></details>\n", f);
                }
            }
        }
    }

    fputs("<p class=\"numeri\">generata a natura_glossae - "
          "regenera: ./tools/natura_glossae.sh -pagina</p>\n"
          "</body></html>\n", f);

    si (fclose(f) != ZEPHYRUM)
    {
        remove(via);
        redde FALSUM;
    }
    redde VERUM;
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*              piscina;
    NaturaBibliotheca*    bib;
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;
    constans character*   radix;
    constans character*   lista_linguarum;
    constans character*   via_html;
    NgLinguae             linguae;
    b32                   modus_porta;
    b32                   modus_machina;
    s32                   i;
    i32*                  ordo;
    i32                   onerata;
    i32                   lacunae;
    i32                   m;
    i32                   g_i;
    i32                   l_i;
    i32                   summa_habentia[NG_LINGUAE_MAXIMAE];
    i32                   summa_omnia;

    radix           = "natura";
    lista_linguarum = "la,en,fr";
    via_html        = NIHIL;
    modus_porta     = FALSUM;
    modus_machina   = FALSUM;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-porta") == ZEPHYRUM)
        {
            modus_porta = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            modus_machina = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-radix") == ZEPHYRUM &&
                     i + I < numerus)
        {
            radix = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-linguae") == ZEPHYRUM &&
                     i + I < numerus)
        {
            lista_linguarum = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-html") == ZEPHYRUM &&
                     i + I < numerus)
        {
            via_html = argumenta[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: natura_glossae [-radix DIR] "
                "[-linguae la,en,fr] [-porta] [-machina] "
                "[-html VIA]\n");
            redde II;
        }
    }

    si (!_linguas_legere(lista_linguarum, &linguae))
    {
        fprintf(stderr,
            "natura_glossae: -linguae '%s' legi nequit\n",
            lista_linguarum);
        redde II;
    }

    piscina = piscina_generare_dynamicum("natura_glossae", 4194304);
    bib     = natura_bibliotheca_creare(piscina);
    si (!bib)
    {
        fprintf(stderr,
            "natura_glossae: bibliotheca creari nequit\n");
        redde II;
    }

    iter = directorium_iterator_aperire(radix, piscina);
    si (!iter)
    {
        fprintf(stderr, "natura_glossae: '%s' aperiri nequit\n",
                radix);
        redde II;
    }

    onerata = ZEPHYRUM;
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        character via[DXII];
        character stirps[CCLVI];
        chorda    fons;

        si (introitus->genus != INTROITUS_FILUM ||
            !_extensionem_habet(&introitus->titulus))
        {
            perge;
        }
        si ((size_t)introitus->titulus.mensura + strlen(radix) + II >
            magnitudo(via))
        {
            perge;
        }
        sprintf(via, "%s/%.*s", radix,
                (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        _stirpem_scribere(&introitus->titulus, stirps,
                          (i32)magnitudo(stirps));
        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "natura_glossae: '%s' legi nequit\n",
                    via);
            perge;
        }
        natura_legere(bib, fons, stirps);
        onerata++;
    }
    directorium_iterator_claudere(iter);

    si (onerata == ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_glossae: NULLUM exemplar in '%s' - nihil "
            "cursum\n", radix);
        redde II;
    }

    /* vulnera corporis lint alterius sunt (natura_examen) -
     * copertura etiam super corpore vulnerato numeratur */
    natura_nectere(bib);

    ordo = _ordo_exemplarium(bib, piscina);

    /* ---- tabula coperturae ---- */

    summa_omnia = ZEPHYRUM;
    per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
    {
        summa_habentia[l_i] = ZEPHYRUM;
    }

    si (!modus_machina)
    {
        printf("%-24s", "GLOSSAE");
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            printf("%-10s", linguae.codices[l_i]);
        }
        printf("\n");
    }

    per (m = ZEPHYRUM; m < xar_numerus(bib->exemplaria); m++)
    {
        NaturaExemplar* ex;
        i32             omnia_moduli;
        i32             habentia[NG_LINGUAE_MAXIMAE];

        ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria,
                                             ordo[m]);
        omnia_moduli = ZEPHYRUM;
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            habentia[l_i] = ZEPHYRUM;
        }

        per (g_i = ZEPHYRUM; g_i < xar_numerus(bib->genera_omnia);
             g_i++)
        {
            NaturaGenus* g;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia,
                                             g_i);
            si (!chorda_aequalis(*g->modulus, *ex->stirps))
            {
                perge;
            }
            omnia_moduli++;
            per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
            {
                si (_genus_linguae_quot(g,
                        linguae.codices[l_i]) >= (i32)I)
                {
                    habentia[l_i]++;
                }
            }
        }

        si (omnia_moduli == ZEPHYRUM)
        {
            perge;   /* exemplar sine generibus propriis */
        }
        summa_omnia += omnia_moduli;

        si (modus_machina)
        {
            per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
            {
                _columnam_scribere(stdout, ex->stirps, ZEPHYRUM);
                printf("\t%s\t%u\t%u\n", linguae.codices[l_i],
                       habentia[l_i], omnia_moduli);
                summa_habentia[l_i] += habentia[l_i];
            }
        }
        alioquin
        {
            _columnam_scribere(stdout, ex->stirps,
                               (i32)NG_COLUMNA);
            per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
            {
                character fractio[XXXII];

                sprintf(fractio, "%u/%u", habentia[l_i],
                        omnia_moduli);
                printf("%-10s", fractio);
                summa_habentia[l_i] += habentia[l_i];
            }
            printf("\n");
        }
    }

    si (modus_machina)
    {
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            printf("SUMMA\t%s\t%u\t%u\n", linguae.codices[l_i],
                   summa_habentia[l_i], summa_omnia);
        }
    }
    alioquin
    {
        printf("%-24s", "SUMMA");
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            character fractio[XXXII];

            sprintf(fractio, "%u/%u", summa_habentia[l_i],
                    summa_omnia);
            printf("%-10s", fractio);
        }
        printf("\n");
    }

    /* ---- carentia et gemina nominatim (index operum) ---- */

    lacunae = ZEPHYRUM;
    per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
    {
        per (m = ZEPHYRUM; m < xar_numerus(bib->exemplaria); m++)
        {
            NaturaExemplar* ex;

            ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria,
                                                 ordo[m]);
            per (g_i = ZEPHYRUM;
                 g_i < xar_numerus(bib->genera_omnia); g_i++)
            {
                NaturaGenus* g;
                i32          quot;

                g = *(NaturaGenus**)xar_obtinere(
                        bib->genera_omnia, g_i);
                si (!chorda_aequalis(*g->modulus, *ex->stirps))
                {
                    perge;
                }
                quot = _genus_linguae_quot(g,
                           linguae.codices[l_i]);
                si (quot == ZEPHYRUM)
                {
                    lacunae++;
                    si (modus_machina)
                    {
                        printf("CARENS\t");
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("\t");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\t%s\n", linguae.codices[l_i]);
                    }
                    alioquin
                    {
                        printf("CARENS %s  ",
                               linguae.codices[l_i]);
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("/");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\n");
                    }
                }
                alioquin si (quot > (i32)I)
                {
                    lacunae++;
                    si (modus_machina)
                    {
                        printf("GEMINA\t");
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("\t");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\t%s\n", linguae.codices[l_i]);
                    }
                    alioquin
                    {
                        printf("GEMINA %s  ",
                               linguae.codices[l_i]);
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("/");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\n");
                    }
                }
            }
        }
    }

    si (via_html)
    {
        StmlNodus* radix_canonis;

        /* canon coctus monolithi pro syntaxi tagorum - absens
         * (corpus fictum probationum) = pagina sine syntaxi,
         * numquam defectus */
        radix_canonis = NIHIL;
        si (strlen(radix) + XXXII < (size_t)DXII)
        {
            character via_canonis[DXII];
            chorda    fons_canonis;

            sprintf(via_canonis, "%s/cocta/individua.canon",
                    radix);
            fons_canonis = filum_legere_totum(via_canonis,
                                              piscina);
            si (fons_canonis.mensura > ZEPHYRUM)
            {
                StmlResultus r;

                r = stml_legere(fons_canonis, piscina,
                                bib->intern);
                si (r.successus)
                {
                    radix_canonis = r.elementum_radix;
                }
            }
        }

        si (!_paginam_scribere(bib, &linguae, radix_canonis,
                               via_html, piscina))
        {
            fprintf(stderr,
                "natura_glossae: pagina '%s' scribi nequit\n",
                via_html);
            redde II;
        }
    }

    si (modus_porta && lacunae > ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_glossae: PORTA - lacunae %u\n", lacunae);
        redde I;
    }
    redde ZEPHYRUM;
}
