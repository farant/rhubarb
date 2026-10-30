/* probatio_musicxml.c - lector MusicXML (include/musicxml.h)
 *
 * Fixa in probationes/fixa/musicxml/ (scripta manu, una res quaeque;
 * linea II dicit quid figat). Plan: project-specs/musicxml-plan.md
 * P2 (nucleus): partwise, voces, acorda, divisio operis (M4), linea
 * temporis communis (M15), attributa, vitia, tabula canonica.
 */

#include "latina.h"
#include "credo.h"
#include "musicxml.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior MusicxmlOpus*
_legere_fixum (
         CredoContextus* c,
     constans character* titulus)
{
    character via[CCLVI];

    strcpy(via, "probationes/fixa/musicxml/");
    strcat(via, titulus);
    redde musicxml_legere(filum_legere_totum(via, c->piscina),
        c->piscina);
}

interior MusicxmlOpus*
_legere_textum (
         CredoContextus* c,
     constans character* textus)
{
    redde musicxml_legere(chorda_ex_literis(textus, c->piscina),
        c->piscina);
}

interior b32
_aequat (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde c.mensura == n && (n == ZEPHYRUM
        || memcmp(c.datum, literae, (size_t)n) == ZEPHYRUM);
}

interior b32
_continet (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);
    i32 i;

    per (i = ZEPHYRUM; i + n <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, literae, (size_t)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* vitium primum generis dati; NIHIL si nullum */
interior constans MusicxmlVitium*
_vitium (
    constans MusicxmlOpus* o,
      MusicxmlVitiumGenus  genus)
{
    i32 i;

    per (i = ZEPHYRUM; i < o->numerus_vitiorum; i++)
    {
        si (o->vitia[i].genus == genus)
        {
            redde &o->vitia[i];
        }
    }
    redde NIHIL;
}

interior i32
_vitia_generis (
    constans MusicxmlOpus* o,
      MusicxmlVitiumGenus  genus)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < o->numerus_vitiorum; i++)
    {
        si (o->vitia[i].genus == genus)
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
_nota_una_probare (
    CredoContextus* c)
{
                MusicxmlOpus* o = _legere_fixum(c, "nota_una.musicxml");
       constans MusicxmlPars* p;
    constans MusicxmlMensura* m;
       constans MusicxmlNota* n;
           ChordaAedificator* a;

    CREDO_NECESSE (o != NIHIL);
    CREDO_VERUM (musicxml_successus(o));
    CREDO_AEQUALIS_I32 (o->numerus_vitiorum, ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)o->forma, (s32)MUSICXML_FORMA_PARTIUM);
    CREDO_VERUM (_aequat(o->versio, "4.0"));
    CREDO_VERUM (_aequat(o->titulus_operis, "Probatio"));
    CREDO_VERUM (_aequat(o->compositor, "Anonymus"));
    CREDO_AEQUALIS_S64 (o->divisiones, (s64)I);
    CREDO_NECESSE (o->numerus_partium == I);
    p = &o->partes[ZEPHYRUM];
    CREDO_VERUM (_aequat(p->id, "P1"));
    CREDO_VERUM (_aequat(p->titulus, "Cantus"));
    CREDO_NECESSE (p->numerus_mensurarum == I);
    m = &p->mensurae[ZEPHYRUM];
    CREDO_VERUM (_aequat(m->numerus, "1"));
    CREDO_AEQUALIS_S64 (m->initium, (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S64 (m->longitudo, (s64)I);
    CREDO_NECESSE (m->numerus_notarum == I);
    n = &m->notae[ZEPHYRUM];
    CREDO_AEQUALIS_S32 ((s32)n->genus, (s32)MUSICXML_NOTA_SONANS);
    CREDO_AEQUALIS_S32 ((s32)n->littera, (s32)'C');
    CREDO_F64_PROXIMUS (n->alteratio, 0.0, 1e-9);
    CREDO_AEQUALIS_S32 (n->octava, (s32)IV);
    CREDO_AEQUALIS_S32 ((s32)n->littera_sonans, (s32)'C');
    CREDO_AEQUALIS_S32 (n->nota, (s32)LX);
    CREDO_AEQUALIS_S64 (n->positio, (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S64 (n->longitudo, (s64)I);
    CREDO_VERUM (_aequat(n->vox, "1"));

    a = chorda_aedificator_creare(c->piscina, CCLVI);
    CREDO_VERUM (musicxml_tabulam_scribere(o, NIHIL, a));
    CREDO_VERUM (_aequat(chorda_aedificator_finire(a),
        "DATA musicxml tabula 1\n"
        "# pars\tinitium\tlongitudo\tmidi\tlittera\talteratio\tgratia"
        "\tversus\n"
        "P1\t0\t1\t60\tC\t0\t0\t-\n"));
}

interior vacuum
_divisiones_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c,
             "divisiones_mutatae.musicxml");
    constans MusicxmlMensura* m1;
    constans MusicxmlMensura* m2;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    /* multiplex communis (II, III, IV) = XII, non maxima IV */
    CREDO_AEQUALIS_S64 (o->divisiones, (s64)XII);
    CREDO_NECESSE (o->numerus_partium == II);
    m1 = &o->partes[ZEPHYRUM].mensurae[ZEPHYRUM];
    m2 = &o->partes[I].mensurae[ZEPHYRUM];
    CREDO_NECESSE (m1->numerus_notarum == II
        && m2->numerus_notarum == IV);
    CREDO_AEQUALIS_S64 (m1->notae[ZEPHYRUM].longitudo, (s64)XII);
    CREDO_AEQUALIS_S64 (m1->notae[I].positio, (s64)XII);
    CREDO_AEQUALIS_S64 (m2->notae[ZEPHYRUM].longitudo, (s64)IV);
    CREDO_AEQUALIS_S64 (m2->notae[I].positio, (s64)IV);
    CREDO_AEQUALIS_S64 (m2->notae[II].positio, (s64)VIII);
    CREDO_AEQUALIS_S64 (m2->notae[III].positio, (s64)XII);
    CREDO_AEQUALIS_S64 (m2->notae[III].longitudo, (s64)XII);
    CREDO_AEQUALIS_S64 (m1->longitudo, (s64)XXIV);
    CREDO_AEQUALIS_S64 (m2->longitudo, (s64)XXIV);
}

interior vacuum
_divisiones_nimiae_probare (
    CredoContextus* c)
{
    MusicxmlOpus* o = _legere_textum(c,
        "<score-partwise version=\"4.0\"><part-list>"
        "<score-part id=\"P1\"/><score-part id=\"P2\"/></part-list>"
        "<part id=\"P1\"><measure number=\"1\"><attributes>"
        "<divisions>2147483647</divisions></attributes></measure>"
        "</part><part id=\"P2\"><measure number=\"1\"><attributes>"
        "<divisions>2</divisions></attributes></measure></part>"
        "</score-partwise>");

    CREDO_NECESSE (o != NIHIL);
    CREDO_FALSUM (musicxml_successus(o));
    CREDO_AEQUALIS_I32 (_vitia_generis(o, MUSICXML_DIVISIONES_NIMIAE),
        (i32)I);
}

interior vacuum
_voces_probare (
    CredoContextus* c)
{
                MusicxmlOpus* o = _legere_fixum(c, "voces.musicxml");
    constans MusicxmlMensura* m;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    m = &o->partes[ZEPHYRUM].mensurae[ZEPHYRUM];
    /* forward notam non relinquit: IV + pausa + G4 */
    CREDO_NECESSE (m->numerus_notarum == VI);
    CREDO_AEQUALIS_S32 ((s32)m->notae[IV].genus,
        (s32)MUSICXML_NOTA_PAUSA);
    CREDO_AEQUALIS_S64 (m->notae[IV].positio, (s64)ZEPHYRUM);
    CREDO_VERUM (_aequat(m->notae[IV].vox, "2"));
    CREDO_AEQUALIS_S32 ((s32)m->notae[V].littera, (s32)'G');
    CREDO_AEQUALIS_S64 (m->notae[V].positio, (s64)II);
    CREDO_AEQUALIS_S64 (m->notae[V].longitudo, (s64)II);
    CREDO_VERUM (_aequat(m->notae[V].vox, "2"));
    CREDO_AEQUALIS_S64 (m->longitudo, (s64)IV);
}

/* Focus recensionis 1: acordum post regressum */
interior vacuum
_acordum_post_regressum_probare (
    CredoContextus* c)
{
                MusicxmlOpus* o = _legere_fixum(c, "acordum.musicxml");
    constans MusicxmlMensura* m;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    m = &o->partes[ZEPHYRUM].mensurae[ZEPHYRUM];
    CREDO_NECESSE (m->numerus_notarum == V);
    CREDO_AEQUALIS_S64 (m->notae[I].positio, (s64)ZEPHYRUM);
    CREDO_FALSUM (m->notae[I].acordum);
    CREDO_AEQUALIS_S64 (m->notae[II].positio, (s64)ZEPHYRUM);
    CREDO_VERUM (m->notae[II].acordum);
    CREDO_AEQUALIS_S64 (m->notae[II].longitudo, (s64)I);
    CREDO_AEQUALIS_S64 (m->notae[III].positio, (s64)ZEPHYRUM);
    CREDO_VERUM (m->notae[III].acordum);
    /* cursor per notam primam acordi (II), non per E4 (I) */
    CREDO_AEQUALIS_S64 (m->notae[IV].positio, (s64)II);
    CREDO_AEQUALIS_I32 (o->numerus_vitiorum, ZEPHYRUM);
}

interior vacuum
_tactus_brevis_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c, "tactus_brevis.musicxml");
    constans MusicxmlPars* p;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    p = &o->partes[ZEPHYRUM];
    CREDO_NECESSE (p->numerus_mensurarum == III);
    CREDO_VERUM (_aequat(p->mensurae[ZEPHYRUM].numerus, "0"));
    CREDO_VERUM (p->mensurae[ZEPHYRUM].implicita);
    CREDO_AEQUALIS_S64 (p->mensurae[ZEPHYRUM].longitudo, (s64)I);
    CREDO_FALSUM (p->mensurae[I].implicita);
    CREDO_AEQUALIS_S64 (p->mensurae[I].initium, (s64)I);
    CREDO_AEQUALIS_S64 (p->mensurae[II].initium, (s64)IV);
    CREDO_NECESSE (o->numerus_mensurarum == III);
    CREDO_AEQUALIS_S64 (o->initia[II], (s64)IV);
    CREDO_AEQUALIS_S64 (o->longitudo, (s64)VII);
}

