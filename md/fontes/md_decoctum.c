/* md_decoctum.c - Vide md_decoctum.h. */

#include "md_decoctum.h"
#include <string.h>

nomen structura {
    constans character* titulus;   /* NON 'nomen' - macro latina.h (typedef) */
                   i32  punctum;
} MdEns;

/* Ordine alphabetico non necessario - scansio linearis (CL) */
hic_manens constans MdEns ENTIA[] = {
    { "amp", 38 }, { "lt", 60 }, { "gt", 62 }, { "quot", 34 }, { "apos",
        39 },
    { "nbsp", 160 }, { "iexcl", 161 }, { "cent", 162 }, { "pound",
        163 },
    { "curren", 164 }, { "yen", 165 }, { "brvbar", 166 }, { "sect",
        167 },
    { "uml", 168 }, { "copy", 169 }, { "ordf", 170 }, { "laquo", 171 },
    { "not", 172 }, { "shy", 173 }, { "reg", 174 }, { "macr", 175 },
    { "deg", 176 }, { "plusmn", 177 }, { "sup2", 178 }, { "sup3", 179 },
    { "acute", 180 }, { "micro", 181 }, { "para", 182 }, { "middot",
        183 },
    { "cedil", 184 }, { "sup1", 185 }, { "ordm", 186 }, { "raquo",
        187 },
    { "frac14", 188 }, { "frac12", 189 }, { "frac34", 190 }, { "iquest",
        191 },
    { "Agrave", 192 }, { "Aacute", 193 }, { "Acirc", 194 }, { "Atilde",
        195 },
    { "Auml", 196 }, { "Aring", 197 }, { "AElig", 198 }, { "Ccedil",
        199 },
    { "Egrave", 200 }, { "Eacute", 201 }, { "Ecirc", 202 }, { "Euml",
        203 },
    { "Igrave", 204 }, { "Iacute", 205 }, { "Icirc", 206 }, { "Iuml",
        207 },
    { "ETH", 208 }, { "Ntilde", 209 }, { "Ograve", 210 }, { "Oacute",
        211 },
    { "Ocirc", 212 }, { "Otilde", 213 }, { "Ouml", 214 }, { "times",
        215 },
    { "Oslash", 216 }, { "Ugrave", 217 }, { "Uacute", 218 }, { "Ucirc",
        219 },
    { "Uuml", 220 }, { "Yacute", 221 }, { "THORN", 222 }, { "szlig",
        223 },
    { "agrave", 224 }, { "aacute", 225 }, { "acirc", 226 }, { "atilde",
        227 },
    { "auml", 228 }, { "aring", 229 }, { "aelig", 230 }, { "ccedil",
        231 },
    { "egrave", 232 }, { "eacute", 233 }, { "ecirc", 234 }, { "euml",
        235 },
    { "igrave", 236 }, { "iacute", 237 }, { "icirc", 238 }, { "iuml",
        239 },
    { "eth", 240 }, { "ntilde", 241 }, { "ograve", 242 }, { "oacute",
        243 },
    { "ocirc", 244 }, { "otilde", 245 }, { "ouml", 246 }, { "divide",
        247 },
    { "oslash", 248 }, { "ugrave", 249 }, { "uacute", 250 }, { "ucirc",
        251 },
    { "uuml", 252 }, { "yacute", 253 }, { "thorn", 254 }, { "yuml",
        255 },
    { "OElig", 338 }, { "oelig", 339 }, { "Scaron", 352 }, { "scaron",
        353 },
    { "Yuml", 376 }, { "fnof", 402 }, { "circ", 710 }, { "tilde", 732 },
    { "Alpha", 913 }, { "Beta", 914 }, { "Gamma", 915 }, { "Delta",
        916 },
    { "Theta", 920 }, { "Lambda", 923 }, { "Pi", 928 }, { "Sigma",
        931 },
    { "Phi", 934 }, { "Omega", 937 }, { "alpha", 945 }, { "beta", 946 },
    { "gamma", 947 }, { "delta", 948 }, { "epsilon", 949 }, { "zeta",
        950 },
    { "eta", 951 }, { "theta", 952 }, { "iota", 953 }, { "kappa", 954 },
    { "lambda", 955 }, { "mu", 956 }, { "nu", 957 }, { "xi", 958 },
    { "pi", 960 }, { "rho", 961 }, { "sigma", 963 }, { "tau", 964 },
    { "upsilon", 965 }, { "phi", 966 }, { "chi", 967 }, { "psi", 968 },
    { "omega", 969 }, { "ensp", 8194 }, { "emsp", 8195 }, { "thinsp",
        8201 },
    { "zwnj", 8204 }, { "zwj", 8205 }, { "lrm", 8206 }, { "rlm", 8207 },
    { "ndash", 8211 }, { "mdash", 8212 }, { "lsquo", 8216 }, { "rsquo",
        8217 },
    { "sbquo", 8218 }, { "ldquo", 8220 }, { "rdquo", 8221 }, { "bdquo",
        8222 },
    { "dagger", 8224 }, { "Dagger", 8225 }, { "bull", 8226 },
        { "hellip", 8230 },
    { "permil", 8240 }, { "prime", 8242 }, { "Prime", 8243 },
        { "lsaquo", 8249 },
    { "rsaquo", 8250 }, { "oline", 8254 }, { "frasl", 8260 }, { "euro",
        8364 },
    { "trade", 8482 }, { "larr", 8592 }, { "uarr", 8593 }, { "rarr",
        8594 },
    { "darr", 8595 }, { "harr", 8596 }, { "rArr", 8658 }, { "lArr",
        8656 },
    { "forall", 8704 }, { "part", 8706 }, { "exist", 8707 }, { "empty",
        8709 },
    { "nabla", 8711 }, { "isin", 8712 }, { "notin", 8713 }, { "ni",
        8715 },
    { "prod", 8719 }, { "sum", 8721 }, { "minus", 8722 }, { "lowast",
        8727 },
    { "radic", 8730 }, { "prop", 8733 }, { "infin", 8734 }, { "ang",
        8736 },
    { "and", 8743 }, { "or", 8744 }, { "cap", 8745 }, { "cup", 8746 },
    { "int", 8747 }, { "there4", 8756 }, { "sim", 8764 }, { "cong",
        8773 },
    { "asymp", 8776 }, { "ne", 8800 }, { "equiv", 8801 }, { "le",
        8804 },
    { "ge", 8805 }, { "sub", 8834 }, { "sup", 8835 }, { "nsub", 8836 },
    { "sube", 8838 }, { "supe", 8839 }, { "oplus", 8853 }, { "otimes",
        8855 },
    { "perp", 8869 }, { "sdot", 8901 }, { "loz", 9674 }, { "spades",
        9824 },
    { "clubs", 9827 }, { "hearts", 9829 }, { "diams", 9830 }, { "check",
        10003 }
};

