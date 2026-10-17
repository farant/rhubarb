/* demo-snapshot.c - GENERATUM (knotapel/archive.sh) - DO NOT EDIT
 *
 * knotapel/demo_129_braid_regions/main.c frozen with its house-library closure as ONE file:
 * headers in dependency order, library sources with file-local
 * names renamed per file (#define/#undef), main.c last; '#line'
 * names each original file. Compile and run:
 *
 *   clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -fbracket-depth=512 -O2 -g demo-snapshot.c -o demo-snapshot
 *
 * Commit (library closure clean): a0c544537d129e2398050bd29f3c0ac23f5833b0
 * Regenerate: ./knotapel/archive.sh knotapel/demo_129_braid_regions/main.c
 * Verified: live build and snapshot gave byte-identical output.
 * Sources (git blob hashes):
 *   64068da453692e55d89f3395ecd9a0e9350cfa29  include/anulus.h
 *   7db315706b013efbb850928c78e1c13b0fb72362  include/chorda.h
 *   6f9b7a043cebe2912c612139453b66fcb04770cb  include/chorda_aedificator.h
 *   37b6fb1c16e76827120976705e418e11c89406a6  include/congruentia.h
 *   26098ae7c146ddd6bdd9e18529319eca305a2f3b  include/cyclotomia.h
 *   53645f652dd16a7a8c8ad79df9e2e70289ee5e52  include/fractio.h
 *   602b7efdffbe09688a7071e799873b60a139d675  include/friatio.h
 *   f45b10ad9c303c02d43655b950600fcdab8997bb  include/latina.h
 *   bfde2073ff18d6f5be1c3844556cc82d39ee0937  include/magnus.h
 *   423a176668f285b2661452e215cdf84259389d93  include/matrix.h
 *   dd64f92c6d23e3fe0e51747ba4502c87412a646f  include/nervus.h
 *   cd2db07dbd9f7bb6f71ffaa27b03f7cdb8ea65ee  include/piscina.h
 *   37c1c4725bd5a45a7f84ec40ccd50e3c37f9a1e0  include/plexus.h
 *   95f5c575951519ca8cee6170ec3a4519b284d0c4  include/polynomium.h
 *   a34b9f2536efa81a09cea36f9a9acba28fb4b2fc  include/postulata_posix.h
 *   3b6102802849c1047af220e2b303523db1df5000  include/radices.h
 *   659b865720bcca5d0fbd4f9eaf5f3fe7baa8cf1e  include/radices_interna.h
 *   91e1de36acad556015e29cef636227ac270a6d56  include/sors.h
 *   97ae44cb64e0a0cc30c9c4fd6f26df204c1ad971  include/tabula_dispersa.h
 *   9f20940b276fc89410cce3ddffdd62a77ac12a29  lib/anulus.c
 *   b4c8c649c03e84eca53408282c39d5b0e24c6016  lib/chorda.c
 *   ee055a36d7e4726ad5b3731e2ca64e62e93d084c  lib/chorda_aedificator.c
 *   c446f3092b17cdca38b584886d86a5ca80856eac  lib/congruentia.c
 *   1c4e7da9aa888a2fe56cc33028e6b1455be8a1dc  lib/cyclotomia.c
 *   61ec3b3106345cce9ea64477aafe7d1072030be7  lib/fractio.c
 *   28af2e24442e4ad61ee2d347939ba94783e42645  lib/friatio.c
 *   fd94a8d26a4bd4b340875ac0a1551bc3f3c55e8e  lib/magnus.c
 *   5fc06983f571b6cf493cc884ab9647db9f499d44  lib/matrix.c
 *   38abc345cba3caf7703ce765db84a24128694f9a  lib/nervus.c
 *   c6ab1e19274a3b36ff5cfdde651e45d079905b56  lib/piscina.c
 *   7c7b7154527cc83b0d04914000c35c41de03d79f  lib/plexus.c
 *   9967d4ccc5d5f5e18bf84f716b4f8fb7b7964a85  lib/polynomium.c
 *   85567c63931bd36cd7ea3eb8fb64ee169ab9ac2e  lib/radices.c
 *   a1a3bbc96bc609e89e4098ef95f770c5e10a3ea5  lib/sors.c
 *   6e44361a1994d098128748844d6efe84c00624a6  lib/tabula_dispersa.c
 *   51cb6004456f207113ff0949383198b42992a983  knotapel/demo_129_braid_regions/main.c (uncommitted, embedded verbatim)
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
#line 1 "include/nervus.h"
/* nervus.h - Retia EXACTA per partes linearia (ReLU), ut data
 *
 * RETE: programma rectum (sine saltibus) super registra. Registra
 * 0 .. introitus-1 sunt introitus; instructio k registrum
 * introitus + k scribit (assignatio unica). Duo genera instructionum:
 *   NERVUS_AFFINIS  r = c_1 f_1 + ... + c_t f_t + b
 *   NERVUS_RELU     r = max(0, c_1 f_1 + ... + c_t f_t + b)
 * f_j registra iam definita (indice minore quam r), c_j et b Magnus
 * (integri exacti, sine limite). min(0, x) = x - relu(x). Exitus:
 * registra nominata (eadem iterari licet; introitus quoque).
 *
 * FORMA ACTIVATIONIS: bitus unus per ReLU, ordine instructionum.
 * ReLU ACTIVA est si et solum si argumentum > 0 (ad 0 ambae partes 0
 * dant, ergo valor non pendet a conventione; forma pendet, et haec
 * est). Bitus k in bitus[k >> 3], positione (k & 7).
 *
 * NULLAE: forma post nervus_aestimare numerum ReLU fert quarum
 * argumentum EXACTE 0 erat. Tale punctum in pariete regionis iacet:
 * forma eius est cellae dimensionis inferioris, non regionis apertae
 * (|x| in x = 0: forma propria, sed |x| duas regiones habet). Retia
 * sine addendis parietes per originem habent, ergo puncta integra in
 * parietibus frequentia sunt: regiones numerans puncta cum nullae > 0
 * separet. Claves et aequalitas formarum nullas ignorant.
 *
 * FIGURA: profunditas registri = 1 + maxima profunditas fontium (0
 * si fontes nulli; introitus profunditatis 0), profunditas retis =
 * maxima; profunditas_relu idem ReLU numerans; latitudo = registra
 * viva plurima, ante primam instructionem et post quamque (vivum:
 * definitum et postea lectum aut exitus).
 *
 * STRUCTURA PUBLICA: rete manu factum per nervus_validus probatur;
 * omnis functio rete invalidum refutat (FALSUM), numquam extra alveum
 * legit. Construere solum per nervus_affinis / nervus_relu /
 * nervus_exitus (quae ordinem et indices custodiunt).
 *
 * FORMA COACTA: nervus_aestimare_coacte ReLU k bitum k formae
 * sequi cogit (activa: argumentum ipsum, etiam negativum; non activa:
 * 0), quacumque signo. Forma fixa rete AFFINE facit: x -> M x + c
 * (nervus_forma_affinis, exacte, per formas affines propagatas). In
 * regione formae (ubi aestimatio ordinaria eandem formam dat) mappa
 * aestimationem ordinariam aequat; extra eam aestimationem coactam.
 * Sine addendis c = 0.
 *
 * COMPOSITIO: nervus_componere(a, b) = b post a: exitus a introitus b
 * fiunt; ReLU ordine a deinde b, ergo forma compositi = forma a
 * deinde forma b.
 *
 * REGIONES PLANI: rete sine addendis cum II introitibus positive
 * homogeneum est, ergo regiones eius CONI ab origine sunt: sectores
 * inter radios rationales (muros). Puncta formae eiusdem regionem
 * CONVEXAM faciunt (sub forma priorum argumentum cuiusque ReLU
 * lineare est, ergo condicio eius semiplanum), ergo forma una sectorem
 * unum dat, angulus <= pi, formae sectorum diversae. Murus: radius ubi
 * forma mutatur. Sectores contra horologium; primus incipit a muro
 * primo ab (1, 0) inclusive. Nulli muri: sector unus, totum planum,
 * initium = finis = (1, 0). NULLAE sectoris: ReLU quarum argumentum in
 * sectore IDENTICE 0 est - bitus 0, et nervus_aestimare in omni puncto
 * interiore easdem nullas reddit (puncta interiora numerans talem
 * sectorem non videt). FRUSTUM (pars linearis): sectores contigui
 * eadem matrice coniuncti; frusta <= sectores. Algorithmus: circulus
 * in IV arcus per axes divisus; per instructionem forma linearis
 * cuiusque registri in quoque arcu; ad ReLU arcus ubi argumentum g
 * signum STRICTE mutat in radio |g(v)| u + |g(u)| v (exacte,
 * primitivo) dividitur; arcus semper < pi; postremo arcus contigui
 * forma eadem coniunguntur. Sumptus O(arcus x registra) Magnus.
 *
 * MEMORIA: rete in piscina sua (data in nervus_creare) crescit;
 * coefficientes in eam transcribuntur. nervus_aestimare valores et
 * spatium operis in piscina vocantis allocat: in gyris
 * piscina_notare / piscina_reficere circum vocationem, exitus per
 * magnus_transcribe servandi. Exitus membra cum introitu (exitus qui
 * introitus est) aut cum addendis retis (instructio constans) PARTIRI
 * potest, ut in magnus. FALSUM -> exitus non tangitur.
 *
 * CAUTIO TYPORUM: numeri i32 (SINE signo), indices registrorum s32.
 *
 * USUS (AND(x, y) = relu(x + y - 1)):
 *   Nervus* n;
 *   s32     f[II] = { 0, 1 };
 *   Magnus  c[II];
 *   s32     r;
 *   c[0] = c[1] = magnus_ex_s64(I);
 *   (vacuum)nervus_creare(II, piscina, &n);
 *   (vacuum)nervus_relu(n, II, f, c, magnus_ex_s64(-I), &r);
 *   (vacuum)nervus_exitus(n, I, &r);
 *
 * Consilium: project-specs/nervus-plan.md. Vide lib/nervus.worklog.md.
 */
/* <aedilis corpus="lib/nervus.c"/> */
#ifndef NERVUS_H
#define NERVUS_H







/* introitus + instructiones; termini per instructionem; exitus */
#define NERVUS_REGISTRA_MAXIMA  16777216
#define NERVUS_TERMINI_MAXIMI   65536

nomen enumeratio {
    NERVUS_AFFINIS = 0,
    NERVUS_RELU    = 1
} NervusGenus;

nomen structura {
    NervusGenus  genus;
            i32  termini;
            s32* fontes;           /* termini registra, < registrum */
         Magnus* coefficientes;    /* termini */
         Magnus  addendum;         /* b */
} NervusInstructio;

nomen structura {
                 i32  introitus;
                 i32  numerus;          /* instructiones */
                 i32  capacitas;
    NervusInstructio* instructiones;
                 i32  relu;             /* ReLU inter instructiones */
                 i32  exitus_numerus;
                 s32* exitus;           /* registra exitus */
             Piscina* piscina;
} Nervus;

nomen structura {
     i32  numerus;     /* ReLU */
      i8* bitus;       /* (numerus + 7) / 8 octeti */
     i32  nullae;      /* ReLU argumento exacte 0 (vide NULLAE) */
} NervusForma;

nomen structura {
     i32 instructiones;
     i32 relu;
     i64 termini;              /* summa terminorum */
     i32 profunditas;
     i32 profunditas_relu;
     i32 latitudo;
} NervusFigura;


/* ==================================================
 * Constructio
 * ================================================== */

/* rete vacuum cum introitus registris (0 licet: rete constans);
 * FALSUM si introitus > NERVUS_REGISTRA_MAXIMA */
b32
nervus_creare (
         i32   introitus,
     Piscina*  piscina,
      Nervus** exitus);

/* instructio affinis; *registrum_novum = index registri novi. FALSUM
 * (rete non mutatum) si caput retis invalidum (numeri, alvei,
 * piscina: O(1), non instructiones singulae - eas nervus_validus
 * probat), termini > NERVUS_TERMINI_MAXIMI, fontes
 * aut coefficientes NIHIL cum termini > 0, fons extra 0 .. registra
 * iam definita, aut registra plena */
b32
nervus_affinis (
               Nervus* rete,
                  i32  termini,
         constans s32* fontes,
      constans Magnus* coefficientes,
               Magnus  addendum,
                  s32* registrum_novum);

/* idem, ReLU */
b32
nervus_relu (
               Nervus* rete,
                  i32  termini,
         constans s32* fontes,
      constans Magnus* coefficientes,
               Magnus  addendum,
                  s32* registrum_novum);

/* exitus ponit (priores reponit); FALSUM (rete non mutatum) si caput
 * retis invalidum, numerus > NERVUS_REGISTRA_MAXIMA, registrum non
 * definitum, aut registra NIHIL cum numerus > 0 */
b32
nervus_exitus (
            Nervus* rete,
               i32  numerus,
      constans s32* registra);

/* VERUM si rete validum: indices omnes intra fines, fontes ante
 * registrum suum, genera nota, relu numeratum recte, alvei non NIHIL
 * ubi opus. O(termini) */
b32
nervus_validus (
      constans Nervus* rete);


/* ==================================================
 * Aestimatio
 * ================================================== */

/* forma vacua (omnes bitus 0) pro rete; FALSUM si rete invalidum */
b32
nervus_forma_creare (
      constans Nervus* rete,
              Piscina* piscina,
          NervusForma* exitus);

/* VERUM si ReLU k activa; FALSUM si non, aut k extra fines */
b32
nervus_forma_bitus (
      constans NervusForma* forma,
                       i32  k);

/* introitus[0 .. introitus-1] -> exitus[0 .. exitus_numerus-1],
 * exacte; forma (NIHIL licet) bitus et nullas accipit. FALSUM si rete
 * invalidum, alveus necessarius NIHIL, aut forma non huius retis
 * (numerus differt) */
b32
nervus_aestimare (
      constans Nervus* rete,
      constans Magnus* introitus,
               Magnus* exitus,
          NervusForma* forma,
              Piscina* piscina);

/* aestimatio coacta (vide FORMA COACTA); FALSUM ut nervus_aestimare,
 * aut forma NIHIL */
b32
nervus_aestimare_coacte (
         constans Nervus* rete,
         constans Magnus* introitus,
                  Magnus* exitus,
    constans NervusForma* forma,
                 Piscina* piscina);

/* figura retis (vide FIGURA supra); FALSUM si rete invalidum. Memoria
 * temporaria propria, ante reditum destructa */
b32
nervus_figura (
      constans Nervus* rete,
         NervusFigura* exitus);


/* ==================================================
 * Formae
 * ================================================== */

/* bitum k ponit; FALSUM si forma invalida aut k extra fines */
b32
nervus_forma_ponere (
    NervusForma* forma,
            i32  k,
            b32  activa);

/* numerus ReLU activarum; 0 si forma invalida */
i32
nervus_forma_activae (
    constans NervusForma* forma);

/* VERUM si numerus idem et bitus omnes idem; FALSUM si non, aut forma
 * invalida */
b32
nervus_formae_aequales (
    constans NervusForma* a,
    constans NervusForma* b);

/* clavis pro tabula_dispersa: octeti formae (bitus ultra numerum
 * nulli), (numerus + 7) / 8 octeti; formae aequales <-> claves
 * aequales (pro numero eodem). FALSUM si forma invalida */
b32
nervus_forma_clavis (
    constans NervusForma* forma,
                 Piscina* piscina,
                  chorda* exitus);

/* mappa affinis formae: M (exitus_numerus x introitus, super
 * ANULUS_INTEGRORUM, elementa Magnus) et c (exitus_numerus Magnus;
 * NIHIL si exitus nulli) in piscina vocantis. Formae affines in
 * piscina temporaria propagantur: memoria O(registra x introitus).
 * FALSUM si rete invalidum, forma NIHIL aut non huius retis, aut
 * registra x (introitus + 1) > 2^27 */
b32
nervus_forma_affinis (
         constans Nervus*  rete,
    constans NervusForma*  forma,
                 Piscina*  piscina,
                  Matrix*  m,
                  Magnus** c);


/* ==================================================
 * Compositio
 * ================================================== */

/* b post a (vide COMPOSITIO); rete novum in piscina. FALSUM si rete
 * invalidum, a->exitus_numerus != b->introitus, aut registra
 * NERVUS_REGISTRA_MAXIMA superant */
b32
nervus_componere (
    constans Nervus*  a,
    constans Nervus*  b,
            Piscina*  piscina,
             Nervus** exitus);


/* ==================================================
 * Regiones plani (sine addendis, II introitus)
 * ================================================== */

/* directio ab origine: (x, y) integri primitivi (divisor communis 1),
 * non ambo nulli */
nomen structura {
    Magnus x;
    Magnus y;
} NervusRadius;

/* regio aperta inter murum initium et murum finis, contra horologium
 * (vide REGIONES PLANI) */
nomen structura {
    NervusRadius initium;
    NervusRadius finis;
    NervusRadius medius;      /* directio aliqua intra sectorem, non
                               * canonica (a divisione arcuum pendet) */
     NervusForma forma;       /* nullae: argumento identice 0 */
          Matrix m;           /* exitus_numerus x II: f(x) = M x */
             i32 frustum;     /* index partis linearis, 0 .. frusta-1 */
} NervusSector;

nomen structura {
             i32  numerus;     /* sectores = regiones */
    NervusSector* regiones;
             i32  frusta;      /* partes lineares */
} NervusPlanum;

/* regiones activationis retis in plano, exacte (vide REGIONES PLANI),
 * in piscina vocantis. FALSUM (exitus non tangitur) si rete invalidum,
 * introitus != II, addendum aliquod non nullum, aut memoria PROPRIA
 * deficit; in piscina vocantis allocatio deficiens fatalis est, ut in
 * omni piscina */
b32
nervus_regiones_plani (
    constans Nervus* rete,
            Piscina* piscina,
       NervusPlanum* exitus);

#endif /* NERVUS_H */
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

/* actio Dynnikov plexus p ut RETE (nervus.h), sine addendis: introitus
 * et exitus 2n - 4, ordine (a_1 .. a_{n-2}, b_1 .. b_{n-2}). Littera
 * per aeq. 8-15 cum f- = f - relu(f): sigma_1, sigma_{n-1} et
 * inversae II ReLU et II affines, mediae V ReLU et VI affines;
 * profunditas IV, profunditas ReLU II per litteram. Aestimatio retis
 * = ansa_agere (probatum). Verbum vacuum: rete sine instructionibus,
 * exitus = introitus. FALSUM si p invalidus, fila < 3, aut registra
 * NERVUS_REGISTRA_MAXIMA superarent */
b32
plexus_nervus (
      Plexus   p,
     Piscina*  piscina,
      Nervus** exitus);

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
 * Census: clavis classis, periodicitas, enumeratio
 * ================================================== */

/* clavis classis: imagines ansarum E_1 .. E_{n+1} plexus in D_{n+2}
 * (translatio 1) concatenatae, familia plexus_aequales_ansis. p = q in
 * B_n -> claves aequales; claves aequales -> p = q quatenus familia
 * ansarum fidelis est (exhaustive contra Artin probata pro verbis
 * brevibus, vide plexus_aequales_ansis; generaliter hic non probata).
 * Clavis in piscina vocantis, temporaria propria. FALSUM si p
 * invalidus aut fila > PLEXUS_FILA_MAXIMA - 2 */
b32
plexus_clavis (
      Plexus  p,
     Piscina* piscina,
      chorda* exitus);

/* periodicitas EXACTA: *periodus = minimum k >= 1 cum beta^k potentia
 * Delta^2 (centri generator), aut 0 si beta non periodicus. Braidae
 * periodicae potentiis delta = sigma_1 ... sigma_{n-1} aut epsilon =
 * sigma_1 delta coniugatae sunt (Brouwer, Kerekjarto, Eilenberg; vide
 * Gonzalez-Meneses et Wiest 2004), ordinum n et n-1 modulo centro:
 * ergo k divisor n aut n-1 est, et soli hi probantur, ordine
 * crescente; j = k e / (n(n-1)) integer esse debet (e summa
 * exponentium). Aequalitas per plexus_aequales_ansis. FALSUM si p
 * invalidus, fila > PLEXUS_FILA_DIMIDIAE_TORSIONIS aut aequalitas
 * refutat */
b32
plexus_periodus (
      Plexus  p,
     Piscina* piscina,
         i32* periodus);

nomen structura {
        i32  fila;
        i32  longitudo_maxima;
        i32  numerus;      /* classes */
     Plexus* verba;        /* repraesentantes, per latitudinem */
     chorda* claves;       /* plexus_clavis cuiusque */
        i32* initia;       /* L + 2: initia[l] = prima classis gradus l,
                            * initia[L + 1] = numerus */
} PlexusCensus;

/* enumeratio per latitudinem omnium classium B_n longitudinis <= L
 * (longitudo = verbum brevissimum in sigma_i^{+-1}). Gradus l ex gradu
 * l - 1 (ordine suo) per litteras -(n-1) .. n-1 (0 omissa); classis
 * nova (clavis nondum visa) verbo PRIMO quod eam attingit
 * repraesentatur (longitudo l, libere reductum); intra gradum ordine
 * clavium (chorda_comparare). Exhaustiva quatenus plexus_clavis
 * classes separat (vide supra). Effectum in piscina vocantis, solum
 * post successum (classes in piscina propria usque ad finem). FALSUM
 * (exitus non tactus, piscina vocantis intacta) si fila extra
 * 2..PLEXUS_FILA_MAXIMA - 2, si classes > classes_maximae (numquam
 * truncatur), si longitudo_maxima >= classes_maximae (omnis gradus
 * classem novam addit: talis census limitem semper superaret), si
 * longitudo_maxima > 2^31 - 1 (longitudo verbi maxima), aut si
 * memoria propria deficit */
b32
plexus_enumerare (
             i32  fila,
             i32  longitudo_maxima,
             i32  classes_maximae,
         Piscina* piscina,
    PlexusCensus* exitus);


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
#line 1 "include/tabula_dispersa.h"
/* tabula_dispersa.h - tabula friationis (hash table, map) */
#ifndef TABULA_DISPERSA_H
#define TABULA_DISPERSA_H






/* ===================================================
 * Tabula Dispersa - Tabula Friationis per Addressing Apertum
 * "Tabula: series plana, per friationes indexata"
 * ================================================== */

/* Status cuiusque sloti in tabula
 * "Status sloti" */
nomen enumeratio {
    SLOT_VACUUM    = 0, /* Slotus vacuus */
    SLOT_OCCUPATUM = 1, /* Clavis-valor par continens */
    SLOT_DELETUM   = 2  /* Deletum (tumulus) */
} SlotusStatus;


/* Genus functionis friationis
 * Clavis chordae capit, friationes i32 reddit */
nomen i32 (*TabulaFriatio)(chorda clavis);


/* Genus functionis comparationis
 * 0 reddit si aequalis, <0 si a<b, >0 si a>b */
nomen s32 (*TabulaComparatio)(chorda a, chorda b);

nomen structura {
    SlotusStatus  status;
          chorda  clavis;
          vacuum* valor;
             i32  hash_cachatum;
} Slotus;

/* Ipsa tabula friationis */
nomen structura {
              Slotus* sloti;
                 i32  capacitas;
                 i32  capacitas_mask;
                 i32  numerus;
                 i32  numerus_deletorum;
             Piscina* piscina;
       TabulaFriatio  friatio;
    TabulaComparatio  comparatio;
                 f32  factor_maximus;
                 f32  factor_deletorum_maximus;
                 i32  collisiones_totales;
                 i32  distantia_maxima;
} TabulaDispersa;

/* Iterator ad omnes introitus ambulandos */
nomen structura {
    TabulaDispersa* tabula;
               i32  positus_currens;
} TabulaIterator;


/* ==================================================
 * Creatio
 * ================================================== */

TabulaDispersa*
tabula_dispersa_creare (
             Piscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio);

/* Commoditas: crea cum comparatione chordae (signum) */
TabulaDispersa*
tabula_dispersa_creare_chorda (
    Piscina* piscina,
        i32  capacitas_initialis);


/* ==================================================
 * Insertio / Quaestio / Deletio
 * ================================================== */

/* Clavis-valor par inserere
 * VERUM reddit in successu, FALSUM in allocationis fractura
 * Valor existens renovat si clavis iam existit */

b32
tabula_dispersa_inserere (
    TabulaDispersa* tabula,
            chorda  clavis,
            vacuum* valor);


/* Clavis per quaestio
 * VERUM reddit et *valor_exitus statuit si inventum
 * FALSUM reddit si non inventum */
b32
tabula_dispersa_invenire (
    TabulaDispersa*  tabula,
            chorda   clavis,
            vacuum** valor_out);

/* Celeris existentiae verificatio */
b32
tabula_dispersa_continet (
    TabulaDispersa* tabula,
            chorda  clavis);

/* Convenientia: quaestio per literis C directe
 * VERUM reddit et *valor_exitus statuit si inventum
 * FALSUM reddit si non inventum */
b32
tabula_dispersa_invenire_literis (
        TabulaDispersa*  tabula,
    constans character*  cstr,
                vacuum** valor_out);

/* Convenientia: verificatio existentiae per literis C */
b32
tabula_dispersa_continet_literis (
        TabulaDispersa* tabula,
    constans character* cstr);

/* Introitum delere per clavis
 * VERUM reddit si inventum et deletum, FALSUM alioquin */
b32
tabula_dispersa_delere (
    TabulaDispersa* tabula,
            chorda  clavis);


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

/* Tabulam vaccare sine deallocatione (arena-amica)
 * Omnes sloti ad VACUUM reficit, computationes ad nihil
 * piscina refectio memoriam veram purificabit */
vacuum
tabula_dispersa_vacare (
    TabulaDispersa* tabula);


/* =================================================
 * Iterator
 * ================================================== */

TabulaIterator
tabula_dispersa_iterator_initium (
    TabulaDispersa* tabula);

/* Sequens introitum in iteratore capere
 * VERUM reddit et *clavis_exitus et *valor_exitus implet si introitus inventus
 * FALSUM reddit in fine iterationis */

b32
tabula_dispersa_iterator_proximum (
    TabulaIterator*  iter,
            chorda*  clavis_out,
            vacuum** valor_out);


/* =================================================
 * Statisticae / Diagnostica
 * ================================================== */

f32
tabula_dispersa_factor_oneris (
    TabulaDispersa* tabula);

f32
tabula_dispersa_factor_deletorum (
    TabulaDispersa* tabula);

i32
tabula_dispersa_numerus (
    TabulaDispersa* tabula);

vacuum
tabula_dispersa_status_imprimere (
    TabulaDispersa* tabula);


/* =================================================
 * Functiones Friationis (in bibliotheca separata ideale)
 * ================================================== */

i32
tabula_friare_fnv1a (
    chorda clavis);

i32
tabula_friare_multiplicatio (
    chorda clavis);

#endif /* TABULA_DISPERSA_H */
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
#line 1 "include/friatio.h"
/* friatio.h - functiones friationis (hash functions) */
#ifndef FRIATIO_H
#define FRIATIO_H





/* ==================================================
 * Friatio - Functiones Friationis (Hash Functions)
 *
 * "Friare" = to crumble, break into pieces
 * "Friatio" = the act of crumbling
 *
 * Continet:
 * - Functiones non-cryptographicae (pro tabulis dispersis)
 * - Functiones cryptographicae (pro UUIDs, checksums, etc.)
 * ================================================== */


/* ==================================================
 * Functiones Non-Cryptographicae (32-bit)
 *
 * Pro tabulis dispersis et distributione rapida.
 * NON securae pro cryptographia!
 * ================================================== */

/* FNV-1a (Fowler-Noll-Vo)
 * Rapida, bona distributio, simplex
 * Offset basis: 2166136261
 * Prime: 16777619
 */
i32
friatio_fnv1a (
    chorda clavis);

/* FNV-1a pro literis C */
i32
friatio_fnv1a_literis (
    constans character* literae,
                   i32  mensura);

/* DJB2 (Dan Bernstein)
 * Classica, rapida, formula: hash * 33 + c
 * Initium: 5381
 */
i32
friatio_djb2 (
    chorda clavis);

/* DJB2 pro literis C */
i32
friatio_djb2_literis (
    constans character* literae,
                   i32  mensura);