/* M15: linea temporis communis */
interior vacuum
_mensurae_inaequales_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c,
                               "mensurae_inaequales.musicxml");
    constans MusicxmlVitium* v;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    CREDO_AEQUALIS_S64 (o->partes[ZEPHYRUM].mensurae[I].initium,
        (s64)IV);
    CREDO_AEQUALIS_S64 (o->partes[I].mensurae[I].initium, (s64)IV);
    CREDO_AEQUALIS_S64 (o->partes[I].mensurae[ZEPHYRUM].longitudo,
        (s64)III);
    CREDO_AEQUALIS_I32 (_vitia_generis(o, MUSICXML_MENSURA_BREVIOR),
        (i32)I);
    v = _vitium(o, MUSICXML_MENSURA_BREVIOR);
    CREDO_NECESSE (v != NIHIL);
    CREDO_AEQUALIS_S32 ((s32)v->gravitas, (s32)MUSICXML_MONITUM);
    CREDO_VERUM (_continet(v->causa, "P2"));
    CREDO_AEQUALIS_I32 (v->linea, (i32)XXII);
    CREDO_AEQUALIS_I32 (v->columna, (i32)V);
}

interior vacuum
_vitia_musica_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c, "vitia_musica.musicxml");
    constans MusicxmlMensura* m;
    constans MusicxmlVitium* v;

    CREDO_NECESSE (o != NIHIL);
    CREDO_VERUM (musicxml_successus(o));
    m = &o->partes[ZEPHYRUM].mensurae[ZEPHYRUM];
    /* C4 sine duration omittitur */
    CREDO_NECESSE (m->numerus_notarum == III);
    CREDO_AEQUALIS_S32 ((s32)m->notae[ZEPHYRUM].littera, (s32)'D');
    CREDO_AEQUALIS_S64 (m->notae[ZEPHYRUM].positio, (s64)ZEPHYRUM);
    /* backup III ex I: coercitus ad 0; chord prima vocis: nota plana */
    CREDO_AEQUALIS_S32 ((s32)m->notae[I].littera, (s32)'E');
    CREDO_AEQUALIS_S64 (m->notae[I].positio, (s64)ZEPHYRUM);
    CREDO_FALSUM (m->notae[I].acordum);
    CREDO_AEQUALIS_S64 (m->notae[II].positio, (s64)I);

    v = _vitium(o, MUSICXML_FILIUS_DEEST);
    CREDO_NECESSE (v != NIHIL);
    CREDO_AEQUALIS_S32 ((s32)v->gravitas, (s32)MUSICXML_MONITUM);
    CREDO_AEQUALIS_I32 (v->linea, (i32)XI);
    CREDO_AEQUALIS_I32 (v->columna, (i32)VII);
    CREDO_AEQUALIS_I32 (_vitia_generis(o, MUSICXML_REGRESSUS_NIMIUS),
        (i32)I);
    CREDO_AEQUALIS_I32 (_vitia_generis(o, MUSICXML_ACORDUM_PRIMUM),
        (i32)I);
    v = _vitium(o, MUSICXML_LIGATURA_PENDENS);
    CREDO_NECESSE (v != NIHIL);
    CREDO_AEQUALIS_I32 (v->linea, (i32)XV);
}

