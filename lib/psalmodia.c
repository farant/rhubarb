/* psalmodia.c - aptator: textus punctatus + tonus -> versus syllabarum
 *
 * Regulae in capite (include/psalmodia.h); mensura in
 * lib/psalmodia.worklog.md. Gradus: lineae (BOM, CR, commentaria,
 * numeri) -> signa (verba, '*', '†' aut '+', octeti omissi) -> verba
 * (syllabae_latinae aut VOCES_HEBRAICAE) -> versus (partes,
 * admonitiones) -> aptatio.
 *
 * Aptatio (planum T5): per partem (ante flexam, ante mediationem,
 * semiversus secundus) tenor, intonatio, clausula retrorsum (REGULA
 * ACCENTUS capitis); regulae textus deficientis et intonationis
 * occurrentis in capite.
 */

#include "psalmodia.h"
#include "syllabae_latinae.h"
#include "utf8.h"
#include "runae.h"
#include <string.h>

/* clavis vocis hebraicae: litterae, non octeti */
#define CLAVIS_MAXIMA  XVI


/* ==================================================
 * Voces hebraicae (decretum Frani 1)
 * ================================================== */

/* clavis: litterae minusculae, j -> i, sine signis; divisio:
 * litterae per syllabam; accentus: syllaba si acutus abest */
nomen structura {
     constans character* clavis;
     constans character* divisio;
                    s32  accentus;
} VoxHebraica;

hic_manens constans VoxHebraica VOCES_HEBRAICAE[] = {
    { "israel",    "222",  0 },     /* Ís-ra-el */
    { "iacob",     "23",   0 },     /* Ja-cob */
    { "sion",      "22",   0 },     /* Si-on */
    { "david",     "23",   0 },     /* Da-vid */
    { "aaron",     "113",  0 },     /* Á-a-ron */
    { "iuda",      "22",   0 },     /* Ju-da */
    { "ephrata",   "142",  0 },     /* É-phra-ta */
    { "basan",     "23",   1 },     /* Ba-sán */
    { "sina",      "22",   1 },     /* Si-ná */
    { "ierusalem", "2223", 1 },     /* Je-rú-sa-lem */
    { "sabaoth",   "223",  0 },     /* Sá-ba-oth */
    { "cedar",     "23",   0 },     /* Ce-dar */
    { NIHIL,       NIHIL,  0 }
};

hic_manens constans character CIFRAE_SEDECIM[] = "0123456789ABCDEF";

/* Gloria Patri (decretum Frani 3): versus duo ultimi */
hic_manens constans character GLORIA_PATRI[] =
    "Glória Patri, et Fílio, * et Spirítui Sancto.\n"
    "Sicut erat in princípio, et nunc, et semper, * et in sǽcula "
    "sæculórum. Amen.\n";


/* ==================================================
 * Typi interni
 * ================================================== */

nomen enumeratio {
    SIGNUM_VERBUM,
    SIGNUM_MEDIATIO,
    SIGNUM_FLEXA
} SignumGenus;

/* signum lineae: verbum (sectio textus, aut copia sine octetis
 * omissis) aut signum divisionis */
nomen structura {
    SignumGenus genus;
         chorda textus;
            i32 columna;
            b32 servatum;   /* signum divisionis non omissum */
} SignumTextus;

nomen structura {
    VerbumLatinum divisio;
              b32 hebraica;
              b32 abrupta;   /* hebraica aut monosyllaba */
              i32 prima;     /* index syllabae primae in versu */
} VerbumLectum;

/* versus lectus, ante aptationem (notae vacuae) */
nomen structura {
    PsalmodiaVersus  versus;
       VerbumLectum* verba;
                i32  numerus_verborum;
                s32* verbum;      /* per syllabam; -1 = signum */
                s32  flexa;       /* index syllabae '†'; -1 */
                s32  mediatio;    /* index syllabae '*'; -1 */
} VersusLectus;

nomen structura {
               Piscina* piscina;
    PsalmodiaAdmonitio* admonitiones;
                   i32  numerus;
                   i32  capacitas;
                   b32  pars;   /* psalmodia_partem_aptare: signa
                                 * omittuntur, asteriscus non
                                 * exspectatur */
} LectorPsalmi;


/* ==================================================
 * Admonitiones
 * ================================================== */

interior vacuum
_monere (
    LectorPsalmi* lector,
             i32  linea,
             i32  columna,
          chorda  causa)
{
    PsalmodiaAdmonitio* novae;
                   i32  capacitas;

    si (lector->numerus == lector->capacitas)
    {
        capacitas = lector->capacitas > ZEPHYRUM
            ? lector->capacitas * II : VIII;
        novae = (PsalmodiaAdmonitio*)piscina_allocare(lector->piscina,
            (memoriae_index)(capacitas
            * magnitudo(PsalmodiaAdmonitio)));
        si (lector->numerus > ZEPHYRUM)
        {
            memcpy(novae, lector->admonitiones, (size_t)lector->numerus
                * magnitudo(PsalmodiaAdmonitio));
        }
        lector->admonitiones  = novae;
        lector->capacitas     = capacitas;
    }
    lector->admonitiones[lector->numerus].linea    = linea;
    lector->admonitiones[lector->numerus].columna  = columna;
    lector->admonitiones[lector->numerus].causa    = causa;
    lector->numerus++;
}

interior vacuum
_monere_literis (
          LectorPsalmi* lector,
                   i32  linea,
                   i32  columna,
    constans character* causa)
{
    _monere(lector, linea, columna, chorda_ex_literis(causa,
        lector->piscina));
}

/* causa = ante + medium + post */
interior vacuum
_monere_tribus (
          LectorPsalmi* lector,
                   i32  linea,
                   i32  columna,
    constans character* ante,
                chorda  medium,
    constans character* post)
{
    ChordaAedificator* a = chorda_aedificator_creare(lector->piscina,
        LXIV);

    chorda_aedificator_appendere_literis(a, ante);
    chorda_aedificator_appendere_chorda(a, medium);
    chorda_aedificator_appendere_literis(a, post);
    _monere(lector, linea, columna, chorda_aedificator_finire(a));
}

/* admonitiones [ab, numerus) ordine columnarum (stabiliter) */
interior vacuum
_monita_ordinare (
    LectorPsalmi* lector,
             i32  ab)
{
    i32 i;
    i32 k;

    per (i = ab + I; i < lector->numerus; i++)
    {
        PsalmodiaAdmonitio a = lector->admonitiones[i];

        k = i;
        dum (k > ab && lector->admonitiones[k - I].columna > a.columna)
        {
            lector->admonitiones[k] = lector->admonitiones[k - I];
            k--;
        }
        lector->admonitiones[k] = a;
    }
}


/* ==================================================
 * Characteres
 * ================================================== */

interior b32
_spatium (
    s32 cp)
{
    redde cp == ' ' || cp == '\t' || cp == '\v' || cp == '\f'
        || cp == 0xA0;
}