/* ==================================================
 * SHA-1 (Secure Hash Algorithm 1)
 *
 * Cryptographica (sed OBSOLETA pro securitate!)
 * Producit 160 bits (20 bytes) digest
 * Usus: UUIDv5, checksums legacy, non pro securitate nova
 * ================================================== */

#define SHA1_DIGEST_MENSURA   XX    /* 20 bytes */
#define SHA1_BLOCK_MENSURA    LXIV  /* 64 bytes */

/* Contextus SHA-1 pro hashing incrementale */
nomen structura {
    i32 status[V];            /* A, B, C, D, E */
    i32 numerus[II];          /* Bit count (low, high) */
     i8 buffer[LXIV];         /* Input buffer */
} SHA1Contextus;

/* Initiare contextum SHA-1 */
vacuum
sha1_initiare (
    SHA1Contextus* ctx);

/* Addere data ad hash (potest vocare multiple) */
vacuum
sha1_addere (
    SHA1Contextus* ctx,
      constans i8* data,
              i32  mensura);

/* Finire et obtinere digest (20 bytes) */
vacuum
sha1_finire (
    SHA1Contextus* ctx,
               i8  digest[SHA1_DIGEST_MENSURA]);

/* Convenientia: hash data in uno passu */
vacuum
sha1_friare (
    constans i8* data,
            i32  mensura,
             i8  digest[SHA1_DIGEST_MENSURA]);

/* Convenientia: hash chorda in uno passu */
vacuum
sha1_friare_chorda (
    chorda clavis,
        i8 digest[SHA1_DIGEST_MENSURA]);


/* ==================================================
 * SHA-256 (Secure Hash Algorithm 256)
 *
 * Cryptographica moderna et secura
 * Producit 256 bits (32 bytes) digest
 * Usus: Securitas, integritas, signaturas digitales
 * ================================================== */

#define SHA256_DIGEST_MENSURA   XXXII  /* 32 bytes */
#define SHA256_BLOCK_MENSURA    LXIV   /* 64 bytes */

/* Contextus SHA-256 pro hashing incrementale */
nomen structura {
    i32 status[VIII];         /* A, B, C, D, E, F, G, H */
    i32 numerus[II];          /* Bit count (low, high) */
     i8 buffer[LXIV];         /* Input buffer */
} SHA256Contextus;

/* Initiare contextum SHA-256 */
vacuum
sha256_initiare (
    SHA256Contextus* ctx);

/* Addere data ad hash (potest vocare multiple) */
vacuum
sha256_addere (
    SHA256Contextus* ctx,
        constans i8* data,
                i32  mensura);

/* Finire et obtinere digest (32 bytes) */
vacuum
sha256_finire (
    SHA256Contextus* ctx,
                 i8  digest[SHA256_DIGEST_MENSURA]);

/* Convenientia: hash data in uno passu */
vacuum
sha256_friare (
    constans i8* data,
            i32  mensura,
             i8  digest[SHA256_DIGEST_MENSURA]);

/* Convenientia: hash chorda in uno passu */
vacuum
sha256_friare_chorda (
    chorda clavis,
        i8 digest[SHA256_DIGEST_MENSURA]);


/* ==================================================
 * CRC32 (Cyclic Redundancy Check)
 *
 * Non-cryptographica, pro integritate datorum
 * Producit 32 bits checksum
 * Usus: Verificatio filorum, packets, checksums
 * Polynomium: 0xEDB88320 (IEEE 802.3)
 * ================================================== */

/* Calculare CRC32 de data */
i32
crc32_calculare (
    constans i8* data,
            i32  mensura);

/* Calculare CRC32 de chorda */
i32
crc32_calculare_chorda (
    chorda clavis);

/* CRC32 incrementale - initiare */
i32
crc32_initiare (vacuum);

/* CRC32 incrementale - addere data */
i32
crc32_addere (
            i32  crc_currens,
    constans i8* data,
            i32  mensura);

/* CRC32 incrementale - finire (invertere bits) */
i32
crc32_finire (
    i32 crc_currens);

#endif /* FRIATIO_H */
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
/* lib/nervus.c: statica per plagulam renominata */
#define ArcusPlani ArcusPlani_nervus
#define _aestimare _aestimare_nervus
#define _applicare _applicare_nervus
#define _arcum_creare _arcum_creare_nervus
#define _arcus_plani _arcus_plani_nervus
#define _bitum_ponere _bitum_ponere_nervus
#define _bitus _bitus_nervus
#define _bitus_aequales _bitus_aequales_nervus
#define _caput_validum _caput_validum_nervus
#define _forma_valida _forma_valida_nervus
#define _formam_computare _formam_computare_nervus
#define _instructionem_addere _instructionem_addere_nervus
#define _primitivus _primitivus_nervus
#define _radium_transcribere _radium_transcribere_nervus
#define _relu_in_arcu _relu_in_arcu_nervus
#line 1 "lib/nervus.c"
/* nervus.c - retia exacta per partes linearia (vide include/nervus.h)
 *
 * Rete: introitus registra, deinde una per instructionem (assignatio
 * unica). Aestimatio ordine instructionum, valoribus Magnus. Figura per
 * transitum unum (profunditas) et intervalla vitae (latitudo).
 */



/* ==================================================
 * Auxilia
 * ================================================== */

/* caput retis: numeri et alvei quos constructio et aestimatio legunt.
 * O(1); instructiones singulas nervus_validus probat. */
interior b32
_caput_validum (
    constans Nervus* rete)
{
    si (rete == NIHIL)
    {
        redde FALSUM;
    }
    si (   rete->introitus > NERVUS_REGISTRA_MAXIMA
        || rete->numerus > rete->capacitas
        || rete->numerus > NERVUS_REGISTRA_MAXIMA
        || rete->introitus + rete->numerus > NERVUS_REGISTRA_MAXIMA
        || rete->relu > rete->numerus
        || rete->exitus_numerus > NERVUS_REGISTRA_MAXIMA)
    {
        redde FALSUM;
    }
    si (rete->capacitas > ZEPHYRUM && rete->instructiones == NIHIL)
    {
        redde FALSUM;
    }
    si (rete->exitus_numerus > ZEPHYRUM && rete->exitus == NIHIL)
    {
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_instructionem_addere (
                Nervus* rete,
           NervusGenus  genus,
                   i32  termini,
          constans s32* fontes,
       constans Magnus* coefficientes,
                Magnus  addendum,
                   s32* registrum_novum)
{
                 i32  registra;
                 i32  k;
    NervusInstructio* instructio;

    si (   !_caput_validum(rete) || rete->piscina == NIHIL
        || registrum_novum == NIHIL)
    {
        redde FALSUM;
    }
    registra = rete->introitus + rete->numerus;
    si (registra >= NERVUS_REGISTRA_MAXIMA)
    {
        redde FALSUM;
    }
    si (termini > NERVUS_TERMINI_MAXIMI)
    {
        redde FALSUM;
    }
    si (   termini > ZEPHYRUM
        && (fontes == NIHIL || coefficientes == NIHIL))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < termini; k++)
    {
        si (fontes[k] < ZEPHYRUM || (i32)fontes[k] >= registra)
        {
            redde FALSUM;
        }
    }
    /* crescere: alveus novus duplex, priores copiantur */
    si (rete->numerus == rete->capacitas)
    {
                     i32 nova = rete->capacitas > ZEPHYRUM
                                 ? II * rete->capacitas : XVI;
        NervusInstructio* alveus;

        alveus = (NervusInstructio*)piscina_allocare(rete->piscina,
            (memoriae_index)nova * magnitudo(NervusInstructio));
        per (k = ZEPHYRUM; k < rete->numerus; k++)
        {
            alveus[k] = rete->instructiones[k];
        }
        rete->instructiones  = alveus;
        rete->capacitas      = nova;
    }
    instructio                 = &rete->instructiones[rete->numerus];
    instructio->genus          = genus;
    instructio->termini        = termini;
    instructio->fontes         = NIHIL;
    instructio->coefficientes  = NIHIL;
    si (termini > ZEPHYRUM)
    {
        instructio->fontes = (s32*)piscina_allocare(rete->piscina,
            (memoriae_index)termini * magnitudo(s32));
        instructio->coefficientes =
            (Magnus*)piscina_allocare(rete->piscina,
            (memoriae_index)termini * magnitudo(Magnus));
        per (k = ZEPHYRUM; k < termini; k++)
        {
            instructio->fontes[k]         = fontes[k];
            instructio->coefficientes[k]  =
                magnus_transcribe(coefficientes[k],
                rete->piscina);
        }
    }
    instructio->addendum = magnus_transcribe(addendum, rete->piscina);
    rete->numerus++;
    si (genus == NERVUS_RELU)
    {
        rete->relu++;
    }
    *registrum_novum = (s32)registra;
    redde VERUM;
}


/* ==================================================
 * Constructio
 * ================================================== */

b32
nervus_creare (
         i32   introitus,
     Piscina*  piscina,
      Nervus** exitus)
{
    Nervus* rete;

    si (   introitus > NERVUS_REGISTRA_MAXIMA || piscina == NIHIL
        || exitus == NIHIL)
    {
        redde FALSUM;
    }
    rete = (Nervus*)piscina_allocare(piscina, magnitudo(Nervus));
    rete->introitus = introitus;
    rete->numerus = ZEPHYRUM;
    rete->capacitas = ZEPHYRUM;
    rete->instructiones = NIHIL;
    rete->relu = ZEPHYRUM;
    rete->exitus_numerus = ZEPHYRUM;
    rete->exitus = NIHIL;
    rete->piscina = piscina;
    *exitus = rete;
    redde VERUM;
}

b32
nervus_affinis (
               Nervus* rete,
                  i32  termini,
         constans s32* fontes,
      constans Magnus* coefficientes,
               Magnus  addendum,
                  s32* registrum_novum)
{
    redde _instructionem_addere(rete, NERVUS_AFFINIS, termini, fontes,
        coefficientes, addendum, registrum_novum);
}

b32
nervus_relu (
               Nervus* rete,
                  i32  termini,
         constans s32* fontes,
      constans Magnus* coefficientes,
               Magnus  addendum,
                  s32* registrum_novum)
{
    redde _instructionem_addere(rete, NERVUS_RELU, termini, fontes,
        coefficientes, addendum, registrum_novum);
}

b32
nervus_exitus (
          Nervus* rete,
             i32  numerus,
    constans s32* registra)
{
     i32  definita;
     i32  k;
     s32* alveus;

    si (!_caput_validum(rete) || rete->piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (numerus > NERVUS_REGISTRA_MAXIMA)
    {
        redde FALSUM;
    }
    si (numerus > ZEPHYRUM && registra == NIHIL)
    {
        redde FALSUM;
    }
    definita = rete->introitus + rete->numerus;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (registra[k] < ZEPHYRUM || (i32)registra[k] >= definita)
        {
            redde FALSUM;
        }
    }
    alveus = NIHIL;
    si (numerus > ZEPHYRUM)
    {
        alveus = (s32*)piscina_allocare(rete->piscina,
            (memoriae_index)numerus * magnitudo(s32));
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            alveus[k] = registra[k];
        }
    }
    rete->exitus          = alveus;
    rete->exitus_numerus  = numerus;
    redde VERUM;
}

b32
nervus_validus (
    constans Nervus* rete)
{
    i32 k;
    i32 j;
    i32 relu = ZEPHYRUM;
    i32 definita;

    si (!_caput_validum(rete))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
        constans NervusInstructio* instructio = &rete->instructiones[k];

        definita = rete->introitus + k;
        si (   instructio->genus != NERVUS_AFFINIS
            && instructio->genus != NERVUS_RELU)
        {
            redde FALSUM;
        }
        si (instructio->termini > NERVUS_TERMINI_MAXIMI)
        {
            redde FALSUM;
        }
        si (   instructio->termini > ZEPHYRUM
            && (instructio->fontes == NIHIL
            || instructio->coefficientes == NIHIL))
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            si (   instructio->fontes[j] < ZEPHYRUM
                || (i32)instructio->fontes[j] >= definita)
            {
                redde FALSUM;
            }
        }
        si (instructio->genus == NERVUS_RELU)
        {
            relu++;
        }
    }
    si (relu != rete->relu)
    {
        redde FALSUM;
    }
    definita = rete->introitus + rete->numerus;
    per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
    {
        si (   rete->exitus[k] < ZEPHYRUM
            || (i32)rete->exitus[k] >= definita)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Aestimatio
 * ================================================== */

/* forma utilis: non NIHIL, bitus non NIHIL */
interior b32
_forma_valida (
    constans NervusForma* forma)
{
    redde forma != NIHIL && forma->bitus != NIHIL;
}

interior b32
_bitus (
    constans NervusForma* forma,
                     i32  k)
{
    redde ((forma->bitus[k >> III] >> (k & VII)) & I) != ZEPHYRUM;
}

interior vacuum
_bitum_ponere (
    NervusForma* forma,
            i32  k,
            b32  activa)
{
    i8 larva = (i8)(I << (k & VII));

    si (activa)
    {
        forma->bitus[k >> III] = (i8)(forma->bitus[k >> III] | larva);
    }
    alioquin
    {
        forma->bitus[k >> III] = (i8)(forma->bitus[k >> III]
            & (i8)~larva);
    }
}

b32
nervus_forma_creare (
    constans Nervus* rete,
            Piscina* piscina,
        NervusForma* exitus)
{
    i32  octeti;
    i32  k;
     i8* bitus;

    si (!nervus_validus(rete) || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    octeti = (rete->relu + VII) / VIII;
    si (octeti == ZEPHYRUM)
    {
        octeti = I;     /* numquam NIHIL */
    }
    bitus = (i8*)piscina_allocare(piscina, (memoriae_index)octeti);
    per (k = ZEPHYRUM; k < octeti; k++)
    {
        bitus[k] = ZEPHYRUM;
    }
    exitus->numerus  = rete->relu;
    exitus->bitus    = bitus;
    exitus->nullae   = ZEPHYRUM;
    redde VERUM;
}

b32
nervus_forma_bitus (
    constans NervusForma* forma,
                     i32  k)
{
    si (!_forma_valida(forma) || k >= forma->numerus)
    {
        redde FALSUM;
    }
    redde _bitus(forma, k);
}

/* aestimatio communis: scripta (NIHIL licet) bitus accipit; coacta
 * (NIHIL licet) ReLU regit */
interior b32
_aestimare (
         constans Nervus* rete,
         constans Magnus* introitus,
                  Magnus* exitus,
             NervusForma* scripta,
    constans NervusForma* coacta,
                 Piscina* piscina)
{
       i32  registra;
       i32  k;
       i32  j;
       i32  r       = ZEPHYRUM;
       i32  nullae  = ZEPHYRUM;
    Magnus* valores;

    si (!nervus_validus(rete) || piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (rete->introitus > ZEPHYRUM && introitus == NIHIL)
    {
        redde FALSUM;
    }
    si (rete->exitus_numerus > ZEPHYRUM && exitus == NIHIL)
    {
        redde FALSUM;
    }
    si (   scripta != NIHIL
        && (scripta->numerus != rete->relu || scripta->bitus == NIHIL))
    {
        redde FALSUM;
    }
    si (   coacta != NIHIL
        && (coacta->numerus != rete->relu || coacta->bitus == NIHIL))
    {
        redde FALSUM;
    }
    registra  = rete->introitus + rete->numerus;
    valores   = NIHIL;
    si (registra > ZEPHYRUM)
    {
        valores = (Magnus*)piscina_allocare(piscina,
            (memoriae_index)registra * magnitudo(Magnus));
    }
    per (k = ZEPHYRUM; k < rete->introitus; k++)
    {
        valores[k] = introitus[k];
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
         constans NervusInstructio* instructio;
                            Magnus  summa;

        instructio  = &rete->instructiones[k];
        summa       = instructio->addendum;
        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            summa = magnus_adde(summa, magnus_multiplica(
                instructio->coefficientes[j],
                valores[instructio->fontes[j]], piscina), piscina);
        }
        si (instructio->genus == NERVUS_RELU)
        {
            b32 activa = coacta != NIHIL ? _bitus(coacta, r)
                         : magnus_signum(summa) > ZEPHYRUM;

            si (magnus_signum(summa) == ZEPHYRUM)
            {
                nullae++;
            }
            si (!activa)
            {
                summa = magnus_ex_s64(ZEPHYRUM);
            }
            si (scripta != NIHIL)
            {
                _bitum_ponere(scripta, r, activa);
            }
            r++;
        }
        valores[rete->introitus + k] = summa;
    }
    per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
    {
        exitus[k] = valores[rete->exitus[k]];
    }
    si (scripta != NIHIL)
    {
        scripta->nullae = nullae;
    }
    redde VERUM;
}

b32
nervus_aestimare (
    constans Nervus* rete,
    constans Magnus* introitus,
             Magnus* exitus,
        NervusForma* forma,
            Piscina* piscina)
{
    redde _aestimare(rete, introitus, exitus, forma, NIHIL, piscina);
}

b32
nervus_aestimare_coacte (
         constans Nervus* rete,
         constans Magnus* introitus,
                  Magnus* exitus,
    constans NervusForma* forma,
                 Piscina* piscina)
{
    si (forma == NIHIL)
    {
        redde FALSUM;
    }
    redde _aestimare(rete, introitus, exitus, NIHIL, forma, piscina);
}

b32
nervus_figura (
    constans Nervus* rete,
       NervusFigura* exitus)
{
         Piscina* temporaria;
             i32  registra;
             i32  k;
             i32  j;
             s32* profunditas;
             s32* profunditas_relu;
             s32* ultimus;       /* ultimus usus; numerus = exitus */
             s32* differentia;   /* tempora -1 .. numerus-1, +1 */
             s32  vivi     = ZEPHYRUM;
             s32  maximum  = ZEPHYRUM;
    NervusFigura  figura;

    si (!nervus_validus(rete) || exitus == NIHIL)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("nervus_figura",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    registra = rete->introitus + rete->numerus;
    profunditas = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(registra + I) * magnitudo(s32));
    profunditas_relu = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(registra + I) * magnitudo(s32));
    ultimus = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(registra + I) * magnitudo(s32));
    differentia = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(rete->numerus + III) * magnitudo(s32));
    per (k = ZEPHYRUM; k < registra; k++)
    {
        profunditas[k]       = ZEPHYRUM;
        profunditas_relu[k]  = ZEPHYRUM;
        ultimus[k]           = -I;
    }
    per (k = ZEPHYRUM; k < rete->numerus + III; k++)
    {
        differentia[k] = ZEPHYRUM;
    }
    figura.instructiones     = rete->numerus;
    figura.relu              = rete->relu;
    figura.termini           = ZEPHYRUM;
    figura.profunditas       = ZEPHYRUM;
    figura.profunditas_relu  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
         constans NervusInstructio* instructio;
                               s32  p     = ZEPHYRUM;
                               s32  q     = ZEPHYRUM;
                               i32  ipse  = rete->introitus + k;

        instructio = &rete->instructiones[k];

        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            s32 f = instructio->fontes[j];

            si (profunditas[f] > p)
            {
                p = profunditas[f];
            }
            si (profunditas_relu[f] > q)
            {
                q = profunditas_relu[f];
            }
            ultimus[f] = (s32)k;
        }
        profunditas[ipse] = p + I;
        profunditas_relu[ipse] = q + (instructio->genus
            == NERVUS_RELU ? I : ZEPHYRUM);
        si ((i32)profunditas[ipse] > figura.profunditas)
        {
            figura.profunditas = (i32)profunditas[ipse];
        }
        si ((i32)profunditas_relu[ipse] > figura.profunditas_relu)
        {
            figura.profunditas_relu = (i32)profunditas_relu[ipse];
        }
        figura.termini += (i64)instructio->termini;
    }
    per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
    {
        ultimus[rete->exitus[k]] = (s32)rete->numerus;
    }
    /* registrum r (definitum tempore d: -1 pro introitu, k pro
     * instructione k) vivit temporibus d .. u-1, u = ultimus usus */
    per (k = ZEPHYRUM; k < registra; k++)
    {
        s32 d = k < rete->introitus ? -I
                : (s32)(k - rete->introitus);
        s32 u = ultimus[k];

        si (u > d)
        {
            differentia[d + I]++;
            differentia[u + I]--;
        }
    }
    per (k = ZEPHYRUM; k < rete->numerus + II; k++)
    {
        vivi += differentia[k];
        si (vivi > maximum)
        {
            maximum = vivi;
        }
    }
    figura.latitudo = (i32)maximum;
    piscina_destruere(temporaria);
    *exitus = figura;
    redde VERUM;
}


/* ==================================================
 * Formae
 * ================================================== */

b32
nervus_forma_ponere (
    NervusForma* forma,
            i32  k,
            b32  activa)
{
    si (!_forma_valida(forma) || k >= forma->numerus)
    {
        redde FALSUM;
    }
    _bitum_ponere(forma, k, activa);
    redde VERUM;
}

i32
nervus_forma_activae (
    constans NervusForma* forma)
{
    i32 k;
    i32 numerus = ZEPHYRUM;

    si (!_forma_valida(forma))
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < forma->numerus; k++)
    {
        si (_bitus(forma, k))
        {
            numerus++;
        }
    }
    redde numerus;
}

b32
nervus_formae_aequales (
    constans NervusForma* a,
    constans NervusForma* b)
{
    i32 k;

    si (   !_forma_valida(a) || !_forma_valida(b)
        || a->numerus != b->numerus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < a->numerus; k++)
    {
        si (_bitus(a, k) != _bitus(b, k))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
nervus_forma_clavis (
    constans NervusForma* forma,
                 Piscina* piscina,
                  chorda* exitus)
{
    i32  octeti;
    i32  k;
     i8* datum;

    si (!_forma_valida(forma) || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    octeti  = (forma->numerus + VII) / VIII;
    datum   = (i8*)piscina_allocare(piscina,
        (memoriae_index)(octeti > ZEPHYRUM ? octeti : I));
    per (k = ZEPHYRUM; k < octeti; k++)
    {
        datum[k] = forma->bitus[k];
    }
    /* bitus ultra numerum nulli */
    si ((forma->numerus & VII) != ZEPHYRUM)
    {
        datum[octeti - I] = (i8)(datum[octeti - I]
            & (i8)((I << (forma->numerus & VII)) - I));
    }
    exitus->mensura  = octeti;
    exitus->datum    = datum;
    redde VERUM;
}


/* ==================================================
 * Mappa affinis formae
 * ================================================== */

b32
nervus_forma_affinis (
         constans Nervus*  rete,
    constans NervusForma*  forma,
                 Piscina*  piscina,
                  Matrix*  m,
                  Magnus** c)
{
    Piscina* temporaria;
     Matrix  matrix_effectus;
     Magnus* cc;
     Magnus* formae;     /* registra x (introitus + 1): coef..., c */
        i32  latus;
        i32  registra;
        i32  k;
        i32  j;
        i32  t;
        i32  r = ZEPHYRUM;

    si (   !nervus_validus(rete) || !_forma_valida(forma)
        || forma->numerus != rete->relu || piscina == NIHIL
        || m              == NIHIL
        || c              == NIHIL)
    {
        redde FALSUM;
    }
    latus     = rete->introitus + I;
    registra  = rete->introitus + rete->numerus;
    /* formae registra x latus Magnus; indices in i32 */
    si ((memoriae_index)registra * (memoriae_index)latus
        > ((memoriae_index)I << XXVII))
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("nervus_affinis",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    /* alveus magnus: conari, ut defectus FALSUM reddat, non abortum */
    formae    = (Magnus*)piscina_conari_allocare(temporaria,
        (memoriae_index)registra * (memoriae_index)latus
        * magnitudo(Magnus));
    si (formae == NIHIL)
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < rete->introitus; k++)
    {
        per (t = ZEPHYRUM; t < latus; t++)
        {
            formae[k * latus + t] = magnus_ex_s64(t
                == k ? I : ZEPHYRUM);
        }
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
        constans NervusInstructio* instructio;
                           Magnus* ipsa;

        instructio  = &rete->instructiones[k];
        ipsa        = &formae[(rete->introitus + k) * latus];
        per (t = ZEPHYRUM; t < rete->introitus; t++)
        {
            ipsa[t] = magnus_ex_s64(ZEPHYRUM);
        }
        ipsa[rete->introitus] = instructio->addendum;
        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            Magnus* fons = &formae[(i32)instructio->fontes[j] * latus];

            per (t = ZEPHYRUM; t < latus; t++)
            {
                ipsa[t] = magnus_adde(ipsa[t], magnus_multiplica(
                    instructio->coefficientes[j], fons[t], temporaria),
                    temporaria);
            }
        }
        si (instructio->genus == NERVUS_RELU)
        {
            si (!_bitus(forma, r))
            {
                per (t = ZEPHYRUM; t < latus; t++)
                {
                    ipsa[t] = magnus_ex_s64(ZEPHYRUM);
                }
            }
            r++;
        }
    }
    si (!matrix_nulla(&ANULUS_INTEGRORUM, rete->exitus_numerus,
        rete->introitus, piscina, &matrix_effectus))
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    cc = NIHIL;
    si (rete->exitus_numerus > ZEPHYRUM)
    {
        cc = (Magnus*)piscina_allocare(piscina,
            (memoriae_index)rete->exitus_numerus * magnitudo(Magnus));
    }
    per (k = ZEPHYRUM; k < rete->exitus_numerus; k++)
    {
        Magnus* ipsa = &formae[(i32)rete->exitus[k] * latus];

        per (t = ZEPHYRUM; t < rete->introitus; t++)
        {
            Magnus v = magnus_transcribe(ipsa[t], piscina);

            matrix_pone(&matrix_effectus, k, t, &v);
        }
        cc[k] = magnus_transcribe(ipsa[rete->introitus], piscina);
    }
    piscina_destruere(temporaria);
    *m = matrix_effectus;
    *c = cc;
    redde VERUM;
}


/* ==================================================
 * Compositio
 * ================================================== */

b32
nervus_componere (
    constans Nervus*  a,
    constans Nervus*  b,
            Piscina*  piscina,
             Nervus** exitus)
{
    Piscina* temporaria;
     Nervus* rete;
        s32* mappa;      /* registra b -> registra compositi */
        s32* fontes;
        s32* exitus_novi;
        i32  k;
        i32  j;
        b32  bene = VERUM;

    si (   !nervus_validus(a) || !nervus_validus(b) || piscina == NIHIL
        || exitus == NIHIL || a->exitus_numerus != b->introitus)
    {
        redde FALSUM;
    }
    si (a->introitus + a->numerus + b->numerus > NERVUS_REGISTRA_MAXIMA)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("nervus_componere", IV
        * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    mappa = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(b->introitus + b->numerus
            + I) * magnitudo(s32));
    /* alveus fontium ad terminos plurimos instructionis b */
    {
        i32 plurimi = I;

        per (k = ZEPHYRUM; k < b->numerus; k++)
        {
            si (b->instructiones[k].termini > plurimi)
            {
                plurimi = b->instructiones[k].termini;
            }
        }
        fontes = (s32*)piscina_allocare(temporaria,
            (memoriae_index)plurimi * magnitudo(s32));
    }
    exitus_novi = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(b->exitus_numerus + I) * magnitudo(s32));
    (vacuum)nervus_creare(a->introitus, piscina, &rete);
    per (k = ZEPHYRUM; k < a->numerus && bene; k++)
    {
         constans NervusInstructio* instructio = &a->instructiones[k];
                               s32  novum;

        bene = _instructionem_addere(rete, instructio->genus,
            instructio->termini, instructio->fontes,
            instructio->coefficientes, instructio->addendum, &novum);
    }
    per (k = ZEPHYRUM; k < b->introitus; k++)
    {
        mappa[k] = a->exitus[k];
    }
    per (k = ZEPHYRUM; k < b->numerus && bene; k++)
    {
         constans NervusInstructio* instructio  = &b->instructiones[k];
                               s32  novum       = -I;

        per (j = ZEPHYRUM; j < instructio->termini; j++)
        {
            fontes[j] = mappa[instructio->fontes[j]];
        }
        bene = _instructionem_addere(rete, instructio->genus,
            instructio->termini, fontes, instructio->coefficientes,
            instructio->addendum, &novum);
        mappa[b->introitus + k] = novum;
    }
    per (k = ZEPHYRUM; k < b->exitus_numerus && bene; k++)
    {
        exitus_novi[k] = mappa[b->exitus[k]];
    }
    si (bene)
    {
        bene = nervus_exitus(rete, b->exitus_numerus, exitus_novi);
    }
    piscina_destruere(temporaria);
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = rete;
    redde VERUM;
}


/* ==================================================
 * Regiones plani
 * ================================================== */

/* arcus elementarius: ab initio ad initium arcus sequentis, angulus
 * < pi. formae: registra x II, forma linearis (coefficientes x, y)
 * cuiusque registri iam computati in arcu */
nomen structura {
    NervusRadius  initium;
          Magnus* formae;
              i8* bitus;
             i32  nullae;
} ArcusPlani;

/* g(u) = g_x u_x + g_y u_y */
interior Magnus
_applicare (
     constans Magnus* g,
        NervusRadius  u,
             Piscina* piscina)
{
    redde magnus_adde(magnus_multiplica(g[ZEPHYRUM], u.x, piscina),
        magnus_multiplica(g[I], u.y, piscina), piscina);
}

/* radius primitivus ex vectore non nullo */
interior NervusRadius
_primitivus (
      Magnus  x,
      Magnus  y,
     Piscina* piscina)
{
    NervusRadius radius;
          Magnus d = magnus_divisor_communis(x, y, piscina);

    (vacuum)magnus_divide(x, d, piscina, &radius.x, NIHIL);
    (vacuum)magnus_divide(y, d, piscina, &radius.y, NIHIL);
    redde radius;
}

interior NervusRadius
_radium_transcribere (
    NervusRadius  radius,
         Piscina* piscina)
{
    NervusRadius copia;

    copia.x = magnus_transcribe(radius.x, piscina);
    copia.y = magnus_transcribe(radius.y, piscina);
    redde copia;
}

/* arcus novus ab initio; formae registrorum 0 .. definita-1, bitus et
 * nullae ex exemplari copiantur */
interior b32
_arcum_creare (
             ArcusPlani* arcus,
           NervusRadius  initium,
    constans ArcusPlani* exemplar,
                    i32  registra,
                    i32  definita,
                    i32  octeti,
                Piscina* piscina)
{
    i32 k;

    arcus->formae = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)registra * II * magnitudo(Magnus));
    arcus->bitus = (i8*)piscina_conari_allocare(piscina,
        (memoriae_index)octeti);
    si (arcus->formae == NIHIL || arcus->bitus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < II * definita; k++)
    {
        arcus->formae[k] = exemplar->formae[k];
    }
    per (k = ZEPHYRUM; k < octeti; k++)
    {
        arcus->bitus[k] = exemplar->bitus[k];
    }
    arcus->initium  = initium;
    arcus->nullae   = exemplar->nullae;
    redde VERUM;
}

