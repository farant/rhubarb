/* kalendarium.c - calendarium, pagina mensis (include/kalendarium.h;
 * agenda-plan A2a). Scriptura status per portam rami, scriptore
 * calendarium.agere (vocantibus extra dispensatorem scriptor ponitur et
 * restituitur - aliter domini scripturam tacite recusant). */

#include "kalendarium.h"
#include "calendarium_liturgicum.h"
#include "color.h"
#include "mandatum.h"
#include "thema.h"
#include "scriba_documentum.h"   /* scriba_effugere / scriba_solvere */
#include "tabula_characterum.h"
#include "vim.h"

#include <stdio.h>
#include <string.h>

hic_manens constans character actio_calendarii[] = "calendarium.agere";

#define KALENDARIUM_TITULI_ALTITUDO  XVI
#define KALENDARIUM_LATITUDO         CCLXXX   /* superficies absens */
#define KALENDARIUM_ALTITUDO         CCLVI
#define KALENDARIUM_LITTERA          VI       /* latitudo glyphi */
#define KALENDARIUM_LINEA            VIII     /* altitudo glyphi */
#define KALENDARIUM_SIGNA_MAXIMA     IV
#define KALENDARIUM_TITULI_DIEI      XXVIII   /* lineae III + margo */
#define KALENDARIUM_PROMPTA          CCLVI    /* octeti promptae */
#define KALENDARIUM_CAPSA            XXIV     /* ictus capsae: x < */
#define KALENDARIUM_MARGO_X          VI       /* mensa: cellula una, */
#define KALENDARIUM_MARGO_Y          VIII     /* ut scriba et pictor */

/* contextus figurarum (privatus; figurae_ctx): liturgia semel creata */
nomen structura {
    KalendariumMontatio* m;
  CalendariumLiturgicum* liturgia;
                Piscina* scheda;     /* A3: actio una (vacatur) */
} KalendariumFigurae;


/* ==================================================
 * Auxilia
 * ================================================== */

interior Dies
hodie (
    constans KalendariumMontatio* m)
{
    Dies d;

    si (m->hodie && m->hodie(m->hodie_ctx, &d) && fasti_dies_valida(d))
    {
        redde d;
    }
    redde fasti_dies(MMXXVI, I, I);
}

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, titulus);
    si (a && chorda_ut_s32(*a, &v) && v > ZEPHYRUM)
    {
        redde v;
    }
    redde praestitutum;
}

/* "YYYY-MM" -> annus, mensis; FALSUM si malum */
interior b32
mensem_legere (
    constans character* argumentum,
                   s32* annus,
                   s32* mensis)
{
    i32 i;

    si (   !argumentum || strlen(argumentum) != VII
        || argumentum[IV] != '-')
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < VII; i++)
    {
        si (i != IV && (argumentum[i] < '0' || argumentum[i] > '9'))
        {
            redde FALSUM;
        }
    }
    *annus   = (s32)((argumentum[ZEPHYRUM] - '0') * M
             + (argumentum[I] - '0') * C + (argumentum[II] - '0') * X
             + (argumentum[III] - '0'));
    *mensis  = (s32)((argumentum[V] - '0') * X + (argumentum[VI]
        - '0'));
    redde *mensis >= I && *mensis <= XII;
}

/* dies argumenti: hodie si in eo mense, aliter dies I */
interior Dies
dies_argumenti (
    constans KalendariumMontatio* m,
                             s32  annus,
                             s32  mensis)
{
    Dies h;

    h = hodie(m);
    si (h.annus == annus && h.mensis == mensis)
    {
        redde h;
    }
    redde fasti_dies(annus, mensis, I);
}

/* par campi pro mutatore */
nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

interior vacuum
campum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParCampi* par;

    par = (ParCampi*)ctx;
    si (!par->valor || !par->valor[ZEPHYRUM])
    {
        /* "" internari non potest: vacuum = tollere */
        (vacuum)insula_attributum_tollere(radix, par->titulus);
        redde;
    }
    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

/* campi plures scriptura UNA */
nomen structura {
              ParCampi* pares;
                   i32  numerus;
} GrexCamporum;

interior vacuum
campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    GrexCamporum* grex;
             i32  i;

    grex = (GrexCamporum*)ctx;
    per (i = ZEPHYRUM; i < grex->numerus; i++)
    {
        campum_ponere(radix, p, in, (vacuum*)(grex->pares + i));
    }
}

/* campum status scribere, scriptore calendarii (prior restituitur -
 * vocantes extra dispensatorem aliter a dominis tacite recusantur) */
interior b32
campum_scribere (
    KalendariumMontatio* m,
     constans character* titulus,
     constans character* valor)
{
    ParCampi par;
      chorda prior;
         b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    prior        = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_calendarii, m->piscina));
    ok = mutare_ramum(&m->ramus, campum_ponere, &par);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

/* dies_electus scribere */
interior b32
diem_scribere (
    KalendariumMontatio* m,
                   Dies  dies)
{
    character iso[XVI];
       chorda textus;

    textus = fasti_formare_diem(dies, FASTI_FORMA_ISO, m->piscina);
    si (textus.mensura <= ZEPHYRUM || textus.mensura >= XVI)
    {
        redde FALSUM;
    }
    memcpy(iso, textus.datum, (size_t)textus.mensura);
    iso[textus.mensura] = '\0';
    redde campum_scribere(m, "dies_electus", iso);
}

interior b32
visus_diei (
    constans KalendariumMontatio* m)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, "visus");
    redde a && chorda_aequalis_literis(*a, "dies");
}

interior b32
dies_aequales (
    Dies a,
    Dies b)
{
    redde a.annus == b.annus && a.mensis == b.mensis
        && a.dies == b.dies;
}

/* ordines visus diei: res diei assignatae, deinde res eo die perfectae
 * sed alibi assignatae (Xar de constans AgendaRes*) */
interior Xar*
ordines_diei (
     constans KalendariumMontatio* m,
                             Dies  dies,
                          Piscina* piscina)
{
                   Xar* l;
                   Xar* perfectae;
    constans AgendaRes* r;
                   i32  i;

    si (!m->documentum)
    {
        redde xar_creare(piscina, (i32)magnitudo(constans AgendaRes*));
    }
    l          = agenda_res_diei(m->documentum, dies, piscina);
    perfectae  = agenda_perfectae_die(m->documentum, dies, piscina);
    per (i = ZEPHYRUM; l && perfectae
        && i < xar_numerus(perfectae); i++)
    {
        r = *(constans AgendaRes**)xar_obtinere(perfectae, i);
        si (!r->dies_posita || !dies_aequales(r->dies, dies))
        {
            *(constans AgendaRes**)xar_addere(l) = r;
        }
    }
    redde l;
}

interior i32
index_rei (
    Xar* l,
    s32  id)
{
    i32 i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si ((*(constans AgendaRes**)xar_obtinere(l, i))->id == id)
        {
            redde (i32)i;
        }
    }
    redde (i32)-I;
}

/* res electa: campus res_electa si in ordinibus, aliter prima; 0 si
 * nulla */
interior s32
res_electa (
    constans KalendariumMontatio* m,
                             Xar* l)
{
    chorda* a;
       s32  id;

    a = insula_ramus_attributum(&m->ramus, "res_electa");
    si (a && chorda_ut_s32(*a, &id) && index_rei(l, id) != (i32)-I)
    {
        redde id;
    }
    redde (l && xar_numerus(l) > ZEPHYRUM)
        ? (*(constans AgendaRes**)xar_obtinere(l, ZEPHYRUM))->id
        : ZEPHYRUM;
}

interior b32
electionem_scribere (
    KalendariumMontatio* m,
                    s32  id)
{
    character numerus[XVI];

    sprintf(numerus, "%d", (integer)id);
    redde campum_scribere(m, "res_electa", numerus);
}

interior b32
visus_agendae (
    constans KalendariumMontatio* m)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, "visus");
    redde a && chorda_aequalis_literis(*a, "agenda");
}

/* grex visus agendae: rubrica (vacua = sine rubrica) et res apertae
 * eius, ordine diei (sine die ultimae) */
nomen structura {
    chorda  rubrica;
       Xar* res;   /* constans AgendaRes* */
} GrexAgendae;

interior b32
rubricam_habet (
    constans AgendaRes* r,
                chorda  rubrica)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(r->rubricae); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(r->rubricae, i),
                rubrica))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* a ante b in grege: datae ordine diei, deinde sine die; id */
interior b32
ante_in_agenda (
    constans AgendaRes* a,
    constans AgendaRes* b)
{
    s32 c;

    si (a->dies_posita != b->dies_posita)
    {
        redde a->dies_posita;
    }
    si (a->dies_posita)
    {
        c = fasti_comparare(a->dies, b->dies);
        si (c != ZEPHYRUM)
        {
            redde c < ZEPHYRUM;
        }
    }
    redde a->id < b->id;
}

/* res in gregem ordine inserere */
interior vacuum
in_gregem_inserere (
                   Xar* l,
    constans AgendaRes* r)
{
    i32 k;
    i32 j;

    k = ZEPHYRUM;
    dum (   k < xar_numerus(l)
         && ante_in_agenda(*(constans AgendaRes**)xar_obtinere(l, k),
         r))
    {
        k++;
    }
    (vacuum)xar_addere(l);
    per (j = xar_numerus(l) - I; j > k; j--)
    {
        *(constans AgendaRes**)xar_obtinere(l, j) =
            *(constans AgendaRes**)xar_obtinere(l, j - I);
    }
    *(constans AgendaRes**)xar_obtinere(l, k) = r;
}

