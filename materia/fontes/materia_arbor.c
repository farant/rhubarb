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
     constans MateriaArborConsilium* consilium;
                     TabulaDispersa* lexemata;
                                Xar* paria;    /* ArborPar; NIHIL = nulla */
                                i32  numerus_notarum;

    /* Ancora - vide legem in capite */
                                b32 ancora_nota;
                                s32 ancora_offset;
                                i32 ancora_linea;
                                i32 ancora_columna;
                                s32 ancora_fons;
                                b32 ancora_initium_lineae;

                 MateriaArborCensus  census;
                 constans character* causa;
              constans MateriaNodus* sedes;
};


/* ==================================================
 * Accessus pro fronte
 * ================================================== */

Piscina*
materia_arbor_scriptor_piscina (
    MateriaArborScriptor* scriptor)
{
    redde (scriptor == NIHIL) ? NIHIL : scriptor->piscina;
}

InternamentumChorda*
materia_arbor_scriptor_intern (
    MateriaArborScriptor* scriptor)
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

    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;

    si (piscina == NIHIL || tabularium == NIHIL)
    { redde vacua;
    }
    mat = chorda_aedificator_creare(piscina, 4096);
    si (mat == NIHIL)
    { redde vacua;
    }

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
    si (exitus == NIHIL)
    { redde vacua;
    }
    per (i = ZEPHYRUM; i < MATERIA_ARBOR_SIGILLI_LONGITUDO; i++)
    {
        i32 gradus = (MATERIA_ARBOR_SIGILLI_LONGITUDO - I - i) * IV;
        i32 nibble = (friatum >> gradus) & (i32)0xF;

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

    si (tabularium == NIHIL || titulus == NIHIL)
    { redde (s32)-I;
    }
    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans character* t = tabularium->genera[i].titulus;

        si (   t                                   != NIHIL
            && (i32)strlen(t)                      == mensura
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
                       character* buffer,
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
    si (titulus == NIHIL)
    { redde ZEPHYRUM;
    }
    praefixum = ratum->lexicon->praefixum_tagi;

    lp       = (i32)strlen(praefixum);
    scripta  = lp + (i32)strlen(titulus);
    si (scripta + I > capacitas)
    { redde ZEPHYRUM;
    }

    memcpy(buffer, praefixum, (size_t)lp);
    per (i = ZEPHYRUM; titulus[i] != '\0'; i++)
    {
        character c = titulus[i];

        si (c >= 'A' && c <= 'Z')
        { c = (character)(c - 'A' + 'a');
        }
        alioquin si (c == '_')
        { c = '-';
        }
        buffer[lp + i] = c;
    }
    buffer[scripta] = '\0';
    redde scripta;
}

s32
materia_arbor_lexema_ex_tag (
    constans MateriaLexiconRatum* ratum,
              constans character* tag,
                             i32  mensura)
{
    character buffer[MATERIA_ARBOR_TAG_CAPACITAS];
          s32 i;
          s32 n;

    si (ratum == NIHIL)
    { redde ZEPHYRUM;
    }
    n = (s32)ratum->lexicon->numerus_generum;
    si (tag == NIHIL || mensura == ZEPHYRUM)
    { redde n;
    }

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
_clavis_monstratoris (
            Piscina* piscina,
    constans vacuum* monstrator)
{
    vacuum* cella;
    chorda  clavis;

    cella = piscina_allocare(piscina, magnitudo(constans vacuum*));
    si (cella == NIHIL)
    {
        clavis.mensura  = ZEPHYRUM;
        clavis.datum    = NIHIL;
        redde clavis;
    }
    *(constans vacuum**)cella  = monstrator;
    clavis.mensura             = (i32)magnitudo(constans vacuum*);
    clavis.datum               = (i8*)cella;
    redde clavis;
}

interior ArborNota*
_nota_lexematis (
     MateriaArborScriptor* st,
    constans MateriaToken* lexema)
{
    vacuum* inventum;
    chorda  clavis;

    clavis = _clavis_monstratoris(st->piscina,
        (constans vacuum*)lexema);
    si (clavis.datum == NIHIL)
    { redde NIHIL;
    }
    si (tabula_dispersa_invenire(st->lexemata, clavis, &inventum))
    {
        redde (ArborNota*)inventum;
    }
    redde NIHIL;
}


/* ==================================================
 * Passus I - usus numerare + ancoram capere
 * ================================================== */

interior vacuum
_numerare_valorem (
    MateriaArborScriptor*,
    MateriaValor);

interior vacuum
_numerare_lexema (
     MateriaArborScriptor* st,
    constans MateriaToken* lexema)
{
    ArborNota* nota;

    si (lexema == NIHIL)
    { redde;
    }

    nota = _nota_lexematis(st, lexema);
    si (nota != NIHIL)
    { nota->usus++; redde;
    }

    nota = (ArborNota*)piscina_allocare(st->piscina,
        magnitudo(ArborNota));
    si (nota == NIHIL)
    { redde;
    }
    nota->usus     = I;
    nota->numerus  = ZEPHYRUM;
    nota->emissum  = FALSUM;
    tabula_dispersa_inserere(st->lexemata,
        _clavis_monstratoris(st->piscina, (constans vacuum*)lexema),
        nota);

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
                      (MateriaToken*)(size_t)(constans vacuum*)lexema,
                      &causa);

            si (r != NIHIL)
            { emissum = r;
            }
        }
        initium = emissum;

        /* TRIVIUM PRIMUM, non lexema: emissio triviis ducentibus
         * incipit. Silva id CLXXVIII divergentiis mensuravit. */
        si (   emissum->numerus_ante > ZEPHYRUM
            && emissum->spatia_ante[0]              != NIHIL
            && emissum->spatia_ante[0]->byte_offset >= ZEPHYRUM)
        {
            initium = emissum->spatia_ante[0];
        }

        st->ancora_nota     = VERUM;
        st->ancora_offset   = initium->byte_offset;
        st->ancora_linea    = initium->linea;
        st->ancora_columna  = initium->columna;
        st->ancora_fons     = emissum->fons_index;
        /* NON DERIVABILE ex subarbore: an lexema primum lineam
         * incipiat pendet ab eo quod ANTE subarborem stat. */
        st->ancora_initium_lineae =
            materia_token_initium_lineae(emissum);
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
_numerare_nodum (
     MateriaArborScriptor* st,
    constans MateriaNodus* nodus)
{
    i32 i;

    si (nodus == NIHIL)
    { redde;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        _numerare_valorem(st, nodus->loci[i]);
    }
}

interior vacuum
_numerare_valorem (
    MateriaArborScriptor* st,
            MateriaValor  valor)
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

            si (e != NIHIL)
            { _numerare_valorem(st, *e);
            }
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
_numerus_ad_literas (
          i32  numerus,
    character* buffer,
          i32  capacitas)
{
    character inversa[XVI];
          i32 longitudo;
          i32 i;

    si (buffer == NIHIL || capacitas < II)
    { redde ZEPHYRUM;
    }
    si (numerus == ZEPHYRUM)
    {
        buffer[0] = '0'; buffer[1] = '\0'; redde I;
    }
    longitudo = ZEPHYRUM;
    dum (numerus > ZEPHYRUM && longitudo < (i32)magnitudo(inversa))
    {
        inversa[longitudo]  = (character)('0' + (numerus % X));
        numerus             /= X;
        longitudo++;
    }
    si (longitudo + I > capacitas)
    { redde ZEPHYRUM;
    }
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

    si (scriptor == NIHIL)
    { redde FALSUM;
    }
    si (_numerus_ad_literas(numerus, buffer, (i32)magnitudo(buffer))
        == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde stml_attributum_addere(elementum, scriptor->piscina,
        scriptor->intern, titulus, buffer);
}

/* An chorda NUL ferat. Periculum TACITUM et INSANABILE: chorda
 * longitudinem fert, textus emissus terminatore legetur. Crudus an
 * non nihil refert - NUL nusquam repraesentabilis est. */
interior b32
_nul_fert (
    constans chorda* valor)
{
    i32 i;

    si (valor == NIHIL)
    { redde FALSUM;
    }
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        si (valor->datum[i] == (i8)'\0')
        { redde VERUM;
        }
    }
    redde FALSUM;
}

/* Textus TUTUS pro elemento NON-CRUDO.
 *
 * DUAE QUAESTIONES DISTINCTAE, olim in una functione confusae:
 *
 *   NUL          numquam repraesentabilis, crudus an non
 *   album solum  ambiguum cum DISPOSITIONE pulchri - SED SOLUM
 *                si elementum crudum non sit
 *
 * MENSURATUM 2026-08-28, non ratiocinatum:
 *   <lex-spatia>\n\n  </>    pulchrum, non crudum -> TEXTUS PERDITUR
 *   <lex-spatia!>\n\n  </>   pulchrum, CRUDUM     -> "\n\n  " IDEM
 * et idem nidificatum, ubi pulcher indentationem revera generat.
 *
 * Ergo refutatio albi recta est HIC et NIMIS LATA in semita
 * triviorum, quae elementum CRUDUM facit duas lineas infra. Vide
 * ibi. */
interior b32
_textus_tutus (
    constans chorda* valor)
{
    i32 i;
    b32 album_solum;

    si (valor == NIHIL || valor->mensura == ZEPHYRUM)
    { redde VERUM;
    }
    si (_nul_fert(valor))
    { redde FALSUM;
    }
    album_solum = VERUM;
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        character c = (character)valor->datum[i];

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

    longitudo           = (i32)strlen(tag);
    clausura[ZEPHYRUM]  = '<';
    clausura[I]         = '/';
    memcpy(clausura + II, tag, (size_t)longitudo);
    clausura[II + longitudo]  = '>';
    longitudo                 += III;

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

/* '\r' IN TEXTU STML NON VIVIT: lector documenti CRLF ad LF normat,
 * crudus an non (MENSURATUM per probam stml directam, B6 - 'a\r\nb'
 * crudum ut 'a\nb' redit). Silva id pro TERMINATORE suo booleano
 * 'crlf' solvit; generibus VERBATIM (CSS: spatia mixta; C89:
 * commenta - quorum vitium latens par silva numquam tetigit quia
 * corpus CR in commento non continuit; replicatio phasis V) positio
 * QUAEQUE opus est. Ergo: textus SINE '\r' scribitur, attributum
 * 'cr' offsets in valore VERO fert (decimales, spatiis separati),
 * lector reinserit. Attributum abest = valor sine '\r' - casus
 * communis, pretium nullum. */
interior b32
_cr_exuere (
    MateriaArborScriptor* st,
               StmlNodus* elementum,
         constans chorda* valor,
                  chorda* nudus)
{
    i32 numerus_cr;
    i32 i;

    numerus_cr = ZEPHYRUM;
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        si (valor->datum[i] == (i8)'\r')
        {
            numerus_cr = numerus_cr + I;
        }
    }
    si (numerus_cr == ZEPHYRUM)
    {
        *nudus = *valor;
        redde VERUM;
    }

    {
        character* littera;
        character  buffer[XVI];
               i8* datum;
              i32  scriptum  = ZEPHYRUM;
              i32  j         = ZEPHYRUM;

        littera = (character*)piscina_allocare(st->piscina,
            (memoriae_index)(numerus_cr * XIII + I));
        si (littera == NIHIL)
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < valor->mensura; i++)
        {
            si (valor->datum[i] == (i8)'\r')
            {
                i32 longitudo = _numerus_ad_literas(i, buffer,
                    (i32)magnitudo(buffer));

                si (longitudo == ZEPHYRUM)
                {
                    redde FALSUM;
                }
                si (scriptum > ZEPHYRUM)
                {
                    littera[scriptum]  = ' ';
                    scriptum           = scriptum + I;
                }
                memcpy(littera + scriptum, buffer, (size_t)longitudo);
                scriptum = scriptum + longitudo;
            }
        }
        littera[scriptum] = '\0';
        si (!stml_attributum_addere(elementum, st->piscina,
                st->intern, "cr", littera))
        {
            redde FALSUM;
        }

        datum = (i8*)piscina_allocare(st->piscina,
            (memoriae_index)(valor->mensura - numerus_cr) + I);
        si (datum == NIHIL)
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < valor->mensura; i++)
        {
            si (valor->datum[i] != (i8)'\r')
            {
                datum[j]  = valor->datum[i];
                j         = j + I;
            }
        }
        nudus->datum    = datum;
        nudus->mensura  = valor->mensura - numerus_cr;
    }
    redde VERUM;
}


