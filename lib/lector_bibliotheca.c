/* lector_bibliotheca.c - bibliotheca codicum EPUB
 * (include/lector_bibliotheca.h) */
#include "lector_bibliotheca.h"
#include "filum.h"
#include "iter_directoria.h"
#include "stml.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

#define VESTIGIUM_MAXIMUM  XXXII   /* octeti vestigii servati */

nomen structura {
    LectorLiber  liber;          /* primum membrum */
          Codex* codex;           /* NIHIL si non legitur */
            Xar* longitudines;    /* i32 per caput; NIHIL = nondum */
} LiberInternus;

structura LectorBibliotheca {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
                    Xar* libri;    /* LiberInternus */
};

interior chorda
vacua (
    vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

/* comparatio sine casu litterarum (ASCII) */
interior s32
comparare_sine_casu (
    chorda a,
    chorda b)
{
    i32 i;
     i8 x;
     i8 y;

    per (i = ZEPHYRUM; i < a.mensura && i < b.mensura; i++)
    {
        x = a.datum[i];
        y = b.datum[i];
        si (x >= 'A' && x <= 'Z')
        {
            x = (i8)(x + ('a' - 'A'));
        }
        si (y >= 'A' && y <= 'Z')
        {
            y = (i8)(y + ('a' - 'A'));
        }
        si (x != y)
        {
            redde x < y ? -I : I;
        }
    }
    si (a.mensura == b.mensura)
    {
        redde ZEPHYRUM;
    }
    redde a.mensura < b.mensura ? -I : I;
}

/* ordo bibliothecae: legibiles ante non legibiles; legibiles per
 * titulum sine casu; ceteri ordinem fili servant */
interior b32
ante_est (
    constans LiberInternus* a,
    constans LiberInternus* b)
{
    si ((a->codex != NIHIL) != (b->codex != NIHIL))
    {
        redde a->codex != NIHIL;
    }
    si (a->codex)
    {
        redde comparare_sine_casu(a->liber.titulus, b->liber.titulus)
            < ZEPHYRUM;
    }
    redde FALSUM;
}

interior b32
nomen_codicis_est (
    chorda titulus)
{
    redde titulus.mensura > V
        && memcmp(titulus.datum + titulus.mensura - V, ".epub", V)
           == ZEPHYRUM;
}

/* liber unus (via, nomen fili) in bibliothecam; duplex identitas
 * praetermittitur */
interior vacuum
librum_addere (
    LectorBibliotheca* b,
               chorda  via,
               chorda  nomen_fili)
{
     LiberInternus* li;
     LiberInternus  novus;
            chorda  datum;
            chorda  vitium;
            chorda  identitas;
               i32  i;

    memset(&novus, ZEPHYRUM, magnitudo(LiberInternus));
    novus.liber.via  = via;
    vitium           = vacua();
    datum            = filum_legere_totum(chorda_ut_cstr(via,
        b->piscina),
        b->piscina);
    novus.codex      = datum.mensura > ZEPHYRUM ? codex_aperire(datum,
        b->piscina, b->intern, &vitium) : NIHIL;
    si (!novus.codex)
    {
        novus.liber.titulus  = nomen_fili;
        novus.liber.vitium   = vitium.mensura > ZEPHYRUM ? vitium
            : chorda_ex_literis("file could not be read", b->piscina);
    }
    alioquin
    {
        identitas = codex_identitas(novus.codex);
        si (identitas.mensura == ZEPHYRUM)
        {
            identitas = nomen_fili;
        }
        per (i = ZEPHYRUM; i < xar_numerus(b->libri); i++)
        {
            li = (LiberInternus*)xar_obtinere(b->libri, i);
            si (   li->codex && chorda_aequalis(li->liber.identitas,
                    identitas))
            {
                redde;   /* idem liber: primum nomine fili vincit */
            }
        }
        novus.liber.identitas  = identitas;
        novus.liber.titulus    = codex_titulus(novus.codex).mensura
            > ZEPHYRUM ? codex_titulus(novus.codex) : nomen_fili;
        novus.liber.auctor     = codex_auctor(novus.codex);
    }
    li = (LiberInternus*)xar_addere(b->libri);
    si (li)
    {
        *li = novus;
    }
}

LectorBibliotheca*
lector_bibliotheca_aperire (
     constans character* via,
                Volumen* volumen,
                Piscina* piscina,
    InternamentumChorda* intern)
{
       LectorBibliotheca* b;
     DirectoriumIterator* it;
    DirectoriumIntroitus* e;
                     Xar* nomina;
                  chorda  t;
                  chorda* x;
                  chorda* y;
           LiberInternus  li;
                     i32  i;
                     i32  j;

    si (!via || !piscina || !intern)
    {
        redde NIHIL;
    }
    b = (LectorBibliotheca*)piscina_allocare(piscina,
        magnitudo(LectorBibliotheca));
    si (!b)
    {
        redde NIHIL;
    }
    b->piscina  = piscina;
    b->intern   = intern;
    b->volumen  = volumen;
    b->libri    = xar_creare(piscina, (i32)magnitudo(LiberInternus));
    nomina      = xar_creare(piscina, (i32)magnitudo(chorda));
    si (!b->libri || !nomina)
    {
        redde NIHIL;
    }
    it = directorium_iterator_aperire(via, piscina);
    dum (it && (e = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (   e->genus == INTROITUS_FILUM
            && nomen_codicis_est(e->titulus))
        {
            *(chorda*)xar_addere(nomina) = chorda_transcribere(
                e->titulus, piscina);
        }
    }
    si (it)
    {
        directorium_iterator_claudere(it);
    }
    /* nomina fili ordine octetorum (duplex: primum vincit) */
    per (i = I; i < xar_numerus(nomina); i++)
    {
        per (j = i; j > ZEPHYRUM; j--)
        {
            x = (chorda*)xar_obtinere(nomina, j - I);
            y = (chorda*)xar_obtinere(nomina, j);
            si (chorda_comparare(*x, *y) <= ZEPHYRUM)
            {
                frange;
            }
            t   = *x;
            *x  = *y;
            *y  = t;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        t = *(chorda*)xar_obtinere(nomina, i);
        librum_addere(b, chorda_concatenare(chorda_ex_literis(via,
            piscina), chorda_concatenare(chorda_ex_literis("/",
            piscina),
            t, piscina), piscina), t);
    }
    /* ordo bibliothecae (insertio: stabilis) */
    per (i = I; i < xar_numerus(b->libri); i++)
    {
        per (j = i; j > ZEPHYRUM; j--)
        {
            si (!ante_est((LiberInternus*)xar_obtinere(b->libri, j),
                    (LiberInternus*)xar_obtinere(b->libri, j - I)))
            {
                frange;
            }
            li = *(LiberInternus*)xar_obtinere(b->libri, j);
            *(LiberInternus*)xar_obtinere(b->libri, j) =
                *(LiberInternus*)xar_obtinere(b->libri, j - I);
            *(LiberInternus*)xar_obtinere(b->libri, j - I) = li;
        }
    }
    redde b;
}

i32
lector_bibliotheca_numerus (
    constans LectorBibliotheca* b)
{
    redde b && b->libri ? xar_numerus(b->libri) : ZEPHYRUM;
}

constans LectorLiber*
lector_bibliotheca_liber (
    constans LectorBibliotheca* b,
                           i32  k)
{
    si (k >= lector_bibliotheca_numerus(b))
    {
        redde NIHIL;
    }
    redde &((constans LiberInternus*)xar_obtinere(b->libri, k))->liber;
}

constans LectorLiber*
lector_bibliotheca_invenire (
    constans LectorBibliotheca* b,
                        chorda  identitas)
{
    constans LiberInternus* li;
                       i32  i;

    per (i = ZEPHYRUM; i < lector_bibliotheca_numerus(b); i++)
    {
        li = (constans LiberInternus*)xar_obtinere(b->libri, i);
        si (   li->codex && identitas.mensura > ZEPHYRUM
            && chorda_aequalis(li->liber.identitas, identitas))
        {
            redde &li->liber;
        }
    }
    redde NIHIL;
}

/* internus libri publici (per indicem, non per conversionem caecam) */
interior LiberInternus*
internum (
          LectorBibliotheca* b,
       constans LectorLiber* liber)
{
    LiberInternus* li;
              i32  i;

    per (i = ZEPHYRUM; liber && i < lector_bibliotheca_numerus(b); i++)
    {
        li = (LiberInternus*)xar_obtinere(b->libri, i);
        si (&li->liber == liber)
        {
            redde li;
        }
    }
    redde NIHIL;
}

Codex*
lector_bibliotheca_codex (
          LectorBibliotheca* b,
       constans LectorLiber* liber,
                     chorda* vitium)
{
    LiberInternus* li;

    li = internum(b, liber);
    si (!li || !li->codex)
    {
        si (vitium)
        {
            *vitium = li ? li->liber.vitium : chorda_ex_literis(
                "no such book", b->piscina);
        }
        redde NIHIL;
    }
    redde li->codex;
}

/* via plagulae: "lector/positio/" + identitas, %XX extra
 * A-Za-z0-9-_.~ */
interior chorda
viam_positionis (
      chorda  identitas,
     Piscina* piscina)
{
    hic_manens constans character  notae[] = "0123456789ABCDEF";
                ChordaAedificator* a;
                              i32  i;
                               i8  x;

    a = chorda_aedificator_creare(piscina, LXIV);
    si (!a)
    {
        redde vacua();
    }
    chorda_aedificator_appendere_literis(a, "lector/positio/");
    per (i = ZEPHYRUM; i < identitas.mensura; i++)
    {
        x = identitas.datum[i];
        si (   (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')
            || (x >= '0' && x <= '9') || x == '-' || x == '_'
            || x == '.'
            || x == '~')
        {
            chorda_aedificator_appendere_character(a, (character)x);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, '%');
            chorda_aedificator_appendere_character(a,
                notae[x >> IV]);
            chorda_aedificator_appendere_character(a,
                notae[x & XV]);
        }
    }
    redde chorda_aedificator_finire(a);
}

b32
lector_positionem_scribere (
         LectorBibliotheca* b,
                    chorda  identitas,
    constans LectorPositio* positio)
{
                Piscina* t;
    InternamentumChorda* in;
              StmlNodus* n;
                 chorda  v;
                    b32  bene;
              character  numerus[XXIV];

    si (!b || !b->volumen || !positio || identitas.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* piscina et internamentum temporaria: scriptura in omni motu
     * paginae, nihil in bibliotheca crescit */
    t = piscina_generare_dynamicum("lector_positio", MXXIV);
    si (!t)
    {
        redde FALSUM;
    }
    in  = internamentum_creare(t);
    n   = in ? stml_elementum_creare(t, in, "positio") : NIHIL;
    v   = positio->vestigium;
    si (v.mensura > VESTIGIUM_MAXIMUM)
    {
        v.mensura = VESTIGIUM_MAXIMUM;
        dum (   v.mensura > ZEPHYRUM
             && (v.datum[v.mensura] & 0xC0) == 0x80)
        {
            v.mensura--;
        }
    }
    bene = n != NIHIL;
    sprintf(numerus, "%u", (insignatus integer)positio->caput);
    bene = bene && stml_attributum_addere(n, t, in, "caput", numerus);
    sprintf(numerus, "%u", (insignatus integer)positio->initium);
    bene = bene && stml_attributum_addere(n, t, in, "initium", numerus);
    /* vestigium ut TEXTUS (scriptor effugit): valores attributorum
     * STML cruda sunt - '"' ea frangeret */
    bene = bene
        && (v.mensura == ZEPHYRUM || stml_textum_addere(n, t, in,
        chorda_ut_cstr(v, t)));
    bene = bene && volumen_plagulam_condere(b->volumen,
        viam_positionis(identitas, t), stml_scribere(n, t, FALSUM),
        "lector:positio");
    piscina_destruere(t);
    redde bene;
}

b32
lector_positionem_legere (
    LectorBibliotheca* b,
               chorda  identitas,
        LectorPositio* positio,
              Piscina* piscina)
{
          chorda  c;
          chorda* a;
    StmlResultus  r;
             b32  inventum;

    si (   !b || !b->volumen || !positio || !piscina
        || identitas.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    inventum  = FALSUM;
    c         = volumen_plagulam_promere(b->volumen, viam_positionis(
        identitas, piscina), piscina, &inventum);
    si (!inventum)
    {
        redde FALSUM;
    }
    r = stml_legere(c, piscina, internamentum_creare(piscina));
    si (!r.successus || !r.elementum_radix)
    {
        redde FALSUM;
    }
    a = stml_attributum_capere(r.elementum_radix, "caput");
    si (!a || !chorda_ut_i32(*a, &positio->caput))
    {
        redde FALSUM;
    }
    a = stml_attributum_capere(r.elementum_radix, "initium");
    si (!a || !chorda_ut_i32(*a, &positio->initium))
    {
        redde FALSUM;
    }
    positio->vestigium = stml_textus_valor(r.elementum_radix, piscina);
    redde VERUM;
}

s32
lector_progressus (
          LectorBibliotheca* b,
       constans LectorLiber* liber)
{
     LiberInternus* li;
           Piscina* t;
        CodexCaput* k;
     LectorPositio  q;
            chorda  vitium;
               i64  ante;
               i64  omnes;
               i32  i;
               i32  longitudo;
               b32  servata;

    li = internum(b, liber);
    si (!li || !li->codex)
    {
        redde -I;
    }
    t = piscina_generare_dynamicum("lector_progressus", MXXIV);
    si (!t)
    {
        redde -I;
    }
    servata = lector_positionem_legere(b, liber->identitas, &q, t);
    piscina_destruere(t);
    si (!servata)
    {
        redde -I;
    }
    si (!li->longitudines)
    {
        /* semel: capita omnia legere, longitudines servare */
        li->longitudines = xar_creare(b->piscina, (i32)magnitudo(i32));
        per (i = ZEPHYRUM; li->longitudines
            && i < codex_numerus_capitum(li->codex); i++)
        {
            t          = piscina_generare_dynamicum("lector_caput",
                MXXIV);
            k          = t ? codex_caput_legere(li->codex, i, t,
                &vitium)
                : NIHIL;
            longitudo = k ? k->textus.mensura : ZEPHYRUM;
            *(i32*)xar_addere(li->longitudines) = longitudo;
            si (t)
            {
                piscina_destruere(t);
            }
        }
    }
    si (!li->longitudines)
    {
        redde -I;
    }
    ante   = ZEPHYRUM;
    omnes  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(li->longitudines); i++)
    {
        longitudo = *(i32*)xar_obtinere(li->longitudines, i);
        si (i < q.caput)
        {
            ante += longitudo;
        }
        alioquin si (i == q.caput)
        {
            ante += q.initium < longitudo ? q.initium : longitudo;
        }
        omnes += longitudo;
    }
    si (omnes == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s32)(ante * C / omnes);
}
