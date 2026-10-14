/* personae.c - Personae (include/personae.h; nomenclator-plan P1b).
 * Forma agendae (lib/agenda.c): piscinae 'status' (proiectio), 'scheda'
 * (actum unum, sigillum), 'acta' (actum novum). */

#include "personae.h"
#include "rubrica.h"
#include "stml.h"
#include "sigillum.h"

#include <stdio.h>
#include <string.h>

hic_manens constans character genus_actorum[]    = "personae";
hic_manens constans character origo_checkpoint[] =
    "personae:checkpoint";

nomen structura {
    Persona persona;
        b32 deleta;
} Recordum;

structura PersonaeDocumentum {
                Piscina* piscina;
    InternamentumChorda* intern;
               Historia* historia;
                Piscina* status;
                Piscina* scheda;
                Piscina* acta;
                    Xar* recorda;    /* Recordum, ordine id */
                    s32  proximum;
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior Recordum*
recordum (
    constans PersonaeDocumentum* doc,
                            s32  id)
{
    Recordum* r;
         i32  i;

    per (i = ZEPHYRUM; doc->recorda
        && i < xar_numerus(doc->recorda); i++)
    {
        r = (Recordum*)xar_obtinere(doc->recorda, i);
        si (r->persona.id == id)
        {
            redde r;
        }
    }
    redde NIHIL;
}

interior Recordum*
vivum (
    constans PersonaeDocumentum* doc,
                            s32  id)
{
    Recordum* r;

    r = recordum(doc, id);
    redde (r && !r->deleta) ? r : NIHIL;
}

interior b32
album_spatium (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* textus non vacuus (spatium album solum = vacuus) */
interior b32
plenus (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (!album_spatium(c.datum[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* sine spatio albo initio et fine (frustum, non copia) */
interior chorda
tondere (
    chorda c)
{
    dum (c.mensura > ZEPHYRUM && album_spatium(c.datum[ZEPHYRUM]))
    {
        c.datum++;
        c.mensura--;
    }
    dum (c.mensura > ZEPHYRUM && album_spatium(c.datum[c.mensura - I]))
    {
        c.mensura--;
    }
    redde c;
}

interior chorda
vacua (
    vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

/* natalis validus: cum anno fasti_dies_valida; sine anno (annus 0)
 * mensis et dies anni bissextilis (29 II licet) */
interior b32
natalis_validus (
    Dies d)
{
    si (d.annus == ZEPHYRUM)
    {
        redde d.mensis >= I && d.mensis <= XII && d.dies >= I
            && d.dies <= fasti_dies_in_mense(d.mensis, MM);
    }
    redde fasti_dies_valida(d);
}

/* "YYYY-MM-DD" aut "--MM-DD" */
interior b32
natalem_legere (
    chorda* c,
      Dies* d)
{
    i32 i;

    si (!c || c->mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (   c->mensura  == VII && c->datum[ZEPHYRUM] == '-'
        && c->datum[I] == '-' && c->datum[IV] == '-')
    {
        per (i = II; i < VII; i++)
        {
            si (i != IV && (c->datum[i] < '0' || c->datum[i] > '9'))
            {
                redde FALSUM;
            }
        }
        d->annus   = ZEPHYRUM;
        d->mensis  = (s32)(c->datum[II] - '0') * X + (s32)(c->datum[III]
            - '0');
        d->dies    = (s32)(c->datum[V] - '0') * X + (s32)(c->datum[VI]
            - '0');
        redde natalis_validus(*d);
    }
    redde fasti_parsare_iso(*c, d) && d->annus > ZEPHYRUM
        && natalis_validus(*d);
}

interior chorda
natalem_formare (
        Dies  d,
     Piscina* p)
{
    character s[XVI];

    si (d.annus == ZEPHYRUM)
    {
        sprintf(s, "--%02d-%02d", (integer)d.mensis, (integer)d.dies);
        redde chorda_ex_literis(s, p);
    }
    redde fasti_formare_diem(d, FASTI_FORMA_ISO, p);
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

/* nomen forma elementi attributi '<@titulus=>...</>' (quota duplex
 * repraesentabilis); vacuum non scribitur */
interior vacuum
nomen_addere (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* titulus,
                 chorda  valor)
{
    StmlNodus* a;

    si (valor.mensura == ZEPHYRUM)
    {
        redde;
    }
    a = stml_elementum_creare(p, in, "@");
    si (!a)
    {
        redde;
    }
    a->attributum_titulus = chorda_internare_ex_literis(in, titulus);
    (vacuum)stml_liberum_addere(a, stml_textum_creare_ex_chorda(p, in,
        valor));
    (vacuum)stml_liberum_addere(n, a);
}

/* index (Xar de chorda) ut liberi <nomen><@valor=>...</></nomen>
 * (forma elementi attributi: '"' in numero aut epistula
 * repraesentabilis - attributum simplex eum perderet) */
interior vacuum
indicem_addere (
              StmlNodus* n,
                Piscina* p,
    InternamentumChorda* in,
     constans character* nomen_liberi,
           constans Xar* l)
{
    StmlNodus* e;
          i32  i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (!plenus(*(chorda*)xar_obtinere(l, i)))
        {
            perge;
        }
        e = stml_elementum_creare(p, in, nomen_liberi);
        nomen_addere(e, p, in, "valor", *(chorda*)xar_obtinere(l, i));
        (vacuum)stml_liberum_addere(n, e);
    }
}

interior StmlNodus*
elementum_personae (
                Piscina* p,
    InternamentumChorda* in,
     constans character* actio,
                    s32  id)
{
    StmlNodus* n;

    n = stml_elementum_creare(p, in, "persona");
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

interior constans character*
genus_nomen (
    PersonaGenus g)
{
    redde g == PERSONA_SOCIETAS ? "societas" : "homo";
}

/* "homo" / "societas"; FALSUM si aliud */
interior b32
genus_legere (
          chorda* c,
    PersonaGenus* g)
{
    si (c && chorda_aequalis_literis(*c, "homo"))
    {
        *g = PERSONA_HOMO;
        redde VERUM;
    }
    si (c && chorda_aequalis_literis(*c, "societas"))
    {
        *g = PERSONA_SOCIETAS;
        redde VERUM;
    }
    redde FALSUM;
}


/* ==================================================
 * Proiectio (historia L1)
 * ================================================== */

interior vacuum
statum_vacare (
    PersonaeDocumentum* doc)
{
    piscina_vacare(doc->status);
    doc->recorda   = xar_creare(doc->status, (i32)magnitudo(Recordum));
    doc->proximum  = I;
}

interior Recordum*
recordum_addere (
    PersonaeDocumentum* doc,
                   s32  id,
          PersonaGenus  genus)
{
    Recordum* r;

    r = (Recordum*)xar_addere(doc->recorda);
    memset(r, ZEPHYRUM, magnitudo(Recordum));
    r->persona.id         = id;
    r->persona.genus      = genus;
    r->persona.praenomen  = vacua();
    r->persona.cognomen   = vacua();
    r->persona.titulus    = vacua();
    r->persona.rubricae   = xar_creare(doc->status,
        (i32)magnitudo(chorda));
    r->persona.telephona  = xar_creare(doc->status,
        (i32)magnitudo(chorda));
    r->persona.epistulae  = xar_creare(doc->status,
        (i32)magnitudo(chorda));
    si (id >= doc->proximum)
    {
        doc->proximum = id + I;
    }
    redde r;
}

/* nomen ex attributo (absens = vacuum), in piscinam status */
interior chorda
nomen_legere (
    PersonaeDocumentum* doc,
             StmlNodus* n,
    constans character* titulus)
{
    chorda* a;

    a = stml_attributum_capere(n, titulus);
    redde a ? chorda_transcribere(*a, doc->status) : vacua();
}

interior b32
rubricas_ponere (
    PersonaeDocumentum* doc,
              Recordum* r,
                chorda* textus)
{
    Xar* l;
    i32  i;

    l = rubricas_legere(textus ? *textus : vacua(), doc->scheda);
    si (!l)
    {
        redde FALSUM;
    }
    r->persona.rubricae = xar_creare(doc->status,
        (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        *(chorda*)xar_addere(r->persona.rubricae) = chorda_transcribere(
            *(chorda*)xar_obtinere(l, i), doc->status);
    }
    redde VERUM;
}

/* index ex liberis <nomen_liberi><@valor=>..</></> in piscinam
 * status */
interior Xar*
indicem_legere (
    PersonaeDocumentum* doc,
             StmlNodus* n,
    constans character* nomen_liberi)
{
          Xar* l;
    StmlNodus* e;
       chorda* v;
          i32  i;

    l = xar_creare(doc->status, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; l && i < stml_numerus_liberorum(n); i++)
    {
        e = stml_liberum_ad_indicem(n, i);
        si (   e->genus != STML_NODUS_ELEMENTUM || !e->titulus
            || !chorda_aequalis_literis(*e->titulus, nomen_liberi))
        {
            perge;
        }
        v = stml_attributum_capere(e, "valor");
        si (v && plenus(*v))
        {
            *(chorda*)xar_addere(l) = chorda_transcribere(*v,
                doc->status);
        }
    }
    redde l;
}

interior vacuum
proiectio_vacare (
    vacuum* ctx)
{
    statum_vacare((PersonaeDocumentum*)ctx);
}

/* actum unum; malum aut ignotum praeteritur (replicatio robusta) */
interior vacuum
proiectio_applicare (
    vacuum* ctx,
    chorda  actum)
{
    PersonaeDocumentum* doc;
          StmlResultus  res;
             StmlNodus* n;
                chorda* actio;
                chorda* t;
              Recordum* r;
                   s32  id;
          PersonaGenus  genus;
                  Dies  d;
                chorda  pn;
                chorda  cn;

    doc = (PersonaeDocumentum*)ctx;
    piscina_vacare(doc->scheda);
    res = stml_legere_ex_literis(chorda_ut_cstr(actum, doc->scheda),
        doc->scheda, doc->intern);
    n = res.elementum_radix;
    si (   !res.successus || !n || !n->titulus
        || !chorda_aequalis_literis(*n->titulus, "persona"))
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
        si (   !recordum(doc, id)
            && genus_legere(stml_attributum_capere(n, "genus"), &genus))
        {
            r                     = recordum_addere(doc, id, genus);
            r->persona.praenomen  = nomen_legere(doc, n, "praenomen");
            r->persona.cognomen   = nomen_legere(doc, n, "cognomen");
            r->persona.titulus    = nomen_legere(doc, n, "titulus");
        }
        redde;
    }
    r = vivum(doc, id);
    si (!r)
    {
        redde;
    }
    si (chorda_aequalis_literis(*actio, "nomen"))
    {
        pn = nomen_legere(doc, n, "praenomen");
        cn = nomen_legere(doc, n, "cognomen");
        si (plenus(pn) || plenus(cn))
        {
            r->persona.praenomen  = pn;
            r->persona.cognomen   = cn;
        }
    }
    alioquin si (chorda_aequalis_literis(*actio, "titulus"))
    {
        t = stml_attributum_capere(n, "titulus");
        si (t && plenus(*t))
        {
            r->persona.titulus = chorda_transcribere(*t, doc->status);
        }
    }
    alioquin si (chorda_aequalis_literis(*actio, "genus"))
    {
        si (genus_legere(stml_attributum_capere(n, "genus"), &genus))
        {
            r->persona.genus = genus;
        }
    }
    alioquin si (chorda_aequalis_literis(*actio, "rubricae"))
    {
        (vacuum)rubricas_ponere(doc, r, stml_attributum_capere(n,
            "rubricae"));
    }
    alioquin si (chorda_aequalis_literis(*actio, "telephona"))
    {
        r->persona.telephona = indicem_legere(doc, n, "telephonum");
    }
    alioquin si (chorda_aequalis_literis(*actio, "epistulae"))
    {
        r->persona.epistulae = indicem_legere(doc, n, "epistula");
    }
    alioquin si (chorda_aequalis_literis(*actio, "natalis"))
    {
        t = stml_attributum_capere(n, "dies");
        si (!t)
        {
            r->persona.natalis_positus = FALSUM;
        }
        alioquin si (natalem_legere(t, &d))
        {
            r->persona.natalis_positus  = VERUM;
            r->persona.natalis          = d;
        }
    }
    alioquin si (chorda_aequalis_literis(*actio, "deleta"))
    {
        r->deleta = VERUM;
    }
}

/* status canonicus: <personae proximum><persona id genus [rubricae]
 * [natalis]><@praenomen/><@cognomen/><@titulus/><telephonum><@valor/>
 * </telephonum>..<epistula><@valor/></epistula>..</persona>..
 * </personae>; deletae absunt */
interior chorda
proiectio_codificare (
     vacuum* ctx,
    Piscina* piscina)
{
    PersonaeDocumentum* doc;
             StmlNodus* radix;
             StmlNodus* e;
              Recordum* r;
                   i32  i;

    doc    = (PersonaeDocumentum*)ctx;
    radix  = stml_elementum_creare(piscina, doc->intern, "personae");
    attributum_ponere(radix, piscina, doc->intern, "proximum",
        chorda_ex_s32(doc->proximum, piscina));
    per (i = ZEPHYRUM; i < xar_numerus(doc->recorda); i++)
    {
        r = (Recordum*)xar_obtinere(doc->recorda, i);
        si (r->deleta)
        {
            perge;
        }
        e = elementum_personae(piscina, doc->intern, NIHIL,
            r->persona.id);
        (vacuum)stml_attributum_addere(e, piscina, doc->intern, "genus",
            genus_nomen(r->persona.genus));
        attributum_ponere(e, piscina, doc->intern, "rubricae",
            rubricas_iungere(r->persona.rubricae, piscina));
        si (r->persona.natalis_positus)
        {
            attributum_ponere(e, piscina, doc->intern, "natalis",
                natalem_formare(r->persona.natalis, piscina));
        }
        nomen_addere(e, piscina, doc->intern, "praenomen",
            r->persona.praenomen);
        nomen_addere(e, piscina, doc->intern, "cognomen",
            r->persona.cognomen);
        nomen_addere(e, piscina, doc->intern, "titulus",
            r->persona.titulus);
        indicem_addere(e, piscina, doc->intern, "telephonum",
            r->persona.telephona);
        indicem_addere(e, piscina, doc->intern, "epistula",
            r->persona.epistulae);
        (vacuum)stml_liberum_addere(radix, e);
    }
    redde stml_scribere(radix, piscina, FALSUM);
}

interior vacuum
proiectio_sigillare (
      vacuum* ctx,
    Sigillum* exitus)
{
    PersonaeDocumentum* doc;
     SigillumContextus  sc;
                chorda  textus;

    doc = (PersonaeDocumentum*)ctx;
    piscina_vacare(doc->scheda);
    textus = proiectio_codificare(doc, doc->scheda);
    sigillum_incipere(&sc);
    sigillum_addere(&sc, textus.datum, (memoriae_index)textus.mensura);
    *exitus = sigillum_finire(&sc);
}

/* restitutio ex statu canonico; FALSUM = codex inutilis */
interior b32
proiectio_decodificare (
    vacuum* ctx,
    chorda  codex)
{
    PersonaeDocumentum* doc;
          StmlResultus  res;
             StmlNodus* radix;
             StmlNodus* e;
              Recordum* r;
                chorda* t;
                   s32  proximum;
          PersonaGenus  genus;
                   i32  i;
                  Dies  d;

    doc = (PersonaeDocumentum*)ctx;
    piscina_vacare(doc->scheda);
    res = stml_legere_ex_literis(chorda_ut_cstr(codex, doc->scheda),
        doc->scheda, doc->intern);
    radix = res.elementum_radix;
    si (   !res.successus || !radix || !radix->titulus
        || !chorda_aequalis_literis(*radix->titulus, "personae"))
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
            || !chorda_aequalis_literis(*e->titulus, "persona"))
        {
            perge;
        }
        si (   attributum_id(e) <= ZEPHYRUM
            || !genus_legere(stml_attributum_capere(e, "genus"),
            &genus))
        {
            redde FALSUM;
        }
        r                     = recordum_addere(doc, attributum_id(e),
            genus);
        r->persona.praenomen  = nomen_legere(doc, e, "praenomen");
        r->persona.cognomen   = nomen_legere(doc, e, "cognomen");
        r->persona.titulus    = nomen_legere(doc, e, "titulus");
        si (!rubricas_ponere(doc, r, stml_attributum_capere(e,
                "rubricae")))
        {
            redde FALSUM;
        }
        t = stml_attributum_capere(e, "natalis");
        si (t)
        {
            si (!natalem_legere(t, &d))
            {
                redde FALSUM;
            }
            r->persona.natalis_positus  = VERUM;
            r->persona.natalis          = d;
        }
        r->persona.telephona = indicem_legere(doc, e, "telephonum");
        r->persona.epistulae = indicem_legere(doc, e, "epistula");
    }
    doc->proximum = proximum;
    redde VERUM;
}

interior HistoriaProiectio
proiectio_facere (
    PersonaeDocumentum* doc)
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

PersonaeDocumentum*
personae_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
                    i32  intervallum)
{
    PersonaeDocumentum* doc;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    doc = (PersonaeDocumentum*)piscina_allocare(piscina,
        magnitudo(PersonaeDocumentum));
    si (!doc)
    {
        redde NIHIL;
    }
    memset(doc, ZEPHYRUM, magnitudo(PersonaeDocumentum));
    doc->piscina  = piscina;
    doc->intern   = intern;
    doc->status   = piscina_generare_dynamicum("personae_status",
        LXIV * M);
    doc->scheda   = piscina_generare_dynamicum("personae_scheda",
        LXIV * M);
    doc->acta     = piscina_generare_dynamicum("personae_acta",
        XVI * M);
    si (!doc->status || !doc->scheda || !doc->acta)
    {
        redde NIHIL;
    }
    statum_vacare(doc);
    doc->historia = historia_aperire(piscina, intern, volumen,
        spatium ? spatium : "", genus_actorum, origo_checkpoint,
        intervallum, proiectio_facere(doc));
    redde doc->historia ? doc : NIHIL;
}


/* ==================================================
 * Acta typata
 * ================================================== */

interior s64
actum_mittere (
    PersonaeDocumentum* doc,
             StmlNodus* n)
{
    si (!n)
    {
        redde ZEPHYRUM;
    }
    redde historia_actum(doc->historia, stml_scribere(n, doc->acta,
        FALSUM));
}

interior StmlNodus*
actum_personae (
    PersonaeDocumentum* doc,
    constans character* actio,
                   s32  id)
{
    si (!doc || !vivum(doc, id))
    {
        redde NIHIL;
    }
    piscina_vacare(doc->acta);
    redde elementum_personae(doc->acta, doc->intern, actio, id);
}

interior s32
personam_creare (
    PersonaeDocumentum* doc,
          PersonaGenus  genus,
                chorda  praenomen,
                chorda  cognomen,
                chorda  titulus)
{
    StmlNodus* n;
          s32  id;

    piscina_vacare(doc->acta);
    id  = doc->proximum;
    n   = elementum_personae(doc->acta, doc->intern, "nova", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    (vacuum)stml_attributum_addere(n, doc->acta, doc->intern, "genus",
        genus_nomen(genus));
    nomen_addere(n, doc->acta, doc->intern, "praenomen", praenomen);
    nomen_addere(n, doc->acta, doc->intern, "cognomen", cognomen);
    nomen_addere(n, doc->acta, doc->intern, "titulus", titulus);
    redde actum_mittere(doc, n) > ZEPHYRUM ? id : ZEPHYRUM;
}

s32
personae_hominem_creare (
    PersonaeDocumentum* doc,
                chorda  praenomen,
                chorda  cognomen)
{
    si (!doc || !(plenus(praenomen) || plenus(cognomen)))
    {
        redde ZEPHYRUM;
    }
    redde personam_creare(doc, PERSONA_HOMO, praenomen, cognomen,
        vacua());
}

s32
personae_societatem_creare (
    PersonaeDocumentum* doc,
                chorda  titulus)
{
    si (!doc || !plenus(titulus))
    {
        redde ZEPHYRUM;
    }
    redde personam_creare(doc, PERSONA_SOCIETAS, vacua(), vacua(),
        titulus);
}

s64
personae_nomen_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
                chorda  praenomen,
                chorda  cognomen)
{
    StmlNodus* n;

    si (!(plenus(praenomen) || plenus(cognomen)))
    {
        redde ZEPHYRUM;
    }
    n = actum_personae(doc, "nomen", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    nomen_addere(n, doc->acta, doc->intern, "praenomen", praenomen);
    nomen_addere(n, doc->acta, doc->intern, "cognomen", cognomen);
    redde actum_mittere(doc, n);
}

s64
personae_titulum_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
                chorda  titulus)
{
    StmlNodus* n;

    si (!plenus(titulus))
    {
        redde ZEPHYRUM;
    }
    n = actum_personae(doc, "titulus", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    nomen_addere(n, doc->acta, doc->intern, "titulus", titulus);
    redde actum_mittere(doc, n);
}

s64
personae_genus_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
          PersonaGenus  genus)
{
    StmlNodus* n;

    si (genus != PERSONA_HOMO && genus != PERSONA_SOCIETAS)
    {
        redde ZEPHYRUM;
    }
    n = actum_personae(doc, "genus", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    (vacuum)stml_attributum_addere(n, doc->acta, doc->intern, "genus",
        genus_nomen(genus));
    redde actum_mittere(doc, n);
}

s64
personae_rubricas_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
                chorda  rubricae)
{
    StmlNodus* n;
          Xar* l;

    n = actum_personae(doc, "rubricae", id);
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
personae_telephona_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
          constans Xar* telephona)
{
    StmlNodus* n;

    n = actum_personae(doc, "telephona", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    indicem_addere(n, doc->acta, doc->intern, "telephonum", telephona);
    redde actum_mittere(doc, n);
}

s64
personae_epistulas_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
          constans Xar* epistulae)
{
    StmlNodus* n;

    n = actum_personae(doc, "epistulae", id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    indicem_addere(n, doc->acta, doc->intern, "epistula", epistulae);
    redde actum_mittere(doc, n);
}

s64
personae_natalem_ponere (
    PersonaeDocumentum* doc,
                   s32  id,
         constans Dies* natalis)
{
    StmlNodus* n;

    si (natalis && !natalis_validus(*natalis))
    {
        redde ZEPHYRUM;
    }
    n = actum_personae(doc, "natalis", id);
    si (n && natalis)
    {
        attributum_ponere(n, doc->acta, doc->intern, "dies",
            natalem_formare(*natalis, doc->acta));
    }
    redde actum_mittere(doc, n);
}

s64
personae_delere (
    PersonaeDocumentum* doc,
                   s32  id)
{
    redde actum_mittere(doc, actum_personae(doc, "deleta", id));
}


/* ==================================================
 * Historia
 * ================================================== */

b32
personae_revocare (
    PersonaeDocumentum* doc)
{
    redde doc && historia_revocare(doc->historia);
}

b32
personae_reficere (
    PersonaeDocumentum* doc)
{
    redde doc && historia_reficere(doc->historia);
}

b32
personae_verificare (
    PersonaeDocumentum* doc)
{
    redde doc && historia_verificare(doc->historia);
}

chorda
personae_sigillum_hex (
    constans PersonaeDocumentum* doc,
                        Piscina* piscina)
{
    redde historia_sigillum_hex(doc->historia, piscina);
}

Historia*
personae_historia (
    PersonaeDocumentum* doc)
{
    redde doc ? doc->historia : NIHIL;
}


/* ==================================================
 * Proiectio publica
 * ================================================== */

i32
personae_numerus (
    constans PersonaeDocumentum* doc)
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

constans Persona*
personae_index (
    constans PersonaeDocumentum* doc,
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
            redde &r->persona;
        }
        n++;
    }
    redde NIHIL;
}

constans Persona*
personae_persona (
    constans PersonaeDocumentum* doc,
                            s32  id)
{
    Recordum* r;

    r = doc ? vivum(doc, id) : NIHIL;
    redde r ? &r->persona : NIHIL;
}


/* ==================================================
 * Nomina, clavis, aetas
 * ================================================== */

/* a + " " + b (partes vacuae omissae, tonsae) */
interior chorda
iungere (
     chorda  a,
     chorda  b,
    Piscina* p)
{
    a = tondere(a);
    b = tondere(b);
    si (a.mensura == ZEPHYRUM)
    {
        redde chorda_transcribere(b, p);
    }
    si (b.mensura == ZEPHYRUM)
    {
        redde chorda_transcribere(a, p);
    }
    redde chorda_concatenare(a,
        chorda_concatenare(chorda_ex_literis(" ",
        p), b, p), p);
}

interior b32
nomina_hominis_plena (
    constans Persona* p)
{
    redde plenus(p->praenomen) || plenus(p->cognomen);
}

chorda
persona_nomen (
    constans Persona* p,
             Piscina* piscina)
{
    si (!p)
    {
        redde vacua();
    }
    si (p->genus == PERSONA_SOCIETAS ? plenus(p->titulus)
                                     : !nomina_hominis_plena(p))
    {
        redde chorda_transcribere(tondere(p->titulus), piscina);
    }
    redde iungere(p->praenomen, p->cognomen, piscina);
}

/* clavis sine minusculis: societas titulus, homo "cognomen praenomen",
 * alterum si vacuum */
interior chorda
clavis_cruda (
    constans Persona* p,
             Piscina* piscina)
{
    si (p->genus == PERSONA_SOCIETAS ? plenus(p->titulus)
                                     : !nomina_hominis_plena(p))
    {
        redde chorda_transcribere(tondere(p->titulus), piscina);
    }
    redde iungere(p->cognomen, p->praenomen, piscina);
}

chorda
persona_clavis (
    constans Persona* p,
             Piscina* piscina)
{
    chorda c;
       i32 i;

    si (!p)
    {
        redde vacua();
    }
    c = clavis_cruda(p, piscina);
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] >= 'A' && c.datum[i] <= 'Z')
        {
            c.datum[i] = (i8)(c.datum[i] - 'A' + 'a');
        }
    }
    redde c;
}

character
persona_littera (
    constans Persona* p)
{
    chorda c;
        i8 x;

    si (!p)
    {
        redde '#';
    }
    c = (p->genus == PERSONA_SOCIETAS ? plenus(p->titulus)
                                      : !nomina_hominis_plena(p))
        ? tondere(p->titulus)
        : tondere(plenus(p->cognomen) ? p->cognomen : p->praenomen);
    si (c.mensura == ZEPHYRUM)
    {
        redde '#';
    }
    x = c.datum[ZEPHYRUM];
    si (x >= 'a' && x <= 'z')
    {
        redde (character)(x - 'a' + 'A');
    }
    redde (x >= 'A' && x <= 'Z') ? (character)x : '#';
}

s32
persona_aetas (
    constans Persona* p,
                Dies  hodie)
{
    s32 a;

    si (!p || !p->natalis_positus || p->natalis.annus == ZEPHYRUM)
    {
        redde -I;
    }
    a = hodie.annus - p->natalis.annus;
    si (   hodie.mensis < p->natalis.mensis
        || (   hodie.mensis == p->natalis.mensis
            && hodie.dies < p->natalis.dies))
    {
        a--;
    }
    redde a < ZEPHYRUM ? -I : a;
}


/* ==================================================
 * Quaestiones
 * ================================================== */

nomen structura {
     constans Persona* p;
               chorda  clavis;
} PersonaClavis;

Xar*
personae_ordinatae (
    constans PersonaeDocumentum* doc,
                        Piscina* piscina)
{
              Xar* claves;
              Xar* l;
         Recordum* r;
    PersonaClavis  k;
              i32  i;
              i32  j;

    si (!doc || !piscina)
    {
        redde NIHIL;
    }
    claves  = xar_creare(piscina, (i32)magnitudo(PersonaClavis));
    l       = xar_creare(piscina, (i32)magnitudo(constans Persona*));
    per (i = ZEPHYRUM; claves && l
        && i < xar_numerus(doc->recorda); i++)
    {
        r = (Recordum*)xar_obtinere(doc->recorda, i);
        si (r->deleta)
        {
            perge;
        }
        k.p       = &r->persona;
        k.clavis  = persona_clavis(&r->persona, piscina);
        /* insertio stabilis (ordo id inter claves aequales) */
        (vacuum)xar_addere(claves);
        j = xar_numerus(claves) - I;
        dum (   j > ZEPHYRUM
             && rubricas_comparare(k.clavis, ((PersonaClavis*)
                xar_obtinere(claves, j - I))->clavis) < ZEPHYRUM)
        {
            *(PersonaClavis*)xar_obtinere(claves, j) =
                *(PersonaClavis*)xar_obtinere(claves, j - I);
            j--;
        }
        *(PersonaClavis*)xar_obtinere(claves, j) = k;
    }
    per (i = ZEPHYRUM; claves && l && i < xar_numerus(claves); i++)
    {
        *(constans Persona**)xar_addere(l) = ((PersonaClavis*)
            xar_obtinere(claves, i))->p;
    }
    redde l;
}

Xar*
personae_rubricae (
    constans PersonaeDocumentum* doc,
                        Piscina* piscina)
{
      Recordum* r;
        chorda  omnes;
           i32  i;

    si (!doc || !piscina)
    {
        redde NIHIL;
    }
    omnes = vacua();
    per (i = ZEPHYRUM; i < xar_numerus(doc->recorda); i++)
    {
        r = (Recordum*)xar_obtinere(doc->recorda, i);
        si (!r->deleta && xar_numerus(r->persona.rubricae) > ZEPHYRUM)
        {
            omnes = chorda_concatenare(omnes, chorda_concatenare(
                chorda_ex_literis(" ", piscina), rubricas_iungere(
                r->persona.rubricae, piscina), piscina), piscina);
        }
    }
    redde rubricas_legere(omnes, piscina);
}