/* greges visus agendae: rubricae ordine canonico (filtro: sola eius),
 * deinde res sine rubrica (non sub filtro); greges vacui absunt */
interior Xar*
greges_agendae (
     constans KalendariumMontatio* m,
                          Piscina* p)
{
                    Xar* greges;
                    Xar* apertae;
                    Xar* rubricae;
                 chorda* filtrum;
            GrexAgendae  g;
     constans AgendaRes* r;
                    i32  i;
                    i32  j;

    greges = xar_creare(p, (i32)magnitudo(GrexAgendae));
    si (!m->documentum || !greges)
    {
        redde greges;
    }
    apertae   = agenda_apertae(m->documentum, p);
    rubricae  = agenda_rubricae(m->documentum, p);
    filtrum   = insula_ramus_attributum(&m->ramus, "rubrica_filtrum");
    per (i = ZEPHYRUM; rubricae && i <= xar_numerus(rubricae); i++)
    {
        si (i < xar_numerus(rubricae))
        {
            g.rubrica = *(chorda*)xar_obtinere(rubricae, i);
            si (filtrum && !chorda_aequalis(*filtrum, g.rubrica))
            {
                perge;
            }
        }
        alioquin
        {
            si (filtrum)
            {
                frange;
            }
            g.rubrica.datum    = NIHIL;
            g.rubrica.mensura  = ZEPHYRUM;
        }
        g.res = xar_creare(p, (i32)magnitudo(constans AgendaRes*));
        per (j = ZEPHYRUM; apertae && j < xar_numerus(apertae); j++)
        {
            r = *(constans AgendaRes**)xar_obtinere(apertae, j);
            si (  g.rubrica.mensura > ZEPHYRUM
                ? rubricam_habet(r, g.rubrica)
                : xar_numerus(r->rubricae) == ZEPHYRUM)
            {
                in_gregem_inserere(g.res, r);
            }
        }
        si (xar_numerus(g.res) > ZEPHYRUM)
        {
            *(GrexAgendae*)xar_addere(greges) = g;
        }
    }
    redde greges;
}

/* ordines currentes (Xar de constans AgendaRes*): visus diei aut
 * agendae (greges iuncti - res in gregibus pluribus pluries) */
interior Xar*
ordines_currentes (
     constans KalendariumMontatio* m,
                          Piscina* p)
{
            Xar* l;
            Xar* greges;
    GrexAgendae* g;
            i32  i;
            i32  j;

    si (!visus_agendae(m))
    {
        redde ordines_diei(m, kalendarium_dies_electus(m), p);
    }
    l       = xar_creare(p, (i32)magnitudo(constans AgendaRes*));
    greges  = greges_agendae(m, p);
    per (i = ZEPHYRUM; l && greges && i < xar_numerus(greges); i++)
    {
        g = (GrexAgendae*)xar_obtinere(greges, i);
        per (j = ZEPHYRUM; j < xar_numerus(g->res); j++)
        {
            *(constans AgendaRes**)xar_addere(l) =
                *(constans AgendaRes**)xar_obtinere(g->res, j);
        }
    }
    redde l;
}

/* post rem e ordinibus remotam: electio ad indicem eundem (aut
 * ultimum) ordinum novorum */
interior vacuum
electionem_reficere (
    KalendariumMontatio* m,
                    i32  index,
                Piscina* p)
{
    Xar* l;
    i32  n;

    l = ordines_currentes(m, p);
    n = l ? xar_numerus(l) : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        (vacuum)electionem_scribere(m, (*(constans AgendaRes**)
            xar_obtinere(l, index < n ? index : n - I))->id);
    }
}


/* ==================================================
 * Actio
 * ================================================== */

/* visus mensis: ictus in cellula (id = dies ISO) diem eligit; claves
 * per tabulam mandatorum */
interior b32
ictus_mensis (
    KalendariumMontatio* m,
              Componens* nodus)
{
    Dies d;

    si (   nodus && fasti_parsare_iso(nodus->id, &d)
        && fasti_dies_valida(d))
    {
        redde diem_scribere(m, d);
    }
    redde FALSUM;
}


/* ==================================================
 * Prompta (A3b): linea una per vim.h (modus inserendi)
 * ================================================== */

/* prompta aperta? genus in *genus (NIHIL licet) */
interior b32
prompta_aperta (
    constans KalendariumMontatio* m,
                          chorda* genus)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, "prompta");
    si (!a || chorda_aequalis_literis(*a, "nulla"))
    {
        redde FALSUM;
    }
    si (genus)
    {
        *genus = *a;
    }
    redde VERUM;
}

/* textus promptae (solutus) in exitus[KALENDARIUM_PROMPTA];
 * redde longitudinem */
interior i32
promptae_textus (
     constans KalendariumMontatio* m,
                        character* exitus)
{
    chorda* a;
       s32  n;

    a = insula_ramus_attributum(&m->ramus, "prompta_textus");
    n = a ? scriba_solvere(*a, exitus, KALENDARIUM_PROMPTA) : ZEPHYRUM;
    redde n > ZEPHYRUM ? (i32)n : ZEPHYRUM;
}

/* status promptae scribere (scriptura una): genus ("nulla" claudit),
 * textus crudus (effugitur), cursor */
interior b32
promptam_scribere (
     KalendariumMontatio* m,
      constans character* genus,
      constans character* textus,
                     i32  longitudo,
                     i32  cursor,
                 Piscina* p)
{
         ParCampi pares[III];
     GrexCamporum grex;
           chorda prior;
           chorda effugitus;
        character numerus[XVI];
              b32 ok;

    effugitus = scriba_effugere(textus, longitudo, p);
    sprintf(numerus, "%d", (integer)cursor);
    pares[ZEPHYRUM].titulus  = "prompta";
    pares[ZEPHYRUM].valor    = genus;
    pares[I].titulus         = "prompta_textus";
    pares[I].valor           = longitudo > ZEPHYRUM
                             ? chorda_ut_cstr(effugitus, p) : "";
    pares[II].titulus  = "prompta_cursor";
    pares[II].valor    = longitudo > ZEPHYRUM ? numerus : "";
    grex.pares         = pares;
    grex.numerus       = III;
    prior              = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_calendarii, m->piscina));
    ok = mutare_ramum(&m->ramus, campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior b32
promptam_claudere (
    KalendariumMontatio* m,
                Piscina* p)
{
    redde promptam_scribere(m, "nulla", "", ZEPHYRUM, ZEPHYRUM, p);
}

/* clavis eventus -> clavis vim (nominatae et imprimibiles); 0 =
 * nulla */
interior s32
clavis_vim (
    constans Eventus* ev)
{
    commutatio (ev->datum.clavis.clavis)
    {
        casus CLAVIS_RETRORSUM:  redde VIM_CLAVIS_BACKSPACE;
        casus CLAVIS_DELERE:     redde VIM_CLAVIS_DELETE;
        casus CLAVIS_SINISTER:   redde VIM_CLAVIS_SINISTRAM;
        casus CLAVIS_DEXTER:     redde VIM_CLAVIS_DEXTRAM;
        casus CLAVIS_DOMUS:      redde VIM_CLAVIS_HOME;
        casus CLAVIS_FINIS:      redde VIM_CLAVIS_END;
        ordinarius:              frange;
    }
    si (   ev->datum.clavis.producta >= ' '
        && ev->datum.clavis.producta < CXXVII)
    {
        redde ev->datum.clavis.producta;
    }
    redde ZEPHYRUM;
}

/* rubrica filtri valida: [a-z0-9-]+ (ut agenda) */
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

/* rubricae rei iunctae ("a b") in piscina */
interior chorda
rubricas_iungere (
    constans AgendaRes* r,
               Piscina* p)
{
    chorda s;
       i32 i;

    s.datum    = NIHIL;
    s.mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(r->rubricae); i++)
    {
        si (i > ZEPHYRUM)
        {
            s = chorda_concatenare(s, chorda_ex_literis(" ", p), p);
        }
        s = chorda_concatenare(s, *(chorda*)xar_obtinere(r->rubricae,
            i),
            p);
    }
    redde s;
}

/* Enter: textus in actum agendae per genus; VERUM si actum (prompta
 * tunc clauditur), FALSUM = recusatum (prompta manet) */