/* ==================================================
 * Trivia - forma per SPECIEM lexici, non per genus notum
 * ================================================== */

interior StmlNodus*
_trivium_scribere (
     MateriaArborScriptor* st,
    constans MateriaToken* trivium)
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
        /* VERBATIM: commenta, spatia verbatim, cetera - valor ut textus.
         *
         * NUL SOLUM refutatur. Album solum NON refutatur, quia hoc
         * elementum CRUDUM fit duabus lineis infra
         * (_valorem_crudum_notare), et pulcher crudum NUMQUAM tangit.
         *
         * OLIM _textus_tutus hic vocabatur, quod album refutat -
         * custodia condicionis quae accidere NON POTEST, quia
         * notatio cruda semper sequitur. Pretium erat REALE: lingua
         * cuius spatium genus VERBATIM est (CSS, ubi unum genus
         * spatia+tabulas+lineas absorbet) plagulam quamlibet spatium
         * ferentem proicere NON POTERAT - centesimam partem CSS
         * veri. Inventum a CSS T9, MENSURATUM per probam stml
         * directam (vide _textus_tutus supra), non coniectatum.
         *
         * Si notatio cruda FALLAT (valor sequentiam claudentem fert),
         * scriptor infra NIHIL reddit - ergo textus albus numquam
         * sine cruditate emittitur. */
        si (_nul_fert(&trivium->valor))
        {
            st->causa = "valor trivii NUL fert";
            redde NIHIL;
        }
        {
            chorda nudus;

            si (!_cr_exuere(st, elementum, &trivium->valor, &nudus))
            {
                st->causa = "attributum 'cr' trivii scribi non potuit";
                redde NIHIL;
            }
            si (nudus.mensura > ZEPHYRUM)
            {
                StmlNodus* textus = stml_textum_creare_ex_chorda(
                    st->piscina, st->intern, nudus);

                si (   textus == NIHIL
                    || !stml_liberum_addere(elementum, textus))
                {
                    st->causa = "textus trivii addi non potuit";
                    redde NIHIL;
                }
                /* OCTETI CODICIS, NON PROSA. Commentum ut textus
                 * nudus scriptum a pulchro RE-IMPLERI potest; genus
                 * crudum id prohibet. Silva id agit et ego portans
                 * OMISI - shim differentiam cepit, quod est ipsum
                 * officium oraculi separantis. */
                si (!_valorem_crudum_notare(st, elementum, tag,
                         &nudus))
                {
                    redde NIHIL;
                }
            }
        }
        frange;
    }
    redde elementum;
}

interior StmlNodus*
_vocatio_spatiorum (
    MateriaArborScriptor* st,
      constans character* praefixum,
                     i32  numerus_spatiorum)
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
    buffer[lp + ln]  = '"';
    valor.datum      = (i8*)buffer;
    valor.mensura    = lp + ln + I;
    st->census.spatia_vocationes++;
    redde stml_transclusionem_creare(st->piscina, st->intern, valor);
}

interior StmlNodus*
_scribere_lexema (
    MateriaArborScriptor*,
    constans MateriaToken*);
interior MateriaToken*
_lexema_legere (
    MateriaArborLector*,
    StmlNodus*,
    chorda*);
interior StmlNodus*
_fragmentum_aperire (
    MateriaArborLector*,
    StmlNodus*,
    chorda**);