/* Focus recensionis 5: index partium et partes discordant */
interior vacuum
_index_partium_probare (
    CredoContextus* c)
{
    MusicxmlOpus* o = _legere_fixum(c, "index_partium.musicxml");

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    CREDO_NECESSE (o->numerus_partium == III);
    CREDO_VERUM (_aequat(o->partes[ZEPHYRUM].id, "P1"));
    CREDO_VERUM (_aequat(o->partes[I].id, "P3"));
    CREDO_VERUM (_aequat(o->partes[I].titulus, "Bassus"));
    CREDO_VERUM (_aequat(o->partes[II].id, "P4"));
    CREDO_VERUM (_aequat(o->partes[II].titulus, "P4"));
    CREDO_AEQUALIS_I32 (_vitia_generis(o, MUSICXML_PARS_DISCORS),
        (i32)II);
}

interior vacuum
_non_musicxml_probare (
    CredoContextus* c)
{
    MusicxmlOpus* o;

    o = _legere_textum(c, "<html/>");
    CREDO_NECESSE (o != NIHIL);
    CREDO_FALSUM (musicxml_successus(o));
    CREDO_NON_NIHIL (_vitium(o, MUSICXML_NON_MUSICXML));

    o = _legere_textum(c, "<a");
    CREDO_NECESSE (o != NIHIL);
    CREDO_FALSUM (musicxml_successus(o));
    CREDO_NON_NIHIL (_vitium(o, MUSICXML_XML_MALUM));
    CREDO_MAIOR_I32 (_vitium(o, MUSICXML_XML_MALUM)->causa.mensura,
        ZEPHYRUM);

    o = _legere_textum(c, "");
    CREDO_NECESSE (o != NIHIL);
    CREDO_FALSUM (musicxml_successus(o));

    o = _legere_textum(c, "<score-partwise version=\"4.0\">"
        "<part-list/></score-partwise>");
    CREDO_NECESSE (o != NIHIL);
    CREDO_FALSUM (musicxml_successus(o));
    CREDO_NON_NIHIL (_vitium(o, MUSICXML_SINE_PARTIBUS));
}