interior b32
promptam_committere (
    KalendariumMontatio* m,
                 chorda  genus,
                 chorda  textus,
                Piscina* p)
{
    Xar* l;
    s32  id;
   Dies  d;
    i32  index;

    si (!m->documentum)
    {
        redde FALSUM;
    }
    d   = kalendarium_dies_electus(m);
    l   = ordines_currentes(m, p);
    id  = res_electa(m, l);
    si (chorda_aequalis_literis(genus, "nova"))
    {
        id = agenda_rem_creare(m->documentum, textus);
        si (id <= ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (visus_agendae(m))
        {
            /* agenda: sine die; rubrica filtri si posita */
            chorda* filtrum;

            filtrum = insula_ramus_attributum(&m->ramus,
                "rubrica_filtrum");
            si (filtrum)
            {
                (vacuum)agenda_rubricas_ponere(m->documentum, id,
                    *filtrum);
            }
        }
        alioquin si (agenda_diem_ponere(m->documentum, id, &d)
                     <= ZEPHYRUM)
        {
            redde FALSUM;
        }
        redde electionem_scribere(m, id);
    }
    si (chorda_aequalis_literis(genus, "filtrum"))
    {
        /* vacuum = filtrum tollere; aliter rubrica valida */
        si (textus.mensura == ZEPHYRUM)
        {
            redde campum_scribere(m, "rubrica_filtrum", "");
        }
        redde rubrica_valida(textus) && campum_scribere(m,
            "rubrica_filtrum", chorda_ut_cstr(textus, p));
    }
    si (id <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(genus, "rubricae"))
    {
        redde agenda_rubricas_ponere(m->documentum, id, textus)
            > ZEPHYRUM;
    }
    si (chorda_aequalis_literis(genus, "nota"))
    {
        redde agenda_notam_addere(m->documentum, id, hodie(m), textus)
            > ZEPHYRUM;
    }
    si (chorda_aequalis_literis(genus, "titulus"))
    {
        redde agenda_titulum_ponere(m->documentum, id, textus)
            > ZEPHYRUM;
    }
    si (chorda_aequalis_literis(genus, "dies"))
    {
        Dies nova;

        index = index_rei(l, id);
        si (textus.mensura == ZEPHYRUM)
        {
            si (agenda_diem_ponere(m->documentum, id, NIHIL)
                <= ZEPHYRUM)
            {
                redde FALSUM;
            }
        }
        alioquin si (   !fasti_parsare_iso(textus, &nova)
                     || !fasti_dies_valida(nova)
                     || agenda_diem_ponere(m->documentum, id, &nova)
                        <= ZEPHYRUM)
        {
            redde FALSUM;
        }
        electionem_reficere(m, index, p);
        redde VERUM;
    }
    redde FALSUM;
}

/* clavis dum prompta aperta est: Esc claudit, Enter committit, ceterae
 * per vim (modus inserendi, linea una) */
interior b32
agere_promptam (
    KalendariumMontatio* m,
       constans Eventus* ev,
                 chorda  genus,
                Piscina* p)
{
     TabulaCharacterum  t;
             VimStatus  st;
             character  textus[KALENDARIUM_PROMPTA];
                chorda* a;
                   s32  cursor;
                   s32  clavis;
                   i32  n;
                   i32  i;

    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    n = promptae_textus(m, textus);
    si (ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        redde promptam_claudere(m, p);
    }
    si (ev->datum.clavis.clavis == CLAVIS_REDITUS)
    {
        chorda c;

        dum (n > ZEPHYRUM && textus[n - I] == ' ')
        {
            n--;
        }
        c.datum    = (i8*)textus;
        c.mensura  = n;
        si (promptam_committere(m, genus, chorda_transcribere(c, p), p))
        {
            (vacuum)promptam_claudere(m, p);
        }
        redde VERUM;
    }
    clavis = clavis_vim(ev);
    si (   clavis == ZEPHYRUM
        || (clavis > ZEPHYRUM && n >= KALENDARIUM_PROMPTA - I))
    {
        redde VERUM;   /* consumpta: prompta claves omnes capit */
    }
    a = insula_ramus_attributum(&m->ramus, "prompta_cursor");
    si (   !a || !chorda_ut_s32(*a, &cursor) || cursor < ZEPHYRUM
        || cursor > (s32)n)
    {
        cursor = (s32)n;
    }
    tabula_initiare(&t, p, KALENDARIUM_PROMPTA, I);
    memcpy(t.cellulae, textus, (size_t)n);
    st = vim_initiare(&t);
    st = vim_ponere_modum(st, MODO_VIM_INSERERE);
    st = vim_ponere_cursor(st, ZEPHYRUM, (i32)cursor);
    st = vim_tractare_clavem(st, clavis);
    /* longitudo: vim cellulas vacuas spatiis implet - ultima cellula
     * non alba, aut cursor (spatium modo typatum manet donec littera
     * sequitur; ad Enter abscinditur) */
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < KALENDARIUM_PROMPTA; i++)
    {
        si (t.cellulae[i] != '\0' && t.cellulae[i] != ' ')
        {
            n = i + I;
        }
    }
    si ((s32)n < (s32)st.cursor_columna)
    {
        n = st.cursor_columna;
    }
    redde promptam_scribere(m, chorda_ut_cstr(genus, p), t.cellulae, n,
        st.cursor_columna, p);
}

/* visus indicis (dies, agenda): ictus in ordine ("r<id>", "r<id>-<k>")
 * rem eligit; claves per tabulam mandatorum */
