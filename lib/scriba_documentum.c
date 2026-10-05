/* scriba_documentum.c - folium textus per historia; mutatio = lineae
 * substitutae */

#include "scriba_documentum.h"
#include "chorda_aedificator.h"
#include "stml.h"

#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

hic_manens constans character litterae_hex[] = "0123456789abcdef";

interior s32
attributum_s32 (
             StmlNodus* n,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = stml_attributum_capere(n, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

/* vacua = linea alba tabulae: cellulae omnes ' ', indentatio -1 (ut
 * tabula_initiare, _inserere_lineam, _delere_lineam scribunt) */
interior b32
linea_vacua (
    constans TabulaCharacterum* t,
                           i32  l)
{
    i32 c;

    si (t->indentatio[l] != -I)
    {
        redde FALSUM;
    }
    per (c = ZEPHYRUM; c < t->latitudo; c++)
    {
        si (tabula_cellula(t, l, c) != ' ')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* numerus linearum sine vacuis finalibus */
interior i32
longitudo_trunca (
    constans TabulaCharacterum* t)
{
    i32 n;

    n = t->altitudo;
    dum (n > ZEPHYRUM && linea_vacua(t, n - I))
    {
        n--;
    }
    redde n;
}

interior b32
lineae_aequales (
    constans TabulaCharacterum* a,
                           i32  la,
    constans TabulaCharacterum* b,
                           i32  lb)
{
    redde    a->indentatio[la] == b->indentatio[lb]
          && memcmp(&tabula_cellula(a, la, ZEPHYRUM),
                    &tabula_cellula(b, lb, ZEPHYRUM),
                    (size_t)a->latitudo) == ZEPHYRUM;
}

interior vacuum
lineam_vacare (
    TabulaCharacterum* t,
                  i32  l)
{
    memset(&tabula_cellula(t, l, ZEPHYRUM), ' ', (size_t)t->latitudo);
    t->indentatio[l] = -I;
}


/* ==================================================
 * Codex lineae
 * ================================================== */

interior vacuum
octetum_effugere (
    ChordaAedificator* a,
                   i8  o)
{
    commutatio (o)
    {
        casus '\\':
            chorda_aedificator_appendere_literis(a, "\\\\");
            frange;
        casus '\0':
            chorda_aedificator_appendere_literis(a, "\\0");
            frange;
        casus '\t':
            chorda_aedificator_appendere_literis(a, "\\t");
            frange;
        casus I:
            chorda_aedificator_appendere_literis(a, "\\1");
            frange;
        casus '"':
            chorda_aedificator_appendere_literis(a, "\\q");
            frange;
        ordinarius:
            si (o < 0x20 || o >= 0x7F)
            {
                chorda_aedificator_appendere_literis(a, "\\x");
                chorda_aedificator_appendere_character(a,
                    litterae_hex[o >> IV]);
                chorda_aedificator_appendere_character(a,
                    litterae_hex[o & 0xF]);
            }
            alioquin
            {
                chorda_aedificator_appendere_character(a,
                    (character)o);
            }
            frange;
    }
}

/* elementum linea: indentatio si posita, textus si non albus */
interior vacuum
lineam_scribere (
           ChordaAedificator* a,
    constans TabulaCharacterum* t,
                           i32  l)
{
    s32 finis;
    s32 c;

    chorda_aedificator_appendere_literis(a, "<linea");
    si (t->indentatio[l] != -I)
    {
        chorda_aedificator_appendere_literis(a, " indentatio=\"");
        chorda_aedificator_appendere_s32(a, t->indentatio[l]);
        chorda_aedificator_appendere_character(a, '"');
    }
    finis = (s32)t->latitudo - I;
    dum (finis >= ZEPHYRUM && tabula_cellula(t, l, (i32)finis) == ' ')
    {
        finis--;
    }
    si (finis >= ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " textus=\"");
        per (c = ZEPHYRUM; c <= finis; c++)
        {
            octetum_effugere(a, (i8)tabula_cellula(t, l, (i32)c));
        }
        chorda_aedificator_appendere_character(a, '"');
    }
    chorda_aedificator_appendere_literis(a, "/>");
}

interior s32
hex_valor (
    i8 o)
{
    si (o >= '0' && o <= '9')
    {
        redde (s32)(o - '0');
    }
    si (o >= 'a' && o <= 'f')
    {
        redde (s32)(o - 'a') + X;
    }
    redde -I;
}

/* textus effugitus -> cellulae[latitudo] (' ' impletae); FALSUM si
 * effugium malum aut textus latior folio */
interior b32
textum_solvere (
         chorda  textus,
      character* cellulae,
            i32  latitudo)
{
    i32 i;
    i32 c;
     i8 o;
    s32 h1;
    s32 h2;

    memset(cellulae, ' ', (size_t)latitudo);
    c = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        o = textus.datum[i];
        si (o == '\\')
        {
            si (i + I >= textus.mensura)
            {
                redde FALSUM;
            }
            i++;
            commutatio (textus.datum[i])
            {
                casus '\\': o = '\\'; frange;
                casus '0':  o = '\0'; frange;
                casus 't':  o = '\t'; frange;
                casus '1':  o = I;    frange;
                casus 'q':  o = '"';  frange;
                casus 'x':
                    si (i + II >= textus.mensura)
                    {
                        redde FALSUM;
                    }
                    h1 = hex_valor(textus.datum[i + I]);
                    h2 = hex_valor(textus.datum[i + II]);
                    si (h1 < ZEPHYRUM || h2 < ZEPHYRUM)
                    {
                        redde FALSUM;
                    }
                    o = (i8)(h1 * XVI + h2);
                    i += II;
                    frange;
                ordinarius:
                    redde FALSUM;
            }
        }
        si (c >= latitudo)
        {
            redde FALSUM;
        }
        cellulae[c] = (character)o;
        c++;
    }
    redde VERUM;
}


/* ==================================================
 * Mutatio
 * ================================================== */

chorda
scriba_mutatio_computare (
    constans TabulaCharacterum* ante,
    constans TabulaCharacterum* post,
                       Piscina* piscina)
{
    ChordaAedificator* a;
                  i32  n_ante;
                  i32  n_post;
                  i32  p;
                  i32  s;
                  i32  l;
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   !ante || !post || !piscina
        || ante->latitudo != post->latitudo
        || ante->altitudo != post->altitudo)
    {
        redde vacua;
    }
    n_ante  = longitudo_trunca(ante);
    n_post  = longitudo_trunca(post);
    p       = ZEPHYRUM;
    dum (p < n_ante && p < n_post && lineae_aequales(ante, p, post, p))
    {
        p++;
    }
    s = ZEPHYRUM;
    dum (   s < n_ante - p && s < n_post - p
         && lineae_aequales(ante, n_ante - I - s, post, n_post - I - s))
    {
        s++;
    }
    si (p + s == n_ante && p + s == n_post)
    {
        redde vacua;
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    chorda_aedificator_appendere_literis(a, "<mutatio linea=\"");
    chorda_aedificator_appendere_s32(a, (s32)p);
    chorda_aedificator_appendere_literis(a, "\" deletae=\"");
    chorda_aedificator_appendere_s32(a, (s32)(n_ante - p - s));
    chorda_aedificator_appendere_literis(a, "\">");
    per (l = p; l < n_post - s; l++)
    {
        lineam_scribere(a, post, l);
    }
    chorda_aedificator_appendere_literis(a, "</mutatio>");
    redde chorda_aedificator_finire(a);
}

b32
scriba_mutatio_applicare (
      TabulaCharacterum* tabula,
                 chorda  mutatio,
               Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlResultus  res;
       StmlNodus* radix;
       StmlNodus* liber;
          chorda* textus;
             s32  p;
             s32  d;
             i32  k;
             i32  n_ante;
             i32  n_post;
             i32  movendae;
             i32  i;
             i32  j;
       character* cellulae;
             s32* indentatio_nova;

    si (!tabula || !piscina || !intern || chorda_vacua(mutatio))
    {
        redde FALSUM;
    }
    res = stml_legere_ex_literis(chorda_ut_cstr(mutatio, piscina),
                                 piscina, intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde FALSUM;
    }
    radix = res.elementum_radix;
    si (!chorda_aequalis_literis(*radix->titulus, "mutatio"))
    {
        redde FALSUM;
    }
    p = attributum_s32(radix, "linea", -I);
    d = attributum_s32(radix, "deletae", -I);
    k = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        si (stml_liberum_ad_indicem(radix, i)->genus
            == STML_NODUS_ELEMENTUM)
        {
            k++;
        }
    }
    n_ante = longitudo_trunca(tabula);
    si (p < ZEPHYRUM || d < ZEPHYRUM || (i32)(p + d) > n_ante)
    {
        redde FALSUM;
    }
    n_post = n_ante - (i32)d + k;
    si (n_post > tabula->altitudo)
    {
        redde FALSUM;
    }
    /* lineae novae primum solvuntur: mutatio mala folium non tangit */
    cellulae = (character*)piscina_allocare(piscina,
        (memoriae_index)(k * tabula->latitudo + I));
    indentatio_nova = (s32*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)(k + I) * magnitudo(s32), magnitudo(s32));
    si (!cellulae || !indentatio_nova)
    {
        redde FALSUM;
    }
    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        liber = stml_liberum_ad_indicem(radix, i);
        si (liber->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (!chorda_aequalis_literis(*liber->titulus, "linea"))
        {
            redde FALSUM;
        }
        indentatio_nova[j]  = attributum_s32(liber, "indentatio", -I);
        textus              = stml_attributum_capere(liber, "textus");
        si (textus)
        {
            si (!textum_solvere(*textus,
                    cellulae + j * tabula->latitudo, tabula->latitudo))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            memset(cellulae + j * tabula->latitudo, ' ',
                   (size_t)tabula->latitudo);
        }
        j++;
    }
    /* cauda [p+d, n_ante) -> [p+k, ...) */
    movendae = n_ante - (i32)(p + d);
    memmove(&tabula_cellula(tabula, (i32)p + k, ZEPHYRUM),
            &tabula_cellula(tabula, (i32)(p + d), ZEPHYRUM),
            (size_t)(movendae * tabula->latitudo));
    memmove(&tabula->indentatio[(i32)p + k],
            &tabula->indentatio[(i32)(p + d)],
            (size_t)movendae * magnitudo(s32));
    per (j = ZEPHYRUM; j < k; j++)
    {
        memcpy(&tabula_cellula(tabula, (i32)p + j, ZEPHYRUM),
               cellulae + j * tabula->latitudo,
               (size_t)tabula->latitudo);
        tabula->indentatio[(i32)p + j] = indentatio_nova[j];
    }
    per (i = n_post; i < tabula->altitudo; i++)
    {
        lineam_vacare(tabula, i);
    }
    redde VERUM;
}


