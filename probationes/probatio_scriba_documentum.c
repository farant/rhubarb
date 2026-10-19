/* probatio_scriba_documentum.c - mutatio (codex, praefixum/suffixum,
 * applicatio exacta) et documentum per historia (scriba-plan S0)
 *
 * Folium parvum VIII x VI: vectores legibiles. Omnis mutatio bis
 * probatur: applicata folium 'post' reddit ET mutatio iterum
 * computata vacua est. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "volumen.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LAT VIII
#define ALT VI

/* '\0' medium, tab cum continuatione, effugia, octetus >= 0x80 */
hic_manens constans character octeti_difficiles[VIII] = {
    'a', '\0', 'b', '\t', TAB_CONTINUATIO, '\\', '"', (character)0xC3
};

interior vacuum
folium (
     TabulaCharacterum* t,
    constans character* literae,
                Piscina* piscina)
{
    /* status initialis (' ', indentatio -1) + characteres solos -
     * tabula_ex_literis indentationem 0 ponit */
    i32 l;
    i32 c;

    tabula_initiare(t, piscina, LAT, ALT);
    l = ZEPHYRUM;
    c = ZEPHYRUM;
    dum (*literae)
    {
        si (*literae == '\n')
        {
            l++;
            c = ZEPHYRUM;
        }
        alioquin
        {
            tabula_cellula(t, l, c) = *literae;
            c++;
        }
        literae++;
    }
}

interior vacuum
copiare (
             TabulaCharacterum* ad,
    constans TabulaCharacterum* ex,
                       Piscina* piscina)
{
    tabula_initiare(ad, piscina, ex->latitudo, ex->altitudo);
    memcpy(ad->cellulae, ex->cellulae,
           (size_t)(ex->latitudo * ex->altitudo));
    memcpy(ad->indentatio, ex->indentatio,
           (size_t)ex->altitudo * magnitudo(s32));
}

interior b32
aequalia (
    constans TabulaCharacterum* a,
    constans TabulaCharacterum* b)
{
    redde    memcmp(a->cellulae, b->cellulae,
                    (size_t)(a->latitudo * a->altitudo)) == ZEPHYRUM
          && memcmp(a->indentatio, b->indentatio,
                    (size_t)a->altitudo * magnitudo(s32)) == ZEPHYRUM;
}

/* mutatio ex ante in post: textus expectatus (NIHIL = non pinnatus),
 * applicata reddit post, iterum computata vacua */
