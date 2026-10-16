/* demo-snapshot.c - GENERATUM (knotapel/archive.sh) - DO NOT EDIT
 *
 * knotapel/demo_128_braid_relu/main.c frozen with its house-library closure as ONE file:
 * headers in dependency order, library sources with file-local
 * names renamed per file (#define/#undef), main.c last; '#line'
 * names each original file. Compile and run:
 *
 *   clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -fbracket-depth=512 -O2 -g demo-snapshot.c -o demo-snapshot
 *
 * Commit (library closure clean): 1fd650ea4d59f3d1ab238ac4a9004ff280772f02
 * Regenerate: ./knotapel/archive.sh knotapel/demo_128_braid_relu/main.c
 * Verified: live build and snapshot gave byte-identical output.
 * Sources (git blob hashes):
 *   64068da453692e55d89f3395ecd9a0e9350cfa29  include/anulus.h
 *   7db315706b013efbb850928c78e1c13b0fb72362  include/chorda.h
 *   6f9b7a043cebe2912c612139453b66fcb04770cb  include/chorda_aedificator.h
 *   37b6fb1c16e76827120976705e418e11c89406a6  include/congruentia.h
 *   26098ae7c146ddd6bdd9e18529319eca305a2f3b  include/cyclotomia.h
 *   53645f652dd16a7a8c8ad79df9e2e70289ee5e52  include/fractio.h
 *   f45b10ad9c303c02d43655b950600fcdab8997bb  include/latina.h
 *   bfde2073ff18d6f5be1c3844556cc82d39ee0937  include/magnus.h
 *   423a176668f285b2661452e215cdf84259389d93  include/matrix.h
 *   cd2db07dbd9f7bb6f71ffaa27b03f7cdb8ea65ee  include/piscina.h
 *   b43fbd63a8b65ce0c9344a5994dc1d4786ac69df  include/plexus.h
 *   95f5c575951519ca8cee6170ec3a4519b284d0c4  include/polynomium.h
 *   a34b9f2536efa81a09cea36f9a9acba28fb4b2fc  include/postulata_posix.h
 *   3b6102802849c1047af220e2b303523db1df5000  include/radices.h
 *   659b865720bcca5d0fbd4f9eaf5f3fe7baa8cf1e  include/radices_interna.h
 *   91e1de36acad556015e29cef636227ac270a6d56  include/sors.h
 *   9f20940b276fc89410cce3ddffdd62a77ac12a29  lib/anulus.c
 *   b4c8c649c03e84eca53408282c39d5b0e24c6016  lib/chorda.c
 *   ee055a36d7e4726ad5b3731e2ca64e62e93d084c  lib/chorda_aedificator.c
 *   c446f3092b17cdca38b584886d86a5ca80856eac  lib/congruentia.c
 *   1c4e7da9aa888a2fe56cc33028e6b1455be8a1dc  lib/cyclotomia.c
 *   61ec3b3106345cce9ea64477aafe7d1072030be7  lib/fractio.c
 *   fd94a8d26a4bd4b340875ac0a1551bc3f3c55e8e  lib/magnus.c
 *   5fc06983f571b6cf493cc884ab9647db9f499d44  lib/matrix.c
 *   c6ab1e19274a3b36ff5cfdde651e45d079905b56  lib/piscina.c
 *   6acb2e0b1c79c62aa0ae99c67a672d6f12570f97  lib/plexus.c
 *   9967d4ccc5d5f5e18bf84f716b4f8fb7b7964a85  lib/polynomium.c
 *   85567c63931bd36cd7ea3eb8fb64ee169ab9ac2e  lib/radices.c
 *   a1a3bbc96bc609e89e4098ef95f770c5e10a3ea5  lib/sors.c
 *   8a802fc9835a5f9d845aab249c0dda0065871cb3  knotapel/demo_128_braid_relu/main.c (uncommitted, embedded verbatim)
 */

#line 1 "include/postulata_posix.h"
/* postulata_posix.h - postulata platformae pro superficie POSIX
 *
 * SUTURA praeprocessoris pura: interfacies portabilis, mores
 * per-platformam. glibc sub -std=c89 declarationes POSIX CELAT nisi
 * macro probationis proprietatum ante caput systematis primum
 * definitur; Darwin et musl ordinarie permissivi sunt. Sine hoc
 * capite plagula quaeque POSIX-utens in Linux glibc cadit
 * (tcp_posix.c: XX errores ex radicibus IV celatis - mensuratum).
 *
 * CUR _DEFAULT_SOURCE: sonda Docker 2026-08-03 (glibc 2.35 gcc 11.4;
 * musl 1.2.5 gcc 13.2; VI plagulae x V variantes - acta in actis
 * tabularii 01KYTGNA36) mensuravit: _DEFAULT_SOURCE omnia
 * macro-sanabilia in AMBABUS libc sanat et in Darwin nihil agit.
 * Variantes strictae PEIORES sunt, non aequales: _XOPEN_SOURCE 700
 * et _POSIX_C_SOURCE usleep RE-CELANT (XPG7 sustulit). Decretum
 * 01KZ3RYZWK: caput unum, non definitiones per plagulam.
 *
 * LEX (codex examinis 85 custodit): hoc caput inclusio PRIMA
 * plagulae POSIX-utentis sit - ante caput proprium, ante latina.h.
 * features.h glibc copiam SEMEL figit, primo tactu capitis systematis
 * cuiuslibet; latina.h stddef.h trahit, ergo "prima" ad litteram.
 *
 * Nomen _DEFAULT_SOURCE classis reservatae est (C89 7.1.3) -
 * REFERIMUS interruptorem glibc documentatum, non coinamus (eadem
 * licentia qua externa systematis referuntur).
 */

#ifndef POSTULATA_POSIX_H
#define POSTULATA_POSIX_H

#define _DEFAULT_SOURCE 1

#endif /* POSTULATA_POSIX_H */
#line 1 "include/latina.h"
/* latina.h - Verba clavis C89 Latine reddita (lexicon domus) */
#ifndef LATINA_H
#define LATINA_H

#include <stddef.h>

#define character     char
#define brevis             short
#define integer         int
#define longus            long
#define fluitans        float
#define duplex            double

#define vacuum            void
#define signatus         signed
#define insignatus  unsigned
#define constans        const
#define volatilis        volatile
#define sponte            auto
#define registrum     register
#define staticus         static
#define    externus         extern

#define si                    if
#define alioquin        else
#define commutatio    switch
#define casus                case
#define ordinarius    default
#define per                    for
#define dum                 while
#define fac                 do
#define frange             break
#define perge             continue
#define salta                goto
#define redde                return

#define structura        struct
#define unio                 union
#define enumeratio     enum
#define nomen             typedef

#define magnitudo     sizeof

#define principale     main

#define NIHIL                NULL
#define VERUM             1
#define FALSUM             0

/* NUMERI ROMANI - GENERATUM ex numerus_romanus_scribere
 * (tools/latina_numeri.sh -scribere) - NE MANU MUTES; porta
 * 'generata' iudicat. Omnes ZEPHYRUM-MMMCMXCIX ordine: numerus
 * Romanus classicus ad MMMCMXCIX finit - maiores ut expressio
 * (vinculum ut '* M'): IV * M (4000), IV * MXXIV (4096); vide
 * numerus_romanus_exprimere. Omne numerale identificator
 * reservatus est: capita systematis ANTE latina.h includenda
 * (dns_util.h membra 'MD' et 'MX' habet). */
#define ZEPHYRUM         0
#define I                1
#define II               2
#define III              3
#define IV               4
#define V                5
#define VI               6
#define VII              7
#define VIII             8
#define IX               9
#define X                10
#define XI               11
#define XII              12
#define XIII             13
#define XIV              14
#define XV               15
#define XVI              16
#define XVII             17
#define XVIII            18
#define XIX              19
#define XX               20
#define XXI              21
#define XXII             22
#define XXIII            23
#define XXIV             24
#define XXV              25
#define XXVI             26
#define XXVII            27
#define XXVIII           28
#define XXIX             29
#define XXX              30
#define XXXI             31
#define XXXII            32
#define XXXIII           33
#define XXXIV            34
#define XXXV             35
#define XXXVI            36
#define XXXVII           37
#define XXXVIII          38
#define XXXIX            39
#define XL               40
#define XLI              41
#define XLII             42
#define XLIII            43
#define XLIV             44
#define XLV              45
#define XLVI             46
#define XLVII            47
#define XLVIII           48
#define XLIX             49
#define L                50
#define LI               51
#define LII              52
#define LIII             53
#define LIV              54
#define LV               55
#define LVI              56
#define LVII             57
#define LVIII            58
#define LIX              59
#define LX               60
#define LXI              61
#define LXII             62
#define LXIII            63
#define LXIV             64
#define LXV              65
#define LXVI             66
#define LXVII            67
#define LXVIII           68
#define LXIX             69
#define LXX              70
#define LXXI             71
#define LXXII            72
#define LXXIII           73
#define LXXIV            74
#define LXXV             75
#define LXXVI            76
#define LXXVII           77
#define LXXVIII          78
#define LXXIX            79
#define LXXX             80
#define LXXXI            81
#define LXXXII           82
#define LXXXIII          83
#define LXXXIV           84
#define LXXXV            85
#define LXXXVI           86
#define LXXXVII          87
#define LXXXVIII         88
#define LXXXIX           89
#define XC               90
#define XCI              91
#define XCII             92
#define XCIII            93
#define XCIV             94
#define XCV              95
#define XCVI             96
#define XCVII            97
#define XCVIII           98
#define XCIX             99
#define C                100
#define CI               101
#define CII              102
#define CIII             103
#define CIV              104
#define CV               105
#define CVI              106
#define CVII             107
#define CVIII            108
#define CIX              109
#define CX               110
#define CXI              111
#define CXII             112
#define CXIII            113
#define CXIV             114
#define CXV              115
#define CXVI             116
#define CXVII            117
#define CXVIII           118
#define CXIX             119
#define CXX              120
#define CXXI             121
#define CXXII            122
#define CXXIII           123
#define CXXIV            124
#define CXXV             125
#define CXXVI            126
#define CXXVII           127
#define CXXVIII          128
#define CXXIX            129
#define CXXX             130
#define CXXXI            131
#define CXXXII           132
#define CXXXIII          133
#define CXXXIV           134
#define CXXXV            135
#define CXXXVI           136
#define CXXXVII          137
#define CXXXVIII         138
#define CXXXIX           139
#define CXL              140
#define CXLI             141
#define CXLII            142
#define CXLIII           143
#define CXLIV            144
#define CXLV             145
#define CXLVI            146
#define CXLVII           147
#define CXLVIII          148
#define CXLIX            149
#define CL               150
#define CLI              151
#define CLII             152
#define CLIII            153
#define CLIV             154
#define CLV              155
#define CLVI             156
#define CLVII            157
#define CLVIII           158
#define CLIX             159
#define CLX              160
#define CLXI             161
#define CLXII            162
#define CLXIII           163
#define CLXIV            164
#define CLXV             165
#define CLXVI            166
#define CLXVII           167
#define CLXVIII          168
#define CLXIX            169
#define CLXX             170
#define CLXXI            171
#define CLXXII           172
#define CLXXIII          173
#define CLXXIV           174
#define CLXXV            175
#define CLXXVI           176
#define CLXXVII          177
#define CLXXVIII         178
#define CLXXIX           179
#define CLXXX            180
#define CLXXXI           181
#define CLXXXII          182
#define CLXXXIII         183
#define CLXXXIV          184
#define CLXXXV           185
#define CLXXXVI          186
#define CLXXXVII         187
#define CLXXXVIII        188
#define CLXXXIX          189
#define CXC              190
#define CXCI             191
#define CXCII            192
#define CXCIII           193
#define CXCIV            194
#define CXCV             195
#define CXCVI            196
#define CXCVII           197
#define CXCVIII          198
#define CXCIX            199
#define CC               200
#define CCI              201
#define CCII             202
#define CCIII            203
#define CCIV             204
#define CCV              205
#define CCVI             206
#define CCVII            207
#define CCVIII           208
#define CCIX             209
#define CCX              210
#define CCXI             211
#define CCXII            212
#define CCXIII           213
#define CCXIV            214
#define CCXV             215
#define CCXVI            216
#define CCXVII           217
#define CCXVIII          218
#define CCXIX            219
#define CCXX             220
#define CCXXI            221
#define CCXXII           222
#define CCXXIII          223
#define CCXXIV           224
#define CCXXV            225
#define CCXXVI           226
#define CCXXVII          227
#define CCXXVIII         228
#define CCXXIX           229
#define CCXXX            230
#define CCXXXI           231
#define CCXXXII          232
#define CCXXXIII         233
#define CCXXXIV          234
#define CCXXXV           235
#define CCXXXVI          236
#define CCXXXVII         237
#define CCXXXVIII        238
#define CCXXXIX          239
#define CCXL             240
#define CCXLI            241
#define CCXLII           242
#define CCXLIII          243
#define CCXLIV           244
#define CCXLV            245
#define CCXLVI           246
#define CCXLVII          247
#define CCXLVIII         248
#define CCXLIX           249
#define CCL              250
#define CCLI             251
#define CCLII            252
#define CCLIII           253
#define CCLIV            254
#define CCLV             255
#define CCLVI            256
#define CCLVII           257
#define CCLVIII          258
#define CCLIX            259
#define CCLX             260
#define CCLXI            261
#define CCLXII           262
#define CCLXIII          263
#define CCLXIV           264
#define CCLXV            265
#define CCLXVI           266
#define CCLXVII          267
#define CCLXVIII         268
#define CCLXIX           269
#define CCLXX            270
#define CCLXXI           271
#define CCLXXII          272
#define CCLXXIII         273
#define CCLXXIV          274
#define CCLXXV           275
#define CCLXXVI          276
#define CCLXXVII         277
#define CCLXXVIII        278
#define CCLXXIX          279
#define CCLXXX           280
#define CCLXXXI          281
#define CCLXXXII         282
#define CCLXXXIII        283
#define CCLXXXIV         284
#define CCLXXXV          285
#define CCLXXXVI         286
#define CCLXXXVII        287
#define CCLXXXVIII       288
#define CCLXXXIX         289
#define CCXC             290
#define CCXCI            291
#define CCXCII           292
#define CCXCIII          293
#define CCXCIV           294
#define CCXCV            295
#define CCXCVI           296
#define CCXCVII          297
#define CCXCVIII         298
#define CCXCIX           299
#define CCC              300
#define CCCI             301
#define CCCII            302
#define CCCIII           303
#define CCCIV            304
#define CCCV             305
#define CCCVI            306
#define CCCVII           307
#define CCCVIII          308
#define CCCIX            309
#define CCCX             310
#define CCCXI            311
#define CCCXII           312
#define CCCXIII          313
#define CCCXIV           314
#define CCCXV            315
#define CCCXVI           316
#define CCCXVII          317
#define CCCXVIII         318
#define CCCXIX           319
#define CCCXX            320
#define CCCXXI           321
#define CCCXXII          322
#define CCCXXIII         323
#define CCCXXIV          324
#define CCCXXV           325
#define CCCXXVI          326
#define CCCXXVII         327
#define CCCXXVIII        328
#define CCCXXIX          329
#define CCCXXX           330
#define CCCXXXI          331
#define CCCXXXII         332
#define CCCXXXIII        333
#define CCCXXXIV         334
#define CCCXXXV          335
#define CCCXXXVI         336
#define CCCXXXVII        337
#define CCCXXXVIII       338
#define CCCXXXIX         339
#define CCCXL            340
#define CCCXLI           341
#define CCCXLII          342
#define CCCXLIII         343
#define CCCXLIV          344
#define CCCXLV           345
#define CCCXLVI          346
#define CCCXLVII         347
#define CCCXLVIII        348
#define CCCXLIX          349
#define CCCL             350
#define CCCLI            351
#define CCCLII           352
#define CCCLIII          353
#define CCCLIV           354
#define CCCLV            355
#define CCCLVI           356
#define CCCLVII          357
#define CCCLVIII         358
#define CCCLIX           359
#define CCCLX            360
#define CCCLXI           361
#define CCCLXII          362
#define CCCLXIII         363
#define CCCLXIV          364
#define CCCLXV           365
#define CCCLXVI          366
#define CCCLXVII         367
#define CCCLXVIII        368
#define CCCLXIX          369
#define CCCLXX           370
#define CCCLXXI          371
#define CCCLXXII         372
#define CCCLXXIII        373
#define CCCLXXIV         374
#define CCCLXXV          375
#define CCCLXXVI         376
#define CCCLXXVII        377
#define CCCLXXVIII       378
#define CCCLXXIX         379
#define CCCLXXX          380
#define CCCLXXXI         381
#define CCCLXXXII        382
#define CCCLXXXIII       383
#define CCCLXXXIV        384
#define CCCLXXXV         385
#define CCCLXXXVI        386
#define CCCLXXXVII       387
#define CCCLXXXVIII      388
#define CCCLXXXIX        389
#define CCCXC            390
#define CCCXCI           391
#define CCCXCII          392
#define CCCXCIII         393
#define CCCXCIV          394
#define CCCXCV           395
#define CCCXCVI          396
#define CCCXCVII         397
#define CCCXCVIII        398
#define CCCXCIX          399
#define CD               400
#define CDI              401
#define CDII             402
#define CDIII            403
#define CDIV             404
#define CDV              405
#define CDVI             406
#define CDVII            407
#define CDVIII           408
#define CDIX             409
#define CDX              410
#define CDXI             411
#define CDXII            412
#define CDXIII           413
#define CDXIV            414
#define CDXV             415
#define CDXVI            416
#define CDXVII           417
#define CDXVIII          418
#define CDXIX            419
#define CDXX             420
#define CDXXI            421
#define CDXXII           422
#define CDXXIII          423
#define CDXXIV           424
#define CDXXV            425
#define CDXXVI           426
#define CDXXVII          427
#define CDXXVIII         428
#define CDXXIX           429
#define CDXXX            430
#define CDXXXI           431
#define CDXXXII          432
#define CDXXXIII         433
#define CDXXXIV          434
#define CDXXXV           435
#define CDXXXVI          436
#define CDXXXVII         437
#define CDXXXVIII        438
#define CDXXXIX          439
#define CDXL             440
#define CDXLI            441
#define CDXLII           442
#define CDXLIII          443
#define CDXLIV           444
#define CDXLV            445
#define CDXLVI           446
#define CDXLVII          447
#define CDXLVIII         448
#define CDXLIX           449
#define CDL              450
#define CDLI             451
#define CDLII            452
#define CDLIII           453
#define CDLIV            454
#define CDLV             455
#define CDLVI            456
#define CDLVII           457
#define CDLVIII          458
#define CDLIX            459
#define CDLX             460
#define CDLXI            461
#define CDLXII           462
#define CDLXIII          463
#define CDLXIV           464
#define CDLXV            465
#define CDLXVI           466
#define CDLXVII          467
#define CDLXVIII         468
#define CDLXIX           469
#define CDLXX            470
#define CDLXXI           471
#define CDLXXII          472
#define CDLXXIII         473
#define CDLXXIV          474
#define CDLXXV           475
#define CDLXXVI          476
#define CDLXXVII         477
#define CDLXXVIII        478
#define CDLXXIX          479
#define CDLXXX           480
#define CDLXXXI          481
#define CDLXXXII         482
#define CDLXXXIII        483
#define CDLXXXIV         484
#define CDLXXXV          485
#define CDLXXXVI         486
#define CDLXXXVII        487
#define CDLXXXVIII       488
#define CDLXXXIX         489
#define CDXC             490
#define CDXCI            491
#define CDXCII           492
#define CDXCIII          493
#define CDXCIV           494
#define CDXCV            495
#define CDXCVI           496
#define CDXCVII          497
#define CDXCVIII         498
#define CDXCIX           499
#define D                500
#define DI               501
#define DII              502
#define DIII             503
#define DIV              504
#define DV               505
#define DVI              506
#define DVII             507
#define DVIII            508
#define DIX              509
#define DX               510
#define DXI              511
#define DXII             512
#define DXIII            513
#define DXIV             514
#define DXV              515
#define DXVI             516
#define DXVII            517
#define DXVIII           518
#define DXIX             519
#define DXX              520
#define DXXI             521
#define DXXII            522
#define DXXIII           523
#define DXXIV            524
#define DXXV             525
#define DXXVI            526
#define DXXVII           527
#define DXXVIII          528
#define DXXIX            529
#define DXXX             530
#define DXXXI            531
#define DXXXII           532
#define DXXXIII          533
#define DXXXIV           534
#define DXXXV            535
#define DXXXVI           536
#define DXXXVII          537
#define DXXXVIII         538
#define DXXXIX           539
#define DXL              540
#define DXLI             541
#define DXLII            542
#define DXLIII           543
#define DXLIV            544
#define DXLV             545
#define DXLVI            546
#define DXLVII           547
#define DXLVIII          548
#define DXLIX            549
#define DL               550
#define DLI              551
#define DLII             552
#define DLIII            553
#define DLIV             554
#define DLV              555
#define DLVI             556
#define DLVII            557
#define DLVIII           558
#define DLIX             559
#define DLX              560
#define DLXI             561
#define DLXII            562
#define DLXIII           563
#define DLXIV            564
#define DLXV             565
#define DLXVI            566
#define DLXVII           567
#define DLXVIII          568
#define DLXIX            569
#define DLXX             570
#define DLXXI            571
#define DLXXII           572
#define DLXXIII          573
#define DLXXIV           574
#define DLXXV            575
#define DLXXVI           576
#define DLXXVII          577
#define DLXXVIII         578
#define DLXXIX           579
#define DLXXX            580
#define DLXXXI           581
#define DLXXXII          582
#define DLXXXIII         583
#define DLXXXIV          584
#define DLXXXV           585
#define DLXXXVI          586
#define DLXXXVII         587
#define DLXXXVIII        588
#define DLXXXIX          589
#define DXC              590
#define DXCI             591
#define DXCII            592
#define DXCIII           593
#define DXCIV            594
#define DXCV             595
#define DXCVI            596
#define DXCVII           597
#define DXCVIII          598
#define DXCIX            599
#define DC               600
#define DCI              601
#define DCII             602
#define DCIII            603
#define DCIV             604
#define DCV              605
#define DCVI             606
#define DCVII            607
#define DCVIII           608
#define DCIX             609
#define DCX              610
#define DCXI             611
#define DCXII            612
#define DCXIII           613
#define DCXIV            614
#define DCXV             615
#define DCXVI            616
#define DCXVII           617
#define DCXVIII          618
#define DCXIX            619
#define DCXX             620
#define DCXXI            621
#define DCXXII           622
#define DCXXIII          623
#define DCXXIV           624
#define DCXXV            625
#define DCXXVI           626
#define DCXXVII          627
#define DCXXVIII         628
#define DCXXIX           629
#define DCXXX            630
#define DCXXXI           631
#define DCXXXII          632
#define DCXXXIII         633
#define DCXXXIV          634
#define DCXXXV           635
#define DCXXXVI          636
#define DCXXXVII         637
#define DCXXXVIII        638
#define DCXXXIX          639
#define DCXL             640
#define DCXLI            641
#define DCXLII           642
#define DCXLIII          643
#define DCXLIV           644
#define DCXLV            645
#define DCXLVI           646
#define DCXLVII          647
#define DCXLVIII         648
#define DCXLIX           649
#define DCL              650
#define DCLI             651
#define DCLII            652
#define DCLIII           653
#define DCLIV            654
#define DCLV             655
#define DCLVI            656
#define DCLVII           657
#define DCLVIII          658
#define DCLIX            659
#define DCLX             660
#define DCLXI            661
#define DCLXII           662
#define DCLXIII          663
#define DCLXIV           664
#define DCLXV            665
#define DCLXVI           666
#define DCLXVII          667
#define DCLXVIII         668
#define DCLXIX           669
#define DCLXX            670
#define DCLXXI           671
#define DCLXXII          672
#define DCLXXIII         673
#define DCLXXIV          674
#define DCLXXV           675
#define DCLXXVI          676
#define DCLXXVII         677
#define DCLXXVIII        678
#define DCLXXIX          679
#define DCLXXX           680
#define DCLXXXI          681
#define DCLXXXII         682
#define DCLXXXIII        683
#define DCLXXXIV         684
#define DCLXXXV          685
#define DCLXXXVI         686
#define DCLXXXVII        687
#define DCLXXXVIII       688
#define DCLXXXIX         689
#define DCXC             690
#define DCXCI            691
#define DCXCII           692
#define DCXCIII          693
#define DCXCIV           694
#define DCXCV            695
#define DCXCVI           696
#define DCXCVII          697
#define DCXCVIII         698
#define DCXCIX           699
#define DCC              700
#define DCCI             701
#define DCCII            702
#define DCCIII           703
#define DCCIV            704
#define DCCV             705
#define DCCVI            706
#define DCCVII           707
#define DCCVIII          708
#define DCCIX            709
#define DCCX             710
#define DCCXI            711
#define DCCXII           712
#define DCCXIII          713
#define DCCXIV           714
#define DCCXV            715
#define DCCXVI           716
#define DCCXVII          717
#define DCCXVIII         718
#define DCCXIX           719
#define DCCXX            720
#define DCCXXI           721
#define DCCXXII          722
#define DCCXXIII         723
#define DCCXXIV          724
#define DCCXXV           725
#define DCCXXVI          726
#define DCCXXVII         727
#define DCCXXVIII        728
#define DCCXXIX          729
#define DCCXXX           730
#define DCCXXXI          731
#define DCCXXXII         732
#define DCCXXXIII        733
#define DCCXXXIV         734
#define DCCXXXV          735
#define DCCXXXVI         736
#define DCCXXXVII        737
#define DCCXXXVIII       738
#define DCCXXXIX         739
#define DCCXL            740
#define DCCXLI           741
#define DCCXLII          742
#define DCCXLIII         743
#define DCCXLIV          744
#define DCCXLV           745
#define DCCXLVI          746
#define DCCXLVII         747
#define DCCXLVIII        748
#define DCCXLIX          749
#define DCCL             750
#define DCCLI            751
#define DCCLII           752
#define DCCLIII          753
#define DCCLIV           754
#define DCCLV            755
#define DCCLVI           756
#define DCCLVII          757
#define DCCLVIII         758
#define DCCLIX           759
#define DCCLX            760
#define DCCLXI           761
#define DCCLXII          762
#define DCCLXIII         763
#define DCCLXIV          764
#define DCCLXV           765
#define DCCLXVI          766
#define DCCLXVII         767
#define DCCLXVIII        768
#define DCCLXIX          769
#define DCCLXX           770
#define DCCLXXI          771
#define DCCLXXII         772
#define DCCLXXIII        773
#define DCCLXXIV         774
#define DCCLXXV          775
#define DCCLXXVI         776
#define DCCLXXVII        777
#define DCCLXXVIII       778
#define DCCLXXIX         779
#define DCCLXXX          780
#define DCCLXXXI         781
#define DCCLXXXII        782
#define DCCLXXXIII       783
#define DCCLXXXIV        784
#define DCCLXXXV         785
#define DCCLXXXVI        786
#define DCCLXXXVII       787
#define DCCLXXXVIII      788
#define DCCLXXXIX        789
#define DCCXC            790
#define DCCXCI           791
#define DCCXCII          792
#define DCCXCIII         793
#define DCCXCIV          794
#define DCCXCV           795
#define DCCXCVI          796
#define DCCXCVII         797
#define DCCXCVIII        798
#define DCCXCIX          799
#define DCCC             800
#define DCCCI            801
#define DCCCII           802
#define DCCCIII          803
#define DCCCIV           804
#define DCCCV            805
#define DCCCVI           806
#define DCCCVII          807
#define DCCCVIII         808
#define DCCCIX           809
#define DCCCX            810
#define DCCCXI           811
#define DCCCXII          812
#define DCCCXIII         813
#define DCCCXIV          814
#define DCCCXV           815
#define DCCCXVI          816
#define DCCCXVII         817
#define DCCCXVIII        818
#define DCCCXIX          819
#define DCCCXX           820
#define DCCCXXI          821
#define DCCCXXII         822
#define DCCCXXIII        823
#define DCCCXXIV         824
#define DCCCXXV          825
#define DCCCXXVI         826
#define DCCCXXVII        827
#define DCCCXXVIII       828
#define DCCCXXIX         829
#define DCCCXXX          830
#define DCCCXXXI         831
#define DCCCXXXII        832
#define DCCCXXXIII       833
#define DCCCXXXIV        834
#define DCCCXXXV         835
#define DCCCXXXVI        836
#define DCCCXXXVII       837
#define DCCCXXXVIII      838
#define DCCCXXXIX        839
#define DCCCXL           840
#define DCCCXLI          841
#define DCCCXLII         842
#define DCCCXLIII        843
#define DCCCXLIV         844
#define DCCCXLV          845
#define DCCCXLVI         846
#define DCCCXLVII        847
#define DCCCXLVIII       848
#define DCCCXLIX         849
#define DCCCL            850
#define DCCCLI           851
#define DCCCLII          852
#define DCCCLIII         853
#define DCCCLIV          854
#define DCCCLV           855
#define DCCCLVI          856
#define DCCCLVII         857
#define DCCCLVIII        858
#define DCCCLIX          859
#define DCCCLX           860
#define DCCCLXI          861
#define DCCCLXII         862
#define DCCCLXIII        863
#define DCCCLXIV         864
#define DCCCLXV          865
#define DCCCLXVI         866
#define DCCCLXVII        867
#define DCCCLXVIII       868
#define DCCCLXIX         869
#define DCCCLXX          870
#define DCCCLXXI         871
#define DCCCLXXII        872
#define DCCCLXXIII       873
#define DCCCLXXIV        874
#define DCCCLXXV         875
#define DCCCLXXVI        876
#define DCCCLXXVII       877
#define DCCCLXXVIII      878
#define DCCCLXXIX        879
#define DCCCLXXX         880
#define DCCCLXXXI        881
#define DCCCLXXXII       882
#define DCCCLXXXIII      883
#define DCCCLXXXIV       884
#define DCCCLXXXV        885
#define DCCCLXXXVI       886
#define DCCCLXXXVII      887
#define DCCCLXXXVIII     888
#define DCCCLXXXIX       889
#define DCCCXC           890
#define DCCCXCI          891
#define DCCCXCII         892
#define DCCCXCIII        893
#define DCCCXCIV         894
#define DCCCXCV          895
#define DCCCXCVI         896
#define DCCCXCVII        897
#define DCCCXCVIII       898
#define DCCCXCIX         899
#define CM               900
#define CMI              901
#define CMII             902
#define CMIII            903
#define CMIV             904
#define CMV              905
#define CMVI             906
#define CMVII            907
#define CMVIII           908
#define CMIX             909
#define CMX              910
#define CMXI             911
#define CMXII            912
#define CMXIII           913
#define CMXIV            914
#define CMXV             915
#define CMXVI            916
#define CMXVII           917
#define CMXVIII          918
#define CMXIX            919
#define CMXX             920
#define CMXXI            921
#define CMXXII           922
#define CMXXIII          923
#define CMXXIV           924
#define CMXXV            925
#define CMXXVI           926
#define CMXXVII          927
#define CMXXVIII         928
#define CMXXIX           929
#define CMXXX            930
#define CMXXXI           931
#define CMXXXII          932
#define CMXXXIII         933
#define CMXXXIV          934
#define CMXXXV           935
#define CMXXXVI          936
#define CMXXXVII         937
#define CMXXXVIII        938
#define CMXXXIX          939
#define CMXL             940
#define CMXLI            941
#define CMXLII           942
#define CMXLIII          943
#define CMXLIV           944
#define CMXLV            945
#define CMXLVI           946
#define CMXLVII          947
#define CMXLVIII         948
#define CMXLIX           949
#define CML              950
#define CMLI             951
#define CMLII            952
#define CMLIII           953
#define CMLIV            954
#define CMLV             955
#define CMLVI            956
#define CMLVII           957
#define CMLVIII          958
#define CMLIX            959
#define CMLX             960
#define CMLXI            961
#define CMLXII           962
#define CMLXIII          963
#define CMLXIV           964
#define CMLXV            965
#define CMLXVI           966
#define CMLXVII          967
#define CMLXVIII         968
#define CMLXIX           969
#define CMLXX            970
#define CMLXXI           971
#define CMLXXII          972
#define CMLXXIII         973
#define CMLXXIV          974
#define CMLXXV           975
#define CMLXXVI          976
#define CMLXXVII         977
#define CMLXXVIII        978
#define CMLXXIX          979
#define CMLXXX           980
#define CMLXXXI          981
#define CMLXXXII         982
#define CMLXXXIII        983
#define CMLXXXIV         984
#define CMLXXXV          985
#define CMLXXXVI         986
#define CMLXXXVII        987
#define CMLXXXVIII       988
#define CMLXXXIX         989
#define CMXC             990
#define CMXCI            991
#define CMXCII           992
#define CMXCIII          993
#define CMXCIV           994
#define CMXCV            995
#define CMXCVI           996
#define CMXCVII          997
#define CMXCVIII         998
#define CMXCIX           999
#define M                1000
#define MI               1001
#define MII              1002
#define MIII             1003
#define MIV              1004
#define MV               1005
#define MVI              1006
#define MVII             1007
#define MVIII            1008
#define MIX              1009
#define MX               1010
#define MXI              1011
#define MXII             1012
#define MXIII            1013
#define MXIV             1014
#define MXV              1015
#define MXVI             1016
#define MXVII            1017
#define MXVIII           1018
#define MXIX             1019
#define MXX              1020
#define MXXI             1021
#define MXXII            1022
#define MXXIII           1023
#define MXXIV            1024
#define MXXV             1025
#define MXXVI            1026
#define MXXVII           1027
#define MXXVIII          1028
#define MXXIX            1029
#define MXXX             1030
#define MXXXI            1031
#define MXXXII           1032
#define MXXXIII          1033
#define MXXXIV           1034
#define MXXXV            1035
#define MXXXVI           1036
#define MXXXVII          1037
#define MXXXVIII         1038
#define MXXXIX           1039
#define MXL              1040
#define MXLI             1041
#define MXLII            1042
#define MXLIII           1043
#define MXLIV            1044
#define MXLV             1045
#define MXLVI            1046
#define MXLVII           1047
#define MXLVIII          1048
#define MXLIX            1049
#define ML               1050
#define MLI              1051
#define MLII             1052
#define MLIII            1053
#define MLIV             1054
#define MLV              1055
#define MLVI             1056
#define MLVII            1057
#define MLVIII           1058
#define MLIX             1059
#define MLX              1060
#define MLXI             1061
#define MLXII            1062
#define MLXIII           1063
#define MLXIV            1064
#define MLXV             1065
#define MLXVI            1066
#define MLXVII           1067
#define MLXVIII          1068
#define MLXIX            1069
#define MLXX             1070
#define MLXXI            1071
#define MLXXII           1072
#define MLXXIII          1073
#define MLXXIV           1074
#define MLXXV            1075
#define MLXXVI           1076
#define MLXXVII          1077
#define MLXXVIII         1078
#define MLXXIX           1079
#define MLXXX            1080
#define MLXXXI           1081
#define MLXXXII          1082
#define MLXXXIII         1083
#define MLXXXIV          1084
#define MLXXXV           1085
#define MLXXXVI          1086
#define MLXXXVII         1087
#define MLXXXVIII        1088
#define MLXXXIX          1089
#define MXC              1090
#define MXCI             1091
#define MXCII            1092
#define MXCIII           1093
#define MXCIV            1094
#define MXCV             1095
#define MXCVI            1096
#define MXCVII           1097
#define MXCVIII          1098
#define MXCIX            1099
#define MC               1100
#define MCI              1101
#define MCII             1102
#define MCIII            1103
#define MCIV             1104
#define MCV              1105
#define MCVI             1106
#define MCVII            1107
#define MCVIII           1108
#define MCIX             1109
#define MCX              1110
#define MCXI             1111
#define MCXII            1112
#define MCXIII           1113
#define MCXIV            1114
#define MCXV             1115
#define MCXVI            1116
#define MCXVII           1117
#define MCXVIII          1118
#define MCXIX            1119
#define MCXX             1120
#define MCXXI            1121
#define MCXXII           1122
#define MCXXIII          1123
#define MCXXIV           1124
#define MCXXV            1125
#define MCXXVI           1126
#define MCXXVII          1127
#define MCXXVIII         1128
#define MCXXIX           1129
#define MCXXX            1130
#define MCXXXI           1131
#define MCXXXII          1132
#define MCXXXIII         1133
#define MCXXXIV          1134
#define MCXXXV           1135
#define MCXXXVI          1136
#define MCXXXVII         1137
#define MCXXXVIII        1138
#define MCXXXIX          1139
#define MCXL             1140
#define MCXLI            1141
#define MCXLII           1142
#define MCXLIII          1143
#define MCXLIV           1144
#define MCXLV            1145
#define MCXLVI           1146
#define MCXLVII          1147
#define MCXLVIII         1148
#define MCXLIX           1149
#define MCL              1150
#define MCLI             1151
#define MCLII            1152
#define MCLIII           1153
#define MCLIV            1154
#define MCLV             1155
#define MCLVI            1156
#define MCLVII           1157
#define MCLVIII          1158
#define MCLIX            1159
#define MCLX             1160
#define MCLXI            1161
#define MCLXII           1162
#define MCLXIII          1163
#define MCLXIV           1164
#define MCLXV            1165
#define MCLXVI           1166
#define MCLXVII          1167
#define MCLXVIII         1168
#define MCLXIX           1169
#define MCLXX            1170
#define MCLXXI           1171
#define MCLXXII          1172
#define MCLXXIII         1173
#define MCLXXIV          1174
#define MCLXXV           1175
#define MCLXXVI          1176
#define MCLXXVII         1177
#define MCLXXVIII        1178
#define MCLXXIX          1179
#define MCLXXX           1180
#define MCLXXXI          1181
#define MCLXXXII         1182
#define MCLXXXIII        1183
#define MCLXXXIV         1184
#define MCLXXXV          1185
#define MCLXXXVI         1186
#define MCLXXXVII        1187
#define MCLXXXVIII       1188
#define MCLXXXIX         1189
#define MCXC             1190
#define MCXCI            1191
#define MCXCII           1192
#define MCXCIII          1193
#define MCXCIV           1194
#define MCXCV            1195
#define MCXCVI           1196
#define MCXCVII          1197
#define MCXCVIII         1198
#define MCXCIX           1199
#define MCC              1200
#define MCCI             1201
#define MCCII            1202
#define MCCIII           1203
#define MCCIV            1204
#define MCCV             1205
#define MCCVI            1206
#define MCCVII           1207
#define MCCVIII          1208
#define MCCIX            1209
#define MCCX             1210
#define MCCXI            1211
#define MCCXII           1212
#define MCCXIII          1213
#define MCCXIV           1214
#define MCCXV            1215
#define MCCXVI           1216
#define MCCXVII          1217
#define MCCXVIII         1218
#define MCCXIX           1219
#define MCCXX            1220
#define MCCXXI           1221
#define MCCXXII          1222
#define MCCXXIII         1223
#define MCCXXIV          1224
#define MCCXXV           1225
#define MCCXXVI          1226
#define MCCXXVII         1227
#define MCCXXVIII        1228
#define MCCXXIX          1229
#define MCCXXX           1230
#define MCCXXXI          1231
#define MCCXXXII         1232
#define MCCXXXIII        1233
#define MCCXXXIV         1234
#define MCCXXXV          1235
#define MCCXXXVI         1236
#define MCCXXXVII        1237
#define MCCXXXVIII       1238
#define MCCXXXIX         1239
#define MCCXL            1240
#define MCCXLI           1241
#define MCCXLII          1242
#define MCCXLIII         1243
#define MCCXLIV          1244
#define MCCXLV           1245
#define MCCXLVI          1246
#define MCCXLVII         1247
#define MCCXLVIII        1248
#define MCCXLIX          1249
#define MCCL             1250
#define MCCLI            1251
#define MCCLII           1252
#define MCCLIII          1253
#define MCCLIV           1254
#define MCCLV            1255
#define MCCLVI           1256
#define MCCLVII          1257
#define MCCLVIII         1258
#define MCCLIX           1259
#define MCCLX            1260
#define MCCLXI           1261
#define MCCLXII          1262
#define MCCLXIII         1263
#define MCCLXIV          1264
#define MCCLXV           1265
#define MCCLXVI          1266
#define MCCLXVII         1267
#define MCCLXVIII        1268
#define MCCLXIX          1269
#define MCCLXX           1270
#define MCCLXXI          1271
#define MCCLXXII         1272
#define MCCLXXIII        1273
#define MCCLXXIV         1274
#define MCCLXXV          1275
#define MCCLXXVI         1276
#define MCCLXXVII        1277
#define MCCLXXVIII       1278
#define MCCLXXIX         1279
#define MCCLXXX          1280
#define MCCLXXXI         1281
#define MCCLXXXII        1282
#define MCCLXXXIII       1283
#define MCCLXXXIV        1284
#define MCCLXXXV         1285
#define MCCLXXXVI        1286
#define MCCLXXXVII       1287
#define MCCLXXXVIII      1288
#define MCCLXXXIX        1289
#define MCCXC            1290
#define MCCXCI           1291
#define MCCXCII          1292
#define MCCXCIII         1293
#define MCCXCIV          1294
#define MCCXCV           1295
#define MCCXCVI          1296
#define MCCXCVII         1297
#define MCCXCVIII        1298
#define MCCXCIX          1299
#define MCCC             1300
#define MCCCI            1301
#define MCCCII           1302
#define MCCCIII          1303
#define MCCCIV           1304
#define MCCCV            1305
#define MCCCVI           1306
#define MCCCVII          1307
#define MCCCVIII         1308
#define MCCCIX           1309
#define MCCCX            1310
#define MCCCXI           1311
#define MCCCXII          1312
#define MCCCXIII         1313
#define MCCCXIV          1314
#define MCCCXV           1315
#define MCCCXVI          1316
#define MCCCXVII         1317
#define MCCCXVIII        1318
#define MCCCXIX          1319
#define MCCCXX           1320
#define MCCCXXI          1321
#define MCCCXXII         1322
#define MCCCXXIII        1323
#define MCCCXXIV         1324
#define MCCCXXV          1325
#define MCCCXXVI         1326
#define MCCCXXVII        1327
#define MCCCXXVIII       1328
#define MCCCXXIX         1329
#define MCCCXXX          1330
#define MCCCXXXI         1331
#define MCCCXXXII        1332
#define MCCCXXXIII       1333
#define MCCCXXXIV        1334
#define MCCCXXXV         1335
#define MCCCXXXVI        1336
#define MCCCXXXVII       1337
#define MCCCXXXVIII      1338
#define MCCCXXXIX        1339
#define MCCCXL           1340
#define MCCCXLI          1341
#define MCCCXLII         1342
#define MCCCXLIII        1343
#define MCCCXLIV         1344
#define MCCCXLV          1345
#define MCCCXLVI         1346
#define MCCCXLVII        1347
#define MCCCXLVIII       1348
#define MCCCXLIX         1349
#define MCCCL            1350
#define MCCCLI           1351
#define MCCCLII          1352
#define MCCCLIII         1353
#define MCCCLIV          1354
#define MCCCLV           1355
#define MCCCLVI          1356
#define MCCCLVII         1357
#define MCCCLVIII        1358
#define MCCCLIX          1359
#define MCCCLX           1360
#define MCCCLXI          1361
#define MCCCLXII         1362
#define MCCCLXIII        1363
#define MCCCLXIV         1364
#define MCCCLXV          1365
#define MCCCLXVI         1366
#define MCCCLXVII        1367
#define MCCCLXVIII       1368
#define MCCCLXIX         1369
#define MCCCLXX          1370
#define MCCCLXXI         1371
#define MCCCLXXII        1372
#define MCCCLXXIII       1373
#define MCCCLXXIV        1374
#define MCCCLXXV         1375
#define MCCCLXXVI        1376
#define MCCCLXXVII       1377
#define MCCCLXXVIII      1378
#define MCCCLXXIX        1379
#define MCCCLXXX         1380
#define MCCCLXXXI        1381
#define MCCCLXXXII       1382
#define MCCCLXXXIII      1383
#define MCCCLXXXIV       1384
#define MCCCLXXXV        1385
#define MCCCLXXXVI       1386
#define MCCCLXXXVII      1387
#define MCCCLXXXVIII     1388
#define MCCCLXXXIX       1389
#define MCCCXC           1390
#define MCCCXCI          1391
#define MCCCXCII         1392
#define MCCCXCIII        1393
#define MCCCXCIV         1394
#define MCCCXCV          1395
#define MCCCXCVI         1396
#define MCCCXCVII        1397
#define MCCCXCVIII       1398
#define MCCCXCIX         1399
#define MCD              1400
#define MCDI             1401
#define MCDII            1402
#define MCDIII           1403
#define MCDIV            1404
#define MCDV             1405
#define MCDVI            1406
#define MCDVII           1407
#define MCDVIII          1408
#define MCDIX            1409
#define MCDX             1410
#define MCDXI            1411
#define MCDXII           1412
#define MCDXIII          1413
#define MCDXIV           1414
#define MCDXV            1415
#define MCDXVI           1416
#define MCDXVII          1417
#define MCDXVIII         1418
#define MCDXIX           1419
#define MCDXX            1420
#define MCDXXI           1421
#define MCDXXII          1422
#define MCDXXIII         1423
#define MCDXXIV          1424
#define MCDXXV           1425
#define MCDXXVI          1426
#define MCDXXVII         1427
#define MCDXXVIII        1428
#define MCDXXIX          1429
#define MCDXXX           1430
#define MCDXXXI          1431
#define MCDXXXII         1432
#define MCDXXXIII        1433
#define MCDXXXIV         1434
#define MCDXXXV          1435
#define MCDXXXVI         1436
#define MCDXXXVII        1437
#define MCDXXXVIII       1438
#define MCDXXXIX         1439
#define MCDXL            1440
#define MCDXLI           1441
#define MCDXLII          1442
#define MCDXLIII         1443
#define MCDXLIV          1444
#define MCDXLV           1445
#define MCDXLVI          1446
#define MCDXLVII         1447
#define MCDXLVIII        1448
#define MCDXLIX          1449
#define MCDL             1450
#define MCDLI            1451
#define MCDLII           1452
#define MCDLIII          1453
#define MCDLIV           1454
#define MCDLV            1455
#define MCDLVI           1456
#define MCDLVII          1457
#define MCDLVIII         1458
#define MCDLIX           1459
#define MCDLX            1460
#define MCDLXI           1461
#define MCDLXII          1462
#define MCDLXIII         1463
#define MCDLXIV          1464
#define MCDLXV           1465
#define MCDLXVI          1466
#define MCDLXVII         1467
#define MCDLXVIII        1468
#define MCDLXIX          1469
#define MCDLXX           1470
#define MCDLXXI          1471
#define MCDLXXII         1472
#define MCDLXXIII        1473
#define MCDLXXIV         1474
#define MCDLXXV          1475
#define MCDLXXVI         1476
#define MCDLXXVII        1477
#define MCDLXXVIII       1478
#define MCDLXXIX         1479
#define MCDLXXX          1480
#define MCDLXXXI         1481
#define MCDLXXXII        1482
#define MCDLXXXIII       1483
#define MCDLXXXIV        1484
#define MCDLXXXV         1485
#define MCDLXXXVI        1486
#define MCDLXXXVII       1487
#define MCDLXXXVIII      1488
#define MCDLXXXIX        1489
#define MCDXC            1490
#define MCDXCI           1491
#define MCDXCII          1492
#define MCDXCIII         1493
#define MCDXCIV          1494
#define MCDXCV           1495
#define MCDXCVI          1496
#define MCDXCVII         1497
#define MCDXCVIII        1498
#define MCDXCIX          1499
#define MD               1500
#define MDI              1501
#define MDII             1502
#define MDIII            1503
#define MDIV             1504
#define MDV              1505
#define MDVI             1506
#define MDVII            1507
#define MDVIII           1508
#define MDIX             1509
#define MDX              1510
#define MDXI             1511
#define MDXII            1512
#define MDXIII           1513
#define MDXIV            1514
#define MDXV             1515
#define MDXVI            1516
#define MDXVII           1517
#define MDXVIII          1518
#define MDXIX            1519
#define MDXX             1520
#define MDXXI            1521
#define MDXXII           1522
#define MDXXIII          1523
#define MDXXIV           1524
#define MDXXV            1525
#define MDXXVI           1526
#define MDXXVII          1527
#define MDXXVIII         1528
#define MDXXIX           1529
#define MDXXX            1530
#define MDXXXI           1531
#define MDXXXII          1532
#define MDXXXIII         1533
#define MDXXXIV          1534
#define MDXXXV           1535
#define MDXXXVI          1536
#define MDXXXVII         1537
#define MDXXXVIII        1538
#define MDXXXIX          1539
#define MDXL             1540
#define MDXLI            1541
#define MDXLII           1542
#define MDXLIII          1543
#define MDXLIV           1544
#define MDXLV            1545
#define MDXLVI           1546
#define MDXLVII          1547
#define MDXLVIII         1548
#define MDXLIX           1549
#define MDL              1550
#define MDLI             1551
#define MDLII            1552
#define MDLIII           1553
#define MDLIV            1554
#define MDLV             1555
#define MDLVI            1556
#define MDLVII           1557
#define MDLVIII          1558
#define MDLIX            1559
#define MDLX             1560
#define MDLXI            1561
#define MDLXII           1562
#define MDLXIII          1563
#define MDLXIV           1564
#define MDLXV            1565
#define MDLXVI           1566
#define MDLXVII          1567
#define MDLXVIII         1568
#define MDLXIX           1569
#define MDLXX            1570
#define MDLXXI           1571
#define MDLXXII          1572
#define MDLXXIII         1573
#define MDLXXIV          1574
#define MDLXXV           1575
#define MDLXXVI          1576
#define MDLXXVII         1577
#define MDLXXVIII        1578
#define MDLXXIX          1579
#define MDLXXX           1580
#define MDLXXXI          1581
#define MDLXXXII         1582
#define MDLXXXIII        1583
#define MDLXXXIV         1584
#define MDLXXXV          1585
#define MDLXXXVI         1586
#define MDLXXXVII        1587
#define MDLXXXVIII       1588
#define MDLXXXIX         1589
#define MDXC             1590
#define MDXCI            1591
#define MDXCII           1592
#define MDXCIII          1593
#define MDXCIV           1594
#define MDXCV            1595
#define MDXCVI           1596
#define MDXCVII          1597
#define MDXCVIII         1598
#define MDXCIX           1599
#define MDC              1600
#define MDCI             1601
#define MDCII            1602
#define MDCIII           1603
#define MDCIV            1604
#define MDCV             1605
#define MDCVI            1606
#define MDCVII           1607
#define MDCVIII          1608
#define MDCIX            1609
#define MDCX             1610
#define MDCXI            1611
#define MDCXII           1612
#define MDCXIII          1613
#define MDCXIV           1614
#define MDCXV            1615
#define MDCXVI           1616
#define MDCXVII          1617
#define MDCXVIII         1618
#define MDCXIX           1619
#define MDCXX            1620
#define MDCXXI           1621
#define MDCXXII          1622
#define MDCXXIII         1623
#define MDCXXIV          1624
#define MDCXXV           1625
#define MDCXXVI          1626
#define MDCXXVII         1627
#define MDCXXVIII        1628
#define MDCXXIX          1629
#define MDCXXX           1630
#define MDCXXXI          1631
#define MDCXXXII         1632
#define MDCXXXIII        1633
#define MDCXXXIV         1634
#define MDCXXXV          1635
#define MDCXXXVI         1636
#define MDCXXXVII        1637
#define MDCXXXVIII       1638
#define MDCXXXIX         1639
#define MDCXL            1640
#define MDCXLI           1641
#define MDCXLII          1642
#define MDCXLIII         1643
#define MDCXLIV          1644
#define MDCXLV           1645
#define MDCXLVI          1646
#define MDCXLVII         1647
#define MDCXLVIII        1648
#define MDCXLIX          1649
#define MDCL             1650
#define MDCLI            1651
#define MDCLII           1652
#define MDCLIII          1653
#define MDCLIV           1654
#define MDCLV            1655
#define MDCLVI           1656
#define MDCLVII          1657
#define MDCLVIII         1658
#define MDCLIX           1659
#define MDCLX            1660
#define MDCLXI           1661
#define MDCLXII          1662
#define MDCLXIII         1663
#define MDCLXIV          1664
#define MDCLXV           1665
#define MDCLXVI          1666
#define MDCLXVII         1667
#define MDCLXVIII        1668
#define MDCLXIX          1669
#define MDCLXX           1670
#define MDCLXXI          1671
#define MDCLXXII         1672
#define MDCLXXIII        1673
#define MDCLXXIV         1674
#define MDCLXXV          1675
#define MDCLXXVI         1676
#define MDCLXXVII        1677
#define MDCLXXVIII       1678
#define MDCLXXIX         1679
#define MDCLXXX          1680
#define MDCLXXXI         1681
#define MDCLXXXII        1682
#define MDCLXXXIII       1683
#define MDCLXXXIV        1684
#define MDCLXXXV         1685
#define MDCLXXXVI        1686
#define MDCLXXXVII       1687
#define MDCLXXXVIII      1688
#define MDCLXXXIX        1689
#define MDCXC            1690
#define MDCXCI           1691
#define MDCXCII          1692
#define MDCXCIII         1693
#define MDCXCIV          1694
#define MDCXCV           1695
#define MDCXCVI          1696
#define MDCXCVII         1697
#define MDCXCVIII        1698
#define MDCXCIX          1699
#define MDCC             1700
#define MDCCI            1701
#define MDCCII           1702
#define MDCCIII          1703
#define MDCCIV           1704
#define MDCCV            1705
#define MDCCVI           1706
#define MDCCVII          1707
#define MDCCVIII         1708
#define MDCCIX           1709
#define MDCCX            1710
#define MDCCXI           1711
#define MDCCXII          1712
#define MDCCXIII         1713
#define MDCCXIV          1714
#define MDCCXV           1715
#define MDCCXVI          1716
#define MDCCXVII         1717
#define MDCCXVIII        1718
#define MDCCXIX          1719
#define MDCCXX           1720
#define MDCCXXI          1721
#define MDCCXXII         1722
#define MDCCXXIII        1723
#define MDCCXXIV         1724
#define MDCCXXV          1725
#define MDCCXXVI         1726
#define MDCCXXVII        1727
#define MDCCXXVIII       1728
#define MDCCXXIX         1729
#define MDCCXXX          1730
#define MDCCXXXI         1731
#define MDCCXXXII        1732
#define MDCCXXXIII       1733
#define MDCCXXXIV        1734
#define MDCCXXXV         1735
#define MDCCXXXVI        1736
#define MDCCXXXVII       1737
#define MDCCXXXVIII      1738
#define MDCCXXXIX        1739
#define MDCCXL           1740
#define MDCCXLI          1741
#define MDCCXLII         1742
#define MDCCXLIII        1743
#define MDCCXLIV         1744
#define MDCCXLV          1745
#define MDCCXLVI         1746
#define MDCCXLVII        1747
#define MDCCXLVIII       1748
#define MDCCXLIX         1749
#define MDCCL            1750
#define MDCCLI           1751
#define MDCCLII          1752
#define MDCCLIII         1753
#define MDCCLIV          1754
#define MDCCLV           1755
#define MDCCLVI          1756
#define MDCCLVII         1757
#define MDCCLVIII        1758
#define MDCCLIX          1759
#define MDCCLX           1760
#define MDCCLXI          1761
#define MDCCLXII         1762
#define MDCCLXIII        1763
#define MDCCLXIV         1764
#define MDCCLXV          1765
#define MDCCLXVI         1766
#define MDCCLXVII        1767
#define MDCCLXVIII       1768
#define MDCCLXIX         1769
#define MDCCLXX          1770
#define MDCCLXXI         1771
#define MDCCLXXII        1772
#define MDCCLXXIII       1773
#define MDCCLXXIV        1774
#define MDCCLXXV         1775
#define MDCCLXXVI        1776
#define MDCCLXXVII       1777
#define MDCCLXXVIII      1778
#define MDCCLXXIX        1779
#define MDCCLXXX         1780
#define MDCCLXXXI        1781
#define MDCCLXXXII       1782
#define MDCCLXXXIII      1783
#define MDCCLXXXIV       1784
#define MDCCLXXXV        1785
#define MDCCLXXXVI       1786
#define MDCCLXXXVII      1787
#define MDCCLXXXVIII     1788
#define MDCCLXXXIX       1789
#define MDCCXC           1790
#define MDCCXCI          1791
#define MDCCXCII         1792
#define MDCCXCIII        1793
#define MDCCXCIV         1794
#define MDCCXCV          1795
#define MDCCXCVI         1796
#define MDCCXCVII        1797
#define MDCCXCVIII       1798
#define MDCCXCIX         1799
#define MDCCC            1800
#define MDCCCI           1801
#define MDCCCII          1802
#define MDCCCIII         1803
#define MDCCCIV          1804
#define MDCCCV           1805
#define MDCCCVI          1806
#define MDCCCVII         1807
#define MDCCCVIII        1808
#define MDCCCIX          1809
#define MDCCCX           1810
#define MDCCCXI          1811
#define MDCCCXII         1812
#define MDCCCXIII        1813
#define MDCCCXIV         1814
#define MDCCCXV          1815
#define MDCCCXVI         1816
#define MDCCCXVII        1817
#define MDCCCXVIII       1818
#define MDCCCXIX         1819
#define MDCCCXX          1820
#define MDCCCXXI         1821
#define MDCCCXXII        1822
#define MDCCCXXIII       1823
#define MDCCCXXIV        1824
#define MDCCCXXV         1825
#define MDCCCXXVI        1826
#define MDCCCXXVII       1827
#define MDCCCXXVIII      1828
#define MDCCCXXIX        1829
#define MDCCCXXX         1830
#define MDCCCXXXI        1831
#define MDCCCXXXII       1832
#define MDCCCXXXIII      1833
#define MDCCCXXXIV       1834
#define MDCCCXXXV        1835
#define MDCCCXXXVI       1836
#define MDCCCXXXVII      1837
#define MDCCCXXXVIII     1838
#define MDCCCXXXIX       1839
#define MDCCCXL          1840
#define MDCCCXLI         1841
#define MDCCCXLII        1842
#define MDCCCXLIII       1843
#define MDCCCXLIV        1844
#define MDCCCXLV         1845
#define MDCCCXLVI        1846
#define MDCCCXLVII       1847
#define MDCCCXLVIII      1848
#define MDCCCXLIX        1849
#define MDCCCL           1850
#define MDCCCLI          1851
#define MDCCCLII         1852
#define MDCCCLIII        1853
#define MDCCCLIV         1854
#define MDCCCLV          1855
#define MDCCCLVI         1856
#define MDCCCLVII        1857
#define MDCCCLVIII       1858
#define MDCCCLIX         1859
#define MDCCCLX          1860
#define MDCCCLXI         1861
#define MDCCCLXII        1862
#define MDCCCLXIII       1863
#define MDCCCLXIV        1864
#define MDCCCLXV         1865
#define MDCCCLXVI        1866
#define MDCCCLXVII       1867
#define MDCCCLXVIII      1868
#define MDCCCLXIX        1869
#define MDCCCLXX         1870
#define MDCCCLXXI        1871
#define MDCCCLXXII       1872
#define MDCCCLXXIII      1873
#define MDCCCLXXIV       1874
#define MDCCCLXXV        1875
#define MDCCCLXXVI       1876
#define MDCCCLXXVII      1877
#define MDCCCLXXVIII     1878
#define MDCCCLXXIX       1879
#define MDCCCLXXX        1880
#define MDCCCLXXXI       1881
#define MDCCCLXXXII      1882
#define MDCCCLXXXIII     1883
#define MDCCCLXXXIV      1884
#define MDCCCLXXXV       1885
#define MDCCCLXXXVI      1886
#define MDCCCLXXXVII     1887
#define MDCCCLXXXVIII    1888
#define MDCCCLXXXIX      1889
#define MDCCCXC          1890
#define MDCCCXCI         1891
#define MDCCCXCII        1892
#define MDCCCXCIII       1893
#define MDCCCXCIV        1894
#define MDCCCXCV         1895
#define MDCCCXCVI        1896
#define MDCCCXCVII       1897
#define MDCCCXCVIII      1898
#define MDCCCXCIX        1899
#define MCM              1900
#define MCMI             1901
#define MCMII            1902
#define MCMIII           1903
#define MCMIV            1904
#define MCMV             1905
#define MCMVI            1906
#define MCMVII           1907
#define MCMVIII          1908
#define MCMIX            1909
#define MCMX             1910
#define MCMXI            1911
#define MCMXII           1912
#define MCMXIII          1913
#define MCMXIV           1914
#define MCMXV            1915
#define MCMXVI           1916
#define MCMXVII          1917
#define MCMXVIII         1918
#define MCMXIX           1919
#define MCMXX            1920
#define MCMXXI           1921
#define MCMXXII          1922
#define MCMXXIII         1923
#define MCMXXIV          1924
#define MCMXXV           1925
#define MCMXXVI          1926
#define MCMXXVII         1927
#define MCMXXVIII        1928
#define MCMXXIX          1929
#define MCMXXX           1930
#define MCMXXXI          1931
#define MCMXXXII         1932
#define MCMXXXIII        1933
#define MCMXXXIV         1934
#define MCMXXXV          1935
#define MCMXXXVI         1936
#define MCMXXXVII        1937
#define MCMXXXVIII       1938
#define MCMXXXIX         1939
#define MCMXL            1940
#define MCMXLI           1941
#define MCMXLII          1942
#define MCMXLIII         1943
#define MCMXLIV          1944
#define MCMXLV           1945
#define MCMXLVI          1946
#define MCMXLVII         1947
#define MCMXLVIII        1948
#define MCMXLIX          1949
#define MCML             1950
#define MCMLI            1951
#define MCMLII           1952
#define MCMLIII          1953
#define MCMLIV           1954
#define MCMLV            1955
#define MCMLVI           1956
#define MCMLVII          1957
#define MCMLVIII         1958
#define MCMLIX           1959
#define MCMLX            1960
#define MCMLXI           1961
#define MCMLXII          1962
#define MCMLXIII         1963
#define MCMLXIV          1964
#define MCMLXV           1965
#define MCMLXVI          1966
#define MCMLXVII         1967
#define MCMLXVIII        1968
#define MCMLXIX          1969
#define MCMLXX           1970
#define MCMLXXI          1971
#define MCMLXXII         1972
#define MCMLXXIII        1973
#define MCMLXXIV         1974
#define MCMLXXV          1975
#define MCMLXXVI         1976
#define MCMLXXVII        1977
#define MCMLXXVIII       1978
#define MCMLXXIX         1979
#define MCMLXXX          1980
#define MCMLXXXI         1981
#define MCMLXXXII        1982
#define MCMLXXXIII       1983
#define MCMLXXXIV        1984
#define MCMLXXXV         1985
#define MCMLXXXVI        1986
#define MCMLXXXVII       1987
#define MCMLXXXVIII      1988
#define MCMLXXXIX        1989
#define MCMXC            1990
#define MCMXCI           1991
#define MCMXCII          1992
#define MCMXCIII         1993
#define MCMXCIV          1994
#define MCMXCV           1995
#define MCMXCVI          1996
#define MCMXCVII         1997
#define MCMXCVIII        1998
#define MCMXCIX          1999
#define MM               2000
#define MMI              2001
#define MMII             2002
#define MMIII            2003
#define MMIV             2004
#define MMV              2005
#define MMVI             2006
#define MMVII            2007
#define MMVIII           2008
#define MMIX             2009
#define MMX              2010
#define MMXI             2011
#define MMXII            2012
#define MMXIII           2013
#define MMXIV            2014
#define MMXV             2015
#define MMXVI            2016
#define MMXVII           2017
#define MMXVIII          2018
#define MMXIX            2019
#define MMXX             2020
#define MMXXI            2021
#define MMXXII           2022
#define MMXXIII          2023
#define MMXXIV           2024
#define MMXXV            2025
#define MMXXVI           2026
#define MMXXVII          2027
#define MMXXVIII         2028
#define MMXXIX           2029
#define MMXXX            2030
#define MMXXXI           2031
#define MMXXXII          2032
#define MMXXXIII         2033
#define MMXXXIV          2034
#define MMXXXV           2035
#define MMXXXVI          2036
#define MMXXXVII         2037
#define MMXXXVIII        2038
#define MMXXXIX          2039
#define MMXL             2040
#define MMXLI            2041
#define MMXLII           2042
#define MMXLIII          2043
#define MMXLIV           2044
#define MMXLV            2045
#define MMXLVI           2046
#define MMXLVII          2047
#define MMXLVIII         2048
#define MMXLIX           2049
#define MML              2050
#define MMLI             2051
#define MMLII            2052
#define MMLIII           2053
#define MMLIV            2054
#define MMLV             2055
#define MMLVI            2056
#define MMLVII           2057
#define MMLVIII          2058
#define MMLIX            2059
#define MMLX             2060
#define MMLXI            2061
#define MMLXII           2062
#define MMLXIII          2063
#define MMLXIV           2064
#define MMLXV            2065
#define MMLXVI           2066
#define MMLXVII          2067
#define MMLXVIII         2068
#define MMLXIX           2069
#define MMLXX            2070
#define MMLXXI           2071
#define MMLXXII          2072
#define MMLXXIII         2073
#define MMLXXIV          2074
#define MMLXXV           2075
#define MMLXXVI          2076
#define MMLXXVII         2077
#define MMLXXVIII        2078
#define MMLXXIX          2079
#define MMLXXX           2080
#define MMLXXXI          2081
#define MMLXXXII         2082
#define MMLXXXIII        2083
#define MMLXXXIV         2084
#define MMLXXXV          2085
#define MMLXXXVI         2086
#define MMLXXXVII        2087
#define MMLXXXVIII       2088
#define MMLXXXIX         2089
#define MMXC             2090
#define MMXCI            2091
#define MMXCII           2092
#define MMXCIII          2093
#define MMXCIV           2094
#define MMXCV            2095
#define MMXCVI           2096
#define MMXCVII          2097
#define MMXCVIII         2098
#define MMXCIX           2099
#define MMC              2100
#define MMCI             2101
#define MMCII            2102
#define MMCIII           2103
#define MMCIV            2104
#define MMCV             2105
#define MMCVI            2106
#define MMCVII           2107
#define MMCVIII          2108
#define MMCIX            2109
#define MMCX             2110
#define MMCXI            2111
#define MMCXII           2112
#define MMCXIII          2113
#define MMCXIV           2114
#define MMCXV            2115
#define MMCXVI           2116
#define MMCXVII          2117
#define MMCXVIII         2118
#define MMCXIX           2119
#define MMCXX            2120
#define MMCXXI           2121
#define MMCXXII          2122
#define MMCXXIII         2123
#define MMCXXIV          2124
#define MMCXXV           2125
#define MMCXXVI          2126
#define MMCXXVII         2127
#define MMCXXVIII        2128
#define MMCXXIX          2129
#define MMCXXX           2130
#define MMCXXXI          2131
#define MMCXXXII         2132
#define MMCXXXIII        2133
#define MMCXXXIV         2134
#define MMCXXXV          2135
#define MMCXXXVI         2136
#define MMCXXXVII        2137
#define MMCXXXVIII       2138
#define MMCXXXIX         2139
#define MMCXL            2140
#define MMCXLI           2141
#define MMCXLII          2142
#define MMCXLIII         2143
#define MMCXLIV          2144
#define MMCXLV           2145
#define MMCXLVI          2146
#define MMCXLVII         2147
#define MMCXLVIII        2148
#define MMCXLIX          2149
#define MMCL             2150
#define MMCLI            2151
#define MMCLII           2152
#define MMCLIII          2153
#define MMCLIV           2154
#define MMCLV            2155
#define MMCLVI           2156
#define MMCLVII          2157
#define MMCLVIII         2158
#define MMCLIX           2159
#define MMCLX            2160
#define MMCLXI           2161
#define MMCLXII          2162
#define MMCLXIII         2163
#define MMCLXIV          2164
#define MMCLXV           2165
#define MMCLXVI          2166
#define MMCLXVII         2167
#define MMCLXVIII        2168
#define MMCLXIX          2169
#define MMCLXX           2170
#define MMCLXXI          2171
#define MMCLXXII         2172
#define MMCLXXIII        2173
#define MMCLXXIV         2174
#define MMCLXXV          2175
#define MMCLXXVI         2176
#define MMCLXXVII        2177
#define MMCLXXVIII       2178
#define MMCLXXIX         2179
#define MMCLXXX          2180
#define MMCLXXXI         2181
#define MMCLXXXII        2182
#define MMCLXXXIII       2183
#define MMCLXXXIV        2184
#define MMCLXXXV         2185
#define MMCLXXXVI        2186
#define MMCLXXXVII       2187
#define MMCLXXXVIII      2188
#define MMCLXXXIX        2189
#define MMCXC            2190
#define MMCXCI           2191
#define MMCXCII          2192
#define MMCXCIII         2193
#define MMCXCIV          2194
#define MMCXCV           2195
#define MMCXCVI          2196
#define MMCXCVII         2197
#define MMCXCVIII        2198
#define MMCXCIX          2199
#define MMCC             2200
#define MMCCI            2201
#define MMCCII           2202
#define MMCCIII          2203
#define MMCCIV           2204
#define MMCCV            2205
#define MMCCVI           2206
#define MMCCVII          2207
#define MMCCVIII         2208
#define MMCCIX           2209
#define MMCCX            2210
#define MMCCXI           2211
#define MMCCXII          2212
#define MMCCXIII         2213
#define MMCCXIV          2214
#define MMCCXV           2215
#define MMCCXVI          2216
#define MMCCXVII         2217
#define MMCCXVIII        2218
#define MMCCXIX          2219
#define MMCCXX           2220
#define MMCCXXI          2221
#define MMCCXXII         2222
#define MMCCXXIII        2223
#define MMCCXXIV         2224
#define MMCCXXV          2225
#define MMCCXXVI         2226
#define MMCCXXVII        2227
#define MMCCXXVIII       2228
#define MMCCXXIX         2229
#define MMCCXXX          2230
#define MMCCXXXI         2231
#define MMCCXXXII        2232
#define MMCCXXXIII       2233
#define MMCCXXXIV        2234
#define MMCCXXXV         2235
#define MMCCXXXVI        2236
#define MMCCXXXVII       2237
#define MMCCXXXVIII      2238
#define MMCCXXXIX        2239
#define MMCCXL           2240
#define MMCCXLI          2241
#define MMCCXLII         2242
#define MMCCXLIII        2243
#define MMCCXLIV         2244
#define MMCCXLV          2245
#define MMCCXLVI         2246
#define MMCCXLVII        2247
#define MMCCXLVIII       2248
#define MMCCXLIX         2249
#define MMCCL            2250
#define MMCCLI           2251
#define MMCCLII          2252
#define MMCCLIII         2253
#define MMCCLIV          2254
#define MMCCLV           2255
#define MMCCLVI          2256
#define MMCCLVII         2257
#define MMCCLVIII        2258
#define MMCCLIX          2259
#define MMCCLX           2260
#define MMCCLXI          2261
#define MMCCLXII         2262
#define MMCCLXIII        2263
#define MMCCLXIV         2264
#define MMCCLXV          2265
#define MMCCLXVI         2266
#define MMCCLXVII        2267
#define MMCCLXVIII       2268
#define MMCCLXIX         2269
#define MMCCLXX          2270
#define MMCCLXXI         2271
#define MMCCLXXII        2272
#define MMCCLXXIII       2273
#define MMCCLXXIV        2274
#define MMCCLXXV         2275
#define MMCCLXXVI        2276
#define MMCCLXXVII       2277
#define MMCCLXXVIII      2278
#define MMCCLXXIX        2279
#define MMCCLXXX         2280
#define MMCCLXXXI        2281
#define MMCCLXXXII       2282
#define MMCCLXXXIII      2283
#define MMCCLXXXIV       2284
#define MMCCLXXXV        2285
#define MMCCLXXXVI       2286
#define MMCCLXXXVII      2287
#define MMCCLXXXVIII     2288
#define MMCCLXXXIX       2289
#define MMCCXC           2290
#define MMCCXCI          2291
#define MMCCXCII         2292
#define MMCCXCIII        2293
#define MMCCXCIV         2294
#define MMCCXCV          2295
#define MMCCXCVI         2296
#define MMCCXCVII        2297
#define MMCCXCVIII       2298
#define MMCCXCIX         2299
#define MMCCC            2300
#define MMCCCI           2301
#define MMCCCII          2302
#define MMCCCIII         2303
#define MMCCCIV          2304
#define MMCCCV           2305
#define MMCCCVI          2306
#define MMCCCVII         2307
#define MMCCCVIII        2308
#define MMCCCIX          2309
#define MMCCCX           2310
#define MMCCCXI          2311
#define MMCCCXII         2312
#define MMCCCXIII        2313
#define MMCCCXIV         2314
#define MMCCCXV          2315
#define MMCCCXVI         2316
#define MMCCCXVII        2317
#define MMCCCXVIII       2318
#define MMCCCXIX         2319
#define MMCCCXX          2320
#define MMCCCXXI         2321
#define MMCCCXXII        2322
#define MMCCCXXIII       2323
#define MMCCCXXIV        2324
#define MMCCCXXV         2325
#define MMCCCXXVI        2326
#define MMCCCXXVII       2327
#define MMCCCXXVIII      2328
#define MMCCCXXIX        2329
#define MMCCCXXX         2330
#define MMCCCXXXI        2331
#define MMCCCXXXII       2332
#define MMCCCXXXIII      2333
#define MMCCCXXXIV       2334
#define MMCCCXXXV        2335
#define MMCCCXXXVI       2336
#define MMCCCXXXVII      2337
#define MMCCCXXXVIII     2338
#define MMCCCXXXIX       2339
#define MMCCCXL          2340
#define MMCCCXLI         2341
#define MMCCCXLII        2342
#define MMCCCXLIII       2343
#define MMCCCXLIV        2344
#define MMCCCXLV         2345
#define MMCCCXLVI        2346
#define MMCCCXLVII       2347
#define MMCCCXLVIII      2348
#define MMCCCXLIX        2349
#define MMCCCL           2350
#define MMCCCLI          2351
#define MMCCCLII         2352
#define MMCCCLIII        2353
#define MMCCCLIV         2354
#define MMCCCLV          2355
#define MMCCCLVI         2356
#define MMCCCLVII        2357
#define MMCCCLVIII       2358
#define MMCCCLIX         2359
#define MMCCCLX          2360
#define MMCCCLXI         2361
#define MMCCCLXII        2362
#define MMCCCLXIII       2363
#define MMCCCLXIV        2364
#define MMCCCLXV         2365
#define MMCCCLXVI        2366
#define MMCCCLXVII       2367
#define MMCCCLXVIII      2368
#define MMCCCLXIX        2369
#define MMCCCLXX         2370
#define MMCCCLXXI        2371
#define MMCCCLXXII       2372
#define MMCCCLXXIII      2373
#define MMCCCLXXIV       2374
#define MMCCCLXXV        2375
#define MMCCCLXXVI       2376
#define MMCCCLXXVII      2377
#define MMCCCLXXVIII     2378
#define MMCCCLXXIX       2379
#define MMCCCLXXX        2380
#define MMCCCLXXXI       2381
#define MMCCCLXXXII      2382
#define MMCCCLXXXIII     2383
#define MMCCCLXXXIV      2384
#define MMCCCLXXXV       2385
#define MMCCCLXXXVI      2386
#define MMCCCLXXXVII     2387
#define MMCCCLXXXVIII    2388
#define MMCCCLXXXIX      2389
#define MMCCCXC          2390
#define MMCCCXCI         2391
#define MMCCCXCII        2392
#define MMCCCXCIII       2393
#define MMCCCXCIV        2394
#define MMCCCXCV         2395
#define MMCCCXCVI        2396
#define MMCCCXCVII       2397
#define MMCCCXCVIII      2398
#define MMCCCXCIX        2399
#define MMCD             2400
#define MMCDI            2401
#define MMCDII           2402
#define MMCDIII          2403
#define MMCDIV           2404
#define MMCDV            2405
#define MMCDVI           2406
#define MMCDVII          2407
#define MMCDVIII         2408
#define MMCDIX           2409
#define MMCDX            2410
#define MMCDXI           2411
#define MMCDXII          2412
#define MMCDXIII         2413
#define MMCDXIV          2414
#define MMCDXV           2415
#define MMCDXVI          2416
#define MMCDXVII         2417
#define MMCDXVIII        2418
#define MMCDXIX          2419
#define MMCDXX           2420
#define MMCDXXI          2421
#define MMCDXXII         2422
#define MMCDXXIII        2423
#define MMCDXXIV         2424
#define MMCDXXV          2425
#define MMCDXXVI         2426
#define MMCDXXVII        2427
#define MMCDXXVIII       2428
#define MMCDXXIX         2429
#define MMCDXXX          2430
#define MMCDXXXI         2431
#define MMCDXXXII        2432
#define MMCDXXXIII       2433
#define MMCDXXXIV        2434
#define MMCDXXXV         2435
#define MMCDXXXVI        2436
#define MMCDXXXVII       2437
#define MMCDXXXVIII      2438
#define MMCDXXXIX        2439
#define MMCDXL           2440
#define MMCDXLI          2441
#define MMCDXLII         2442
#define MMCDXLIII        2443
#define MMCDXLIV         2444
#define MMCDXLV          2445
#define MMCDXLVI         2446
#define MMCDXLVII        2447
#define MMCDXLVIII       2448
#define MMCDXLIX         2449
#define MMCDL            2450
#define MMCDLI           2451
#define MMCDLII          2452
#define MMCDLIII         2453
#define MMCDLIV          2454
#define MMCDLV           2455
#define MMCDLVI          2456
#define MMCDLVII         2457
#define MMCDLVIII        2458
#define MMCDLIX          2459
#define MMCDLX           2460
#define MMCDLXI          2461
#define MMCDLXII         2462
#define MMCDLXIII        2463
#define MMCDLXIV         2464
#define MMCDLXV          2465
#define MMCDLXVI         2466
#define MMCDLXVII        2467
#define MMCDLXVIII       2468
#define MMCDLXIX         2469
#define MMCDLXX          2470
#define MMCDLXXI         2471
#define MMCDLXXII        2472
#define MMCDLXXIII       2473
#define MMCDLXXIV        2474
#define MMCDLXXV         2475
#define MMCDLXXVI        2476
#define MMCDLXXVII       2477
#define MMCDLXXVIII      2478
#define MMCDLXXIX        2479
#define MMCDLXXX         2480
#define MMCDLXXXI        2481
#define MMCDLXXXII       2482
#define MMCDLXXXIII      2483
#define MMCDLXXXIV       2484
#define MMCDLXXXV        2485
#define MMCDLXXXVI       2486
#define MMCDLXXXVII      2487
#define MMCDLXXXVIII     2488
#define MMCDLXXXIX       2489
#define MMCDXC           2490
#define MMCDXCI          2491
#define MMCDXCII         2492
#define MMCDXCIII        2493
#define MMCDXCIV         2494
#define MMCDXCV          2495
#define MMCDXCVI         2496
#define MMCDXCVII        2497
#define MMCDXCVIII       2498
#define MMCDXCIX         2499
#define MMD              2500
#define MMDI             2501
#define MMDII            2502
#define MMDIII           2503
#define MMDIV            2504
#define MMDV             2505
#define MMDVI            2506
#define MMDVII           2507
#define MMDVIII          2508
#define MMDIX            2509
#define MMDX             2510
#define MMDXI            2511
#define MMDXII           2512
#define MMDXIII          2513
#define MMDXIV           2514
#define MMDXV            2515
#define MMDXVI           2516
#define MMDXVII          2517
#define MMDXVIII         2518
#define MMDXIX           2519
#define MMDXX            2520
#define MMDXXI           2521
#define MMDXXII          2522
#define MMDXXIII         2523
#define MMDXXIV          2524
#define MMDXXV           2525
#define MMDXXVI          2526
#define MMDXXVII         2527
#define MMDXXVIII        2528
#define MMDXXIX          2529
#define MMDXXX           2530
#define MMDXXXI          2531
#define MMDXXXII         2532
#define MMDXXXIII        2533
#define MMDXXXIV         2534
#define MMDXXXV          2535
#define MMDXXXVI         2536
#define MMDXXXVII        2537
#define MMDXXXVIII       2538
#define MMDXXXIX         2539
#define MMDXL            2540
#define MMDXLI           2541
#define MMDXLII          2542
#define MMDXLIII         2543
#define MMDXLIV          2544
#define MMDXLV           2545
#define MMDXLVI          2546
#define MMDXLVII         2547
#define MMDXLVIII        2548
#define MMDXLIX          2549
#define MMDL             2550
#define MMDLI            2551
#define MMDLII           2552
#define MMDLIII          2553
#define MMDLIV           2554
#define MMDLV            2555
#define MMDLVI           2556
#define MMDLVII          2557
#define MMDLVIII         2558
#define MMDLIX           2559
#define MMDLX            2560
#define MMDLXI           2561
#define MMDLXII          2562
#define MMDLXIII         2563
#define MMDLXIV          2564
#define MMDLXV           2565
#define MMDLXVI          2566
#define MMDLXVII         2567
#define MMDLXVIII        2568
#define MMDLXIX          2569
#define MMDLXX           2570
#define MMDLXXI          2571
#define MMDLXXII         2572
#define MMDLXXIII        2573
#define MMDLXXIV         2574
#define MMDLXXV          2575
#define MMDLXXVI         2576
#define MMDLXXVII        2577
#define MMDLXXVIII       2578
#define MMDLXXIX         2579
#define MMDLXXX          2580
#define MMDLXXXI         2581
#define MMDLXXXII        2582
#define MMDLXXXIII       2583
#define MMDLXXXIV        2584
#define MMDLXXXV         2585
#define MMDLXXXVI        2586
#define MMDLXXXVII       2587
#define MMDLXXXVIII      2588
#define MMDLXXXIX        2589
#define MMDXC            2590
#define MMDXCI           2591
#define MMDXCII          2592
#define MMDXCIII         2593
#define MMDXCIV          2594
#define MMDXCV           2595
#define MMDXCVI          2596
#define MMDXCVII         2597
#define MMDXCVIII        2598
#define MMDXCIX          2599
#define MMDC             2600
#define MMDCI            2601
#define MMDCII           2602
#define MMDCIII          2603
#define MMDCIV           2604
#define MMDCV            2605
#define MMDCVI           2606
#define MMDCVII          2607
#define MMDCVIII         2608
#define MMDCIX           2609
#define MMDCX            2610
#define MMDCXI           2611
#define MMDCXII          2612
#define MMDCXIII         2613
#define MMDCXIV          2614
#define MMDCXV           2615
#define MMDCXVI          2616
#define MMDCXVII         2617
#define MMDCXVIII        2618
#define MMDCXIX          2619
#define MMDCXX           2620
#define MMDCXXI          2621
#define MMDCXXII         2622
#define MMDCXXIII        2623
#define MMDCXXIV         2624
#define MMDCXXV          2625
#define MMDCXXVI         2626
#define MMDCXXVII        2627
#define MMDCXXVIII       2628
#define MMDCXXIX         2629
#define MMDCXXX          2630
#define MMDCXXXI         2631
#define MMDCXXXII        2632
#define MMDCXXXIII       2633
#define MMDCXXXIV        2634
#define MMDCXXXV         2635
#define MMDCXXXVI        2636
#define MMDCXXXVII       2637
#define MMDCXXXVIII      2638
#define MMDCXXXIX        2639
#define MMDCXL           2640
#define MMDCXLI          2641
#define MMDCXLII         2642
#define MMDCXLIII        2643
#define MMDCXLIV         2644
#define MMDCXLV          2645
#define MMDCXLVI         2646
#define MMDCXLVII        2647
#define MMDCXLVIII       2648
#define MMDCXLIX         2649
#define MMDCL            2650
#define MMDCLI           2651
#define MMDCLII          2652
#define MMDCLIII         2653
#define MMDCLIV          2654
#define MMDCLV           2655
#define MMDCLVI          2656
#define MMDCLVII         2657
#define MMDCLVIII        2658
#define MMDCLIX          2659
#define MMDCLX           2660
#define MMDCLXI          2661
#define MMDCLXII         2662
#define MMDCLXIII        2663
#define MMDCLXIV         2664
#define MMDCLXV          2665
#define MMDCLXVI         2666
#define MMDCLXVII        2667
#define MMDCLXVIII       2668
#define MMDCLXIX         2669
#define MMDCLXX          2670
#define MMDCLXXI         2671
#define MMDCLXXII        2672
#define MMDCLXXIII       2673
#define MMDCLXXIV        2674
#define MMDCLXXV         2675
#define MMDCLXXVI        2676
#define MMDCLXXVII       2677
#define MMDCLXXVIII      2678
#define MMDCLXXIX        2679
#define MMDCLXXX         2680
#define MMDCLXXXI        2681
#define MMDCLXXXII       2682
#define MMDCLXXXIII      2683
#define MMDCLXXXIV       2684
#define MMDCLXXXV        2685
#define MMDCLXXXVI       2686
#define MMDCLXXXVII      2687
#define MMDCLXXXVIII     2688
#define MMDCLXXXIX       2689
#define MMDCXC           2690
#define MMDCXCI          2691
#define MMDCXCII         2692
#define MMDCXCIII        2693
#define MMDCXCIV         2694
#define MMDCXCV          2695
#define MMDCXCVI         2696
#define MMDCXCVII        2697
#define MMDCXCVIII       2698
#define MMDCXCIX         2699
#define MMDCC            2700
#define MMDCCI           2701
#define MMDCCII          2702
#define MMDCCIII         2703
#define MMDCCIV          2704
#define MMDCCV           2705
#define MMDCCVI          2706
#define MMDCCVII         2707
#define MMDCCVIII        2708
#define MMDCCIX          2709
#define MMDCCX           2710
#define MMDCCXI          2711
#define MMDCCXII         2712
#define MMDCCXIII        2713
#define MMDCCXIV         2714
#define MMDCCXV          2715
#define MMDCCXVI         2716
#define MMDCCXVII        2717
#define MMDCCXVIII       2718
#define MMDCCXIX         2719
#define MMDCCXX          2720
#define MMDCCXXI         2721
#define MMDCCXXII        2722
#define MMDCCXXIII       2723
#define MMDCCXXIV        2724
#define MMDCCXXV         2725
#define MMDCCXXVI        2726
#define MMDCCXXVII       2727
#define MMDCCXXVIII      2728
#define MMDCCXXIX        2729
#define MMDCCXXX         2730
#define MMDCCXXXI        2731
#define MMDCCXXXII       2732
#define MMDCCXXXIII      2733
#define MMDCCXXXIV       2734
#define MMDCCXXXV        2735
#define MMDCCXXXVI       2736
#define MMDCCXXXVII      2737
#define MMDCCXXXVIII     2738
#define MMDCCXXXIX       2739
#define MMDCCXL          2740
#define MMDCCXLI         2741
#define MMDCCXLII        2742
#define MMDCCXLIII       2743
#define MMDCCXLIV        2744
#define MMDCCXLV         2745
#define MMDCCXLVI        2746
#define MMDCCXLVII       2747
#define MMDCCXLVIII      2748
#define MMDCCXLIX        2749
#define MMDCCL           2750
#define MMDCCLI          2751
#define MMDCCLII         2752
#define MMDCCLIII        2753
#define MMDCCLIV         2754
#define MMDCCLV          2755
#define MMDCCLVI         2756
#define MMDCCLVII        2757
#define MMDCCLVIII       2758
#define MMDCCLIX         2759
#define MMDCCLX          2760
#define MMDCCLXI         2761
#define MMDCCLXII        2762
#define MMDCCLXIII       2763
#define MMDCCLXIV        2764
#define MMDCCLXV         2765
#define MMDCCLXVI        2766
#define MMDCCLXVII       2767
#define MMDCCLXVIII      2768
#define MMDCCLXIX        2769
#define MMDCCLXX         2770
#define MMDCCLXXI        2771
#define MMDCCLXXII       2772
#define MMDCCLXXIII      2773
#define MMDCCLXXIV       2774
#define MMDCCLXXV        2775
#define MMDCCLXXVI       2776
#define MMDCCLXXVII      2777
#define MMDCCLXXVIII     2778
#define MMDCCLXXIX       2779
#define MMDCCLXXX        2780
#define MMDCCLXXXI       2781
#define MMDCCLXXXII      2782
#define MMDCCLXXXIII     2783
#define MMDCCLXXXIV      2784
#define MMDCCLXXXV       2785
#define MMDCCLXXXVI      2786
#define MMDCCLXXXVII     2787
#define MMDCCLXXXVIII    2788
#define MMDCCLXXXIX      2789
#define MMDCCXC          2790
#define MMDCCXCI         2791
#define MMDCCXCII        2792
#define MMDCCXCIII       2793
#define MMDCCXCIV        2794
#define MMDCCXCV         2795
#define MMDCCXCVI        2796
#define MMDCCXCVII       2797
#define MMDCCXCVIII      2798
#define MMDCCXCIX        2799
#define MMDCCC           2800
#define MMDCCCI          2801
#define MMDCCCII         2802
#define MMDCCCIII        2803
#define MMDCCCIV         2804
#define MMDCCCV          2805
#define MMDCCCVI         2806
#define MMDCCCVII        2807
#define MMDCCCVIII       2808
#define MMDCCCIX         2809
#define MMDCCCX          2810
#define MMDCCCXI         2811
#define MMDCCCXII        2812
#define MMDCCCXIII       2813
#define MMDCCCXIV        2814
#define MMDCCCXV         2815
#define MMDCCCXVI        2816
#define MMDCCCXVII       2817
#define MMDCCCXVIII      2818
#define MMDCCCXIX        2819
#define MMDCCCXX         2820
#define MMDCCCXXI        2821
#define MMDCCCXXII       2822
#define MMDCCCXXIII      2823
#define MMDCCCXXIV       2824
#define MMDCCCXXV        2825
#define MMDCCCXXVI       2826
#define MMDCCCXXVII      2827
#define MMDCCCXXVIII     2828
#define MMDCCCXXIX       2829
#define MMDCCCXXX        2830
#define MMDCCCXXXI       2831
#define MMDCCCXXXII      2832
#define MMDCCCXXXIII     2833
#define MMDCCCXXXIV      2834
#define MMDCCCXXXV       2835
#define MMDCCCXXXVI      2836
#define MMDCCCXXXVII     2837
#define MMDCCCXXXVIII    2838
#define MMDCCCXXXIX      2839
#define MMDCCCXL         2840
#define MMDCCCXLI        2841
#define MMDCCCXLII       2842
#define MMDCCCXLIII      2843
#define MMDCCCXLIV       2844
#define MMDCCCXLV        2845
#define MMDCCCXLVI       2846
#define MMDCCCXLVII      2847
#define MMDCCCXLVIII     2848
#define MMDCCCXLIX       2849
#define MMDCCCL          2850
#define MMDCCCLI         2851
#define MMDCCCLII        2852
#define MMDCCCLIII       2853
#define MMDCCCLIV        2854
#define MMDCCCLV         2855
#define MMDCCCLVI        2856
#define MMDCCCLVII       2857
#define MMDCCCLVIII      2858
#define MMDCCCLIX        2859
#define MMDCCCLX         2860
#define MMDCCCLXI        2861
#define MMDCCCLXII       2862
#define MMDCCCLXIII      2863
#define MMDCCCLXIV       2864
#define MMDCCCLXV        2865
#define MMDCCCLXVI       2866
#define MMDCCCLXVII      2867
#define MMDCCCLXVIII     2868
#define MMDCCCLXIX       2869
#define MMDCCCLXX        2870
#define MMDCCCLXXI       2871
#define MMDCCCLXXII      2872
#define MMDCCCLXXIII     2873
#define MMDCCCLXXIV      2874
#define MMDCCCLXXV       2875
#define MMDCCCLXXVI      2876
#define MMDCCCLXXVII     2877
#define MMDCCCLXXVIII    2878
#define MMDCCCLXXIX      2879
#define MMDCCCLXXX       2880
#define MMDCCCLXXXI      2881
#define MMDCCCLXXXII     2882
#define MMDCCCLXXXIII    2883
#define MMDCCCLXXXIV     2884
#define MMDCCCLXXXV      2885
#define MMDCCCLXXXVI     2886
#define MMDCCCLXXXVII    2887
#define MMDCCCLXXXVIII   2888
#define MMDCCCLXXXIX     2889
#define MMDCCCXC         2890
#define MMDCCCXCI        2891
#define MMDCCCXCII       2892
#define MMDCCCXCIII      2893
#define MMDCCCXCIV       2894
#define MMDCCCXCV        2895
#define MMDCCCXCVI       2896
#define MMDCCCXCVII      2897
#define MMDCCCXCVIII     2898
#define MMDCCCXCIX       2899
#define MMCM             2900
#define MMCMI            2901
#define MMCMII           2902
#define MMCMIII          2903
#define MMCMIV           2904
#define MMCMV            2905
#define MMCMVI           2906
#define MMCMVII          2907
#define MMCMVIII         2908
#define MMCMIX           2909
#define MMCMX            2910
#define MMCMXI           2911
#define MMCMXII          2912
#define MMCMXIII         2913
#define MMCMXIV          2914
#define MMCMXV           2915
#define MMCMXVI          2916
#define MMCMXVII         2917
#define MMCMXVIII        2918
#define MMCMXIX          2919
#define MMCMXX           2920
#define MMCMXXI          2921
#define MMCMXXII         2922
#define MMCMXXIII        2923
#define MMCMXXIV         2924
#define MMCMXXV          2925
#define MMCMXXVI         2926
#define MMCMXXVII        2927
#define MMCMXXVIII       2928
#define MMCMXXIX         2929
#define MMCMXXX          2930
#define MMCMXXXI         2931
#define MMCMXXXII        2932
#define MMCMXXXIII       2933
#define MMCMXXXIV        2934
#define MMCMXXXV         2935
#define MMCMXXXVI        2936
#define MMCMXXXVII       2937
#define MMCMXXXVIII      2938
#define MMCMXXXIX        2939
#define MMCMXL           2940
#define MMCMXLI          2941
#define MMCMXLII         2942
#define MMCMXLIII        2943
#define MMCMXLIV         2944
#define MMCMXLV          2945
#define MMCMXLVI         2946
#define MMCMXLVII        2947
#define MMCMXLVIII       2948
#define MMCMXLIX         2949
#define MMCML            2950
#define MMCMLI           2951
#define MMCMLII          2952
#define MMCMLIII         2953
#define MMCMLIV          2954
#define MMCMLV           2955
#define MMCMLVI          2956
#define MMCMLVII         2957
#define MMCMLVIII        2958
#define MMCMLIX          2959
#define MMCMLX           2960
#define MMCMLXI          2961
#define MMCMLXII         2962
#define MMCMLXIII        2963
#define MMCMLXIV         2964
#define MMCMLXV          2965
#define MMCMLXVI         2966
#define MMCMLXVII        2967
#define MMCMLXVIII       2968
#define MMCMLXIX         2969
#define MMCMLXX          2970
#define MMCMLXXI         2971
#define MMCMLXXII        2972
#define MMCMLXXIII       2973
#define MMCMLXXIV        2974
#define MMCMLXXV         2975
#define MMCMLXXVI        2976
#define MMCMLXXVII       2977
#define MMCMLXXVIII      2978
#define MMCMLXXIX        2979
#define MMCMLXXX         2980
#define MMCMLXXXI        2981
#define MMCMLXXXII       2982
#define MMCMLXXXIII      2983
#define MMCMLXXXIV       2984
#define MMCMLXXXV        2985
#define MMCMLXXXVI       2986
#define MMCMLXXXVII      2987
#define MMCMLXXXVIII     2988
#define MMCMLXXXIX       2989
#define MMCMXC           2990
#define MMCMXCI          2991
#define MMCMXCII         2992
#define MMCMXCIII        2993
#define MMCMXCIV         2994
#define MMCMXCV          2995
#define MMCMXCVI         2996
#define MMCMXCVII        2997
#define MMCMXCVIII       2998
#define MMCMXCIX         2999
#define MMM              3000
#define MMMI             3001
#define MMMII            3002
#define MMMIII           3003
#define MMMIV            3004
#define MMMV             3005
#define MMMVI            3006
#define MMMVII           3007
#define MMMVIII          3008
#define MMMIX            3009
#define MMMX             3010
#define MMMXI            3011
#define MMMXII           3012
#define MMMXIII          3013
#define MMMXIV           3014
#define MMMXV            3015
#define MMMXVI           3016
#define MMMXVII          3017
#define MMMXVIII         3018
#define MMMXIX           3019
#define MMMXX            3020
#define MMMXXI           3021
#define MMMXXII          3022
#define MMMXXIII         3023
#define MMMXXIV          3024
#define MMMXXV           3025
#define MMMXXVI          3026
#define MMMXXVII         3027
#define MMMXXVIII        3028
#define MMMXXIX          3029
#define MMMXXX           3030
#define MMMXXXI          3031
#define MMMXXXII         3032
#define MMMXXXIII        3033
#define MMMXXXIV         3034
#define MMMXXXV          3035
#define MMMXXXVI         3036
#define MMMXXXVII        3037
#define MMMXXXVIII       3038
#define MMMXXXIX         3039
#define MMMXL            3040
#define MMMXLI           3041
#define MMMXLII          3042
#define MMMXLIII         3043
#define MMMXLIV          3044
#define MMMXLV           3045
#define MMMXLVI          3046
#define MMMXLVII         3047
#define MMMXLVIII        3048
#define MMMXLIX          3049
#define MMML             3050
#define MMMLI            3051
#define MMMLII           3052
#define MMMLIII          3053
#define MMMLIV           3054
#define MMMLV            3055
#define MMMLVI           3056
#define MMMLVII          3057
#define MMMLVIII         3058
#define MMMLIX           3059
#define MMMLX            3060
#define MMMLXI           3061
#define MMMLXII          3062
#define MMMLXIII         3063
#define MMMLXIV          3064
#define MMMLXV           3065
#define MMMLXVI          3066
#define MMMLXVII         3067
#define MMMLXVIII        3068
#define MMMLXIX          3069
#define MMMLXX           3070
#define MMMLXXI          3071
#define MMMLXXII         3072
#define MMMLXXIII        3073
#define MMMLXXIV         3074
#define MMMLXXV          3075
#define MMMLXXVI         3076
#define MMMLXXVII        3077
#define MMMLXXVIII       3078
#define MMMLXXIX         3079
#define MMMLXXX          3080
#define MMMLXXXI         3081
#define MMMLXXXII        3082
#define MMMLXXXIII       3083
#define MMMLXXXIV        3084
#define MMMLXXXV         3085
#define MMMLXXXVI        3086
#define MMMLXXXVII       3087
#define MMMLXXXVIII      3088
#define MMMLXXXIX        3089
#define MMMXC            3090
#define MMMXCI           3091
#define MMMXCII          3092
#define MMMXCIII         3093
#define MMMXCIV          3094
#define MMMXCV           3095
#define MMMXCVI          3096
#define MMMXCVII         3097
#define MMMXCVIII        3098
#define MMMXCIX          3099
#define MMMC             3100
#define MMMCI            3101
#define MMMCII           3102
#define MMMCIII          3103
#define MMMCIV           3104
#define MMMCV            3105
#define MMMCVI           3106
#define MMMCVII          3107
#define MMMCVIII         3108
#define MMMCIX           3109
#define MMMCX            3110
#define MMMCXI           3111
#define MMMCXII          3112
#define MMMCXIII         3113
#define MMMCXIV          3114
#define MMMCXV           3115
#define MMMCXVI          3116
#define MMMCXVII         3117
#define MMMCXVIII        3118
#define MMMCXIX          3119
#define MMMCXX           3120
#define MMMCXXI          3121
#define MMMCXXII         3122
#define MMMCXXIII        3123
#define MMMCXXIV         3124
#define MMMCXXV          3125
#define MMMCXXVI         3126
#define MMMCXXVII        3127
#define MMMCXXVIII       3128
#define MMMCXXIX         3129
#define MMMCXXX          3130
#define MMMCXXXI         3131
#define MMMCXXXII        3132
#define MMMCXXXIII       3133
#define MMMCXXXIV        3134
#define MMMCXXXV         3135
#define MMMCXXXVI        3136
#define MMMCXXXVII       3137
#define MMMCXXXVIII      3138
#define MMMCXXXIX        3139
#define MMMCXL           3140
#define MMMCXLI          3141
#define MMMCXLII         3142
#define MMMCXLIII        3143
#define MMMCXLIV         3144
#define MMMCXLV          3145
#define MMMCXLVI         3146
#define MMMCXLVII        3147
#define MMMCXLVIII       3148
#define MMMCXLIX         3149
#define MMMCL            3150
#define MMMCLI           3151
#define MMMCLII          3152
#define MMMCLIII         3153
#define MMMCLIV          3154
#define MMMCLV           3155
#define MMMCLVI          3156
#define MMMCLVII         3157
#define MMMCLVIII        3158
#define MMMCLIX          3159
#define MMMCLX           3160
#define MMMCLXI          3161
#define MMMCLXII         3162
#define MMMCLXIII        3163
#define MMMCLXIV         3164
#define MMMCLXV          3165
#define MMMCLXVI         3166
#define MMMCLXVII        3167
#define MMMCLXVIII       3168
#define MMMCLXIX         3169
#define MMMCLXX          3170
#define MMMCLXXI         3171
#define MMMCLXXII        3172
#define MMMCLXXIII       3173
#define MMMCLXXIV        3174
#define MMMCLXXV         3175
#define MMMCLXXVI        3176
#define MMMCLXXVII       3177
#define MMMCLXXVIII      3178
#define MMMCLXXIX        3179
#define MMMCLXXX         3180
#define MMMCLXXXI        3181
#define MMMCLXXXII       3182
#define MMMCLXXXIII      3183
#define MMMCLXXXIV       3184
#define MMMCLXXXV        3185
#define MMMCLXXXVI       3186
#define MMMCLXXXVII      3187
#define MMMCLXXXVIII     3188
#define MMMCLXXXIX       3189
#define MMMCXC           3190
#define MMMCXCI          3191
#define MMMCXCII         3192
#define MMMCXCIII        3193
#define MMMCXCIV         3194
#define MMMCXCV          3195
#define MMMCXCVI         3196
#define MMMCXCVII        3197
#define MMMCXCVIII       3198
#define MMMCXCIX         3199
#define MMMCC            3200
#define MMMCCI           3201
#define MMMCCII          3202
#define MMMCCIII         3203
#define MMMCCIV          3204
#define MMMCCV           3205
#define MMMCCVI          3206
#define MMMCCVII         3207
#define MMMCCVIII        3208
#define MMMCCIX          3209
#define MMMCCX           3210
#define MMMCCXI          3211
#define MMMCCXII         3212
#define MMMCCXIII        3213
#define MMMCCXIV         3214
#define MMMCCXV          3215
#define MMMCCXVI         3216
#define MMMCCXVII        3217
#define MMMCCXVIII       3218
#define MMMCCXIX         3219
#define MMMCCXX          3220
#define MMMCCXXI         3221
#define MMMCCXXII        3222
#define MMMCCXXIII       3223
#define MMMCCXXIV        3224
#define MMMCCXXV         3225
#define MMMCCXXVI        3226
#define MMMCCXXVII       3227
#define MMMCCXXVIII      3228
#define MMMCCXXIX        3229
#define MMMCCXXX         3230
#define MMMCCXXXI        3231
#define MMMCCXXXII       3232
#define MMMCCXXXIII      3233
#define MMMCCXXXIV       3234
#define MMMCCXXXV        3235
#define MMMCCXXXVI       3236
#define MMMCCXXXVII      3237
#define MMMCCXXXVIII     3238
#define MMMCCXXXIX       3239
#define MMMCCXL          3240
#define MMMCCXLI         3241
#define MMMCCXLII        3242
#define MMMCCXLIII       3243
#define MMMCCXLIV        3244
#define MMMCCXLV         3245
#define MMMCCXLVI        3246
#define MMMCCXLVII       3247
#define MMMCCXLVIII      3248
#define MMMCCXLIX        3249
#define MMMCCL           3250
#define MMMCCLI          3251
#define MMMCCLII         3252
#define MMMCCLIII        3253
#define MMMCCLIV         3254
#define MMMCCLV          3255
#define MMMCCLVI         3256
#define MMMCCLVII        3257
#define MMMCCLVIII       3258
#define MMMCCLIX         3259
#define MMMCCLX          3260
#define MMMCCLXI         3261
#define MMMCCLXII        3262
#define MMMCCLXIII       3263
#define MMMCCLXIV        3264
#define MMMCCLXV         3265
#define MMMCCLXVI        3266
#define MMMCCLXVII       3267
#define MMMCCLXVIII      3268
#define MMMCCLXIX        3269
#define MMMCCLXX         3270
#define MMMCCLXXI        3271
#define MMMCCLXXII       3272
#define MMMCCLXXIII      3273
#define MMMCCLXXIV       3274
#define MMMCCLXXV        3275
#define MMMCCLXXVI       3276
#define MMMCCLXXVII      3277
#define MMMCCLXXVIII     3278
#define MMMCCLXXIX       3279
#define MMMCCLXXX        3280
#define MMMCCLXXXI       3281
#define MMMCCLXXXII      3282
#define MMMCCLXXXIII     3283
#define MMMCCLXXXIV      3284
#define MMMCCLXXXV       3285
#define MMMCCLXXXVI      3286
#define MMMCCLXXXVII     3287
#define MMMCCLXXXVIII    3288
#define MMMCCLXXXIX      3289
#define MMMCCXC          3290
#define MMMCCXCI         3291
#define MMMCCXCII        3292
#define MMMCCXCIII       3293
#define MMMCCXCIV        3294
#define MMMCCXCV         3295
#define MMMCCXCVI        3296
#define MMMCCXCVII       3297
#define MMMCCXCVIII      3298
#define MMMCCXCIX        3299
#define MMMCCC           3300
#define MMMCCCI          3301
#define MMMCCCII         3302
#define MMMCCCIII        3303
#define MMMCCCIV         3304
#define MMMCCCV          3305
#define MMMCCCVI         3306
#define MMMCCCVII        3307
#define MMMCCCVIII       3308
#define MMMCCCIX         3309
#define MMMCCCX          3310
#define MMMCCCXI         3311
#define MMMCCCXII        3312
#define MMMCCCXIII       3313
#define MMMCCCXIV        3314
#define MMMCCCXV         3315
#define MMMCCCXVI        3316
#define MMMCCCXVII       3317
#define MMMCCCXVIII      3318
#define MMMCCCXIX        3319
#define MMMCCCXX         3320
#define MMMCCCXXI        3321
#define MMMCCCXXII       3322
#define MMMCCCXXIII      3323
#define MMMCCCXXIV       3324
#define MMMCCCXXV        3325
#define MMMCCCXXVI       3326
#define MMMCCCXXVII      3327
#define MMMCCCXXVIII     3328
#define MMMCCCXXIX       3329
#define MMMCCCXXX        3330
#define MMMCCCXXXI       3331
#define MMMCCCXXXII      3332
#define MMMCCCXXXIII     3333
#define MMMCCCXXXIV      3334
#define MMMCCCXXXV       3335
#define MMMCCCXXXVI      3336
#define MMMCCCXXXVII     3337
#define MMMCCCXXXVIII    3338
#define MMMCCCXXXIX      3339
#define MMMCCCXL         3340
#define MMMCCCXLI        3341
#define MMMCCCXLII       3342
#define MMMCCCXLIII      3343
#define MMMCCCXLIV       3344
#define MMMCCCXLV        3345
#define MMMCCCXLVI       3346
#define MMMCCCXLVII      3347
#define MMMCCCXLVIII     3348
#define MMMCCCXLIX       3349
#define MMMCCCL          3350
#define MMMCCCLI         3351
#define MMMCCCLII        3352
#define MMMCCCLIII       3353
#define MMMCCCLIV        3354
#define MMMCCCLV         3355
#define MMMCCCLVI        3356
#define MMMCCCLVII       3357
#define MMMCCCLVIII      3358
#define MMMCCCLIX        3359
#define MMMCCCLX         3360
#define MMMCCCLXI        3361
#define MMMCCCLXII       3362
#define MMMCCCLXIII      3363
#define MMMCCCLXIV       3364
#define MMMCCCLXV        3365
#define MMMCCCLXVI       3366
#define MMMCCCLXVII      3367
#define MMMCCCLXVIII     3368
#define MMMCCCLXIX       3369
#define MMMCCCLXX        3370
#define MMMCCCLXXI       3371
#define MMMCCCLXXII      3372
#define MMMCCCLXXIII     3373
#define MMMCCCLXXIV      3374
#define MMMCCCLXXV       3375
#define MMMCCCLXXVI      3376
#define MMMCCCLXXVII     3377
#define MMMCCCLXXVIII    3378
#define MMMCCCLXXIX      3379
#define MMMCCCLXXX       3380
#define MMMCCCLXXXI      3381
#define MMMCCCLXXXII     3382
#define MMMCCCLXXXIII    3383
#define MMMCCCLXXXIV     3384
#define MMMCCCLXXXV      3385
#define MMMCCCLXXXVI     3386
#define MMMCCCLXXXVII    3387
#define MMMCCCLXXXVIII   3388
#define MMMCCCLXXXIX     3389
#define MMMCCCXC         3390
#define MMMCCCXCI        3391
#define MMMCCCXCII       3392
#define MMMCCCXCIII      3393
#define MMMCCCXCIV       3394
#define MMMCCCXCV        3395
#define MMMCCCXCVI       3396
#define MMMCCCXCVII      3397
#define MMMCCCXCVIII     3398
#define MMMCCCXCIX       3399
#define MMMCD            3400
#define MMMCDI           3401
#define MMMCDII          3402
#define MMMCDIII         3403
#define MMMCDIV          3404
#define MMMCDV           3405
#define MMMCDVI          3406
#define MMMCDVII         3407
#define MMMCDVIII        3408
#define MMMCDIX          3409
#define MMMCDX           3410
#define MMMCDXI          3411
#define MMMCDXII         3412
#define MMMCDXIII        3413
#define MMMCDXIV         3414
#define MMMCDXV          3415
#define MMMCDXVI         3416
#define MMMCDXVII        3417
#define MMMCDXVIII       3418
#define MMMCDXIX         3419
#define MMMCDXX          3420
#define MMMCDXXI         3421
#define MMMCDXXII        3422
#define MMMCDXXIII       3423
#define MMMCDXXIV        3424
#define MMMCDXXV         3425
#define MMMCDXXVI        3426
#define MMMCDXXVII       3427
#define MMMCDXXVIII      3428
#define MMMCDXXIX        3429
#define MMMCDXXX         3430
#define MMMCDXXXI        3431
#define MMMCDXXXII       3432
#define MMMCDXXXIII      3433
#define MMMCDXXXIV       3434
#define MMMCDXXXV        3435
#define MMMCDXXXVI       3436
#define MMMCDXXXVII      3437
#define MMMCDXXXVIII     3438
#define MMMCDXXXIX       3439
#define MMMCDXL          3440
#define MMMCDXLI         3441
#define MMMCDXLII        3442
#define MMMCDXLIII       3443
#define MMMCDXLIV        3444
#define MMMCDXLV         3445
#define MMMCDXLVI        3446
#define MMMCDXLVII       3447
#define MMMCDXLVIII      3448
#define MMMCDXLIX        3449
#define MMMCDL           3450
#define MMMCDLI          3451
#define MMMCDLII         3452
#define MMMCDLIII        3453
#define MMMCDLIV         3454
#define MMMCDLV          3455
#define MMMCDLVI         3456
#define MMMCDLVII        3457
#define MMMCDLVIII       3458
#define MMMCDLIX         3459
#define MMMCDLX          3460
#define MMMCDLXI         3461
#define MMMCDLXII        3462
#define MMMCDLXIII       3463
#define MMMCDLXIV        3464
#define MMMCDLXV         3465
#define MMMCDLXVI        3466
#define MMMCDLXVII       3467
#define MMMCDLXVIII      3468
#define MMMCDLXIX        3469
#define MMMCDLXX         3470
#define MMMCDLXXI        3471
#define MMMCDLXXII       3472
#define MMMCDLXXIII      3473
#define MMMCDLXXIV       3474
#define MMMCDLXXV        3475
#define MMMCDLXXVI       3476
#define MMMCDLXXVII      3477
#define MMMCDLXXVIII     3478
#define MMMCDLXXIX       3479
#define MMMCDLXXX        3480
#define MMMCDLXXXI       3481
#define MMMCDLXXXII      3482
#define MMMCDLXXXIII     3483
#define MMMCDLXXXIV      3484
#define MMMCDLXXXV       3485
#define MMMCDLXXXVI      3486
#define MMMCDLXXXVII     3487
#define MMMCDLXXXVIII    3488
#define MMMCDLXXXIX      3489
#define MMMCDXC          3490
#define MMMCDXCI         3491
#define MMMCDXCII        3492
#define MMMCDXCIII       3493
#define MMMCDXCIV        3494
#define MMMCDXCV         3495
#define MMMCDXCVI        3496
#define MMMCDXCVII       3497
#define MMMCDXCVIII      3498
#define MMMCDXCIX        3499
#define MMMD             3500
#define MMMDI            3501
#define MMMDII           3502
#define MMMDIII          3503
#define MMMDIV           3504
#define MMMDV            3505
#define MMMDVI           3506
#define MMMDVII          3507
#define MMMDVIII         3508
#define MMMDIX           3509
#define MMMDX            3510
#define MMMDXI           3511
#define MMMDXII          3512
#define MMMDXIII         3513
#define MMMDXIV          3514
#define MMMDXV           3515
#define MMMDXVI          3516
#define MMMDXVII         3517
#define MMMDXVIII        3518
#define MMMDXIX          3519
#define MMMDXX           3520
#define MMMDXXI          3521
#define MMMDXXII         3522
#define MMMDXXIII        3523
#define MMMDXXIV         3524
#define MMMDXXV          3525
#define MMMDXXVI         3526
#define MMMDXXVII        3527
#define MMMDXXVIII       3528
#define MMMDXXIX         3529
#define MMMDXXX          3530
#define MMMDXXXI         3531
#define MMMDXXXII        3532
#define MMMDXXXIII       3533
#define MMMDXXXIV        3534
#define MMMDXXXV         3535
#define MMMDXXXVI        3536
#define MMMDXXXVII       3537
#define MMMDXXXVIII      3538
#define MMMDXXXIX        3539
#define MMMDXL           3540
#define MMMDXLI          3541
#define MMMDXLII         3542
#define MMMDXLIII        3543
#define MMMDXLIV         3544
#define MMMDXLV          3545
#define MMMDXLVI         3546
#define MMMDXLVII        3547
#define MMMDXLVIII       3548
#define MMMDXLIX         3549
#define MMMDL            3550
#define MMMDLI           3551
#define MMMDLII          3552
#define MMMDLIII         3553
#define MMMDLIV          3554
#define MMMDLV           3555
#define MMMDLVI          3556
#define MMMDLVII         3557
#define MMMDLVIII        3558
#define MMMDLIX          3559
#define MMMDLX           3560
#define MMMDLXI          3561
#define MMMDLXII         3562
#define MMMDLXIII        3563
#define MMMDLXIV         3564
#define MMMDLXV          3565
#define MMMDLXVI         3566
#define MMMDLXVII        3567
#define MMMDLXVIII       3568
#define MMMDLXIX         3569
#define MMMDLXX          3570
#define MMMDLXXI         3571
#define MMMDLXXII        3572
#define MMMDLXXIII       3573
#define MMMDLXXIV        3574
#define MMMDLXXV         3575
#define MMMDLXXVI        3576
#define MMMDLXXVII       3577
#define MMMDLXXVIII      3578
#define MMMDLXXIX        3579
#define MMMDLXXX         3580
#define MMMDLXXXI        3581
#define MMMDLXXXII       3582
#define MMMDLXXXIII      3583
#define MMMDLXXXIV       3584
#define MMMDLXXXV        3585
#define MMMDLXXXVI       3586
#define MMMDLXXXVII      3587
#define MMMDLXXXVIII     3588
#define MMMDLXXXIX       3589
#define MMMDXC           3590
#define MMMDXCI          3591
#define MMMDXCII         3592
#define MMMDXCIII        3593
#define MMMDXCIV         3594
#define MMMDXCV          3595
#define MMMDXCVI         3596
#define MMMDXCVII        3597
#define MMMDXCVIII       3598
#define MMMDXCIX         3599
#define MMMDC            3600
#define MMMDCI           3601
#define MMMDCII          3602
#define MMMDCIII         3603
#define MMMDCIV          3604
#define MMMDCV           3605
#define MMMDCVI          3606
#define MMMDCVII         3607
#define MMMDCVIII        3608
#define MMMDCIX          3609
#define MMMDCX           3610
#define MMMDCXI          3611
#define MMMDCXII         3612
#define MMMDCXIII        3613
#define MMMDCXIV         3614
#define MMMDCXV          3615
#define MMMDCXVI         3616
#define MMMDCXVII        3617
#define MMMDCXVIII       3618
#define MMMDCXIX         3619
#define MMMDCXX          3620
#define MMMDCXXI         3621
#define MMMDCXXII        3622
#define MMMDCXXIII       3623
#define MMMDCXXIV        3624
#define MMMDCXXV         3625
#define MMMDCXXVI        3626
#define MMMDCXXVII       3627
#define MMMDCXXVIII      3628
#define MMMDCXXIX        3629
#define MMMDCXXX         3630
#define MMMDCXXXI        3631
#define MMMDCXXXII       3632
#define MMMDCXXXIII      3633
#define MMMDCXXXIV       3634
#define MMMDCXXXV        3635
#define MMMDCXXXVI       3636
#define MMMDCXXXVII      3637
#define MMMDCXXXVIII     3638
#define MMMDCXXXIX       3639
#define MMMDCXL          3640
#define MMMDCXLI         3641
#define MMMDCXLII        3642
#define MMMDCXLIII       3643
#define MMMDCXLIV        3644
#define MMMDCXLV         3645
#define MMMDCXLVI        3646
#define MMMDCXLVII       3647
#define MMMDCXLVIII      3648
#define MMMDCXLIX        3649
#define MMMDCL           3650
#define MMMDCLI          3651
#define MMMDCLII         3652
#define MMMDCLIII        3653
#define MMMDCLIV         3654
#define MMMDCLV          3655
#define MMMDCLVI         3656
#define MMMDCLVII        3657
#define MMMDCLVIII       3658
#define MMMDCLIX         3659
#define MMMDCLX          3660
#define MMMDCLXI         3661
#define MMMDCLXII        3662
#define MMMDCLXIII       3663
#define MMMDCLXIV        3664
#define MMMDCLXV         3665
#define MMMDCLXVI        3666
#define MMMDCLXVII       3667
#define MMMDCLXVIII      3668
#define MMMDCLXIX        3669
#define MMMDCLXX         3670
#define MMMDCLXXI        3671
#define MMMDCLXXII       3672
#define MMMDCLXXIII      3673
#define MMMDCLXXIV       3674
#define MMMDCLXXV        3675
#define MMMDCLXXVI       3676
#define MMMDCLXXVII      3677
#define MMMDCLXXVIII     3678
#define MMMDCLXXIX       3679
#define MMMDCLXXX        3680
#define MMMDCLXXXI       3681
#define MMMDCLXXXII      3682
#define MMMDCLXXXIII     3683
#define MMMDCLXXXIV      3684
#define MMMDCLXXXV       3685
#define MMMDCLXXXVI      3686
#define MMMDCLXXXVII     3687
#define MMMDCLXXXVIII    3688
#define MMMDCLXXXIX      3689
#define MMMDCXC          3690
#define MMMDCXCI         3691
#define MMMDCXCII        3692
#define MMMDCXCIII       3693
#define MMMDCXCIV        3694
#define MMMDCXCV         3695
#define MMMDCXCVI        3696
#define MMMDCXCVII       3697
#define MMMDCXCVIII      3698
#define MMMDCXCIX        3699
#define MMMDCC           3700
#define MMMDCCI          3701
#define MMMDCCII         3702
#define MMMDCCIII        3703
#define MMMDCCIV         3704
#define MMMDCCV          3705
#define MMMDCCVI         3706
#define MMMDCCVII        3707
#define MMMDCCVIII       3708
#define MMMDCCIX         3709
#define MMMDCCX          3710
#define MMMDCCXI         3711
#define MMMDCCXII        3712
#define MMMDCCXIII       3713
#define MMMDCCXIV        3714
#define MMMDCCXV         3715
#define MMMDCCXVI        3716
#define MMMDCCXVII       3717
#define MMMDCCXVIII      3718
#define MMMDCCXIX        3719
#define MMMDCCXX         3720
#define MMMDCCXXI        3721
#define MMMDCCXXII       3722
#define MMMDCCXXIII      3723
#define MMMDCCXXIV       3724
#define MMMDCCXXV        3725
#define MMMDCCXXVI       3726
#define MMMDCCXXVII      3727
#define MMMDCCXXVIII     3728
#define MMMDCCXXIX       3729
#define MMMDCCXXX        3730
#define MMMDCCXXXI       3731
#define MMMDCCXXXII      3732
#define MMMDCCXXXIII     3733
#define MMMDCCXXXIV      3734
#define MMMDCCXXXV       3735
#define MMMDCCXXXVI      3736
#define MMMDCCXXXVII     3737
#define MMMDCCXXXVIII    3738
#define MMMDCCXXXIX      3739
#define MMMDCCXL         3740
#define MMMDCCXLI        3741
#define MMMDCCXLII       3742
#define MMMDCCXLIII      3743
#define MMMDCCXLIV       3744
#define MMMDCCXLV        3745
#define MMMDCCXLVI       3746
#define MMMDCCXLVII      3747
#define MMMDCCXLVIII     3748
#define MMMDCCXLIX       3749
#define MMMDCCL          3750
#define MMMDCCLI         3751
#define MMMDCCLII        3752
#define MMMDCCLIII       3753
#define MMMDCCLIV        3754
#define MMMDCCLV         3755
#define MMMDCCLVI        3756
#define MMMDCCLVII       3757
#define MMMDCCLVIII      3758
#define MMMDCCLIX        3759
#define MMMDCCLX         3760
#define MMMDCCLXI        3761
#define MMMDCCLXII       3762
#define MMMDCCLXIII      3763
#define MMMDCCLXIV       3764
#define MMMDCCLXV        3765
#define MMMDCCLXVI       3766
#define MMMDCCLXVII      3767
#define MMMDCCLXVIII     3768
#define MMMDCCLXIX       3769
#define MMMDCCLXX        3770
#define MMMDCCLXXI       3771
#define MMMDCCLXXII      3772
#define MMMDCCLXXIII     3773
#define MMMDCCLXXIV      3774
#define MMMDCCLXXV       3775
#define MMMDCCLXXVI      3776
#define MMMDCCLXXVII     3777
#define MMMDCCLXXVIII    3778
#define MMMDCCLXXIX      3779
#define MMMDCCLXXX       3780
#define MMMDCCLXXXI      3781
#define MMMDCCLXXXII     3782
#define MMMDCCLXXXIII    3783
#define MMMDCCLXXXIV     3784
#define MMMDCCLXXXV      3785
#define MMMDCCLXXXVI     3786
#define MMMDCCLXXXVII    3787
#define MMMDCCLXXXVIII   3788
#define MMMDCCLXXXIX     3789
#define MMMDCCXC         3790
#define MMMDCCXCI        3791
#define MMMDCCXCII       3792
#define MMMDCCXCIII      3793
#define MMMDCCXCIV       3794
#define MMMDCCXCV        3795
#define MMMDCCXCVI       3796
#define MMMDCCXCVII      3797
#define MMMDCCXCVIII     3798
#define MMMDCCXCIX       3799
#define MMMDCCC          3800
#define MMMDCCCI         3801
#define MMMDCCCII        3802
#define MMMDCCCIII       3803
#define MMMDCCCIV        3804
#define MMMDCCCV         3805
#define MMMDCCCVI        3806
#define MMMDCCCVII       3807
#define MMMDCCCVIII      3808
#define MMMDCCCIX        3809
#define MMMDCCCX         3810
#define MMMDCCCXI        3811
#define MMMDCCCXII       3812
#define MMMDCCCXIII      3813
#define MMMDCCCXIV       3814
#define MMMDCCCXV        3815
#define MMMDCCCXVI       3816
#define MMMDCCCXVII      3817
#define MMMDCCCXVIII     3818
#define MMMDCCCXIX       3819
#define MMMDCCCXX        3820
#define MMMDCCCXXI       3821
#define MMMDCCCXXII      3822
#define MMMDCCCXXIII     3823
#define MMMDCCCXXIV      3824
#define MMMDCCCXXV       3825
#define MMMDCCCXXVI      3826
#define MMMDCCCXXVII     3827
#define MMMDCCCXXVIII    3828
#define MMMDCCCXXIX      3829
#define MMMDCCCXXX       3830
#define MMMDCCCXXXI      3831
#define MMMDCCCXXXII     3832
#define MMMDCCCXXXIII    3833
#define MMMDCCCXXXIV     3834
#define MMMDCCCXXXV      3835
#define MMMDCCCXXXVI     3836
#define MMMDCCCXXXVII    3837
#define MMMDCCCXXXVIII   3838
#define MMMDCCCXXXIX     3839
#define MMMDCCCXL        3840
#define MMMDCCCXLI       3841
#define MMMDCCCXLII      3842
#define MMMDCCCXLIII     3843
#define MMMDCCCXLIV      3844
#define MMMDCCCXLV       3845
#define MMMDCCCXLVI      3846
#define MMMDCCCXLVII     3847
#define MMMDCCCXLVIII    3848
#define MMMDCCCXLIX      3849
#define MMMDCCCL         3850
#define MMMDCCCLI        3851
#define MMMDCCCLII       3852
#define MMMDCCCLIII      3853
#define MMMDCCCLIV       3854
#define MMMDCCCLV        3855
#define MMMDCCCLVI       3856
#define MMMDCCCLVII      3857
#define MMMDCCCLVIII     3858
#define MMMDCCCLIX       3859
#define MMMDCCCLX        3860
#define MMMDCCCLXI       3861
#define MMMDCCCLXII      3862
#define MMMDCCCLXIII     3863
#define MMMDCCCLXIV      3864
#define MMMDCCCLXV       3865
#define MMMDCCCLXVI      3866
#define MMMDCCCLXVII     3867
#define MMMDCCCLXVIII    3868
#define MMMDCCCLXIX      3869
#define MMMDCCCLXX       3870
#define MMMDCCCLXXI      3871
#define MMMDCCCLXXII     3872
#define MMMDCCCLXXIII    3873
#define MMMDCCCLXXIV     3874
#define MMMDCCCLXXV      3875
#define MMMDCCCLXXVI     3876
#define MMMDCCCLXXVII    3877
#define MMMDCCCLXXVIII   3878
#define MMMDCCCLXXIX     3879
#define MMMDCCCLXXX      3880
#define MMMDCCCLXXXI     3881
#define MMMDCCCLXXXII    3882
#define MMMDCCCLXXXIII   3883
#define MMMDCCCLXXXIV    3884
#define MMMDCCCLXXXV     3885
#define MMMDCCCLXXXVI    3886
#define MMMDCCCLXXXVII   3887
#define MMMDCCCLXXXVIII  3888
#define MMMDCCCLXXXIX    3889
#define MMMDCCCXC        3890
#define MMMDCCCXCI       3891
#define MMMDCCCXCII      3892
#define MMMDCCCXCIII     3893
#define MMMDCCCXCIV      3894
#define MMMDCCCXCV       3895
#define MMMDCCCXCVI      3896
#define MMMDCCCXCVII     3897
#define MMMDCCCXCVIII    3898
#define MMMDCCCXCIX      3899
#define MMMCM            3900
#define MMMCMI           3901
#define MMMCMII          3902
#define MMMCMIII         3903
#define MMMCMIV          3904
#define MMMCMV           3905
#define MMMCMVI          3906
#define MMMCMVII         3907
#define MMMCMVIII        3908
#define MMMCMIX          3909
#define MMMCMX           3910
#define MMMCMXI          3911
#define MMMCMXII         3912
#define MMMCMXIII        3913
#define MMMCMXIV         3914
#define MMMCMXV          3915
#define MMMCMXVI         3916
#define MMMCMXVII        3917
#define MMMCMXVIII       3918
#define MMMCMXIX         3919
#define MMMCMXX          3920
#define MMMCMXXI         3921
#define MMMCMXXII        3922
#define MMMCMXXIII       3923
#define MMMCMXXIV        3924
#define MMMCMXXV         3925
#define MMMCMXXVI        3926
#define MMMCMXXVII       3927
#define MMMCMXXVIII      3928
#define MMMCMXXIX        3929
#define MMMCMXXX         3930
#define MMMCMXXXI        3931
#define MMMCMXXXII       3932
#define MMMCMXXXIII      3933
#define MMMCMXXXIV       3934
#define MMMCMXXXV        3935
#define MMMCMXXXVI       3936
#define MMMCMXXXVII      3937
#define MMMCMXXXVIII     3938
#define MMMCMXXXIX       3939
#define MMMCMXL          3940
#define MMMCMXLI         3941
#define MMMCMXLII        3942
#define MMMCMXLIII       3943
#define MMMCMXLIV        3944
#define MMMCMXLV         3945
#define MMMCMXLVI        3946
#define MMMCMXLVII       3947
#define MMMCMXLVIII      3948
#define MMMCMXLIX        3949
#define MMMCML           3950
#define MMMCMLI          3951
#define MMMCMLII         3952
#define MMMCMLIII        3953
#define MMMCMLIV         3954
#define MMMCMLV          3955
#define MMMCMLVI         3956
#define MMMCMLVII        3957
#define MMMCMLVIII       3958
#define MMMCMLIX         3959
#define MMMCMLX          3960
#define MMMCMLXI         3961
#define MMMCMLXII        3962
#define MMMCMLXIII       3963
#define MMMCMLXIV        3964
#define MMMCMLXV         3965
#define MMMCMLXVI        3966
#define MMMCMLXVII       3967
#define MMMCMLXVIII      3968
#define MMMCMLXIX        3969
#define MMMCMLXX         3970
#define MMMCMLXXI        3971
#define MMMCMLXXII       3972
#define MMMCMLXXIII      3973
#define MMMCMLXXIV       3974
#define MMMCMLXXV        3975
#define MMMCMLXXVI       3976
#define MMMCMLXXVII      3977
#define MMMCMLXXVIII     3978
#define MMMCMLXXIX       3979
#define MMMCMLXXX        3980
#define MMMCMLXXXI       3981
#define MMMCMLXXXII      3982
#define MMMCMLXXXIII     3983
#define MMMCMLXXXIV      3984
#define MMMCMLXXXV       3985
#define MMMCMLXXXVI      3986
#define MMMCMLXXXVII     3987
#define MMMCMLXXXVIII    3988
#define MMMCMLXXXIX      3989
#define MMMCMXC          3990
#define MMMCMXCI         3991
#define MMMCMXCII        3992
#define MMMCMXCIII       3993
#define MMMCMXCIV        3994
#define MMMCMXCV         3995
#define MMMCMXCVI        3996
#define MMMCMXCVII       3997
#define MMMCMXCVIII      3998
#define MMMCMXCIX        3999
/* finis numerorum generatorum */

#define imprimere     printf
#define liberare         free
#define memoriae_allocare    malloc
#define exire                exit

#define interior         static
#define hic_manens     static
#define universalis static

#define FILUM FILE

nomen insignatus character    i8;
nomen insignatus brevis         i16;
nomen insignatus integer       i32;
nomen insignatus longus longus    i64;

nomen signatus character    s8;
nomen signatus brevis            s16;
nomen signatus integer         s32;
nomen signatus longus longus    s64;

nomen fluitans                  f32;
nomen duplex                         f64;

nomen integer                    b32;

nomen size_t                                 memoriae_index;

#endif /* LATINA_H */
#line 1 "include/piscina.h"
/* piscina.h - arena memoriae: liberatio tota semel (arena, pool) */
#ifndef PISCINA_H
#define PISCINA_H




/* ===============================================
 * Creatio
 * =============================================== */

nomen structura Piscina Piscina;

/* PiscinaNotatio - nota pro mark/reset pattern
 * Captat statum piscinam ut postea reficere possit
 */
nomen structura PiscinaNotatio {
            vacuum* alveus_nunc;   /* Index ad alveum currentem */
    memoriae_index  positus;       /* Offset in alveo */
} PiscinaNotatio;

Piscina*
piscina_generare_dynamicum (
          constans character* piscinae_titulum,
              memoriae_index  mensura_alvei_initia);

Piscina*
piscina_generare_certae_magnitudinis (
        constans character* piscinae_titulum,
            memoriae_index  mensura_buffer);


/* ===============================================
 * Destructio
 * =============================================== */

vacuum
piscina_destruere (
        Piscina* piscina);


/* ===============================================
 * Allocatio - fatalis si fallit
 *
 * piscina_allocare ordinat ad PISCINA_ORDINATIO_ORDINARIA (VIII):
 * satis pro omni typo domus (indices, i64/s64, f64), sicut malloc.
 * Octeti soli (textus) arte stipari possunt per
 * piscina_allocare_ordinatum(piscina, mensura, I). Ante 2026-10-05
 * ordinatio ordinaria erat I: membra latiora non ordinata - mores
 * indefiniti in C, quos sanitas 'alignment' capit.
 * =============================================== */

#define PISCINA_ORDINATIO_ORDINARIA VIII

vacuum*
piscina_allocare (
                         Piscina* piscina,
                  memoriae_index  mensura);

vacuum*
piscina_allocare_ordinatum (
                         Piscina* piscina,
                  memoriae_index  mensura,
                  memoriae_index  ordinatio);


/* ===============================================
 * Allocatio - redde NIHIL si defectu
 * =============================================== */

vacuum*
piscina_conari_allocare (
                         Piscina* piscina,
                  memoriae_index  mensura);

vacuum*
piscina_conari_allocare_ordinatum (
                         Piscina* piscina,
                  memoriae_index  mensura,
                  memoriae_index  ordinatio);


/* ===============================================
 * Cyclus Vitae
 * =============================================== */

vacuum
piscina_vacare (
        Piscina* piscina);


/* ===============================================
 * Quaestio
 * =============================================== */

memoriae_index
piscina_summa_usus (
        constans Piscina* piscina);

memoriae_index
piscina_summa_inutilis_allocatus (
        constans Piscina* piscina);

memoriae_index
piscina_reliqua_antequam_cresca_alvei (
        constans Piscina* piscina);

memoriae_index
piscina_summa_apex_usus (
        constans Piscina* piscina);

/* Numerus alveorum (alvei omnes, ab primo). Cum summa_usus +
 * summa_inutilis_allocatus imaginem memoriae dat: quot alvei,
 * quantum commissum (usus + inutilis), quantum otiosum. */
memoriae_index
piscina_numerus_alveorum (
        constans Piscina* piscina);

/* Numerus allocationum successarum ab ortu piscinae (reficere eum
 * non minuit - historia est, non status). Instrumentis: piscina
 * allocationibus, non octetis, ligata est (RP 2.4 - Xar per
 * lexema quattuor), ergo numerus ipse mensura celeritatis est. */
memoriae_index
piscina_numerus_allocationum (
        constans Piscina* piscina);


/* ===============================================
 * Notatio - mark/reset pattern
 * =============================================== */

/* piscina_notare - Captat statum currentem
 * "Notare positionem currentem pro refectione postea"
 *
 * Usus: PiscinaNotatio nota = piscina_notare(piscina);
 *       ... allocare temporaria ...
 *       piscina_reficere(piscina, nota);
 */
PiscinaNotatio
piscina_notare (
        Piscina* piscina);

/* piscina_reficere - Reficit statum ad notationem
 * "Reficere piscinam ad statum notatum"
 *
 * Omnia allocata post notationem erunt invalida! Memoria NON
 * liberatur nec deletur: lectio post refectionem valores veteres
 * saepe adhuc videt, ergo vitium tacitum est. Modus probandi:
 * -DPISCINA_VENENUM=1 octetos liberatos 0xA5 implet (porta
 * tools/venenum_probare.sh).
 */
vacuum
piscina_reficere (
               Piscina* piscina,
        PiscinaNotatio  notatio);

/* piscina_potesne_allocare - Verificat si allocatio possibilis
 * "Potesne allocare hanc mensuram sine crescentia?"
 *
 * Utile pro piscinis certae magnitudinis
 * Redde: VERUM si allocatio in alveo nunc capit
 */
b32
piscina_potesne_allocare (
        constans Piscina* piscina,
          memoriae_index  mensura);

#endif
#line 1 "include/chorda.h"
/* chorda.h - chorda: mensura + datum, SINE NUL (string slice) */
#ifndef CHORDA_H
#define CHORDA_H





/* ==================================================
 * Creatio
 * ================================================== */

nomen structura chorda {
    i32  mensura;
     i8* datum;
} chorda;


/* ==================================================
 * Constructores
 * ================================================== */

chorda
chorda_ex_literis (
                  constans character* litterae,
                             Piscina* piscina);

chorda
chorda_ex_buffer (
     i8* buffer,
    i32  mensura);

chorda
chorda_sectio (
          chorda s,
             i32 initium,
             i32 finis);

chorda
chorda_transcribere (
         chorda  s,
        Piscina* piscina);

chorda
chorda_concatenare (
         chorda  a,
         chorda  b,
        Piscina* piscina);

chorda
chorda_praecidi_laterale (
         chorda  s,
        Piscina* piscina);


/* ==================================================
 * Divisio
 * ================================================= */

nomen structura {
    chorda* elementa;
       i32  numerus;
} chorda_fissio_fructus;

chorda_fissio_fructus
chorda_fissio (
           chorda  s,
        character  delim,
          Piscina* piscina);

/* chorda_fissio_chorda - Dividere chordam per delimitatorem chorda
 * Similis chorda_fissio sed delimitator est chorda, non character
 */
chorda_fissio_fructus
chorda_fissio_chorda (
         chorda  s,
         chorda  delim,
        Piscina* piscina);

/* chorda_iungere - Iungere array chordarum cum separatore
 * elementa: array chordarum
 * numerus: numerus elementorum in array
 * separator: chorda inter elementa (potest esse vacua)
 * Redde: Nova chorda iuncta (allocata ex piscina)
 */
chorda
chorda_iungere (
         chorda* elementa,
            i32  numerus,
         chorda  separator,
        Piscina* piscina);


/* ==================================================
 * Comparatio
 * ================================================= */

b32
chorda_aequalis (
        chorda a,
        chorda b);

b32
chorda_aequalis_literis (
                    chorda  s,
        constans character* cstr);

b32
chorda_aequalis_case_insensitivus (
        chorda a,
        chorda b);

s32
chorda_comparare (
        chorda a,
        chorda b);


/* ==================================================
 * Quaestio
 * ================================================= */

b32
chorda_continet (
        chorda fenum,
        chorda acus);

b32
chorda_incipit (
        chorda s,
        chorda prefixum);

b32
chorda_terminatur (
        chorda s,
        chorda suffixum);

/* chorda_invenire - Invenire primam occurrentiam acus in fenum
 * Redde: sectionem CONGRUENTEM solam (mensura = acus.mensura, datum
 *        intra fenum), NON reliquum fenum ut strstr; chorda vacua
 *        {0, NIHIL} si non inventus. Acus vacua -> fenum totum.
 *        Pro reliquo: chorda_invenire_index + chorda_sectio; pro
 *        inventum/non: chorda_continet.
 */
chorda
chorda_invenire (
        chorda fenum,
        chorda acus);

/* chorda_invenire_index - Invenire positionem acus in fenum
 * Redde: Index (0-based) primae occurrentiae, vel -1 si non inventus
 */
s32
chorda_invenire_index (
        chorda fenum,
        chorda acus);

/* chorda_invenire_ultimum - Invenire ultimam occurrentiam acus in fenum
 * Redde: sectionem congruentem ultimam (ut chorda_invenire, non
 *        reliquum), vel chorda vacua si non inventus
 */
chorda
chorda_invenire_ultimum (
        chorda fenum,
        chorda acus);

/* chorda_invenire_ultimum_index - Invenire positionem ultimae occurrentiae
 * Redde: Index (0-based) ultimae occurrentiae, vel -1 si non inventus
 */
s32
chorda_invenire_ultimum_index (
        chorda fenum,
        chorda acus);

i32
chorda_numerare_occurrentia (
        chorda fenum,
        chorda acus);


/* ==================================================
 * Manipulatio
 * ================================================= */

chorda
chorda_praecidere (
        chorda s);

chorda
chorda_minuscula (
         chorda  s,
        Piscina* piscina);

chorda
chorda_maiuscula (
         chorda  s,
        Piscina* piscina);

/* chorda_praecidere_sinistram - Praecidere spatium album a sinistra
 * Redde: Vista (non allocatio) ad chordam sine spatio album initiali
 */
chorda
chorda_praecidere_sinistram (
        chorda s);

/* chorda_praecidere_dextram - Praecidere spatium album a dextra
 * Redde: Vista (non allocatio) ad chordam sine spatio album finali
 */
chorda
chorda_praecidere_dextram (
        chorda s);

/* chorda_substituere - Substituere omnes occurrentias antiqui cum novo
 * Redde: Nova chorda cum substitutionibus (allocata ex piscina)
 */
chorda
chorda_substituere (
          chorda  s,
          chorda  antiquum,
          chorda  novum,
         Piscina* piscina);

/* chorda_invertere - Invertere ordinem characterum
 * Redde: Nova chorda inversa (allocata ex piscina)
 */
chorda
chorda_invertere (
         chorda  s,
        Piscina* piscina);

/* chorda_duplicare - Repetere chordam n vicibus
 * Redde: Nova chorda cum repetitionibus (allocata ex piscina)
 */
chorda
chorda_duplicare (
         chorda  s,
            i32  numerus,
        Piscina* piscina);


/* ==================================================
 * Convenientia
 * ================================================= */

character*
chorda_ut_cstr (
         chorda  s,
        Piscina* piscina);

/* chorda_ut_s32 - Convertere chordam ad integrum signatum (basis X)
 *
 * TEXTUS -> NUMERUS pro chorda (atoi/strtol in c.datum ULTRA
 * mensuram legunt: chorda NUL finale non fert). Tota chorda consumi
 * debet: signum optionale, cifrae, nihil post (spatia ducentia
 * tolerantur, ut strtol). RECUSAT: vacuam, NULLUM insertum, sordes
 * post numerum ("12x"), extra fines s32. Pro LXIV bitis:
 * chorda_ut_s64; praefixum numericum ("p003" -> III): chorda_sectio
 * prius.
 *
 * Redde: VERUM si conversa (fructus solum tunc positus)
 */
b32
chorda_ut_s32 (
        chorda  s,
           s32* fructus);

/* chorda_ut_i32 - Ut chorda_ut_s32, sed insignatum: signum '-'
 * recusat, fines 0..4294967295.
 *
 * Redde: VERUM si conversa
 */
b32
chorda_ut_i32 (
          chorda  s,
             i32* fructus);

/* chorda_ut_f64 - Convertere chordam ad numerum fluitantem (strtod
 * super copiam terminatam): tota chorda consumi debet; recusat
 * vacuam, NULLUM insertum, sordes post numerum.
 *
 * Redde: VERUM si conversa
 */
b32
chorda_ut_f64 (
        chorda  s,
           f64* fructus);

/* chorda_ex_s32 - Convertere integrum signatum ad chordam
 * Redde: Nova chorda (allocata ex piscina)
 */
chorda
chorda_ex_s32 (
            s32  numerus,
        Piscina* piscina);

/* chorda_ex_f64 - Convertere numerum puncta fluitantia ad chordam
 * praecisio: numerus locorum decimalium
 * Redde: Nova chorda (allocata ex piscina)
 */
chorda
chorda_ex_f64 (
            f64  numerus,
            i32  praecisio,
        Piscina* piscina);

/* chorda_ut_s64 - Convertere chordam ad integrum LXIV bitorum
 *
 * Omnes cifrae, signum optionale, nihil aliud; zephyra ducentia sine
 * pondere. RECUSAT: NULLUM insertum (chorda terminatorem non fert,
 * ergo cstr eam truncaret), sordes post numerum, superfluitas (limes
 * LEXICE confertur - strtoll sub -std=c89 latet).
 *
 * Redde: VERUM si conversa
 */
b32
chorda_ut_s64 (
         chorda  s,
            s64* fructus);

/* chorda_ex_s64 - Convertere integrum LXIV bitorum ad chordam
 * Redde: Nova chorda (allocata ex piscina)
 */
chorda
chorda_ex_s64 (
            s64  numerus,
        Piscina* piscina);

/* chorda_ex_f64_exacta - Forma quae ITER REDITUS fert ('%.17g')
 *
 * chorda_ex_f64 praecisionem decimalem FIXAM scribit, ergo 1e300 et
 * 1e-20 per eam pereunt. Haec forma legi potest et eundem duplicem
 * reddere.
 *
 * Redde: Nova chorda (allocata ex piscina), vacua si defectu
 */
chorda
chorda_ex_f64_exacta (
            f64  numerus,
        Piscina* piscina);

/* chorda_character_ad - Extrahere singularem characterem ut chordam
 * Redde: Chorda longitudinis 1, vel vacua si index extra limites
 */
chorda
chorda_character_ad (
         chorda  s,
            i32  index,
        Piscina* piscina);

/* chorda_vacua - Verificare si chorda vacua est
 * Redde: VERUM si datum == NIHIL vel mensura == 0
 */
b32
chorda_vacua (
        chorda s);


/* ==================================================
 * Conversio Casus
 * ================================================= */

/* chorda_pascalis - Convertere ad PascalCase
 * "hello world" → "HelloWorld"
 * "getElementById" → "GetElementById"
 */
chorda
chorda_pascalis (
         chorda  s,
        Piscina* piscina);

/* chorda_camelus - Convertere ad camelCase
 * "hello world" → "helloWorld"
 * "getElementById" → "getElementById"
 */
chorda
chorda_camelus (
         chorda  s,
        Piscina* piscina);

/* chorda_serpens - Convertere ad snake_case
 * "HelloWorld" → "hello_world"
 * "getElementById" → "get_element_by_id"
 */
chorda
chorda_serpens (
         chorda  s,
        Piscina* piscina);

/* chorda_kebab - Convertere ad kebab-case
 * "HelloWorld" → "hello-world"
 * "getElementById" → "get-element-by-id"
 */
chorda
chorda_kebab (
         chorda  s,
        Piscina* piscina);

/* chorda_pascalis_serpens - Convertere ad Pascal_Snake_Case
 * "hello world" → "Hello_World"
 * "getElementById" → "Get_Element_By_Id"
 */
chorda
chorda_pascalis_serpens (
         chorda  s,
        Piscina* piscina);


/* ==================================================
 * Formatatio
 * ================================================= */

/* chorda_ex_bytes_legibilis - Convertere bytes ad formam legibilem
 * Exempla:
 *   1024      → "1.0 KB"
 *   1536      → "1.5 KB"
 *   1048576   → "1.0 MB"
 *   1073741824 → "1.0 GB"
 */
chorda
chorda_ex_bytes_legibilis (
            i64  bytes,
        Piscina* piscina);


/* ==================================================
 * Friatio
 * ================================================= */

i32
chorda_friare (
        chorda s);

#endif /* CHORDA_H */
#line 1 "include/magnus.h"
/* magnus.h - Integri magni EXACTI (sine limite)
 *
 * Numerus integer quilibet, exacte: nulla exundatio tacita. Valor
 * immutabilis (sicut chorda): omnis operatio novum valorem reddit;
 * memoria ex piscina. Valores qui in s64 capiunt INTRA structuram
 * manent (via celeris, sine allocatione); maiores in membris
 * XXXII bitorum in piscina. Nulli fluitantes: idem effectus in omni
 * machina et omni gradu optimizationis.
 *
 * Divisio EUCLIDEA: 0 <= residuum < |divisor|, semper (C89 signum
 * residui negativi non definit; nos definimus).
 *
 * VITA: membra immutabilia sunt, ergo effectus membra argumentorum
 * PARTIRI potest (e.g. magnus_nega signum tantum vertit). Effectus
 * valet dum piscinae argumentorum et effectus vivunt.
 *
 * USUS:
 *   Magnus a = magnus_ex_s64(XII);
 *   Magnus b = magnus_potentia(a, C, piscina);      (* 12^100 *)
 *   chorda t = magnus_ad_chordam(b, piscina);
 *
 * Vide lib/magnus.worklog.md.
 */
/* <aedilis corpus="lib/magnus.c"/> */
#ifndef MAGNUS_H
#define MAGNUS_H





/* Membra PRIVATA - per functiones tantum legenda.
 * Invarians: si valor in s64 capit, membra == NIHIL et valor in
 * 'parvus'; aliter signum (+1/-1) et longitudo membrorum (>= II,
 * membrum summum non nullum). Ergo forma canonica et unica. */
nomen structura {
     s64  parvus;
     s32  signum;
     i32  longitudo;
     i32* membra;      /* ordine parvo primum, basis 2^32 */
} Magnus;


/* ==================================================
 * Creatio et conversio
 * ================================================== */

Magnus
magnus_ex_s64 (
    s64 valor);

/* VERUM et *exitus si valor in s64 capit; aliter FALSUM */
b32
magnus_ad_s64 (
    Magnus  a,
       s64* exitus);

/* decimalis, signum '-' optionale, nulla spatia; FALSUM si textus
 * malformatus (exitus non tangitur) */
b32
magnus_ex_chorda (
      chorda  textus,
     Piscina* piscina,
      Magnus* exitus);

/* decimalis */
chorda
magnus_ad_chordam (
      Magnus  a,
     Piscina* piscina);

/* copia in piscinam datam: membra NOVA (valores in s64 sine
 * allocatione). Ad effectum ex piscina temporaria servandum, cum
 * piscina illa reficienda aut destruenda est. */
Magnus
magnus_transcribe (
      Magnus  a,
     Piscina* piscina);


/* ==================================================
 * Inspectio
 * ================================================== */

/* -1, 0, +1 */
s32
magnus_signum (
    Magnus a);

/* -1, 0, +1 sicut a <, =, > b */
s32
magnus_compara (
    Magnus a,
    Magnus b);

b32
magnus_aequalis (
    Magnus a,
    Magnus b);


/* ==================================================
 * Arithmetica - piscina tantum si via celeris non sufficit
 * ================================================== */

Magnus
magnus_nega (
      Magnus  a,
     Piscina* piscina);

Magnus
magnus_absolutum (
      Magnus  a,
     Piscina* piscina);

Magnus
magnus_adde (
      Magnus  a,
      Magnus  b,
     Piscina* piscina);

Magnus
magnus_subtrahe (
      Magnus  a,
      Magnus  b,
     Piscina* piscina);

Magnus
magnus_multiplica (
      Magnus  a,
      Magnus  b,
     Piscina* piscina);

/* a = quotiens * divisor + residuum, 0 <= residuum < |divisor|.
 * FALSUM si divisor nullus (exitus non tanguntur). Uterque exitus
 * NIHIL esse potest. */
b32
magnus_divide (
      Magnus  a,
      Magnus  divisor,
     Piscina* piscina,
      Magnus* quotiens,
      Magnus* residuum);

/* basis^exponens; 0^0 = 1 */
Magnus
magnus_potentia (
      Magnus  basis,
         i32  exponens,
     Piscina* piscina);

/* numerus bitorum |a|: 0 pro 0, 1 pro +-1, k + 1 pro 2^k <= |a| <
 * 2^(k+1). Pro limitibus (log2) sine allocatione. */
i32
magnus_bitorum (
    Magnus a);

/* a mod n (Euclideum: 0 <= r < n) pro moduli verbi (1 <= n < 2^32):
 * per membra, sine piscina; n == 0 -> 0 */
i32
magnus_residuum_parvum (
    Magnus a,
       i32 n);

/* maximus divisor communis, semper >= 0; (0, 0) -> 0 */
Magnus
magnus_divisor_communis (
      Magnus  a,
      Magnus  b,
     Piscina* piscina);

/* idem cum TESTIBUS (Bezout): g = u*a + v*b - certificatum quod
 * quisque sine hac bibliotheca verificare potest */
Magnus
magnus_divisor_communis_testatus (
      Magnus  a,
      Magnus  b,
     Piscina* piscina,
      Magnus* u,
      Magnus* v);


/* ==================================================
 * Diagnosis
 * ================================================== */

/* maximus usus (octeti) piscinae internae alternae in ultimo divisore
 * communi magnorum operandorum (Euclides in piscinis alternis); 0 si
 * via vocantis electa est (sine testibus: operandus aliquis pauci
 * membrorum; cum testibus: ambo). Computator sumptus
 * deterministicus: idem in omni machina, ergo probationibus
 * asseribilis. */
memoriae_index
magnus_apex_alternarum (
    vacuum);

#endif /* MAGNUS_H */
#line 1 "include/fractio.h"
/* fractio.h - Numeri rationales EXACTI
 *
 * Fractio = numerator / denominator, ambo Magnus, in forma canonica:
 * denominator > 0, divisor communis 1, nihil = 0/1. Valor immutabilis
 * in piscina, sicut magnus (membra partiri potest; effectus valet dum
 * piscinae argumentorum et effectus vivunt). Via celeris magni
 * hereditate: fractiones parvae sine allocatione manent ubi possibile.
 * Nulli fluitantes.
 *
 * Reductio Henrici (Knuth II, 4.5.1): divisores communes ANTE
 * multiplicationem tolluntur, ergo intermedia parva manent et effectus
 * iam reductus est.
 *
 * USUS:
 *   Fractio a = fractio_ex_s64(III);
 *   Fractio b;
 *   (vacuum)fractio_ex_s64_s64(I, IV, piscina, &b);     (* 1/4 *)
 *   Fractio c = fractio_adde(a, b, piscina);
 *   chorda t = fractio_ad_chordam(c, piscina);           (* "13/4" *)
 *
 * Vide lib/fractio.worklog.md.
 */
/* <aedilis corpus="lib/fractio.c"/> */
#ifndef FRACTIO_H
#define FRACTIO_H






/* Membra PRIVATA - per functiones legenda.
 * Invarians: denominator > 0; divisor communis numeratoris et
 * denominatoris 1; nihil = 0/1. Forma canonica et unica.
 *
 * SEMPER per fractio_ex_* (aut operationem) creanda. Fractio octetis
 * nullis impleta (memset, calloc, structura statica) NON est nihil sed
 * 0/0, quod tacite propagatur (0/0 + 1/2 = 0/0) - dissimile Magno,
 * cuius octeti nulli nihil validum sunt. Nihil: fractio_ex_s64(0). */
nomen structura {
    Magnus numerator;
    Magnus denominator;
} Fractio;


/* ==================================================
 * Creatio et conversio
 * ================================================== */

Fractio
fractio_ex_s64 (
    s64 valor);

Fractio
fractio_ex_magno (
    Magnus valor);

/* numerator/denominator reducta; FALSUM si denominator nullus
 * (exitus non tangitur) */
b32
fractio_ex_s64_s64 (
         s64  numerator,
         s64  denominator,
     Piscina* piscina,
     Fractio* exitus);

b32
fractio_ex_magnis (
      Magnus  numerator,
      Magnus  denominator,
     Piscina* piscina,
     Fractio* exitus);

/* "a" aut "a/b" decimalis, signum '-' tantum ante numeratorem, b > 0,
 * nulla spatia; non canonica accipitur ("6/4" -> 3/2). FALSUM si
 * malformata aut b nullus (exitus non tangitur). */
b32
fractio_ex_chorda (
      chorda  textus,
     Piscina* piscina,
     Fractio* exitus);

/* copia in piscinam datam: membra NOVA numeratoris et denominatoris
 * (vide magnus_transcribe) */
Fractio
fractio_transcribe (
     Fractio  a,
     Piscina* piscina);

/* forma canonica: "a" si integra, aliter "a/b" */
chorda
fractio_ad_chordam (
     Fractio  a,
     Piscina* piscina);

Magnus
fractio_numerator (
    Fractio a);

/* semper > 0 */
Magnus
fractio_denominator (
    Fractio a);


/* ==================================================
 * Inspectio
 * ================================================== */

/* -1, 0, +1 */
s32
fractio_signum (
    Fractio a);

/* -1, 0, +1 sicut a <, =, > b; per a*d contra c*b (denominatores
 * positivi), sine divisione */
s32
fractio_compara (
     Fractio  a,
     Fractio  b,
     Piscina* piscina);

b32
fractio_aequalis (
    Fractio a,
    Fractio b);

/* denominator == 1 */
b32
fractio_est_integra (
    Fractio a);


/* ==================================================
 * Arithmetica
 * ================================================== */

Fractio
fractio_nega (
     Fractio  a,
     Piscina* piscina);

Fractio
fractio_absolutum (
     Fractio  a,
     Piscina* piscina);

Fractio
fractio_adde (
     Fractio  a,
     Fractio  b,
     Piscina* piscina);

Fractio
fractio_subtrahe (
     Fractio  a,
     Fractio  b,
     Piscina* piscina);

Fractio
fractio_multiplica (
     Fractio  a,
     Fractio  b,
     Piscina* piscina);

/* FALSUM si divisor nullus (exitus non tangitur) */
b32
fractio_divide (
     Fractio  a,
     Fractio  divisor,
     Piscina* piscina,
     Fractio* exitus);

/* 1/a; FALSUM si a nullus */
b32
fractio_inversa (
     Fractio  a,
     Piscina* piscina,
     Fractio* exitus);

/* basis^exponens, exponens signatus (corpus est: a^-n = 1/a^n);
 * FALSUM si basis nulla et exponens < 0; 0^0 = 1 */
b32
fractio_potentia (
     Fractio  basis,
         s32  exponens,
     Piscina* piscina,
     Fractio* exitus);


/* ==================================================
 * Ad integros
 * ================================================== */

/* maximus integer <= a */
Magnus
fractio_pavimentum (
     Fractio  a,
     Piscina* piscina);

/* minimus integer >= a */
Magnus
fractio_tectum (
     Fractio  a,
     Piscina* piscina);

/* integer proximus; dimidia AD PAREM (1/2 -> 0, 3/2 -> 2, 5/2 -> 2,
 * -1/2 -> 0): sine inclinatione cum multa dimidia rotundantur (regula
 * IEEE). Regula vulgaris ("a nihilo recedens") ex pavimento et tecto
 * facile componitur. */
Magnus
fractio_rotunda (
     Fractio  a,
     Piscina* piscina);

#endif /* FRACTIO_H */
#line 1 "include/polynomium.h"
/* polynomium.h - Polynomia Laurentiana EXACTA super magnum
 *
 * Z[t, t^-1]: polynomia unius variabilis, coefficientibus integris
 * magnis, exponentibus signatis (polynomia ordinaria = casus imus >=
 * 0). Valores immutabiles in piscina, sicut magnus: omnis operatio
 * novum valorem reddit; effectus coefficientes argumentorum PARTIRI
 * potest.
 *
 * Forma canonica: nullum = nulla membra (imus 0); aliter
 * coefficientes imus et summus non nulli. Ergo aequalitas =
 * aequalitas partium.
 *
 * EXPONENTES: |e| <= POLYNOMIUM_EXPONENS_MAXIMUS (2^30 - 1), ita ut
 * amplitudo omnis summae in i32 capiat et omne productum exponentis in
 * s64. Operationes quae exponentem accipiunt aut movent b32 reddunt:
 * FALSUM si exponens aliquis extra fines caderet (exitus non tangitur)
 * - numquam exundatio tacita. nega, adde, subtrahe, multiplica_scalari
 * exponentes non movent et valorem reddunt.
 *
 * MEMORIA densa: summus - imus + 1 coefficientes. t^1000000 unum
 * coefficientem habet, sed t^1000000 + 1 decies centena milia.
 * Amplitudo est sumptus: lectio ideo amplitudinem maiorem quam
 * POLYNOMIUM_AMPLITUDO_LECTIONIS refutat (textus XXX octetorum aliter
 * gigaoctetos posceret). Operationes ipsae nullum talem finem habent.
 *
 * USUS:
 *   Polynomium p;
 *   Polynomium q;
 *   (vacuum)polynomium_ex_chorda(chorda_ex_literis("t^2 - 1", piscina),
 *       't', piscina, &p);
 *   si (!polynomium_multiplica(p, p, piscina, &q)) refutare;
 *
 * Vide lib/polynomium.worklog.md.
 */
/* <aedilis corpus="lib/polynomium.c"/> */
#ifndef POLYNOMIUM_H
#define POLYNOMIUM_H







#define POLYNOMIUM_EXPONENS_MAXIMUS    ((s32)0x3FFFFFFFL)
#define POLYNOMIUM_AMPLITUDO_LECTIONIS  ((i32)0x100000L)   /* 2^20 */

/* Membra PRIVATA - per functiones legenda. coefficientes[i] pertinet
 * ad t^(imus + i). */
nomen structura {
     constans Magnus* coefficientes;
                 i32  numerus;
                 s32  imus;
} Polynomium;


/* ==================================================
 * Constructio et textus
 * ================================================== */

Polynomium
polynomium_nullum (
    vacuum);

Polynomium
polynomium_constans (
      Magnus  c,
     Piscina* piscina);

/* c t^exponens; FALSUM si exponens extra fines */
b32
polynomium_monomium (
        Magnus  c,
           s32  exponens,
       Piscina* piscina,
    Polynomium* exitus);

/* c[i] pro t^(imus + i), copiati; zephyra extrema absciduntur.
 * FALSUM si exponens coefficientis non nulli extra fines. */
b32
polynomium_ex_coefficientibus (
     constans Magnus* c,
                 i32  numerus,
                 s32  imus,
             Piscina* piscina,
          Polynomium* exitus);

/* "3t^2 - t + 1 - 2t^-1": termini ordine quolibet, exponentes iterati
 * coniuncti, spatia et tabulae liberae; "0" = nullum. Littera
 * variabilis ASCII (a-z, A-Z). FALSUM si littera alia, textus
 * malformatus, exponens extra fines, aut summus - imus + 1 >
 * POLYNOMIUM_AMPLITUDO_LECTIONIS (exitus non tangitur). */
b32
polynomium_ex_chorda (
        chorda  textus,
     character  littera,
       Piscina* piscina,
    Polynomium* exitus);

/* copia profunda in piscinam datam: alveus et coefficientes novi (vide
 * magnus_transcribe) */
Polynomium
polynomium_transcribe (
    Polynomium  p,
       Piscina* piscina);

/* forma canonica: gradu summo primo, " + " / " - " inter terminos,
 * nec "1t" nec "t^1"; nullum = "0". Littera ASCII ut supra (aliter
 * textus non relegibilis). */
chorda
polynomium_ad_chordam (
    Polynomium  p,
     character  littera,
       Piscina* piscina);


/* ==================================================
 * Lectio
 * ================================================== */

b32
polynomium_est_nullum (
    Polynomium p);

/* 0 si nullum (vide polynomium_est_nullum) */
s32
polynomium_gradus_imus (
    Polynomium p);

/* 0 si nullum */
s32
polynomium_gradus_summus (
    Polynomium p);

/* coefficiens t^exponens; 0 extra */
Magnus
polynomium_coefficiens (
    Polynomium p,
           s32 exponens);

b32
polynomium_aequalis (
    Polynomium a,
    Polynomium b);

/* maximus divisor communis coefficientium, >= 0; nullum -> 0 */
Magnus
polynomium_contentum (
    Polynomium  p,
       Piscina* piscina);


/* ==================================================
 * Arithmetica
 * ================================================== */

Polynomium
polynomium_nega (
    Polynomium  a,
       Piscina* piscina);

Polynomium
polynomium_adde (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina);

Polynomium
polynomium_subtrahe (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina);

Polynomium
polynomium_multiplica_scalari (
    Polynomium  p,
        Magnus  c,
       Piscina* piscina);

/* FALSUM si exponens extra fines */
b32
polynomium_multiplica (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* exitus);

/* p^n; p^0 = 1 (etiam 0^0). FALSUM si exponens extra fines */
b32
polynomium_potentia (
    Polynomium  p,
           i32  n,
       Piscina* piscina,
    Polynomium* exitus);

/* q = a / b in Z[t, t^-1] (t unitas est). FALSUM si b nullum, si b
 * non dividit a, aut si exponens quotientis extra fines. */
b32
polynomium_divide_exacte (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* quotiens);


/* ==================================================
 * Substitutiones
 * ================================================== */

/* p t^k (coefficientes partiti, nulla allocatio); FALSUM si extra
 * fines */
b32
polynomium_translata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus);

/* t -> t^k, k != 0 (k = 0 FALSUM); k = -1 speculum (chiralitas) */
b32
polynomium_dilata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus);

/* inversa dilatationis: t^k -> t. FALSUM si k = 0 aut exponens
 * aliquis non divisibilis per k (uncinus Kauffman in A -> Jones in
 * t: A = t^(-1/4), ergo contrahe per -4 ubi exponentes A per 4
 * divisibiles sunt) */
b32
polynomium_contrahe (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus);

/* forma normalis ad unitatem +-t^k: imus = 0, coefficiens imus > 0
 * (Alexander definitum est ad +-t^k); nullum -> nullum. FALSUM si
 * summus - imus extra fines. */
b32
polynomium_normale (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus);

/* p(1/t): exponentes negati (fines symmetrici: semper intra) */
Polynomium
polynomium_inversum (
    Polynomium  p,
       Piscina* piscina);

/* coefficientes ordine inverso idem (p = t^k p(1/t), signo eodem):
 * symmetria omnis polynomii Alexander et Jones nodorum amphichiralium;
 * nullum symmetricum est */
b32
polynomium_est_symmetricum (
    Polynomium p);


/* ==================================================
 * Derivata, divisor communis, pars libera, translatio Taylor
 * (radices.h his innititur)
 * ================================================== */

/* p' : t^e -> e t^(e-1) (Laurent licet); FALSUM si exponens extra
 * fines */
b32
polynomium_derivata (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus);

/* maximus divisor communis in Q[t], forma primitiva, coefficiente
 * summo > 0 (unicus); gcd(0, 0) = 0, gcd(a, 0) = forma primitiva a.
 * Polynomia ORDINARIA solum (exponentes >= 0): in Z[t, t^-1] divisor
 * usque ad unitatem t^k ambiguus - FALSUM si exponens negativus adest. */
b32
polynomium_divisor_communis (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* exitus);

/* pars libera quadratis: p / gcd(p, p'), primitiva, coefficiente summo
 * > 0 - easdem radices DISTINCTAS habet, simplices. nullum -> nullum,
 * constans -> 1. FALSUM sicut polynomium_divisor_communis. */
b32
polynomium_pars_libera (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus);

/* p(t + c), translatio Taylor; FALSUM si exponens negativus adest */
b32
polynomium_translatum (
    Polynomium  p,
        Magnus  c,
       Piscina* piscina,
    Polynomium* exitus);


/* ==================================================
 * Valor
 * ================================================== */

/* p(x) per Hornerum super fractionem; FALSUM si x = 0 et exponens
 * negativus adest */
b32
polynomium_valor (
    Polynomium  p,
       Fractio  x,
       Piscina* piscina,
       Fractio* exitus);


/* ==================================================
 * Diagnosis
 * ================================================== */

/* maximus usus (octeti) officinae internae in ultima multiplicatione
 * aut divisione exacta per officinas computata; 0 si operatio parva in
 * piscina vocantis facta est. Computator sumptus deterministicus,
 * probationibus asseribilis (sicut magnus_apex_alternarum). */
memoriae_index
polynomium_apex_officinarum (
    vacuum);

#endif /* POLYNOMIUM_H */
#line 1 "include/anulus.h"
/* anulus.h - Descriptio anuli: operationes elementorum per tabulam
 * functionum
 *
 * Bibliothecae algebraicae (matrix, et futurae) super quemlibet anulum
 * domus operantur per hanc tabulam: elementa opaca magnitudinis datae,
 * operationes per indices functionum. Tres anuli parati: Z (magnus), Q
 * (fractio, corpus), Z[t, t^-1] (polynomium, littera 't').
 *
 * Operationes b32 reddunt: FALSUM si operatio elementi refutat
 * (polynomium: exponens extra fines; divisio exacta non exacta) -
 * exitus tunc non tangitur. Effectus in piscina data vivunt;
 * transcribe copiam PROFUNDAM facit (pro piscinis temporariis
 * reficiendis).
 *
 * USUS:
 *   constans Anulus* z = &ANULUS_INTEGRORUM;
 *   Magnus a = magnus_ex_s64(VI);
 *   Magnus b = magnus_ex_s64(VII);
 *   Magnus c;
 *   (vacuum)z->multiplica(z, &a, &b, piscina, &c);
 *
 * Vide lib/anulus.worklog.md.
 */
/* <aedilis corpus="lib/anulus.c"/> */
#ifndef ANULUS_H
#define ANULUS_H





nomen structura Anulus Anulus;

/* Omnis functio descriptionem ipsam primam accipit (anulus): anuli cum
 * parametris (Z/n: modulus in contextu) eadem tabula utuntur. */
structura Anulus {
    constans character* titulus;      /* "Z", "Q", "Z[t,t^-1]" */
        memoriae_index  mensura;      /* octeti elementi */
                   b32  corpus;       /* non nulli invertibiles */

    vacuum (*nullum) (
        constans Anulus* anulus,
        vacuum* exitus);
    vacuum (*unum) (
        constans Anulus* anulus,
        Piscina* piscina,
         vacuum* exitus);
    b32 (*est_nullum) (
        constans Anulus* anulus,
        constans vacuum* a);
    /* elementum sine memoria externa (totum in structura): Z in s64, Q
     * numerator et denominator in s64, Z[t] nullum solum. Algorithmi
     * hoc ad viam parvam (sine piscinis temporariis) eligendam
     * utuntur. */
    b32 (*parvum) (
        constans Anulus* anulus,
        constans vacuum* a);
    b32 (*aequalis) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b);
    b32 (*adde) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    b32 (*subtrahe) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    b32 (*multiplica) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    /* a / b in anulo: FALSUM si b nullus aut b non dividit a */
    b32 (*divide_exacte) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    vacuum (*transcribe) (
        constans Anulus* anulus,
        constans vacuum* a,
                Piscina* piscina,
                 vacuum* exitus);
    chorda (*ad_chordam) (
        constans Anulus* anulus,
        constans vacuum* a,
                Piscina* piscina);
    b32 (*ex_chorda) (
        constans Anulus* anulus,
          chorda  textus,
        Piscina*  piscina,
         vacuum*  exitus);

    /* Anuli Euclidei solum (NIHIL aliter: Q, Z[t, t^-1]) */
    /* g = u a + v b, g normalis (Z: g >= 0; (0, 0) -> 0) */
    b32 (*divisor_communis) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* g,
                 vacuum* u,
                 vacuum* v);
    /* a = q b + r, r reductus (Z: 0 <= r < |b|); FALSUM si b nullus */
    b32 (*divide_cum_residuo) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* q,
                 vacuum* r);
    /* norma Euclidea: -1, 0, +1 sicut N(a) <, =, > N(b) (Z: |a|
     * ad |b|); residuum divisionis normam stricte minorem habet quam
     * divisor */
    s32 (*compara_normam) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina);

    /* parametra anuli (Z/n: modulus); NIHIL pro Z, Q, Z[t] */
    constans vacuum* contextus;
    /* anulus integer (sine divisoribus nullius): Z, Q, Z[t] VERUM; Z/n
     * sse n primus. Eliminatio (Bareiss: determinans, gradus, nucleus)
     * solum super anulum integrum valet - Z/6 [0,3,3; 4,4,5; 5,5,5]
     * cardine 4 (divisore nullius) columnam totam necabat, det 0 pro 3
     * (recensio anulus-I). */
    b32 integrum;

    /* anuli ORDINATI solum (NIHIL aliter: Z[t, t^-1], Z/n,
     * cyclotomia, extensio sine radice electa): signum a, -1 / 0 / +1;
     * FALSUM si refutatum. Geometria exacta (quaternio: angulus,
     * directio proxima) per quadrata et hoc signum comparat. */
    b32 (*signum) (
        constans Anulus* anulus,
        constans vacuum* a,
                Piscina* piscina,
                    s32* exitus);
};

/* Z: elementa Magnus */
extern constans Anulus ANULUS_INTEGRORUM;

/* Q: elementa Fractio (corpus) */
extern constans Anulus ANULUS_RATIONALIUM;

/* Z[t, t^-1]: elementa Polynomium, textus littera 't' */
extern constans Anulus ANULUS_POLYNOMIORUM;

/* Z/n (2 <= n < 2^32): elementa i32 (residua 0 <= x < n, congruentia),
 * corpus et integrum sse n primus; divide_exacte per inversam (FALSUM
 * si divisor non invertibilis). Descriptio in piscina vivit; NIHIL si
 * n < 2. Matrices comparantur per INDICEM descriptionis: una descriptio
 * pro quoque modulo (duae Z/7 anuli diversi sunt). */
constans Anulus*
anulus_residuorum (
         i32  n,
     Piscina* piscina);

#endif /* ANULUS_H */
#line 1 "include/matrix.h"
/* matrix.h - Matrices EXACTAE super anulum (anulus.h)
 *
 * Matrix super quemlibet anulum domus: Z, Q, Z[t, t^-1]. Elementa opaca
 * (magnitudo anuli), ordine linearum. Algorithmi sine fractionibus
 * (Bareiss): divisiones exactae tantum, ergo super omni anulo integro
 * cum divisione exacta operantur.
 *
 * CONSTRUCTIO: matrix_pone solum dum vocans matricem aedificat. Omnis
 * operatio matricem NOVAM reddit et argumenta numquam mutat.
 *
 * DEFECTUS: functiones b32 reddunt - FALSUM si dimensiones aut anuli
 * discrepant, aut operatio elementi refutat (polynomium: exponens
 * extra fines). Exitus tunc non tangitur.
 *
 * MEMORIA: eliminatio in officinis internis (piscinae temporariae,
 * alternae); solus effectus in piscina vocantis. Matrices parvae (<=
 * XXV elementa) elementis parvis (anulus->parvum) in piscina vocantis
 * directe.
 *
 * VITA: effectus determinantis, nuclei, multiplicationis semper copia
 * profunda in piscina vocantis - memoriam argumentorum non partiuntur.
 * transposita, adde, subtrahe, pone autem structuras elementorum
 * copiant: effectus elementa argumentorum PARTIRI potest (sicut
 * magnus) et valet dum piscinae argumentorum vivunt.
 *
 * Z[t, t^-1]: eliminatio exponentes intermedios crescere facit, ergo
 * determinans, gradus, nucleus FALSUM reddere possunt etiam ubi
 * effectus bene definitus est (e.g. gradus [t^(2^29), 1; 1, t^(2^29)]
 * - t^(2^30) extra fines), et exponentes magni rari memoriam densam
 * polynomii poscunt (vide polynomium.h).
 *
 * USUS:
 *   Matrix v;
 *   Polynomium delta;
 *   (vacuum)matrix_ex_chorda(&ANULUS_POLYNOMIORUM,
 *       chorda_ex_literis("[t - 1, 1; -t, t - 1]", piscina), piscina,
 *       &v);
 *   si (matrix_determinans(v, piscina, &delta)) ... (t^2 - t + 1)
 *
 * Vide lib/matrix.worklog.md.
 */
/* <aedilis corpus="lib/matrix.c"/> */
#ifndef MATRIX_H
#define MATRIX_H






/* Membra PRIVATA - per functiones legenda */
nomen structura {
     constans Anulus* anulus;
                 i32  lineae;
                 i32  columnae;
                  i8* elementa;
} Matrix;


/* ==================================================
 * Constructio et textus
 * ================================================== */

b32
matrix_nulla (
     constans Anulus* anulus,
                 i32  lineae,
                 i32  columnae,
             Piscina* piscina,
              Matrix* exitus);

b32
matrix_identitas (
     constans Anulus* anulus,
                 i32  n,
             Piscina* piscina,
              Matrix* exitus);

/* "[a, b; c, d]": elementa per ',' (textus anuli, spatia circum
 * libera), lineae per ';'; "[]" = 0 x 0. FALSUM si malformatum aut
 * lineae longitudinis inaequalis. */
b32
matrix_ex_chorda (
     constans Anulus* anulus,
              chorda  textus,
             Piscina* piscina,
              Matrix* exitus);

/* "[a, b; c, d]"; matrix cum dimensione nulla (r x 0, 0 x c) "[]"
 * scribitur - dimensiones perduntur, lectio 0 x 0 reddit */
chorda
matrix_ad_chordam (
     Matrix  m,
    Piscina* piscina);

constans Anulus*
matrix_anulus (
    Matrix m);

i32
matrix_lineae (
    Matrix m);

i32
matrix_columnae (
    Matrix m);

/* index ad elementum (typus anuli, e.g. constans Magnus*); NIHIL
 * extra fines */
constans vacuum*
matrix_elementum (
    Matrix m,
       i32 linea,
       i32 columna);

/* solum dum vocans matricem aedificat; extra fines nihil agit */
vacuum
matrix_pone (
               Matrix* m,
                  i32  linea,
                  i32  columna,
      constans vacuum* valor);


/* ==================================================
 * Arithmetica
 * ================================================== */

b32
matrix_aequalis (
    Matrix a,
    Matrix b);

b32
matrix_adde (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus);

b32
matrix_subtrahe (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus);

b32
matrix_multiplica (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus);

b32
matrix_transposita (
     Matrix  m,
    Piscina* piscina,
     Matrix* exitus);


/* ==================================================
 * Eliminatio (Bareiss)
 * ================================================== */

/* determinans matricis quadratae (0 x 0 -> 1); exitus elementum
 * anuli */
b32
matrix_determinans (
     Matrix  m,
    Piscina* piscina,
     vacuum* exitus);

/* gradus (rank) super corpus fractionum anuli */
b32
matrix_gradus (
     Matrix  m,
    Piscina* piscina,
        i32* exitus);

/* nucleus: columnae = basis nuclei super corpus fractionum anuli,
 * elementis IN anulo, per formam Gauss-Jordan sine fractionibus:
 * elementa minores matricis (magnitudo Hadamard finita), sed non
 * reducti (super Z: non primitivi, non basis reticuli). columnae -
 * gradus columnae; 0 si nucleus nullus. */
b32
matrix_nucleus (
     Matrix  m,
    Piscina* piscina,
     Matrix* exitus);


/* ==================================================
 * Formae normales (anulus Euclideus: divisor_communis,
 * divide_cum_residuo, compara_normam; aliter FALSUM - Z solus hodie).
 * Euclides "cardo normae minimae sursum, ceteri modulo eum":
 * incrementum intermedium modicum (Hermite 40 x 40 ~18 ms; Smith cum
 * U, V 40 x 40 ~46 ms).
 * ================================================== */

/* Hermite (lineae): H = U A, U unimodularis; cardines normales (Z: >
 * 0), elementa supra cardinem reducta (Z: 0 <= x < cardo), lineae
 * nullae in fundo. u NIHIL si certificatum non quaeritur. */
b32
matrix_forma_hermite (
     Matrix  a,
    Piscina* piscina,
     Matrix* h,
     Matrix* u);

/* Smith: D = U A V, D diagonalis, d_i normales (Z: >= 0), d_i |
 * d_(i+1), nulli ultimi; U, V unimodulares (NIHIL si non quaeruntur) */
b32
matrix_forma_smith (
     Matrix  a,
    Piscina* piscina,
     Matrix* d,
     Matrix* u,
     Matrix* v);

/* basis RETICULI nuclei (columnae): omnis vector integer x cum A x = 0
 * combinatio integra columnarum est (contra matrix_nucleus) */
b32
matrix_reticulum_nuclei (
     Matrix  a,
    Piscina* piscina,
     Matrix* exitus);


/* ==================================================
 * Diagnosis
 * ================================================== */

/* maximus usus (octeti) officinarum alternarum in ultima eliminatione;
 * computator sumptus deterministicus (sicut magnus_apex_alternarum) */
memoriae_index
matrix_apex_officinarum (
    vacuum);

#endif /* MATRIX_H */
#line 1 "include/radices.h"
/* radices.h - Radices reales polynomiorum integrorum, EXACTE
 *
 * Numerus algebraicus realis = RADIX polynomii f in Z[t] intra
 * intervallum (infra, supra) quod radicem f UNAM continet; infra ==
 * supra si radix est hic numerus rationalis exacte. f liber quadratis,
 * primitivus, coefficiente summo > 0; termini intervalli dyadici (aut
 * fractio data) - radicem suam numquam, radices ALIAS f fortasse (sqrt 3
 * in (1, 2) ubi 1, 2 quoque radices). Radices diversorum polynomiorum
 * comparantur (extensio corpora mixta refutat): aequalitas per
 * divisorem communem decernitur, numquam per latitudinem.
 *
 * ISOLATIO: regula signorum Cartesii cum bisectione (Vincent - Collins
 * - Akritas) super partem liberam quadratis, a limite Cauchy; radix
 * rationalis in medio puncto inventa exacte redditur.
 *
 * Valores in piscina vocantis, sicut polynomium; nihil ibi liberatur.
 * Ibi effecta SOLA (radices, f, termini, chorda): comparatio et signum
 * nihil allocant; temporaria in piscinis propriis, refectis et
 * destructis (radices_interna.h apices metitur). FALSUM si piscina
 * temporaria creari non potest; allocatio deficiens INTRA eam fatalis
 * est, ut in omni piscina.
 *
 * USUS:
 *   RadixRealis* r;
 *   i32          n;
 *   si (radices_reales(f, piscina, &r, &n))  (* r[0] < ... < r[n-1] *)
 *       (vacuum)radix_ad_chordam(r[0], XX, piscina, &textus);
 *
 * Vide lib/radices.worklog.md.
 */
/* <aedilis corpus="lib/radices.c"/> */
#ifndef RADICES_H
#define RADICES_H








nomen structura {
    Polynomium f;                  /* liber quadratis, primitivus, lc > 0 */
       Fractio infra;
       Fractio supra;              /* infra == supra -> radix rationalis
                                    * (non vice versa: 1/5 ex 5t - 1
                                    * intervallum est); aliter radix in
                                    * (infra, supra) APERTO, termini radices
                                    * f aliae esse possunt */
} RadixRealis;

/* omnes radices reales DISTINCTAE f, ordine crescente (multiplicitas
 * abiecta); *exitus in piscina, *numerus = 0 si nullae. FALSUM si f
 * nullum aut exponens negativus adest. */
b32
radices_reales (
     Polynomium   f,
        Piscina*  piscina,
    RadixRealis** exitus,
            i32*  numerus);

/* q exacte, radix (den t - num) */
RadixRealis
radix_ex_fractione (
    Fractio  q,
    Piscina* piscina);

/* signum(a - b) EXACTE; FALSUM solum si limes bisectionum superatus
 * (exitus non tangitur) */
b32
radix_compara (
    RadixRealis  a,
    RadixRealis  b,
        Piscina* piscina,
            s32* exitus);

/* signum h(a), h in Z[t] (exponentes >= 0): 0 sse a radix gcd(f, h);
 * FALSUM sicut radix_compara aut exponens negativus in h */
b32
radix_signum_polynomii (
     Polynomium  h,
    RadixRealis  a,
        Piscina* piscina,
            s32* exitus);

/* intervallum angustatum infra latitudinem (> 0); radix rationalis
 * manet punctum */
b32
radix_angusta (
    RadixRealis  a,
        Fractio  latitudo,
        Piscina* piscina,
    RadixRealis* exitus);

/* decimalis CERTA: n digiti post punctum, versus nullum truncata
 * ("-1.4142" pro -sqrt 2, n = 4; "-0.00" pro -0.003, n = 2 - signum
 * servatur; n = 0 sine puncto: "1"). FALSUM si plus quam ~600 digiti
 * (limes bisectionum, MMXLVIII) */
b32
radix_ad_chordam (
    RadixRealis  a,
            i32  digiti,
        Piscina* piscina,
         chorda* exitus);

#endif /* RADICES_H */
#line 1 "include/cyclotomia.h"
/* cyclotomia.h - Integri cyclotomici EXACTI: Z[zeta_n] = Z[t]/Phi_n(t)
 *
 * Elementum = polynomium in zeta reductum ad gradum < phi(n) (basis
 * potentiarum 1, zeta, ..., zeta^(phi(n)-1)), coefficientibus magnus:
 * exactum, sine exundatione (knotapel Cyc8 'long' habebat). Forma
 * canonica unica, ergo aequalitas = aequalitas polynomiorum.
 *
 * Contextus per n (Cyclotomia): n, phi(n), Phi_n et tabula zeta^k ->
 * basis (k < n), ergo reductio = lectio tabulae. cyclotomia_anulus
 * anulum pro matrix reddit (integrum, non corpus), sicut
 * anulus_residuorum.
 *
 * DECISIONES: elementa ab omnibus decisionibus EXACTE tractantur
 * (aequalis, est_radix, norma, vestigium, modulus_quadratus);
 * cyclotomicus_ad_ostendendum SOLUM ad ostendendum (f64 interius) -
 * textum reddit, non numeros, ne in comparationes intret.
 *
 * USUS:
 *   Cyclotomia* r8 = cyclotomia_creare(VIII, piscina);
 *   Cyclotomicus z = cyclotomicus_radix(r8, I, piscina);
 *   Cyclotomicus v;
 *   (vacuum)cyclotomicus_ex_polynomio(r8, jones, II, piscina, &v);
 *       (* V(zeta_8^2) = V(i) *)
 *
 * Vide lib/cyclotomia.worklog.md.
 */
/* <aedilis corpus="lib/cyclotomia.c"/> */
#ifndef CYCLOTOMIA_H
#define CYCLOTOMIA_H








#define CYCLOTOMIA_ORDO_MAXIMUS M

/* contextus Z[zeta_n]: opacus */
nomen structura Cyclotomia Cyclotomia;

/* Elementum SIGNATUM anulo suo: operationes anulum ex elementis legunt,
 * anuli mixti refutantur (aequalis(zeta_8, zeta_16) FALSUM; recensio
 * I). anulus NIHIL = INVALIDUM: ex anulis mixtis ortum, sicut NaN
 * propagatur (cyclotomicus_est_validum). Forma canonica: polynomium in
 * zeta, gradus < phi(n), imus >= 0. PRIVATUM - per functiones
 * legendum. */
nomen structura {
     constans Cyclotomia* anulus;
              Polynomium  p;
} Cyclotomicus;

/* Phi_n(t), n >= 1, per Phi_n = (t^n - 1) / prod_{d | n, d < n} Phi_d.
 * FALSUM si n nullus aut n > CYCLOTOMIA_ORDO_MAXIMUS. */
b32
polynomium_cyclotomicum (
           i32  n,
       Piscina* piscina,
    Polynomium* exitus);

/* contextus Z[zeta_n]; NIHIL si n < 1 aut n > CYCLOTOMIA_ORDO_MAXIMUS.
 * Memoria: tabulae potentiarum (n * phi(n) coefficientes, plerumque
 * parvi); n = 840: ~6 MB, creatio < 5 ms (recensio). */
Cyclotomia*
cyclotomia_creare (
         i32  n,
     Piscina* piscina);

/* n */
i32
cyclotomia_ordo (
    constans Cyclotomia* r);

/* phi(n) = dimensio basis */
i32
cyclotomia_gradus (
    constans Cyclotomia* r);

/* Phi_n contextus */
Polynomium
cyclotomia_polynomium (
    constans Cyclotomia* r);

/* anulus Z[zeta_n] (elementa Cyclotomicus, titulus "Z[zeta_n]") pro
 * matrix: integrum, non corpus, non Euclideus; divide_exacte per
 * normam. Una descriptio per contextum; elementa alterius anuli
 * refutantur (FALSUM). */
constans Anulus*
cyclotomia_anulus (
    constans Cyclotomia* r);


/* ==================================================
 * Constructio (contextum accipiunt)
 * ================================================== */

Cyclotomicus
cyclotomicus_nullum (
    constans Cyclotomia* r);

Cyclotomicus
cyclotomicus_integer (
    constans Cyclotomia* r,
                 Magnus  c,
                Piscina* piscina);

/* zeta^k, k quilibet (modulo n, negativus licet). Elementum tabulam
 * contextus PARTITUR (immutabile, sicut omnia polynomia): non copia. */
Cyclotomicus
cyclotomicus_radix (
    constans Cyclotomia* r,
                    s32  k,
                Piscina* piscina);

/* p(zeta^k): variabilis polynomii (exponentes Laurent quilibet)
 * substituitur et reducitur - e.g. Jones V(t) ad t = zeta^k, uncinus
 * ad A = zeta^k. k = 1: reductio simplex. Etiam immersio: a in
 * Z[zeta_8] -> Z[zeta_16] per cyclotomicus_ex_polynomio(r16, a.p, II)
 * (zeta_8 = zeta_16^2). b32 pro consensu (FALSUM si r NIHIL). */
b32
cyclotomicus_ex_polynomio (
    constans Cyclotomia* r,
             Polynomium  p,
                    s32  k,
                Piscina* piscina,
           Cyclotomicus* exitus);

/* summa c[k] zeta^k, k < numerus (quilibet: reducitur) - ex
 * arithmetica 's64' demonstrationum (Cyc8: numerus IV) */
b32
cyclotomicus_ex_s64 (
    constans Cyclotomia* r,
           constans s64* c,
                    i32  numerus,
                Piscina* piscina,
           Cyclotomicus* exitus);


/* ==================================================
 * Lectio
 * ================================================== */

/* VERUM nisi ex anulis mixtis ortum */
b32
cyclotomicus_est_validum (
    Cyclotomicus a);

/* contextus elementi; NIHIL si invalidum */
constans Cyclotomia*
cyclotomicus_anulus (
    Cyclotomicus a);

/* coefficiens zeta^j in basi (0 <= j < phi(n)); 0 si invalidum */
Magnus
cyclotomicus_coefficiens (
    Cyclotomicus a,
             i32 j);

/* phi(n) coefficientes in exitus (basis potentiarum); FALSUM si
 * invalidum aut coefficiens extra s64 (exitus tunc non fidus) */
b32
cyclotomicus_ad_s64 (
    Cyclotomicus  a,
             s64* exitus);


/* ==================================================
 * Arithmetica (anulus ex elementis; mixti -> invalidum)
 * ================================================== */

/* FALSUM si invalidum */
b32
cyclotomicus_est_nullum (
    Cyclotomicus a);

/* FALSUM si anuli diversi aut invalidum */
b32
cyclotomicus_aequalis (
    Cyclotomicus a,
    Cyclotomicus b);

/* VERUM si a in Z (gradus 0); *valor scribitur si non NIHIL */
b32
cyclotomicus_est_integer (
    Cyclotomicus  a,
          Magnus* valor);

Cyclotomicus
cyclotomicus_adde (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina);

Cyclotomicus
cyclotomicus_subtrahe (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina);

Cyclotomicus
cyclotomicus_nega (
    Cyclotomicus  a,
         Piscina* piscina);

Cyclotomicus
cyclotomicus_multiplica (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina);

/* a^e, e >= 0 (quadrando) */
Cyclotomicus
cyclotomicus_potentia (
    Cyclotomicus  a,
             i32  e,
         Piscina* piscina);

/* q = a / b in Z[zeta_n]: q = a * prod_{sigma != 1} sigma(b) / N(b).
 * FALSUM si b nullum, b non dividit a, aut anuli mixti (exitus non
 * tangitur). */
b32
cyclotomicus_divide_exacte (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina,
    Cyclotomicus* exitus);


/* ==================================================
 * Galois et invariantes EXACTAE
 * ================================================== */

/* sigma_j: zeta -> zeta^j; FALSUM si gcd(j, n) != 1 aut invalidum */
b32
cyclotomicus_automorphismus (
    Cyclotomicus  a,
             s32  j,
         Piscina* piscina,
    Cyclotomicus* exitus);

/* conjugatum complexum = sigma_{-1} */
Cyclotomicus
cyclotomicus_conjugatum (
    Cyclotomicus  a,
         Piscina* piscina);

/* N(a) = prod_{gcd(j, n) = 1} sigma_j(a), in Z; FALSUM si invalidum */
b32
cyclotomicus_norma (
    Cyclotomicus  a,
         Piscina* piscina,
          Magnus* exitus);

/* Tr(a) = summa_{gcd(j, n) = 1} sigma_j(a), in Z; FALSUM si invalidum
 * (vestigium 0 legitimum est - ergo b32) */
b32
cyclotomicus_vestigium (
    Cyclotomicus  a,
         Piscina* piscina,
          Magnus* exitus);

/* |a|^2 = a * conjugatum(a): realis (in Z[zeta + zeta^-1]), EXACTUM */
Cyclotomicus
cyclotomicus_modulus_quadratus (
    Cyclotomicus  a,
         Piscina* piscina);

/* a = signum * zeta^k? (radix unitatis, angulus EXACTUS). VERUM et
 * *signum (+1/-1), *k (0 <= k < n) - signum +1 praefertur (n par:
 * -zeta^k = zeta^(k + n/2)). FALSUM si non radix aut invalidum. */
b32
cyclotomicus_est_radix (
    Cyclotomicus  a,
             s32* signum,
             i32* k);


/* ==================================================
 * Textus
 * ================================================== */

/* "z^3 - 2z + 1": littera 'z' = zeta_n; nullum = "0"; invalidum =
 * "invalidum" */
chorda
cyclotomicus_ad_chordam (
    Cyclotomicus  a,
         Piscina* piscina);

/* polynomium in 'z' (Laurent licet), reductum */
b32
cyclotomicus_ex_chorda (
    constans Cyclotomia* r,
                 chorda  textus,
                Piscina* piscina,
           Cyclotomicus* exitus);

/* AD OSTENDENDUM SOLUM: "0.7071 + 0.7071i" cum digitis decimalibus
 * datis (maximum XV; |x| >= 10^15 per notationem e). f64 interius
 * (cos/sin): error absolutus ~ phi(n) * max|c| * 2^-52 - cancellatio
 * TACITA est ((sqrt2 - 1)^60 = 1e-23 ut milliones ostenditur). Numquam
 * ad decisiones: aequalis, est_radix, modulus_quadratus exacta sunt.
 * invalidum = "invalidum". */
chorda
cyclotomicus_ad_ostendendum (
    Cyclotomicus  a,
             i32  digiti,
         Piscina* piscina);

#endif /* CYCLOTOMIA_H */
#line 1 "include/plexus.h"
/* plexus.h - Greges braidarum (Artin) B_n, EXACTE: verba, actio in
 * gregem liberum F_n, aequalitas
 *
 * VERBUM: litterae s32, +i = sigma_i, -i = sigma_i^-1, 1 <= i < fila;
 * verbum vacuum = identitas. Plexus valor est (fila, longitudo,
 * litterae); litterae in piscina vocantis, numquam mutantur. Structura
 * publica est: plexus manu factus per plexus_validus probatur, et omnis
 * actio (permutatio, imagines, aequalitas, ansae, in_maiore) plexum
 * invalidum refutat (FALSUM), numquam extra alveum legit.
 *
 * CAUTIO TYPORUM: fila et longitudo i32 sunt (SINE signo), litterae
 * s32. Gyrus retrogradus 'k = longitudo - 1; k >= 0; k--' cum k i32
 * numquam finit: indicem s32 adhibe.
 *
 * FILA: 2 <= fila <= PLEXUS_FILA_MAXIMA (2^20); maius refutatur
 * (FALSUM), numquam truncatur.
 *
 * PERMUTATIO: sigma_i positiones i et i+1 (ab 1 numeratas) commutat,
 * litteris a sinistra ad dextram actis. plexus_permutatio ponit
 * imago[k] = filum (positione initiali, ab 0) quod in positione k (ab
 * 0) finit. Ergo imago_{pq}[k] = imago_p[imago_q[k]].
 *
 * ACTIO IN F_n (Artin): x_1 ... x_n generatores liberi;
 *   sigma_i:    x_i -> x_i x_{i+1} x_i^-1,  x_{i+1} -> x_i
 *   sigma_i^-1: x_i -> x_{i+1},             x_{i+1} -> x_{i+1}^-1 x_i
 *                                                      x_{i+1}
 * ceteri fixi. Plexui a_1 ... a_m automorphismus phi = phi_{a_1} o ...
 * o phi_{a_m} (compositio substitutionum), ergo phi_{pq} = phi_p o
 * phi_q (actio SINISTRA). Actio FIDELIS est (Artin 1925): p = q in B_n
 * si et solum si phi_p = phi_q, ergo plexus_aequales problema verborum
 * EXACTE solvit. plexus_aequales est REFERENTIA (Artin); methodus
 * celerior per ansas: plexus_aequales_ansis.
 *
 * LIMES: imagines verborum exponentialiter crescere possunt (|phi(x)|
 * usque ad 3^m). Omnis functio actionis limitem longitudinis summae
 * accipit; limite superato FALSUM redditur, numquam responsum
 * truncatum.
 *
 * MEMORIA: effecta in piscina vocantis; temporaria in piscinis
 * propriis, destructis ante reditum. Memoria propria deficiens ->
 * FALSUM in actione in F_n; in piscina vocantis (et intra Magnus)
 * allocatio deficiens fatalis est, ut in omni piscina. Purgamenta
 * verborum contrahuntur cum piscina propria quater crevit post
 * contractionem ultimam; plexus_contractiones numerat.
 * FALSUM -> exitus non tangitur.
 *
 * USUS:
 *   Plexus p;
 *   b32    aequales;
 *   (vacuum)plexus_ex_chorda(III, chorda_ex_literis("1 2 1", piscina),
 *       piscina, &p);
 *   si (plexus_aequales(p, q, M, piscina, &aequales)) ...
 *
 * Consilium: project-specs/plexus-plan.md. Vide lib/plexus.worklog.md.
 */
/* <aedilis corpus="lib/plexus.c"/> */
#ifndef PLEXUS_H
#define PLEXUS_H










#define PLEXUS_FILA_MAXIMA              1048576
/* Delta longitudinem n(n-1)/2 habet: n <= 65536 ut in 2^31 capiat */
#define PLEXUS_FILA_DIMIDIAE_TORSIONIS  65536

nomen structura {
     i32  fila;         /* 2 <= n <= PLEXUS_FILA_MAXIMA */
     i32  longitudo;
     s32* litterae;     /* +i sigma_i, -i sigma_i^-1, 1 <= i < fila */
} Plexus;

/* verbum gregis liberi F_n: +j = x_j, -j = x_j^-1, j >= 1; semper
 * libere reductum (nulla littera iuxta inversam suam) */
nomen structura {
     i32  longitudo;
     s32* litterae;
} VerbumLiberum;


/* ==================================================
 * Verba
 * ================================================== */

/* litterae copiantur; FALSUM si fila extra 2..PLEXUS_FILA_MAXIMA aut
 * littera 0 vel |littera| >= fila */
b32
plexus_creare (
                i32  fila,
       constans s32* litterae,
                i32  numerus,
            Piscina* piscina,
             Plexus* exitus);

/* textus: integri signati, character ' ' (solum) separati ("1 -2 1");
 * spatia multiplicia, initialia, finalia et cifrae '0' initiales
 * admittuntur ("-0002" = -2); '+', tabula, linea nova non. Vacuus aut
 * spatia sola = identitas. FALSUM si malformatus aut littera extra
 * 1..fila-1 (exitus non tangitur, piscina refecta). Plexus in alveo
 * analysis ipso: nihil aliud in piscina relinquitur. */
b32
plexus_ex_chorda (
         i32  fila,
      chorda  textus,
     Piscina* piscina,
      Plexus* exitus);

/* "1 -2 1"; identitas "" */
chorda
plexus_ad_chordam (
     Plexus  p,
    Piscina* piscina);

/* a_m^-1 ... a_1^-1 */
Plexus
plexus_inversum (
     Plexus  p,
    Piscina* piscina);

/* p deinde q; FALSUM si fila diversa aut longitudo summa > 2^31 - 1 */
b32
plexus_iungere (
     Plexus  p,
     Plexus  q,
    Piscina* piscina,
     Plexus* exitus);

/* reductio libera: sigma_i sigma_i^-1 et sigma_i^-1 sigma_i delentur
 * (solum - relationes braidarum non adhibentur) */
Plexus
plexus_reducere (
     Plexus  p,
    Piscina* piscina);

/* summa signorum litterarum (homomorphismus B_n -> Z) */
s64
plexus_summa_exponentium (
    Plexus p);

/* VERUM si plexus validus: fila 2..PLEXUS_FILA_MAXIMA, litterae non
 * NIHIL si longitudo > 0, omnis littera 1 <= |littera| < fila. O(m). */
b32
plexus_validus (
    Plexus p);

/* imago[0 .. fila-1]: vide PERMUTATIO supra. FALSUM si plexus
 * invalidus (imago non tangitur) */
b32
plexus_permutatio (
    Plexus  p,
       i32* imago);

/* Delta = (sigma_1 ... sigma_{n-1}) (sigma_1 ... sigma_{n-2}) ...
 * sigma_1, dimidia torsio Garside; Delta^2 centrum B_n generat.
 * FALSUM si fila extra 2..PLEXUS_FILA_DIMIDIAE_TORSIONIS */
b32
plexus_dimidia_torsio (
         i32  fila,
     Piscina* piscina,
      Plexus* exitus);


/* ==================================================
 * Actio in gregem liberum (Artin) et aequalitas
 * ================================================== */

/* phi_p(x_j), 1 <= j <= fila. limes: longitudo maxima, post quamque
 * litteram, summae imaginum omnium generatorum, et cuiusque producti
 * ante reductionem (supra 2^31 - 1 non repraesentabilis); limes etiam
 * memoriam propriam terminat (~ 4 * limes octeti per productum).
 * FALSUM si plexus invalidus, j extra, limes superatus, aut memoria
 * propria deficit. Temporaria in piscinis propriis. */
b32
plexus_imago_libera (
           Plexus  p,
              i32  j,
              i64  limes,
          Piscina* piscina,
    VerbumLiberum* exitus);

/* imagines omnium generatorum: (*exitus)[j - 1] = phi_p(x_j), j = 1 ..
 * fila; idem limes et eadem refutatio quae plexus_imago_libera */
b32
plexus_imagines_liberae (
            Plexus   p,
               i64   limes,
           Piscina*  piscina,
     VerbumLiberum** exitus);

/* *aequales = (p == q in B_n): r = reductio(p q^-1); r vacuum -> VERUM
 * statim (sine limite); summa exponentium et permutatio r prius
 * (necessaria), deinde phi_r = identitas. FALSUM si plexus invalidus,
 * fila diversa, aut limes superatus (tunc *aequales non tangitur).
 * piscina ut alveus temporarius: usus ad statum initii reficitur. */
b32
plexus_aequales (
     Plexus  p,
     Plexus  q,
        i64  limes,
    Piscina* piscina,
        b32* aequales);

/* "1 2 -1"; identitas "" */
chorda
verbum_liberum_ad_chordam (
    VerbumLiberum  w,
          Piscina* piscina);

/* plexus p (n fila) in B_fila: littera +-i -> +-(i + translatio).
 * FALSUM si p invalidus, fila extra 2..PLEXUS_FILA_MAXIMA aut p.fila +
 * translatio > fila */
b32
plexus_in_maiore (
     Plexus  p,
        i32  fila,
        i32  translatio,
    Piscina* piscina,
     Plexus* exitus);


/* ==================================================
 * Actio Dynnikov in ansas (laminae integrae)
 * ==================================================
 *
 * ANSA: lamina integra (multicurva) in disco cum fila punctis, per
 * coordinatas Dynnikov u = (a_1 .. a_{n-2}, b_1 .. b_{n-2}), n = fila
 * >= 3. Omnis vector integer laminam describit (Dynnikov 2002;
 * citatum, non hic probatum).
 * Valores Magnus: crescentia numquam superfluit.
 *
 * FONS: J.-L. Thiffeault, "Braids of entangled particle trajectories",
 * Chaos 20, 017516 (2010), arXiv:0906.3647: coordinatae aeq. (3)-(4),
 * L(u) aeq. (5)-(6), regulae aeq. (7)-(15), eaedem in appendice A.2
 * (loopsigma, loopinter) - ambae transcriptiones congruunt.
 * CONVENTIO fontis: sigma_i puncta i et i+1 HOROLOGII SENSU commutat
 * (Birman contra); verbum ansae a sinistra ad dextram adhibetur, ergo
 * actio DEXTRA: u . (pq) = (u . p) . q (probatum), contra actionem in
 * F_n quae sinistra est.
 *
 * DISCUS: in D_n torsio plena Delta^2 (torsio circa marginem) omnem
 * ansam clausam fixam relinquit, ergo actio in D_n Delta^2 non videt.
 * plexus_aequales_ansis plexum in D_{n+2} (punctis mediis) agit, ubi
 * Delta^2 punctorum mediorum non centralis est.
 *
 * Regulae per partes lineares sunt: f+ = max(f, 0), f- = min(f, 0);
 * omnis generator stratum ReLU integrum est. */

nomen structura {
        i32  fila;      /* puncta, >= 3 */
     Magnus* a;         /* fila - 2 */
     Magnus* b;         /* fila - 2 */
} Ansa;

/* VERUM si ansa valida: fila 3..PLEXUS_FILA_MAXIMA, a et b non NIHIL.
 * Omnis functio ansarum ansam invalidam refutat (FALSUM; sectiones 0;
 * ad_chordam ""), numquam extra alveum legit. */
b32
ansa_valida (
    Ansa u);

/* coordinatae copiantur; FALSUM si fila extra 3..PLEXUS_FILA_MAXIMA */
b32
ansa_ex_coordinatis (
                i32  fila,
    constans Magnus* a,
    constans Magnus* b,
            Piscina* piscina,
               Ansa* exitus);

/* ansa simplex circa puncta k et k+1 (1 <= k < fila): a = 0, b_k = 1
 * (si k <= n-2), b_{k-1} = -1 (si k >= 2), ceterae 0 */
b32
ansa_circa (
         i32  fila,
         i32  k,
     Piscina* piscina,
        Ansa* exitus);

/* ansa post plexum (litterae a sinistra; actio dextra). FALSUM si p
 * invalidus, ansa invalida (fila < 3, coordinatae NIHIL) aut p.fila !=
 * u.fila. Temporaria in piscina propria; effectum in piscina vocantis.
 */
b32
ansa_agere (
        Ansa  u,
      Plexus  p,
     Piscina* piscina,
        Ansa* exitus);

/* L(u): sectiones minimae ansae cum linea horizontali per puncta (aeq.
 * 5-6); ansa invalida -> 0 */
Magnus
ansa_sectiones (
        Ansa  u,
     Piscina* piscina);

b32
ansae_aequales (
    Ansa u,
    Ansa v);

/* "(a_1 ... a_{n-2}; b_1 ... b_{n-2})" */
chorda
ansa_ad_chordam (
        Ansa  u,
     Piscina* piscina);

/* *aequales = (p == q in B_n) per actionem in ansas circa puncta
 * vicina D_{n+2}: r = p q^-1 in punctis mediis, aequales si r omnes
 * ansas E_1 .. E_{n+1} fixas relinquit. Congruentia cum Artin
 * (plexus_aequales) exhaustive probata pro verbis brevibus (B_3 <= 6,
 * B_4 <= 5, B_5 <= 4 litterae; probatio_plexus); fidelitas generalis
 * hic non probatur - plexus_aequales manet referentia (familiae
 * minores, e.g. sine E_1 aut in D_{n+1}, eandem partitionem exhaustive
 * dant - recensio T5, N1 - sed familia plena servatur). Ut
 * plexus_aequales:
 * r vacuum, summa exponentium, permutatio prius; omnis ansa sub nota
 * sua (memoria O(n)). FALSUM si plexus invalidus, fila diversa, aut
 * fila > PLEXUS_FILA_MAXIMA - 2. piscina ad statum initii reficitur. */
b32
plexus_aequales_ansis (
     Plexus  p,
     Plexus  q,
    Piscina* piscina,
        b32* aequales);


/* ==================================================
 * Repraesentatio Burau reducta
 * ==================================================
 *
 * B(p) matrix (n-1) x (n-1) super Z[t, t^-1] (ANULUS_POLYNOMIORUM),
 * B(pq) = B(p) B(q). Generatores (forma transposita formae vulgaris,
 * eadem determinantia et polynomia characteristica):
 *   n = 2:       sigma_1 -> [-t]
 *   sigma_1:     columnae 1, 2 per [[-t, 0], [1, 1]]
 *   sigma_{n-1}: columnae n-2, n-1 per [[1, t], [0, -t]]
 *   sigma_i (1 < i < n-1): columnae i-1, i, i+1 per
 *                [[1, t, 0], [0, -t, 0], [0, 1, 1]]
 * ergo B(p sigma_i) ex B(p) mutatione columnae UNIUS. det B(p) =
 * (-t)^(summa exponentium). Fidelis in B_3 (Magnus-Peluso, citatum;
 * congruentia cum Artin exhaustive probata ad longitudinem 6, B_4 ad
 * 5), non fidelis pro n >= 5 (citatum).
 *
 * LIMES FILORUM: matrix (n-1)^2 polynomiorum; omnis littera O(n)
 * operationes. fila <= PLEXUS_FILA_BURAU. */

#define PLEXUS_FILA_BURAU 4096

/* B(p) super Z[t, t^-1]. FALSUM si p invalidus, fila >
 * PLEXUS_FILA_BURAU, aut exponens polynomii extra fines (verbum ~2^30
 * litterarum) */
b32
plexus_burau (
     Plexus  p,
    Piscina* piscina,
     Matrix* exitus);

/* B(p) ad t = -1 super Z (ANULUS_INTEGRORUM), computata directe
 * (integri magni, sine polynomiis) */
b32
plexus_burau_minus_uno (
     Plexus  p,
    Piscina* piscina,
     Matrix* exitus);

/* polynomium Alexander clausurae p (nodi aut catenae, una variabilis):
 * det(I - B(p)) (1 - t) / (1 - t^n), forma normalis polynomium_normale
 * (imus 0, coefficiens imus > 0; definitum ad +-t^k). Catena
 * disiuncta -> 0. */
b32
plexus_alexander (
         Plexus  p,
        Piscina* piscina,
     Polynomium* exitus);

/* limes inferior dilatationis: max(1, max |lambda| super valores
 * proprios REALES lambda matricis B(p) ad t = -1), radix realis exacta.
 * Radius spectralis B(-1) dilatationem non superat (Kolev 1989,
 * citatum; comparatum cum incremento ansarum in braidis fortuitis,
 * non probatum); maximum radicum realium radium non superat, ergo
 * limes inferior manet. In B_3
 * (matrix 2 x 2 integra) radius ipse et dilatatio. */
b32
plexus_burau_radix_maxima (
          Plexus  p,
         Piscina* piscina,
     RadixRealis* exitus);


/* Certificatum entropiae per Burau in radicibus unitatis: VERUM in
 * *maior si radius spectralis B(zeta) > 1 pro aliqua radice primitiva
 * zeta ordinis 'ordo' (2 <= ordo <= 12), EXACTE. Tunc entropia > 0:
 * radius spectralis B(zeta), |zeta| = 1, dilatationem non superat
 * (Kolev 1989; Band et Boyland 2007; citatum).
 * Via: B(t) in Z[zeta] reducitur (cyclotomia) et per restrictionem
 * scalarium matrix integra R ((n-1) phi(ordo) quadrata) fit, cuius
 * valores proprii sunt valores B(zeta^k) omnium coniugatorum
 * Galois; polynomium characteristicum R monicum est, terminus
 * constans +-1 (det B = (-t)^e unitas). Kronecker (1857): polynomium
 * monicum integrum cuius radices omnes in disco unitatis iacent
 * productum polynomiorum cyclotomicorum est - ergo factoribus
 * cyclotomicis divisis residuum non trivialum radicem |z| > 1 habet.
 * ordo 2 = t = -1 (in B_4 idem ac plexus_burau_radix_maxima > 1, quia
 * p(1) = 0 semper; in B_n generali non). FALSUM si p invalidus, ordo
 * extra 2..12, aut (fila - 1) phi(ordo) > PLEXUS_BURAU_CIRCULI (tunc
 * *maior non tangitur). Temporaria in piscina propria; nihil in
 * vocantis. */
#define PLEXUS_BURAU_CIRCULI 22

b32
plexus_burau_radius_maior (
     Plexus  p,
        i32  ordo,
        b32* maior);


/* ==================================================
 * Entropia per incrementum ansarum
 * ==================================================
 *
 * (*exitus)[m] = L(u . p^m), m = 0 .. periodi (periodi + 1 valores
 * Magnus, exacti, in piscina vocantis). Ratio L_{m+1} / L_m ad
 * dilatationem lambda tendit si p pseudo-Anosov est et u non reducit
 * (Moussafir 2006; Thiffeault 2010 Sect. III B: entropia = log lambda
 * per periodum); crescentia linearis aut nulla pro periodicis et
 * reducibilibus. Ansa singula limitem INFERIOREM incrementi dat (ansa
 * reducens non crescit). Dilatatio exacta (matrices viarum, Hall et
 * Yurttas) hic non computatur (consilium: dilata). Temporaria in
 * piscinis propriis alternantibus (apex per plexus_apex_proprius).
 * FALSUM si p aut u invalidi, fila diversa, aut periodi > 2^31 - 2. */

b32
plexus_incrementum (
     Plexus   p,
       Ansa   u,
        i32   periodi,
    Piscina*  piscina,
     Magnus** exitus);

/* contractiones piscinarum propriarum (imagines in F_n, ansae) ab
 * initio processus: instrumentum probationis et mensurae, non status
 * logicus (nullum effectum mutat) */
i64
plexus_contractiones (vacuum);

/* apex usus (octeti) piscinae propriae cuiusvis (imagines, ansae,
 * Burau, incrementum) ab initio aut ultima purgatione - notatur cum
 * piscina propria destruitur aut vacatur. Instrumentum probationis et
 * mensurae, sicut plexus_contractiones. */
memoriae_index
plexus_apex_proprius (vacuum);

vacuum
plexus_apicem_purgare (vacuum);

#endif /* PLEXUS_H */
#line 1 "include/sors.h"
/* sors.h - Numeri pseudo-fortuiti SEMINABILES et deterministici
 *
 * Idem semen = eadem series in omni machina, compilatore et versione:
 * integri soli (PCG32 pro rivo, SplitMix64 pro semine expandendo), ergo
 * replay (ictus aspergilli), artificia proceduralia et probationes
 * fuzzing reproducibiles sunt. Distincta a moneta (/dev/urandom pro
 * identitatibus - tuta sed NON seminabilis).
 *
 * Sors valor purus est (XVI octeti): sine piscina, sine statu globali;
 * copiatur et in ictu servatur libere.
 *
 * USUS:
 *   Sors s;
 *   sors_seminare(&s, semen, ZEPHYRUM);
 *   x = sors_intra(&s, C);            (* [0, C) sine inclinatione *)
 *   Sors ictus = sors_derivare(&s, numerus_ictus);   (* rivus filius *)
 */
/* <aedilis corpus="lib/sors.c"/> */
#ifndef SORS_H
#define SORS_H



nomen structura {
    i64 status;
    i64 incrementum;   /* impar semper - rivum (seriem) eligit */
} Sors;

/* semen: quilibet valor (SplitMix64 eum miscet); series: numerus rivi
 * independentis (0, 1, 2...) - eadem semen, series diversae = rivi
 * disiuncti */
vacuum
sors_seminare (
    Sors* s,
     i64  semen,
     i64  series);

/* rivus filius ex parente et clave - PURUS in parente (parens non
 * mutatur): filius clavis k idem est quotcumque alii ante derivati
 * sunt. Pro seminibus per ictum, per artificium, per casum
 * probationis */
Sors
sors_derivare (
    constans Sors* s,
              i64  clavis);

/* XXXII bits */
i32
sors_proximum (
    Sors* s);

/* [0, limes) sine inclinatione (Lemire); limes 0 -> 0 */
i32
sors_intra (
    Sors* s,
     i32  limes);

/* [imum, summum] inclusive; imum > summum -> imum */
s32
sors_inter (
    Sors* s,
     s32  imum,
     s32  summum);

/* [0, 1) ex LIII bits */
f64
sors_f64 (
    Sors* s);

/* [0, 1) ex XXIV bits */
f32
sors_f32 (
    Sors* s);

/* VERUM cum probabilitate numerator/denominator; denominator 0 ->
 * FALSUM */
b32
sors_casu (
    Sors* s,
     i32  numerator,
     i32  denominator);

/* Fisher-Yates: numerus elementa magnitudinis 'magnitudo_elementi'
 * permutat (octeti permutantur - nulla allocatio) */
vacuum
sors_miscere (
      Sors* s,
    vacuum* elementa,
       i32  numerus,
       i32  magnitudo_elementi);

/* LEGATUM: LCG classicus ANSI (status * 1103515245 + 12345) - statum
 * provehit et reddit. Ut probationes totalitatis bibliotheca utantur
 * sine casibus generatis mutatis */
i32
sors_lcg_classicus (
    i32* status);

#endif /* SORS_H */
#line 1 "include/chorda_aedificator.h"
/* chorda_aedificator.h - chordas accumulare (string builder) */
#ifndef CHORDA_AEDIFICATOR_H
#define CHORDA_AEDIFICATOR_H






/* ==================================================
 * ChordaAedificator - Accumulatio efficax chordarum
 *
 * Ad chordas aedificandas quando magnitudo finalis ignota.
 * Destinatus ad generationem texti structurati:
 * - INI/JSON/XML aedificatores (casus primarii)
 * - Formatatio diariorum
 * - Generatio texti ex structuris datorum
 *
 * EXEMPLUM:
 *   				   Piscina* p = piscina_generare_dynamicum("probatio", 4096);
 *   ChordaAedificator* a = chorda_aedificator_creare(p, 256);
 *
 *   chorda_aedificator_appendere_literis(a, "nomen");
 *   chorda_aedificator_appendere_character(a, ':');
 *   chorda_aedificator_appendere_integer(a, 42);
 *   chorda_aedificator_appendere_lineam_novam(a);
 *
 *   chorda fructus = chorda_aedificator_finire(a);
 *   piscina_destruere(p);
 *
 * PROPRIETATES:
 * - Crescentia automatica capacitatis (duplicatur quando plenus)
 * - Indentationis observatio (pro impressione pulchra)
 * - Effugium specificum formati (JSON effugium inclusum)
 * - Reutilizabilis per reset()
 * - Omnes allocationes ex Piscina data
 *
 * ================================================== */

nomen structura ChordaAedificator ChordaAedificator;


/* ==================================================
 * Creatio / Destructio
 * ================================================== */

ChordaAedificator*
chorda_aedificator_creare (
           Piscina* piscina,
    memoriae_index  capacitas_initialis);

vacuum
chorda_aedificator_destruere (
    ChordaAedificator* aedificator);


/* ==================================================
 * Appendere - Singularis Character
 * ================================================== */

b32
chorda_aedificator_appendere_character (
    ChordaAedificator* aedificator,
            character  c);


/* ==================================================
 * Appendere - Chordae (Chordae et C-chordae)
 * ================================================== */

b32
chorda_aedificator_appendere_literis (
     ChordaAedificator* aedificator,
    constans character* cstr);

b32
chorda_aedificator_appendere_chorda (
    ChordaAedificator* aedificator,
               chorda  s);


/* ==================================================
 * Appendere - Numeri (Integri)
 * ================================================== */

b32
chorda_aedificator_appendere_s32 (
    ChordaAedificator* aedificator,
                  s32  n);

b32
chorda_aedificator_appendere_i32 (
    ChordaAedificator* aedificator,
                  i32  n);


/* ==================================================
 * Appendere - Numeri (Puncta Fluitantia)
 *
 * decimales: numerus locorum decimalium ostendorum
 * ================================================== */

b32
chorda_aedificator_appendere_f64 (
    ChordaAedificator* aedificator,
                  f64  n,
                  i32  decimales);

/* appendere_repetita: appende characterem n vicibus
 * Utile ad padding vel indentationem */
b32
chorda_aedificator_appendere_repetita (
    ChordaAedificator* aedificator,
            character  c,
                  i32  numerus);

/* appendere_hex_i32: appende integrum ut hexadecimale (minusculae, sine 0x)
 * Utile ad debugging vel IDs */
b32
chorda_aedificator_appendere_hex_i32 (
    ChordaAedificator* aedificator,
                  i32  n);


/* ==================================================
 * Appendere - Evasus (Specificus Formati)
 *
 * appendere_evasus_json: Effugium chordae JSON
 *   - Tractat: citationem, virgulam inversam, characteres imperantes
 *   - Proprie effugit ad usum in chordis JSON
 * ================================================== */

b32
chorda_aedificator_appendere_evasus_json (
    ChordaAedificator* aedificator,
               chorda  s);

b32
chorda_aedificator_appendere_literis_evasus_json (
     ChordaAedificator* aedificator,
    constans character* cstr);


/* ==================================================
 * Appendere - Spatium Album et Structura
 * ================================================== */

b32
chorda_aedificator_appendere_lineam_novam (
    ChordaAedificator* aedificator);

/* appendere_indentationem: appende N spatia (pro gradu currenti)
 * ubi N = gradus * spatia_per_gradum (typice 2 vel 4)
 *
 * Trade indentatio_gradus ab aedificatore, vel specifica manualiter */
b32
chorda_aedificator_appendere_indentationem (
    ChordaAedificator* aedificator,
                  i32  gradus);


/* ==================================================
 * Indentationis Observatio (pro impressione pulchra)
 *
 * Aedificatores observant internum gradum indentationis.
 * push_indentationem(): incrementa gradum
 * pop_indentationem(): decrementa gradum
 * indentatio_gradus(): interroga gradum currentem
 *
 * Modus communis:
 *   chorda_aedificator_appendere_literis(a, "{");
 *   chorda_aedificator_appendere_lineam_novam(a);
 *   chorda_aedificator_push_indentationem(a);
 *   chorda_aedificator_appendere_indentationem(a,
 *       chorda_aedificator_indentatio_gradus(a));
 *   ... contenta appendere ...
 *   chorda_aedificator_pop_indentationem(a);
 * ================================================== */

vacuum
chorda_aedificator_push_indentationem (
    ChordaAedificator* aedificator);

vacuum
chorda_aedificator_pop_indentationem (
    ChordaAedificator* aedificator);

i32
chorda_aedificator_indentatio_gradus (
    ChordaAedificator* aedificator);


/* ==================================================
 * Quaestio - Interrogatio Status
 * ================================================== */

/* longitudo: longitudo currens chordae accumulatae */
memoriae_index
chorda_aedificator_longitudo (
    ChordaAedificator* aedificator);

/* spectare: vide contentum currentem sine finiendo
 * Reddit chordam spectationem buffer currenti.
 * Validus solum usque ad proximam mutationem. */
chorda
chorda_aedificator_spectare (
    ChordaAedificator* aedificator);

/* truncare: longitudinem ad valorem minorem reducere (numquam
 * auget; capacitas servatur). Pro redditione optimistica cum
 * reversione: signum = longitudo(), scribe, si displicet
 * truncare(signum). */
vacuum
chorda_aedificator_truncare (
    ChordaAedificator* aedificator,
       memoriae_index  longitudo_nova);


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

/* reset: purga contentum, serva capacitatem allocatam
 * Utile ad reutilizandum aedificatorem pro chordis multiplicibus */
vacuum
chorda_aedificator_reset (
    ChordaAedificator* aedificator);

/* finire: converte aedificatorem ad chordam
 * Transfert dominium chordae accumulatae ad vocantem.
 * Aedificator destruitur post finire.
 * Vocans debet finaliter deallocare per piscinam. */
chorda
chorda_aedificator_finire (
    ChordaAedificator* aedificator);


/* ==================================================
 * Constantae Configurationis
 * ================================================== */

/* CHORDA_AEDIFICATOR_INDENTATIO_SPATIA
 * Numerus spatiorum per gradum indentationis (typice 2 vel 4) */
#define CHORDA_AEDIFICATOR_INDENTATIO_SPATIA II


#endif /* CHORDA_AEDIFICATOR_H */
#line 1 "include/radices_interna.h"
/* radices_interna.h - INTERNA radicum: apices piscinarum temporariarum.
 * NON API - solum lib/radices.c et probationes id includunt;
 * consumptores radices.h solum. Praefixum 'radicum_' (genetivus) ab API
 * 'radix_' / 'radices_' distinguit.
 *
 * Probatio memoriae piscinam vocantis solam metiri non debet: lucrum
 * verum (refectiones in piscinis temporariis) ibi invisibile est
 * (recensio radicum II, L1: tres plantae sine refectione virides). */

#ifndef RADICES_INTERNA_H
#define RADICES_INTERNA_H



/* apices ad nihilum */
vacuum
radicum_apices_purgare (vacuum);

/* apex maximus (octeti, piscina_summa_apex_usus) acervorum VCA ab
 * ultima purgatione */
memoriae_index
radicum_apex_acervi (vacuum);

/* apex maximus piscinarum officinae (status, opus) ab ultima
 * purgatione */
memoriae_index
radicum_apex_officinae (vacuum);

#endif /* RADICES_INTERNA_H */
#line 1 "include/congruentia.h"
/* congruentia.h - Arithmetica modularis EXACTA (Z/n, n < 2^32)
 *
 * "Si numerus a numerorum b, c differentiam metitur, b et c secundum a
 * congrui dicuntur" (Gauss, Disquisitiones Arithmeticae, art. 1).
 * Residua 0 <= x < n in i32 (insignatus), modulus explicitus: nulla
 * structura, ergo ansae calidae (gradus et determinantes modulo p,
 * restitutio Sinica) sine sumptu. Producta in i64: n^2 < 2^64, C89
 * sine 128 bitis.
 *
 * Modulus n quilibet 2 <= n < 2^32 (non solum primi): inversa FALSUM
 * reddit si mdc(a, n) != 1. Argumenta arithmeticae iam reducta esse
 * debent (0 <= a < n) - ex_s64 / ex_magno reducunt. Contractus non
 * custoditur (ansa calida): n == 0 in arithmetica divisio per nullum
 * est (indefinitum); argumenta non reducta effectum falsum dant.
 * ex_s64, ex_magno n == 0 -> 0 definiunt.
 *
 * USUS:
 *   i32 p = congruentia_primus_infra(0xFFFFFFFFU);
 *   i32 x = congruentia_ex_s64(-17, p);
 *   i32 inversa;
 *   si (congruentia_inversa(x, p, &inversa)) ...
 *
 * Vide lib/congruentia.worklog.md.
 */
/* <aedilis corpus="lib/congruentia.c"/> */
#ifndef CONGRUENTIA_H
#define CONGRUENTIA_H






/* ==================================================
 * Reductio
 * ================================================== */

/* x mod n, 0 <= r < n, etiam x negativus (S64 imus incluso) */
i32
congruentia_ex_s64 (
    s64 x,
    i32 n);

/* x mod n per membra magni (magnus_residuum_parvum), sine piscina */
i32
congruentia_ex_magno (
    Magnus x,
       i32 n);


/* ==================================================
 * Arithmetica (argumenta reducta: 0 <= a, b < n)
 * ================================================== */

i32
congruentia_adde (
    i32 a,
    i32 b,
    i32 n);

i32
congruentia_subtrahe (
    i32 a,
    i32 b,
    i32 n);

i32
congruentia_multiplica (
    i32 a,
    i32 b,
    i32 n);

/* a^e mod n; 0^0 = 1 */
i32
congruentia_potentia (
    i32 a,
    i64 e,
    i32 n);

/* a^-1 mod n; FALSUM si mdc(a, n) != 1 aut n < 2 (exitus non
 * tangitur) */
b32
congruentia_inversa (
     i32  a,
     i32  n,
     i32* exitus);

/* representans symmetricus: -n/2 < x <= n/2 */
s64
congruentia_symmetrica (
    i32 a,
    i32 n);


/* ==================================================
 * Primi
 * ================================================== */

/* Miller-Rabin deterministicus (bases 2, 7, 61: exactus pro n <
 * 4759123141 > 2^32) */
b32
congruentia_est_primus (
    i32 n);

/* primus maximus < limes; 0 si nullus (limes <= 2). Series pro
 * reconstructione: p0 = primus_infra(0xFFFFFFFF), p1 =
 * primus_infra(p0), ... */
i32
congruentia_primus_infra (
    i32 limes);


/* ==================================================
 * Reconstructio Sinica (CRT)
 * ================================================== */

/* x mod M = prod(moduli) ex residuis per Garner: 0 <= x < M, aut si
 * symmetricus (verum) representans symmetricus -M/2 < x <= M/2.
 * FALSUM si moduli
 * non bini coprimi, modulus < 2, aut residuum >= modulus suus (exitus
 * non tangitur). numerus 0 -> 0. */
b32
congruentia_restitue (
    constans i32* residua,
    constans i32* moduli,
             i32  numerus,
             b32  symmetricus,
         Piscina* piscina,
          Magnus* exitus);

#endif /* CONGRUENTIA_H */
/* lib/piscina.c: statica per plagulam renominata */
#define Alveus Alveus_piscina
#define _allocare_interna _allocare_interna_piscina
#define _alveus_destruere _alveus_destruere_piscina
#define _alveus_nova _alveus_nova_piscina
#define _catena_alveus_destruere _catena_alveus_destruere_piscina
#define _catena_alveus_vacare _catena_alveus_vacare_piscina
#define _debug_imprimere _debug_imprimere_piscina
#define _proxima_ordinatio _proxima_ordinatio_piscina
#line 1 "lib/piscina.c"
/* <portabile/> probationibus sub glibc probata 2026-08-03 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef PISCINA_DEBUG
#define PISCINA_DEBUG FALSUM /* Muta ad VERUM pro imprimere debugging,
                              * vel -DPISCINA_DEBUG=1 in linea compilandi */
#endif

/* VENENUM (modus probandi, -DPISCINA_VENENUM=1): piscina_reficere
 * octetos liberatos PISCINA_OCTETUS_VENENI implet. Sine eo memoria
 * post refectionem valores veteres servat, et valor qui refectionem
 * superstat (vitium vitae) recte legi videtur - recensio polynomium-II
 * duo talia vitia plantata invenit quae suita ordinaria non videbat.
 * Porta: tools/venenum_probare.sh. */
#ifndef PISCINA_VENENUM
#define PISCINA_VENENUM FALSUM
#endif
#define PISCINA_OCTETUS_VENENI 0xA5


/* ===========================================================
 * Structura Alvei - allocatio singularis
 * =========================================================== */

nomen structura Alveus {
              vacuum* buffer;
      memoriae_index  capacitas;
      memoriae_index  offset;
    structura Alveus* sequens;
} Alveus;


/* ===========================================================
 * Structura Piscinae - regit alveos multiples
 * =========================================================== */

structura Piscina {
            Alveus* primus;
            Alveus* nunc;
    memoriae_index  mensura_alvei_initia;
         character* titulus;
               b32  est_dynamicum;
        memoriae_index  maximus_usus;
    memoriae_index  usus_currens;           /* summa offsetuum, incrementalis */
    memoriae_index  numerus_allocationum;   /* historia, numquam minuitur */
};


/* ===========================================================
 * ADIUTORES INTERNI
 * =========================================================== */

interior memoriae_index
_proxima_ordinatio (
        memoriae_index ptr,
        memoriae_index ordinatio)
{
    memoriae_index ordinatus = ptr + (ordinatio - I);
    redde ordinatus - (ordinatus % ordinatio);
}

interior vacuum
_debug_imprimere (
    constans character* piscinae_titulum,
    constans character* operatio,
        memoriae_index  mensura)
{
    si (PISCINA_DEBUG)
    {
        imprimere("[PISCINA %s] %s: %lu bytes\n", piscinae_titulum,
                  operatio, (insignatus longus)mensura);
    }
}


/* ===========================================================
 * REGIO ALVEI
 * =========================================================== */

interior Alveus*
_alveus_nova (
    memoriae_index capacitas)
{
    Alveus* alveus = (Alveus*)memoriae_allocare(magnitudo(Alveus));
    si (!alveus) redde NIHIL;

    alveus->buffer = memoriae_allocare(capacitas);
    si (!alveus->buffer)
    {
        liberare(alveus);
        redde NIHIL;
    }

    alveus->capacitas  = capacitas;
    alveus->offset     = ZEPHYRUM;
    alveus->sequens    = NIHIL;

    redde alveus;
}

interior vacuum
_alveus_destruere (
        Alveus* alveus)
{
    si (!alveus) redde;

    si (alveus->buffer) liberare(alveus->buffer);
    liberare(alveus);
}

interior vacuum
_catena_alveus_destruere (
        Alveus* alveus)
{
    dum (alveus)
    {
        Alveus* sequens_temporalis = alveus->sequens;
        _alveus_destruere(alveus);
        alveus = sequens_temporalis;
    }
}

interior vacuum
_catena_alveus_vacare (
        Alveus* alveus)
{
    dum (alveus)
    {
        memset(alveus->buffer, ZEPHYRUM, alveus->capacitas);
        alveus->offset  = ZEPHYRUM;
        alveus          = alveus->sequens;
    }
}


/* ===========================================================
 * ALLOCATIO FUNDAMENTALIS LOGICA
 * =========================================================== */

interior vacuum*
_allocare_interna (
               Piscina* piscina,
        memoriae_index  mensura,
        memoriae_index  ordinatio,
                   b32  fatalis)
{
        memoriae_index  ordinatus_offset;
        memoriae_index  necessaria;
                vacuum* ptr;

    si (!piscina || mensura == ZEPHYRUM) redde NIHIL;

    ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset,
        ordinatio);
    necessaria = ordinatus_offset + mensura;

    /* Si allocatio in alveum nunc non capit, invenire vel generare alveum novum */
    dum (necessaria > piscina->nunc->capacitas)
    {
        si (piscina->nunc->sequens)
        {
            /* Transire ad alveum sequentem */
            piscina->nunc = piscina->nunc->sequens;
            ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset,
                ordinatio);
            necessaria = ordinatus_offset + mensura;
        }
        alioquin si (piscina->est_dynamicum)
        {
            Alveus* alveus_novum;

            /* Generare alveum novum */
            memoriae_index capacitas_nova =
                piscina->mensura_alvei_initia * II;

            /* Petitio maior quam duplum: alveus ad mensuram petitionis
             * (+ basis), BASIS INTACTA. Olim basis ad hanc mensuram
             * ratchetabatur et numquam decrescebat: lib/stml.c alvei
             * 1, 2, 3, 6, 9, 18, 27, 54 MB, ultimo 54 MB VII tenente -
             * XXXIX% otiosum (RP §6, 2026-09-02). */
            si (necessaria > capacitas_nova)
            {
                capacitas_nova = necessaria
                    + piscina->mensura_alvei_initia;
            }

            alveus_novum = _alveus_nova(capacitas_nova);
            si (!alveus_novum)
            {
                si (fatalis)
                {
                    imprimere("CREATIO ALVEI FRACTA: %s\n",
                              piscina->titulus ? piscina->titulus : "nemo");
                    exire(I);
                }
                redde NIHIL;
            }

            piscina->nunc->sequens  = alveus_novum;
            piscina->nunc           = alveus_novum;

            ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset,
                ordinatio);
            necessaria = ordinatus_offset + mensura;

            _debug_imprimere(
                    piscina->titulus ? piscina->titulus : "nemo",
                    "alveus_novum",
                    capacitas_nova);
        }
        alioquin
        {
            /* Non dynamicum et nulli alvei reliqui */
            si (fatalis)
            {
                imprimere("ALLOCATIO PISCINAE FRACTA: %s (indigentia %lu)\n",
                          piscina->titulus ? piscina->titulus : "nemo",
                          (insignatus longus)necessaria);
                exire(I);
            }
            redde NIHIL;
        }
    }


        /* Allocare ex alveo nunc. Apex INCREMENTALITER (2026-09-02): olim
     * omnes alvei per allocationem percurrebantur (I.II M allocationes
     * x XVII alvei in lib/stml.c = XIII% foliorum profili); summa
     * offsetuum mutatur solum hic (delta), in vacare (nihil) et in
     * reficere (recomputata semel). */
    ptr = (character*)(piscina->nunc->buffer) + ordinatus_offset;
    piscina->usus_currens += necessaria - piscina->nunc->offset;
    piscina->nunc->offset = necessaria;
    si (piscina->usus_currens > piscina->maximus_usus)
    {
        piscina->maximus_usus = piscina->usus_currens;
    }
    piscina->numerus_allocationum += I;

    _debug_imprimere(piscina->titulus ? piscina->titulus : "nemo",
        "allocare", mensura);

    redde ptr;
}


/* ===========================================================
 * GENERATIO
 * =========================================================== */

Piscina*
piscina_generare_dynamicum (
    constans character* piscinae_titulum,
        memoriae_index  mensura_alvei_initia)
{
    Alveus* alveus_primus;

    Piscina* piscina = (Piscina*)memoriae_allocare(magnitudo(Piscina));
    si (!piscina) redde NIHIL;

    alveus_primus = _alveus_nova(mensura_alvei_initia);
    si (!alveus_primus)
    {
        liberare(piscina);
        redde NIHIL;
    }

    piscina->primus                = alveus_primus;
    piscina->nunc                  = alveus_primus;
    piscina->mensura_alvei_initia  = mensura_alvei_initia;
        piscina->est_dynamicum     = VERUM;
    piscina->maximus_usus          = ZEPHYRUM;
    piscina->usus_currens          = ZEPHYRUM;
    piscina->numerus_allocationum  = ZEPHYRUM;

    si (piscinae_titulum)
    {
        memoriae_index mensura_tituli = strlen(piscinae_titulum);
        piscina->titulus = (character*)memoriae_allocare(mensura_tituli
            + I);

        si (piscina->titulus)
        {
            strcpy(piscina->titulus, piscinae_titulum);
        }
        alioquin
        {
            piscina->titulus = NIHIL;
        }
    }
    alioquin
    {
        piscina->titulus = NIHIL;
    }

    redde piscina;
}

Piscina*
piscina_generare_certae_magnitudinis (
    constans character* piscinae_titulum,
        memoriae_index  mensura_buffer)
{
    Alveus* alveus_primus;

    Piscina* piscina = (Piscina*)memoriae_allocare(magnitudo(Piscina));
    si (!piscina) redde NIHIL;

    alveus_primus = _alveus_nova(mensura_buffer);
    si (!alveus_primus)
    {
        liberare(piscina);
        redde NIHIL;
    }

    piscina->primus                = alveus_primus;
    piscina->nunc                  = alveus_primus;
    piscina->mensura_alvei_initia  = mensura_buffer;
        piscina->est_dynamicum     = FALSUM;
    piscina->numerus_allocationum  = ZEPHYRUM;
    piscina->maximus_usus          = ZEPHYRUM;
    piscina->usus_currens          = ZEPHYRUM;

    si (piscinae_titulum)
    {
        memoriae_index mensura_tituli = strlen(piscinae_titulum);
        piscina->titulus = (character*)memoriae_allocare(mensura_tituli
            + I);
        si (piscina->titulus)
        {
            strcpy(piscina->titulus, piscinae_titulum);
        }
        alioquin
        {
            piscina->titulus = NIHIL;
        }
    }
    alioquin
    {
        piscina->titulus = NIHIL;
    }

    redde piscina;
}


/* ===========================================================
 * DESTRUCTIO
 * =========================================================== */

vacuum
piscina_destruere (
        Piscina* piscina)
{
    si (!piscina) redde;

    si (piscina->primus) _catena_alveus_destruere(piscina->primus);
    si (piscina->titulus) liberare(piscina->titulus);

    liberare(piscina);
}


/* ===========================================================
 * ALLOCATIO - EXITIUM SI DEFECIT
 * =========================================================== */

vacuum*
piscina_allocare (
           Piscina* piscina,
    memoriae_index  mensura)
{
    redde _allocare_interna(piscina, mensura,
        PISCINA_ORDINATIO_ORDINARIA, VERUM);
}

vacuum*
piscina_allocare_ordinatum (
           Piscina* piscina,
    memoriae_index  mensura,
    memoriae_index  ordinatio)
{
    redde _allocare_interna(piscina, mensura, ordinatio, VERUM);
}


/* ===========================================================
 * ALLOCATIO - REDDE NIHIL SI DEFECIT
 * =========================================================== */

vacuum*
piscina_conari_allocare (
           Piscina* piscina,
    memoriae_index  mensura)
{
    redde _allocare_interna(piscina, mensura,
        PISCINA_ORDINATIO_ORDINARIA, FALSUM);
}

vacuum*
piscina_conari_allocare_ordinatum (
           Piscina* piscina,
    memoriae_index  mensura,
    memoriae_index  ordinatio)
{
    redde _allocare_interna(piscina, mensura, ordinatio, FALSUM);
}


/* ===========================================================
 * CYCLUS VITAE
 * =========================================================== */

vacuum
piscina_vacare (
        Piscina* piscina)
{
        si (!piscina) redde;
    _catena_alveus_vacare(piscina->primus);
    piscina->nunc          = piscina->primus;
    piscina->usus_currens  = ZEPHYRUM;
    _debug_imprimere(piscina->titulus ? piscina->titulus : "nemo",
        "vacare", ZEPHYRUM);
}


/* ===========================================================
 * QUAESTIO
 * =========================================================== */

memoriae_index
piscina_summa_usus (
        constans Piscina* piscina)
{
    constans Alveus* b;
     memoriae_index  summa;

    si (!piscina) redde ZEPHYRUM;

    summa = ZEPHYRUM;
    per (b = piscina->primus; b; b = b->sequens)
    {
        summa += b->offset;
    }
    redde summa;
}

memoriae_index
piscina_summa_inutilis_allocatus (
        constans Piscina* piscina)
{
    constans Alveus* b;
     memoriae_index  reliqua;

    si (!piscina) redde ZEPHYRUM;

    reliqua = ZEPHYRUM;
    per (b = piscina->primus; b; b = b->sequens)
    {
        reliqua += (b->capacitas - b->offset);
    }
    redde reliqua;
}

memoriae_index
piscina_reliqua_antequam_cresca_alvei (
        constans Piscina* piscina)
{
    si (!piscina || !piscina->nunc) redde ZEPHYRUM;
    redde piscina->nunc->capacitas - piscina->nunc->offset;
}

memoriae_index
piscina_summa_apex_usus (
        constans Piscina* piscina)
{
    redde piscina ? piscina->maximus_usus : ZEPHYRUM;
}

memoriae_index
piscina_numerus_alveorum (
        constans Piscina* piscina)
{
    constans Alveus* b;
     memoriae_index  numerus;

    si (!piscina) redde ZEPHYRUM;

    numerus = ZEPHYRUM;
    per (b = piscina->primus; b; b = b->sequens)
    {
        numerus += I;
    }
    redde numerus;
}

memoriae_index
piscina_numerus_allocationum (
        constans Piscina* piscina)
{
    redde piscina ? piscina->numerus_allocationum : ZEPHYRUM;
}


/* ===========================================================
 * NOTATIO - MARK/RESET PATTERN
 * =========================================================== */

PiscinaNotatio
piscina_notare (
        Piscina* piscina)
{
    PiscinaNotatio notatio;

    si (!piscina)
    {
        notatio.alveus_nunc  = NIHIL;
        notatio.positus      = ZEPHYRUM;
        redde notatio;
    }

    notatio.alveus_nunc  = piscina->nunc;
    notatio.positus      = piscina->nunc->offset;

    _debug_imprimere(
            piscina->titulus ? piscina->titulus : "nemo",
            "notare",
            notatio.positus);

    redde notatio;
}

vacuum
piscina_reficere (
               Piscina* piscina,
        PiscinaNotatio  notatio)
{
    Alveus* alveus_notatus;
    Alveus* alveus_iter;

    si (!piscina || !notatio.alveus_nunc) redde;

    alveus_notatus = (Alveus*)notatio.alveus_nunc;

    si (PISCINA_VENENUM)
    {
        /* octeti liberati: pars alvei notati post positum, et alvei
         * sequentes toti (usque ad offset suum) */
        si (alveus_notatus->offset > notatio.positus)
        {
            memset((i8*)alveus_notatus->buffer + notatio.positus,
                PISCINA_OCTETUS_VENENI,
                alveus_notatus->offset - notatio.positus);
        }
        per (alveus_iter = alveus_notatus->sequens; alveus_iter;
             alveus_iter = alveus_iter->sequens)
        {
            memset(alveus_iter->buffer, PISCINA_OCTETUS_VENENI,
                alveus_iter->offset);
        }
    }

    /* Reficere alveum notatum ad positionem notatam */
    alveus_notatus->offset = notatio.positus;

    /* Vacare omnes alveos post alveum notatum */
    per (alveus_iter =
        alveus_notatus->sequens; alveus_iter; alveus_iter =
        alveus_iter->sequens)
    {
        alveus_iter->offset = ZEPHYRUM;
    }

        /* Reficere piscina->nunc ad alveum notatum; summa offsetuum
     * recomputata SEMEL (alvei ante notatum offsetus servant) */
    piscina->nunc          = alveus_notatus;
    piscina->usus_currens  = ZEPHYRUM;
    per (alveus_iter = piscina->primus; alveus_iter;
         alveus_iter = alveus_iter->sequens)
    {
        piscina->usus_currens += alveus_iter->offset;
    }

    _debug_imprimere(
            piscina->titulus ? piscina->titulus : "nemo",
            "reficere",
            notatio.positus);
}

b32
piscina_potesne_allocare (
        constans Piscina* piscina,
          memoriae_index  mensura)
{
    memoriae_index reliqua;

    si (!piscina || !piscina->nunc) redde FALSUM;

    reliqua = piscina->nunc->capacitas - piscina->nunc->offset;

    /* Pro piscinis dynamicis, semper possibile (crescet) */
    si (piscina->est_dynamicum) redde VERUM;

    /* Pro piscinis certae magnitudinis, verificare spatium */
    redde mensura <= reliqua ? VERUM : FALSUM;
}
#undef Alveus
#undef PISCINA_DEBUG
#undef PISCINA_OCTETUS_VENENI
#undef PISCINA_VENENUM
#undef _allocare_interna
#undef _alveus_destruere
#undef _alveus_nova
#undef _catena_alveus_destruere
#undef _catena_alveus_vacare
#undef _debug_imprimere
#undef _proxima_ordinatio
/* lib/chorda.c: statica per plagulam renominata */
#define _chorda_nullum_habet _chorda_nullum_habet_chorda
#define _extrahere_verba _extrahere_verba_chorda
#line 1 "lib/chorda.c"
/* <portabile/> probationibus sub glibc probata 2026-08-03 */




#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define CHORDA_FRIATUM_OFFSET  2166136261U
#define CHORDA_FRIATUM_PRIMUS    16777619U


/* ==================================================
 * Constructores
 * ================================================== */

chorda
chorda_ex_literis (
    constans character* litterae,
               Piscina* piscina)
{
    chorda  fructus;
       i32  mensura;
        i8* allocatus;

    si (!litterae || !piscina)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

      mensura = (i32)strlen(litterae);
    allocatus = (i8*)piscina_allocare(piscina, mensura);

    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, litterae, mensura);
    fructus.mensura  = mensura;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_ex_buffer (
     i8* buffer,
    i32  mensura)
{
    chorda fructus;

    si (!buffer)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    fructus.mensura  = mensura;
    fructus.datum    = buffer;

    redde fructus;
}

chorda
chorda_sectio (
    chorda s,
       i32 initium,
       i32 finis)
{
    chorda fructus;

    si (!s.datum || initium > finis || finis > s.mensura)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    fructus.mensura  = finis - initium;
    fructus.datum    = s.datum + initium;

    redde fructus;
}

chorda
chorda_transcribere (
     chorda  s,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina, s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, s.datum, s.mensura);

    fructus.mensura  = s.mensura;
    fructus.datum    = allocatus;

    redde fructus;
}

chorda
chorda_concatenare (
     chorda  a,
     chorda  b,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;
       i32  mensura_totalis;

    si (!piscina)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Si a vacuus est, redde transcriptionem b */
    si (a.mensura == ZEPHYRUM || !a.datum)
    {
        redde chorda_transcribere(b, piscina);
    }

    /* Si b vacuus est, redde transcriptionem a */
    si (b.mensura == ZEPHYRUM || !b.datum)
    {
        redde chorda_transcribere(a, piscina);
    }

    mensura_totalis  = a.mensura + b.mensura;
    allocatus        = (i8*)piscina_allocare(piscina, mensura_totalis);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    memcpy(allocatus,             a.datum, (memoriae_index)a.mensura);
    memcpy(allocatus + a.mensura, b.datum, (memoriae_index)b.mensura);

    fructus.mensura  = mensura_totalis;
    fructus.datum    = allocatus;

    redde fructus;
}

chorda
chorda_praecidi_laterale (
     chorda  s,
    Piscina* piscina)
{
    chorda fructus;
       i32 initium;
       i32 finis;
       i32 i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* initium = sententia "non inventum" (s.mensura): chorda tota
	 * spatia -> initium >= finis -> vacua redditur (ante 2026-07-17
	 * initium ZEPHYRUM manebat ET circulus retro insignatus
	 * volvebatur - circulus infinitus cum lectionibus feris) */
    initium  = s.mensura;
    finis    = s.mensura;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (!isspace((i8)s.datum[i]))
        {
            initium = i;
            frange;
        }
    }

    /* idioma numerus-deorsum: i insignatus numquam sub zephyrum */
    per (i = s.mensura; i > ZEPHYRUM; i--)
    {
        si (!isspace((i8)s.datum[i - I]))
        {
            finis = i;
            frange;
        }
    }

    si (initium >= finis)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    redde chorda_sectio(s, initium, finis);
}


/* ==================================================
 * Divisio
 * ================================================== */

chorda_fissio_fructus
chorda_fissio (
       chorda  s,
    character  delim,
      Piscina* piscina)
{
    chorda_fissio_fructus  fructus;
                   chorda* elementa;
                      i32  capacitas;
                      i32  numerus;
                      i32  initium;
                      i32  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.elementa  = NIHIL;
        fructus.numerus   = ZEPHYRUM;
        redde fructus;
    }

    capacitas = XVI;
    elementa = (chorda*)piscina_allocare(piscina,
        capacitas * magnitudo(chorda));
    si (!elementa)
    {
        fructus.elementa  = NIHIL;
        fructus.numerus   = ZEPHYRUM;
        redde fructus;
    }

    numerus = ZEPHYRUM;
    initium = ZEPHYRUM;

    per (i = ZEPHYRUM; i <= s.mensura; i++)
    {
        b32 est_delim = (i < s.mensura && s.datum[i] == delim);
        b32 est_finis = (i == s.mensura);

        si (est_delim || est_finis)
        {
            si (numerus >= capacitas)
            {
                 chorda* elementa_nova;
                    i32  j;

                capacitas *= II;
                elementa_nova = (chorda*)piscina_allocare(piscina,
                    capacitas * magnitudo(chorda));
                si (!elementa_nova)
                {
                    fructus.elementa  = NIHIL;
                    fructus.numerus   = ZEPHYRUM;
                    redde fructus;
                }

                /* Transcribere elementa veteres ad array novum */
                per (j = ZEPHYRUM; j < numerus; j++)
                {
                    elementa_nova[j] = elementa[j];
                }

                elementa = elementa_nova;
            }

            elementa[numerus] = chorda_sectio(s, initium, i);
            numerus++;
            initium = i + I;
        }
    }

    fructus.elementa  = elementa;
    fructus.numerus   = numerus;
    redde fructus;
}


/* ==================================================
 * Comparatio
 * ================================================== */

b32
chorda_aequalis (
    chorda a,
    chorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }

    si (a.datum == b.datum)
    {
        redde VERUM;
    }

    redde memcmp(a.datum, b.datum, a.mensura) == ZEPHYRUM;
}

b32
chorda_aequalis_literis (
                chorda  s,
    constans character* cstr)
{
    i32 len;
    i32 i;

    si (!cstr)
    {
        redde FALSUM;
    }

    /* Mensura literarum */
    len = ZEPHYRUM;
    dum (cstr[len] != '\0')
    {
        len++;
    }

    si (s.mensura != len)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < len; i++)
    {
        si ((character)s.datum[i] != cstr[i])
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

b32
chorda_aequalis_case_insensitivus (
    chorda a,
    chorda b)
{
    memoriae_index i;

    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < a.mensura; i++)
    {
        si (tolower((character)a.datum[i])
            != tolower((character)b.datum[i]))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

s32
chorda_comparare (
    chorda a,
    chorda b)
{
    memoriae_index minima_mensura;
               s32 cmp_result;

    minima_mensura  = a.mensura < b.mensura ? a.mensura : b.mensura;
    cmp_result      = memcmp(a.datum, b.datum, minima_mensura);

    si (cmp_result != ZEPHYRUM)
    {
        redde cmp_result;
    }

    si (a.mensura < b.mensura) redde -I;
    si (a.mensura > b.mensura) redde I;

    redde ZEPHYRUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

b32
chorda_continet (
    chorda fenum,
    chorda acus)
{
    memoriae_index i;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        redde FALSUM;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    per (i = ZEPHYRUM; i <= fenum.mensura - acus.mensura; i++)
    {
        si (memcmp(fenum.datum + i, acus.datum, acus.mensura)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

b32
chorda_incipit (
    chorda s,
    chorda prefixum)
{
    si (!s.datum || !prefixum.datum || prefixum.mensura > s.mensura)
    {
        redde FALSUM;
    }

    si (prefixum.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    redde memcmp(s.datum, prefixum.datum, prefixum.mensura) == ZEPHYRUM;
}

b32
chorda_terminatur (
    chorda s,
    chorda suffixum)
{
    memoriae_index offset;

    si (!s.datum || !suffixum.datum || suffixum.mensura > s.mensura)
    {
        redde FALSUM;
    }

    si (suffixum.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    offset = s.mensura - suffixum.mensura;
    redde memcmp(s.datum + offset, suffixum.datum, suffixum.mensura)
        == ZEPHYRUM;
}

chorda
chorda_invenire (
    chorda fenum,
    chorda acus)
{
    memoriae_index i;
            chorda fructus;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde fenum;
    }

    per (i = ZEPHYRUM; i <= fenum.mensura - acus.mensura; i++)
    {
        si (memcmp(fenum.datum + i, acus.datum, acus.mensura)
            == ZEPHYRUM)
        {
            fructus.mensura  = acus.mensura;
            fructus.datum    = fenum.datum + i;
            redde fructus;
        }
    }

    fructus.mensura  = ZEPHYRUM;
    fructus.datum    = NIHIL;

    redde fructus;
}

s32
chorda_invenire_index (
    chorda fenum,
    chorda acus)
{
    i32 i;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        redde -I;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    per (i = ZEPHYRUM; i <= fenum.mensura - acus.mensura; i++)
    {
        si (memcmp(fenum.datum + i, acus.datum,
            (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }

    redde -I;
}

chorda
chorda_invenire_ultimum (
    chorda fenum,
    chorda acus)
{
    chorda fructus;
       s32 i;
       s32 max_i;
       s32 ultima_positio;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde fenum;
    }

    ultima_positio  = -I;
    max_i           = (s32)(fenum.mensura - acus.mensura);

    per (i = max_i; i >= ZEPHYRUM; i--)
    {
        si (memcmp(fenum.datum + i, acus.datum,
            (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            ultima_positio = i;
            frange;
        }
    }

    si (ultima_positio < ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    fructus.mensura  = acus.mensura;
    fructus.datum    = fenum.datum + ultima_positio;
    redde fructus;
}

s32
chorda_invenire_ultimum_index (
    chorda fenum,
    chorda acus)
{
    s32 i;
    s32 max_i;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        redde -I;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde (s32)fenum.mensura;
    }

    max_i = (s32)(fenum.mensura - acus.mensura);

    per (i = max_i; i >= ZEPHYRUM; i--)
    {
        si (memcmp(fenum.datum + i, acus.datum,
            (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            redde i;
        }
    }

    redde -I;
}

i32
chorda_numerare_occurrentia (
    chorda fenum,
    chorda acus)
{
    i32 count;
    i32 positus;

    si (   !fenum.datum || !acus.datum || acus.mensura > fenum.mensura
        || acus.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    count    = ZEPHYRUM;
    positus  = ZEPHYRUM;

    dum (positus <= fenum.mensura - acus.mensura)
    {
        si (memcmp(fenum.datum + positus, acus.datum,
            (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            count++;
            positus += acus.mensura;
        }
        alioquin
        {
            positus++;
        }
    }

    redde count;
}


/* ==================================================
 * Manipulatio
 * ================================================== */

chorda
chorda_praecidere (
    chorda s)
{
    i32 initium;
    i32 finis;
    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        redde s;
    }

    initium  = ZEPHYRUM;
    finis    = s.mensura;

    /* Praecidere initium */
    dum (initium < s.mensura && isspace((character)s.datum[initium]))
    {
        initium++;
    }

    /* Praecidere finis */
    dum (finis > initium && isspace((character)s.datum[finis - I]))
    {
        finis--;
    }

    redde chorda_sectio(s, initium, finis);
}

chorda
chorda_minuscula (
     chorda  s,
    Piscina* piscina)
{
            chorda  fructus;
         character* allocatus;
    memoriae_index  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    allocatus = (character*)piscina_allocare(piscina, s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        allocatus[i] = (character)tolower((character)s.datum[i]);
    }

    fructus.mensura  = s.mensura;
    fructus.datum    = (i8*)(void*)allocatus;

    redde fructus;
}

chorda
chorda_maiuscula (
     chorda  s,
    Piscina* piscina)
{
            chorda  fructus;
         character* allocatus;
    memoriae_index  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    allocatus = (character*)piscina_allocare(piscina,
        (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    per (i = ZEPHYRUM; i < (memoriae_index)s.mensura; i++)
    {
        allocatus[i] = (character)toupper((character)s.datum[i]);
    }

    fructus.mensura  = s.mensura;
    fructus.datum    = (i8*)(void*)allocatus;

    redde fructus;
}

chorda
chorda_praecidere_sinistram (
    chorda s)
{
    i32 initium;

    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        redde s;
    }

    initium = ZEPHYRUM;

    dum (initium < s.mensura && isspace((character)s.datum[initium]))
    {
        initium++;
    }

    redde chorda_sectio(s, initium, s.mensura);
}

chorda
chorda_praecidere_dextram (
    chorda s)
{
    i32 finis;

    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        redde s;
    }

    finis = s.mensura;

    dum (finis > ZEPHYRUM && isspace((character)s.datum[finis - I]))
    {
        finis--;
    }

    redde chorda_sectio(s, ZEPHYRUM, finis);
}

chorda
chorda_substituere (
      chorda  s,
      chorda  antiquum,
      chorda  novum,
     Piscina* piscina)
{
    chorda  fructus;
       i32  numerus_occurrentia;
       i32  mensura_nova;
        i8* allocatus;
       i32  positus_lecti;
       i32  positus_scripti;
       i32  i;

    si (!piscina)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Si antiquum vacuus, redde copiam originalem */
    si (!antiquum.datum || antiquum.mensura == ZEPHYRUM)
    {
        redde chorda_transcribere(s, piscina);
    }

    /* Si s vacuus, redde vacuus */
    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Numerare occurrentias */
    numerus_occurrentia = chorda_numerare_occurrentia(s, antiquum);

    si (numerus_occurrentia == ZEPHYRUM)
    {
        redde chorda_transcribere(s, piscina);
    }

    /* Calculare mensuram novam */
    mensura_nova = s.mensura + (numerus_occurrentia * (novum.mensura
        - antiquum.mensura));

    si (mensura_nova <= ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus_lecti    = ZEPHYRUM;
    positus_scripti  = ZEPHYRUM;

    dum (positus_lecti < s.mensura)
    {
        /* Verificare si hic est antiquum */
        si (   positus_lecti <= s.mensura - antiquum.mensura
            && memcmp(s.datum + positus_lecti, antiquum.datum,
            (memoriae_index)antiquum.mensura) == ZEPHYRUM)
        {
            /* Scribere novum */
            per (i = ZEPHYRUM; i < novum.mensura; i++)
            {
                allocatus[positus_scripti++] = novum.datum[i];
            }
            positus_lecti += antiquum.mensura;
        }
        alioquin
        {
            allocatus[positus_scripti++] = s.datum[positus_lecti++];
        }
    }

    fructus.mensura  = mensura_nova;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_invertere (
     chorda  s,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;
       i32  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        allocatus[i] = s.datum[s.mensura - I - i];
    }

    fructus.mensura  = s.mensura;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_duplicare (
     chorda  s,
        i32  numerus,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;
       i32  mensura_nova;
       i32  i;

    si (!piscina || numerus <= ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    mensura_nova = s.mensura * numerus;

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        memcpy(allocatus + (i * s.mensura), s.datum,
            (memoriae_index)s.mensura);
    }

    fructus.mensura  = mensura_nova;
    fructus.datum    = allocatus;
    redde fructus;
}


/* ==================================================
 * Convenientia
 * ================================================== */

character*
chorda_ut_cstr (
     chorda  s,
    Piscina* piscina)
{
    character* allocatus;

    si (!s.datum)
    {
        redde NIHIL;
    }

    allocatus = (character*)piscina_allocare(piscina, s.mensura + I);
    si (!allocatus)
    {
        redde NIHIL;
    }

    memcpy(allocatus, s.datum, s.mensura);
    allocatus[s.mensura] = '\0';

    redde allocatus;
}

/* NULLUM insertum? Conversiones numericae per cstr transeunt, ubi
 * NULLUM tacite truncaret ('1\0garbage' -> 1, quia *terminus == '\0'
 * VERUM est AD nullum insertum). Custodia UNA in limine. */
interior b32
_chorda_nullum_habet (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (s.datum[i] == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
chorda_ut_s32 (
    chorda  s,
       s32* fructus)
{
          character* cstr_temporalis;
          character* terminus;
    signatus longus  longus_valor;

    si (   !s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s))
    {
        redde FALSUM;
    }

    cstr_temporalis = (character*)memoriae_allocare(s.mensura + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.datum, s.mensura);
    cstr_temporalis[s.mensura] = '\0';

    longus_valor = strtol(cstr_temporalis, &terminus, X);

    si (   terminus     != cstr_temporalis && *terminus == '\0'
        && longus_valor <= 2147483647L
        && longus_valor >= -2147483647L - 1L)
    {
        *fructus = (s32)longus_valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }

    liberare(cstr_temporalis);
    redde FALSUM;
}

b32
chorda_ut_i32 (
    chorda  s,
       i32* fructus)
{
            character* cstr_temporalis;
            character* terminus;
    insignatus longus  valor;

    si (   !s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s) || s.datum[ZEPHYRUM] == '-')
    {
        redde FALSUM;
    }

    cstr_temporalis = (character*)memoriae_allocare(s.mensura + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.datum, s.mensura);
    cstr_temporalis[s.mensura] = '\0';

    valor = strtoul(cstr_temporalis, &terminus, X);


    /* Confer si tota chorda parata est */
    si (   terminus != cstr_temporalis && *terminus == '\0'
        && valor    <= 4294967295UL)
    {
        *fructus = (i32)valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }

    liberare(cstr_temporalis);
    redde FALSUM;
}


/* ==================================================
 * FRIATIO
 * ================================================== */

i32
chorda_friare (
    chorda s)
{
    /* Algoritmus FNV-1a friationis */
    i32 friatum;
    i32 i;

    friatum = CHORDA_FRIATUM_OFFSET;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        friatum ^= (i8)s.datum[i];
        friatum *= CHORDA_FRIATUM_PRIMUS;
    }

    redde friatum;
}


/* ==================================================
 * Conversio Casus
 * ================================================== */

/* Interior: Extrahere verba ex chorda
 * Regulae:
 * - Characteres non-alphanumerici sunt delimitatores (omittuntur)
 * - Transitio minuscula→maiuscula incipit verbum novum
 * - Littera post digitum incipit verbum novum
 * - Maiusculae consecutivae ante minusculam: scinde ante ultimam maiusculam
 */
interior chorda_fissio_fructus
_extrahere_verba (
       chorda  s,
      Piscina* piscina)
{
    chorda_fissio_fructus  fructus;
                   chorda* verba;
                      i32  capacitas;
                      i32  numerus;
                      i32  i;
                      s32  initium_verbi;
                      b32  in_verbo;
                character  c_currens;
                character  c_praecedans;

    fructus.elementa  = NIHIL;
    fructus.numerus   = ZEPHYRUM;

    si (!s.datum || s.mensura == ZEPHYRUM || !piscina)
    {
        redde fructus;
    }

    capacitas = XVI;
    verba = (chorda*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(chorda));
    si (!verba)
    {
        redde fructus;
    }

    numerus        = ZEPHYRUM;
    initium_verbi  = -I;
    in_verbo       = FALSUM;
    c_praecedans   = '\0';

    per (i = ZEPHYRUM; i <= s.mensura; i++)
    {
        b32 est_finis        = (i == s.mensura);
        b32 est_alpha        = FALSUM;
        b32 est_digitus      = FALSUM;
        b32 est_maiuscula    = FALSUM;
        b32 praec_minuscula  = FALSUM;
        b32 praec_digitus    = FALSUM;
        b32 praec_maiuscula  = FALSUM;
        b32 debet_scindere   = FALSUM;

        si (!est_finis)
        {
            c_currens      = (character)s.datum[i];
            est_alpha      = isalpha((integer)c_currens) != ZEPHYRUM;
            est_digitus    = isdigit((integer)c_currens) != ZEPHYRUM;
            est_maiuscula  = isupper((integer)c_currens) != ZEPHYRUM;
        }

        si (c_praecedans != '\0')
        {
            praec_minuscula = islower((integer)c_praecedans)
                != ZEPHYRUM;
            praec_digitus = isdigit((integer)c_praecedans) != ZEPHYRUM;
            praec_maiuscula = isupper((integer)c_praecedans)
                != ZEPHYRUM;
        }

        /* Determinare si debemus scindere */
        si (in_verbo && !est_finis && (est_alpha || est_digitus))
        {
            /* Minuscula ad maiusculam: scinde */
            si (praec_minuscula && est_maiuscula)
            {
                debet_scindere = VERUM;
            }
            /* Digitus ad litteram: scinde */
            alioquin si (praec_digitus && est_alpha)
            {
                debet_scindere = VERUM;
            }
            /* Maiuscula ante maiusculam, sed proximus est minuscula: scinde ante currens */
            /* Exemplum: "XMLParser" - quando ad 'P' venimus post 'L', scinde */
            alioquin si (   praec_maiuscula && est_maiuscula
                         && i + I < s.mensura)
            {
                character c_proximus = (character)s.datum[i + I];
                si (islower((integer)c_proximus))
                {
                    debet_scindere = VERUM;
                }
            }
        }

        /* Finis verbi */
        si (   in_verbo
            && (est_finis || (!est_alpha && !est_digitus)
            || debet_scindere))
        {
            /* Verificare capacitatem */
            si (numerus >= capacitas)
            {
                 chorda* verba_nova;
                    i32  j;

                capacitas *= II;
                verba_nova = (chorda*)piscina_allocare(piscina,
                    (memoriae_index)capacitas * magnitudo(chorda));
                si (!verba_nova)
                {
                    fructus.elementa  = NIHIL;
                    fructus.numerus   = ZEPHYRUM;
                    redde fructus;
                }

                per (j = ZEPHYRUM; j < numerus; j++)
                {
                    verba_nova[j] = verba[j];
                }
                verba = verba_nova;
            }

            verba[numerus] = chorda_sectio(s, (i32)initium_verbi, i);
            numerus++;

            si (debet_scindere)
            {
                /* Initium novi verbi est currens character */
                initium_verbi  = (s32)i;
                in_verbo       = VERUM;
            }
            alioquin
            {
                in_verbo       = FALSUM;
                initium_verbi  = -I;
            }
        }

        /* Initium verbi novi */
        si (!in_verbo && !est_finis && (est_alpha || est_digitus))
        {
            initium_verbi  = (s32)i;
            in_verbo       = VERUM;
        }

        si (!est_finis)
        {
            c_praecedans = c_currens;
        }
    }

    fructus.elementa  = verba;
    fructus.numerus   = numerus;
    redde fructus;
}

chorda
chorda_pascalis (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Allocare maximum possibile (originalis mensura) */
    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            character c = (character)verbum.datum[j];

            si (j == ZEPHYRUM)
            {
                /* Prima littera maiuscula */
                allocatus[positus++] = (i8)toupper((integer)c);
            }
            alioquin
            {
                /* Reliquae minusculae */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_camelus (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            character c = (character)verbum.datum[j];

            si (i == ZEPHYRUM)
            {
                /* Primum verbum totum minusculum */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
            alioquin si (j == ZEPHYRUM)
            {
                /* Prima littera verborum subsequentium maiuscula */
                allocatus[positus++] = (i8)toupper((integer)c);
            }
            alioquin
            {
                /* Reliquae minusculae */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_serpens (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  mensura_nova;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Calculare mensuram: verba + separatores */
    mensura_nova = ZEPHYRUM;
    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        mensura_nova += verba.elementa[i].mensura;
        si (i > ZEPHYRUM)
        {
            mensura_nova += I; /* Pro '_' */
        }
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        si (i > ZEPHYRUM)
        {
            allocatus[positus++] = '_';
        }

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            allocatus[positus++] =
                (i8)tolower((integer)(character)verbum.datum[j]);
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_kebab (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  mensura_nova;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Calculare mensuram: verba + separatores */
    mensura_nova = ZEPHYRUM;
    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        mensura_nova += verba.elementa[i].mensura;
        si (i > ZEPHYRUM)
        {
            mensura_nova += I; /* Pro '-' */
        }
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        si (i > ZEPHYRUM)
        {
            allocatus[positus++] = '-';
        }

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            allocatus[positus++] =
                (i8)tolower((integer)(character)verbum.datum[j]);
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_pascalis_serpens (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  mensura_nova;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Calculare mensuram: verba + separatores */
    mensura_nova = ZEPHYRUM;
    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        mensura_nova += verba.elementa[i].mensura;
        si (i > ZEPHYRUM)
        {
            mensura_nova += I; /* Pro '_' */
        }
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        si (i > ZEPHYRUM)
        {
            allocatus[positus++] = '_';
        }

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            character c = (character)verbum.datum[j];

            si (j == ZEPHYRUM)
            {
                /* Prima littera maiuscula */
                allocatus[positus++] = (i8)toupper((integer)c);
            }
            alioquin
            {
                /* Reliquae minusculae */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}


/* ==================================================
 * Novae Functiones Utilitatis
 * ================================================== */

b32
chorda_ut_f64 (
    chorda  s,
       f64* fructus)
{
    character* cstr_temporalis;
    character* terminus;
          f64  valor;

    si (   !s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s))
    {
        redde FALSUM;
    }

    cstr_temporalis =
        (character*)memoriae_allocare((memoriae_index)s.mensura + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.datum, (memoriae_index)s.mensura);
    cstr_temporalis[s.mensura] = '\0';

    valor = strtod(cstr_temporalis, &terminus);

    si (terminus != cstr_temporalis && *terminus == '\0')
    {
        *fructus = valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }

    liberare(cstr_temporalis);
    redde FALSUM;
}

chorda
chorda_ex_s32 (
        s32  numerus,
    Piscina* piscina)
{
         chorda  fructus;
      character  buffer[CXXXII];
            s32  mensura_signed;
            i32  mensura;
             i8* allocatus;

    fructus.mensura  = ZEPHYRUM;
    fructus.datum    = NIHIL;

    si (!piscina)
    {
        redde fructus;
    }

    mensura_signed = snprintf(buffer, magnitudo(buffer), "%d", numerus);
    si (mensura_signed < ZEPHYRUM)
    {
        redde fructus;
    }

    mensura = (i32)mensura_signed;
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }

    memcpy(allocatus, buffer, (memoriae_index)mensura);
    fructus.mensura  = mensura;
    fructus.datum    = allocatus;

    redde fructus;
}

b32
chorda_ut_s64 (
     chorda  s,
        s64* fructus)
{
    constans character* limes;
                   s64  valor    = ZEPHYRUM;
                   i32  initium  = ZEPHYRUM;
                   i32  cifrae;
                   i32  k;
                   b32  negans  = FALSUM;

    si (   !s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s))
    {
        redde FALSUM;
    }
    si (s.datum[ZEPHYRUM] == '+' || s.datum[ZEPHYRUM] == '-')
    {
        negans   = (s.datum[ZEPHYRUM] == '-') ? VERUM : FALSUM;
        initium  = I;
    }
    si (initium >= s.mensura)
    {
        redde FALSUM;
    }
    per (k = initium; k < s.mensura; k++)
    {
        si (s.datum[k] < '0' || s.datum[k] > '9')
        {
            redde FALSUM;
        }
    }
    /* zephyra ducentia sine pondere (unum servatur) */
    dum (initium < s.mensura - I && s.datum[initium] == '0')
    {
        initium++;
    }
    cifrae = s.mensura - initium;
    si (cifrae > XIX)
    {
        redde FALSUM;
    }
    /* limes LEXICE confertur: arithmetica ipsa circumvolveret */
    limes = negans ? "9223372036854775808" : "9223372036854775807";
    si (cifrae == XIX)
    {
        per (k = ZEPHYRUM; k < XIX; k++)
        {
            si (s.datum[initium + k] > (i8)limes[k])
            {
                redde FALSUM;
            }
            si (s.datum[initium + k] < (i8)limes[k])
            {
                frange;
            }
        }
    }
    /* negativa NEGATIVE accumulantur: -9223372036854775808 positivum
     * esse non potest */
    per (k = initium; k < s.mensura; k++)
    {
        s64 cifra = (s64)(s.datum[k] - '0');

        valor = negans ? valor * X - cifra : valor * X + cifra;
    }
    *fructus = valor;
    redde VERUM;
}

chorda
chorda_ex_s64 (
            s64  numerus,
        Piscina* piscina)
{
       chorda  fructus;
    character  buffer[XXIV];
    character  inversa[XXIV];
          i32  mensura = ZEPHYRUM;
          i32  k;
          i64  magnitudo_numeri;
          b32  negans;
           i8* allocatus;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (!piscina)
    {
        redde fructus;
    }
    si (numerus < ZEPHYRUM)
    {
        negans            = VERUM;
        /* -(numerus + I) + I: extremum negativum negari non potest */
        magnitudo_numeri  = (i64)(-(numerus + I)) + I;
    }
    alioquin
    {
        negans            = FALSUM;
        magnitudo_numeri  = (i64)numerus;
    }
    fac
    {
        buffer[mensura] = (character)('0'
            + (integer)(magnitudo_numeri % X));
        mensura++;
        magnitudo_numeri = magnitudo_numeri / X;
    }
    dum (magnitudo_numeri != ZEPHYRUM);
    si (negans)
    {
        buffer[mensura] = '-';
        mensura++;
    }
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        inversa[k] = buffer[mensura - I - k];
    }
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }
    memcpy(allocatus, inversa, (memoriae_index)mensura);
    fructus.datum    = allocatus;
    fructus.mensura  = mensura;
    redde fructus;
}

chorda
chorda_ex_f64_exacta (
            f64  numerus,
        Piscina* piscina)
{
       chorda  fructus;
    character  buffer[XLVIII];
          s32  mensura_signed;
          i32  mensura;
           i8* allocatus;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (!piscina)
    {
        redde fructus;
    }
    mensura_signed = snprintf(buffer, magnitudo(buffer), "%.17g",
                              numerus);
    si (   mensura_signed < ZEPHYRUM
        || mensura_signed >= (s32)magnitudo(buffer))
    {
        redde fructus;
    }
    mensura = (i32)mensura_signed;
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }
    memcpy(allocatus, buffer, (memoriae_index)mensura);
    fructus.datum    = allocatus;
    fructus.mensura  = mensura;
    redde fructus;
}

chorda
chorda_ex_f64 (
        f64  numerus,
        i32  praecisio,
    Piscina* piscina)
{
         chorda  fructus;
      character  buffer[CXXXII];
      character  formatalis[XVI];
            s32  mensura_signed;
            i32  mensura;
             i8* allocatus;

    fructus.mensura  = ZEPHYRUM;
    fructus.datum    = NIHIL;

    si (!piscina || praecisio < ZEPHYRUM || praecisio > L)
    {
        redde fructus;
    }

    snprintf(formatalis, magnitudo(formatalis), "%%.%df", praecisio);
    mensura_signed = snprintf(buffer, magnitudo(buffer), formatalis,
        numerus);
    /* snprintf longitudinem VERAM reddit, non scriptam: sine hac
     * custodia memcpy infra trans buffer legebat (1e300 sub '%.6f'
     * CCCVII octetos poscit, buffer CXXXII fert) */
    si (   mensura_signed < ZEPHYRUM
        || mensura_signed >= (s32)magnitudo(buffer))
    {
        redde fructus;
    }

    mensura = (i32)mensura_signed;
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }

    memcpy(allocatus, buffer, (memoriae_index)mensura);
    fructus.mensura  = mensura;
    fructus.datum    = allocatus;

    redde fructus;
}

chorda
chorda_character_ad (
     chorda  s,
        i32  index,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;

    fructus.mensura  = ZEPHYRUM;
    fructus.datum    = NIHIL;

    si (!piscina || !s.datum || index < ZEPHYRUM || index >= s.mensura)
    {
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina, I);
    si (!allocatus)
    {
        redde fructus;
    }

    allocatus[ZEPHYRUM]  = s.datum[index];
    fructus.mensura      = I;
    fructus.datum        = allocatus;

    redde fructus;
}

b32
chorda_vacua (
    chorda s)
{
    redde s.datum == NIHIL || s.mensura == ZEPHYRUM;
}

chorda_fissio_fructus
chorda_fissio_chorda (
     chorda  s,
     chorda  delim,
    Piscina* piscina)
{
    chorda_fissio_fructus  fructus;
                   chorda* elementa;
                      i32  capacitas;
                      i32  numerus;
                      i32  initium;
                      i32  i;

    fructus.elementa  = NIHIL;
    fructus.numerus   = ZEPHYRUM;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        redde fructus;
    }

    /* Si delimitator vacuus, redde totam chordam ut unum elementum */
    si (!delim.datum || delim.mensura == ZEPHYRUM)
    {
        elementa = (chorda*)piscina_allocare(piscina,
            magnitudo(chorda));
        si (!elementa)
        {
            redde fructus;
        }
        elementa[ZEPHYRUM]  = s;
        fructus.elementa    = elementa;
        fructus.numerus     = I;
        redde fructus;
    }

    capacitas = XVI;
    elementa = (chorda*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(chorda));
    si (!elementa)
    {
        redde fructus;
    }

    numerus  = ZEPHYRUM;
    initium  = ZEPHYRUM;
    i        = ZEPHYRUM;

    dum (i <= s.mensura - delim.mensura)
    {
        /* Verificare si hic est delimitator */
        si (memcmp(s.datum + i, delim.datum,
            (memoriae_index)delim.mensura) == ZEPHYRUM)
        {
            /* Verificare capacitatem */
            si (numerus >= capacitas)
            {
                 chorda* elementa_nova;
                    i32  j;

                capacitas *= II;
                elementa_nova = (chorda*)piscina_allocare(piscina,
                    (memoriae_index)capacitas * magnitudo(chorda));
                si (!elementa_nova)
                {
                    fructus.elementa  = NIHIL;
                    fructus.numerus   = ZEPHYRUM;
                    redde fructus;
                }

                per (j = ZEPHYRUM; j < numerus; j++)
                {
                    elementa_nova[j] = elementa[j];
                }
                elementa = elementa_nova;
            }

            elementa[numerus] = chorda_sectio(s, initium, i);
            numerus++;
            initium  = i + delim.mensura;
            i        = initium;
        }
        alioquin
        {
            i++;
        }
    }

    /* Addere ultimum segmentum */
    si (numerus >= capacitas)
    {
         chorda* elementa_nova;
            i32  j;

        capacitas *= II;
        elementa_nova = (chorda*)piscina_allocare(piscina,
            (memoriae_index)capacitas * magnitudo(chorda));
        si (!elementa_nova)
        {
            fructus.elementa  = NIHIL;
            fructus.numerus   = ZEPHYRUM;
            redde fructus;
        }

        per (j = ZEPHYRUM; j < numerus; j++)
        {
            elementa_nova[j] = elementa[j];
        }
        elementa = elementa_nova;
    }

    elementa[numerus] = chorda_sectio(s, initium, s.mensura);
    numerus++;

    fructus.elementa  = elementa;
    fructus.numerus   = numerus;
    redde fructus;
}

chorda
chorda_iungere (
     chorda* elementa,
        i32  numerus,
     chorda  separator,
    Piscina* piscina)
{
     ChordaAedificator* aed;
                   i32  i;
                chorda  fructus;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (!piscina)
    {
        redde fructus;
    }

    si (numerus == ZEPHYRUM || elementa == NIHIL)
    {
        redde fructus;
    }

    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (aed == NIHIL)
    {
        redde fructus;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   i > ZEPHYRUM && separator.datum != NIHIL
            && separator.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(aed, separator);
        }
        si (elementa[i].datum != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed, elementa[i]);
        }
    }

    redde chorda_aedificator_finire(aed);
}


/* ==================================================
 * Formatatio
 * ================================================== */

chorda
chorda_ex_bytes_legibilis (
            i64  bytes,
        Piscina* piscina)
{
                chorda  fructus;
             character  buffer[XXXII];
    constans character* suffixum;
                   f64  valor;
               integer  longitudo;

    fructus.datum    = NIHIL;
    fructus.mensura  = 0;

    si (piscina == NIHIL)
    {
        redde fructus;
    }

    /* Determinare suffixum et valorem (bytes i64 insignatus -
     * cohibitio negativi mortua erat, remota 2026-07-17) */
    si (bytes < M * M)  /* < 1 MB */
    {
        si (bytes < M)  /* < 1 KB */
        {
            valor     = (f64)bytes;
            suffixum  = " B";
        }
        alioquin
        {
            valor     = (f64)bytes / (f64)M;
            suffixum  = " KB";
        }
    }
    alioquin si (bytes < (i64)M * (i64)M * (i64)M)  /* < 1 GB */
    {
        valor     = (f64)bytes / (f64)(M * M);
        suffixum  = " MB";
    }
    alioquin
    {
        valor     = (f64)bytes / (f64)((i64)M * (i64)M * (i64)M);
        suffixum  = " GB";
    }

    /* Formatare */
    si (valor < 10.0)
    {
        longitudo = snprintf(buffer, XXXII, "%.1f%s", valor, suffixum);
    }
    alioquin si (valor < 100.0)
    {
        longitudo = snprintf(buffer, XXXII, "%.1f%s", valor, suffixum);
    }
    alioquin
    {
        longitudo = snprintf(buffer, XXXII, "%.0f%s", valor, suffixum);
    }

    si (longitudo <= 0 || longitudo >= (integer)XXXII)
    {
        redde fructus;
    }

    fructus = chorda_ex_literis(buffer, piscina);

    redde fructus;
}
#undef CHORDA_FRIATUM_OFFSET
#undef CHORDA_FRIATUM_PRIMUS
#undef _chorda_nullum_habet
#undef _extrahere_verba
/* lib/magnus.c: statica per plagulam renominata */
#define MagnusAlternae MagnusAlternae_magnus
#define _absolutum_s64 _absolutum_s64_magnus
#define _alternae_aperire _alternae_aperire_magnus
#define _alternae_claudere _alternae_claudere_magnus
#define _alternae_illic _alternae_illic_magnus
#define _alternae_vertere _alternae_vertere_magnus
#define _apex_alternarum _apex_alternarum_magnus
#define _apex_notare _apex_notare_magnus
#define _aspectus _aspectus_magnus
#define _ex_moduli_propriis _ex_moduli_propriis_magnus
#define _ex_signo_et_modulo _ex_signo_et_modulo_magnus
#define _longitudo_vera _longitudo_vera_magnus
#define _membra_nova _membra_nova_magnus
#define _moduli_adde _moduli_adde_magnus
#define _moduli_compara _moduli_compara_magnus
#define _moduli_divide _moduli_divide_magnus
#define _moduli_divide_parvo _moduli_divide_parvo_magnus
#define _moduli_multiplica _moduli_multiplica_magnus
#define _moduli_subtrahe _moduli_subtrahe_magnus
#define _parvus _parvus_magnus
#define _per_alternas _per_alternas_magnus
#define _summa_signata _summa_signata_magnus
#define _transcribere _transcribere_magnus
#line 1 "lib/magnus.c"
/* magnus.c - Integri magni exacti
 *
 * Moduli (valores absoluti) in membris XXXII bitorum insignatis,
 * ordine parvo primum; producta et portationes in i64 insignato, ubi
 * omnis gradus exactus est: (2^32 - 1)^2 + 2(2^32 - 1) = 2^64 - 1.
 * Arithmetica insignata in membris ubique (C definit circumvolutionem
 * insignatam; exundatio signata indefinita est). Via celeris in s64
 * limites ANTE operationem probat, numquam post. Divisio: Knuth,
 * algorithmus D (TAOCP II, 4.3.1). Nulli fluitantes.
 * Vide lib/magnus.worklog.md.
 */


#define MAGNUS_S64_SUMMUS       ((s64)0x7FFFFFFFFFFFFFFFLL)
#define MAGNUS_S64_IMUS         (-MAGNUS_S64_SUMMUS - I)
#define MAGNUS_LIMES_NEGATIVUS  0x8000000000000000ULL   /* |S64_IMUS| */
#define MAGNUS_BASIS            0x100000000ULL           /* 2^32 */
#define MAGNUS_FRUSTUM          1000000000U              /* 10^9 */
#define MAGNUS_FACTOR_PARVUS    0x7FFFFFFFLL             /* 2^31 - 1 */


/* ==================================================
 * Auxilia: creatio canonica
 * ================================================== */

interior Magnus
_parvus (
    s64 valor)
{
    Magnus a;

    a.parvus     = valor;
    a.signum     = ZEPHYRUM;
    a.longitudo  = ZEPHYRUM;
    a.membra     = NIHIL;
    redde a;
}

/* |valor| sine exundatione: -(valor + 1) semper capit */
interior i64
_absolutum_s64 (
    s64 valor)
{
    si (valor < ZEPHYRUM)
    {
        redde (i64)(-(valor + I)) + 1ULL;
    }
    redde (i64)valor;
}

interior i32*
_membra_nova (
    Piscina* piscina,
        i32  numerus)
{
    i32* membra;
    i32  k;

    membra = (i32*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)numerus * magnitudo(i32), magnitudo(i32));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        membra[k] = ZEPHYRUM;
    }
    redde membra;
}

/* longitudo sine membris summis nullis */
interior i32
_longitudo_vera (
    constans i32* moduli,
             i32  longitudo)
{
    dum (longitudo > ZEPHYRUM && moduli[longitudo - I] == ZEPHYRUM)
    {
        longitudo--;
    }
    redde longitudo;
}

/* valor ex signo et modulo i64; piscina tantum si non capit */
interior Magnus
_ex_signo_et_modulo (
         s32  signum,
         i64  modulus,
    Piscina*  piscina)
{
    Magnus a;

    si (modulus == ZEPHYRUM)
    {
        redde _parvus(ZEPHYRUM);
    }
    si (signum > ZEPHYRUM && modulus <= (i64)MAGNUS_S64_SUMMUS)
    {
        redde _parvus((s64)modulus);
    }
    si (signum < ZEPHYRUM && modulus <= MAGNUS_LIMES_NEGATIVUS)
    {
        redde _parvus(-(s64)(modulus - 1ULL) - I);
    }
    a.parvus            = ZEPHYRUM;
    a.signum            = signum;
    a.longitudo         = II;
    a.membra            = _membra_nova(piscina, II);
    a.membra[ZEPHYRUM]  = (i32)modulus;
    a.membra[I]         = (i32)(modulus >> XXXII);
    redde a;
}

/* forma canonica ex membris PROPRIIS (ex piscina aut membris
 * immutabilibus alius valoris): numquam alveum temporarium recipit,
 * nisi valor certe in s64 capit */
interior Magnus
_ex_moduli_propriis (
    s32  signum,
    i32* moduli,
    i32  longitudo)
{
    Magnus a;

    longitudo = _longitudo_vera(moduli, longitudo);
    si (longitudo == ZEPHYRUM)
    {
        redde _parvus(ZEPHYRUM);
    }
    si (longitudo <= II)
    {
        i64 modulus = (i64)moduli[ZEPHYRUM];

        si (longitudo == II)
        {
            modulus |= (i64)moduli[I] << XXXII;
        }
        si (signum > ZEPHYRUM && modulus <= (i64)MAGNUS_S64_SUMMUS)
        {
            redde _parvus((s64)modulus);
        }
        si (signum < ZEPHYRUM && modulus <= MAGNUS_LIMES_NEGATIVUS)
        {
            redde _parvus(-(s64)(modulus - 1ULL) - I);
        }
    }
    a.parvus     = ZEPHYRUM;
    a.signum     = signum;
    a.longitudo  = longitudo;
    a.membra     = moduli;
    redde a;
}

/* aspectus moduli: parvi in alveum II membrorum explicantur */
interior vacuum
_aspectus (
             Magnus   a,
                i32*  alveus,
    constans    i32** moduli,
                i32*  longitudo,
                s32*  signum)
{
    si (a.membra == NIHIL)
    {
        i64 modulus = _absolutum_s64(a.parvus);

        alveus[ZEPHYRUM] = (i32)modulus;
        alveus[I] = (i32)(modulus >> XXXII);
        *moduli = alveus;
        *longitudo = _longitudo_vera(alveus, II);
        *signum = (a.parvus > ZEPHYRUM) - (a.parvus < ZEPHYRUM);
    }
    alioquin
    {
        *moduli     = a.membra;
        *longitudo  = a.longitudo;
        *signum     = a.signum;
    }
}


/* ==================================================
 * Auxilia: arithmetica modulorum
 * ================================================== */

/* -1, 0, +1 */
interior s32
_moduli_compara (
    constans i32* a,
             i32  la,
    constans i32* b,
             i32  lb)
{
    s32 k;

    si (la != lb)
    {
        redde (la < lb) ? -I : I;
    }
    per (k = (s32)la - I; k >= ZEPHYRUM; k--)
    {
        si (a[k] != b[k])
        {
            redde (a[k] < b[k]) ? -I : I;
        }
    }
    redde ZEPHYRUM;
}

/* exitus longitudinis max(la, lb) + 1 */
interior vacuum
_moduli_adde (
    constans i32* a,
             i32  la,
    constans i32* b,
             i32  lb,
             i32* exitus)
{
    i64 portans          = ZEPHYRUM;
    i32 summa_longitudo  = (la > lb) ? la : lb;
    i32 k;

    per (k = ZEPHYRUM; k < summa_longitudo; k++)
    {
        i64 t = portans;

        si (k < la) t += a[k];
        si (k < lb) t += b[k];
        exitus[k]  = (i32)t;
        portans    = t >> XXXII;
    }
    exitus[summa_longitudo] = (i32)portans;
}

/* a >= b; exitus longitudinis la */
interior vacuum
_moduli_subtrahe (
    constans i32* a,
             i32  la,
    constans i32* b,
             i32  lb,
             i32* exitus)
{
    i64 mutuum = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < la; k++)
    {
        i64 d = (i64)a[k] - mutuum;

        si (k < lb) d -= b[k];
        exitus[k]  = (i32)d;
        mutuum     = d >> LXIII;   /* circumvolutio: bit summum */
    }
}

/* exitus nullus, longitudinis la + lb */
interior vacuum
_moduli_multiplica (
    constans i32* a,
             i32  la,
    constans i32* b,
             i32  lb,
             i32* exitus)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < la; k++)
    {
        i64 portans = ZEPHYRUM;

        per (j = ZEPHYRUM; j < lb; j++)
        {
            i64 t = (i64)a[k] * b[j] + exitus[k + j] + portans;

            exitus[k + j]  = (i32)t;
            portans        = t >> XXXII;
        }
        exitus[k + lb] = (i32)portans;
    }
}

/* quotiens in exitum (idem ac a licet); residuum redditur */
interior i32
_moduli_divide_parvo (
    constans i32* a,
             i32  la,
             i32  divisor,
             i32* exitus)
{
    i64 residuum = ZEPHYRUM;
    s32 k;

    per (k = (s32)la - I; k >= ZEPHYRUM; k--)
    {
        i64 numerus = (residuum << XXXII) | a[k];

        exitus[k]  = (i32)(numerus / divisor);
        residuum   = numerus % divisor;
    }
    redde (i32)residuum;
}

/* Knuth D: u (lu membra) / v (lv >= 2 membra), lu >= lv, v summum
 * non nullum. quotiens: lu - lv + 1 membra; residuum: lv membra. */
interior vacuum
_moduli_divide (
    constans i32* u,
             i32  lu,
    constans i32* v,
             i32  lv,
             i32* quotiens,
             i32* residuum,
         Piscina* piscina)
{
    i32* un  = _membra_nova(piscina, lu + I);
    i32* vn  = _membra_nova(piscina, lv);
    i32  s   = ZEPHYRUM;
    i32  summum;
    s32  j;
    i32  k;

    /* D1: normalizatio - divisor summum bit habeat */
    summum = v[lv - I];
    dum (!(summum & 0x80000000U))
    {
        summum <<= I;
        s++;
    }
    per (k = lv - I; k > ZEPHYRUM; k--)
    {
        vn[k] = (i32)((((i64)v[k] << XXXII) | v[k - I]) >> (XXXII - s));
    }
    vn[ZEPHYRUM]  = v[ZEPHYRUM] << s;
    un[lu]        = (i32)((i64)u[lu - I] >> (XXXII - s));
    per (k = lu - I; k > ZEPHYRUM; k--)
    {
        un[k] = (i32)((((i64)u[k] << XXXII) | u[k - I]) >> (XXXII - s));
    }
    un[ZEPHYRUM] = u[ZEPHYRUM] << s;

    per (j = (s32)(lu - lv); j >= ZEPHYRUM; j--)
    {
        i64 numerus;
        i64 aestimatio;
        i64 reliquum;
        i64 portans  = ZEPHYRUM;
        i64 mutuum   = ZEPHYRUM;
        i64 d;
        i32 jj = (i32)j;

        /* D3: aestimatio quotientis ex duobus membris summis */
        numerus     = ((i64)un[jj + lv] << XXXII) | un[jj + lv - I];
        aestimatio  = numerus / vn[lv - I];
        reliquum    = numerus - aestimatio * vn[lv - I];
        dum (   aestimatio >= MAGNUS_BASIS
             || aestimatio * vn[lv - II]
            > ((reliquum << XXXII) | un[jj + lv - II]))
        {
            aestimatio--;
            reliquum += vn[lv - I];
            si (reliquum >= MAGNUS_BASIS)
            {
                frange;
            }
        }

        /* D4: multiplica et subtrahe */
        per (k = ZEPHYRUM; k < lv; k++)
        {
            i64 p = aestimatio * vn[k] + portans;

            portans     = p >> XXXII;
            d           = (i64)un[k + jj] - (i32)p - mutuum;
            un[k + jj]  = (i32)d;
            mutuum      = d >> LXIII;
        }
        d            = (i64)un[jj + lv] - portans - mutuum;
        un[jj + lv]  = (i32)d;

        /* D6: aestimatio uno nimia (rarissime) - adde retro */
        si (d >> LXIII)
        {
            aestimatio--;
            portans = ZEPHYRUM;
            per (k = ZEPHYRUM; k < lv; k++)
            {
                i64 t = (i64)un[k + jj] + vn[k] + portans;

                un[k + jj]  = (i32)t;
                portans     = t >> XXXII;
            }
            /* portatio ultima neglegitur (Knuth D6): un[jj + lv]
             * post hunc gradum non legitur */
        }
        quotiens[jj] = (i32)aestimatio;
    }

    /* D8: denormalizatio residui */
    per (k = ZEPHYRUM; k + I < lv; k++)
    {
        residuum[k] = (i32)((((i64)un[k + I] << XXXII) | un[k]) >> s);
    }
    residuum[lv - I] = un[lv - I] >> s;
}


/* ==================================================
 * Creatio et conversio
 * ================================================== */

Magnus
magnus_ex_s64 (
    s64 valor)
{
    redde _parvus(valor);
}

b32
magnus_ad_s64 (
    Magnus  a,
       s64* exitus)
{
    si (a.membra != NIHIL)
    {
        redde FALSUM;
    }
    si (exitus)
    {
        *exitus = a.parvus;
    }
    redde VERUM;
}

b32
magnus_ex_chorda (
      chorda  textus,
     Piscina* piscina,
      Magnus* exitus)
{
    i32  positus  = ZEPHYRUM;
    s32  signum   = I;
    i32  numerus_digitorum;
    i32  frustum;
    i32  longitudo = ZEPHYRUM;
    i32* moduli;
    i32  k;

    si (textus.mensura == ZEPHYRUM || textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    si (textus.datum[ZEPHYRUM] == '-')
    {
        signum   = -I;
        positus  = I;
    }
    si (positus == textus.mensura)
    {
        redde FALSUM;
    }
    per (k = positus; k < textus.mensura; k++)
    {
        si (textus.datum[k] < '0' || textus.datum[k] > '9')
        {
            redde FALSUM;
        }
    }

    numerus_digitorum = textus.mensura - positus;
    moduli = _membra_nova(piscina, numerus_digitorum / IX + II);

    /* frusta IX digitorum a sinistra; primum frustum residuum */
    frustum = numerus_digitorum % IX;
    si (frustum == ZEPHYRUM)
    {
        frustum = IX;
    }
    dum (positus < textus.mensura)
    {
        i32 valor          = ZEPHYRUM;
        i32 multiplicator  = I;
        i64 portans;
        i32 m;

        per (m = ZEPHYRUM; m < frustum; m++)
        {
            valor = valor * X + (i32)(textus.datum[positus + m]
                - '0');
            multiplicator *= X;
        }
        positus += frustum;
        frustum = IX;

        /* moduli = moduli * multiplicator + valor */
        portans = valor;
        per (m = ZEPHYRUM; m < longitudo; m++)
        {
            i64 t = (i64)moduli[m] * multiplicator + portans;

            moduli[m]  = (i32)t;
            portans    = t >> XXXII;
        }
        si (portans)
        {
            moduli[longitudo++] = (i32)portans;
        }
    }

    *exitus = _ex_moduli_propriis(signum, moduli, longitudo);
    redde VERUM;
}

chorda
magnus_ad_chordam (
      Magnus  a,
     Piscina* piscina)
{
               i32  alveus[II];
    constans   i32* moduli;
               i32  longitudo;
               s32  signum;
               i32* opus;
               i32* frusta;
               i32  numerus_frustorum = ZEPHYRUM;
                i8* litterae;
               i32  positus = ZEPHYRUM;
               i32  k;

    _aspectus(a, alveus, &moduli, &longitudo, &signum);
    si (longitudo == ZEPHYRUM)
    {
        litterae            = (i8*)piscina_allocare(piscina, I);
        litterae[ZEPHYRUM]  = '0';
        redde chorda_ex_buffer(litterae, I);
    }

    /* frusta 10^9 a dextra, divisione parva repetita */
    opus    = _membra_nova(piscina, longitudo);
    frusta  = _membra_nova(piscina, longitudo * II + II);
    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        opus[k] = moduli[k];
    }
    dum (longitudo > ZEPHYRUM)
    {
        frusta[numerus_frustorum++] =
            _moduli_divide_parvo(opus, longitudo, MAGNUS_FRUSTUM, opus);
        longitudo = _longitudo_vera(opus, longitudo);
    }

    /* IX digiti per frustum 10^9; signum unum */
    litterae = (i8*)piscina_allocare(piscina,
        (memoriae_index)numerus_frustorum * IX + II);
    si (signum < ZEPHYRUM)
    {
        litterae[positus++] = '-';
    }
    {
         i8 digiti[X];
        i32 numerus  = ZEPHYRUM;
        i32 summum   = frusta[numerus_frustorum - I];

        dum (summum > ZEPHYRUM)
        {
            digiti[numerus++]  = (i8)('0' + summum % X);
            summum             /= X;
        }
        dum (numerus > ZEPHYRUM)
        {
            litterae[positus++] = digiti[--numerus];
        }
    }
    per (k = numerus_frustorum - I; k > ZEPHYRUM; k--)
    {
        i32 frustum = frusta[k - I];
        s32 m;

        per (m = VIII; m >= ZEPHYRUM; m--)
        {
            litterae[positus + (i32)m]  = (i8)('0' + frustum % X);
            frustum                     /= X;
        }
        positus += IX;
    }
    redde chorda_ex_buffer(litterae, positus);
}


/* ==================================================
 * Inspectio
 * ================================================== */

s32
magnus_signum (
    Magnus a)
{
    si (a.membra != NIHIL)
    {
        redde a.signum;
    }
    redde (a.parvus > ZEPHYRUM) - (a.parvus < ZEPHYRUM);
}

s32
magnus_compara (
    Magnus a,
    Magnus b)
{
               i32  alveus_a[II];
               i32  alveus_b[II];
    constans   i32* ma;
    constans   i32* mb;
               i32  la;
               i32  lb;
               s32  sa;
               s32  sb;
               s32  ordo;

    si (a.membra == NIHIL && b.membra == NIHIL)
    {
        redde (a.parvus > b.parvus) - (a.parvus < b.parvus);
    }
    _aspectus(a, alveus_a, &ma, &la, &sa);
    _aspectus(b, alveus_b, &mb, &lb, &sb);
    si (sa != sb)
    {
        redde (sa < sb) ? -I : I;
    }
    ordo = _moduli_compara(ma, la, mb, lb);
    redde (sa >= ZEPHYRUM) ? ordo : -ordo;
}

b32
magnus_aequalis (
    Magnus a,
    Magnus b)
{
    redde magnus_compara(a, b) == ZEPHYRUM;
}


/* ==================================================
 * Arithmetica
 * ================================================== */

Magnus
magnus_nega (
      Magnus  a,
    Piscina*  piscina)
{
    si (a.membra == NIHIL)
    {
        si (a.parvus != MAGNUS_S64_IMUS)
        {
            redde _parvus(-a.parvus);
        }
        redde _ex_signo_et_modulo(I, MAGNUS_LIMES_NEGATIVUS, piscina);
    }
    /* membra partita: +2^63 negatum in s64 capit */
    redde _ex_moduli_propriis(-a.signum, a.membra, a.longitudo);
}

Magnus
magnus_absolutum (
      Magnus  a,
     Piscina* piscina)
{
    si (magnus_signum(a) >= ZEPHYRUM)
    {
        redde a;
    }
    redde magnus_nega(a, piscina);
}

/* summa signata modulorum; semper in membra nova */
interior Magnus
_summa_signata (
             Magnus  a,
             Magnus  b,
                s32  flexio_b,
            Piscina* piscina)
{
               i32  alveus_a[II];
               i32  alveus_b[II];
    constans   i32* ma;
    constans   i32* mb;
               i32  la;
               i32  lb;
               s32  sa;
               s32  sb;
               i32* exitus;

    _aspectus(a, alveus_a, &ma, &la, &sa);
    _aspectus(b, alveus_b, &mb, &lb, &sb);
    sb *= flexio_b;

    si (sa == ZEPHYRUM || sb == ZEPHYRUM || sa == sb)
    {
        i32 longitudo = ((la > lb) ? la : lb) + I;

        exitus = _membra_nova(piscina, longitudo);
        _moduli_adde(ma, la, mb, lb, exitus);
        redde _ex_moduli_propriis((sa != ZEPHYRUM) ? sa : sb,
            exitus, longitudo);
    }
    si (_moduli_compara(ma, la, mb, lb) >= ZEPHYRUM)
    {
        exitus = _membra_nova(piscina, la);
        _moduli_subtrahe(ma, la, mb, lb, exitus);
        redde _ex_moduli_propriis(sa, exitus, la);
    }
    exitus = _membra_nova(piscina, lb);
    _moduli_subtrahe(mb, lb, ma, la, exitus);
    redde _ex_moduli_propriis(sb, exitus, lb);
}

Magnus
magnus_adde (
      Magnus  a,
      Magnus  b,
    Piscina*  piscina)
{
    si (a.membra == NIHIL && b.membra == NIHIL)
    {
        s64 x = a.parvus;
        s64 y = b.parvus;

        si (!((y > ZEPHYRUM && x > MAGNUS_S64_SUMMUS - y)
            || (y < ZEPHYRUM && x < MAGNUS_S64_IMUS - y)))
        {
            redde _parvus(x + y);
        }
    }
    redde _summa_signata(a, b, I, piscina);
}

Magnus
magnus_subtrahe (
      Magnus  a,
      Magnus  b,
    Piscina*  piscina)
{
    si (a.membra == NIHIL && b.membra == NIHIL)
    {
        s64 x = a.parvus;
        s64 y = b.parvus;

        si (!((y < ZEPHYRUM && x > MAGNUS_S64_SUMMUS + y)
            || (y > ZEPHYRUM && x < MAGNUS_S64_IMUS + y)))
        {
            redde _parvus(x - y);
        }
    }
    redde _summa_signata(a, b, -I, piscina);
}

Magnus
magnus_multiplica (
      Magnus  a,
      Magnus  b,
     Piscina* piscina)
{
               i32  alveus_a[II];
               i32  alveus_b[II];
    constans   i32* ma;
    constans   i32* mb;
               i32  la;
               i32  lb;
               s32  sa;
               s32  sb;
               i32* exitus;

    si (a.membra == NIHIL && b.membra == NIHIL)
    {
        s64 x = a.parvus;
        s64 y = b.parvus;

        /* |x|, |y| < 2^31 => |x*y| < 2^62 */
        si (   x >= -MAGNUS_FACTOR_PARVUS && x <= MAGNUS_FACTOR_PARVUS
            && y >= -MAGNUS_FACTOR_PARVUS && y <= MAGNUS_FACTOR_PARVUS)
        {
            redde _parvus(x * y);
        }
    }
    _aspectus(a, alveus_a, &ma, &la, &sa);
    _aspectus(b, alveus_b, &mb, &lb, &sb);
    si (la == ZEPHYRUM || lb == ZEPHYRUM)
    {
        redde _parvus(ZEPHYRUM);
    }
    exitus = _membra_nova(piscina, la + lb);
    _moduli_multiplica(ma, la, mb, lb, exitus);
    redde _ex_moduli_propriis(sa * sb, exitus, la + lb);
}

b32
magnus_divide (
      Magnus  a,
      Magnus  divisor,
     Piscina* piscina,
      Magnus* quotiens,
      Magnus* residuum)
{
               i32  alveus_a[II];
               i32  alveus_b[II];
    constans   i32* ma;
    constans   i32* mb;
               i32  la;
               i32  lb;
               s32  sa;
               s32  sb;
               i32* q;
               i32* r;
               i32  lq;
               i32  k;

    si (magnus_signum(divisor) == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* via celeris: moduli in i64, divisio insignata (definita) */
    si (a.membra == NIHIL && divisor.membra == NIHIL)
    {
        i64 modulus_a  = _absolutum_s64(a.parvus);
        i64 modulus_b  = _absolutum_s64(divisor.parvus);
        i64 q0         = modulus_a / modulus_b;
        i64 r0         = modulus_a % modulus_b;
        s32 signum_b   = (divisor.parvus > ZEPHYRUM) ? I : -I;

        si (a.parvus < ZEPHYRUM && r0 != ZEPHYRUM)
        {
            q0 += 1ULL;
            r0 = modulus_b - r0;
        }
        si (quotiens)
        {
            *quotiens = _ex_signo_et_modulo(
                (a.parvus < ZEPHYRUM) ? -signum_b : signum_b, q0,
                piscina);
        }
        si (residuum)
        {
            *residuum = _parvus((s64)r0);
        }
        redde VERUM;
    }

    _aspectus(a, alveus_a, &ma, &la, &sa);
    _aspectus(divisor, alveus_b, &mb, &lb, &sb);

    lq  = (la >= lb) ? la - lb + I : I;
    q   = _membra_nova(piscina, lq + I);
    r   = _membra_nova(piscina, lb + I);
    si (_moduli_compara(ma, la, mb, lb) < ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < la; k++)
        {
            r[k] = ma[k];
        }
    }
    alioquin si (lb == I)
    {
        r[ZEPHYRUM] = _moduli_divide_parvo(ma, la, mb[ZEPHYRUM], q);
    }
    alioquin
    {
        _moduli_divide(ma, la, mb, lb, q, r, piscina);
    }

    /* Euclidea: a < 0 et residuum non nullum => q + 1, |b| - r */
    si (sa < ZEPHYRUM && _longitudo_vera(r, lb) > ZEPHYRUM)
    {
        i32  unum[I];
        i32* q2 = _membra_nova(piscina, lq + I);
        i32* r2 = _membra_nova(piscina, lb + I);

        unum[ZEPHYRUM] = I;
        _moduli_adde(q, lq, unum, I, q2);
        _moduli_subtrahe(mb, lb, r, _longitudo_vera(r, lb), r2);
        q = q2;
        r = r2;
    }
    si (quotiens)
    {
        *quotiens = _ex_moduli_propriis((sa < ZEPHYRUM) ? -sb : sb, q,
            lq + I);
    }
    si (residuum)
    {
        *residuum = _ex_moduli_propriis(I, r, lb);
    }
    redde VERUM;
}

Magnus
magnus_potentia (
      Magnus  basis,
         i32  exponens,
     Piscina* piscina)
{
    Magnus fructus = _parvus(I);

    dum (exponens > ZEPHYRUM)
    {
        si (exponens & I)
        {
            fructus = magnus_multiplica(fructus, basis, piscina);
        }
        exponens >>= I;
        si (exponens > ZEPHYRUM)
        {
            basis = magnus_multiplica(basis, basis, piscina);
        }
    }
    redde fructus;
}

/* Euclides in piscinis ALTERNIS. Gradus quisque in piscinam
 * alteram computat, valores qui supersunt eo transcribit, priorem ad
 * notam initialem reficit: memoria ergo proportionalis magnitudini
 * operandorum, non gradibus (recensio I, 2026-10-05: olim CXX MB pro
 * mdc X milium digitorum, omnia in piscina vocantis). Effectus solus
 * in piscinam vocantis transcribitur. */
nomen structura {
           Piscina* piscinae[II];
    PiscinaNotatio  notae[II];
               i32  hic;
} MagnusAlternae;

/* Operandi usque ad IV membra (~XXXVIII digiti): Euclides in piscina
 * vocantis, ut olim - piscinae alternae pro numeris parvis plus
 * constant quam servant (recensio II: mdc XX digitorum 0.46 -> 2.35
 * us); iactura vocantis paucis milibus octetorum finitur. */
#define MAGNUS_LIMES_ALTERNARUM IV

/* DIAGNOSIS (agenda A3): maximus usus piscinae alternae in ultimo
 * divisore communi per alternas computato */
interior memoriae_index _apex_alternarum = ZEPHYRUM;

/* Divisor communis SINE testibus: AMBO magni requiruntur - si unus
 * parvus est, gradus primus omnia parva facit, ergo Euclides in
 * piscina vocantis finitus manet (recensio III: mdc(magnus, 1) in
 * fractione piscinas alternas sine causa aperiebat - saltus temporis
 * ad XL digitos). CUM testibus: UNUS magnus sufficit - post gradum
 * primum testis operandi parvi ~ |a|/g magnus est et in omni gradu
 * sequente novus fit (recensio IV: K F184 + F183 et F184, X M
 * digitorum: MB 1.5 in piscina vocantis pro KB IV effectus). */
interior b32
_per_alternas (
    Magnus a,
    Magnus b,
       b32 ambo)
{
    b32 a_magnus = a.membra != NIHIL
        && a.longitudo > MAGNUS_LIMES_ALTERNARUM;
    b32 b_magnus = b.membra != NIHIL
        && b.longitudo > MAGNUS_LIMES_ALTERNARUM;

    redde ambo ? (a_magnus && b_magnus) : (a_magnus || b_magnus);
}

interior vacuum
_apex_notare (
    Piscina* piscina)
{
    memoriae_index usus = piscina_summa_usus(piscina);

    si (usus > _apex_alternarum)
    {
        _apex_alternarum = usus;
    }
}

interior b32
_alternae_aperire (
    MagnusAlternae* al)
{
    al->piscinae[ZEPHYRUM]  =
        piscina_generare_dynamicum("magnus_alterna",
        (memoriae_index)4096);
    al->piscinae[I]         =
        piscina_generare_dynamicum("magnus_alterna",
        (memoriae_index)4096);
    al->hic           = ZEPHYRUM;
    _apex_alternarum  = ZEPHYRUM;
    si (al->piscinae[ZEPHYRUM] == NIHIL || al->piscinae[I] == NIHIL)
    {
        si (al->piscinae[ZEPHYRUM])
        {
            piscina_destruere(al->piscinae[ZEPHYRUM]);
        }
        si (al->piscinae[I])
        {
            piscina_destruere(al->piscinae[I]);
        }
        redde FALSUM;
    }
    al->notae[ZEPHYRUM]  = piscina_notare(al->piscinae[ZEPHYRUM]);
    al->notae[I]         = piscina_notare(al->piscinae[I]);
    redde VERUM;
}

interior Piscina*
_alternae_illic (
    constans MagnusAlternae* al)
{
    redde al->piscinae[I - al->hic];
}

/* Piscina currens ad notam initialem reficitur, NON vacatur:
 * piscina_vacare totam capacitatem memset implet, et id gradu quoque
 * erat sumptus a recensione II inventus (membra nova a _membra_nova
 * iam nullantur). Altera fit currens. */
interior vacuum
_alternae_vertere (
    MagnusAlternae* al)
{
    _apex_notare(al->piscinae[al->hic]);
    piscina_reficere(al->piscinae[al->hic], al->notae[al->hic]);
    al->hic = I - al->hic;
}

interior vacuum
_alternae_claudere (
    MagnusAlternae* al)
{
    _apex_notare(al->piscinae[ZEPHYRUM]);
    _apex_notare(al->piscinae[I]);
    piscina_destruere(al->piscinae[ZEPHYRUM]);
    piscina_destruere(al->piscinae[I]);
}

/* copia valoris in piscinam datam (parvi sine allocatione) */
interior Magnus
_transcribere (
      Magnus  a,
     Piscina* piscina)
{
    Magnus copia = a;
       i32 k;

    si (a.membra == NIHIL)
    {
        redde a;
    }
    copia.membra = _membra_nova(piscina, a.longitudo);
    per (k = ZEPHYRUM; k < a.longitudo; k++)
    {
        copia.membra[k] = a.membra[k];
    }
    redde copia;
}

i32
magnus_bitorum (
    Magnus a)
{
               i32  alveus[II];
    constans   i32* moduli;
               i32  longitudo;
               s32  signum;
               i32  summum;
               i32  bita;

    si (magnus_signum(a) == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    _aspectus(a, alveus, &moduli, &longitudo, &signum);
    summum  = moduli[longitudo - I];
    bita    = (longitudo - I) * XXXII;
    dum (summum != ZEPHYRUM)
    {
        bita++;
        summum = summum >> I;
    }
    redde bita;
}

i32
magnus_residuum_parvum (
    Magnus a,
       i32 n)
{
    i64 residuum = ZEPHYRUM;
    i32 k;

    si (n == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (a.membra == NIHIL)
    {
        /* |parvus| sine exundatione etiam pro S64 imo */
        i64 modulus = a.parvus < ZEPHYRUM
            ? (i64)(-(a.parvus + I)) + (i64)I : (i64)a.parvus;

        residuum = modulus % (i64)n;
        si (a.parvus < ZEPHYRUM && residuum != ZEPHYRUM)
        {
            residuum = (i64)n - residuum;
        }
        redde (i32)residuum;
    }
    /* Horner super membra ab summo: residuum < n < 2^32, ergo
     * (residuum << 32) | membrum < 2^64 */
    per (k = a.longitudo; k-- > ZEPHYRUM;)
    {
        residuum = ((residuum << XXXII) | (i64)a.membra[k]) % (i64)n;
    }
    si (a.signum < ZEPHYRUM && residuum != ZEPHYRUM)
    {
        residuum = (i64)n - residuum;
    }
    redde (i32)residuum;
}

Magnus
magnus_transcribe (
      Magnus  a,
     Piscina* piscina)
{
    redde _transcribere(a, piscina);
}

Magnus
magnus_divisor_communis (
      Magnus  a,
      Magnus  b,
     Piscina* piscina)
{
    MagnusAlternae al;
            Magnus x = magnus_absolutum(a, piscina);
            Magnus y = magnus_absolutum(b, piscina);

    _apex_alternarum = ZEPHYRUM;
    /* operandus pauci membrorum: in piscina vocantis (vide
     * _per_alternas) */
    si (!_per_alternas(a, b, VERUM) || !_alternae_aperire(&al))
    {
        dum (magnus_signum(y) != ZEPHYRUM)
        {
            Magnus r;

            (vacuum)magnus_divide(x, y, piscina, NIHIL, &r);
            x = y;
            y = r;
        }
        redde x;
    }
    dum (magnus_signum(y) != ZEPHYRUM)
    {
        Piscina* illic = _alternae_illic(&al);
         Magnus  r;

        (vacuum)magnus_divide(x, y, illic, NIHIL, &r);
        x = _transcribere(y, illic);
        y = r;
        _alternae_vertere(&al);
    }
    x = _transcribere(x, piscina);
    _alternae_claudere(&al);
    redde x;
}

Magnus
magnus_divisor_communis_testatus (
      Magnus  a,
      Magnus  b,
     Piscina* piscina,
      Magnus* u,
      Magnus* v)
{
    MagnusAlternae al;
               b32 alternae;
            Magnus r0 = magnus_absolutum(a, piscina);
            Magnus r1 = magnus_absolutum(b, piscina);
            Magnus s0 = _parvus(I);
            Magnus s1 = _parvus(ZEPHYRUM);
            Magnus t0 = _parvus(ZEPHYRUM);
            Magnus t1 = _parvus(I);

    /* ambo operandi pauci membrorum: testes |s| <= |b|, |t| <= |a|
     * parvi manent, ergo in piscina vocantis (vide _per_alternas) */
    _apex_alternarum  = ZEPHYRUM;
    alternae          = _per_alternas(a, b, FALSUM)
        && _alternae_aperire(&al);

    dum (magnus_signum(r1) != ZEPHYRUM)
    {
        Piscina* illic = alternae ? _alternae_illic(&al) : piscina;
         Magnus  q;
         Magnus  r2;
         Magnus  s2;
         Magnus  t2;

        (vacuum)magnus_divide(r0, r1, illic, &q, &r2);
        s2 = magnus_subtrahe(s0, magnus_multiplica(q, s1, illic),
            illic);
        t2 = magnus_subtrahe(t0, magnus_multiplica(q, t1, illic),
            illic);
        si (alternae)
        {
            r0 = _transcribere(r1, illic);
            s0 = _transcribere(s1, illic);
            t0 = _transcribere(t1, illic);
        }
        alioquin
        {
            r0 = r1;
            s0 = s1;
            t0 = t1;
        }
        r1 = r2;
        s1 = s2;
        t1 = t2;
        si (alternae)
        {
            _alternae_vertere(&al);
        }
    }
    si (alternae)
    {
        r0 = _transcribere(r0, piscina);
        s0 = _transcribere(s0, piscina);
        t0 = _transcribere(t0, piscina);
        _alternae_claudere(&al);
    }
    /* g = s0|a| + t0|b|: signa argumentorum in testes transfer */
    si (u)
    {
        *u = (magnus_signum(a) < ZEPHYRUM) ? magnus_nega(s0,
            piscina) : s0;
    }
    si (v)
    {
        *v = (magnus_signum(b) < ZEPHYRUM) ? magnus_nega(t0,
            piscina) : t0;
    }
    redde r0;
}

memoriae_index
magnus_apex_alternarum (
    vacuum)
{
    redde _apex_alternarum;
}
#undef MAGNUS_BASIS
#undef MAGNUS_FACTOR_PARVUS
#undef MAGNUS_FRUSTUM
#undef MAGNUS_LIMES_ALTERNARUM
#undef MAGNUS_LIMES_NEGATIVUS
#undef MAGNUS_S64_IMUS
#undef MAGNUS_S64_SUMMUS
#undef MagnusAlternae
#undef _absolutum_s64
#undef _alternae_aperire
#undef _alternae_claudere
#undef _alternae_illic
#undef _alternae_vertere
#undef _apex_alternarum
#undef _apex_notare
#undef _aspectus
#undef _ex_moduli_propriis
#undef _ex_signo_et_modulo
#undef _longitudo_vera
#undef _membra_nova
#undef _moduli_adde
#undef _moduli_compara
#undef _moduli_divide
#undef _moduli_divide_parvo
#undef _moduli_multiplica
#undef _moduli_subtrahe
#undef _parvus
#undef _per_alternas
#undef _summa_signata
#undef _transcribere
/* lib/fractio.c: statica per plagulam renominata */
#define _est_unum _est_unum_fractio
#define _ex_partibus _ex_partibus_fractio
#define _quotiens _quotiens_fractio
#line 1 "lib/fractio.c"
/* fractio.c - Numeri rationales exacti super magnum
 *
 * Omnis operatio formam canonicam reddit (denominator > 0, divisor
 * communis 1). Summa et productum per reductionem Henrici (Knuth II,
 * 4.5.1): divisores communes ante multiplicationem tolluntur, et
 * effectus sine reductione finali iam canonicus est. Divisio exacta
 * per divisionem Euclideam magni (residuum nullum). Nulli fluitantes.
 * Vide lib/fractio.worklog.md.
 */



/* ==================================================
 * Auxilia
 * ================================================== */

/* fractio ex partibus IAM canonicis */
interior Fractio
_ex_partibus (
    Magnus numerator,
    Magnus denominator)
{
    Fractio a;

    a.numerator    = numerator;
    a.denominator  = denominator;
    redde a;
}

/* quotiens Euclideus a / b (exactus ubi b a dividit) */
interior Magnus
_quotiens (
      Magnus  a,
      Magnus  b,
     Piscina* piscina)
{
    Magnus quotiens = magnus_ex_s64(ZEPHYRUM);

    (vacuum)magnus_divide(a, b, piscina, &quotiens, NIHIL);
    redde quotiens;
}

interior b32
_est_unum (
    Magnus a)
{
    redde magnus_aequalis(a, magnus_ex_s64(I));
}


/* ==================================================
 * Creatio et conversio
 * ================================================== */

Fractio
fractio_ex_s64 (
    s64 valor)
{
    redde _ex_partibus(magnus_ex_s64(valor), magnus_ex_s64(I));
}

Fractio
fractio_ex_magno (
    Magnus valor)
{
    redde _ex_partibus(valor, magnus_ex_s64(I));
}

b32
fractio_ex_magnis (
      Magnus  numerator,
      Magnus  denominator,
     Piscina* piscina,
     Fractio* exitus)
{
    Magnus divisor;

    si (magnus_signum(denominator) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* divisor communis(0, d) = |d|: nihil fit 0/1 */
    divisor      = magnus_divisor_communis(numerator, denominator,
        piscina);
    numerator    = _quotiens(numerator, divisor, piscina);
    denominator  = _quotiens(denominator, divisor, piscina);
    si (magnus_signum(denominator) < ZEPHYRUM)
    {
        numerator    = magnus_nega(numerator, piscina);
        denominator  = magnus_nega(denominator, piscina);
    }
    *exitus = _ex_partibus(numerator, denominator);
    redde VERUM;
}

b32
fractio_ex_s64_s64 (
         s64  numerator,
         s64  denominator,
     Piscina* piscina,
     Fractio* exitus)
{
    redde fractio_ex_magnis(magnus_ex_s64(numerator),
        magnus_ex_s64(denominator), piscina, exitus);
}

b32
fractio_ex_chorda (
      chorda  textus,
     Piscina* piscina,
     Fractio* exitus)
{
       i32 k;
       i32 vinculum = textus.mensura;
    Magnus numerator;
    Magnus denominator;

    si (textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == '/')
        {
            vinculum = k;
            frange;
        }
    }

    si (vinculum == textus.mensura)
    {
        si (!magnus_ex_chorda(textus, piscina, &numerator))
        {
            redde FALSUM;
        }
        *exitus = fractio_ex_magno(numerator);
        redde VERUM;
    }

    /* denominator: digiti soli (signum in numeratore tantum) */
    si (   vinculum + I >= textus.mensura
        || textus.datum[vinculum + I] < '0'
        || textus.datum[vinculum + I] > '9')
    {
        redde FALSUM;
    }
    si (   !magnus_ex_chorda(chorda_sectio(textus, ZEPHYRUM, vinculum),
            piscina, &numerator)
        || !magnus_ex_chorda(chorda_sectio(textus, vinculum + I,
            textus.mensura), piscina, &denominator))
    {
        redde FALSUM;
    }
    redde fractio_ex_magnis(numerator, denominator, piscina, exitus);
}

Fractio
fractio_transcribe (
     Fractio  a,
     Piscina* piscina)
{
    Fractio copia;

    copia.numerator    = magnus_transcribe(a.numerator, piscina);
    copia.denominator  = magnus_transcribe(a.denominator, piscina);
    redde copia;
}

chorda
fractio_ad_chordam (
     Fractio  a,
     Piscina* piscina)
{
    chorda textus = magnus_ad_chordam(a.numerator, piscina);

    si (_est_unum(a.denominator))
    {
        redde textus;
    }
    textus = chorda_concatenare(textus, chorda_ex_literis("/", piscina),
        piscina);
    redde chorda_concatenare(textus,
        magnus_ad_chordam(a.denominator, piscina), piscina);
}

Magnus
fractio_numerator (
    Fractio a)
{
    redde a.numerator;
}

Magnus
fractio_denominator (
    Fractio a)
{
    redde a.denominator;
}


/* ==================================================
 * Inspectio
 * ================================================== */

s32
fractio_signum (
    Fractio a)
{
    redde magnus_signum(a.numerator);
}

s32
fractio_compara (
     Fractio  a,
     Fractio  b,
     Piscina* piscina)
{
    si (_est_unum(a.denominator) && _est_unum(b.denominator))
    {
        redde magnus_compara(a.numerator, b.numerator);
    }
    redde magnus_compara(
        magnus_multiplica(a.numerator, b.denominator, piscina),
        magnus_multiplica(b.numerator, a.denominator, piscina));
}

/* forma canonica unica: aequalitas partium */
b32
fractio_aequalis (
    Fractio a,
    Fractio b)
{
    redde magnus_aequalis(a.numerator, b.numerator)
        && magnus_aequalis(a.denominator, b.denominator);
}

b32
fractio_est_integra (
    Fractio a)
{
    redde _est_unum(a.denominator);
}


/* ==================================================
 * Arithmetica
 * ================================================== */

Fractio
fractio_nega (
     Fractio  a,
     Piscina* piscina)
{
    redde _ex_partibus(magnus_nega(a.numerator, piscina),
        a.denominator);
}

Fractio
fractio_absolutum (
     Fractio  a,
     Piscina* piscina)
{
    redde _ex_partibus(magnus_absolutum(a.numerator, piscina),
        a.denominator);
}

/* u/u' + v/v' (Knuth II, 4.5.1): d1 = mdc(u', v'); si d1 = 1, summa
 * (u v' + u' v) / (u' v') iam reducta. Aliter
 * t = u (v'/d1) + v (u'/d1), d2 = mdc(t, d1), et summa
 * (t/d2) / ((u'/d1)(v'/d2)) reducta. */
Fractio
fractio_adde (
     Fractio  a,
     Fractio  b,
     Piscina* piscina)
{
    Magnus d1;
    Magnus d2;
    Magnus t;
    Magnus ua;
    Magnus vb;

    d1 = magnus_divisor_communis(a.denominator, b.denominator, piscina);
    si (_est_unum(d1))
    {
        t = magnus_adde(
            magnus_multiplica(a.numerator, b.denominator, piscina),
            magnus_multiplica(b.numerator, a.denominator, piscina),
            piscina);
        redde _ex_partibus(t,
            magnus_multiplica(a.denominator, b.denominator, piscina));
    }
    ua = _quotiens(a.denominator, d1, piscina);
    vb = _quotiens(b.denominator, d1, piscina);
    t   = magnus_adde(magnus_multiplica(a.numerator, vb, piscina),
        magnus_multiplica(b.numerator, ua, piscina), piscina);
    d2  = magnus_divisor_communis(t, d1, piscina);
    redde _ex_partibus(_quotiens(t, d2, piscina),
        magnus_multiplica(ua,
            _quotiens(b.denominator, d2, piscina), piscina));
}

Fractio
fractio_subtrahe (
     Fractio  a,
     Fractio  b,
     Piscina* piscina)
{
    redde fractio_adde(a, fractio_nega(b, piscina), piscina);
}

/* (u/u')(v/v'): d1 = mdc(u, v'), d2 = mdc(u', v); productum
 * ((u/d1)(v/d2)) / ((u'/d2)(v'/d1)) reductum. mdc(x, 0) = |x| facit
 * ut nihil 0/1 fiat. */
Fractio
fractio_multiplica (
     Fractio  a,
     Fractio  b,
     Piscina* piscina)
{
    Magnus d1 = magnus_divisor_communis(a.numerator, b.denominator,
        piscina);
    Magnus d2 = magnus_divisor_communis(a.denominator, b.numerator,
        piscina);

    redde _ex_partibus(
        magnus_multiplica(_quotiens(a.numerator, d1, piscina),
            _quotiens(b.numerator, d2, piscina), piscina),
        magnus_multiplica(_quotiens(a.denominator, d2, piscina),
            _quotiens(b.denominator, d1, piscina), piscina));
}

b32
fractio_inversa (
     Fractio  a,
     Piscina* piscina,
     Fractio* exitus)
{
    s32 signum = magnus_signum(a.numerator);

    si (signum == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (signum > ZEPHYRUM)
    {
        *exitus = _ex_partibus(a.denominator, a.numerator);
    }
    alioquin
    {
        *exitus = _ex_partibus(magnus_nega(a.denominator, piscina),
            magnus_nega(a.numerator, piscina));
    }
    redde VERUM;
}

b32
fractio_divide (
     Fractio  a,
     Fractio  divisor,
     Piscina* piscina,
     Fractio* exitus)
{
    Fractio inversa;

    si (!fractio_inversa(divisor, piscina, &inversa))
    {
        redde FALSUM;
    }
    *exitus = fractio_multiplica(a, inversa, piscina);
    redde VERUM;
}

b32
fractio_potentia (
     Fractio  basis,
         s32  exponens,
     Piscina* piscina,
     Fractio* exitus)
{
    i32 modulus;

    si (exponens < ZEPHYRUM)
    {
        si (!fractio_inversa(basis, piscina, &basis))
        {
            redde FALSUM;
        }
        /* -(exponens + 1) + 1: sine exundatione etiam pro S32 imo */
        modulus = (i32)(-(exponens + I)) + I;
    }
    alioquin
    {
        modulus = (i32)exponens;
    }
    /* mdc(n, d) = 1 => mdc(n^k, d^k) = 1: nulla reductio */
    *exitus = _ex_partibus(magnus_potentia(basis.numerator, modulus,
        piscina), magnus_potentia(basis.denominator, modulus, piscina));
    redde VERUM;
}


/* ==================================================
 * Ad integros
 * ================================================== */

/* divisio Euclidea per denominatorem positivum: quotiens est
 * pavimentum */
Magnus
fractio_pavimentum (
     Fractio  a,
     Piscina* piscina)
{
    redde _quotiens(a.numerator, a.denominator, piscina);
}

Magnus
fractio_tectum (
     Fractio  a,
     Piscina* piscina)
{
    Magnus quotiens = magnus_ex_s64(ZEPHYRUM);
    Magnus residuum = magnus_ex_s64(ZEPHYRUM);

    (vacuum)magnus_divide(a.numerator, a.denominator, piscina,
        &quotiens, &residuum);
    si (magnus_signum(residuum) == ZEPHYRUM)
    {
        redde quotiens;
    }
    redde magnus_adde(quotiens, magnus_ex_s64(I), piscina);
}

/* q = pavimentum, r = residuum (0 <= r < d): 2r < d -> q; 2r > d ->
 * q + 1; 2r = d (dimidium) -> par ex q et q + 1 */
Magnus
fractio_rotunda (
     Fractio  a,
     Piscina* piscina)
{
    Magnus quotiens  = magnus_ex_s64(ZEPHYRUM);
    Magnus residuum  = magnus_ex_s64(ZEPHYRUM);
    Magnus paritas   = magnus_ex_s64(ZEPHYRUM);
       s32 ordo;

    (vacuum)magnus_divide(a.numerator, a.denominator, piscina,
        &quotiens, &residuum);
    ordo = magnus_compara(magnus_adde(residuum, residuum, piscina),
        a.denominator);
    si (ordo < ZEPHYRUM)
    {
        redde quotiens;
    }
    si (ordo == ZEPHYRUM)
    {
        (vacuum)magnus_divide(quotiens, magnus_ex_s64(II), piscina,
            NIHIL, &paritas);
        si (magnus_signum(paritas) == ZEPHYRUM)
        {
            redde quotiens;
        }
    }
    redde magnus_adde(quotiens, magnus_ex_s64(I), piscina);
}
#undef _est_unum
#undef _ex_partibus
#undef _quotiens
/* lib/polynomium.c: statica per plagulam renominata */
#define Officinae Officinae_polynomium
#define _alveus _alveus_polynomium
#define _apex_notare _apex_notare_polynomium
#define _apex_officinarum _apex_officinarum_polynomium
#define _est_digitus _est_digitus_polynomium
#define _ex_alveo _ex_alveo_polynomium
#define _intra _intra_polynomium
#define _modulus_maximus _modulus_maximus_polynomium
#define _officina_reficere _officina_reficere_polynomium
#define _officinae_aperire _officinae_aperire_polynomium
#define _officinae_claudere _officinae_claudere_polynomium
#define _officinis_utendum _officinis_utendum_polynomium
#define _primitiva_positiva _primitiva_positiva_polynomium
#define _pseudo_residuum _pseudo_residuum_polynomium
#define _servare _servare_polynomium
#define _summa _summa_polynomium
#define _summus _summus_polynomium
#define _transili _transili_polynomium
#line 1 "lib/polynomium.c"
/* polynomium.c - Polynomia Laurentiana exacta super magnum
 *
 * Densa: coefficientes[i] pro t^(imus + i). Fines exponentium
 * (|e| <= 2^30 - 1) efficiunt ut omnis amplitudo (summus - imus + 1)
 * in i32 capiat et omne productum exponentis in s64 - ergo probationes
 * finium ipsae sine exundatione computantur. Divisio exacta: longa ex
 * summo, quotiens partialis per coefficientem ducem divisibilis esse
 * debet, residuum nullum. Vide lib/polynomium.worklog.md.
 */



/* Officinae: piscinae temporariae pro summis partialibus (recensio
 * polynomium-I, A3: multiplicatio CC x CC terminorum C digitorum VI.VI
 * MB in piscina vocantis relinquebat pro XLII KB effectus). Solum si
 * na * nb >= LXIV ET summa partialis s64 relinquere potest (vide
 * _officinis_utendum): aliter via celeris magni nihil allocat et
 * officinae tempus solum constant (recensio polynomium-II: 8 x 8
 * coefficientium parvorum 0.42 -> 0.80 us - regimen ipsum polynomiorum
 * nodorum). Divisio: residua per quotientem crescere possunt; criterium
 * idem in dividendo et divisore, casus rarus residuorum magnorum ex
 * operandis parvis in piscina vocantis manet (ut olim). */
#define POLYNOMIUM_LIMES_OFFICINARUM LXIV
#define POLYNOMIUM_S64_SUMMUS ((s64)0x7FFFFFFFFFFFFFFFLL)


/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
_intra (
    s64 exponens)
{
    redde exponens >= -(s64)POLYNOMIUM_EXPONENS_MAXIMUS
        && exponens <= (s64)POLYNOMIUM_EXPONENS_MAXIMUS;
}

interior s64
_summus (
    Polynomium p)
{
    redde (s64)p.imus + (s64)p.numerus - I;
}

/* numerus coefficientium nullorum */
interior Magnus*
_alveus (
    Piscina* piscina,
        i32  numerus)
{
    Magnus* c = (Magnus*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Magnus));
       i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        c[k] = magnus_ex_s64(ZEPHYRUM);
    }
    redde c;
}

/* forma canonica: zephyra extrema absciduntur (imus crescit) */
interior Polynomium
_ex_alveo (
     constans Magnus* c,
                 i32  numerus,
                 s32  imus)
{
    Polynomium p;
           i32 primus = ZEPHYRUM;

    dum (primus < numerus && magnus_signum(c[primus]) == ZEPHYRUM)
    {
        primus++;
    }
    si (primus == numerus)
    {
        redde polynomium_nullum();
    }
    dum (magnus_signum(c[numerus - I]) == ZEPHYRUM)
    {
        numerus--;
    }
    p.coefficientes  = c + primus;
    p.numerus        = numerus - primus;
    p.imus           = (s32)((s64)imus + (s64)primus);
    redde p;
}

interior b32
_est_digitus (
    i8 c)
{
    redde c >= '0' && c <= '9';
}

interior i32
_transili (
    chorda textus,
       i32 k)
{
    dum (   k < textus.mensura && (textus.datum[k] == ' '
        || textus.datum[k] == '\t'))
    {
        k++;
    }
    redde k;
}


/* Duae piscinae temporariae cum notis initialibus. Si creari non
 * possunt (aut operatio parva est), ambae = piscina vocantis et
 * refectio nihil agit: effectus idem, memoria ut olim. */
nomen structura {
           Piscina* piscinae[II];
    PiscinaNotatio  notae[II];
               b32  propriae;
} Officinae;

/* DIAGNOSIS: maximus usus officinae in operatione ultima */
interior memoriae_index _apex_officinarum = ZEPHYRUM;

interior vacuum
_apex_notare (
    Piscina* officina)
{
    memoriae_index usus = piscina_summa_usus(officina);

    si (usus > _apex_officinarum)
    {
        _apex_officinarum = usus;
    }
}

interior vacuum
_officinae_aperire (
     Officinae* o,
       Piscina* vocantis,
           b32  utendae)
{
    o->piscinae[ZEPHYRUM]  = vocantis;
    o->piscinae[I]         = vocantis;
    o->propriae            = FALSUM;
    _apex_officinarum      = ZEPHYRUM;
    si (!utendae)
    {
        redde;
    }
    o->piscinae[ZEPHYRUM] =
        piscina_generare_dynamicum("polynomium_officina",
        (memoriae_index)4096);
    o->piscinae[I] = piscina_generare_dynamicum("polynomium_officina",
        (memoriae_index)4096);
    si (o->piscinae[ZEPHYRUM] == NIHIL || o->piscinae[I] == NIHIL)
    {
        si (o->piscinae[ZEPHYRUM])
        {
            piscina_destruere(o->piscinae[ZEPHYRUM]);
        }
        si (o->piscinae[I])
        {
            piscina_destruere(o->piscinae[I]);
        }
        o->piscinae[ZEPHYRUM]  = vocantis;
        o->piscinae[I]         = vocantis;
        redde;
    }
    o->notae[ZEPHYRUM]  = piscina_notare(o->piscinae[ZEPHYRUM]);
    o->notae[I]         = piscina_notare(o->piscinae[I]);
    o->propriae         = VERUM;
}

interior vacuum
_officina_reficere (
     Officinae* o,
           i32  index)
{
    si (o->propriae)
    {
        _apex_notare(o->piscinae[index]);
        piscina_reficere(o->piscinae[index], o->notae[index]);
    }
}

interior vacuum
_officinae_claudere (
    Officinae* o)
{
    si (o->propriae)
    {
        _apex_notare(o->piscinae[ZEPHYRUM]);
        _apex_notare(o->piscinae[I]);
        piscina_destruere(o->piscinae[ZEPHYRUM]);
        piscina_destruere(o->piscinae[I]);
    }
}

/* maximus |c| si omnes |c| < 2^31 (producta in via celeri magni);
 * -1 si coefficiens aliquis maior */
interior s64
_modulus_maximus (
    Polynomium p)
{
    s64 maximus = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        s64 valor;

        si (!magnus_ad_s64(p.coefficientes[k], &valor))
        {
            redde -I;
        }
        si (valor < ZEPHYRUM)
        {
            valor = -valor;
        }
        si (valor > (s64)0x7FFFFFFFL)
        {
            redde -I;
        }
        si (valor > maximus)
        {
            maximus = valor;
        }
    }
    redde maximus;
}

/* officinae nisi nulla summa partialis s64 relinquere potest:
 * max|a| max|b| min(na, nb) < 2^63 (producta < 2^62, summae minus quam
 * min(na, nb) productorum). Coefficientes < 2^31 soli non sufficiunt:
 * summa duorum productorum ~2^62 iam exundat et allocat (recensio
 * polynomium-II, planta "scrutatio omissa"). */
interior b32
_officinis_utendum (
    Polynomium a,
    Polynomium b,
           s64 opera)
{
    s64 maximus_a;
    s64 maximus_b;
    s64 minimus_terminorum;

    si (opera < (s64)POLYNOMIUM_LIMES_OFFICINARUM)
    {
        redde FALSUM;
    }
    maximus_a = _modulus_maximus(a);
    maximus_b = _modulus_maximus(b);
    si (maximus_a < ZEPHYRUM || maximus_b < ZEPHYRUM)
    {
        redde VERUM;
    }
    minimus_terminorum = a.numerus < b.numerus ? (s64)a.numerus
        : (s64)b.numerus;
    redde maximus_a * maximus_b > POLYNOMIUM_S64_SUMMUS
        / minimus_terminorum;
}

/* valor ex officina in piscinam vocantis servandus */
interior Magnus
_servare (
    constans Officinae* o,
                Magnus  valor,
               Piscina* piscina)
{
    redde o->propriae ? magnus_transcribe(valor, piscina) : valor;
}


/* ==================================================
 * Constructio et textus
 * ================================================== */

Polynomium
polynomium_nullum (
    vacuum)
{
    Polynomium p;

    p.coefficientes  = NIHIL;
    p.numerus        = ZEPHYRUM;
    p.imus           = ZEPHYRUM;
    redde p;
}

Polynomium
polynomium_constans (
      Magnus  c,
     Piscina* piscina)
{
    Polynomium p = polynomium_nullum();

    (vacuum)polynomium_monomium(c, ZEPHYRUM, piscina, &p);
    redde p;
}

b32
polynomium_monomium (
        Magnus  c,
           s32  exponens,
       Piscina* piscina,
    Polynomium* exitus)
{
    Magnus* alveus;

    si (!_intra((s64)exponens))
    {
        redde FALSUM;
    }
    si (magnus_signum(c) == ZEPHYRUM)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    alveus                 = _alveus(piscina, I);
    alveus[ZEPHYRUM]       = c;
    exitus->coefficientes  = alveus;
    exitus->numerus        = I;
    exitus->imus           = exponens;
    redde VERUM;
}

b32
polynomium_ex_coefficientibus (
     constans Magnus* c,
                 i32  numerus,
                 s32  imus,
             Piscina* piscina,
          Polynomium* exitus)
{
       i32  primus = ZEPHYRUM;
       i32  ultimus;
       i32  k;
    Magnus* alveus;

    dum (primus < numerus && magnus_signum(c[primus]) == ZEPHYRUM)
    {
        primus++;
    }
    si (primus == numerus)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    ultimus = numerus - I;
    dum (magnus_signum(c[ultimus]) == ZEPHYRUM)
    {
        ultimus--;
    }
    si (   !_intra((s64)imus + (s64)primus)
        || !_intra((s64)imus + (s64)ultimus))
    {
        redde FALSUM;
    }
    alveus = _alveus(piscina, ultimus - primus + I);
    per (k = primus; k <= ultimus; k++)
    {
        alveus[k - primus] = c[k];
    }
    exitus->coefficientes  = alveus;
    exitus->numerus        = ultimus - primus + I;
    exitus->imus           = (s32)((s64)imus + (s64)primus);
    redde VERUM;
}

b32
polynomium_ex_chorda (
        chorda  textus,
     character  littera,
       Piscina* piscina,
    Polynomium* exitus)
{
    Magnus* valores;
       s64* exponentes;
       i32  numerus_terminorum = ZEPHYRUM;
       i32  k;
       s64  imus;
       s64  summus;
    Magnus* alveus;

    si (textus.datum == NIHIL || textus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* littera ASCII tantum: byte >= 0x80 solus UTF-8 non est, et
     * character signatus cum i8 non congruit (recensio polynomium-I,
     * A1) */
    si (!(   (littera >= 'a' && littera <= 'z')
          || (littera >= 'A' && littera <= 'Z')))
    {
        redde FALSUM;
    }
    /* quisque terminus saltem characterem unum consumit */
    valores     = _alveus(piscina, textus.mensura);
    exponentes  = (s64*)piscina_allocare(piscina,
        (memoriae_index)textus.mensura * magnitudo(s64));

    k = _transili(textus, ZEPHYRUM);
    dum (VERUM)
    {
           s32 signum               = I;
           b32 habet_coefficientem  = FALSUM;
           b32 habet_litteram       = FALSUM;
        Magnus valor                = magnus_ex_s64(I);
           s64 exponens             = ZEPHYRUM;
           i32 initium;

        si (numerus_terminorum > ZEPHYRUM)
        {
            si (k == textus.mensura)
            {
                frange;
            }
            si (textus.datum[k] == '-')
            {
                signum = -I;
            }
            alioquin si (textus.datum[k] != '+')
            {
                redde FALSUM;
            }
            k = _transili(textus, k + I);
        }
        alioquin si (k < textus.mensura && textus.datum[k] == '-')
        {
            signum  = -I;
            k       = _transili(textus, k + I);
        }

        initium = k;
        dum (k < textus.mensura && _est_digitus(textus.datum[k]))
        {
            k++;
        }
        si (k > initium)
        {
            si (!magnus_ex_chorda(chorda_sectio(textus, initium, k),
                piscina, &valor))
            {
                redde FALSUM;
            }
            habet_coefficientem  = VERUM;
            k                    = _transili(textus, k);
        }

        si (k < textus.mensura && textus.datum[k] == (i8)littera)
        {
            habet_litteram  = VERUM;
            exponens        = I;
            k++;
            si (k < textus.mensura && textus.datum[k] == '^')
            {
                b32 negativus = FALSUM;

                k++;
                si (k < textus.mensura && textus.datum[k] == '-')
                {
                    negativus = VERUM;
                    k++;
                }
                initium   = k;
                exponens  = ZEPHYRUM;
                dum (   k < textus.mensura
                     && _est_digitus(textus.datum[k]))
                {
                    exponens = exponens * X
                        + (s64)(textus.datum[k] - '0');
                    si (exponens > (s64)POLYNOMIUM_EXPONENS_MAXIMUS)
                    {
                        redde FALSUM;
                    }
                    k++;
                }
                si (k == initium)
                {
                    redde FALSUM;
                }
                si (negativus)
                {
                    exponens = -exponens;
                }
            }
            k = _transili(textus, k);
        }

        si (!habet_coefficientem && !habet_litteram)
        {
            redde FALSUM;
        }
        si (signum < ZEPHYRUM)
        {
            valor = magnus_nega(valor, piscina);
        }
        valores[numerus_terminorum]     = valor;
        exponentes[numerus_terminorum]  = exponens;
        numerus_terminorum++;
    }

    /* termini in alveum densum; exponentes iterati coniunguntur */
    imus    = exponentes[ZEPHYRUM];
    summus  = exponentes[ZEPHYRUM];
    per (k = I; k < numerus_terminorum; k++)
    {
        si (exponentes[k] < imus)
        {
            imus = exponentes[k];
        }
        si (exponentes[k] > summus)
        {
            summus = exponentes[k];
        }
    }
    si (summus - imus + I > (s64)POLYNOMIUM_AMPLITUDO_LECTIONIS)
    {
        redde FALSUM;
    }
    alveus = _alveus(piscina, (i32)(summus - imus + I));
    per (k = ZEPHYRUM; k < numerus_terminorum; k++)
    {
        i32 locus = (i32)(exponentes[k] - imus);

        alveus[locus] = magnus_adde(alveus[locus], valores[k], piscina);
    }
    *exitus = _ex_alveo(alveus, (i32)(summus - imus + I), (s32)imus);
    redde VERUM;
}

Polynomium
polynomium_transcribe (
    Polynomium  p,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (p.numerus == ZEPHYRUM)
    {
        redde p;
    }
    alveus = _alveus(piscina, p.numerus);
    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        alveus[k] = magnus_transcribe(p.coefficientes[k], piscina);
    }
    p.coefficientes = alveus;
    redde p;
}

chorda
polynomium_ad_chordam (
    Polynomium  p,
     character  littera,
       Piscina* piscina)
{
     ChordaAedificator* scriba;
                   i32  k;
                   b32  primus  = VERUM;
                Magnus  unum    = magnus_ex_s64(I);

    si (p.numerus == ZEPHYRUM)
    {
        redde chorda_ex_literis("0", piscina);
    }
    scriba = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);
    per (k = p.numerus; k-- > ZEPHYRUM;)
    {
        Magnus c         = p.coefficientes[k];
           s32 exponens  = (s32)((s64)p.imus + (s64)k);
        Magnus modulus;

        si (magnus_signum(c) == ZEPHYRUM)
        {
            perge;
        }
        si (primus)
        {
            si (magnus_signum(c) < ZEPHYRUM)
            {
                (vacuum)chorda_aedificator_appendere_character(scriba,
                    '-');
            }
            primus = FALSUM;
        }
        alioquin
        {
            (vacuum)chorda_aedificator_appendere_literis(scriba,
                magnus_signum(c) < ZEPHYRUM ? " - " : " + ");
        }
        modulus = magnus_absolutum(c, piscina);
        si (exponens == ZEPHYRUM || !magnus_aequalis(modulus, unum))
        {
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                magnus_ad_chordam(modulus, piscina));
        }
        si (exponens != ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_character(scriba,
                littera);
            si (exponens != I)
            {
                (vacuum)chorda_aedificator_appendere_character(scriba,
                    '^');
                (vacuum)chorda_aedificator_appendere_s32(scriba,
                    exponens);
            }
        }
    }
    redde chorda_aedificator_finire(scriba);
}


/* ==================================================
 * Lectio
 * ================================================== */

b32
polynomium_est_nullum (
    Polynomium p)
{
    redde p.numerus == ZEPHYRUM;
}

s32
polynomium_gradus_imus (
    Polynomium p)
{
    redde p.imus;
}

s32
polynomium_gradus_summus (
    Polynomium p)
{
    si (p.numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s32)_summus(p);
}

Magnus
polynomium_coefficiens (
    Polynomium p,
           s32 exponens)
{
    s64 locus = (s64)exponens - (s64)p.imus;

    si (locus < ZEPHYRUM || locus >= (s64)p.numerus)
    {
        redde magnus_ex_s64(ZEPHYRUM);
    }
    redde p.coefficientes[(i32)locus];
}

b32
polynomium_aequalis (
    Polynomium a,
    Polynomium b)
{
    i32 k;

    si (a.numerus != b.numerus || a.imus != b.imus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        si (!magnus_aequalis(a.coefficientes[k], b.coefficientes[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

Magnus
polynomium_contentum (
    Polynomium  p,
       Piscina* piscina)
{
    Magnus g = magnus_ex_s64(ZEPHYRUM);
       i32 k;

    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        g = magnus_divisor_communis(g, p.coefficientes[k], piscina);
    }
    redde g;
}


/* ==================================================
 * Arithmetica
 * ================================================== */

Polynomium
polynomium_nega (
    Polynomium  a,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (a.numerus == ZEPHYRUM)
    {
        redde a;
    }
    alveus = _alveus(piscina, a.numerus);
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        alveus[k] = magnus_nega(a.coefficientes[k], piscina);
    }
    redde _ex_alveo(alveus, a.numerus, a.imus);
}

/* a + signum * b */
interior Polynomium
_summa (
    Polynomium  a,
    Polynomium  b,
           s32  signum,
       Piscina* piscina)
{
     Magnus* alveus;
        s64  imus;
        s64  summus;
        i32  numerus;
        i32  k;

    si (b.numerus == ZEPHYRUM)
    {
        redde a;
    }
    si (a.numerus == ZEPHYRUM)
    {
        redde signum > ZEPHYRUM ? b : polynomium_nega(b, piscina);
    }
    imus     = a.imus < b.imus ? (s64)a.imus : (s64)b.imus;
    summus   = _summus(a) > _summus(b) ? _summus(a) : _summus(b);
    numerus  = (i32)(summus - imus + I);
    alveus   = _alveus(piscina, numerus);
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        alveus[(i32)((s64)a.imus - imus) + k] = a.coefficientes[k];
    }
    per (k = ZEPHYRUM; k < b.numerus; k++)
    {
        i32 locus = (i32)((s64)b.imus - imus) + k;

        alveus[locus] = signum > ZEPHYRUM
            ? magnus_adde(alveus[locus], b.coefficientes[k], piscina)
            : magnus_subtrahe(alveus[locus], b.coefficientes[k],
            piscina);
    }
    redde _ex_alveo(alveus, numerus, (s32)imus);
}

Polynomium
polynomium_adde (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina)
{
    redde _summa(a, b, I, piscina);
}

Polynomium
polynomium_subtrahe (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina)
{
    redde _summa(a, b, -I, piscina);
}

Polynomium
polynomium_multiplica_scalari (
    Polynomium  p,
        Magnus  c,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (p.numerus == ZEPHYRUM || magnus_signum(c) == ZEPHYRUM)
    {
        redde polynomium_nullum();
    }
    alveus = _alveus(piscina, p.numerus);
    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        alveus[k] = magnus_multiplica(p.coefficientes[k], c, piscina);
    }
    redde _ex_alveo(alveus, p.numerus, p.imus);
}

b32
polynomium_multiplica (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* exitus)
{
      Magnus* alveus;
   Officinae  officinae;
         s64  imus;
         i32  numerus;
         i32  m;

    si (a.numerus == ZEPHYRUM || b.numerus == ZEPHYRUM)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    imus = (s64)a.imus + (s64)b.imus;
    si (!_intra(imus) || !_intra(_summus(a) + _summus(b)))
    {
        redde FALSUM;
    }
    numerus  = a.numerus + b.numerus - I;
    alveus   = _alveus(piscina, numerus);

    _officinae_aperire(&officinae, piscina, _officinis_utendum(a, b,
        (s64)a.numerus * (s64)b.numerus));
    si (!officinae.propriae)
    {
        /* via vocantis: ansa ordinaria (i, j) sine sumptu officinarum -
         * regimen polynomiorum nodorum */
        i32 i;
        i32 j;

        per (i = ZEPHYRUM; i < a.numerus; i++)
        {
            per (j = ZEPHYRUM; j < b.numerus; j++)
            {
                alveus[i + j] = magnus_adde(alveus[i + j],
                    magnus_multiplica(a.coefficientes[i],
                        b.coefficientes[j], piscina), piscina);
            }
        }
    }
    alioquin
    {
        /* coefficiens quisque totus in officina computatur, solus valor
         * finalis in piscinam vocantis transcribitur */
        per (m = ZEPHYRUM; m < numerus; m++)
        {
             Piscina* officina  = officinae.piscinae[ZEPHYRUM];
              Magnus  summa     = magnus_ex_s64(ZEPHYRUM);
                 i32  i         = m + I > b.numerus ? m + I - b.numerus
                     : ZEPHYRUM;

            per (; i < a.numerus && i <= m; i++)
            {
                summa = magnus_adde(summa, magnus_multiplica(
                    a.coefficientes[i], b.coefficientes[m - i],
                    officina),
                    officina);
            }
            alveus[m] = magnus_transcribe(summa, piscina);
            _officina_reficere(&officinae, ZEPHYRUM);
        }
    }
    _officinae_claudere(&officinae);
    *exitus = _ex_alveo(alveus, numerus, (s32)imus);
    redde VERUM;
}

b32
polynomium_potentia (
    Polynomium  p,
           i32  n,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium effectus;
    Polynomium basis     = p;
           i32 reliquum  = n;

    si (n == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    /* |exponens| <= 2^30, n < 2^32: productum in s64 capit */
    si (   !_intra((s64)p.imus * (s64)n)
        || !_intra(_summus(p) * (s64)n))
    {
        redde FALSUM;
    }
    /* quadrata intermedia p^(2^k), 2^k <= n: intra fines */
    effectus = polynomium_constans(magnus_ex_s64(I), piscina);
    dum (VERUM)
    {
        si (reliquum & I)
        {
            si (!polynomium_multiplica(effectus, basis, piscina,
                &effectus))
            {
                redde FALSUM;
            }
        }
        reliquum >>= I;
        si (reliquum == ZEPHYRUM)
        {
            frange;
        }
        si (!polynomium_multiplica(basis, basis, piscina, &basis))
        {
            redde FALSUM;
        }
    }
    *exitus = effectus;
    redde VERUM;
}

b32
polynomium_divide_exacte (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* quotiens)
{
      Magnus* residua;
      Magnus* partes;
      Magnus  dux;
   Officinae  officinae;
         i32  hic = ZEPHYRUM;
         s64  imus;
         i32  numerus;
         i32  k;
         i32  j;
         b32  exacta = VERUM;

    si (b.numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (a.numerus == ZEPHYRUM)
    {
        *quotiens = a;
        redde VERUM;
    }
    si (a.numerus < b.numerus)
    {
        redde FALSUM;
    }
    imus = (s64)a.imus - (s64)b.imus;
    si (!_intra(imus) || !_intra(_summus(a) - _summus(b)))
    {
        redde FALSUM;
    }

    /* A = a t^-imus(a), B = b t^-imus(b): termini constantes non
     * nulli (t unitas est); divisio longa in Z[t] ex summo.
     * Fenestra residuorum mutatorum [k, k + nb - 1] gradu quoque tota
     * rescribitur, ergo in officinis ALTERNIS vivit (sicut Euclides
     * magni): gradus in officinam alteram computat, priorem reficit.
     * Partes quotientis in piscinam vocantis transcribuntur. */
    numerus  = a.numerus - b.numerus + I;
    residua  = _alveus(piscina, a.numerus);
    partes   = _alveus(piscina, numerus);
    dux      = b.coefficientes[b.numerus - I];
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        residua[k] = a.coefficientes[k];
    }
    _officinae_aperire(&officinae, piscina, _officinis_utendum(a, b,
        (s64)numerus * (s64)b.numerus));
    per (k = numerus; k-- > ZEPHYRUM;)
    {
         Piscina* illic = officinae.piscinae[I - hic];
          Magnus  pars;
          Magnus  residuum;

        (vacuum)magnus_divide(residua[k + b.numerus - I], dux, illic,
            &pars, &residuum);
        si (magnus_signum(residuum) != ZEPHYRUM)
        {
            exacta = FALSUM;
            frange;
        }
        partes[k] = _servare(&officinae, pars, piscina);
        si (magnus_signum(pars) == ZEPHYRUM)
        {
            _officina_reficere(&officinae, I - hic);
            perge;
        }
        per (j = ZEPHYRUM; j < b.numerus; j++)
        {
            /* coefficiens B internus nullus: x - 0 = x ipse, fortasse
             * in officina reficienda - transcribe in illic */
            si (   officinae.propriae
                && magnus_signum(b.coefficientes[j]) == ZEPHYRUM)
            {
                residua[k + j] = _servare(&officinae, residua[k + j],
                    illic);
                perge;
            }
            residua[k + j] = magnus_subtrahe(residua[k + j],
                magnus_multiplica(pars, b.coefficientes[j], illic),
                illic);
        }
        _officina_reficere(&officinae, hic);
        hic = I - hic;
    }
    /* residuum infra gradum B nullum esse debet (lectum ANTE
     * officinas clausas) */
    per (k = ZEPHYRUM; exacta && k + I < b.numerus; k++)
    {
        si (magnus_signum(residua[k]) != ZEPHYRUM)
        {
            exacta = FALSUM;
        }
    }
    _officinae_claudere(&officinae);
    si (!exacta)
    {
        redde FALSUM;
    }
    *quotiens = _ex_alveo(partes, numerus, (s32)imus);
    redde VERUM;
}


/* ==================================================
 * Substitutiones
 * ================================================== */

b32
polynomium_translata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus)
{
    (vacuum)piscina;
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    si (   !_intra((s64)p.imus + (s64)k)
        || !_intra(_summus(p) + (s64)k))
    {
        redde FALSUM;
    }
    *exitus       = p;
    exitus->imus  = (s32)((s64)p.imus + (s64)k);
    redde VERUM;
}

b32
polynomium_dilata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus)
{
     Magnus* alveus;
        s64  imus;
        s64  summus;
        i32  i;

    si (k == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    /* |exponens| < 2^30, |k| <= 2^31: productum in s64 capit */
    imus    = (s64)p.imus * (s64)k;
    summus  = _summus(p) * (s64)k;
    si (k < ZEPHYRUM)
    {
        s64 t = imus;

        imus    = summus;
        summus  = t;
    }
    si (!_intra(imus) || !_intra(summus))
    {
        redde FALSUM;
    }
    alveus = _alveus(piscina, (i32)(summus - imus + I));
    per (i = ZEPHYRUM; i < p.numerus; i++)
    {
        s64 exponens = ((s64)p.imus + (s64)i) * (s64)k;

        alveus[(i32)(exponens - imus)] = p.coefficientes[i];
    }
    *exitus = _ex_alveo(alveus, (i32)(summus - imus + I), (s32)imus);
    redde VERUM;
}

b32
polynomium_contrahe (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus)
{
     Magnus* alveus;
        s64  modulus_k;
        s64  imus;
        s64  summus;
        i32  i;

    si (k == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    /* divisibilitas per modulos non negativos (C89 '%' cum negativis
     * definitionem non habet) */
    modulus_k = k < ZEPHYRUM ? -(s64)k : (s64)k;
    per (i = ZEPHYRUM; i < p.numerus; i++)
    {
        s64 exponens  = (s64)p.imus + (s64)i;
        s64 modulus   = exponens < ZEPHYRUM ? -exponens : exponens;

        si (   magnus_signum(p.coefficientes[i]) != ZEPHYRUM
            && modulus % modulus_k               != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    /* quotientes exacti: e / k = signum * (|e| / |k|); |e/k| <= |e|,
     * ergo intra fines */
    imus    = (s64)p.imus;
    summus  = _summus(p);
    imus    = (imus < ZEPHYRUM ? -((-imus) / modulus_k) : imus
        / modulus_k);
    summus  = (summus < ZEPHYRUM ? -((-summus) / modulus_k) : summus
        / modulus_k);
    si (k < ZEPHYRUM)
    {
        s64 t = -imus;

        imus    = -summus;
        summus  = t;
    }
    alveus = _alveus(piscina, (i32)(summus - imus + I));
    per (i = ZEPHYRUM; i < p.numerus; i++)
    {
        s64 exponens  = (s64)p.imus + (s64)i;
        s64 modulus   = exponens < ZEPHYRUM ? -exponens : exponens;
        s64 novus;

        si (magnus_signum(p.coefficientes[i]) == ZEPHYRUM)
        {
            perge;
        }
        novus = modulus / modulus_k;
        si ((exponens < ZEPHYRUM) != (k < ZEPHYRUM))
        {
            novus = -novus;
        }
        alveus[(i32)(novus - imus)] = p.coefficientes[i];
    }
    *exitus = _ex_alveo(alveus, (i32)(summus - imus + I), (s32)imus);
    redde VERUM;
}

b32
polynomium_normale (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium q;

    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    si (!polynomium_translata(p, -p.imus, piscina, &q))
    {
        redde FALSUM;
    }
    si (magnus_signum(q.coefficientes[ZEPHYRUM]) < ZEPHYRUM)
    {
        q = polynomium_nega(q, piscina);
    }
    *exitus = q;
    redde VERUM;
}

Polynomium
polynomium_inversum (
    Polynomium  p,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (p.numerus == ZEPHYRUM)
    {
        redde p;
    }
    alveus = _alveus(piscina, p.numerus);
    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        alveus[k] = p.coefficientes[p.numerus - I - k];
    }
    /* t^e -> t^-e: summus fit -imus; |e| <= MAXIMUS utrimque */
    redde _ex_alveo(alveus, p.numerus, (s32)(-_summus(p)));
}

b32
polynomium_est_symmetricum (
    Polynomium p)
{
    i32 k;

    per (k = ZEPHYRUM; k < p.numerus / II; k++)
    {
        si (!magnus_aequalis(p.coefficientes[k],
            p.coefficientes[p.numerus - I - k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Valor
 * ================================================== */

b32
polynomium_valor (
    Polynomium  p,
       Fractio  x,
       Piscina* piscina,
       Fractio* exitus)
{
    Fractio valor = fractio_ex_s64(ZEPHYRUM);
    Fractio factor_imus;
        i32 k;

    si (p.numerus == ZEPHYRUM)
    {
        *exitus = valor;
        redde VERUM;
    }
    si (!fractio_potentia(x, p.imus, piscina, &factor_imus))
    {
        redde FALSUM;   /* x = 0, exponens negativus */
    }
    per (k = p.numerus; k-- > ZEPHYRUM;)
    {
        valor = fractio_adde(fractio_multiplica(valor, x, piscina),
            fractio_ex_magno(p.coefficientes[k]), piscina);
    }
    *exitus = fractio_multiplica(valor, factor_imus, piscina);
    redde VERUM;
}


/* ==================================================
 * Diagnosis
 * ================================================== */

memoriae_index
polynomium_apex_officinarum (
    vacuum)
{
    redde _apex_officinarum;
}


/* ==================================================
 * Derivata, divisor communis, pars libera, translatio Taylor
 * ================================================== */

b32
polynomium_derivata (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus)
{
    Magnus* c;
       s32  imus;
       s32  summus;
       s32  e;

    si (polynomium_est_nullum(p))
    {
        *exitus = p;
        redde VERUM;
    }
    imus    = polynomium_gradus_imus(p);
    summus  = polynomium_gradus_summus(p);
    si (imus - I < -POLYNOMIUM_EXPONENS_MAXIMUS)
    {
        redde FALSUM;
    }
    c = (Magnus*)piscina_allocare(piscina, (memoriae_index)(summus
        - imus
        + I) * magnitudo(Magnus));
    per (e = imus; e <= summus; e++)
    {
        c[e - imus] = magnus_multiplica(polynomium_coefficiens(p, e),
            magnus_ex_s64((s64)e), piscina);
    }
    redde polynomium_ex_coefficientibus(c, (i32)(summus - imus + I),
        imus - I, piscina, exitus);
}

/* p / contentum, coefficiens summus > 0 */
interior Polynomium
_primitiva_positiva (
    Polynomium  p,
       Piscina* piscina)
{
    Magnus g;

    si (polynomium_est_nullum(p))
    {
        redde p;
    }
    g = polynomium_contentum(p, piscina);
    si (magnus_signum(polynomium_coefficiens(p,
            polynomium_gradus_summus(p))) < ZEPHYRUM)
    {
        g = magnus_nega(g, piscina);
    }
    si (magnus_compara(g, magnus_ex_s64(I)) != ZEPHYRUM)
    {
        (vacuum)polynomium_divide_exacte(p, polynomium_constans(g,
            piscina), piscina, &p);
    }
    redde p;
}

/* pseudo-residuum scala POSITIVA |lc(b)|^k a mod b (extensio.c _residuum;
 * limes structuralis passuum) */
interior Polynomium
_pseudo_residuum (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina)
{
       s32 gradus_b   = polynomium_gradus_summus(b);
    Magnus lc_b       = polynomium_coefficiens(b, gradus_b);
    Magnus absolutum  = magnus_absolutum(lc_b, piscina);
       s32 signum_b   = magnus_signum(lc_b);
       s32 gradus_initium;
       s32 iteratio;

    si (polynomium_est_nullum(a))
    {
        redde a;
    }
    gradus_initium = polynomium_gradus_summus(a);
    per (iteratio = ZEPHYRUM; iteratio <= gradus_initium - gradus_b
        && !polynomium_est_nullum(a)
        && polynomium_gradus_summus(a) >= gradus_b; iteratio++)
    {
               s32 gradus_a   = polynomium_gradus_summus(a);
            Magnus lc_a       = polynomium_coefficiens(a, gradus_a);
        Polynomium translata  = polynomium_nullum();

        (vacuum)polynomium_translata(b, gradus_a - gradus_b, piscina,
            &translata);
        si (signum_b < ZEPHYRUM)
        {
            lc_a = magnus_nega(lc_a, piscina);
        }
        a = polynomium_subtrahe(polynomium_multiplica_scalari(a,
            absolutum, piscina),
            polynomium_multiplica_scalari(translata,
            lc_a, piscina), piscina);
    }
    redde a;
}

b32
polynomium_divisor_communis (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* exitus)
{
    s32 iteratio;
    s32 limes;

    si (   (!polynomium_est_nullum(a)
        && polynomium_gradus_imus(a) < ZEPHYRUM)
        || (!polynomium_est_nullum(b)
        && polynomium_gradus_imus(b) < ZEPHYRUM))
    {
        redde FALSUM;
    }
    a = _primitiva_positiva(a, piscina);
    b = _primitiva_positiva(b, piscina);
    si (   !polynomium_est_nullum(a) && !polynomium_est_nullum(b)
        && polynomium_gradus_summus(a) < polynomium_gradus_summus(b))
    {
        Polynomium t = a;

        a = b;
        b = t;
    }
    /* gradus b strictim decrescit: limes structuralis */
    limes = polynomium_est_nullum(b) ? ZEPHYRUM
        : polynomium_gradus_summus(b) + II;
    per (iteratio = ZEPHYRUM; iteratio < limes
        && !polynomium_est_nullum(b); iteratio++)
    {
        Polynomium r = _primitiva_positiva(_pseudo_residuum(a, b,
            piscina),
            piscina);

        a = b;
        b = r;
    }
    si (!polynomium_est_nullum(b))
    {
        redde FALSUM;               /* non accidit: limes structuralis */
    }
    *exitus = _primitiva_positiva(a, piscina);
    redde VERUM;
}

b32
polynomium_pars_libera (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium derivata  = polynomium_nullum();
    Polynomium g         = polynomium_nullum();
    Polynomium q         = polynomium_nullum();

    si (polynomium_est_nullum(p))
    {
        *exitus = p;
        redde VERUM;
    }
    si (polynomium_gradus_imus(p) < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (polynomium_gradus_summus(p) == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    si (   !polynomium_derivata(p, piscina, &derivata)
        || !polynomium_divisor_communis(p, derivata, piscina, &g)
        || !polynomium_divide_exacte(_primitiva_positiva(p, piscina), g,
        piscina, &q))
    {
        redde FALSUM;
    }
    *exitus = _primitiva_positiva(q, piscina);
    redde VERUM;
}

b32
polynomium_translatum (
    Polynomium  p,
        Magnus  c,
       Piscina* piscina,
    Polynomium* exitus)
{
       s32  n;
       s32  i;
       s32  j;
       b32  unus;
    Magnus* a;

    si (polynomium_est_nullum(p))
    {
        *exitus = p;
        redde VERUM;
    }
    si (polynomium_gradus_imus(p) < ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* translatio Taylor in loco (Horner iteratus): pro i = 0..n-1, pro j
     * = n-1 .. i: a_j += c a_(j+1). Additiones solae si c = 1 (VCA);
     * olim Horner per polynomium_multiplica, polynomium novum in omni
     * gradu (worklog 2026-10-10) */
    n = polynomium_gradus_summus(p);
    a = (Magnus*)piscina_allocare(piscina, (memoriae_index)(n + I)
        * magnitudo(Magnus));
    per (i = ZEPHYRUM; i <= n; i++)
    {
        a[i] = polynomium_coefficiens(p, i);
    }
    unus = magnus_compara(c, magnus_ex_s64(I)) == ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (j = n - I; j >= i; j--)
        {
            a[j] = magnus_adde(a[j], unus ? a[j + I]
                : magnus_multiplica(c, a[j + I], piscina), piscina);
        }
    }
    redde polynomium_ex_coefficientibus(a, (i32)(n + I), ZEPHYRUM,
        piscina,
        exitus);
}
#undef Officinae
#undef POLYNOMIUM_LIMES_OFFICINARUM
#undef POLYNOMIUM_S64_SUMMUS
#undef _alveus
#undef _apex_notare
#undef _apex_officinarum
#undef _est_digitus
#undef _ex_alveo
#undef _intra
#undef _modulus_maximus
#undef _officina_reficere
#undef _officinae_aperire
#undef _officinae_claudere
#undef _officinis_utendum
#undef _primitiva_positiva
#undef _pseudo_residuum
#undef _servare
#undef _summa
#undef _summus
#undef _transili
/* lib/anulus.c: statica per plagulam renominata */
#define FORMA_RESIDUORUM FORMA_RESIDUORUM_anulus
#define _modulus _modulus_anulus
#define _p_ad_chordam _p_ad_chordam_anulus
#define _p_adde _p_adde_anulus
#define _p_aequalis _p_aequalis_anulus
#define _p_divide_exacte _p_divide_exacte_anulus
#define _p_est_nullum _p_est_nullum_anulus
#define _p_ex_chorda _p_ex_chorda_anulus
#define _p_multiplica _p_multiplica_anulus
#define _p_nullum _p_nullum_anulus
#define _p_parvum _p_parvum_anulus
#define _p_subtrahe _p_subtrahe_anulus
#define _p_transcribe _p_transcribe_anulus
#define _p_unum _p_unum_anulus
#define _q_ad_chordam _q_ad_chordam_anulus
#define _q_adde _q_adde_anulus
#define _q_aequalis _q_aequalis_anulus
#define _q_divide_exacte _q_divide_exacte_anulus
#define _q_est_nullum _q_est_nullum_anulus
#define _q_ex_chorda _q_ex_chorda_anulus
#define _q_multiplica _q_multiplica_anulus
#define _q_nullum _q_nullum_anulus
#define _q_parvum _q_parvum_anulus
#define _q_signum _q_signum_anulus
#define _q_subtrahe _q_subtrahe_anulus
#define _q_transcribe _q_transcribe_anulus
#define _q_unum _q_unum_anulus
#define _r_ad_chordam _r_ad_chordam_anulus
#define _r_adde _r_adde_anulus
#define _r_aequalis _r_aequalis_anulus
#define _r_divide_exacte _r_divide_exacte_anulus
#define _r_est_nullum _r_est_nullum_anulus
#define _r_ex_chorda _r_ex_chorda_anulus
#define _r_multiplica _r_multiplica_anulus
#define _r_nullum _r_nullum_anulus
#define _r_parvum _r_parvum_anulus
#define _r_subtrahe _r_subtrahe_anulus
#define _r_transcribe _r_transcribe_anulus
#define _r_unum _r_unum_anulus
#define _z_ad_chordam _z_ad_chordam_anulus
#define _z_adde _z_adde_anulus
#define _z_aequalis _z_aequalis_anulus
#define _z_compara_normam _z_compara_normam_anulus
#define _z_divide_cum_residuo _z_divide_cum_residuo_anulus
#define _z_divide_exacte _z_divide_exacte_anulus
#define _z_divisor_communis _z_divisor_communis_anulus
#define _z_est_nullum _z_est_nullum_anulus
#define _z_ex_chorda _z_ex_chorda_anulus
#define _z_multiplica _z_multiplica_anulus
#define _z_nullum _z_nullum_anulus
#define _z_parvum _z_parvum_anulus
#define _z_signum _z_signum_anulus
#define _z_subtrahe _z_subtrahe_anulus
#define _z_transcribe _z_transcribe_anulus
#define _z_unum _z_unum_anulus
#line 1 "lib/anulus.c"
/* anulus.c - Anuli domus per tabulas functionum: Z (magnus), Q
 * (fractio), Z[t, t^-1] (polynomium), Z/n (congruentia, modulus in
 * contextu). Involucra tenuia: elementa opaca
 * ad typum suum convertuntur et functio bibliothecae vocatur. Vide
 * lib/anulus.worklog.md.
 */







/* ==================================================
 * Z: magnus
 * ================================================== */

#define VALOR_Z(x) (*(constans Magnus*)(x))

interior vacuum
_z_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_ex_s64(ZEPHYRUM);
}

interior vacuum
_z_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *(Magnus*)exitus = magnus_ex_s64(I);
}

interior b32
_z_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde magnus_signum(VALOR_Z(a)) == ZEPHYRUM;
}

interior b32
_z_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    s64 valor;

    (vacuum)anulus;
    redde magnus_ad_s64(VALOR_Z(a), &valor);
}

interior b32
_z_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    (vacuum)anulus;
    redde magnus_aequalis(VALOR_Z(a), VALOR_Z(b));
}

interior b32
_z_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_adde(VALOR_Z(a), VALOR_Z(b), piscina);
    redde VERUM;
}

interior b32
_z_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_subtrahe(VALOR_Z(a), VALOR_Z(b), piscina);
    redde VERUM;
}

interior b32
_z_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_multiplica(VALOR_Z(a), VALOR_Z(b),
        piscina);
    redde VERUM;
}

interior b32
_z_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    Magnus quotiens;
    Magnus residuum;

    (vacuum)anulus;
    si (   !magnus_divide(VALOR_Z(a), VALOR_Z(b), piscina, &quotiens,
        &residuum)
        || magnus_signum(residuum) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *(Magnus*)exitus = quotiens;
    redde VERUM;
}

interior vacuum
_z_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_transcribe(VALOR_Z(a), piscina);
}

interior chorda
_z_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde magnus_ad_chordam(VALOR_Z(a), piscina);
}

interior b32
_z_ex_chorda (
    constans Anulus* anulus,
             chorda  textus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde magnus_ex_chorda(textus, piscina, (Magnus*)exitus);
}

interior b32
_z_divisor_communis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* g,
             vacuum* u,
             vacuum* v)
{
    (vacuum)anulus;
    *(Magnus*)g = magnus_divisor_communis_testatus(VALOR_Z(a),
        VALOR_Z(b),
        piscina, (Magnus*)u, (Magnus*)v);
    redde VERUM;
}

interior b32
_z_divide_cum_residuo (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* q,
             vacuum* r)
{
    (vacuum)anulus;
    redde magnus_divide(VALOR_Z(a), VALOR_Z(b), piscina, (Magnus*)q,
        (Magnus*)r);
}

interior s32
_z_compara_normam (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde magnus_compara(magnus_absolutum(VALOR_Z(a), piscina),
        magnus_absolutum(VALOR_Z(b), piscina));
}

interior b32
_z_signum (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
                s32* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *exitus = magnus_signum(VALOR_Z(a));
    redde VERUM;
}

constans Anulus ANULUS_INTEGRORUM = {
    "Z", (memoriae_index)magnitudo(Magnus), FALSUM,
    _z_nullum, _z_unum, _z_est_nullum, _z_parvum, _z_aequalis, _z_adde,
        _z_subtrahe,
    _z_multiplica, _z_divide_exacte, _z_transcribe, _z_ad_chordam,
    _z_ex_chorda, _z_divisor_communis, _z_divide_cum_residuo,
    _z_compara_normam, NIHIL, VERUM, _z_signum
};


/* ==================================================
 * Q: fractio
 * ================================================== */

#define VALOR_Q(x) (*(constans Fractio*)(x))

interior vacuum
_q_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_ex_s64(ZEPHYRUM);
}

interior vacuum
_q_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *(Fractio*)exitus = fractio_ex_s64(I);
}

interior b32
_q_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde fractio_signum(VALOR_Q(a)) == ZEPHYRUM;
}

interior b32
_q_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    s64 valor;

    (vacuum)anulus;
    redde magnus_ad_s64(fractio_numerator(VALOR_Q(a)), &valor)
        && magnus_ad_s64(fractio_denominator(VALOR_Q(a)), &valor);
}

interior b32
_q_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    (vacuum)anulus;
    redde fractio_aequalis(VALOR_Q(a), VALOR_Q(b));
}

interior b32
_q_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_adde(VALOR_Q(a), VALOR_Q(b), piscina);
    redde VERUM;
}

interior b32
_q_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_subtrahe(VALOR_Q(a), VALOR_Q(b),
        piscina);
    redde VERUM;
}

interior b32
_q_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_multiplica(VALOR_Q(a), VALOR_Q(b),
        piscina);
    redde VERUM;
}

interior b32
_q_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde fractio_divide(VALOR_Q(a), VALOR_Q(b), piscina,
        (Fractio*)exitus);
}

interior vacuum
_q_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_transcribe(VALOR_Q(a), piscina);
}

interior chorda
_q_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde fractio_ad_chordam(VALOR_Q(a), piscina);
}

interior b32
_q_ex_chorda (
    constans Anulus* anulus,
             chorda  textus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde fractio_ex_chorda(textus, piscina, (Fractio*)exitus);
}

interior b32
_q_signum (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
                s32* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *exitus = fractio_signum(VALOR_Q(a));
    redde VERUM;
}

constans Anulus ANULUS_RATIONALIUM = {
    "Q", (memoriae_index)magnitudo(Fractio), VERUM,
    _q_nullum, _q_unum, _q_est_nullum, _q_parvum, _q_aequalis, _q_adde,
        _q_subtrahe,
    _q_multiplica, _q_divide_exacte, _q_transcribe, _q_ad_chordam,
    _q_ex_chorda, NIHIL, NIHIL, NIHIL, NIHIL, VERUM, _q_signum
};


/* ==================================================
 * Z[t, t^-1]: polynomium
 * ================================================== */

#define VALOR_P(x) (*(constans Polynomium*)(x))

interior vacuum
_p_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_nullum();
}

interior vacuum
_p_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_constans(magnus_ex_s64(I),
        piscina);
}

interior b32
_p_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde polynomium_est_nullum(VALOR_P(a));
}

interior b32
_p_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde polynomium_est_nullum(VALOR_P(a));
}

interior b32
_p_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    (vacuum)anulus;
    redde polynomium_aequalis(VALOR_P(a), VALOR_P(b));
}

interior b32
_p_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_adde(VALOR_P(a), VALOR_P(b),
        piscina);
    redde VERUM;
}

interior b32
_p_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_subtrahe(VALOR_P(a), VALOR_P(b),
        piscina);
    redde VERUM;
}

interior b32
_p_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde polynomium_multiplica(VALOR_P(a), VALOR_P(b), piscina,
        (Polynomium*)exitus);
}

interior b32
_p_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde polynomium_divide_exacte(VALOR_P(a), VALOR_P(b), piscina,
        (Polynomium*)exitus);
}

interior vacuum
_p_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_transcribe(VALOR_P(a), piscina);
}

interior chorda
_p_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde polynomium_ad_chordam(VALOR_P(a), 't', piscina);
}

interior b32
_p_ex_chorda (
    constans Anulus* anulus,
             chorda  textus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde polynomium_ex_chorda(textus, 't', piscina,
        (Polynomium*)exitus);
}

constans Anulus ANULUS_POLYNOMIORUM = {
    "Z[t,t^-1]", (memoriae_index)magnitudo(Polynomium), FALSUM,
    _p_nullum, _p_unum, _p_est_nullum, _p_parvum, _p_aequalis, _p_adde,
        _p_subtrahe,
    _p_multiplica, _p_divide_exacte, _p_transcribe, _p_ad_chordam,
    _p_ex_chorda, NIHIL, NIHIL, NIHIL, NIHIL, VERUM, NIHIL
};


/* ==================================================
 * Z/n: congruentia (modulus in anulus->contextus)
 * ================================================== */

#define VALOR_R(x) (*(constans i32*)(x))

interior i32
_modulus (
    constans Anulus* anulus)
{
    redde *(constans i32*)anulus->contextus;
}

interior vacuum
_r_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(i32*)exitus = ZEPHYRUM;
}

interior vacuum
_r_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)piscina;
    *(i32*)exitus = I % _modulus(anulus);
}

interior b32
_r_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    i32 valor = VALOR_R(a);

    (vacuum)anulus;
    redde valor == ZEPHYRUM;
}

interior b32
_r_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    (vacuum)a;
    redde VERUM;
}

interior b32
_r_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    i32 x = VALOR_R(a);
    i32 y = VALOR_R(b);

    (vacuum)anulus;
    redde x == y;
}

interior b32
_r_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)piscina;
    *(i32*)exitus = congruentia_adde(VALOR_R(a), VALOR_R(b),
        _modulus(anulus));
    redde VERUM;
}

interior b32
_r_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)piscina;
    *(i32*)exitus = congruentia_subtrahe(VALOR_R(a), VALOR_R(b),
        _modulus(anulus));
    redde VERUM;
}

interior b32
_r_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)piscina;
    *(i32*)exitus = congruentia_multiplica(VALOR_R(a), VALOR_R(b),
        _modulus(anulus));
    redde VERUM;
}

/* a b^-1; FALSUM si b non invertibilis (etiam si a forte divisibilis:
 * Z/n compositus solum per unitates dividit) */
interior b32
_r_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    i32 inversa;

    (vacuum)piscina;
    si (!congruentia_inversa(VALOR_R(b), _modulus(anulus), &inversa))
    {
        redde FALSUM;
    }
    *(i32*)exitus = congruentia_multiplica(VALOR_R(a), inversa,
        _modulus(anulus));
    redde VERUM;
}

interior vacuum
_r_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *(i32*)exitus = VALOR_R(a);
}

interior chorda
_r_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde magnus_ad_chordam(magnus_ex_s64((s64)VALOR_R(a)), piscina);
}

/* integer decimalis quilibet (etiam negativus), modulo n reductus */
interior b32
_r_ex_chorda (
     constans Anulus* anulus,
              chorda  textus,
             Piscina* piscina,
              vacuum* exitus)
{
    Magnus valor;

    si (!magnus_ex_chorda(textus, piscina, &valor))
    {
        redde FALSUM;
    }
    *(i32*)exitus = congruentia_ex_magno(valor, _modulus(anulus));
    redde VERUM;
}

interior constans Anulus FORMA_RESIDUORUM = {
    "Z/n", (memoriae_index)magnitudo(i32), FALSUM,
    _r_nullum, _r_unum, _r_est_nullum, _r_parvum, _r_aequalis, _r_adde,
    _r_subtrahe, _r_multiplica, _r_divide_exacte, _r_transcribe,
    _r_ad_chordam, _r_ex_chorda, NIHIL, NIHIL, NIHIL, NIHIL, FALSUM,
    NIHIL
};

constans Anulus*
anulus_residuorum (
         i32  n,
     Piscina* piscina)
{
       Anulus* anulus;
          i32* modulus;
    character* titulus;
    character  digiti[XII];
          i32  numerus = ZEPHYRUM;
          i32  k;
          i32  x = n;

    si (n < II)
    {
        redde NIHIL;
    }
    anulus    = (Anulus*)piscina_allocare(piscina, magnitudo(Anulus));
    modulus   = (i32*)piscina_allocare(piscina, magnitudo(i32));
    *anulus   = FORMA_RESIDUORUM;
    *modulus  = n;
    /* titulus "Z/n" */
    dum (x > ZEPHYRUM)
    {
        digiti[numerus++]  = (character)('0' + (s32)(x % X));
        x                  /= X;
    }
    titulus = (character*)piscina_allocare(piscina, (memoriae_index)(
        numerus + IV));
    titulus[ZEPHYRUM]  = 'Z';
    titulus[I]         = '/';
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        titulus[II + k] = digiti[numerus - I - k];
    }
    titulus[II + numerus]  = '\0';
    anulus->titulus        = titulus;
    anulus->corpus         = congruentia_est_primus(n);
    anulus->integrum       = anulus->corpus;
    anulus->contextus      = modulus;
    redde anulus;
}
#undef FORMA_RESIDUORUM
#undef VALOR_P
#undef VALOR_Q
#undef VALOR_R
#undef VALOR_Z
#undef _modulus
#undef _p_ad_chordam
#undef _p_adde
#undef _p_aequalis
#undef _p_divide_exacte
#undef _p_est_nullum
#undef _p_ex_chorda
#undef _p_multiplica
#undef _p_nullum
#undef _p_parvum
#undef _p_subtrahe
#undef _p_transcribe
#undef _p_unum
#undef _q_ad_chordam
#undef _q_adde
#undef _q_aequalis
#undef _q_divide_exacte
#undef _q_est_nullum
#undef _q_ex_chorda
#undef _q_multiplica
#undef _q_nullum
#undef _q_parvum
#undef _q_signum
#undef _q_subtrahe
#undef _q_transcribe
#undef _q_unum
#undef _r_ad_chordam
#undef _r_adde
#undef _r_aequalis
#undef _r_divide_exacte
#undef _r_est_nullum
#undef _r_ex_chorda
#undef _r_multiplica
#undef _r_nullum
#undef _r_parvum
#undef _r_subtrahe
#undef _r_transcribe
#undef _r_unum
#undef _z_ad_chordam
#undef _z_adde
#undef _z_aequalis
#undef _z_compara_normam
#undef _z_divide_cum_residuo
#undef _z_divide_exacte
#undef _z_divisor_communis
#undef _z_est_nullum
#undef _z_ex_chorda
#undef _z_multiplica
#undef _z_nullum
#undef _z_parvum
#undef _z_signum
#undef _z_subtrahe
#undef _z_transcribe
#undef _z_unum
/* lib/matrix.c: statica per plagulam renominata */
#define Officinae Officinae_matrix
#define Operarius Operarius_matrix
#define _apex_notare _apex_notare_matrix
#define _apex_officinarum _apex_officinarum_matrix
#define _columnae _columnae_matrix
#define _columnae_transforma _columnae_transforma_matrix
#define _columnas_permuta _columnas_permuta_matrix
#define _compacta _compacta_matrix
#define _dividit_reliqua _dividit_reliqua_matrix
#define _effectus _effectus_matrix
#define _est_spatium _est_spatium_matrix
#define _hermite _hermite_matrix
#define _identitas_operis _identitas_operis_matrix
#define _linea_normalis _linea_normalis_matrix
#define _lineae _lineae_matrix
#define _lineae_transforma _lineae_transforma_matrix
#define _lineas_permuta _lineas_permuta_matrix
#define _locus _locus_matrix
#define _minima_in_columna _minima_in_columna_matrix
#define _minima_in_linea _minima_in_linea_matrix
#define _nova _nova_matrix
#define _officina_reficere _officina_reficere_matrix
#define _officinae_aperire _officinae_aperire_matrix
#define _officinae_claudere _officinae_claudere_matrix
#define _officinis_utendum _officinis_utendum_matrix
#define _operarius_aperire _operarius_aperire_matrix
#define _operis _operis_matrix
#define _operis_piscina _operis_piscina_matrix
#define _parvae _parvae_matrix
#define _passus _passus_matrix
#define _permuta _permuta_matrix
#define _reductio _reductio_matrix
#define _scala _scala_matrix
#define _servare _servare_matrix
#define _sine_spatiis _sine_spatiis_matrix
#define _summa _summa_matrix
#define _tabula_reddere _tabula_reddere_matrix
#line 1 "lib/matrix.c"
/* matrix.c - Matrices exactae super anulum: Bareiss sine fractionibus
 *
 * Forma scalaris sine fractionibus (_scala): post cardinem (r, c) omne
 * elementum E[i][j] (i > r, j > c) minor est (r + 2) ordinis, ergo
 * divisio per cardinem priorem exacta (identitas Sylvestri) - etiam cum
 * columnis sine cardine. Determinans = cardo ultimus (signo
 * permutationum); gradus = numerus cardinum; nucleus per
 * substitutionem retrogradam SINE divisione (vide _nucleus_vector).
 *
 * Officinae (piscinae internae): 0 et 1 alternae (submatrix adhuc
 * eliminanda), II temporaria (producta unius elementi, refecta post
 * quodque), III stabilis (tabula laboris, lineae cardinum perfectae).
 * Omnis valor qui in piscinam aliam transit per anulus->transcribe
 * transit: nullus valor piscinam refectam partitur. Vide
 * lib/matrix.worklog.md.
 */


#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

/* passus elementi: mensura anuli ad VIII rotundata (ordinatio) */
interior memoriae_index
_passus (
    constans Anulus* anulus)
{
    redde (anulus->mensura + (memoriae_index)VII)
        & ~(memoriae_index)VII;
}

interior i8*
_locus (
                 i8* elementa,
    constans Anulus* anulus,
                i32  columnae,
                i32  linea,
                i32  columna)
{
    redde elementa + ((memoriae_index)linea * (memoriae_index)columnae
        + (memoriae_index)columna) * _passus(anulus);
}

/* matrix nova, elementis nullis */
interior b32
_nova (
     constans Anulus* anulus,
                 i32  lineae,
                 i32  columnae,
             Piscina* piscina,
              Matrix* exitus)
{
    memoriae_index numerus = (memoriae_index)lineae
        * (memoriae_index)columnae;
    memoriae_index k;

    si (anulus == NIHIL)
    {
        redde FALSUM;
    }
    exitus->anulus    = anulus;
    exitus->lineae    = lineae;
    exitus->columnae  = columnae;
    exitus->elementa  = NIHIL;
    si (numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    exitus->elementa = (i8*)piscina_allocare(piscina, numerus
        * _passus(anulus));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        anulus->nullum(anulus, exitus->elementa + k * _passus(anulus));
    }
    redde VERUM;
}


/* ==================================================
 * Officinae
 * ================================================== */

#define OFFICINA_TEMPORARIA  II
#define OFFICINA_STABILIS    III

nomen structura {
           Piscina* piscinae[IV];
    PiscinaNotatio  notae[IV];
               b32  propriae;
} Officinae;

/* DIAGNOSIS: maximus usus officinae in operatione ultima */
interior memoriae_index _apex_officinarum = ZEPHYRUM;

interior vacuum
_apex_notare (
    Piscina* officina)
{
    memoriae_index usus = piscina_summa_usus(officina);

    si (usus > _apex_officinarum)
    {
        _apex_officinarum = usus;
    }
}

/* Via parva (sine officinis, in piscina vocantis) solum si matrix
 * parva ET omnia elementa parva (anulus->parvum: sine memoria externa):
 * creatio quattuor piscinarum plus constat quam servat (recensio
 * matrix-I: 2 x 2 0.5 us contra 0.007 us ad - bc), et iactura vocantis
 * minoribus elementorum parvorum finita. Numerus solus non sufficit
 * (recensio matrix-II: 5 x 5 elementis M digitorum 204 KB pro
 * determinante 2 KB). */
#define MATRIX_LIMES_ELEMENTORUM  XXV
#define MATRIX_LIMES_OPERUM       CXXV

interior b32
_parvae (
    Matrix m)
{
    memoriae_index numerus = (memoriae_index)m.lineae
        * (memoriae_index)m.columnae;
    memoriae_index k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (!m.anulus->parvum(m.anulus, m.elementa
            + k * _passus(m.anulus)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* via officinarum nisi matrix parva et elementis parvis */
interior b32
_officinis_utendum (
            Matrix m,
    memoriae_index limes)
{
    redde (memoriae_index)m.lineae * (memoriae_index)m.columnae > limes
        || !_parvae(m);
}

/* effectus vocanti redditus: semper copia profunda in piscinam eius,
 * etiam via parva - effectus eliminationis et multiplicationis numquam
 * memoriam argumentorum partiuntur (recensio matrix-II) */
interior vacuum
_effectus (
    constans Anulus* anulus,
                 i8* fons,
            Piscina* piscina,
                 i8* destinatio)
{
    anulus->transcribe(anulus, fons, piscina, destinatio);
}

/* Si non utendae aut creatio deficit, omnes = piscina vocantis et
 * refectio nihil agit: effectus idem, memoria sine refectione. */
interior vacuum
_officinae_aperire (
     Officinae* o,
       Piscina* vocantis,
           b32  utendae)
{
    i32 k;
    b32 bene = VERUM;

    _apex_officinarum = ZEPHYRUM;
    si (!utendae)
    {
        per (k = ZEPHYRUM; k < IV; k++)
        {
            o->piscinae[k] = vocantis;
        }
        o->propriae = FALSUM;
        redde;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        o->piscinae[k] = piscina_generare_dynamicum("matrix_officina",
            (memoriae_index)4096);
        si (o->piscinae[k] == NIHIL)
        {
            bene = FALSUM;
        }
    }
    si (!bene)
    {
        per (k = ZEPHYRUM; k < IV; k++)
        {
            si (o->piscinae[k])
            {
                piscina_destruere(o->piscinae[k]);
            }
            o->piscinae[k] = vocantis;
        }
        o->propriae = FALSUM;
        redde;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        o->notae[k] = piscina_notare(o->piscinae[k]);
    }
    o->propriae = VERUM;
}

interior vacuum
_officina_reficere (
     Officinae* o,
           i32  index)
{
    si (o->propriae)
    {
        _apex_notare(o->piscinae[index]);
        piscina_reficere(o->piscinae[index], o->notae[index]);
    }
}

/* valor in piscinam destinationis: copia profunda si officinae
 * propriae (fons refici potest), aliter copia structurae (omnia in
 * piscina vocantis manent) */
interior vacuum
_servare (
    constans Officinae* o,
       constans Anulus* anulus,
                    i8* fons,
               Piscina* piscina,
                    i8* destinatio)
{
    si (o->propriae)
    {
        anulus->transcribe(anulus, fons, piscina, destinatio);
    }
    alioquin si (fons != destinatio)
    {
        memcpy(destinatio, fons, anulus->mensura);
    }
}

interior vacuum
_officinae_claudere (
    Officinae* o)
{
    i32 k;

    si (!o->propriae)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        _apex_notare(o->piscinae[k]);
        piscina_destruere(o->piscinae[k]);
    }
}


/* ==================================================
 * Constructio et textus
 * ================================================== */

b32
matrix_nulla (
     constans Anulus* anulus,
                 i32  lineae,
                 i32  columnae,
             Piscina* piscina,
              Matrix* exitus)
{
    Matrix m;

    si (!_nova(anulus, lineae, columnae, piscina, &m))
    {
        redde FALSUM;
    }
    *exitus = m;
    redde VERUM;
}

b32
matrix_identitas (
     constans Anulus* anulus,
                 i32  n,
             Piscina* piscina,
              Matrix* exitus)
{
    Matrix m;
       i32 k;

    si (!_nova(anulus, n, n, piscina, &m))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        anulus->unum(anulus, piscina, _locus(m.elementa, anulus, n, k,
            k));
    }
    *exitus = m;
    redde VERUM;
}

interior b32
_est_spatium (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* sectio sine spatiis extremis */
interior chorda
_sine_spatiis (
    chorda textus)
{
    i32 initium  = ZEPHYRUM;
    i32 finis    = textus.mensura;

    dum (initium < finis && _est_spatium(textus.datum[initium]))
    {
        initium++;
    }
    dum (finis > initium && _est_spatium(textus.datum[finis - I]))
    {
        finis--;
    }
    redde chorda_sectio(textus, initium, finis);
}

b32
matrix_ex_chorda (
     constans Anulus* anulus,
              chorda  textus,
             Piscina* piscina,
              Matrix* exitus)
{
    chorda  interior_textus;
       i32  lineae    = I;
       i32  columnae  = I;
       i32  commata   = ZEPHYRUM;
       i32  k;
       i32  linea;
       i32  columna;
       i32  initium;
    Matrix  m;
        i8* valor;

    si (anulus == NIHIL || textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    textus = _sine_spatiis(textus);
    si (   textus.mensura < II || textus.datum[ZEPHYRUM] != '['
        || textus.datum[textus.mensura - I] != ']')
    {
        redde FALSUM;
    }
    interior_textus = _sine_spatiis(chorda_sectio(textus, I,
        textus.mensura - I));
    si (interior_textus.mensura == ZEPHYRUM)
    {
        redde matrix_nulla(anulus, ZEPHYRUM, ZEPHYRUM, piscina, exitus);
    }

    /* dimensiones: lineae per ';', columnae lineae primae per ',';
     * omnes lineae tot commata habere debent */
    per (k = ZEPHYRUM; k < interior_textus.mensura; k++)
    {
        si (interior_textus.datum[k] == ';')
        {
            si (lineae == I)
            {
                columnae = commata + I;
            }
            alioquin si (commata + I != columnae)
            {
                redde FALSUM;
            }
            lineae++;
            commata = ZEPHYRUM;
        }
        alioquin si (interior_textus.datum[k] == ',')
        {
            commata++;
        }
    }
    si (lineae == I)
    {
        columnae = commata + I;
    }
    alioquin si (commata + I != columnae)
    {
        redde FALSUM;
    }

    si (!_nova(anulus, lineae, columnae, piscina, &m))
    {
        redde FALSUM;
    }
    valor    = (i8*)piscina_allocare(piscina, _passus(anulus));
    linea    = ZEPHYRUM;
    columna  = ZEPHYRUM;
    initium  = ZEPHYRUM;
    per (k = ZEPHYRUM; k <= interior_textus.mensura; k++)
    {
        si (   k                        == interior_textus.mensura
            || interior_textus.datum[k] == ','
            || interior_textus.datum[k] == ';')
        {
            si (!anulus->ex_chorda(anulus, _sine_spatiis(chorda_sectio(
                interior_textus, initium, k)), piscina, valor))
            {
                redde FALSUM;
            }
            memcpy(_locus(m.elementa, anulus, columnae, linea, columna),
                valor, anulus->mensura);
            si (   k < interior_textus.mensura
                && interior_textus.datum[k] == ';')
            {
                linea++;
                columna = ZEPHYRUM;
            }
            alioquin
            {
                columna++;
            }
            initium = k + I;
        }
    }
    *exitus = m;
    redde VERUM;
}

chorda
matrix_ad_chordam (
     Matrix  m,
    Piscina* piscina)
{
    ChordaAedificator* scriba = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);
                   i32 linea;
                   i32 columna;

    (vacuum)chorda_aedificator_appendere_character(scriba, '[');
    per (linea = ZEPHYRUM; m.columnae > ZEPHYRUM && linea < m.lineae;
         linea++)
    {
        si (linea > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_literis(scriba, "; ");
        }
        per (columna = ZEPHYRUM; columna < m.columnae; columna++)
        {
            si (columna > ZEPHYRUM)
            {
                (vacuum)chorda_aedificator_appendere_literis(scriba,
                    ", ");
            }
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                m.anulus->ad_chordam(m.anulus, _locus(m.elementa,
                m.anulus,
                    m.columnae, linea, columna), piscina));
        }
    }
    (vacuum)chorda_aedificator_appendere_character(scriba, ']');
    redde chorda_aedificator_finire(scriba);
}

constans Anulus*
matrix_anulus (
    Matrix m)
{
    redde m.anulus;
}

i32
matrix_lineae (
    Matrix m)
{
    redde m.lineae;
}

i32
matrix_columnae (
    Matrix m)
{
    redde m.columnae;
}

constans vacuum*
matrix_elementum (
    Matrix m,
       i32 linea,
       i32 columna)
{
    si (linea >= m.lineae || columna >= m.columnae)
    {
        redde NIHIL;
    }
    redde _locus(m.elementa, m.anulus, m.columnae, linea, columna);
}

vacuum
matrix_pone (
               Matrix* m,
                  i32  linea,
                  i32  columna,
      constans vacuum* valor)
{
    si (linea >= m->lineae || columna >= m->columnae)
    {
        redde;
    }
    memcpy(_locus(m->elementa, m->anulus, m->columnae, linea, columna),
        valor, m->anulus->mensura);
}


/* ==================================================
 * Arithmetica
 * ================================================== */

b32
matrix_aequalis (
    Matrix a,
    Matrix b)
{
    i32 linea;
    i32 columna;

    si (   a.anulus   != b.anulus || a.lineae != b.lineae
        || a.columnae != b.columnae)
    {
        redde FALSUM;
    }
    per (linea = ZEPHYRUM; linea < a.lineae; linea++)
    {
        per (columna = ZEPHYRUM; columna < a.columnae; columna++)
        {
            si (!a.anulus->aequalis(a.anulus, _locus(a.elementa,
                a.anulus,
                a.columnae,
                linea, columna), _locus(b.elementa, b.anulus,
                b.columnae,
                linea, columna)))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* a + signum b, per elementa */
interior b32
_summa (
     Matrix  a,
     Matrix  b,
        s32  signum,
    Piscina* piscina,
     Matrix* exitus)
{
    Matrix m;
       i32 linea;
       i32 columna;

    si (   a.anulus   != b.anulus || a.lineae != b.lineae
        || a.columnae != b.columnae
        || !_nova(a.anulus, a.lineae, a.columnae, piscina, &m))
    {
        redde FALSUM;
    }
    per (linea = ZEPHYRUM; linea < a.lineae; linea++)
    {
        per (columna = ZEPHYRUM; columna < a.columnae; columna++)
        {
            constans vacuum* x = _locus(a.elementa, a.anulus,
                a.columnae,
                linea, columna);
            constans vacuum* y = _locus(b.elementa, b.anulus,
                b.columnae,
                linea, columna);
                     vacuum* z = _locus(m.elementa, m.anulus,
                         m.columnae,
                         linea, columna);

            si (!(signum > ZEPHYRUM ? a.anulus->adde(a.anulus, x, y,
                piscina, z)
                : a.anulus->subtrahe(a.anulus, x, y, piscina, z)))
            {
                redde FALSUM;
            }
        }
    }
    *exitus = m;
    redde VERUM;
}

b32
matrix_adde (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus)
{
    redde _summa(a, b, I, piscina, exitus);
}

b32
matrix_subtrahe (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus)
{
    redde _summa(a, b, -I, piscina, exitus);
}

b32
matrix_multiplica (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus)
{
             Matrix  m;
          Officinae  officinae;
    constans Anulus* anulus = a.anulus;
                 i8* productum;
                 i8* summa;
                i32  linea;
                i32  columna;
                i32  k;
                b32  bene = VERUM;

    si (   a.anulus != b.anulus || a.columnae != b.lineae
        || !_nova(anulus, a.lineae, b.columnae, piscina, &m))
    {
        redde FALSUM;
    }
    /* elementum quodque totum in officina temporaria, solus valor
     * finalis transcriptus */
    _officinae_aperire(&officinae, piscina, (memoriae_index)a.lineae
        * (memoriae_index)b.columnae * (memoriae_index)a.columnae
        > (memoriae_index)MATRIX_LIMES_OPERUM || !_parvae(a)
        || !_parvae(b));
    productum  = (i8*)piscina_allocare(officinae.piscinae[
        OFFICINA_STABILIS], _passus(anulus));
    summa      = (i8*)piscina_allocare(officinae.piscinae[
        OFFICINA_STABILIS], _passus(anulus));
    per (linea = ZEPHYRUM; bene && linea < a.lineae; linea++)
    {
        per (columna = ZEPHYRUM; bene
            && columna < b.columnae; columna++)
        {
            Piscina* temporaria =
                officinae.piscinae[OFFICINA_TEMPORARIA];

            anulus->nullum(anulus, summa);
            per (k = ZEPHYRUM; bene && k < a.columnae; k++)
            {
                bene = anulus->multiplica(anulus, _locus(a.elementa,
                    anulus,
                    a.columnae, linea, k), _locus(b.elementa, anulus,
                    b.columnae, k, columna), temporaria, productum)
                    && anulus->adde(anulus, summa, productum,
                    temporaria,
                    summa);
            }
            si (bene)
            {
                _effectus(anulus, summa, piscina,
                    _locus(m.elementa, anulus, m.columnae, linea,
                    columna));
            }
            _officina_reficere(&officinae, OFFICINA_TEMPORARIA);
        }
    }
    _officinae_claudere(&officinae);
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = m;
    redde VERUM;
}

b32
matrix_transposita (
     Matrix  m,
    Piscina* piscina,
     Matrix* exitus)
{
    Matrix t;
       i32 linea;
       i32 columna;

    si (!_nova(m.anulus, m.columnae, m.lineae, piscina, &t))
    {
        redde FALSUM;
    }
    per (linea = ZEPHYRUM; linea < m.lineae; linea++)
    {
        per (columna = ZEPHYRUM; columna < m.columnae; columna++)
        {
            memcpy(_locus(t.elementa, t.anulus, t.columnae, columna,
                linea), _locus(m.elementa, m.anulus, m.columnae, linea,
                columna), m.anulus->mensura);
        }
    }
    *exitus = t;
    redde VERUM;
}


/* ==================================================
 * Eliminatio
 * ================================================== */

/* Forma scalaris sine fractionibus in tabula laboris (officina
 * stabilis). cardines[r] = columna cardinis lineae r (vocans praebet
 * lineae elementa); *gradus, *signum (permutationes linearum). FALSUM
 * si operatio elementi refutat.
 *
 * plena FALSUM (determinans, gradus): lineae infra cardinem solae;
 * lineae cardinum perfectae in officina stabili.
 * plena VERUM (Gauss-Jordan sine fractionibus, nucleus): lineae SUPRA
 * cardinem quoque - post gradum k omnes cardines priores = cardo k
 * (minor), omnia elementa minores, ergo divisio exacta et magnitudo
 * minoribus finita (recensio matrix-I: substitutio retrograda sine
 * divisione 1207 bitorum pro 104 in 25 x 30). Lineae omnes mutantur,
 * ergo omnes in officina alterna vivunt. */
interior b32
_scala (
         Matrix   m,
            b32   plena,
      Officinae*  officinae,
             i8** tabula_exitus,
            i32*  cardines,
            i32*  gradus,
            s32*  signum)
{
     constans Anulus* anulus    = m.anulus;
             Piscina* stabilis;
      memoriae_index  passus = _passus(anulus);
                  i8* tabula = NIHIL;
                  i8* prior;
                  i8* primum;
                  i8* secundum;
                  i8* permutatio;
                 i32  hic  = ZEPHYRUM;
                 i32  r    = ZEPHYRUM;
                 i32  c;
                 i32  i;
                 i32  j;

    stabilis  = officinae->piscinae[OFFICINA_STABILIS];
    *signum   = I;
    si (m.lineae > ZEPHYRUM && m.columnae > ZEPHYRUM)
    {
        tabula = (i8*)piscina_allocare(stabilis,
            (memoriae_index)m.lineae
            * (memoriae_index)m.columnae * passus);
        memcpy(tabula, m.elementa, (memoriae_index)m.lineae
            * (memoriae_index)m.columnae * passus);
    }
    prior       = (i8*)piscina_allocare(stabilis, passus);
    primum      = (i8*)piscina_allocare(stabilis, passus);
    secundum    = (i8*)piscina_allocare(stabilis, passus);
    permutatio  = (i8*)piscina_allocare(stabilis, passus);
    anulus->unum(anulus, stabilis, prior);

    per (c = ZEPHYRUM; c < m.columnae && r < m.lineae; c++)
    {
         Piscina* illic           = officinae->piscinae[I - hic];
             i32  linea_cardinis  = r;

        dum (   linea_cardinis < m.lineae
             && anulus->est_nullum(anulus, _locus(tabula,
            anulus, m.columnae, linea_cardinis, c)))
        {
            linea_cardinis++;
        }
        si (linea_cardinis == m.lineae)
        {
            perge;   /* columna sine cardine */
        }
        si (linea_cardinis != r)
        {
            per (j = ZEPHYRUM; j < m.columnae; j++)
            {
                i8* x = _locus(tabula, anulus, m.columnae, r, j);
                i8* y = _locus(tabula, anulus, m.columnae,
                    linea_cardinis,
                    j);

                memcpy(permutatio, x, anulus->mensura);
                memcpy(x, y, anulus->mensura);
                memcpy(y, permutatio, anulus->mensura);
            }
            *signum = -*signum;
        }

        /* E[i][j] = (E[r][c] E[i][j] - E[i][c] E[r][j]) / prior; supra
         * cardinem (plena) etiam j < c: E[r][j] ibi nullum, ergo
         * scalatio per E[r][c] / prior */
        per (i = plena ? ZEPHYRUM : r + I; i < m.lineae; i++)
        {
            si (i == r)
            {
                perge;
            }
            per (j = (i < r) ? ZEPHYRUM : c + I; j < m.columnae; j++)
            {
                Piscina* temporaria = officinae->piscinae[
                    OFFICINA_TEMPORARIA];
                     b32 bene;

                si (j == c)
                {
                    perge;
                }
                bene = anulus->multiplica(anulus, _locus(tabula, anulus,
                    m.columnae, r, c), _locus(tabula, anulus,
                    m.columnae, i,
                    j), temporaria, primum)
                    && anulus->multiplica(anulus, _locus(tabula, anulus,
                        m.columnae, i, c), _locus(tabula, anulus,
                        m.columnae, r, j), temporaria, secundum)
                    && anulus->subtrahe(anulus, primum, secundum,
                    temporaria,
                        primum)
                    && anulus->divide_exacte(anulus, primum, prior,
                    temporaria,
                        secundum);
                si (!bene)
                {
                    redde FALSUM;
                }
                _servare(officinae, anulus, secundum, illic,
                    _locus(tabula, anulus, m.columnae, i, j));
                _officina_reficere(officinae, OFFICINA_TEMPORARIA);
            }
            anulus->nullum(anulus, _locus(tabula, anulus, m.columnae, i,
                c));
        }

        /* linea r: perfecta in officinam stabilem (plena: in alternam,
         * quia gradibus sequentibus mutatur); cardo fit prior */
        per (j = plena ? ZEPHYRUM : c; j < m.columnae; j++)
        {
            i8* x = _locus(tabula, anulus, m.columnae, r, j);

            memcpy(permutatio, x, anulus->mensura);
            _servare(officinae, anulus, permutatio, plena ? illic
                : stabilis, x);
        }
        memcpy(prior, _locus(tabula, anulus, m.columnae, r, c),
            anulus->mensura);
        _officina_reficere(officinae, hic);
        hic          = I - hic;
        cardines[r]  = c;
        r++;
    }
    *tabula_exitus  = tabula;
    *gradus         = r;
    redde VERUM;
}

b32
matrix_determinans (
     Matrix  m,
    Piscina* piscina,
     vacuum* exitus)
{
     constans Anulus* anulus = m.anulus;
           Officinae  officinae;
                  i8* tabula;
                 i32* cardines;
                 i32  gradus;
                 s32  signum;
                  i8* valor;

    si (m.lineae != m.columnae || !anulus->integrum)
    {
        redde FALSUM;
    }
    si (m.lineae == ZEPHYRUM)
    {
        anulus->unum(anulus, piscina, exitus);
        redde VERUM;
    }
    _officinae_aperire(&officinae, piscina, _officinis_utendum(m,
        (memoriae_index)MATRIX_LIMES_ELEMENTORUM));
    cardines =
        (i32*)piscina_allocare(officinae.piscinae[OFFICINA_STABILIS],
        (memoriae_index)m.lineae * magnitudo(i32));
    valor = (i8*)piscina_allocare(officinae.piscinae[OFFICINA_STABILIS],
        _passus(anulus));
    si (!_scala(m, FALSUM, &officinae, &tabula, cardines, &gradus,
        &signum))
    {
        _officinae_claudere(&officinae);
        redde FALSUM;
    }
    si (gradus < m.lineae)
    {
        anulus->nullum(anulus, exitus);
        _officinae_claudere(&officinae);
        redde VERUM;
    }
    /* cardo ultimus = determinans matricis permutatae */
    memcpy(valor, _locus(tabula, anulus, m.columnae, m.lineae - I,
        m.columnae - I), anulus->mensura);
    si (signum < ZEPHYRUM)
    {
        i8* nullum = (i8*)piscina_allocare(officinae.piscinae[
            OFFICINA_STABILIS], _passus(anulus));

        anulus->nullum(anulus, nullum);
        si (!anulus->subtrahe(anulus, nullum, valor, officinae.piscinae[
            OFFICINA_STABILIS], valor))
        {
            _officinae_claudere(&officinae);
            redde FALSUM;
        }
    }
    _effectus(anulus, valor, piscina, (i8*)exitus);
    _officinae_claudere(&officinae);
    redde VERUM;
}

b32
matrix_gradus (
     Matrix  m,
    Piscina* piscina,
        i32* exitus)
{
    Officinae  officinae;
           i8* tabula;
          i32* cardines;
          i32  gradus;
          s32  signum;
          b32  bene;

    si (!m.anulus->integrum)
    {
        /* gradus super anulum non integrum non definitus */
        redde FALSUM;
    }
    _officinae_aperire(&officinae, piscina, _officinis_utendum(m,
        (memoriae_index)MATRIX_LIMES_ELEMENTORUM));
    cardines =
        (i32*)piscina_allocare(officinae.piscinae[OFFICINA_STABILIS],
        (memoriae_index)(m.lineae + I) * magnitudo(i32));
    bene = _scala(m, FALSUM, &officinae, &tabula, cardines, &gradus,
        &signum);
    _officinae_claudere(&officinae);
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = gradus;
    redde VERUM;
}

/* Nucleus ex forma Gauss-Jordan sine fractionibus: cardines omnes = D
 * (cardo ultimus), ergo linea i: D z_p(i) + summa_{f libera} E[i][f]
 * z_f = 0. Pro columna libera f: z_f = D, z_p(i) = -E[i][f], ceteri 0
 * - elementa minores (magnitudo Hadamard finita). Sine cardine: D = 1,
 * nucleus = identitas. */
b32
matrix_nucleus (
     Matrix  m,
    Piscina* piscina,
     Matrix* exitus)
{
     constans Anulus* anulus = m.anulus;
           Officinae  officinae;
              Matrix  nucleus;
                  i8* tabula;
                 i32* cardines;
                 b32* est_cardo;
                  i8* d;
                  i8* nullum;
                  i8* valor;
                 i32  gradus;
                 s32  signum;
                 i32  j;
                 i32  q = ZEPHYRUM;
             Piscina* stabilis;

    si (!anulus->integrum)
    {
        /* nucleus super corpus fractionum: integrum requiritur */
        redde FALSUM;
    }
    _officinae_aperire(&officinae, piscina, _officinis_utendum(m,
        (memoriae_index)MATRIX_LIMES_ELEMENTORUM));
    stabilis   = officinae.piscinae[OFFICINA_STABILIS];
    cardines   = (i32*)piscina_allocare(stabilis, (memoriae_index)(
        m.lineae + I) * magnitudo(i32));
    est_cardo  = (b32*)piscina_allocare(stabilis, (memoriae_index)(
        m.columnae + I) * magnitudo(b32));
    d       = (i8*)piscina_allocare(stabilis, _passus(anulus));
    nullum  = (i8*)piscina_allocare(stabilis, _passus(anulus));
    valor   = (i8*)piscina_allocare(stabilis, _passus(anulus));
    si (   !_scala(m, VERUM, &officinae, &tabula, cardines, &gradus,
            &signum)
        || !_nova(anulus, m.columnae, m.columnae - gradus, piscina,
            &nucleus))
    {
        _officinae_claudere(&officinae);
        redde FALSUM;
    }
    anulus->nullum(anulus, nullum);
    si (gradus == ZEPHYRUM)
    {
        anulus->unum(anulus, stabilis, d);
    }
    alioquin
    {
        memcpy(d, _locus(tabula, anulus, m.columnae, gradus - I,
            cardines[gradus - I]), anulus->mensura);
    }
    per (j = ZEPHYRUM; j < m.columnae; j++)
    {
        est_cardo[j] = FALSUM;
    }
    per (j = ZEPHYRUM; j < gradus; j++)
    {
        est_cardo[cardines[j]] = VERUM;
    }
    per (j = ZEPHYRUM; j < m.columnae; j++)
    {
        i32 i;

        si (est_cardo[j])
        {
            perge;
        }
        _effectus(anulus, d, piscina,
            _locus(nucleus.elementa,
            anulus, nucleus.columnae, j, q));
        per (i = ZEPHYRUM; i < gradus; i++)
        {
            si (!anulus->subtrahe(anulus, nullum, _locus(tabula, anulus,
                m.columnae, i, j),
                officinae.piscinae[OFFICINA_TEMPORARIA],
                valor))
            {
                _officinae_claudere(&officinae);
                redde FALSUM;
            }
            _effectus(anulus, valor, piscina, _locus(
                nucleus.elementa, anulus, nucleus.columnae, cardines[i],
                q));
            _officina_reficere(&officinae, OFFICINA_TEMPORARIA);
        }
        q++;
    }
    _officinae_claudere(&officinae);
    *exitus = nucleus;
    redde VERUM;
}


/* ==================================================
 * Formae normales super anulum Euclideum (Hermite, Smith)
 * ================================================== */

#define TABULA_A  ZEPHYRUM
#define TABULA_U  I
#define TABULA_V  II

/* Operarius: tabulae laboris (A, et U, V si certificata quaeruntur) in
 * officina stabili; valores vivi in officina alterna currente (hic).
 * Post
 * quemque gradum cardinis _compacta omnes valores vivos in alteram
 * transcribit et priorem reficit: memoria proportionalis tabulis, non
 * operationibus. Alvei elementorum (coefficientes, temporaria) in
 * stabili. */
nomen structura {
    constans Anulus* anulus;
          Officinae  officinae;
                i32  hic;
                 i8* tabulae[III];
                i32  lineae[III];
                i32  columnae[III];
                 i8* alvei[XII];
} Operarius;

#define ALVEUS_A    ZEPHYRUM
#define ALVEUS_B    I
#define ALVEUS_C    II
#define ALVEUS_D    III
#define ALVEUS_G    IV
#define ALVEUS_P    V
#define ALVEUS_Q    VI
#define ALVEUS_X    VII
#define ALVEUS_Y    VIII
#define ALVEUS_T    IX
#define ALVEUS_NX   X
#define ALVEUS_NY   XI

interior i8*
_operis (
     Operarius* o,
           i32  tabula,
           i32  linea,
           i32  columna)
{
    redde _locus(o->tabulae[tabula], o->anulus, o->columnae[tabula],
        linea,
        columna);
}

/* tabula identitatis n x n in officina stabili */
interior i8*
_identitas_operis (
            Operarius* o,
                  i32  n)
{
     Piscina* stabilis  = o->officinae.piscinae[OFFICINA_STABILIS];
          i8* tabula    = NIHIL;
         i32  i;
         i32  j;

    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    tabula = (i8*)piscina_allocare(stabilis, (memoriae_index)n
        * (memoriae_index)n * _passus(o->anulus));
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (j = ZEPHYRUM; j < n; j++)
        {
            i8* x = _locus(tabula, o->anulus, n, i, j);

            si (i == j)
            {
                o->anulus->unum(o->anulus, stabilis, x);
            }
            alioquin
            {
                o->anulus->nullum(o->anulus, x);
            }
        }
    }
    redde tabula;
}

interior vacuum
_operarius_aperire (
      Operarius* o,
         Matrix  m,
        Piscina* piscina,
            b32  cum_u,
            b32  cum_v)
{
     Piscina* stabilis;
         i32  k;

    o->anulus = m.anulus;
    _officinae_aperire(&o->officinae, piscina, VERUM);
    o->hic    = ZEPHYRUM;
    stabilis  = o->officinae.piscinae[OFFICINA_STABILIS];
    per (k = ZEPHYRUM; k < XII; k++)
    {
        o->alvei[k] = (i8*)piscina_allocare(stabilis,
            _passus(m.anulus));
    }
    o->lineae[TABULA_A]    = m.lineae;
    o->columnae[TABULA_A]  = m.columnae;
    o->tabulae[TABULA_A]   = NIHIL;
    si (m.lineae > ZEPHYRUM && m.columnae > ZEPHYRUM)
    {
        o->tabulae[TABULA_A] = (i8*)piscina_allocare(stabilis,
            (memoriae_index)m.lineae * (memoriae_index)m.columnae
            * _passus(m.anulus));
        memcpy(o->tabulae[TABULA_A], m.elementa,
            (memoriae_index)m.lineae
            * (memoriae_index)m.columnae * _passus(m.anulus));
    }
    o->lineae[TABULA_U]    = cum_u ? m.lineae : ZEPHYRUM;
    o->columnae[TABULA_U]  = o->lineae[TABULA_U];
    o->tabulae[TABULA_U]   = _identitas_operis(o, o->lineae[TABULA_U]);
    o->lineae[TABULA_V]    = cum_v ? m.columnae : ZEPHYRUM;
    o->columnae[TABULA_V]  = o->lineae[TABULA_V];
    o->tabulae[TABULA_V]   = _identitas_operis(o, o->lineae[TABULA_V]);
}

interior Piscina*
_operis_piscina (
    Operarius* o)
{
    redde o->officinae.piscinae[o->hic];
}

/* omnes valores vivi in officinam alteram; prior reficitur */
interior vacuum
_compacta (
    Operarius* o)
{
     Piscina* illic = o->officinae.piscinae[I - o->hic];
         i32  t;
         i32  i;
         i32  j;

    per (t = ZEPHYRUM; t < III; t++)
    {
        per (i = ZEPHYRUM; i < o->lineae[t]; i++)
        {
            per (j = ZEPHYRUM; j < o->columnae[t]; j++)
            {
                i8* x = _operis(o, t, i, j);

                _servare(&o->officinae, o->anulus, x, illic, x);
            }
        }
    }
    _officina_reficere(&o->officinae, o->hic);
    o->hic = I - o->hic;
}

/* lineae i, k tabulae t: L_i <- a L_i + b L_k, L_k <- c L_i + d L_k */
interior b32
_lineae_transforma (
     Operarius* o,
           i32  t,
           i32  i,
           i32  k)
{
     constans Anulus* anulus  = o->anulus;
             Piscina* hic     = _operis_piscina(o);
                 i32  j;

    per (j = ZEPHYRUM; j < o->columnae[t]; j++)
    {
        i8* x = _operis(o, t, i, j);
        i8* y = _operis(o, t, k, j);

        si (   !anulus->multiplica(anulus, o->alvei[ALVEUS_A], x, hic,
                o->alvei[ALVEUS_NX])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_B], y, hic,
                o->alvei[ALVEUS_T])
            || !anulus->adde(anulus, o->alvei[ALVEUS_NX],
            o->alvei[ALVEUS_T],
            hic,
                o->alvei[ALVEUS_NX])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_C], x, hic,
                o->alvei[ALVEUS_NY])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_D], y, hic,
                o->alvei[ALVEUS_T])
            || !anulus->adde(anulus, o->alvei[ALVEUS_NY],
            o->alvei[ALVEUS_T],
            hic,
                o->alvei[ALVEUS_NY]))
        {
            redde FALSUM;
        }
        memcpy(x, o->alvei[ALVEUS_NX], anulus->mensura);
        memcpy(y, o->alvei[ALVEUS_NY], anulus->mensura);
    }
    redde VERUM;
}

/* columnae j, k tabulae t: C_j <- a C_j + b C_k, C_k <- c C_j +
 * d C_k */
interior b32
_columnae_transforma (
     Operarius* o,
           i32  t,
           i32  j,
           i32  k)
{
     constans Anulus* anulus  = o->anulus;
             Piscina* hic     = _operis_piscina(o);
                 i32  i;

    per (i = ZEPHYRUM; i < o->lineae[t]; i++)
    {
        i8* x = _operis(o, t, i, j);
        i8* y = _operis(o, t, i, k);

        si (   !anulus->multiplica(anulus, o->alvei[ALVEUS_A], x, hic,
                o->alvei[ALVEUS_NX])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_B], y, hic,
                o->alvei[ALVEUS_T])
            || !anulus->adde(anulus, o->alvei[ALVEUS_NX],
            o->alvei[ALVEUS_T],
            hic,
                o->alvei[ALVEUS_NX])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_C], x, hic,
                o->alvei[ALVEUS_NY])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_D], y, hic,
                o->alvei[ALVEUS_T])
            || !anulus->adde(anulus, o->alvei[ALVEUS_NY],
            o->alvei[ALVEUS_T],
            hic,
                o->alvei[ALVEUS_NY]))
        {
            redde FALSUM;
        }
        memcpy(x, o->alvei[ALVEUS_NX], anulus->mensura);
        memcpy(y, o->alvei[ALVEUS_NY], anulus->mensura);
    }
    redde VERUM;
}

/* Reductio Euclidea: x = q p + r; alvei (A, B, C, D) = (1, 0, -q, 1),
 * ergo operatio (cardo, k) L_k <- L_k - q L_cardinis (aut columnae).
 * *exacta = (r nullum). Sine Bezout: multiplicatores toti lineae soli
 * quotientes sunt (recensio matrix-III: Bezout lineam cardinis per
 * columnam totam multiplicabat - Hermite 36 x 36 22.6 s, 40 MB). */
interior b32
_reductio (
             Operarius* o,
       constans vacuum* x,
       constans vacuum* p,
                   b32* exacta)
{
    constans Anulus* anulus  = o->anulus;
            Piscina* hic     = _operis_piscina(o);

    si (!anulus->divide_cum_residuo(anulus, x, p, hic,
        o->alvei[ALVEUS_X],
        o->alvei[ALVEUS_Y]))
    {
        redde FALSUM;
    }
    *exacta = anulus->est_nullum(anulus, o->alvei[ALVEUS_Y]);
    anulus->unum(anulus, hic, o->alvei[ALVEUS_A]);
    anulus->nullum(anulus, o->alvei[ALVEUS_B]);
    anulus->unum(anulus, hic, o->alvei[ALVEUS_D]);
    anulus->nullum(anulus, o->alvei[ALVEUS_T]);
    redde anulus->subtrahe(anulus, o->alvei[ALVEUS_T],
        o->alvei[ALVEUS_X], hic,
        o->alvei[ALVEUS_C]);
}

/* linea (>= ab) cum elemento non nullo normae minimae in columna c; -1
 * si nullum */
interior s32
_minima_in_columna (
     Operarius* o,
           i32  c,
           i32  ab)
{
     constans Anulus* anulus = o->anulus;
                 s32  optima = -I;
                 i32  i;

    per (i = ab; i < o->lineae[TABULA_A]; i++)
    {
        i8* x = _operis(o, TABULA_A, i, c);

        si (anulus->est_nullum(anulus, x))
        {
            perge;
        }
        si (   optima < ZEPHYRUM
            || anulus->compara_normam(anulus, x, _operis(o, TABULA_A,
            (i32)optima,
                c), _operis_piscina(o)) < ZEPHYRUM)
        {
            optima = (s32)i;
        }
    }
    redde optima;
}

/* columna (>= ab) cum elemento non nullo normae minimae in linea l */
interior s32
_minima_in_linea (
     Operarius* o,
           i32  l,
           i32  ab)
{
     constans Anulus* anulus = o->anulus;
                 s32  optima = -I;
                 i32  j;

    per (j = ab; j < o->columnae[TABULA_A]; j++)
    {
        i8* x = _operis(o, TABULA_A, l, j);

        si (anulus->est_nullum(anulus, x))
        {
            perge;
        }
        si (   optima < ZEPHYRUM
            || anulus->compara_normam(anulus, x, _operis(o, TABULA_A, l,
                (i32)optima), _operis_piscina(o)) < ZEPHYRUM)
        {
            optima = (s32)j;
        }
    }
    redde optima;
}

/* operatio linearum in A et U */
interior b32
_lineae (
     Operarius* o,
           i32  i,
           i32  k)
{
    redde _lineae_transforma(o, TABULA_A, i, k)
        && (o->lineae[TABULA_U] == ZEPHYRUM
            || _lineae_transforma(o, TABULA_U, i, k));
}

/* operatio columnarum in A et V */
interior b32
_columnae (
     Operarius* o,
           i32  j,
           i32  k)
{
    redde _columnae_transforma(o, TABULA_A, j, k)
        && (o->lineae[TABULA_V] == ZEPHYRUM
            || _columnae_transforma(o, TABULA_V, j, k));
}

interior vacuum
_permuta (
    Operarius* o,
           i8* x,
           i8* y)
{
    memcpy(o->alvei[ALVEUS_T], x, o->anulus->mensura);
    memcpy(x, y, o->anulus->mensura);
    memcpy(y, o->alvei[ALVEUS_T], o->anulus->mensura);
}

interior vacuum
_lineas_permuta (
     Operarius* o,
           i32  i,
           i32  k)
{
    i32 t;
    i32 j;

    per (t = TABULA_A; t <= TABULA_U; t++)
    {
        per (j = ZEPHYRUM; j < o->columnae[t] && i != k; j++)
        {
            _permuta(o, _operis(o, t, i, j), _operis(o, t, k, j));
        }
    }
}

interior vacuum
_columnas_permuta (
     Operarius* o,
           i32  j,
           i32  k)
{
    i32 i;

    per (i = ZEPHYRUM; i < o->lineae[TABULA_A] && j != k; i++)
    {
        _permuta(o, _operis(o, TABULA_A, i, j), _operis(o, TABULA_A, i,
            k));
    }
    per (i = ZEPHYRUM; i < o->lineae[TABULA_V] && j != k; i++)
    {
        _permuta(o, _operis(o, TABULA_V, i, j), _operis(o, TABULA_V, i,
            k));
    }
}

/* cardo lineae i normalis: linea per unitatem u multiplicatur ubi u p =
 * g = mdc(p, 0) (Z: signum) */
interior b32
_linea_normalis (
     Operarius* o,
           i32  i,
           i32  c)
{
     constans Anulus* anulus  = o->anulus;
             Piscina* hic     = _operis_piscina(o);
                 i32  t;
                 i32  j;

    anulus->nullum(anulus, o->alvei[ALVEUS_T]);
    si (!anulus->divisor_communis(anulus, _operis(o, TABULA_A, i, c),
        o->alvei[ALVEUS_T], hic, o->alvei[ALVEUS_G], o->alvei[ALVEUS_A],
        o->alvei[ALVEUS_B]))
    {
        redde FALSUM;
    }
    per (t = TABULA_A; t <= TABULA_U; t++)
    {
        per (j = ZEPHYRUM; j < o->columnae[t]; j++)
        {
            i8* x = _operis(o, t, i, j);

            si (!anulus->multiplica(anulus, o->alvei[ALVEUS_A], x, hic,
                x))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* tabula operis in matricem vocantis (copia profunda) */
interior b32
_tabula_reddere (
      Operarius* o,
            i32  t,
        Piscina* piscina,
         Matrix* exitus)
{
    Matrix m;
       i32 i;
       i32 j;

    si (!_nova(o->anulus, o->lineae[t], o->columnae[t], piscina, &m))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < o->lineae[t]; i++)
    {
        per (j = ZEPHYRUM; j < o->columnae[t]; j++)
        {
            _effectus(o->anulus, _operis(o, t, i, j), piscina, _locus(
                m.elementa, o->anulus, m.columnae, i, j));
        }
    }
    *exitus = m;
    redde VERUM;
}

/* Hermite in operario per Euclidem in quaque columna: cardo normae
 * minimae sursum, ceterae lineae modulo eum (quotiens solus
 * multiplicator), donec columna infra nulla; *gradus = lineae non
 * nullae */
interior b32
_hermite (
    Operarius* o,
          i32* gradus)
{
     constans Anulus* anulus    = o->anulus;
                 i32  lineae    = o->lineae[TABULA_A];
                 i32  columnae  = o->columnae[TABULA_A];
                 i32  r         = ZEPHYRUM;
                 i32  c;
                 i32  i;
                 i32  k;

    per (c = ZEPHYRUM; c < columnae && r < lineae; c++)
    {
        dum (VERUM)
        {
            s32 cardo   = _minima_in_columna(o, c, r);
            b32 exacta  = VERUM;

            si (cardo < ZEPHYRUM)
            {
                frange;
            }
            _lineas_permuta(o, r, (i32)cardo);
            per (i = r + I; i < lineae; i++)
            {
                b32 haec_exacta;

                si (anulus->est_nullum(anulus, _operis(o, TABULA_A, i,
                    c)))
                {
                    perge;
                }
                si (   !_reductio(o, _operis(o, TABULA_A, i, c),
                        _operis(o, TABULA_A, r, c), &haec_exacta)
                    || !_lineae(o, r, i))
                {
                    redde FALSUM;
                }
                si (!haec_exacta)
                {
                    exacta = FALSUM;
                }
            }
            _compacta(o);
            si (exacta)
            {
                frange;
            }
        }
        si (anulus->est_nullum(anulus, _operis(o, TABULA_A, r, c)))
        {
            perge;
        }
        si (!_linea_normalis(o, r, c))
        {
            redde FALSUM;
        }
        /* supra cardinem: residua 0 <= x < cardo */
        per (k = ZEPHYRUM; k < r; k++)
        {
            Piscina* hic = _operis_piscina(o);

            si (!anulus->divide_cum_residuo(anulus, _operis(o, TABULA_A,
                k, c),
                _operis(o, TABULA_A, r, c), hic, o->alvei[ALVEUS_X],
                o->alvei[ALVEUS_Y]))
            {
                redde FALSUM;
            }
            anulus->unum(anulus, hic, o->alvei[ALVEUS_A]);
            anulus->nullum(anulus, o->alvei[ALVEUS_T]);
            si (!anulus->subtrahe(anulus, o->alvei[ALVEUS_T],
                o->alvei[ALVEUS_X],
                hic, o->alvei[ALVEUS_B]))
            {
                redde FALSUM;
            }
            anulus->nullum(anulus, o->alvei[ALVEUS_C]);
            anulus->unum(anulus, hic, o->alvei[ALVEUS_D]);
            si (!_lineae(o, k, r))
            {
                redde FALSUM;
            }
        }
        _compacta(o);
        r++;
    }
    *gradus = r;
    redde VERUM;
}

b32
matrix_forma_hermite (
     Matrix  a,
    Piscina* piscina,
     Matrix* h,
     Matrix* u)
{
    Operarius o;
          i32 gradus;
       Matrix forma;
       Matrix transformatio;

    si (   a.anulus->divisor_communis   == NIHIL
        || a.anulus->divide_cum_residuo == NIHIL
        || a.anulus->compara_normam     == NIHIL)
    {
        redde FALSUM;
    }
    _operarius_aperire(&o, a, piscina, u != NIHIL, FALSUM);
    si (   !_hermite(&o, &gradus)
        || !_tabula_reddere(&o, TABULA_A, piscina, &forma)
        || (u != NIHIL && !_tabula_reddere(&o, TABULA_U, piscina,
            &transformatio)))
    {
        _officinae_claudere(&o.officinae);
        redde FALSUM;
    }
    _officinae_claudere(&o.officinae);
    *h = forma;
    si (u != NIHIL)
    {
        *u = transformatio;
    }
    redde VERUM;
}

/* E[t][t] dividit omnia E[i][j] (i, j > t)? Si non, *linea = i. */
interior b32
_dividit_reliqua (
     Operarius* o,
           i32  t,
           i32* linea)
{
     constans Anulus* anulus = o->anulus;
                 i32  i;
                 i32  j;

    per (i = t + I; i < o->lineae[TABULA_A]; i++)
    {
        per (j = t + I; j < o->columnae[TABULA_A]; j++)
        {
            si (!anulus->divide_exacte(anulus, _operis(o, TABULA_A, i,
                j),
                _operis(o, TABULA_A, t, t), _operis_piscina(o),
                o->alvei[ALVEUS_X]))
            {
                *linea = i;
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

b32
matrix_forma_smith (
     Matrix  a,
    Piscina* piscina,
     Matrix* d,
     Matrix* u,
     Matrix* v)
{
     constans Anulus* anulus = a.anulus;
           Operarius  o;
                 i32  t;
                 i32  n = a.lineae < a.columnae ? a.lineae : a.columnae;
              Matrix  forma;
              Matrix  sinistra;
              Matrix  dextra;

    si (   anulus->divisor_communis   == NIHIL
        || anulus->divide_cum_residuo == NIHIL
        || anulus->compara_normam     == NIHIL)
    {
        redde FALSUM;
    }
    _operarius_aperire(&o, a, piscina, u != NIHIL, v != NIHIL);
    per (t = ZEPHYRUM; t < n; t++)
    {
        i32 i;
        i32 j;
        b32 inventum = FALSUM;

        /* elementum non nullum in submatrice ad (t, t) */
        per (i = t; i < a.lineae && !inventum; i++)
        {
            per (j = t; j < a.columnae && !inventum; j++)
            {
                si (!anulus->est_nullum(anulus, _operis(&o, TABULA_A, i,
                    j)))
                {
                    _lineas_permuta(&o, t, i);
                    _columnas_permuta(&o, t, j);
                    inventum = VERUM;
                }
            }
        }
        si (!inventum)
        {
            frange;
        }
        /* Euclides alternus: columna t (cardo minimus sursum, lineae
         * modulo), deinde linea t (cardo minimus sinistrorsum, columnae
         * modulo); quodque residuum non nullum normam cardinis stricte
         * minuit, ergo terminatio. Operationes columnarum columnam t
         * non replent (infra cardinem nulla). */
        dum (VERUM)
        {
            s32 k;
            b32 exacta = VERUM;
            i32 linea;

            k = _minima_in_columna(&o, t, t);
            _lineas_permuta(&o, t, (i32)k);
            per (i = t + I; i < a.lineae; i++)
            {
                b32 haec_exacta;

                si (anulus->est_nullum(anulus, _operis(&o, TABULA_A, i,
                    t)))
                {
                    perge;
                }
                si (   !_reductio(&o, _operis(&o, TABULA_A, i, t),
                        _operis(&o, TABULA_A, t, t), &haec_exacta)
                    || !_lineae(&o, t, i))
                {
                    _officinae_claudere(&o.officinae);
                    redde FALSUM;
                }
                si (!haec_exacta)
                {
                    exacta = FALSUM;
                }
            }
            si (!exacta)
            {
                _compacta(&o);
                perge;
            }
            k = _minima_in_linea(&o, t, t);
            si ((i32)k != t)
            {
                _columnas_permuta(&o, t, (i32)k);
                _compacta(&o);
                perge;
            }
            per (j = t + I; j < a.columnae; j++)
            {
                b32 haec_exacta;

                si (anulus->est_nullum(anulus, _operis(&o, TABULA_A, t,
                    j)))
                {
                    perge;
                }
                si (   !_reductio(&o, _operis(&o, TABULA_A, t, j),
                        _operis(&o, TABULA_A, t, t), &haec_exacta)
                    || !_columnae(&o, t, j))
                {
                    _officinae_claudere(&o.officinae);
                    redde FALSUM;
                }
                si (!haec_exacta)
                {
                    exacta = FALSUM;
                }
            }
            _compacta(&o);
            si (!exacta)
            {
                perge;
            }
            /* divisibilitas: si cardo elementum reliquum non dividit,
             * L_t += L_i et iterum (cardo ad mdc decrescit) */
            si (!_dividit_reliqua(&o, t, &linea))
            {
                anulus->unum(anulus, _operis_piscina(&o),
                    o.alvei[ALVEUS_A]);
                anulus->unum(anulus, _operis_piscina(&o),
                    o.alvei[ALVEUS_B]);
                anulus->nullum(anulus, o.alvei[ALVEUS_C]);
                anulus->unum(anulus, _operis_piscina(&o),
                    o.alvei[ALVEUS_D]);
                si (!_lineae(&o, t, linea))
                {
                    _officinae_claudere(&o.officinae);
                    redde FALSUM;
                }
                perge;
            }
            frange;
        }
        si (!_linea_normalis(&o, t, t))
        {
            _officinae_claudere(&o.officinae);
            redde FALSUM;
        }
        _compacta(&o);
    }
    si (   !_tabula_reddere(&o, TABULA_A, piscina, &forma)
        || (u != NIHIL && !_tabula_reddere(&o, TABULA_U, piscina,
            &sinistra))
        || (v != NIHIL
            && !_tabula_reddere(&o, TABULA_V, piscina, &dextra)))
    {
        _officinae_claudere(&o.officinae);
        redde FALSUM;
    }
    _officinae_claudere(&o.officinae);
    *d = forma;
    si (u != NIHIL)
    {
        *u = sinistra;
    }
    si (v != NIHIL)
    {
        *v = dextra;
    }
    redde VERUM;
}

/* Basis reticuli nuclei: U A^T = H (Hermite); lineae U quibus lineae H
 * nullae respondent nucleum A generant super Z (U unimodularis: basis
 * totius reticuli, non sub-reticuli). */
b32
matrix_reticulum_nuclei (
     Matrix  a,
    Piscina* piscina,
     Matrix* exitus)
{
     constans Anulus* anulus = a.anulus;
             Piscina* privata;
              Matrix  transposita;
              Matrix  nucleus;
           Operarius  o;
                 i32  gradus;
                 i32  i;
                 i32  q;
                 b32  bene;

    si (   anulus->divisor_communis   == NIHIL
        || anulus->divide_cum_residuo == NIHIL
        || anulus->compara_normam     == NIHIL)
    {
        redde FALSUM;
    }
    privata = piscina_generare_dynamicum("matrix_reticulum",
        (memoriae_index)4096);
    si (privata == NIHIL)
    {
        privata = piscina;
    }
    bene = matrix_transposita(a, privata, &transposita);
    si (bene)
    {
        _operarius_aperire(&o, transposita, privata, VERUM, FALSUM);
        bene = _hermite(&o, &gradus)
            && _nova(anulus, a.columnae, a.columnae - gradus, piscina,
                &nucleus);
        per (q = ZEPHYRUM; bene && q < a.columnae - gradus; q++)
        {
            per (i = ZEPHYRUM; i < a.columnae; i++)
            {
                _effectus(anulus, _operis(&o, TABULA_U, gradus + q, i),
                    piscina, _locus(nucleus.elementa, anulus,
                    nucleus.columnae, i, q));
            }
        }
        _officinae_claudere(&o.officinae);
    }
    si (privata != piscina)
    {
        piscina_destruere(privata);
    }
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = nucleus;
    redde VERUM;
}


/* ==================================================
 * Diagnosis
 * ================================================== */

memoriae_index
matrix_apex_officinarum (
    vacuum)
{
    redde _apex_officinarum;
}
#undef ALVEUS_A
#undef ALVEUS_B
#undef ALVEUS_C
#undef ALVEUS_D
#undef ALVEUS_G
#undef ALVEUS_NX
#undef ALVEUS_NY
#undef ALVEUS_P
#undef ALVEUS_Q
#undef ALVEUS_T
#undef ALVEUS_X
#undef ALVEUS_Y
#undef MATRIX_LIMES_ELEMENTORUM
#undef MATRIX_LIMES_OPERUM
#undef OFFICINA_STABILIS
#undef OFFICINA_TEMPORARIA
#undef Officinae
#undef Operarius
#undef TABULA_A
#undef TABULA_U
#undef TABULA_V
#undef _apex_notare
#undef _apex_officinarum
#undef _columnae
#undef _columnae_transforma
#undef _columnas_permuta
#undef _compacta
#undef _dividit_reliqua
#undef _effectus
#undef _est_spatium
#undef _hermite
#undef _identitas_operis
#undef _linea_normalis
#undef _lineae
#undef _lineae_transforma
#undef _lineas_permuta
#undef _locus
#undef _minima_in_columna
#undef _minima_in_linea
#undef _nova
#undef _officina_reficere
#undef _officinae_aperire
#undef _officinae_claudere
#undef _officinis_utendum
#undef _operarius_aperire
#undef _operis
#undef _operis_piscina
#undef _parvae
#undef _passus
#undef _permuta
#undef _reductio
#undef _scala
#undef _servare
#undef _sine_spatiis
#undef _summa
#undef _tabula_reddere
/* lib/radices.c: statica per plagulam renominata */
#define Opus Opus_radices
#define _ad_chordam_intra _ad_chordam_intra_radices
#define _angusta_intra _angusta_intra_radices
#define _apex_acervi _apex_acervi_radices
#define _apex_officinae _apex_officinae_radices
#define _compara_intra _compara_intra_radices
#define _decimalis _decimalis_radices
#define _dimidium _dimidium_radices
#define _est_exacta _est_exacta_radices
#define _exponentes_ordinarii _exponentes_ordinarii_radices
#define _fractio_binaria _fractio_binaria_radices
#define _gradus _gradus_radices
#define _opus_pone _opus_pone_radices
#define _ordo_intervallorum _ordo_intervallorum_radices
#define _piscina_destrue _piscina_destrue_radices
#define _piscina_temporaria _piscina_temporaria_radices
#define _piscinae_destrue _piscinae_destrue_radices
#define _primitiva _primitiva_radices
#define _reales_intra _reales_intra_radices
#define _reversum _reversum_radices
#define _scala_coefficientium _scala_coefficientium_radices
#define _seca_medio _seca_medio_radices
#define _separa_intra _separa_intra_radices
#define _separa_positivas _separa_positivas_radices
#define _signum_ad _signum_ad_radices
#define _signum_iuxta _signum_iuxta_radices
#define _signum_polynomii_intra _signum_polynomii_intra_radices
#define _variationes _variationes_radices
#define _variationes_in _variationes_in_radices
#define _variationes_unum _variationes_unum_radices
#line 1 "lib/radices.c"
/* radices.c - Radices reales polynomiorum integrorum, exacte
 *
 * VCA (Vincent - Collins - Akritas): radices positivae f in (0, 2^k)
 * (limes Cauchy: |radix| < 1 + max|a_i| < 2^k) per Q(x) = f(2^k x) in
 * (0, 1). Pro Q: v = variationes signorum (x+1)^n Q(1/(x+1)) (regula
 * Cartesii: radices in (0, 1) <= v, paritate eadem; v = 0 nullae, v = 1
 * una). v >= 2: bisectio - L(x) = 2^n Q(x/2) (dimidium sinistrum), R(x)
 * = L(x + 1) (dextrum); Q(1/2) = 0 sse L(1) = summa coefficientium L =
 * 0: radix rationalis in medio, exacte reddita (Cartesius radices in
 * terminis non numerat, ergo filii non turbantur). Acervus: dextrum,
 * radix media, sinistrum - tracta sinistrum primum: ordo crescens. f
 * liber quadratis, ergo terminatur. Negativae per f(-t); 0 per f(0).
 *
 * Termini intervallorum dyadici; radicem SUAM numquam continent, sed
 * radices ALIAS f esse possunt (radix media exacte reddita terminus
 * filiorum fit: sqrt3 in (1, 2) ubi 1 et 2 radices sunt). f liber
 * quadratis, ergo radices simplices: signum iuxta terminum x (intra
 * intervallum) = signum f(x), aut si f(x) = 0 signum f'(x) (dextrum)
 * vel -signum f'(x) (sinistrum). Bisectio et probatio radicis communis
 * hoc signo utuntur.
 *
 * Memoria: in piscina vocantis effecta sola (radices, f, termini,
 * chorda); comparatio et signum nihil ibi allocant. Temporaria in
 * piscinis propriis: VCA acervum habet LIFO per notas (opus extractum
 * cum omnibus post eum allocatis liberatur) et opus post quodque opus
 * refectum; ansae angustationis opus post quamque iterationem reficiunt,
 * termini novi in status transcribuntur. Porta venenum (0xA5) lectionem
 * post refectionem capit.
 * Vide lib/radices.worklog.md.
 */



/* limites: separatio 2^-2048 longe ultra usum (geminae Mignotte gradu
 * 7 ~2^-25); limes vitium in ansam lentam (minuta) non vertit */
#define LIMES_PROFUNDITATIS  4096      /* bisectiones VCA */
#define LIMES_BISECTIONUM    2048      /* angustatio per radicem */


/* ==================================================
 * Auxilia polynomiorum
 * ================================================== */

interior s32
_gradus (
    Polynomium p)
{
    redde polynomium_est_nullum(p) ? -(s32)I : polynomium_gradus_summus(p);
}

/* p / contentum (signum servatur) */
interior Polynomium
_primitiva (
    Polynomium  p,
       Piscina* piscina)
{
    Magnus g;

    si (polynomium_est_nullum(p))
    {
        redde p;
    }
    g = polynomium_contentum(p, piscina);
    si (magnus_compara(g, magnus_ex_s64(I)) != ZEPHYRUM)
    {
        (vacuum)polynomium_divide_exacte(p, polynomium_constans(g,
            piscina),
            piscina, &p);
    }
    redde p;
}

/* t^n p(1/t), n = gradus */
interior Polynomium
_reversum (
    Polynomium  p,
       Piscina* piscina)
{
    Polynomium r = polynomium_inversum(p, piscina);
    Polynomium e = polynomium_nullum();

    (vacuum)polynomium_translata(r, polynomium_gradus_summus(p),
        piscina, &e);
    redde e;
}

/* a_i -> a_i m^i, id est p(m t); inversa: a_i -> a_i m^(n - i), id est
 * m^n p(t/m) */
interior Polynomium
_scala_coefficientium (
    Polynomium  p,
        Magnus  m,
           b32  inversa,
       Piscina* piscina)
{
           s32  n = _gradus(p);
        Magnus* potentiae;
        Magnus* c;
    Polynomium  e = polynomium_nullum();
           s32  i;

    si (n < ZEPHYRUM)
    {
        redde p;
    }
    potentiae = (Magnus*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(Magnus));
    c = (Magnus*)piscina_allocare(piscina, (memoriae_index)(n + I)
        * magnitudo(Magnus));
    potentiae[ZEPHYRUM] = magnus_ex_s64(I);
    per (i = I; i <= n; i++)
    {
        potentiae[i] = magnus_multiplica(potentiae[i - I], m, piscina);
    }
    per (i = ZEPHYRUM; i <= n; i++)
    {
        c[i] = magnus_multiplica(polynomium_coefficiens(p, i),
            potentiae[inversa ? n - i : i], piscina);
    }
    (vacuum)polynomium_ex_coefficientibus(c, (i32)(n + I), ZEPHYRUM,
        piscina, &e);
    redde e;
}

/* variationes signorum coefficientium (nulla omissa) */
interior s32
_variationes (
    Polynomium p)
{
    s32 n      = _gradus(p);
    s32 prior  = ZEPHYRUM;
    s32 v      = ZEPHYRUM;
    s32 i;

    per (i = ZEPHYRUM; i <= n; i++)
    {
        s32 s = magnus_signum(polynomium_coefficiens(p, i));

        si (s != 0)
        {
            si (prior != 0 && s != prior)
            {
                v++;
            }
            prior = s;
        }
    }
    redde v;
}

/* limes Cartesii radicum Q in (0, 1): variationes (x+1)^n Q(1/(x+1)) */
interior b32
_variationes_unum (
    Polynomium  q,
       Piscina* piscina,
           s32* v)
{
    Polynomium t = polynomium_nullum();

    si (_gradus(q) < I)
    {
        *v = ZEPHYRUM;
        redde VERUM;
    }
    si (!polynomium_translatum(_reversum(q, piscina), magnus_ex_s64(I),
        piscina,
            &t))
    {
        redde FALSUM;
    }
    *v = _variationes(t);
    redde VERUM;
}

/* limes Cartesii radicum h in (l, r), l < r rationales */
interior b32
_variationes_in (
    Polynomium  h,
       Fractio  l,
       Fractio  r,
       Piscina* piscina,
           s32* v)
{
    Magnus d = magnus_multiplica(fractio_denominator(l),
        fractio_denominator(r), piscina);
    Fractio df = fractio_ex_magno(d);
     Magnus lz = fractio_numerator(fractio_multiplica(l, df,
         piscina));
     Magnus w = fractio_numerator(fractio_multiplica(
         fractio_subtrahe(r, l, piscina), df, piscina));
    Polynomium p = polynomium_nullum();

    /* d^n h(y/d) integrum; y = l d + w x, x in (0, 1) */
    si (!polynomium_translatum(
        _scala_coefficientium(h, d, VERUM, piscina), lz, piscina, &p))
    {
        redde FALSUM;
    }
    p = _primitiva(_scala_coefficientium(p, w, FALSUM, piscina),
        piscina);
    redde _variationes_unum(p, piscina, v);
}

interior b32
_exponentes_ordinarii (
    Polynomium p)
{
    redde polynomium_est_nullum(p)
        || polynomium_gradus_imus(p) >= ZEPHYRUM;
}

/* signum f(p/q), q > 0: signum sum_i c_i p^i q^(d - i) - Horner
 * homogeneus in integris, sine divisore communi (Fractio in omni
 * multiplicatione divisorem communem quaerebat: XC centesimae temporis
 * angustationis, worklog 2026-10-10) */
interior b32
_signum_ad (
    Polynomium  f,
       Fractio  x,
       Piscina* piscina,
           s32* s)
{
       s32 d = _gradus(f);
    Magnus p = fractio_numerator(x);
    Magnus q = fractio_denominator(x);
    Magnus summa;
    Magnus potentia_q = magnus_ex_s64(I);
       s32 i;

    si (!_exponentes_ordinarii(f))
    {
        redde FALSUM;
    }
    si (d < ZEPHYRUM)
    {
        *s = ZEPHYRUM;
        redde VERUM;
    }
    summa = polynomium_coefficiens(f, d);
    per (i = d - I; i >= ZEPHYRUM; i--)
    {
        potentia_q  = magnus_multiplica(potentia_q, q, piscina);
        summa       = magnus_adde(magnus_multiplica(summa, p, piscina),
            magnus_multiplica(polynomium_coefficiens(f, i), potentia_q,
            piscina), piscina);
    }
    *s = magnus_signum(summa);
    redde VERUM;
}

/* signum f iuxta x: dextrum (latus > 0) aut sinistrum (latus < 0);
 * f liber quadratis */
interior b32
_signum_iuxta (
    Polynomium  f,
       Fractio  x,
           s32  latus,
       Piscina* piscina,
           s32* s)
{
    Polynomium d   = polynomium_nullum();
           s32 sd  = ZEPHYRUM;

    si (!_signum_ad(f, x, piscina, s))
    {
        redde FALSUM;
    }
    si (*s != 0)
    {
        redde VERUM;
    }
    si (   !polynomium_derivata(f, piscina, &d)
        || !_signum_ad(d, x, piscina, &sd))
    {
        redde FALSUM;
    }
    *s = latus > 0 ? sd : -sd;
    redde *s != 0;               /* radix multiplex: contractus fractus */
}

/* c 2^k / 2^h */
interior Fractio
_fractio_binaria (
     Magnus  c,
        s32  k,
        s32  h,
    Piscina* piscina)
{
    Fractio q = fractio_ex_s64(ZEPHYRUM);

    (vacuum)fractio_ex_magnis(magnus_multiplica(c, magnus_potentia(
        magnus_ex_s64(II), (i32)k, piscina), piscina), magnus_potentia(
        magnus_ex_s64(II), (i32)h, piscina), piscina, &q);
    redde q;
}


/* ==================================================
 * Isolatio (VCA)
 * ================================================== */

nomen structura {
        Polynomium q;
            Magnus c;
               s32 h;
               b32 exacta;         /* radix media: c/2^h exacte */
    PiscinaNotatio nota;           /* acervi, ante q et c huius operis */
} Opus;

/* piscina temporaria; NIHIL si memoria deest */
interior Piscina*
_piscina_temporaria (vacuum)
{
    redde piscina_generare_dynamicum("radices", (memoriae_index)65536);
}

/* apices piscinarum temporariarum (radices_interna.h): probationes
 * refectiones metiuntur, non piscinam vocantis solam */
interior memoriae_index _apex_acervi     = ZEPHYRUM;
interior memoriae_index _apex_officinae  = ZEPHYRUM;

vacuum
radicum_apices_purgare (vacuum)
{
    _apex_acervi     = ZEPHYRUM;
    _apex_officinae  = ZEPHYRUM;
}

memoriae_index
radicum_apex_acervi (vacuum)
{
    redde _apex_acervi;
}

memoriae_index
radicum_apex_officinae (vacuum)
{
    redde _apex_officinae;
}

/* piscinam destruere, apicem eius notare (NIHIL toleratur) */
interior vacuum
_piscina_destrue (
           Piscina* piscina,
    memoriae_index* apex)
{
    si (piscina != NIHIL)
    {
        si (piscina_summa_apex_usus(piscina) > *apex)
        {
            *apex = piscina_summa_apex_usus(piscina);
        }
        piscina_destruere(piscina);
    }
}

/* piscinae temporariae functionis publicae */
interior vacuum
_piscinae_destrue (
    Piscina* status,
    Piscina* opus)
{
    _piscina_destrue(status, &_apex_officinae);
    _piscina_destrue(opus, &_apex_officinae);
}

/* opus in acervum: nota ante q et c (ordo LIFO: opus extractum cum
 * omnibus post eum allocatis per notam suam liberatur) */
interior vacuum
_opus_pone (
          Opus* o,
    Polynomium  q,
        Magnus  c,
           s32  h,
           b32  exacta,
       Piscina* acervus_piscina)
{
    o->nota    = piscina_notare(acervus_piscina);
    o->q       = polynomium_transcribe(q, acervus_piscina);
    o->c       = magnus_transcribe(c, acervus_piscina);
    o->h       = h;
    o->exacta  = exacta;
}

/* radices positivae g (g(0) != 0, liber quadratis), ordine crescente;
 * negativae = VERUM: g iam speculatum, radices negantur (ordo a vocante
 * invertitur). Termini in piscina; polynomia acervi in acervus_piscina
 * (LIFO per notas), cetera in opus_piscina (post quodque opus refecta):
 * memoria = profunditas x polynomium, non summa omnium operum */
interior b32
_separa_intra (
     Polynomium  g,
     Polynomium  f,
            b32  negativae,
        Piscina* piscina,
        Piscina* acervus_piscina,
        Piscina* opus_piscina,
    RadixRealis* radices,
            i32* n)
{
              Opus* acervus;
               i32  capacitas;
               i32  altitudo;
               s32  k;
            Magnus  maximus = magnus_ex_s64(ZEPHYRUM);
               s32  i;
    PiscinaNotatio  basis;

    si (_gradus(g) < I)
    {
        redde VERUM;
    }
    /* acervus: profunditas ternis operibus per gradum */
    capacitas = (i32)(III * (LIMES_PROFUNDITATIS + II));
    acervus = (Opus*)piscina_allocare(acervus_piscina,
        (memoriae_index)capacitas * magnitudo(Opus));
    basis = piscina_notare(opus_piscina);
    per (i = ZEPHYRUM; i <= _gradus(g); i++)
    {
        Magnus a = magnus_absolutum(polynomium_coefficiens(g, i),
            opus_piscina);

        si (magnus_compara(a, maximus) > 0)
        {
            maximus = a;
        }
    }
    k = (s32)magnus_bitorum(maximus) + I;    /* 2^k > 2 max >= Cauchy */
    _opus_pone(&acervus[ZEPHYRUM], _primitiva(_scala_coefficientium(g,
        magnus_potentia(magnus_ex_s64(II), (i32)k, opus_piscina),
        FALSUM,
        opus_piscina), opus_piscina), magnus_ex_s64(ZEPHYRUM), ZEPHYRUM,
        FALSUM, acervus_piscina);
    piscina_reficere(opus_piscina, basis);
    altitudo = I;
    dum (altitudo > ZEPHYRUM)
    {
              Opus o = acervus[--altitudo];
               s32 v = ZEPHYRUM;
        Polynomium sinistrum;
        Polynomium dextrum  = polynomium_nullum();
            Magnus summa    = magnus_ex_s64(ZEPHYRUM);
            Magnus duplex_c;

        si (o.exacta)
        {
            Fractio m = _fractio_binaria(o.c, k, o.h, piscina);

            radices[*n].f = f;
            radices[*n].infra = negativae ? fractio_nega(m,
                piscina) : m;
            radices[*n].supra = radices[*n].infra;
            (*n)++;
            piscina_reficere(acervus_piscina, o.nota);
            perge;
        }
        si (!_variationes_unum(o.q, opus_piscina, &v))
        {
            redde FALSUM;
        }
        si (v == I)
        {
            Fractio a = _fractio_binaria(o.c, k, o.h, piscina);
            Fractio b = _fractio_binaria(magnus_adde(o.c,
                magnus_ex_s64(I), opus_piscina),
                k, o.h, piscina);

            radices[*n].f = f;
            radices[*n].infra = negativae ? fractio_nega(b,
                piscina) : a;
            radices[*n].supra = negativae ? fractio_nega(a,
                piscina) : b;
            (*n)++;
        }
        si (v <= I)
        {
            piscina_reficere(acervus_piscina, o.nota);
            piscina_reficere(opus_piscina, basis);
            perge;
        }
        si (o.h >= LIMES_PROFUNDITATIS || altitudo + III > capacitas)
        {
            redde FALSUM;
        }
        sinistrum = _primitiva(_scala_coefficientium(o.q,
            magnus_ex_s64(II),
            VERUM, opus_piscina), opus_piscina);
        si (!polynomium_translatum(sinistrum, magnus_ex_s64(I),
                opus_piscina, &dextrum))
        {
            redde FALSUM;
        }
        duplex_c = magnus_multiplica(o.c, magnus_ex_s64(II),
            opus_piscina);
        per (i = ZEPHYRUM; i <= _gradus(sinistrum); i++)
        {
            summa = magnus_adde(summa, polynomium_coefficiens(sinistrum,
                i),
                opus_piscina);
        }
        /* o.q mortuum (filii in opus_piscina); acervus: dextrum, [radix
         * media], sinistrum */
        piscina_reficere(acervus_piscina, o.nota);
        _opus_pone(&acervus[altitudo++], _primitiva(dextrum,
            opus_piscina),
            magnus_adde(duplex_c, magnus_ex_s64(I), opus_piscina), o.h
                + I,
            FALSUM, acervus_piscina);
        si (magnus_signum(summa) == 0)
        {
            _opus_pone(&acervus[altitudo++], polynomium_nullum(),
                magnus_adde(duplex_c, magnus_ex_s64(I), opus_piscina),
                o.h + I, VERUM, acervus_piscina);
        }
        _opus_pone(&acervus[altitudo++], sinistrum, duplex_c, o.h + I,
            FALSUM, acervus_piscina);
        piscina_reficere(opus_piscina, basis);
    }
    redde VERUM;
}

interior b32
_separa_positivas (
     Polynomium  g,
     Polynomium  f,
            b32  negativae,
        Piscina* piscina,
    RadixRealis* radices,
            i32* n)
{
     Piscina* acervus_piscina  = _piscina_temporaria();
     Piscina* opus_piscina     = _piscina_temporaria();
         b32  bene             = FALSUM;

    si (acervus_piscina != NIHIL && opus_piscina != NIHIL)
    {
        bene = _separa_intra(g, f, negativae, piscina, acervus_piscina,
            opus_piscina, radices, n);
    }
    _piscina_destrue(acervus_piscina, &_apex_acervi);
    _piscina_destrue(opus_piscina, &_apex_officinae);
    redde bene;
}

interior b32
_reales_intra (
     Polynomium   f,
        Piscina*  piscina,
        Piscina*  opus,
    RadixRealis** exitus,
            i32*  numerus)
{
     Polynomium  g = polynomium_nullum();
     Polynomium  g0;
    RadixRealis* r;
    RadixRealis* neg;
            i32  n      = ZEPHYRUM;
            i32  n_neg  = ZEPHYRUM;
            s32  gradus;

    si (   polynomium_est_nullum(f) || !_exponentes_ordinarii(f)
        || !polynomium_pars_libera(f, opus, &g))
    {
        redde FALSUM;
    }
    gradus = _gradus(g);
    r = (RadixRealis*)piscina_allocare(piscina, (memoriae_index)(gradus
        + I) * magnitudo(RadixRealis));
    neg = (RadixRealis*)piscina_allocare(opus,
        (memoriae_index)(gradus
        + I) * magnitudo(RadixRealis));
    g0 = g;
    si (   gradus                                             >= I
        && magnus_signum(polynomium_coefficiens(g, ZEPHYRUM)) == 0)
    {
        (vacuum)polynomium_translata(g, -(s32)I, opus, &g0);   /* g / t */
    }
    g = polynomium_transcribe(g, piscina);   /* f omnium radicum */
    si (!_separa_positivas(_scala_coefficientium(g0,
        magnus_ex_s64(-(s64)I),
            FALSUM, opus), g, VERUM, piscina, neg, &n_neg))
    {
        redde FALSUM;
    }
    {
        s32 j;

        per (j = (s32)n_neg - I; j >= ZEPHYRUM; j--)
        {
            r[n++] = neg[j];
        }
    }
    si (   gradus                                             >= I
        && magnus_signum(polynomium_coefficiens(g, ZEPHYRUM)) == 0)
    {
        r[n].f      = g;
        r[n].infra  = fractio_ex_s64(ZEPHYRUM);
        r[n].supra  = r[n].infra;
        n++;
    }
    si (!_separa_positivas(g0, g, FALSUM, piscina, r, &n))
    {
        redde FALSUM;
    }
    *exitus   = r;
    *numerus  = n;
    redde VERUM;
}

b32
radices_reales (
     Polynomium   f,
        Piscina*  piscina,
    RadixRealis** exitus,
            i32*  numerus)
{
     Piscina* opus = _piscina_temporaria();
         b32  bene;

    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    bene = _reales_intra(f, piscina, opus, exitus, numerus);
    _piscina_destrue(opus, &_apex_officinae);
    redde bene;
}

RadixRealis
radix_ex_fractione (
    Fractio  q,
    Piscina* piscina)
{
    RadixRealis r;
         Magnus c[II];

    c[ZEPHYRUM]  = magnus_nega(fractio_numerator(q), piscina);
    c[I]         = fractio_denominator(q);
    r.f          = polynomium_nullum();
    (vacuum)polynomium_ex_coefficientibus(c, II, ZEPHYRUM, piscina,
        &r.f);
    r.infra = q;
    r.supra = q;
    redde r;
}


/* ==================================================
 * Angustatio
 * ================================================== */

interior b32
_est_exacta (
    RadixRealis  a,
        Piscina* piscina)
{
    redde fractio_compara(a.infra, a.supra, piscina) == 0;
}

interior Fractio
_dimidium (
    Piscina* piscina)
{
    Fractio d = fractio_ex_s64(ZEPHYRUM);

    (vacuum)fractio_ex_s64_s64(I, II, piscina, &d);
    redde d;
}

/* bisectio una (f radicem unam in (infra, supra), termini non radices);
 * radix rationalis in medio -> punctum. Computatio in opus, terminus
 * novus in status: vocans opus post quamque iterationem reficit */
interior b32
_seca_medio (
    RadixRealis* a,
        Piscina* opus,
        Piscina* status)
{
    Fractio m;
        s32 sm = ZEPHYRUM;
        s32 sl = ZEPHYRUM;

    si (_est_exacta(*a, opus))
    {
        redde VERUM;
    }
    m = fractio_multiplica(fractio_adde(a->infra, a->supra, opus),
        _dimidium(opus), opus);
    si (   !_signum_ad(a->f, m, opus, &sm)
        || !_signum_iuxta(a->f, a->infra, I, opus, &sl))
    {
        redde FALSUM;
    }
    m = fractio_transcribe(m, status);
    si (sm == 0)
    {
        a->infra = m;
        a->supra = m;
    } alioquin si (sm == sl)
    {
        a->infra = m;
    } alioquin
    {
        a->supra = m;
    }
    redde VERUM;
}

/* a < b per intervalla disiuncta: -1 / +1, aliter 0 (incertum) */
interior s32
_ordo_intervallorum (
    RadixRealis  a,
    RadixRealis  b,
        Piscina* piscina)
{
    si (   fractio_compara(a.supra, b.infra, piscina) <= 0
        && !(_est_exacta(a, piscina) && _est_exacta(b, piscina)
        && fractio_compara(a.supra, b.infra, piscina) == 0))
    {
        redde -(s32)I;
    }
    si (   fractio_compara(b.supra, a.infra, piscina) <= 0
        && !(_est_exacta(a, piscina) && _est_exacta(b, piscina)
        && fractio_compara(b.supra, a.infra, piscina) == 0))
    {
        redde I;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Signum polynomii, comparatio
 * ================================================== */

interior b32
_signum_polynomii_intra (
     Polynomium  h,
    RadixRealis  a,
        Piscina* status,
        Piscina* opus,
            s32* exitus)
{
        Polynomium g = polynomium_nullum();
               s32 iteratio;
    PiscinaNotatio basis;

    si (_est_exacta(a, opus))
    {
        redde _signum_ad(h, a.infra, opus, exitus);
    }
    /* a radix gcd(f, h)? g | f liber quadratis: radicem summum unam in
     * intervallo aperto; adest sse signum g iuxta terminos intra mutatur */
    si (!polynomium_divisor_communis(a.f, h, opus, &g))
    {
        redde FALSUM;
    }
    si (_gradus(g) >= I)
    {
        s32 sl = ZEPHYRUM;
        s32 sr = ZEPHYRUM;

        si (   !_signum_iuxta(g, a.infra, I, opus, &sl)
            || !_signum_iuxta(g, a.supra, -(s32)I, opus, &sr))
        {
            redde FALSUM;
        }
        si (sl != sr)
        {
            *exitus = ZEPHYRUM;
            redde VERUM;
        }
    }
    /* h(a) != 0: angusta donec h nullam radicem in intervallo habet */
    basis = piscina_notare(opus);
    per (iteratio = ZEPHYRUM; iteratio < LIMES_BISECTIONUM; iteratio++)
    {
        s32 v = ZEPHYRUM;

        si (_est_exacta(a, opus))
        {
            redde _signum_ad(h, a.infra, opus, exitus);
        }
        si (!_variationes_in(h, a.infra, a.supra, opus, &v))
        {
            redde FALSUM;
        }
        si (v == ZEPHYRUM)
        {
            redde _signum_ad(h, fractio_multiplica(fractio_adde(a.infra,
                a.supra, opus), _dimidium(opus), opus), opus, exitus);
        }
        si (!_seca_medio(&a, opus, status))
        {
            redde FALSUM;
        }
        piscina_reficere(opus, basis);
    }
    redde FALSUM;
}

b32
radix_signum_polynomii (
     Polynomium  h,
    RadixRealis  a,
        Piscina* piscina,
            s32* exitus)
{
     Piscina* status;
     Piscina* opus;
         b32  bene = FALSUM;

    (vacuum)piscina;             /* exitus s32: nihil in piscina vocantis */
    si (!_exponentes_ordinarii(h))
    {
        redde FALSUM;
    }
    si (polynomium_est_nullum(h))
    {
        *exitus = ZEPHYRUM;
        redde VERUM;
    }
    status  = _piscina_temporaria();
    opus    = _piscina_temporaria();
    si (status != NIHIL && opus != NIHIL)
    {
        bene = _signum_polynomii_intra(h, a, status, opus, exitus);
    }
    _piscinae_destrue(status, opus);
    redde bene;
}

interior b32
_compara_intra (
    RadixRealis  a,
    RadixRealis  b,
        Piscina* status,
        Piscina* opus,
            s32* exitus)
{
               s32 s = ZEPHYRUM;
               s32 ordo;
               s32 iteratio;
    PiscinaNotatio basis;

    si (_est_exacta(a, opus) && _est_exacta(b, opus))
    {
        *exitus = fractio_compara(a.infra, b.infra, opus);
        redde VERUM;
    }
    ordo = _ordo_intervallorum(a, b, opus);
    si (ordo != 0)
    {
        *exitus = ordo;
        redde VERUM;
    }
    /* a radix f_b? */
    si (_est_exacta(a, opus))
    {
        si (!_compara_intra(b, a, status, opus, &s))
        {
            redde FALSUM;
        }
        *exitus = -s;
        redde VERUM;
    }
    /* b = q exacta: signum(a - q) = signum (den t - num) in a. Non b.f -
     * punctum medium in radicem incidens f plenum servat, et b.f(a) = 0
     * pro QUAVIS radice b.f */
    si (_est_exacta(b, opus))
    {
        redde radix_signum_polynomii(radix_ex_fractione(b.infra,
            opus).f, a, opus, exitus);
    }
    si (!radix_signum_polynomii(b.f, a, opus, &s))
    {
        redde FALSUM;
    }
    basis = piscina_notare(opus);
    per (iteratio = ZEPHYRUM; iteratio
        < II * LIMES_BISECTIONUM; iteratio++)
    {
        ordo = _ordo_intervallorum(a, b, opus);
        si (ordo != 0)
        {
            *exitus = ordo;
            redde VERUM;
        }
        si (s == 0)
        {
            /* a radix f_b: a == b sse a in intervallo b (ibi radix f_b
             * una); b hic numquam exacta (supra), sola a secatur */
            si (   fractio_compara(b.infra, a.infra, opus) <= 0
                && fractio_compara(a.supra, b.supra, opus) <= 0)
            {
                *exitus = ZEPHYRUM;
                redde VERUM;
            }
            si (!_seca_medio(&a, opus, status))
            {
                redde FALSUM;
            }
        } alioquin
        {
            si (   !_seca_medio(&a, opus, status)
                || !_seca_medio(&b, opus, status))
            {
                redde FALSUM;
            }
        }
        piscina_reficere(opus, basis);
    }
    redde FALSUM;
}

b32
radix_compara (
    RadixRealis  a,
    RadixRealis  b,
        Piscina* piscina,
            s32* exitus)
{
     Piscina* status  = _piscina_temporaria();
     Piscina* opus    = _piscina_temporaria();
         b32  bene    = FALSUM;

    (vacuum)piscina;             /* exitus s32: nihil in piscina vocantis */
    si (status != NIHIL && opus != NIHIL)
    {
        bene = _compara_intra(a, b, status, opus, exitus);
    }
    _piscinae_destrue(status, opus);
    redde bene;
}


/* ==================================================
 * Angustatio publica, decimalis
 * ================================================== */

interior b32
_angusta_intra (
    RadixRealis  a,
        Fractio  latitudo,
        Piscina* status,
        Piscina* opus,
    RadixRealis* exitus)
{
               s32 iteratio;
    PiscinaNotatio basis = piscina_notare(opus);

    per (iteratio = ZEPHYRUM; iteratio < LIMES_BISECTIONUM; iteratio++)
    {
        si (   _est_exacta(a, opus) || fractio_compara(fractio_subtrahe(
                a.supra, a.infra, opus), latitudo, opus) < 0)
        {
            *exitus = a;
            redde VERUM;
        }
        si (!_seca_medio(&a, opus, status))
        {
            redde FALSUM;
        }
        piscina_reficere(opus, basis);
    }
    redde FALSUM;
}

b32
radix_angusta (
    RadixRealis  a,
        Fractio  latitudo,
        Piscina* piscina,
    RadixRealis* exitus)
{
        Piscina* status;
        Piscina* opus;
    RadixRealis  e;
            b32  bene = FALSUM;

    si (fractio_signum(latitudo) <= 0)
    {
        redde FALSUM;
    }
    status  = _piscina_temporaria();
    opus    = _piscina_temporaria();
    si (   status != NIHIL && opus != NIHIL
        && _angusta_intra(a, latitudo, status, opus, &e))
    {
        exitus->f      = a.f;
        exitus->infra  = fractio_transcribe(e.infra, piscina);
        exitus->supra  = fractio_transcribe(e.supra, piscina);
        bene           = VERUM;
    }
    _piscinae_destrue(status, opus);
    redde bene;
}

/* m (>= 0) / 10^d ut textus "i.dddd" */
interior chorda
_decimalis (
     Magnus  m,
        i32  digiti,
        b32  negativa,
    Piscina* piscina)
{
    chorda c = magnus_ad_chordam(m, piscina);
       i32 integra = c.mensura > digiti ? c.mensura
           - digiti : ZEPHYRUM;
       i32 longitudo = (integra > ZEPHYRUM ? integra : I)
           + (digiti > ZEPHYRUM ? I + digiti : ZEPHYRUM)
           + (negativa ? I : ZEPHYRUM);
    i8* datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)longitudo);
       i32 j = ZEPHYRUM;
       i32 i;
    chorda e;

    si (negativa)
    {
        datum[j++] = (i8)'-';
    }
    si (integra == ZEPHYRUM)
    {
        datum[j++] = (i8)'0';
    }
    per (i = ZEPHYRUM; i < integra; i++)
    {
        datum[j++] = c.datum[i];
    }
    si (digiti > ZEPHYRUM)
    {
        datum[j++] = (i8)'.';
    }
    per (i = ZEPHYRUM; i < digiti; i++)
    {
        /* digiti post punctum: zephyra praefixa si m < 10^(d-1) */
        s32 positio = (s32)c.mensura - (s32)digiti + (s32)i;

        datum[j++] = positio >= ZEPHYRUM ? c.datum[positio] : (i8)'0';
    }
    e.datum    = datum;
    e.mensura  = j;
    redde e;
}

interior b32
_ad_chordam_intra (
    RadixRealis  a,
            i32  digiti,
        Piscina* piscina,
        Piscina* status,
        Piscina* opus,
         chorda* exitus)
{
           Fractio scala =
               fractio_ex_magno(magnus_potentia(magnus_ex_s64(X),
               digiti, status));
               b32 negativa = FALSUM;
               s32 iteratio;
    PiscinaNotatio basis = piscina_notare(opus);

    /* signum notum facere: intervallum non 0 transiens */
    per (iteratio = ZEPHYRUM; !_est_exacta(a, opus)
        && fractio_signum(a.infra) < 0 && fractio_signum(a.supra) > 0;
        iteratio++)
    {
        si (   iteratio >= LIMES_BISECTIONUM
            || !_seca_medio(&a, opus, status))
        {
            redde FALSUM;
        }
        piscina_reficere(opus, basis);
    }
    si (fractio_signum(a.supra) <= 0 && !(fractio_signum(a.supra) == 0
        && fractio_signum(a.infra) == 0))
    {
        /* a < 0: -a radix f(-t) in (-supra, -infra) */
        RadixRealis b;

        negativa = VERUM;
        b.f = _scala_coefficientium(a.f, magnus_ex_s64(-(s64)I), FALSUM,
            status);
        b.infra  = fractio_nega(a.supra, status);
        b.supra  = fractio_nega(a.infra, status);
        a        = b;
    }
    per (iteratio = ZEPHYRUM; iteratio < LIMES_BISECTIONUM; iteratio++)
    {
        Magnus m = fractio_pavimentum(fractio_multiplica(a.infra, scala,
            opus), opus);

        si (   _est_exacta(a, opus)
            || fractio_compara(fractio_multiplica(a.supra, scala,
            opus),
            fractio_ex_magno(magnus_adde(m, magnus_ex_s64(I), opus)),
            opus) <= 0)
        {
            *exitus = _decimalis(m, digiti, negativa, piscina);
            redde VERUM;
        }
        /* radix ipsa (m + 1) / 10^d? tunc supra numquam infra eam
         * descendit (bisectio dyadica 1/5 numquam attingit) */
        {
            Fractio c   = fractio_ex_s64(ZEPHYRUM);
                s32 sc  = ZEPHYRUM;

            si (   !fractio_ex_magnis(magnus_adde(m, magnus_ex_s64(I),
                    opus), fractio_numerator(scala), opus, &c)
                || !_signum_ad(a.f, c, opus, &sc))
            {
                redde FALSUM;
            }
            si (   sc == 0
                && fractio_compara(a.infra, c, opus) < 0
                && fractio_compara(c, a.supra, opus) < 0)
            {
                a.infra = fractio_transcribe(c, status);
                a.supra = a.infra;
                piscina_reficere(opus, basis);
                perge;
            }
        }
        si (!_seca_medio(&a, opus, status))
        {
            redde FALSUM;
        }
        piscina_reficere(opus, basis);
    }
    redde FALSUM;
}

b32
radix_ad_chordam (
    RadixRealis  a,
            i32  digiti,
        Piscina* piscina,
         chorda* exitus)
{
     Piscina* status  = _piscina_temporaria();
     Piscina* opus    = _piscina_temporaria();
         b32  bene    = FALSUM;

    si (status != NIHIL && opus != NIHIL)
    {
        bene = _ad_chordam_intra(a, digiti, piscina, status, opus,
            exitus);
    }
    _piscinae_destrue(status, opus);
    redde bene;
}
#undef LIMES_BISECTIONUM
#undef LIMES_PROFUNDITATIS
#undef Opus
#undef _ad_chordam_intra
#undef _angusta_intra
#undef _apex_acervi
#undef _apex_officinae
#undef _compara_intra
#undef _decimalis
#undef _dimidium
#undef _est_exacta
#undef _exponentes_ordinarii
#undef _fractio_binaria
#undef _gradus
#undef _opus_pone
#undef _ordo_intervallorum
#undef _piscina_destrue
#undef _piscina_temporaria
#undef _piscinae_destrue
#undef _primitiva
#undef _reales_intra
#undef _reversum
#undef _scala_coefficientium
#undef _seca_medio
#undef _separa_intra
#undef _separa_positivas
#undef _signum_ad
#undef _signum_iuxta
#undef _signum_polynomii_intra
#undef _variationes
#undef _variationes_in
#undef _variationes_unum
/* lib/cyclotomia.c: statica per plagulam renominata */
#define _ad_f64 _ad_f64_cyclotomia
#define _addere_potentiam _addere_potentiam_cyclotomia
#define _an_ad_chordam _an_ad_chordam_cyclotomia
#define _an_adde _an_adde_cyclotomia
#define _an_aequalis _an_aequalis_cyclotomia
#define _an_bonum _an_bonum_cyclotomia
#define _an_divide_exacte _an_divide_exacte_cyclotomia
#define _an_est_nullum _an_est_nullum_cyclotomia
#define _an_ex_chorda _an_ex_chorda_cyclotomia
#define _an_multiplica _an_multiplica_cyclotomia
#define _an_nullum _an_nullum_cyclotomia
#define _an_parvum _an_parvum_cyclotomia
#define _an_subtrahe _an_subtrahe_cyclotomia
#define _an_transcribe _an_transcribe_cyclotomia
#define _an_unum _an_unum_cyclotomia
#define _communis _communis_cyclotomia
#define _conjugata_producta _conjugata_producta_cyclotomia
#define _divisor_communis _divisor_communis_cyclotomia
#define _elementum _elementum_cyclotomia
#define _invalidum _invalidum_cyclotomia
#define _reducere _reducere_cyclotomia
#define _t_d_minus_unum _t_d_minus_unum_cyclotomia
#line 1 "lib/cyclotomia.c"
/* cyclotomia.c - Integri cyclotomici exacti (vide include/cyclotomia.h)
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

structura Cyclotomia {
             i32  n;
             i32  phi;
      Polynomium  phi_n;          /* Phi_n, monicum, gradus phi */
      Polynomium* potentiae;      /* zeta^k reducta, k = 0..n-1 */
      /* -zeta^k: est_radix sine piscina */
      Polynomium* potentiae_negatae;
             i32* unitates;       /* j in [1, n], gcd(j, n) = 1 */
             i32  numerus_unitatum;
          Anulus  anulus;
};

interior i32
_divisor_communis (
    i32 a,
    i32 b)
{
    dum (b != ZEPHYRUM)
    {
        i32 r = a % b;

        a = b;
        b = r;
    }
    redde a;
}

/* t^d - 1 */
interior b32
_t_d_minus_unum (
           i32  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium monomium = polynomium_nullum();

    si (!polynomium_monomium(magnus_ex_s64(I), (s32)d, piscina,
        &monomium))
    {
        redde FALSUM;
    }
    *exitus = polynomium_subtrahe(monomium, polynomium_constans(
        magnus_ex_s64(I), piscina), piscina);
    redde VERUM;
}

b32
polynomium_cyclotomicum (
           i32  n,
       Piscina* piscina,
    Polynomium* exitus)
{
           i32 divisores[CC];
    Polynomium phi[CC];
           i32 numerus = ZEPHYRUM;
           i32 a;
           i32 b;

    si (n == ZEPHYRUM || n > CYCLOTOMIA_ORDO_MAXIMUS)
    {
        redde FALSUM;
    }
    per (a = I; a <= n; a++)
    {
        si (n % a == ZEPHYRUM)
        {
            divisores[numerus++] = a;
        }
    }
    /* ordine crescente: Phi_d = (t^d - 1) / prod_{e | d, e < d}
     * Phi_e */
    per (a = ZEPHYRUM; a < numerus; a++)
    {
        Polynomium q = polynomium_nullum();

        si (!_t_d_minus_unum(divisores[a], piscina, &q))
        {
            redde FALSUM;
        }
        per (b = ZEPHYRUM; b < a; b++)
        {
            si (   divisores[a] % divisores[b] == ZEPHYRUM
                && !polynomium_divide_exacte(q, phi[b], piscina, &q))
            {
                redde FALSUM;
            }
        }
        phi[a] = q;
    }
    *exitus = phi[numerus - I];
    redde VERUM;
}

/* summa c * potentiae[m] in alveum (phi coefficientes) */
interior vacuum
_addere_potentiam (
    constans Cyclotomia* r,
                 Magnus* alveus,
                 Magnus  c,
                    i32  m,
                Piscina* piscina)
{
    Polynomium pm = r->potentiae[m];
           s32 j;

    per (j = polynomium_gradus_imus(pm); !polynomium_est_nullum(pm)
        && j <= polynomium_gradus_summus(pm); j++)
    {
        Magnus d = polynomium_coefficiens(pm, j);

        si (magnus_signum(d) != ZEPHYRUM)
        {
            alveus[j] = magnus_adde(alveus[j], magnus_multiplica(c, d,
                piscina), piscina);
        }
    }
}

/* elementum signatum */
interior Cyclotomicus
_elementum (
    constans Cyclotomia* r,
             Polynomium  p)
{
    Cyclotomicus a;

    a.anulus  = r;
    a.p       = p;
    redde a;
}

interior Cyclotomicus
_invalidum (vacuum)
{
    redde _elementum(NIHIL, polynomium_nullum());
}

/* anulus communis duorum; NIHIL si mixti aut invalidi */
interior constans Cyclotomia*
_communis (
    Cyclotomicus a,
    Cyclotomicus b)
{
    redde a.anulus == b.anulus ? a.anulus : NIHIL;
}

/* p(zeta^k) reductum: terminus c t^e -> c * zeta^((k e) mod n) */
interior Cyclotomicus
_reducere (
    constans Cyclotomia* r,
             Polynomium  p,
                    s32  k,
                Piscina* piscina)
{
    Polynomium  exitus = polynomium_nullum();
        Magnus* alveus;
           s32  e;
           i32  j;

    si (polynomium_est_nullum(p))
    {
        redde _elementum(r, exitus);
    }
    alveus = (Magnus*)piscina_allocare(piscina, (memoriae_index)r->phi
        * magnitudo(Magnus));
    per (j = ZEPHYRUM; j < r->phi; j++)
    {
        alveus[j] = magnus_ex_s64(ZEPHYRUM);
    }
    per (e = polynomium_gradus_imus(p); e
        <= polynomium_gradus_summus(p);
        e++)
    {
        Magnus c = polynomium_coefficiens(p, e);
           s64 m;

        si (magnus_signum(c) == ZEPHYRUM)
        {
            perge;
        }
        m = ((s64)k * (s64)e) % (s64)r->n;
        si (m < ZEPHYRUM)
        {
            m = m + (s64)r->n;
        }
        _addere_potentiam(r, alveus, c, (i32)m, piscina);
    }
    (vacuum)polynomium_ex_coefficientibus(alveus, r->phi, ZEPHYRUM,
        piscina,
        &exitus);
    redde _elementum(r, exitus);
}


/* ==================================================
 * Anulus Z[zeta_n]: elementa Cyclotomicus eiusdem contextus
 * ================================================== */

#define CYCLO(anulus) ((constans Cyclotomia*)(anulus)->contextus)
#define ELEMENTUM(x) (*(constans Cyclotomicus*)(x))

interior vacuum
_an_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    *(Cyclotomicus*)exitus = cyclotomicus_nullum(CYCLO(anulus));
}

interior vacuum
_an_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    *(Cyclotomicus*)exitus = cyclotomicus_integer(CYCLO(anulus),
        magnus_ex_s64(I), piscina);
}

interior b32
_an_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    redde ((constans Cyclotomicus*)a)->anulus == CYCLO(anulus)
        && cyclotomicus_est_nullum(ELEMENTUM(a));
}

interior b32
_an_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    redde _an_est_nullum(anulus, a);
}

interior b32
_an_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    redde ((constans Cyclotomicus*)a)->anulus == CYCLO(anulus)
        && cyclotomicus_aequalis(ELEMENTUM(a), ELEMENTUM(b));
}

/* exitus validus et huius anuli? */
interior b32
_an_bonum (
    constans Anulus* anulus,
        Cyclotomicus c,
             vacuum* exitus)
{
    si (c.anulus != CYCLO(anulus))
    {
        redde FALSUM;
    }
    *(Cyclotomicus*)exitus = c;
    redde VERUM;
}

interior b32
_an_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    redde _an_bonum(anulus, cyclotomicus_adde(ELEMENTUM(a),
        ELEMENTUM(b),
        piscina), exitus);
}

interior b32
_an_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    redde _an_bonum(anulus, cyclotomicus_subtrahe(ELEMENTUM(a),
        ELEMENTUM(b), piscina), exitus);
}

interior b32
_an_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    redde _an_bonum(anulus, cyclotomicus_multiplica(ELEMENTUM(a),
        ELEMENTUM(b), piscina), exitus);
}

interior b32
_an_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    redde ((constans Cyclotomicus*)a)->anulus == CYCLO(anulus)
        && cyclotomicus_divide_exacte(ELEMENTUM(a), ELEMENTUM(b),
        piscina,
            (Cyclotomicus*)exitus);
}

interior vacuum
_an_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Cyclotomicus*)exitus =
        _elementum(((constans Cyclotomicus*)a)->anulus,
        polynomium_transcribe(((constans Cyclotomicus*)a)->p, piscina));
}

interior chorda
_an_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde cyclotomicus_ad_chordam(ELEMENTUM(a), piscina);
}

interior b32
_an_ex_chorda (
    constans Anulus* anulus,
              chorda  textus,
            Piscina*  piscina,
             vacuum*  exitus)
{
    redde cyclotomicus_ex_chorda(CYCLO(anulus), textus, piscina,
        (Cyclotomicus*)exitus);
}


/* ==================================================
 * Contextus
 * ================================================== */

Cyclotomia*
cyclotomia_creare (
         i32  n,
     Piscina* piscina)
{
     Cyclotomia* r;
         Magnus* alveus;
      character* titulus;
            i32  k;
            i32  j;

    si (n == ZEPHYRUM || n > CYCLOTOMIA_ORDO_MAXIMUS)
    {
        redde NIHIL;
    }
    r = (Cyclotomia*)piscina_allocare(piscina, magnitudo(Cyclotomia));
    r->n = n;
    si (!polynomium_cyclotomicum(n, piscina, &r->phi_n))
    {
        redde NIHIL;
    }
    r->phi = (i32)polynomium_gradus_summus(r->phi_n);
    /* tabula potentiarum: zeta^k = t^k pro k < phi; deinde
     * t * zeta^(k-1) reductum per Phi_n monicum */
    r->potentiae = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n
        * magnitudo(Polynomium));
    alveus = (Magnus*)piscina_allocare(piscina, (memoriae_index)(r->phi
        + I) * magnitudo(Magnus));
    per (j = ZEPHYRUM; j <= r->phi; j++)
    {
        alveus[j] = magnus_ex_s64(ZEPHYRUM);
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (k == ZEPHYRUM)
        {
            alveus[ZEPHYRUM] = magnus_ex_s64(I);
        }
        alioquin
        {
            Magnus summus;

            /* translatio per t */
            per (j = r->phi; j > ZEPHYRUM; j--)
            {
                alveus[j] = alveus[j - I];
            }
            alveus[ZEPHYRUM]  = magnus_ex_s64(ZEPHYRUM);
            summus            = alveus[r->phi];
            si (magnus_signum(summus) != ZEPHYRUM)
            {
                per (j = ZEPHYRUM; j < r->phi; j++)
                {
                    alveus[j] = magnus_subtrahe(alveus[j],
                        magnus_multiplica(summus,
                        polynomium_coefficiens(
                        r->phi_n, (s32)j), piscina), piscina);
                }
                alveus[r->phi] = magnus_ex_s64(ZEPHYRUM);
            }
        }
        (vacuum)polynomium_ex_coefficientibus(alveus, r->phi, ZEPHYRUM,
            piscina, &r->potentiae[k]);
    }
    r->potentiae_negatae = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Polynomium));
    per (k = ZEPHYRUM; k < n; k++)
    {
        r->potentiae_negatae[k] = polynomium_nega(r->potentiae[k],
            piscina);
    }
    r->unitates = (i32*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(i32));
    r->numerus_unitatum = ZEPHYRUM;
    per (j = I; j <= n; j++)
    {
        si (_divisor_communis(j, n) == I)
        {
            r->unitates[r->numerus_unitatum++] = j;
        }
    }
    titulus = (character*)piscina_allocare(piscina,
        (memoriae_index)XXXII);
    sprintf(titulus, "Z[zeta_%u]", n);
    r->anulus.titulus             = titulus;
    r->anulus.mensura             = magnitudo(Cyclotomicus);
    r->anulus.corpus              = FALSUM;
    r->anulus.nullum              = _an_nullum;
    r->anulus.unum                = _an_unum;
    r->anulus.est_nullum          = _an_est_nullum;
    r->anulus.parvum              = _an_parvum;
    r->anulus.aequalis            = _an_aequalis;
    r->anulus.adde                = _an_adde;
    r->anulus.subtrahe            = _an_subtrahe;
    r->anulus.multiplica          = _an_multiplica;
    r->anulus.divide_exacte       = _an_divide_exacte;
    r->anulus.transcribe          = _an_transcribe;
    r->anulus.ad_chordam          = _an_ad_chordam;
    r->anulus.ex_chorda           = _an_ex_chorda;
    r->anulus.divisor_communis    = NIHIL;
    r->anulus.divide_cum_residuo  = NIHIL;
    r->anulus.compara_normam      = NIHIL;
    r->anulus.contextus           = r;
    r->anulus.integrum            = VERUM;
    r->anulus.signum              = NIHIL;
    redde r;
}

i32
cyclotomia_ordo (
    constans Cyclotomia* r)
{
    redde r->n;
}

i32
cyclotomia_gradus (
    constans Cyclotomia* r)
{
    redde r->phi;
}

Polynomium
cyclotomia_polynomium (
    constans Cyclotomia* r)
{
    redde r->phi_n;
}

constans Anulus*
cyclotomia_anulus (
    constans Cyclotomia* r)
{
    redde &r->anulus;
}


/* ==================================================
 * Constructio
 * ================================================== */

Cyclotomicus
cyclotomicus_nullum (
    constans Cyclotomia* r)
{
    redde _elementum(r, polynomium_nullum());
}

Cyclotomicus
cyclotomicus_integer (
    constans Cyclotomia* r,
                 Magnus  c,
                Piscina* piscina)
{
    redde _elementum(r, polynomium_constans(c, piscina));
}

Cyclotomicus
cyclotomicus_radix (
    constans Cyclotomia* r,
                    s32  k,
                Piscina* piscina)
{
    s64 m;

    (vacuum)piscina;
    si (r == NIHIL)
    {
        redde _invalidum();
    }
    m = (s64)k % (s64)r->n;
    si (m < ZEPHYRUM)
    {
        m = m + (s64)r->n;
    }
    redde _elementum(r, r->potentiae[(i32)m]);
}

b32
cyclotomicus_ex_polynomio (
    constans Cyclotomia* r,
             Polynomium  p,
                    s32  k,
                Piscina* piscina,
           Cyclotomicus* exitus)
{
    si (r == NIHIL)
    {
        redde FALSUM;
    }
    *exitus = _reducere(r, p, k, piscina);
    redde VERUM;
}

b32
cyclotomicus_ex_s64 (
    constans Cyclotomia* r,
           constans s64* c,
                    i32  numerus,
                Piscina* piscina,
           Cyclotomicus* exitus)
{
        Magnus* alveus;
           i32  k;
    Polynomium  p = polynomium_nullum();

    si (r == NIHIL)
    {
        redde FALSUM;
    }
    si (numerus == ZEPHYRUM)
    {
        *exitus = cyclotomicus_nullum(r);
        redde VERUM;
    }
    alveus = (Magnus*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(Magnus));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        alveus[k] = magnus_ex_s64(c[k]);
    }
    si (!polynomium_ex_coefficientibus(alveus, numerus, ZEPHYRUM,
        piscina,
            &p))
    {
        redde FALSUM;
    }
    *exitus = _reducere(r, p, I, piscina);
    redde VERUM;
}


/* ==================================================
 * Lectio
 * ================================================== */

b32
cyclotomicus_est_validum (
    Cyclotomicus a)
{
    redde a.anulus != NIHIL;
}

constans Cyclotomia*
cyclotomicus_anulus (
    Cyclotomicus a)
{
    redde a.anulus;
}

Magnus
cyclotomicus_coefficiens (
    Cyclotomicus a,
             i32 j)
{
    si (a.anulus == NIHIL)
    {
        redde magnus_ex_s64(ZEPHYRUM);
    }
    redde polynomium_coefficiens(a.p, (s32)j);
}

b32
cyclotomicus_ad_s64 (
    Cyclotomicus  a,
             s64* exitus)
{
    i32 j;

    si (a.anulus == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < a.anulus->phi; j++)
    {
        exitus[j] = ZEPHYRUM;
        si (!magnus_ad_s64(polynomium_coefficiens(a.p, (s32)j),
                &exitus[j]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Arithmetica
 * ================================================== */

b32
cyclotomicus_est_nullum (
    Cyclotomicus a)
{
    redde a.anulus != NIHIL && polynomium_est_nullum(a.p);
}

b32
cyclotomicus_aequalis (
    Cyclotomicus a,
    Cyclotomicus b)
{
    redde _communis(a, b) != NIHIL && polynomium_aequalis(a.p, b.p);
}

b32
cyclotomicus_est_integer (
    Cyclotomicus  a,
          Magnus* valor)
{
    si (a.anulus == NIHIL)
    {
        redde FALSUM;
    }
    si (polynomium_est_nullum(a.p))
    {
        si (valor != NIHIL)
        {
            *valor = magnus_ex_s64(ZEPHYRUM);
        }
        redde VERUM;
    }
    si (polynomium_gradus_summus(a.p) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (valor != NIHIL)
    {
        *valor = polynomium_coefficiens(a.p, ZEPHYRUM);
    }
    redde VERUM;
}

Cyclotomicus
cyclotomicus_adde (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina)
{
    constans Cyclotomia* r = _communis(a, b);

    si (r == NIHIL)
    {
        redde _invalidum();
    }
    redde _elementum(r, polynomium_adde(a.p, b.p, piscina));
}

Cyclotomicus
cyclotomicus_subtrahe (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina)
{
    constans Cyclotomia* r = _communis(a, b);

    si (r == NIHIL)
    {
        redde _invalidum();
    }
    redde _elementum(r, polynomium_subtrahe(a.p, b.p, piscina));
}

Cyclotomicus
cyclotomicus_nega (
    Cyclotomicus  a,
         Piscina* piscina)
{
    si (a.anulus == NIHIL)
    {
        redde _invalidum();
    }
    redde _elementum(a.anulus, polynomium_nega(a.p, piscina));
}

Cyclotomicus
cyclotomicus_multiplica (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina)
{
     constans Cyclotomia* r          = _communis(a, b);
              Polynomium  productum  = polynomium_nullum();

    si (r == NIHIL)
    {
        redde _invalidum();
    }
    /* gradus < 2 phi: exponentes semper intra fines */
    (vacuum)polynomium_multiplica(a.p, b.p, piscina, &productum);
    redde _reducere(r, productum, I, piscina);
}

Cyclotomicus
cyclotomicus_potentia (
    Cyclotomicus  a,
             i32  e,
         Piscina* piscina)
{
    Cyclotomicus summa;

    si (a.anulus == NIHIL)
    {
        redde _invalidum();
    }
    summa = cyclotomicus_integer(a.anulus, magnus_ex_s64(I), piscina);
    dum (e > ZEPHYRUM)
    {
        si (e & I)
        {
            summa = cyclotomicus_multiplica(summa, a, piscina);
        }
        e = e >> I;
        si (e > ZEPHYRUM)
        {
            a = cyclotomicus_multiplica(a, a, piscina);
        }
    }
    redde summa;
}

b32
cyclotomicus_automorphismus (
    Cyclotomicus  a,
             s32  j,
         Piscina* piscina,
    Cyclotomicus* exitus)
{
     constans Cyclotomia* r = a.anulus;
                     s64  m;

    si (r == NIHIL)
    {
        redde FALSUM;
    }
    m = (s64)j % (s64)r->n;
    si (m < ZEPHYRUM)
    {
        m = m + (s64)r->n;
    }
    si (r->n > I && _divisor_communis((i32)m, r->n) != I)
    {
        redde FALSUM;
    }
    *exitus = _reducere(r, a.p, j, piscina);
    redde VERUM;
}

Cyclotomicus
cyclotomicus_conjugatum (
    Cyclotomicus  a,
         Piscina* piscina)
{
    si (a.anulus == NIHIL)
    {
        redde _invalidum();
    }
    redde _reducere(a.anulus, a.p, -I, piscina);
}

/* prod_{j unitas, j != 1} sigma_j(a) */
interior Cyclotomicus
_conjugata_producta (
    Cyclotomicus  a,
         Piscina* piscina)
{
     constans Cyclotomia* r = a.anulus;
            Cyclotomicus  productum = cyclotomicus_integer(r,
                magnus_ex_s64(I), piscina);
                     i32 u;

    per (u = ZEPHYRUM; u < r->numerus_unitatum; u++)
    {
        si (r->unitates[u] % r->n == I % r->n)
        {
            perge;
        }
        productum = cyclotomicus_multiplica(productum, _reducere(r, a.p,
            (s32)r->unitates[u], piscina), piscina);
    }
    redde productum;
}

b32
cyclotomicus_norma (
     Cyclotomicus  a,
          Piscina* piscina,
           Magnus* exitus)
{
    si (a.anulus == NIHIL)
    {
        redde FALSUM;
    }
    redde cyclotomicus_est_integer(cyclotomicus_multiplica(a,
        _conjugata_producta(a, piscina), piscina), exitus);
}

b32
cyclotomicus_vestigium (
     Cyclotomicus  a,
          Piscina* piscina,
           Magnus* exitus)
{
     constans Cyclotomia* r = a.anulus;
            Cyclotomicus  summa;
                     i32  u;

    si (r == NIHIL)
    {
        redde FALSUM;
    }
    summa = cyclotomicus_nullum(r);
    per (u = ZEPHYRUM; u < r->numerus_unitatum; u++)
    {
        summa = cyclotomicus_adde(summa, _reducere(r, a.p,
            (s32)r->unitates[u], piscina), piscina);
    }
    redde cyclotomicus_est_integer(summa, exitus);
}

b32
cyclotomicus_divide_exacte (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina,
    Cyclotomicus* exitus)
{
     constans Cyclotomia* r = _communis(a, b);
            Cyclotomicus  c;
                  Magnus  n = magnus_ex_s64(ZEPHYRUM);
                  Magnus* alveus;
                     s32  e;

    si (r == NIHIL || cyclotomicus_est_nullum(b))
    {
        redde FALSUM;
    }
    /* productum conjugatorum semel: N(b) = b * productum (recensio:
     * olim bis computatum) */
    {
        Cyclotomicus productum = _conjugata_producta(b, piscina);

        (vacuum)cyclotomicus_est_integer(cyclotomicus_multiplica(b,
            productum, piscina), &n);
        c = cyclotomicus_multiplica(a, productum, piscina);
    }
    si (cyclotomicus_est_nullum(c))
    {
        *exitus = c;
        redde VERUM;
    }
    alveus = (Magnus*)piscina_allocare(piscina, (memoriae_index)r->phi
        * magnitudo(Magnus));
    per (e = ZEPHYRUM; e < (s32)r->phi; e++)
    {
        Magnus q;
        Magnus residuum;

        alveus[e] = magnus_ex_s64(ZEPHYRUM);
        si (   e < polynomium_gradus_imus(c.p)
            || e > polynomium_gradus_summus(c.p))
        {
            perge;
        }
        si (   !magnus_divide(polynomium_coefficiens(c.p, e), n,
            piscina,
                &q, &residuum)
            || magnus_signum(residuum) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        alveus[e] = q;
    }
    exitus->anulus = r;
    (vacuum)polynomium_ex_coefficientibus(alveus, r->phi, ZEPHYRUM,
        piscina,
        &exitus->p);
    redde VERUM;
}

Cyclotomicus
cyclotomicus_modulus_quadratus (
    Cyclotomicus  a,
         Piscina* piscina)
{
    redde cyclotomicus_multiplica(a, cyclotomicus_conjugatum(a,
        piscina),
        piscina);
}

b32
cyclotomicus_est_radix (
    Cyclotomicus  a,
             s32* signum,
             i32* k)
{
     constans Cyclotomia* r = a.anulus;
                     i32  m;

    si (r == NIHIL)
    {
        redde FALSUM;
    }
    per (m = ZEPHYRUM; m < r->n; m++)
    {
        si (polynomium_aequalis(a.p, r->potentiae[m]))
        {
            *signum  = I;
            *k       = m;
            redde VERUM;
        }
    }
    per (m = ZEPHYRUM; m < r->n; m++)
    {
        si (polynomium_aequalis(a.p, r->potentiae_negatae[m]))
        {
            *signum  = -I;
            *k       = m;
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Textus
 * ================================================== */

chorda
cyclotomicus_ad_chordam (
    Cyclotomicus  a,
         Piscina* piscina)
{
    si (a.anulus == NIHIL)
    {
        redde chorda_transcribere(chorda_ex_literis("invalidum",
            piscina),
            piscina);
    }
    redde polynomium_ad_chordam(a.p, 'z', piscina);
}

b32
cyclotomicus_ex_chorda (
    constans Cyclotomia* r,
                 chorda  textus,
                Piscina* piscina,
           Cyclotomicus* exitus)
{
    Polynomium p = polynomium_nullum();

    si (r == NIHIL || !polynomium_ex_chorda(textus, 'z', piscina, &p))
    {
        redde FALSUM;
    }
    *exitus = _reducere(r, p, I, piscina);
    redde VERUM;
}

/* magnus -> f64 (ad ostendendum solum) */
interior f64
_ad_f64 (
     Magnus  c,
    Piscina* piscina)
{
       s64 parvus = ZEPHYRUM;
    chorda textus;
 character alveus[CCLVI];
       i32 n;

    si (magnus_ad_s64(c, &parvus))
    {
        redde (f64)parvus;
    }
    /* textus decimalis longus: mantissa (XVII digiti) et exponens,
     * ne truncatio 10^300 in 10^254 vertat (recensio) */
    textus = magnus_ad_chordam(c, piscina);
    {
        i32 signum = textus.mensura > ZEPHYRUM && textus.datum[ZEPHYRUM]
            == '-' ? I : ZEPHYRUM;
        i32 digiti = (i32)textus.mensura - signum;
        i32 k;

        n = ZEPHYRUM;
        si (signum)
        {
            alveus[n++] = '-';
        }
        alveus[n++] = (character)textus.datum[signum];
        alveus[n++] = '.';
        per (k = I; k < digiti && k < XVII; k++)
        {
            alveus[n++] = (character)textus.datum[signum + k];
        }
        sprintf(alveus + n, "e%u", digiti - I);
    }
    redde strtod(alveus, NIHIL);
}

chorda
cyclotomicus_ad_ostendendum (
    Cyclotomicus  a,
             i32  digiti,
         Piscina* piscina)
{
          f64 re = 0.0;
          f64 im = 0.0;
          f64 limes;
          s32 e;
    /* |x| >= 10^15 per %e scribitur: longitudo finita (C89 snprintf
     * caret; olim %.*f numeri magni alveum CXXVIII excedebat) */
    character alveus[CCLVI];
    character pars_re[CXXVIII];
    character pars_im[CXXVIII];

    si (a.anulus == NIHIL)
    {
        redde chorda_transcribere(chorda_ex_literis("invalidum",
            piscina),
            piscina);
    }
    si (!polynomium_est_nullum(a.p))
    {
        per (e = polynomium_gradus_imus(a.p);
            e <= polynomium_gradus_summus(a.p); e++)
        {
            f64 c = _ad_f64(polynomium_coefficiens(a.p, e), piscina);
            f64 angulus = 2.0 * 3.14159265358979323846 * (f64)e
                / (f64)a.anulus->n;

            re = re + c * cos(angulus);
            im = im + c * sin(angulus);
        }
    }
    si (digiti > XV)
    {
        digiti = XV;
    }
    limes = 0.5 * pow(10.0, -(f64)digiti);
    si (fabs(re) < limes)
    {
        re = 0.0;
    }
    si (fabs(im) < limes)
    {
        im = 0.0;
    }
    sprintf(pars_re, fabs(re) >= 1e15 ? "%.*e" : "%.*f",
        (integer)digiti,
        re);
    sprintf(pars_im, fabs(im) >= 1e15 ? "%.*e" : "%.*f",
        (integer)digiti,
        fabs(im));
    sprintf(alveus, "%s %c %si", pars_re, im < 0.0 ? '-' : '+',
        pars_im);
    redde chorda_transcribere(chorda_ex_literis(alveus, piscina),
        piscina);
}
#undef CYCLO
#undef ELEMENTUM
#undef _ad_f64
#undef _addere_potentiam
#undef _an_ad_chordam
#undef _an_adde
#undef _an_aequalis
#undef _an_bonum
#undef _an_divide_exacte
#undef _an_est_nullum
#undef _an_ex_chorda
#undef _an_multiplica
#undef _an_nullum
#undef _an_parvum
#undef _an_subtrahe
#undef _an_transcribe
#undef _an_unum
#undef _communis
#undef _conjugata_producta
#undef _divisor_communis
#undef _elementum
#undef _invalidum
#undef _reducere
#undef _t_d_minus_unum
/* lib/plexus.c: statica per plagulam renominata */
#define ImaginesLiberae ImaginesLiberae_plexus
#define _ansam_contrahere _ansam_contrahere_plexus
#define _ansam_littera _ansam_littera_plexus
#define _ansam_transcribere _ansam_transcribere_plexus
#define _apex_proprius _apex_proprius_plexus
#define _apicem_notare _apicem_notare_plexus
#define _burau_littera _burau_littera_plexus
#define _burau_littera_integra _burau_littera_integra_plexus
#define _contractiones _contractiones_plexus
#define _contrahere _contrahere_plexus
#define _copia _copia_plexus
#define _imagines_computare _imagines_computare_plexus
#define _index _index_plexus
#define _inversum _inversum_plexus
#define _limes _limes_plexus
#define _littera_valida _littera_valida_plexus
#define _litterae _litterae_plexus
#define _litterae_ad_chordam _litterae_ad_chordam_plexus
#define _litterae_conari _litterae_conari_plexus
#define _negativa _negativa_plexus
#define _phi _phi_plexus
#define _positiva _positiva_plexus
#define _productum _productum_plexus
#define _propriam_destruere _propriam_destruere_plexus
#define _propriam_vacare _propriam_vacare_plexus
#define _reducere_in _reducere_in_plexus
#line 1 "lib/plexus.c"
/* plexus.c - Greges braidarum EXACTE (vide include/plexus.h)
 */


#include <stdio.h>
#include <string.h>

/* longitudo maxima verbi repraesentabilis */
#define PLEXUS_LONGITUDO_SUMMA ((s64)0x7FFFFFFFL)


/* ==================================================
 * Auxilia
 * ================================================== */

interior s32*
_litterae (
        i32  numerus,
    Piscina* piscina)
{
    /* numerus 0: alveus unius litterae, ne NIHIL reddatur */
    redde (s32*)piscina_allocare(piscina, (memoriae_index)(numerus
        > ZEPHYRUM ? numerus : I) * magnitudo(s32));
}

/* contractiones piscinarum propriarum (plexus_contractiones) */
hic_manens i64 _contractiones = ZEPHYRUM;

/* apex usus piscinarum propriarum (plexus_apex_proprius) */
hic_manens memoriae_index _apex_proprius = ZEPHYRUM;

/* omnis piscina propria per haec destruitur aut vacatur: apex eius
 * prius notatur */
interior vacuum
_apicem_notare (
    constans Piscina* piscina)
{
    memoriae_index apex = piscina_summa_apex_usus(piscina);

    si (apex > _apex_proprius)
    {
        _apex_proprius = apex;
    }
}

interior vacuum
_propriam_destruere (
    Piscina* piscina)
{
    _apicem_notare(piscina);
    piscina_destruere(piscina);
}

interior vacuum
_propriam_vacare (
    Piscina* piscina)
{
    _apicem_notare(piscina);
    piscina_vacare(piscina);
}

/* sicut _litterae, sed NIHIL si memoria deficit (piscinae propriae) */
interior s32*
_litterae_conari (
        i32  numerus,
    Piscina* piscina)
{
    redde (s32*)piscina_conari_allocare(piscina,
        (memoriae_index)(numerus
        > ZEPHYRUM ? numerus : I) * magnitudo(s32));
}

/* littera valida in B_fila: 1 <= |littera| < fila */
interior b32
_littera_valida (
    s32 littera,
    i32 fila)
{
    redde littera != ZEPHYRUM && littera < (s32)fila
        && littera > -(s32)fila;
}

/* index positionis (ab 0) litterae: |littera| - 1 */
interior i32
_index (
    s32 littera)
{
    redde (i32)(littera > ZEPHYRUM ? littera : -littera) - I;
}

/* limes vocantis intra longitudinem repraesentabilem */
interior s64
_limes (
    i64 limes)
{
    redde limes > (i64)PLEXUS_LONGITUDO_SUMMA ? PLEXUS_LONGITUDO_SUMMA
        : (s64)limes;
}


/* ==================================================
 * Verba
 * ================================================== */

b32
plexus_creare (
                i32  fila,
     constans s32*   litterae,
                i32  numerus,
            Piscina* piscina,
             Plexus* exitus)
{
    Plexus p;
       i32 k;

    si (   fila < II || fila > PLEXUS_FILA_MAXIMA
        || (s64)numerus > PLEXUS_LONGITUDO_SUMMA
        || (numerus > ZEPHYRUM && litterae == NIHIL))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (!_littera_valida(litterae[k], fila))
        {
            redde FALSUM;
        }
    }
    p.fila       = fila;
    p.longitudo  = numerus;
    p.litterae   = _litterae(numerus, piscina);
    si (numerus > ZEPHYRUM)
    {
        memcpy(p.litterae, litterae, (size_t)numerus * magnitudo(s32));
    }
    *exitus = p;
    redde VERUM;
}

b32
plexus_ex_chorda (
         i32  fila,
      chorda  textus,
     Piscina* piscina,
      Plexus* exitus)
{
    PiscinaNotatio  nota;
               s32* litterae;
               i32  numerus  = ZEPHYRUM;
               i32  k        = ZEPHYRUM;

    si (   fila < II || fila > PLEXUS_FILA_MAXIMA
        || (textus.mensura > ZEPHYRUM && textus.datum == NIHIL))
    {
        redde FALSUM;
    }
    nota = piscina_notare(piscina);
    /* litterae non plures quam (mensura + 1) / 2 */
    litterae = _litterae(textus.mensura / II + I, piscina);
    dum (k < textus.mensura)
    {
        s32 signum  = I;
        s64 valor   = ZEPHYRUM;
        i32 cifrae  = ZEPHYRUM;

        si (textus.datum[k] == ' ')
        {
            k++;
            perge;
        }
        si (textus.datum[k] == '-')
        {
            signum = -I;
            k++;
        }
        dum (   k < textus.mensura && textus.datum[k] >= '0'
             && textus.datum[k] <= '9')
        {
            valor = valor * X + (s64)(textus.datum[k] - '0');
            si (valor >= (s64)fila)
            {
                piscina_reficere(piscina, nota);
                redde FALSUM;
            }
            cifrae++;
            k++;
        }
        si (   cifrae == ZEPHYRUM || valor == ZEPHYRUM
            || (k < textus.mensura && textus.datum[k] != ' '))
        {
            piscina_reficere(piscina, nota);
            redde FALSUM;
        }
        litterae[numerus++] = signum * (s32)valor;
    }
    /* plexus in alveo analysis ipso: omnis littera iam probata (0 <
     * |valor| < fila) */
    exitus->fila       = fila;
    exitus->longitudo  = numerus;
    exitus->litterae   = litterae;
    redde VERUM;
}

interior chorda
_litterae_ad_chordam (
      constans s32* litterae,
               i32  numerus,
           Piscina* piscina)
{
    chorda  exitus;
        i8* alveus;
       i32  k;
       i32  positio = ZEPHYRUM;

    /* " -2147483647" ad summum XII characteres per litteram */
    alveus = (i8*)piscina_allocare(piscina,
        (memoriae_index)numerus * XII
        + I);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        positio += (i32)sprintf((character*)alveus + positio,
            k == ZEPHYRUM ? "%d" : " %d", (integer)litterae[k]);
    }
    exitus.mensura  = positio;
    exitus.datum    = alveus;
    redde exitus;
}

chorda
plexus_ad_chordam (
     Plexus  p,
    Piscina* piscina)
{
    redde _litterae_ad_chordam(p.litterae, p.longitudo, piscina);
}

chorda
verbum_liberum_ad_chordam (
    VerbumLiberum  w,
          Piscina* piscina)
{
    redde _litterae_ad_chordam(w.litterae, w.longitudo, piscina);
}

Plexus
plexus_inversum (
     Plexus  p,
    Piscina* piscina)
{
    Plexus q;
       i32 k;

    q.fila       = p.fila;
    q.longitudo  = p.longitudo;
    q.litterae   = _litterae(p.longitudo, piscina);
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        q.litterae[k] = -p.litterae[p.longitudo - I - k];
    }
    redde q;
}

b32
plexus_iungere (
     Plexus  p,
     Plexus  q,
    Piscina* piscina,
     Plexus* exitus)
{
    Plexus r;

    si (   p.fila != q.fila
        || (s64)p.longitudo + (s64)q.longitudo > PLEXUS_LONGITUDO_SUMMA)
    {
        redde FALSUM;
    }
    r.fila       = p.fila;
    r.longitudo  = p.longitudo + q.longitudo;
    r.litterae   = _litterae(r.longitudo, piscina);
    si (p.longitudo > ZEPHYRUM)
    {
        memcpy(r.litterae, p.litterae, (size_t)p.longitudo
            * magnitudo(s32));
    }
    si (q.longitudo > ZEPHYRUM)
    {
        memcpy(r.litterae + p.longitudo, q.litterae, (size_t)q.longitudo
            * magnitudo(s32));
    }
    *exitus = r;
    redde VERUM;
}

/* reductio libera per acervum: littera inversa summi eum delet;
 * redde altitudinem novam */
interior i32
_reducere_in (
      constans s32* fons,
               i32  numerus,
               s32* acervus,
               i32  altitudo)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (altitudo > ZEPHYRUM && acervus[altitudo - I] == -fons[k])
        {
            altitudo--;
        }
        alioquin
        {
            acervus[altitudo++] = fons[k];
        }
    }
    redde altitudo;
}

Plexus
plexus_reducere (
     Plexus  p,
    Piscina* piscina)
{
    Plexus q;

    q.fila      = p.fila;
    q.litterae  = _litterae(p.longitudo, piscina);
    q.longitudo  = _reducere_in(p.litterae, p.longitudo, q.litterae,
        ZEPHYRUM);
    redde q;
}

s64
plexus_summa_exponentium (
    Plexus p)
{
    s64 summa = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        summa += p.litterae[k] > ZEPHYRUM ? (s64)I : -(s64)I;
    }
    redde summa;
}

b32
plexus_validus (
    Plexus p)
{
    i32 k;

    si (   p.fila < II || p.fila > PLEXUS_FILA_MAXIMA
        || (s64)p.longitudo > PLEXUS_LONGITUDO_SUMMA
        || (p.longitudo > ZEPHYRUM && p.litterae == NIHIL))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        si (!_littera_valida(p.litterae[k], p.fila))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
plexus_permutatio (
    Plexus  p,
       i32* imago)
{
    i32 k;

    si (!plexus_validus(p))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < p.fila; k++)
    {
        imago[k] = k;
    }
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        i32 i           = _index(p.litterae[k]);
        i32 commutatum  = imago[i];

        imago[i]      = imago[i + I];
        imago[i + I]  = commutatum;
    }
    redde VERUM;
}

b32
plexus_dimidia_torsio (
         i32  fila,
     Piscina* piscina,
      Plexus* exitus)
{
    Plexus p;
       i32 gradus;
       i32 k;
       i32 positio = ZEPHYRUM;

    si (fila < II || fila > PLEXUS_FILA_DIMIDIAE_TORSIONIS)
    {
        redde FALSUM;
    }
    p.fila       = fila;
    p.longitudo  = (i32)(((i64)fila * (i64)(fila - I)) / II);
    p.litterae   = _litterae(p.longitudo, piscina);
    per (gradus = fila - I; gradus >= I; gradus--)
    {
        per (k = I; k <= gradus; k++)
        {
            p.litterae[positio++] = (s32)k;
        }
    }
    *exitus = p;
    redde VERUM;
}


/* ==================================================
 * Actio in gregem liberum
 * ================================================== */

/* imagines generatorum x_1 .. x_n (indice 0 .. n-1) in piscina propria;
 * summa = longitudo summa omnium */
nomen structura {
               i32  fila;
     VerbumLiberum* imagines;
               s64  summa;
           Piscina* piscina;
    memoriae_index  basis;      /* usus post contractionem ultimam */
} ImaginesLiberae;

/* a b c (c^-1 si c_inversum) libere reductum; FALSUM si longitudo ante
 * reductionem limitem superat */
interior b32
_productum (
    constans VerbumLiberum* a,
    constans VerbumLiberum* b,
    constans VerbumLiberum* c,
                       b32  c_inversum,
                       s64  limes,
                   Piscina* piscina,
             VerbumLiberum* exitus)
{
    s64 longitudo = (s64)a->longitudo + (s64)b->longitudo
        + (s64)c->longitudo;
    s32* acervus;
    i32  altitudo;
    s32  k;

    si (longitudo > limes)
    {
        redde FALSUM;
    }
    acervus   = _litterae_conari((i32)longitudo, piscina);
    si (acervus == NIHIL)
    {
        redde FALSUM;
    }
    altitudo  = _reducere_in(a->litterae, a->longitudo, acervus,
        ZEPHYRUM);
    altitudo  = _reducere_in(b->litterae, b->longitudo, acervus,
        altitudo);
    si (c_inversum)
    {
        per (k = (s32)c->longitudo - I; k >= ZEPHYRUM; k--)
        {
            s32 littera = -c->litterae[k];

            si (   altitudo > ZEPHYRUM
                && acervus[altitudo - I] == -littera)
            {
                altitudo--;
            }
            alioquin
            {
                acervus[altitudo++] = littera;
            }
        }
    }
    alioquin
    {
        altitudo = _reducere_in(c->litterae, c->longitudo, acervus,
            altitudo);
    }
    exitus->longitudo  = altitudo;
    exitus->litterae   = acervus;
    redde VERUM;
}

/* copia; conari: memoria deficiens -> FALSUM (piscinae propriae),
 * aliter fatalis (piscina vocantis) */
interior b32
_copia (
    VerbumLiberum  w,
              b32  conari,
          Piscina* piscina,
    VerbumLiberum* exitus)
{
    VerbumLiberum c;

    c.longitudo  = w.longitudo;
    c.litterae   = conari ? _litterae_conari(w.longitudo, piscina)
        : _litterae(w.longitudo, piscina);
    si (c.litterae == NIHIL)
    {
        redde FALSUM;
    }
    si (w.longitudo > ZEPHYRUM)
    {
        memcpy(c.litterae, w.litterae, (size_t)w.longitudo
            * magnitudo(s32));
    }
    *exitus = c;
    redde VERUM;
}

/* w^-1 in piscina propria; FALSUM si memoria deficit */
interior b32
_inversum (
    VerbumLiberum  w,
          Piscina* piscina,
    VerbumLiberum* exitus)
{
    VerbumLiberum c;
              i32 k;

    c.longitudo  = w.longitudo;
    c.litterae   = _litterae_conari(w.longitudo, piscina);
    si (c.litterae == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < w.longitudo; k++)
    {
        c.litterae[k] = -w.litterae[w.longitudo - I - k];
    }
    *exitus = c;
    redde VERUM;
}

/* omnes imagines in piscinam novam transferuntur, vetus destruitur:
 * memoria imaginibus vivis proportionalis, non historiae */
interior b32
_contrahere (
    ImaginesLiberae* m)
{
           Piscina* nova = piscina_generare_dynamicum("plexus_imagines",
               (memoriae_index)m->summa * magnitudo(s32) + 4096);
     VerbumLiberum* imagines;
               i32  k;

    si (nova == NIHIL)
    {
        redde FALSUM;
    }
    imagines = (VerbumLiberum*)piscina_conari_allocare(nova,
        (memoriae_index)m->fila * magnitudo(VerbumLiberum));
    si (imagines == NIHIL)
    {
        _propriam_destruere(nova);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < m->fila; k++)
    {
        si (!_copia(m->imagines[k], VERUM, nova, &imagines[k]))
        {
            _propriam_destruere(nova);
            redde FALSUM;
        }
    }
    _propriam_destruere(m->piscina);
    m->piscina   = nova;
    m->imagines  = imagines;
    m->basis     = piscina_summa_usus(nova);
    _contractiones++;
    redde VERUM;
}

/* phi_p in m; FALSUM si limes superatus aut memoria deficit.
 * m->piscina (si non NIHIL) a vocante destruenda. */
interior b32
_imagines_computare (
             Plexus  p,
                s64  limes,
    ImaginesLiberae* m)
{
    i32 k;

    m->fila      = p.fila;
    m->summa     = (s64)p.fila;
    m->piscina   = NIHIL;
    m->imagines  = NIHIL;
    m->basis     = ZEPHYRUM;
    si (m->summa > limes)
    {
        redde FALSUM;
    }
    m->piscina = piscina_generare_dynamicum("plexus_imagines", 4096);
    si (m->piscina == NIHIL)
    {
        redde FALSUM;
    }
    m->imagines = (VerbumLiberum*)piscina_conari_allocare(m->piscina,
        (memoriae_index)p.fila * magnitudo(VerbumLiberum));
    si (m->imagines == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < p.fila; k++)
    {
        m->imagines[k].longitudo  = I;
        m->imagines[k].litterae   = _litterae_conari(I, m->piscina);
        si (m->imagines[k].litterae == NIHIL)
        {
            redde FALSUM;
        }
        m->imagines[k].litterae[ZEPHYRUM] = (s32)(k + I);
    }
    m->basis = piscina_summa_usus(m->piscina);
    /* phi := phi o phi_a, littera per litteram a sinistra (W = imagines
     * ante litteram):
     *   sigma_i:    W_i' = W_i W_{i+1} W_i^-1,  W_{i+1}' = W_i
     *   sigma_i^-1: W_i' = W_{i+1},
     *               W_{i+1}' = W_{i+1}^-1 W_i W_{i+1} */
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
                  s32 littera  = p.litterae[k];
                  i32 i        = _index(littera);
        VerbumLiberum prima    = m->imagines[i];
        VerbumLiberum secunda  = m->imagines[i + I];
        VerbumLiberum nova;
                  s64 summa_nova;

        si (littera > ZEPHYRUM)
        {
            si (!_productum(&prima, &secunda, &prima, VERUM, limes,
                m->piscina,
                &nova))
            {
                redde FALSUM;
            }
            m->imagines[i]      = nova;
            m->imagines[i + I]  = prima;
        }
        alioquin
        {
            VerbumLiberum secunda_inversa;

            si (!_inversum(secunda, m->piscina, &secunda_inversa))
            {
                redde FALSUM;
            }
            si (!_productum(&secunda_inversa, &prima, &secunda, FALSUM,
                limes,
                m->piscina, &nova))
            {
                redde FALSUM;
            }
            m->imagines[i]      = secunda;
            m->imagines[i + I]  = nova;
        }
        summa_nova = m->summa - (s64)prima.longitudo
            - (s64)secunda.longitudo
            + (s64)m->imagines[i].longitudo
            + (s64)m->imagines[i + I].longitudo;
        si (summa_nova > limes)
        {
            redde FALSUM;
        }
        m->summa = summa_nova;
        /* purgamenta: contractio cum piscina quater crevit post
         * initium aut contractionem ultimam (onus fixum per filum in
         * basi: recensio M1, olim omni littera supra ~8192 fila) */
        si (piscina_summa_usus(m->piscina) > IV * m->basis + 65536)
        {
            si (!_contrahere(m))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

b32
plexus_imago_libera (
           Plexus  p,
              i32  j,
              i64  limes,
          Piscina* piscina,
    VerbumLiberum* exitus)
{
    ImaginesLiberae m;
                b32 bene;

    si (!plexus_validus(p) || j < I || j > p.fila)
    {
        redde FALSUM;
    }
    bene = _imagines_computare(p, _limes(limes), &m);
    si (bene)
    {
        (vacuum)_copia(m.imagines[j - I], FALSUM, piscina, exitus);
    }
    si (m.piscina != NIHIL)
    {
        _propriam_destruere(m.piscina);
    }
    redde bene;
}

b32
plexus_imagines_liberae (
            Plexus   p,
               i64   limes,
           Piscina*  piscina,
     VerbumLiberum** exitus)
{
    ImaginesLiberae m;
                b32 bene;
                i32 k;

    si (!plexus_validus(p))
    {
        redde FALSUM;
    }
    bene = _imagines_computare(p, _limes(limes), &m);
    si (bene)
    {
        VerbumLiberum* imagines = (VerbumLiberum*)piscina_allocare(
            piscina, (memoriae_index)p.fila * magnitudo(VerbumLiberum));

        per (k = ZEPHYRUM; k < p.fila; k++)
        {
            (vacuum)_copia(m.imagines[k], FALSUM, piscina,
                &imagines[k]);
        }
        *exitus = imagines;
    }
    si (m.piscina != NIHIL)
    {
        _propriam_destruere(m.piscina);
    }
    redde bene;
}

b32
plexus_aequales (
     Plexus  p,
     Plexus  q,
        i64  limes,
    Piscina* piscina,
        b32* aequales)
{
          PiscinaNotatio  nota;
                  Plexus  r;
         ImaginesLiberae  m;
                     i32* imago;
                     b32  bene = VERUM;
                     b32  idem = VERUM;
                     i32  k;

    si (p.fila != q.fila || !plexus_validus(p) || !plexus_validus(q))
    {
        redde FALSUM;
    }
    nota = piscina_notare(piscina);
    si (!plexus_iungere(p, plexus_inversum(q, piscina), piscina, &r))
    {
        piscina_reficere(piscina, nota);
        redde FALSUM;
    }
    r = plexus_reducere(r, piscina);
    /* r vacuum: aequales sine actione (recensio L2: etiam limite infra
     * fila) */
    si (r.longitudo == ZEPHYRUM)
    {
        piscina_reficere(piscina, nota);
        *aequales = VERUM;
        redde VERUM;
    }
    /* invariantia necessaria (exacta): summa exponentium, permutatio */
    si (plexus_summa_exponentium(r) != ZEPHYRUM)
    {
        idem = FALSUM;
    }
    alioquin
    {
        imago = (i32*)piscina_allocare(piscina, (memoriae_index)r.fila
            * magnitudo(i32));
        (vacuum)plexus_permutatio(r, imago);
        per (k = ZEPHYRUM; k < r.fila; k++)
        {
            si (imago[k] != k)
            {
                idem = FALSUM;
            }
        }
    }
    si (idem)
    {
        bene = _imagines_computare(r, _limes(limes), &m);
        si (bene)
        {
            per (k = ZEPHYRUM; k < r.fila; k++)
            {
                si (   m.imagines[k].longitudo          != I
                    || m.imagines[k].litterae[ZEPHYRUM] != (s32)(k + I))
                {
                    idem = FALSUM;
                }
            }
        }
        si (m.piscina != NIHIL)
        {
            _propriam_destruere(m.piscina);
        }
    }
    piscina_reficere(piscina, nota);
    si (bene)
    {
        *aequales = idem;
    }
    redde bene;
}

b32
plexus_in_maiore (
     Plexus  p,
        i32  fila,
        i32  translatio,
    Piscina* piscina,
     Plexus* exitus)
{
    Plexus q;
       i32 k;

    si (   !plexus_validus(p) || fila < II || fila > PLEXUS_FILA_MAXIMA
        || (i64)p.fila + (i64)translatio > (i64)fila)
    {
        redde FALSUM;
    }
    q.fila       = fila;
    q.longitudo  = p.longitudo;
    q.litterae   = _litterae(p.longitudo, piscina);
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        q.litterae[k] = p.litterae[k] > ZEPHYRUM
            ? p.litterae[k] + (s32)translatio
            : p.litterae[k] - (s32)translatio;
    }
    *exitus = q;
    redde VERUM;
}


/* ==================================================
 * Actio Dynnikov
 * ================================================== */

/* f+ = max(f, 0) */
interior Magnus
_positiva (
    Magnus f)
{
    redde magnus_signum(f) > ZEPHYRUM ? f : magnus_ex_s64(ZEPHYRUM);
}

/* f- = min(f, 0) */
interior Magnus
_negativa (
    Magnus f)
{
    redde magnus_signum(f) < ZEPHYRUM ? f : magnus_ex_s64(ZEPHYRUM);
}

b32
ansa_valida (
    Ansa u)
{
    redde u.fila >= III && u.fila <= PLEXUS_FILA_MAXIMA && u.a != NIHIL
        && u.b != NIHIL;
}

b32
ansa_ex_coordinatis (
                i32  fila,
    constans Magnus* a,
    constans Magnus* b,
            Piscina* piscina,
               Ansa* exitus)
{
    Ansa u;
     i32 k;

    si (   fila < III || fila > PLEXUS_FILA_MAXIMA || a == NIHIL
        || b == NIHIL)
    {
        redde FALSUM;
    }
    u.fila  = fila;
    u.a     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(fila
        - II) * magnitudo(Magnus));
    u.b     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(fila
        - II) * magnitudo(Magnus));
    per (k = ZEPHYRUM; k < fila - II; k++)
    {
        u.a[k] = magnus_transcribe(a[k], piscina);
        u.b[k] = magnus_transcribe(b[k], piscina);
    }
    *exitus = u;
    redde VERUM;
}

b32
ansa_circa (
         i32  fila,
         i32  k,
     Piscina* piscina,
        Ansa* exitus)
{
    Ansa u;
     i32 j;

    si (fila < III || fila > PLEXUS_FILA_MAXIMA || k < I || k >= fila)
    {
        redde FALSUM;
    }
    u.fila  = fila;
    u.a     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(fila
        - II) * magnitudo(Magnus));
    u.b     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(fila
        - II) * magnitudo(Magnus));
    per (j = ZEPHYRUM; j < fila - II; j++)
    {
        u.a[j] = magnus_ex_s64(ZEPHYRUM);
        u.b[j] = magnus_ex_s64(ZEPHYRUM);
    }
    /* nu_k = 2, cetera 0: b_i = (nu_i - nu_{i+1}) / 2 (aeq. 3) */
    si (k <= fila - II)
    {
        u.b[k - I] = magnus_ex_s64(I);
    }
    si (k >= II)
    {
        u.b[k - II] = magnus_ex_s64(-I);
    }
    *exitus = u;
    redde VERUM;
}

/* una littera in (a, b) loco, aeq. (8)-(15); indices fontis j (1 ..
 * n-2) -> a[j-1]. piscina pro valoribus magnis. */
interior vacuum
_ansam_littera (
        Magnus* a,
        Magnus* b,
           i32  fila,
           s32  littera,
       Piscina* piscina)
{
       i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera);
       i32 m = fila - II;    /* n - 2: index fontis ultimus */
    Magnus nb;
    Magnus na;

    si (i == I && littera > ZEPHYRUM)
    {
        /* (10): b1' = a1 + b1+, a1' = -b1 + (b1')+ */
        nb = magnus_adde(a[ZEPHYRUM], _positiva(b[ZEPHYRUM]), piscina);
        na = magnus_adde(magnus_nega(b[ZEPHYRUM], piscina),
            _positiva(nb), piscina);
        a[ZEPHYRUM] = na;
        b[ZEPHYRUM] = nb;
    }
    alioquin si (i == I)
    {
        /* (14): b1' = b1+ - a1, a1' = b1 - (b1')+ */
        nb = magnus_subtrahe(_positiva(b[ZEPHYRUM]), a[ZEPHYRUM],
            piscina);
        na = magnus_subtrahe(b[ZEPHYRUM], _positiva(nb), piscina);
        a[ZEPHYRUM] = na;
        b[ZEPHYRUM] = nb;
    }
    alioquin si (i == fila - I && littera > ZEPHYRUM)
    {
        /* (11): b' = a + b-, a' = -b + (b')- (index n-2) */
        nb = magnus_adde(a[m - I], _negativa(b[m - I]), piscina);
        na = magnus_adde(magnus_nega(b[m - I], piscina), _negativa(nb),
            piscina);
        a[m - I] = na;
        b[m - I] = nb;
    }
    alioquin si (i == fila - I)
    {
        /* (15): b' = b- - a, a' = b - (b')- */
        nb = magnus_subtrahe(_negativa(b[m - I]), a[m - I], piscina);
        na = magnus_subtrahe(b[m - I], _negativa(nb), piscina);
        a[m - I] = na;
        b[m - I] = nb;
    }
    alioquin
    {
        /* 2 <= i <= n-2: indices fontis i-1 (p) et i (q) */
        Magnus a_prior    = a[i - II];
        Magnus a_sequens  = a[i - I];
        Magnus b_prior    = b[i - II];
        Magnus b_sequens  = b[i - I];
        Magnus c;

        si (littera > ZEPHYRUM)
        {
            /* (8)-(9): c = a_p - a_q - b_q+ + b_p- */
            c = magnus_adde(magnus_subtrahe(magnus_subtrahe(a_prior,
                a_sequens,
                piscina), _positiva(b_sequens), piscina),
                _negativa(b_prior),
                piscina);
            a[i - II] = magnus_subtrahe(magnus_subtrahe(a_prior,
                _positiva(b_prior), piscina), _positiva(magnus_adde(
                _positiva(b_sequens), c, piscina)), piscina);
            b[i - II] = magnus_adde(b_sequens, _negativa(c), piscina);
            a[i - I] = magnus_subtrahe(magnus_subtrahe(a_sequens,
                _negativa(b_sequens), piscina),
                _negativa(magnus_subtrahe(
                _negativa(b_prior), c, piscina)), piscina);
            b[i - I] = magnus_subtrahe(b_prior, _negativa(c), piscina);
        }
        alioquin
        {
            /* (12)-(13): d = a_p - a_q + b_q+ - b_p- */
            c = magnus_subtrahe(magnus_adde(magnus_subtrahe(a_prior,
                a_sequens,
                piscina), _positiva(b_sequens), piscina),
                _negativa(b_prior),
                piscina);
            a[i - II] = magnus_adde(magnus_adde(a_prior,
                _positiva(b_prior),
                piscina),
                _positiva(magnus_subtrahe(_positiva(b_sequens), c,
                piscina)), piscina);
            b[i - II] = magnus_subtrahe(b_sequens, _positiva(c),
                piscina);
            a[i - I] = magnus_adde(magnus_adde(a_sequens,
                _negativa(b_sequens),
                piscina), _negativa(magnus_adde(_negativa(b_prior), c,
                piscina)), piscina);
            b[i - I] = magnus_adde(b_prior, _positiva(c), piscina);
        }
    }
}

/* coordinatae in piscinam novam transcribuntur, vetus destruitur */
interior b32
_ansam_contrahere (
      Magnus*  a,
      Magnus*  b,
         i32   numerus,
     Piscina** piscina)
{
     Piscina* nova = piscina_generare_dynamicum("plexus_ansa", 4096);
         i32  k;

    si (nova == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        a[k] = magnus_transcribe(a[k], nova);
        b[k] = magnus_transcribe(b[k], nova);
    }
    _propriam_destruere(*piscina);
    *piscina = nova;
    _contractiones++;
    redde VERUM;
}

b32
ansa_agere (
        Ansa  u,
      Plexus  p,
     Piscina* piscina,
        Ansa* exitus)
{
          Piscina* temporaria;
           Magnus* a;
           Magnus* b;
              i32  numerus = u.fila - II;
              i32  k;
             Ansa  v;
   memoriae_index  basis;
   memoriae_index  limen;

    si (!ansa_valida(u) || p.fila != u.fila || !plexus_validus(p))
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_ansa", 4096);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    /* coordinatae currentes in alveo vocantis (non temporario: alveus
     * contractionem superstes esse debet) */
    a = (Magnus*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(Magnus));
    b = (Magnus*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(Magnus));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        a[k] = u.a[k];
        b[k] = u.b[k];
    }
    basis = piscina_summa_usus(temporaria);
    limen = (memoriae_index)numerus * IV * magnitudo(Magnus);
    si (limen < 1048576UL)
    {
        limen = 1048576UL;
    }
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        _ansam_littera(a, b, u.fila, p.litterae[k], temporaria);
        /* purgamenta: inspectio omni littera (recensio T5 M2: olim
         * post max(numerus, 256) litteras - verbum fila brevius numquam
         * contrahebatur); contractio cum piscina quater crevit post
         * contractionem ultimam et max(1 MB, 4 numerus Magnus) superat:
         * transcriptio O(numerus) amortizata */
        {
            si (piscina_summa_usus(temporaria) > IV * basis + limen)
            {
                si (!_ansam_contrahere(a, b, numerus, &temporaria))
                {
                    _propriam_destruere(temporaria);
                    redde FALSUM;
                }
                basis = piscina_summa_usus(temporaria);
            }
        }
    }
    v.fila  = u.fila;
    v.a     = a;
    v.b     = b;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        v.a[k] = magnus_transcribe(a[k], piscina);
        v.b[k] = magnus_transcribe(b[k], piscina);
    }
    _propriam_destruere(temporaria);
    *exitus = v;
    redde VERUM;
}

Magnus
ansa_sectiones (
        Ansa  u,
     Piscina* piscina)
{
     Piscina* t;
         i32  m = u.fila - II;
         i32  k;
      Magnus  summa_b;
      Magnus  maximum;
      Magnus  b_prima;
      Magnus  b_ultima;
      Magnus  longitudo;

    /* ansa invalida: 0 (vide caput) */
    si (!ansa_valida(u))
    {
        redde magnus_ex_s64(ZEPHYRUM);
    }
    /* temporaria in piscina propria t (si deficit, in vocantis) */
    t = piscina_generare_dynamicum("plexus_sectiones", 4096);
    si (t == NIHIL)
    {
        t = piscina;
    }
    summa_b = magnus_ex_s64(ZEPHYRUM);
    maximum = magnus_ex_s64(ZEPHYRUM);
    /* b_0 = -max_i (|a_i| + b_i+ + sum_{j<i} b_j)  (aeq. 6) */
    per (k = ZEPHYRUM; k < m; k++)
    {
        Magnus terminus =
            magnus_adde(magnus_adde(magnus_absolutum(u.a[k],
            t), _positiva(u.b[k]), t), summa_b, t);

        si (   k == ZEPHYRUM
            || magnus_compara(terminus, maximum) > ZEPHYRUM)
        {
            maximum = terminus;
        }
        summa_b = magnus_adde(summa_b, u.b[k], t);
    }
    b_prima   = magnus_nega(maximum, t);
    b_ultima  = magnus_subtrahe(magnus_nega(b_prima, t), summa_b, t);
    /* L = |a_1| + |a_{n-2}| + sum |a_{i+1} - a_i| + sum_{0..n-1} |b_i|
     * (aeq. 5) */
    longitudo = magnus_adde(magnus_absolutum(u.a[ZEPHYRUM], t),
        magnus_absolutum(u.a[m - I], t), t);
    per (k = ZEPHYRUM; k + I < m; k++)
    {
        longitudo = magnus_adde(longitudo, magnus_absolutum(
            magnus_subtrahe(u.a[k + I], u.a[k], t), t), t);
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        longitudo = magnus_adde(longitudo, magnus_absolutum(u.b[k], t),
            t);
    }
    longitudo = magnus_adde(longitudo, magnus_absolutum(b_prima, t), t);
    longitudo = magnus_adde(longitudo, magnus_absolutum(b_ultima, t),
        t);
    si (t != piscina)
    {
        longitudo = magnus_transcribe(longitudo, piscina);
        _propriam_destruere(t);
    }
    redde longitudo;
}

b32
ansae_aequales (
    Ansa u,
    Ansa v)
{
    i32 k;

    si (u.fila != v.fila || !ansa_valida(u) || !ansa_valida(v))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + II < u.fila; k++)
    {
        si (   !magnus_aequalis(u.a[k], v.a[k])
            || !magnus_aequalis(u.b[k], v.b[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

chorda
ansa_ad_chordam (
        Ansa  u,
     Piscina* piscina)
{
     ChordaAedificator* ae;
                   i32  k;

    si (!ansa_valida(u))
    {
        redde chorda_ex_literis("", piscina);
    }
    ae = chorda_aedificator_creare(piscina, CXXVIII);
    (vacuum)chorda_aedificator_appendere_character(ae, '(');
    per (k = ZEPHYRUM; k + II < u.fila; k++)
    {
        si (k > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_character(ae, ' ');
        }
        (vacuum)chorda_aedificator_appendere_chorda(ae,
            magnus_ad_chordam(u.a[k], piscina));
    }
    (vacuum)chorda_aedificator_appendere_literis(ae, "; ");
    per (k = ZEPHYRUM; k + II < u.fila; k++)
    {
        si (k > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_character(ae, ' ');
        }
        (vacuum)chorda_aedificator_appendere_chorda(ae,
            magnus_ad_chordam(u.b[k], piscina));
    }
    (vacuum)chorda_aedificator_appendere_character(ae, ')');
    redde chorda_aedificator_finire(ae);
}

b32
plexus_aequales_ansis (
     Plexus  p,
     Plexus  q,
    Piscina* piscina,
        b32* aequales)
{
    PiscinaNotatio  nota;
            Plexus  r;
            Plexus  maior;
               i32* imago;
               i32  fila = p.fila + II;
               i32  k;
               b32  idem = VERUM;

    si (   p.fila != q.fila || fila > PLEXUS_FILA_MAXIMA
        || !plexus_validus(p) || !plexus_validus(q))
    {
        redde FALSUM;
    }
    nota = piscina_notare(piscina);
    si (!plexus_iungere(p, plexus_inversum(q, piscina), piscina, &r))
    {
        piscina_reficere(piscina, nota);
        redde FALSUM;
    }
    r = plexus_reducere(r, piscina);
    /* r vacuum, summa exponentium, permutatio: exacta, sine ansis
     * (recensio T5 M1: structura plexus_aequales) */
    si (r.longitudo == ZEPHYRUM)
    {
        piscina_reficere(piscina, nota);
        *aequales = VERUM;
        redde VERUM;
    }
    si (plexus_summa_exponentium(r) != ZEPHYRUM)
    {
        piscina_reficere(piscina, nota);
        *aequales = FALSUM;
        redde VERUM;
    }
    imago = (i32*)piscina_allocare(piscina, (memoriae_index)r.fila
        * magnitudo(i32));
    (vacuum)plexus_permutatio(r, imago);
    per (k = ZEPHYRUM; k < r.fila; k++)
    {
        si (imago[k] != k)
        {
            idem = FALSUM;
        }
    }
    si (!idem)
    {
        piscina_reficere(piscina, nota);
        *aequales = FALSUM;
        redde VERUM;
    }
    si (!plexus_in_maiore(r, fila, I, piscina, &maior))
    {
        piscina_reficere(piscina, nota);
        redde FALSUM;
    }
    /* omnis ansa sub nota sua: memoria O(fila), non O(fila^2) */
    per (k = I; idem && k < fila; k++)
    {
        PiscinaNotatio nota_ansae = piscina_notare(piscina);
                  Ansa e;
                  Ansa imago_ansae;

        si (   !ansa_circa(fila, k, piscina, &e)
            || !ansa_agere(e, maior, piscina, &imago_ansae))
        {
            piscina_reficere(piscina, nota);
            redde FALSUM;
        }
        idem = ansae_aequales(e, imago_ansae);
        piscina_reficere(piscina, nota_ansae);
    }
    piscina_reficere(piscina, nota);
    *aequales = idem;
    redde VERUM;
}

i64
plexus_contractiones (vacuum)
{
    redde _contractiones;
}


/* ==================================================
 * Repraesentatio Burau reducta
 * ================================================== */

/* columna c matricis (n-1) x (n-1) ordine linearum: m[r * d + c] */

/* una littera: B <- B G(littera), columna una mutatur. FALSUM si
 * exponens extra fines polynomii (2^30: verbum longissimum) */
interior b32
_burau_littera (
    Polynomium* m,
           i32  fila,
           s32  littera,
       Piscina* piscina)
{
           i32 d = fila - I;
           i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera);
           i32 r;
           s32 k = littera > ZEPHYRUM ? I : -I;
    Polynomium x;

    si (fila == II)
    {
        /* [-t] aut [-t^-1] */
        si (!polynomium_translata(m[ZEPHYRUM], k, piscina, &x))
        {
            redde FALSUM;
        }
        m[ZEPHYRUM] = polynomium_nega(x, piscina);
        redde VERUM;
    }
    per (r = ZEPHYRUM; r < d; r++)
    {
        Polynomium* l = m + r * d;

        si (i == I)
        {
            /* G = [[-t, 0], [1, 1]]: c0' = c1 - t c0;
             * G^-1 = [[-t^-1, 0], [t^-1, 1]]: c0' = t^-1 (c1 - c0) */
            si (littera > ZEPHYRUM)
            {
                si (!polynomium_translata(l[ZEPHYRUM], I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[ZEPHYRUM] = polynomium_subtrahe(l[I], x, piscina);
            }
            alioquin
            {
                si (!polynomium_translata(polynomium_subtrahe(l[I],
                    l[ZEPHYRUM], piscina), -I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[ZEPHYRUM] = x;
            }
        }
        alioquin si (i == fila - I)
        {
            /* G = [[1, t], [0, -t]]: c1' = t (c0 - c1);
             * G^-1 = [[1, 1], [0, -t^-1]]: c1' = c0 - t^-1 c1 */
            si (littera > ZEPHYRUM)
            {
                si (!polynomium_translata(polynomium_subtrahe(l[d - II],
                    l[d - I], piscina), I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[d - I] = x;
            }
            alioquin
            {
                si (!polynomium_translata(l[d - I], -I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[d - I] = polynomium_subtrahe(l[d - II], x, piscina);
            }
        }
        alioquin
        {
            /* columnae i-2, i-1, i (ab 0); G: c1' = t (c0 - c1) + c2;
             * G^-1: c1' = c0 + t^-1 (c2 - c1) */
            i32 c0 = i - II;

            si (littera > ZEPHYRUM)
            {
                si (!polynomium_translata(polynomium_subtrahe(l[c0],
                    l[c0 + I], piscina), I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[c0 + I] = polynomium_adde(x, l[c0 + II], piscina);
            }
            alioquin
            {
                si (!polynomium_translata(polynomium_subtrahe(l[c0
                    + II],
                    l[c0 + I], piscina), -I, piscina, &x))
                {
                    redde FALSUM;
                }
                l[c0 + I] = polynomium_adde(l[c0], x, piscina);
            }
        }
    }
    redde VERUM;
}

/* eadem ad t = -1 super Z: t -> -1, t^-1 -> -1 */
interior vacuum
_burau_littera_integra (
     Magnus* m,
        i32  fila,
        s32  littera,
    Piscina* piscina)
{
    i32 d = fila - I;
    i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera);
    i32 r;

    si (fila == II)
    {
        redde;    /* -t = -t^-1 = 1 */
    }
    per (r = ZEPHYRUM; r < d; r++)
    {
        Magnus* l = m + r * d;

        si (i == I)
        {
            /* c0' = c0 + c1 (sigma), c0 - c1 (inversum) */
            l[ZEPHYRUM] = littera > ZEPHYRUM
                ? magnus_adde(l[ZEPHYRUM], l[I], piscina)
                : magnus_subtrahe(l[ZEPHYRUM], l[I], piscina);
        }
        alioquin si (i == fila - I)
        {
            /* c1' = c1 - c0 (sigma), c0 + c1 (inversum) */
            l[d - I] = littera > ZEPHYRUM
                ? magnus_subtrahe(l[d - I], l[d - II], piscina)
                : magnus_adde(l[d - II], l[d - I], piscina);
        }
        alioquin
        {
            /* c1' = c1 + c2 - c0 (sigma), c0 + c1 - c2 (inversum) */
            i32 c0 = i - II;

            l[c0 + I] = littera > ZEPHYRUM
                ? magnus_subtrahe(magnus_adde(l[c0 + I], l[c0 + II],
                piscina), l[c0], piscina)
                : magnus_subtrahe(magnus_adde(l[c0], l[c0 + I],
                piscina),
                l[c0 + II], piscina);
        }
    }
}

b32
plexus_burau (
     Plexus  p,
    Piscina* piscina,
     Matrix* exitus)
{
       Piscina* temporaria;
    Polynomium* m;
           i32  d = p.fila - I;
           i32  k;
           i32  r;
        Matrix  b;
memoriae_index  basis;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_BURAU)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_burau", 4096);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    m = (Polynomium*)piscina_allocare(temporaria, (memoriae_index)(d
        * d) * magnitudo(Polynomium));
    per (k = ZEPHYRUM; k < d * d; k++)
    {
        m[k] = (k / d == k % d)
            ? polynomium_constans(magnus_ex_s64(I), temporaria)
            : polynomium_nullum();
    }
    basis = piscina_summa_usus(temporaria);
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        si (!_burau_littera(m, p.fila, p.litterae[k], temporaria))
        {
            _propriam_destruere(temporaria);
            redde FALSUM;
        }
        /* purgamenta: contractio cum piscina quater crevit et 1 MB
         * superat (inspectio post LXIV litteras) */
        si (   (k % LXIV) == LXIII
            && piscina_summa_usus(temporaria) > IV * basis + 1048576UL)
        {
            Piscina* nova = piscina_generare_dynamicum("plexus_burau",
                4096);

            si (nova == NIHIL)
            {
                _propriam_destruere(temporaria);
                redde FALSUM;
            }
            {
                /* alveus matricis quoque in piscinam novam */
                Polynomium* m_nova = (Polynomium*)piscina_allocare(nova,
                    (memoriae_index)(d * d) * magnitudo(Polynomium));

                per (r = ZEPHYRUM; r < d * d; r++)
                {
                    m_nova[r] = polynomium_transcribe(m[r], nova);
                }
                m = m_nova;
            }
            _propriam_destruere(temporaria);
            temporaria  = nova;
            basis       = piscina_summa_usus(temporaria);
            _contractiones++;
        }
    }
    si (!matrix_nulla(&ANULUS_POLYNOMIORUM, d, d, piscina, &b))
    {
        _propriam_destruere(temporaria);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < d * d; k++)
    {
        Polynomium x = polynomium_transcribe(m[k], piscina);

        matrix_pone(&b, k / d, k % d, &x);
    }
    _propriam_destruere(temporaria);
    *exitus = b;
    redde VERUM;
}

b32
plexus_burau_minus_uno (
     Plexus  p,
    Piscina* piscina,
     Matrix* exitus)
{
       Piscina* temporaria;
        Magnus* m;
           i32  d = p.fila - I;
           i32  k;
           i32  r;
        Matrix  b;
memoriae_index  basis;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_BURAU)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_burau", 4096);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    m = (Magnus*)piscina_allocare(temporaria, (memoriae_index)(d * d)
        * magnitudo(Magnus));
    per (k = ZEPHYRUM; k < d * d; k++)
    {
        m[k] = magnus_ex_s64(k / d == k % d ? I : ZEPHYRUM);
    }
    basis = piscina_summa_usus(temporaria);
    per (k = ZEPHYRUM; k < p.longitudo; k++)
    {
        _burau_littera_integra(m, p.fila, p.litterae[k], temporaria);
        si (   (k % LXIV) == LXIII
            && piscina_summa_usus(temporaria) > IV * basis + 1048576UL)
        {
            Piscina* nova = piscina_generare_dynamicum("plexus_burau",
                4096);

            si (nova == NIHIL)
            {
                _propriam_destruere(temporaria);
                redde FALSUM;
            }
            {
                /* alveus matricis quoque in piscinam novam */
                Magnus* m_nova = (Magnus*)piscina_allocare(nova,
                    (memoriae_index)(d * d) * magnitudo(Magnus));

                per (r = ZEPHYRUM; r < d * d; r++)
                {
                    m_nova[r] = magnus_transcribe(m[r], nova);
                }
                m = m_nova;
            }
            _propriam_destruere(temporaria);
            temporaria  = nova;
            basis       = piscina_summa_usus(temporaria);
            _contractiones++;
        }
    }
    si (!matrix_nulla(&ANULUS_INTEGRORUM, d, d, piscina, &b))
    {
        _propriam_destruere(temporaria);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < d * d; k++)
    {
        Magnus x = magnus_transcribe(m[k], piscina);

        matrix_pone(&b, k / d, k % d, &x);
    }
    _propriam_destruere(temporaria);
    *exitus = b;
    redde VERUM;
}

b32
plexus_alexander (
         Plexus  p,
        Piscina* piscina,
     Polynomium* exitus)
{
    Piscina* t;
     Matrix  b;
     Matrix  unitas;
     Matrix  differentia;
 Polynomium  determinans;
 Polynomium  unum_minus_t;
 Polynomium  denominator;
 Polynomium  numerator;
 Polynomium  quotiens;
 Polynomium  effectum;
 Polynomium  t_n;
        b32  bene;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_BURAU)
    {
        redde FALSUM;
    }
    t = piscina_generare_dynamicum("plexus_alexander", 4096);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    /* det(I - B) (1 - t) / (1 - t^n) */
    bene = plexus_burau(p, t, &b)
        && matrix_identitas(&ANULUS_POLYNOMIORUM, p.fila - I, t,
        &unitas)
        && matrix_subtrahe(unitas, b, t, &differentia)
        && matrix_determinans(differentia, t, &determinans)
        && polynomium_monomium(magnus_ex_s64(I), I, t, &t_n);
    si (bene)
    {
        unum_minus_t = polynomium_subtrahe(polynomium_constans(
            magnus_ex_s64(I), t), t_n, t);
        bene = polynomium_monomium(magnus_ex_s64(I), (s32)p.fila, t,
            &t_n)
            && polynomium_multiplica(determinans, unum_minus_t, t,
            &numerator);
    }
    si (bene)
    {
        denominator = polynomium_subtrahe(polynomium_constans(
            magnus_ex_s64(I), t), t_n, t);
        bene = polynomium_divide_exacte(numerator, denominator, t,
            &quotiens)
            && polynomium_normale(quotiens, t, &effectum);
    }
    si (bene)
    {
        *exitus = polynomium_transcribe(effectum, piscina);
    }
    _propriam_destruere(t);
    redde bene;
}

b32
plexus_burau_radix_maxima (
          Plexus  p,
         Piscina* piscina,
     RadixRealis* exitus)
{
         Piscina* t;
          Matrix  b;
          Matrix  matrix_propria;
      Polynomium  polynomium_proprium;
      Polynomium  polynomium_reflexum;
      Polynomium  h;
     RadixRealis* radices;
     RadixRealis  unum;
     RadixRealis  maxima;
             i32  numerus  = ZEPHYRUM;
             i32  d        = p.fila - I;
             i32  k;
             s32  signum  = ZEPHYRUM;
             b32  bene;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_BURAU)
    {
        redde FALSUM;
    }
    t = piscina_generare_dynamicum("plexus_radix", 4096);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    /* chi(x) = det(x I - B(-1)) super Z[x] */
    bene = plexus_burau_minus_uno(p, t, &b)
        && matrix_nulla(&ANULUS_POLYNOMIORUM, d, d, t,
        &matrix_propria);
    per (k = ZEPHYRUM; bene && k < d * d; k++)
    {
               i32 linea    = k / d;
               i32 columna  = k % d;
        Polynomium x;

        x = polynomium_nega(polynomium_constans(*(constans Magnus*)
            matrix_elementum(b, linea, columna), t), t);
        si (linea == columna)
        {
            Polynomium monomium;

            bene = polynomium_monomium(magnus_ex_s64(I), I, t,
                &monomium);
            x = polynomium_adde(x, monomium, t);
        }
        matrix_pone(&matrix_propria, linea, columna, &x);
    }
    bene = bene
        && matrix_determinans(matrix_propria, t, &polynomium_proprium);
    si (bene)
    {
        /* chi(-x): coefficientes graduum imparium negantur */
           s32  summus = polynomium_gradus_summus(polynomium_proprium);
        Magnus* c      = (Magnus*)piscina_allocare(t,
            (memoriae_index)(summus + I) * magnitudo(Magnus));
        s32 e;

        per (e = ZEPHYRUM; e <= summus; e++)
        {
            Magnus coefficiens =
                polynomium_coefficiens(polynomium_proprium, e);

            c[e] = (e % II == I) ? magnus_nega(coefficiens, t)
                : coefficiens;
        }
        bene = polynomium_ex_coefficientibus(c, (i32)(summus + I),
            ZEPHYRUM, t, &polynomium_reflexum)
            && polynomium_multiplica(polynomium_proprium,
            polynomium_reflexum, t, &h)
            && radices_reales(h, t, &radices, &numerus);
    }
    si (bene)
    {
        /* radices h = +-lambda: maxima = max |lambda| realis;
         * minimum 1 */
        unum    = radix_ex_fractione(fractio_ex_s64(I), t);
        maxima  = unum;
        si (numerus > ZEPHYRUM)
        {
            bene = radix_compara(radices[numerus - I], unum, t,
                &signum);
            si (bene && signum > ZEPHYRUM)
            {
                maxima = radices[numerus - I];
            }
        }
    }
    si (bene)
    {
        RadixRealis copia;

        copia.f      = polynomium_transcribe(maxima.f, piscina);
        copia.infra  = fractio_transcribe(maxima.infra, piscina);
        copia.supra  = fractio_transcribe(maxima.supra, piscina);
        *exitus      = copia;
    }
    _propriam_destruere(t);
    redde bene;
}


/* ==================================================
 * Entropia per incrementum ansarum
 * ================================================== */

/* ansa in piscinam transcripta (coordinatae et alvei novi) */
interior Ansa
_ansam_transcribere (
        Ansa  u,
     Piscina* piscina)
{
    Ansa v;
     i32 k;

    v.fila  = u.fila;
    v.a     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(u.fila
        - II) * magnitudo(Magnus));
    v.b     = (Magnus*)piscina_allocare(piscina, (memoriae_index)(u.fila
        - II) * magnitudo(Magnus));
    per (k = ZEPHYRUM; k + II < u.fila; k++)
    {
        v.a[k] = magnus_transcribe(u.a[k], piscina);
        v.b[k] = magnus_transcribe(u.b[k], piscina);
    }
    redde v;
}

b32
plexus_incrementum (
     Plexus   p,
       Ansa   u,
        i32   periodi,
    Piscina*  piscina,
     Magnus** exitus)
{
     Piscina* prior;
     Piscina* sequens;
      Magnus* longitudines;
        Ansa  currens;
         i32  m;
         b32  bene = VERUM;

    si (   !plexus_validus(p) || !ansa_valida(u) || p.fila != u.fila
        || periodi > 0x7FFFFFFEUL)
    {
        redde FALSUM;
    }
    prior    = piscina_generare_dynamicum("plexus_incrementum", 4096);
    sequens  = piscina_generare_dynamicum("plexus_incrementum", 4096);
    si (prior == NIHIL || sequens == NIHIL)
    {
        si (prior != NIHIL)
        {
            _propriam_destruere(prior);
        }
        si (sequens != NIHIL)
        {
            _propriam_destruere(sequens);
        }
        redde FALSUM;
    }
    longitudines = (Magnus*)piscina_allocare(piscina,
        (memoriae_index)(periodi + I) * magnitudo(Magnus));
    currens = _ansam_transcribere(u, prior);
    per (m = ZEPHYRUM; bene && m <= periodi; m++)
    {
        longitudines[m] = magnus_transcribe(ansa_sectiones(currens,
            prior), piscina);
        si (m < periodi)
        {
               Ansa  nova;
            Piscina* commutata;

            /* u_{m+1} in 'sequens'; 'prior' vacatur et commutatur */
            bene = ansa_agere(currens, p, sequens, &nova);
            si (bene)
            {
                _propriam_vacare(prior);
                commutata  = prior;
                prior      = sequens;
                sequens    = commutata;
                currens    = nova;
            }
        }
    }
    _propriam_destruere(prior);
    _propriam_destruere(sequens);
    si (bene)
    {
        *exitus = longitudines;
    }
    redde bene;
}

memoriae_index
plexus_apex_proprius (vacuum)
{
    redde _apex_proprius;
}

vacuum
plexus_apicem_purgare (vacuum)
{
    _apex_proprius = ZEPHYRUM;
}


/* ==================================================
 * Certificatum per Burau in radicibus unitatis
 * ================================================== */

/* phi(k) per divisionem (k parvus) */
interior i32
_phi (
    i32 k)
{
    i32 r = k;
    i32 n = k;
    i32 d;

    per (d = II; d * d <= n; d++)
    {
        si (n % d == ZEPHYRUM)
        {
            dum (n % d == ZEPHYRUM)
            {
                n /= d;
            }
            r -= r / d;
        }
    }
    si (n > I)
    {
        r -= r / n;
    }
    redde r;
}

b32
plexus_burau_radius_maior (
     Plexus  p,
        i32  ordo,
        b32* maior)
{
        Piscina* t;
     Cyclotomia* anulus;
         Matrix  b;
         Matrix  propria;
     Polynomium  proprium;
            i32  d = p.fila - I;
            i32  g;
            i32  dim;
            i32  a;
            i32  c;
            i32  i;
            i32  j;
            i32  k;
            b32  bene;
     Polynomium  monomium;

    si (!plexus_validus(p) || ordo < II || ordo > XII)
    {
        redde FALSUM;
    }
    g = _phi(ordo);
    si ((i64)d * (i64)g > (i64)PLEXUS_BURAU_CIRCULI)
    {
        redde FALSUM;
    }
    dim  = d * g;
    t    = piscina_generare_dynamicum("plexus_circulus", 4096);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    anulus = cyclotomia_creare(ordo, t);
    bene = anulus != NIHIL && plexus_burau(p, t, &b)
        && matrix_nulla(&ANULUS_POLYNOMIORUM, dim, dim, t, &propria)
        && polynomium_monomium(magnus_ex_s64(I), I, t, &monomium);
    /* xI - R: R[(a,i),(c,j)] = coefficiens zeta^i in B[a][c] zeta^j */
    per (a = ZEPHYRUM; bene && a < d; a++)
    {
        per (c = ZEPHYRUM; bene && c < d; c++)
        {
            Cyclotomicus elementum;

            bene = cyclotomicus_ex_polynomio(anulus,
                *(constans Polynomium*)matrix_elementum(b, a, c), I, t,
                &elementum);
            per (j = ZEPHYRUM; bene && j < g; j++)
            {
                Cyclotomicus columna =
                    cyclotomicus_multiplica(elementum,
                    cyclotomicus_radix(anulus, (s32)j, t), t);

                per (i = ZEPHYRUM; i < g; i++)
                {
                    Polynomium x = polynomium_nega(polynomium_constans(
                        cyclotomicus_coefficiens(columna, i), t), t);

                    si (a == c && i == j)
                    {
                        x = polynomium_adde(x, monomium, t);
                    }
                    matrix_pone(&propria, a * g + i, c * g + j, &x);
                }
            }
        }
    }
    bene = bene && matrix_determinans(propria, t, &proprium);
    /* Kronecker: factores cyclotomici dividuntur; residuum gradus > 0
     * -> radix |z| > 1 */
    per (k = I; bene && k <= II * (i32)dim * (i32)dim + II; k++)
    {
        Polynomium phi_k;
        Polynomium quotiens;
               i32 gradus_k = _phi(k);

        si ((s32)gradus_k > polynomium_gradus_summus(proprium))
        {
            perge;
        }
        si (!polynomium_cyclotomicum(k, t, &phi_k))
        {
            bene = FALSUM;
            frange;
        }
        dum (   polynomium_gradus_summus(proprium)
            >= (s32)gradus_k
             && polynomium_divide_exacte(proprium, phi_k, t,
             &quotiens))
        {
            proprium = quotiens;
        }
    }
    si (bene)
    {
        *maior = polynomium_gradus_summus(proprium) > ZEPHYRUM;
    }
    _propriam_destruere(t);
    redde bene;
}
#undef ImaginesLiberae
#undef PLEXUS_LONGITUDO_SUMMA
#undef _ansam_contrahere
#undef _ansam_littera
#undef _ansam_transcribere
#undef _apex_proprius
#undef _apicem_notare
#undef _burau_littera
#undef _burau_littera_integra
#undef _contractiones
#undef _contrahere
#undef _copia
#undef _imagines_computare
#undef _index
#undef _inversum
#undef _limes
#undef _littera_valida
#undef _litterae
#undef _litterae_ad_chordam
#undef _litterae_conari
#undef _negativa
#undef _phi
#undef _positiva
#undef _productum
#undef _propriam_destruere
#undef _propriam_vacare
#undef _reducere_in
/* lib/sors.c: statica per plagulam renominata */
#define _pcg_initiare _pcg_initiare_sors
#define _splitmix_proximum _splitmix_proximum_sors
#line 1 "lib/sors.c"
/* sors.c - Numeri pseudo-fortuiti seminabiles (PCG32 + SplitMix64)
 *
 * PCG32 (O'Neill): status LXIV bits, effusio XXXII bits per
 * 'xorshift + rotatio'. SplitMix64 (Vigna) semen usoris in statum
 * initialem bene mixtum vertit. Vide lib/sors.worklog.md.
 */


#define SORS_MULTIPLICATOR  6364136223846793005ULL
#define SPLITMIX_GAMMA      0x9E3779B97F4A7C15ULL
#define SPLITMIX_MISCENS_I   0xBF58476D1CE4E5B9ULL
#define SPLITMIX_MISCENS_II  0x94D049BB133111EBULL

/* SplitMix64: statum provehit, valorem mixtum reddit */
interior i64
_splitmix_proximum (
    i64* x)
{
    i64 z;

    *x  = *x + SPLITMIX_GAMMA;
    z   = *x;
    z   = (z ^ (z >> XXX)) * SPLITMIX_MISCENS_I;
    z   = (z ^ (z >> XXVII)) * SPLITMIX_MISCENS_II;
    redde z ^ (z >> XXXI);
}

/* PCG32 initium canonicum (pcg32_srandom_r): status nullus,
 * incrementum impar ex serie, gradus, status += initium, gradus */
interior vacuum
_pcg_initiare (
    Sors* s,
     i64  initium,
     i64  series)
{
    s->status       = 0ULL;
    s->incrementum  = (series << I) | 1ULL;
    (vacuum)sors_proximum(s);
    s->status = s->status + initium;
    (vacuum)sors_proximum(s);
}

vacuum
sors_seminare (
    Sors* s,
     i64  semen,
     i64  series)
{
    i64 x = semen;

    _pcg_initiare(s, _splitmix_proximum(&x), series);
}

Sors
sors_derivare (
    constans Sors* s,
              i64  clavis)
{
    Sors filius;
     i64 y = s->status + clavis * SPLITMIX_GAMMA;

    sors_seminare(&filius, _splitmix_proximum(&y) ^ s->incrementum,
        clavis);
    redde filius;
}

i32
sors_proximum (
    Sors* s)
{
    i64 vetus = s->status;
    i32 mixtum;
    i32 rotatio;

    s->status  = vetus * SORS_MULTIPLICATOR + s->incrementum;
    mixtum     = (i32)(((vetus >> XVIII) ^ vetus) >> XXVII);
    rotatio    = (i32)(vetus >> LIX);
    redde (mixtum >> rotatio) | (mixtum << ((0U - rotatio) & XXXI));
}

i32
sors_intra (
    Sors* s,
     i32  limes)
{
    i64 productum;
    i32 inferior;

    si (limes == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    productum  = (i64)sors_proximum(s) * (i64)limes;
    inferior   = (i32)productum;
    si (inferior < limes)
    {
        i32 limen = (0U - limes) % limes;

        dum (inferior < limen)
        {
            productum  = (i64)sors_proximum(s) * (i64)limes;
            inferior   = (i32)productum;
        }
    }
    redde (i32)(productum >> XXXII);
}

s32
sors_inter (
    Sors* s,
     s32  imum,
     s32  summum)
{
    i64 spatium;

    si (imum >= summum)
    {
        redde imum;
    }
    spatium = (i64)((s64)summum - (s64)imum) + 1ULL;
    si (spatium > 0xFFFFFFFFULL)
    {
        redde (s32)sors_proximum(s);
    }
    redde (s32)((s64)imum + (s64)sors_intra(s, (i32)spatium));
}

f64
sors_f64 (
    Sors* s)
{
    i32 superior = sors_proximum(s) >> V;
    i32 inferior = sors_proximum(s) >> VI;

    redde ((f64)superior * 67108864.0 + (f64)inferior)
        / 9007199254740992.0;
}

f32
sors_f32 (
    Sors* s)
{
    redde (f32)(sors_proximum(s) >> VIII) * (1.0f / 16777216.0f);
}

b32
sors_casu (
    Sors* s,
     i32  numerator,
     i32  denominator)
{
    si (denominator == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde sors_intra(s, denominator) < numerator;
}

vacuum
sors_miscere (
      Sors* s,
    vacuum* elementa,
       i32  numerus,
       i32  magnitudo_elementi)
{
     i8* octeti = (i8*)elementa;
    i32  i;

    si (elementa == NIHIL || numerus < II)
    {
        redde;
    }
    per (i = numerus - I; i > ZEPHYRUM; i--)
    {
        i32 j = sors_intra(s, i + I);
        i32 k;

        si (j == i)
        {
            perge;
        }
        per (k = ZEPHYRUM; k < magnitudo_elementi; k++)
        {
            i8 transitus = octeti[i * magnitudo_elementi + k];

            octeti[i * magnitudo_elementi + k]  =
                octeti[j * magnitudo_elementi + k];
            octeti[j * magnitudo_elementi + k]  = transitus;
        }
    }
}

i32
sors_lcg_classicus (
    i32* status)
{
    *status = *status * 1103515245U + 12345U;
    redde *status;
}
#undef SORS_MULTIPLICATOR
#undef SPLITMIX_GAMMA
#undef SPLITMIX_MISCENS_I
#undef SPLITMIX_MISCENS_II
#undef _pcg_initiare
#undef _splitmix_proximum
/* lib/chorda_aedificator.c: statica per plagulam renominata */
#define _appendere_interna _appendere_interna_chorda_aedificator
#define _crescere _crescere_chorda_aedificator
#define _evadere_json _evadere_json_chorda_aedificator
#define _format_duplex _format_duplex_chorda_aedificator
#define _format_integer_i32 _format_integer_i32_chorda_aedificator
#define _format_integer_s32 _format_integer_s32_chorda_aedificator
#define _proxima_capacitas _proxima_capacitas_chorda_aedificator
#line 1 "lib/chorda_aedificator.c"


#include <stdio.h>
#include <string.h>


/* ==================================================
 * Structura ChordaAedificator - Interna
 * ================================================== */

structura ChordaAedificator {
                i8* buffer;
    memoriae_index  capacitas;
    memoriae_index  offset;
           Piscina* piscina;
               i32  indentatio_gradus;
};


/* ==================================================
 * ADIUTORES INTERNI
 * ================================================== */

interior memoriae_index
_proxima_capacitas (
    memoriae_index nunc)
{
    /* Duplica capacitatem donec satis habeamus */
    redde nunc > ZEPHYRUM ? nunc * II : XVI;
}

interior b32
_crescere (
    ChordaAedificator* aedificator,
       memoriae_index  necessaria)
{
    memoriae_index  capacitas_nova;
                i8* buffer_novum;

    capacitas_nova = aedificator->capacitas;
    dum (capacitas_nova < necessaria)
    {
        capacitas_nova = _proxima_capacitas(capacitas_nova);
    }

    buffer_novum = (i8*)piscina_allocare(aedificator->piscina,
        capacitas_nova);
    si (!buffer_novum) redde FALSUM;

    si (aedificator->buffer && aedificator->offset > ZEPHYRUM)
    {
        memcpy(buffer_novum, aedificator->buffer, aedificator->offset);
    }

    aedificator->buffer     = buffer_novum;
    aedificator->capacitas  = capacitas_nova;

    redde VERUM;
}

interior b32
_appendere_interna (
    ChordaAedificator* aedificator,
          constans i8* datum,
       memoriae_index  mensura)
{
    memoriae_index necessaria;

    /* Appendix vacua bona est */
    si (!aedificator || !datum || mensura == ZEPHYRUM)
    {
        redde mensura == ZEPHYRUM;
    }

    necessaria = aedificator->offset + mensura;

    si (necessaria > aedificator->capacitas)
    {
        si (!_crescere(aedificator, necessaria)) redde FALSUM;
    }

    memcpy(aedificator->buffer + aedificator->offset, datum, mensura);
    aedificator->offset += mensura;

    redde VERUM;
}

/* Formata integrum signatum ad buffer
 * Vocans praebet buffer, debet esse >= 32 bytes (tutus pro s32) */
interior memoriae_index
_format_integer_s32 (
               s32  n,
                i8* buffer,
    memoriae_index  capacitas)
{
            character cstr[CXXXII];
                  s32 mensura_signed;
       memoriae_index mensura;

    mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr),
        "%d", n);
    si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

    mensura = (memoriae_index)mensura_signed;
    si (mensura >= capacitas) redde ZEPHYRUM;

    memcpy(buffer, cstr, mensura);
    redde mensura;
}

interior memoriae_index
_format_integer_i32 (
               i32  n,
                i8* buffer,
    memoriae_index  capacitas)
{
         character cstr[CXXXII];
               s32 mensura_signed;
    memoriae_index mensura;

    mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr),
        "%u", n);
    si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

    mensura = (memoriae_index)mensura_signed;
    si (mensura >= capacitas) redde ZEPHYRUM;

    memcpy(buffer, cstr, mensura);
    redde mensura;
}

interior memoriae_index
_format_duplex (
               f64  n,
               i32  decimales,
                i8* buffer,
    memoriae_index  capacitas)
{
         character cstr[CXXXII];
         character formatalis[XVI];
               s32 mensura_signed;
    memoriae_index mensura;

    snprintf(formatalis, (memoriae_index)magnitudo(formatalis),
        "%%.%df", decimales);
    mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr),
        formatalis, n);

    si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

    mensura = (memoriae_index)mensura_signed;
    si (mensura >= capacitas) redde ZEPHYRUM;

    memcpy(buffer, cstr, mensura);
    redde mensura;
}

/* Effugium chordae JSON
 * Tractat: citationem, virgulam inversam, characteres imperantes
 * Vocans praebet buffer */
interior memoriae_index
_evadere_json (
       constans i8* input,
    memoriae_index  mensura,
                i8* output,
    memoriae_index  capacitas_output)
{
          memoriae_index  index_input;
          memoriae_index  index_output;
               character  c;
    insignatus character  u;
      constans character* hex = "0123456789abcdef";

    index_input   = ZEPHYRUM;
    index_output  = ZEPHYRUM;

    dum (index_input < mensura && index_output < capacitas_output - I)
    {
        c = (character)input[index_input];

        si (c == '"')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = '"';
        }
        alioquin si (c == '\\')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = '\\';
        }
        alioquin si (c == '\n')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = 'n';
        }
        alioquin si (c == '\r')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = 'r';
        }
        alioquin si (c == '\t')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = 't';
        }
        alioquin si (   (insignatus character)c < 0x20
                     || (insignatus character)c == 0x7F)
        {
            /* Alii characteres imperantes ut \u00xx. REGULA JSON per
             * se, non ctype (2026-09-22): iscntrl((signed char)c)
             * erat UB pro octetis >= 0x80 (valor negativus non EOF)
             * et a locale pendebat. DEL (0x7F) effugitur ut olim -
             * scriptura immutata. */
            u = (insignatus character)c;
            si (index_output + VI > capacitas_output)
            {
                redde ZEPHYRUM;
            }
            output[index_output++] = '\\';
            output[index_output++] = 'u';
            output[index_output++] = '0';
            output[index_output++] = '0';
            output[index_output++] = (i8)hex[(u >> IV) & 0xF];
            output[index_output++] = (i8)hex[u & 0xF];
        }
        alioquin
        {
            output[index_output++] = (i8)c;
        }

        index_input++;
    }

    redde index_output;
}


/* ==================================================
 * Creatio
 * ================================================== */

ChordaAedificator*
chorda_aedificator_creare (
           Piscina* piscina,
    memoriae_index  capacitas_initialis)
{
    ChordaAedificator* aedificator;
                   i8* buffer;

    si (!piscina || capacitas_initialis == ZEPHYRUM) redde NIHIL;

    aedificator = (ChordaAedificator*)piscina_allocare(
                                        piscina,
                                        magnitudo(ChordaAedificator));
    si (!aedificator) redde NIHIL;

    buffer = (i8*)piscina_allocare(piscina, capacitas_initialis);
    si (!buffer) redde NIHIL;

    aedificator->buffer             = buffer;
    aedificator->capacitas          = capacitas_initialis;
    aedificator->offset             = ZEPHYRUM;
    aedificator->piscina            = piscina;
    aedificator->indentatio_gradus  = ZEPHYRUM;

    redde aedificator;
}


/* ==================================================
 * Destructio
 * ================================================== */

vacuum
chorda_aedificator_destruere (
    ChordaAedificator* aedificator)
{
    /* Piscina possidet memoriam; solum liberamus structuram */
    si (aedificator)
    {
        /* Nota: buffer etiam allocatus ex piscina,
		 * ergo liberabitur quando piscina destruitur */
    }
}


/* ==================================================
 * Appendere - Character
 * ================================================== */

b32
chorda_aedificator_appendere_character (
    ChordaAedificator* aedificator,
            character  c)
{
    i8 ch = (i8)c;
    redde _appendere_interna(aedificator, &ch, I);
}


/* ==================================================
 * Appendere - Chordae
 * ================================================== */

b32
chorda_aedificator_appendere_literis (
     ChordaAedificator* aedificator,
    constans character* cstr)
{
    memoriae_index mensura;

    si (!aedificator || !cstr) redde FALSUM;

    mensura = strlen(cstr);
    redde _appendere_interna(aedificator, (constans i8*)cstr, mensura);
}

b32
chorda_aedificator_appendere_chorda (
    ChordaAedificator* aedificator,
               chorda  s)
{
    si (!aedificator || !s.datum) redde FALSUM;

    redde _appendere_interna(aedificator, s.datum, s.mensura);
}


/* ==================================================
 * Appendere - Numeri
 * ================================================== */

b32
chorda_aedificator_appendere_s32 (
    ChordaAedificator* aedificator,
                  s32  n)
{
                i8 buffer[CXXXII];
    memoriae_index mensura;

    si (!aedificator) redde FALSUM;

    mensura = _format_integer_s32(n, buffer, magnitudo(buffer));
    si (mensura == ZEPHYRUM) redde FALSUM;

    redde _appendere_interna(aedificator, buffer, mensura);
}

b32
chorda_aedificator_appendere_i32 (
    ChordaAedificator* aedificator,
                  i32  n)
{
                i8 buffer[CXXXII];
    memoriae_index mensura;

    si (!aedificator) redde FALSUM;

    mensura = _format_integer_i32(n, buffer, magnitudo(buffer));
    si (mensura == ZEPHYRUM) redde FALSUM;

    redde _appendere_interna(aedificator, buffer, mensura);
}

b32
chorda_aedificator_appendere_f64 (
    ChordaAedificator* aedificator,
                  f64  n,
                  i32  decimales)
{
                i8 buffer[CXXXII];
    memoriae_index mensura;

    si (!aedificator || decimales < ZEPHYRUM || decimales > XXX)
    {
        redde FALSUM;
    }

    mensura = _format_duplex(n, decimales, buffer, magnitudo(buffer));
    si (mensura == ZEPHYRUM) redde FALSUM;

    redde _appendere_interna(aedificator, buffer, mensura);
}

b32
chorda_aedificator_appendere_repetita (
     ChordaAedificator* aedificator,
             character  c,
                   i32  numerus)
{
    i32 i;

    /* numerus i32 insignatus - custodia negativi mortua remota
	 * (2026-07-17); familia indentationis non-negativa per push/pop */
    si (!aedificator) redde FALSUM;

    si (numerus == ZEPHYRUM) redde VERUM;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (!chorda_aedificator_appendere_character(aedificator, c))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

b32
chorda_aedificator_appendere_hex_i32 (
    ChordaAedificator* aedificator,
                  i32  n)
{
             character buffer[XVI];
                   s32 mensura_signed;
        memoriae_index mensura;

    si (!aedificator) redde FALSUM;

    mensura_signed = snprintf(buffer, magnitudo(buffer), "%x", n);
    si (mensura_signed < ZEPHYRUM) redde FALSUM;

    mensura = (memoriae_index)mensura_signed;

    redde _appendere_interna(aedificator, (i8*)buffer, mensura);
}


/* ==================================================
 * Appendere - Evasus
 * ================================================== */

b32
chorda_aedificator_appendere_evasus_json (
    ChordaAedificator* aedificator,
               chorda  s)
{
                i8  buffer[D];  /* Buffer in acervo pro effugiis parvis */
                i8* output_buffer;
    memoriae_index  mensura_evasus;
    memoriae_index  necessaria;

    si (!aedificator)
    {
        redde FALSUM;
    }
    /* chorda vacua (datum quodvis, etiam NIHIL - forma domus): nihil
     * addendum, SUCCESSUS. Olim FALSUM: _evadere_json nullos octetos
     * scriptos ut defectum reddebat (2026-09-22). */
    si (s.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (!s.datum)
    {
        redde FALSUM;
    }

    /* Pessimus casus: omnis character fit \uXXXX (6 bytes) */
    necessaria = s.mensura * VI;

    si (necessaria <= magnitudo(buffer))
    {
        output_buffer = buffer;
    }
    alioquin
    {
        output_buffer = (i8*)piscina_allocare(aedificator->piscina,
            necessaria);
        si (!output_buffer) redde FALSUM;
    }

    mensura_evasus = _evadere_json(s.datum, s.mensura, output_buffer,
        necessaria);
    si (mensura_evasus == ZEPHYRUM)
    {
        redde FALSUM;
    }

    redde _appendere_interna(aedificator, output_buffer,
        mensura_evasus);
}

b32
chorda_aedificator_appendere_literis_evasus_json (
     ChordaAedificator* aedificator,
    constans character* cstr)
{
    memoriae_index  mensura;
                i8* buffer_temporalis;
            chorda  s;
               b32  result;

    si (!cstr || !aedificator) redde FALSUM;

    mensura = strlen(cstr);
    /* "" = successus, nihil additum (piscina_allocare(0) NIHIL
     * reddit - olim FALSUM, 2026-09-22) */
    si (mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    buffer_temporalis = (i8*)piscina_allocare(aedificator->piscina,
        mensura);
    si (!buffer_temporalis) redde FALSUM;

    memcpy(buffer_temporalis, cstr, mensura);
    s.datum    = buffer_temporalis;
    s.mensura  = (i32)mensura;

    result = chorda_aedificator_appendere_evasus_json(aedificator, s);
    redde result;
}


/* ==================================================
 * Appendere - Spatium Album/Structura
 * ================================================== */

b32
chorda_aedificator_appendere_lineam_novam (
    ChordaAedificator* aedificator)
{
    si (!aedificator) redde FALSUM;

    redde chorda_aedificator_appendere_character(aedificator, '\n');
}

b32
chorda_aedificator_appendere_indentationem (
    ChordaAedificator* aedificator,
                  i32  gradus)
{
    i32 i;
    i32 spatia;

    /* gradus i32 insignatus - custodia negativi mortua remota
	 * (2026-07-17); gradus tractatus <= M per push, pop ad zephyrum
	 * sistit */
    si (!aedificator) redde FALSUM;

    spatia = gradus * CHORDA_AEDIFICATOR_INDENTATIO_SPATIA;

    per (i = ZEPHYRUM; i < spatia; i++)
    {
        si (!chorda_aedificator_appendere_character(aedificator, ' '))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}


/* ==================================================
 * Indentationis Observatio
 * ================================================== */

vacuum
chorda_aedificator_push_indentationem (
    ChordaAedificator* aedificator)
{
    si (aedificator && aedificator->indentatio_gradus < M)
    {
        aedificator->indentatio_gradus++;
    }
}

vacuum
chorda_aedificator_pop_indentationem (
    ChordaAedificator* aedificator)
{
    si (aedificator && aedificator->indentatio_gradus > ZEPHYRUM)
    {
        aedificator->indentatio_gradus--;
    }
}

i32
chorda_aedificator_indentatio_gradus (
    ChordaAedificator* aedificator)
{
    redde aedificator ? aedificator->indentatio_gradus : ZEPHYRUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

memoriae_index
chorda_aedificator_longitudo (
    ChordaAedificator* aedificator)
{
    redde aedificator ? aedificator->offset : ZEPHYRUM;
}

vacuum
chorda_aedificator_truncare (
    ChordaAedificator* aedificator,
       memoriae_index  longitudo_nova)
{
    si (aedificator == NIHIL)
    {
        redde;
    }
    si (longitudo_nova < aedificator->offset)
    {
        aedificator->offset = longitudo_nova;
    }
}

chorda
chorda_aedificator_spectare (
    ChordaAedificator* aedificator)
{
    chorda result;

    si (!aedificator || !aedificator->buffer)
    {
        result.mensura  = ZEPHYRUM;
        result.datum    = NIHIL;
    }
    alioquin
    {
        result.mensura  = (i32)aedificator->offset;
        result.datum    = aedificator->buffer;
    }

    redde result;
}


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

vacuum
chorda_aedificator_reset (
    ChordaAedificator* aedificator)
{
    si (!aedificator) redde;

    aedificator->offset             = ZEPHYRUM;
    aedificator->indentatio_gradus  = ZEPHYRUM;
}

chorda
chorda_aedificator_finire (
    ChordaAedificator* aedificator)
{
    chorda result;

    si (!aedificator)
    {
        result.mensura  = ZEPHYRUM;
        result.datum    = NIHIL;
        redde result;
    }

    result.mensura  = (i32)aedificator->offset;
    result.datum    = aedificator->buffer;

    redde result;
}
#undef _appendere_interna
#undef _crescere
#undef _evadere_json
#undef _format_duplex
#undef _format_integer_i32
#undef _format_integer_s32
#undef _proxima_capacitas
/* lib/congruentia.c: statica per plagulam renominata */
#define _testis _testis_congruentia
#line 1 "lib/congruentia.c"
/* congruentia.c - Arithmetica modularis exacta, moduli verbi (n < 2^32)
 *
 * Producta in i64 insignato: (n - 1)^2 < 2^64. Inversa per Euclidem
 * extensum in s64 (|coefficientes| <= n < 2^32). Primalitas per
 * Miller-Rabin cum basibus 2, 7, 61 - exacta pro omni n < 4759123141
 * (Jaeschke 1993), ergo pro omni i32. Reconstructio Sinica per Garner
 * super magnum. Vide lib/congruentia.worklog.md.
 */



/* ==================================================
 * Reductio
 * ================================================== */

i32
congruentia_ex_s64 (
    s64 x,
    i32 n)
{
    i64 modulus;
    i64 residuum;

    si (n == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (x >= ZEPHYRUM)
    {
        redde (i32)((i64)x % (i64)n);
    }
    /* |x| sine exundatione etiam pro S64 imo */
    modulus   = (i64)(-(x + I)) + (i64)I;
    residuum  = modulus % (i64)n;
    redde residuum == ZEPHYRUM ? ZEPHYRUM : (i32)((i64)n - residuum);
}

i32
congruentia_ex_magno (
    Magnus x,
       i32 n)
{
    redde magnus_residuum_parvum(x, n);
}


/* ==================================================
 * Arithmetica
 * ================================================== */

i32
congruentia_adde (
    i32 a,
    i32 b,
    i32 n)
{
    i64 summa = (i64)a + (i64)b;

    si (summa >= (i64)n)
    {
        summa -= (i64)n;
    }
    redde (i32)summa;
}

i32
congruentia_subtrahe (
    i32 a,
    i32 b,
    i32 n)
{
    redde a >= b ? a - b : (i32)((i64)a + (i64)n - (i64)b);
}

i32
congruentia_multiplica (
    i32 a,
    i32 b,
    i32 n)
{
    redde (i32)(((i64)a * (i64)b) % (i64)n);
}

i32
congruentia_potentia (
    i32 a,
    i64 e,
    i32 n)
{
    i32 effectus  = (n == I) ? ZEPHYRUM : I;
    i32 basis     = a;

    dum (e > ZEPHYRUM)
    {
        si (e & (i64)I)
        {
            effectus = congruentia_multiplica(effectus, basis, n);
        }
        e >>= I;
        si (e > ZEPHYRUM)
        {
            basis = congruentia_multiplica(basis, basis, n);
        }
    }
    redde effectus;
}

b32
congruentia_inversa (
     i32  a,
     i32  n,
     i32* exitus)
{
    s64 r0 = (s64)n;
    s64 r1 = (s64)a;
    s64 t0 = ZEPHYRUM;
    s64 t1 = I;

    si (n < II)
    {
        redde FALSUM;
    }
    /* invarians: r_k = t_k a (mod n); |t_k| <= n */
    dum (r1 != ZEPHYRUM)
    {
        s64 quotiens  = r0 / r1;   /* r0, r1 >= 0: C89 definitum */
        s64 r2        = r0 - quotiens * r1;
        s64 t2        = t0 - quotiens * t1;

        r0 = r1;
        r1 = r2;
        t0 = t1;
        t1 = t2;
    }
    si (r0 != I)
    {
        redde FALSUM;
    }
    *exitus = congruentia_ex_s64(t0, n);
    redde VERUM;
}

s64
congruentia_symmetrica (
    i32 a,
    i32 n)
{
    redde a > n / II ? (s64)a - (s64)n : (s64)a;
}


/* ==================================================
 * Primi
 * ================================================== */

/* testis Miller-Rabin: n - 1 = d 2^s, d impar; VERUM si n probabiliter
 * primus ad basim b */
interior b32
_testis (
    i32 n,
    i32 basis,
    i32 d,
    i32 s)
{
    i32 x = congruentia_potentia(basis % n, (i64)d, n);
    i32 k;

    si (x == I || x == n - I || basis % n == ZEPHYRUM)
    {
        redde VERUM;
    }
    per (k = I; k < s; k++)
    {
        x = congruentia_multiplica(x, x, n);
        si (x == n - I)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
congruentia_est_primus (
    i32 n)
{
    i32 d;
    i32 s = ZEPHYRUM;

    si (n < II)
    {
        redde FALSUM;
    }
    si (n < IV)
    {
        redde VERUM;
    }
    si (n % II == ZEPHYRUM)
    {
        redde FALSUM;
    }
    d = n - I;
    dum (d % II == ZEPHYRUM)
    {
        d /= II;
        s++;
    }
    redde _testis(n, II, d, s) && _testis(n, VII, d, s)
        && _testis(n, LXI, d, s);
}

i32
congruentia_primus_infra (
    i32 limes)
{
    i32 n;

    si (limes <= II)
    {
        redde ZEPHYRUM;
    }
    si (limes == III)
    {
        redde II;
    }
    /* impar maximus < limes */
    n = (limes - I) % II == ZEPHYRUM ? limes - II : limes - I;
    dum (n >= III)
    {
        si (congruentia_est_primus(n))
        {
            redde n;
        }
        n -= II;
    }
    redde II;
}


/* ==================================================
 * Reconstructio Sinica
 * ================================================== */

b32
congruentia_restitue (
    constans i32* residua,
    constans i32* moduli,
             i32  numerus,
             b32  symmetricus,
         Piscina* piscina,
          Magnus* exitus)
{
    Magnus x;
    Magnus productum;
       i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (moduli[k] < II || residua[k] >= moduli[k])
        {
            redde FALSUM;
        }
    }
    si (numerus == ZEPHYRUM)
    {
        *exitus = magnus_ex_s64(ZEPHYRUM);
        redde VERUM;
    }
    /* Garner: x_k = x_(k-1) + M_(k-1) t, t = (r_k - x_(k-1)) M^-1 mod
     * m_k; M^-1 existit sse moduli coprimi */
    x          = magnus_ex_s64((s64)residua[ZEPHYRUM]);
    productum  = magnus_ex_s64((s64)moduli[ZEPHYRUM]);
    per (k = I; k < numerus; k++)
    {
        i32 m = moduli[k];
        i32 inversa;
        i32 t;

        si (!congruentia_inversa(magnus_residuum_parvum(productum, m),
            m,
            &inversa))
        {
            redde FALSUM;
        }
        t = congruentia_multiplica(congruentia_subtrahe(residua[k],
            magnus_residuum_parvum(x, m), m), inversa, m);
        x = magnus_adde(x, magnus_multiplica(productum, magnus_ex_s64(
            (s64)t), piscina), piscina);
        productum = magnus_multiplica(productum, magnus_ex_s64((s64)m),
            piscina);
    }
    /* symmetricus: x > M/2 sse 2x > M */
    si (   symmetricus
        && magnus_compara(magnus_multiplica(x, magnus_ex_s64(II),
        piscina),
            productum) > ZEPHYRUM)
    {
        x = magnus_subtrahe(x, productum, piscina);
    }
    *exitus = x;
    redde VERUM;
}
#undef _testis
#line 1 "knotapel/demo_128_braid_relu/main.c"
/*
 * KNOTAPEL DEMO 128: Braids as Exact ReLU Networks (N1)
 * =====================================================
 *
 * Is a braid word an exact, untrained ReLU network? Each Artin generator
 * acts on Dynnikov coordinates u = (a_1 .. a_{n-2}, b_1 .. b_{n-2}) by a
 * piecewise-linear update built from f+ = max(0, f) and f- = min(0, f)
 * (Thiffeault, "Braids of entangled particle trajectories", Chaos 20
 * (2010), arXiv:0906.3647, eq. 8-15). Here each update is written as a
 * straight-line program over integer registers with two instructions,
 *   AFFINE  dst = sum coef * src
 *   RELU    dst = max(0, sum coef * src)
 * and f- as f - relu(f), so a braid word of length m on n strands is a
 * network of depth O(m) - two ReLU layers per letter - whose state is
 * 2n - 4 coordinates (hidden width at most 2n + 1: the state plus a
 * middle generator's 5 ReLUs), and every letter touches at most 4
 * coordinates. Plan: plan.md (approved 2026-10-15); plexus
 * (house library) supplies the reference action and everything else.
 *
 *   Part A  the layer programs (typed from the paper's equations, not
 *           from lib/plexus.c), an interpreter on exact integers
 *           (Magnus), the anchor: interpreter = ansa_agere on random
 *           words and vectors, n = 3..8, values up to 2^200; operation
 *           counts per generator and the shape of a word's network.
 *   Part B  (T6b) growth census of every braid up to length 8 on B_3
 *           and 6 on B_4.
 *   Part C  (T6b) audit of D106's Burau classification.
 *   Part D  (T6c) the active-ReLU pattern along each iteration.
 *
 * House libraries: plexus (and its closure: magnus, polynomium, matrix,
 * radices, ...), hence latina.h: Roman numerals and Latin keywords are
 * macros here (never name a variable I, V, X, L, C, D or M).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_128_braid_relu/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 */





#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0;
static int n_fail = 0;

static void
check (
    const char *msg,
    int         ok)
{
    if (ok) {
        n_pass++;
        printf("  PASS: %s\n", msg);
    } else {
        n_fail++;
        printf("  FAIL: %s\n", msg);
    }
}

static Piscina *pool;

static void
fatal (
    const char *msg)
{
    printf("  FATAL: %s\n", msg);
    exit(1);
}

/* a Magnus as a C string (chorda is not NUL-terminated); the buffer is
 * reused, so print one at a time */
static const char *
mstr (
    Magnus x)
{
    static char buf[256];
    chorda      c = magnus_ad_chordam(x, pool);
    size_t      n = c.mensura < 255 ? c.mensura : 255;

    memcpy(buf, c.datum, n);
    buf[n] = '\0';
    return buf;
}

static Plexus
braid (
    int         n,
    const char *text)
{
    Plexus p;

    if (!plexus_ex_chorda((i32)n, chorda_ex_literis(text, pool), pool,
            &p)) {
        printf("  FATAL: bad braid %s\n", text);
        exit(1);
    }
    return p;
}

/* ================================================================
 * Part A: the layer programs
 * ================================================================
 *
 * Registers 0 .. 2m-1 hold the coordinates (a_j in register j - 1,
 * b_j in register m + j - 1, m = n - 2); temporaries follow. A letter's
 * program reads the OLD coordinates and writes NEW values into fresh
 * registers; the last instructions are the outputs, and "out" maps each
 * changed coordinate to the register that holds its new value. In a
 * network the outputs are simply the next layer's neurons, so the
 * final copy is bookkeeping, not an operation, and is not counted.
 */

#define OP_AFFINE 0
#define OP_RELU   1
#define MAX_TERMS 8
#define MAX_INSTR 16

typedef struct {
    int  op;
    int  dst;
    int  nterms;
    int  src[MAX_TERMS];
    long coef[MAX_TERMS];
} Instr;

typedef struct {
    int   ninstr;
    Instr in[MAX_INSTR];
    int   nregs;                 /* coordinate registers + temporaries */
    int   nout;
    int   out_coord[4];          /* coordinate register changed ... */
    int   out_reg[4];            /* ... takes the value of this register */
} Program;

static int
emit (
    Program *pr,
    int      op,
    int      nterms,
    const int  *src,
    const long *coef)
{
    Instr *ins = &pr->in[pr->ninstr++];
    int    k;

    if (pr->ninstr > MAX_INSTR || nterms > MAX_TERMS) {
        fatal("program too long");
    }
    ins->op = op;
    ins->dst = pr->nregs++;
    ins->nterms = nterms;
    for (k = 0; k < nterms; k++) {
        ins->src[k] = src[k];
        ins->coef[k] = coef[k];
    }
    return ins->dst;
}

/* shorthands: up to five terms, coefficient 0 ends the list */
static int
affine (
    Program *pr,
    int s0, long c0, int s1, long c1, int s2, long c2, int s3, long c3,
    int s4, long c4)
{
    int  src[5];
    long coef[5];
    int  n = 0;

    if (c0) { src[n] = s0; coef[n++] = c0; }
    if (c1) { src[n] = s1; coef[n++] = c1; }
    if (c2) { src[n] = s2; coef[n++] = c2; }
    if (c3) { src[n] = s3; coef[n++] = c3; }
    if (c4) { src[n] = s4; coef[n++] = c4; }
    return emit(pr, OP_AFFINE, n, src, coef);
}

static int
relu (
    Program *pr,
    int s0, long c0, int s1, long c1, int s2, long c2)
{
    int  src[3];
    long coef[3];
    int  n = 0;

    if (c0) { src[n] = s0; coef[n++] = c0; }
    if (c1) { src[n] = s1; coef[n++] = c1; }
    if (c2) { src[n] = s2; coef[n++] = c2; }
    return emit(pr, OP_RELU, n, src, coef);
}

static void
output (
    Program *pr,
    int      coord,
    int      reg)
{
    pr->out_coord[pr->nout] = coord;
    pr->out_reg[pr->nout] = reg;
    pr->nout++;
}

/* The program for one letter on n strands (eq. 8-15 of the paper, with
 * f- = f - relu(f)). Source indices are 1-based (a_j, b_j, j = 1 ..
 * n-2); A(j), B(j) are their registers. */
static void
build_letter (
    int      n,
    long     letter,
    Program *pr)
{
    int m = n - 2;
    int i = (int)(letter > 0 ? letter : -letter);

#define A(j) ((j) - 1)
#define B(j) (m + (j) - 1)
    memset(pr, 0, sizeof(*pr));
    pr->nregs = 2 * m;
    if (i == 1 && letter > 0) {
        /* (10)  b1' = a1 + b1+,  a1' = -b1 + (b1')+ */
        int t1 = relu(pr, B(1), 1, 0, 0, 0, 0);
        int nb = affine(pr, A(1), 1, t1, 1, 0, 0, 0, 0, 0, 0);
        int t2 = relu(pr, nb, 1, 0, 0, 0, 0);
        int na = affine(pr, B(1), -1, t2, 1, 0, 0, 0, 0, 0, 0);

        output(pr, A(1), na);
        output(pr, B(1), nb);
    } else if (i == 1) {
        /* (14)  b1' = b1+ - a1,  a1' = b1 - (b1')+ */
        int t1 = relu(pr, B(1), 1, 0, 0, 0, 0);
        int nb = affine(pr, t1, 1, A(1), -1, 0, 0, 0, 0, 0, 0);
        int t2 = relu(pr, nb, 1, 0, 0, 0, 0);
        int na = affine(pr, B(1), 1, t2, -1, 0, 0, 0, 0, 0, 0);

        output(pr, A(1), na);
        output(pr, B(1), nb);
    } else if (i == n - 1 && letter > 0) {
        /* (11)  b' = a + b-,  a' = -b + (b')-   (index m) */
        int t1 = relu(pr, B(m), 1, 0, 0, 0, 0);
        int nb = affine(pr, A(m), 1, B(m), 1, t1, -1, 0, 0, 0, 0);
        int t2 = relu(pr, nb, 1, 0, 0, 0, 0);
        int na = affine(pr, B(m), -1, nb, 1, t2, -1, 0, 0, 0, 0);

        output(pr, A(m), na);
        output(pr, B(m), nb);
    } else if (i == n - 1) {
        /* (15)  b' = b- - a,  a' = b - (b')- */
        int t1 = relu(pr, B(m), 1, 0, 0, 0, 0);
        int nb = affine(pr, B(m), 1, t1, -1, A(m), -1, 0, 0, 0, 0);
        int t2 = relu(pr, nb, 1, 0, 0, 0, 0);
        int na = affine(pr, B(m), 1, nb, -1, t2, 1, 0, 0, 0, 0);

        output(pr, A(m), na);
        output(pr, B(m), nb);
    } else if (letter > 0) {
        /* (8)-(9), p = i-1, q = i:
         *   c    = a_p - a_q - b_q+ + b_p-
         *   a_p' = a_p - b_p+ - (b_q+ + c)+
         *   b_p' = b_q + c-
         *   a_q' = a_q - b_q- - (b_p- - c)-
         *   b_q' = b_p - c- */
        int p = i - 1, q = i;
        int bq_pos = relu(pr, B(q), 1, 0, 0, 0, 0);
        int bp_pos = relu(pr, B(p), 1, 0, 0, 0, 0);
        int c = affine(pr, A(p), 1, A(q), -1, bq_pos, -1, B(p), 1,
            bp_pos, -1);
        int u = relu(pr, bq_pos, 1, c, 1, 0, 0);      /* (b_q+ + c)+ */
        int c_pos = relu(pr, c, 1, 0, 0, 0, 0);
        /* (b_p- - c)- = w - relu(w), w = b_p - b_p+ - c */
        int w_pos = relu(pr, B(p), 1, bp_pos, -1, c, -1);
        int nap = affine(pr, A(p), 1, bp_pos, -1, u, -1, 0, 0, 0, 0);
        int nbp = affine(pr, B(q), 1, c, 1, c_pos, -1, 0, 0, 0, 0);
        /* a_q' = a_q - (b_q - b_q+) - (w - w+) */
        int naq_part = affine(pr, A(q), 1, B(q), -1, bq_pos, 1, B(p), -1,
            bp_pos, 1);
        int naq = affine(pr, naq_part, 1, c, 1, w_pos, 1, 0, 0, 0, 0);
        int nbq = affine(pr, B(p), 1, c, -1, c_pos, 1, 0, 0, 0, 0);

        output(pr, A(p), nap);
        output(pr, B(p), nbp);
        output(pr, A(q), naq);
        output(pr, B(q), nbq);
    } else {
        /* (12)-(13):
         *   d    = a_p - a_q + b_q+ - b_p-
         *   a_p' = a_p + b_p+ + (b_q+ - d)+
         *   b_p' = b_q - d+
         *   a_q' = a_q + b_q- + (b_p- + d)-
         *   b_q' = b_p + d+ */
        int p = i - 1, q = i;
        int bq_pos = relu(pr, B(q), 1, 0, 0, 0, 0);
        int bp_pos = relu(pr, B(p), 1, 0, 0, 0, 0);
        int d = affine(pr, A(p), 1, A(q), -1, bq_pos, 1, B(p), -1,
            bp_pos, 1);
        int u = relu(pr, bq_pos, 1, d, -1, 0, 0);     /* (b_q+ - d)+ */
        int d_pos = relu(pr, d, 1, 0, 0, 0, 0);
        /* (b_p- + d)- = w - relu(w), w = b_p - b_p+ + d */
        int w_pos = relu(pr, B(p), 1, bp_pos, -1, d, 1);
        int nap = affine(pr, A(p), 1, bp_pos, 1, u, 1, 0, 0, 0, 0);
        int nbp = affine(pr, B(q), 1, d_pos, -1, 0, 0, 0, 0, 0, 0);
        /* a_q' = a_q + (b_q - b_q+) + (w - w+) */
        int naq_part = affine(pr, A(q), 1, B(q), 1, bq_pos, -1, B(p), 1,
            bp_pos, -1);
        int naq = affine(pr, naq_part, 1, d, 1, w_pos, -1, 0, 0, 0, 0);
        int nbq = affine(pr, B(p), 1, d_pos, 1, 0, 0, 0, 0, 0, 0);

        output(pr, A(p), nap);
        output(pr, B(p), nbp);
        output(pr, A(q), naq);
        output(pr, B(q), nbq);
    }
#undef A
#undef B
}

/* run one letter's program on the coordinate registers (exact) */
static void
run_letter (
    const Program *pr,
    Magnus        *coord,       /* 2m registers, updated in place */
    Magnus        *scratch)     /* pr->nregs registers */
{
    int k;
    int t;
    int m2 = pr->nregs - pr->ninstr;   /* the coordinate registers */

    for (k = 0; k < m2; k++) {
        scratch[k] = coord[k];
    }
    for (k = 0; k < pr->ninstr; k++) {
        const Instr *ins = &pr->in[k];
        Magnus       sum = magnus_ex_s64(0);

        for (t = 0; t < ins->nterms; t++) {
            sum = magnus_adde(sum, magnus_multiplica(scratch[ins->src[t]],
                magnus_ex_s64((s64)ins->coef[t]), pool), pool);
        }
        if (ins->op == OP_RELU && magnus_signum(sum) < 0) {
            sum = magnus_ex_s64(0);
        }
        scratch[ins->dst] = sum;
    }
    for (k = 0; k < pr->nout; k++) {
        coord[pr->out_coord[k]] = scratch[pr->out_reg[k]];
    }
}

/* depth: longest chain of instructions (inputs at depth 0); relu_depth:
 * the most ReLUs on any chain */
static void
program_depth (
    const Program *pr,
    int           *depth,
    int           *relu_depth)
{
    int lvl[64];
    int rl[64];
    int k;
    int t;
    int m2 = pr->nregs - pr->ninstr;

    for (k = 0; k < pr->nregs; k++) {
        lvl[k] = 0;
        rl[k] = 0;
    }
    *depth = 0;
    *relu_depth = 0;
    for (k = 0; k < pr->ninstr; k++) {
        const Instr *ins = &pr->in[k];
        int          l = 0;
        int          r = 0;

        for (t = 0; t < ins->nterms; t++) {
            if (lvl[ins->src[t]] > l) l = lvl[ins->src[t]];
            if (rl[ins->src[t]] > r) r = rl[ins->src[t]];
        }
        lvl[m2 + k] = l + 1;
        rl[m2 + k] = r + (ins->op == OP_RELU ? 1 : 0);
        if (lvl[m2 + k] > *depth) *depth = lvl[m2 + k];
        if (rl[m2 + k] > *relu_depth) *relu_depth = rl[m2 + k];
    }
}

static void
print_program (
    const Program *pr,
    int            n)
{
    int m = n - 2;
    int m2 = pr->nregs - pr->ninstr;
    int k;
    int t;

    for (k = 0; k < pr->ninstr; k++) {
        const Instr *ins = &pr->in[k];

        printf("      t%-2d = %s", ins->dst - m2,
            ins->op == OP_RELU ? "relu(" : "");
        for (t = 0; t < ins->nterms; t++) {
            int  s = ins->src[t];
            long c = ins->coef[t];

            printf("%s", t == 0 ? (c < 0 ? "-" : "") : (c < 0 ? " - " :
                " + "));
            if (c != 1 && c != -1) printf("%ld ", c < 0 ? -c : c);
            if (s < m) printf("a%d", s + 1);
            else if (s < 2 * m) printf("b%d", s - m + 1);
            else printf("t%d", s - m2);
        }
        printf("%s\n", ins->op == OP_RELU ? ")" : "");
    }
    for (k = 0; k < pr->nout; k++) {
        int c = pr->out_coord[k];

        printf("      %s%d' = t%d\n", c < m ? "a" : "b",
            c < m ? c + 1 : c - m + 1, pr->out_reg[k] - m2);
    }
}

/* run a whole word through the programs */
static void
run_word (
    int     n,
    Plexus  p,
    Magnus *coord)
{
    Program pr;
    Magnus  scratch[64];
    i32     k;

    for (k = 0; k < p.longitudo; k++) {
        build_letter(n, (long)p.litterae[k], &pr);
        run_letter(&pr, coord, scratch);
    }
}

static Plexus
random_word (
    Sors *s,
    int   n,
    int   len)
{
    s32    lit[64];
    Plexus p;
    int    k;

    for (k = 0; k < len; k++) {
        s32 i = (s32)sors_intra(s, (i32)(n - 1)) + 1;

        lit[k] = sors_intra(s, 2) == 0 ? i : -i;
    }
    if (!plexus_creare((i32)n, lit, (i32)len, pool, &p)) {
        fatal("random word refused");
    }
    return p;
}

static void
part_a (void)
{
    Sors   s;
    int    trial;
    int    compared = 0;
    int    disagree = 0;
    int    big_compared = 0;
    int    big_disagree = 0;
    Magnus scale;
    int    n;

    printf("\n=== Part A: the layer programs ===\n\n");

    /* the program of a middle generator, printed */
    {
        Program pr;
        int     depth;
        int     rdepth;

        build_letter(4, 2, &pr);
        printf("  sigma_2 on B_4 (registers a1 a2 b1 b2):\n");
        print_program(&pr, 4);
        program_depth(&pr, &depth, &rdepth);
        printf("    %d instructions, depth %d, ReLU depth %d\n\n",
            pr.ninstr, depth, rdepth);
    }

    /* operation counts per generator type */
    {
        const char *names[6] = { "sigma_1", "sigma_1^-1", "sigma_i (mid)",
            "sigma_i^-1 (mid)", "sigma_{n-1}", "sigma_{n-1}^-1" };
        long        letters[6] = { 1, -1, 2, -2, 4, -4 };
        int         k;

        printf("  per generator (n = 5; middle = sigma_2):\n");
        printf("    %-18s %5s %7s %6s %10s %8s\n", "generator", "ReLU",
            "affine", "depth", "ReLU depth", "touches");
        for (k = 0; k < 6; k++) {
            Program pr;
            int     relus = 0;
            int     affs = 0;
            int     depth;
            int     rdepth;
            int     t;

            build_letter(5, letters[k], &pr);
            for (t = 0; t < pr.ninstr; t++) {
                if (pr.in[t].op == OP_RELU) relus++;
                else affs++;
            }
            program_depth(&pr, &depth, &rdepth);
            printf("    %-18s %5d %7d %6d %10d %8d\n", names[k], relus,
                affs, depth, rdepth, pr.nout);
        }
        printf("\n");
    }

    /* structure: every program for n = 3..8 touches <= 4 coordinates
     * with ReLU depth 2; sigma then sigma^-1 through the networks is the
     * identity on random vectors (a group relation, independent of
     * plexus) */
    {
        int shape_ok = 1;
        int inverse_ok = 1;
        int nn;
        int letter;

        sors_seminare(&s, 1280, 1);
        for (nn = 3; nn <= 8; nn++) {
            for (letter = -(nn - 1); letter <= nn - 1; letter++) {
                Program pr;
                Program inv;
                int     depth;
                int     rdepth;
                int     t;

                if (letter == 0) continue;
                build_letter(nn, (long)letter, &pr);
                program_depth(&pr, &depth, &rdepth);
                if (pr.nout > 4 || rdepth != 2) shape_ok = 0;
                build_letter(nn, (long)-letter, &inv);
                for (t = 0; t < 50; t++) {
                    Magnus coord[16];
                    Magnus orig[16];
                    Magnus scratch[64];
                    int    k;

                    for (k = 0; k < 2 * (nn - 2); k++) {
                        orig[k] = coord[k] = magnus_ex_s64(
                            (s64)sors_inter(&s, -50, 50));
                    }
                    run_letter(&pr, coord, scratch);
                    run_letter(&inv, coord, scratch);
                    for (k = 0; k < 2 * (nn - 2); k++) {
                        if (!magnus_aequalis(coord[k], orig[k])) {
                            inverse_ok = 0;
                        }
                    }
                }
            }
        }
        check("every generator program (n = 3..8) touches <= 4 "
            "coordinates, ReLU depth 2", shape_ok);
        check("sigma then sigma^-1 through the networks is the identity "
            "(50 vectors per generator, n = 3..8)", inverse_ok);
    }

    /* the anchor: interpreter = ansa_agere, n = 3..8, random words */
    sors_seminare(&s, 128, 1);
    (void)magnus_ex_chorda(chorda_ex_literis(
        "1606938044258990275541962092341162602522202993782792835301376",
        pool), pool, &scale);                              /* 2^200 */
    for (trial = 0; trial < 4000; trial++) {
        PiscinaNotatio mark = piscina_notare(pool);
        int            nn = 3 + (int)sors_intra(&s, 6);
        int            m = nn - 2;
        Plexus         p = random_word(&s, nn, (int)sors_intra(&s, 20));
        Magnus         a[8];
        Magnus         b[8];
        Magnus         coord[16];
        Ansa           u;
        Ansa           v;
        int            big = trial % 4 == 3;
        int            same = 1;
        int            k;

        for (k = 0; k < m; k++) {
            a[k] = magnus_ex_s64((s64)sors_inter(&s, -1000, 1000));
            b[k] = magnus_ex_s64((s64)sors_inter(&s, -1000, 1000));
            if (big) {
                a[k] = magnus_multiplica(a[k], scale, pool);
                b[k] = magnus_multiplica(b[k], scale, pool);
            }
            coord[k] = a[k];
            coord[m + k] = b[k];
        }
        if (!ansa_ex_coordinatis((i32)nn, a, b, pool, &u)
                || !ansa_agere(u, p, pool, &v)) {
            fatal("plexus refused");
        }
        run_word(nn, p, coord);
        for (k = 0; k < m; k++) {
            if (!magnus_aequalis(coord[k], v.a[k])
                    || !magnus_aequalis(coord[m + k], v.b[k])) {
                same = 0;
            }
        }
        if (big) {
            big_compared++;
            if (!same) big_disagree++;
        } else {
            compared++;
            if (!same) disagree++;
        }
        piscina_reficere(pool, mark);
    }
    printf("  anchor: %d random (word, vector) pairs, n = 3..8, |coord| "
        "< 10^3: %d disagree\n", compared, disagree);
    printf("          %d with |coord| up to 1000 * 2^200: %d disagree\n",
        big_compared, big_disagree);
    check("layer programs = ansa_agere on every random pair (small and "
        "2^200-scaled)", disagree == 0 && big_disagree == 0
        && compared + big_compared == 4000);

    /* the paper's worked example and growth through the network */
    {
        Magnus coord[16];
        Ansa   e1;
        Ansa   v;
        Plexus w = braid(3, "1 -2");
        int    k;
        int    ok = 1;

        (void)ansa_circa(3, 1, pool, &e1);
        coord[0] = e1.a[0];
        coord[1] = e1.b[0];
        for (k = 0; k < 20; k++) {
            run_word(3, w, coord);
        }
        (void)ansa_circa(3, 1, pool, &v);
        for (k = 0; k < 20; k++) {
            (void)ansa_agere(v, w, pool, &v);
        }
        ok = magnus_aequalis(coord[0], v.a[0])
            && magnus_aequalis(coord[1], v.b[0]);
        printf("  (s1 s2^-1)^20 on E_1 through the network: (%s; ",
            mstr(coord[0]));
        printf("%s), L = %s\n", mstr(coord[1]),
            mstr(ansa_sectiones(v, pool)));
        check("(s1 s2^-1)^20 through the network = plexus iteration", ok);
    }

    /* shape of a word's network */
    printf("\n  a word as a network (state 2n - 4 coordinates, hidden "
        "width <= 2n + 1, 2 ReLU layers\n  per letter, each letter "
        "touches <= 4 coordinates):\n");
    printf("    %-34s %3s %7s %7s %9s %11s\n", "word", "n", "letters",
        "ReLUs", "depth", "ReLU depth");
    for (n = 0; n < 3; n++) {
        const char *words[3] = { "1 -2", "-3 -2 -3 2 1",
            "1 1 2 -1 -3 2 -3" };
        int         ns[3] = { 3, 4, 4 };
        Plexus      p = braid(ns[n], words[n]);
        int         relus = 0;
        int         depth = 0;
        int         rdepth = 0;
        i32         k;

        for (k = 0; k < p.longitudo; k++) {
            Program pr;
            int     d;
            int     r;
            int     t;

            build_letter(ns[n], (long)p.litterae[k], &pr);
            for (t = 0; t < pr.ninstr; t++) {
                if (pr.in[t].op == OP_RELU) relus++;
            }
            program_depth(&pr, &d, &r);
            depth += d;
            rdepth += r;
        }
        printf("    %-34s %3d %7u %7d %9d %11d\n", words[n], ns[n],
            p.longitudo, relus, depth, rdepth);
    }
    printf("    (depth adds along the word: letters on far-apart strands "
        "commute and could share a layer; not merged here)\n");
}

/* ================================================================
 * Part B: growth census; Part C: audit of D106's classification
 * ================================================================
 *
 * Distinct braids are enumerated breadth-first: level k holds the
 * braids whose shortest word has k letters, found by appending each
 * generator to every braid of level k-1 and keeping the new equality
 * classes. Equality key: the images of E_1 .. E_{n+1} on D_{n+2}
 * (plexus_aequales_ansis's family, exhaustively congruent with Artin in
 * plexus T2 and in the T5 review). Classes of each braid:
 *   P  periodic, EXACT: beta^n or beta^(n-1) equals a power of Delta^2
 *      (every periodic braid is conjugate to a power of delta = s1..s_{n-1}
 *      or epsilon = s1 delta: Brouwer, Kerekjarto, Eilenberg; cited);
 *   E  positive entropy: CERTIFIED if Burau at t = -1 or at a primitive
 *      cube root of unity has spectral radius > 1 (plexus: Kolev's lower
 *      bound, cited, decided exactly with Kronecker), otherwise
 *      MEASURED: max_k L^(k)_60 / L^(k)_40 > 2 over the loops E_k on
 *      D_n (and then, checked, not quasi-affine and > 10^6);
 *   Z  zero entropy, not periodic: not certified, growth quasi-affine.
 * (periodic braids: Gonzalez-Meneses and Wiest 2004; minimal
 * dilatations: phi^2 on B_3, Ko, Los and Song 2002 on B_4, the largest
 * root of x^4 - 2x^3 - 2x + 1 = 2.29663...)
 * The gap is wide: a pseudo-Anosov piece on <= 4 strands has dilatation
 * >= 2.29 (minimal dilatations of B_3, B_4; cited), so its ratio over
 * 20 periods is > 10^7, while a zero-entropy braid grows at most
 * linearly (ratio about 1.5). */

typedef struct {
    Plexus word;
    int    len;
    chorda key;
    int    cls;          /* CLS_P, CLS_E, CLS_Z */
    int    certified;    /* Burau(-1) radix > 1 */
    int    cert_omega;   /* Burau at a cube root of unity: radius > 1 */
    int    period;       /* P: k with beta^k a power of Delta^2 */
    double rate;         /* measured (L60/L40)^(1/20), display only */
    int    d106_pa;      /* D106's rule says pseudo-Anosov */
    long   tr;           /* trace, c1 of Burau(-1) (B_4 only) */
    long   c1;
    chorda tlkey;        /* D106's TL matrix (Part C) */
} Census;

#define CLS_P 0
#define CLS_E 1
#define CLS_Z 2

static int celer = 0;     /* D128_CELER: B_3 <= 5, B_4 <= 4 */

static chorda
class_key (
    Plexus p)
{
    int                n = (int)p.fila + 2;
    Plexus             big;
    ChordaAedificator *ae = chorda_aedificator_creare(pool, 64);
    int                k;

    if (!plexus_in_maiore(p, (i32)n, 1, pool, &big)) {
        fatal("embedding refused");
    }
    for (k = 1; k < n; k++) {
        Ansa e;
        Ansa img;

        memset(&img, 0, sizeof(img));
        if (!ansa_circa((i32)n, (i32)k, pool, &e)
                || !ansa_agere(e, big, pool, &img)) {
            fatal("loop action refused");
        }
        (void)chorda_aedificator_appendere_chorda(ae,
            ansa_ad_chordam(img, pool));
    }
    return chorda_aedificator_finire(ae);
}

static int
key_cmp (
    const void *x,
    const void *y)
{
    const chorda *a = (const chorda *)x;
    const chorda *b = (const chorda *)y;

    return (int)chorda_comparare(*a, *b);
}

static int
census_cmp (
    const void *x,
    const void *y)
{
    const Census *a = (const Census *)x;
    const Census *b = (const Census *)y;

    return (int)chorda_comparare(a->key, b->key);
}

/* breadth-first enumeration of distinct braids up to length maxlen */
static Census *
enumerate (
    int  n,
    int  maxlen,
    int *count)
{
    Census *all = NULL;
    int     nall = 0;
    int     cap = 0;
    chorda *seen = NULL;     /* sorted keys of every class found */
    int     nseen = 0;
    int     level_start = 0;
    int     level;

    cap = 1024;
    all = (Census *)malloc((size_t)cap * sizeof(Census));
    seen = (chorda *)malloc((size_t)cap * sizeof(chorda));
    memset(&all[0], 0, sizeof(Census));
    all[0].word = braid(n, "");
    all[0].len = 0;
    all[0].key = class_key(all[0].word);
    nall = 1;
    seen[0] = all[0].key;
    nseen = 1;
    for (level = 1; level <= maxlen; level++) {
        int     level_end = nall;
        int     first_new = nall;
        int     i;
        int     letter;

        for (i = level_start; i < level_end; i++) {
            for (letter = -(n - 1); letter <= n - 1; letter++) {
                s32    lit;
                Plexus g;
                Plexus w;
                chorda key;

                if (letter == 0) continue;
                lit = (s32)letter;
                (void)plexus_creare((i32)n, &lit, 1, pool, &g);
                (void)plexus_iungere(all[i].word, g, pool, &w);
                key = class_key(w);
                if (bsearch(&key, seen, (size_t)nseen, sizeof(chorda),
                        key_cmp) != NULL) {
                    continue;
                }
                /* new at this level (may repeat within the level) */
                if (nall == cap) {
                    cap *= 2;
                    all = (Census *)realloc(all, (size_t)cap
                        * sizeof(Census));
                    seen = (chorda *)realloc(seen, (size_t)cap
                        * sizeof(chorda));
                }
                memset(&all[nall], 0, sizeof(Census));
                all[nall].word = plexus_reducere(w, pool);
                all[nall].len = level;
                all[nall].key = key;
                nall++;
            }
        }
        /* dedupe this level's new classes by key */
        qsort(all + first_new, (size_t)(nall - first_new), sizeof(Census),
            census_cmp);
        {
            int w = first_new;
            int r;

            for (r = first_new; r < nall; r++) {
                if (w > first_new && chorda_aequalis(all[r].key,
                        all[w - 1].key)) {
                    continue;
                }
                all[w++] = all[r];
            }
            nall = w;
        }
        for (i = first_new; i < nall; i++) {
            seen[nseen++] = all[i].key;
        }
        qsort(seen, (size_t)nseen, sizeof(chorda), key_cmp);
        level_start = first_new;
    }
    *count = nall;
    return all;
}

/* beta^k as a word */
static Plexus
power (
    Plexus p,
    int    k)
{
    Plexus r = braid((int)p.fila, "");
    int    t;

    for (t = 0; t < k; t++) {
        (void)plexus_iungere(r, p, pool, &r);
    }
    return r;
}

/* periodic, exactly: beta^k = Delta^(2j) for k = n or n-1; returns
 * that k, or 0 */
static int
is_periodic (
    Plexus p)
{
    int    n = (int)p.fila;
    long   e = (long)plexus_summa_exponentium(p);
    int    ks[2];
    int    t;
    Plexus delta;
    Plexus twist;

    ks[0] = n;
    ks[1] = n - 1;
    (void)plexus_dimidia_torsio((i32)n, pool, &delta);
    (void)plexus_iungere(delta, delta, pool, &twist);
    for (t = 0; t < 2; t++) {
        long   num = (long)ks[t] * e;
        long   den = (long)n * (long)(n - 1);
        long   j;
        Plexus lhs;
        Plexus rhs;
        b32    same = FALSUM;

        if (num % den != 0) continue;
        j = num / den;
        lhs = power(p, ks[t]);
        rhs = power(j >= 0 ? twist : plexus_inversum(twist, pool),
            (int)(j >= 0 ? j : -j));
        if (!plexus_aequales_ansis(lhs, rhs, pool, &same)) {
            fatal("equality refused");
        }
        if (same) return ks[t];
    }
    return 0;
}

/* x^(1/20) by bisection (display only, no libm) */
static double
root20 (
    double x)
{
    double lo = 1.0;
    double hi = x > 1.0 ? x : 1.0;
    int    it;

    for (it = 0; it < 200; it++) {
        double mid = 0.5 * (lo + hi);
        double p = 1.0;
        int    k;

        for (k = 0; k < 20; k++) p *= mid;
        if (p > x) hi = mid; else lo = mid;
    }
    return lo;
}

/* L60 / L40 as a double from the leading digits (display only) */
static double
ratio_display (
    Magnus num,
    Magnus den)
{
    char   a[256];
    char   b[256];
    size_t la;
    size_t lb;
    double x;
    double y;
    long   shift;

    strcpy(a, mstr(num));
    strcpy(b, mstr(den));
    la = strlen(a);
    lb = strlen(b);
    /* compare the first 15 digits, then scale by 10^(la - lb) */
    a[la > 15 ? 15 : la] = '\0';
    b[lb > 15 ? 15 : lb] = '\0';
    x = atof(a);
    y = atof(b);
    shift = (long)(la > 15 ? la - 15 : 0) - (long)(lb > 15 ? lb - 15 : 0);
    while (shift > 0) { x *= 10.0; shift--; }
    while (shift < 0) { y *= 10.0; shift++; }
    return y > 0.0 ? x / y : 0.0;
}

/* growth over 60 periods on every E_k of D_n. Returns 1 if some
 * L^(k)_60 > 2 L^(k)_40 (exact comparison); *rate = the largest
 * (L60/L40)^(1/20) (display). Exact characterizations, for the
 * consistency checks:
 *   *repeats = 1 if L^(k)_{m+period} = L^(k)_m for all m (period > 0);
 *   *affine  = 1 if for every loop some p <= 24 makes L^(k)_{m+p} -
 *              L^(k)_m constant for m = 30 .. 60 - p (quasi-affine) */
static int
grows_exponentially (
    Plexus  p,
    int     period,
    double *rate,
    int    *repeats,
    int    *affine,
    int    *wide)
{
    int    n = (int)p.fila;
    int    k;
    int    expo = 0;
    double best = 1.0;

    *repeats = period > 0;
    *affine = 1;
    *wide = 0;
    for (k = 1; k < n; k++) {
        Ansa    e;
        Magnus *lengths = NULL;
        double  r;
        int     m;

        if (!ansa_circa((i32)n, (i32)k, pool, &e)
                || !plexus_incrementum(p, e, 60, pool, &lengths)) {
            fatal("growth refused");
        }
        if (magnus_compara(lengths[60], magnus_multiplica(lengths[40],
                magnus_ex_s64(2), pool)) > 0) {
            expo = 1;
        }
        /* wide margin: lambda >= 2.29 gives lambda^20 > 1.6 * 10^7 */
        if (magnus_compara(lengths[60], magnus_multiplica(lengths[40],
                magnus_ex_s64(1000000), pool)) > 0) {
            *wide = 1;
        }
        r = root20(ratio_display(lengths[60], lengths[40]));
        if (r > best) best = r;
        if (period > 0) {
            for (m = 0; m + period <= 60; m++) {
                if (!magnus_aequalis(lengths[m + period], lengths[m])) {
                    *repeats = 0;
                }
            }
        }
        {
            int found = 0;
            int q;

            for (q = 1; q <= 24 && !found; q++) {
                Magnus step = magnus_subtrahe(lengths[30 + q], lengths[30],
                    pool);
                int    same = 1;

                for (m = 30; m + q <= 60; m++) {
                    if (!magnus_aequalis(magnus_subtrahe(lengths[m + q],
                            lengths[m], pool), step)) {
                        same = 0;
                    }
                }
                if (same) found = 1;
            }
            if (!found) *affine = 0;
        }
    }
    *rate = best;
    return expo;
}

/* D106's rule on B_4: p(x) = det(xI - M) = x^3 - tr x^2 + c1 x - det,
 * det = 1 (checked); "pA" iff p(1) = c1 - tr != 0, else iff
 * |1 - tr| > 2 */
static void
d106_rule (
    Census *c)
{
    Matrix m;
    long   a[3][3];
    int    i;
    int    j;
    long   det;

    if (!plexus_burau_minus_uno(c->word, pool, &m)) {
        fatal("Burau(-1) refused");
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            s64 v = 0;

            if (!magnus_ad_s64(*(const Magnus *)matrix_elementum(m,
                    (i32)i, (i32)j), &v)) {
                fatal("Burau(-1) entry beyond s64");
            }
            a[i][j] = (long)v;
        }
    }
    c->tr = a[0][0] + a[1][1] + a[2][2];
    c->c1 = a[0][0] * a[1][1] - a[0][1] * a[1][0]
        + a[0][0] * a[2][2] - a[0][2] * a[2][0]
        + a[1][1] * a[2][2] - a[1][2] * a[2][1];
    det = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
        - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0])
        + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
    if (det != 1) {
        fatal("Burau(-1) det != 1 on B_4");
    }
    if (c->c1 - c->tr != 0) {
        c->d106_pa = 1;
    } else {
        long s = 1 - c->tr;

        c->d106_pa = (s > 2 || s < -2) ? 1 : 0;
    }
}

static const char *
word_text (
    Plexus p)
{
    static char buf[256];
    chorda      c = plexus_ad_chordam(p, pool);
    size_t      n = c.mensura < 255 ? c.mensura : 255;

    if (n == 0) {
        return "(empty)";
    }
    memcpy(buf, c.datum, n);
    buf[n] = '\0';
    return buf;
}

static RadixRealis
radix_one (void)
{
    return radix_ex_fractione(fractio_ex_s64(1), pool);
}

/* classify every census entry */
static void
classify (
    Census *cs,
    int     count,
    int    *consistent)
{
    int i;

    *consistent = 1;
    for (i = 0; i < count; i++) {
        RadixRealis rho;
        s32         sg = 0;
        int         periodic = is_periodic(cs[i].word);
        int         repeats;
        int         affine;
        int         wide;
        int         expo = grows_exponentially(cs[i].word, periodic,
            &cs[i].rate, &repeats, &affine, &wide);

        if (!plexus_burau_radix_maxima(cs[i].word, pool, &rho)
                || !radix_compara(rho, radix_one(), pool, &sg)) {
            fatal("radix refused");
        }
        cs[i].certified = sg > 0;
        cs[i].period = periodic;
        {
            b32 maior = FALSUM;

            if (!plexus_burau_radius_maior(cs[i].word, 3, &maior)) {
                fatal("certificate refused");
            }
            cs[i].cert_omega = maior ? 1 : 0;
        }
        if (periodic) {
            cs[i].cls = CLS_P;
            /* periodic: the lengths repeat with the period; no
             * certificate (Kolev) */
            if (expo || !repeats || cs[i].certified || cs[i].cert_omega) {
                *consistent = 0;
            }
        } else if (cs[i].certified || cs[i].cert_omega || expo) {
            cs[i].cls = CLS_E;
            /* a certificate must show in the growth; measured growth
             * must not be quasi-affine and must clear a wide margin */
            if (!expo || affine || !wide) {
                *consistent = 0;
            }
        } else {
            cs[i].cls = CLS_Z;
            /* zero entropy: eventually quasi-affine growth */
            if (!affine) {
                *consistent = 0;
            }
        }
    }
}

static void
census_table (
    Census *cs,
    int     count,
    int     maxlen)
{
    int len;

    printf("    %4s %8s %6s %11s %11s %10s %6s %9s\n", "len", "braids",
        "P", "E cert t=-1", "E cert t=w", "E measured", "Z", "E share");
    for (len = 0; len <= maxlen; len++) {
        int tot = 0, p = 0, ec = 0, ew = 0, em = 0, z = 0;
        int i;

        for (i = 0; i < count; i++) {
            if (cs[i].len != len) continue;
            tot++;
            if (cs[i].cls == CLS_P) p++;
            else if (cs[i].cls == CLS_Z) z++;
            else if (cs[i].certified) ec++;
            else if (cs[i].cert_omega) ew++;
            else em++;
        }
        printf("    %4d %8d %6d %11d %11d %10d %6d %8.1f%%\n", len, tot, p,
            ec, ew, em, z, tot ? 100.0 * (ec + ew + em) / tot : 0.0);
    }
}

static void
part_b (
    int      n,
    int      maxlen,
    int      expected,
    Census **out,
    int     *out_count)
{
    Census *cs;
    int     count;
    int     consistent;
    int     i;
    double  min_rate = 1e300;
    int     min_i = -1;
    char    msg[256];

    printf("\n=== Part B: growth census, B_%d up to length %d ===\n\n", n,
        maxlen);
    cs = enumerate(n, maxlen, &count);
    printf("  %d distinct braids (breadth-first, exact classes)\n", count);
    if (expected > 0) {
        sprintf(msg, "B_%d up to length %d: %d distinct braids (the T5 "
            "review's independent Artin count, or its prefix)", n, maxlen,
            expected);
        check(msg, count == expected);
    }
    classify(cs, count, &consistent);
    census_table(cs, count, maxlen);
    if (n == 4) {
        /* delta, epsilon periodic; Thiffeault's braid positive entropy */
        const char *periodic_words[2] = { "1 2 3", "1 1 2 3" };
        int         k;
        int         ok = 1;
        int         found = 0;

        for (k = 0; k < 2; k++) {
            if (!is_periodic(braid(4, periodic_words[k]))) ok = 0;
        }
        for (i = 0; i < count; i++) {
            if (chorda_aequalis(cs[i].key, class_key(braid(4,
                    "-3 -2 -3 2 1")))) {
                found = 1;
                if (cs[i].cls != CLS_E || cs[i].rate < 2.2965
                        || cs[i].rate > 2.2968) ok = 0;
            }
        }
        if (maxlen < 5) found = 1;   /* length 5: full mode only */
        check("B_4: delta = s1 s2 s3 and epsilon = s1 delta periodic "
            "(exact); s3^-1 s2^-1 s3^-1 s2 s1 positive entropy, rate "
            "2.2966", ok && found);
    }
    for (i = 0; i < count; i++) {
        if (cs[i].cls == CLS_E && cs[i].rate < min_rate) {
            min_rate = cs[i].rate;
            min_i = i;
        }
    }
    if (min_i >= 0) {
        printf("  smallest measured rate among E: %.5f (%s)\n", min_rate,
            word_text(cs[min_i].word));
    }
    sprintf(msg, "B_%d: periodic braids' loop lengths repeat with their "
        "period, uncertified; certified and measured E grow beyond "
        "10^6 and not quasi-affinely; Z grows quasi-affinely", n);
    check(msg, consistent);
    /* pinned per-level class counts: the D128 review's independent
     * oracle (free-group growth, periodicity for every k <= 12) */
    {
        int p3[9] = { 1, 0, 4, 10, 20, 16, 34, 32, 92 };
        int e3[9] = { 0, 0, 4, 12, 32, 92, 248, 544, 1176 };
        int z3[9] = { 0, 4, 4, 8, 16, 40, 32, 80, 88 };
        int p4[7] = { 1, 0, 0, 8, 24, 36, 102 };
        int e4[7] = { 0, 0, 8, 48, 198, 802, 2958 };
        int z4[7] = { 0, 6, 18, 42, 116, 272, 482 };
        int same = 1;
        int measured_only = 0;
        int len;

        for (len = 0; len <= maxlen; len++) {
            int pp = 0, ee = 0, zz = 0;

            for (i = 0; i < count; i++) {
                if (cs[i].len != len) continue;
                if (cs[i].cls == CLS_P) pp++;
                else if (cs[i].cls == CLS_E) ee++;
                else zz++;
            }
            if (n == 3 && (pp != p3[len] || ee != e3[len]
                    || zz != z3[len])) {
                same = 0;
            }
            if (n == 4 && (pp != p4[len] || ee != e4[len]
                    || zz != z4[len])) {
                same = 0;
            }
        }
        for (i = 0; i < count; i++) {
            if (cs[i].cls == CLS_E && !cs[i].certified
                    && !cs[i].cert_omega) {
                measured_only++;
            }
        }
        sprintf(msg, "B_%d: per-level P / E / Z counts = the review's "
            "independent counts", n);
        check(msg, same);
        sprintf(msg, "B_%d: every positive-entropy braid is CERTIFIED "
            "(Burau at t = -1 or at a cube root of unity): %d measured "
            "only", n, measured_only);
        check(msg, measured_only == 0);
        check(n == 3 ? "B_3: the identity is periodic"
            : "B_4: the identity is periodic", cs[0].len == 0
            && cs[0].cls == CLS_P);
    }
    /* distinct classes are unequal under Artin (sample) */
    {
        Sors s;
        int  t;
        int  wrong = 0;

        sors_seminare(&s, (i64)(1000 + n), 1);
        for (t = 0; t < 300 && count > 1; t++) {
            int a = (int)sors_intra(&s, (i32)count);
            int b = (int)sors_intra(&s, (i32)count);
            b32 same = FALSUM;

            if (a == b) continue;
            if (!plexus_aequales(cs[a].word, cs[b].word, 100000000UL,
                    pool, &same) || same) {
                wrong++;
            }
        }
        sprintf(msg, "B_%d: 300 random pairs of distinct classes are "
            "unequal under Artin (plexus_aequales)", n);
        check(msg, wrong == 0);
    }
    *out = cs;
    *out_count = count;
}

/* D106's TL representation of B_4 on W_{4,2} (D106 main.c, from
 * D100): 3x3 over Z[zeta_8], A = -zeta_8, A^-1 = conj(A);
 * sigma_i = A I + A^-1 e_i, sigma_i^-1 = A^-1 I + A e_i with
 * e_1 = E(0,1), e_2 = E(1,0) + E(1,2), e_3 = E(2,1). Here only to
 * replicate D106's catalog, which deduplicated by this matrix ALONE
 * (D106 main.c:409, "dedup by TL matrix"). */
typedef struct {
    Cyclotomicus m[3][3];
} TL3;

static Cyclotomia *z8;

static TL3
tl_mul (
    const TL3 *a,
    const TL3 *b)
{
    TL3 r;
    int i;
    int j;
    int k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            Cyclotomicus sum = cyclotomicus_nullum(z8);

            for (k = 0; k < 3; k++) {
                sum = cyclotomicus_adde(sum, cyclotomicus_multiplica(
                    a->m[i][k], b->m[k][j], pool), pool);
            }
            r.m[i][j] = sum;
        }
    }
    return r;
}

static TL3
tl_generator (
    int letter)        /* +-1, +-2, +-3 */
{
    TL3          g;
    Cyclotomicus a = cyclotomicus_nega(cyclotomicus_radix(z8, 1, pool),
        pool);                                         /* -zeta */
    Cyclotomicus ai = cyclotomicus_conjugatum(a, pool);
    Cyclotomicus diag = letter > 0 ? a : ai;
    Cyclotomicus off = letter > 0 ? ai : a;
    int          i = letter > 0 ? letter : -letter;
    int          r;
    int          c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            g.m[r][c] = r == c ? diag : cyclotomicus_nullum(z8);
        }
    }
    if (i == 1) {
        g.m[0][1] = cyclotomicus_adde(g.m[0][1], off, pool);
    } else if (i == 2) {
        g.m[1][0] = cyclotomicus_adde(g.m[1][0], off, pool);
        g.m[1][2] = cyclotomicus_adde(g.m[1][2], off, pool);
    } else {
        g.m[2][1] = cyclotomicus_adde(g.m[2][1], off, pool);
    }
    return g;
}

static chorda
tl_key (
    const TL3 *t)
{
    ChordaAedificator *ae = chorda_aedificator_creare(pool, 64);
    int                r;
    int                c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            (void)chorda_aedificator_appendere_chorda(ae,
                cyclotomicus_ad_chordam(t->m[r][c], pool));
            (void)chorda_aedificator_appendere_character(ae, '|');
        }
    }
    return chorda_aedificator_finire(ae);
}

static TL3
tl_of_word (
    Plexus p)
{
    TL3 t = tl_generator(1);
    i32 k;
    int r;
    int c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            t.m[r][c] = r == c ? cyclotomicus_integer(z8,
                magnus_ex_s64(1), pool) : cyclotomicus_nullum(z8);
        }
    }
    for (k = 0; k < p.longitudo; k++) {
        TL3 g = tl_generator((int)p.litterae[k]);

        t = tl_mul(&t, &g);
    }
    return t;
}

/* D106's BFS replicated: level by level, entries of the previous level
 * in catalog order, generators in D106's order (s1, s1^-1, s2, s2^-1,
 * s3, s3^-1), right multiplication, kept iff the TL matrix is new */
static void
d106_bfs (
    int     maxdepth,
    int    *level_count,
    int    *level_pa,
    Plexus *first,        /* out: the catalog's first nfirst words */
    int     nfirst)
{
    TL3    *tls = NULL;
    Plexus *words = NULL;
    chorda *keys = NULL;    /* unsorted, parallel to tls */
    chorda *seen = NULL;    /* sorted */
    int     n = 0;
    int     cap = 4096;
    int     nseen = 0;
    int     start = 0;
    int     depth;
    int     gens[6] = { 1, -1, 2, -2, 3, -3 };

    tls = (TL3 *)malloc((size_t)cap * sizeof(TL3));
    words = (Plexus *)malloc((size_t)cap * sizeof(Plexus));
    keys = (chorda *)malloc((size_t)cap * sizeof(chorda));
    seen = (chorda *)malloc((size_t)cap * sizeof(chorda));
    words[0] = braid(4, "");
    tls[0] = tl_of_word(words[0]);
    keys[0] = tl_key(&tls[0]);
    seen[0] = keys[0];
    nseen = 1;
    n = 1;
    level_count[0] = 1;
    level_pa[0] = 0;
    for (depth = 1; depth <= maxdepth; depth++) {
        int end = n;
        int i;
        int g;

        for (i = start; i < end; i++) {
            for (g = 0; g < 6; g++) {
                TL3    gen = tl_generator(gens[g]);
                TL3    prod = tl_mul(&tls[i], &gen);
                chorda key = tl_key(&prod);
                s32    lit = (s32)gens[g];
                Plexus one;
                int    lo = 0;
                int    hi = nseen;
                int    found = 0;
                int    k;

                /* seen is sorted: binary search, then linear insert */
                while (lo < hi) {
                    int mid = (lo + hi) / 2;
                    int cmp = (int)chorda_comparare(seen[mid], key);

                    if (cmp == 0) { found = 1; break; }
                    if (cmp < 0) lo = mid + 1; else hi = mid;
                }
                if (found) continue;
                if (n == cap) {
                    cap *= 2;
                    tls = (TL3 *)realloc(tls, (size_t)cap * sizeof(TL3));
                    words = (Plexus *)realloc(words, (size_t)cap
                        * sizeof(Plexus));
                    keys = (chorda *)realloc(keys, (size_t)cap
                        * sizeof(chorda));
                    seen = (chorda *)realloc(seen, (size_t)cap
                        * sizeof(chorda));
                }
                (void)plexus_creare(4, &lit, 1, pool, &one);
                (void)plexus_iungere(words[i], one, pool, &words[n]);
                tls[n] = prod;
                keys[n] = key;
                for (k = nseen; k > lo; k--) {
                    seen[k] = seen[k - 1];
                }
                seen[lo] = key;
                nseen++;
                n++;
            }
        }
        level_count[depth] = n - end;
        level_pa[depth] = 0;
        for (i = end; i < n; i++) {
            Census c;

            memset(&c, 0, sizeof(c));
            c.word = words[i];
            d106_rule(&c);
            if (c.d106_pa) level_pa[depth]++;
        }
        start = end;
    }
    free(tls);
    for (depth = 0; depth < nfirst && depth < n; depth++) {
        first[depth] = words[depth];
    }
    free(words);
    free(keys);
    free(seen);
}

static int
tlkey_cmp (
    const void *x,
    const void *y)
{
    const Census *a = (const Census *)x;
    const Census *b = (const Census *)y;

    return (int)chorda_comparare(a->tlkey, b->tlkey);
}

static void
part_c (
    Census *cs,
    int     count,
    int     maxlen)
{
    int  cross[2][3];
    int  i;
    int  r;
    int  c;
    int  first[2][3];
    char msg[256];
    int  p1_zero = 1;

    printf("\n=== Part C: D106's rule against the exact classes "
        "(B_4 up to length %d) ===\n\n", maxlen);
    memset(cross, 0, sizeof(cross));
    for (r = 0; r < 2; r++) for (c = 0; c < 3; c++) first[r][c] = -1;
    for (i = 0; i < count; i++) {
        d106_rule(&cs[i]);
        if (cs[i].c1 - cs[i].tr != 0) p1_zero = 0;
        r = cs[i].d106_pa ? 0 : 1;
        cross[r][cs[i].cls]++;
        if (first[r][cs[i].cls] < 0) first[r][cs[i].cls] = i;
    }
    /* lemma (D128 review, L1): on B_4 at t = -1 every reduced Burau
     * matrix has eigenvalue 1 - unreduced Burau fixes (1,1,1,1), which
     * lies in ker (1, t, t^2, t^3) when t^4 = 1, t != 1. So p(1) = 0,
     * D106's first branch never fires, and the other two eigenvalues
     * are real or on the unit circle */
    check("B_4: p(1) = 0 for the Burau(-1) of every braid (D106's first "
        "branch never fires)", p1_zero);
    printf("    %-24s %8s %8s %8s\n", "D106's rule says", "P", "Z", "E");
    printf("    %-24s %8d %8d %8d\n", "pseudo-Anosov", cross[0][CLS_P],
        cross[0][CLS_Z], cross[0][CLS_E]);
    printf("    %-24s %8d %8d %8d\n", "periodic", cross[1][CLS_P],
        cross[1][CLS_Z], cross[1][CLS_E]);
    printf("  (P periodic, exact; Z zero entropy, not periodic; E positive "
        "entropy, certified)\n");
    if (maxlen >= 6) {
        check("cross table = the review's independent table: 'pA' row "
            "0 / 0 / 3006, 'periodic' row 171 / 936 / 1008",
            cross[0][CLS_P] == 0 && cross[0][CLS_Z] == 0
            && cross[0][CLS_E] == 3006 && cross[1][CLS_P] == 171
            && cross[1][CLS_Z] == 936 && cross[1][CLS_E] == 1008);
    }
    printf("  shortest disagreement (breadth-first order):\n");
    if (first[1][CLS_E] >= 0) {
        Census *d = &cs[first[1][CLS_E]];

        printf("    D106 'periodic', positive entropy: %s (rate %.5f; "
            "Burau(-1) char. poly (x-1)^3: tr %ld, c1 %ld; certified by "
            "Burau at a cube root of unity: %s)\n", word_text(d->word),
            d->rate, d->tr, d->c1, d->cert_omega ? "yes" : "no");
    }
    /* what the rule decides: p(1) = 0 always, so 'pA' iff |1 - tr| > 2
     * iff an eigenvalue is real with modulus > 1 iff Burau(-1) radius
     * > 1 - exactly the plexus certificate at t = -1 */
    {
        int same = 1;

        for (i = 0; i < count; i++) {
            if (cs[i].d106_pa != cs[i].certified) same = 0;
        }
        check("D106's 'pA' = Burau(-1) spectral radius > 1 on every "
            "braid", same);
    }
    /* by length: D106's rule on the exact set against certified E */
    {
        int len;

        printf("\n    %4s %8s %16s %16s\n", "len", "braids",
            "D106 rule 'pA'", "E (certified)");
        for (len = 0; len <= maxlen; len++) {
            int tot = 0, pa = 0, e = 0;

            for (i = 0; i < count; i++) {
                if (cs[i].len != len) continue;
                tot++;
                if (cs[i].d106_pa) pa++;
                if (cs[i].cls == CLS_E) e++;
            }
            printf("    %4d %8d %15.1f%% %15.1f%%\n", len, tot,
                tot ? 100.0 * pa / tot : 0.0, tot ? 100.0 * e / tot : 0.0);
        }
        printf("    (the 'pA' column is D106's RULE applied to the exact "
            "braids; D106's published\n     share differs only at depth "
            "6, where its catalog is smaller: see below)\n");
    }
    /* D106's catalog: replicate its TL-only BFS */
    if (maxlen >= 6) {
        int d106_total[7] = { 1, 6, 26, 98, 338, 1110, 3444 };
        int d106_pa[7]    = { 0, 0, 8, 32, 146, 562, 2234 };
        int lc[7];
        int lp[7];
        int same = 1;
        int len;
        int tot6 = 0;
        int pa6 = 0;

        Plexus head[128];

        d106_bfs(6, lc, lp, head, 128);
        for (len = 0; len <= 6; len++) {
            if (lc[len] != d106_total[len] || lp[len] != d106_pa[len]) {
                same = 0;
            }
        }
        for (i = 0; i < count; i++) {
            if (cs[i].len == 6) {
                tot6++;
                if (cs[i].d106_pa) pa6++;
            }
        }
        printf("\n  D106's catalog, replicated (TL-only dedup): %d %d %d "
            "%d %d %d %d braids, %d %d %d %d %d %d %d 'pA'\n", lc[0],
            lc[1], lc[2], lc[3], lc[4], lc[5], lc[6], lp[0], lp[1], lp[2],
            lp[3], lp[4], lp[5], lp[6]);
        check("D106's table, depths 0-6, is reproduced EXACTLY by its "
            "TL-only breadth-first search", same);
        check("exact census at length 6: 3542 braids, 2258 'pA' under "
            "D106's rule", tot6 == 3542 && pa6 == 2258);
        /* the merges: exact classes sharing a TL matrix */
        {
            int pairs = 0;
            int all_len6 = 1;
            int same_burau = 1;
            int bigger = 0;

            Census *cs_sorted = (Census *)malloc((size_t)count
                * sizeof(Census));

            for (i = 0; i < count; i++) {
                TL3 t = tl_of_word(cs[i].word);

                cs[i].tlkey = tl_key(&t);
            }
            /* sort a copy: the census order stays breadth-first */
            memcpy(cs_sorted, cs, (size_t)count * sizeof(Census));
            qsort(cs_sorted, (size_t)count, sizeof(Census), tlkey_cmp);
            for (i = 0; i < count; ) {
                int j = i + 1;

                while (j < count && chorda_aequalis(cs_sorted[j].tlkey,
                        cs_sorted[i].tlkey)) {
                    j++;
                }
                if (j - i == 2) {
                    pairs++;
                    if (cs_sorted[i].len != 6 || cs_sorted[i + 1].len != 6) {
                        all_len6 = 0;
                    }
                    if (cs_sorted[i].tr != cs_sorted[i + 1].tr
                            || cs_sorted[i].c1 != cs_sorted[i + 1].c1) {
                        same_burau = 0;
                    }
                    if (pairs == 1) {
                        printf("  first merged pair: %s", word_text(
                            cs_sorted[i].word));
                        printf("  and  %s\n", word_text(cs_sorted[i + 1].word));
                    }
                } else if (j - i > 2) {
                    bigger++;
                }
                i = j;
            }
            free(cs_sorted);
            printf("  exact classes sharing a TL matrix: %d pairs, %d "
                "larger groups\n", pairs, bigger);
            check("exactly 98 pairs of distinct braids share a TL matrix, "
                "all at length 6, with equal Burau(-1) char. poly: "
                "3542 - 98 = 3444", pairs == 98 && bigger == 0 && all_len6
                && same_burau);
        }
    }
    /* D106's contingency table used its catalog's first 128 entries:
     * depths 0-2 and all but the last 3 of depth 3 (1 + 6 + 26 + 98 =
     * 131). Classify exactly those. The split does not depend on the
     * generator order (plants: three other orders give the same) */
    if (maxlen >= 6) {
        Plexus  head[128];
        int     lc[7];
        int     lp[7];
        Census *by_key = (Census *)malloc((size_t)count * sizeof(Census));
        int     rule[2] = { 0, 0 };    /* 'pA', 'periodic' (non-identity) */
        int     exact[2][3];
        int     h;

        memset(exact, 0, sizeof(exact));
        memcpy(by_key, cs, (size_t)count * sizeof(Census));
        qsort(by_key, (size_t)count, sizeof(Census), census_cmp);
        d106_bfs(4, lc, lp, head, 128);
        for (h = 1; h < 128; h++) {
            Census  probe;
            Census *hit;

            memset(&probe, 0, sizeof(probe));
            probe.key = class_key(head[h]);
            hit = (Census *)bsearch(&probe, by_key, (size_t)count,
                sizeof(Census), census_cmp);
            if (hit == NULL) fatal("catalog entry not in the census");
            r = hit->d106_pa ? 0 : 1;
            rule[r]++;
            exact[r][hit->cls]++;
        }
        free(by_key);
        printf("\n  D106's first 128 catalog entries (its contingency "
            "table): identity + %d 'periodic' + %d 'pA'\n", rule[1],
            rule[0]);
        printf("    exact classes of its 'periodic' entries: P %d, Z %d, "
            "E %d; of its 'pA' entries: E %d\n", exact[1][CLS_P],
            exact[1][CLS_Z], exact[1][CLS_E], exact[0][CLS_E]);
        check("D106's first 128 entries split as its table: identity, 88 "
            "'periodic', 39 'pA'",
            rule[1] == 88 && rule[0] == 39 && exact[0][CLS_E] == 39);
        /* regression pin (D128's own exact classes, not D106's) */
        check("its 88 'periodic' entries are exactly P 8, Z 64, E 16",
            exact[1][CLS_P] == 8 && exact[1][CLS_Z] == 64
            && exact[1][CLS_E] == 16);
    }
    /* P2, registered: both directions */
    printf("\n  P2 (registered): zero-entropy braids called 'pA': %d (this "
        "direction was impossible:\n      'pA' means Burau(-1) radius "
        "> 1, which forces positive entropy by Kolev's bound);\n"
        "      positive-entropy braids called 'periodic': %d\n",
        cross[0][CLS_P] + cross[0][CLS_Z], cross[1][CLS_E]);
    /* D106's depth-2 claim */
    {
        int  len2_pa = 0;
        int  len2_ok = 1;
        int  len2_reducible = 1;
        RadixRealis phi2;
        Polynomium  f;
        RadixRealis *roots;
        i32          nroots;

        (void)polynomium_ex_chorda(chorda_ex_literis("t^2 - 3t + 1", pool),
            't', pool, &f);
        (void)radices_reales(f, pool, &roots, &nroots);
        phi2 = roots[nroots - 1];
        for (i = 0; i < count; i++) {
            if (cs[i].len == 2 && cs[i].d106_pa) {
                RadixRealis rho;
                s32         sg = 1;
                s32         a = cs[i].word.litterae[0];
                s32         b = cs[i].word.litterae[1];
                s32         ia = a > 0 ? a : -a;
                s32         ib = b > 0 ? b : -b;

                len2_pa++;
                (void)plexus_burau_radix_maxima(cs[i].word, pool, &rho);
                (void)radix_compara(rho, phi2, pool, &sg);
                if (cs[i].tr != 4 || cs[i].c1 != 4 || sg != 0
                        || cs[i].cls != CLS_E) {
                    len2_ok = 0;
                }
                /* two adjacent generators: a braid on 3 consecutive
                 * strands inside B_4, hence reducible */
                if (!(ia - ib == 1 || ib - ia == 1)) len2_reducible = 0;
            }
        }
        printf("  D106's depth-2 'pA' braids: %d; each is a word in two "
            "adjacent generators (a 3-strand braid inside B_4: REDUCIBLE, "
            "not pseudo-Anosov); Burau bound = phi^2, exact on the "
            "3-strand piece\n", len2_pa);
        check("D106's depth-2 'pA' braids: char. poly x^3 - 4x^2 + 4x - 1, "
            "Burau radix (3 + sqrt 5)/2 exactly, positive entropy, and "
            "reducible (two adjacent generators)", len2_pa == 8 && len2_ok
            && len2_reducible);
        sprintf(msg, "D106's rule disagrees with the exact classes on B_4 "
            "up to length %d in %d braids", maxlen,
            cross[0][CLS_P] + cross[0][CLS_Z] + cross[1][CLS_E]);
        printf("  %s\n", msg);
    }
}

/* ================================================================
 * Part D: the active-ReLU pattern along each braid's iteration
 * ================================================================
 *
 * Run beta on a loop again and again and record, in each pass through
 * the word, which ReLUs fire (argument > 0; at 0 both pieces give 0).
 * A pass's pattern names the linear region the network is in.
 * Descriptive (plan.md, Part D): does the pattern become periodic,
 * from which pass s, with what period q? Two exact checks: periodic
 * braids repeat their pattern from pass 0; and on every stabilized
 * tail the network IS one integer linear map M per q passes
 * (v_{s+jq} = M^j v_s), since the programs have no biases. M is the
 * bridge to the exact dilatation (Hall and Yurttas), which Part D does
 * not compute.
 */

#define D_PASSES 60
#define D_BITS   64
#define D_QMAX   12
#define D_TAIL   20

/* one letter, recording ReLU activity; with force != NULL each ReLU
 * follows force (1: identity, 0: zero) instead of its sign, which
 * makes the letter linear. Returns the number of ReLUs. */
static int
run_letter_pattern (
    const Program       *pr,
    Magnus              *coord,
    Magnus              *scratch,
    unsigned char       *bits,
    const unsigned char *force)
{
    int k;
    int t;
    int r = 0;
    int m2 = pr->nregs - pr->ninstr;

    for (k = 0; k < m2; k++) {
        scratch[k] = coord[k];
    }
    for (k = 0; k < pr->ninstr; k++) {
        const Instr *ins = &pr->in[k];
        Magnus       sum = magnus_ex_s64(0);

        for (t = 0; t < ins->nterms; t++) {
            sum = magnus_adde(sum, magnus_multiplica(scratch[ins->src[t]],
                magnus_ex_s64((s64)ins->coef[t]), pool), pool);
        }
        if (ins->op == OP_RELU) {
            int on = force != NULL ? (int)force[r]
                : magnus_signum(sum) > 0;

            if (bits != NULL) bits[r] = (unsigned char)on;
            if (!on) sum = magnus_ex_s64(0);
            r++;
        }
        scratch[ins->dst] = sum;
    }
    for (k = 0; k < pr->nout; k++) {
        coord[pr->out_coord[k]] = scratch[pr->out_reg[k]];
    }
    return r;
}

/* one pass through the word; returns the number of ReLUs */
static int
run_pass (
    const Program       *prs,
    int                  len,
    Magnus              *coord,
    unsigned char       *bits,
    const unsigned char *force)
{
    Magnus scratch[64];
    int    k;
    int    nb = 0;

    for (k = 0; k < len; k++) {
        nb += run_letter_pattern(&prs[k], coord, scratch,
            bits != NULL ? bits + nb : NULL,
            force != NULL ? force + nb : NULL);
    }
    return nb;
}

typedef struct {
    int stable;      /* the last >= D_TAIL passes repeat with q */
    int start;       /* first pass of the periodic tail */
    int period;      /* q */
    int regions;     /* distinct patterns over all passes */
    int linear_ok;   /* v_{s+jq} = M^j v_s on the whole tail */
} Orbita;

static void
pattern_orbit (
    Plexus  p,
    int     k,          /* the loop E_k */
    Orbita *o)
{
    int           n = (int)p.fila;
    int           d = 2 * (n - 2);
    int           len = (int)p.longitudo;
    Program       prs[16];
    unsigned char pat[D_PASSES][D_BITS];
    Magnus        orbit[D_PASSES + 1][16];
    Ansa          e;
    int           nb = 0;
    int           t;
    int           j;
    int           q;

    memset(o, 0, sizeof(*o));
    memset(pat, 0, sizeof(pat));
    if (len > 16) fatal("word too long for Part D");
    for (t = 0; t < len; t++) {
        build_letter(n, (long)p.litterae[t], &prs[t]);
    }
    if (!ansa_circa((i32)n, (i32)k, pool, &e)) fatal("loop refused");
    for (j = 0; j < n - 2; j++) {
        orbit[0][j] = e.a[j];
        orbit[0][n - 2 + j] = e.b[j];
    }
    for (t = 0; t < D_PASSES; t++) {
        for (j = 0; j < d; j++) orbit[t + 1][j] = orbit[t][j];
        nb = run_pass(prs, len, orbit[t + 1], pat[t], NULL);
    }
    if (nb > D_BITS) fatal("too many ReLUs for Part D");
    /* distinct regions */
    for (t = 0; t < D_PASSES; t++) {
        int seen = 0;

        for (j = 0; j < t && !seen; j++) {
            if (memcmp(pat[j], pat[t], (size_t)nb) == 0) seen = 1;
        }
        if (!seen) o->regions++;
    }
    /* the smallest q whose periodic tail is long enough */
    for (q = 1; q <= D_QMAX && !o->stable; q++) {
        int s = 0;

        for (t = D_PASSES - 1 - q; t >= 0; t--) {
            if (memcmp(pat[t], pat[t + q], (size_t)nb) != 0) {
                s = t + 1;
                break;
            }
        }
        if (D_PASSES - s >= D_TAIL && D_PASSES - s >= 3 * q) {
            o->stable = 1;
            o->start = s;
            o->period = q;
        }
    }
    if (!o->stable) return;
    /* M: the forced q-pass map on the basis vectors (column c = M e_c),
     * then v_{s+jq} = M^j v_s along the tail */
    {
        Magnus mat[16][16];
        Magnus v[16];
        int    c;
        int    r;
        int    s = o->start;

        for (c = 0; c < d; c++) {
            Magnus col[16];

            for (r = 0; r < d; r++) {
                col[r] = magnus_ex_s64(r == c ? 1 : 0);
            }
            for (t = 0; t < o->period; t++) {
                (void)run_pass(prs, len, col, NULL, pat[s + t]);
            }
            for (r = 0; r < d; r++) mat[r][c] = col[r];
        }
        o->linear_ok = 1;
        for (r = 0; r < d; r++) v[r] = orbit[s][r];
        for (t = s + o->period; t <= D_PASSES; t += o->period) {
            Magnus w[16];

            for (r = 0; r < d; r++) {
                Magnus sum = magnus_ex_s64(0);

                for (c = 0; c < d; c++) {
                    sum = magnus_adde(sum, magnus_multiplica(mat[r][c],
                        v[c], pool), pool);
                }
                w[r] = sum;
            }
            for (r = 0; r < d; r++) {
                v[r] = w[r];
                if (!magnus_aequalis(v[r], orbit[t][r])) o->linear_ok = 0;
            }
        }
    }
}

static int
lcm_int (
    int a,
    int b)
{
    int x = a;
    int y = b;

    while (y != 0) {
        int r = x % y;

        x = y;
        y = r;
    }
    return a / x * b;
}

static int
int_cmp (
    const void *x,
    const void *y)
{
    return *(const int *)x - *(const int *)y;
}

static void
part_d (
    Census *cs,
    int     count,
    int     n,
    int     maxlen)
{
    const char *names[3] = { "P", "E", "Z" };
    int         cls;
    int         periodic_ok = 1;
    int         linear_ok = 1;
    int         orbits = 0;
    int         stable_orbits = 0;
    int         ecross[2][3];
    int        *starts = (int *)malloc((size_t)(count + 1) * sizeof(int));
    char        msg[256];

    memset(ecross, 0, sizeof(ecross));
    printf("\n=== Part D: the active-ReLU pattern, B_%d up to length %d "
        "(%d passes on every E_k) ===\n\n", n, maxlen, D_PASSES);
    printf("    %-5s %7s %10s %9s %9s %6s %6s %6s %13s\n", "class",
        "braids", "stabilized", "start med", "start max", "q = 1", "q = 2",
        "q > 2", "regions mean");
    for (cls = 0; cls < 3; cls++) {
        int    braids = 0;
        int    stab = 0;
        int    q1 = 0;
        int    q2 = 0;
        int    qn = 0;
        int    ns = 0;
        long   regions = 0;
        int    i;

        for (i = 0; i < count; i++) {
            PiscinaNotatio mark;
            int            all_stable = 1;
            int            start = 0;
            int            period = 1;
            int            k;

            if (cs[i].cls != cls) continue;
            braids++;
            mark = piscina_notare(pool);
            for (k = 1; k < n; k++) {
                Orbita o;

                pattern_orbit(cs[i].word, k, &o);
                orbits++;
                regions += o.regions;
                if (!o.stable) {
                    all_stable = 0;
                    continue;
                }
                stable_orbits++;
                if (!o.linear_ok) linear_ok = 0;
                if (o.start > start) start = o.start;
                period = lcm_int(period, o.period);
                if (cls == CLS_P && (o.start != 0
                        || cs[i].period % o.period != 0)) {
                    periodic_ok = 0;
                }
            }
            piscina_reficere(pool, mark);
            if (cls == CLS_P && !all_stable) periodic_ok = 0;
            if (!all_stable) continue;
            if (cls == CLS_E) {
                ecross[cs[i].certified ? 0 : 1][period == 1 ? 0
                    : period == 2 ? 1 : 2]++;
            }
            stab++;
            starts[ns++] = start;
            if (period == 1) q1++;
            else if (period == 2) q2++;
            else qn++;
        }
        qsort(starts, (size_t)ns, sizeof(int), int_cmp);
        printf("    %-5s %7d %10d %9d %9d %6d %6d %6d %13.1f\n", names[cls],
            braids, stab, ns ? starts[ns / 2] : 0, ns ? starts[ns - 1] : 0,
            q1, q2, qn, braids ? (double)regions / (double)(braids
            * (n - 1)) : 0.0);
    }
    printf("  (per braid: stabilized = every loop's pattern repeats over "
        "its last >= %d passes; start\n   and q are the largest over the "
        "loops; regions = distinct patterns per loop over %d passes)\n",
        D_TAIL, D_PASSES);
    printf("  %d of %d orbits stabilized within the run (observed, not "
        "proved)\n", stable_orbits, orbits);
    printf("  E braids by certificate and settled period q:\n");
    printf("    %-30s %6s %6s %6s\n", "", "q = 1", "q = 2", "q > 2");
    printf("    %-30s %6d %6d %6d\n", "Burau(-1) radius > 1",
        ecross[0][0], ecross[0][1], ecross[0][2]);
    printf("    %-30s %6d %6d %6d\n", "only at a cube root of unity",
        ecross[1][0], ecross[1][1], ecross[1][2]);
    sprintf(msg, "B_%d: periodic braids repeat their ReLU pattern from "
        "pass 0, with a period dividing the braid's", n);
    check(msg, periodic_ok);
    sprintf(msg, "B_%d: on every stabilized tail the network is ONE "
        "integer linear map per period: v_{s+jq} = M^j v_s exactly", n);
    check(msg, linear_ok && stable_orbits > 0);
    free(starts);
}

/* ================================================================
 * main
 * ================================================================ */

int
main (void)
{
    pool = piscina_generare_dynamicum("demo128", 1 << 20);
    if (pool == NULL) {
        printf("cannot create pool\n");
        return 1;
    }
    printf("KNOTAPEL DEMO 128: Braids as Exact ReLU Networks\n");
    printf("================================================\n");
    celer = getenv("D128_CELER") != NULL;
    z8 = cyclotomia_creare(8, pool);
    part_a();
    {
        Census *b3;
        Census *b4;
        int     n3;
        int     n4;
        int     l3 = celer ? 5 : 8;
        int     l4 = celer ? 4 : 6;

        /* counts: full mode = the T5 review's independent Artin counts;
         * quick mode = the same breadth-first prefixes (B_3 levels 0-5,
         * B_4 levels 0-4 of the full run) */
        part_b(3, l3, celer ? 263 : 2589, &b3, &n3);
        part_b(4, l4, celer ? 469 : 5121, &b4, &n4);
        part_c(b4, n4, l4);
        part_d(b3, n3, 3, l3);
        part_d(b4, n4, 4, l4);
        free(b3);
        free(b4);
    }
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pool);
    return n_fail == 0 ? 0 : 1;
}
