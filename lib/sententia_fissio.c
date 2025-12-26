#include "sententia_fissio.h"
#include "utf8.h"
#include <string.h>

/* ==================================================
 * Constantae - Smart Quote Codepoints
 * ================================================== */

#define QUOTE_DOUBLE_SINISTER   0x201C  /* " */
#define QUOTE_DOUBLE_DEXTER     0x201D  /* " */
#define QUOTE_SINGLE_SINISTER   0x2018  /* ' */
#define QUOTE_SINGLE_DEXTER     0x2019  /* ' */
#define QUOTE_DOUBLE_RECTUS     '"'
#define QUOTE_SINGLE_RECTUS     '\''


/* ==================================================
 * Constantae - Abbreviationes
 * ================================================== */

interior constans character* ABBREVIATIONES[] = {
    /* Tituli */
    "mr", "mrs", "ms", "miss", "dr", "prof", "sr", "jr",
    "rev", "fr", "br", "sis",

    /* Academici */
    "ph.d", "phd", "m.d", "md", "b.a", "ba", "m.a", "ma", "b.s", "bs", "m.s",

    /* Abbreviationes Latinae */
    "i.e", "ie", "e.g", "eg", "etc", "vs", "cf", "viz", "al", "ex",

    /* Negotia */
    "inc", "ltd", "co", "corp",

    /* Litterae singulae (praenomina Romana) */
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
    "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",

    /* Abbreviationes datae Latinae */
    "a.u.c", "auc", "p.u.c", "puc",

    /* Adressum */
    "st", "ave", "rd", "blvd", "ct", "ln", "pl", "ph",

    /* Menses */
    "jan", "feb", "mar", "apr", "may", "jun", "jul",
    "aug", "sep", "sept", "oct", "nov", "dec",

    /* Miscellanea */
    "no", "vol", "pp", "ed", "misc",

    /* Militaria */
    "hon", "capt", "col", "gen", "lt", "maj", "sgt", "pvt", "cpl", "adm", "cmdr",

    /* Nationes */
    "u.s", "u.k", "uk", "u.s.a", "usa", "u.n", "un",

    /* Tempus */
    "a.m", "am", "p.m", "pm", "b.c", "bc", "a.d", "ad",

    /* Numeri Romani */
    "ii", "iii", "iv", "vi", "vii", "viii", "ix",
    "xi", "xii", "xiii", "xiv", "xv", "xvi", "xvii", "xviii", "xix", "xx",

    NIHIL  /* Terminator */
};


/* ==================================================
 * Functiones Auxiliares - Characteres
 * ================================================== */