/* signa quae gabc in textu syllabae non patitur ('$' characterem
 * sequentem effugit, '~' = U+203F: lector gabc, T6) */
interior b32
_signum_gabc (
    s32 cp)
{
    redde cp == '(' || cp == ')' || cp == '[' || cp == ']' || cp == '{'
        || cp == '}' || cp == '<' || cp == '>' || cp == '|'
        || cp == '%' || cp == '$' || cp == '~';
}

/* octetus malus aut signum moderans (praeter spatia) */
interior b32
_octetus_malus (
    s32 cp)
{
    redde cp < ZEPHYRUM || (cp < 0x20 && !_spatium(cp)) || cp == 0x7F;
}

interior b32
_omittendum (
    s32 cp)
{
    redde _signum_gabc(cp) || _octetus_malus(cp);
}

/* acutus praecompositus (á é í ó ú ý ǽ, maiusculae) aut
 * U+0301 */
interior b32
_acutus (
    s32 cp)
{
    redde cp == 0xE1 || cp == 0xE9 || cp == 0xED || cp == 0xF3
        || cp == 0xFA || cp == 0xFD || cp == 0xC1 || cp == 0xC9
        || cp == 0xCD || cp == 0xD3 || cp == 0xDA || cp == 0xDD
        || cp == 0x1FC || cp == 0x1FD || cp == 0x301;
}

/* littera Latina (basis a-z, æ, œ): 'a'..'z' minuscula; '#' alia
 * littera; 0 non littera */
interior character
_littera (
    s32 cp)
{
    s32 basis;

    si (cp < ZEPHYRUM)
    {
        redde 0;
    }
    basis = runae_basis(cp);
    si (basis >= 'A' && basis <= 'Z')
    {
        basis = basis - 'A' + 'a';
    }
    si (basis >= 'a' && basis <= 'z')
    {
        redde (character)(basis == 'j' ? 'i' : basis);
    }
    si (   basis == 0xE6 || basis == 0xC6 || basis == 0x153
        || basis == 0x152)
    {
        redde '#';
    }
    redde 0;
}

/* signum combinatum (U+0300..U+036F) litterae priori adhaeret */
interior b32
_combinatum (
    s32 cp)
{
    redde cp >= 0x300 && cp <= 0x36F;
}

/* characteres (runae) in octetis [0, mensura) textus validi */
interior i32
_characteres (
    constans i8* datum,
            i32  mensura)
{
    i32 i;
    i32 numerus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        si (!utf8_est_continuatio(datum[i]))
        {
            numerus++;
        }
    }
    redde numerus;
}

/* a + " " + b */
interior chorda
_iungere (
     Piscina* piscina,
      chorda  a,
      chorda  b)
{
    chorda exitus;

    exitus.mensura  = a.mensura + I + b.mensura;
    exitus.datum    = (i8*)piscina_allocare(piscina,
        (memoriae_index)exitus.mensura);
    si (a.mensura > ZEPHYRUM)
    {
        memcpy(exitus.datum, a.datum, (size_t)a.mensura);
    }
    exitus.datum[a.mensura] = ' ';
    si (b.mensura > ZEPHYRUM)
    {
        memcpy(exitus.datum + a.mensura + I, b.datum,
            (size_t)b.mensura);
    }
    redde exitus;
}

/* verbum sine interpunctione initiali et finali (admonitiones) */
interior chorda
_verbum_nudum (
    chorda verbum)
{
     constans i8* p      = verbum.datum;
     constans i8* finis  = verbum.datum + verbum.mensura;
     constans i8* ante;
             s32  cp;
             s32  initium = -I;
             i32  ultimum = ZEPHYRUM;

    dum (p < finis)
    {
        ante  = p;
        cp    = utf8_decodere(&p, finis);
        si (_littera(cp) != 0 || (_combinatum(cp) && initium >= 0))
        {
            si (initium < 0)
            {
                initium = (s32)(ante - verbum.datum);
            }
            ultimum = (i32)(p - verbum.datum);
        }
    }
    si (initium < 0)
    {
        redde verbum;
    }
    redde chorda_sectio(verbum, (i32)initium, ultimum);
}


/* ==================================================
 * Verba
 * ================================================== */

/* vox hebraica? clavis ex litteris verbi; litterae[] initia
 * litterarum accipit; cogit VERUM si '-' inter litteras stat */
interior constans VoxHebraica*
_hebraicam_quaerere (
    chorda  verbum,
       i32* initia,
       i32* numerus_litterarum,
       s32* acutus,
       b32* cogit)
{
      character  clavis[CLAVIS_MAXIMA + I];
    constans i8* p      = verbum.datum;
    constans i8* finis  = verbum.datum + verbum.mensura;
    constans i8* ante;
            i32  n        = ZEPHYRUM;
            b32  lineola  = FALSUM;
            s32  cp;
      character  littera;
            i32  k;

    *acutus  = -I;
    *cogit   = FALSUM;
    dum (p < finis)
    {
        ante     = p;
        cp       = utf8_decodere(&p, finis);
        littera  = _littera(cp);
        si (littera != 0)
        {
            si (n >= CLAVIS_MAXIMA)
            {
                redde NIHIL;
            }
            si (lineola && n > ZEPHYRUM)
            {
                *cogit = VERUM;
            }
            lineola    = FALSUM;
            initia[n]  = (i32)(ante - verbum.datum);
            clavis[n]  = littera;
            n++;
            si (_acutus(cp))
            {
                *acutus = (s32)(n - I);
            }
        }
        alioquin si (cp == 0x301 && n > ZEPHYRUM)
        {
            *acutus = (s32)(n - I);
        }
        alioquin si (cp == '-')
        {
            lineola = VERUM;
        }
    }
    clavis[n]            = '\0';
    *numerus_litterarum  = n;
    per (k = ZEPHYRUM; VOCES_HEBRAICAE[k].clavis != NIHIL; k++)
    {
        si (strcmp(VOCES_HEBRAICAE[k].clavis, clavis) == ZEPHYRUM)
        {
            redde &VOCES_HEBRAICAE[k];
        }
    }
    redde NIHIL;
}

/* syllabae vocis hebraicae ex divisione indicis */
interior vacuum
_hebraicam_dividere (
                 Piscina* piscina,
                  chorda  verbum,
    constans VoxHebraica* vox,
            constans i32* initia,
                     s32  acutus,
           VerbumLatinum* exitus)
{
    i32 numerus = (i32)strlen(vox->divisio);
    i32 littera = ZEPHYRUM;
    i32 k;

    exitus->syllabae = (SyllabaLatina*)piscina_allocare(piscina,
        (memoriae_index)(numerus * magnitudo(SyllabaLatina)));
    exitus->numerus   = numerus;
    exitus->accentus  = vox->accentus;
    exitus->notatus   = acutus >= 0;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        i32 longitudo = (i32)(vox->divisio[k] - '0');

        exitus->syllabae[k].initium = k == ZEPHYRUM ? ZEPHYRUM
            : initia[littera];
        si (   acutus >= (s32)littera
            && acutus < (s32)(littera + longitudo))
        {
            exitus->accentus = (s32)k;
        }
        littera += longitudo;
        exitus->syllabae[k].finis = k + I == numerus ? verbum.mensura
            : initia[littera];
    }
}