/* ==================================================
 * Proiectio pro historia
 * ================================================== */

interior vacuum
proiectio_vacare (
    vacuum* ctx)
{
    ScribaDocumentum* doc;
                 i32  l;

    doc = (ScribaDocumentum*)ctx;
    per (l = ZEPHYRUM; l < doc->altitudo; l++)
    {
        lineam_vacare(&doc->tabula, l);
    }
}

interior vacuum
proiectio_applicare (
    vacuum* ctx,
    chorda  actum)
{
    ScribaDocumentum* doc;

    doc = (ScribaDocumentum*)ctx;
    (vacuum)scriba_mutatio_applicare(&doc->tabula, actum, doc->piscina,
                                     doc->intern);
}

interior HistoriaProiectio
proiectio_facere (
    ScribaDocumentum* doc)
{
    HistoriaProiectio p;

    p.memoria    = doc->memoria;
    p.mensura    = doc->mensura;
    p.vacare     = proiectio_vacare;
    p.applicare  = proiectio_applicare;
    p.ctx        = doc;
    redde p;
}


/* ==================================================
 * Vita
 * ================================================== */

interior ScribaDocumentum*
documentum_struere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum)
{
    ScribaDocumentum* doc;
      memoriae_index  offset;

    si (   !piscina || !intern || !volumen || latitudo == ZEPHYRUM
        || altitudo == ZEPHYRUM)
    {
        redde NIHIL;
    }
    doc = (ScribaDocumentum*)piscina_allocare(piscina, magnitudo(*doc));
    si (!doc)
    {
        redde NIHIL;
    }
    memset(doc, ZEPHYRUM, magnitudo(ScribaDocumentum));
    doc->volumen      = volumen;
    doc->piscina      = piscina;
    doc->intern       = intern;
    doc->latitudo     = latitudo;
    doc->altitudo     = altitudo;
    doc->intervallum  = intervallum > ZEPHYRUM ? intervallum : LXIV;
    /* indentatio ad offset ordinatum post cellulas */
    offset = ((memoriae_index)latitudo * (memoriae_index)altitudo
              + (memoriae_index)III) & ~(memoriae_index)III;
    doc->mensura = offset + (memoriae_index)altitudo * magnitudo(s32);
    doc->memoria = (i8*)piscina_allocare_ordinatum(piscina,
        doc->mensura,
                                                   magnitudo(s32));
    si (!doc->memoria)
    {
        redde NIHIL;
    }
    doc->tabula.latitudo    = latitudo;
    doc->tabula.altitudo    = altitudo;
    doc->tabula.cellulae    = (character*)doc->memoria;
    doc->tabula.indentatio  = (s32*)(vacuum*)(doc->memoria + offset);
    memset(doc->memoria, ZEPHYRUM, doc->mensura);
    redde doc;
}