interior vacuum
_attributa_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c, "attributa.musicxml");
    constans MusicxmlMensura* m;
    constans MusicxmlAttributa* t;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    m = &o->partes[ZEPHYRUM].mensurae[ZEPHYRUM];
    CREDO_NECESSE (m->numerus_attributorum == I);
    t = &m->attributa[ZEPHYRUM];
    CREDO_AEQUALIS_S64 (t->positio, (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S64 (t->divisiones, (s64)VI);
    CREDO_VERUM (t->clavis_adest);
    CREDO_VERUM (t->clavis.tradita);
    CREDO_AEQUALIS_S32 (t->clavis.quintae, (s32)-II);
    CREDO_VERUM (_aequat(t->clavis.modus, "minor"));
    CREDO_VERUM (t->metrum_adest);
    CREDO_FALSUM (t->metrum.liberum);
    CREDO_NECESSE (t->metrum.numerus_partium == I);
    CREDO_VERUM (_aequat(t->metrum.partes[ZEPHYRUM].pulsus, "6"));
    CREDO_AEQUALIS_I32 (t->metrum.partes[ZEPHYRUM].summa, (i32)VI);
    CREDO_AEQUALIS_I32 (t->metrum.partes[ZEPHYRUM].unitas, (i32)VIII);
    CREDO_AEQUALIS_I32 (t->baculi, (i32)II);
    CREDO_NECESSE (t->numerus_clavicularum == II);
    CREDO_AEQUALIS_I32 (t->claviculae[ZEPHYRUM].baculum, (i32)I);
    CREDO_AEQUALIS_S32 ((s32)t->claviculae[ZEPHYRUM].signum, (s32)'G');
    CREDO_AEQUALIS_I32 (t->claviculae[ZEPHYRUM].linea, (i32)II);
    CREDO_AEQUALIS_I32 (t->claviculae[I].baculum, (i32)II);
    CREDO_AEQUALIS_S32 ((s32)t->claviculae[I].signum, (s32)'F');
    CREDO_AEQUALIS_I32 (t->claviculae[I].linea, (i32)IV);
    CREDO_FALSUM (t->transpositio_adest);
    CREDO_AEQUALIS_I32 (m->notae[I].baculum, (i32)II);
    CREDO_VERUM (_aequat(m->notae[I].vox, "5"));

    m = &o->partes[ZEPHYRUM].mensurae[I];
    CREDO_NECESSE (m->numerus_attributorum == I);
    t = &m->attributa[ZEPHYRUM];
    CREDO_AEQUALIS_S64 (t->divisiones, (s64)ZEPHYRUM);
    CREDO_VERUM (t->clavis_adest);
    CREDO_FALSUM (t->clavis.tradita);
    CREDO_NECESSE (t->clavis.numerus_graduum == II);
    CREDO_AEQUALIS_S32 ((s32)t->clavis.litterae[ZEPHYRUM], (s32)'F');
    CREDO_AEQUALIS_S32 ((s32)t->clavis.litterae[I], (s32)'G');
    CREDO_F64_PROXIMUS (t->clavis.alterationes[I], 1.0, 1e-9);
    CREDO_VERUM (_aequat(t->metrum.partes[ZEPHYRUM].pulsus, "3+2"));
    CREDO_AEQUALIS_I32 (t->metrum.partes[ZEPHYRUM].summa, (i32)V);
    CREDO_AEQUALIS_I32 (t->metrum.partes[ZEPHYRUM].unitas, (i32)VIII);
    CREDO_AEQUALIS_S64 (m->longitudo, (s64)XV);
}

/* .mxl: idem exemplar ac XML nudum (tabulae octetim aequales) */
interior vacuum
_arca_probare (
    CredoContextus* c)
{
         MusicxmlOpus* x = _legere_fixum(c, "nota_una.musicxml");
         MusicxmlOpus* z = _legere_fixum(c, "nota_una.mxl");
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, CCLVI);
    ChordaAedificator* b = chorda_aedificator_creare(c->piscina, CCLVI);

    CREDO_NECESSE (x != NIHIL && z != NIHIL);
    CREDO_VERUM (musicxml_successus(z));
    CREDO_AEQUALIS_I32 (z->numerus_vitiorum, ZEPHYRUM);
    CREDO_VERUM (_aequat(z->titulus_operis, "Probatio"));
    CREDO_VERUM (musicxml_tabulam_scribere(x, NIHIL, a));
    CREDO_VERUM (musicxml_tabulam_scribere(z, NIHIL, b));
    CREDO_VERUM (chorda_aequalis(chorda_aedificator_finire(a),
        chorda_aedificator_finire(b)));
}