/* verbum -> syllabae et accentus (voces hebraicae ex indice) */
interior vacuum
_verbum_dividere (
    LectorPsalmi* lector,
          chorda  verbum,
             i32  linea,
             i32  columna,
    VerbumLectum* exitus)
{
                     i32  initia[CLAVIS_MAXIMA];
                     i32  numerus_litterarum;
                     s32  acutus;
                     b32  cogit;
    constans VoxHebraica* vox;

    memset(exitus, ZEPHYRUM, magnitudo(VerbumLectum));
    vox = _hebraicam_quaerere(verbum, initia, &numerus_litterarum,
        &acutus, &cogit);
    si (vox != NIHIL && !cogit)
    {
        _hebraicam_dividere(lector->piscina, verbum, vox, initia,
            acutus,
            &exitus->divisio);
    }
    alioquin
    {
        (vacuum)syllabae_latinae_dividere(verbum, lector->piscina,
            &exitus->divisio);
        si (vox != NIHIL && !exitus->divisio.notatus)
        {
            exitus->divisio.accentus = vox->accentus
                < (s32)exitus->divisio.numerus ? vox->accentus : -I;
        }
    }
    exitus->hebraica = vox != NIHIL;
    exitus->abrupta = exitus->hebraica
        || exitus->divisio.numerus == I;
    si (   !exitus->hebraica && exitus->divisio.numerus >= III
        && !exitus->divisio.notatus)
    {
        _monere_tribus(lector, linea, columna, "verbum \"",
            _verbum_nudum(verbum), "\" sine accentu");
    }
}


/* ==================================================
 * Signa lineae
 * ================================================== */

/* verbum [initium, finis) lineae; octeti omissi intra -> copia */
interior chorda
_verbum_mundare (
     Piscina* piscina,
      chorda  linea,
         i32  initium,
         i32  finis)
{
     constans i8* p      = linea.datum + initium;
     constans i8* ultra  = linea.datum + finis;
     constans i8* ante;
             b32  mundum = VERUM;
          chorda  exitus;
             s32  cp;

    dum (p < ultra)
    {
        si (_omittendum(utf8_decodere(&p, ultra)))
        {
            mundum = FALSUM;
        }
    }
    si (mundum)
    {
        redde chorda_sectio(linea, initium, finis);
    }
    exitus.datum    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(finis - initium));
    exitus.mensura  = ZEPHYRUM;
    p               = linea.datum + initium;
    dum (p < ultra)
    {
        ante  = p;
        cp    = utf8_decodere(&p, ultra);
        si (!_omittendum(cp))
        {
            memcpy(exitus.datum + exitus.mensura, ante,
                (size_t)(p - ante));
            exitus.mensura += (i32)(p - ante);
        }
    }
    redde exitus;
}

/* linea (post numerum) -> signa; exitus NIHIL = numera tantum (sine
 * admonitionibus). finis_columna: columna post characterem ultimum
 * non spatium. Reddit numerum signorum. */
interior i32
_signa_legere (
    LectorPsalmi* lector,
          chorda  linea,
             i32  ab,
             i32  numerus_lineae,
             i32  columna,
    SignumTextus* exitus,
             i32* finis_columna)
{
     constans i8* p      = linea.datum + ab;
     constans i8* finis  = linea.datum + linea.mensura;
     constans i8* ante;
             s32  cp;
             i32  numerus    = ZEPHYRUM;
             s32  initium    = -I;      /* verbi currentis, octetus */
             i32  ultimum    = ZEPHYRUM; /* post characterem servatum */
             i32  col_verbi  = ZEPHYRUM;
             i32  positio;

    *finis_columna = columna;
    dum (p <= finis)
    {
        ante     = p;
        positio  = (i32)(p - linea.datum);
        cp       = p < finis ? utf8_decodere(&p, finis) : ' ';
        si (ante < finis && !_spatium(cp))
        {
            *finis_columna = columna + I;
        }
        si (   ante == finis || _spatium(cp) || cp == '*' || cp == '+'
            || cp   == 0x2020)
        {
            si (initium >= 0)
            {
                si (exitus != NIHIL)
                {
                    exitus[numerus].genus     = SIGNUM_VERBUM;
                    exitus[numerus].textus    = _verbum_mundare(
                        lector->piscina, linea, (i32)initium, ultimum);
                    exitus[numerus].columna   = col_verbi;
                    exitus[numerus].servatum  = VERUM;
                }
                numerus++;
                initium = -I;
            }
            si (ante < finis && !_spatium(cp))
            {
                si (exitus != NIHIL)
                {
                    exitus[numerus].genus = cp == '*' ? SIGNUM_MEDIATIO
                        : SIGNUM_FLEXA;
                    exitus[numerus].textus.datum    = NIHIL;
                    exitus[numerus].textus.mensura  = ZEPHYRUM;
                    exitus[numerus].columna         = columna;
                    exitus[numerus].servatum        = VERUM;
                }
                numerus++;
            }
            si (ante == finis)
            {
                frange;
            }
        }
        alioquin si (_omittendum(cp))
        {
            si (exitus != NIHIL)
            {
                si (_signum_gabc(cp))
                {
                    character signum[II];

                    signum[ZEPHYRUM]  = (character)cp;
                    signum[I]         = '\0';
                    _monere_tribus(lector, numerus_lineae, columna,
                        "signum gabc '", chorda_ex_literis(signum,
                        lector->piscina), "' omissum");
                }
                alioquin
                {
                    character cifrae[V];
                          i32 octetus;

                    octetus = (i32)linea.datum[positio] & 0xFF;

                    cifrae[ZEPHYRUM]  = '0';
                    cifrae[I]         = 'x';
                    cifrae[II]        = CIFRAE_SEDECIM[octetus >> IV];
                    cifrae[III]       = CIFRAE_SEDECIM[octetus & 0xF];
                    cifrae[IV]        = '\0';
                    _monere_tribus(lector, numerus_lineae, columna,
                        "octetus ", chorda_ex_literis(cifrae,
                        lector->piscina), " omissus");
                }
            }
        }
        alioquin
        {
            si (initium < 0)
            {
                initium    = (s32)positio;
                col_verbi  = columna;
            }
            ultimum = (i32)(p - linea.datum);
        }
        columna++;
    }
    redde numerus;
}


/* ==================================================
 * Versus
 * ================================================== */

interior PsalmodiaSyllaba*
_syllabam_addere (
    VersusLectus* versus,
             s32  verbum)
{
    i32 n = versus->versus.numerus++;

    memset(&versus->versus.syllabae[n], ZEPHYRUM,
        magnitudo(PsalmodiaSyllaba));
    versus->verbum[n] = verbum;
    redde &versus->versus.syllabae[n];
}

