/* agenda.c - Agenda (include/agenda.h; agenda-plan A1).
 *
 * Piscinae tres praeter vocantis: 'status' proiectionem tenet (vacatur
 * in vacare - historia reproicit), 'scheda' actum unum parsat et
 * sigillum facit, 'acta' actum novum struit (vivit dum historia eum
 * appendit et applicat). */

#include "agenda.h"
#include "stml.h"
#include "sigillum.h"

#include <string.h>

hic_manens constans character genus_actorum[]   = "agenda";
hic_manens constans character origo_checkpoint[] = "agenda:checkpoint";

/* res cum signo deletionis (deletae ex proiectione publica absunt,
 * id earum numeratur) */
nomen structura {
    AgendaRes res;
          b32 deleta;
} Recordum;

structura AgendaDocumentum {
                Piscina* piscina;
    InternamentumChorda* intern;
               Historia* historia;
                Piscina* status;
                Piscina* scheda;
                Piscina* acta;
                    Xar* recorda;    /* Recordum, ordine id */
                    s32  proximum;   /* id rei novae */
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior Recordum*
recordum (
    constans AgendaDocumentum* doc,
                          s32  id)
{
    Recordum* r;
         i32  i;

    per (i = ZEPHYRUM; doc->recorda
        && i < xar_numerus(doc->recorda); i++)
    {
        r = (Recordum*)xar_obtinere(doc->recorda, i);
        si (r->res.id == id)
        {
            redde r;
        }
    }
    redde NIHIL;
}

/* res viva (non deleta) aut NIHIL */
interior Recordum*
vivum (
    constans AgendaDocumentum* doc,
                          s32  id)
{
    Recordum* r;

    r = recordum(doc, id);
    redde (r && !r->deleta) ? r : NIHIL;
}

/* textus non vacuus (spatium album solum = vacuus) */
interior b32
plenus (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (   c.datum[i] != ' ' && c.datum[i] != '\t'
            && c.datum[i] != '\n' && c.datum[i] != '\r')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
dies_legere (
    chorda* c,
      Dies* dies)
{
    redde c && c->mensura > ZEPHYRUM && fasti_parsare_iso(*c, dies)
        && fasti_dies_valida(*dies);
}

/* rubrica [a-z0-9-]+ */
interior b32
rubrica_valida (
    chorda c)
{
    i32 i;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (!(   (c.datum[i] >= 'a' && c.datum[i] <= 'z')
              || (c.datum[i] >= '0' && c.datum[i] <= '9')
              || c.datum[i] == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior s32
rubricas_comparare (
    chorda a,
    chorda b)
{
    i32 n;
    s32 r;

    n = a.mensura < b.mensura ? a.mensura : b.mensura;
    r = (s32)memcmp(a.datum, b.datum, (size_t)n);
    si (r != ZEPHYRUM)
    {
        redde r;
    }
    redde (s32)a.mensura - (s32)b.mensura;
}

/* "a b  c" -> Xar chorda (frusta in textu ipso), ordinata, sine
 * iteratis; NIHIL si rubrica mala */
interior Xar*
rubricas_legere (
     chorda  textus,
    Piscina* piscina)
{
       Xar* l;
    chorda  r;
    chorda* x;
       i32  i;
       i32  j;
       i32  k;

    l = xar_creare(piscina, (i32)magnitudo(chorda));
    i = ZEPHYRUM;
    dum (i < textus.mensura)
    {
        dum (i < textus.mensura && textus.datum[i] == ' ')
        {
            i++;
        }
        j = i;
        dum (j < textus.mensura && textus.datum[j] != ' ')
        {
            j++;
        }
        si (j == i)
        {
            frange;
        }
        r.datum    = textus.datum + i;
        r.mensura  = j - i;
        i          = j;
        si (!rubrica_valida(r))
        {
            redde NIHIL;
        }
        /* insertio ordinata, iterata praeteruntur */
        k = ZEPHYRUM;
        dum (   k < xar_numerus(l)
             && rubricas_comparare(*(chorda*)xar_obtinere(l, k), r)
                < ZEPHYRUM)
        {
            k++;
        }
        si (   k < xar_numerus(l)
            && rubricas_comparare(*(chorda*)xar_obtinere(l, k), r)
               == ZEPHYRUM)
        {
            perge;
        }
        (vacuum)xar_addere(l);
        per (j = xar_numerus(l) - I; j > k; j--)
        {
            x   = (chorda*)xar_obtinere(l, j);
            *x  = *(chorda*)xar_obtinere(l, j - I);
        }
        *(chorda*)xar_obtinere(l, k) = r;
    }
    redde l;
}

/* copia rubricarum -> "a b" (vacua: datum NIHIL) */
interior chorda
rubricas_iungere (
        Xar* l,
    Piscina* piscina)
{
    chorda s;
       i32 i;

    s.datum    = NIHIL;
    s.mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (i > ZEPHYRUM)
        {
            s = chorda_concatenare(s, chorda_ex_literis(" ", piscina),
                piscina);
        }
        s = chorda_concatenare(s, *(chorda*)xar_obtinere(l, i),
            piscina);
    }
    redde s;
}

interior vacuum
attributum_ponere (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* titulus,
                 chorda  valor)
{
    si (valor.mensura > ZEPHYRUM)
    {
        (vacuum)stml_attributum_addere_chorda(n, p, in, titulus, valor);
    }
}

interior vacuum
diem_ponere (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* titulus,
                   Dies  dies)
{
    attributum_ponere(n, p, in, titulus, fasti_formare_diem(dies,
        FASTI_FORMA_ISO, p));
}

/* titulus forma elementi attributi '<@titulus=>...</>' (quota duplex
 * repraesentabilis); liberum PRIMUM esse debet */
interior vacuum
titulum_addere (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
                 chorda  titulus)
{
    StmlNodus* a;

    a = stml_elementum_creare(p, in, "@");
    si (!a)
    {
        redde;
    }
    a->attributum_titulus = chorda_internare_ex_literis(in, "titulus");
    (vacuum)stml_liberum_addere(a, stml_textum_creare_ex_chorda(p, in,
        titulus));
    (vacuum)stml_liberum_addere(n, a);
}

interior StmlNodus*
elementum_rei (
                Piscina* p,
    InternamentumChorda* in,
     constans character* actio,
                    s32  id)
{
    StmlNodus* n;

    n = stml_elementum_creare(p, in, "res");
    si (!n)
    {
        redde NIHIL;
    }
    si (actio)
    {
        (vacuum)stml_attributum_addere(n, p, in, "actio", actio);
    }
    attributum_ponere(n, p, in, "id", chorda_ex_s32(id, p));
    redde n;
}

interior s32
attributum_id (
    StmlNodus* n)
{
    chorda* a;
       s32  v;

    a = stml_attributum_capere(n, "id");
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Proiectio (historia L1)
 * ================================================== */

interior vacuum
statum_vacare (
    AgendaDocumentum* doc)
{
    piscina_vacare(doc->status);
    doc->recorda   = xar_creare(doc->status, (i32)magnitudo(Recordum));
    doc->proximum  = I;
}

/* recordum novum ordine id (id > omnibus: append; decodificatio ordine
 * canonico) */
interior Recordum*
recordum_addere (
    AgendaDocumentum* doc,
                 s32  id,
              chorda  titulus)
{
    Recordum* r;

    r = (Recordum*)xar_addere(doc->recorda);
    memset(r, ZEPHYRUM, magnitudo(Recordum));
    r->res.id        = id;
    r->res.titulus   = chorda_transcribere(titulus, doc->status);
    r->res.rubricae  = xar_creare(doc->status, (i32)magnitudo(chorda));
    r->res.notae     = xar_creare(doc->status,
        (i32)magnitudo(AgendaNota));
    si (id >= doc->proximum)
    {
        doc->proximum = id + I;
    }
    redde r;
}

/* rubricae in piscinam status (frusta textus parsati transcribuntur) */
interior b32
rubricas_ponere (
    AgendaDocumentum* doc,
            Recordum* r,
              chorda* textus)
{
     Xar* l;
     i32  i;

    l = rubricas_legere(textus ? *textus : chorda_ex_literis("",
        doc->scheda), doc->scheda);
    si (!l)
    {
        redde FALSUM;
    }
    r->res.rubricae = xar_creare(doc->status, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        *(chorda*)xar_addere(r->res.rubricae) = chorda_transcribere(
            *(chorda*)xar_obtinere(l, i), doc->status);
    }
    redde VERUM;
}

interior vacuum
notam_addere (
    AgendaDocumentum* doc,
            Recordum* r,
                Dies  dies,
              chorda  textus)
{
    AgendaNota* n;

    n          = (AgendaNota*)xar_addere(r->res.notae);
    n->dies    = dies;
    n->textus  = chorda_transcribere(textus, doc->status);
}

interior vacuum
proiectio_vacare (
    vacuum* ctx)
{
    statum_vacare((AgendaDocumentum*)ctx);
}

/* actum unum; malum aut ignotum praeteritur (replicatio robusta) */
interior vacuum
proiectio_applicare (
    vacuum* ctx,
    chorda  actum)
{
    AgendaDocumentum* doc;
        StmlResultus  res;
           StmlNodus* n;
              chorda* actio;
              chorda* t;
            Recordum* r;
                 s32  id;
                Dies  dies;

    doc = (AgendaDocumentum*)ctx;
    piscina_vacare(doc->scheda);
    res = stml_legere_ex_literis(chorda_ut_cstr(actum, doc->scheda),
        doc->scheda, doc->intern);
    n = res.elementum_radix;
    si (   !res.successus || !n || !n->titulus
        || !chorda_aequalis_literis(*n->titulus, "res"))
    {
        redde;
    }
    actio  = stml_attributum_capere(n, "actio");
    id     = attributum_id(n);
    si (!actio || id <= ZEPHYRUM)
    {
        redde;
    }
    si (chorda_aequalis_literis(*actio, "nova"))
    {
        t = stml_attributum_capere(n, "titulus");
        si (t && plenus(*t) && !recordum(doc, id))
        {
            (vacuum)recordum_addere(doc, id, *t);
        }
        redde;
    }
    r = vivum(doc, id);
    si (!r)
    {
        redde;
    }
    si (chorda_aequalis_literis(*actio, "titulus"))
    {
        t = stml_attributum_capere(n, "titulus");
        si (t && plenus(*t))
        {
            r->res.titulus = chorda_transcribere(*t, doc->status);
        }
    }
    alioquin si (chorda_aequalis_literis(*actio, "dies"))
    {
        r->res.dies_posita = dies_legere(stml_attributum_capere(n,
            "dies"), &dies);
        si (r->res.dies_posita)
        {
            r->res.dies = dies;
        }
    }
    alioquin si (chorda_aequalis_literis(*actio, "rubricae"))
    {
        (vacuum)rubricas_ponere(doc, r, stml_attributum_capere(n,
            "rubricae"));
    }
    alioquin si (chorda_aequalis_literis(*actio, "nota"))
    {
        si (dies_legere(stml_attributum_capere(n, "dies"), &dies))
        {
            notam_addere(doc, r, dies, stml_textus_valor(n,
                doc->scheda));
        }
    }
    alioquin si (chorda_aequalis_literis(*actio, "perfecta"))
    {
        si (dies_legere(stml_attributum_capere(n, "dies"), &dies))
        {
            r->res.perfecta           = VERUM;
            r->res.dies_perfectionis  = dies;
        }
    }
    alioquin si (chorda_aequalis_literis(*actio, "aperta"))
    {
        r->res.perfecta = FALSUM;
    }
    alioquin si (chorda_aequalis_literis(*actio, "deleta"))
    {
        r->deleta = VERUM;
    }
}

/* status canonicus: <agenda proximum><res id [dies] [rubricae]
 * [perfecta]><@titulus=>..</><nota dies>..</nota>..</res>..</agenda>;
 * deletae absunt (proximum eas numerat) */
interior chorda
proiectio_codificare (
     vacuum* ctx,
    Piscina* piscina)
{
    AgendaDocumentum* doc;
           StmlNodus* radix;
           StmlNodus* e;
           StmlNodus* nota;
            Recordum* r;
          AgendaNota* an;
                 i32  i;
                 i32  k;

    doc    = (AgendaDocumentum*)ctx;
    radix  = stml_elementum_creare(piscina, doc->intern, "agenda");
    attributum_ponere(radix, piscina, doc->intern, "proximum",
        chorda_ex_s32(doc->proximum, piscina));
    per (i = ZEPHYRUM; i < xar_numerus(doc->recorda); i++)
    {
        r = (Recordum*)xar_obtinere(doc->recorda, i);
        si (r->deleta)
        {
            perge;
        }
        e = elementum_rei(piscina, doc->intern, NIHIL, r->res.id);
        si (r->res.dies_posita)
        {
            diem_ponere(e, piscina, doc->intern, "dies", r->res.dies);
        }
        attributum_ponere(e, piscina, doc->intern, "rubricae",
            rubricas_iungere(r->res.rubricae, piscina));
        si (r->res.perfecta)
        {
            diem_ponere(e, piscina, doc->intern, "perfecta",
                r->res.dies_perfectionis);
        }
        titulum_addere(e, piscina, doc->intern, r->res.titulus);
        per (k = ZEPHYRUM; k < xar_numerus(r->res.notae); k++)
        {
            an    = (AgendaNota*)xar_obtinere(r->res.notae, k);
            nota  = stml_elementum_creare(piscina, doc->intern, "nota");
            diem_ponere(nota, piscina, doc->intern, "dies", an->dies);
            (vacuum)stml_liberum_addere(nota,
                stml_textum_creare_ex_chorda(piscina, doc->intern,
                an->textus));
            (vacuum)stml_liberum_addere(e, nota);
        }
        (vacuum)stml_liberum_addere(radix, e);
    }
    redde stml_scribere(radix, piscina, FALSUM);
}

interior vacuum
proiectio_sigillare (
      vacuum* ctx,
    Sigillum* exitus)
{
     AgendaDocumentum* doc;
    SigillumContextus  sc;
               chorda  textus;

    doc = (AgendaDocumentum*)ctx;
    piscina_vacare(doc->scheda);
    textus = proiectio_codificare(doc, doc->scheda);
    sigillum_incipere(&sc);
    sigillum_addere(&sc, textus.datum, (memoriae_index)textus.mensura);
    *exitus = sigillum_finire(&sc);
}

/* restitutio ex statu canonico; FALSUM = codex inutilis (historia
 * reproicit ex nihilo) */
interior b32
proiectio_decodificare (
    vacuum* ctx,
    chorda  codex)
{
    AgendaDocumentum* doc;
        StmlResultus  res;
           StmlNodus* radix;
           StmlNodus* e;
           StmlNodus* nota;
            Recordum* r;
              chorda* t;
                 s32  proximum;
                 i32  i;
                 i32  k;
                Dies  dies;

    doc = (AgendaDocumentum*)ctx;
    piscina_vacare(doc->scheda);
    res = stml_legere_ex_literis(chorda_ut_cstr(codex, doc->scheda),
        doc->scheda, doc->intern);
    radix = res.elementum_radix;
    si (   !res.successus || !radix || !radix->titulus
        || !chorda_aequalis_literis(*radix->titulus, "agenda"))
    {
        redde FALSUM;
    }
    t = stml_attributum_capere(radix, "proximum");
    si (!t || !chorda_ut_s32(*t, &proximum) || proximum < I)
    {
        redde FALSUM;
    }
    statum_vacare(doc);
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        e = stml_liberum_ad_indicem(radix, i);
        si (   e->genus != STML_NODUS_ELEMENTUM || !e->titulus
            || !chorda_aequalis_literis(*e->titulus, "res"))
        {
            perge;
        }
        t = stml_attributum_capere(e, "titulus");
        si (attributum_id(e) <= ZEPHYRUM || !t)
        {
            redde FALSUM;
        }
        r = recordum_addere(doc, attributum_id(e), *t);
        r->res.dies_posita = dies_legere(stml_attributum_capere(e,
            "dies"), &dies);
        si (r->res.dies_posita)
        {
            r->res.dies = dies;
        }
        si (!rubricas_ponere(doc, r, stml_attributum_capere(e,
                "rubricae")))
        {
            redde FALSUM;
        }
        si (dies_legere(stml_attributum_capere(e, "perfecta"), &dies))
        {
            r->res.perfecta           = VERUM;
            r->res.dies_perfectionis  = dies;
        }
        per (k = ZEPHYRUM; k < stml_numerus_liberorum(e); k++)
        {
            nota = stml_liberum_ad_indicem(e, k);
            si (   nota->genus != STML_NODUS_ELEMENTUM || !nota->titulus
                || !chorda_aequalis_literis(*nota->titulus, "nota"))
            {
                perge;
            }
            si (!dies_legere(stml_attributum_capere(nota, "dies"),
                &dies))
            {
                redde FALSUM;
            }
            notam_addere(doc, r, dies, stml_textus_valor(nota,
                doc->scheda));
        }
    }
    doc->proximum = proximum;
    redde VERUM;
}

interior HistoriaProiectio
proiectio_facere (
    AgendaDocumentum* doc)
{
    HistoriaProiectio p;

    memset(&p, ZEPHYRUM, magnitudo(p));
    p.memoria       = NIHIL;
    p.mensura       = ZEPHYRUM;
    p.vacare        = proiectio_vacare;
    p.applicare     = proiectio_applicare;
    p.ctx           = doc;
    p.sigillare     = proiectio_sigillare;
    p.codificare    = proiectio_codificare;
    p.decodificare  = proiectio_decodificare;
    redde p;
}


/* ==================================================
 * Vita
 * ================================================== */

AgendaDocumentum*
agenda_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
                    i32  intervallum)
{
    AgendaDocumentum* doc;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    doc = (AgendaDocumentum*)piscina_allocare(piscina,
        magnitudo(AgendaDocumentum));
    si (!doc)
    {
        redde NIHIL;
    }
    memset(doc, ZEPHYRUM, magnitudo(AgendaDocumentum));
    doc->piscina  = piscina;
    doc->intern   = intern;
    doc->status = piscina_generare_dynamicum("agenda_status",
        LXIV * M);
    doc->scheda = piscina_generare_dynamicum("agenda_scheda",
        LXIV * M);
    doc->acta = piscina_generare_dynamicum("agenda_acta", XVI * M);
    si (!doc->status || !doc->scheda || !doc->acta)
    {
        redde NIHIL;
    }
    statum_vacare(doc);
    doc->historia = historia_aperire(piscina, intern, volumen,
        spatium ? spatium : "", genus_actorum, origo_checkpoint,
        intervallum, proiectio_facere(doc));
    si (!doc->historia)
    {
        redde NIHIL;
    }
    redde doc;
}


/* ==================================================
 * Acta typata
 * ================================================== */

interior s64
actum_mittere (
    AgendaDocumentum* doc,
           StmlNodus* n)
{
    si (!n)
    {
        redde ZEPHYRUM;
    }
    redde historia_actum(doc->historia, stml_scribere(n, doc->acta,
        FALSUM));
}

/* actum de re viva: elementum paratum aut NIHIL */
interior StmlNodus*
actum_rei (
      AgendaDocumentum* doc,
    constans character* actio,
                   s32  id)
{
    si (!doc || !vivum(doc, id))
    {
        redde NIHIL;
    }
    piscina_vacare(doc->acta);
    redde elementum_rei(doc->acta, doc->intern, actio, id);
}

s32
agenda_rem_creare (
    AgendaDocumentum* doc,
              chorda  titulus)
{
    StmlNodus* n;
          s32  id;

    si (!doc || !plenus(titulus))
    {
        redde ZEPHYRUM;
    }
    piscina_vacare(doc->acta);
    id  = doc->proximum;
    n   = elementum_rei(doc->acta, doc->intern, "nova", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    titulum_addere(n, doc->acta, doc->intern, titulus);
    redde actum_mittere(doc, n) > ZEPHYRUM ? id : ZEPHYRUM;
}

s64
agenda_titulum_ponere (
    AgendaDocumentum* doc,
                 s32  id,
              chorda  titulus)
{
    StmlNodus* n;

    si (!plenus(titulus))
    {
        redde ZEPHYRUM;
    }
    n = actum_rei(doc, "titulus", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    titulum_addere(n, doc->acta, doc->intern, titulus);
    redde actum_mittere(doc, n);
}

s64
agenda_diem_ponere (
    AgendaDocumentum* doc,
                 s32  id,
       constans Dies* dies)
{
    StmlNodus* n;

    si (dies && !fasti_dies_valida(*dies))
    {
        redde ZEPHYRUM;
    }
    n = actum_rei(doc, "dies", id);
    si (n && dies)
    {
        diem_ponere(n, doc->acta, doc->intern, "dies", *dies);
    }
    redde actum_mittere(doc, n);
}

s64
agenda_rubricas_ponere (
    AgendaDocumentum* doc,
                 s32  id,
              chorda  rubricae)
{
    StmlNodus* n;
          Xar* l;

    n = actum_rei(doc, "rubricae", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    l = rubricas_legere(rubricae, doc->acta);
    si (!l)
    {
        redde ZEPHYRUM;
    }
    attributum_ponere(n, doc->acta, doc->intern, "rubricae",
        rubricas_iungere(l, doc->acta));
    redde actum_mittere(doc, n);
}

s64
agenda_notam_addere (
    AgendaDocumentum* doc,
                 s32  id,
                Dies  dies,
              chorda  textus)
{
    StmlNodus* n;

    si (!plenus(textus) || !fasti_dies_valida(dies))
    {
        redde ZEPHYRUM;
    }
    n = actum_rei(doc, "nota", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    diem_ponere(n, doc->acta, doc->intern, "dies", dies);
    (vacuum)stml_liberum_addere(n, stml_textum_creare_ex_chorda(
        doc->acta, doc->intern, textus));
    redde actum_mittere(doc, n);
}

s64
agenda_perficere (
    AgendaDocumentum* doc,
                 s32  id,
                Dies  dies)
{
    StmlNodus* n;

    si (!fasti_dies_valida(dies))
    {
        redde ZEPHYRUM;
    }
    n = actum_rei(doc, "perfecta", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    diem_ponere(n, doc->acta, doc->intern, "dies", dies);
    redde actum_mittere(doc, n);
}

s64
agenda_perfectionem_tollere (
    AgendaDocumentum* doc,
                 s32  id)
{
    redde actum_mittere(doc, actum_rei(doc, "aperta", id));
}

s64
agenda_rem_delere (
    AgendaDocumentum* doc,
                 s32  id)
{
    redde actum_mittere(doc, actum_rei(doc, "deleta", id));
}


/* ==================================================
 * Historia
 * ================================================== */

b32
agenda_revocare (
    AgendaDocumentum* doc)
{
    redde doc && historia_revocare(doc->historia);
}

b32
agenda_reficere (
    AgendaDocumentum* doc)
{
    redde doc && historia_reficere(doc->historia);
}

b32
agenda_verificare (
    AgendaDocumentum* doc)
{
    redde doc && historia_verificare(doc->historia);
}

chorda
agenda_sigillum_hex (
    constans AgendaDocumentum* doc,
                      Piscina* piscina)
{
    redde historia_sigillum_hex(doc->historia, piscina);
}

Historia*
agenda_historia (
    AgendaDocumentum* doc)
{
    redde doc ? doc->historia : NIHIL;
}


/* ==================================================
 * Proiectio publica
 * ================================================== */

i32
agenda_numerus_rerum (
    constans AgendaDocumentum* doc)
{
    i32 i;
    i32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; doc && i < xar_numerus(doc->recorda); i++)
    {
        si (!((Recordum*)xar_obtinere(doc->recorda, i))->deleta)
        {
            n++;
        }
    }
    redde n;
}

constans AgendaRes*
agenda_res_index (
    constans AgendaDocumentum* doc,
                          i32  index)
{
    Recordum* r;
         i32  i;
         i32  n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; doc && i < xar_numerus(doc->recorda); i++)
    {
        r = (Recordum*)xar_obtinere(doc->recorda, i);
        si (r->deleta)
        {
            perge;
        }
        si (n == index)
        {
            redde &r->res;
        }
        n++;
    }
    redde NIHIL;
}

constans AgendaRes*
agenda_res (
    constans AgendaDocumentum* doc,
                          s32  id)
{
    Recordum* r;

    r = doc ? vivum(doc, id) : NIHIL;
    redde r ? &r->res : NIHIL;
}


/* ==================================================
 * Quaestiones (A1b)
 * ================================================== */

interior b32
dies_aequales (
    Dies a,
    Dies b)
{
    redde a.annus == b.annus && a.mensis == b.mensis
        && a.dies == b.dies;
}

/* res vivae quibus 'congruit' (perfectae: dies perfectionis; aliter:
 * dies assignata) == dies, ordine id */
interior Xar*
res_colligere (
    constans AgendaDocumentum* doc,
                         Dies  dies,
                          b32  perfectae,
                      Piscina* piscina)
{
          Xar* l;
     Recordum* r;
          i32  i;

    si (!doc || !piscina)
    {
        redde NIHIL;
    }
    l = xar_creare(piscina, (i32)magnitudo(constans AgendaRes*));
    per (i = ZEPHYRUM; l && i < xar_numerus(doc->recorda); i++)
    {
        r = (Recordum*)xar_obtinere(doc->recorda, i);
        si (r->deleta)
        {
            perge;
        }
        si (perfectae ? (r->res.perfecta && dies_aequales(
                r->res.dies_perfectionis, dies))
                      : (r->res.dies_posita
                          && dies_aequales(r->res.dies,
                dies)))
        {
            *(constans AgendaRes**)xar_addere(l) = &r->res;
        }
    }
    redde l;
}

Xar*
agenda_res_diei (
    constans AgendaDocumentum* doc,
                         Dies  dies,
                      Piscina* piscina)
{
    redde res_colligere(doc, dies, FALSUM, piscina);
}

Xar*
agenda_perfectae_die (
    constans AgendaDocumentum* doc,
                         Dies  dies,
                      Piscina* piscina)
{
    redde res_colligere(doc, dies, VERUM, piscina);
}

b32
agenda_signa_mensis (
    constans AgendaDocumentum* doc,
                          s32  annus,
                          s32  mensis,
                 AgendaSignum  signa[XXXI])
{
     Recordum* r;
          i32  i;

    si (!doc || !signa || mensis < I || mensis > XII)
    {
        redde FALSUM;
    }
    memset(signa, ZEPHYRUM, magnitudo(AgendaSignum) * XXXI);
    per (i = ZEPHYRUM; i < xar_numerus(doc->recorda); i++)
    {
        r = (Recordum*)xar_obtinere(doc->recorda, i);
        si (r->deleta)
        {
            perge;
        }
        /* dies validae (fasti_dies_valida in actis): 1..XXXI, mensis
         * sui longitudine */
        si (   r->res.dies_posita && r->res.dies.annus == annus
            && r->res.dies.mensis == mensis)
        {
            signa[r->res.dies.dies - I].assignatae++;
            si (!r->res.perfecta)
            {
                signa[r->res.dies.dies - I].apertae++;
            }
        }
        si (   r->res.perfecta
            && r->res.dies_perfectionis.annus  == annus
            && r->res.dies_perfectionis.mensis == mensis)
        {
            signa[r->res.dies_perfectionis.dies - I].perfectae++;
        }
    }
    redde VERUM;
}