/* Focus recensionis 4: arcae veteres et variae */
interior vacuum
_arcae_variae_probare (
    CredoContextus* c)
{
               MusicxmlOpus* o;
    constans MusicxmlVitium* v;

    o = _legere_fixum(c, "sine_mimetype.mxl");
    CREDO_NECESSE (o != NIHIL);
    CREDO_VERUM (musicxml_successus(o));
    CREDO_AEQUALIS_I32 (o->numerus_partium, (i32)I);

    /* rootfile PRIMUS, in directorio, sine media-type */
    o = _legere_fixum(c, "radix_in_directorio.mxl");
    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    CREDO_VERUM (_aequat(o->partes[ZEPHYRUM].titulus, "Cantus"));

    o = _legere_fixum(c, "opus.mxl");
    CREDO_NECESSE (o != NIHIL);
    CREDO_FALSUM (musicxml_successus(o));
    CREDO_NON_NIHIL (_vitium(o, MUSICXML_OPUS_RADIX));

    o = _legere_fixum(c, "container_malum.mxl");
    CREDO_NECESSE (o != NIHIL);
    CREDO_FALSUM (musicxml_successus(o));
    v = _vitium(o, MUSICXML_ARCA_MALA);
    CREDO_NECESSE (v != NIHIL);
    CREDO_VERUM (_continet(v->causa, "META-INF/container.xml"));
    CREDO_AEQUALIS_I32 (v->linea, ZEPHYRUM);

    o = _legere_fixum(c, "radix_absens.mxl");
    CREDO_NECESSE (o != NIHIL);
    v = _vitium(o, MUSICXML_ARCA_MALA);
    CREDO_NECESSE (v != NIHIL);
    CREDO_VERUM (_continet(v->causa, "nihil.musicxml"));

    o = _legere_fixum(c, "utf16.mxl");
    CREDO_NECESSE (o != NIHIL);
    CREDO_FALSUM (musicxml_successus(o));
    CREDO_NON_NIHIL (_vitium(o, MUSICXML_NON_UTF8));

    /* signum ZIP cum octetis temerariis: vitium, non ruina */
    o = musicxml_legere(chorda_ex_literis("PK\003\004\377\377junk-data",
        c->piscina), c->piscina);
    CREDO_NECESSE (o != NIHIL);
    CREDO_FALSUM (musicxml_successus(o));
    CREDO_NON_NIHIL (_vitium(o, MUSICXML_ARCA_MALA));

    /* XML nudum UTF-16 quoque */
    o =
        musicxml_legere(chorda_ex_literis("\377\376<\000a\000/\000>\000",
        c->piscina), c->piscina);
    CREDO_NECESSE (o != NIHIL);
    CREDO_NON_NIHIL (_vitium(o, MUSICXML_NON_UTF8));
}

/* fons verus: Tallis, O nata lux (CPDL #80317, MusicXML 2.0, .mxl sine
 * mimetype, container DEFLATE) */
