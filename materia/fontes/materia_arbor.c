/* materia_arbor.c - Arbor -> STML canonicum (scriptor)
 *
 * Portatum ex silva_arbor.c, lineis 145-2177. Quattuor mutata:
 *   - genus lexematis per materia_lexicon solvitur, non per tabulas
 *     ORTHOGRAPHIAE/NOMINA_GENERUM in hoc modulo conditas
 *   - 'valor portandus' est species==VERBATIM, non 'orthographia
 *     NIHIL et non EOF'
 *   - campi frontis proprii (standard, scissurae, origo) per uncos
 *     MateriaArborFrons eunt
 *   - ancora et lexema derivatum per MateriaOrigoUncus solvuntur
 */

#include "materia_arbor.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "friatio.h"
#include <string.h>

hic_manens constans character HEX_CIFRAE[] = "0123456789abcdef";

/* Nota per lexema: quotiens visum (passus I), quis numerus
 * fragmenti ei datus sit, an definitio iam emissa (passus II) */
nomen structura {
    i32 usus;
    i32 numerus;
    b32 emissum;
} ArborNota;

nomen structura {
       constans vacuum* clavis;
                   b32  est_lexema;
    constans StmlNodus* elementum;
} ArborPar;

structura MateriaArborScriptor {
                            Piscina* piscina;
                InternamentumChorda* intern;
    constans MateriaArborConsilium*  consilium;
                     TabulaDispersa* lexemata;
                                Xar* paria;    /* ArborPar; NIHIL = nulla */
                                i32  numerus_notarum;

    /* Ancora - vide legem in capite */
                                b32  ancora_nota;
                                s32  ancora_offset;
                                i32  ancora_linea;
                                i32  ancora_columna;
                                s32  ancora_fons;
                                b32  ancora_initium_lineae;

                 MateriaArborCensus  census;
                constans character*  causa;
             constans MateriaNodus*  sedes;
};


/* ==================================================
 * Accessus pro fronte
 * ================================================== */

Piscina*
materia_arbor_scriptor_piscina (MateriaArborScriptor* scriptor)
{
    redde (scriptor == NIHIL) ? NIHIL : scriptor->piscina;
}

InternamentumChorda*
materia_arbor_scriptor_intern (MateriaArborScriptor* scriptor)
{
    redde (scriptor == NIHIL) ? NIHIL : scriptor->intern;
}

vacuum
materia_arbor_scriptor_recusare (
    MateriaArborScriptor* scriptor,
      constans character* causa)
{
    si (scriptor != NIHIL && scriptor->causa == NIHIL)
    {
        scriptor->causa = causa;
    }
}


/* ==================================================
 * Sigillum registri
 * ================================================== */

chorda
materia_arbor_sigillum (
                          Piscina* piscina,
    constans MateriaRegistrumCoctum* tabularium)
{
    ChordaAedificator* mat;
    ChordaAedificator* exitus;
               chorda  friandum;
               chorda  vacua;
                  i32  friatum;
                  i32  i;

    vacua.mensura = ZEPHYRUM;
    vacua.datum   = NIHIL;

    si (piscina == NIHIL || tabularium == NIHIL) { redde vacua; }
    mat = chorda_aedificator_creare(piscina, 4096);
    si (mat == NIHIL) { redde vacua; }

    /* Separator post CAMPUM QUEMQUE: sine eo 'ab'+'c' et 'a'+'bc'
     * eandem materiam darent. */
    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans MateriaTabGenus* g = &tabularium->genera[i];

        si (g->titulus != NIHIL)
        {
            chorda_aedificator_appendere_literis(mat, g->titulus);
        }
        chorda_aedificator_appendere_character(mat, '\n');
        chorda_aedificator_appendere_i32(mat, g->loci_offset);
        chorda_aedificator_appendere_character(mat, '\n');
        chorda_aedificator_appendere_i32(mat, g->loci_numerus);
        chorda_aedificator_appendere_character(mat, '\n');
    }
    per (i = ZEPHYRUM; i < tabularium->numerus_locorum; i++)
    {
        constans MateriaTabLocus* l = &tabularium->loci[i];

        si (l->titulus != NIHIL)
        {
            chorda_aedificator_appendere_literis(mat, l->titulus);
        }
        chorda_aedificator_appendere_character(mat, '\n');
        chorda_aedificator_appendere_s32(mat, l->species);
        chorda_aedificator_appendere_character(mat, '\n');
    }

    friandum = chorda_aedificator_spectare(mat);
    friatum  = friatio_fnv1a_literis(
        (constans character*)friandum.datum, friandum.mensura);
    chorda_aedificator_destruere(mat);

    /* Hexadecimale longitudinis FIXAE - sigilla oculo conferenda
     * '%x' variabile ferre non possunt (0x0000abcd 'abcd' fieret). */
    exitus = chorda_aedificator_creare(piscina,
        MATERIA_ARBOR_SIGILLI_LONGITUDO + I);
    si (exitus == NIHIL) { redde vacua; }
    per (i = ZEPHYRUM; i < MATERIA_ARBOR_SIGILLI_LONGITUDO; i++)
    {
        i32 gradus  = (MATERIA_ARBOR_SIGILLI_LONGITUDO - I - i) * IV;
        i32 nibble  = (friatum >> gradus) & (i32)0xF;

        chorda_aedificator_appendere_character(exitus,
            HEX_CIFRAE[nibble]);
    }
    redde chorda_aedificator_finire(exitus);
}