/* forma registri ipse = summa c_j formae[f_j] (sine addendo) */
interior vacuum
_formam_computare (
                   ArcusPlani* arcus,
    constans NervusInstructio* instructio,
                          i32  ipse,
                      Piscina* piscina)
{
    Magnus summa_x = magnus_ex_s64(ZEPHYRUM);
    Magnus summa_y = magnus_ex_s64(ZEPHYRUM);
       i32 j;

    per (j = ZEPHYRUM; j < instructio->termini; j++)
    {
        constans Magnus* fons = &arcus->formae[II
            * (i32)instructio->fontes[j]];

        summa_x = magnus_adde(summa_x, magnus_multiplica(
            instructio->coefficientes[j], fons[ZEPHYRUM], piscina),
            piscina);
        summa_y = magnus_adde(summa_y, magnus_multiplica(
            instructio->coefficientes[j], fons[I], piscina), piscina);
    }
    arcus->formae[II * ipse]      = summa_x;
    arcus->formae[II * ipse + I]  = summa_y;
}

/* ReLU r in arcu: activa -> forma manet, aliter 0; bitus r */
interior vacuum
_relu_in_arcu (
    ArcusPlani* arcus,
           i32  ipse,
           i32  r,
           b32  activa)
{
    NervusForma forma;

    si (!activa)
    {
        arcus->formae[II * ipse]      = magnus_ex_s64(ZEPHYRUM);
        arcus->formae[II * ipse + I]  = magnus_ex_s64(ZEPHYRUM);
    }
    forma.bitus = arcus->bitus;
    _bitum_ponere(&forma, r, activa);
}