interior b32
ictus_ordinis (
    KalendariumMontatio* m,
              Componens* nodus)
{
    s32 id;

    /* "r<id>" (dies) aut "r<id>-<grex>" (agenda) */
    si (   nodus && nodus->id.mensura > I
        && nodus->id.datum[ZEPHYRUM] == 'r')
    {
        chorda numerus;

        numerus.datum    = nodus->id.datum + I;
        numerus.mensura  = ZEPHYRUM;
        dum (   numerus.mensura < nodus->id.mensura - I
             && numerus.datum[numerus.mensura] != '-')
        {
            numerus.mensura++;
        }
        si (chorda_ut_s32(numerus, &id))
        {
            redde electionem_scribere(m, id);
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Tabula mandatorum (claves; postea mus et index mandatorum)
 * ================================================== */

/* visus in quibus mandatum valet (larva) */
#define VISUS_MENSIS   I
#define VISUS_DIEI     II
#define VISUS_AGENDAE  IV
#define VISUS_INDICIS  (VISUS_DIEI | VISUS_AGENDAE)   /* ordines */

/* requisita: quod mandatum praesto facit (data, non functio) */
#define REQUIRIT_REM      I    /* res electa */
#define REQUIRIT_FILTRUM  II   /* filtrum positum */

/* contextus: semel per eventum computatus */
nomen structura {
    KalendariumMontatio* m;
                Piscina* p;
                   Dies  dies;       /* dies electa */
                    Xar* ordines;    /* ordines currentes */
                    s32  id;         /* res electa, 0 = nulla */
                    i32  index;
     constans AgendaRes* r;
} ContextusMandati;

nomen b32 (*MandatumFn)(
    ContextusMandati* cm);

nomen structura {
     constans character* titulus;      /* "calendarium.perficere" */
     constans character* descriptio;   /* "Complete / reopen item" */
     constans character* clavis;       /* "x", "Enter", "Esc", "C-r";
                                        * NIHIL = sine clave (mus) */
                    i32 visus;        /* larva VISUS_* */
                    i32 requisita;    /* larva REQUIRIT_* */
             MandatumFn fn;
} MandatumKalendarii;

interior b32
fac_diem_priorem (
    ContextusMandati* cm)
{
    redde diem_scribere(cm->m, fasti_addere_dies(cm->dies, -I));
}

interior b32
fac_diem_proximum (
    ContextusMandati* cm)
{
    redde diem_scribere(cm->m, fasti_addere_dies(cm->dies, I));
}

interior b32
fac_hebdomadem_priorem (
    ContextusMandati* cm)
{
    redde diem_scribere(cm->m, fasti_addere_dies(cm->dies, -VII));
}

interior b32
fac_hebdomadem_proximam (
    ContextusMandati* cm)
{
    redde diem_scribere(cm->m, fasti_addere_dies(cm->dies, VII));
}

/* dies ad finem mensis praecisa (fasti_addere_menses) */
interior b32
fac_mensem_priorem (
    ContextusMandati* cm)
{
    redde diem_scribere(cm->m, fasti_addere_menses(cm->dies, -I));
}

interior b32
fac_mensem_proximum (
    ContextusMandati* cm)
{
    redde diem_scribere(cm->m, fasti_addere_menses(cm->dies, I));
}

interior b32
fac_hodie (
    ContextusMandati* cm)
{
    redde diem_scribere(cm->m, hodie(cm->m));
}

interior b32
fac_diem_aperire (
    ContextusMandati* cm)
{
    redde campum_scribere(cm->m, "visus", "dies");
}

/* res proxima/prior: res in gregibus pluribus iterata praetereunt */
interior b32
rem_gradi (
    ContextusMandati* cm,
                 s32  gradus)
{
    constans AgendaRes* proxima;
                   s32  k;

    si (cm->index == (i32)-I)
    {
        redde VERUM;
    }
    k = (s32)cm->index;
    dum (   k + gradus >= ZEPHYRUM && k + gradus < (s32)xar_numerus(
        cm->ordines))
    {
        k        += gradus;
        proxima  = *(constans AgendaRes**)xar_obtinere(cm->ordines,
            (i32)k);
        si (proxima->id != cm->id)
        {
            redde electionem_scribere(cm->m, proxima->id);
        }
    }
    redde VERUM;
}

interior b32
fac_rem_proximam (
    ContextusMandati* cm)
{
    redde rem_gradi(cm, I);
}

interior b32
fac_rem_priorem (
    ContextusMandati* cm)
{
    redde rem_gradi(cm, -I);
}

/* dies perfectionis = hodie ex porta; agenda: res perfecta abit -
 * electio ad ordinem proximum */
interior b32
fac_perficere (
    ContextusMandati* cm)
{
    (vacuum)(cm->r->perfecta
        ? agenda_perfectionem_tollere(cm->m->documentum, cm->id)
        : agenda_perficere(cm->m->documentum, cm->id, hodie(cm->m)));
    si (visus_agendae(cm->m))
    {
        electionem_reficere(cm->m, cm->index, cm->p);
    }
    redde VERUM;
}

interior b32
fac_delere (
    ContextusMandati* cm)
{
    (vacuum)agenda_rem_delere(cm->m->documentum, cm->id);
    electionem_reficere(cm->m, cm->index, cm->p);
    redde VERUM;
}

interior b32
fac_revocare (
    ContextusMandati* cm)
{
    (vacuum)agenda_revocare(cm->m->documentum);
    redde VERUM;
}

interior b32
fac_reficere (
    ContextusMandati* cm)
{
    (vacuum)agenda_reficere(cm->m->documentum);
    redde VERUM;
}

/* res ad diem priorem/proximum (visus manet) */
interior b32
rem_movere (
    ContextusMandati* cm,
                 s32  gradus)
{
    Dies novus;

    novus = fasti_addere_dies(cm->r->dies_posita ? cm->r->dies
                                                 : cm->dies, gradus);
    (vacuum)agenda_diem_ponere(cm->m->documentum, cm->id, &novus);
    electionem_reficere(cm->m, cm->index, cm->p);
    redde VERUM;
}

interior b32
fac_rem_ante (
    ContextusMandati* cm)
{
    redde rem_movere(cm, -I);
}

interior b32
fac_rem_post (
    ContextusMandati* cm)
{
    redde rem_movere(cm, I);
}

/* promptae (A3b): textus initialis, cursor in fine */
interior b32
promptam_aperire (
      ContextusMandati* cm,
    constans character* genus,
                chorda  textus)
{
    redde promptam_scribere(cm->m, genus, (constans character*)
        textus.datum, textus.mensura, textus.mensura, cm->p);
}

interior b32
fac_novam (
    ContextusMandati* cm)
{
    redde promptam_scribere(cm->m, "nova", "", ZEPHYRUM, ZEPHYRUM,
        cm->p);
}

interior b32
fac_notam (
    ContextusMandati* cm)
{
    redde promptam_scribere(cm->m, "nota", "", ZEPHYRUM, ZEPHYRUM,
        cm->p);
}

interior b32
fac_titulum (
    ContextusMandati* cm)
{
    redde promptam_aperire(cm, "titulus", cm->r->titulus);
}

/* dies scripta (vacua = sine die) */
interior b32
fac_diem_rei (
    ContextusMandati* cm)
{
    chorda iso;

    iso.datum    = NIHIL;
    iso.mensura  = ZEPHYRUM;
    si (cm->r->dies_posita)
    {
        iso = fasti_formare_diem(cm->r->dies, FASTI_FORMA_ISO, cm->p);
    }
    redde promptam_aperire(cm, "dies", iso);
}

interior b32
fac_rubricas (
    ContextusMandati* cm)
{
    redde promptam_aperire(cm, "rubricae", rubricas_iungere(cm->r,
        cm->p));
}

/* filtrum (A4b): vacuum tollit, rubrica persistens */
interior b32
fac_filtrum (
    ContextusMandati* cm)
{
    chorda* filtrum;
    chorda  textus;

    filtrum         = insula_ramus_attributum(&cm->m->ramus,
        "rubrica_filtrum");
    textus.datum    = NIHIL;
    textus.mensura  = ZEPHYRUM;
    si (filtrum)
    {
        textus = *filtrum;
    }
    redde promptam_aperire(cm, "filtrum", textus);
}

interior b32
fac_filtrum_tollere (
    ContextusMandati* cm)
{
    redde campum_scribere(cm->m, "rubrica_filtrum", "");
}

interior b32
fac_mensem (
    ContextusMandati* cm)
{
    redde campum_scribere(cm->m, "visus", "mensis");
}

/* tabula: clavis in visu UNO mandatum unum (mandata_verificare);
 * descriptio Anglice (lingua interfaciei, ut horologium vici) */
hic_manens constans MandatumKalendarii TABULA_MANDATORUM[] = {
    { "calendarium.dies_prior", "Previous day", "h", VISUS_MENSIS
        | VISUS_DIEI, ZEPHYRUM, fac_diem_priorem },
    { "calendarium.dies_proximus", "Next day", "l", VISUS_MENSIS
        | VISUS_DIEI, ZEPHYRUM, fac_diem_proximum },
    { "calendarium.hebdomas_prior", "Previous week", "k", VISUS_MENSIS,
        ZEPHYRUM, fac_hebdomadem_priorem },
    { "calendarium.hebdomas_proxima", "Next week", "j", VISUS_MENSIS,
        ZEPHYRUM, fac_hebdomadem_proximam },
    { "calendarium.mensis_prior", "Previous month", "H", VISUS_MENSIS,
        ZEPHYRUM, fac_mensem_priorem },
    { "calendarium.mensis_proximus", "Next month", "L", VISUS_MENSIS,
        ZEPHYRUM, fac_mensem_proximum },
    { "calendarium.hodie", "Go to today", "t", VISUS_MENSIS, ZEPHYRUM,
        fac_hodie },
    { "calendarium.diem_aperire", "Open day", "Enter", VISUS_MENSIS,
        ZEPHYRUM, fac_diem_aperire },
    { "calendarium.res_proxima", "Next item", "j", VISUS_INDICIS,
        ZEPHYRUM, fac_rem_proximam },
    { "calendarium.res_prior", "Previous item", "k", VISUS_INDICIS,
        ZEPHYRUM, fac_rem_priorem },
    { "calendarium.perficere", "Complete / reopen item", "x",
        VISUS_INDICIS, REQUIRIT_REM, fac_perficere },
    { "calendarium.delere", "Delete item", "X", VISUS_INDICIS,
        REQUIRIT_REM, fac_delere },
    { "calendarium.revocare", "Undo", "u", VISUS_INDICIS, ZEPHYRUM,
        fac_revocare },
    { "calendarium.reficere", "Redo", "C-r", VISUS_INDICIS, ZEPHYRUM,
        fac_reficere },
    { "calendarium.nova", "New item", "n", VISUS_INDICIS, ZEPHYRUM,
        fac_novam },
    { "calendarium.nota", "Add note", "a", VISUS_INDICIS, REQUIRIT_REM,
        fac_notam },
    { "calendarium.titulus", "Edit title", "c", VISUS_INDICIS,
        REQUIRIT_REM, fac_titulum },
    { "calendarium.dies_rei", "Set date", "D", VISUS_INDICIS,
        REQUIRIT_REM, fac_diem_rei },
    { "calendarium.rubricae", "Edit rubrics", "r", VISUS_INDICIS,
        REQUIRIT_REM, fac_rubricas },
    { "calendarium.res_ante", "Move item to previous day", "<",
        VISUS_DIEI, REQUIRIT_REM, fac_rem_ante },
    { "calendarium.res_post", "Move item to next day", ">", VISUS_DIEI,
        REQUIRIT_REM, fac_rem_post },
    { "calendarium.ad_mensem", "Back to month", "Esc", VISUS_DIEI,
        ZEPHYRUM, fac_mensem },
    { "calendarium.filtrum", "Filter by rubric", "f", VISUS_AGENDAE,
        ZEPHYRUM, fac_filtrum },
    { "calendarium.filtrum_tollere", "Clear filter", "Esc",
        VISUS_AGENDAE, REQUIRIT_FILTRUM, fac_filtrum_tollere },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

interior i32
visus_currens (
    constans KalendariumMontatio* m)
{
    redde visus_agendae(m) ? VISUS_AGENDAE
         : visus_diei(m)   ? VISUS_DIEI
         :                   VISUS_MENSIS;
}

/* clavis eventus = forma tabulae? "Enter", "Esc", "C-r" (Ctrl), aut
 * littera una SINE Ctrl/Cmd (Cmd chorda etiam ut clavis advenit) */
interior b32
clavis_congruit (
    constans character* forma,
      constans Eventus* ev)
{
    i32 mod;

    mod = ev->datum.clavis.modificantes;
    si (strcmp(forma, "Enter") == ZEPHYRUM)
    {
        redde ev->datum.clavis.clavis == CLAVIS_REDITUS;
    }
    si (strcmp(forma, "Esc") == ZEPHYRUM)
    {
        redde ev->datum.clavis.clavis == CLAVIS_EFFUGIUM;
    }
    si (forma[ZEPHYRUM] == 'C' && forma[I] == '-')
    {
        redde (mod & (i32)MOD_IMPERIUM)
            && ev->datum.clavis.runa == (s32)(insignatus character)
            forma[II];
    }
    redde !(mod & (i32)(MOD_IMPERIUM | MOD_SUPER))
        && ev->datum.clavis.producta == (s32)(insignatus character)
        forma[ZEPHYRUM];
}

/* tabula sana: omnia posita, titulus unicus, clavis in visu uno
 * semel (montatio aliter recusat - tabula fracta nulla probatione
 * praeterit) */
interior b32
mandata_verificare (
    vacuum)
{
    constans MandatumKalendarii* a;
    constans MandatumKalendarii* b;

    per (a = TABULA_MANDATORUM; a->titulus; a++)
    {
        si (!a->descriptio || !a->fn || !a->visus)
        {
            redde FALSUM;
        }
        per (b = a + I; b->titulus; b++)
        {
            /* titulus unicus (mandatum_invocare); clavis semel per
             * visum */
            si (   strcmp(a->titulus, b->titulus) == ZEPHYRUM
                || (   a->clavis && b->clavis && (a->visus & b->visus)
                    && strcmp(a->clavis, b->clavis) == ZEPHYRUM))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* contextus mandati: dies, ordines currentes, res electa */
interior vacuum
contextum_parare (
     KalendariumMontatio* m,
                Piscina* p,
       ContextusMandati* cm)
{
    memset(cm, ZEPHYRUM, magnitudo(*cm));
    cm->m      = m;
    cm->p      = p;
    cm->dies   = kalendarium_dies_electus(m);
    cm->index  = (i32)-I;
    si (visus_currens(m) & VISUS_INDICIS)
    {
        cm->ordines  = ordines_currentes(m, p);
        cm->id       = res_electa(m, cm->ordines);
        cm->index    = index_rei(cm->ordines, cm->id);
        cm->r        = (m->documentum && cm->id > ZEPHYRUM)
                     ? agenda_res(m->documentum, cm->id) : NIHIL;
    }
}

/* requisita mandati impleta? (visus a vocante probatur) */
interior b32
requisita_impleta (
       constans MandatumKalendarii* t,
         constans ContextusMandati* cm)
{
    redde (!(t->requisita & REQUIRIT_REM) || cm->r)
        && (   !(t->requisita & REQUIRIT_FILTRUM)
            || insula_ramus_attributum(&cm->m->ramus,
            "rubrica_filtrum"));
}

/* mandatum per titulum (mus; postea index mandatorum): FALSUM si
 * ignotum aut in visu currenti non praesto */
interior b32
mandatum_invocare (
    KalendariumMontatio* m,
     constans character* titulus,
                Piscina* p)
{
    constans MandatumKalendarii* t;
               ContextusMandati  cm;

    per (t = TABULA_MANDATORUM; t->titulus; t++)
    {
        si (strcmp(t->titulus, titulus) == ZEPHYRUM)
        {
            frange;
        }
    }
    si (!t->titulus || !(t->visus & visus_currens(m)))
    {
        redde FALSUM;
    }
    contextum_parare(m, p, &cm);
    redde requisita_impleta(t, &cm) && t->fn(&cm);
}

/* clavis -> mandatum visus currentis. Clavis visus sed mandatum non
 * praesto: consumpta (nihil fit), nisi Esc - Esc hospiti transit
 * (gradus sursum) */
interior b32
clavem_agere (
    KalendariumMontatio* m,
       constans Eventus* ev,
                Piscina* p)
{
    constans MandatumKalendarii* t;
               ContextusMandati  cm;
                            i32  visus;

    visus = visus_currens(m);
    per (t = TABULA_MANDATORUM; t->titulus; t++)
    {
        si (   (t->visus & visus) && t->clavis
            && clavis_congruit(t->clavis, ev))
        {
            frange;
        }
    }
    si (!t->titulus)
    {
        redde FALSUM;
    }
    contextum_parare(m, p, &cm);
    si (!requisita_impleta(t, &cm))
    {
        redde strcmp(t->clavis, "Esc") != ZEPHYRUM;
    }
    redde t->fn(&cm);
}

/* <actio/> calendarium.agere: visus mensis aut diei */
interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    KalendariumMontatio* m;
     KalendariumFigurae* privatum;

    (vacuum)repo;
    (vacuum)motus;
    m         = (KalendariumMontatio*)ctx;
    privatum  = (KalendariumFigurae*)m->figurae_ctx;
    si (!privatum || !privatum->scheda)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    si (visus_diei(m) || visus_agendae(m))
    {
        chorda genus;

        si (prompta_aperta(m, &genus))
        {
            redde agere_promptam(m, ev, genus, privatum->scheda);
        }
    }
    si (ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        si (!(visus_diei(m) || visus_agendae(m)))
        {
            redde ictus_mensis(m, nodus);
        }
        si (!ictus_ordinis(m, nodus))
        {
            redde FALSUM;
        }
        /* capsa "[ ]" (M1a): linea prima, ante titulum */
        si (   des && des->punctum_locale.x < KALENDARIUM_CAPSA
            && des->punctum_locale.y < KALENDARIUM_LINEA + II)
        {
            (vacuum)mandatum_invocare(m, "calendarium.perficere",
                privatum->scheda);
        }
        redde VERUM;
    }
    /* duplex (M1a): in cellula mensis diem aperit (ictus primus iam
     * elegit) */
    si (ev->genus == EVENTUS_MUS_DUPLEX)
    {
        Dies d;

        redde !visus_diei(m) && !visus_agendae(m) && nodus
            && fasti_parsare_iso(nodus->id, &d)
            && mandatum_invocare(m, "calendarium.diem_aperire",
            privatum->scheda);
    }
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    redde clavem_agere(m, ev, privatum->scheda);
}


/* ==================================================
 * Figurae (A2b)
 * ================================================== */

hic_manens constans character* constans menses_anglice[XII] = {
    "January", "February", "March", "April", "May", "June", "July",
    "August", "September", "October", "November", "December"
};

/* colores liturgici RGB, ordine ColorLiturgicus (albus aureus: in fundo
 * clarissimo videtur) */
hic_manens constans i32 colores_liturgici[VI][III] = {
    { CCXXX, CCV, CXX },     /* albus */
    { CXC, XL, XL },         /* ruber */
    { LX, CXL, LXX },        /* viridis */
    { CXX, LX, CL },         /* violaceus */
    { CCXXX, CXL, CLXX },    /* rosaceus */
    { XXX, XXX, XXX }        /* niger */
};

interior ColorMandati
color_liturgicus (
    ColorLiturgicus color)
{
    ColorMandati c;
             i32 k;

    k        = ((i32)color < VI) ? (i32)color : ZEPHYRUM;
    c.genus  = COLOR_MANDATI_RGBA;
    c.valor  = color_ad_pixelum(color_ex_rgba(
        (i8)colores_liturgici[k][ZEPHYRUM], (i8)colores_liturgici[k][I],
        (i8)colores_liturgici[k][II], (i8)CCLV));
    redde c;
}

/* textus ad latitudinem (pixela) praecisus */
interior chorda
praecidere (
    chorda textus,
       s32 latitudo)
{
    s32 n;

    n = latitudo / KALENDARIUM_LITTERA;
    si (n < ZEPHYRUM)
    {
        n = ZEPHYRUM;
    }
    si (textus.mensura > (i32)n)
    {
        textus.mensura = (i32)n;
    }
    redde textus;
}

interior vacuum
rectangulum (
         Mandata* md,
             s32  x,
             s32  y,
             s32  latitudo,
             s32  altitudo,
    ColorMandati  color,
             b32  impletum)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    mandata_rectangulum(md, f, color, impletum);
}

/* signa (apertae, perfectae) diei ex signis mensium praesentium */
nomen structura {
             s32 annus;
             s32 mensis;
    AgendaSignum signa[XXXI];
} SignaMensis;

interior constans AgendaSignum*
signum_diei (
    constans SignaMensis* menses,
                     i32  numerus,
                    Dies  dies)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   menses[i].annus  == dies.annus
            && menses[i].mensis == dies.mensis)
        {
            redde &menses[i].signa[dies.dies - I];
        }
    }
    redde NIHIL;
}

/* cellula una (in spatio tabulae; cellulae liberi tabulae in 0,0) */
interior vacuum
cellulam_pingere (
       KalendariumFigurae* contextus,
                  Mandata* md,
                    Fines  f,
                     Dies  dies,
                     Dies  electa,
                     Dies  hodierna,
    constans AgendaSignum* signum)
{
     InformatioDiei* info;
          character  numerus[IV];
             chorda  textus;
                b32  alienus;
                s32  x;
                i32  k;
                i32  apertae;
                i32  perfectae;

    alienus = dies.mensis != electa.mensis
        || dies.annus != electa.annus;
    info = NIHIL;
    si (contextus->liturgia)
    {
        info = calendarium_obtinere_diem(contextus->liturgia, dies,
            md->piscina);
    }
    /* numerus diei: hodie inversus, alieni obscuri */
    numerus[ZEPHYRUM]  = (character)('0' + dies.dies / X);
    numerus[I]         = (character)('0' + dies.dies % X);
    numerus[II]        = '\0';
    textus = chorda_ex_literis(dies.dies >= X ? numerus : numerus + I,
        md->piscina);
    si (dies_aequales(dies, hodierna))
    {
        rectangulum(md, f.x + III, f.y + I, (s32)textus.mensura
            * KALENDARIUM_LITTERA + II, KALENDARIUM_LINEA + I,
            figura_color_thematis(COLOR_ACCENT_PRIMARY), VERUM);
        mandata_textus(md, f.x + IV, f.y + I, textus, ZEPHYRUM,
            figura_color_thematis(COLOR_BACKGROUND));
    }
    alioquin
    {
        mandata_textus(md, f.x + IV, f.y + I, textus, ZEPHYRUM,
            figura_color_thematis(alienus ? COLOR_TEXT_DIM
                                          : COLOR_TEXT));
    }
    /* color liturgicus: quadratum iuxta numerum (Franus: linea ad
     * marginem ut divisor legebatur) */
    si (info)
    {
        rectangulum(md, f.x + V + (s32)textus.mensura
            * KALENDARIUM_LITTERA + II, f.y + II, V, V,
            color_liturgicus(info->color_diei), VERUM);
    }
    /* festum: memoria aut maius; dominica nuda nihil (Franus: color et
     * columna dicunt - titulus praecisus "Dominica X" fallebat) */
    si (   info && info->celebratio_principalis
        && info->celebratio_principalis->gradus >= GRADUS_MEMORIA
        && info->celebratio_principalis->gradus != GRADUS_DOMINICA)
    {
        mandata_textus(md, f.x + IV, f.y + KALENDARIUM_LINEA + III,
            praecidere(info->celebratio_principalis->titulus_brevis,
            f.latitudo - VI), ZEPHYRUM, figura_color_thematis(alienus
            ? COLOR_TEXT_DIM : COLOR_TEXT));
    }
    /* signa: plena = apertae, vacua = perfectae; IV summum, '+' */
    apertae    = signum ? signum->apertae : ZEPHYRUM;
    perfectae  = signum ? signum->perfectae : ZEPHYRUM;
    x          = f.x + IV;
    per (k = ZEPHYRUM; k < apertae + perfectae; k++)
    {
        si (k == KALENDARIUM_SIGNA_MAXIMA)
        {
            mandata_textus(md, x, f.y + f.altitudo - KALENDARIUM_LINEA
                - I, chorda_ex_literis("+", md->piscina), ZEPHYRUM,
                figura_color_thematis(COLOR_TEXT_DIM));
            frange;
        }
        rectangulum(md, x, f.y + f.altitudo - V, III, III,
            figura_color_thematis(k < apertae ? COLOR_ACCENT_PRIMARY
                                       : COLOR_SUCCESS), k < apertae);
        x += V;
    }
    si (dies_aequales(dies, electa))
    {
        rectangulum(md, f.x, f.y, f.latitudo, f.altitudo,
            figura_color_thematis(COLOR_BORDER_ACTIVE), FALSUM);
    }
}

hic_manens constans character* constans dies_anglice[VII] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
    "Saturday"
};

/* "Oct 13" in piscina */
interior chorda
dies_brevis (
        Dies  dies,
     Piscina* piscina)
{
    character textus[XVI];

    sprintf(textus, "%.3s %d", menses_anglice[dies.mensis - I],
        (integer)dies.dies);
    redde chorda_ex_literis(textus, piscina);
}

/* numerus post praefixum unius litterae, usque ad '-' ("r5-0" -> 5,
 * "g2" -> 2); FALSUM si id alterius praefixi */
interior b32
numerus_identitatis (
         chorda  id,
      character  praefixum,
            s32* numerus)
{
    chorda n;

    si (id.mensura < II || id.datum[ZEPHYRUM] != (i8)praefixum)
    {
        redde FALSUM;
    }
    n.datum    = id.datum + I;
    n.mensura  = ZEPHYRUM;
    dum (n.mensura < id.mensura - I && n.datum[n.mensura] != '-')
    {
        n.mensura++;
    }
    redde chorda_ut_s32(n, numerus);
}

/* visus agendae: tituli gregum (liberi "g<k>" PARTES_INDEX) -
 * "#rubrica" aut "(none)" */
interior vacuum
greges_pingere (
    KalendariumFigurae* contextus,
               Mandata* md,
    constans Componens* c)
{
             Xar* greges;
     GrexAgendae* g;
       Componens* l;
          chorda  titulus;
             s32  k;
             i32  i;

    greges = greges_agendae(contextus->m, md->piscina);
    per (i = ZEPHYRUM; greges
        && i < componens_numerus_liberorum(c); i++)
    {
        l = componens_liberum(c, i);
        si (   l->partes != PARTES_INDEX
            || !numerus_identitatis(l->id, 'g', &k) || k < ZEPHYRUM
            || (i32)k    >= xar_numerus(greges))
        {
            perge;
        }
        g = (GrexAgendae*)xar_obtinere(greges, (i32)k);
        titulus = g->rubrica.mensura > ZEPHYRUM
            ? chorda_concatenare(chorda_ex_literis("#", md->piscina),
            g->rubrica, md->piscina)
            : chorda_ex_literis("(none)", md->piscina);
        mandata_textus(md, l->fines.x + IV, l->fines.y + II, praecidere(
            titulus, l->fines.latitudo - VIII), ZEPHYRUM,
            figura_color_thematis(
            COLOR_TEXT_DIM));
    }
}

/* ordines (liberi "r<id>", in agenda "r<id>-<k>") - electio, capsa et
 * titulus, notae infra. Visus diei: res alibi assignata obscura cum
 * die sua, notae omnium. Agenda: dies ad dextrum ("-" sine die), notae
 * solius electae. */
interior vacuum
ordines_pingere (
    KalendariumFigurae* contextus,
               Mandata* md,
    constans Componens* c)
{
      KalendariumMontatio* m;
       constans AgendaRes* r;
      constans AgendaNota* nota;
                Componens* l;
                   chorda  linea;
                   chorda  dextrum;
                     Dies  dies;
                      s32  id;
                      s32  electa;
                      s32  spatium;
                      b32  alibi;
                      b32  agenda;
             ColorMandati  color_notae;
                      i32  i;
                      i32  k;

    m       = contextus->m;
    agenda  = visus_agendae(m);
    dies    = kalendarium_dies_electus(m);
    electa  = res_electa(m, ordines_currentes(m, md->piscina));
    si (agenda)
    {
        greges_pingere(contextus, md, c);
    }
    per (i = ZEPHYRUM; m->documentum
        && i < componens_numerus_liberorum(c); i++)
    {
        l = componens_liberum(c, i);
        r = (   l->partes == PARTES_ITEM
             && numerus_identitatis(l->id, 'r', &id))
            ? agenda_res(m->documentum, id) : NIHIL;
        si (!r)
        {
            perge;
        }
        si (id == electa)
        {
            rectangulum(md, l->fines.x, l->fines.y, l->fines.latitudo,
                l->fines.altitudo,
                figura_color_thematis(COLOR_SELECTION),
                VERUM);
        }
        alibi  = !agenda && (!r->dies_posita || !dies_aequales(r->dies,
            dies));
        /* ordo electus: notae colore pleno (obscurae in fundo
         * selectionis non leguntur) */
        color_notae = figura_color_thematis(id == electa ? COLOR_TEXT
                                                  : COLOR_TEXT_DIM);
        linea  = chorda_concatenare(chorda_ex_literis(r->perfecta
            ? "[x] " : "[ ] ", md->piscina), r->titulus, md->piscina);
        si (alibi)
        {
            linea = chorda_concatenare(linea, chorda_ex_literis("  (",
                md->piscina), md->piscina);
            linea = chorda_concatenare(linea, r->dies_posita
                ? dies_brevis(r->dies, md->piscina)
                : chorda_ex_literis("unscheduled", md->piscina),
                md->piscina);
            linea = chorda_concatenare(linea, chorda_ex_literis(")",
                md->piscina), md->piscina);
        }
        spatium = l->fines.latitudo - VIII;
        si (agenda)
        {
            dextrum  = r->dies_posita ? dies_brevis(r->dies,
                md->piscina)
                                      : chorda_ex_literis("-",
                                      md->piscina);
            spatium -= (s32)(dextrum.mensura
                + II) * KALENDARIUM_LITTERA;
            mandata_textus(md, l->fines.x + l->fines.latitudo - VIII
                - (s32)dextrum.mensura * KALENDARIUM_LITTERA, l->fines.y
                + I, dextrum, ZEPHYRUM, figura_color_thematis(id
                == electa
                ? COLOR_TEXT : COLOR_TEXT_DIM));
        }
        mandata_textus(md, l->fines.x + IV, l->fines.y + I, praecidere(
            linea, spatium), ZEPHYRUM, figura_color_thematis((alibi
                && id
            != electa) ? COLOR_TEXT_DIM : COLOR_TEXT));
        per (k = ZEPHYRUM; (!agenda || id == electa)
            && k < xar_numerus(r->notae); k++)
        {
            nota   = (constans AgendaNota*)xar_obtinere(r->notae, k);
            linea  = chorda_concatenare(dies_brevis(nota->dies,
                md->piscina), chorda_ex_literis("  ", md->piscina),
                md->piscina);
            linea  = chorda_concatenare(linea, nota->textus,
                md->piscina);
            mandata_textus(md, l->fines.x + XX, l->fines.y + I
                + (s32)(k + I) * KALENDARIUM_LINEA, praecidere(linea,
                l->fines.latitudo - XXIV), ZEPHYRUM, color_notae);
        }
    }
}

/* <figura/> <purus/> tabula: fundus et cellulae omnes (liberi
 * PARTES_ITEM quorum id = dies ISO); dies liturgici et signa semel per
 * quadrum */
interior vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    KalendariumFigurae* contextus;
                 Fines  mensa;
           SignaMensis  menses[III];
                   i32  numerus;
                   i32  i;
                  Dies  electa;
                  Dies  hodierna;
                  Dies  dies;
             Componens* l;

    (vacuum)thema;
    contextus = (KalendariumFigurae*)ctx;
    /* mensa (superficies, exemplar foci), fundus intra marginem */
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    rectangulum(md, KALENDARIUM_MARGO_X, KALENDARIUM_MARGO_Y,
        c->fines.latitudo - II * KALENDARIUM_MARGO_X, c->fines.altitudo
        - II * KALENDARIUM_MARGO_Y,
        figura_color_thematis(COLOR_BACKGROUND),
        VERUM);
    si (!contextus || !contextus->m)
    {
        redde;
    }
    si (visus_diei(contextus->m) || visus_agendae(contextus->m))
    {
        ordines_pingere(contextus, md, c);
        redde;
    }
    electa    = kalendarium_dies_electus(contextus->m);
    hodierna  = hodie(contextus->m);
    /* signa mensium cellularum (III summum: prior, electus, sequens) */
    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; contextus->m->documentum
        && i < componens_numerus_liberorum(c); i++)
    {
        l = componens_liberum(c, i);
        si (   l->partes != PARTES_ITEM
            || !fasti_parsare_iso(l->id, &dies)
            || signum_diei(menses, numerus, dies) || numerus >= III)
        {
            perge;
        }
        menses[numerus].annus   = dies.annus;
        menses[numerus].mensis  = dies.mensis;
        si (agenda_signa_mensis(contextus->m->documentum, dies.annus,
                dies.mensis, menses[numerus].signa))
        {
            numerus++;
        }
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        l = componens_liberum(c, i);
        si (l->partes == PARTES_ITEM && fasti_parsare_iso(l->id, &dies))
        {
            cellulam_pingere(contextus, md, l->fines, dies, electa,
                hodierna,
                signum_diei(menses, numerus, dies));
        }
    }
}