s32
materia_arbor_genus_index (
    constans MateriaRegistrumCoctum* tabularium,
                 constans character* titulus,
                                i32  mensura)
{
    i32 i;

    si (tabularium == NIHIL || titulus == NIHIL) { redde (s32)-I; }
    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans character* t = tabularium->genera[i].titulus;

        si (   t != NIHIL
            && (i32)strlen(t) == mensura
            && memcmp(t, titulus, (size_t)mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}


/* ==================================================
 * Mangulatio tagorum - per lexicon, non per tabulam localem
 * ================================================== */

i32
materia_arbor_lexema_tag (
    constans MateriaLexiconRatum* ratum,
                             s32  genus,
                      character*  buffer,
                             i32  capacitas)
{
    constans character* titulus;
    constans character* praefixum;
                   i32  lp;
                   i32  scripta;
                   i32  i;

    si (buffer == NIHIL || capacitas == ZEPHYRUM || ratum == NIHIL)
    {
        redde ZEPHYRUM;
    }
    titulus = materia_lexicon_titulus(ratum, genus);
    si (titulus == NIHIL) { redde ZEPHYRUM; }
    praefixum = ratum->lexicon->praefixum_tagi;

    lp      = (i32)strlen(praefixum);
    scripta = lp + (i32)strlen(titulus);
    si (scripta + I > capacitas) { redde ZEPHYRUM; }

    memcpy(buffer, praefixum, (size_t)lp);
    per (i = ZEPHYRUM; titulus[i] != '\0'; i++)
    {
        character c = titulus[i];

        si      (c >= 'A' && c <= 'Z') { c = (character)(c - 'A' + 'a'); }
        alioquin si (c == '_')         { c = '-'; }
        buffer[lp + i] = c;
    }
    buffer[scripta] = '\0';
    redde scripta;
}

s32
materia_arbor_lexema_ex_tag (
    constans MateriaLexiconRatum* ratum,
             constans character*  tag,
                             i32  mensura)
{
    character buffer[MATERIA_ARBOR_TAG_CAPACITAS];
          s32 i;
          s32 n;

    si (ratum == NIHIL) { redde ZEPHYRUM; }
    n = (s32)ratum->lexicon->numerus_generum;
    si (tag == NIHIL || mensura == ZEPHYRUM) { redde n; }

    /* Per mangulationem ANTRORSAM - ergo directiones divergere
     * NON possunt. Quaesitio semel per elementum fit. */
    per (i = ZEPHYRUM; i < n; i++)
    {
        i32 longitudo = materia_arbor_lexema_tag(ratum, i, buffer,
            (i32)magnitudo(buffer));

        si (   longitudo                            == mensura
            && memcmp(buffer, tag, (size_t)mensura) == ZEPHYRUM)
        {
            redde i;
        }
    }
    redde n;
}


/* ==================================================
 * Claves et notae
 * ================================================== */

interior chorda
_clavis_monstratoris (Piscina* piscina, constans vacuum* monstrator)
{
    vacuum* cella;
    chorda  clavis;

    cella = piscina_allocare(piscina, magnitudo(constans vacuum*));
    si (cella == NIHIL)
    {
        clavis.mensura = ZEPHYRUM;
        clavis.datum   = NIHIL;
        redde clavis;
    }
    *(constans vacuum**)cella = monstrator;
    clavis.mensura = (i32)magnitudo(constans vacuum*);
    clavis.datum   = (i8*)cella;
    redde clavis;
}

interior ArborNota*
_nota_lexematis (MateriaArborScriptor* st, constans MateriaToken* lexema)
{
    vacuum* inventum;
    chorda  clavis;

    clavis = _clavis_monstratoris(st->piscina, (constans vacuum*)lexema);
    si (clavis.datum == NIHIL) { redde NIHIL; }
    si (tabula_dispersa_invenire(st->lexemata, clavis, &inventum))
    {
        redde (ArborNota*)inventum;
    }
    redde NIHIL;
}


/* ==================================================
 * Passus I - usus numerare + ancoram capere
 * ================================================== */

interior vacuum _numerare_valorem (MateriaArborScriptor*, MateriaValor);

interior vacuum
_numerare_lexema (MateriaArborScriptor* st, constans MateriaToken* lexema)
{
    ArborNota* nota;

    si (lexema == NIHIL) { redde; }

    nota = _nota_lexematis(st, lexema);
    si (nota != NIHIL) { nota->usus++; redde; }

    nota = (ArborNota*)piscina_allocare(st->piscina, magnitudo(ArborNota));
    si (nota == NIHIL) { redde; }
    nota->usus    = I;
    nota->numerus = ZEPHYRUM;
    nota->emissum = FALSUM;
    tabula_dispersa_inserere(st->lexemata,
        _clavis_monstratoris(st->piscina, (constans vacuum*)lexema), nota);

    /* ANCORA - vide legem in capite. Radix emissionis primum, quia
     * emissio ab invocatione strati 0 incipit, non a lexemate
     * derivato (cuius sedes in plagula ALIA est). */
    si (!st->ancora_nota)
    {
        constans MateriaToken* emissum = lexema;
        constans MateriaToken* initium;
        constans MateriaOrigoUncus* uncus = st->consilium->origo;

        si (uncus != NIHIL && uncus->radix_quaerere != NIHIL)
        {
            constans character* causa = NIHIL;
            MateriaToken* r = uncus->radix_quaerere(uncus->datum,
                (MateriaToken*)(size_t)(constans vacuum*)lexema, &causa);

            si (r != NIHIL) { emissum = r; }
        }
        initium = emissum;

        /* TRIVIUM PRIMUM, non lexema: emissio triviis ducentibus
         * incipit. Silva id CLXXVIII divergentiis mensuravit. */
        si (emissum->numerus_ante > ZEPHYRUM
            && emissum->spatia_ante[0] != NIHIL
            && emissum->spatia_ante[0]->byte_offset >= ZEPHYRUM)
        {
            initium = emissum->spatia_ante[0];
        }

        st->ancora_nota    = VERUM;
        st->ancora_offset  = initium->byte_offset;
        st->ancora_linea   = initium->linea;
        st->ancora_columna = initium->columna;
        st->ancora_fons    = emissum->fons_index;
        /* NON DERIVABILE ex subarbore: an lexema primum lineam
         * incipiat pendet ab eo quod ANTE subarborem stat. */
        st->ancora_initium_lineae = materia_token_initium_lineae(emissum);
    }

    /* Lexemata quae ORIGO nominat quoque numeranda - invocatio una
     * plura lexemata expansa gignere potest, et si quodque suam
     * copiam scriberet emissor invocationem PLURIES redderet. */
    {
        constans MateriaArborFrons* frons = st->consilium->frons;

        si (frons != NIHIL && frons->origo_numerare != NIHIL)
        {
            frons->origo_numerare(frons->datum, lexema,
                (vacuum (*)(vacuum*, constans MateriaToken*))
                    _numerare_lexema, st);
        }
    }
}

interior vacuum
_numerare_nodum (MateriaArborScriptor* st, constans MateriaNodus* nodus)
{
    i32 i;

    si (nodus == NIHIL) { redde; }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        _numerare_valorem(st, nodus->loci[i]);
    }
}

interior vacuum
_numerare_valorem (MateriaArborScriptor* st, MateriaValor valor)
{
    i32 i;
    i32 numerus;

    commutatio (valor.genus)
    {
    casus MATERIA_VALOR_NODUS:
        _numerare_nodum(st, valor.datum.nodus); frange;
    casus MATERIA_VALOR_TOKEN:
        _numerare_lexema(st, valor.datum.token); frange;
    casus MATERIA_VALOR_LISTA:
        numerus = materia_valor_lista_numerus(valor);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            MateriaValor* e = materia_valor_lista_obtinere(valor, i);

            si (e != NIHIL) { _numerare_valorem(st, *e); }
        }
        frange;
    ordinarius: frange;
    }
}