interior b32
_bitus_aequales (
    constans i8* a,
    constans i8* b,
            i32  octeti)
{
    i32 k;

    per (k = ZEPHYRUM; k < octeti; k++)
    {
        si (a[k] != b[k])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* arcus post omnes instructiones (temporaria); FALSUM si memoria
 * deficit */
interior b32
_arcus_plani (
    constans Nervus*  rete,
                i32   octeti,
            Piscina*  temporaria,
         ArcusPlani** exitus,
                i32*  numerus_exitus)
{
    constans s64 axes[IV][II] = {
        { I, ZEPHYRUM }, { ZEPHYRUM, I }, { -I, ZEPHYRUM },
        { ZEPHYRUM, -I }
    };
          ArcusPlani* vetera;
          ArcusPlani* nova;
          ArcusPlani  initialis;
                 i32  numerus            = IV;
                 i32  capacitas_veterum  = VIII;
                 i32  capacitas_novorum  = VIII;
                 i32  registra           = II + rete->numerus;
                 i32  k;
                 i32  a;
                 i32  r          = ZEPHYRUM;

    vetera = (ArcusPlani*)piscina_conari_allocare(temporaria,
        (memoriae_index)capacitas_veterum * magnitudo(ArcusPlani));
    nova = (ArcusPlani*)piscina_conari_allocare(temporaria,
        (memoriae_index)capacitas_novorum * magnitudo(ArcusPlani));
    initialis.formae = (Magnus*)piscina_conari_allocare(temporaria,
        IV * magnitudo(Magnus));
    initialis.bitus = (i8*)piscina_conari_allocare(temporaria,
        (memoriae_index)octeti);
    si (   vetera == NIHIL || nova == NIHIL || initialis.formae == NIHIL
        || initialis.bitus == NIHIL)
    {
        redde FALSUM;
    }
    /* introitus: x -> (1, 0), y -> (0, 1) */
    initialis.formae[ZEPHYRUM]  = magnus_ex_s64(I);
    initialis.formae[I]         = magnus_ex_s64(ZEPHYRUM);
    initialis.formae[II]        = magnus_ex_s64(ZEPHYRUM);
    initialis.formae[III]       = magnus_ex_s64(I);
    per (k = ZEPHYRUM; k < octeti; k++)
    {
        initialis.bitus[k] = ZEPHYRUM;
    }
    initialis.nullae = ZEPHYRUM;
    per (a = ZEPHYRUM; a < IV; a++)
    {
        NervusRadius axis;

        axis.x = magnus_ex_s64(axes[a][ZEPHYRUM]);
        axis.y = magnus_ex_s64(axes[a][I]);
        si (!_arcum_creare(&vetera[a], axis, &initialis, registra, II,
            octeti, temporaria))
        {
            redde FALSUM;
        }
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
        constans NervusInstructio* instructio =
            &rete->instructiones[k];
                               i32 ipse  = II + k;
                               i32 m     = ZEPHYRUM;

        si (instructio->genus == NERVUS_AFFINIS)
        {
            per (a = ZEPHYRUM; a < numerus; a++)
            {
                _formam_computare(&vetera[a], instructio, ipse,
                    temporaria);
            }
            perge;
        }
        /* quisque arcus in duos ad summum */
        si (II * numerus > capacitas_novorum)
        {
            capacitas_novorum = II * numerus;
            nova = (ArcusPlani*)piscina_conari_allocare(temporaria,
                (memoriae_index)capacitas_novorum
                * magnitudo(ArcusPlani));
            si (nova == NIHIL)
            {
                redde FALSUM;
            }
        }
        per (a = ZEPHYRUM; a < numerus; a++)
        {
              ArcusPlani* arcus  = &vetera[a];
            NervusRadius  v      = vetera[(a + I) % numerus].initium;
                  Magnus  ad_initium;
                  Magnus  ad_finem;
                     s32  su;
                     s32  sv;

            _formam_computare(arcus, instructio, ipse, temporaria);
            ad_initium  = _applicare(&arcus->formae[II * ipse],
                arcus->initium,
                temporaria);
            ad_finem = _applicare(&arcus->formae[II * ipse], v,
                temporaria);
            su = magnus_signum(ad_initium);
            sv = magnus_signum(ad_finem);
            si (su * sv < ZEPHYRUM)
            {
                /* radius zero: |g(v)| u + |g(u)| v, intra arcum */
                      Magnus au = magnus_absolutum(ad_initium,
                          temporaria);
                      Magnus av = magnus_absolutum(ad_finem,
                          temporaria);
                NervusRadius z;

                z = _primitivus(
                    magnus_adde(magnus_multiplica(av, arcus->initium.x,
                    temporaria), magnus_multiplica(au, v.x, temporaria),
                    temporaria),
                    magnus_adde(magnus_multiplica(av, arcus->initium.y,
                    temporaria), magnus_multiplica(au, v.y, temporaria),
                    temporaria), temporaria);
                si (!_arcum_creare(&nova[m + I], z, arcus, registra,
                    ipse + I, octeti, temporaria))
                {
                    redde FALSUM;
                }
                nova[m] = *arcus;
                _relu_in_arcu(&nova[m], ipse, r, su > ZEPHYRUM);
                _relu_in_arcu(&nova[m + I], ipse, r, sv > ZEPHYRUM);
                m += II;
            }
            alioquin
            {
                si (su == ZEPHYRUM && sv == ZEPHYRUM)
                {
                    arcus->nullae++;
                }
                nova[m] = *arcus;
                _relu_in_arcu(&nova[m], ipse, r, su + sv > ZEPHYRUM);
                m++;
            }
        }
        {
            ArcusPlani* permutata = vetera;
                   i32  capacitas = capacitas_veterum;

            vetera             = nova;
            nova               = permutata;
            capacitas_veterum  = capacitas_novorum;
            capacitas_novorum  = capacitas;
            numerus            = m;
        }
        r++;
    }
    *exitus          = vetera;
    *numerus_exitus  = numerus;
    redde VERUM;
}

b32
nervus_regiones_plani (
    constans Nervus* rete,
            Piscina* piscina,
       NervusPlanum* exitus)
{
         Piscina* temporaria;
      ArcusPlani* arcus;
    NervusSector* regiones;
             i32* muri;
             i32  numerus_arcuum;
             i32  numerus_murorum  = ZEPHYRUM;
             i32  numerus;
             i32  octeti;
             i32  frusta;
             i32  k;
             i32  a;
             i32  o;

    si (   !nervus_validus(rete) || rete->introitus != II
        || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < rete->numerus; k++)
    {
        si (magnus_signum(rete->instructiones[k].addendum) != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    octeti = (rete->relu + VII) / VIII;
    si (octeti == ZEPHYRUM)
    {
        octeti = I;
    }
    temporaria = piscina_generare_dynamicum("nervus_regiones_plani",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    si (!_arcus_plani(rete, octeti, temporaria, &arcus,
        &numerus_arcuum))
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    /* murus ante arcum a: forma arcus prioris differt */
    muri = (i32*)piscina_conari_allocare(temporaria,
        (memoriae_index)numerus_arcuum * magnitudo(i32));
    si (muri == NIHIL)
    {
        piscina_destruere(temporaria);
        redde FALSUM;
    }
    per (a = ZEPHYRUM; a < numerus_arcuum; a++)
    {
        i32 prior = (a + numerus_arcuum - I) % numerus_arcuum;

        si (!_bitus_aequales(arcus[prior].bitus, arcus[a].bitus,
            octeti))
        {
            muri[numerus_murorum++] = a;
        }
    }
    /* murus unus impossibilis (forma circum rediret); nulli: sector
     * unus ab arcu 0, cuius initium (1, 0) est */
    numerus = numerus_murorum > ZEPHYRUM ? numerus_murorum : I;
    si (numerus_murorum == ZEPHYRUM)
    {
        muri[ZEPHYRUM] = ZEPHYRUM;
    }
    regiones = (NervusSector*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(NervusSector));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        NervusSector* sector = &regiones[k];
          ArcusPlani* primus = &arcus[muri[k]];
        NervusRadius  sequens  = arcus[(muri[k] + I)
                                 % numerus_arcuum].initium;
                 i8* bitus;

        sector->initium = _radium_transcribere(primus->initium,
            piscina);
        sector->finis = _radium_transcribere(arcus[muri[(k + I)
            % numerus]].initium, piscina);
        /* arcus primus < pi: u + v intra eum */
        sector->medius = _radium_transcribere(_primitivus(
            magnus_adde(primus->initium.x, sequens.x, temporaria),
            magnus_adde(primus->initium.y, sequens.y, temporaria),
            temporaria), piscina);
        bitus = (i8*)piscina_allocare(piscina, (memoriae_index)octeti);
        per (o = ZEPHYRUM; o < octeti; o++)
        {
            bitus[o] = primus->bitus[o];
        }
        sector->forma.numerus  = rete->relu;
        sector->forma.bitus    = bitus;
        sector->forma.nullae   = primus->nullae;
        si (!matrix_nulla(&ANULUS_INTEGRORUM, rete->exitus_numerus, II,
            piscina, &sector->m))
        {
            piscina_destruere(temporaria);
            redde FALSUM;
        }
        per (o = ZEPHYRUM; o < rete->exitus_numerus; o++)
        {
            constans Magnus* forma;
                     Magnus  valor_x;
                     Magnus  valor_y;

            forma    = &primus->formae[II * (i32)rete->exitus[o]];
            valor_x  = magnus_transcribe(forma[ZEPHYRUM], piscina);
            valor_y  = magnus_transcribe(forma[I], piscina);
            matrix_pone(&sector->m, o, ZEPHYRUM, &valor_x);
            matrix_pone(&sector->m, o, I, &valor_y);
        }
    }
    piscina_destruere(temporaria);
    /* frusta: regiones contiguae matrice eadem; cursus ultimus cum
     * primo coniungitur si matrix eadem (circulus) */
    regiones[ZEPHYRUM].frustum = ZEPHYRUM;
    per (k = I; k < numerus; k++)
    {
        regiones[k].frustum = regiones[k - I].frustum
            + (matrix_aequalis(regiones[k].m, regiones[k - I].m)
            ? ZEPHYRUM : I);
    }
    frusta = regiones[numerus - I].frustum + I;
    si (   frusta > I
        && matrix_aequalis(regiones[numerus - I].m,
        regiones[ZEPHYRUM].m))
    {
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            si (regiones[k].frustum == frusta - I)
            {
                regiones[k].frustum = ZEPHYRUM;
            }
        }
        frusta--;
    }
    exitus->numerus   = numerus;
    exitus->regiones  = regiones;
    exitus->frusta    = frusta;
    redde VERUM;
}
#undef ArcusPlani
#undef _aestimare
#undef _applicare
#undef _arcum_creare
#undef _arcus_plani
#undef _bitum_ponere
#undef _bitus
#undef _bitus_aequales
#undef _caput_validum
#undef _forma_valida
#undef _formam_computare
#undef _instructionem_addere
#undef _primitivus
#undef _radium_transcribere
#undef _relu_in_arcu
/* lib/plexus.c: statica per plagulam renominata */
#define ImaginesLiberae ImaginesLiberae_plexus
#define ParClavisPlexus ParClavisPlexus_plexus
#define _ansam_contrahere _ansam_contrahere_plexus
#define _ansam_littera _ansam_littera_plexus
#define _ansam_transcribere _ansam_transcribere_plexus
#define _apex_proprius _apex_proprius_plexus
#define _apicem_notare _apicem_notare_plexus
#define _burau_littera _burau_littera_plexus
#define _burau_littera_integra _burau_littera_integra_plexus
#define _clavis _clavis_plexus
#define _comparare_paria _comparare_paria_plexus
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
#define _nervus_instructio _nervus_instructio_plexus
#define _nervus_littera _nervus_littera_plexus
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
#include <stdlib.h>
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


/* ==================================================
 * Actio Dynnikov ut rete (nervus)
 * ================================================== */

/* instructio ex terminis parvis: V paria (fons, coefficiens),
 * coefficiens 0 terminum omittit. Registrum novum, aut -1 si *bene iam
 * FALSUM aut constructio refutat (tunc *bene = FALSUM). */
interior s32
_nervus_instructio (
          Nervus* rete,
     NervusGenus  genus,
             s32  f0,
             s32  c0,
             s32  f1,
             s32  c1,
             s32  f2,
             s32  c2,
             s32  f3,
             s32  c3,
             s32  f4,
             s32  c4,
             b32* bene)
{
       s32 fontes[V];
       s32 coef[V];
       s32 f[V];
    Magnus c[V];
       i32 k;
       i32 termini  = ZEPHYRUM;
       s32 novum    = -I;
       b32 factum;

    si (!*bene)
    {
        redde -I;
    }
    fontes[ZEPHYRUM]  = f0;
    fontes[I]         = f1;
    fontes[II]        = f2;
    fontes[III]       = f3;
    fontes[IV]        = f4;
    coef[ZEPHYRUM]    = c0;
    coef[I]           = c1;
    coef[II]          = c2;
    coef[III]         = c3;
    coef[IV]          = c4;
    per (k = ZEPHYRUM; k < V; k++)
    {
        si (coef[k] != ZEPHYRUM)
        {
            f[termini] = fontes[k];
            c[termini] = magnus_ex_s64((s64)coef[k]);
            termini++;
        }
    }
    si (genus == NERVUS_RELU)
    {
        factum = nervus_relu(rete, termini, f, c,
            magnus_ex_s64(ZEPHYRUM),
            &novum);
    }
    alioquin
    {
        factum = nervus_affinis(rete, termini, f, c,
            magnus_ex_s64(ZEPHYRUM), &novum);
    }
    si (!factum)
    {
        *bene = FALSUM;
        redde -I;
    }
    redde novum;
}

/* littera una: registra currentia coordinatarum (cur: a_j in cur[j-1],
 * b_j in cur[m+j-1]) legit et novat. Aeq. 8-15 (Thiffeault 2010),
 * f- = f - relu(f). */
interior b32
_nervus_littera (
     Nervus* rete,
        i32  fila,
        s32  littera,
        s32* cur)
{
    i32 m     = fila - II;
    i32 i     = (i32)(littera > ZEPHYRUM ? littera : -littera);
    b32 bene  = VERUM;

    si (i == I || i == fila - I)
    {
        /* sigma_1 (10, 14) et sigma_{n-1} (11, 15), indice j */
        i32 j = i == I ? I : m;
        s32 a = cur[j - I];
        s32 b = cur[m + j - I];
        s32 t1;
        s32 nb;
        s32 t2;
        s32 na;

        t1 = _nervus_instructio(rete, NERVUS_RELU, b, I, ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, &bene);
        si (i == I && littera > ZEPHYRUM)
        {
            /* b' = a + b+,  a' = -b + (b')+ */
            nb = _nervus_instructio(rete, NERVUS_AFFINIS, a, I, t1, I,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, &bene);
            t2 = _nervus_instructio(rete, NERVUS_RELU, nb, I,
                ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, &bene);
            na = _nervus_instructio(rete, NERVUS_AFFINIS, b, -I, t2, I,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, &bene);
        }
        alioquin si (i == I)
        {
            /* b' = b+ - a,  a' = b - (b')+ */
            nb = _nervus_instructio(rete, NERVUS_AFFINIS, t1, I, a, -I,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, &bene);
            t2 = _nervus_instructio(rete, NERVUS_RELU, nb, I,
                ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, &bene);
            na = _nervus_instructio(rete, NERVUS_AFFINIS, b, I, t2, -I,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, &bene);
        }
        alioquin si (littera > ZEPHYRUM)
        {
            /* b' = a + b-,  a' = -b + (b')-   (f- = f - f+) */
            nb = _nervus_instructio(rete, NERVUS_AFFINIS, a, I, b, I,
                t1,
                -I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
            t2 = _nervus_instructio(rete, NERVUS_RELU, nb, I,
                ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, &bene);
            na = _nervus_instructio(rete, NERVUS_AFFINIS, b, -I, nb, I,
                t2, -I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        }
        alioquin
        {
            /* b' = b- - a,  a' = b - (b')- */
            nb = _nervus_instructio(rete, NERVUS_AFFINIS, b, I, t1, -I,
                a,
                -I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
            t2 = _nervus_instructio(rete, NERVUS_RELU, nb, I,
                ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, ZEPHYRUM, &bene);
            na = _nervus_instructio(rete, NERVUS_AFFINIS, b, I, nb, -I,
                t2, I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        }
        cur[j - I]      = na;
        cur[m + j - I]  = nb;
    }
    alioquin
    {
        /* mediae, p = i-1, q = i: positiva (8)-(9) cum c, negativa
         * (12)-(13) cum d; signum sg = +1 / -1 utrumque tegit */
        i32 p          = i - I;
        i32 q          = i;
        s32 a_prior    = cur[p - I];
        s32 a_sequens  = cur[q - I];
        s32 b_prior    = cur[m + p - I];
        s32 b_sequens  = cur[m + q - I];
        s32 sg         = littera > ZEPHYRUM ? I : -I;
        s32 pos        = littera > ZEPHYRUM ? I : ZEPHYRUM;
        s32 b_sequens_positiva;
        s32 b_prior_positiva;
        s32 d;
        s32 u;
        s32 d_positiva;
        s32 w_positiva;
        s32 a_prior_nova;
        s32 b_prior_nova;
        s32 pars;
        s32 a_sequens_nova;
        s32 b_sequens_nova;

        b_sequens_positiva = _nervus_instructio(rete, NERVUS_RELU,
            b_sequens, I,
            ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, &bene);
        b_prior_positiva = _nervus_instructio(rete, NERVUS_RELU,
            b_prior, I,
            ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, &bene);
        /* c = a_p - a_q - b_q+ + b_p - b_p+
         * d = a_p - a_q + b_q+ - b_p + b_p+ */
        d = _nervus_instructio(rete, NERVUS_AFFINIS, a_prior, I,
            a_sequens, -I,
            b_sequens_positiva,
            -sg, b_prior, sg, b_prior_positiva, -sg, &bene);
        /* (b_q+ + c)+  aut  (b_q+ - d)+ */
        u = _nervus_instructio(rete, NERVUS_RELU, b_sequens_positiva, I,
            d, sg,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            &bene);
        d_positiva = _nervus_instructio(rete, NERVUS_RELU, d, I,
            ZEPHYRUM,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM, &bene);
        /* w = b_p - b_p+ - c  aut  b_p - b_p+ + d */
        w_positiva = _nervus_instructio(rete, NERVUS_RELU, b_prior, I,
            b_prior_positiva,
            -I,
            d, -sg, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        /* a_p' = a_p - b_p+ - u  aut  a_p + b_p+ + u */
        a_prior_nova = _nervus_instructio(rete, NERVUS_AFFINIS, a_prior,
            I, b_prior_positiva,
            -sg,
            u, -sg, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        /* b_p' = b_q + c - c+  aut  b_q - d+ */
        b_prior_nova = _nervus_instructio(rete, NERVUS_AFFINIS,
            b_sequens, I, d, pos,
            d_positiva,
            -I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        /* a_q' = [a_q - (b_q - b_q+) - (b_p - b_p+)] + c + w+  aut
         *       [a_q + (b_q - b_q+) + (b_p - b_p+)] + d - w+ */
        pars = _nervus_instructio(rete, NERVUS_AFFINIS, a_sequens, I,
            b_sequens, -sg,
            b_sequens_positiva, sg, b_prior, -sg, b_prior_positiva, sg,
            &bene);
        a_sequens_nova = _nervus_instructio(rete, NERVUS_AFFINIS, pars,
            I, d, I,
            w_positiva,
            sg, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &bene);
        /* b_q' = b_p - c + c+  aut  b_p + d+ */
        b_sequens_nova = _nervus_instructio(rete, NERVUS_AFFINIS,
            b_prior, I, d, -pos,
            d_positiva, I, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
            &bene);
        cur[p - I]      = a_prior_nova;
        cur[m + p - I]  = b_prior_nova;
        cur[q - I]      = a_sequens_nova;
        cur[m + q - I]  = b_sequens_nova;
    }
    redde bene;
}

b32
plexus_nervus (
      Plexus   p,
     Piscina*  piscina,
      Nervus** exitus)
{
    Piscina* temporaria;
     Nervus* rete;
        s32* cur;
        i32  m;
        i32  k;
        b32  bene = VERUM;

    si (   !plexus_validus(p) || p.fila < III || piscina == NIHIL
        || exitus == NIHIL)
    {
        redde FALSUM;
    }
    m = p.fila - II;
    /* registra exacte: 2m, deinde IV per litteram extremam (sigma_1,
     * sigma_{n-1}), XI per mediam */
    {
        memoriae_index registra = (memoriae_index)II * m;

        per (k = ZEPHYRUM; k < p.longitudo; k++)
        {
            s32 littera = p.litterae[k];
            i32 i = (i32)(littera > ZEPHYRUM ? littera : -littera);

            registra += (i == I || i == p.fila - I) ? IV : XI;
        }
        si (registra > (memoriae_index)NERVUS_REGISTRA_MAXIMA)
        {
            redde FALSUM;
        }
    }
    si (!nervus_creare(II * m, piscina, &rete))
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_nervus",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    cur = (s32*)piscina_allocare(temporaria,
        (memoriae_index)(II * m) * magnitudo(s32));
    per (k = ZEPHYRUM; k < II * m; k++)
    {
        cur[k] = (s32)k;
    }
    per (k = ZEPHYRUM; k < p.longitudo && bene; k++)
    {
        bene = _nervus_littera(rete, p.fila, p.litterae[k], cur);
    }
    si (bene)
    {
        bene = nervus_exitus(rete, II * m, cur);
    }
    piscina_destruere(temporaria);
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = rete;
    redde VERUM;
}


/* ==================================================
 * Census: clavis classis, periodicitas, enumeratio
 * ================================================== */

/* clavis in piscina data (temporaria vocantis) */
interior b32
_clavis (
      Plexus  p,
     Piscina* piscina,
      chorda* exitus)
{
                i32  fila = p.fila + II;
             Plexus  maior;
  ChordaAedificator* aedificator;
                i32  k;

    si (!plexus_validus(p) || p.fila > PLEXUS_FILA_MAXIMA - II)
    {
        redde FALSUM;
    }
    si (!plexus_in_maiore(p, fila, I, piscina, &maior))
    {
        redde FALSUM;
    }
    aedificator = chorda_aedificator_creare(piscina, LXIV);
    per (k = I; k < fila; k++)
    {
        Ansa e;
        Ansa imago;

        si (   !ansa_circa(fila, k, piscina, &e)
            || !ansa_agere(e, maior, piscina, &imago))
        {
            redde FALSUM;
        }
        (vacuum)chorda_aedificator_appendere_chorda(aedificator,
            ansa_ad_chordam(imago, piscina));
    }
    *exitus = chorda_aedificator_finire(aedificator);
    redde VERUM;
}

b32
plexus_clavis (
      Plexus  p,
     Piscina* piscina,
      chorda* exitus)
{
    Piscina* temporaria;
     chorda  clavis;
        b32  bene;

    si (piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_clavis",
        IV * MXXIV);
    si (temporaria == NIHIL)
    {
        redde FALSUM;
    }
    bene = _clavis(p, temporaria, &clavis);
    si (bene)
    {
        *exitus = chorda_transcribere(clavis, piscina);
    }
    piscina_destruere(temporaria);
    redde bene;
}

b32
plexus_periodus (
      Plexus  p,
     Piscina* piscina,
         i32* periodus)
{
    PiscinaNotatio nota;
               i32 n;
               s64 e;
               s64 torsiones;
               i32 k;
            Plexus torsio;
            Plexus plena;

    si (   !plexus_validus(p) || piscina == NIHIL || periodus == NIHIL
        || p.fila > PLEXUS_FILA_DIMIDIAE_TORSIONIS)
    {
        redde FALSUM;
    }
    n = p.fila;
    e = plexus_summa_exponentium(p);
    /* summa exponentium Delta^2 */
    torsiones  = (s64)n * (s64)(n - I);
    nota       = piscina_notare(piscina);
    si (   !plexus_dimidia_torsio(n, piscina, &torsio)
        || !plexus_iungere(torsio, torsio, piscina, &plena))
    {
        piscina_reficere(piscina, nota);
        redde FALSUM;
    }
    per (k = I; k <= n; k++)
    {
        PiscinaNotatio nota_k;
                   s64 j;
                Plexus sinistra;
                Plexus dextra;
                Plexus basis;
                   b32 aequales  = FALSUM;
                   b32 bene      = VERUM;
                   i32 t;

        si (n % k != ZEPHYRUM && (n - I) % k != ZEPHYRUM)
        {
            perge;
        }
        si (((s64)k * e) % torsiones != ZEPHYRUM)
        {
            perge;
        }
        j       = ((s64)k * e) / torsiones;
        nota_k  = piscina_notare(piscina);
        /* producta refutata (verba nimis longa) -> FALSUM, numquam
         * comparatio truncatorum */
        bene = plexus_creare(n, NIHIL, ZEPHYRUM, piscina, &sinistra);
        per (t = ZEPHYRUM; t < k && bene; t++)
        {
            bene = plexus_iungere(sinistra, p, piscina, &sinistra);
        }
        bene = bene && plexus_creare(n, NIHIL, ZEPHYRUM, piscina,
            &dextra);
        basis = j >= ZEPHYRUM ? plena : plexus_inversum(plena, piscina);
        per (t = ZEPHYRUM; bene
            && (s64)t < (j >= ZEPHYRUM ? j : -j); t++)
        {
            bene = plexus_iungere(dextra, basis, piscina, &dextra);
        }
        si (!bene || !plexus_aequales_ansis(sinistra, dextra, piscina,
            &aequales))
        {
            piscina_reficere(piscina, nota);
            redde FALSUM;
        }
        piscina_reficere(piscina, nota_k);
        si (aequales)
        {
            piscina_reficere(piscina, nota);
            *periodus = k;
            redde VERUM;
        }
    }
    piscina_reficere(piscina, nota);
    *periodus = ZEPHYRUM;
    redde VERUM;
}

/* ordo intra gradum: paria (clavis, index) per claves */
nomen structura {
    chorda clavis;
       i32 index;
} ParClavisPlexus;

interior s32
_comparare_paria (
    constans vacuum* a,
    constans vacuum* b)
{
    redde (s32)chorda_comparare(((constans ParClavisPlexus*)a)->clavis,
        ((constans ParClavisPlexus*)b)->clavis);
}

b32
plexus_enumerare (
             i32  fila,
             i32  longitudo_maxima,
             i32  classes_maximae,
         Piscina* piscina,
    PlexusCensus* exitus)
{
              Piscina* temporaria;
              Piscina* servata;     /* verba et claves usque ad finem */
       TabulaDispersa* visae;
               Plexus* verba;
               chorda* claves;
                  i32* initia;
      ParClavisPlexus* paria;
               Plexus* verba_tmp;
               chorda* claves_tmp;
                  i32  capacitas  = CCLVI;
                  i32  numerus    = ZEPHYRUM;
                  i32  gradus;
                  i32  k;
                  b32  bene = VERUM;
               Plexus  vacuum_verbum;
               chorda  clavis;

    si (   fila < II || fila > PLEXUS_FILA_MAXIMA - II
        || piscina == NIHIL
        || exitus  == NIHIL || longitudo_maxima >= classes_maximae
        || (s64)longitudo_maxima > PLEXUS_LONGITUDO_SUMMA)
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("plexus_enumerare", LXIV
        * MXXIV);
    servata = piscina_generare_dynamicum("plexus_enumerare_servata",
        LXIV * MXXIV);
    si (temporaria == NIHIL || servata == NIHIL)
    {
        si (temporaria != NIHIL)
        {
            piscina_destruere(temporaria);
        }
        si (servata != NIHIL)
        {
            piscina_destruere(servata);
        }
        redde FALSUM;
    }
    visae   = tabula_dispersa_creare_chorda(temporaria, MXXIV);
    verba   = (Plexus*)piscina_allocare(temporaria,
        (memoriae_index)capacitas * magnitudo(Plexus));
    claves  = (chorda*)piscina_allocare(temporaria,
        (memoriae_index)capacitas * magnitudo(chorda));
    initia  = (i32*)piscina_conari_allocare(temporaria,
        (memoriae_index)(longitudo_maxima + II) * magnitudo(i32));
    si (initia == NIHIL)
    {
        piscina_destruere(temporaria);
        piscina_destruere(servata);
        redde FALSUM;
    }
    /* gradus 0: identitas */
    (vacuum)plexus_creare(fila, NIHIL, ZEPHYRUM, servata,
        &vacuum_verbum);
    {
        PiscinaNotatio nota = piscina_notare(temporaria);

        bene = _clavis(vacuum_verbum, temporaria, &clavis);
        si (bene)
        {
            clavis = chorda_transcribere(clavis, servata);
        }
        piscina_reficere(temporaria, nota);
    }
    si (!bene)
    {
        piscina_destruere(temporaria);
        piscina_destruere(servata);
        redde FALSUM;
    }
    verba[ZEPHYRUM]   = vacuum_verbum;
    claves[ZEPHYRUM]  = clavis;
    (vacuum)tabula_dispersa_inserere(visae, clavis, NIHIL);
    numerus           = I;
    initia[ZEPHYRUM]  = ZEPHYRUM;
    per (gradus = I; gradus <= longitudo_maxima && bene; gradus++)
    {
        i32 initium_prius  = initia[gradus - I];
        i32 finis_prius    = numerus;
        i32 i;
        s32 littera;

        initia[gradus] = numerus;
        per (i = initium_prius; i < finis_prius && bene; i++)
        {
            per (littera = -(s32)(fila - I); littera <= (s32)(fila - I)
                && bene; littera++)
            {
                PiscinaNotatio nota;
                        Plexus unum;
                        Plexus verbum;

                si (littera == ZEPHYRUM)
                {
                    perge;
                }
                nota = piscina_notare(temporaria);
                (vacuum)plexus_creare(fila, &littera, I, temporaria,
                    &unum);
                (vacuum)plexus_iungere(verba[i], unum, temporaria,
                    &verbum);
                si (!_clavis(verbum, temporaria, &clavis))
                {
                    bene = FALSUM;
                    piscina_reficere(temporaria, nota);
                    frange;
                }
                si (tabula_dispersa_continet(visae, clavis))
                {
                    piscina_reficere(temporaria, nota);
                    perge;
                }
                /* classis nova: verbum et clavis servantur (vocanti
                 * solum post successum) */
                clavis = chorda_transcribere(clavis, servata);
                (vacuum)plexus_creare(fila, verbum.litterae,
                    verbum.longitudo, servata, &verbum);
                piscina_reficere(temporaria, nota);
                si (numerus >= classes_maximae)
                {
                    bene = FALSUM;
                    frange;
                }
                si (numerus == capacitas)
                {
                    i32 nova = II * capacitas;
                    i32 t;

                    verba_tmp = (Plexus*)piscina_allocare(temporaria,
                        (memoriae_index)nova * magnitudo(Plexus));
                    claves_tmp = (chorda*)piscina_allocare(temporaria,
                        (memoriae_index)nova * magnitudo(chorda));
                    per (t = ZEPHYRUM; t < numerus; t++)
                    {
                        verba_tmp[t]   = verba[t];
                        claves_tmp[t]  = claves[t];
                    }
                    verba      = verba_tmp;
                    claves     = claves_tmp;
                    capacitas  = nova;
                }
                verba[numerus]   = verbum;
                claves[numerus]  = clavis;
                (vacuum)tabula_dispersa_inserere(visae, clavis, NIHIL);
                numerus++;
            }
        }
        /* gradus novus ordine clavium */
        si (bene && numerus > initia[gradus])
        {
            i32  m = numerus - initia[gradus];
            i32  t;
         Plexus* v;
         chorda* c;

            paria = (ParClavisPlexus*)piscina_allocare(temporaria,
                (memoriae_index)m * magnitudo(ParClavisPlexus));
            v = (Plexus*)piscina_allocare(temporaria,
                (memoriae_index)m * magnitudo(Plexus));
            c = (chorda*)piscina_allocare(temporaria,
                (memoriae_index)m * magnitudo(chorda));
            per (t = ZEPHYRUM; t < m; t++)
            {
                paria[t].clavis  = claves[initia[gradus] + t];
                paria[t].index   = initia[gradus] + t;
            }
            qsort(paria, (memoriae_index)m, magnitudo(ParClavisPlexus),
                _comparare_paria);
            per (t = ZEPHYRUM; t < m; t++)
            {
                v[t] = verba[paria[t].index];
                c[t] = claves[paria[t].index];
            }
            per (t = ZEPHYRUM; t < m; t++)
            {
                verba[initia[gradus] + t]   = v[t];
                claves[initia[gradus] + t]  = c[t];
            }
        }
    }
    si (!bene)
    {
        piscina_destruere(temporaria);
        piscina_destruere(servata);
        redde FALSUM;
    }
    initia[longitudo_maxima + I]  = numerus;
    exitus->fila                  = fila;
    exitus->longitudo_maxima      = longitudo_maxima;
    exitus->numerus               = numerus;
    exitus->verba   = (Plexus*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Plexus));
    exitus->claves  = (chorda*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(chorda));
    exitus->initia  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(longitudo_maxima + II) * magnitudo(i32));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        (vacuum)plexus_creare(fila, verba[k].litterae,
            verba[k].longitudo, piscina, &exitus->verba[k]);
        exitus->claves[k] = chorda_transcribere(claves[k], piscina);
    }
    per (k = ZEPHYRUM; k <= longitudo_maxima + I; k++)
    {
        exitus->initia[k] = initia[k];
    }
    piscina_destruere(temporaria);
    piscina_destruere(servata);
    redde VERUM;
}
#undef ImaginesLiberae
#undef PLEXUS_LONGITUDO_SUMMA
#undef ParClavisPlexus
#undef _ansam_contrahere
#undef _ansam_littera
#undef _ansam_transcribere
#undef _apex_proprius
#undef _apicem_notare
#undef _burau_littera
#undef _burau_littera_integra
#undef _clavis
#undef _comparare_paria
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
#undef _nervus_instructio
#undef _nervus_littera
#undef _phi
#undef _positiva
#undef _productum
#undef _propriam_destruere
#undef _propriam_vacare
#undef _reducere_in
/* lib/tabula_dispersa.c: statica per plagulam renominata */
#define _aequalis_literis_chorda _aequalis_literis_chorda_tabula_dispersa
#define _friare_literis _friare_literis_tabula_dispersa
#define _invenire_slotum _invenire_slotum_tabula_dispersa
#define _invenire_slotum_literis _invenire_slotum_literis_tabula_dispersa
#define _proxima_potentia_2 _proxima_potentia_2_tabula_dispersa
#define _tabula_dispersa_crescere _tabula_dispersa_crescere_tabula_dispersa
#line 1 "lib/tabula_dispersa.c"
/* <portabile/> probationibus sub glibc probata 2026-08-03 */


#include <stdio.h>
#include <string.h>


/* ==================================================
 * Functiones Friationis (wrappers ad friatio.h)
 * ================================================== */

i32
tabula_friare_fnv1a (
    chorda clavis)
{
    redde friatio_fnv1a(clavis);
}

/* Friare literis C directe (pro evitare const cast) */
interior i32
_friare_literis (
    constans character* cstr)
{
    redde friatio_fnv1a_literis(cstr, (i32)strlen(cstr));
}

/* Comparare literis C cum chorda */
interior b32
_aequalis_literis_chorda (
    constans character* cstr,
                chorda  clavis)
{
    i32 mensura_cstr;
    i32 i;

    mensura_cstr = (i32)strlen(cstr);

    si (mensura_cstr != clavis.mensura)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < mensura_cstr; i++)
    {
        si (cstr[i] != (character)clavis.datum[i])
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}


/* ==================================================
 * Adiutor: Proxima Potentia Binaria
 * ================================================== */

interior i32
_proxima_potentia_2 (
    i32 n)
{
    n--;
    n |= n >> I;
    n |= n >> II;
    n |= n >> IV;
    n |= n >> VIII;
    n |= n >> XVI;
    n++;
    redde n;
}


/* ==================================================
 * Functiones Friationis (wrappers ad friatio.h)
 * ================================================== */

i32
tabula_friare_multiplicatio (
    chorda clavis)
{
    redde friatio_djb2(clavis);
}


/* ==================================================
 * Adiutor: Invenire Slotum
 * Redde positus sloti, statuit vexillum *inventum
 * ================================================== */

interior i32
_invenire_slotum (
    TabulaDispersa* tabula,
            chorda  clavis,
               i32  friatum,
               b32* inventum)
{
    i32 positus;
    i32 distantia;
    i32 positus_primus_deletum;

    *inventum               = FALSUM;
    positus                 = friatum & tabula->capacitas_mask;
    distantia               = ZEPHYRUM;
    positus_primus_deletum  = (i32)-I;

    dum (tabula->sloti[positus].status != SLOT_VACUUM)
    {
        si (tabula->sloti[positus].status == SLOT_OCCUPATUM)
        {
            si (   tabula->sloti[positus].hash_cachatum == friatum
                && tabula->comparatio(
                    tabula->sloti[positus].clavis, clavis) == ZEPHYRUM)
            {
                *inventum = VERUM;
                redde positus;
            }
            tabula->collisiones_totales++;
        }
        alioquin si (tabula->sloti[positus].status == SLOT_DELETUM)
        {
            si (positus_primus_deletum == (i32)-I)
            {
                positus_primus_deletum = positus;
            }
        }

        positus = (positus + I) & tabula->capacitas_mask;
        distantia++;

        si (distantia > tabula->distantia_maxima)
        {
            tabula->distantia_maxima = distantia;
        }

        si (distantia > tabula->capacitas)
        {
            frange;
        }
    }

    si (positus_primus_deletum != (i32)-I)
    {
        redde positus_primus_deletum;
    }

    redde positus;
}

/* Invenire slotum pro literis C (evitare const cast) */
interior i32
_invenire_slotum_literis (
        TabulaDispersa* tabula,
    constans character* cstr,
                   i32  friatum,
                   b32* inventum)
{
    i32 positus;
    i32 distantia;
    i32 positus_primus_deletum;

    *inventum               = FALSUM;
    positus                 = friatum & tabula->capacitas_mask;
    distantia               = ZEPHYRUM;
    positus_primus_deletum  = (i32)-I;

    dum (tabula->sloti[positus].status != SLOT_VACUUM)
    {
        si (tabula->sloti[positus].status == SLOT_OCCUPATUM)
        {
            si (   tabula->sloti[positus].hash_cachatum == friatum
                && _aequalis_literis_chorda(cstr, tabula->sloti[positus].clavis))
            {
                *inventum = VERUM;
                redde positus;
            }
            tabula->collisiones_totales++;
        }
        alioquin si (tabula->sloti[positus].status == SLOT_DELETUM)
        {
            si (positus_primus_deletum == (i32)-I)
            {
                positus_primus_deletum = positus;
            }
        }

        positus = (positus + I) & tabula->capacitas_mask;
        distantia++;

        si (distantia > tabula->distantia_maxima)
        {
            tabula->distantia_maxima = distantia;
        }

        si (distantia > tabula->capacitas)
        {
            frange;
        }
    }

    si (positus_primus_deletum != (i32)-I)
    {
        redde positus_primus_deletum;
    }

    redde positus;
}


/* ==================================================
 * Crescentia
 * ================================================== */

interior b32
_tabula_dispersa_crescere (
    TabulaDispersa* tabula)
{
    Slotus* sloti_veteres;
       i32  capacitas_vetus;
       i32  i;

    sloti_veteres    = tabula->sloti;
    capacitas_vetus  = tabula->capacitas;

    tabula->capacitas       = capacitas_vetus * II;
    tabula->capacitas_mask  = tabula->capacitas - I;

    tabula->sloti = (Slotus*)piscina_allocare(
        tabula->piscina,
        tabula->capacitas * magnitudo(Slotus));

    si (!tabula->sloti)
    {
        tabula->sloti           = sloti_veteres;
        tabula->capacitas       = capacitas_vetus;
        tabula->capacitas_mask  = tabula->capacitas - I;
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < tabula->capacitas; i++)
    {
        tabula->sloti[i].status = SLOT_VACUUM;
    }

    tabula->numerus              = ZEPHYRUM;
    tabula->numerus_deletorum    = ZEPHYRUM;
    tabula->collisiones_totales  = ZEPHYRUM;
    tabula->distantia_maxima     = ZEPHYRUM;

    per (i = ZEPHYRUM; i < capacitas_vetus; i++)
    {
        si (sloti_veteres[i].status == SLOT_OCCUPATUM)
        {
            tabula_dispersa_inserere(
                tabula,
                sloti_veteres[i].clavis,
                sloti_veteres[i].valor);
        }
    }

    redde VERUM;
}


/* ==================================================
 * Creatio
 * ================================================== */

TabulaDispersa*
tabula_dispersa_creare (
             Piscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio)
{
    TabulaDispersa* tabula;
               i32  capacitas;
               i32  i;

    si (!piscina || !friatio || !comparatio)
    {
        redde NIHIL;
    }

    capacitas = _proxima_potentia_2(capacitas_initialis);
    si (capacitas < XVI) capacitas = XVI;

    tabula = (TabulaDispersa*)piscina_allocare(
                                piscina,
                                magnitudo(TabulaDispersa));
    si (!tabula) redde NIHIL;

    tabula->sloti = (Slotus*)piscina_allocare(
                                piscina,
                                capacitas * magnitudo(Slotus));
    si (!tabula->sloti) redde NIHIL;

    per (i = ZEPHYRUM; i < capacitas; i++)
    {
        tabula->sloti[i].status          = SLOT_VACUUM;
        tabula->sloti[i].clavis.mensura  = ZEPHYRUM;
        tabula->sloti[i].clavis.datum    = NIHIL;
        tabula->sloti[i].valor           = NIHIL;
        tabula->sloti[i].hash_cachatum   = ZEPHYRUM;
    }

    tabula->capacitas                 = capacitas;
    tabula->capacitas_mask            = capacitas - I;
    tabula->numerus                   = ZEPHYRUM;
    tabula->numerus_deletorum         = ZEPHYRUM;
    tabula->piscina                   = piscina;
    tabula->comparatio                = comparatio;
    tabula->friatio                   = friatio;
    tabula->factor_maximus            = 0.75f;
    tabula->factor_deletorum_maximus  = 0.25f;
    tabula->collisiones_totales       = ZEPHYRUM;
    tabula->distantia_maxima          = ZEPHYRUM;

    redde tabula;
}

TabulaDispersa*
tabula_dispersa_creare_chorda (
    Piscina* piscina,
        i32  capacitas_initialis)
{
    redde tabula_dispersa_creare(
        piscina,
        capacitas_initialis,
        tabula_friare_fnv1a,
        (TabulaComparatio)chorda_comparare);
}


/* ==================================================
 * Insertio
 * ================================================== */

b32
tabula_dispersa_inserere (
    TabulaDispersa* tabula,
            chorda  clavis,
            vacuum* valor)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Confer factores oneris */
    si (   tabula_dispersa_factor_oneris(tabula) > tabula->factor_maximus
        || tabula_dispersa_factor_deletorum(tabula) > tabula->factor_deletorum_maximus)
    {
        si (!_tabula_dispersa_crescere(tabula))
        {
            redde FALSUM;
        }
    }

    friatum = tabula->friatio(clavis);
    positus = _invenire_slotum(tabula, clavis, friatum, &inventum);

    si (inventum)
    {
        /* Renovatio existentis */
        tabula->sloti[positus].valor = valor;
        redde VERUM;
    }

    /* Insertio nova */
    si (tabula->sloti[positus].status == SLOT_DELETUM)
    {
        tabula->numerus_deletorum--;
    }

    tabula->sloti[positus].clavis         = clavis;
    tabula->sloti[positus].valor          = valor;
    tabula->sloti[positus].hash_cachatum  = friatum;
    tabula->sloti[positus].status         = SLOT_OCCUPATUM;
    tabula->numerus++;

    redde VERUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

b32
tabula_dispersa_invenire (
    TabulaDispersa*  tabula,
            chorda   clavis,
            vacuum** valor_out)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    friatum = tabula->friatio(clavis);
    positus = _invenire_slotum(tabula, clavis, friatum, &inventum);

    si (inventum)
    {
        si (valor_out)
        {
            *valor_out = tabula->sloti[positus].valor;
        }
        redde VERUM;
    }

    redde FALSUM;
}

b32
tabula_dispersa_continet (
    TabulaDispersa* tabula,
            chorda  clavis)
{
    redde tabula_dispersa_invenire(tabula, clavis, NIHIL);
}

b32
tabula_dispersa_invenire_literis (
        TabulaDispersa*  tabula,
    constans character*  cstr,
                vacuum** valor_out)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || !cstr)
    {
        redde FALSUM;
    }

    /* Friare literis directe */
    friatum = _friare_literis(cstr);

    /* Invenire slotum (comparare literis directe) */
    positus = _invenire_slotum_literis(tabula, cstr, friatum, &inventum);

    si (inventum)
    {
        si (valor_out)
        {
            *valor_out = tabula->sloti[positus].valor;
        }
        redde VERUM;
    }

    redde FALSUM;
}

b32
tabula_dispersa_continet_literis (
        TabulaDispersa* tabula,
    constans character* cstr)
{
    redde tabula_dispersa_invenire_literis(tabula, cstr, NIHIL);
}


/* ==================================================
 * Deletio
 * ================================================== */

b32
tabula_dispersa_delere (
    TabulaDispersa* tabula,
            chorda  clavis)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    friatum = tabula->friatio(clavis);
    positus = _invenire_slotum(tabula, clavis, friatum, &inventum);

    si (inventum)
    {
        tabula->sloti[positus].status = SLOT_DELETUM;
        tabula->numerus--;
        tabula->numerus_deletorum++;
        redde VERUM;
    }

    redde FALSUM;
}


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

vacuum
tabula_dispersa_vacare (
    TabulaDispersa* tabula)
{
    i32 i;

    si (!tabula) redde;

    per (i = ZEPHYRUM; i < tabula->capacitas; i++)
    {
        tabula->sloti[i].status          = SLOT_VACUUM;
        tabula->sloti[i].clavis.mensura  = ZEPHYRUM;
        tabula->sloti[i].clavis.datum    = NIHIL;
        tabula->sloti[i].valor           = NIHIL;
    }

    tabula->numerus              = ZEPHYRUM;
    tabula->numerus_deletorum    = ZEPHYRUM;
    tabula->collisiones_totales  = ZEPHYRUM;
    tabula->distantia_maxima     = ZEPHYRUM;
}


/* ==================================================
 * Iterator
 * ================================================== */

TabulaIterator
tabula_dispersa_iterator_initium (
    TabulaDispersa* tabula)
{
    TabulaIterator iter;
    iter.tabula           = tabula;
    iter.positus_currens  = ZEPHYRUM;
    redde iter;
}

b32
tabula_dispersa_iterator_proximum (
    TabulaIterator*  iter,
            chorda*  clavis_out,
            vacuum** valor_out)
{
    si (!iter || !iter->tabula) redde FALSUM;

    dum (iter->positus_currens < iter->tabula->capacitas)
    {
        si (iter->tabula->sloti[iter->positus_currens].status == SLOT_OCCUPATUM)
        {
            si (clavis_out)
            {
                *clavis_out = iter->tabula->sloti[iter->positus_currens].clavis;
            }
            si (valor_out)
            {
                *valor_out  = iter->tabula->sloti[iter->positus_currens].valor;
            }
            iter->positus_currens++;
            redde VERUM;
        }
        iter->positus_currens++;
    }

    redde FALSUM;
}


/* ==================================================
 * Statisticae
 * ================================================== */

f32
tabula_dispersa_factor_oneris (
    TabulaDispersa* tabula)
{
    si (!tabula || tabula->capacitas == ZEPHYRUM)
    {
        redde 0.0f;
    }
    redde (f32)tabula->numerus / (f32)tabula->capacitas;
}

f32
tabula_dispersa_factor_deletorum (
    TabulaDispersa* tabula)
{
    si (!tabula || tabula->capacitas == ZEPHYRUM)
    {
        redde 0.0f;
    }
    redde (f32)tabula->numerus_deletorum / (f32)tabula->capacitas;
}

i32
tabula_dispersa_numerus (
    TabulaDispersa* tabula)
{
    redde tabula ? tabula->numerus : ZEPHYRUM;
}

vacuum
tabula_dispersa_status_imprimere (
    TabulaDispersa* tabula)
{
    si (!tabula)
    {
        imprimere("Tabula Dispersa: NIHIL\n");
        redde;
    }

    imprimere("Status Tabulae Dispersae:\n");
    imprimere("  Capacitas: %u\n", tabula->capacitas);
    imprimere("  Numerus: %u\n", tabula->numerus);
    imprimere("  Numerus deletorum: %u\n", tabula->numerus_deletorum);
    imprimere("  Factor oneris: %.2f%%\n", tabula_dispersa_factor_oneris(tabula) * C);
    imprimere("  Factor deletorum: %.2f%%\n",
                tabula_dispersa_factor_deletorum(tabula) * C);
    imprimere("  Collisiones totales: %u\n", tabula->collisiones_totales);
    imprimere("  Distantia maxima: %u\n", tabula->distantia_maxima);
}
#undef _aequalis_literis_chorda
#undef _friare_literis
#undef _invenire_slotum
#undef _invenire_slotum_literis
#undef _proxima_potentia_2
#undef _tabula_dispersa_crescere
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
/* lib/friatio.c: statica per plagulam renominata */
#define CRC32_TABULA CRC32_TABULA_friatio
#define SHA256_K SHA256_K_friatio
#define _sha1_transformare _sha1_transformare_friatio
#define _sha256_transformare _sha256_transformare_friatio
#line 1 "lib/friatio.c"

#include <string.h>


/* ==================================================
 * FNV-1a (Fowler-Noll-Vo)
 * ================================================== */

/* FNV-1a constants (32-bit) */
#define FNV_OFFSET_BASIS 2166136261U
#define FNV_PRIME        16777619U

i32
friatio_fnv1a (
    chorda clavis)
{
    i32 friatum;
    i32 i;

    friatum = (i32)FNV_OFFSET_BASIS;

    per (i = ZEPHYRUM; i < clavis.mensura; i++)
    {
        friatum ^= (i8)clavis.datum[i];
        friatum *= (i32)FNV_PRIME;
    }

    redde friatum;
}

i32
friatio_fnv1a_literis (
    constans character* literae,
                   i32  mensura)
{
    i32 friatum;
    i32 i;

    friatum = (i32)FNV_OFFSET_BASIS;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        friatum ^= (i8)literae[i];
        friatum *= (i32)FNV_PRIME;
    }

    redde friatum;
}


/* ==================================================
 * DJB2 (Dan Bernstein)
 * ================================================== */

#define DJB2_INIT 5381U

i32
friatio_djb2 (
    chorda clavis)
{
    i32 friatum;
    i32 i;

    friatum = (i32)DJB2_INIT;

    per (i = ZEPHYRUM; i < clavis.mensura; i++)
    {
        /* friatum = friatum * 33 + c */
        friatum = ((friatum << V) + friatum) + (i8)clavis.datum[i];
    }

    redde friatum;
}

i32
friatio_djb2_literis (
    constans character* literae,
                   i32  mensura)
{
    i32 friatum;
    i32 i;

    friatum = (i32)DJB2_INIT;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        friatum = ((friatum << V) + friatum) + (i8)literae[i];
    }

    redde friatum;
}


/* ==================================================
 * SHA-1 Implementation
 *
 * Based on RFC 3174
 * Produces 160-bit (20-byte) message digest
 * ================================================== */

/* Left rotate 32-bit value */
#define SHA1_ROL(value, bits) (((value) << (bits)) | ((value) >> (XXXII - (bits))))

/* SHA-1 round functions */
#define SHA1_F0(b, c, d) (((b) & (c)) | ((~(b)) & (d)))
#define SHA1_F1(b, c, d) ((b) ^ (c) ^ (d))
#define SHA1_F2(b, c, d) (((b) & (c)) | ((b) & (d)) | ((c) & (d)))
#define SHA1_F3(b, c, d) ((b) ^ (c) ^ (d))

/* SHA-1 constants */
#define SHA1_K0 0x5A827999UL
#define SHA1_K1 0x6ED9EBA1UL
#define SHA1_K2 0x8F1BBCDCUL
#define SHA1_K3 0xCA62C1D6UL

