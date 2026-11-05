/* chronica_eventus.c - eventa ex lineis paginae (chronica C1).
 * Lineae in textum unum iunguntur ('\n'), initia linearum servata;
 * scrutatio '<event' quaerit, attributa legit, deinde primum ex
 * "</event>", "</>", '<event' novo. Positiones textus in (linea,
 * columna) per initia vertuntur. */
#include "chronica_eventus.h"
#include <string.h>

nomen structura {
    chorda  textus;     /* lineae '\n' iunctae (in piscina) */
       i32* initia;     /* initium lineae cuiusque in textu */
       i32  numerus;    /* lineae */
} Textus;

nomen structura {
    chorda annus;
    chorda mensis;
    chorda dies;
    chorda personae;
    chorda locus;
    chorda genus;
       b32 annus_adest;
       b32 mensis_adest;
       b32 dies_adest;
} Attributa;

interior b32
spatium_album (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* positio textus -> linea (ultima cuius initium <= positio) */
interior i32
linea_positionis (
    constans Textus* t,
                i32  positio)
{
    i32 a = ZEPHYRUM;
    i32 b = t->numerus;
    i32 m;

    dum (b - a > I)
    {
        m = a + (b - a) / II;
        si (t->initia[m] <= positio)
        {
            a = m;
        }
        alioquin
        {
            b = m;
        }
    }
    redde a;
}

/* ambitum [initium, finis) in quattuor campos vertere: finis post
 * octetum ultimum (finis - 1 eius lineam dat) */
interior vacuum
ambitum_ponere (
    constans Textus* t,
                i32  initium,
                i32  finis,
                i32* linea,
                i32* columna,
                i32* linea_finis,
                i32* columna_finis)
{
    i32 l;

    *linea    = linea_positionis(t, initium);
    *columna  = initium - t->initia[*linea];
    l         = finis > initium ? linea_positionis(t, finis - I)
                                : *linea;
    *linea_finis    = l;
    *columna_finis  = finis - t->initia[l];
}

interior b32
textum_iungere (
    constans Xar* lineae,
         Piscina* piscina,
          Textus* t)
{
    i32 n;
    i32 i;
    i32 mensura;
    i32 positio;

    n        = xar_numerus(lineae);
    mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        mensura += ((constans chorda*)xar_obtinere(lineae,
            i))->mensura + I;
    }
    t->numerus  = n > ZEPHYRUM ? n : I;
    t->initia   = (i32*)piscina_allocare(piscina,
        (memoriae_index)t->numerus * magnitudo(i32));
    t->textus.datum    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + I));
    t->textus.mensura  = ZEPHYRUM;
    si (!t->initia || !t->textus.datum)
    {
        redde FALSUM;
    }
    t->initia[ZEPHYRUM]  = ZEPHYRUM;
    positio              = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans chorda* l = (constans chorda*)xar_obtinere(lineae, i);

        t->initia[i] = positio;
        si (l->mensura > ZEPHYRUM)
        {
            memcpy(t->textus.datum + positio, l->datum,
                (size_t)l->mensura);
        }
        positio += l->mensura;
        si (i + I < n)
        {
            t->textus.datum[positio] = '\n';
            positio++;
        }
    }
    t->textus.mensura = positio;
    redde VERUM;
}

/* literae ad positionem? */
interior b32
congruit (
       constans Textus* t,
                   i32  positio,
    constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde    positio + n <= t->textus.mensura
          && memcmp(t->textus.datum + positio, literae, (size_t)n)
             == ZEPHYRUM;
}

/* '<event' ad positionem, deinde spatium album aut '>' */
interior b32
tag_incipit (
    constans Textus* t,
                i32  positio)
{
    i8 sequens;

    si (!congruit(t, positio, "<event"))
    {
        redde FALSUM;
    }
    si (positio + VI >= t->textus.mensura)
    {
        redde VERUM;    /* "<event" in fine: tag non clausum */
    }
    sequens = t->textus.datum[positio + VI];
    redde spatium_album(sequens) || sequens == '>';
}

/* tag proximus ab initio; t->textus.mensura si nullus */
interior i32
tag_proximum (
    constans Textus* t,
                i32  initium)
{
    i32 i;

    per (i = initium; i < t->textus.mensura; i++)
    {
        si (t->textus.datum[i] == '<' && tag_incipit(t, i))
        {
            redde i;
        }
    }
    redde t->textus.mensura;
}