/* linea una -> versus lectus (notae vacuae); FALSUM si linea versum
 * non dat (vacua, commentarium, sine syllabis). Gloria: lineae et
 * columnae 0. */
interior b32
_versum_legere (
    LectorPsalmi* lector,
          chorda  linea,
             i32  numerus_lineae,
             b32  gloria,
    VersusLectus* exitus)
{
    SignumTextus* signa;
    VerbumLectum* lecta;
             i32  numerus_signorum;
             i32  ab       = ZEPHYRUM;
             i32  columna  = I;
             i32  col_prima;
             i32  finis_columna;
             i32  monita_ab             = lector->numerus;
             i32  numerus_syllabarum    = ZEPHYRUM;
             i32  numerus_verborum      = ZEPHYRUM;
             b32  mediatio              = FALSUM;
             b32  flexa                 = FALSUM;
             i32  syllabae_verborum     = ZEPHYRUM;  /* verborum */
             i32  syllabae_flexae       = ZEPHYRUM;  /* ante '†' */
             i32  syllabae_mediationis  = ZEPHYRUM; /* ante '*' */
          chorda  pendens;
             i32  col_pendentis  = ZEPHYRUM;
             s32  ultima_verbi   = -I;
             i32  k;
             i32  j;

    si (gloria)
    {
        numerus_lineae = ZEPHYRUM;
    }

    /* spatia initialia; commentarium */
    dum (   ab < linea.mensura && (linea.datum[ab] == ' '
        || linea.datum[ab] == '\t'))
    {
        ab++;
        columna++;
    }
    si (ab == linea.mensura || linea.datum[ab] == '%')
    {
        redde FALSUM;
    }
    col_prima = columna;

    /* numerus initio: cifrae, deinde '.' (quodlibet sequitur) aut
     * spatium aut finis */
    si (linea.datum[ab] >= '0' && linea.datum[ab] <= '9')
    {
        i32 q = ab;

        dum (   q < linea.mensura && linea.datum[q] >= '0'
             && linea.datum[q] <= '9')
        {
            q++;
        }
        si (q < linea.mensura && linea.datum[q] == '.')
        {
            q++;
        }
        alioquin si (   q < linea.mensura && linea.datum[q] != ' '
                     && linea.datum[q] != '\t')
        {
            q = ab;
        }
        columna  += q - ab;
        ab       = q;
    }

    /* signa: numerare, deinde legere */
    numerus_signorum = _signa_legere(lector, linea, ab, numerus_lineae,
        columna, NIHIL, &finis_columna);
    signa = (SignumTextus*)piscina_allocare(lector->piscina,
        (memoriae_index)((numerus_signorum + I)
        * magnitudo(SignumTextus)));
    lecta = (VerbumLectum*)piscina_allocare(lector->piscina,
        (memoriae_index)((numerus_signorum + I)
        * magnitudo(VerbumLectum)));
    (vacuum)_signa_legere(lector, linea, ab, numerus_lineae, columna,
        signa, &finis_columna);

    /* signa divisionis: '*' semel; '†' semel et ante '*' */
    per (k = ZEPHYRUM; k < numerus_signorum; k++)
    {
        si (lector->pars && signa[k].genus != SIGNUM_VERBUM)
        {
            signa[k].servatum = FALSUM;
            _monere_literis(lector, numerus_lineae, signa[k].columna,
                "signum in parte omissum");
        }
        alioquin si (signa[k].genus == SIGNUM_MEDIATIO)
        {
            si (mediatio)
            {
                signa[k].servatum = FALSUM;
                _monere_literis(lector, numerus_lineae,
                    signa[k].columna,
                    "asteriscus iteratus");
            }
            mediatio = VERUM;
        }
        alioquin si (signa[k].genus == SIGNUM_FLEXA)
        {
            si (mediatio)
            {
                signa[k].servatum = FALSUM;
                _monere_literis(lector, numerus_lineae,
                    signa[k].columna,
                    "flexa post asteriscum");
            }
            alioquin si (flexa)
            {
                signa[k].servatum = FALSUM;
                _monere_literis(lector, numerus_lineae,
                    signa[k].columna,
                    "flexa iterata");
            }
            flexa = VERUM;
        }
    }

    /* verba dividere */
    per (k = ZEPHYRUM; k < numerus_signorum; k++)
    {
        si (signa[k].genus == SIGNUM_VERBUM)
        {
            _verbum_dividere(lector, signa[k].textus, numerus_lineae,
                signa[k].columna, &lecta[k]);
            numerus_syllabarum += lecta[k].divisio.numerus;
            si (lecta[k].divisio.numerus > ZEPHYRUM)
            {
                numerus_verborum++;
            }
        }
        alioquin si (signa[k].servatum)
        {
            numerus_syllabarum++;
        }
    }
    si (numerus_verborum == ZEPHYRUM)
    {
        _monere_literis(lector, numerus_lineae, col_prima,
            "versus sine syllabis");
        _monita_ordinare(lector, monita_ab);
        redde FALSUM;
    }

    /* syllabae */
    exitus->versus.syllabae = (PsalmodiaSyllaba*)piscina_allocare(
        lector->piscina, (memoriae_index)(numerus_syllabarum
        * magnitudo(PsalmodiaSyllaba)));
    exitus->verbum = (s32*)piscina_allocare(lector->piscina,
        (memoriae_index)(numerus_syllabarum * magnitudo(s32)));
    exitus->verba = (VerbumLectum*)piscina_allocare(lector->piscina,
        (memoriae_index)(numerus_verborum * magnitudo(VerbumLectum)));
    exitus->numerus_verborum  = ZEPHYRUM;
    exitus->versus.numerus    = ZEPHYRUM;
    exitus->versus.linea      = numerus_lineae;
    exitus->versus.gloria     = gloria;
    exitus->flexa             = -I;
    exitus->mediatio          = -I;
    pendens.datum             = NIHIL;
    pendens.mensura           = ZEPHYRUM;
    mediatio                  = FALSUM;
    flexa                     = FALSUM;
    per (k = ZEPHYRUM; k < numerus_signorum; k++)
    {
            SignumTextus* s = &signa[k];
        PsalmodiaSyllaba* y;

        si (s->genus != SIGNUM_VERBUM)
        {
            si (!s->servatum)
            {
                perge;
            }
            y = _syllabam_addere(exitus, -I);
            y->textus = chorda_ex_literis(s->genus == SIGNUM_MEDIATIO
                ? "*" : "†", lector->piscina);
            y->notae           = y->textus;
            y->verbum_incipit  = VERUM;
            y->linea           = numerus_lineae;
            y->columna         = gloria ? ZEPHYRUM : s->columna;
            si (s->genus == SIGNUM_FLEXA)
            {
                exitus->flexa    = (s32)(exitus->versus.numerus - I);
                flexa            = VERUM;
                syllabae_flexae  = syllabae_verborum;
                si (syllabae_verborum == ZEPHYRUM)
                {
                    _monere_literis(lector, numerus_lineae, s->columna,
                        "pars ante flexam vacua");
                }
            }
            alioquin
            {
                exitus->mediatio = (s32)(exitus->versus.numerus - I);
                mediatio = VERUM;
                syllabae_mediationis = syllabae_verborum;
                si (syllabae_verborum
                    == (flexa ? syllabae_flexae : ZEPHYRUM))
                {
                    _monere_literis(lector, numerus_lineae, s->columna,
                        "semiversus vacuus");
                }
            }
            perge;
        }

        si (lecta[k].divisio.numerus == ZEPHYRUM)
        {
            /* verbum sine vocali (— « 3): syllabae priori adhaeret,
             * aut sequenti si prior nulla aut signum est */
            si (   exitus->versus.numerus > ZEPHYRUM
                && exitus->verbum[exitus->versus.numerus - I] >= 0)
            {
                y = &exitus->versus.syllabae[exitus->versus.numerus
                    - I];
                y->textus = _iungere(lector->piscina, y->textus,
                    s->textus);
            }
            alioquin si (pendens.mensura > ZEPHYRUM)
            {
                pendens = _iungere(lector->piscina, pendens, s->textus);
            }
            alioquin
            {
                pendens        = s->textus;
                col_pendentis  = s->columna;
            }
            perge;
        }

        {
                               i32  vi = exitus->numerus_verborum++;
            constans VerbumLatinum* d;

            exitus->verba[vi]        = lecta[k];
            exitus->verba[vi].prima  = exitus->versus.numerus;
            d                        = &exitus->verba[vi].divisio;
            per (j = ZEPHYRUM; j < d->numerus; j++)
            {
                y = _syllabam_addere(exitus, (s32)vi);
                y->textus = chorda_sectio(s->textus,
                    d->syllabae[j].initium, d->syllabae[j].finis);
                y->linea    = numerus_lineae;
                y->columna  = s->columna + _characteres(s->textus.datum,
                    d->syllabae[j].initium);
                si (j == ZEPHYRUM)
                {
                    y->verbum_incipit = VERUM;
                    si (pendens.mensura > ZEPHYRUM)
                    {
                        y->textus = _iungere(lector->piscina, pendens,
                            y->textus);
                        y->columna       = col_pendentis;
                        pendens.mensura  = ZEPHYRUM;
                    }
                }
                si (gloria)
                {
                    y->columna = ZEPHYRUM;
                }
            }
            ultima_verbi       = (s32)(exitus->versus.numerus - I);
            syllabae_verborum  += d->numerus;
        }
    }
    si (pendens.mensura > ZEPHYRUM && ultima_verbi >= 0)
    {
        exitus->versus.syllabae[ultima_verbi].textus = _iungere(
            lector->piscina,
            exitus->versus.syllabae[ultima_verbi].textus,
            pendens);
    }

    /* semiversus secundus */
    si (!mediatio && !lector->pars)
    {
        _monere_literis(lector, numerus_lineae, finis_columna,
            "sine asterisco");
    }
    si (   !lector->pars && syllabae_verborum == (mediatio
        ? syllabae_mediationis : flexa ? syllabae_flexae : ZEPHYRUM))
    {
        _monere_literis(lector, numerus_lineae, finis_columna,
            "semiversus vacuus");
    }
    _monita_ordinare(lector, monita_ab);
    redde VERUM;
}