/* titulus visus diei: dies plena; quadratum coloris et celebratio;
 * tempus liturgicum */
interior vacuum
titulum_diei_pingere (
     KalendariumFigurae* contextus,
                Mandata* md,
     constans Componens* c,
                   Dies  dies)
{
    InformatioDiei* info;
         character  titulus[LXIV];

    sprintf(titulus, "%s, %s %d, %d",
        dies_anglice[fasti_dies_hebdomadis(
        dies)], menses_anglice[dies.mensis - I], (integer)dies.dies,
        (integer)dies.annus);
    mandata_textus(md, IV, ZEPHYRUM, praecidere(chorda_ex_literis(
        titulus, md->piscina), c->fines.latitudo - VIII), ZEPHYRUM,
        figura_color_thematis(COLOR_TEXT));
    info = NIHIL;
    si (contextus->liturgia)
    {
        info = calendarium_obtinere_diem(contextus->liturgia, dies,
            md->piscina);
    }
    si (!info)
    {
        redde;
    }
    rectangulum(md, IV, KALENDARIUM_LINEA + III, V, V, color_liturgicus(
        info->color_diei), VERUM);
    si (info->celebratio_principalis)
    {
        mandata_textus(md, XII, KALENDARIUM_LINEA + I, praecidere(
            info->celebratio_principalis->titulus, c->fines.latitudo
            - XVI), ZEPHYRUM, figura_color_thematis(COLOR_TEXT));
    }
    mandata_textus(md, IV, II * KALENDARIUM_LINEA + II, praecidere(
        calendarium_nomen_temporis(info->tempus_info.tempus, FALSUM,
        md->piscina), c->fines.latitudo - VIII), ZEPHYRUM,
        figura_color_thematis(COLOR_TEXT_DIM));
}

