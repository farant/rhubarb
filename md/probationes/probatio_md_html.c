/* probatio_md_html.c - Porta programmatis md -> html (B2.2)
 *
 * md_html_reddere super programma vivum md/html/md-html.stml: exempla
 * spicae (capitulum cum emphasi, lista nidificata, nexus referens)
 * OCTETIM pinnata - octeti NOSTRI (nullae lineae novae inter bloca:
 * templum spatium album fingere nequit; oraculum CommonMark cum
 * normalizatore B3 venit), deinde genera cetera: successus + octeti
 * impressi (pinnandi post inspectionem). Culpa plantata nativitatis:
 * bracchium h2 -> h3 in programmate.
 */
#include "latina.h"
#include "md_html.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_plagulam_legere (
               Piscina* piscina,
    constans character* via)
{
        chorda c;
         FILE* f;
        longus longitudo;
    character* memoria;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    f          = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde c;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde c;
    }
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde c;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde c;
    }
    fclose(f);
    c.datum    = (i8*)memoria;
    c.mensura  = (i32)longitudo;
    redde c;
}

interior MdHtmlResultus
_reddere (
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda  programma,
     constans character* md)
{
    redde md_html_reddere(piscina, intern, md, (i32)strlen(md),
        programma);
}

/* html pinnatum octetim; in defectu causa aut html receptum imprimitur */
interior vacuum
_pinna (
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda  programma,
     constans character* md,
     constans character* html)
{
    MdHtmlResultus r = _reddere(piscina, intern, programma, md);

    CREDO_VERUM (r.successus);
    si (!r.successus)
    {
        imprimere("  causa: %.*s\n", (integer)r.causa.mensura,
            (constans character*)r.causa.datum);
        redde;
    }
    CREDO_CHORDA_AEQUALIS_LITERIS (r.html, html);
}

/* successus + impressio (pinnandum post inspectionem) */
interior vacuum
_ostende (
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda  programma,
     constans character* md)
{
    MdHtmlResultus r = _reddere(piscina, intern, programma, md);

    CREDO_VERUM (r.successus);
    si (r.successus)
    {
        CREDO_VERUM (r.html.mensura > ZEPHYRUM);
        imprimere("  %.*s\n", (integer)r.html.mensura,
            (constans character*)r.html.datum);
    }
    alioquin
    {
        imprimere("  causa: %.*s\n", (integer)r.causa.mensura,
            (constans character*)r.causa.datum);
    }
}

s32
principale (
    vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* radix_viae;
              character  via[4096];
                 chorda  programma;

    piscina = piscina_generare_dynamicum("probatio_md_html", 67108864);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern      = internamentum_creare(piscina);
    radix_viae  = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    sprintf(via, "%s/md/html/md-html.stml", radix_viae);
    programma = _plagulam_legere(piscina, via);
    CREDO_VERUM (programma.datum != NIHIL);
    si (programma.datum == NIHIL)
    {
        imprimere("FRACTA: programma absens: %s\n", via);
        credo_imprimere_compendium();
        redde I;
    }

    imprimere("\n--- exempla spicae: octetim ---\n");
    _pinna(piscina, intern, programma, "## Hello *w*\n",
        "<h2>Hello <em>w</em></h2>");
    _pinna(piscina, intern, programma, "- one\n  - nested\n- two\n",
        "<ul><li>one<ul><li>nested</li></ul></li><li>two</li></ul>");
    _pinna(piscina, intern, programma, "[a][x]\n\n[x]: /u \"t\"\n",
        "<p><a href=\"/u\" title=\"t\">a</a></p>");

    imprimere("\n--- genera cetera: octetim (post inspectionem) ---\n");
    _pinna(piscina, intern, programma, "```c\nint x;\n```\n",
        "<pre><code class=\"language-c\">int x;\n</code></pre>");
    _pinna(piscina, intern, programma, "    ind\n",
        "<pre><code>ind\n</code></pre>");
    _pinna(piscina, intern, programma, "a  \nb\nc\n",
        "<p>a<br>\nb\nc</p>");
    _pinna(piscina, intern, programma,
        "![alt *t*](/i.png \"ti\") `co de` <http://x.y> a&amp;b\n",
        "<p><img src=\"/i.png\" alt=\"alt t\" title=\"ti\"> <code>co de</code>"
        " <a href=\"http://x.y\">http://x.y</a> a&amp;b</p>");
    _pinna(piscina, intern, programma,
        "| a | b |\n|:--|--:|\n| 1 | 2 |\n",
        "<table><thead><tr><th align=\"left\">a</th><th align=\"right\">b</th>"
        "</tr></thead><tbody><tr><td align=\"left\">1</td>"
        "<td align=\"right\">2</td></tr></tbody></table>");
    _pinna(piscina, intern, programma, "3. y\n\n- [x] d\n- [ ] e\n",
        "<ol start=\"3\"><li>y</li></ol><ul><li><input type=\"checkbox\""
        " checked disabled>d</li><li><input type=\"checkbox\" disabled>e"
        "</li></ul>");
    _pinna(piscina, intern, programma, "> q\n\n***\n",
        "<blockquote><p>q</p></blockquote><hr>");
    _pinna(piscina, intern, programma, "- a\n\n- b\n",
        "<ul><li><p>a</p></li><li><p>b</p></li></ul>");
    _pinna(piscina, intern, programma,
        "# H1\n###### H6\n**b** ~~d~~ [l](/u) ![i](/p)\n",
        "<h1>H1</h1><h6>H6</h6><p><strong>b</strong> <del>d</del>"
        " <a href=\"/u\">l</a> <img src=\"/p\" alt=\"i\"></p>");
    _ostende(piscina, intern, programma,
        "<div>\nraw\n</div>\n\nx <b>y</b>\n");

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        si (praeteritus)
        {
            redde ZEPHYRUM;
        }
    }
    redde I;
}