/* textus -> versus lecti (BOM, LF, CRLF, CR) */
interior VersusLectus*
_textum_legere (
    LectorPsalmi* lector,
          chorda  textus,
             b32  gloria,
             i32* numerus)
{
    VersusLectus* versus;
             i32  lineae  = I;
             i32  p       = ZEPHYRUM;
             i32  q;
             i32  numerus_lineae = I;
             i32  k;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == '\n' || textus.datum[k] == '\r')
        {
            lineae++;
        }
    }
    versus = (VersusLectus*)piscina_allocare(lector->piscina,
        (memoriae_index)(lineae * magnitudo(VersusLectus)));
    *numerus = ZEPHYRUM;
    si (   textus.mensura  >= III && textus.datum[ZEPHYRUM] == (i8)0xEF
        && textus.datum[I] == (i8)0xBB && textus.datum[II] == (i8)0xBF)
    {
        p = III;
    }
    per (;;)
    {
        q = p;
        dum (   q < textus.mensura && textus.datum[q] != '\n'
             && textus.datum[q] != '\r')
        {
            q++;
        }
        si (_versum_legere(lector, chorda_sectio(textus, p, q),
            numerus_lineae, gloria, &versus[*numerus]))
        {
            (*numerus)++;
        }
        si (q >= textus.mensura)
        {
            frange;
        }
        si (   textus.datum[q]     == '\r' && q + I < textus.mensura
            && textus.datum[q + I] == '\n')
        {
            q++;
        }
        p = q + I;
        numerus_lineae++;
    }
    redde versus;
}


/* ==================================================
 * Aptatio
 * ================================================== */

/* soror monosyllaba si syllaba ultima partis verbum abruptum
 * claudit */
interior constans PsalmodiaClausula*
_clausulam_eligere (
        constans PsalmodiaTonus* tonus,
     constans PsalmodiaClausula* clausula,
          constans VersusLectus* versus,
                            s32  ultima)
{
    s32 verbum = versus->verbum[ultima];

    si (verbum >= 0 && versus->verba[verbum].abrupta)
    {
        redde psalmodia_clausulam_monosyllabam(tonus, clausula);
    }
    redde clausula;
}

/* syllaba i accentum verbi sui proxime sequitur (dó-MI-nus) */
interior b32
_post_accentum (
    constans VersusLectus* versus,
                      i32  i)
{
                      s32  verbum = versus->verbum[i];
    constans VerbumLectum* v;

    si (verbum < 0)
    {
        redde FALSUM;
    }
    v = &versus->verba[verbum];
    redde v->divisio.accentus >= 0
        && (s32)(i - v->prima) == v->divisio.accentus + I;
}

/* notae + 'r': superveniens, nota cava */
interior chorda
_cavam_facere (
    Piscina* piscina,
     chorda  notae)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, VIII);

    chorda_aedificator_appendere_chorda(a, notae);
    chorda_aedificator_appendere_character(a, 'r');
    redde chorda_aedificator_finire(a);
}

/* locus accentus -> <b>, praeparatio -> <i>, nota -> nullus */
interior PsalmodiaStilus
_stilus_loci (
    PsalmodiaLociGenus genus)
{
    si (genus == PSALMODIA_LOCUS_ACCENTUS)
    {
        redde PSALMODIA_STILUS_ACCENTUS;
    }
    si (genus == PSALMODIA_LOCUS_PRAEPARATIO)
    {
        redde PSALMODIA_STILUS_PRAEPARATIO;
    }
    redde PSALMODIA_STILUS_NULLUS;
}