/* SHA-1 initial hash values */
#define SHA1_H0 0x67452301UL
#define SHA1_H1 0xEFCDAB89UL
#define SHA1_H2 0x98BADCFEUL
#define SHA1_H3 0x10325476UL
#define SHA1_H4 0xC3D2E1F0UL

/* Internal: Process a single 64-byte block */
interior vacuum
_sha1_transformare (
            i32 status[V],
    constans i8 buffer[LXIV])
{
    i32 a, b, c, d, e;
    i32 w[LXXX];
    i32 i;

    /* Copy buffer into 16 32-bit words (big-endian) */
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        w[i] = ((i32)((i8)buffer[i * IV] & 0xFF) << XXIV)
            | ((i32)((i8)buffer[i * IV + I] & 0xFF) << XVI)
            | ((i32)((i8)buffer[i * IV + II] & 0xFF) << VIII)
            | ((i32)((i8)buffer[i * IV + III] & 0xFF));
    }

    /* Extend the sixteen 32-bit words into eighty 32-bit words */
    per (i = XVI; i < LXXX; i++)
    {
        w[i] = SHA1_ROL(w[i - III] ^ w[i - VIII] ^ w[i - XIV] ^ w[i - XVI], I);
    }

    /* Initialize working variables */
    a = status[ZEPHYRUM];
    b = status[I];
    c = status[II];
    d = status[III];
    e = status[IV];

    /* Main loop - 80 rounds */
    per (i = ZEPHYRUM; i < XX; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F0(b, c, d) + e + w[i] + (i32)SHA1_K0;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    per (i = XX; i < XL; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F1(b, c, d) + e + w[i] + (i32)SHA1_K1;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    per (i = XL; i < LX; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F2(b, c, d) + e + w[i] + (i32)SHA1_K2;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    per (i = LX; i < LXXX; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F3(b, c, d) + e + w[i] + (i32)SHA1_K3;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    /* Add to state */
    status[ZEPHYRUM]  += a;
    status[I]         += b;
    status[II]        += c;
    status[III]       += d;
    status[IV]        += e;
}

vacuum
sha1_initiare (
    SHA1Contextus* ctx)
{
    ctx->status[ZEPHYRUM]   = (i32)SHA1_H0;
    ctx->status[I]          = (i32)SHA1_H1;
    ctx->status[II]         = (i32)SHA1_H2;
    ctx->status[III]        = (i32)SHA1_H3;
    ctx->status[IV]         = (i32)SHA1_H4;
    ctx->numerus[ZEPHYRUM]  = ZEPHYRUM;
    ctx->numerus[I]         = ZEPHYRUM;
}

vacuum
sha1_addere (
    SHA1Contextus* ctx,
      constans i8* data,
              i32  mensura)
{
    i32 i;
    i32 j;

    j = (ctx->numerus[ZEPHYRUM] >> III) & 0x3F;

    /* Update bit count */
    ctx->numerus[ZEPHYRUM] += (mensura << III);
    si (ctx->numerus[ZEPHYRUM] < (mensura << III))
    {
        ctx->numerus[I]++;
    }
    ctx->numerus[I] += (mensura >> XXIX);

    /* Process data */
    si ((j + mensura) > LXIII)
    {
        i = LXIV - j;
        memcpy(&ctx->buffer[j], data, (memoriae_index)i);
        _sha1_transformare(ctx->status, ctx->buffer);

        per (; i + LXIII < mensura; i += LXIV)
        {
            _sha1_transformare(ctx->status, &data[i]);
        }
        j = ZEPHYRUM;
    }
    alioquin
    {
        i = ZEPHYRUM;
    }

    memcpy(&ctx->buffer[j], &data[i], (memoriae_index)(mensura - i));
}

vacuum
sha1_finire (
    SHA1Contextus* ctx,
               i8  digest[SHA1_DIGEST_MENSURA])
{
     i8 numerus_finalis[VIII];
     i8 c;
    i32 i;

    /* Store bit count (big-endian) */
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        numerus_finalis[i] = (i8)((ctx->numerus[(i >= IV ? ZEPHYRUM : I)]
            >> ((III - (i & III)) * VIII)) & 0xFF);
    }

    /* Pad to 56 mod 64 */
    c = (i8)0x80;
    sha1_addere(ctx, &c, I);
    dum ((ctx->numerus[ZEPHYRUM] & 0x1F8) != 0x1C0)
    {
        c = (i8)0x00;
        sha1_addere(ctx, &c, I);
    }

    /* Append length */
    sha1_addere(ctx, numerus_finalis, VIII);

    /* Output hash (big-endian) */
    per (i = ZEPHYRUM; i < SHA1_DIGEST_MENSURA; i++)
    {
        digest[i] = (i8)((ctx->status[i >> II] >> ((III - (i & III)) * VIII)) & 0xFF);
    }
}

vacuum
sha1_friare (
    constans i8* data,
            i32  mensura,
             i8  digest[SHA1_DIGEST_MENSURA])
{
    SHA1Contextus ctx;

    sha1_initiare(&ctx);
    sha1_addere(&ctx, data, mensura);
    sha1_finire(&ctx, digest);
}

vacuum
sha1_friare_chorda (
    chorda clavis,
        i8 digest[SHA1_DIGEST_MENSURA])
{
    sha1_friare((constans i8*)clavis.datum, clavis.mensura, digest);
}


/* ==================================================
 * SHA-256 Implementation
 *
 * Based on FIPS 180-4
 * Produces 256-bit (32-byte) message digest
 * ================================================== */

/* Right rotate 32-bit value */
#define SHA256_ROR(value, bits) (((value) >> (bits)) | ((value) << (XXXII - (bits))))

/* SHA-256 logical functions */
#define SHA256_CH(x, y, z)  (((x) & (y)) ^ ((~(x)) & (z)))
#define SHA256_MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SHA256_EP0(x)     (SHA256_ROR(x, II) ^ SHA256_ROR(x, XIII) ^ SHA256_ROR(x, XXII))
#define SHA256_EP1(x)     (SHA256_ROR(x, VI) ^ SHA256_ROR(x, XI) ^ SHA256_ROR(x, XXV))
#define SHA256_SIG0(x)    (SHA256_ROR(x, VII) ^ SHA256_ROR(x, XVIII) ^ ((x) >> III))
#define SHA256_SIG1(x)    (SHA256_ROR(x, XVII) ^ SHA256_ROR(x, XIX) ^ ((x) >> X))

/* SHA-256 initial hash values (first 32 bits of fractional parts of square roots of first 8 primes) */
#define SHA256_H0 0x6A09E667UL
#define SHA256_H1 0xBB67AE85UL
#define SHA256_H2 0x3C6EF372UL
#define SHA256_H3 0xA54FF53AUL
#define SHA256_H4 0x510E527FUL
#define SHA256_H5 0x9B05688CUL
#define SHA256_H6 0x1F83D9ABUL
#define SHA256_H7 0x5BE0CD19UL

/* SHA-256 round constants (first 32 bits of fractional parts of cube roots of first 64 primes) */
hic_manens constans i32 SHA256_K[LXIV] = {
    (i32)0x428A2F98UL, (i32)0x71374491UL, (i32)0xB5C0FBCFUL, (i32)0xE9B5DBA5UL,
    (i32)0x3956C25BUL, (i32)0x59F111F1UL, (i32)0x923F82A4UL, (i32)0xAB1C5ED5UL,
    (i32)0xD807AA98UL, (i32)0x12835B01UL, (i32)0x243185BEUL, (i32)0x550C7DC3UL,
    (i32)0x72BE5D74UL, (i32)0x80DEB1FEUL, (i32)0x9BDC06A7UL, (i32)0xC19BF174UL,
    (i32)0xE49B69C1UL, (i32)0xEFBE4786UL, (i32)0x0FC19DC6UL, (i32)0x240CA1CCUL,
    (i32)0x2DE92C6FUL, (i32)0x4A7484AAUL, (i32)0x5CB0A9DCUL, (i32)0x76F988DAUL,
    (i32)0x983E5152UL, (i32)0xA831C66DUL, (i32)0xB00327C8UL, (i32)0xBF597FC7UL,
    (i32)0xC6E00BF3UL, (i32)0xD5A79147UL, (i32)0x06CA6351UL, (i32)0x14292967UL,
    (i32)0x27B70A85UL, (i32)0x2E1B2138UL, (i32)0x4D2C6DFCUL, (i32)0x53380D13UL,
    (i32)0x650A7354UL, (i32)0x766A0ABBUL, (i32)0x81C2C92EUL, (i32)0x92722C85UL,
    (i32)0xA2BFE8A1UL, (i32)0xA81A664BUL, (i32)0xC24B8B70UL, (i32)0xC76C51A3UL,
    (i32)0xD192E819UL, (i32)0xD6990624UL, (i32)0xF40E3585UL, (i32)0x106AA070UL,
    (i32)0x19A4C116UL, (i32)0x1E376C08UL, (i32)0x2748774CUL, (i32)0x34B0BCB5UL,
    (i32)0x391C0CB3UL, (i32)0x4ED8AA4AUL, (i32)0x5B9CCA4FUL, (i32)0x682E6FF3UL,
    (i32)0x748F82EEUL, (i32)0x78A5636FUL, (i32)0x84C87814UL, (i32)0x8CC70208UL,
    (i32)0x90BEFFFAUL, (i32)0xA4506CEBUL, (i32)0xBEF9A3F7UL, (i32)0xC67178F2UL
};

/* Internal: Process a single 64-byte block */
interior vacuum
_sha256_transformare (
            i32 status[VIII],
    constans i8 buffer[LXIV])
{
    i32 a, b, c, d, e, f, g, h;
    i32 w[LXIV];
    i32 i;
    i32 t1, t2;

    /* Copy buffer into 16 32-bit words (big-endian) */
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        w[i] = ((i32)((i8)buffer[i * IV] & 0xFF) << XXIV)
            | ((i32)((i8)buffer[i * IV + I] & 0xFF) << XVI)
            | ((i32)((i8)buffer[i * IV + II] & 0xFF) << VIII)
            | ((i32)((i8)buffer[i * IV + III] & 0xFF));
    }

    /* Extend the sixteen 32-bit words into sixty-four 32-bit words */
    per (i = XVI; i < LXIV; i++)
    {
        w[i] = SHA256_SIG1(w[i - II]) + w[i - VII] + SHA256_SIG0(w[i - XV]) + w[i - XVI];
    }

    /* Initialize working variables */
    a = status[ZEPHYRUM];
    b = status[I];
    c = status[II];
    d = status[III];
    e = status[IV];
    f = status[V];
    g = status[VI];
    h = status[VII];

    /* Main loop - 64 rounds */
    per (i = ZEPHYRUM; i < LXIV; i++)
    {
        t1 = h + SHA256_EP1(e) + SHA256_CH(e, f, g) + SHA256_K[i] + w[i];
        t2 = SHA256_EP0(a) + SHA256_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    /* Add to state */
    status[ZEPHYRUM]  += a;
    status[I]         += b;
    status[II]        += c;
    status[III]       += d;
    status[IV]        += e;
    status[V]         += f;
    status[VI]        += g;
    status[VII]       += h;
}

vacuum
sha256_initiare (
    SHA256Contextus* ctx)
{
    ctx->status[ZEPHYRUM]   = (i32)SHA256_H0;
    ctx->status[I]          = (i32)SHA256_H1;
    ctx->status[II]         = (i32)SHA256_H2;
    ctx->status[III]        = (i32)SHA256_H3;
    ctx->status[IV]         = (i32)SHA256_H4;
    ctx->status[V]          = (i32)SHA256_H5;
    ctx->status[VI]         = (i32)SHA256_H6;
    ctx->status[VII]        = (i32)SHA256_H7;
    ctx->numerus[ZEPHYRUM]  = ZEPHYRUM;
    ctx->numerus[I]         = ZEPHYRUM;
}

vacuum
sha256_addere (
    SHA256Contextus* ctx,
        constans i8* data,
                i32  mensura)
{
    i32 i;
    i32 j;

    j = (ctx->numerus[ZEPHYRUM] >> III) & 0x3F;

    /* Update bit count */
    ctx->numerus[ZEPHYRUM] += (mensura << III);
    si (ctx->numerus[ZEPHYRUM] < (mensura << III))
    {
        ctx->numerus[I]++;
    }
    ctx->numerus[I] += (mensura >> XXIX);

    /* Process data */
    si ((j + mensura) > LXIII)
    {
        i = LXIV - j;
        memcpy(&ctx->buffer[j], data, (memoriae_index)i);
        _sha256_transformare(ctx->status, ctx->buffer);

        per (; i + LXIII < mensura; i += LXIV)
        {
            _sha256_transformare(ctx->status, &data[i]);
        }
        j = ZEPHYRUM;
    }
    alioquin
    {
        i = ZEPHYRUM;
    }

    memcpy(&ctx->buffer[j], &data[i], (memoriae_index)(mensura - i));
}

vacuum
sha256_finire (
    SHA256Contextus* ctx,
                 i8  digest[SHA256_DIGEST_MENSURA])
{
     i8 numerus_finalis[VIII];
     i8 c;
    i32 i;

    /* Store bit count (big-endian) */
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        numerus_finalis[i] = (i8)((ctx->numerus[(i >= IV ? ZEPHYRUM : I)]
            >> ((III - (i & III)) * VIII)) & 0xFF);
    }

    /* Pad to 56 mod 64 */
    c = (i8)0x80;
    sha256_addere(ctx, &c, I);
    dum ((ctx->numerus[ZEPHYRUM] & 0x1F8) != 0x1C0)
    {
        c = (i8)0x00;
        sha256_addere(ctx, &c, I);
    }

    /* Append length */
    sha256_addere(ctx, numerus_finalis, VIII);

    /* Output hash (big-endian) */
    per (i = ZEPHYRUM; i < SHA256_DIGEST_MENSURA; i++)
    {
        digest[i] = (i8)((ctx->status[i >> II] >> ((III - (i & III)) * VIII)) & 0xFF);
    }
}

vacuum
sha256_friare (
    constans i8* data,
            i32  mensura,
             i8  digest[SHA256_DIGEST_MENSURA])
{
    SHA256Contextus ctx;

    sha256_initiare(&ctx);
    sha256_addere(&ctx, data, mensura);
    sha256_finire(&ctx, digest);
}

vacuum
sha256_friare_chorda (
    chorda clavis,
        i8 digest[SHA256_DIGEST_MENSURA])
{
    sha256_friare((constans i8*)clavis.datum, clavis.mensura, digest);
}


/* ==================================================
 * CRC32 Implementation
 *
 * IEEE 802.3 polynomial: 0xEDB88320 (reflected)
 * Used in: Ethernet, ZIP, PNG, etc.
 * ================================================== */

/* CRC32 lookup table (pre-computed for IEEE polynomial) */
hic_manens constans i32 CRC32_TABULA[CCLVI] = {
    (i32)0x00000000UL, (i32)0x77073096UL, (i32)0xEE0E612CUL, (i32)0x990951BAUL,
    (i32)0x076DC419UL, (i32)0x706AF48FUL, (i32)0xE963A535UL, (i32)0x9E6495A3UL,
    (i32)0x0EDB8832UL, (i32)0x79DCB8A4UL, (i32)0xE0D5E91EUL, (i32)0x97D2D988UL,
    (i32)0x09B64C2BUL, (i32)0x7EB17CBDUL, (i32)0xE7B82D07UL, (i32)0x90BF1D91UL,
    (i32)0x1DB71064UL, (i32)0x6AB020F2UL, (i32)0xF3B97148UL, (i32)0x84BE41DEUL,
    (i32)0x1ADAD47DUL, (i32)0x6DDDE4EBUL, (i32)0xF4D4B551UL, (i32)0x83D385C7UL,
    (i32)0x136C9856UL, (i32)0x646BA8C0UL, (i32)0xFD62F97AUL, (i32)0x8A65C9ECUL,
    (i32)0x14015C4FUL, (i32)0x63066CD9UL, (i32)0xFA0F3D63UL, (i32)0x8D080DF5UL,
    (i32)0x3B6E20C8UL, (i32)0x4C69105EUL, (i32)0xD56041E4UL, (i32)0xA2677172UL,
    (i32)0x3C03E4D1UL, (i32)0x4B04D447UL, (i32)0xD20D85FDUL, (i32)0xA50AB56BUL,
    (i32)0x35B5A8FAUL, (i32)0x42B2986CUL, (i32)0xDBBBC9D6UL, (i32)0xACBCF940UL,
    (i32)0x32D86CE3UL, (i32)0x45DF5C75UL, (i32)0xDCD60DCFUL, (i32)0xABD13D59UL,
    (i32)0x26D930ACUL, (i32)0x51DE003AUL, (i32)0xC8D75180UL, (i32)0xBFD06116UL,
    (i32)0x21B4F4B5UL, (i32)0x56B3C423UL, (i32)0xCFBA9599UL, (i32)0xB8BDA50FUL,
    (i32)0x2802B89EUL, (i32)0x5F058808UL, (i32)0xC60CD9B2UL, (i32)0xB10BE924UL,
    (i32)0x2F6F7C87UL, (i32)0x58684C11UL, (i32)0xC1611DABUL, (i32)0xB6662D3DUL,
    (i32)0x76DC4190UL, (i32)0x01DB7106UL, (i32)0x98D220BCUL, (i32)0xEFD5102AUL,
    (i32)0x71B18589UL, (i32)0x06B6B51FUL, (i32)0x9FBFE4A5UL, (i32)0xE8B8D433UL,
    (i32)0x7807C9A2UL, (i32)0x0F00F934UL, (i32)0x9609A88EUL, (i32)0xE10E9818UL,
    (i32)0x7F6A0DBBUL, (i32)0x086D3D2DUL, (i32)0x91646C97UL, (i32)0xE6635C01UL,
    (i32)0x6B6B51F4UL, (i32)0x1C6C6162UL, (i32)0x856530D8UL, (i32)0xF262004EUL,
    (i32)0x6C0695EDUL, (i32)0x1B01A57BUL, (i32)0x8208F4C1UL, (i32)0xF50FC457UL,
    (i32)0x65B0D9C6UL, (i32)0x12B7E950UL, (i32)0x8BBEB8EAUL, (i32)0xFCB9887CUL,
    (i32)0x62DD1DDFUL, (i32)0x15DA2D49UL, (i32)0x8CD37CF3UL, (i32)0xFBD44C65UL,
    (i32)0x4DB26158UL, (i32)0x3AB551CEUL, (i32)0xA3BC0074UL, (i32)0xD4BB30E2UL,
    (i32)0x4ADFA541UL, (i32)0x3DD895D7UL, (i32)0xA4D1C46DUL, (i32)0xD3D6F4FBUL,
    (i32)0x4369E96AUL, (i32)0x346ED9FCUL, (i32)0xAD678846UL, (i32)0xDA60B8D0UL,
    (i32)0x44042D73UL, (i32)0x33031DE5UL, (i32)0xAA0A4C5FUL, (i32)0xDD0D7CC9UL,
    (i32)0x5005713CUL, (i32)0x270241AAUL, (i32)0xBE0B1010UL, (i32)0xC90C2086UL,
    (i32)0x5768B525UL, (i32)0x206F85B3UL, (i32)0xB966D409UL, (i32)0xCE61E49FUL,
    (i32)0x5EDEF90EUL, (i32)0x29D9C998UL, (i32)0xB0D09822UL, (i32)0xC7D7A8B4UL,
    (i32)0x59B33D17UL, (i32)0x2EB40D81UL, (i32)0xB7BD5C3BUL, (i32)0xC0BA6CADUL,
    (i32)0xEDB88320UL, (i32)0x9ABFB3B6UL, (i32)0x03B6E20CUL, (i32)0x74B1D29AUL,
    (i32)0xEAD54739UL, (i32)0x9DD277AFUL, (i32)0x04DB2615UL, (i32)0x73DC1683UL,
    (i32)0xE3630B12UL, (i32)0x94643B84UL, (i32)0x0D6D6A3EUL, (i32)0x7A6A5AA8UL,
    (i32)0xE40ECF0BUL, (i32)0x9309FF9DUL, (i32)0x0A00AE27UL, (i32)0x7D079EB1UL,
    (i32)0xF00F9344UL, (i32)0x8708A3D2UL, (i32)0x1E01F268UL, (i32)0x6906C2FEUL,
    (i32)0xF762575DUL, (i32)0x806567CBUL, (i32)0x196C3671UL, (i32)0x6E6B06E7UL,
    (i32)0xFED41B76UL, (i32)0x89D32BE0UL, (i32)0x10DA7A5AUL, (i32)0x67DD4ACCUL,
    (i32)0xF9B9DF6FUL, (i32)0x8EBEEFF9UL, (i32)0x17B7BE43UL, (i32)0x60B08ED5UL,
    (i32)0xD6D6A3E8UL, (i32)0xA1D1937EUL, (i32)0x38D8C2C4UL, (i32)0x4FDFF252UL,
    (i32)0xD1BB67F1UL, (i32)0xA6BC5767UL, (i32)0x3FB506DDUL, (i32)0x48B2364BUL,
    (i32)0xD80D2BDAUL, (i32)0xAF0A1B4CUL, (i32)0x36034AF6UL, (i32)0x41047A60UL,
    (i32)0xDF60EFC3UL, (i32)0xA867DF55UL, (i32)0x316E8EEFUL, (i32)0x4669BE79UL,
    (i32)0xCB61B38CUL, (i32)0xBC66831AUL, (i32)0x256FD2A0UL, (i32)0x5268E236UL,
    (i32)0xCC0C7795UL, (i32)0xBB0B4703UL, (i32)0x220216B9UL, (i32)0x5505262FUL,
    (i32)0xC5BA3BBEUL, (i32)0xB2BD0B28UL, (i32)0x2BB45A92UL, (i32)0x5CB36A04UL,
    (i32)0xC2D7FFA7UL, (i32)0xB5D0CF31UL, (i32)0x2CD99E8BUL, (i32)0x5BDEAE1DUL,
    (i32)0x9B64C2B0UL, (i32)0xEC63F226UL, (i32)0x756AA39CUL, (i32)0x026D930AUL,
    (i32)0x9C0906A9UL, (i32)0xEB0E363FUL, (i32)0x72076785UL, (i32)0x05005713UL,
    (i32)0x95BF4A82UL, (i32)0xE2B87A14UL, (i32)0x7BB12BAEUL, (i32)0x0CB61B38UL,
    (i32)0x92D28E9BUL, (i32)0xE5D5BE0DUL, (i32)0x7CDCEFB7UL, (i32)0x0BDBDF21UL,
    (i32)0x86D3D2D4UL, (i32)0xF1D4E242UL, (i32)0x68DDB3F8UL, (i32)0x1FDA836EUL,
    (i32)0x81BE16CDUL, (i32)0xF6B9265BUL, (i32)0x6FB077E1UL, (i32)0x18B74777UL,
    (i32)0x88085AE6UL, (i32)0xFF0F6A70UL, (i32)0x66063BCAUL, (i32)0x11010B5CUL,
    (i32)0x8F659EFFUL, (i32)0xF862AE69UL, (i32)0x616BFFD3UL, (i32)0x166CCF45UL,
    (i32)0xA00AE278UL, (i32)0xD70DD2EEUL, (i32)0x4E048354UL, (i32)0x3903B3C2UL,
    (i32)0xA7672661UL, (i32)0xD06016F7UL, (i32)0x4969474DUL, (i32)0x3E6E77DBUL,
    (i32)0xAED16A4AUL, (i32)0xD9D65ADCUL, (i32)0x40DF0B66UL, (i32)0x37D83BF0UL,
    (i32)0xA9BCAE53UL, (i32)0xDEBB9EC5UL, (i32)0x47B2CF7FUL, (i32)0x30B5FFE9UL,
    (i32)0xBDBDF21CUL, (i32)0xCABAC28AUL, (i32)0x53B39330UL, (i32)0x24B4A3A6UL,
    (i32)0xBAD03605UL, (i32)0xCDD70693UL, (i32)0x54DE5729UL, (i32)0x23D967BFUL,
    (i32)0xB3667A2EUL, (i32)0xC4614AB8UL, (i32)0x5D681B02UL, (i32)0x2A6F2B94UL,
    (i32)0xB40BBE37UL, (i32)0xC30C8EA1UL, (i32)0x5A05DF1BUL, (i32)0x2D02EF8DUL
};

i32
crc32_initiare (vacuum)
{
    redde (i32)0xFFFFFFFFUL;
}

i32
crc32_addere (
            i32  crc_currens,
    constans i8* data,
            i32  mensura)
{
    i32 i;
    i32 crc;

    crc = crc_currens;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        crc = CRC32_TABULA[((crc ^ (i8)data[i]) & 0xFF)] ^ (crc >> VIII);
    }

    redde crc;
}

i32
crc32_finire (
    i32 crc_currens)
{
    redde crc_currens ^ (i32)0xFFFFFFFFUL;
}

i32
crc32_calculare (
    constans i8* data,
            i32  mensura)
{
    i32 crc;

    crc = crc32_initiare();
    crc = crc32_addere(crc, data, mensura);
    crc = crc32_finire(crc);

    redde crc;
}

i32
crc32_calculare_chorda (
    chorda clavis)
{
    redde crc32_calculare((constans i8*)clavis.datum, clavis.mensura);
}
#undef CRC32_TABULA
#undef DJB2_INIT
#undef FNV_OFFSET_BASIS
#undef FNV_PRIME
#undef SHA1_F0
#undef SHA1_F1
#undef SHA1_F2
#undef SHA1_F3
#undef SHA1_H0
#undef SHA1_H1
#undef SHA1_H2
#undef SHA1_H3
#undef SHA1_H4
#undef SHA1_K0
#undef SHA1_K1
#undef SHA1_K2
#undef SHA1_K3
#undef SHA1_ROL
#undef SHA256_CH
#undef SHA256_EP0
#undef SHA256_EP1
#undef SHA256_H0
#undef SHA256_H1
#undef SHA256_H2
#undef SHA256_H3
#undef SHA256_H4
#undef SHA256_H5
#undef SHA256_H6
#undef SHA256_H7
#undef SHA256_K
#undef SHA256_MAJ
#undef SHA256_ROR
#undef SHA256_SIG0
#undef SHA256_SIG1
#undef _sha1_transformare
#undef _sha256_transformare
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
#line 1 "knotapel/demo_129_braid_regions/main.c"
/*
 * KNOTAPEL DEMO 129: How Many Linear Regions Does a Braid Network Carve? (N6)
 * ===========================================================================
 *
 * A braid word w on n strands is an exact, bias-free ReLU network on the
 * 2n - 4 Dynnikov coordinates (D128, KC 55; plexus_nervus). Its iterate
 * w^m is m copies composed. How does the number of its LINEAR REGIONS
 * grow with m, and does the braid's dynamics (periodic P, positive
 * entropy E, zero entropy Z) decide it? Plan: plan.md (approved
 * 2026-10-16, amended after the T5.0 review: regions belong to the WORD,
 * pieces to the braid).
 *
 *   Part A  (T5a) B_3, EXACT: every class up to length 8 (2,589), every
 *           geodesic word of each class, m = 1..12: activation regions
 *           (sectors, nervus_regiones_plani) and linear pieces of w^m.
 *           The registered rule H_lin vs H_exp.
 *   Part B  (T5a) B_3, the lattice lower bound (interior points of the
 *           shells max |x_i| = R, itineraries by refinement) against
 *           Part A's exact count.
 *   Part C  (T5b) B_4 lattice bounds; the period-2 side question.
 *   Part D  (T5b) nulls.
 *
 * Classes on B_3 are EXACT: P iff beta^k is a power of Delta^2
 * (plexus_periodus); otherwise the type is read from the trace of the
 * reduced Burau matrix at t = -1, the image of beta in SL(2, Z): E iff
 * |tr| > 2, with dilatation lambda = (|tr| + sqrt(tr^2 - 4)) / 2, Z iff
 * |tr| = 2 (reducible); |tr| < 2 is always periodic (B_3 / Z(B_3) =
 * PSL(2, Z); Murasugi 1974; cited, and checked against D128's table).
 *
 * House libraries: plexus, nervus (and their closure), hence latina.h:
 * Roman numerals and Latin keywords are macros here (never name a
 * variable I, V, X, L, C, D or M).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_129_braid_regions/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Quick mode for plants: D129_CELER=1 (B_3 up to length 5, shells 4, 16).
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

/* a word as text into buf (letters separated by spaces) */
static void
word_text (
    Plexus p,
    char  *buf,
    int    size)
{
    int k;
    int used = 0;

    buf[0] = '\0';
    for (k = 0; k < (int)p.longitudo && used < size - 8; k++) {
        used += sprintf(buf + used, k ? " %d" : "%d", (int)p.litterae[k]);
    }
    if (p.longitudo == 0) {
        sprintf(buf, "(empty)");
    }
}