interior vacuum
_nata_lux_probare (
    CredoContextus* c)
{
    MusicxmlOpus* o = musicxml_legere(filum_legere_totum(
        "musica/fontes/nata_lux_80317.mxl", c->piscina), c->piscina);
              i32 p;
              i32 notae = ZEPHYRUM;

    CREDO_NECESSE (o != NIHIL);
    CREDO_VERUM (musicxml_successus(o));
    CREDO_VERUM (_aequat(o->versio, "2.0"));
    /* editio munda: ne monitum quidem */
    CREDO_AEQUALIS_I32 (o->numerus_vitiorum, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (o->divisiones, (s64)CDLXXX);
    CREDO_NECESSE (o->numerus_partium == V);
    per (p = ZEPHYRUM; p < o->numerus_partium; p++)
    {
        i32 k;

        CREDO_AEQUALIS_I32 (o->partes[p].numerus_mensurarum, (i32)IV);
        per (k = ZEPHYRUM; k < o->partes[p].numerus_mensurarum; k++)
        {
            notae += o->partes[p].mensurae[k].numerus_notarum;
        }
    }
    CREDO_AEQUALIS_I32 (notae, (i32)CCCLXXIII);
    CREDO_VERUM (_aequat(o->partes[ZEPHYRUM].mensurae[ZEPHYRUM].numerus,
        "0"));
    CREDO_NECESSE (o->partes[ZEPHYRUM].mensurae[ZEPHYRUM]
        .numerus_attributorum >= I);
    CREDO_VERUM (o->partes[ZEPHYRUM].mensurae[ZEPHYRUM].attributa[ZEPHYRUM]
        .metrum.liberum);
}

interior chorda
_tabula (
           CredoContextus* c,
    constans MusicxmlOpus* o)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, MXXIV);

    (vacuum)musicxml_tabulam_scribere(o, NIHIL, a);
    redde chorda_aedificator_finire(a);
}

interior vacuum
_verba_probare (
    CredoContextus* c)
{
             MusicxmlOpus* o = _legere_fixum(c, "verba.musicxml");
    constans MusicxmlNota* n;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    n = o->partes[ZEPHYRUM].mensurae[ZEPHYRUM].notae;
    CREDO_NECESSE (n[ZEPHYRUM].numerus_syllabarum == II);
    CREDO_AEQUALIS_I32 (n[ZEPHYRUM].syllabae[ZEPHYRUM].versus, (i32)I);
    CREDO_VERUM (_aequat(n[ZEPHYRUM].syllabae[ZEPHYRUM].versus_textus,
        "1"));
    CREDO_AEQUALIS_S32 ((s32)n[ZEPHYRUM].syllabae[ZEPHYRUM].locus,
        (s32)MUSICXML_SYLLABA_INITIUM);
    CREDO_VERUM (_aequat(n[ZEPHYRUM].syllabae[ZEPHYRUM].textus, "Glo"));
    CREDO_AEQUALIS_I32 (n[ZEPHYRUM].syllabae[I].versus, (i32)II);
    CREDO_AEQUALIS_S32 ((s32)n[ZEPHYRUM].syllabae[I].locus,
        (s32)MUSICXML_SYLLABA_SINGULA);
    CREDO_NECESSE (n[I].numerus_syllabarum == I);
    CREDO_AEQUALIS_S32 ((s32)n[I].syllabae[ZEPHYRUM].locus,
        (s32)MUSICXML_SYLLABA_MEDIA);
    /* elisio: duae syllabae versus I, deinde versus II (ordine
     * versuum, quamvis versus II prior scriptus) */
    CREDO_NECESSE (n[II].numerus_syllabarum == III);
    CREDO_VERUM (_aequat(n[II].syllabae[ZEPHYRUM].textus, "a"));
    CREDO_FALSUM (n[II].syllabae[ZEPHYRUM].iuncta);
    CREDO_VERUM (_aequat(n[II].syllabae[I].textus, "in"));
    CREDO_VERUM (n[II].syllabae[I].iuncta);
    CREDO_AEQUALIS_S32 ((s32)n[II].syllabae[I].locus,
        (s32)MUSICXML_SYLLABA_INITIUM);
    CREDO_AEQUALIS_I32 (n[II].syllabae[II].versus, (i32)II);
    CREDO_VERUM (n[III].syllabae[ZEPHYRUM].extensio);
    CREDO_AEQUALIS_I32 (o->partes[ZEPHYRUM].mensurae[I].notae[ZEPHYRUM]
        .numerus_syllabarum, ZEPHYRUM);
    CREDO_VERUM (_continet(_tabula(c, o),
        "\t1:end:a|1:begin:in|2:single:et\n"));
}