interior b32
_interpunctio_ascii (
    character c)
{
    redde (b32)((c >= '!' && c <= '/') || (c >= ':' && c <= '@')
                || (c >= '[' && c <= '`') || (c >= '{' && c <= '~'));
}

interior i32
_utf8 (
          i32  punctum,
    character* exitus)
{
    si (punctum < 0x80)
    {
        exitus[0] = (character)punctum;
        redde I;
    }
    si (punctum < 0x800)
    {
        exitus[0] = (character)(0xC0 | (punctum >> 6));
        exitus[1] = (character)(0x80 | (punctum & 0x3F));
        redde II;
    }
    si (punctum < 0x10000)
    {
        exitus[0] = (character)(0xE0 | (punctum >> 12));
        exitus[1] = (character)(0x80 | ((punctum >> 6) & 0x3F));
        exitus[2] = (character)(0x80 | (punctum & 0x3F));
        redde III;
    }
    exitus[0] = (character)(0xF0 | (punctum >> 18));
    exitus[1] = (character)(0x80 | ((punctum >> 12) & 0x3F));
    exitus[2] = (character)(0x80 | ((punctum >> 6) & 0x3F));
    exitus[3] = (character)(0x80 | (punctum & 0x3F));
    redde IV;
}

b32
md_ens_decoquere (
    constans character* fons,
                   s32  ab,
                   s32  ad,
             character* exitus,
                   i32* longitudo,
                   s32* post)
{
    s32 i        = ab + I;
    i32 punctum  = ZEPHYRUM;

    si (ab >= ad || fons[ab] != '&')
    {
        redde FALSUM;
    }
    si (i < ad && fons[i] == '#')
    {
        i32 digiti = ZEPHYRUM;

        i = i + I;
        si (i < ad && (fons[i] == 'x' || fons[i] == 'X'))
        {
            i = i + I;
            dum (i < ad && digiti < VI)
            {
                character c = fons[i];
                      i32 v;

                si (c >= '0' && c <= '9') v = (i32)(c - '0');
                alioquin si (   c >= 'a'
                             && c <= 'f') v =
                                                                        (i32)(c
                                                                            - 'a')
                                                                            + X;
                alioquin si (   c >= 'A'
                             && c <= 'F') v =
                                                                        (i32)(c
                                                                            - 'A')
                                                                            + X;
                alioquin frange;
                punctum  = punctum * XVI + v;
                digiti   = digiti + I;
                i        = i + I;
            }
        }
        alioquin
        {
            dum (   i < ad && digiti < VII && fons[i] >= '0'
                 && fons[i] <= '9')
            {
                punctum  = punctum * X + (i32)(fons[i] - '0');
                digiti   = digiti + I;
                i        = i + I;
            }
        }
        si (digiti == ZEPHYRUM || i >= ad || fons[i] != ';')
        {
            redde FALSUM;
        }
        si (   punctum == ZEPHYRUM || punctum > 0x10FFFF
            || (punctum >= 0xD800 && punctum <= 0xDFFF))
        {
            punctum = 0xFFFD;
        }
    }
    alioquin
    {
        s32 n_ab = i;
        i32 k;

        dum (   i < ad && ((fons[i] >= 'a' && fons[i] <= 'z')
                        || (fons[i] >= 'A' && fons[i] <= 'Z')
                        || (fons[i] >= '0' && fons[i] <= '9')))
        {
            i = i + I;
        }
        si (i == n_ab || i >= ad || fons[i] != ';')
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < (i32)(magnitudo(ENTIA)
            / magnitudo(ENTIA[0])); k++)
        {
            i32 l = (i32)strlen(ENTIA[k].titulus);

            si (   (s32)l == i - n_ab
                && memcmp(fons + n_ab, ENTIA[k].titulus, (size_t)l)
                    == ZEPHYRUM)
            {
                punctum = ENTIA[k].punctum;
                frange;
            }
        }
        si (punctum == ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    *longitudo  = _utf8(punctum, exitus);
    *post       = i + I;
    redde VERUM;
}

chorda
md_decoquere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   b32* mutatus)
{
       chorda  c;
    character* d;
          s32  i = ZEPHYRUM;
          i32  n = ZEPHYRUM;

    /* decoctum numquam longius crudo (ens >= IV octeti -> <= IV) */
    d = (character*)piscina_allocare(piscina, (memoriae_index)mensura
        + IV);
    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    *mutatus   = FALSUM;
    si (d == NIHIL)
    {
        redde c;
    }
    dum (i < (s32)mensura)
    {
        character ch = fons[i];

        si (   ch == '\\' && i + I < (s32)mensura
            && _interpunctio_ascii(fons[i + I]))
        {
            d[n]      = fons[i + I];
            n         = n + I;
            i         = i + II;
            *mutatus  = VERUM;
            perge;
        }
        si (ch == '&')
        {
            character e[VIII];
                  i32 l;
                  s32 post;

            si (md_ens_decoquere(fons, i, (s32)mensura, e, &l, &post))
            {
                memcpy(d + n, e, (size_t)l);
                n         = n + l;
                i         = post;
                *mutatus  = VERUM;
                perge;
            }
        }
        d[n]  = ch;
        n     = n + I;
        i     = i + I;
    }
    {
        unio { character* c; i8* m; } u;

        u.c        = d;
        c.datum    = u.m;
        c.mensura  = n;
    }
    redde c;
}