ScribaDocumentum*
scriba_documentum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum)
{
     ScribaDocumentum* doc;
    ChordaAedificator* a;

    doc = documentum_struere(piscina, intern, volumen, latitudo,
                             altitudo, intervallum);
    si (!doc)
    {
        redde NIHIL;
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    chorda_aedificator_appendere_literis(a, "<documentum latitudo=\"");
    chorda_aedificator_appendere_i32(a, latitudo);
    chorda_aedificator_appendere_literis(a, "\" altitudo=\"");
    chorda_aedificator_appendere_i32(a, altitudo);
    chorda_aedificator_appendere_literis(a, "\" intervallum=\"");
    chorda_aedificator_appendere_i32(a, doc->intervallum);
    chorda_aedificator_appendere_literis(a, "\"/>");
    volumen_plagulam_condere(volumen,
                             chorda_ex_literis("documentum", piscina),
                             chorda_aedificator_finire(a),
                             "scriba:documentum");
    doc->historia = historia_creare(piscina, intern, volumen, "mutatio",
                                    "scriba:checkpoint",
                                    doc->intervallum,
                                    proiectio_facere(doc));
    redde doc->historia ? doc : NIHIL;
}

ScribaDocumentum*
scriba_documentum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen)
{
    ScribaDocumentum* doc;
              chorda  manifestum;
                 b32  inventum;
        StmlResultus  res;
                 s32  latitudo;
                 s32  altitudo;
                 s32  intervallum;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    manifestum = volumen_plagulam_promere(volumen,
        chorda_ex_literis("documentum", piscina), piscina, &inventum);
    si (!inventum)
    {
        redde NIHIL;
    }
    res = stml_legere_ex_literis(chorda_ut_cstr(manifestum, piscina),
                                 piscina, intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde NIHIL;
    }
    latitudo     = attributum_s32(res.elementum_radix, "latitudo",
        ZEPHYRUM);
    altitudo     = attributum_s32(res.elementum_radix, "altitudo",
        ZEPHYRUM);
    intervallum  = attributum_s32(res.elementum_radix, "intervallum",
        LXIV);
    si (latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    doc = documentum_struere(piscina, intern, volumen, (i32)latitudo,
                             (i32)altitudo, (i32)intervallum);
    si (!doc)
    {
        redde NIHIL;
    }
    doc->historia = historia_aperire(piscina, intern, volumen,
        "mutatio",
                                     "scriba:checkpoint",
                                     doc->intervallum,
                                     proiectio_facere(doc));
    redde doc->historia ? doc : NIHIL;
}


/* ==================================================
 * Acta
 * ================================================== */

s64
scriba_documentum_committere (
               ScribaDocumentum* doc,
     constans TabulaCharacterum* post)
{
    chorda mutatio;

    si (!doc || !post)
    {
        redde ZEPHYRUM;
    }
    mutatio = scriba_mutatio_computare(&doc->tabula, post,
        doc->piscina);
    si (chorda_vacua(mutatio))
    {
        redde ZEPHYRUM;
    }
    redde historia_actum(doc->historia, mutatio);
}

b32
scriba_documentum_revocare (
    ScribaDocumentum* doc)
{
    redde doc ? historia_revocare(doc->historia) : FALSUM;
}

b32
scriba_documentum_reficere (
    ScribaDocumentum* doc)
{
    redde doc ? historia_reficere(doc->historia) : FALSUM;
}

b32
scriba_documentum_verificare (
    ScribaDocumentum* doc)
{
    redde doc ? historia_verificare(doc->historia) : FALSUM;
}


/* ==================================================
 * Lectio
 * ================================================== */

constans TabulaCharacterum*
scriba_documentum_tabula (
    constans ScribaDocumentum* doc)
{
    redde doc ? &doc->tabula : NIHIL;
}

chorda
scriba_documentum_sigillum_hex (
    constans ScribaDocumentum* doc,
                      Piscina* piscina)
{
    chorda vacua;

    si (!doc)
    {
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde vacua;
    }
    redde historia_sigillum_hex(doc->historia, piscina);
}

s64
scriba_documentum_cursor (
    constans ScribaDocumentum* doc)
{
    redde doc ? historia_cursor(doc->historia) : ZEPHYRUM;
}

s64
scriba_documentum_finis (
    constans ScribaDocumentum* doc)
{
    redde doc ? historia_finis(doc->historia) : ZEPHYRUM;
}

i32
scriba_documentum_numerus_vivorum (
    constans ScribaDocumentum* doc)
{
    redde doc ? historia_numerus_vivorum(doc->historia) : ZEPHYRUM;
}