interior vacuum
_ligaturae_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c, "ligaturae_tupli.musicxml");
    constans MusicxmlPars* p;
                   chorda  t;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    CREDO_AEQUALIS_I32 (o->numerus_vitiorum, ZEPHYRUM);
    CREDO_AEQUALIS_S64 (o->divisiones, (s64)XXI);
    p = &o->partes[ZEPHYRUM];
    CREDO_VERUM (p->mensurae[ZEPHYRUM].notae[III].ligatura_initium);
    CREDO_VERUM (p->mensurae[I].notae[ZEPHYRUM].ligatura_finis);
    /* tied sine tie non ligat */
    CREDO_FALSUM (p->mensurae[I].notae[I].ligatura_initium);
    CREDO_FALSUM (p->mensurae[II].notae[ZEPHYRUM].ligatura_finis);
    t = _tabula(c, o);
    CREDO_VERUM (_continet(t, "P1\t1\t2\t79\tG\t"));
    CREDO_VERUM (_continet(t, "P1\t3\t1\t81\tA\t"));
    CREDO_VERUM (_continet(t, "P1\t4\t2\t81\tA\t"));
}

interior vacuum
_rationes_temporis_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c, "ligaturae_tupli.musicxml");
    constans MusicxmlNota* n;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    n = o->partes[ZEPHYRUM].mensurae[ZEPHYRUM].notae;
    CREDO_AEQUALIS_S64 (n[ZEPHYRUM].longitudo, (s64)VII);
    CREDO_AEQUALIS_I32 (n[ZEPHYRUM].ratio_notarum, (i32)III);
    CREDO_AEQUALIS_I32 (n[ZEPHYRUM].ratio_spatii, (i32)II);
    CREDO_AEQUALIS_S64 (n[III].positio, (s64)XXI);
    n = o->partes[I].mensurae[ZEPHYRUM].notae;
    CREDO_AEQUALIS_S64 (n[VI].positio, (s64)XVIII);
    CREDO_AEQUALIS_S64 (n[VI].longitudo, (s64)III);
    CREDO_AEQUALIS_I32 (n[VI].ratio_notarum, (i32)VII);
    CREDO_AEQUALIS_I32 (n[VI].ratio_spatii, (i32)IV);
    CREDO_VERUM (_continet(_tabula(c, o), "P2\t6/7\t1/7\t59\tB\t"));
}

interior vacuum
_gratiae_probare (
    CredoContextus* c)
{
                MusicxmlOpus* o = _legere_fixum(c, "gratiae.musicxml");
    constans MusicxmlMensura* m;
       constans MusicxmlNota* n;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    CREDO_AEQUALIS_I32 (o->numerus_vitiorum, ZEPHYRUM);
    m = &o->partes[ZEPHYRUM].mensurae[ZEPHYRUM];
    n = m->notae;
    CREDO_NECESSE (m->numerus_notarum == VI);
    CREDO_VERUM (n[ZEPHYRUM].gratia);
    CREDO_VERUM (n[ZEPHYRUM].gratia_obliqua);
    CREDO_AEQUALIS_S64 (n[ZEPHYRUM].longitudo, (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S64 (n[ZEPHYRUM].positio, (s64)ZEPHYRUM);
    CREDO_AEQUALIS_S64 (n[I].positio, (s64)ZEPHYRUM);
    CREDO_VERUM (n[II].gratia);
    CREDO_FALSUM (n[II].gratia_obliqua);
    CREDO_AEQUALIS_S32 (n[II].furtum_prioris, (s32)XX);
    CREDO_AEQUALIS_S64 (n[II].tempus_factum, (s64)I);
    CREDO_AEQUALIS_S64 (n[II].positio, (s64)II);
    CREDO_AEQUALIS_S64 (n[III].positio, (s64)II);
    CREDO_VERUM (n[IV].parva);
    CREDO_AEQUALIS_S64 (n[IV].longitudo, (s64)II);
    CREDO_AEQUALIS_S64 (n[V].positio, (s64)VI);
    CREDO_AEQUALIS_S64 (m->longitudo, (s64)VIII);
    CREDO_VERUM (_continet(_tabula(c, o),
        "P1\t0\t0\t72\tC\t0\t1\t-\n"));
}

interior vacuum
_transpositio_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c, "transpositio.musicxml");
    constans MusicxmlNota* n;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    CREDO_FALSUM (o->concentus);
    n = o->partes[ZEPHYRUM].mensurae[ZEPHYRUM].notae;
    CREDO_AEQUALIS_S32 ((s32)n[ZEPHYRUM].littera, (s32)'D');
    CREDO_AEQUALIS_S32 (n[ZEPHYRUM].octava, (s32)V);
    CREDO_AEQUALIS_S32 (n[ZEPHYRUM].nota, (s32)LXXII);
    CREDO_AEQUALIS_S32 ((s32)n[ZEPHYRUM].littera_sonans, (s32)'C');
    CREDO_AEQUALIS_S32 (n[ZEPHYRUM].octava_sonans, (s32)V);
    CREDO_F64_PROXIMUS (n[ZEPHYRUM].alteratio_sonans, 0.0, 1e-9);
    CREDO_AEQUALIS_S32 (n[I].nota, (s32)LXXVI);
    CREDO_AEQUALIS_S32 ((s32)n[I].littera_sonans, (s32)'E');
    CREDO_F64_PROXIMUS (n[I].alteratio_sonans, 0.0, 1e-9);
    n = o->partes[I].mensurae[ZEPHYRUM].notae;
    CREDO_AEQUALIS_S32 (n[ZEPHYRUM].nota, (s32)LII);
    CREDO_AEQUALIS_S32 ((s32)n[ZEPHYRUM].littera_sonans, (s32)'E');
    CREDO_AEQUALIS_S32 (n[ZEPHYRUM].octava_sonans, (s32)III);
}