/* ==================================================
 * Passus II - auxiliares
 * ================================================== */

/* Decimale sine stdio (snprintf C99 est) */
interior i32
_numerus_ad_literas (i32 numerus, character* buffer, i32 capacitas)
{
    character inversa[XVI];
          i32 longitudo;
          i32 i;

    si (buffer == NIHIL || capacitas < II) { redde ZEPHYRUM; }
    si (numerus == ZEPHYRUM)
    {
        buffer[0] = '0'; buffer[1] = '\0'; redde I;
    }
    longitudo = ZEPHYRUM;
    dum (numerus > ZEPHYRUM && longitudo < (i32)magnitudo(inversa))
    {
        inversa[longitudo] = (character)('0' + (numerus % X));
        numerus /= X;
        longitudo++;
    }
    si (longitudo + I > capacitas) { redde ZEPHYRUM; }
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        buffer[i] = inversa[longitudo - I - i];
    }
    buffer[longitudo] = '\0';
    redde longitudo;
}

b32
materia_arbor_attributum_numeri (
    MateriaArborScriptor* scriptor,
               StmlNodus* elementum,
      constans character* titulus,
                     i32  numerus)
{
    character buffer[XVI];

    si (scriptor == NIHIL) { redde FALSUM; }
    si (_numerus_ad_literas(numerus, buffer, (i32)magnitudo(buffer))
        == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde stml_attributum_addere(elementum, scriptor->piscina,
        scriptor->intern, titulus, buffer);
}

/* An chorda TEXTUI tuta sit. Duo pericula, ambo TACITA: spatium
 * album SOLUM (scriptor pulcher tales nodos PRAETERIT, ergo octeti
 * sine querela perirent) et NUL (chorda longitudinem fert, textus
 * emissus terminatore legetur). */
interior b32
_textus_tutus (constans chorda* valor)
{
    i32 i;
    b32 album_solum;

    si (valor == NIHIL || valor->mensura == ZEPHYRUM) { redde VERUM; }
    album_solum = VERUM;
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        character c = (character)valor->datum[i];

        si (c == '\0') { redde FALSUM; }
        si (   c != ' ' && c != '\t' && c != '\n'
            && c != '\r' && c != '\f' && c != '\v')
        {
            album_solum = FALSUM;
        }
    }
    redde album_solum ? FALSUM : VERUM;
}

/* Valorem genere CRUDO notare: valores arboris OCTETI CODICIS sunt,
 * non prosa - pulcher crudum numquam tangit. Custos CLARUS: valor
 * sequentiam claudentem propriam ferre non potest. */