/* <figura/> <purus/> prompta: genus, textus (solutus, in piscinam
 * mandatorum transcriptus), cursor */
interior vacuum
figura_promptae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
     KalendariumFigurae* contextus;
                 chorda  genus;
                 chorda  linea;
                 chorda  textus;
                 chorda* a;
              character  crudus[KALENDARIUM_PROMPTA];
     constans character* nomen_generis;
                    s32  cursor;
                    i32  n;

    (vacuum)thema;
    contextus = (KalendariumFigurae*)ctx;
    rectangulum(md, ZEPHYRUM, ZEPHYRUM, c->fines.latitudo,
        c->fines.altitudo, figura_color_thematis(COLOR_SELECTION),
        VERUM);
    si (   !contextus || !contextus->m || !prompta_aperta(contextus->m,
            &genus))
    {
        redde;
    }
    nomen_generis = chorda_aequalis_literis(genus, "nova") ? "new: "
                  : chorda_aequalis_literis(genus, "nota") ? "note: "
                  : chorda_aequalis_literis(genus,
                  "titulus") ? "title: "
                  : "date: ";
    n               = promptae_textus(contextus->m, crudus);
    textus.datum    = (i8*)crudus;
    textus.mensura  = n;
    linea = chorda_concatenare(chorda_ex_literis(nomen_generis,
        md->piscina), chorda_transcribere(textus, md->piscina),
        md->piscina);
    mandata_textus(md, IV, I, praecidere(linea, c->fines.latitudo
        - VIII), ZEPHYRUM, figura_color_thematis(COLOR_TEXT));
    a = insula_ramus_attributum(&contextus->m->ramus, "prompta_cursor");
    si (!a || !chorda_ut_s32(*a, &cursor) || cursor < ZEPHYRUM)
    {
        cursor = (s32)n;
    }
    rectangulum(md, IV + ((s32)strlen(nomen_generis) + cursor)
        * KALENDARIUM_LITTERA, I, I, KALENDARIUM_LINEA,
        figura_color_thematis(
        COLOR_CURSOR), VERUM);
}