interior vacuum
attributum_ponere (
    Attributa* a,
       chorda  titulus,
       chorda  valor)
{
    si (chorda_aequalis_literis(titulus, "year"))
    {
        a->annus        = valor;
        a->annus_adest  = VERUM;
    }
    alioquin si (chorda_aequalis_literis(titulus, "month"))
    {
        a->mensis        = valor;
        a->mensis_adest  = VERUM;
    }
    alioquin si (chorda_aequalis_literis(titulus, "day"))
    {
        a->dies        = valor;
        a->dies_adest  = VERUM;
    }
    alioquin si (chorda_aequalis_literis(titulus, "people"))
    {
        a->personae = valor;
    }
    alioquin si (chorda_aequalis_literis(titulus, "place"))
    {
        a->locus = valor;
    }
    alioquin si (chorda_aequalis_literis(titulus, "type"))
    {
        a->genus = valor;
    }
}

/* attributa a positione (post "<event") usque ad '>'; positio '>'
 * in *finis. FALSUM si '>' deest aut '<' ante eum (tag fractum) */
interior b32
attributa_legere (
    constans Textus* t,
                i32  positio,
          Attributa* a,
                i32* finis)
{
        i8* d = t->textus.datum;
       i32  n = t->textus.mensura;
       i32  initium;
    chorda  titulus;
        i8  quotum;

    memset(a, ZEPHYRUM, magnitudo(*a));
    dum (positio < n)
    {
        dum (positio < n && spatium_album(d[positio]))
        {
            positio++;
        }
        si (positio >= n || d[positio] == '<')
        {
            redde FALSUM;
        }
        si (d[positio] == '>')
        {
            *finis = positio;
            redde VERUM;
        }
        initium = positio;
        dum (   positio < n && d[positio] != '=' && d[positio] != '>'
             && d[positio] != '<' && !spatium_album(d[positio]))
        {
            positio++;
        }
        titulus = chorda_sectio(t->textus, initium, positio);
        si (positio >= n || d[positio] != '=')
        {
            perge;    /* attributum sine valore: neglectum */
        }
        positio++;
        si (positio < n && (d[positio] == '"' || d[positio] == '\''))
        {
            quotum   = d[positio];
            positio++;
            initium  = positio;
            dum (positio < n && d[positio] != quotum)
            {
                positio++;
            }
            si (positio >= n)
            {
                redde FALSUM;
            }
            attributum_ponere(a, titulus, chorda_sectio(t->textus,
                initium, positio));
            positio++;
        }
        alioquin
        {
            initium = positio;
            dum (   positio < n && d[positio] != '>'
                 && d[positio] != '<' && !spatium_album(d[positio]))
            {
                positio++;
            }
            attributum_ponere(a, titulus, chorda_sectio(t->textus,
                initium, positio));
        }
    }
    redde FALSUM;
}

/* spatia (et fines linearum) collapsa, extrema tonsa */
interior chorda
collapsum (
     chorda  s,
    Piscina* piscina)
{
    chorda r;
       i32 i;
       b32 spatium;

    r.datum    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(s.mensura + I));
    r.mensura  = ZEPHYRUM;
    si (!r.datum)
    {
        redde r;
    }
    spatium = FALSUM;
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (spatium_album(s.datum[i]))
        {
            spatium = r.mensura > ZEPHYRUM;
            perge;
        }
        si (spatium)
        {
            r.datum[r.mensura] = ' ';
            r.mensura++;
            spatium = FALSUM;
        }
        r.datum[r.mensura] = s.datum[i];
        r.mensura++;
    }
    redde r;
}

/* nomina ';' divisa, tonsa, vacua omissa */
interior Xar*
personas_dividere (
     chorda  s,
    Piscina* piscina)
{
       Xar* x;
    chorda  nomen_personae;
       i32  initium;
       i32  i;

    x = xar_creare(piscina, (i32)magnitudo(chorda));
    si (!x)
    {
        redde NIHIL;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= s.mensura; i++)
    {
        si (i == s.mensura || s.datum[i] == ';')
        {
            nomen_personae = chorda_praecidere(chorda_sectio(s,
                initium, i));
            si (nomen_personae.mensura > ZEPHYRUM)
            {
                chorda* c = (chorda*)xar_addere(x);

                si (!c)
                {
                    redde NIHIL;
                }
                *c = nomen_personae;
            }
            initium = i + I;
        }
    }
    redde x;
}

