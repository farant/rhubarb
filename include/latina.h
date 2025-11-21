#ifndef LATINA_H
#define LATINA_H

#include <stddef.h>

#define character 	char
#define brevis 			short
#define integer 		int
#define longus			long
#define fluitans		float
#define duplex			double

#define vacuum			void
#define signatus 		signed
#define insignatus  unsigned
#define constans		const
#define volatilis		volatile
#define sponte			auto
#define registrum 	register
#define staticus 		static
#define	externus 		extern

#define si					if
#define alioquin		else
#define commutatio	switch
#define casus				case
#define ordinarius	default
#define per					for
#define dum 				while
#define fac 				do
#define frange 			break
#define perge 			continue
#define salta				goto
#define redde				return

#define structura		struct
#define unio 				union	
#define enumeratio 	enum
#define nomen 			typedef

#define magnitudo 	sizeof

#define principale 	main

#define NIHIL				NULL
#define VERUM 			1
#define FALSUM 			0

#define ZEPHYRUM    0
#define I 					1
#define II					2
#define III					3
#define IV					4
#define V						5
#define VI					6
#define VII					7
#define VIII				8
#define IX					9
#define X						10
#define XI					11
#define XII					12
#define XIII				13
#define XIV					14
#define XV					15
#define XVI					16
#define XVII				17
#define XVIII				18
#define XIX					19
#define XX					20
#define XXI					21
#define XXII				22
#define XXIII				23
#define XXIV				24
#define XXV					25
#define XXVI				26
#define XXVII				27
#define XXVIII			28
#define XXIX				29
#define XXX					30
#define XXXI				31
#define XXXII				32
#define XXXIII			33
#define XXXIV				34
#define XXXV				35
#define XXXVI				36
#define XXXVII			37
#define XXXVIII			38
#define XXXIX				39
#define XL					40
#define XLI					41
#define XLII				42
#define XLIII				43
#define XLIV				44
#define XLV					45
#define XLVI				46
#define XLVII				47
#define XLVIII			48
#define XLIX				49
#define L						50
#define LI					51
#define LII					52
#define LIII				53
#define LIV					54
#define LV					55
#define LVI					56
#define LVII				57
#define LVIII				58
#define LIX					59
#define LX					60
#define LXI					61
#define LXII				62
#define LXIII				63
#define LXIV				64
#define LXX					70
#define LXXI				71
#define LXXX				80
#define XC					90
#define XCI					91
#define XCII				92
#define XCIII				93
#define XCIV				94
#define XCV					95
#define XCVI				96
#define XCVII				97
#define XCVIII			98
#define XCIX				99
#define C						100
#define CI 					101
#define CII 				102
#define CIII				103
#define CIV					104
#define CV					105
#define CVI					106
#define CVII				107
#define CVIII				108
#define CIX					109
#define CX					110
#define CXI					111
#define CXII				112
#define CXIII				113
#define CXIV				114
#define CXV					115
#define CXVI				116
#define CXVII				117
#define CXVIII			118
#define CXIX				119
#define CXX 				120
#define CXXI				121
#define CXXII 			122
#define CXXIII			123
#define CXXIV 			124
#define CXXV 				125
#define CXXVI 			126
#define CXXVII 			127
#define CXXVIII 		128
#define CXXIX 			129
#define CXXX 				130
#define CXXXI 			131
#define CXXXII 			132
#define CXXXIII			133
#define CXXXIV 			134
#define CXXXV 			135
#define CXXXVI 			136
#define CXXXVII 		137
#define CXXXVIII 		138
#define CXXXIX 			139
#define CXL 				140
#define CXLI 				141
#define CXLII 			142
#define CXLIII			143
#define CXLIV 			144
#define CXLV 				145
#define CXLVI 			146
#define CXLVII 			147
#define CXLVIII 		148
#define CXLIX 			149
#define CL					150
#define CLI 				151
#define CLII 				152
#define CLIII 			153
#define CLIV 				154
#define CLV 				155
#define CLVI 				156
#define CLX					160
#define CLXV				165
#define CLXX				170
#define CLXXX				180
#define CXC					190
#define CXCI 				191
#define CXCII 			192
#define CC					200
#define CCX					210
#define CCXX				220
#define CCXXX				230
#define CCXXXI			231
#define CCXXXII			232
#define CCXXXIII		233
#define CCXXXIV			234
#define CCXXXV			235
#define CCXXXVI			236
#define CCXXXVII		237
#define CCXXXVIII		238
#define CCXXXIX			239
#define CCXL				240
#define CCL					250
#define CCLI 				251
#define CCLII 			252
#define CCLIII 			253
#define CCLIV 			254
#define CCLV 				255
#define CCLVI 			256
#define CCLX				260
#define CCLXX				270
#define CCLXXX			280
#define CCXC				290
#define CCC				  300
#define CCCX				310
#define CCCXX				320
#define CCCXXX			330
#define CCCXL				340
#define CD					400
#define CDX					410
#define CDXI					411
#define CDXII					412
#define CDXIII				413
#define CDXIV				414
#define CDXV				415
#define CDXVI				416
#define CDXVII			417
#define CDXVIII			418
#define CDXIX				419
#define CDXX 				420
#define CDXXX 			430
#define CDXL 				440
#define CDL 				450
#define CDLI 				451
#define CDLII 			452
#define CDLIII 			453
#define CDLIV 			454
#define CDLV 				455
#define CDLVI 			456
#define CDLVII			457
#define CDLVIII			458
#define CDLIX				459
#define CDLX 				460
#define CDLXX				470
#define CDLXXX			480
#define D						500
#define DX 					510
#define DXI 				511
#define DXII 				512
#define DC					600
#define DCC					700
#define DCCL				750
#define DCCC				800
#define DCCCLIII		853
#define CM					900
#define M						1000
#define MCMX 			  1900
#define MM					2000
#define MMM					3000
#define MMMM				4000
#define MMMMXCVI		4096

#define imprimere 	printf
#define liberare 		free
#define memoriae_allocare	malloc
#define exire				exit

#define interior 		static 
#define hic_manens 	static
#define universalis static

#define FILE FILUM

nomen insignatus character	i8;
nomen insignatus brevis 		i16;
nomen insignatus integer   	i32;
nomen insignatus longus longus	i64;

nomen signatus character    s8;
nomen signatus brevis   	 	s16;
nomen signatus integer     	s32;
nomen signatus longus longus	s64;

nomen fluitans          		f32;
nomen duplex         				f64;

nomen integer            		b32;

nomen size_t 								memoriae_index;

#endif /* LATINA_H */