interior vacuum
_partitura_concentus_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c,
                               "partitura_concentus.musicxml");
    constans MusicxmlNota* n;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    CREDO_VERUM (o->concentus);
    n = o->partes[ZEPHYRUM].mensurae[ZEPHYRUM].notae;
    CREDO_AEQUALIS_S32 (n[ZEPHYRUM].nota, (s32)LXXIV);
    CREDO_AEQUALIS_S32 ((s32)n[ZEPHYRUM].littera_sonans, (s32)'D');
}

interior vacuum
_percussio_probare (
    CredoContextus* c)
{
         MusicxmlOpus* o = _legere_fixum(c, "percussio.musicxml");
    constans MusicxmlPars* p;
    constans MusicxmlMensura* m;

    CREDO_NECESSE (o != NIHIL && musicxml_successus(o));
    p = &o->partes[ZEPHYRUM];
    CREDO_NECESSE (p->numerus_instrumentorum == II);
    CREDO_VERUM (_aequat(p->instrumenta[I].id, "P1-I38"));
    CREDO_VERUM (_aequat(p->instrumenta[I].titulus, "Snare"));
    CREDO_AEQUALIS_I32 (p->instrumenta[I].canalis, (i32)X);
    CREDO_AEQUALIS_I32 (p->instrumenta[I].percussio, (i32)XXXIX);
    CREDO_F64_PROXIMUS (p->instrumenta[I].volumen, 80.0, 1e-9);
    CREDO_F64_PROXIMUS (p->instrumenta[I].positio, -45.0, 1e-9);
    m = &p->mensurae[ZEPHYRUM];
    CREDO_AEQUALIS_S32 ((s32)m->attributa[ZEPHYRUM].claviculae[ZEPHYRUM]
        .signum, (s32)'P');
    CREDO_AEQUALIS_S32 ((s32)m->notae[ZEPHYRUM].genus,
        (s32)MUSICXML_NOTA_PERCUSSA);
    CREDO_VERUM (_aequat(m->notae[ZEPHYRUM].instrumentum, "P1-I38"));
    CREDO_AEQUALIS_S32 (m->notae[ZEPHYRUM].nota, (s32)XXXVIII);
    /* instrumentum ignotum: nota ex display (E4) */
    CREDO_AEQUALIS_S32 (m->notae[I].nota, (s32)LXIV);
    CREDO_VERUM (_continet(_tabula(c, o), "P1\t0\t1\t38\tC\t"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "nota una: exemplar et tabula", _nota_una_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "divisio operis: multiplex communis", _divisiones_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "divisiones nimiae: vitium", _divisiones_nimiae_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "voces: backup, forward", _voces_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "acordum post regressum", _acordum_post_regressum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "tactus brevis (anacrusis)", _tactus_brevis_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "mensurae inaequales: linea communis",
        _mensurae_inaequales_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia musica: monita, opus utile", _vitia_musica_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "index partium discors", _index_partium_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "non MusicXML, XML malum, vacuum", _non_musicxml_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "attributa: armatura, metrum, claviculae", _attributa_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { ".mxl: idem ac XML nudum", _arca_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { ".mxl varia: mimetype, directorium, opus, mala, UTF-16",
        _arcae_variae_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nata lux (fons verus)", _nata_lux_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "verba: versus, syllabica, elisio, extensio", _verba_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "ligaturae: tie (non tied)", _ligaturae_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "tupli: 3:2, 7:4", _rationes_temporis_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "gratiae et notae parvae", _gratiae_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "transpositio: sonans derivatum", _transpositio_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "concert-score: numquam bis", _partitura_concentus_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "percussio et instrumenta", _percussio_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("musicxml", SECTIONES, NIHIL, NIHIL);
}