/* clausula retrorsum in syllabas [infima, ad) (REGULA ACCENTUS).
 * prima: syllaba infima posita (ad si nulla). Reddit numerum locorum
 * initialium non positorum: syllabae defecerunt, aut accentus in
 * syllabam sub infima recederet. */
interior i32
_clausulam_ponere (
                       Piscina* piscina,
                  VersusLectus* versus,
    constans PsalmodiaClausula* clausula,
                           i32  infima,
                           i32  ad,
                           i32* prima)
{
    PsalmodiaSyllaba* syllabae  = versus->versus.syllabae;
                 s32  s         = (s32)ad - I;
                 s32  l;

    per (l = (s32)clausula->numerus_locorum - I; l >= ZEPHYRUM; l--)
    {
        constans PsalmodiaLocus* locus = &clausula->loci[l];

        si (s < (s32)infima)
        {
            *prima = (i32)(s + I);
            redde (i32)(l + I);
        }
        si (   locus->genus == PSALMODIA_LOCUS_ACCENTUS
            && locus->superveniens.mensura > ZEPHYRUM
            && _post_accentum(versus, (i32)s))
        {
            chorda cava;

            si (s - I < (s32)infima)
            {
                *prima = (i32)(s + I);
                redde (i32)(l + I);
            }
            cava = _cavam_facere(piscina, locus->superveniens);
            syllabae[(i32)s].notae = locus->anticipata ? locus->notae
                : cava;
            syllabae[(i32)(s - I)].notae = locus->anticipata ? cava
                : locus->notae;
            syllabae[(i32)(s - I)].stilus  = PSALMODIA_STILUS_ACCENTUS;
            s                              -= II;
        }
        alioquin
        {
            syllabae[(i32)s].notae   = locus->notae;
            syllabae[(i32)s].stilus  = _stilus_loci(locus->genus);
            s--;
        }
    }
    *prima = (i32)(s + I);
    redde ZEPHYRUM;
}

/* inter locos [0, reliqui) est locus non praeparatio */
interior b32
_reliqui_graves (
    constans PsalmodiaClausula* clausula,
                           i32  reliqui)
{
    i32 l;

    per (l = ZEPHYRUM; l < reliqui; l++)
    {
        si (clausula->loci[l].genus != PSALMODIA_LOCUS_PRAEPARATIO)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* mediatio deficiens: tenor + notae locorum reliquorum (praeter
 * praeparationes) + notae syllabae primae, in syllaba prima (I: Qui
 * = h ixi h) */
interior vacuum
_contrahere (
                       Piscina* piscina,
              PsalmodiaSyllaba* prima,
                        chorda  tenor,
    constans PsalmodiaClausula* clausula,
                           i32  reliqui)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, XXXII);
                  i32  l;

    chorda_aedificator_appendere_chorda(a, tenor);
    per (l = ZEPHYRUM; l < reliqui; l++)
    {
        si (clausula->loci[l].genus != PSALMODIA_LOCUS_PRAEPARATIO)
        {
            chorda_aedificator_appendere_chorda(a,
                clausula->loci[l].notae);
        }
    }
    chorda_aedificator_appendere_chorda(a, prima->notae);
    prima->notae = chorda_aedificator_finire(a);
}

/* flexa in syllabas [infima, ad): ultima, et paenultima proparoxytoni;
 * verbum abruptum in tenore; intonatio occurrens flexam omittit */
interior vacuum
_flexam_ponere (
     VersusLectus* versus,
           chorda  flexa,
              i32  infima,
              i32  ad)
{
                      s32  verbum;
    constans VerbumLectum* v;
                      b32  antepaenultima;

    si (flexa.mensura == ZEPHYRUM || ad <= infima)
    {
        redde;
    }
    verbum = versus->verbum[ad - I];
    si (verbum < 0 || versus->verba[verbum].abrupta)
    {
        redde;
    }
    v = &versus->verba[verbum];
    antepaenultima = v->divisio.numerus >= III
        && v->divisio.accentus == (s32)v->divisio.numerus - III;
    si (antepaenultima && ad < infima + II)
    {
        redde;
    }
    versus->versus.syllabae[ad - I].notae = flexa;
    si (antepaenultima)
    {
        versus->versus.syllabae[ad - II].notae = flexa;
    }
}

/* notae intonationis in syllabas [ab, ad), una per syllabam */
interior vacuum
_intonationem_ponere (
        VersusLectus* versus,
                 i32  ab,
                 i32  ad,
     constans chorda* notae)
{
    i32 i;

    per (i = ab; i < ad; i++)
    {
        versus->versus.syllabae[i].notae   = notae[i - ab];
        versus->versus.syllabae[i].stilus  = PSALMODIA_STILUS_NULLUS;
    }
}

/* pars una [ab, ad): tenor, intonatio, clausula. Terminatio: notae
 * primae deficientes omittuntur, intonatio syllabas a clausula
 * relictas capit. Ceterae: intonatio primum; mediatio deficiens in
 * syllabam primam contrahitur, aut omittitur si intonatio eam
 * occupat; flexa occupata omittitur. */
interior vacuum
_partem_ponere (
                       Piscina* piscina,
                  VersusLectus* versus,
                           i32  ab,
                           i32  ad,
                        chorda  tenor,
               constans chorda* intonatio,
                           i32  numerus_intonationis,
                PsalmodiaFinis  finis,
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* clausula)
{
    i32 infima;
    i32 prima;
    i32 reliqui;
    i32 i;

    si (ab >= ad)
    {
        redde;
    }
    per (i = ab; i < ad; i++)
    {
        versus->versus.syllabae[i].notae   = tenor;
        versus->versus.syllabae[i].stilus  = PSALMODIA_STILUS_NULLUS;
    }
    si (finis == PSALMODIA_FINIS_TERMINATIO)
    {
        prima = ad;
        si (clausula != NIHIL)
        {
            clausula = _clausulam_eligere(tonus, clausula, versus,
                (s32)(ad - I));
            (vacuum)_clausulam_ponere(piscina, versus, clausula, ab, ad,
                &prima);
        }
        _intonationem_ponere(versus, ab, ab + (numerus_intonationis
            < prima - ab ? numerus_intonationis : prima - ab),
            intonatio);
        redde;
    }
    infima = ab + (numerus_intonationis < ad - ab ? numerus_intonationis
        : ad - ab);
    _intonationem_ponere(versus, ab, infima, intonatio);
    si (finis == PSALMODIA_FINIS_FLEXA)
    {
        _flexam_ponere(versus, tonus->flexa, infima, ad);
    }
    si (finis != PSALMODIA_FINIS_MEDIATIO || clausula == NIHIL)
    {
        redde;
    }
    clausula = _clausulam_eligere(tonus, clausula, versus, (s32)(ad
        - I));
    reliqui = _clausulam_ponere(piscina, versus, clausula, infima, ad,
        &prima);
    si (!_reliqui_graves(clausula, reliqui))
    {
        redde;
    }
    si (infima > ab)
    {
        /* semiversus nimis brevis pro intonatione et mediatione */
        per (i = infima; i < ad; i++)
        {
            versus->versus.syllabae[i].notae = tenor;
            versus->versus.syllabae[i].stilus =
                PSALMODIA_STILUS_NULLUS;
        }
    }
    alioquin
    {
        _contrahere(piscina, &versus->versus.syllabae[ab], tenor,
            clausula, reliqui);
    }
}