interior b32
_valorem_crudum_notare (
    MateriaArborScriptor* st,
               StmlNodus* elementum,
      constans character* tag,
         constans chorda* valor)
{
    character clausura[MATERIA_ARBOR_TAG_CAPACITAS + IV];
          i32 longitudo;
          i32 i;

    longitudo          = (i32)strlen(tag);
    clausura[ZEPHYRUM] = '<';
    clausura[I]        = '/';
    memcpy(clausura + II, tag, (size_t)longitudo);
    clausura[II + longitudo] = '>';
    longitudo += III;

    si (valor->mensura >= longitudo)
    {
        per (i = ZEPHYRUM; i <= valor->mensura - longitudo; i++)
        {
            si (memcmp(valor->datum + i, clausura,
                    (size_t)longitudo) == ZEPHYRUM)
            {
                st->causa = "valor sequentiam claudentem fert";
                redde FALSUM;
            }
        }
    }
    elementum->crudus = VERUM;
    redde VERUM;
}


/* ==================================================
 * Trivia - forma per SPECIEM lexici, non per genus notum
 * ================================================== */

interior StmlNodus*
_trivium_scribere (MateriaArborScriptor* st, constans MateriaToken* trivium)
{
    character  tag[MATERIA_ARBOR_TAG_CAPACITAS];
    StmlNodus* elementum;

    si (trivium == NIHIL)
    {
        st->causa = "trivium nihil";
        redde NIHIL;
    }
    si (materia_arbor_lexema_tag(st->consilium->lexicon, trivium->genus,
            tag, (i32)magnitudo(tag)) == ZEPHYRUM)
    {
        st->causa = "genus trivii ignotum";
        redde NIHIL;
    }
    elementum = stml_elementum_creare(st->piscina, st->intern, tag);
    si (elementum == NIHIL)
    {
        st->causa = "elementum trivii creari non potuit";
        redde NIHIL;
    }

    commutatio (materia_lexicon_species(st->consilium->lexicon,
                    trivium->genus))
    {
    casus MATERIA_LEX_REPETITUM:
        /* numerus unitatum; valor numero plene determinatur */
        si (!materia_arbor_attributum_numeri(st, elementum, "n",
                 trivium->valor.mensura))
        {
            st->causa = "numerus trivii scribi non potuit";
            redde NIHIL;
        }
        frange;

    casus MATERIA_LEX_TERMINATOR:
        /* '\r\n' contra '\n': octetus '\r' adest an non */
        si (   trivium->valor.mensura >= II
            && trivium->valor.datum[trivium->valor.mensura - II]
                   == (i8)'\r')
        {
            stml_attributum_boolean_addere(elementum, st->piscina,
                st->intern, "crlf");
        }
        frange;

    casus MATERIA_LEX_FIXUM:
        /* orthographia in genere IPSO vivit - nihil scribendum */
        frange;

    ordinarius:
        /* VERBATIM: commenta et cetera - valor ut textus */
        si (!_textus_tutus(&trivium->valor))
        {
            st->causa = "valor trivii textui non tutus";
            redde NIHIL;
        }
        si (trivium->valor.mensura > ZEPHYRUM)
        {
            StmlNodus* textus = stml_textum_creare_ex_chorda(
                st->piscina, st->intern, trivium->valor);

            si (textus == NIHIL || !stml_liberum_addere(elementum, textus))
            {
                st->causa = "textus trivii addi non potuit";
                redde NIHIL;
            }
        }
        frange;
    }
    redde elementum;
}

interior StmlNodus*
_vocatio_spatiorum (MateriaArborScriptor* st, constans character* praefixum,
                    i32 numerus_spatiorum)
{
    character numeri[XVI];
    character buffer[48];
       chorda valor;
          i32 lp;
          i32 ln;

    si (_numerus_ad_literas(numerus_spatiorum, numeri,
            (i32)magnitudo(numeri)) == ZEPHYRUM)
    {
        st->causa = "numerus spatiorum scribi non potuit";
        redde NIHIL;
    }
    lp = (i32)strlen(praefixum);
    ln = (i32)strlen(numeri);
    memcpy(buffer, praefixum, (size_t)lp);
    memcpy(buffer + lp, numeri, (size_t)ln);
    buffer[lp + ln] = '"';
    valor.datum     = (i8*)buffer;
    valor.mensura   = lp + ln + I;
    st->census.spatia_vocationes++;
    redde stml_transclusionem_creare(st->piscina, st->intern, valor);
}

interior StmlNodus*
_involucrum_triviorum (
    MateriaArborScriptor* st,
    MateriaToken* constans* trivia,
                       i32  numerus,
      constans character*  titulus)
{
    StmlNodus* involucrum;
          i32  i;

    si (trivia == NIHIL || numerus == ZEPHYRUM) { redde NIHIL; }

    /* TEMPLUM: involucrum cum spatio UNO SOLO ut vocatio scribitur.
     * genus_spatii ex lexico venit - littera hic tabula QUARTA
     * veritatis esset. */
    si (st->consilium->templa_activa && numerus == I)
    {
        constans MateriaToken* trivium = trivia[0];
        s32 gs = st->consilium->lexicon->lexicon->genus_spatii;

        si (   trivium != NIHIL && gs >= ZEPHYRUM
            && trivium->genus == gs)
        {
            si (strcmp(titulus, MATERIA_ARBOR_TAG_POST) == ZEPHYRUM)
            {
                redde _vocatio_spatiorum(st, "#@post-spatia n=\"",
                    trivium->valor.mensura);
            }
            si (strcmp(titulus, MATERIA_ARBOR_TAG_ANTE) == ZEPHYRUM)
            {
                redde _vocatio_spatiorum(st, "#@ante-spatia n=\"",
                    trivium->valor.mensura);
            }
        }
    }

    involucrum = stml_elementum_creare(st->piscina, st->intern, titulus);
    si (involucrum == NIHIL)
    {
        st->causa = "involucrum triviorum creari non potuit";
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* elementum = _trivium_scribere(st, trivia[i]);

        si (elementum == NIHIL) { redde NIHIL; }
        si (!stml_liberum_addere(involucrum, elementum))
        {
            st->causa = "trivium addi non potuit";
            redde NIHIL;
        }
    }
    redde involucrum;
}


