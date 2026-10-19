#!/usr/bin/env python3
# tools/pecunia_rotundatio_oraculum.py - ORACULUM rotundationis pecuniae
#
# Scribit probationes/fixa/pecunia/rotundatio.txt per modulum decimal
# Pythonis (bibliotheca norma, nihil aliud) - SEMEL currendum; fixura
# committitur cum sigillo sha256 in probationes/probatio_pecunia.c.
# Probatio C fixuram SOLAM legit: Python in probatione numquam eget.
#
# Lineae (pecunia-plan T3, regula P5):
#   M <modus> <quantitas> <num>/<den> <exspectatum>
#       round(quantitas * num / den) ad integrum (decimillesimae)
#   U <modus> <exponens> <quantitas> <exspectatum>
#       quantitas (scala IV) ad <exponens> decimales, effectus in scala IV
# <exspectatum> = integer aut SUPERFLUXUS (extra s64).
#
# Modi: PAR = ROUND_HALF_EVEN, DIMIDIUM_SURSUM = ROUND_HALF_UP (Pythonis:
# dimidium a nihilo), VERSUS_NIHIL = ROUND_DOWN, SOLUM = ROUND_FLOOR,
# TECTUM = ROUND_CEILING. Praecisio LXXX: quotiens terminans exacte; non
# terminans dimidium habere non potest, ergo truncatio directionem non
# mutat.
#
# Usus: python3 -I tools/pecunia_rotundatio_oraculum.py > probationes/fixa/pecunia/rotundatio.txt
import decimal
import random
import sys

D = decimal.Decimal
CTX = decimal.Context(prec=80)
MODI = [('PAR', decimal.ROUND_HALF_EVEN),
        ('DIMIDIUM_SURSUM', decimal.ROUND_HALF_UP),
        ('VERSUS_NIHIL', decimal.ROUND_DOWN),
        ('SOLUM', decimal.ROUND_FLOOR),
        ('TECTUM', decimal.ROUND_CEILING)]
MAX = 2 ** 63 - 1
MIN = -(2 ** 63)


def capit(v):
    return MIN <= v <= MAX


def multiplica(q, num, den, rot):
    v = CTX.divide(D(q * num), D(den))
    r = int(v.to_integral_value(rounding=rot, context=CTX))
    return r if capit(r) else 'SUPERFLUXUS'


def ad_unitatem(q, e, rot):
    f = 10 ** (4 - e)
    v = CTX.divide(D(q), D(f))
    r = int(v.to_integral_value(rounding=rot, context=CTX)) * f
    return r if capit(r) else 'SUPERFLUXUS'


Q_M = [0, 1, -1, 3, -3, 5, -5, 15, -15, 25, -25, 35, 49, -49, 50, -50,
       51, -51, 150, 250, -250, 10000, -10000, 123456789, -123456789,
       MAX, MAX - 1, MIN + 1, MIN]
F_M = [(1, 1), (1, 2), (-1, 2), (1, 3), (2, 3), (-2, 3), (71, 800),
       (7, 4), (3, 2), (5, 2), (1, 10), (9, 10), (-9, 10), (1, 7), (2, 1),
       (100, 1)]
Q_U = [0, 1, -1, 49, -49, 50, -50, 51, -51, 150, -150, 250, -250, 4999,
       5000, -5000, 5001, 9999, -9999, 15000, 25000, -25000, 123456,
       -123456, MAX, MAX - 1, MIN, MIN + 1]

# exponentes monetarum veri (tabula ISO 2026-09-17): nulla moneta exponentem
# I habet, et ad_unitatem exponentem ex moneta sumit - linea quae per API
# probari non potest fixuram non intrat (pecunia-plan R5)
EXPONENTES = [0, 2, 3, 4]

o = sys.stdout
o.write('# pecunia rotundatio - oraculum SEMEL scriptum a tools/pecunia_rotundatio_oraculum.py\n')
o.write('# (decimal Pythonis %s, praecisio 80); fixura sigillata - noli manu mutare\n' % decimal.__version__)
o.write('# M <modus> <quantitas> <num>/<den> <exspectatum>\n')
o.write('# U <modus> <exponens> <quantitas> <exspectatum>\n')
for q in Q_M:
    for num, den in F_M:
        for nomen, rot in MODI:
            o.write('M %s %d %d/%d %s\n' % (nomen, q, num, den,
                                            multiplica(q, num, den, rot)))
rnd = random.Random(1729)
for _ in range(200):
    q = rnd.randint(-10 ** 15, 10 ** 15)
    num = rnd.choice([-1, 1]) * rnd.randint(1, 1000)
    den = rnd.randint(1, 1000)
    for nomen, rot in MODI:
        o.write('M %s %d %d/%d %s\n' % (nomen, q, num, den,
                                        multiplica(q, num, den, rot)))
for e in EXPONENTES:
    for q in Q_U:
        for nomen, rot in MODI:
            o.write('U %s %d %d %s\n' % (nomen, e, q, ad_unitatem(q, e, rot)))
for _ in range(200):
    q = rnd.randint(-10 ** 15, 10 ** 15)
    e = rnd.choice(EXPONENTES)
    for nomen, rot in MODI:
        o.write('U %s %d %d %s\n' % (nomen, e, q, ad_unitatem(q, e, rot)))
