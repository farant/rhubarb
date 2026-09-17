/* crusta_arithmetica.c - Machina praecedentiae et aestimator (vide .h)
 */

#include "crusta_arithmetica.h"
#include <string.h>

/* Signum in acervo operatorum: lexema + operator tabulae; ternaria
 * '?' signum secundum ':' cum advenit recipit. corpora = corpora
 * heredoc post lexema signi, corpora_coli post ':' (P9b; NIHIL =
 * nulla). */
nomen structura {
               MateriaToken* tok;
               MateriaToken* colon;   /* ternaria: ':' aut NIHIL */
    constans CrustaOperator* op;
                        Xar* corpora;
                        Xar* corpora_coli;
} Signum;

/* Operandum in acervo: nodus (NIHIL = absens) + corpora heredoc post
 * lexema eius ultimum (P9b) */
nomen structura {
    MateriaNodus* nodus;
             Xar* corpora;
} Operandum;

/* quid ultimum pulsum: corpus interpositum ei sequitur */
nomen enumeratio {
    ULTIMUM_NIHIL = 0,
    ULTIMUM_OPERANDUM,
    ULTIMUM_SIGNUM,
    ULTIMUM_COLON
} Ultimum;

interior i32
_loci_numerus (
    s32 genus)
{
    redde CRUSTA_REGISTRUM.genera[genus].loci_numerus;
}

constans CrustaOperator*
crusta_arithmetica_quaerere (
    constans CrustaDialectus* dialectus,
                      chorda  titulus,
       CrustaOperatorSpecies  species)
{
    s32 i;

    per (i = ZEPHYRUM; dialectus->arithmetici[i].titulus != NIHIL; i++)
    {
        constans CrustaOperator* o = &dialectus->arithmetici[i];

        si (   o->species              == species
            && (i32)strlen(o->titulus) == titulus.mensura
            && memcmp(o->titulus, titulus.datum,
            (size_t)titulus.mensura)
                == ZEPHYRUM)
        {
            redde o;
        }
    }
    redde NIHIL;
}

vacuum
crusta_arithmetica_incipere (
           CrustaArithmetica* machina,
                     Piscina* piscina,
    constans CrustaDialectus* dialectus)
{
    memset(machina, ZEPHYRUM, magnitudo(*machina));
    machina->piscina    = piscina;
    machina->dialectus  = dialectus;
    machina->operanda = xar_creare(piscina,
        (i32)magnitudo(Operandum));
    machina->signa = xar_creare(piscina, (i32)magnitudo(Signum));
    machina->operandum_exspectatur = VERUM;
    machina->ultimum = (i32)ULTIMUM_NIHIL;
}

interior Operandum
_operandum_tollere (
    CrustaArithmetica* m)
{
    Operandum o;

    si (!xar_tollere(m->operanda, &o))
    {
        m->mala++;
        o.nodus    = NIHIL;
        o.corpora  = NIHIL;
    }
    redde o;
}

interior b32
_operandum_pellere (
    CrustaArithmetica* m,
         MateriaNodus* n,
                  Xar* corpora)
{
    Operandum* sedes = (Operandum*)xar_addere(m->operanda);

    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    sedes->nodus    = n;
    sedes->corpora  = corpora;
    redde VERUM;
}

interior b32
_signum_pellere (
          CrustaArithmetica* m,
               MateriaToken* tok,
    constans CrustaOperator* op)
{
    Signum* sedes = (Signum*)xar_addere(m->signa);

    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    sedes->tok           = tok;
    sedes->colon         = NIHIL;
    sedes->op            = op;
    sedes->corpora       = NIHIL;
    sedes->corpora_coli  = NIHIL;
    m->ultimum           = (i32)ULTIMUM_SIGNUM;
    redde VERUM;
}

interior b32
_ponere (
    MateriaNodus* nodus,
             i32  locus,
    MateriaNodus* filius)
{
    si (filius == NIHIL)
    {
        redde VERUM;   /* absens */
    }
    redde materia_nodus_ponere(nodus, locus,
        materia_valor_nodus(filius),
        MATERIA_LOCUS_NODUS);
}