/* ==================================================
 * Lexema
 * ================================================== */

interior StmlNodus*
_scribere_lexema (MateriaArborScriptor* st, constans MateriaToken* lexema)
{
    character   tag[MATERIA_ARBOR_TAG_CAPACITAS];
    character   nomen_fragmenti[XXXII];
    ArborNota*  nota;
    StmlNodus*  elementum;
    StmlNodus*  involucrum;
    MateriaSedes sedes;
    constans MateriaArborFrons* frons;

    si (lexema == NIHIL)
    {
        st->causa = "lexema nihil";
        redde NIHIL;
    }
    frons = st->consilium->frons;
    nota  = _nota_lexematis(st, lexema);

    /* Usus secundus et sequentes: TRANSCLUSIO. Identitas res est -
     * duplicatio mentiretur (bracchia ambigua lexemata EADEM ferunt). */
    si (nota != NIHIL && nota->usus > I && nota->emissum)
    {
        StmlNodus* transclusio;

        si (_numerus_ad_literas(nota->numerus, nomen_fragmenti + IV,
                (i32)magnitudo(nomen_fragmenti) - IV) == ZEPHYRUM)
        {
            st->causa = "nomen fragmenti scribi non potuit";
            redde NIHIL;
        }
        nomen_fragmenti[0] = '#';
        nomen_fragmenti[1] = 'l';
        nomen_fragmenti[2] = 'e';
        nomen_fragmenti[3] = 'x';

        transclusio = stml_elementum_creare(st->piscina, st->intern,
            "transclusio");
        si (transclusio == NIHIL)
        {
            st->causa = "transclusio creari non potuit";
            redde NIHIL;
        }
        transclusio->genus = STML_NODUS_TRANSCLUSIO;
        transclusio->valor = chorda_internare_ex_literis(st->intern,
            nomen_fragmenti);
        si (transclusio->valor == NIHIL)
        {
            st->causa = "valor transclusionis internari non potuit";
            redde NIHIL;
        }
        redde transclusio;
    }

    si (materia_arbor_lexema_tag(st->consilium->lexicon, lexema->genus,
            tag, (i32)magnitudo(tag)) == ZEPHYRUM)
    {
        st->causa = "genus lexematis ignotum";
        redde NIHIL;
    }
    elementum = stml_elementum_creare(st->piscina, st->intern, tag);
    si (elementum == NIHIL)
    {
        st->causa = "elementum lexematis creari non potuit";
        redde NIHIL;
    }

    /* Par pro tabula sedium. Semita transclusionis supra iam rediit,
     * ergo lexema communicatum hic SEMEL solum venit. */
    si (st->paria != NIHIL)
    {
        ArborPar* par = (ArborPar*)xar_addere(st->paria);

        si (par != NIHIL)
        {
            par->clavis     = (constans vacuum*)lexema;
            par->est_lexema = VERUM;
            par->elementum  = elementum;
        }
    }

    /* ATTRIBUTA FRONTIS PRIMA - ordo octetim visibilis */
    si (frons != NIHIL && frons->attributa_ornare != NIHIL)
    {
        si (!frons->attributa_ornare(frons->datum, st, elementum, lexema))
        {
            redde NIHIL;
        }
    }

    si (   lexema->fons_index >= ZEPHYRUM
        && lexema->fons_index != st->ancora_fons)
    {
        si (!materia_arbor_attributum_numeri(st, elementum, "f",
                 (i32)lexema->fons_index))
        {
            st->causa = "fons scribi non potuit";
            redde NIHIL;
        }
    }

    /* ante */
    involucrum = _involucrum_triviorum(st, lexema->spatia_ante,
        lexema->numerus_ante, MATERIA_ARBOR_TAG_ANTE);
    si (involucrum == NIHIL && st->causa != NIHIL) { redde NIHIL; }
    si (involucrum != NIHIL && !stml_liberum_addere(elementum, involucrum))
    {
        st->causa = "involucrum ante addi non potuit";
        redde NIHIL;
    }

    /* valor - SOLUM generibus VERBATIM. Orthographia fixa in genere
     * IPSO vivit; eam scribere fontem veritatis tertium faceret. */
    si (materia_lexicon_textum_fert(st->consilium->lexicon, lexema->genus))
    {
        si (!_textus_tutus(&lexema->valor))
        {
            st->causa = "valor lexematis textui non tutus";
            redde NIHIL;
        }
        si (lexema->valor.mensura > ZEPHYRUM)
        {
            StmlNodus* textus = stml_textum_creare_ex_chorda(st->piscina,
                st->intern, lexema->valor);

            si (textus == NIHIL || !stml_liberum_addere(elementum, textus))
            {
                st->causa = "textus lexematis addi non potuit";
                redde NIHIL;
            }
        }
    }

    /* post */
    involucrum = _involucrum_triviorum(st, lexema->spatia_post,
        lexema->numerus_post, MATERIA_ARBOR_TAG_POST);
    si (involucrum == NIHIL && st->causa != NIHIL) { redde NIHIL; }
    si (involucrum != NIHIL && !stml_liberum_addere(elementum, involucrum))
    {
        st->causa = "involucrum post addi non potuit";
        redde NIHIL;
    }

    /* SEDES PORTATA pro lexemate DERIVATO, et solum pro eo.
     *
     * Lexema derivatum sedem DEF-SITE fert - in plagula ALIA. Ex hoc
     * fluxu derivari NEQUIT quia in hoc fluxu OMNINO NON EST: octetos
     * invocatio tenet. Lex 'sedes derivatae' integra manet UBI
     * derivatio possibilis est; ubi non est, portare honestius quam
     * fingere. */
    materia_sedes_tokeni(st->consilium->origo, lexema, &sedes);
    si (!sedes.est_fons)
    {
        si (lexema->byte_offset >= ZEPHYRUM)
        {
            materia_arbor_attributum_numeri(st, elementum, "b",
                (i32)lexema->byte_offset);
        }
        materia_arbor_attributum_numeri(st, elementum, "linea",
            lexema->linea);
        materia_arbor_attributum_numeri(st, elementum, "columna",
            lexema->columna);
        si (materia_token_initium_lineae(lexema))
        {
            stml_attributum_boolean_addere(elementum, st->piscina,
                st->intern, "linea-initium");
        }
    }

    /* LIBERI FRONTIS ULTIMI (C89: scissurae, origo nestata) - post
     * trivia, ante involucrum fragmenti, ut fragmentum lexema TOTUM
     * cum origine sua ferat. */
    si (frons != NIHIL && frons->liberos_ornare != NIHIL)
    {
        si (!frons->liberos_ornare(frons->datum, st, elementum, lexema))
        {
            redde NIHIL;
        }
    }

    /* Octeti codicis: genus crudum cum elementum SOLUM textum ferat.
     * POST liberos OMNES - elementum mixtum crudum esse NON potest. */
    si (   materia_lexicon_textum_fert(st->consilium->lexicon,
               lexema->genus)
        && lexema->valor.mensura > ZEPHYRUM
        && stml_numerus_liberorum(elementum) == I
        && !_valorem_crudum_notare(st, elementum, tag, &lexema->valor))
    {
        redde NIHIL;
    }

    /* Usus primus lexematis COMMUNICATI: fragmentum nominatum.
     * Passus I numeravit, ergo hic iam scimus - quod est ipsa ratio
     * cur passus duo sunt. */
    si (nota != NIHIL && nota->usus > I)
    {
        StmlNodus* fragmentum;

        st->numerus_notarum++;
        nota->numerus = st->numerus_notarum;
        nota->emissum = VERUM;

        si (_numerus_ad_literas(nota->numerus, nomen_fragmenti + III,
                (i32)magnitudo(nomen_fragmenti) - III) == ZEPHYRUM)
        {
            st->causa = "nomen fragmenti scribi non potuit";
            redde NIHIL;
        }
        nomen_fragmenti[0] = 'l';
        nomen_fragmenti[1] = 'e';
        nomen_fragmenti[2] = 'x';

        fragmentum = stml_elementum_creare(st->piscina, st->intern,
            "fragmentum");
        si (fragmentum == NIHIL)
        {
            st->causa = "fragmentum creari non potuit";
            redde NIHIL;
        }
        fragmentum->fragmentum    = VERUM;
        fragmentum->fragmentum_id = chorda_internare_ex_literis(st->intern,
            nomen_fragmenti);
        si (   fragmentum->fragmentum_id == NIHIL
            || !stml_liberum_addere(fragmentum, elementum))
        {
            st->causa = "lexema in fragmentum addi non potuit";
            redde NIHIL;
        }
        redde fragmentum;
    }
    redde elementum;
}