interior vacuum
probare (
    constans character* titulus,
     TabulaCharacterum* ante,
     TabulaCharacterum* post,
    constans character* expectata,
                Piscina* piscina,
    InternamentumChorda* intern)
{
               chorda m;
    TabulaCharacterum c;

    m = scriba_mutatio_computare(ante, post, piscina);
    imprimere("  %s: %.*s\n", titulus, (integer)m.mensura,
              m.datum ? (constans character*)m.datum : "");
    CREDO_FALSUM(chorda_vacua(m));
    si (expectata)
    {
        CREDO_VERUM(chorda_aequalis_literis(m, expectata));
    }
    copiare(&c, ante, piscina);
    CREDO_VERUM(scriba_mutatio_applicare(&c, m, piscina, intern));
    CREDO_VERUM(aequalia(&c, post));
    CREDO_VERUM(chorda_vacua(scriba_mutatio_computare(&c, post,
        piscina)));
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
      TabulaCharacterum  ante;
      TabulaCharacterum  post;
      TabulaCharacterum  alia;

    piscina  = c->piscina;

    folium(&ante, "ab\ncd", piscina);
    folium(&post, "ab\ncd", piscina);
    CREDO_VERUM(chorda_vacua(scriba_mutatio_computare(&ante, &post,
        piscina)));
    tabula_initiare(&alia, piscina, IX, ALT);
    CREDO_VERUM(chorda_vacua(scriba_mutatio_computare(&ante, &alia,
        piscina)));
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
      TabulaCharacterum  ante;
      TabulaCharacterum  post;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* ante ut in sectione I */
    folium(&ante, "ab\ncd", piscina);
    folium(&post, "ab\nxd", piscina);
    probare("unus", &ante, &post,
            "<mutatio linea=\"1\" deletae=\"1\"><linea textus=\"xd\"/>"
            "</mutatio>", piscina, intern);
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
      TabulaCharacterum  ante;
      TabulaCharacterum  post;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    folium(&ante, "ab\ncd\nef", piscina);
    copiare(&post, &ante, piscina);
    CREDO_VERUM(tabula_inserere_lineam(&post, I));
    probare("inserta", &ante, &post,
            "<mutatio linea=\"1\" deletae=\"0\"><linea/></mutatio>",
            piscina, intern);
    copiare(&post, &ante, piscina);
    tabula_delere_lineam(&post, ZEPHYRUM);
    probare("deleta", &ante, &post,
            "<mutatio linea=\"0\" deletae=\"1\"></mutatio>", piscina,
            intern);
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
      TabulaCharacterum  ante;
      TabulaCharacterum  post;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    folium(&ante, "a\n\n\n\n\nzz", piscina);
    folium(&post, "a\n\n\n\n\nzy", piscina);
    probare("ultima", &ante, &post,
            "<mutatio linea=\"5\" deletae=\"1\"><linea textus=\"zy\"/>"
            "</mutatio>", piscina, intern);
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
      TabulaCharacterum  ante;
      TabulaCharacterum  post;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* linea ultima cum '\0' solis: tabula eam 'vacuam' putat et
     * inserere permittit - linea ex folio cadit */
    folium(&ante, "a", piscina);
    tabula_cellula(&ante, V, ZEPHYRUM) = '\0';
    copiare(&post, &ante, piscina);
    CREDO_VERUM(tabula_inserere_lineam(&post, ZEPHYRUM));
    probare("trusa", &ante, &post, NIHIL, piscina, intern);
    folium(&ante, "a", piscina);
    tabula_cellula(&ante, I, I) = '\0';
    folium(&post, "a", piscina);
    probare("nullum deletum", &ante, &post,
            "<mutatio linea=\"1\" deletae=\"1\"></mutatio>", piscina,
            intern);
}

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
      TabulaCharacterum  ante;
      TabulaCharacterum  post;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    folium(&ante, "ab", piscina);
    copiare(&post, &ante, piscina);
    post.indentatio[ZEPHYRUM] = IV;
    probare("indentatio", &ante, &post,
            "<mutatio linea=\"0\" deletae=\"1\">"
            "<linea indentatio=\"4\" textus=\"ab\"/></mutatio>",
            piscina, intern);
}

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
      TabulaCharacterum  ante;
      TabulaCharacterum  post;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    folium(&ante, "", piscina);
    copiare(&post, &ante, piscina);
    memcpy(&tabula_cellula(&post, ZEPHYRUM, ZEPHYRUM),
        octeti_difficiles,
           VIII);
    memcpy(&tabula_cellula(&post, I, ZEPHYRUM), "<&> ", IV);
    probare("codex", &ante, &post,
            "<mutatio linea=\"0\" deletae=\"0\">"
            "<linea textus=\"a\\0b\\t\\1\\\\\\q\\xc3\"/>"
            "<linea textus=\"<&>\"/></mutatio>", piscina, intern);
}

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        /* 'c' hic tabula est (contextum obumbrat) */
        TabulaCharacterum ante;
        TabulaCharacterum c;

        folium(&ante, "ab\ncd", piscina);
        copiare(&c, &ante, piscina);
        CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
            "<mutatio linea=\"1\" deletae=\"5\"></mutatio>", piscina),
            piscina, intern));
        CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
            "<mutatio linea=\"0\" deletae=\"1\"><linea textus=\"cd\"/>"
            "<linea textus=\"abcdefghi\"/></mutatio>", piscina),
            piscina,
            intern));
        CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
            "<mutatio linea=\"0\" deletae=\"1\"><linea textus=\"\\z\"/>"
            "</mutatio>", piscina), piscina, intern));
        CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
            "<mutatio linea=\"0\" deletae=\"0\"><linea/><linea/>"
            "<linea/><linea/><linea/></mutatio>", piscina), piscina,
            intern));
        CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
            "<ictus/>", piscina), piscina, intern));
        CREDO_VERUM(aequalia(&c, &ante));
    }
}

/* IX et X una sectio: X documentum ab IX commissum aperit (vol, doc,
 * cursor q, folium c) - gradus X intra */
interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        /* 'c' hic tabula est (contextum obumbrat) */
        TabulaCharacterum  ante;
        TabulaCharacterum  c;
        TabulaCharacterum  momenta[III];
                  Volumen* vol;
         ScribaDocumentum* doc;
         ScribaDocumentum* doc2;
                      s64  q;
                      i32  i;
                   chorda  m;

        vol = volumen_temporarium(piscina,
            "probatio_scriba_documentum");
        doc = scriba_documentum_creare(piscina, intern, vol, "", LAT,
            ALT,
            II);
        CREDO_NON_NIHIL(doc);
        copiare(&c, scriba_documentum_tabula(doc), piscina);
        CREDO_AEQUALIS_S64(scriba_documentum_committere(doc, &c),
            ZEPHYRUM);
        tabula_inserere_characterem(&c, ZEPHYRUM, ZEPHYRUM, 'h');
        CREDO_VERUM(scriba_documentum_committere(doc, &c) > ZEPHYRUM);
        copiare(&momenta[ZEPHYRUM], &c, piscina);
        tabula_inserere_characterem(&c, ZEPHYRUM, I, 'i');
        CREDO_VERUM(tabula_inserere_lineam(&c, ZEPHYRUM));
        CREDO_VERUM(scriba_documentum_committere(doc, &c) > ZEPHYRUM);
        copiare(&momenta[I], &c, piscina);
        c.indentatio[I] = II;
        tabula_inserere_characterem(&c, II, ZEPHYRUM, '!');
        CREDO_VERUM(scriba_documentum_committere(doc, &c) > ZEPHYRUM);
        copiare(&momenta[II], &c, piscina);
        CREDO_VERUM(aequalia(scriba_documentum_tabula(doc), &c));
        CREDO_AEQUALIS_I32(scriba_documentum_numerus_vivorum(doc), III);
        CREDO_VERUM(scriba_documentum_revocare(doc));
        CREDO_VERUM(aequalia(scriba_documentum_tabula(doc),
            &momenta[I]));
        CREDO_VERUM(scriba_documentum_revocare(doc));
        CREDO_VERUM(aequalia(scriba_documentum_tabula(doc),
            &momenta[ZEPHYRUM]));
        CREDO_VERUM(scriba_documentum_reficere(doc));
        CREDO_VERUM(aequalia(scriba_documentum_tabula(doc),
            &momenta[I]));
        CREDO_VERUM(scriba_documentum_verificare(doc));
        /* ramus: mutatio nova post revocationem */
        copiare(&c, scriba_documentum_tabula(doc), piscina);
        tabula_inserere_characterem(&c, III, ZEPHYRUM, 'r');
        q = scriba_documentum_committere(doc, &c);
        CREDO_VERUM(q > ZEPHYRUM);
        CREDO_FALSUM(scriba_documentum_reficere(doc));
        CREDO_VERUM(scriba_documentum_verificare(doc));

        CREDO_GRADUS("X: aperire - folium et sigillum eadem");
        doc2 = scriba_documentum_aperire(piscina, intern, vol, "");
        CREDO_NON_NIHIL(doc2);
        CREDO_AEQUALIS_S64(scriba_documentum_cursor(doc2), q);
        CREDO_VERUM(aequalia(scriba_documentum_tabula(doc2), &c));
        CREDO_VERUM(chorda_aequalis(scriba_documentum_sigillum_hex(doc,
            piscina), scriba_documentum_sigillum_hex(doc2, piscina)));
        per (i = ZEPHYRUM; i < III; i++)
        {
            CREDO_VERUM(scriba_documentum_revocare(doc2));
        }
        CREDO_FALSUM(scriba_documentum_revocare(doc2));
        m = scriba_mutatio_computare(scriba_documentum_tabula(doc2),
            scriba_documentum_tabula(doc2), piscina);
        CREDO_VERUM(chorda_vacua(m));
        folium(&ante, "", piscina);
        CREDO_VERUM(aequalia(scriba_documentum_tabula(doc2), &ante));
        volumen_claudere(vol);
    }
}

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
      TabulaCharacterum  ante;
      TabulaCharacterum  post;
                Volumen* vol_duplex;
       ScribaDocumentum* d1;
       ScribaDocumentum* d2;
       ScribaDocumentum* r1;
       ScribaDocumentum* r2;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    vol_duplex = volumen_temporarium(piscina,
        "probatio_scriba_duplex");
    d1 = scriba_documentum_creare(piscina, intern, vol_duplex, "s1",
                                  LAT, ALT, II);
    d2 = scriba_documentum_creare(piscina, intern, vol_duplex, "s2",
                                  LAT, ALT, II);
    CREDO_NON_NIHIL(d1);
    CREDO_NON_NIHIL(d2);
    folium(&ante, "unum", piscina);
    CREDO_VERUM(scriba_documentum_committere(d1, &ante) > ZEPHYRUM);
    folium(&post, "duo", piscina);
    CREDO_VERUM(scriba_documentum_committere(d2, &post) > ZEPHYRUM);
    CREDO_VERUM(scriba_documentum_revocare(d1));
    CREDO_VERUM(aequalia(scriba_documentum_tabula(d2), &post));
    CREDO_VERUM(scriba_documentum_reficere(d1));
    r1 = scriba_documentum_aperire(piscina, intern, vol_duplex,
        "s1");
    r2 = scriba_documentum_aperire(piscina, intern, vol_duplex,
        "s2");
    CREDO_NON_NIHIL(r1);
    CREDO_NON_NIHIL(r2);
    CREDO_VERUM(aequalia(scriba_documentum_tabula(r1), &ante));
    CREDO_VERUM(aequalia(scriba_documentum_tabula(r2), &post));
    /* spatium ignotum: nullum documentum */
    CREDO_NIHIL(scriba_documentum_aperire(piscina, intern,
        vol_duplex,
        "s3"));
    volumen_claudere(vol_duplex);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I: aequalia, dimensiones aliae -> vacua", _sectio_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "II: character unus", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "III: linea inserta et deleta (tabula ipsa)", _sectio_tertia,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "IV: linea ultima folii", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "V: '\\0' non album; linea ex fundo trusa", _sectio_quinta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "VI: indentatio sola", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "VII: codex - octeti difficiles", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "VIII: mutationes malae recusantur", _sectio_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "IX: committere, revocare, ramus", _sectio_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "XI: duo documenta in volumine uno (spatia)", _sectio_decima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("scriba_documentum", SECTIONES);
}