interior StmlNodus*
_involucrum_triviorum (
    MateriaArborScriptor* st,
    MateriaToken* constans* trivia,
                       i32  numerus,
      constans character*  titulus)
{
    StmlNodus* involucrum;
          i32  i;

    si (trivia == NIHIL || numerus == ZEPHYRUM)
    { redde NIHIL;
    }

    /* TEMPLUM: involucrum cum spatio UNO SOLO ut vocatio scribitur.
     * genus_spatii ex lexico venit - littera hic tabula QUARTA
     * veritatis esset. */
    si (st->consilium->templa_activa && numerus == I)
    {
        constans MateriaToken* trivium = trivia[0];
        s32 gs = st->consilium->lexicon->lexicon->genus_spatii;

        /* SPECIES REPETITUM POSCITUR, non genus solum.
         *
         * Vocatio 'n="4"' scribit; lector eam per speciem invertit,
         * et REPETITUM sola numerum in octetos vertit. Lingua cuius
         * genus spatii VERBATIM est (CSS: CSS_LEX_SPATIA spatia,
         * tabulas, lineas novas simul absorbet) numerum scriberet et
         * textum relegeret - circuitus TACITE frangeretur.
         *
         * Inventum descriptorem CSS scribendo, non probatione:
         * templa_activa CSS falsum est, ergo vitium LATENS erat et
         * primo consumptori qui ea accenderet erupisset. */
        si (   trivium        != NIHIL && gs >= ZEPHYRUM
            && trivium->genus == gs
            && materia_lexicon_species(st->consilium->lexicon, gs)
                   == MATERIA_LEX_REPETITUM)
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

    involucrum = stml_elementum_creare(st->piscina, st->intern,
        titulus);
    si (involucrum == NIHIL)
    {
        st->causa = "involucrum triviorum creari non potuit";
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* elementum = _trivium_scribere(st, trivia[i]);

        si (elementum == NIHIL)
        { redde NIHIL;
        }
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

StmlNodus*
materia_arbor_lexema_scribere (
                                MateriaArborScriptor* scriptor,
                               constans MateriaToken* lexema)
{
    si (scriptor == NIHIL)
    { redde NIHIL;
    }
    redde _scribere_lexema(scriptor, lexema);
}

MateriaToken*
materia_arbor_lexema_legere (
                    MateriaArborLector* lector,
                             StmlNodus* elementum,
                                chorda* fragmenti_id)
{
    chorda* id_apertum;

    si (lector == NIHIL || elementum == NIHIL)
    { redde NIHIL;
    }

    /* FRAGMENTUM HIC APERITUR, non a vocante.
     *
     * Lexema saepe fragmentum est (scriptor id sub '#lexN' deponit ut
     * transclusiones sequentes IDEM OBIECTUM inveniant). Ambulatio
     * arboris id iam aperit; superficies frontis, si aperire oblita
     * esset, titulum '#lexN' ipsum tag lexematis haberet et registrum
     * recte nesciret.
     *
     * SILVA ID EXACTE PASSA EST: _origo_legere superficies NOVA erat
     * et apertionem NON hereditavit - XXXI plagulae latinae hinc
     * RECUSABANTUR (silva_arbor.c:2795). Ergo hic in SUTURA ipsa
     * agitur, non in vocante: frons hoc errare non potest quia
     * facultas errandi ei non datur. */
    id_apertum  = NIHIL;
    elementum   = _fragmentum_aperire(lector, elementum, &id_apertum);
    si (elementum == NIHIL)
    { redde NIHIL;
    }
    redde _lexema_legere(lector, elementum,
        (fragmenti_id != NIHIL) ? fragmenti_id : id_apertum);
}

interior StmlNodus*
_scribere_lexema (
     MateriaArborScriptor* st,
    constans MateriaToken* lexema)
{
                     character  tag[MATERIA_ARBOR_TAG_CAPACITAS];
                     character  nomen_fragmenti[XXXII];
                     ArborNota* nota;
                     StmlNodus* elementum;
                     StmlNodus* involucrum;
                  MateriaSedes  sedes;
                        chorda  valor_nudus;
    constans MateriaArborFrons* frons;

    si (lexema == NIHIL)
    {
        st->causa = "lexema nihil";
        redde NIHIL;
    }
    frons  = st->consilium->frons;
    nota   = _nota_lexematis(st, lexema);

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
            par->clavis      = (constans vacuum*)lexema;
            par->est_lexema  = VERUM;
            par->elementum   = elementum;
        }
    }

    /* ATTRIBUTA FRONTIS PRIMA - ordo octetim visibilis */
    si (frons != NIHIL && frons->attributa_ornare != NIHIL)
    {
        si (!frons->attributa_ornare(frons->datum, st, elementum,
            lexema))
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
    si (involucrum == NIHIL && st->causa != NIHIL)
    { redde NIHIL;
    }
    si (   involucrum != NIHIL
        && !stml_liberum_addere(elementum, involucrum))
    {
        st->causa = "involucrum ante addi non potuit";
        redde NIHIL;
    }

    /* valor - SOLUM generibus VERBATIM. Orthographia fixa in genere
     * IPSO vivit; eam scribere fontem veritatis tertium faceret.
     *
     * NUL SOLUM hic refutatur - speculum emendationis triviorum
     * (T9): refutatio 'textui non tutus' ante conatum crudum
     * sedebat, ergo lexema significans albi valoris (CSS: spatia
     * servata regiminis contenti, D7) proici NUMQUAM poterat.
     * C89 id numquam tetigit: genera VERBATIM significantia non
     * habet. Custodia vera POST conatum crudum infra sedet. */
    valor_nudus = lexema->valor;
    si (materia_lexicon_textum_fert(st->consilium->lexicon,
        lexema->genus))
    {
        si (_nul_fert(&lexema->valor))
        {
            st->causa = "valor lexematis NUL fert";
            redde NIHIL;
        }
        si (!_cr_exuere(st, elementum, &lexema->valor, &valor_nudus))
        {
            st->causa = "attributum 'cr' lexematis scribi non potuit";
            redde NIHIL;
        }
        si (valor_nudus.mensura > ZEPHYRUM)
        {
            StmlNodus* textus =
                stml_textum_creare_ex_chorda(st->piscina,
                st->intern, valor_nudus);

            si (   textus == NIHIL
                || !stml_liberum_addere(elementum, textus))
            {
                st->causa = "textus lexematis addi non potuit";
                redde NIHIL;
            }
        }
    }

    /* TERMINATOR in LOCO (non trivium): variatio CRLF ut in triviis
     * supra - lector (_lexema_legere) 'crlf' iam legit, scriptor loci
     * eam non scribebat. C89 id numquam tetigit (NOVA_LINEA trivium
     * est); markdown terminatorem ut CONTENTUM fert (2026-09-03). */
    si (   materia_lexicon_species(st->consilium->lexicon,
        lexema->genus)
               == MATERIA_LEX_TERMINATOR
        && lexema->valor.mensura                           >= II
        && lexema->valor.datum[lexema->valor.mensura - II] == (i8)'\r')
    {
        stml_attributum_boolean_addere(elementum, st->piscina,
            st->intern,
            "crlf");
    }
    /* post */
    involucrum = _involucrum_triviorum(st, lexema->spatia_post,
        lexema->numerus_post, MATERIA_ARBOR_TAG_POST);
    si (involucrum == NIHIL && st->causa != NIHIL)
    { redde NIHIL;
    }
    si (   involucrum != NIHIL
        && !stml_liberum_addere(elementum, involucrum))
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
        && valor_nudus.mensura > ZEPHYRUM
        && stml_numerus_liberorum(elementum) == I
        && !_valorem_crudum_notare(st, elementum, tag, &valor_nudus))
    {
        redde NIHIL;
    }

    /* CUSTODIA LOCO RECTO (constrictum T9 nominatum): valor textui
     * non tutus sine cruditate emitti non potest - lector eum
     * praeteriret et circuitus tacite corrumperetur. Elementum
     * mixtum (trivia ferens) crudum esse nequit, ergo lexema
     * significans albi valoris CUM triviis irrepresentabile MANET -
     * sed nunc post conatum crudum custoditur, non ante eum. */
    si (   materia_lexicon_textum_fert(st->consilium->lexicon,
               lexema->genus)
        && valor_nudus.mensura > ZEPHYRUM
        && !_textus_tutus(&valor_nudus)
        && !elementum->crudus)
    {
        st->causa = "valor lexematis textui non tutus (mixtum)";
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
        fragmentum->fragmentum_id =
            chorda_internare_ex_literis(st->intern,
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

interior StmlNodus*
_scribere_nodum_internum (
    MateriaArborScriptor*,
    constans MateriaNodus*);

/* Species custodia est, non ornamentum: materia_nodus_appendere
 * speciem listae solam probat, NUMQUAM quid intus eat - ergo
 * elementa listae mixtae HIC custodienda sunt. */
interior b32
_scribere_valorem_in (
     MateriaArborScriptor* st,
                StmlNodus* parens,
             MateriaValor  valor,
      MateriaLocusSpecies  species,
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
        si (liberum == NIHIL)
        { redde FALSUM;
        }
        redde stml_liberum_addere(parens, liberum);

    casus MATERIA_LOCUS_TOKEN:
        si (valor.genus != MATERIA_VALOR_TOKEN)
        {
            st->causa = "locus TOKEN valorem alienum fert";
            st->sedes = sedes;
            redde FALSUM;
        }
        liberum = _scribere_lexema(st, valor.datum.token);
        si (liberum == NIHIL)
        { st->sedes = sedes; redde FALSUM;
        }
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
        redde stml_textum_addere(parens, st->piscina, st->intern,
            buffer);

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
                si (st->sedes == NIHIL)
                { st->sedes = sedes;
                }
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
_scribere_nodum_internum (
                           MateriaArborScriptor* st,
                          constans MateriaNodus* nodus)
{
    constans MateriaTabGenus* genus;
                   StmlNodus* elementum;
                         i32  i;

    si (nodus == NIHIL)
    { st->causa = "nodus nihil"; redde NIHIL;
    }
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
            par->clavis      = (constans vacuum*)nodus;
            par->est_lexema  = FALSUM;
            par->elementum   = elementum;
        }
    }

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaTabLocus* locus;
                       StmlNodus* involucrum;
                             i32  absolutus;

        /* Locus NIHIL = elementum OMISSUM (absentia canonica) */
        si (nodus->loci[i].genus == MATERIA_VALOR_NIHIL)
        { perge;
        }

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
    si (consilium == NIHIL)
    { redde;
    }
    /* MEMSET PRIMUM, deinde campi nominati. Campus consilio additus
     * sed constructori oblitus GARBAGE fert - et garbage in
     * 'lacunae' Xar* est, ergo xar_numerus in eo ruit. Id me
     * momordit 2026-08-27 (EXC_BAD_ACCESS in _positiones_lexematis).
     * Memset classem erroris tollit; enumeratio sola eam invitat. */
    memset(consilium, ZEPHYRUM, magnitudo(*consilium));
    consilium->tabularium  = tabularium;
    consilium->lexicon     = lexicon;
    consilium->grammatica  = grammatica;
}

MateriaArborScriptura
materia_arbor_scribere_nodum (
                           Piscina* piscina,
             constans MateriaNodus* nodus,
    constans MateriaArborConsilium* consilium)
{
    MateriaArborScriptura  fructus;
     MateriaArborScriptor  st;
                StmlNodus* involucrum;
                StmlNodus* radix;
                   chorda  sigillum;
      InternamentumChorda* intern;

    fructus.successus                 = FALSUM;
    fructus.textus.mensura            = ZEPHYRUM;
    fructus.textus.datum              = NIHIL;
    fructus.causa                     = NIHIL;
    fructus.sedes                     = NIHIL;
    fructus.sedes_valorum             = NIHIL;
    fructus.census.spatia_vocationes  = ZEPHYRUM;

    si (   piscina == NIHIL || nodus == NIHIL || consilium == NIHIL
        || consilium->tabularium == NIHIL
        || consilium->lexicon == NIHIL)
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

    st.piscina = piscina;
    st.intern = intern;
    st.consilium = consilium;
    st.numerus_notarum = ZEPHYRUM;
    st.ancora_nota = FALSUM;
    st.ancora_offset = (s32)-I;
    st.ancora_linea = ZEPHYRUM;
    st.ancora_columna = ZEPHYRUM;
    st.ancora_fons = ZEPHYRUM;
    st.ancora_initium_lineae = FALSUM;
    st.causa = NIHIL;
    st.sedes = NIHIL;
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

    fructus.textus     = stml_scribere(involucrum, piscina, VERUM);
    fructus.census     = st.census;
    fructus.successus  = VERUM;
    redde fructus;
}


/* ==================================================
 * LECTOR: STML canonicum -> arbor
 * ================================================== */

structura MateriaArborLector {
                       Piscina* piscina;
           InternamentumChorda* intern;
    constans MateriaArborConsilium* consilium;
            MateriaArborVitium* vitium;
                 TabulaDispersa* fragmenta;   /* id -> MateriaToken* */
                            s32  fons_ordinarius;
};

Piscina*
materia_arbor_lector_piscina (
    MateriaArborLector* lector)
{
    redde (lector == NIHIL) ? NIHIL : lector->piscina;
}

b32
materia_arbor_lector_recusare (
     MateriaArborLector* lector,
     constans character* causa,
                    i32  linea)
{
    si (   lector                != NIHIL && lector->vitium != NIHIL
        && lector->vitium->causa == NIHIL)
    {
        lector->vitium->causa = causa;
        lector->vitium->linea = linea;
    }
    redde FALSUM;
}

interior b32
_spatium_solum (
    constans chorda* valor)
{
    i32 i;

    si (valor == NIHIL)
    { redde VERUM;
    }
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        character c = (character)valor->datum[i];

        si (   c != ' ' && c != '\t' && c != '\n'
            && c != '\r' && c != '\f' && c != '\v')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Textus liberorum DIRECTORUM solum.
 *
 * NON stml_textus_internus: illud posteros OMNES concatenat, quod
 * super contentum MIXTUM tacite fallit - elementum lexematis quod
 * commentum in involucro 'ante' fert commentum IPSUM ante valorem
 * redderet, id est textum commenti in valorem lexematis absorptum.
 * De AMBITU est, non de transformatione. */
interior chorda
_textus_directus (
    MateriaArborLector* lector,
    constans StmlNodus* elementum)
{
    ChordaAedificator* aed;
                  i32  numerus;
                  i32  i;

    aed = chorda_aedificator_creare(lector->piscina, 64);
    si (aed == NIHIL)
    {
        chorda vacua;

        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    numerus = stml_numerus_liberorum((StmlNodus*)(size_t)elementum);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum = stml_liberum_ad_indicem(
            (StmlNodus*)(size_t)elementum, i);

        /* Textus spatii albi SOLIUS praeteritur NISI elementum
         * CRUDUM sit.
         *
         * CONTRACTUS UNUS PER DUAS PARTES, nunc recte enuntiatus:
         * pulcher DISPOSITIONEM possidet - EXTRA crudum. Intra
         * crudum octeti verbatim sunt, ergo spatium album ibi
         * CONTENTUM est, non dispositio, et ambiguitas quam haec
         * omissio vitabat simpliciter non exsistit.
         *
         * Prius omissio erat inconditionalis, et scriptor album
         * inconditionaliter refutabat - duae partes CONGRUEBANT sed
         * AMBAE nimis latae erant. Congruentia partium veritatem
         * non probat. */
        si (   liberum        != NIHIL
            && liberum->genus == STML_NODUS_TEXTUS
            && liberum->valor != NIHIL
            && (elementum->crudus || !_spatium_solum(liberum->valor)))
        {
            chorda_aedificator_appendere_chorda(aed, *liberum->valor);
        }
    }
    redde chorda_aedificator_finire(aed);
}

/* Inversum _cr_exuere: '\r' reinsertum ad offsets attributi 'cr'
 * (decimales ascendentes, in valore VERO). Attributum abest =
 * valor ut lectus. Pravum = recusatio, numquam silentium. */
interior chorda
_cr_induere (
    MateriaArborLector* lector,
             StmlNodus* elementum,
                chorda  nuda)
{
       chorda* attributum;
       chorda  plena;
           i8* datum;
          i32  numerus_cr;
          i32  i;
          b32  in_numero;

    attributum = stml_attributum_capere(elementum, "cr");
    si (attributum == NIHIL)
    {
        redde nuda;
    }

    numerus_cr  = ZEPHYRUM;
    in_numero   = FALSUM;
    per (i = ZEPHYRUM; i < attributum->mensura; i++)
    {
        b32 digitus = (b32)(   attributum->datum[i] >= (i8)'0'
                            && attributum->datum[i] <= (i8)'9');

        si (digitus && !in_numero)
        {
            numerus_cr = numerus_cr + I;
        }
        in_numero = digitus;
    }
    si (numerus_cr == ZEPHYRUM)
    {
        redde nuda;
    }

    plena.mensura = nuda.mensura + numerus_cr;
    datum = (i8*)piscina_allocare(lector->piscina,
        (memoriae_index)plena.mensura + I);
    si (datum == NIHIL)
    {
        materia_arbor_lector_recusare(lector,
            "memoria pro 'cr' deficit", elementum->linea);
        redde nuda;
    }

    {
        i32 attr_i  = ZEPHYRUM;
        i32 fons    = ZEPHYRUM;
        i32 exitus;
        s32 proximus = -I;

        per (exitus = ZEPHYRUM; exitus < plena.mensura; exitus++)
        {
            si (proximus < ZEPHYRUM)
            {
                dum (   attr_i < attributum->mensura
                     && (   attributum->datum[attr_i] < (i8)'0'
                         || attributum->datum[attr_i] > (i8)'9'))
                {
                    attr_i = attr_i + I;
                }
                si (attr_i < attributum->mensura)
                {
                    proximus = ZEPHYRUM;
                    dum (   attr_i < attributum->mensura
                         && attributum->datum[attr_i] >= (i8)'0'
                         && attributum->datum[attr_i] <= (i8)'9')
                    {
                        proximus = proximus * (s32)X
                                 + (s32)(attributum->datum[attr_i]
                                         - (i8)'0');
                        attr_i = attr_i + I;
                    }
                }
            }
            si (proximus >= ZEPHYRUM && (s32)exitus == proximus)
            {
                datum[exitus]  = (i8)'\r';
                proximus       = -I;
            }
            alioquin
            {
                si (fons >= nuda.mensura)
                {
                    materia_arbor_lector_recusare(lector,
                        "attributum 'cr' pravum", elementum->linea);
                    redde nuda;
                }
                datum[exitus]  = nuda.datum[fons];
                fons           = fons + I;
            }
        }
    }
    plena.datum = datum;
    redde plena;
}

b32
materia_arbor_numerus_ex_chorda (
    constans chorda* valor,
                i32* exitus)
{
    i32 fructus;
    i32 i;

    si (valor == NIHIL || valor->mensura == ZEPHYRUM)
    { redde FALSUM;
    }
    fructus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        character c = (character)valor->datum[i];

        si (c < '0' || c > '9')
        { redde FALSUM;
        }
        fructus = (fructus * X) + (i32)(c - '0');
    }
    *exitus = fructus;
    redde VERUM;
}

interior chorda
_chorda_repetita (
    MateriaArborLector* lector,
             character  c,
                   i32  numerus)
{
    chorda fructus;
       i32 i;

    fructus.mensura = numerus;
    fructus.datum   = (i8*)piscina_allocare(lector->piscina,
        (memoriae_index)(numerus > ZEPHYRUM ? numerus : I));
    si (fructus.datum == NIHIL)
    { fructus.mensura = ZEPHYRUM; redde fructus;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    { fructus.datum[i] = (i8)c;
    }
    redde fructus;
}

/* Variatio CRLF terminatoris: '\r' ante '\n' ULTIMUM inseritur.
 * Regula generalis, non casus per genus: '\n' -> '\r\n' et
 * '\\\n' -> '\\\r\n' eodem modo. */
interior chorda
_terminator_crlf (
    MateriaArborLector* lector,
    constans character* basis)
{
    chorda fructus;
       i32 n = (i32)strlen(basis);

    fructus.mensura = n + I;
    fructus.datum   = (i8*)piscina_allocare(lector->piscina,
        (memoriae_index)(n + I));
    si (fructus.datum == NIHIL)
    { fructus.mensura = ZEPHYRUM; redde fructus;
    }
    memcpy(fructus.datum, basis, (size_t)(n - I));
    fructus.datum[n - I]  = (i8)'\r';
    fructus.datum[n]      = (i8)'\n';
    redde fructus;
}

/* Elementum liberum proximum; textum spatii albi solius praeterit,
 * textum alium in sede structurali RECUSAT. */
interior StmlNodus*
_elementum_proximum (
      MateriaArborLector* lector,
               StmlNodus* parens,
                     i32* cursor)
{
    i32 numerus = stml_numerus_liberorum(parens);

    dum (*cursor < numerus)
    {
        StmlNodus* liberum = stml_liberum_ad_indicem(parens, *cursor);

        (*cursor)++;
        si (liberum == NIHIL)
        { perge;
        }
        si (liberum->genus == STML_NODUS_TEXTUS)
        {
            si (_spatium_solum(liberum->valor))
            { perge;
            }
            materia_arbor_lector_recusare(lector,
                "textus in sede structurali", liberum->linea);
            redde NIHIL;
        }
        si (liberum->genus == STML_NODUS_COMMENTUM)
        { perge;
        }
        redde liberum;
    }
    redde NIHIL;
}


/* ==================================================
 * Trivia - lentes speciei INVERTUNTUR
 * ================================================== */

interior MateriaToken*
_trivium_legere (
    MateriaArborLector* lector,
             StmlNodus* elementum)
{
    constans MateriaLexiconRatum* lex = lector->consilium->lexicon;
                             s32  genus;
                          chorda  valor;
                          chorda* attributum;
                             i32  numerus;
              constans character* orthographia;

    si (elementum->titulus == NIHIL)
    {
        materia_arbor_lector_recusare(lector, "trivium sine titulo",
            elementum->linea);
        redde NIHIL;
    }
    genus = materia_arbor_lexema_ex_tag(lex,
        (constans character*)elementum->titulus->datum,
        elementum->titulus->mensura);
    si (genus == (s32)lex->lexicon->numerus_generum)
    {
        materia_arbor_lector_recusare(lector,
            "genus trivii lexico ignotum", elementum->linea);
        redde NIHIL;
    }

    valor.mensura  = ZEPHYRUM;
    valor.datum    = NIHIL;
    orthographia   = materia_lexicon_orthographia(lex, genus);

    commutatio (materia_lexicon_species(lex, genus))
    {
    casus MATERIA_LEX_REPETITUM:
        attributum = stml_attributum_capere(elementum, "n");
        si (!materia_arbor_numerus_ex_chorda(attributum, &numerus))
        {
            materia_arbor_lector_recusare(lector,
                "trivium sine numero 'n'", elementum->linea);
            redde NIHIL;
        }
        valor = _chorda_repetita(lector, orthographia[0], numerus);
        frange;

    casus MATERIA_LEX_TERMINATOR:
        valor = stml_attributum_habet(elementum, "crlf")
            ? _terminator_crlf(lector, orthographia)
            : chorda_ex_literis(orthographia, lector->piscina);
        frange;

    casus MATERIA_LEX_FIXUM:
        valor = chorda_ex_literis(orthographia, lector->piscina);
        frange;

    ordinarius:
        valor = _cr_induere(lector, elementum,
            _textus_directus(lector, elementum));
        frange;
    }

    redde materia_token_creare(lector->piscina,
        &lector->consilium->forma, genus, valor,
        (s32)-I, ZEPHYRUM, ZEPHYRUM, lector->fons_ordinarius);
}

/* Involucrum <ante>/<post> -> series EXACTA (NIHIL si vacuum) */
interior b32
_trivia_legere (
          MateriaArborLector*   lector,
                   StmlNodus*   involucrum,
                MateriaToken*** exitus,
                         i32*   numerus_exitus)
{
    Xar* series;
    i32  cursor;

    series = xar_creare(lector->piscina, magnitudo(MateriaToken*));
    si (series == NIHIL)
    {
        redde materia_arbor_lector_recusare(lector,
            "series triviorum creari non potuit", involucrum->linea);
    }
    cursor = ZEPHYRUM;
    per (;;)
    {
        StmlNodus* liberum = _elementum_proximum(lector, involucrum,
            &cursor);
        MateriaToken* trivium;

        si (liberum == NIHIL)
        {
            si (   lector->vitium        != NIHIL
                && lector->vitium->causa != NIHIL)
            {
                redde FALSUM;
            }
            frange;
        }
        trivium = _trivium_legere(lector, liberum);
        si (trivium == NIHIL)
        { redde FALSUM;
        }
        {
            MateriaToken** cella = (MateriaToken**)xar_addere(series);

            si (cella == NIHIL)
            {
                redde materia_arbor_lector_recusare(lector,
                    "trivium addi non potuit", liberum->linea);
            }
            *cella = trivium;
        }
    }

    /* Series EXACTA, non Xar - vide materia_token.h (RP XV) */
    *numerus_exitus = xar_numerus(series);
    si (*numerus_exitus == ZEPHYRUM)
    { *exitus = NIHIL; redde VERUM;
    }
    {
        MateriaToken** exacta =
            (MateriaToken**)piscina_allocare_ordinatum(
            lector->piscina,
            (memoriae_index)*numerus_exitus * magnitudo(MateriaToken*),
            magnitudo(vacuum*));
        i32 i;

        si (exacta == NIHIL)
        {
            redde materia_arbor_lector_recusare(lector,
                "series exacta creari non potuit", involucrum->linea);
        }
        per (i = ZEPHYRUM; i < *numerus_exitus; i++)
        {
            exacta[i] = *(MateriaToken**)xar_obtinere(series, i);
        }
        *exitus = exacta;
    }
    redde VERUM;
}

/* TRIVIA FONTEM DOMINI SEQUUNTUR. Trivium in eadem plagula iacet ac
 * lexema cui adhaeret - DERIVABILE, ergo non portandum. Sed lector
 * ea cum 'fons_ordinarius' creat, quod fontem PRINCIPEM significat;
 * ergo trivium plagulae alienae tacite in principem mutaretur.
 * Silva id mensuravit: VII plagulae latinae hinc divergebant, omnes
 * ad NOVA_LINEA in regione degradata. CORPUS PLANUM ID CAPERE NON
 * POTUIT - uno fonte, ordinarius IPSE fons est. */
interior vacuum
_trivia_fontem_ponere (
    MateriaToken** series,
             i32   numerus,
             s32   fons)
{
    i32 i;

    si (series == NIHIL || fons < ZEPHYRUM)
    { redde;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (series[i] != NIHIL)
        { series[i]->fons_index = fons;
        }
    }
}


/* ==================================================
 * Lexema
 * ================================================== */

interior MateriaNodus*
_nodum_legere (
    MateriaArborLector*,
    StmlNodus*);

interior b32
_est_tag_lexematis (
    MateriaArborLector* lector,
       constans chorda* titulus)
{
    constans character* praefixum =
        lector->consilium->lexicon->lexicon->praefixum_tagi;
    i32 longitudo = (i32)strlen(praefixum);

    si (titulus == NIHIL || titulus->mensura < longitudo)
    {
        redde FALSUM;
    }
    redde (b32)(memcmp(titulus->datum, praefixum,
                    (size_t)longitudo) == ZEPHYRUM);
}

interior MateriaToken*
_lexema_legere (
    MateriaArborLector* lector,
             StmlNodus* elementum,
                chorda* fragmenti_id)
{
    constans MateriaLexiconRatum* lex    = lector->consilium->lexicon;
      constans MateriaArborFrons* frons  = lector->consilium->frons;
                             s32  genus;
                    MateriaToken* lexema;
                          chorda  valor;
                          chorda* attributum;
                             i32  cursor;
                             i32  numerus;
                             b32  valor_visus;

    /* TRANSCLUSIO: lexema IDEM reddere, non par. Identitas res est -
     * bracchia ambigua obiecta EADEM ferunt. */
    si (elementum->genus == STML_NODUS_TRANSCLUSIO)
    {
        chorda  clavis;
        vacuum* inventum;

        si (elementum->valor == NIHIL || elementum->valor->mensura < II)
        {
            materia_arbor_lector_recusare(lector,
                "transclusio sine identitate", elementum->linea);
            redde NIHIL;
        }
        clavis.mensura  = elementum->valor->mensura - I;
        clavis.datum    = elementum->valor->datum + I;
        si (!tabula_dispersa_invenire(lector->fragmenta, clavis,
            &inventum))
        {
            /* Fragmentum ANTE usum definiendum est. */
            materia_arbor_lector_recusare(lector,
                "transclusio ad fragmentum ignotum", elementum->linea);
            redde NIHIL;
        }
        redde (MateriaToken*)inventum;
    }
    si (elementum->titulus == NIHIL)
    {
        materia_arbor_lector_recusare(lector, "lexema sine titulo",
            elementum->linea);
        redde NIHIL;
    }

    genus = materia_arbor_lexema_ex_tag(lex,
        (constans character*)elementum->titulus->datum,
        elementum->titulus->mensura);
    si (genus == (s32)lex->lexicon->numerus_generum)
    {
        materia_arbor_lector_recusare(lector,
            "genus lexematis lexico ignotum", elementum->linea);
        redde NIHIL;
    }

    /* VALOR adesse debet si et solum si species VERBATIM est */
    valor       = _cr_induere(lector, elementum,
        _textus_directus(lector, elementum));
    valor_visus = (b32)(valor.mensura > ZEPHYRUM);

    si (valor_visus && !materia_lexicon_textum_fert(lex, genus))
    {
        materia_arbor_lector_recusare(lector,
            "valor in genere orthographiae fixae", elementum->linea);
        redde NIHIL;
    }
    si (!materia_lexicon_textum_fert(lex, genus))
    {
        constans character* orthographia =
            materia_lexicon_orthographia(lex, genus);

        si (orthographia != NIHIL)
        {
            /* TERMINATOR in loco: 'crlf' ut in triviis (speculum
             * scriptoris loci, 2026-09-03) */
            valor = (materia_lexicon_species(lex, genus)
                == MATERIA_LEX_TERMINATOR
                     && stml_attributum_habet(elementum, "crlf"))
                  ? _terminator_crlf(lector, orthographia)
                  : chorda_ex_literis(orthographia, lector->piscina);
        }
    }

    lexema = materia_token_creare(lector->piscina,
        &lector->consilium->forma, genus, valor,
        (s32)-I, ZEPHYRUM, ZEPHYRUM, lector->fons_ordinarius);
    si (lexema == NIHIL)
    {
        materia_arbor_lector_recusare(lector,
            "lexema creari non potuit",
            elementum->linea);
        redde NIHIL;
    }

    /* Attributa frontis PRIMA (specula scripturae) */
    si (frons != NIHIL && frons->attributa_legere != NIHIL)
    {
        si (!frons->attributa_legere(frons->datum, lector, elementum,
                 lexema))
        {
            redde NIHIL;
        }
    }

    attributum = stml_attributum_capere(elementum, "f");
    si (attributum != NIHIL)
    {
        i32 fons;

        si (!materia_arbor_numerus_ex_chorda(attributum, &fons))
        {
            materia_arbor_lector_recusare(lector, "fons non numerus",
                elementum->linea);
            redde NIHIL;
        }
        lexema->fons_index = (s32)fons;
    }

    /* SEDES PORTATA (lexema derivatum solum - vide scriptorem) */
    attributum = stml_attributum_capere(elementum, "b");
    si (attributum != NIHIL)
    {
        i32 sedes_portata;

        si (!materia_arbor_numerus_ex_chorda(attributum,
            &sedes_portata))
        {
            materia_arbor_lector_recusare(lector, "b non numerus",
                elementum->linea);
            redde NIHIL;
        }
        lexema->byte_offset = (s32)sedes_portata;
    }
    attributum = stml_attributum_capere(elementum, "linea");
    si (attributum != NIHIL)
    {
        i32 n;

        si (!materia_arbor_numerus_ex_chorda(attributum, &n))
        {
            materia_arbor_lector_recusare(lector, "linea non numerus",
                elementum->linea);
            redde NIHIL;
        }
        lexema->linea = n;
    }
    attributum = stml_attributum_capere(elementum, "columna");
    si (attributum != NIHIL)
    {
        i32 n;

        si (!materia_arbor_numerus_ex_chorda(attributum, &n))
        {
            materia_arbor_lector_recusare(lector, "columna non numerus",
                elementum->linea);
            redde NIHIL;
        }
        lexema->columna = n;
    }
    si (stml_attributum_habet(elementum, "linea-initium"))
    {
        materia_token_initium_lineae_ponere(lexema, VERUM);
    }

    /* Liberi: <ante>, <post>, cetera ad frontem */
    cursor   = ZEPHYRUM;
    numerus  = stml_numerus_liberorum(elementum);
    per (;;)
    {
        StmlNodus* liberum;

        si (cursor >= numerus)
        { frange;
        }
        liberum = stml_liberum_ad_indicem(elementum, cursor);
        cursor++;
        si (   liberum          == NIHIL
            || liberum->genus   != STML_NODUS_ELEMENTUM
            || liberum->titulus == NIHIL)
        {
            perge;
        }

        si (chorda_aequalis_literis(*liberum->titulus,
                MATERIA_ARBOR_TAG_ANTE))
        {
            si (!_trivia_legere(lector, liberum, &lexema->spatia_ante,
                     &lexema->numerus_ante))
            {
                redde NIHIL;
            }
        }
        alioquin si (chorda_aequalis_literis(*liberum->titulus,
                         MATERIA_ARBOR_TAG_POST))
        {
            si (!_trivia_legere(lector, liberum, &lexema->spatia_post,
                     &lexema->numerus_post))
            {
                redde NIHIL;
            }
        }
        alioquin
        {
            s32 fructus = (s32)MATERIA_LECTIO_IGNOTUM;

            si (frons != NIHIL && frons->liberum_legere != NIHIL)
            {
                fructus = frons->liberum_legere(frons->datum, lector,
                    liberum, lexema);
            }
            si (fructus == (s32)MATERIA_LECTIO_FRACTUM)
            {
                /* Frons causam suam iam posuit; ne eam obruamus. */
                materia_arbor_lector_recusare(lector,
                    "frons liberum lexematis recusavit",
                    liberum->linea);
                redde NIHIL;
            }
            si (fructus == (s32)MATERIA_LECTIO_IGNOTUM)
            {
                materia_arbor_lector_recusare(lector,
                    "elementum in lexemate ignotum", liberum->linea);
                redde NIHIL;
            }
        }
    }

    _trivia_fontem_ponere(lexema->spatia_ante, lexema->numerus_ante,
        lexema->fons_index);
    _trivia_fontem_ponere(lexema->spatia_post, lexema->numerus_post,
        lexema->fons_index);

    /* Fragmentum: lexema sub ID suo deponere, ut transclusiones
     * sequentes HOC OBIECTUM inveniant */
    si (fragmenti_id != NIHIL)
    {
        tabula_dispersa_inserere(lector->fragmenta, *fragmenti_id,
            lexema);
    }
    redde lexema;
}

/* Fragmentum involucrum est: contentum eius reddere, ID vocanti
 * tradere (transclusiones OBIECTUM IDEM invenire debent). */
interior StmlNodus*
_fragmentum_aperire (
         MateriaArborLector*  lector,
                  StmlNodus*  elementum,
                     chorda** id_exitus)
{
    i32 cursor;

    si (id_exitus != NIHIL)
    { *id_exitus = NIHIL;
    }
    si (elementum == NIHIL || !elementum->fragmentum)
    { redde elementum;
    }
    si (id_exitus != NIHIL)
    { *id_exitus = elementum->fragmentum_id;
    }
    cursor = ZEPHYRUM;
    redde _elementum_proximum(lector, elementum, &cursor);
}


/* ==================================================
 * Valor et nodus
 * ================================================== */

/* Lector elementa IPSE custodire debet: materia_nodus_appendere
 * speciem listae solam probat, numquam quid intus eat. */
interior b32
_valorem_loci_legere (
     MateriaArborLector* lector,
              StmlNodus* involucrum,
    MateriaLocusSpecies  species,
           MateriaNodus* nodus,
                    i32  locus)
{
     StmlNodus* liberum;
  MateriaValor  valor;
        chorda* fragmenti_id;
           i32  cursor;
           i32  numerus;

    fragmenti_id  = NIHIL;
    cursor        = ZEPHYRUM;

    si (species == MATERIA_LOCUS_INDEX)
    {
        chorda textus = _textus_directus(lector, involucrum);

        si (!materia_arbor_numerus_ex_chorda(&textus, &numerus))
        {
            redde materia_arbor_lector_recusare(lector,
                "locus INDEX numerum non fert", involucrum->linea);
        }
        si (!materia_nodus_ponere(nodus, locus,
                 materia_valor_index((s32)numerus), species))
        {
            redde materia_arbor_lector_recusare(lector,
                "index poni non potuit", involucrum->linea);
        }
        redde VERUM;
    }

    si (   species == MATERIA_LOCUS_NODUS
        || species == MATERIA_LOCUS_TOKEN)
    {
        liberum = _elementum_proximum(lector, involucrum, &cursor);
        si (liberum == NIHIL)
        {
            redde materia_arbor_lector_recusare(lector, "locus vacuus",
                involucrum->linea);
        }
        liberum = _fragmentum_aperire(lector, liberum, &fragmenti_id);
        si (liberum == NIHIL)
        { redde FALSUM;
        }

        si (species == MATERIA_LOCUS_NODUS)
        {
            MateriaNodus* filius;

            si (_est_tag_lexematis(lector, liberum->titulus))
            {
                redde materia_arbor_lector_recusare(lector,
                    "lexema in loco NODUS", liberum->linea);
            }
            filius = _nodum_legere(lector, liberum);
            si (filius == NIHIL)
            { redde FALSUM;
            }
            valor = materia_valor_nodus(filius);
        }
        alioquin
        {
            MateriaToken* lexema;

            si (   liberum->genus != STML_NODUS_TRANSCLUSIO
                && !_est_tag_lexematis(lector, liberum->titulus))
            {
                redde materia_arbor_lector_recusare(lector,
                    "nodus in loco TOKEN", liberum->linea);
            }
            lexema = _lexema_legere(lector, liberum, fragmenti_id);
            si (lexema == NIHIL)
            { redde FALSUM;
            }
            valor = materia_valor_token(lexema);
        }

        /* Plus quam unum elementum in loco singulari = forma corrupta */
        si (_elementum_proximum(lector, involucrum, &cursor) != NIHIL)
        {
            redde materia_arbor_lector_recusare(lector,
                "locus singularis plura fert", involucrum->linea);
        }
        si (lector->vitium != NIHIL && lector->vitium->causa != NIHIL)
        {
            redde FALSUM;
        }
        si (!materia_nodus_ponere(nodus, locus, valor, species))
        {
            redde materia_arbor_lector_recusare(lector,
                "valor loci poni non potuit", involucrum->linea);
        }
        redde VERUM;
    }

    valor = materia_valor_lista_nova(lector->piscina);
    per (;;)
    {
        b32 est_lexema;

        liberum = _elementum_proximum(lector, involucrum, &cursor);
        si (liberum == NIHIL)
        {
            si (   lector->vitium        != NIHIL
                && lector->vitium->causa != NIHIL)
            {
                redde FALSUM;
            }
            frange;
        }
        liberum = _fragmentum_aperire(lector, liberum, &fragmenti_id);
        si (liberum == NIHIL)
        { redde FALSUM;
        }

        est_lexema = (b32)(liberum->genus == STML_NODUS_TRANSCLUSIO
                        || _est_tag_lexematis(lector,
                        liberum->titulus));

        si (est_lexema)
        {
            MateriaToken* lexema;

            si (species == MATERIA_LOCUS_LISTA_NODUS)
            {
                redde materia_arbor_lector_recusare(lector,
                    "lexema in lista NODUS", liberum->linea);
            }
            lexema = _lexema_legere(lector, liberum, fragmenti_id);
            si (lexema == NIHIL)
            { redde FALSUM;
            }
            valor = materia_valor_lista_appendere(lector->piscina,
                valor,
                materia_valor_token(lexema));
        }
        alioquin
        {
            MateriaNodus* filius;

            si (species == MATERIA_LOCUS_LISTA_TOKEN)
            {
                redde materia_arbor_lector_recusare(lector,
                    "nodus in lista TOKEN", liberum->linea);
            }
            filius = _nodum_legere(lector, liberum);
            si (filius == NIHIL)
            { redde FALSUM;
            }
            valor = materia_valor_lista_appendere(lector->piscina,
                valor,
                materia_valor_nodus(filius));
        }
    }

    si (!materia_nodus_ponere(nodus, locus, valor, species))
    {
        redde materia_arbor_lector_recusare(lector,
            "lista poni non potuit", involucrum->linea);
    }
    redde VERUM;
}

interior s32
_locus_index (
    constans MateriaRegistrumCoctum* tab,
                                s32  genus,
                 constans character* titulus,
                                i32  mensura)
{
    constans MateriaTabGenus* g;
                         i32  k;

    si (   tab        == NIHIL || genus < ZEPHYRUM
        || (i32)genus >= tab->numerus_generum)
    {
        redde (s32)-I;
    }
    g = &tab->genera[genus];
    per (k = ZEPHYRUM; k < g->loci_numerus; k++)
    {
        constans character* t = tab->loci[g->loci_offset + k].titulus;

        si (   t != NIHIL && (i32)strlen(t) == mensura
            && memcmp(t, titulus, (size_t)mensura) == ZEPHYRUM)
        {
            redde (s32)(g->loci_offset + k);
        }
    }
    redde (s32)-I;
}

interior MateriaNodus*
_nodum_legere (
    MateriaArborLector* lector,
             StmlNodus* elementum)
{
    constans MateriaRegistrumCoctum* tab =
        lector->consilium->tabularium;
           constans MateriaTabGenus* genus;
                       MateriaNodus* nodus;
                                s32  genus_index;
                                i32  cursor;

    si (elementum == NIHIL || elementum->titulus == NIHIL)
    {
        materia_arbor_lector_recusare(lector, "nodus sine titulo",
            elementum ? elementum->linea : ZEPHYRUM);
        redde NIHIL;
    }
    genus_index = materia_arbor_genus_index(tab,
        (constans character*)elementum->titulus->datum,
        elementum->titulus->mensura);
    si (genus_index < ZEPHYRUM)
    {
        materia_arbor_lector_recusare(lector, "genus registro ignotum",
            elementum->linea);
        redde NIHIL;
    }
    genus = &tab->genera[genus_index];

    /* Semita CUSTODITA: species probata, semel-tantum scribere */
    nodus = materia_nodus_creare(lector->piscina, genus_index,
        genus->loci_numerus);
    si (nodus == NIHIL)
    {
        materia_arbor_lector_recusare(lector, "nodus creari non potuit",
            elementum->linea);
        redde NIHIL;
    }

    cursor = ZEPHYRUM;
    per (;;)
    {
        StmlNodus* involucrum;
              s32  absolutus;
              i32  relativus;

        involucrum = _elementum_proximum(lector, elementum, &cursor);
        si (involucrum == NIHIL)
        {
            si (   lector->vitium        != NIHIL
                && lector->vitium->causa != NIHIL)
            {
                redde NIHIL;
            }
            frange;
        }
        si (involucrum->titulus == NIHIL)
        {
            materia_arbor_lector_recusare(lector,
                "involucrum loci sine titulo", involucrum->linea);
            redde NIHIL;
        }
        absolutus = _locus_index(tab, genus_index,
            (constans character*)involucrum->titulus->datum,
            involucrum->titulus->mensura);
        si (absolutus < ZEPHYRUM)
        {
            materia_arbor_lector_recusare(lector,
                "locus generi ignotus",
                involucrum->linea);
            redde NIHIL;
        }
        relativus = (i32)absolutus - genus->loci_offset;

        si (!_valorem_loci_legere(lector, involucrum,
                 (MateriaLocusSpecies)tab->loci[absolutus].species,
                 nodus, relativus))
        {
            redde NIHIL;
        }
    }
    redde nodus;
}


/* ==================================================
 * Fixurae: positiones ex ancora derivare
 *
 * Documentum positiones NON fert; involucrum ancoram solam. Hic
 * ambulatione EMISSIONIS (eadem quam scriptor sequitur) sedes
 * reficimus. Lex: documentum canonicum mentiri non possit -
 * positio PORTATA cum contento dissentire potest, DERIVATA non.
 * ================================================== */

nomen structura {
                MateriaArborCursor  cursor;
    constans MateriaArborConsilium* consilium;
                               Xar* lacunae;
                               i32  lacuna_proxima;
} ArborFixura;

interior vacuum
_cursorem_promovere (
    MateriaArborCursor* c,
       constans chorda* octeti)
{
    i32 i;

    si (octeti == NIHIL)
    { redde;
    }
    per (i = ZEPHYRUM; i < octeti->mensura; i++)
    {
        si ((character)octeti->datum[i] == '\n')
        {
            c->linea++;
            c->columna = I;
        }
        alioquin
        {
            c->columna++;
        }
    }
    c->offset += (s32)octeti->mensura;
}

interior vacuum
_trivium_ponere (
     ArborFixura* f,
    MateriaToken* trivium)
{
    si (trivium == NIHIL)
    { redde;
    }
    si (f->cursor.sedes_notae)
    {
        trivium->byte_offset  = f->cursor.offset;
        trivium->linea        = f->cursor.linea;
        trivium->columna      = f->cursor.columna;
    }
    _cursorem_promovere(&f->cursor, &trivium->valor);

    /* Linea LOGICA: munus LINEA lineam novam incipit, LAMINA NON
     * (linea physica crescit, logica perstat). Haec est ipsa
     * distinctio propter quam munus LAMINA exsistit. */
    si (materia_lexicon_munus(f->consilium->lexicon, trivium->genus)
        == MATERIA_MUNUS_LINEA)
    {
        f->cursor.post_lineam = VERUM;
    }
}

interior vacuum
_lexema_ponere (
     ArborFixura* f,
    MateriaToken* lexema)
{
    constans MateriaArborFrons* frons = f->consilium->frons;

    /* initium_lineae ex TRIVIIS derivatur, non ex ancora - ergo arbor
     * AUCTORATA (sine textu fontis) eam tamen recte accipit. Ancora
     * ORDINATAS solas regit. */
    materia_token_initium_lineae_ponere(lexema, f->cursor.post_lineam);
    si (f->cursor.sedes_notae)
    {
        lexema->byte_offset  = f->cursor.offset;
        lexema->linea        = f->cursor.linea;
        lexema->columna      = f->cursor.columna;
    }

    /* Frons cursorem movere potest (C89: scissurae laminas
     * reinserunt, ergo octeti plures quam valor). */
    si (   frons == NIHIL || frons->cursorem_movere == NIHIL
        || !frons->cursorem_movere(frons->datum, &f->cursor, lexema))
    {
        _cursorem_promovere(&f->cursor, &lexema->valor);
    }
    f->cursor.post_lineam = FALSUM;
}

interior vacuum
_positiones_valoris (
    ArborFixura*,
    MateriaValor);

interior vacuum
_positiones_lexematis (
     ArborFixura* f,
    MateriaToken* lexema)
{
    constans MateriaOrigoUncus* uncus;
                           i32  i;

    si (lexema == NIHIL)
    { redde;
    }
    uncus = f->consilium->origo;

    /* LEXEMA DERIVATUM OCTETOS NON TENET. In fluxu octetorum radix
     * emissionis stat, ergo derivatio idem facere DEBET: radicem
     * ponere, derivatum praeterire. Aliter cursor BIS promovetur -
     * semel derivato, semel radice - et sedes omnes post primam
     * expansionem labuntur. */
    si (uncus != NIHIL && uncus->radix_quaerere != NIHIL)
    {
        constans character* causa = NIHIL;
              MateriaToken* radix = uncus->radix_quaerere(uncus->datum,
                  lexema, &causa);

        si (radix != NIHIL && radix != lexema)
        {
            Xar* lamina = (uncus->extentum_quaerere != NIHIL)
                ? uncus->extentum_quaerere(uncus->datum, radix) : NIHIL;

            /* EXTENTUM: radix functio-similis octetos [nomen..')']
             * tegit, non nomen solum. Emissor totam laminam scribit;
             * derivatio quae nomen solum promovet omnes sedes
             * sequentes labi facit. */
            si (lamina != NIHIL && xar_numerus(lamina) > I)
            {
                i32 k;

                per (k = ZEPHYRUM; k < xar_numerus(lamina); k++)
                {
                    _positiones_lexematis(f,
                        *(MateriaToken**)xar_obtinere(lamina, k));
                }
            }
            alioquin
            {
                _positiones_lexematis(f, radix);
            }
            redde;
        }
    }

    /* IAM POSITUM: lexema communicatum eosdem octetos tegit, ergo
     * cursor bis promoveri NON debet. */
    si (f->cursor.sedes_notae && lexema->byte_offset >= ZEPHYRUM)
    {
        redde;
    }

    /* LACUNAS transilire ad quas cursor pervenit. Omnis derivatio
     * per hanc functionem fluit - punctum mutationis unum. */
    si (f->lacunae != NIHIL)
    {
        /* Scansio LOCALIS: 'i' semper crescit, sed 'lacuna_proxima'
         * SOLUM committitur cum lacuna vere applicata aut plane
         * praeterita est. Lacuna ALIENI FONTIS praetermittitur SINE
         * commissione - alioquin lexema sequens eiusdem plagulae eam
         * amitteret. */
        i = f->lacuna_proxima;
        dum (i < xar_numerus(f->lacunae))
        {
            MateriaLacuna* lacuna = (MateriaLacuna*)xar_obtinere(
                f->lacunae, i);

            si (lacuna == NIHIL || lacuna->finis <= f->cursor.offset)
            {
                i++;
                f->lacuna_proxima = i;
                perge;
            }
            si (lacuna->offset > f->cursor.offset)
            { frange;
            }
            /* FONS CONGRUAT - offset sine fonte sensu caret */
            si (   lacuna->fons       >= ZEPHYRUM
                && lexema->fons_index >= ZEPHYRUM
                && lacuna->fons       != lexema->fons_index)
            {
                i++;
                perge;
            }
            f->cursor.offset       = lacuna->finis;
            f->cursor.linea        = lacuna->linea_finalis;
            f->cursor.columna      = lacuna->columna_finalis;
            f->cursor.post_lineam  = lacuna->post_lineam_finalis;
            i++;
            f->lacuna_proxima = i;
        }
    }

    per (i = ZEPHYRUM; i < lexema->numerus_ante; i++)
    {
        _trivium_ponere(f, lexema->spatia_ante[i]);
    }
    _lexema_ponere(f, lexema);
    per (i = ZEPHYRUM; i < lexema->numerus_post; i++)
    {
        _trivium_ponere(f, lexema->spatia_post[i]);
    }
}

interior vacuum
_positiones_nodi (
     ArborFixura* f,
    MateriaNodus* nodus)
{
    i32 i;

    si (nodus == NIHIL)
    { redde;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        _positiones_valoris(f, nodus->loci[i]);
    }
}

interior vacuum
_positiones_valoris (
     ArborFixura* f,
    MateriaValor  valor)
{
    i32 numerus;
    i32 i;

    commutatio (valor.genus)
    {
    casus MATERIA_VALOR_NODUS:
        _positiones_nodi(f, valor.datum.nodus); frange;
    casus MATERIA_VALOR_TOKEN:
        _positiones_lexematis(f, valor.datum.token); frange;
    casus MATERIA_VALOR_LISTA:
        numerus = materia_valor_lista_numerus(valor);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            MateriaValor* e = materia_valor_lista_obtinere(valor, i);

            si (e != NIHIL)
            { _positiones_valoris(f, *e);
            }
        }
        frange;
    ordinarius: frange;
    }
}

/* PATRES figere. Silva id commissioni relinquit; materia
 * commissionem nondum habet, et ambulatio parentum GENERALIS est -
 * ergo hic, et frons per uncum 'perficere' plus agat si vult. */
interior vacuum
_patres_figere (
         Piscina* piscina,
    MateriaNodus* nodus)
{
    Xar* liberi;
    i32  i;

    si (nodus == NIHIL)
    { redde;
    }
    liberi = materia_nodus_liberi(piscina, nodus);
    si (liberi == NIHIL)
    { redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(liberi); i++)
    {
        MateriaNodus* filius = *(MateriaNodus**)xar_obtinere(liberi, i);

        si (filius != NIHIL)
        {
            filius->pater = nodus;
            _patres_figere(piscina, filius);
        }
    }
}

/* Publica: parsatores linguarum (descensu recursivo, sine
 * commissione) eam post constructionem vocant, ut arbores eorum
 * politicae reconstructionis materiae congruant - lector patres
 * OMNES ponit, ergo arbor parsata sine patribus comparatori (B1)
 * falso divergeret. Divergentia politicae ad phasim V nominata
 * (bracchia ambigui; vide phase-log B1). */
vacuum
materia_arbor_patres_figere (
         Piscina* piscina,
    MateriaNodus* radix)
{
    _patres_figere(piscina, radix);
}


/* ==================================================
 * Ingressus lectionis
 * ================================================== */

MateriaNodus*
materia_arbor_legere (
                           Piscina* piscina,
               InternamentumChorda* intern,
                            chorda  textus,
    constans MateriaArborConsilium* consilium,
                MateriaArborVitium* vitium)
{
     MateriaArborLector  lector;
           StmlResultus  resultus;
              StmlNodus* involucrum;
              StmlNodus* radix;
           MateriaNodus* arbor;
            ArborFixura  fixura;
                 chorda* attributum;
                 chorda  sigillum;
                    i32  cursor;
                    i32  numerus;
                    b32  ancora_adest;

    si (vitium != NIHIL)
    { vitium->causa = NIHIL; vitium->linea = ZEPHYRUM;
    }

    lector.piscina          = piscina;
    lector.intern           = intern;
    lector.consilium        = consilium;
    lector.vitium           = vitium;
    lector.fragmenta        = NIHIL;
    lector.fons_ordinarius  = ZEPHYRUM;

    si (   piscina               == NIHIL || consilium == NIHIL
        || consilium->tabularium == NIHIL || consilium->lexicon == NIHIL
        || consilium->grammatica == NIHIL)
    {
        materia_arbor_lector_recusare(&lector, "argumenta nihil",
            ZEPHYRUM);
        redde NIHIL;
    }
    si (!consilium->lexicon->ratum)
    {
        materia_arbor_lector_recusare(&lector,
            "lexicon non ratum (porta oneris non transita)", ZEPHYRUM);
        redde NIHIL;
    }
    si (intern == NIHIL)
    {
        intern = internamentum_creare(piscina);
        si (intern == NIHIL)
        {
            materia_arbor_lector_recusare(&lector,
                "internamentum creari non potuit", ZEPHYRUM);
            redde NIHIL;
        }
        lector.intern = intern;
    }

    resultus = stml_legere(textus, piscina, intern);
    si (!resultus.successus)
    {
        materia_arbor_lector_recusare(&lector,
            "STML parsari non potuit",
            resultus.linea_erroris);
        redde NIHIL;
    }
    involucrum = resultus.elementum_radix;
    si (   involucrum == NIHIL || involucrum->titulus == NIHIL
        || !chorda_aequalis_literis(*involucrum->titulus,
                MATERIA_ARBOR_TAG_ENVOLUCRI))
    {
        materia_arbor_lector_recusare(&lector,
            "involucrum <arbor> deest",
            involucrum ? involucrum->linea : ZEPHYRUM);
        redde NIHIL;
    }

    attributum = stml_attributum_capere(involucrum, "grammatica");
    si (   attributum == NIHIL
        || !chorda_aequalis_literis(*attributum, consilium->grammatica))
    {
        materia_arbor_lector_recusare(&lector,
            "grammatica non congruit",
            involucrum->linea);
        redde NIHIL;
    }

    /* SIGILLUM: arbor vocabulario FALSO iudicata mendacium est */
    sigillum = materia_arbor_sigillum(piscina, consilium->tabularium);
    attributum = stml_attributum_capere(involucrum,
        "registrum-sigillum");
    si (   attributum == NIHIL || sigillum.mensura == ZEPHYRUM
        || !chorda_aequalis(*attributum, sigillum))
    {
        materia_arbor_lector_recusare(&lector,
            "sigillum registri non congruit", involucrum->linea);
        redde NIHIL;
    }

    /* ANCORA absens = arbor AUCTORATA (sine fonte): positiones -I
     * manent, quod LEGITIMUM est. */
    ancora_adest               = FALSUM;
    fixura.consilium           = consilium;
    fixura.lacunae             = consilium->lacunae;
    fixura.lacuna_proxima      = ZEPHYRUM;
    fixura.cursor.offset       = ZEPHYRUM;
    fixura.cursor.linea        = I;
    fixura.cursor.columna      = I;
    fixura.cursor.post_lineam  = VERUM;
    fixura.cursor.sedes_notae  = FALSUM;

    attributum = stml_attributum_capere(involucrum, "b");
    si (   attributum != NIHIL
        && materia_arbor_numerus_ex_chorda(attributum, &numerus))
    {
        fixura.cursor.offset  = (s32)numerus;
        ancora_adest          = VERUM;
        fixura.cursor.post_lineam = stml_attributum_habet(involucrum,
            "linea-initium");
    }
    attributum = stml_attributum_capere(involucrum, "linea");
    si (   attributum != NIHIL
        && materia_arbor_numerus_ex_chorda(attributum, &numerus))
    {
        fixura.cursor.linea = numerus;
    }
    attributum = stml_attributum_capere(involucrum, "columna");
    si (   attributum != NIHIL
        && materia_arbor_numerus_ex_chorda(attributum, &numerus))
    {
        fixura.cursor.columna = numerus;
    }
    attributum = stml_attributum_capere(involucrum, "fons");
    si (   attributum != NIHIL
        && materia_arbor_numerus_ex_chorda(attributum, &numerus))
    {
        lector.fons_ordinarius = (s32)numerus;
    }

    lector.fragmenta = tabula_dispersa_creare_chorda(piscina, 64);
    si (lector.fragmenta == NIHIL)
    {
        materia_arbor_lector_recusare(&lector,
            "tabula fragmentorum creari non potuit", involucrum->linea);
        redde NIHIL;
    }

    cursor  = ZEPHYRUM;
    radix   = _elementum_proximum(&lector, involucrum, &cursor);
    si (radix == NIHIL)
    {
        materia_arbor_lector_recusare(&lector,
            "involucrum arborem non fert", involucrum->linea);
        redde NIHIL;
    }
    radix = _fragmentum_aperire(&lector, radix, NIHIL);
    si (radix == NIHIL)
    { redde NIHIL;
    }

    arbor = _nodum_legere(&lector, radix);
    si (arbor == NIHIL)
    { redde NIHIL;
    }

    fixura.cursor.sedes_notae = ancora_adest;
    _positiones_nodi(&fixura, arbor);
    _patres_figere(piscina, arbor);

    si (   consilium->frons            != NIHIL
        && consilium->frons->perficere != NIHIL
        && !consilium->frons->perficere(consilium->frons->datum,
        &lector,
               arbor))
    {
        materia_arbor_lector_recusare(&lector,
            "frons arborem perficere non potuit", involucrum->linea);
        redde NIHIL;
    }
    redde arbor;
}