/* ==================================================
 * Nodus et valor
 * ================================================== */

interior StmlNodus* _scribere_nodum_internum (MateriaArborScriptor*,
    constans MateriaNodus*);

/* Species custodia est, non ornamentum: materia_nodus_appendere
 * speciem listae solam probat, NUMQUAM quid intus eat - ergo
 * elementa listae mixtae HIC custodienda sunt. */
interior b32
_scribere_valorem_in (
    MateriaArborScriptor* st,
               StmlNodus* parens,
             MateriaValor valor,
      MateriaLocusSpecies species,
   constans MateriaNodus* sedes)
{
    StmlNodus* liberum;
    character  buffer[XVI];
          i32  numerus;
          i32  i;

    commutatio (species)
    {
    casus MATERIA_LOCUS_NODUS:
        si (valor.genus != MATERIA_VALOR_NODUS)
        {
            st->causa = "locus NODUS valorem alienum fert";
            st->sedes = sedes;
            redde FALSUM;
        }
        liberum = _scribere_nodum_internum(st, valor.datum.nodus);
        si (liberum == NIHIL) { redde FALSUM; }
        redde stml_liberum_addere(parens, liberum);

    casus MATERIA_LOCUS_TOKEN:
        si (valor.genus != MATERIA_VALOR_TOKEN)
        {
            st->causa = "locus TOKEN valorem alienum fert";
            st->sedes = sedes;
            redde FALSUM;
        }
        liberum = _scribere_lexema(st, valor.datum.token);
        si (liberum == NIHIL) { st->sedes = sedes; redde FALSUM; }
        redde stml_liberum_addere(parens, liberum);

    casus MATERIA_LOCUS_INDEX:
        si (valor.genus != MATERIA_VALOR_INDEX)
        {
            st->causa = "locus INDEX valorem alienum fert";
            st->sedes = sedes;
            redde FALSUM;
        }
        si (valor.datum.index < ZEPHYRUM)
        {
            st->causa = "index negativus";
            st->sedes = sedes;
            redde FALSUM;
        }
        si (_numerus_ad_literas((i32)valor.datum.index, buffer,
                (i32)magnitudo(buffer)) == ZEPHYRUM)
        {
            st->causa = "index scribi non potuit";
            st->sedes = sedes;
            redde FALSUM;
        }
        redde stml_textum_addere(parens, st->piscina, st->intern, buffer);

    casus MATERIA_LOCUS_LISTA_NODUS:
    casus MATERIA_LOCUS_LISTA_TOKEN:
    casus MATERIA_LOCUS_LISTA_MIXTA:
        si (valor.genus != MATERIA_VALOR_LISTA)
        {
            st->causa = "locus LISTA valorem alienum fert";
            st->sedes = sedes;
            redde FALSUM;
        }
        /* Mensura PROSPECTUS, numquam xar_numerus repositorii */
        numerus = materia_valor_lista_numerus(valor);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            MateriaValor* e = materia_valor_lista_obtinere(valor, i);

            si (e == NIHIL)
            {
                st->causa = "elementum listae nihil";
                st->sedes = sedes;
                redde FALSUM;
            }
            si (e->genus == MATERIA_VALOR_NODUS)
            {
                si (species == MATERIA_LOCUS_LISTA_TOKEN)
                {
                    st->causa = "nodus in lista TOKEN";
                    st->sedes = sedes;
                    redde FALSUM;
                }
                liberum = _scribere_nodum_internum(st, e->datum.nodus);
            }
            alioquin si (e->genus == MATERIA_VALOR_TOKEN)
            {
                si (species == MATERIA_LOCUS_LISTA_NODUS)
                {
                    st->causa = "lexema in lista NODUS";
                    st->sedes = sedes;
                    redde FALSUM;
                }
                liberum = _scribere_lexema(st, e->datum.token);
            }
            alioquin
            {
                st->causa = "elementum listae generis alieni";
                st->sedes = sedes;
                redde FALSUM;
            }
            si (liberum == NIHIL)
            {
                si (st->sedes == NIHIL) { st->sedes = sedes; }
                redde FALSUM;
            }
            si (!stml_liberum_addere(parens, liberum))
            {
                st->causa = "elementum listae addi non potuit";
                st->sedes = sedes;
                redde FALSUM;
            }
        }
        redde VERUM;

    ordinarius:
        st->causa = "species loci ignota";
        st->sedes = sedes;
        redde FALSUM;
    }
}