interior b32
_tok_ponere (
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* t)
{
    si (t == NIHIL)
    {
        redde VERUM;
    }
    redde materia_nodus_ponere(nodus, locus, materia_valor_token(t),
        MATERIA_LOCUS_TOKEN);
}

/* corpora in listam loci appendere (NIHIL = nulla) */
interior b32
_corpora_ponere (
    CrustaArithmetica* m,
         MateriaNodus* nodus,
                  i32  locus,
                  Xar* corpora)
{
    i32 i;

    si (corpora == NIHIL)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(corpora); i++)
    {
        MateriaNodus* c = *(MateriaNodus**)xar_obtinere(corpora, i);

        si (!materia_nodus_appendere(m->piscina, nodus, locus,
                materia_valor_nodus(c), MATERIA_LOCUS_LISTA_NODUS))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* corpus in corpora (Xar creata si NIHIL) addere */
interior b32
_corpus_addere (
    CrustaArithmetica*  m,
                  Xar** corpora,
         MateriaNodus*  corpus)
{
    MateriaNodus** sedes;

    si (*corpora == NIHIL)
    {
        *corpora = xar_creare(m->piscina,
            (i32)magnitudo(MateriaNodus*));
        si (*corpora == NIHIL)
        {
            redde FALSUM;
        }
    }
    sedes = (MateriaNodus**)xar_addere(*corpora);
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    *sedes = corpus;
    redde VERUM;
}

/* signum verticis reducere in nodum. dexter_absens: operandum dextrum
 * numquam venit (operator caudalis, aut operator post operatorem) -
 * locus absens, ordo octetorum servatus. CORPORA (P9b): post operandum
 * et post signum in listas post_* nodi novi; post operandum ULTIMUM
 * (dextrum) cum nodo novo ascendunt. */
interior b32
_reducere (
    CrustaArithmetica* m,
                  b32  dexter_absens)
{
       Signum s;
    MateriaNodus* n;
    Operandum  dexter;
    Operandum  sinister;
    Operandum  probatio;

    si (!xar_tollere(m->signa, &s))
    {
        redde FALSUM;
    }
    si (dexter_absens)
    {
        m->mala++;
    }
    dexter.nodus    = NIHIL;
    dexter.corpora  = NIHIL;
    commutatio (s.op->species)
    {
        casus CRUSTA_OPERATOR_PRAEPOSITUS:
            si (!dexter_absens)
            {
                dexter = _operandum_tollere(m);
            }
            n = materia_nodus_creare(m->piscina,
                (s32)CRUSTA_GENUS_PRAEPOSITA,
                _loci_numerus((s32)CRUSTA_GENUS_PRAEPOSITA));
            si (   n == NIHIL
                || !_tok_ponere(n, (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR,
                    s.tok)
                || !_corpora_ponere(m, n,
                (i32)CRUSTA_PRAEPOSITA_POST_SIGNUM,
                    s.corpora)
                || !_ponere(n, (i32)CRUSTA_PRAEPOSITA_OPERANDUM,
                    dexter.nodus))
            {
                redde FALSUM;
            }
            frange;
        casus CRUSTA_OPERATOR_TERNARIUS:
            si (s.colon != NIHIL && !dexter_absens)
            {
                dexter = _operandum_tollere(m);
            }
            sinister = _operandum_tollere(m);
            probatio = _operandum_tollere(m);
            n = materia_nodus_creare(m->piscina,
                (s32)CRUSTA_GENUS_TERNARIA,
                _loci_numerus((s32)CRUSTA_GENUS_TERNARIA));
            si (   n == NIHIL
                || !_ponere(n, (i32)CRUSTA_TERNARIA_PROBATIO,
                    probatio.nodus)
                || !_corpora_ponere(m, n,
                    (i32)CRUSTA_TERNARIA_POST_PROBATIONEM,
                    probatio.corpora)
                || !_tok_ponere(n, (i32)CRUSTA_TERNARIA_TOK_QUAESTIO,
                    s.tok)
                || !_corpora_ponere(m, n,
                    (i32)CRUSTA_TERNARIA_POST_QUAESTIONEM, s.corpora)
                || !_ponere(n, (i32)CRUSTA_TERNARIA_SINISTER,
                    sinister.nodus)
                || !_corpora_ponere(m, n,
                    (i32)CRUSTA_TERNARIA_POST_SINISTRUM,
                    sinister.corpora)
                || !_tok_ponere(n, (i32)CRUSTA_TERNARIA_TOK_COLON,
                    s.colon)
                || !_corpora_ponere(m, n,
                (i32)CRUSTA_TERNARIA_POST_COLON,
                    s.corpora_coli)
                || !_ponere(n, (i32)CRUSTA_TERNARIA_DEXTER,
                dexter.nodus))
            {
                redde FALSUM;
            }
            frange;
        ordinarius:   /* BINARIUS (et ':' orphanum, iuxtapositio) */
            si (!dexter_absens)
            {
                dexter = _operandum_tollere(m);
            }
            sinister = _operandum_tollere(m);
            n = materia_nodus_creare(m->piscina,
                (s32)CRUSTA_GENUS_BINARIA,
                _loci_numerus((s32)CRUSTA_GENUS_BINARIA));
            si (   n == NIHIL
                || !_ponere(n, (i32)CRUSTA_BINARIA_SINISTER,
                    sinister.nodus)
                || !_corpora_ponere(m, n,
                (i32)CRUSTA_BINARIA_POST_SINISTRUM,
                    sinister.corpora)
                || !_tok_ponere(n, (i32)CRUSTA_BINARIA_TOK_OPERATOR,
                    s.tok)
                || !_corpora_ponere(m, n,
                (i32)CRUSTA_BINARIA_POST_SIGNUM,
                    s.corpora)
                || !_ponere(n, (i32)CRUSTA_BINARIA_DEXTER,
                dexter.nodus))
            {
                redde FALSUM;
            }
            frange;
    }
    redde _operandum_pellere(m, n, dexter.corpora);
}

interior constans Signum*
_vertex (
    constans CrustaArithmetica* m)
{
    i32 n = xar_numerus(m->signa);

    redde n > ZEPHYRUM ? (constans Signum*)xar_obtinere(m->signa, n - I)
        : NIHIL;
}

/* IUXTAPOSITIO ('a b'): operator binarius IMPLICITUS sine lexemate,
 * praecedentia infima et sinistra - signa omnia priora reducuntur,
 * deinde binaria cum operatore absenti formatur (ordo octetorum
 * servatus, mala++). INVENTUM P9 (porta totalitatis): operandum prius
 * in finire olim perdebatur - '(( a b ))' emittebat '(( b ))'. */
hic_manens constans CrustaOperator IUXTAPOSITIO = {
    "", ZEPHYRUM, FALSUM, CRUSTA_OPERATOR_BINARIUS
};

b32
crusta_arithmetica_operandum (
    CrustaArithmetica* machina,
         MateriaNodus* nodus)
{
    si (!machina->operandum_exspectatur)
    {
        machina->mala++;
        dum (_vertex(machina) != NIHIL)
        {
            si (!_reducere(machina, FALSUM))
            {
                redde FALSUM;
            }
        }
        si (!_signum_pellere(machina, NIHIL, &IUXTAPOSITIO))
        {
            redde FALSUM;
        }
    }
    machina->operandum_exspectatur  = FALSUM;
    machina->ultimum                = (i32)ULTIMUM_OPERANDUM;
    redde _operandum_pellere(machina, nodus, NIHIL);
}

b32
crusta_arithmetica_interponere (
    CrustaArithmetica* machina,
         MateriaNodus* corpus)
{
    i32 n;

    si (machina->ultimum == (i32)ULTIMUM_OPERANDUM)
    {
        n = xar_numerus(machina->operanda);
        si (n > ZEPHYRUM)
        {
            Operandum* o = (Operandum*)xar_obtinere(machina->operanda,
                n - I);

            redde _corpus_addere(machina, &o->corpora, corpus);
        }
    }
    alioquin si (   machina->ultimum == (i32)ULTIMUM_SIGNUM
                 || machina->ultimum == (i32)ULTIMUM_COLON)
    {
        n = xar_numerus(machina->signa);
        si (n > ZEPHYRUM)
        {
            Signum* s = (Signum*)xar_obtinere(machina->signa, n - I);

            redde _corpus_addere(machina,
                machina->ultimum == (i32)ULTIMUM_COLON
                    ? &s->corpora_coli : &s->corpora,
                corpus);
        }
    }
    redde _corpus_addere(machina, &machina->corpora_initii, corpus);
}

/* colon orphanum ut binaria (totalitas) */
hic_manens constans CrustaOperator COLON_ORPHANUM = {
    ":", (i32)III, VERUM, CRUSTA_OPERATOR_BINARIUS
};

b32
crusta_arithmetica_operator (
    CrustaArithmetica* machina,
         MateriaToken* signum)
{
    constans CrustaOperator* o;

    si (machina->operandum_exspectatur)
    {
        o = crusta_arithmetica_quaerere(machina->dialectus,
            signum->valor,
            CRUSTA_OPERATOR_PRAEPOSITUS);
        si (o == NIHIL)
        {
            /* operator ubi operandum exspectatur: operator prior
             * dextrum absens accipit (aut, initio, sinister absens);
             * ordo octetorum servatur */
            si (xar_numerus(machina->signa) > ZEPHYRUM)
            {
                si (!_reducere(machina, VERUM))
                {
                    redde FALSUM;
                }
            }
            alioquin
            {
                machina->mala++;
            }
            machina->operandum_exspectatur = FALSUM;
            o = crusta_arithmetica_quaerere(machina->dialectus,
                signum->valor, CRUSTA_OPERATOR_BINARIUS);
            si (o == NIHIL)
            {
                o = crusta_arithmetica_quaerere(machina->dialectus,
                    signum->valor, CRUSTA_OPERATOR_TERNARIUS);
            }
            si (o == NIHIL)
            {
                o = &COLON_ORPHANUM;
            }
        }
        alioquin
        {
            redde _signum_pellere(machina, signum, o);
        }
    }
    alioquin
    {
        o = crusta_arithmetica_quaerere(machina->dialectus,
            signum->valor,
            CRUSTA_OPERATOR_POSTPOSITUS);
        si (o != NIHIL)
        {
            MateriaNodus* n = materia_nodus_creare(machina->piscina,
                (s32)CRUSTA_GENUS_POSTPOSITA,
                _loci_numerus((s32)CRUSTA_GENUS_POSTPOSITA));
                Operandum operandum = _operandum_tollere(machina);

            si (   n == NIHIL
                || !_ponere(n, (i32)CRUSTA_POSTPOSITA_OPERANDUM,
                    operandum.nodus)
                || !_corpora_ponere(machina, n,
                    (i32)CRUSTA_POSTPOSITA_POST_OPERANDUM,
                    operandum.corpora)
                || !_tok_ponere(n, (i32)CRUSTA_POSTPOSITA_TOK_OPERATOR,
                    signum))
            {
                redde FALSUM;
            }
            machina->ultimum = (i32)ULTIMUM_OPERANDUM;
            redde _operandum_pellere(machina, n, NIHIL);
        }
        o = crusta_arithmetica_quaerere(machina->dialectus,
            signum->valor,
            CRUSTA_OPERATOR_BINARIUS);
        si (o == NIHIL)
        {
            o = crusta_arithmetica_quaerere(machina->dialectus,
                signum->valor, CRUSTA_OPERATOR_TERNARIUS);
        }
        si (o == NIHIL)
        {
            /* praepositus post operandum: '3 !': mala, ut binarius */
            machina->mala++;
            o = &COLON_ORPHANUM;
        }
    }

    /* ':' - '?' apertum in vertice quaeritur post reductiones */
    dum (_vertex(machina) != NIHIL)
    {
        constans Signum* v = _vertex(machina);
                    b32  reducendum;
                    b32  colon_novum;

        colon_novum = o->species == CRUSTA_OPERATOR_TERNARIUS
            && signum->valor.datum[ZEPHYRUM] == ':';
        /* ':' novum: ternaria completa in vertice prius reducitur
         * (1?0?4:5:6) */
        reducendum = v->op->praecedentia > o->praecedentia
            || (v->op->praecedentia == o->praecedentia && !o->dextra)
            || (colon_novum
                && v->op->species == CRUSTA_OPERATOR_TERNARIUS
                && v->colon != NIHIL);

        si (   o->species == CRUSTA_OPERATOR_TERNARIUS
            && signum->valor.datum[ZEPHYRUM] == ':'
            && v->op->species == CRUSTA_OPERATOR_TERNARIUS
            && v->colon == NIHIL)
        {
            /* '?' apertum: colon accipit, operandum exspectatur */
            Signum* mutandum = (Signum*)xar_obtinere(machina->signa,
                xar_numerus(machina->signa) - I);

            mutandum->colon                 = signum;
            machina->operandum_exspectatur  = VERUM;
            machina->ultimum                = (i32)ULTIMUM_COLON;
            redde VERUM;
        }
        si (!reducendum)
        {
            frange;
        }
        si (!_reducere(machina, FALSUM))
        {
            redde FALSUM;
        }
    }
    si (   o->species                    == CRUSTA_OPERATOR_TERNARIUS
        && signum->valor.datum[ZEPHYRUM] == ':')
    {
        /* ':' sine '?': orphanum */
        machina->mala++;
        o = &COLON_ORPHANUM;
    }
    si (!_signum_pellere(machina, signum, o))
    {
        redde FALSUM;
    }
    machina->operandum_exspectatur = VERUM;
    redde VERUM;
}

MateriaNodus*
crusta_arithmetica_finire (
    CrustaArithmetica*  machina,
                  Xar** corpora_initii,
                  Xar** corpora_post)
{
    Operandum o;

    *corpora_initii  = machina->corpora_initii;
    *corpora_post    = NIHIL;
    /* operator caudalis: dextrum absens */
    si (   machina->operandum_exspectatur && xar_numerus(machina->signa)
        > ZEPHYRUM)
    {
        si (!_reducere(machina, VERUM))
        {
            redde NIHIL;
        }
        machina->operandum_exspectatur = FALSUM;
    }
    dum (xar_numerus(machina->signa) > ZEPHYRUM)
    {
        si (!_reducere(machina, FALSUM))
        {
            redde NIHIL;
        }
    }
    si (xar_numerus(machina->operanda) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    si (xar_numerus(machina->operanda) > (i32)I)
    {
        machina->mala++;
    }
    xar_tollere(machina->operanda, &o);
    *corpora_post = o.corpora;
    redde o.nodus;
}


/* ==================================================
 * Aestimator constans
 * ================================================== */

interior b32
_numerum_legere (
    constans MateriaToken* t,
                      s64* valor)
{
    constans character* d      = (constans character*)t->valor.datum;
                   i32  n      = t->valor.mensura;
                   i32  i      = ZEPHYRUM;
                   i64  basis  = (i64)X;
                   i64  v      = ZEPHYRUM;

    si (   n >= (i32)II && d[ZEPHYRUM] == '0'
        && (d[I] == 'x' || d[I] == 'X'))
    {
        basis  = (i64)XVI;
        i      = (i32)II;
    }
    alioquin
    {
        i32 k;

        per (k = ZEPHYRUM; k < n; k++)
        {
            si (d[k] == '#')
            {
                s64 b = ZEPHYRUM;
                i32 j;

                per (j = ZEPHYRUM; j < k; j++)
                {
                    si (d[j] < '0' || d[j] > '9')
                    {
                        redde FALSUM;
                    }
                    b = b * (s64)X + (s64)(d[j] - '0');
                }
                si (b < (s64)II || b > (s64)LXIV)
                {
                    redde FALSUM;
                }
                basis  = (i64)b;
                i      = k + I;
                frange;
            }
        }
        si (basis == (i64)X && n > I && d[ZEPHYRUM] == '0')
        {
            basis  = (i64)VIII;
            i      = I;
        }
    }
    per (; i < n; i++)
    {
        character c = d[i];
              i64 digitus;

        si (c >= '0' && c <= '9')
        {
            digitus = (i64)(c - '0');
        }
        alioquin si (c >= 'a' && c <= 'z')
        {
            digitus = (i64)(c - 'a') + (i64)X;
            /* basis <= XXXVI: litterae maiusculae et minusculae
             * aequae */
        }
        alioquin si (c >= 'A' && c <= 'Z')
        {
            digitus = (i64)(c - 'A') + (basis <= (i64)XXXVI ? (i64)X
                : (i64)XXXVI);
        }
        alioquin si (c == '@')
        {
            digitus = (i64)LXII;
        }
        alioquin si (c == '_')
        {
            digitus = (i64)LXIII;
        }
        alioquin
        {
            redde FALSUM;
        }
        si (digitus >= basis)
        {
            redde FALSUM;
        }
        v = v * basis + digitus;
    }
    *valor = (s64)v;
    redde VERUM;
}

interior b32
_signum_est (
    constans MateriaNodus* n,
                      i32  locus,
       constans character* litterae)
{
    constans MateriaValor* v = &n->loci[locus];

    redde v->genus == MATERIA_VALOR_TOKEN
        && v->datum.token->valor.mensura == (i32)strlen(litterae)
        && memcmp(v->datum.token->valor.datum, litterae,
        strlen(litterae))
            == ZEPHYRUM;
}

interior b32
_aestimare_locum (
    constans MateriaNodus* n,
                      i32  locus,
                      s64* valor)
{
    constans MateriaValor* v = &n->loci[locus];

    si (v->genus != MATERIA_VALOR_NODUS)
    {
        redde FALSUM;
    }
    redde crusta_arithmetica_aestimare(v->datum.nodus, valor);
}

b32
crusta_arithmetica_aestimare (
    constans MateriaNodus* e,
                      s64* valor)
{
    s64 a;
    s64 b;
    s64 c;

    si (e == NIHIL)
    {
        redde FALSUM;
    }
    commutatio (e->genus)
    {
        casus CRUSTA_GENUS_NUMERUS:
            si (e->loci[CRUSTA_NUMERUS_TOK].genus
                != MATERIA_VALOR_TOKEN)
            {
                redde FALSUM;
            }
            redde _numerum_legere(
                e->loci[CRUSTA_NUMERUS_TOK].datum.token, valor);
        casus CRUSTA_GENUS_INCLUSA:
            redde _aestimare_locum(e, (i32)CRUSTA_INCLUSA_EXPRESSIO,
                valor);
        casus CRUSTA_GENUS_PRAEPOSITA:
            si (!_aestimare_locum(e, (i32)CRUSTA_PRAEPOSITA_OPERANDUM,
                &a))
            {
                redde FALSUM;
            }
            si (_signum_est(e, (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR,
                "-"))
            {
                *valor = (s64)(ZEPHYRUM - (i64)a);
            }
            alioquin si (_signum_est(e,
                         (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR,
                         "+"))
            {
                *valor = a;
            }
            alioquin si (_signum_est(e,
                         (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR,
                         "!"))
            {
                *valor = a == ZEPHYRUM ? (s64)I : ZEPHYRUM;
            }
            alioquin si (_signum_est(e,
                         (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR,
                         "~"))
            {
                *valor = (s64)~(i64)a;
            }
            alioquin
            {
                redde FALSUM;   /* ++ -- non constantia */
            }
            redde VERUM;
        casus CRUSTA_GENUS_TERNARIA:
            si (!_aestimare_locum(e, (i32)CRUSTA_TERNARIA_PROBATIO, &c))
            {
                redde FALSUM;
            }
            redde c != ZEPHYRUM
                ? _aestimare_locum(e, (i32)CRUSTA_TERNARIA_SINISTER,
                valor)
                : _aestimare_locum(e, (i32)CRUSTA_TERNARIA_DEXTER,
                valor);
        casus CRUSTA_GENUS_BINARIA:
        {
            constans MateriaValor* s =
                &e->loci[CRUSTA_BINARIA_TOK_OPERATOR];
               constans character* op;
                              i32  n;

            si (s->genus != MATERIA_VALOR_TOKEN)
            {
                redde FALSUM;
            }
            op  = (constans character*)s->datum.token->valor.datum;
            n   = s->datum.token->valor.mensura;
            /* comma: dexter solus; && || breviter */
            si (n == I && op[ZEPHYRUM] == ',')
            {
                redde _aestimare_locum(e, (i32)CRUSTA_BINARIA_DEXTER,
                    valor);
            }
            si (!_aestimare_locum(e, (i32)CRUSTA_BINARIA_SINISTER, &a))
            {
                redde FALSUM;
            }
            si (n == (i32)II && op[ZEPHYRUM] == '&' && op[I] == '&')
            {
                si (a == ZEPHYRUM)
                {
                    *valor = ZEPHYRUM;
                    redde VERUM;
                }
                si (!_aestimare_locum(e, (i32)CRUSTA_BINARIA_DEXTER,
                    &b))
                {
                    redde FALSUM;
                }
                *valor = b != ZEPHYRUM ? (s64)I : ZEPHYRUM;
                redde VERUM;
            }
            si (n == (i32)II && op[ZEPHYRUM] == '|' && op[I] == '|')
            {
                si (a != ZEPHYRUM)
                {
                    *valor = (s64)I;
                    redde VERUM;
                }
                si (!_aestimare_locum(e, (i32)CRUSTA_BINARIA_DEXTER,
                    &b))
                {
                    redde FALSUM;
                }
                *valor = b != ZEPHYRUM ? (s64)I : ZEPHYRUM;
                redde VERUM;
            }
            si (!_aestimare_locum(e, (i32)CRUSTA_BINARIA_DEXTER, &b))
            {
                redde FALSUM;
            }
            si (n == I)
            {
                commutatio (op[ZEPHYRUM])
                {
                    casus '+':
                        *valor = (s64)((i64)a + (i64)b);
                        redde VERUM;
                    casus '-':
                        *valor = (s64)((i64)a - (i64)b);
                        redde VERUM;
                    casus '*':
                        *valor = (s64)((i64)a * (i64)b);
                        redde VERUM;
                    casus '/':
                        si (b == ZEPHYRUM)
                        {
                            redde FALSUM;
                        }
                        *valor = a / b;
                        redde VERUM;
                    casus '%':
                        si (b == ZEPHYRUM)
                        {
                            redde FALSUM;
                        }
                        *valor = a % b;
                        redde VERUM;
                    casus '<':
                        *valor = a < b;
                        redde VERUM;
                    casus '>':
                        *valor = a > b;
                        redde VERUM;
                    casus '&':
                        *valor = (s64)((i64)a & (i64)b);
                        redde VERUM;
                    casus '^':
                        *valor = (s64)((i64)a ^ (i64)b);
                        redde VERUM;
                    casus '|':
                        *valor = (s64)((i64)a | (i64)b);
                        redde VERUM;
                    ordinarius:
                        redde FALSUM;   /* '=' */
                }
            }
            si (n == (i32)II)
            {
                si (op[ZEPHYRUM] == '*' && op[I] == '*')
                {
                    i64 r      = (i64)I;
                    i64 basis  = (i64)a;
                    s64 k;

                    si (b < ZEPHYRUM) redde FALSUM;
                    per (k = ZEPHYRUM; k < b; k++)
                    {
                        r = r * basis;
                    }
                    *valor = (s64)r;
                    redde VERUM;
                }
                si (op[ZEPHYRUM] == '<' && op[I] == '<')
                {
                    *valor = (s64)((i64)a << ((i64)b & (i64)LXIII));
                    redde VERUM;
                }
                si (op[ZEPHYRUM] == '>' && op[I] == '>')
                {
                    *valor = a >> ((i64)b & (i64)LXIII);
                    redde VERUM;
                }
                si (   op[I]        == '=' && op[ZEPHYRUM] != '='
                    && op[ZEPHYRUM] != '!'
                    && op[ZEPHYRUM] != '<' && op[ZEPHYRUM] != '>')
                {
                    redde FALSUM;   /* assignationes compositae */
                }
                si (op[ZEPHYRUM] == '<' && op[I] == '=')
                {
                    *valor = a <= b;
                    redde VERUM;
                }
                si (op[ZEPHYRUM] == '>' && op[I] == '=')
                {
                    *valor = a >= b;
                    redde VERUM;
                }
                si (op[ZEPHYRUM] == '=' && op[I] == '=')
                {
                    *valor = a == b;
                    redde VERUM;
                }
                si (op[ZEPHYRUM] == '!' && op[I] == '=')
                {
                    *valor = a != b;
                    redde VERUM;
                }
            }
            redde FALSUM;
        }
        ordinarius:
            redde FALSUM;   /* variabilis, postposita, partes */
    }
}
