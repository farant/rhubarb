/* crustula.c - crustula HTTP (RFC 6265) */
#include "crustula.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = 0;
    redde v;
}

interior b32
_spatium (
    i8 c)
{
    redde c == ' ' || c == '\t';
}

chorda
crustulum_valor (
                 chorda  caput_crustulorum,
     constans character* titulus)
{
    i32 n;
    i32 i = 0;

    si (!titulus || !caput_crustulorum.datum)
    {
        redde _vacua();
    }
    n = (i32)strlen(titulus);
    dum (i < caput_crustulorum.mensura)
    {
        i32 initium;
        i32 aequale  = 0;
        b32 habet    = FALSUM;
        i32 finis;
        i32 a;
        i32 b;

        dum (   i < caput_crustulorum.mensura
             && (caput_crustulorum.datum[i] == ';'
                 || _spatium(caput_crustulorum.datum[i])))
        {
            i++;
        }
        initium = i;
        dum (   i < caput_crustulorum.mensura
             && caput_crustulorum.datum[i] != ';')
        {
            si (!habet && caput_crustulorum.datum[i] == '=')
            {
                aequale  = i;
                habet    = VERUM;
            }
            i++;
        }
        finis = i;
        si (!habet || aequale == initium)
        {
            perge;   /* sine '=' aut sine titulo */
        }
        /* titulus: [initium, aequale) sine spatiis finalibus */
        b = aequale;
        dum (b > initium && _spatium(caput_crustulorum.datum[b - I]))
        {
            b--;
        }
        si (   b - initium != n
            || memcmp(caput_crustulorum.datum + initium, titulus,
            (size_t)n)
               != 0)
        {
            perge;
        }
        /* valor: (aequale, finis), sine spatiis et virgulis */
        a = aequale + I;
        dum (a < finis && _spatium(caput_crustulorum.datum[a]))
        {
            a++;
        }
        dum (finis > a && _spatium(caput_crustulorum.datum[finis - I]))
        {
            finis--;
        }
        si (   finis - a >= II && caput_crustulorum.datum[a] == '"'
            && caput_crustulorum.datum[finis - I] == '"')
        {
            a++;
            finis--;
        }
        {
            chorda v;

            v.datum    = caput_crustulorum.datum + a;
            v.mensura  = finis - a;
            redde v;
        }
    }
    redde _vacua();
}

/* token RFC 2616: sine CTL, spatio, separatoribus */
interior b32
_titulus_validus (
    constans character* t)
{
    constans character* s;

    si (!t || t[0] == '\0')
    {
        redde FALSUM;
    }
    per (s = t; *s; s++)
    {
        insignatus character c = (insignatus character)*s;

        si (c <= 0x20 || c >= 0x7F || strchr("()<>@,;:\\\"/[]?={}", c))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* cookie-octet: %x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E */
interior b32
_valor_validus (
    chorda v)
{
    i32 i;

    si (v.mensura > 0 && !v.datum)
    {
        redde FALSUM;
    }
    per (i = 0; i < v.mensura; i++)
    {
        i8 c = v.datum[i];

        si (   c < 0x21 || c > 0x7E || c == '"' || c == ','
            || c == ';' || c == '\\')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Path / Domain: sine CTL et sine ';' */
interior b32
_attributum_validum (
    constans character* a)
{
    constans character* s;

    per (s = a; *s; s++)
    {
        insignatus character c = (insignatus character)*s;

        si (c < 0x20 || c == 0x7F || c == ';')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

chorda
crustulum_scribere (
            constans character* titulus,
                        chorda  valor,
    constans CrustulumOptiones* optiones,
                       Piscina* piscina)
{
         CrustulumOptiones  o;
         ChordaAedificator* aed;
        constans character* via;

    si (   !piscina || !_titulus_validus(titulus)
        || !_valor_validus(valor))
    {
        redde _vacua();
    }
    si (optiones)
    {
        o = *optiones;
    }
    alioquin
    {
        memset(&o, 0, magnitudo(o));
        o.max_aetas_s = -I;
    }
    via = o.via ? o.via : "/";
    si (   !_attributum_validum(via)
        || (o.dominium && !_attributum_validum(o.dominium)))
    {
        redde _vacua();
    }
    si (o.eadem == CRUSTULUM_EADEM_NULLA && !o.securum)
    {
        redde _vacua();
    }
    si (strncmp(titulus, "__Secure-", IX) == 0 && !o.securum)
    {
        redde _vacua();
    }
    si (   strncmp(titulus, "__Host-", VII) == 0
        && (!o.securum || o.dominium || strcmp(via, "/") != 0))
    {
        redde _vacua();
    }

    aed = chorda_aedificator_creare(piscina, CXXVIII);
    chorda_aedificator_appendere_literis(aed, titulus);
    chorda_aedificator_appendere_literis(aed, "=");
    chorda_aedificator_appendere_chorda(aed, valor);
    si (o.max_aetas_s >= 0)
    {
        character numerus[XXXII];

        sprintf(numerus, "; Max-Age=%ld", (longus)o.max_aetas_s);
        chorda_aedificator_appendere_literis(aed, numerus);
    }
    chorda_aedificator_appendere_literis(aed, "; Path=");
    chorda_aedificator_appendere_literis(aed, via);
    si (o.dominium)
    {
        chorda_aedificator_appendere_literis(aed, "; Domain=");
        chorda_aedificator_appendere_literis(aed, o.dominium);
    }
    si (o.securum)
    {
        chorda_aedificator_appendere_literis(aed, "; Secure");
    }
    si (o.http_solum)
    {
        chorda_aedificator_appendere_literis(aed, "; HttpOnly");
    }
    chorda_aedificator_appendere_literis(aed,
        o.eadem == CRUSTULUM_EADEM_STRICTA ? "; SameSite=Strict"
        : o.eadem == CRUSTULUM_EADEM_NULLA ? "; SameSite=None"
        : "; SameSite=Lax");
    redde chorda_aedificator_finire(aed);
}