/* titulus visus agendae: "Agenda: <n> open" (res distinctae in
 * gregibus), "  #rubrica" sub filtro */
interior vacuum
titulum_agendae_pingere (
    KalendariumFigurae* contextus,
               Mandata* md,
    constans Componens* c)
{
                    Xar* ordines;
                 chorda* filtrum;
                 chorda  textus;
              character  numerus[XXXII];
                    i32  distinctae;
                    i32  i;
                    i32  j;

    ordines     = ordines_currentes(contextus->m, md->piscina);
    distinctae  = ZEPHYRUM;
    per (i = ZEPHYRUM; ordines && i < xar_numerus(ordines); i++)
    {
        per (j = ZEPHYRUM; j < i; j++)
        {
            si (   *(constans AgendaRes**)xar_obtinere(ordines, j)
                == *(constans AgendaRes**)xar_obtinere(ordines, i))
            {
                frange;
            }
        }
        si (j == i)
        {
            distinctae++;
        }
    }
    sprintf(numerus, "Agenda: %u open", (insignatus integer)distinctae);
    textus   = chorda_ex_literis(numerus, md->piscina);
    filtrum  = insula_ramus_attributum(&contextus->m->ramus,
        "rubrica_filtrum");
    si (filtrum)
    {
        textus = chorda_concatenare(textus, chorda_ex_literis("  #",
            md->piscina), md->piscina);
        textus = chorda_concatenare(textus, *filtrum, md->piscina);
    }
    mandata_textus(md, IV, ZEPHYRUM, praecidere(textus,
        c->fines.latitudo - VIII), ZEPHYRUM,
        figura_color_thematis(COLOR_TEXT));
}

/* <figura/> <purus/> titulus: "October 2026 · tempus", deinde litterae
 * dierum sub columnis */
interior vacuum
figura_tituli (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
                        character  litterae[VIII];
               KalendariumFigurae* contextus;
                   InformatioDiei* info;
                             Dies  electa;
                        character  titulus[LXIV];
                           chorda  textus;
                              s32  columna;
                              i32  j;

    (vacuum)thema;
    contextus = (KalendariumFigurae*)ctx;
    si (!contextus || !contextus->m)
    {
        redde;
    }
    electa = kalendarium_dies_electus(contextus->m);
    si (visus_diei(contextus->m))
    {
        titulum_diei_pingere(contextus, md, c, electa);
        redde;
    }
    si (visus_agendae(contextus->m))
    {
        titulum_agendae_pingere(contextus, md, c);
        redde;
    }
    sprintf(titulus, "%s %d", menses_anglice[electa.mensis - I],
        (integer)electa.annus);
    textus  = chorda_ex_literis(titulus, md->piscina);
    info    = NIHIL;
    si (contextus->liturgia)
    {
        info = calendarium_obtinere_diem(contextus->liturgia, electa,
            md->piscina);
    }
    si (info)
    {
        textus = chorda_concatenare(textus, chorda_ex_literis(" - ",
            md->piscina), md->piscina);
        textus = chorda_concatenare(textus, calendarium_nomen_temporis(
            info->tempus_info.tempus, FALSUM, md->piscina),
            md->piscina);
    }
    mandata_textus(md, IV, ZEPHYRUM, praecidere(textus,
        c->fines.latitudo - VIII), ZEPHYRUM,
        figura_color_thematis(COLOR_TEXT));
    columna = c->fines.latitudo / VII;
    strcpy(litterae, "SMTWTFS");
    per (j = ZEPHYRUM; j < VII; j++)
    {
        /* mandata textum per indicem servant: littera in memoriam
         * mandatorum transcribitur (acervus figurae post reditum
         * perit) */
        textus.datum    = (i8*)(litterae + j);
        textus.mensura  = I;
        textus          = chorda_transcribere(textus, md->piscina);
        mandata_textus(md, (s32)j * columna + (columna
            - KALENDARIUM_LITTERA) / II, KALENDARIUM_LINEA, textus,
            ZEPHYRUM, figura_color_thematis(COLOR_TEXT_DIM));
    }
}

interior b32
figuras_registrare (
    KalendariumMontatio* m)
{
    KalendariumFigurae* contextus;

    contextus = (KalendariumFigurae*)piscina_allocare(m->piscina,
        magnitudo(KalendariumFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    contextus->m         = m;
    contextus->liturgia  = calendarium_creare(m->piscina);
    contextus->scheda    = piscina_generare_dynamicum(
        "kalendarium_scheda", XVI * M);
    m->figurae_ctx       = contextus;
    redde figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_tabulae, contextus)
        && figura_registrare(m->figurae, PARTES_TITULUS, ZEPHYRUM,
               figura_tituli, contextus)
        && figura_registrare(m->figurae, PARTES_CAMPUS, ZEPHYRUM,
               figura_promptae, contextus);
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
kalendarium_montare (
     KalendariumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
      constans character* argumentum,
        AgendaDocumentum* documentum,
        KalendariumHodie  hodie_porta,
                  vacuum* hodie_ctx,
         StatusSystemata* systemata)
{
    Organum* organum;
        s32  annus;
        s32  mensis;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    si (   argumentum && strcmp(argumentum, "agenda") != ZEPHYRUM
        && !mensem_legere(argumentum, &annus, &mensis))
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(KalendariumMontatio));
    m->piscina     = piscina;
    m->intern      = intern;
    m->documentum  = documentum;
    m->hodie       = hodie_porta;
    m->hodie_ctx   = hodie_ctx;
    m->ramus       = id ? insula_ramus(repo, "calendarium", id)
                        : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "calendarium",
        "kalendarium");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"kalendarium\"", id ? id : "calendarium",
               piscina, intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae || !mandata_verificare()
        || !actio_registrare(m->actiones, actio_calendarii, agere, m)
        || !figuras_registrare(m))
    {
        redde FALSUM;
    }
    si (argumentum)
    {
        si (strcmp(argumentum, "agenda") == ZEPHYRUM)
        {
            redde campum_scribere(m, "visus", "agenda");
        }
        redde diem_scribere(m, dies_argumenti(m, annus, mensis));
    }
    redde VERUM;
}