/* versus: pars ante flexam (intonatio), ante mediationem (intonatio
 * si flexa nulla), semiversus secundus (tenor et intonatio
 * terminationis: peregrinus) */
interior vacuum
_versum_aptare (
                       Piscina* piscina,
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* terminatio,
    constans PsalmodiaOptiones* optiones,
                  VersusLectus* versus,
                           b32  intonatio)
{
             chorda tenor_secundus = terminatio->tenor.mensura
                         > ZEPHYRUM ? terminatio->tenor : tonus->tenor;
    constans chorda* initium         = tonus->initium;
                i32  numerus_initii  = intonatio ? tonus->numerus_initii
                         : ZEPHYRUM;
                i32 ab              = ZEPHYRUM;

    si (versus->flexa >= 0)
    {
        _partem_ponere(piscina, versus, ab, (i32)versus->flexa,
            tonus->tenor, initium, numerus_initii,
            PSALMODIA_FINIS_FLEXA,
            tonus, NIHIL);
        numerus_initii  = ZEPHYRUM;
        ab              = (i32)versus->flexa + I;
    }
    si (versus->mediatio >= 0)
    {
        _partem_ponere(piscina, versus, ab, (i32)versus->mediatio,
            tonus->tenor, initium, numerus_initii,
            PSALMODIA_FINIS_MEDIATIO, tonus,
            psalmodia_mediationem_eligere(tonus, optiones->antiqua));
        ab = (i32)versus->mediatio + I;
    }
    _partem_ponere(piscina, versus, ab, versus->versus.numerus,
        tenor_secundus, &terminatio->initium, intonatio
        && terminatio->initium.mensura > ZEPHYRUM ? I : ZEPHYRUM,
        PSALMODIA_FINIS_TERMINATIO, tonus, terminatio);
}


/* ==================================================
 * Functiones publicae
 * ================================================== */

PsalmodiaPsalmus*
psalmodia_aptare (
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* terminatio,
                        chorda  textus,
    constans PsalmodiaOptiones* optiones,
                       Piscina* piscina)
{
         LectorPsalmi  lector;
    PsalmodiaOptiones  ordinariae;
     PsalmodiaPsalmus* psalmus;
         VersusLectus* lecti            = NIHIL;
         VersusLectus* gloriae          = NIHIL;
                  i32  numerus          = ZEPHYRUM;
                  i32  numerus_gloriae  = ZEPHYRUM;
                  i32  k;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    si (optiones == NIHIL)
    {
        memset(&ordinariae, ZEPHYRUM, magnitudo(ordinariae));
        optiones = &ordinariae;
    }
    memset(&lector, ZEPHYRUM, magnitudo(lector));
    lector.piscina = piscina;
    psalmus = (PsalmodiaPsalmus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(PsalmodiaPsalmus));
    memset(psalmus, ZEPHYRUM, magnitudo(PsalmodiaPsalmus));
    psalmus->tonus       = tonus;
    psalmus->terminatio  = terminatio;

    si (tonus == NIHIL)
    {
        _monere_literis(&lector, ZEPHYRUM, ZEPHYRUM, "tonus nullus");
    }
    alioquin si (terminatio == NIHIL)
    {
        _monere_literis(&lector, ZEPHYRUM, ZEPHYRUM,
            "terminatio nulla");
    }
    alioquin
    {
        lecti = _textum_legere(&lector, textus, FALSUM, &numerus);
        si (!optiones->sine_gloria)
        {
            gloriae = _textum_legere(&lector, chorda_ex_literis(
                GLORIA_PATRI, piscina), VERUM, &numerus_gloriae);
        }
        psalmus->versus = (PsalmodiaVersus*)piscina_allocare(piscina,
            (memoriae_index)((numerus + numerus_gloriae + I)
            * magnitudo(PsalmodiaVersus)));
        per (k = ZEPHYRUM; k < numerus + numerus_gloriae; k++)
        {
            VersusLectus* v = k < numerus ? &lecti[k]
                : &gloriae[k - numerus];

            _versum_aptare(piscina, tonus, terminatio, optiones, v,
                optiones->canticum || (k == ZEPHYRUM
                && !optiones->sine_intonatione));
            psalmus->versus[psalmus->numerus_versuum++] = v->versus;
        }
    }
    psalmus->admonitiones          = lector.admonitiones;
    psalmus->numerus_admonitionum  = lector.numerus;
    redde psalmus;
}

PsalmodiaPsalmus*
psalmodia_partem_aptare (
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* clausula,
                PsalmodiaFinis  finis,
                           b32  intonatio,
                        chorda  textus,
                       Piscina* piscina)
{
        LectorPsalmi  lector;
    PsalmodiaPsalmus* psalmus;
        VersusLectus* lecti;
                 i32  numerus   = ZEPHYRUM;
              chorda  tenor;
     constans chorda* notae            = NIHIL;
                 i32  numerus_notarum  = ZEPHYRUM;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    memset(&lector, ZEPHYRUM, magnitudo(lector));
    lector.piscina  = piscina;
    lector.pars     = VERUM;
    psalmus = (PsalmodiaPsalmus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(PsalmodiaPsalmus));
    memset(psalmus, ZEPHYRUM, magnitudo(PsalmodiaPsalmus));
    psalmus->tonus       = tonus;
    psalmus->terminatio  = finis == PSALMODIA_FINIS_TERMINATIO
        ? clausula : NIHIL;

    si (tonus == NIHIL)
    {
        _monere_literis(&lector, ZEPHYRUM, ZEPHYRUM, "tonus nullus");
    }
    alioquin si (clausula == NIHIL && (finis == PSALMODIA_FINIS_MEDIATIO
                 || finis == PSALMODIA_FINIS_TERMINATIO))
    {
        _monere_literis(&lector, ZEPHYRUM, ZEPHYRUM, "clausula nulla");
    }
    alioquin
    {
        lecti = _textum_legere(&lector, textus, FALSUM, &numerus);
        si (numerus == ZEPHYRUM)
        {
            _monere_literis(&lector, I, I, "pars vacua");
        }
        alioquin
        {
            si (numerus > I)
            {
                _monere_literis(&lector, lecti[I].versus.linea, I,
                    "pars una exspectatur");
            }
            tenor = tonus->tenor;
            si (finis == PSALMODIA_FINIS_TERMINATIO)
            {
                si (clausula->tenor.mensura > ZEPHYRUM)
                {
                    tenor = clausula->tenor;
                }
                si (intonatio && clausula->initium.mensura > ZEPHYRUM)
                {
                    notae            = &clausula->initium;
                    numerus_notarum  = I;
                }
            }
            alioquin si (intonatio)
            {
                notae            = tonus->initium;
                numerus_notarum  = tonus->numerus_initii;
            }
            _partem_ponere(piscina, &lecti[ZEPHYRUM], ZEPHYRUM,
                lecti[ZEPHYRUM].versus.numerus, tenor, notae,
                numerus_notarum, finis, tonus, clausula);
            psalmus->versus =
                (PsalmodiaVersus*)piscina_allocare(piscina,
                (memoriae_index)magnitudo(PsalmodiaVersus));
            psalmus->versus[ZEPHYRUM]  = lecti[ZEPHYRUM].versus;
            psalmus->numerus_versuum   = I;
        }
    }
    psalmus->admonitiones          = lector.admonitiones;
    psalmus->numerus_admonitionum  = lector.numerus;
    redde psalmus;
}

