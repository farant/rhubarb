/* psalmodia.c - aptator: textus punctatus + tonus -> versus syllabarum
 *
 * Regulae in capite (include/psalmodia.h); mensura in
 * lib/psalmodia.worklog.md. Gradus: lineae (BOM, CR, commentaria,
 * numeri) -> signa (verba, '*', '†' aut '+', octeti omissi) -> verba
 * (syllabae_latinae aut VOCES_HEBRAICAE) -> versus (partes,
 * admonitiones) -> aptatio.
 *
 * Aptatio hic adhuc provisoria (planum T4): tenor in omni syllaba,
 * nota ultima clausulae in syllaba ultima semiversus (soror
 * monosyllaba ubi semiversus abrupte desinit). Regula ipsa: T5.
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

/* signa quae gabc in textu syllabae non patitur */
interior b32
_signum_gabc (
    s32 cp)
{
    redde cp == '(' || cp == ')' || cp == '[' || cp == ']' || cp == '{'
        || cp == '}' || cp == '<' || cp == '>' || cp == '|'
        || cp == '%';
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
        si (signa[k].genus == SIGNUM_MEDIATIO)
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
    si (!mediatio)
    {
        _monere_literis(lector, numerus_lineae, finis_columna,
            "sine asterisco");
    }
    si (syllabae_verborum == (mediatio ? syllabae_mediationis
        : flexa ? syllabae_flexae : ZEPHYRUM))
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
 * Aptatio (provisoria, T4)
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

interior vacuum
_notam_ultimam (
    constans PsalmodiaClausula* clausula,
              PsalmodiaSyllaba* syllaba)
{
    si (clausula != NIHIL && clausula->numerus_locorum > ZEPHYRUM)
    {
        syllaba->notae = clausula->loci[clausula->numerus_locorum - I]
            .notae;
    }
}

interior vacuum
_versum_aptare (
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* terminatio,
    constans PsalmodiaOptiones* optiones,
                  VersusLectus* versus)
{
    chorda tenor_secundus = terminatio->tenor.mensura > ZEPHYRUM
        ? terminatio->tenor : tonus->tenor;
       s32 ultima_primae  = -I;
       s32 ultima         = -I;
       i32 i;

    per (i = ZEPHYRUM; i < versus->versus.numerus; i++)
    {
        PsalmodiaSyllaba* s = &versus->versus.syllabae[i];
                     b32  secunda;

        si (versus->verbum[i] < 0)
        {
            perge;
        }
        secunda   = versus->mediatio < 0 || (s32)i > versus->mediatio;
        s->notae  = secunda ? tenor_secundus : tonus->tenor;
        si (secunda)
        {
            ultima = (s32)i;
        }
        alioquin si ((s32)i > versus->flexa)
        {
            ultima_primae = (s32)i;
        }
    }
    si (ultima_primae >= 0)
    {
        _notam_ultimam(_clausulam_eligere(tonus,
            psalmodia_mediationem_eligere(tonus, optiones->antiqua),
            versus, ultima_primae), &versus->versus.syllabae
            [ultima_primae]);
    }
    si (ultima >= 0)
    {
        _notam_ultimam(_clausulam_eligere(tonus, terminatio, versus,
            ultima), &versus->versus.syllabae[ultima]);
    }
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
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            _versum_aptare(tonus, terminatio, optiones, &lecti[k]);
            psalmus->versus[psalmus->numerus_versuum++] =
                lecti[k].versus;
        }
        per (k = ZEPHYRUM; k < numerus_gloriae; k++)
        {
            _versum_aptare(tonus, terminatio, optiones, &gloriae[k]);
            psalmus->versus[psalmus->numerus_versuum++] =
                gloriae[k].versus;
        }
    }
    psalmus->admonitiones          = lector.admonitiones;
    psalmus->numerus_admonitionum  = lector.numerus;
    redde psalmus;
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
