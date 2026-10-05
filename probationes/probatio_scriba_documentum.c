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

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior vacuum
folium (
    TabulaCharacterum* t,
    constans character* literae)
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
    constans TabulaCharacterum* ex)
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
    constans character* expectata)
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
    copiare(&c, ante);
    CREDO_VERUM(scriba_mutatio_applicare(&c, m, piscina, intern));
    CREDO_VERUM(aequalia(&c, post));
    CREDO_VERUM(chorda_vacua(scriba_mutatio_computare(&c, post,
        piscina)));
}

s32 principale (vacuum)
{
       TabulaCharacterum  ante;
       TabulaCharacterum  post;
       TabulaCharacterum  c;
       TabulaCharacterum  alia;
       TabulaCharacterum  momenta[III];
                 Volumen* vol;
        ScribaDocumentum* doc;
        ScribaDocumentum* doc2;
                     s64  q;
                     i32  i;
                  chorda  m;

    piscina = piscina_generare_dynamicum("probatio_scriba_documentum",
        XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- I: aequalia, dimensiones aliae -> vacua ---\n");
    folium(&ante, "ab\ncd");
    folium(&post, "ab\ncd");
    CREDO_VERUM(chorda_vacua(scriba_mutatio_computare(&ante, &post,
        piscina)));
    tabula_initiare(&alia, piscina, IX, ALT);
    CREDO_VERUM(chorda_vacua(scriba_mutatio_computare(&ante, &alia,
        piscina)));

    imprimere("\n--- II: character unus ---\n");
    folium(&post, "ab\nxd");
    probare("unus", &ante, &post,
            "<mutatio linea=\"1\" deletae=\"1\"><linea textus=\"xd\"/>"
            "</mutatio>");

    imprimere("\n--- III: linea inserta et deleta (tabula ipsa) ---\n");
    folium(&ante, "ab\ncd\nef");
    copiare(&post, &ante);
    CREDO_VERUM(tabula_inserere_lineam(&post, I));
    probare("inserta", &ante, &post,
            "<mutatio linea=\"1\" deletae=\"0\"><linea/></mutatio>");
    copiare(&post, &ante);
    tabula_delere_lineam(&post, ZEPHYRUM);
    probare("deleta", &ante, &post,
            "<mutatio linea=\"0\" deletae=\"1\"></mutatio>");

    imprimere("\n--- IV: linea ultima folii ---\n");
    folium(&ante, "a\n\n\n\n\nzz");
    folium(&post, "a\n\n\n\n\nzy");
    probare("ultima", &ante, &post,
            "<mutatio linea=\"5\" deletae=\"1\"><linea textus=\"zy\"/>"
            "</mutatio>");

    imprimere("\n--- V: '\\0' non album; linea ex fundo trusa ---\n");
    /* linea ultima cum '\0' solis: tabula eam 'vacuam' putat et
     * inserere permittit - linea ex folio cadit */
    folium(&ante, "a");
    tabula_cellula(&ante, V, ZEPHYRUM) = '\0';
    copiare(&post, &ante);
    CREDO_VERUM(tabula_inserere_lineam(&post, ZEPHYRUM));
    probare("trusa", &ante, &post, NIHIL);
    folium(&ante, "a");
    tabula_cellula(&ante, I, I) = '\0';
    folium(&post, "a");
    probare("nullum deletum", &ante, &post,
            "<mutatio linea=\"1\" deletae=\"1\"></mutatio>");

    imprimere("\n--- VI: indentatio sola ---\n");
    folium(&ante, "ab");
    copiare(&post, &ante);
    post.indentatio[ZEPHYRUM] = IV;
    probare("indentatio", &ante, &post,
            "<mutatio linea=\"0\" deletae=\"1\">"
            "<linea indentatio=\"4\" textus=\"ab\"/></mutatio>");

    imprimere("\n--- VII: codex - octeti difficiles ---\n");
    folium(&ante, "");
    copiare(&post, &ante);
    memcpy(&tabula_cellula(&post, ZEPHYRUM, ZEPHYRUM),
        octeti_difficiles,
           VIII);
    memcpy(&tabula_cellula(&post, I, ZEPHYRUM), "<&> ", IV);
    probare("codex", &ante, &post,
            "<mutatio linea=\"0\" deletae=\"0\">"
            "<linea textus=\"a\\0b\\t\\1\\\\\\q\\xc3\"/>"
            "<linea textus=\"<&>\"/></mutatio>");

    imprimere("\n--- VIII: mutationes malae recusantur ---\n");
    folium(&ante, "ab\ncd");
    copiare(&c, &ante);
    CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
        "<mutatio linea=\"1\" deletae=\"5\"></mutatio>", piscina),
        piscina, intern));
    CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
        "<mutatio linea=\"0\" deletae=\"1\"><linea textus=\"cd\"/>"
        "<linea textus=\"abcdefghi\"/></mutatio>", piscina), piscina,
        intern));
    CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
        "<mutatio linea=\"0\" deletae=\"1\"><linea textus=\"\\z\"/>"
        "</mutatio>", piscina), piscina, intern));
    CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
        "<mutatio linea=\"0\" deletae=\"0\"><linea/><linea/><linea/>"
        "<linea/><linea/></mutatio>", piscina), piscina, intern));
    CREDO_FALSUM(scriba_mutatio_applicare(&c, chorda_ex_literis(
        "<ictus/>", piscina), piscina, intern));
    CREDO_VERUM(aequalia(&c, &ante));

    imprimere("\n--- IX: committere, revocare, ramus ---\n");
    vol = volumen_temporarium(piscina, "probatio_scriba_documentum");
    doc = scriba_documentum_creare(piscina, intern, vol, LAT, ALT, II);
    CREDO_NON_NIHIL(doc);
    copiare(&c, scriba_documentum_tabula(doc));
    CREDO_AEQUALIS_S64(scriba_documentum_committere(doc, &c), ZEPHYRUM);
    tabula_inserere_characterem(&c, ZEPHYRUM, ZEPHYRUM, 'h');
    CREDO_VERUM(scriba_documentum_committere(doc, &c) > ZEPHYRUM);
    copiare(&momenta[ZEPHYRUM], &c);
    tabula_inserere_characterem(&c, ZEPHYRUM, I, 'i');
    CREDO_VERUM(tabula_inserere_lineam(&c, ZEPHYRUM));
    CREDO_VERUM(scriba_documentum_committere(doc, &c) > ZEPHYRUM);
    copiare(&momenta[I], &c);
    c.indentatio[I] = II;
    tabula_inserere_characterem(&c, II, ZEPHYRUM, '!');
    CREDO_VERUM(scriba_documentum_committere(doc, &c) > ZEPHYRUM);
    copiare(&momenta[II], &c);
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
    copiare(&c, scriba_documentum_tabula(doc));
    tabula_inserere_characterem(&c, III, ZEPHYRUM, 'r');
    q = scriba_documentum_committere(doc, &c);
    CREDO_VERUM(q > ZEPHYRUM);
    CREDO_FALSUM(scriba_documentum_reficere(doc));
    CREDO_VERUM(scriba_documentum_verificare(doc));

    imprimere("\n--- X: aperire - folium et sigillum eadem ---\n");
    doc2 = scriba_documentum_aperire(piscina, intern, vol);
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
    folium(&ante, "");
    CREDO_VERUM(aequalia(scriba_documentum_tabula(doc2), &ante));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