/* one decimal of num / den, integers only */
static void
mean_text (
    long  num,
    long  den,
    char *buf)
{
    long t;

    if (den == 0) {
        sprintf(buf, "-");
        return;
    }
    t = (10 * num + den / 2) / den;
    sprintf(buf, "%ld.%ld", t / 10, t % 10);
}

static long
pow3 (
    int e)
{
    long r = 1;

    while (e-- > 0) r *= 3;
    return r;
}

/* length after free CYCLIC reduction (strip first/last when inverse) */
static int
cyclic_length (
    Plexus p)
{
    int lo = 0;
    int hi = (int)p.longitudo - 1;

    while (hi - lo >= 1 && p.litterae[lo] == -p.litterae[hi]) {
        lo++;
        hi--;
    }
    return hi - lo + 1;
}

static int
words_equal (
    Plexus a,
    Plexus b)
{
    int k;

    if (a.longitudo != b.longitudo) return 0;
    for (k = 0; k < (int)a.longitudo; k++) {
        if (a.litterae[k] != b.litterae[k]) return 0;
    }
    return 1;
}

/* ================================================================
 * The census, its classes and its geodesic words
 * ================================================================ */

#define MMAX 12
#define CLS_P 0
#define CLS_E 1
#define CLS_Z 2

static const char *cls_name[3] = { "P", "E", "Z" };

typedef struct {
    Plexus word;              /* census representative */
    int    len;
    int    cls;
    int    period;
    long   tr;                /* trace of Burau(-1) */
    int    ngeo;              /* geodesic words of the class */
    int    geo0;              /* first in geo[] */
    int    rep;               /* the representative's index in geo[] */
    int    pieces[MMAX + 1];
    int    rmin[MMAX + 1];
    int    rmax[MMAX + 1];
    int    rrep[MMAX + 1];
    int    rbest[MMAX + 1];   /* the word with the most regions at MMAX */
    int    best;              /* its index in geo[] */
    int    null_sectors;      /* sectors with an identically-0 argument */
} Klass;

static int     celer = 0;
static int     maxlen = 8;
static Klass  *ks;
static int     nk;
static Plexus *geo;           /* geodesic words, grouped by class */
static int     ngeo;
static int    *geo_class;

/* per-length pins (D128 findings, B_3 up to 8) */
static const int pin_P[9] = { 1, 0, 4, 10, 20, 16, 34, 32, 92 };
static const int pin_E[9] = { 0, 0, 4, 12, 32, 92, 248, 544, 1176 };
static const int pin_Z[9] = { 0, 4, 4, 8, 16, 40, 32, 80, 88 };

static long
burau_trace (
    Plexus p)
{
    Matrix m;
    s64    a = 0;
    s64    d = 0;

    if (!plexus_burau_minus_uno(p, pool, &m)
            || !magnus_ad_s64(*(const Magnus *)matrix_elementum(m, 0, 0),
                &a)
            || !magnus_ad_s64(*(const Magnus *)matrix_elementum(m, 1, 1),
                &d)) {
        fatal("Burau(-1) refused");
    }
    return (long)(a + d);
}

static void
build_census (void)
{
    PlexusCensus cs;
    int          i;
    int          ok_cls = 1;
    int          ok_trace = 1;
    int          ok_radix = 1;
    int          len;

    if (!plexus_enumerare(3, (i32)maxlen, 100000, pool, &cs)) {
        fatal("census refused");
    }
    nk = (int)cs.numerus;
    ks = (Klass *)calloc((size_t)nk, sizeof(Klass));
    for (i = 0; i < nk; i++) {
        i32          period = 0;
        RadixRealis  rho;
        s32          sg = 0;
        long         atr;

        ks[i].word = cs.verba[i];
        ks[i].len = (int)cs.verba[i].longitudo;
        if (!plexus_periodus(ks[i].word, pool, &period)) {
            fatal("period refused");
        }
        ks[i].period = (int)period;
        ks[i].tr = burau_trace(ks[i].word);
        atr = ks[i].tr < 0 ? -ks[i].tr : ks[i].tr;
        if (period > 0) {
            ks[i].cls = CLS_P;
            if (atr > 2) ok_trace = 0;       /* periodic: |tr| <= 2 */
        } else if (atr > 2) {
            ks[i].cls = CLS_E;
        } else {
            ks[i].cls = CLS_Z;
            if (atr != 2) ok_trace = 0;      /* |tr| < 2 is periodic */
        }
        /* Burau(-1) spectral radius > 1 iff |tr| > 2 (2 x 2, det 1) */
        if (!plexus_burau_radix_maxima(ks[i].word, pool, &rho)
                || !radix_compara(rho, radix_ex_fractione(
                    fractio_ex_s64(1), pool), pool, &sg)) {
            fatal("radix refused");
        }
        if ((sg > 0) != (atr > 2)) ok_radix = 0;
    }
    for (len = 0; len <= maxlen; len++) {
        int p = 0, e = 0, z = 0;

        for (i = 0; i < nk; i++) {
            if (ks[i].len != len) continue;
            if (ks[i].cls == CLS_P) p++;
            else if (ks[i].cls == CLS_E) e++;
            else z++;
        }
        if (p != pin_P[len] || e != pin_E[len] || z != pin_Z[len]) {
            ok_cls = 0;
        }
    }
    check("classes per length = D128's table (P exact, E certified)",
        ok_cls);
    check("trace law: periodic => |tr| <= 2, not periodic and |tr| <= 2 "
        "=> |tr| = 2", ok_trace);
    check("Burau(-1) radius > 1 iff |tr| > 2 (plexus radix)", ok_radix);
}

/* every freely reduced word of length <= maxlen, keyed to its class;
 * geodesic = length equals the class's census length */
static void
collect_geodesics (void)
{
    TabulaDispersa *index = tabula_dispersa_creare_chorda(pool, 4096);
    int            *slot = (int *)calloc((size_t)nk, sizeof(int));
    Plexus         *cand;
    int            *cand_class;
    int             ncand = 0;
    int             cap = 1;
    int             len;
    int             i;
    int             ok_rep = 1;
    int             ok_found = 1;
    int             ok_short = 1;
    int             letters[4] = { -2, -1, 1, 2 };
    int            *counter;

    for (i = 0; i < nk; i++) {
        chorda key;

        slot[i] = i;
        if (!plexus_clavis(ks[i].word, pool, &key)) fatal("key refused");
        tabula_dispersa_inserere(index, key, &slot[i]);
    }
    for (len = 1; len <= maxlen; len++) cap += 4 * (int)pow3(len - 1);
    cand = (Plexus *)malloc((size_t)cap * sizeof(Plexus));
    cand_class = (int *)malloc((size_t)cap * sizeof(int));
    /* breadth-first over reduced words */
    {
        Plexus empty = braid(3, "");
        int    lo = 0;
        int    hi;

        cand[ncand++] = empty;
        for (len = 1; len <= maxlen; len++) {
            int j;

            hi = ncand;
            for (j = lo; j < hi; j++) {
                int t;

                for (t = 0; t < 4; t++) {
                    s32    l = (s32)letters[t];
                    Plexus one;
                    Plexus w;

                    if (cand[j].longitudo > 0 && cand[j].litterae[
                            cand[j].longitudo - 1] == -l) {
                        continue;
                    }
                    plexus_creare(3, &l, 1, pool, &one);
                    plexus_iungere(cand[j], one, pool, &w);
                    cand[ncand++] = w;
                }
            }
            lo = hi;
        }
    }
    for (i = 0; i < ncand; i++) {
        chorda  key;
        vacuum *val = NULL;

        if (!plexus_clavis(cand[i], pool, &key)) fatal("key refused");
        if (!tabula_dispersa_invenire(index, key, &val)) {
            ok_found = 0;           /* every word of length <= L is in */
            cand_class[i] = -1;
            continue;
        }
        cand_class[i] = *(int *)val;
        if ((int)cand[i].longitudo < ks[cand_class[i]].len) ok_short = 0;
    }
    check("every reduced word up to the census length finds its class",
        ok_found);
    check("no word is shorter than its class's census length (census "
        "levels are geodesic)", ok_short);
    /* group geodesic words by class, in enumeration order */
    for (i = 0; i < ncand; i++) {
        if (cand_class[i] >= 0 && (int)cand[i].longitudo
                == ks[cand_class[i]].len) {
            ks[cand_class[i]].ngeo++;
        }
    }
    ngeo = 0;
    for (i = 0; i < nk; i++) {
        ks[i].geo0 = ngeo;
        ngeo += ks[i].ngeo;
    }
    geo = (Plexus *)malloc((size_t)ngeo * sizeof(Plexus));
    geo_class = (int *)malloc((size_t)ngeo * sizeof(int));
    counter = (int *)calloc((size_t)nk, sizeof(int));
    for (i = 0; i < ncand; i++) {
        int c = cand_class[i];

        if (c >= 0 && (int)cand[i].longitudo == ks[c].len) {
            geo[ks[c].geo0 + counter[c]] = cand[i];
            geo_class[ks[c].geo0 + counter[c]] = c;
            counter[c]++;
        }
    }
    for (i = 0; i < nk; i++) {
        int j;

        ks[i].rep = -1;
        for (j = ks[i].geo0; j < ks[i].geo0 + ks[i].ngeo; j++) {
            if (words_equal(geo[j], ks[i].word)) ks[i].rep = j;
        }
        if (ks[i].rep < 0) ok_rep = 0;
    }
    check("each class's representative is among its geodesic words",
        ok_rep);
    printf("  reduced words up to length %d: %d; geodesic: %d\n", maxlen,
        ncand, ngeo);
    free(counter);
    free(slot);
    free(cand_class);
    free(cand);
}

/* ================================================================
 * Part A: exact sectors of w^m
 * ================================================================ */

/* the 2 x 2 sector matrix applied to (x, y) equals a 2-vector */
static int
matrix_maps (
    Matrix        m,
    Magnus        x,
    Magnus        y,
    const Magnus *out)
{
    i32 r;

    for (r = 0; r < 2; r++) {
        Magnus v = magnus_adde(magnus_multiplica(*(const Magnus *)
            matrix_elementum(m, r, 0), x, pool), magnus_multiplica(
            *(const Magnus *)matrix_elementum(m, r, 1), y, pool), pool);

        if (!magnus_aequalis(v, out[r])) return 0;
    }
    return 1;
}

static int ok_monotone = 1;
static int ok_sector_eval = 1;     /* pattern, nullae, M x at medius */
static int ok_sector_dyn = 1;      /* M x = ansa_agere^m at medius */
static int ok_pieces_class = 1;    /* every geodesic word: same pieces */
static int ok_periodic_r = 1;      /* r(m) = r(k), m >= k */
static int ok_periodic_p = 1;      /* p(k) = 1, M = I; p(m + k) = p(m) */

/* checks every sector of the representative's w^m at its medius */
static void
check_sectors (
    Plexus              w,
    const Nervus       *net,
    const NervusPlanum *pl,
    int                 m)
{
    int k;

    for (k = 0; k < (int)pl->numerus; k++) {
        const NervusSector *s = &pl->regiones[k];
        NervusForma         f;
        Magnus              in[2];
        Magnus              out[2];
        Ansa                u;
        int                 t;

        in[0] = s->medius.x;
        in[1] = s->medius.y;
        if (!nervus_forma_creare(net, pool, &f)
                || !nervus_aestimare(net, in, out, &f, pool)) {
            fatal("evaluation refused");
        }
        if (!nervus_formae_aequales(&f, &s->forma)
                || f.nullae != s->forma.nullae
                || !matrix_maps(s->m, in[0], in[1], out)) {
            ok_sector_eval = 0;
        }
        /* independent: the Dynnikov action itself, m passes */
        if (!ansa_ex_coordinatis(3, &in[0], &in[1], pool, &u)) {
            fatal("loop refused");
        }
        for (t = 0; t < m; t++) {
            if (!ansa_agere(u, w, pool, &u)) fatal("action refused");
        }
        if (!magnus_aequalis(u.a[0], out[0])
                || !magnus_aequalis(u.b[0], out[1])) {
            ok_sector_dyn = 0;
        }
    }
}

/* matrix is the 2 x 2 identity */
static int
is_identity (
    Matrix m)
{
    i32 r, c;

    for (r = 0; r < 2; r++) {
        for (c = 0; c < 2; c++) {
            Magnus e = *(const Magnus *)matrix_elementum(m, r, c);

            if (!magnus_aequalis(e, magnus_ex_s64(r == c ? 1 : 0))) {
                return 0;
            }
        }
    }
    return 1;
}

/* regions and pieces of w^1 .. w^MMAX for one word */
static void
sweep_word (
    Klass *kl,
    int    g,
    int   *r,
    int   *p,
    int    is_rep)
{
    PiscinaNotatio mark = piscina_notare(pool);
    Nervus        *one;
    Nervus        *net;
    int            m;

    if (!plexus_nervus(geo[g], pool, &one)) fatal("network refused");
    net = one;
    for (m = 1; m <= MMAX; m++) {
        NervusPlanum pl;
        int          k;

        if (m > 1 && !nervus_componere(net, one, pool, &net)) {
            fatal("composition refused");
        }
        if (!nervus_regiones_plani(net, pool, &pl)) fatal("sweep refused");
        r[m] = (int)pl.numerus;
        p[m] = (int)pl.frusta;
        for (k = 0; k < (int)pl.numerus; k++) {
            if (pl.regiones[k].forma.nullae > 0) kl->null_sectors++;
        }
        if (is_rep) {
            check_sectors(geo[g], net, &pl, m);
        }
        if (kl->cls == CLS_P && m == kl->period) {
            /* w^k is the identity map: one piece, M = I */
            if (pl.frusta != 1 || !is_identity(pl.regiones[0].m)) {
                ok_periodic_p = 0;
            }
        }
    }
    piscina_reficere(pool, mark);
    for (m = 2; m <= MMAX; m++) {
        if (r[m] < r[m - 1]) ok_monotone = 0;
    }
    if (kl->cls == CLS_P) {
        int k = kl->period;

        for (m = k; m <= MMAX; m++) {
            if (r[m] != r[k]) ok_periodic_r = 0;
        }
        for (m = 1; m + k <= MMAX; m++) {
            if (p[m + k] != p[m]) ok_periodic_p = 0;
        }
    }
}

static void
part_a_sweeps (void)
{
    int i;

    for (i = 0; i < nk; i++) {
        Klass *kl = &ks[i];
        int    g;
        int    m;

        for (m = 0; m <= MMAX; m++) {
            kl->rmin[m] = 1 << 30;
            kl->rmax[m] = 0;
        }
        kl->best = -1;
        for (g = kl->geo0; g < kl->geo0 + kl->ngeo; g++) {
            int r[MMAX + 1];
            int p[MMAX + 1];

            sweep_word(kl, g, r, p, g == kl->rep);
            for (m = 1; m <= MMAX; m++) {
                if (r[m] < kl->rmin[m]) kl->rmin[m] = r[m];
                if (r[m] > kl->rmax[m]) kl->rmax[m] = r[m];
            }
            if (g == kl->geo0) {
                memcpy(kl->pieces, p, sizeof(p));
            } else if (memcmp(kl->pieces + 1, p + 1, MMAX * sizeof(int))) {
                ok_pieces_class = 0;
            }
            if (g == kl->rep) memcpy(kl->rrep, r, sizeof(r));
            if (kl->best < 0 || r[MMAX] > kl->rbest[MMAX]) {
                kl->best = g;
                memcpy(kl->rbest, r, sizeof(r));
            }
        }
    }
}

/* ---------------- the registered rule ---------------- */

/* H_lin: increments d(m) = s(m+1) - s(m), m = 6..11, repeat with some
 * period q <= 3 */
static int
h_lin (
    const int *s)
{
    int q;

    for (q = 1; q <= 3; q++) {
        int m;
        int ok = 1;

        for (m = 6; m + q <= 11; m++) {
            if (s[m + q + 1] - s[m + q] != s[m + 1] - s[m]) ok = 0;
        }
        if (ok) return 1;
    }
    return 0;
}

/* H_exp: s(m+1) / s(m) >= 1.5 for every m in 8..11 */
static int
h_exp (
    const int *s)
{
    int m;

    for (m = 8; m <= 11; m++) {
        if (2 * s[m + 1] < 3 * s[m]) return 0;
    }
    return 1;
}

static void
part_a_report (void)
{
    int  c, m, i, len;
    char b1[32], b2[32], b3[32], b4[32];
    long nulls = 0;

    printf("\n  Classes and geodesic words by length:\n");
    printf("    %4s %8s %5s %5s %5s %10s %10s\n", "len", "classes", "P", "E",
        "Z", "geodesics", "per class");
    for (len = 0; len <= maxlen; len++) {
        int n = 0, cnt[3] = { 0, 0, 0 };
        long g = 0;

        for (i = 0; i < nk; i++) {
            if (ks[i].len != len) continue;
            n++;
            cnt[ks[i].cls]++;
            g += ks[i].ngeo;
        }
        mean_text(g, n, b1);
        printf("    %4d %8d %5d %5d %5d %10ld %10s\n", len, n, cnt[0],
            cnt[1], cnt[2], g, b1);
    }
    for (i = 0; i < nk; i++) nulls += ks[i].null_sectors;
    printf("\n  Sectors with an identically-zero ReLU argument, over all "
        "geodesic words\n  and m = 1..%d: %ld\n", MMAX, nulls);

    printf("\n  Means over classes of pieces p(m) and regions (min / max "
        "over geodesic words):\n");
    for (c = 0; c < 3; c++) {
        int n = 0;

        for (i = 0; i < nk; i++) if (ks[i].cls == c) n++;
        printf("    %s (%d classes)\n      %3s %8s %8s %8s %8s\n",
            cls_name[c], n, "m", "pieces", "r min", "r max", "r rep");
        for (m = 1; m <= MMAX; m++) {
            long sp = 0, s1 = 0, s2 = 0, s3 = 0;

            for (i = 0; i < nk; i++) {
                if (ks[i].cls != c) continue;
                sp += ks[i].pieces[m];
                s1 += ks[i].rmin[m];
                s2 += ks[i].rmax[m];
                s3 += ks[i].rrep[m];
            }
            mean_text(sp, n, b1);
            mean_text(s1, n, b2);
            mean_text(s2, n, b3);
            mean_text(s3, n, b4);
            printf("      %3d %8s %8s %8s %8s\n", m, b1, b2, b3, b4);
        }
    }

    /* pieces at m = MMAX, by type */
    printf("\n  Pieces p(%d) by type (count of classes per value):\n", MMAX);
    for (c = 0; c < 3; c++) {
        int v;

        printf("    %s:", cls_name[c]);
        for (v = 1; v <= 64; v++) {
            int n = 0;

            for (i = 0; i < nk; i++) {
                if (ks[i].cls == c && ks[i].pieces[MMAX] == v) n++;
            }
            if (n) printf("  %d:%d", v, n);
        }
        printf("\n");
    }
    /* E: regions added over m = 6..12 (most-regions word) vs pieces */
    printf("\n  E: regions added over m = 6..%d by the most-regions word, "
        "r(%d) - r(6),\n  against the pieces p(%d) (count of classes):\n",
        MMAX, MMAX, MMAX);
    {
        int d;

        for (d = 0; d <= 64; d++) {
            int v, any = 0;

            for (v = 1; v <= 64; v++) {
                int n = 0;

                for (i = 0; i < nk; i++) {
                    if (ks[i].cls == CLS_E && ks[i].pieces[MMAX] == v
                            && ks[i].rbest[MMAX] - ks[i].rbest[6] == d) {
                        n++;
                    }
                }
                if (n) {
                    if (!any) printf("    added %2d:", d);
                    any = 1;
                    printf("  p=%d:%d", v, n);
                }
            }
            if (any) printf("\n");
        }
    }

    /* exploratory (post hoc, not registered): is the eventual growth
     * per pass the cyclically reduced length of the word? */
    {
        int n = 0, eq = 0, lt = 0, gt = 0;

        for (i = 0; i < nk; i++) {
            int added, cyc;

            if (ks[i].cls != CLS_E) continue;
            n++;
            added = ks[i].rbest[MMAX] - ks[i].rbest[6];
            cyc = cyclic_length(geo[ks[i].best]);
            if (added == (MMAX - 6) * cyc) eq++;
            else if (added < (MMAX - 6) * cyc) lt++;
            else gt++;
        }
        printf("\n  E, exploratory: r(%d) - r(6) of the most-regions word "
            "against %d x its\n  cyclically reduced length: equal %d, "
            "below %d, above %d (of %d)\n", MMAX, MMAX - 6, eq, lt, gt, n);
    }

    /* word dependence */
    printf("\n  Word dependence at m = %d (classes with r min < r max):\n",
        MMAX);
    for (c = 0; c < 3; c++) {
        int n = 0, differ = 0, gap = 0, several = 0;

        for (i = 0; i < nk; i++) {
            if (ks[i].cls != c) continue;
            n++;
            if (ks[i].ngeo > 1) several++;
            if (ks[i].rmin[MMAX] < ks[i].rmax[MMAX]) differ++;
            if (ks[i].rmax[MMAX] - ks[i].rmin[MMAX] > gap) {
                gap = ks[i].rmax[MMAX] - ks[i].rmin[MMAX];
            }
        }
        printf("    %s: %d of %d (with > 1 geodesic word: %d); largest gap "
            "%d\n", cls_name[c], differ, n, several, gap);
    }
    {
        Plexus q = braid(3, "2 1 2 1");
        chorda key;
        chorda k2;

        plexus_clavis(q, pool, &key);
        for (i = 0; i < nk; i++) {
            plexus_clavis(ks[i].word, pool, &k2);
            if (chorda_aequalis(key, k2)) break;
        }
        if (i < nk) {
            int g;
            int saved = ks[i].null_sectors;

            printf("    Delta sigma_1 (%d geodesic words), r(m) m = 1..6 "
                "and pieces:\n", ks[i].ngeo);
            for (g = ks[i].geo0; g < ks[i].geo0 + ks[i].ngeo; g++) {
                int  r[MMAX + 1], p[MMAX + 1];
                char w[64];

                sweep_word(&ks[i], g, r, p, 0);
                word_text(geo[g], w, 64);
                printf("      [%s]%s  r: %d %d %d %d %d %d   p: %d %d %d %d "
                    "%d %d\n", w, g == ks[i].rep ? " (census)" : "", r[1],
                    r[2], r[3], r[4], r[5], r[6], p[1], p[2], p[3], p[4],
                    p[5], p[6]);
            }
            ks[i].null_sectors = saved;  /* the reprint counted again */
        }
    }
}

/* the rule's code on hand sequences (index 0 unused) */
static void
check_rule (void)
{
    int lin[MMAX + 1], per2[MMAX + 1], quad[MMAX + 1], dbl[MMAX + 1];
    int near[MMAX + 1];
    int m;

    for (m = 0; m <= MMAX; m++) {
        lin[m] = 5 + 3 * m;                      /* increments 3 */
        per2[m] = 2 * m + (m % 2);               /* increments 3, 1 */
        quad[m] = m * m;                         /* increments grow */
        dbl[m] = 1 << m;                         /* ratio 2 */
        near[m] = m;
    }
    /* ratio exactly 1.5 at m = 8..11: 16 * 3^(m-8) * 2^(12-m) */
    for (m = 8; m <= MMAX; m++) {
        int t, v = 16;

        for (t = 8; t < m; t++) v *= 3;
        for (t = m; t < 12; t++) v *= 2;
        near[m] = v;
    }
    check("rule: H_lin on a line and on period-2 increments, not on m^2",
        h_lin(lin) && h_lin(per2) && !h_lin(quad));
    check("rule: H_exp on 2^m and at ratio exactly 1.5, not on a line",
        h_exp(dbl) && h_exp(near) && !h_exp(lin));
}

static void
part_a_rule (void)
{
    int c, i;

    printf("\n  Registered rule over m = 6..%d (H_lin: increments repeat "
        "with period <= 3;\n  H_exp: ratio >= 1.5 for every m in 8..11), "
        "on pieces and on the geodesic\n  word with the most regions at "
        "m = %d:\n", MMAX, MMAX);
    printf("    %4s %8s %10s %10s %8s %10s %10s\n", "type", "classes",
        "lin(p)", "lin(r)", "lin both", "exp(p)", "exp(r)");
    for (c = 0; c < 3; c++) {
        int n = 0, lp = 0, lr = 0, lb = 0, ep = 0, er = 0;

        for (i = 0; i < nk; i++) {
            int a, b;

            if (ks[i].cls != c) continue;
            n++;
            a = h_lin(ks[i].pieces);
            b = h_lin(ks[i].rbest);
            lp += a;
            lr += b;
            lb += a && b;
            ep += h_exp(ks[i].pieces);
            er += h_exp(ks[i].rbest);
        }
        printf("    %4s %8d %10d %10d %8d %10d %10d\n", cls_name[c], n, lp,
            lr, lb, ep, er);
        if (c == CLS_E) {
            printf("    prediction (H_lin on both for >= 90%% of E, H_exp "
                "for none): %s\n", (10 * lb >= 9 * n && ep == 0 && er == 0)
                ? "HOLDS" : "FAILS");
        }
    }
    /* E by trace: growth per pass of pieces and of max regions */
    printf("\n  E classes by |tr| (lambda = (|tr| + sqrt(tr^2 - 4)) / 2): "
        "mean growth\n  per pass over m = 6..%d, (s(%d) - s(6)) / 6:\n",
        MMAX, MMAX);
    printf("    %5s %8s %10s %12s\n", "|tr|", "classes", "pieces",
        "max regions");
    {
        long t;
        long top = 0;

        for (i = 0; i < nk; i++) {
            long a = ks[i].tr < 0 ? -ks[i].tr : ks[i].tr;

            if (ks[i].cls == CLS_E && a > top) top = a;
        }
        for (t = 3; t <= top; t++) {
            int  n = 0;
            long sp = 0, sr = 0;
            char b1[32], b2[32];

            for (i = 0; i < nk; i++) {
                long a = ks[i].tr < 0 ? -ks[i].tr : ks[i].tr;

                if (ks[i].cls != CLS_E || a != t) continue;
                n++;
                sp += ks[i].pieces[MMAX] - ks[i].pieces[6];
                sr += ks[i].rbest[MMAX] - ks[i].rbest[6];
            }
            if (n == 0) continue;
            mean_text(sp, 6L * n, b1);
            mean_text(sr, 6L * n, b2);
            printf("    %5ld %8d %10s %12s\n", t, n, b1, b2);
        }
    }
}

static void
part_a (void)
{
    printf("\n=== Part A: exact regions and pieces on B_3 ===\n");
    build_census();
    collect_geodesics();
    part_a_sweeps();
    check("regions never decrease in m (every geodesic word)", ok_monotone);
    check("every sector of the census word's w^m: evaluation at medius "
        "gives its pattern, nullae and M x", ok_sector_eval);
    check("... and M x equals the Dynnikov action m times (ansa_agere, "
        "not nervus)", ok_sector_dyn);
    check("pieces are a class invariant (every geodesic word, every m)",
        ok_pieces_class);
    check("periodic: r(m) = r(k) for m >= k (every geodesic word)",
        ok_periodic_r);
    check("periodic: w^k is the identity (one piece, M = I), "
        "p(m + k) = p(m)", ok_periodic_p);
    part_a_report();
    check_rule();
    part_a_rule();
}

/* ================================================================
 * Part B: the lattice lower bound on B_3
 * ================================================================
 *
 * Points of the shells max(|x|, |y|) = R. A point is INTERIOR at m if
 * no ReLU argument was exactly 0 in passes 1..m. Its itinerary at m is
 * interned by refinement: id_m = intern(id_{m-1}, pattern of pass m).
 * Distinct interior itineraries are distinct full-dimensional regions,
 * so their number is a lower bound on r(m); the union over shells
 * R' <= R grows with R. The exact sectors (Part A, census word) check
 * every interior point: it lies strictly inside one sector whose
 * pattern equals its itinerary. */