interior b32
_est_littera(character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

interior b32
_est_cifra(character c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_est_spatium(character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior b32
_est_punctum_terminale(character c)
{
    redde c == '.' || c == '!' || c == '?';
}

interior b32
_est_maiuscula(character c)
{
    redde c >= 'A' && c <= 'Z';
}

interior b32
_est_minuscula(character c)
{
    redde c >= 'a' && c <= 'z';
}

interior character
_ad_minusculam(character c)
{
    si (_est_maiuscula(c))
    {
        redde (character)(c + ('a' - 'A'));
    }
    redde c;
}


/* ==================================================
 * Functiones Auxiliares - UTF-8 et Quotes
 * ================================================== */

/* Decodere codepoint ad positione */
interior s32
_decodere_runam(chorda textus, i32 index)
{
    constans i8* ptr;
    constans i8* finis;

    si (index < ZEPHYRUM || index >= textus.mensura)
    {
        redde -1;
    }

    ptr = (constans i8*)(textus.datum + index);
    finis = (constans i8*)(textus.datum + textus.mensura);

    redde utf8_decodere(&ptr, finis);
}

/* Obtinere longitudinem bytes runae ad positione */
interior i32
_longitudo_runae(chorda textus, i32 index)
{
    s32 result;

    si (index < ZEPHYRUM || index >= textus.mensura)
    {
        redde ZEPHYRUM;
    }

    result = utf8_longitudo_byte((i8)textus.datum[index]);
    redde (i32)result;
}

/* An codepoint est quote (rectus vel curvus) */
interior b32
_est_quote_runa(s32 codepoint)
{
    redde codepoint == QUOTE_DOUBLE_RECTUS ||
           codepoint == QUOTE_SINGLE_RECTUS ||
           codepoint == QUOTE_DOUBLE_SINISTER ||
           codepoint == QUOTE_DOUBLE_DEXTER ||
           codepoint == QUOTE_SINGLE_SINISTER ||
           codepoint == QUOTE_SINGLE_DEXTER;
}

/* An codepoint est quote duplex */
interior b32
_est_quote_duplex(s32 codepoint)
{
    redde codepoint == QUOTE_DOUBLE_RECTUS ||
           codepoint == QUOTE_DOUBLE_SINISTER ||
           codepoint == QUOTE_DOUBLE_DEXTER;
}

/* An codepoint est quote simplex */
interior b32
_est_quote_simplex(s32 codepoint)
{
    redde codepoint == QUOTE_SINGLE_RECTUS ||
           codepoint == QUOTE_SINGLE_SINISTER ||
           codepoint == QUOTE_SINGLE_DEXTER;
}


/* ==================================================
 * Functiones Auxiliares - Abbreviationes
 * ================================================== */

/* Comparare verbum cum abbreviatione (case-insensitive) */
interior b32
_comparare_abbreviationem(constans character* verbum, i32 longitudo, constans character* abbrev)
{
    i32 i;
    i32 abbrev_long;

    abbrev_long = (i32)strlen(abbrev);

    si (longitudo != abbrev_long)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        si (_ad_minusculam(verbum[i]) != abbrev[i])
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* An verbum est abbreviatio */
interior b32
_est_abbreviatio(constans character* verbum, i32 longitudo)
{
    i32 i;
    character verbum_sine_puncto[LXIV];
    i32 long_sine_puncto;

    si (longitudo <= ZEPHYRUM || longitudo >= LXIV)
    {
        redde FALSUM;
    }

    /* Primo: probare verbum integrum */
    per (i = ZEPHYRUM; ABBREVIATIONES[i] != NIHIL; i++)
    {
        si (_comparare_abbreviationem(verbum, longitudo, ABBREVIATIONES[i]))
        {
            redde VERUM;
        }
    }

    /* Secundo: probare sine puncto finale */
    si (longitudo > I && verbum[longitudo - I] == '.')
    {
        long_sine_puncto = longitudo - I;
        memcpy(verbum_sine_puncto, verbum, (size_t)long_sine_puncto);
        verbum_sine_puncto[long_sine_puncto] = '\0';

        per (i = ZEPHYRUM; ABBREVIATIONES[i] != NIHIL; i++)
        {
            si (_comparare_abbreviationem(verbum_sine_puncto, long_sine_puncto, ABBREVIATIONES[i]))
            {
                redde VERUM;
            }
        }
    }

    redde FALSUM;
}

/* Extrahere verbum ante punctum */
interior i32
_extrahere_verbum_ante_punctum(chorda textus, i32 punctum_index, character* buffer, i32 buffer_max)
{
    i32 initium;
    i32 longitudo;
    i32 i;
    character c;

    /* Verificare parametros */
    si (punctum_index <= ZEPHYRUM || punctum_index > textus.mensura)
    {
        redde ZEPHYRUM;
    }

    /* Invenire initium verbi - ire retro usque ad non-litteram */
    initium = punctum_index - I;

    /* Scan backwards through letters and dots, but never below 0 */
    dum (initium >= ZEPHYRUM)
    {
        c = (character)textus.datum[initium];
        si (_est_littera(c) || c == '.')
        {
            /* Check before decrementing - stop at 0 */
            si (initium == ZEPHYRUM)
            {
                frange;
            }
            initium--;
        }
        alioquin
        {
            frange;
        }
    }

    /* initium now points to either:
     * - index 0 (word starts at beginning of string)
     * - a non-letter character (word starts at initium + 1)
     */
    si (initium > ZEPHYRUM && !_est_littera((character)textus.datum[initium]) && textus.datum[initium] != '.')
    {
        initium++;
    }

    /* Copiare verbum */
    longitudo = punctum_index - initium;
    si (longitudo <= ZEPHYRUM || longitudo >= buffer_max)
    {
        redde ZEPHYRUM;
    }

    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        buffer[i] = (character)textus.datum[initium + i];
    }
    buffer[longitudo] = '\0';

    redde longitudo;
}


/* ==================================================
 * Functiones Auxiliares - Detectiones Speciales
 * ================================================== */

/* An character ante est cifra et character post est cifra (numerus decimalis) */
interior b32
_est_numerus_decimalis(chorda textus, i32 punctum_index)
{
    character ante;
    character post;

    si (punctum_index <= ZEPHYRUM || punctum_index >= textus.mensura - I)
    {
        redde FALSUM;
    }

    ante = (character)textus.datum[punctum_index - I];
    post = (character)textus.datum[punctum_index + I];

    redde _est_cifra(ante) && _est_cifra(post);
}

/* An est ellipsis (tres puncti) */
interior b32
_est_ellipsis(chorda textus, i32 punctum_index)
{
    si ((character)textus.datum[punctum_index] != '.')
    {
        redde FALSUM;
    }

    si (punctum_index + II < textus.mensura)
    {
        redde (character)textus.datum[punctum_index + I] == '.' &&
               (character)textus.datum[punctum_index + II] == '.';
    }

    redde FALSUM;
}

/* An verbum est possessivum cum smart apostrophe (terminans in 's) */
interior b32
_est_possessivum_smart(constans character* verbum, i32 longitudo)
{
    /* Minimum: X's = 4+ bytes (X + 3-byte apostrophe + s) */
    si (longitudo < IV)
    {
        redde FALSUM;
    }

    /* Probare si terminat in smart apostrophe + s */
    /* Smart apostrophe (') est 3 bytes: 0xE2 0x80 0x99 */
    si (longitudo >= IV &&
        (i8)verbum[longitudo - IV] == (i8)0xE2 &&
        (i8)verbum[longitudo - III] == (i8)0x80 &&
        (i8)verbum[longitudo - II] == (i8)0x99 &&
        (verbum[longitudo - I] == 's' || verbum[longitudo - I] == 'S'))
    {
        redde VERUM;
    }

    redde FALSUM;
}

/* An est abbreviatio specialis (etc., al.) sequuta per maiusculam */
interior b32
_est_abbreviatio_specialis(chorda textus, i32 punctum_index, constans character* verbum)
{
    i32 i;
    character verbum_lower[LXIV];
    i32 longitudo;

    /* Convertere ad minusculas */
    longitudo = (i32)strlen(verbum);
    si (longitudo >= LXIV)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        verbum_lower[i] = _ad_minusculam(verbum[i]);
    }
    verbum_lower[longitudo] = '\0';

    /* Probare si est "etc" vel "al" */
    si (strcmp(verbum_lower, "etc") != ZEPHYRUM && strcmp(verbum_lower, "al") != ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Probare si sequitur spatium et maiuscula */
    i = punctum_index + I;
    dum (i < textus.mensura && _est_spatium((character)textus.datum[i]))
    {
        i++;
    }

    si (i < textus.mensura && _est_maiuscula((character)textus.datum[i]))
    {
        redde VERUM;
    }

    redde FALSUM;
}

/* An est attributio dialogi post quote */
interior b32
_est_attributio_dialogi(chorda textus, i32 post_quote_index)
{
    i32 i;
    i32 num_verba;
    b32 in_verbo;

    /* Saltare spatia */
    i = post_quote_index;
    dum (i < textus.mensura && _est_spatium((character)textus.datum[i]))
    {
        i++;
    }

    si (i >= textus.mensura)
    {
        redde FALSUM;
    }

    /* Si incipit cum minuscula, est attributio */
    si (_est_minuscula((character)textus.datum[i]))
    {
        redde VERUM;
    }

    /* Casus specialis: "I said", "I murmured", etc. */
    si ((character)textus.datum[i] == 'I' && i + I < textus.mensura && _est_spatium((character)textus.datum[i + I]))
    {
        /* Numerare verba post "I" */
        i += II;  /* Saltare "I " */
        num_verba = ZEPHYRUM;
        in_verbo = FALSUM;

        dum (i < textus.mensura && num_verba <= II)
        {
            character c = (character)textus.datum[i];

            si (_est_littera(c))
            {
                si (!in_verbo)
                {
                    num_verba++;
                    in_verbo = VERUM;
                }
            }
            alioquin si (_est_spatium(c))
            {
                in_verbo = FALSUM;
            }
            alioquin si (_est_punctum_terminale(c))
            {
                /* Finis sententiae - probare si tantum 1-2 verba */
                si (num_verba <= II && num_verba >= I)
                {
                    redde VERUM;
                }
                frange;
            }
            alioquin
            {
                frange;
            }

            i++;
        }
    }

    redde FALSUM;
}


/* ==================================================
 * Structura Iterator
 * ================================================== */

structura SententiaIterator {
    chorda   textus;           /* Textus originalis */
    Piscina* piscina;          /* Pro allocationibus */

    /* Cache sententiarum (crescit dum iteramus) */
    chorda*  cache;            /* Array sententiarum in cache */
    i32      cache_numerus;    /* Quot in cache */
    i32      cache_capacitas;  /* Capacitas allocata */

    /* Positio currens */
    i32      index_currens;    /* Index sententiae currentis */
    i32      byte_currens;     /* Offset byte in textu */

    /* Vexillum completionis */
    b32      completum;        /* Verum si totum textum analysatum */
};


/* ==================================================
 * Functio Principalis - Invenire Finem Sententiae
 *
 * Haec functio determinat ubi sententia finit.
 * Redde: index byte post finem sententiae, vel -1 si nihil
 * ================================================== */

interior i32
_invenire_finem_sententiae(chorda textus, i32 initium)
{
    i32 i;
    b32 in_quote;
    s32 quote_genus;  /* Codepoint quote aperientis */
    i32 sententia_initium;

    si (initium >= textus.mensura)
    {
        redde (i32)-1;
    }

    in_quote = FALSUM;
    quote_genus = ZEPHYRUM;
    sententia_initium = initium;

    i = initium;
    dum (i < textus.mensura)
    {
        character c = (character)textus.datum[i];
        s32 codepoint;
        i32 runa_long;

        /* Obtinere codepoint (pro smart quotes) */
        codepoint = _decodere_runam(textus, i);
        runa_long = _longitudo_runae(textus, i);
        si (runa_long <= ZEPHYRUM)
        {
            runa_long = I;
        }

        /* ==================================================
         * Tractare Quotes
         * ================================================== */

        /* Quotes duplices */
        si (_est_quote_duplex(codepoint))
        {
            si (!in_quote)
            {
                in_quote = VERUM;
                quote_genus = codepoint;
            }
            alioquin si (_est_quote_duplex(quote_genus))
            {
                in_quote = FALSUM;
                quote_genus = ZEPHYRUM;
            }
            i += runa_long;
            perge;
        }

        /* Quotes simplices - tractare possessiva/contractiones */
        si (_est_quote_simplex(codepoint))
        {
            b32 est_possessivum;

            est_possessivum = FALSUM;

            /* Probare si est possessivum/contractio */
            si (i > ZEPHYRUM && _est_littera((character)textus.datum[i - I]))
            {
                /* Post apostrophe: s, t, ll, ve, re, d, m */
                i32 post = i + runa_long;
                si (post < textus.mensura)
                {
                    character post_c = (character)textus.datum[post];
                    si (post_c == 's' || post_c == 't' || post_c == 'd' || post_c == 'm')
                    {
                        /* Probare si sequitur spatium vel punctum */
                        si (post + I >= textus.mensura ||
                            _est_spatium((character)textus.datum[post + I]) ||
                            _est_punctum_terminale((character)textus.datum[post + I]))
                        {
                            est_possessivum = VERUM;
                        }
                    }
                    alioquin si (post_c == 'l' && post + I < textus.mensura &&
                                (character)textus.datum[post + I] == 'l')
                    {
                        est_possessivum = VERUM;  /* 'll */
                    }
                    alioquin si (post_c == 'v' && post + I < textus.mensura &&
                                (character)textus.datum[post + I] == 'e')
                    {
                        est_possessivum = VERUM;  /* 've */
                    }
                    alioquin si (post_c == 'r' && post + I < textus.mensura &&
                                (character)textus.datum[post + I] == 'e')
                    {
                        est_possessivum = VERUM;  /* 're */
                    }
                }
            }

            si (!est_possessivum)
            {
                /* Est quote */
                si (!in_quote)
                {
                    in_quote = VERUM;
                    quote_genus = codepoint;
                }
                alioquin si (_est_quote_simplex(quote_genus))
                {
                    in_quote = FALSUM;
                    quote_genus = ZEPHYRUM;
                }
            }

            i += runa_long;
            perge;
        }

        /* ==================================================
         * Tractare Puncta Terminalia
         * ================================================== */

        si (_est_punctum_terminale(c))
        {
            character verbum[LXIV];
            i32 verbum_long;
            b32 est_abbrev;
            b32 est_decimal;
            b32 est_ellips;
            b32 est_specialis;

            /* Extrahere verbum ante punctum */
            verbum_long = _extrahere_verbum_ante_punctum(textus, i, verbum, LXIV);

            /* Probare possessivum cum smart apostrophe */
            si (verbum_long > ZEPHYRUM && _est_possessivum_smart(verbum, verbum_long))
            {
                est_abbrev = FALSUM;
            }
            /* Casus specialis: "s" praecedetur per smart apostrophe (possessivum) */
            alioquin si (verbum_long == I && (verbum[ZEPHYRUM] == 's' || verbum[ZEPHYRUM] == 'S'))
            {
                /* Probare si praeceditur per smart apostrophe (0xE2 0x80 0x99) */
                i32 pos_s = i - I;  /* Positio 's' in textu */
                si (pos_s >= III)
                {
                    si ((i8)textus.datum[pos_s - III] == (i8)0xE2 &&
                        (i8)textus.datum[pos_s - II] == (i8)0x80 &&
                        (i8)textus.datum[pos_s - I] == (i8)0x99)
                    {
                        /* Est possessivum (Satan's, etc.) - non abbreviatio */
                        est_abbrev = FALSUM;
                    }
                    alioquin
                    {
                        est_abbrev = _est_abbreviatio(verbum, verbum_long);
                    }
                }
                alioquin
                {
                    est_abbrev = _est_abbreviatio(verbum, verbum_long);
                }
            }
            alioquin
            {
                est_abbrev = _est_abbreviatio(verbum, verbum_long);
            }

            est_decimal = _est_numerus_decimalis(textus, i);
            est_ellips = _est_ellipsis(textus, i);
            est_specialis = est_abbrev && _est_abbreviatio_specialis(textus, i, verbum);

            /* Saltare ellipsis */
            si (est_ellips)
            {
                i += III;  /* Saltare "..." */
                perge;
            }

            /* Non findere pro abbreviatione (nisi specialis) vel decimali */
            /* Consecutive abbreviationes tractantur sicut regulares abbreviationes */
            si ((est_abbrev && !est_specialis) || est_decimal)
            {
                i++;
                perge;
            }

            /* Probare si debemus findere */
            {
                i32 post;
                s32 post_codepoint;
                b32 debet_findere;

                post = i + I;

                /* Saltare quote post punctum */
                si (post < textus.mensura)
                {
                    post_codepoint = _decodere_runam(textus, post);
                    si (_est_quote_runa(post_codepoint))
                    {
                        /* Probare attributionem dialogi */
                        i32 runa_long_post = _longitudo_runae(textus, post);
                        si (runa_long_post <= ZEPHYRUM) runa_long_post = I;

                        si (_est_attributio_dialogi(textus, post + runa_long_post))
                        {
                            /* Non findere - est attributio */
                            i = post + runa_long_post;

                            /* Actualizare quote status */
                            si (in_quote && _est_quote_duplex(quote_genus) && _est_quote_duplex(post_codepoint))
                            {
                                in_quote = FALSUM;
                                quote_genus = ZEPHYRUM;
                            }
                            alioquin si (in_quote && _est_quote_simplex(quote_genus) && _est_quote_simplex(post_codepoint))
                            {
                                in_quote = FALSUM;
                                quote_genus = ZEPHYRUM;
                            }

                            perge;
                        }

                        /* Includere quote in sententia */
                        post += runa_long_post;
                    }
                }

                /* Probare condiciones pro findere */
                debet_findere = FALSUM;

                si (post >= textus.mensura)
                {
                    debet_findere = VERUM;
                }
                alioquin si (_est_spatium((character)textus.datum[post]))
                {
                    debet_findere = VERUM;
                }
                alioquin
                {
                    post_codepoint = _decodere_runam(textus, post);
                    si (_est_quote_runa(post_codepoint))
                    {
                        debet_findere = VERUM;
                    }
                }

                /* Si in quote et non debemus findere normalmente */
                si (in_quote && !debet_findere)
                {
                    i++;
                    perge;
                }

                si (debet_findere)
                {
                    /* Invenimus finem sententiae */
                    redde post;
                }
            }
        }

        i += runa_long;
    }

    /* Si textus finit sine puncto, tota residua est sententia */
    si (i > sententia_initium)
    {
        redde textus.mensura;
    }

    redde (i32)-1;
}


/* ==================================================
 * API Principalis
 * ================================================== */

SententiaeFissioFructus
sententia_fissio(
    chorda   textus,
    Piscina* piscina)
{
    SententiaeFissioFructus fructus;
    i32 capacitas;
    i32 i;
    i32 finis;

    fructus.sententiae = NIHIL;
    fructus.numerus = ZEPHYRUM;

    si (textus.mensura <= ZEPHYRUM || piscina == NIHIL)
    {
        redde fructus;
    }

    /* Allocare array initiale */
    capacitas = XVI;
    fructus.sententiae = piscina_allocare(piscina, (i64)(capacitas * (i32)magnitudo(chorda)));
    si (fructus.sententiae == NIHIL)
    {
        redde fructus;
    }

    /* Iterare per textum */
    i = ZEPHYRUM;

    /* Saltare spatia initialia */
    dum (i < textus.mensura && _est_spatium((character)textus.datum[i]))
    {
        i++;
    }

    dum (i < textus.mensura)
    {
        i32 initium;
        chorda sententia;

        initium = i;
        finis = _invenire_finem_sententiae(textus, initium);

        si (finis < ZEPHYRUM)
        {
            finis = textus.mensura;
        }

        /* Creare sententiam (trim spatia) */
        {
            i32 sent_init;
            i32 sent_fin;

            sent_init = initium;
            sent_fin = finis;

            /* Trim initium */
            dum (sent_init < sent_fin && _est_spatium((character)textus.datum[sent_init]))
            {
                sent_init++;
            }

            /* Trim finem */
            dum (sent_fin > sent_init && _est_spatium((character)textus.datum[sent_fin - I]))
            {
                sent_fin--;
            }

            si (sent_fin > sent_init)
            {
                /* Allocare et copiare */
                sententia.mensura = sent_fin - sent_init;
                sententia.datum = piscina_allocare(piscina, (i64)(sententia.mensura + I));
                si (sententia.datum != NIHIL)
                {
                    memcpy(sententia.datum, textus.datum + sent_init, (size_t)sententia.mensura);

                    /* Expandere array si necesse */
                    si (fructus.numerus >= capacitas)
                    {
                        i32 nova_capacitas;
                        chorda* novum_array;

                        nova_capacitas = capacitas * II;
                        novum_array = piscina_allocare(piscina, (i64)(nova_capacitas * (i32)magnitudo(chorda)));
                        si (novum_array != NIHIL)
                        {
                            memcpy(novum_array, fructus.sententiae, (size_t)(fructus.numerus * (i32)magnitudo(chorda)));
                            fructus.sententiae = novum_array;
                            capacitas = nova_capacitas;
                        }
                    }

                    fructus.sententiae[fructus.numerus++] = sententia;
                }
            }
        }

        /* Movere ad proximam sententiam */
        i = finis;

        /* Saltare spatia */
        dum (i < textus.mensura && _est_spatium((character)textus.datum[i]))
        {
            i++;
        }
    }

    redde fructus;
}


i32
sententia_numerare(
    chorda textus)
{
    i32 numerus;
    i32 i;
    i32 finis;

    si (textus.mensura <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    numerus = ZEPHYRUM;
    i = ZEPHYRUM;

    /* Saltare spatia initialia */
    dum (i < textus.mensura && _est_spatium((character)textus.datum[i]))
    {
        i++;
    }

    dum (i < textus.mensura)
    {
        finis = _invenire_finem_sententiae(textus, i);

        si (finis < ZEPHYRUM)
        {
            finis = textus.mensura;
        }

        /* Probare si non vacua */
        {
            i32 sent_init;
            i32 sent_fin;

            sent_init = i;
            sent_fin = finis;

            dum (sent_init < sent_fin && _est_spatium((character)textus.datum[sent_init]))
            {
                sent_init++;
            }

            si (sent_fin > sent_init)
            {
                numerus++;
            }
        }

        i = finis;

        dum (i < textus.mensura && _est_spatium((character)textus.datum[i]))
        {
            i++;
        }
    }

    redde numerus;
}


SententiaIndicesFructus
sententia_fissio_indices(
    chorda   textus,
    Piscina* piscina)
{
    SententiaIndicesFructus fructus;
    i32 capacitas;
    i32 i;
    i32 finis;

    fructus.indices = NIHIL;
    fructus.numerus = ZEPHYRUM;

    si (textus.mensura <= ZEPHYRUM || piscina == NIHIL)
    {
        redde fructus;
    }

    /* Allocare array initiale */
    capacitas = XVI;
    fructus.indices = piscina_allocare(piscina, (i64)(capacitas * (i32)magnitudo(SententiaIndices)));
    si (fructus.indices == NIHIL)
    {
        redde fructus;
    }

    /* Iterare per textum */
    i = ZEPHYRUM;

    /* Saltare spatia initialia */
    dum (i < textus.mensura && _est_spatium((character)textus.datum[i]))
    {
        i++;
    }

    dum (i < textus.mensura)
    {
        i32 initium;
        i32 sent_init;
        i32 sent_fin;

        initium = i;
        finis = _invenire_finem_sententiae(textus, initium);

        si (finis < ZEPHYRUM)
        {
            finis = textus.mensura;
        }

        /* Trim spatia */
        sent_init = initium;
        sent_fin = finis;

        dum (sent_init < sent_fin && _est_spatium((character)textus.datum[sent_init]))
        {
            sent_init++;
        }

        dum (sent_fin > sent_init && _est_spatium((character)textus.datum[sent_fin - I]))
        {
            sent_fin--;
        }

        si (sent_fin > sent_init)
        {
            /* Expandere array si necesse */
            si (fructus.numerus >= capacitas)
            {
                i32 nova_capacitas;
                SententiaIndices* novum_array;

                nova_capacitas = capacitas * II;
                novum_array = piscina_allocare(piscina, (i64)(nova_capacitas * (i32)magnitudo(SententiaIndices)));
                si (novum_array != NIHIL)
                {
                    memcpy(novum_array, fructus.indices, (size_t)(fructus.numerus * (i32)magnitudo(SententiaIndices)));
                    fructus.indices = novum_array;
                    capacitas = nova_capacitas;
                }
            }

            /* Addere indices */
            fructus.indices[fructus.numerus].initium = sent_init;
            fructus.indices[fructus.numerus].finis = sent_fin;
            fructus.numerus++;
        }

        /* Movere ad proximam sententiam */
        i = finis;

        /* Saltare spatia */
        dum (i < textus.mensura && _est_spatium((character)textus.datum[i]))
        {
            i++;
        }
    }

    redde fructus;
}


/* ==================================================
 * API Iterator
 * ================================================== */

SententiaIterator*
sententia_iterator_creare(
    chorda   textus,
    Piscina* piscina)
{
    SententiaIterator* iter;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    iter = piscina_allocare(piscina, (i64)magnitudo(SententiaIterator));
    si (iter == NIHIL)
    {
        redde NIHIL;
    }

    iter->textus = textus;
    iter->piscina = piscina;

    /* Initiare cache */
    iter->cache_capacitas = XVI;
    iter->cache = piscina_allocare(piscina, (i64)(iter->cache_capacitas * (i32)magnitudo(chorda)));
    iter->cache_numerus = ZEPHYRUM;

    iter->index_currens = (i32)-1;  /* Ante primam */
    iter->byte_currens = ZEPHYRUM;
    iter->completum = FALSUM;

    /* Saltare spatia initialia */
    dum (iter->byte_currens < textus.mensura && _est_spatium((character)textus.datum[iter->byte_currens]))
    {
        iter->byte_currens++;
    }

    redde iter;
}


/* Auxiliaris: expandere cache si necesse */
interior b32
_iterator_expandere_cache(SententiaIterator* iter)
{
    i32 nova_capacitas;
    chorda* novum_array;

    si (iter->cache_numerus < iter->cache_capacitas)
    {
        redde VERUM;
    }

    nova_capacitas = iter->cache_capacitas * II;
    novum_array = piscina_allocare(iter->piscina, (i64)(nova_capacitas * (i32)magnitudo(chorda)));
    si (novum_array == NIHIL)
    {
        redde FALSUM;
    }

    memcpy(novum_array, iter->cache, (size_t)(iter->cache_numerus * (i32)magnitudo(chorda)));
    iter->cache = novum_array;
    iter->cache_capacitas = nova_capacitas;

    redde VERUM;
}


/* Auxiliaris: parsere proximam sententiam in cache */
interior b32
_iterator_parsere_proximam(SententiaIterator* iter)
{
    i32 initium;
    i32 finis;
    chorda sententia;
    i32 sent_init;
    i32 sent_fin;

    si (iter->completum)
    {
        redde FALSUM;
    }

    si (iter->byte_currens >= iter->textus.mensura)
    {
        iter->completum = VERUM;
        redde FALSUM;
    }

    initium = iter->byte_currens;
    finis = _invenire_finem_sententiae(iter->textus, initium);

    si (finis < ZEPHYRUM)
    {
        finis = iter->textus.mensura;
    }

    /* Trim */
    sent_init = initium;
    sent_fin = finis;

    dum (sent_init < sent_fin && _est_spatium((character)iter->textus.datum[sent_init]))
    {
        sent_init++;
    }

    dum (sent_fin > sent_init && _est_spatium((character)iter->textus.datum[sent_fin - I]))
    {
        sent_fin--;
    }

    si (sent_fin <= sent_init)
    {
        iter->byte_currens = finis;
        dum (iter->byte_currens < iter->textus.mensura &&
             _est_spatium((character)iter->textus.datum[iter->byte_currens]))
        {
            iter->byte_currens++;
        }
        si (iter->byte_currens >= iter->textus.mensura)
        {
            iter->completum = VERUM;
        }
        redde FALSUM;
    }

    /* Expandere cache si necesse */
    si (!_iterator_expandere_cache(iter))
    {
        redde FALSUM;
    }

    /* Creare sententia */
    sententia.mensura = sent_fin - sent_init;
    sententia.datum = piscina_allocare(iter->piscina, (i64)(sententia.mensura + I));
    si (sententia.datum == NIHIL)
    {
        redde FALSUM;
    }

    memcpy(sententia.datum, iter->textus.datum + sent_init, (size_t)sententia.mensura);

    iter->cache[iter->cache_numerus++] = sententia;

    /* Movere byte_currens */
    iter->byte_currens = finis;
    dum (iter->byte_currens < iter->textus.mensura &&
         _est_spatium((character)iter->textus.datum[iter->byte_currens]))
    {
        iter->byte_currens++;
    }

    si (iter->byte_currens >= iter->textus.mensura)
    {
        iter->completum = VERUM;
    }

    redde VERUM;
}


chorda
sententia_iterator_proxima(
    SententiaIterator* iter)
{
    chorda vacua;
    i32 proximus_index;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;

    si (iter == NIHIL)
    {
        redde vacua;
    }

    proximus_index = iter->index_currens + I;

    /* Si iam in cache */
    si (proximus_index < iter->cache_numerus)
    {
        iter->index_currens = proximus_index;
        redde iter->cache[proximus_index];
    }

    /* Parsere plures si necesse */
    dum (!iter->completum && iter->cache_numerus <= proximus_index)
    {
        _iterator_parsere_proximam(iter);
    }

    si (proximus_index < iter->cache_numerus)
    {
        iter->index_currens = proximus_index;
        redde iter->cache[proximus_index];
    }

    redde vacua;
}


chorda
sententia_iterator_prior(
    SententiaIterator* iter)
{
    chorda vacua;
    i32 prior_index;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;

    si (iter == NIHIL)
    {
        redde vacua;
    }

    prior_index = iter->index_currens - I;

    si (prior_index < ZEPHYRUM)
    {
        redde vacua;
    }

    si (prior_index < iter->cache_numerus)
    {
        iter->index_currens = prior_index;
        redde iter->cache[prior_index];
    }

    redde vacua;
}


i32
sententia_iterator_index(
    SententiaIterator* iter)
{
    si (iter == NIHIL)
    {
        redde (i32)-1;
    }

    redde iter->index_currens;
}


i32
sententia_iterator_numerus(
    SententiaIterator* iter)
{
    si (iter == NIHIL)
    {
        redde ZEPHYRUM;
    }

    /* Parsere totum si non completum */
    dum (!iter->completum)
    {
        _iterator_parsere_proximam(iter);
    }

    redde iter->cache_numerus;
}


vacuum
sententia_iterator_reset(
    SententiaIterator* iter)
{
    si (iter == NIHIL)
    {
        redde;
    }

    iter->index_currens = (i32)-1;
}