b32
kalendarium_argumentum_ponere (
    KalendariumMontatio* m,
     constans character* argumentum)
{
    s32 annus;
    s32 mensis;

    si (m && argumentum && strcmp(argumentum, "agenda") == ZEPHYRUM)
    {
        redde campum_scribere(m, "visus", "agenda");
    }
    si (!m || !mensem_legere(argumentum, &annus, &mensis))
    {
        redde FALSUM;
    }
    redde diem_scribere(m, dies_argumenti(m, annus, mensis));
}

Dies
kalendarium_dies_electus (
    constans KalendariumMontatio* m)
{
     chorda* a;
       Dies  d;

    a = insula_ramus_attributum(&m->ramus, "dies_electus");
    si (a && fasti_parsare_iso(*a, &d) && fasti_dies_valida(d))
    {
        redde d;
    }
    redde hodie(m);
}


/* ==================================================
 * Componere
 * ================================================== */

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  latitudo,
                    s32  altitudo)
{
    Componens* c;
        Fines  f;

    c = componens_creare(p, in, id, partes);
    si (!c)
    {
        redde NIHIL;
    }
    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    componens_ponere_fines(c, f);
    redde c;
}

/* visus diei: titulus (lineae III) + ordo per rem ("r<id>"), altitudine
 * linea tituli et linea per notam */
interior vacuum
diem_componere (
    KalendariumMontatio* m,
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                    s32  latitudo,
                    s32  altitudo)
{
                    Xar* l;
     constans AgendaRes* r;
              Componens* ordo;
              character  id[XVI];
                    s32  y;
                    s32  alt_ordinis;
                    i32  i;

    componens_addere_liberum(radix, nodus(piscina, intern, "titulus",
        PARTES_TITULUS, ZEPHYRUM, ZEPHYRUM, latitudo,
        KALENDARIUM_TITULI_DIEI));
    l = ordines_diei(m, kalendarium_dies_electus(m), piscina);
    y = KALENDARIUM_TITULI_DIEI;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        r            = *(constans AgendaRes**)xar_obtinere(l, i);
        alt_ordinis  = (s32)(I + xar_numerus(r->notae))
                     * KALENDARIUM_LINEA + II;
        sprintf(id, "r%d", (integer)r->id);
        ordo = nodus(piscina, intern, id, PARTES_ITEM, ZEPHYRUM, y,
            latitudo, alt_ordinis);
        si (ordo)
        {
            componens_ponere_actio(ordo, actio_calendarii);
            componens_addere_liberum(radix, ordo);
        }
        y += alt_ordinis;
    }
    /* prompta (A3b): linea ultima dum aperta */
    si (prompta_aperta(m, NIHIL))
    {
        componens_addere_liberum(radix, nodus(piscina, intern,
            "prompta",
            PARTES_CAMPUS, ZEPHYRUM, altitudo - KALENDARIUM_LINEA - II,
            latitudo, KALENDARIUM_LINEA + II));
    }
}

/* visus agendae: titulus (linea una), per gregem titulus gregis
 * ("g<k>", PARTES_INDEX) et ordines "r<id>-<k>" (res electa cum notis
 * suis altior) */
interior vacuum
agendam_componere (
    KalendariumMontatio* m,
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                    s32  latitudo,
                    s32  altitudo)
{
                    Xar* greges;
            GrexAgendae* g;
     constans AgendaRes* r;
              Componens* ordo;
              character  id[XXXII];
                    s32  y;
                    s32  alt_ordinis;
                    s32  electa;
                    i32  i;
                    i32  j;

    componens_addere_liberum(radix, nodus(piscina, intern, "titulus",
        PARTES_TITULUS, ZEPHYRUM, ZEPHYRUM, latitudo,
        KALENDARIUM_TITULI_ALTITUDO));
    greges  = greges_agendae(m, piscina);
    electa  = res_electa(m, ordines_currentes(m, piscina));
    y       = KALENDARIUM_TITULI_ALTITUDO;
    per (i = ZEPHYRUM; greges && i < xar_numerus(greges); i++)
    {
        g = (GrexAgendae*)xar_obtinere(greges, i);
        sprintf(id, "g%d", (integer)i);
        componens_addere_liberum(radix, nodus(piscina, intern, id,
            PARTES_INDEX, ZEPHYRUM, y, latitudo, KALENDARIUM_LINEA
                + II));
        y += KALENDARIUM_LINEA + II;
        per (j = ZEPHYRUM; j < xar_numerus(g->res); j++)
        {
            r = *(constans AgendaRes**)xar_obtinere(g->res, j);
            alt_ordinis = (s32)(I + (r->id == electa
                ? xar_numerus(r->notae) : ZEPHYRUM)) * KALENDARIUM_LINEA
                + II;
            sprintf(id, "r%d-%d", (integer)r->id, (integer)i);
            ordo = nodus(piscina, intern, id, PARTES_ITEM, ZEPHYRUM, y,
                latitudo, alt_ordinis);
            si (ordo)
            {
                componens_ponere_actio(ordo, actio_calendarii);
                componens_addere_liberum(radix, ordo);
            }
            y += alt_ordinis;
        }
    }
    si (prompta_aperta(m, NIHIL))
    {
        componens_addere_liberum(radix, nodus(piscina, intern,
            "prompta",
            PARTES_CAMPUS, ZEPHYRUM, altitudo - KALENDARIUM_LINEA - II,
            latitudo, KALENDARIUM_LINEA + II));
    }
}

/* visus mensis: titulus et cellulae XLII */
interior vacuum
mensem_componere (
    KalendariumMontatio* m,
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                    s32  latitudo,
                    s32  altitudo)
{
              Componens* cellula;
                   Dies  electus;
                   Dies  initium;
                   Dies  dies;
                    s32  lat_cellulae;
                    s32  alt_cellulae;
                    i32  k;

    componens_addere_liberum(radix, nodus(piscina, intern, "titulus",
        PARTES_TITULUS, ZEPHYRUM, ZEPHYRUM, latitudo,
        KALENDARIUM_TITULI_ALTITUDO));
    /* VI hebdomades x VII, dominica prima ante aut in die I */
    electus = kalendarium_dies_electus(m);
    initium = fasti_dies(electus.annus, electus.mensis, I);
    initium       = fasti_addere_dies(initium,
        -fasti_dies_hebdomadis(initium));
    lat_cellulae = latitudo / VII;
    alt_cellulae = (altitudo - KALENDARIUM_TITULI_ALTITUDO) / VI;
    per (k = ZEPHYRUM; k < XLII; k++)
    {
        dies     = fasti_addere_dies(initium, (s32)k);
        cellula  = nodus(piscina, intern, chorda_ut_cstr(
            fasti_formare_diem(dies, FASTI_FORMA_ISO, piscina),
            piscina), PARTES_ITEM,
            (s32)(k % VII) * lat_cellulae, KALENDARIUM_TITULI_ALTITUDO
            + (s32)(k / VII) * alt_cellulae, lat_cellulae,
            alt_cellulae);
        si (cellula)
        {
            componens_ponere_actio(cellula, actio_calendarii);
            componens_addere_liberum(radix, cellula);
        }
    }
}

/* liberi radicis intra marginem mensae */
interior vacuum
liberos_movere (
     Componens* radix,
           s32  dx,
           s32  dy)
{
     Componens* l;
         Fines  f;
           i32  i;

    per (i = ZEPHYRUM; i < componens_numerus_liberorum(radix); i++)
    {
        l    = componens_liberum(radix, i);
        f    = l->fines;
        f.x  += dx;
        f.y  += dy;
        componens_ponere_fines(l, f);
    }
}

/* <componens/> <purus/> */
Componens*
kalendarium_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    KalendariumMontatio* m;
              Componens* radix;
                    s32  latitudo;
                    s32  altitudo;

    (vacuum)repo;
    m = (KalendariumMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo  = attributum_s32(&m->ramus, "superficies_latitudo",
        KALENDARIUM_LATITUDO);
    altitudo  = attributum_s32(&m->ramus, "superficies_altitudo",
        KALENDARIUM_ALTITUDO);
    radix = nodus(piscina, intern, "kalendarium", PARTES_TABULA,
        ZEPHYRUM, ZEPHYRUM, latitudo, altitudo);
    si (!radix)
    {
        redde NIHIL;
    }
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_calendarii);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    /* visus intra mensam: area interior, deinde liberi moventur */
    latitudo -= II * KALENDARIUM_MARGO_X;
    altitudo -= II * KALENDARIUM_MARGO_Y;
    si (visus_agendae(m))
    {
        agendam_componere(m, radix, piscina, intern, latitudo,
            altitudo);
    }
    alioquin si (visus_diei(m))
    {
        diem_componere(m, radix, piscina, intern, latitudo, altitudo);
    }
    alioquin
    {
        mensem_componere(m, radix, piscina, intern, latitudo, altitudo);
    }
    liberos_movere(radix, KALENDARIUM_MARGO_X, KALENDARIUM_MARGO_Y);
    redde radix;
}