static int shells[4] = { 4, 16, 64, 256 };
static int nshells = 4;

typedef struct {
    long   x, y;
    int    shell;      /* index into shells */
    int    half;       /* 0: angle in [0, pi), 1: [pi, 2 pi) */
    int    times4;     /* index of 4 p (next shell), or -1 */
} Point;

static Point *pts;
static int    npts;

/* exact angular order */
static int
point_cmp (
    const void *a,
    const void *b)
{
    const Point *p = (const Point *)a;
    const Point *q = (const Point *)b;
    long         cr;

    if (p->half != q->half) return p->half - q->half;
    cr = p->x * q->y - p->y * q->x;
    if (cr > 0) return -1;
    if (cr < 0) return 1;
    /* same ray: smaller shell first */
    return p->shell - q->shell;
}

static void
make_points (void)
{
    int s;

    npts = 0;
    for (s = 0; s < nshells; s++) npts += 8 * shells[s];
    pts = (Point *)malloc((size_t)npts * sizeof(Point));
    npts = 0;
    for (s = 0; s < nshells; s++) {
        long R = shells[s];
        long t;

        for (t = -R; t < R; t++) {
            long xy[4][2];
            int  k;

            xy[0][0] = R;  xy[0][1] = t;
            xy[1][0] = -t; xy[1][1] = R;
            xy[2][0] = -R; xy[2][1] = -t;
            xy[3][0] = t;  xy[3][1] = -R;
            for (k = 0; k < 4; k++) {
                Point *p = &pts[npts++];

                p->x = xy[k][0];
                p->y = xy[k][1];
                p->shell = s;
                p->half = (p->y > 0 || (p->y == 0 && p->x > 0)) ? 0 : 1;
            }
        }
    }
    qsort(pts, (size_t)npts, sizeof(Point), point_cmp);
    /* 4 p sits on the same ray, one shell further: in angular order the
     * points of one ray are adjacent, by shell */
    {
        int i;

        for (i = 0; i < npts; i++) {
            int j;

            pts[i].times4 = -1;
            for (j = i + 1; j < npts && j <= i + nshells; j++) {
                if (pts[j].x == 4 * pts[i].x && pts[j].y == 4 * pts[i].y) {
                    pts[i].times4 = j;
                }
            }
        }
    }
}

/* angle(u) against angle(x, y) in [0, 2 pi): -1, 0, +1 */
static int
ray_vs_point (
    NervusRadius u,
    long         x,
    long         y)
{
    int hu = (magnus_signum(u.y) > 0 || (magnus_signum(u.y) == 0
        && magnus_signum(u.x) > 0)) ? 0 : 1;
    int hp = (y > 0 || (y == 0 && x > 0)) ? 0 : 1;
    int cr;

    if (hu != hp) return hu < hp ? -1 : 1;
    cr = magnus_signum(magnus_subtrahe(magnus_multiplica(u.x,
        magnus_ex_s64(y), pool), magnus_multiplica(u.y, magnus_ex_s64(x),
        pool), pool));
    return -cr;
}

/* sign of cross(u, (x, y)) */
static int
cross_sign (
    NervusRadius u,
    long         x,
    long         y)
{
    return magnus_signum(magnus_subtrahe(magnus_multiplica(u.x,
        magnus_ex_s64(y), pool), magnus_multiplica(u.y, magnus_ex_s64(x),
        pool), pool));
}

/* strictly inside sector s (angle <= pi, or the whole plane) */
static int
inside (
    const NervusPlanum *pl,
    int                 k,
    long                x,
    long                y)
{
    const NervusSector *s = &pl->regiones[k];
    int                 a, b, uw;

    if (pl->numerus == 1) return 1;
    a = cross_sign(s->initium, x, y);
    uw = magnus_signum(magnus_subtrahe(magnus_multiplica(s->initium.x,
        s->finis.y, pool), magnus_multiplica(s->initium.y, s->finis.x,
        pool), pool));
    if (uw == 0) return a > 0;        /* half-plane */
    b = -cross_sign(s->finis, x, y);
    return a > 0 && b > 0;
}

static int ok_lattice_le = 1;     /* lattice count <= exact r(m) */
static int ok_lattice_sector = 1; /* interior point: inside one sector,
                                   * same pattern */
static int ok_homogeneous = 1;    /* id(4 p) = id(p) */
static int ok_union = 1;          /* union over shells nondecreasing */
static int ok_ids_sectors = 1;    /* distinct itineraries = distinct
                                   * sectors hit (all shells) */
static long homog_pairs = 0;

static long tot_points = 0;
static long tot_wall = 0;         /* points with nullae > 0 by m = MMAX */

/* per class: lattice counts low[s][m] (union of shells <= s) */
static void
lattice_class (
    Klass *kl,
    int    low[4][MMAX + 1])
{
    Piscina        *cpool = piscina_generare_dynamicum("d129_class",
        1 << 16);
    Piscina        *vpool[2];
    Nervus         *one;
    Nervus         *net;
    NervusPlanum    pl[MMAX + 1];
    TabulaDispersa *tab = tabula_dispersa_creare_chorda(cpool, 1024);
    int            *ids = (int *)malloc((size_t)npts * sizeof(int));
    int            *alive = (int *)malloc((size_t)npts * sizeof(int));
    Magnus         *vx = (Magnus *)malloc((size_t)npts * sizeof(Magnus));
    Magnus         *vy = (Magnus *)malloc((size_t)npts * sizeof(Magnus));
    unsigned char  *bits;            /* per point, all passes */
    int            *store;           /* id values */
    int            *seen;
    int             nstore = 0;
    int             capstore = npts * MMAX + 1;
    int             nrelu;
    int             width;
    int             m, i, s;

    vpool[0] = piscina_generare_dynamicum("d129_v0", 1 << 16);
    vpool[1] = piscina_generare_dynamicum("d129_v1", 1 << 16);
    if (!plexus_nervus(kl->word, cpool, &one)) fatal("network refused");
    nrelu = (int)one->relu;
    if (sizeof(int) != 4 || nrelu > 96) fatal("itinerary key too small");
    width = nrelu * MMAX;
    bits = (unsigned char *)malloc((size_t)npts * (size_t)(width + 1));
    store = (int *)malloc((size_t)capstore * sizeof(int));
    seen = (int *)calloc((size_t)capstore + 1, sizeof(int));
    net = one;
    for (m = 1; m <= MMAX; m++) {
        if (m > 1 && !nervus_componere(net, one, cpool, &net)) {
            fatal("composition refused");
        }
        if (!nervus_regiones_plani(net, cpool, &pl[m])) {
            fatal("sweep refused");
        }
    }
    for (i = 0; i < npts; i++) {
        ids[i] = 0;
        alive[i] = 1;
        vx[i] = magnus_ex_s64(pts[i].x);
        vy[i] = magnus_ex_s64(pts[i].y);
    }
    for (m = 1; m <= MMAX; m++) {
        Piscina *vp = vpool[m % 2];

        piscina_vacare(vp);       /* held pass m - 2 */
        for (i = 0; i < npts; i++) {
            PiscinaNotatio mark = piscina_notare(pool);
            NervusForma    f;
            Magnus         in[2];
            Magnus         out[2];
            char           keybuf[16];
            chorda         key;
            vacuum        *val = NULL;
            int            b;

            in[0] = vx[i];
            in[1] = vy[i];
            if (!nervus_forma_creare(one, pool, &f)
                    || !nervus_aestimare(one, in, out, &f, pool)) {
                fatal("evaluation refused");
            }
            vx[i] = magnus_transcribe(out[0], vp);
            vy[i] = magnus_transcribe(out[1], vp);
            if (f.nullae > 0) alive[i] = 0;
            for (b = 0; b < nrelu; b++) {
                bits[(size_t)i * (size_t)(width + 1)
                    + (size_t)((m - 1) * nrelu + b)]
                    = nervus_forma_bitus(&f, (i32)b) ? 1 : 0;
            }
            if (alive[i]) {
                /* key: previous id (4 bytes) + this pass's pattern */
                memcpy(keybuf, &ids[i], 4);
                for (b = 0; b < (nrelu + 7) / 8; b++) {
                    keybuf[4 + b] = (char)f.bitus[b];
                }
                if (nrelu % 8) {
                    keybuf[4 + (nrelu - 1) / 8] = (char)(keybuf[4
                        + (nrelu - 1) / 8] & ((1 << (nrelu % 8)) - 1));
                }
                key.datum = (i8 *)keybuf;
                key.mensura = (i32)(4 + (nrelu + 7) / 8);
                if (tabula_dispersa_invenire(tab, key, &val)) {
                    ids[i] = *(int *)val;
                } else {
                    chorda copy = chorda_transcribere(key, cpool);

                    if (nstore >= capstore) fatal("itinerary store full");
                    store[nstore] = nstore + 1;
                    tabula_dispersa_inserere(tab, copy, &store[nstore]);
                    ids[i] = nstore + 1;
                    nstore++;
                }
            }
            piscina_reficere(pool, mark);
        }
        /* counts: distinct ids among interior points of shells <= s */
        for (s = 0; s < nshells; s++) {
            int count = 0;
            int tag = 1 + s + 8 * m;

            for (i = 0; i < npts; i++) {
                if (!alive[i] || pts[i].shell > s) continue;
                if (seen[ids[i]] != tag) {
                    seen[ids[i]] = tag;
                    count++;
                }
            }
            low[s][m] = count;
            if (count > kl->rrep[m]) ok_lattice_le = 0;
            if (s > 0 && count < low[s - 1][m]) ok_union = 0;
        }
        /* angular walk: each interior point strictly inside the sector
         * whose wall precedes it, with its itinerary as pattern */
        {
            const NervusPlanum *q = &pl[m];
            int                 j = 0;
            int                *hit = (int *)calloc((size_t)q->numerus,
                sizeof(int));
            int                 nhit = 0;

            for (i = 0; i < npts; i++) {
                int k, b, on_wall = 0;

                while (j + 1 < (int)q->numerus && ray_vs_point(
                        q->regiones[j + 1].initium, pts[i].x,
                        pts[i].y) <= 0) {
                    j++;
                }
                k = j;
                if (q->numerus > 1) {
                    int c0 = ray_vs_point(q->regiones[j].initium, pts[i].x,
                        pts[i].y);

                    if (c0 > 0) k = (int)q->numerus - 1;   /* wraps */
                    if (c0 == 0) on_wall = 1;
                }
                if (!alive[i]) continue;
                if (on_wall || !inside(q, k, pts[i].x, pts[i].y)) {
                    ok_lattice_sector = 0;
                    continue;
                }
                for (b = 0; b < nrelu * m; b++) {
                    if (bits[(size_t)i * (size_t)(width + 1) + (size_t)b]
                            != (nervus_forma_bitus(&q->regiones[k].forma,
                            (i32)b) ? 1 : 0)) {
                        ok_lattice_sector = 0;
                    }
                }
                if (!hit[k]) {
                    hit[k] = 1;
                    nhit++;
                }
            }
            if (nhit != low[nshells - 1][m]) ok_ids_sectors = 0;
            free(hit);
        }
    }
    /* homogeneity: 4 p (next shell) has p's itinerary */
    for (i = 0; i < npts; i++) {
        int j = pts[i].times4;

        if (j < 0) continue;
        homog_pairs++;
        if (ids[j] != ids[i] || alive[j] != alive[i]) ok_homogeneous = 0;
    }
    for (i = 0; i < npts; i++) {
        tot_points++;
        if (!alive[i]) tot_wall++;
    }
    free(seen);
    free(store);
    free(bits);
    free(vy);
    free(vx);
    free(alive);
    free(ids);
    piscina_destruere(vpool[0]);
    piscina_destruere(vpool[1]);
    piscina_destruere(cpool);
}

static void
part_b (void)
{
    int  i, s, m, c;
    long sum_exact[3][MMAX + 1];
    long sum_low[3][4][MMAX + 1];
    int  ncls[3] = { 0, 0, 0 };
    int  reach[3][4];             /* classes whose bound reaches r(12) */
    char b1[32];

    printf("\n=== Part B: the lattice lower bound on B_3 ===\n");
    make_points();
    memset(sum_exact, 0, sizeof(sum_exact));
    memset(sum_low, 0, sizeof(sum_low));
    memset(reach, 0, sizeof(reach));
    for (i = 0; i < nk; i++) {
        int low[4][MMAX + 1];

        lattice_class(&ks[i], low);
        ncls[ks[i].cls]++;
        for (m = 1; m <= MMAX; m++) {
            sum_exact[ks[i].cls][m] += ks[i].rrep[m];
            for (s = 0; s < nshells; s++) {
                sum_low[ks[i].cls][s][m] += low[s][m];
            }
        }
        for (s = 0; s < nshells; s++) {
            if (low[s][MMAX] == ks[i].rrep[MMAX]) reach[ks[i].cls][s]++;
        }
    }
    check("lattice count <= exact r(m) (census word, every shell, every m)",
        ok_lattice_le);
    check("every interior lattice point lies strictly inside one exact "
        "sector with its itinerary as pattern", ok_lattice_sector);
    check("union over shells never decreases", ok_union);
    check("distinct interior itineraries = distinct sectors they lie in "
        "(the refinement is exact)", ok_ids_sectors);
    {
        long expect = 0;

        for (s = 0; s + 1 < nshells; s++) expect += 8L * shells[s];
        check("homogeneity: 4 p has p's itinerary and interiority (every "
            "point of every inner shell, every class)", ok_homogeneous
            && homog_pairs == expect * nk);
    }
    printf("\n  Points on walls by m = %d (some argument exactly 0 in some "
        "pass): %ld of %ld\n", MMAX, tot_wall, tot_points);
    printf("\n  Mean regions of the census word at m = %d: exact vs lattice "
        "(shells <= R):\n", MMAX);
    printf("    %4s %8s %8s", "type", "classes", "exact");
    for (s = 0; s < nshells; s++) printf("   R<=%-4d", shells[s]);
    printf("\n");
    for (c = 0; c < 3; c++) {
        mean_text(sum_exact[c][MMAX], ncls[c], b1);
        printf("    %4s %8d %8s", cls_name[c], ncls[c], b1);
        for (s = 0; s < nshells; s++) {
            mean_text(sum_low[c][s][MMAX], ncls[c], b1);
            printf(" %9s", b1);
        }
        printf("\n");
    }
    printf("\n  Classes whose lattice bound reaches the exact r(%d):\n",
        MMAX);
    for (c = 0; c < 3; c++) {
        printf("    %s:", cls_name[c]);
        for (s = 0; s < nshells; s++) {
            printf("  R<=%d %d/%d", shells[s], reach[c][s], ncls[c]);
        }
        printf("\n");
    }
}

/* ================================================================
 * Part C: B_4 - a weak lattice bound, and the period-2 question
 * ================================================================
 *
 * Classes on B_4 (D128's method, all certified up to length 6): P iff
 * plexus_periodus > 0; E iff Burau(-1) has spectral radius > 1 or
 * Burau at a primitive cube root of unity has radius > 1 (Kolev's
 * bound, plexus); Z the rest (D128 checked their growth is
 * quasi-affine). */

static const int pin4_P[7] = { 1, 0, 0, 8, 24, 36, 102 };
static const int pin4_E[7] = { 0, 0, 8, 48, 198, 802, 2958 };
static const int pin4_Z[7] = { 0, 6, 18, 42, 116, 272, 482 };

typedef struct {
    Plexus word;
    int    len;
    int    cls;
    int    cube_only;      /* E certified only at a cube root */
} Klass4;

static Klass4 *k4;
static int     nk4;

static void
build_census4 (
    int len4)
{
    PlexusCensus cs;
    int          i;
    int          len;
    int          ok = 1;

    if (!plexus_enumerare(4, (i32)len4, 100000, pool, &cs)) {
        fatal("B_4 census refused");
    }
    nk4 = (int)cs.numerus;
    k4 = (Klass4 *)calloc((size_t)nk4, sizeof(Klass4));
    for (i = 0; i < nk4; i++) {
        i32         period = 0;
        RadixRealis rho;
        s32         sg = 0;
        b32         cube = FALSUM;

        k4[i].word = cs.verba[i];
        k4[i].len = (int)cs.verba[i].longitudo;
        if (!plexus_periodus(k4[i].word, pool, &period)
                || !plexus_burau_radix_maxima(k4[i].word, pool, &rho)
                || !radix_compara(rho, radix_ex_fractione(fractio_ex_s64(1),
                    pool), pool, &sg)
                || !plexus_burau_radius_maior(k4[i].word, 3, &cube)) {
            fatal("B_4 classification refused");
        }
        if (period > 0) {
            k4[i].cls = CLS_P;
        } else if (sg > 0 || cube) {
            k4[i].cls = CLS_E;
            k4[i].cube_only = sg > 0 ? 0 : 1;
        } else {
            k4[i].cls = CLS_Z;
        }
    }
    for (len = 0; len <= len4; len++) {
        int p = 0, e = 0, z = 0;

        for (i = 0; i < nk4; i++) {
            if (k4[i].len != len) continue;
            if (k4[i].cls == CLS_P) p++;
            else if (k4[i].cls == CLS_E) e++;
            else z++;
        }
        if (p != pin4_P[len] || e != pin4_E[len] || z != pin4_Z[len]) {
            ok = 0;
        }
    }
    check("B_4 classes per length = D128's table", ok);
}

/* ---------------- the weak lattice bound in d = 4 ---------------- */

#define L4_MMAX 8

static int ok4_monotone = 1;      /* refinement: count(m+1) >= count(m) */
static int ok4_union = 1;
static int ok4_homog = 1;         /* 2 p (shell 8) has p's itinerary */
static long homog4_pairs = 0;

/* the points of the shells max |x_i| = 4 and = 8 (shell 0 / 1) */
static long (*pts4)[4];
static int  *shell4;
static int  *twice4;              /* index of 2 p, or -1 */
static int   npts4;

static int
shell_index (
    const long *x,
    long        R)
{
    /* rank of x in the box [-R, R]^4, for the 2 p lookup */
    long w = 2 * R + 1;

    return (int)((((x[0] + R) * w + (x[1] + R)) * w + (x[2] + R)) * w
        + (x[3] + R));
}

static long shells4[2] = { 4, 8 };   /* quick mode: 2, 4 */

static void
make_points4 (
    int nsh)
{
    int   s, i;
    int  *rank8;
    long *sizes = shells4;

    npts4 = 0;
    for (s = 0; s < nsh; s++) {
        long R = sizes[s];
        long a = (2 * R + 1), b = (2 * R - 1);

        npts4 += (int)(a * a * a * a - b * b * b * b);
    }
    pts4 = (long (*)[4])malloc((size_t)npts4 * sizeof(*pts4));
    shell4 = (int *)malloc((size_t)npts4 * sizeof(int));
    twice4 = (int *)malloc((size_t)npts4 * sizeof(int));
    npts4 = 0;
    for (s = 0; s < nsh; s++) {
        long R = sizes[s];
        long x[4];

        for (x[0] = -R; x[0] <= R; x[0]++)
        for (x[1] = -R; x[1] <= R; x[1]++)
        for (x[2] = -R; x[2] <= R; x[2]++)
        for (x[3] = -R; x[3] <= R; x[3]++) {
            long mx = 0;
            int  t;

            for (t = 0; t < 4; t++) {
                long a = x[t] < 0 ? -x[t] : x[t];

                if (a > mx) mx = a;
            }
            if (mx != R) continue;
            for (t = 0; t < 4; t++) pts4[npts4][t] = x[t];
            shell4[npts4] = s;
            npts4++;
        }
    }
    /* 2 p for p on shell 4 lies on shell 8 */
    {
        long w = 2 * shells4[1] + 1;
        long box = w * w * w * w;

        rank8 = (int *)malloc((size_t)box * sizeof(int));
        for (i = 0; i < (int)box; i++) rank8[i] = -1;
    }
    for (i = 0; i < npts4; i++) {
        if (shell4[i] == 1) rank8[shell_index(pts4[i], shells4[1])] = i;
    }
    for (i = 0; i < npts4; i++) {
        long y[4];
        int  t;

        twice4[i] = -1;
        if (shell4[i] != 0 || nsh < 2) continue;
        for (t = 0; t < 4; t++) y[t] = 2 * pts4[i][t];
        twice4[i] = rank8[shell_index(y, shells4[1])];
    }
    free(rank8);
}

/* lattice counts low[s][m], m = 1..L4_MMAX, for one B_4 word */
static void
lattice4_class (
    Plexus w,
    int    nsh,
    int    low[2][L4_MMAX + 1])
{
    Piscina        *cpool = piscina_generare_dynamicum("d129_c4", 1 << 16);
    Piscina        *vpool[2];
    Nervus         *one;
    TabulaDispersa *tab = tabula_dispersa_creare_chorda(cpool, 4096);
    int            *ids = (int *)malloc((size_t)npts4 * sizeof(int));
    int            *prev = (int *)malloc((size_t)npts4 * sizeof(int));
    int            *alive = (int *)malloc((size_t)npts4 * sizeof(int));
    Magnus        (*v)[4] = (Magnus (*)[4])malloc((size_t)npts4
        * sizeof(*v));
    int            *store;
    int            *seen;
    int             nstore = 0;
    int             capstore = npts4 * L4_MMAX + 1;
    int             nrelu;
    int             m, i, s;

    vpool[0] = piscina_generare_dynamicum("d129_c4v0", 1 << 16);
    vpool[1] = piscina_generare_dynamicum("d129_c4v1", 1 << 16);
    if (!plexus_nervus(w, cpool, &one)) fatal("network refused");
    nrelu = (int)one->relu;
    if (sizeof(int) != 4 || nrelu > 96) fatal("itinerary key too small");
    store = (int *)malloc((size_t)capstore * sizeof(int));
    seen = (int *)calloc((size_t)capstore + 1, sizeof(int));
    for (i = 0; i < npts4; i++) {
        int t;

        ids[i] = 0;
        alive[i] = 1;
        for (t = 0; t < 4; t++) v[i][t] = magnus_ex_s64(pts4[i][t]);
    }
    for (m = 1; m <= L4_MMAX; m++) {
        Piscina *vp = vpool[m % 2];

        piscina_vacare(vp);
        for (i = 0; i < npts4; i++) {
            PiscinaNotatio mark = piscina_notare(pool);
            NervusForma    f;
            Magnus         out[4];
            char           keybuf[16];
            chorda         key;
            vacuum        *val = NULL;
            int            b, t;

            if (!nervus_forma_creare(one, pool, &f)
                    || !nervus_aestimare(one, v[i], out, &f, pool)) {
                fatal("evaluation refused");
            }
            prev[i] = ids[i];
            for (t = 0; t < 4; t++) v[i][t] = magnus_transcribe(out[t], vp);
            if (f.nullae > 0) alive[i] = 0;
            if (alive[i]) {
                memcpy(keybuf, &ids[i], 4);
                for (b = 0; b < (nrelu + 7) / 8; b++) {
                    keybuf[4 + b] = (char)f.bitus[b];
                }
                if (nrelu % 8) {
                    keybuf[4 + (nrelu - 1) / 8] = (char)(keybuf[4
                        + (nrelu - 1) / 8] & ((1 << (nrelu % 8)) - 1));
                }
                key.datum = (i8 *)keybuf;
                key.mensura = (i32)(4 + (nrelu + 7) / 8);
                if (tabula_dispersa_invenire(tab, key, &val)) {
                    ids[i] = *(int *)val;
                } else {
                    chorda copy = chorda_transcribere(key, cpool);

                    if (nstore >= capstore) fatal("itinerary store full");
                    store[nstore] = nstore + 1;
                    tabula_dispersa_inserere(tab, copy, &store[nstore]);
                    ids[i] = nstore + 1;
                    nstore++;
                }
            }
            piscina_reficere(pool, mark);
        }
        for (s = 0; s < nsh; s++) {
            int count = 0;
            int tag = 1 + s + 4 * m;

            for (i = 0; i < npts4; i++) {
                if (!alive[i] || shell4[i] > s) continue;
                if (seen[ids[i]] != tag) {
                    seen[ids[i]] = tag;
                    count++;
                }
            }
            low[s][m] = count;
            if (s > 0 && count < low[s - 1][m]) ok4_union = 0;
        }
        /* refinement: among points still interior, the m-itineraries
         * are at least as many as the (m-1)-itineraries */
        if (m > 1) {
            int now = 0, before = 0;

            for (i = 0; i < npts4; i++) {
                if (!alive[i]) continue;
                if (seen[ids[i]] != -m) {
                    seen[ids[i]] = -m;
                    now++;
                }
                if (seen[prev[i]] != -m - 100) {
                    seen[prev[i]] = -m - 100;
                    before++;
                }
            }
            if (now < before) ok4_monotone = 0;
        }
    }
    for (i = 0; i < npts4; i++) {
        int j = twice4[i];

        if (j < 0) continue;
        homog4_pairs++;
        if (ids[j] != ids[i] || alive[j] != alive[i]) ok4_homog = 0;
    }
    free(seen);
    free(store);
    free(v);
    free(alive);
    free(prev);
    free(ids);
    piscina_destruere(vpool[0]);
    piscina_destruere(vpool[1]);
    piscina_destruere(cpool);
}

static void
part_c_lattice (
    int len4,
    int nsh)
{
    long sum[3][2][L4_MMAX + 1];
    int  ncls[3] = { 0, 0, 0 };
    int  lin[3] = { 0, 0, 0 }, ex[3] = { 0, 0, 0 };
    int  i, s, m, c;
    char b1[32];

    printf("\n  (a) Weak lattice LOWER bound on B_4 up to length %d, shells "
        "R = %ld, %ld, m = 1..%d.\n  Part B showed this bound misses most "
        "regions of E braids on B_3 (13 of 88 at\n  R <= 256): read it "
        "as a floor, never as a count.\n", len4, shells4[0], shells4[1],
        L4_MMAX);
    make_points4(nsh);
    memset(sum, 0, sizeof(sum));
    for (i = 0; i < nk4; i++) {
        int low[2][L4_MMAX + 1];
        int seq[MMAX + 1];

        if (k4[i].len > len4) continue;
        lattice4_class(k4[i].word, nsh, low);
        c = k4[i].cls;
        ncls[c]++;
        for (m = 1; m <= L4_MMAX; m++) {
            for (s = 0; s < nsh; s++) sum[c][s][m] += low[s][m];
        }
        /* the rule's shapes on the bound (m = 4..8): increments repeat
         * with period <= 2, or ratio >= 1.5 at m = 6..7 */
        memset(seq, 0, sizeof(seq));
        for (m = 1; m <= L4_MMAX; m++) seq[m] = low[nsh - 1][m];
        {
            int q, okq = 0;

            for (q = 1; q <= 2 && !okq; q++) {
                int t, all = 1;

                for (t = 4; t + q + 1 <= L4_MMAX; t++) {
                    if (seq[t + q + 1] - seq[t + q] != seq[t + 1] - seq[t]) {
                        all = 0;
                    }
                }
                okq = all;
            }
            lin[c] += okq;
            ex[c] += 2 * seq[7] >= 3 * seq[6] && 2 * seq[8] >= 3 * seq[7];
        }
    }
    check("B_4 lattice: itineraries refine (points still interior keep "
        "at least their earlier\n        distinct itineraries), and the "
        "shell union never decreases", ok4_monotone && ok4_union);
    {
        long expect = 0;

        for (i = 0; i < npts4; i++) if (twice4[i] >= 0) expect++;
        check("B_4 lattice: homogeneity, 2 p has p's itinerary (every point "
            "of the inner shell, every class)", ok4_homog && (nsh < 2
            || (expect > 0 && homog4_pairs == expect * (long)(ncls[0]
            + ncls[1] + ncls[2]))));
    }
    printf("    %4s %8s", "type", "classes");
    for (m = 1; m <= L4_MMAX; m++) printf(" %6d", m);
    printf("   lin   exp\n");
    for (c = 0; c < 3; c++) {
        printf("    %4s %8d", cls_name[c], ncls[c]);
        for (m = 1; m <= L4_MMAX; m++) {
            mean_text(sum[c][nsh - 1][m], ncls[c], b1);
            printf(" %6s", b1);
        }
        printf(" %5d %5d\n", lin[c], ex[c]);
    }
    printf("  (mean lattice count, shells <= R = %ld; lin = increments over "
        "m = 4..8 repeat with\n  period <= 2, exp = ratio >= 1.5 at m = 6 "
        "and 7: shapes of a FLOOR, consistent-with only)\n",
        shells4[nsh - 1]);
}