/* valor capitis gabc: ';' et lineae novae (finis valoris) ->
 * spatium */
interior vacuum
_valorem_scribere (
               chorda  valor,
    ChordaAedificator* exitus)
{
    i32 k;

    per (k = ZEPHYRUM; k < valor.mensura; k++)
    {
        character c = (character)valor.datum[k];

        chorda_aedificator_appendere_character(exitus, c == ';'
            || c == '\n' || c == '\r' ? ' ' : c);
    }
}

/* syllaba signi: '*' -> ':' (divisio maior), '†' -> ',' (minima);
 * 0 = syllaba verbi */
interior character
_divisio_signi (
    constans PsalmodiaSyllaba* syllaba)
{
    si (syllaba->textus.mensura == I && syllaba->textus.datum[ZEPHYRUM]
        == '*')
    {
        redde ':';
    }
    si (   syllaba->textus.mensura == III
        && memcmp(syllaba->textus.datum, "\xE2\x80\xA0", III)
           == ZEPHYRUM)
    {
        redde ',';
    }
    redde 0;
}

b32
psalmodia_gabc_scribere (
     constans PsalmodiaPsalmus* psalmus,
    constans PsalmodiaOptiones* optiones,
             ChordaAedificator* exitus)
{
         constans PsalmodiaTonus* tonus;
      constans PsalmodiaClausula* terminatio;
                          chorda  clavis;
                             i32  numerus = ZEPHYRUM;
                             i32  v;
                             i32  i;

    si (psalmus == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    tonus       = psalmus->tonus;
    terminatio  = psalmus->terminatio;
    si (optiones != NIHIL && optiones->titulus.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(exitus, "name: ");
        _valorem_scribere(optiones->titulus, exitus);
        chorda_aedificator_appendere_literis(exitus, ";\n");
    }
    si (tonus != NIHIL)
    {
        chorda_aedificator_appendere_literis(exitus, "mode: ");
        chorda_aedificator_appendere_chorda(exitus, tonus->modus);
        chorda_aedificator_appendere_literis(exitus, ";\nannotation: ");
        chorda_aedificator_appendere_chorda(exitus, tonus->modus);
        si (   terminatio != NIHIL && terminatio->differentia.mensura
            > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(exitus, ' ');
            chorda_aedificator_appendere_chorda(exitus,
                terminatio->differentia);
        }
        /* versus primus numeratur: "1" non fiat littera initialis */
        chorda_aedificator_appendere_literis(exitus,
            ";\ninitial-style: 0;\n");
    }
    chorda_aedificator_appendere_literis(exitus, "%%\n");
    si (tonus == NIHIL)
    {
        redde VERUM;
    }

    /* clavis terminationis (II A: c3) aut toni, psalmo toto */
    clavis = terminatio != NIHIL
        && terminatio->clavis.mensura > ZEPHYRUM
        ? terminatio->clavis : tonus->clavis;
    chorda_aedificator_appendere_character(exitus, '(');
    chorda_aedificator_appendere_chorda(exitus, clavis);
    chorda_aedificator_appendere_character(exitus, ')');
    per (v = ZEPHYRUM; v < psalmus->numerus_versuum; v++)
    {
        constans PsalmodiaVersus* versus = &psalmus->versus[v];

        si (v == ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(exitus, ' ');
        }
        per (i = ZEPHYRUM; i < versus->numerus; i++)
        {
             constans PsalmodiaSyllaba* s        =
                 &versus->syllabae[i];
                             character divisio  = _divisio_signi(s);

            si (i > ZEPHYRUM && s->verbum_incipit)
            {
                chorda_aedificator_appendere_character(exitus, ' ');
            }
            si (i == ZEPHYRUM && !versus->gloria)
            {
                chorda_aedificator_appendere_i32(exitus, ++numerus);
                chorda_aedificator_appendere_literis(exitus, ". ");
            }
            si (divisio != 0)
            {
                chorda_aedificator_appendere_chorda(exitus, s->textus);
                chorda_aedificator_appendere_character(exitus, '(');
                chorda_aedificator_appendere_character(exitus, divisio);
                chorda_aedificator_appendere_character(exitus, ')');
                perge;
            }
            si (s->stilus == PSALMODIA_STILUS_ACCENTUS)
            {
                chorda_aedificator_appendere_literis(exitus, "<b>");
            }
            alioquin si (s->stilus == PSALMODIA_STILUS_PRAEPARATIO)
            {
                chorda_aedificator_appendere_literis(exitus, "<i>");
            }
            chorda_aedificator_appendere_chorda(exitus, s->textus);
            si (s->stilus == PSALMODIA_STILUS_ACCENTUS)
            {
                chorda_aedificator_appendere_literis(exitus, "</b>");
            }
            alioquin si (s->stilus == PSALMODIA_STILUS_PRAEPARATIO)
            {
                chorda_aedificator_appendere_literis(exitus, "</i>");
            }
            chorda_aedificator_appendere_character(exitus, '(');
            chorda_aedificator_appendere_chorda(exitus, s->notae);
            chorda_aedificator_appendere_character(exitus, ')');
        }
        chorda_aedificator_appendere_literis(exitus, " (::)\n");
    }
    redde VERUM;
}

b32
psalmodia_admonitiones_scribere (
    constans PsalmodiaPsalmus* psalmus,
           constans character* via,
            ChordaAedificator* exitus)
{
    i32 k;

    si (psalmus == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < psalmus->numerus_admonitionum; k++)
    {
        constans PsalmodiaAdmonitio* a = &psalmus->admonitiones[k];

        si (via != NIHIL)
        {
            chorda_aedificator_appendere_literis(exitus, via);
            chorda_aedificator_appendere_character(exitus, ':');
        }
        chorda_aedificator_appendere_i32(exitus, a->linea);
        chorda_aedificator_appendere_character(exitus, ':');
        chorda_aedificator_appendere_i32(exitus, a->columna);
        chorda_aedificator_appendere_literis(exitus, ": ");
        chorda_aedificator_appendere_chorda(exitus, a->causa);
        chorda_aedificator_appendere_character(exitus, '\n');
    }
    redde VERUM;
}
