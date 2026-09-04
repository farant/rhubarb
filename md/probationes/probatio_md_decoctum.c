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

s32
principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;
        b32  mutatus;
     chorda  c;

    piscina = piscina_generare_dynamicum("probatio_md_decoctum", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans effugia ---\n");
    c = DEC("\\*not emphasized\\*");
    CREDO_VERUM (mutatus);
    CREDO_VERUM (_aequalis(c, "*not emphasized*"));
    c = DEC("\\a \\\\ \\");
    CREDO_VERUM (_aequalis(c, "\\a \\ \\"));   /* '\a' manet; '\\' -> '\'; '\' finalis manet */
    c = DEC("planus");
    CREDO_FALSUM (mutatus);
    CREDO_VERUM (_aequalis(c, "planus"));
    c = DEC("");
    CREDO_FALSUM (mutatus);
    CREDO_AEQUALIS_I32 (c.mensura, ZEPHYRUM);

    imprimere("\n--- Probans entia ---\n");
    c = DEC("&amp; &lt; &gt; &quot;");
    CREDO_VERUM (mutatus);
    CREDO_VERUM (_aequalis(c, "& < > \""));
    c = DEC("&#35; &#1234; &#x22; &#XD06;");
    CREDO_VERUM (_aequalis(c, "# \xd3\x92 \" \xe0\xb4\x86"));
    c = DEC("&copy; &nbsp;");
    CREDO_VERUM (_aequalis(c, "\xc2\xa9 \xc2\xa0"));
    /* invalida: litterae manent */
    c =
        DEC("&nbsp &x; &#; &#x; &#87654321; &#abcdef0; &ThisIsNotDefined; &hi?;");
    CREDO_VERUM (_aequalis(c,
        "&nbsp &x; &#; &#x; &#87654321; &#abcdef0; &ThisIsNotDefined; &hi?;"));
    CREDO_FALSUM (mutatus);
    /* zephyrum -> U+FFFD */
    c = DEC("&#0;");
    CREDO_VERUM (_aequalis(c, "\xef\xbf\xbd"));
    /* ens effugitum non decoquitur */
    c = DEC("\\&amp;");
    CREDO_VERUM (_aequalis(c, "&amp;"));

    imprimere("\n");
    imprimere("\n--- url codificare (B3.3) ---\n");
    {
        chorda u = md_url_codificare(piscina,
            chorda_ex_literis("/my uri\\[x]%20b\xc3\xa4\"", piscina));

        CREDO_CHORDA_AEQUALIS_LITERIS (u,
            "/my%20uri%5C%5Bx%5D%20b%C3%A4%22");
        u = md_url_codificare(piscina, chorda_ex_literis("/a?b=1&c=2#f",
            piscina));
        CREDO_CHORDA_AEQUALIS_LITERIS (u, "/a?b=1&c=2#f");
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