interior StmlNodus*
_scribere_nodum_internum (MateriaArborScriptor* st,
                          constans MateriaNodus* nodus)
{
    constans MateriaTabGenus* genus;
                  StmlNodus*  elementum;
                        i32   i;

    si (nodus == NIHIL) { st->causa = "nodus nihil"; redde NIHIL; }
    si (   nodus->genus < ZEPHYRUM
        || (i32)nodus->genus
               >= st->consilium->tabularium->numerus_generum)
    {
        st->causa = "genus registro ignotum";
        st->sedes = nodus;
        redde NIHIL;
    }
    genus = &st->consilium->tabularium->genera[nodus->genus];

    /* Forma nodi cum forma registri congruere DEBET - aliter loci et
     * species inter se labuntur et documentum TACITE mentitur. */
    si (nodus->numerus_locorum != genus->loci_numerus)
    {
        st->causa = "forma nodi registro non congruit";
        st->sedes = nodus;
        redde NIHIL;
    }

    elementum = stml_elementum_creare(st->piscina, st->intern,
        genus->titulus);
    si (elementum == NIHIL)
    {
        st->causa = "elementum generis creari non potuit";
        st->sedes = nodus;
        redde NIHIL;
    }
    si (st->paria != NIHIL)
    {
        ArborPar* par = (ArborPar*)xar_addere(st->paria);

        si (par != NIHIL)
        {
            par->clavis     = (constans vacuum*)nodus;
            par->est_lexema = FALSUM;
            par->elementum  = elementum;
        }
    }

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaTabLocus* locus;
                      StmlNodus*  involucrum;
                            i32   absolutus;

        /* Locus NIHIL = elementum OMISSUM (absentia canonica) */
        si (nodus->loci[i].genus == MATERIA_VALOR_NIHIL) { perge; }

        absolutus = genus->loci_offset + i;
        si (absolutus >= st->consilium->tabularium->numerus_locorum)
        {
            st->causa = "locus extra tabulam";
            st->sedes = nodus;
            redde NIHIL;
        }
        locus = &st->consilium->tabularium->loci[absolutus];

        involucrum = stml_elementum_creare(st->piscina, st->intern,
            locus->titulus);
        si (involucrum == NIHIL)
        {
            st->causa = "involucrum loci creari non potuit";
            st->sedes = nodus;
            redde NIHIL;
        }
        si (!_scribere_valorem_in(st, involucrum, nodus->loci[i],
                 (MateriaLocusSpecies)locus->species, nodus))
        {
            redde NIHIL;
        }
        si (!stml_liberum_addere(elementum, involucrum))
        {
            st->causa = "involucrum loci addi non potuit";
            st->sedes = nodus;
            redde NIHIL;
        }
    }
    redde elementum;
}