/* ---------------- the period-2 question ---------------- */

#define O_PASSES 60
#define O_QMAX   12
#define O_TAIL   20
#define O_TERMS  12

/* minimal polynomial of a vector sequence y_0, y_1, ... (dim rows):
 * the first d with a kernel of [y_0 .. y_d]; FALSE if none <= dmax */
static int
min_poly (
    Magnus     **y,         /* y[j][r] */
    int          rows,
    int          nterms,
    int          dmax,
    Polynomium  *out,
    int         *deg)
{
    int d;

    for (d = 0; d <= dmax && d < nterms; d++) {
        Matrix a;
        Matrix ker;
        int    r, j;

        /* scalar sequences: Hankel rows = shifts, so the relation holds
         * for every window that fits. Vectors: ONE window, the Krylov
         * vectors y_0 .. y_d themselves; if the tail is one linear map,
         * the first dependency is its minimal polynomial and holds on
         * every later window (the caller rechecks that) */
        if (rows == 1) {
            int nr = nterms - d;

            if (nr < d + 1) return 0;
            if (!matrix_nulla(&ANULUS_INTEGRORUM, (i32)nr, (i32)(d + 1),
                    pool, &a)) fatal("matrix refused");
            for (r = 0; r < nr; r++) {
                for (j = 0; j <= d; j++) {
                    matrix_pone(&a, (i32)r, (i32)j, &y[r + j][0]);
                }
            }
        } else {
            if (!matrix_nulla(&ANULUS_INTEGRORUM, (i32)rows, (i32)(d + 1),
                    pool, &a)) fatal("matrix refused");
            for (r = 0; r < rows; r++) {
                for (j = 0; j <= d; j++) {
                    matrix_pone(&a, (i32)r, (i32)j, &y[j][r]);
                }
            }
        }
        if (!matrix_nucleus(a, pool, &ker)) fatal("kernel refused");
        if (matrix_columnae(ker) > 0) {
            Magnus c[8];

            for (j = 0; j <= d; j++) {
                c[j] = *(const Magnus *)matrix_elementum(ker, (i32)j, 0);
            }
            if (!polynomium_ex_coefficientibus(c, (i32)(d + 1), 0, pool,
                    out)) fatal("polynomial refused");
            *deg = d;
            return 1;
        }
    }
    return 0;
}

/* the argument of ReLU k of w as its own network (prefix, then the
 * ReLU's sum as an output) */
static Nervus *
argument_net (
    const Nervus *net,
    int           k)
{
    Nervus *a;
    int     t, r = 0;
    s32     reg = -1;

    if (!nervus_creare(net->introitus, pool, &a)) fatal("net refused");
    for (t = 0; t < (int)net->numerus; t++) {
        const NervusInstructio *in = &net->instructiones[t];

        if (in->genus == NERVUS_RELU && r++ == k) {
            if (!nervus_affinis(a, in->termini, in->fontes,
                    in->coefficientes, in->addendum, &reg)) {
                fatal("argument refused");
            }
            break;
        }
        if (!(in->genus == NERVUS_RELU ? nervus_relu : nervus_affinis)(a,
                in->termini, in->fontes, in->coefficientes, in->addendum,
                &reg)) {
            fatal("copy refused");
        }
    }
    if (!nervus_exitus(a, 1, &reg)) fatal("exit refused");
    return a;
}

typedef struct {
    int         settled;
    int         start;
    int         period;
    int         tie;      /* some argument vanishes in the limit */
    int         ok_mu;    /* mu real, > 1, simple; orbit sees it */
    int         grows;    /* mu > 1; else the loop is invariant (its
                           * orbit does not grow: a reducing curve) */
    RadixRealis mu;       /* over the braid's period Q (lambda^Q) */
    Magnus     *x[O_PASSES + 1];
    NervusForma f[O_PASSES];  /* pattern of each pass */
} Orbit4;

static int ok_q_tie = 1;          /* theorem: q >= 2 => tie */
static int ok_mu_all = 1;
static int ok_same_limit = 1;     /* the loops of one braid agree */
static int ok_recurrence = 1;     /* the tail obeys pi_x */
static int ok_invariant = 1;      /* a loop that does not grow is fixed:
                                   * tail polynomial t - 1 */
static int ok_vanish_pos = 1;     /* vanishing is the same at every
                                   * position of the cycle */
static int ok_varies_vanish = 1;  /* a bit that varies on the tail has a
                                   * vanishing argument */

/* phase 1: the orbit of loop E_k and its settled period */
static void
orbit4_settle (
    Nervus *net,
    int     k,
    Orbit4 *o)
{
    chorda pat[O_PASSES];
    Ansa   e;
    int    t, q;

    memset(o, 0, sizeof(*o));
    if (!ansa_circa(4, (i32)k, pool, &e)) fatal("loop refused");
    o->x[0] = (Magnus *)malloc(4 * sizeof(Magnus));
    o->x[0][0] = e.a[0];
    o->x[0][1] = e.a[1];
    o->x[0][2] = e.b[0];
    o->x[0][3] = e.b[1];
    for (t = 0; t < O_PASSES; t++) {
        o->x[t + 1] = (Magnus *)malloc(4 * sizeof(Magnus));
        if (!nervus_forma_creare(net, pool, &o->f[t])
                || !nervus_aestimare(net, o->x[t], o->x[t + 1], &o->f[t],
                    pool)
                || !nervus_forma_clavis(&o->f[t], pool, &pat[t])) {
            fatal("orbit refused");
        }
    }
    for (q = 1; q <= O_QMAX && !o->settled; q++) {
        int s = 0;

        for (t = O_PASSES - 1 - q; t >= 0; t--) {
            if (!chorda_aequalis(pat[t], pat[t + q])) {
                s = t + 1;
                break;
            }
        }
        if (O_PASSES - s >= O_TAIL && O_PASSES - s >= 3 * q) {
            o->settled = 1;
            o->start = s;
            o->period = q;
        }
    }
}

/* phase 2 (E): mu over the braid's period Q (a multiple of the loop's
 * own q), and the tie test at every position of a Q-cycle */
static void
orbit4_limit (
    Nervus **args,
    int      nrelu,
    int      start,
    int      Q,
    Orbit4  *o)
{
    Magnus      *y[O_TERMS + 1];
    Polynomium   px;
    Polynomium   dpx;
    RadixRealis *roots;
    i32          nroots = 0;
    s32          sg;
    int          deg = 0;
    int          nt = 0;
    int          j, p, kk;

    for (j = 0; j <= O_TERMS && start + j * Q <= O_PASSES; j++) {
        y[j] = o->x[start + j * Q];
        nt++;
    }
    if (!min_poly(y, 4, nt, 4, &px, &deg) || deg < 1) {
        ok_mu_all = 0;
        return;
    }
    /* the relation holds on every later window */
    for (j = 0; j + deg < nt; j++) {
        int r;

        for (r = 0; r < 4; r++) {
            Magnus sum = magnus_ex_s64(0);
            int    i;

            for (i = 0; i <= deg; i++) {
                sum = magnus_adde(sum, magnus_multiplica(
                    polynomium_coefficiens(px, (s32)i), y[j + i][r], pool),
                    pool);
            }
            if (magnus_signum(sum) != 0) ok_recurrence = 0;
        }
    }
    if (!radices_reales(px, pool, &roots, &nroots) || nroots < 1) {
        ok_mu_all = 0;
        return;
    }
    o->mu = roots[nroots - 1];
    if (!radix_compara(o->mu, radix_ex_fractione(fractio_ex_s64(1), pool),
            pool, &sg)) {
        fatal("compare refused");
    }
    if (sg <= 0) {
        /* the orbit does not grow: this loop is invariant under a
         * reducible braid (E with a pseudo-Anosov PIECE); it does not
         * flow to the attractor and is left out of the limit. Invariant
         * means x fixed on the tail: the polynomial is t - 1 exactly
         * (a Jordan block at 1 would grow linearly; checked) */
        if (deg != 1 || !magnus_aequalis(polynomium_coefficiens(px, 0),
                magnus_nega(polynomium_coefficiens(px, 1), pool))) {
            ok_invariant = 0;
        }
        o->grows = 0;
        return;
    }
    o->grows = 1;
    if (!polynomium_derivata(px, pool, &dpx)
            || !radix_signum_polynomii(dpx, o->mu, pool, &sg) || sg == 0) {
        ok_mu_all = 0;     /* mu must be simple */
        return;
    }
    o->ok_mu = 1;
    /* each argument at each position of the Q-cycle: if mu is not a
     * root of its minimal polynomial, it vanishes against |x|. The
     * argument is a continuous function of the input and every
     * position tends to the same direction, so the answer must not
     * depend on the position (checked). */
    for (kk = 0; kk < nrelu; kk++) {
        int vanish = -1;
        int t;
        int varies = 0;

        for (p = 0; p < Q; p++) {
            Magnus     *a[O_TERMS + 1];
            Magnus      store[O_TERMS + 1];
            Polynomium  pa;
            int         zero = 1;
            int         v;

            nt = 0;
            for (j = 0; j <= O_TERMS && start + p + j * Q < O_PASSES; j++) {
                if (!nervus_aestimare(args[kk], o->x[start + p + j * Q],
                        &store[j], NULL, pool)) {
                    fatal("argument evaluation refused");
                }
                a[j] = &store[j];
                if (magnus_signum(store[j]) != 0) zero = 0;
                nt++;
            }
            if (zero) {
                v = 1;
            } else {
                if (!min_poly(a, 1, nt, 4, &pa, &deg)) {
                    ok_mu_all = 0;
                    continue;
                }
                if (!radix_signum_polynomii(pa, o->mu, pool, &sg)) {
                    fatal("sign refused");
                }
                v = sg != 0;
            }
            if (vanish >= 0 && v != vanish) ok_vanish_pos = 0;
            vanish = v;
        }
        if (vanish == 1) o->tie = 1;
        for (t = o->start + 1; t < O_PASSES; t++) {
            if (nervus_forma_bitus(&o->f[t], (i32)kk)
                    != nervus_forma_bitus(&o->f[o->start], (i32)kk)) {
                varies = 1;
            }
        }
        if (varies && vanish != 1) ok_varies_vanish = 0;
    }
    if (o->period >= 2 && !o->tie) ok_q_tie = 0;
}

static void
orbit4_free (
    Orbit4 *o)
{
    int t;

    for (t = 0; t <= O_PASSES; t++) free(o->x[t]);
}

static int
lcm2 (
    int a,
    int b)
{
    int x = a, y = b;

    while (y) {
        int r = x % y;

        x = y;
        y = r;
    }
    return a / x * b;
}

static void
part_c_period (
    int len4)
{
    int tab[2][3][2];          /* cert (0 t = -1, 1 cube only), q, tie */
    int reducible[2] = { 0, 0 };  /* E with an invariant loop */
    int no_growth = 0;         /* E with no growing loop at all */
    int qcount[3][3];          /* type x (q = 1, 2, > 2) */
    int i, c;
    int settled_all = 1;

    memset(tab, 0, sizeof(tab));
    memset(qcount, 0, sizeof(qcount));
    printf("\n  (b) The period-2 question (KC 55.2.3.1), B_4 up to length "
        "%d: each braid on\n  every loop E_1..E_3, %d passes; settled period "
        "q per braid = lcm over loops\n  (D128 Part D). For E, exactly: mu "
        "= the largest real root of the tail's\n  minimal polynomial (dominant "
        "by north-south dynamics: ASSUMED, not checked);\n  an argument "
        "VANISHES in the limit iff mu is not a root of its own minimal\n  "
        "polynomial (its mu-component is 0). Tie = some argument vanishes "
        "at some\n  position of the cycle, on a loop whose orbit GROWS (mu > "
        "1; a loop that does\n  not grow is invariant and never reaches the "
        "attractor).\n", len4, O_PASSES);
    for (i = 0; i < nk4; i++) {
        PiscinaNotatio mark = piscina_notare(pool);
        Nervus        *net;
        Nervus       **args;
        Orbit4         o[3];
        int            k, q = 1, tie = 0, settled = 1, fixed_loop = 0;
        int            is_e = k4[i].cls == CLS_E;

        if (k4[i].len > len4) {
            piscina_reficere(pool, mark);
            continue;
        }
        if (!plexus_nervus(k4[i].word, pool, &net)) fatal("net refused");
        args = (Nervus **)malloc((size_t)(net->relu + 1) * sizeof(Nervus *));
        for (k = 0; k < (int)net->relu; k++) args[k] = argument_net(net, k);
        for (k = 1; k <= 3; k++) {
            orbit4_settle(net, k, &o[k - 1]);
            if (!o[k - 1].settled) settled = 0;
            else q = lcm2(q, o[k - 1].period);
        }
        if (is_e && settled) {
            int start = 0;

            for (k = 0; k < 3; k++) {
                if (o[k].start > start) start = o[k].start;
            }
            for (k = 0; k < 3; k++) {
                orbit4_limit(args, (int)net->relu, start, q, &o[k]);
                if (o[k].grows) tie |= o[k].tie;
                else fixed_loop = 1;
            }
        }
        if (is_e && settled) {
            int ref = -1;

            for (k = 0; k < 3; k++) {
                s32 sg = 1;

                if (!o[k].grows) continue;
                if (ref < 0) {
                    ref = k;
                    continue;
                }
                if (o[k].ok_mu && o[ref].ok_mu
                        && (!radix_compara(o[k].mu, o[ref].mu, pool, &sg)
                        || sg != 0)) {
                    ok_same_limit = 0;
                }
                if (o[k].tie != o[ref].tie) ok_same_limit = 0;
            }
            if (ref < 0) no_growth++;
            if (fixed_loop) reducible[k4[i].cube_only]++;
        }
        for (k = 0; k < 3; k++) orbit4_free(&o[k]);
        if (!settled) settled_all = 0;
        else {
            qcount[k4[i].cls][q == 1 ? 0 : q == 2 ? 1 : 2]++;
            if (is_e && q <= 2) {
                tab[k4[i].cube_only][q - 1][tie]++;
            }
        }
        free(args);
        piscina_reficere(pool, mark);
    }
    check("B_4: every orbit settles within the run (as in D128)",
        settled_all);
    {
        /* D128 Part D: full (length 6) and quick (length 4) runs */
        int e6[3] = { 3630, 384, 0 }, z6[3] = { 488, 200, 248 };
        int e4[3] = { 222, 32, 0 }, z4[3] = { 92, 26, 64 };
        int c6[2][2] = { { 2950, 56 }, { 680, 328 } };
        int c4[2][2] = { { 186, 0 }, { 36, 32 } };
        int *e = len4 == 6 ? e6 : e4, *z = len4 == 6 ? z6 : z4;
        int ok = len4 == 6 || len4 == 4;
        int t;

        for (t = 0; t < 3; t++) {
            if (qcount[CLS_E][t] != e[t] || qcount[CLS_Z][t] != z[t]) ok = 0;
        }
        for (c = 0; c < 2; c++) {
            for (t = 0; t < 2; t++) {
                int have = tab[c][t][0] + tab[c][t][1];

                if (have != (len4 == 6 ? c6[c][t] : c4[c][t])) ok = 0;
            }
        }
        check("B_4 periods per braid = D128 Part D (E and Z by q, E by "
            "certificate and q)", ok);
    }
    check("E tails: the minimal polynomial holds on the whole tail, its "
        "largest real root\n        mu is > 1 and simple", ok_recurrence
        && ok_mu_all);
    check("theorem: every E orbit of period >= 2 has a tie (continuity at "
        "the limit)", ok_q_tie);
    check("theorem, per argument: a bit that varies on the settled tail "
        "has a vanishing argument", ok_varies_vanish);
    check("vanishing does not depend on the position in the cycle (one "
        "limit direction)", ok_vanish_pos);
    check("the growing loops of an E braid reach the same limit (same mu "
        "= lambda^Q over the\n        braid's period Q, same tie)",
        ok_same_limit);
    check("every E braid has a growing loop", no_growth == 0);
    check("a loop that does not grow is fixed on its tail (polynomial "
        "t - 1)", ok_invariant);
    printf("    E braids with an INVARIANT loop (orbit does not grow: "
        "reducible, a pseudo-Anosov\n    piece): %d certified at t = -1, "
        "%d only at a cube root (left out of the limit)\n", reducible[0],
        reducible[1]);
    printf("\n    %-32s %10s %10s %10s %10s\n", "E braids", "q=1 no tie",
        "q=1 tie", "q=2 tie", "q=2 no tie");
    for (c = 0; c < 2; c++) {
        printf("    %-32s %10d %10d %10d %10d\n", c == 0
            ? "Burau(-1) radius > 1" : "only at a cube root of unity",
            tab[c][0][0], tab[c][0][1], tab[c][1][1], tab[c][1][0]);
    }
    printf("    registered prediction (period 2 iff tie): %s\n",
        tab[0][0][1] + tab[1][0][1] == 0 && tab[0][1][0] + tab[1][1][0] == 0
        ? "HOLDS" : "FAILS (q = 1 braids with a tie, or q = 2 without)");
}

static void
part_c (void)
{
    int len4 = celer ? 4 : 6;
    int lat = celer ? 3 : 4;

    printf("\n=== Part C: B_4 ===\n");
    build_census4(len4);
    if (celer) {
        shells4[0] = 2;
        shells4[1] = 4;
    }
    part_c_lattice(lat, 2);
    part_c_period(len4);
}

/* ================================================================
 * Part D: nulls (B_3, exact)
 * ================================================================
 *
 * (a) a LINEAR network (Burau(-1) of the word, two affine instructions):
 *     one region by construction - the floor;
 * (b) the census word's own network with each instruction's
 *     coefficients permuted at random (same sources, same terms, same
 *     depth and ReLU count);
 * (c) the census word's letters in random order: a different braid,
 *     whose type is looked up in the census. */

#define D_REGION_CAP 20000

static Sors srng;
static int  shuffled_type_ok = 1;

static int null_changed = 0;      /* permuted networks that differ */

static Nervus *
shuffled_net (
    const Nervus *net)
{
    int changed = 0;

    Nervus *a;
    int     t;

    if (!nervus_creare(net->introitus, pool, &a)) fatal("net refused");
    for (t = 0; t < (int)net->numerus; t++) {
        const NervusInstructio *in = &net->instructiones[t];
        Magnus                  c[16];
        s32                     reg;
        int                     j;

        if (in->termini > 16) fatal("too many terms");
        for (j = 0; j < (int)in->termini; j++) c[j] = in->coefficientes[j];
        for (j = (int)in->termini - 1; j > 0; j--) {
            int    r = (int)sors_intra(&srng, (i32)(j + 1));
            Magnus tmp = c[j];

            c[j] = c[r];
            c[r] = tmp;
        }
        for (j = 0; j < (int)in->termini; j++) {
            if (!magnus_aequalis(c[j], in->coefficientes[j])) changed = 1;
        }
        if (!(in->genus == NERVUS_RELU ? nervus_relu : nervus_affinis)(a,
                in->termini, in->fontes, c, in->addendum, &reg)) {
            fatal("copy refused");
        }
    }
    if (!nervus_exitus(a, net->exitus_numerus, net->exitus)) {
        fatal("exit refused");
    }
    null_changed += changed;
    return a;
}

/* regions and pieces of the iterates of one 2 -> 2 network; returns
 * the last m reached (stops past D_REGION_CAP) */
static int
sweep_net (
    Nervus *one,
    int    *r,
    int    *p)
{
    PiscinaNotatio mark = piscina_notare(pool);
    Nervus        *net = one;
    int            m;
    int            last = 0;

    for (m = 1; m <= MMAX; m++) {
        NervusPlanum pl;

        if (m > 1 && !nervus_componere(net, one, pool, &net)) {
            fatal("composition refused");
        }
        if (!nervus_regiones_plani(net, pool, &pl)) fatal("sweep refused");
        r[m] = (int)pl.numerus;
        p[m] = (int)pl.frusta;
        last = m;
        if (r[m] > D_REGION_CAP) break;
    }
    piscina_reficere(pool, mark);
    return last;
}

static void
part_d (void)
{
    int  i, c, m;
    int  floor_ok = 1;
    int  shape_ok = 1;
    long sr[3][MMAX + 1], sp[3][MMAX + 1];
    int  n[3] = { 0, 0, 0 }, lin[3] = { 0, 0, 0 }, ex[3] = { 0, 0, 0 };
    int  capped[3] = { 0, 0, 0 };
    int  trans[3][3];
    long wr[3][MMAX + 1];
    int  wn[3] = { 0, 0, 0 }, wlin[3] = { 0, 0, 0 }, wex[3] = { 0, 0, 0 };
    char b1[32], b2[32];
    TabulaDispersa *index = tabula_dispersa_creare_chorda(pool, 4096);
    int            *slot = (int *)malloc((size_t)nk * sizeof(int));

    printf("\n=== Part D: nulls on B_3 (exact) ===\n");
    sors_seminare(&srng, 129, 1);
    memset(sr, 0, sizeof(sr));
    memset(sp, 0, sizeof(sp));
    memset(trans, 0, sizeof(trans));
    memset(wr, 0, sizeof(wr));
    for (i = 0; i < nk; i++) {
        chorda key;

        slot[i] = i;
        plexus_clavis(ks[i].word, pool, &key);
        tabula_dispersa_inserere(index, key, &slot[i]);
    }
    for (i = 0; i < nk; i++) {
        PiscinaNotatio mark = piscina_notare(pool);
        Nervus        *one;
        Nervus        *sh;
        NervusFigura   f1, f2;
        int            r[MMAX + 1], p[MMAX + 1];
        int            last;

        c = ks[i].cls;
        /* (a) the linear floor: Burau(-1) as two affine instructions */
        {
            Matrix  bm;
            Nervus *lin_net;
            s32     f[2] = { 0, 1 };
            s32     out[2];
            Magnus  row[2];
            NervusPlanum pl;
            int     t;

            if (!plexus_burau_minus_uno(ks[i].word, pool, &bm)
                    || !nervus_creare(2, pool, &lin_net)) {
                fatal("linear null refused");
            }
            for (t = 0; t < 2; t++) {
                row[0] = *(const Magnus *)matrix_elementum(bm, (i32)t, 0);
                row[1] = *(const Magnus *)matrix_elementum(bm, (i32)t, 1);
                if (!nervus_affinis(lin_net, 2, f, row, magnus_ex_s64(0),
                        &out[t])) fatal("affine refused");
            }
            nervus_exitus(lin_net, 2, out);
            if (!nervus_regiones_plani(lin_net, pool, &pl)
                    || pl.numerus != 1 || pl.frusta != 1) {
                floor_ok = 0;
            }
        }
        /* (b) coefficients permuted */
        if (!plexus_nervus(ks[i].word, pool, &one)) fatal("net refused");
        sh = shuffled_net(one);
        if (!nervus_figura(one, &f1) || !nervus_figura(sh, &f2)
                || f1.relu != f2.relu || f1.profunditas != f2.profunditas
                || f1.profunditas_relu != f2.profunditas_relu
                || f1.termini != f2.termini) {
            shape_ok = 0;
        }
        last = sweep_net(sh, r, p);
        n[c]++;
        if (last < MMAX) {
            capped[c]++;
        } else {
            for (m = 1; m <= MMAX; m++) {
                sr[c][m] += r[m];
                sp[c][m] += p[m];
            }
            lin[c] += h_lin(r);
            ex[c] += h_exp(r);
        }
        piscina_reficere(pool, mark);
        /* (c) letters shuffled */
        {
            Plexus  w = ks[i].word;
            s32     lt[16];
            Plexus  ws;
            chorda  key;
            vacuum *val = NULL;
            int     j, nc;
            Nervus *wn_net;

            mark = piscina_notare(pool);
            for (j = 0; j < (int)w.longitudo; j++) lt[j] = w.litterae[j];
            for (j = (int)w.longitudo - 1; j > 0; j--) {
                int r2 = (int)sors_intra(&srng, (i32)(j + 1));
                s32 tmp = lt[j];

                lt[j] = lt[r2];
                lt[r2] = tmp;
            }
            plexus_creare(3, lt, w.longitudo, pool, &ws);
            plexus_clavis(ws, pool, &key);
            if (!tabula_dispersa_invenire(index, key, &val)) {
                fatal("shuffled word has no class");
            }
            nc = ks[*(int *)val].cls;
            {
                /* the type straight from the word (period, trace) */
                i32  period = 0;
                long tr = burau_trace(ws);
                long at = tr < 0 ? -tr : tr;
                int  direct;

                if (!plexus_periodus(ws, pool, &period)) {
                    fatal("period refused");
                }
                direct = period > 0 ? CLS_P : at > 2 ? CLS_E : CLS_Z;
                if (direct != nc) shuffled_type_ok = 0;
            }
            trans[c][nc]++;
            if (!plexus_nervus(ws, pool, &wn_net)) fatal("net refused");
            (void)sweep_net(wn_net, r, p);
            wn[nc]++;
            for (m = 1; m <= MMAX; m++) wr[nc][m] += r[m];
            wlin[nc] += h_lin(r);
            wex[nc] += h_exp(r);
            piscina_reficere(pool, mark);
        }
    }
    check("(a) a linear network has one region and one piece (the floor)",
        floor_ok);
    check("(b) permuted coefficients keep the shape (ReLUs, depth, ReLU "
        "depth, terms)", shape_ok);
    printf("  (b) permuted networks that differ from the original: %d of "
        "%d\n", null_changed, nk);
    check("(b) the permutation changes most networks", 2 * null_changed
        > nk);
    check("(c) each shuffled word's type (census lookup) = its own period "
        "and trace", shuffled_type_ok);
    printf("\n  (b) Permuted coefficients, by the original braid's type "
        "(mean over classes;\n  networks past %d regions are stopped and "
        "counted apart):\n", D_REGION_CAP);
    printf("    %4s %8s %7s %9s %9s %9s %9s %6s %6s\n", "type", "classes",
        "capped", "r(1)", "r(6)", "r(12)", "p(12)", "lin", "exp");
    for (c = 0; c < 3; c++) {
        int k = n[c] - capped[c];
        char b3[32], b4[32];

        mean_text(sr[c][1], k, b1);
        mean_text(sr[c][6], k, b2);
        mean_text(sr[c][MMAX], k, b3);
        mean_text(sp[c][MMAX], k, b4);
        printf("    %4s %8d %7d %9s %9s %9s %9s %6d %6d\n", cls_name[c],
            n[c], capped[c], b1, b2, b3, b4, lin[c], ex[c]);
    }
    printf("\n  (c) Letters shuffled: type of the new braid (rows: "
        "original, columns: new):\n");
    printf("    %4s %6s %6s %6s\n", "", "P", "E", "Z");
    for (c = 0; c < 3; c++) {
        printf("    %4s %6d %6d %6d\n", cls_name[c], trans[c][0],
            trans[c][1], trans[c][2]);
    }
    printf("    by the NEW type: mean r(6), r(12), and the rule\n");
    printf("    %4s %8s %9s %9s %6s %6s\n", "type", "words", "r(6)",
        "r(12)", "lin", "exp");
    for (c = 0; c < 3; c++) {
        mean_text(wr[c][6], wn[c], b1);
        mean_text(wr[c][MMAX], wn[c], b2);
        printf("    %4s %8d %9s %9s %6d %6d\n", cls_name[c], wn[c], b1, b2,
            wlin[c], wex[c]);
    }
    free(slot);
}

/* ================================================================
 * main
 * ================================================================ */

int
main (void)
{
    const char *q = getenv("D129_CELER");

    pool = piscina_generare_dynamicum("demo129", 1 << 20);
    if (q && q[0] == '1') {
        celer = 1;
        maxlen = 5;
        nshells = 2;
    }
    printf("KNOTAPEL DEMO 129: linear regions of braid networks%s\n",
        celer ? " (CELER)" : "");
    part_a();
    part_b();
    part_c();
    part_d();
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    return n_fail ? 1 : 0;
}