/* causa vitii attributorum aut NIHIL si bona; valores in e */
interior constans character*
attributa_iudicare (
    constans Attributa* a,
       ChronicaEventus* e)
{
    s32 n;

    si (!a->annus_adest)
    {
        redde "annus deest";
    }
    si (   !chorda_ut_s32(chorda_praecidere(a->annus), &n)
        || n == ZEPHYRUM)
    {
        redde "annus invalidus";
    }
    e->annus = n;
    si (a->mensis_adest)
    {
        si (   !chorda_ut_s32(chorda_praecidere(a->mensis), &n)
            || n < I || n > XII)
        {
            redde "mensis invalidus";
        }
        e->mensis = n;
    }
    si (a->dies_adest)
    {
        si (!a->mensis_adest)
        {
            redde "dies sine mense";
        }
        si (   !chorda_ut_s32(chorda_praecidere(a->dies), &n)
            || n < I || n > XXXI)
        {
            redde "dies invalidus";
        }
        e->dies = n;
    }
    redde NIHIL;
}

interior b32
vitium_addere (
       constans Textus* t,
                   Xar* vitia,
                   i32  initium,
                   i32  finis,
    constans character* causa,
               Piscina* piscina)
{
    ChronicaVitium* v;

    si (!vitia)
    {
        redde VERUM;
    }
    v = (ChronicaVitium*)xar_addere(vitia);
    si (!v)
    {
        redde FALSUM;
    }
    ambitum_ponere(t, initium, finis, &v->linea, &v->columna,
        &v->linea_finis, &v->columna_finis);
    v->causa = chorda_ex_literis(causa, piscina);
    redde VERUM;
}

b32
chronica_eventa_legere (
    constans Xar* lineae,
         Piscina* piscina,
             Xar* eventa,
             Xar* vitia)
{
                Textus  t;
             Attributa  a;
       ChronicaEventus  e;
    constans character* causa;
                   i32  positio;
                   i32  finis_tag;
                   i32  proximus;
                   i32  i;
                   i32  clausura;
                   i32  finis;

    si (   !lineae || !piscina || !eventa
        || !textum_iungere(lineae, piscina, &t))
    {
        redde FALSUM;
    }
    positio = tag_proximum(&t, ZEPHYRUM);
    dum (positio < t.textus.mensura)
    {
        si (!attributa_legere(&t, positio + VI, &a, &finis_tag))
        {
            proximus = tag_proximum(&t, positio + I);
            si (!vitium_addere(&t, vitia, positio, proximus
                < t.textus.mensura ? proximus : t.textus.mensura,
                    "non clausum", piscina))
            {
                redde FALSUM;
            }
            positio = proximus;
            perge;
        }
        /* clausura aut tag novus, quod prius */
        proximus  = tag_proximum(&t, finis_tag + I);
        clausura  = t.textus.mensura;
        finis     = t.textus.mensura;
        per (i = finis_tag + I; i < proximus; i++)
        {
            si (congruit(&t, i, "</event>"))
            {
                clausura  = i;
                finis     = i + VIII;
                frange;
            }
            si (congruit(&t, i, "</>"))
            {
                clausura  = i;
                finis     = i + III;
                frange;
            }
        }
        si (clausura >= proximus)
        {
            si (!vitium_addere(&t, vitia, positio, proximus,
                    "non clausum", piscina))
            {
                redde FALSUM;
            }
            positio = proximus;
            perge;
        }
        memset(&e, ZEPHYRUM, magnitudo(e));
        causa = attributa_iudicare(&a, &e);
        si (causa)
        {
            si (!vitium_addere(&t, vitia, positio, finis, causa,
                    piscina))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            ChronicaEventus* novum;

            e.descriptio  = collapsum(chorda_sectio(t.textus,
                finis_tag + I, clausura), piscina);
            e.personae  = personas_dividere(a.personae, piscina);
            e.locus     = chorda_praecidere(a.locus);
            e.genus     = chorda_praecidere(a.genus);
            ambitum_ponere(&t, positio, finis, &e.linea, &e.columna,
                &e.linea_finis, &e.columna_finis);
            novum = (ChronicaEventus*)xar_addere(eventa);
            si (!novum || !e.personae)
            {
                redde FALSUM;
            }
            *novum = e;
        }
        positio = tag_proximum(&t, finis);
    }
    redde VERUM;
}