/* ==================================================
 * Ingressus
 * ================================================== */

vacuum
materia_arbor_consilium_nudum (
        MateriaArborConsilium* consilium,
    constans MateriaRegistrumCoctum* tabularium,
       constans MateriaLexiconRatum* lexicon,
                 constans character* grammatica)
{
    si (consilium == NIHIL) { redde; }
    consilium->tabularium      = tabularium;
    consilium->lexicon         = lexicon;
    consilium->grammatica      = grammatica;
    consilium->origo           = NIHIL;
    consilium->frons           = NIHIL;
    consilium->intern          = NIHIL;
    consilium->templa_activa   = FALSUM;
    consilium->sedes_colligere = FALSUM;
}

MateriaArborScriptura
materia_arbor_scribere_nodum (
                        Piscina* piscina,
          constans MateriaNodus* nodus,
    constans MateriaArborConsilium* consilium)
{
    MateriaArborScriptura fructus;
    MateriaArborScriptor  st;
               StmlNodus* involucrum;
               StmlNodus* radix;
                  chorda  sigillum;
     InternamentumChorda* intern;

    fructus.successus     = FALSUM;
    fructus.textus.mensura = ZEPHYRUM;
    fructus.textus.datum   = NIHIL;
    fructus.causa          = NIHIL;
    fructus.sedes          = NIHIL;
    fructus.sedes_valorum  = NIHIL;
    fructus.census.spatia_vocationes = ZEPHYRUM;

    si (   piscina == NIHIL || nodus == NIHIL || consilium == NIHIL
        || consilium->tabularium == NIHIL || consilium->lexicon == NIHIL)
    {
        fructus.causa = "argumenta nihil";
        redde fructus;
    }
    si (consilium->grammatica == NIHIL)
    {
        fructus.causa = "grammatica innominata";
        redde fructus;
    }
    si (!consilium->lexicon->ratum)
    {
        fructus.causa = "lexicon non ratum (porta oneris non transita)";
        redde fructus;
    }

    intern = consilium->intern;
    si (intern == NIHIL)
    {
        intern = internamentum_creare(piscina);
        si (intern == NIHIL)
        {
            fructus.causa = "internamentum creari non potuit";
            redde fructus;
        }
    }

    st.piscina         = piscina;
    st.intern          = intern;
    st.consilium       = consilium;
    st.numerus_notarum = ZEPHYRUM;
    st.ancora_nota     = FALSUM;
    st.ancora_offset   = (s32)-I;
    st.ancora_linea    = ZEPHYRUM;
    st.ancora_columna  = ZEPHYRUM;
    st.ancora_fons     = ZEPHYRUM;
    st.ancora_initium_lineae = FALSUM;
    st.causa           = NIHIL;
    st.sedes           = NIHIL;
    st.census.spatia_vocationes = ZEPHYRUM;
    st.lexemata = tabula_dispersa_creare_chorda(piscina, 256);
    st.paria    = consilium->sedes_colligere
        ? xar_creare(piscina, magnitudo(ArborPar)) : NIHIL;
    si (st.lexemata == NIHIL)
    {
        fructus.causa = "tabula lexematum creari non potuit";
        redde fructus;
    }

    /* PASSUS I: usus numerare + ancoram capere. Sine hoc usus primus
     * scire non posset an fragmentum opus esset. */
    _numerare_nodum(&st, nodus);

    involucrum = stml_elementum_creare(piscina, intern,
        MATERIA_ARBOR_TAG_ENVOLUCRI);
    si (involucrum == NIHIL)
    {
        fructus.causa = "involucrum creari non potuit";
        redde fructus;
    }
    stml_attributum_addere(involucrum, piscina, intern, "grammatica",
        consilium->grammatica);

    sigillum = materia_arbor_sigillum(piscina, consilium->tabularium);
    si (sigillum.mensura == ZEPHYRUM)
    {
        fructus.causa = "sigillum computari non potuit";
        redde fructus;
    }
    stml_attributum_addere_chorda(involucrum, piscina, intern,
        "registrum-sigillum", sigillum);

    /* ANCORA sola - positiones ceterae ambulatione derivantur.
     * Lex: documentum canonicum mentiri non possit. */
    si (st.ancora_nota && st.ancora_offset >= ZEPHYRUM)
    {
        materia_arbor_attributum_numeri(&st, involucrum, "fons",
            (i32)st.ancora_fons);
        materia_arbor_attributum_numeri(&st, involucrum, "b",
            (i32)st.ancora_offset);
        materia_arbor_attributum_numeri(&st, involucrum, "linea",
            st.ancora_linea);
        materia_arbor_attributum_numeri(&st, involucrum, "columna",
            st.ancora_columna);
        si (st.ancora_initium_lineae)
        {
            stml_attributum_boolean_addere(involucrum, piscina, intern,
                "linea-initium");
        }
    }

    /* PASSUS II */
    radix = _scribere_nodum_internum(&st, nodus);
    si (radix == NIHIL)
    {
        fructus.causa = st.causa ? st.causa : "scriptura fracta";
        fructus.sedes = st.sedes;
        redde fructus;
    }
    si (!stml_liberum_addere(involucrum, radix))
    {
        fructus.causa = "radix in involucrum addi non potuit";
        redde fructus;
    }

    fructus.textus    = stml_scribere(involucrum, piscina, VERUM);
    fructus.census    = st.census;
    fructus.successus = VERUM;
    redde fructus;
}
