/* probatio_md_decoctum.c - Effugia et entia (CommonMark par. 2.4, 2.5)
 *
 * Exempla spec: effugium interpunctionis ASCII solius ('\a' manet),
 * entia nominata/decimalia/hexadecimalia, invalida litterae manent,
 * zephyrum et surrogata -> U+FFFD. 'mutatus' FALSUM cum nihil decoctum
 * (crudum sufficit - lexema derivatum non meretur).
 */

#include "latina.h"
#include "credo.h"
#include "md_decoctum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior b32
_aequalis (
                chorda  c,
    constans character* litterae)
{
    redde (b32)(c.mensura == (i32)strlen(litterae)
                && memcmp(c.datum, litterae, (size_t)c.mensura)
                    == ZEPHYRUM);
}

#define DEC(lit) md_decoquere(piscina, (lit), (i32)strlen(lit), &mutatus)

interior vacuum
_effugia_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        b32  mutatus;
     chorda  res;

    res = DEC("\\*not emphasized\\*");
    CREDO_VERUM (mutatus);
    CREDO_VERUM (_aequalis(res, "*not emphasized*"));
    res = DEC("\\a \\\\ \\");
    CREDO_VERUM (_aequalis(res, "\\a \\ \\"));   /* '\a' manet; '\\' -> '\'; '\' finalis manet */
    res = DEC("planus");
    CREDO_FALSUM (mutatus);
    CREDO_VERUM (_aequalis(res, "planus"));
    res = DEC("");
    CREDO_FALSUM (mutatus);
    CREDO_AEQUALIS_I32 (res.mensura, ZEPHYRUM);
}

interior vacuum
_entia_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        b32  mutatus;
     chorda  res;

    res = DEC("&amp; &lt; &gt; &quot;");
    CREDO_VERUM (mutatus);
    CREDO_VERUM (_aequalis(res, "& < > \""));
    res = DEC("&#35; &#1234; &#x22; &#XD06;");
    CREDO_VERUM (_aequalis(res, "# \xd3\x92 \" \xe0\xb4\x86"));
    res = DEC("&copy; &nbsp;");
    CREDO_VERUM (_aequalis(res, "\xc2\xa9 \xc2\xa0"));
    /* invalida: litterae manent */
    res =
        DEC("&nbsp &x; &#; &#x; &#87654321; &#abcdef0; &ThisIsNotDefined; &hi?;");
    CREDO_VERUM (_aequalis(res,
        "&nbsp &x; &#; &#x; &#87654321; &#abcdef0; &ThisIsNotDefined; &hi?;"));
    CREDO_FALSUM (mutatus);
    /* zephyrum -> U+FFFD */
    res = DEC("&#0;");
    CREDO_VERUM (_aequalis(res, "\xef\xbf\xbd"));
    /* ens effugitum non decoquitur */
    res = DEC("\\&amp;");
    CREDO_VERUM (_aequalis(res, "&amp;"));
}

interior vacuum
_url_codificare_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    {
        chorda u = md_url_codificare(piscina,
            chorda_ex_literis("/my uri\\[x]%20b\xc3\xa4\"", piscina));

        CREDO_CHORDA_AEQUALIS_LITERIS (u,
            "/my%20uri%5C%5Bx%5D%20b%C3%A4%22");
        u = md_url_codificare(piscina, chorda_ex_literis("/a?b=1&c=2#f",
            piscina));
        CREDO_CHORDA_AEQUALIS_LITERIS (u, "/a?b=1&c=2#f");
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "effugia", _effugia_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "entia", _entia_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "url codificare (B3.3)", _url_codificare_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("md_decoctum", SECTIONES);
}
