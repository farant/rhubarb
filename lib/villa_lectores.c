/* villa_lectores.c - Lectores villae (implementatio)
 *
 * Vide villa_lectores.h pro contractu et pro insidiis quas fixa
 * vera revelaverunt. Nihil hic rete tangit.
 *
 * FORMA INTERNA: adiutores privati VISTAS in effusionem intrantem
 * reddunt (nulla allocatio dum parsatur); functiones publicae
 * TRANSCRIBUNT in piscinam ad momentum quo structura conditur.
 * Ergo parsura celeris est et fructus per tictus supervivit.
 */

#include "villa_lectores.h"
#include <stdio.h>
#include <string.h>

/* ========================================================================
 * ADIUTORES PRIVATI
 * ======================================================================== */

/* litterae -> chorda sine copia (unio contra -Wcast-qual) */
interior chorda
_ch (constans character* litterae)
{
	chorda c;
	unio { constans character* l; i8* m; } u;

	u.l = litterae;
	c.datum   = u.m;
	c.mensura = (i32)strlen(litterae);
	redde c;
}

interior chorda
_vacua (vacuum)
{
	chorda c;

	c.datum   = NIHIL;
	c.mensura = ZEPHYRUM;
	redde c;
}

interior b32
_album (i8 c)
{
	redde (c == (i8)' ' || c == (i8)'\t'
		|| c == (i8)'\r' || c == (i8)'\n');
}

/* chordam in piscinam transcribere (vacuas tuto tractat) */
interior chorda
_copia (chorda s, Piscina* piscina)
{
	chorda fructus;

	si (s.mensura == ZEPHYRUM || s.datum == NIHIL)
	{
		redde _vacua();
	}
	fructus.datum = (i8*)piscina_allocare(piscina,
		(memoriae_index)s.mensura);
	si (fructus.datum == NIHIL)
	{
		redde _vacua();
	}
	memcpy(fructus.datum, s.datum, (memoriae_index)s.mensura);
	fructus.mensura = s.mensura;
	redde fructus;
}

/* lineam proximam sumere; positus ultra '\n' provehitur */
interior b32
_linea_proxima (chorda totum, i32* positus, chorda* linea)
{
	i32 initium;
	i32 i;

	si (*positus >= totum.mensura)
	{
		redde FALSUM;
	}
	initium = *positus;
	i       = initium;
	dum (i < totum.mensura && totum.datum[i] != (i8)'\n')
	{
		i++;
	}
	*linea   = chorda_sectio(totum, initium, i);
	*positus = (i < totum.mensura) ? i + I : totum.mensura;
	redde VERUM;
}

/* verbum n-tum (spatio albo divisum) ut vista */
interior chorda
_verbum (chorda c, i32 index)
{
	i32 i = ZEPHYRUM;
	i32 n = ZEPHYRUM;

	dum (i < c.mensura)
	{
		dum (i < c.mensura && _album(c.datum[i]))
		{
			i++;
		}
		si (i >= c.mensura)
		{
			frange;
		}
		{
			i32 initium = i;

			dum (i < c.mensura && !_album(c.datum[i]))
			{
				i++;
			}
			si (n == index)
			{
				redde chorda_sectio(c, initium, i);
			}
			n++;
		}
	}
	redde _vacua();
}

/* reliquum a verbo n-to ad finem (ultima columna spatia fert) */
interior chorda
_reliquum_a_verbo (chorda c, i32 index)
{
	i32 i = ZEPHYRUM;
	i32 n = ZEPHYRUM;

	dum (i < c.mensura)
	{
		dum (i < c.mensura && _album(c.datum[i]))
		{
			i++;
		}
		si (i >= c.mensura)
		{
			frange;
		}
		si (n == index)
		{
			redde chorda_sectio(c, i, c.mensura);
		}
		dum (i < c.mensura && !_album(c.datum[i]))
		{
			i++;
		}
		n++;
	}
	redde _vacua();
}

interior i32
_numerus_verborum (chorda c)
{
	i32 i = ZEPHYRUM;
	i32 n = ZEPHYRUM;

	dum (i < c.mensura)
	{
		dum (i < c.mensura && _album(c.datum[i]))
		{
			i++;
		}
		si (i >= c.mensura)
		{
			frange;
		}
		dum (i < c.mensura && !_album(c.datum[i]))
		{
			i++;
		}
		n++;
	}
	redde n;
}

/* numerum integrum initialem legere ('27223431.74' -> 27223431) */
interior b32
_ut_i64 (chorda c, i64* fructus)
{
	i32 i       = ZEPHYRUM;
	i64 valor   = ZEPHYRUM;
	b32 aliquid = FALSUM;

	dum (i < c.mensura && _album(c.datum[i]))
	{
		i++;
	}
	dum (i < c.mensura
		&& c.datum[i] >= (i8)'0' && c.datum[i] <= (i8)'9')
	{
		valor   = valor * (i64)X + (i64)(c.datum[i] - (i8)'0');
		aliquid = VERUM;
		i++;
	}
	si (!aliquid)
	{
		redde FALSUM;
	}
	*fructus = valor;
	redde VERUM;
}

interior b32
_ut_i32 (chorda c, i32* fructus)
{
	i64 valor;

	si (!_ut_i64(c, &valor))
	{
		redde FALSUM;
	}
	*fructus = (i32)valor;
	redde VERUM;
}


/* ========================================================================
 * I. AEDIFICATOR MANDATI SSH
 * ======================================================================== */

constans character*
villa_ssh_error_nomen (VillaSshError error)
{
	commutatio (error)
	{
	casus VILLA_SSH_OK:
		redde "OK";
	casus VILLA_SSH_SINE_DESTINATIONE:
		redde "sine destinatione";
	casus VILLA_SSH_SINE_IMPERIO:
		redde "sine imperio";
	casus VILLA_SSH_VIA_LONGA:
		redde "via moderandi longa";
	casus VILLA_SSH_NIMIS_ARGUMENTORUM:
		redde "nimis argumentorum";
	ordinarius:
		redde "ignotus";
	}
}

interior vacuum
_arg (constans character** vec, i32* n, b32* fractum,
	constans character* valor)
{
	si (*n >= VILLA_SSH_ARGUMENTA_MAXIMA - I)
	{
		*fractum = VERUM;
		redde;
	}
	vec[*n] = valor;
	(*n)++;
}

interior constans character*
_optio (Piscina* piscina, constans character* clavis,
	constans character* valor)
{
	character* b = (character*)piscina_allocare(piscina,
		strlen(clavis) + strlen(valor) + (memoriae_index)II);

	si (b == NIHIL)
	{
		redde "";
	}
	sprintf(b, "%s=%s", clavis, valor);
	redde b;
}

interior constans character*
_optio_numeri (Piscina* piscina, constans character* clavis,
	i32 valor)
{
	character* b = (character*)piscina_allocare(piscina,
		strlen(clavis) + (memoriae_index)XXXII);

	si (b == NIHIL)
	{
		redde "";
	}
	sprintf(b, "%s=%lu", clavis, (insignatus longus)valor);
	redde b;
}

interior MandatumSsh
_ssh_recusare (VillaSshError error, constans character* causa,
	Piscina* piscina)
{
	MandatumSsh m;

	m.successus  = FALSUM;
	m.error      = error;
	m.causa      = _copia(_ch(causa), piscina);
	m.argumenta  = NIHIL;
	m.numerus    = ZEPHYRUM;
	redde m;
}

MandatumSsh
villa_mandatum_ssh (constans ConfiguratioSsh* configuratio,
	chorda imperium, Piscina* piscina)
{
	MandatumSsh          m;
	constans character** vec;
	constans character*  destinatio;
	constans character*  via_ssh;
	i32                  n       = ZEPHYRUM;
	b32                  fractum = FALSUM;
	i32                  mora;
	i32                  perseverantia;

	si (configuratio == NIHIL)
	{
		redde _ssh_recusare(VILLA_SSH_SINE_DESTINATIONE,
			"configuratio NIHIL", piscina);
	}
	si (chorda_vacua(configuratio->alias)
		&& chorda_vacua(configuratio->hospes))
	{
		redde _ssh_recusare(VILLA_SSH_SINE_DESTINATIONE,
			"nec alias nec hospes datus - villa destinationem"
			" fingere non debet", piscina);
	}
	si (chorda_vacua(imperium))
	{
		redde _ssh_recusare(VILLA_SSH_SINE_IMPERIO,
			"imperium remotum vacuum", piscina);
	}

	/* PORTA VIAE MODERANDI: ssh '%C' in XL characteres expandit.
	 * Supra terminum socketus ligari non potest et ssh id TACITE
	 * praeterit (nuntius solum sub '-v'), ergo omne imperium ad
	 * manum plenam degenerat et nemo scit cur villa lenta sit.
	 * Recusatio clara mendacio lento praestat. */
	si (!chorda_vacua(configuratio->via_moderandi))
	{
		i32 expansa = configuratio->via_moderandi.mensura + I
			+ (i32)VILLA_MODERANDI_SIGILLUM;

		si (expansa > (i32)VILLA_MODERANDI_TERMINUS)
		{
			character* nuntius = (character*)piscina_allocare(
				piscina,
				(memoriae_index)(configuratio->via_moderandi.mensura
					+ (i32)CXXVIII));

			si (nuntius == NIHIL)
			{
				redde _ssh_recusare(VILLA_SSH_VIA_LONGA,
					"via moderandi longa", piscina);
			}
			sprintf(nuntius,
				"via moderandi expansa %lu octetos habet"
				" (terminus %lu) - socketus ligari non potest:"
				" %.*s/%%C",
				(insignatus longus)expansa,
				(insignatus longus)VILLA_MODERANDI_TERMINUS,
				(integer)configuratio->via_moderandi.mensura,
				(constans character*)
					configuratio->via_moderandi.datum);
			redde _ssh_recusare(VILLA_SSH_VIA_LONGA, nuntius,
				piscina);
		}
	}

	vec = (constans character**)piscina_allocare(piscina,
		magnitudo(constans character*)
			* (memoriae_index)VILLA_SSH_ARGUMENTA_MAXIMA);
	si (vec == NIHIL)
	{
		redde _ssh_recusare(VILLA_SSH_NIMIS_ARGUMENTORUM,
			"piscina vectorem negavit", piscina);
	}

	via_ssh = (configuratio->via_ssh != NIHIL)
		? configuratio->via_ssh : "ssh";
	mora = (configuratio->mora_conexionis != ZEPHYRUM)
		? configuratio->mora_conexionis : (i32)V;
	perseverantia = (configuratio->perseverantia != ZEPHYRUM)
		? configuratio->perseverantia : (i32)CCC;

	_arg(vec, &n, &fractum, via_ssh);

	/* BatchMode NON NEGOTIABILE: rogatio interactiva (tessera,
	 * confirmatio hospitis novi) faciem congelaret sine termino. */
	_arg(vec, &n, &fractum, "-o");
	_arg(vec, &n, &fractum, "BatchMode=yes");
	_arg(vec, &n, &fractum, "-o");
	_arg(vec, &n, &fractum,
		_optio_numeri(piscina, "ConnectTimeout", mora));

	si (!chorda_vacua(configuratio->via_moderandi))
	{
		character* via_cum_sigillo;
		i32        m_via = configuratio->via_moderandi.mensura;

		via_cum_sigillo = (character*)piscina_allocare(piscina,
			(memoriae_index)(m_via + (i32)XVI));
		si (via_cum_sigillo != NIHIL)
		{
			sprintf(via_cum_sigillo, "%.*s/%%C", (integer)m_via,
				(constans character*)
					configuratio->via_moderandi.datum);
			_arg(vec, &n, &fractum, "-o");
			_arg(vec, &n, &fractum, "ControlMaster=auto");
			_arg(vec, &n, &fractum, "-o");
			_arg(vec, &n, &fractum,
				_optio(piscina, "ControlPath", via_cum_sigillo));
			_arg(vec, &n, &fractum, "-o");
			_arg(vec, &n, &fractum,
				_optio_numeri(piscina, "ControlPersist",
					perseverantia));
		}
	}

	si (configuratio->portus != ZEPHYRUM)
	{
		character* p = (character*)piscina_allocare(piscina,
			(memoriae_index)XXXII);

		si (p != NIHIL)
		{
			sprintf(p, "%lu",
				(insignatus longus)configuratio->portus);
			_arg(vec, &n, &fractum, "-p");
			_arg(vec, &n, &fractum, p);
		}
	}
	si (!chorda_vacua(configuratio->clavis))
	{
		_arg(vec, &n, &fractum, "-i");
		_arg(vec, &n, &fractum,
			chorda_ut_cstr(configuratio->clavis, piscina));
	}

	/* destinatio: alias praevalet (ssh reliqua omnia resolvit) */
	si (!chorda_vacua(configuratio->alias))
	{
		destinatio = chorda_ut_cstr(configuratio->alias, piscina);
	}
	alioquin si (!chorda_vacua(configuratio->usor))
	{
		character* b = (character*)piscina_allocare(piscina,
			(memoriae_index)(configuratio->usor.mensura
				+ configuratio->hospes.mensura + (i32)II));

		si (b == NIHIL)
		{
			redde _ssh_recusare(VILLA_SSH_SINE_DESTINATIONE,
				"piscina destinationem negavit", piscina);
		}
		sprintf(b, "%.*s@%.*s",
			(integer)configuratio->usor.mensura,
			(constans character*)configuratio->usor.datum,
			(integer)configuratio->hospes.mensura,
			(constans character*)configuratio->hospes.datum);
		destinatio = b;
	}
	alioquin
	{
		destinatio = chorda_ut_cstr(configuratio->hospes, piscina);
	}
	_arg(vec, &n, &fractum, destinatio);

	/* imperium ut ELEMENTUM UNUM - nulla concha id findit, ergo
	 * spatia, virgulae, lineae novae in eo tuta sunt */
	_arg(vec, &n, &fractum, chorda_ut_cstr(imperium, piscina));

	si (fractum)
	{
		redde _ssh_recusare(VILLA_SSH_NIMIS_ARGUMENTORUM,
			"vector argumentorum excessus", piscina);
	}
	vec[n] = NIHIL;

	m.successus = VERUM;
	m.error     = VILLA_SSH_OK;
	m.causa     = _vacua();
	m.argumenta = vec;
	m.numerus   = n;
	redde m;
}


/* ========================================================================
 * II. SYSTEMCTL SHOW
 * ======================================================================== */

interior vacuum
_servitium_condere (Xar* fructus, constans StatusServitii* fons,
	Piscina* piscina)
{
	StatusServitii* s = (StatusServitii*)xar_addere(fructus);

	si (s == NIHIL)
	{
		redde;
	}
	s->id              = _copia(fons->id, piscina);
	s->descriptio      = _copia(fons->descriptio, piscina);
	s->status_oneris   = _copia(fons->status_oneris, piscina);
	s->status_vitae    = _copia(fons->status_vitae, piscina);
	s->sub_status      = _copia(fons->sub_status, piscina);
	s->status_plagulae = _copia(fons->status_plagulae, piscina);
	s->tempus_initii   = _copia(fons->tempus_initii, piscina);
	s->causa_finis     = _copia(fons->causa_finis, piscina);
	s->codex_exitus    = fons->codex_exitus;
	s->pid             = fons->pid;
	s->restitutiones   = fons->restitutiones;

	/* iudicia derivata semel hic, ne quisque consumptor chordas
	 * iterum comparet (et alius aliter) */
	s->inventa = (b32)(!chorda_vacua(s->status_oneris)
		&& !chorda_aequalis_literis(s->status_oneris, "not-found"));
	s->currit = chorda_aequalis_literis(s->status_vitae, "active");
	/* 'fracta' portat iudicium quod causa_finis SOLA non portat:
	 * systemd 'Result=success' etiam mortuis et ignotis reddit */
	s->fracta = chorda_aequalis_literis(s->status_vitae, "failed");
}

Xar*
villa_systemctl_legere (chorda effusio, Piscina* piscina)
{
	Xar*           fructus;
	StatusServitii currens;
	chorda         linea;
	i32            positus = ZEPHYRUM;
	b32            aliquid = FALSUM;

	fructus = xar_creare(piscina, (i32)magnitudo(StatusServitii));
	si (fructus == NIHIL)
	{
		redde NIHIL;
	}
	memset(&currens, 0, magnitudo(StatusServitii));

	dum (_linea_proxima(effusio, &positus, &linea))
	{
		chorda nuda = chorda_praecidere(linea);
		chorda clavis;
		chorda valor;
		s32    index_aequalis;

		/* linea vacua = terminus recordi (unitates plures uno
		 * vocamine sic separantur) */
		si (nuda.mensura == ZEPHYRUM)
		{
			si (aliquid)
			{
				_servitium_condere(fructus, &currens, piscina);
				memset(&currens, 0, magnitudo(StatusServitii));
				aliquid = FALSUM;
			}
			perge;
		}

		/* findere ad PRIMUM '=' solum: valores '=' continere
		 * possunt (Description praesertim) */
		index_aequalis = chorda_invenire_index(nuda, _ch("="));
		si (index_aequalis < ZEPHYRUM)
		{
			perge;
		}
		clavis = chorda_sectio(nuda, ZEPHYRUM, (i32)index_aequalis);
		valor  = chorda_sectio(nuda, (i32)index_aequalis + I,
			nuda.mensura);
		aliquid = VERUM;

		si (chorda_aequalis_literis(clavis, "Id"))
		{
			currens.id = valor;
		}
		alioquin si (chorda_aequalis_literis(clavis, "Description"))
		{
			currens.descriptio = valor;
		}
		alioquin si (chorda_aequalis_literis(clavis, "LoadState"))
		{
			currens.status_oneris = valor;
		}
		alioquin si (chorda_aequalis_literis(clavis, "ActiveState"))
		{
			currens.status_vitae = valor;
		}
		alioquin si (chorda_aequalis_literis(clavis, "SubState"))
		{
			currens.sub_status = valor;
		}
		alioquin si (chorda_aequalis_literis(clavis,
			"UnitFileState"))
		{
			currens.status_plagulae = valor;
		}
		/* comparatio TOTA: 'ActiveEnterTimestampMonotonic' hanc
		 * non capiat */
		alioquin si (chorda_aequalis_literis(clavis,
			"ActiveEnterTimestamp"))
		{
			currens.tempus_initii = valor;
		}
		alioquin si (chorda_aequalis_literis(clavis, "Result"))
		{
			currens.causa_finis = valor;
		}
		alioquin si (chorda_aequalis_literis(clavis,
			"ExecMainStatus"))
		{
			_ut_i32(valor, &currens.codex_exitus);
		}
		alioquin si (chorda_aequalis_literis(clavis, "MainPID"))
		{
			_ut_i32(valor, &currens.pid);
		}
		alioquin si (chorda_aequalis_literis(clavis, "NRestarts"))
		{
			_ut_i32(valor, &currens.restitutiones);
		}
	}
	si (aliquid)
	{
		_servitium_condere(fructus, &currens, piscina);
	}
	redde fructus;
}


/* ========================================================================
 * III. SYSTEMCTL LIST-UNITS
 * ======================================================================== */

Xar*
villa_unitates_legere (chorda effusio, Piscina* piscina)
{
	Xar*   fructus;
	chorda linea;
	i32    positus       = ZEPHYRUM;
	b32    caput_transiit = FALSUM;

	fructus = xar_creare(piscina, (i32)magnitudo(UnitasCursoria));
	si (fructus == NIHIL)
	{
		redde NIHIL;
	}

	dum (_linea_proxima(effusio, &positus, &linea))
	{
		chorda          nuda = chorda_praecidere(linea);
		UnitasCursoria* u;

		/* LEGENDA sequitur post lineam vacuam - hic desinendum,
		 * alioquin prosa ut unitates legeretur */
		si (nuda.mensura == ZEPHYRUM)
		{
			frange;
		}
		si (!caput_transiit
			&& chorda_aequalis_literis(_verbum(nuda, ZEPHYRUM),
				"UNIT"))
		{
			caput_transiit = VERUM;
			perge;
		}
		si (_numerus_verborum(nuda) < (i32)IV)
		{
			perge;
		}

		u = (UnitasCursoria*)xar_addere(fructus);
		si (u == NIHIL)
		{
			perge;
		}
		u->unitas = _copia(_verbum(nuda, ZEPHYRUM), piscina);
		u->onus   = _copia(_verbum(nuda, I), piscina);
		u->vita   = _copia(_verbum(nuda, II), piscina);
		u->sub    = _copia(_verbum(nuda, III), piscina);
		/* DESCRIPTIO spatia continet: reliquum verbatim */
		u->descriptio = _copia(
			chorda_praecidere(_reliquum_a_verbo(nuda, IV)),
			piscina);
	}
	redde fructus;
}


/* ========================================================================
 * IV. NGINX -T
 * ======================================================================== */

/* commentarium tollere (citationes honorans: '#' intra chordam
 * citatam commentarium NON incipit) */
interior chorda
_commentarium_tollere (chorda linea)
{
	i32 i;
	i32 citatio = ZEPHYRUM;

	per (i = ZEPHYRUM; i < linea.mensura; i++)
	{
		i32 c = (i32)linea.datum[i];

		si (citatio != ZEPHYRUM)
		{
			si (c == citatio)
			{
				citatio = ZEPHYRUM;
			}
			perge;
		}
		si (c == (i32)'\'' || c == (i32)'"')
		{
			citatio = c;
			perge;
		}
		si (c == (i32)'#')
		{
			redde chorda_sectio(linea, ZEPHYRUM, i);
		}
	}
	redde linea;
}

/* pars directivae ante ';' aut '{' */
interior chorda
_ante_terminum (chorda linea)
{
	i32 i;
	i32 citatio = ZEPHYRUM;

	per (i = ZEPHYRUM; i < linea.mensura; i++)
	{
		i32 c = (i32)linea.datum[i];

		si (citatio != ZEPHYRUM)
		{
			si (c == citatio)
			{
				citatio = ZEPHYRUM;
			}
			perge;
		}
		si (c == (i32)'\'' || c == (i32)'"')
		{
			citatio = c;
			perge;
		}
		si (c == (i32)';' || c == (i32)'{')
		{
			redde chorda_sectio(linea, ZEPHYRUM, i);
		}
	}
	redde linea;
}

interior vacuum
_clausulam_vacuare (ClausulaNginx* c)
{
	memset(c, 0, magnitudo(ClausulaNginx));
}

interior vacuum
_directivam_capere (ClausulaNginx* c, chorda caput, chorda directiva)
{
	/* 'server_name', non 'server' - comparatio verbi TOTA;
	 * praefixum 'server_tokens' quoque caperet */
	si (chorda_aequalis_literis(caput, "server_name"))
	{
		chorda reliquum = _reliquum_a_verbo(directiva, I);

		si (!chorda_vacua(reliquum))
		{
			c->hospites = reliquum;
			c->hospes   = _verbum(directiva, I);
		}
	}
	/* 'proxy_pass' intra 'location' vivit - uno unco altius quam
	 * 'server_name'. Ideo profunditate NON discriminamus: quidquid
	 * intra clausulam est, clausulae pertinet. */
	alioquin si (chorda_aequalis_literis(caput, "proxy_pass"))
	{
		/* PRIMUM tenetur, sed OMNIA numerantur - terminus visibilis
		 * potius quam mendacium tacitum */
		c->destinationes++;
		si (chorda_vacua(c->destinatio))
		{
			c->destinatio = _verbum(directiva, I);
		}
	}
	/* 'ssl_certificate_key' hanc NON capiat */
	alioquin si (chorda_aequalis_literis(caput, "ssl_certificate"))
	{
		si (chorda_vacua(c->certificatum))
		{
			c->certificatum = _verbum(directiva, I);
		}
	}
	alioquin si (chorda_aequalis_literis(caput, "root"))
	{
		c->radix = _verbum(directiva, I);
	}
	alioquin si (chorda_aequalis_literis(caput, "listen"))
	{
		i32 i;
		i32 n = _numerus_verborum(directiva);

		per (i = I; i < n; i++)
		{
			si (chorda_aequalis_literis(_verbum(directiva, i),
				"ssl"))
			{
				c->ssl = VERUM;
			}
		}
	}
	alioquin si (chorda_aequalis_literis(caput, "return"))
	{
		chorda codex = _verbum(directiva, I);

		/* 'return 404' Certbot NON redirectio est */
		si (chorda_aequalis_literis(codex, "301")
			|| chorda_aequalis_literis(codex, "302")
			|| chorda_aequalis_literis(codex, "307")
			|| chorda_aequalis_literis(codex, "308"))
		{
			c->redirigit = VERUM;
		}
	}
}

interior vacuum
_clausulam_condere (Xar* fructus, constans ClausulaNginx* fons,
	Piscina* piscina)
{
	ClausulaNginx* c = (ClausulaNginx*)xar_addere(fructus);

	si (c == NIHIL)
	{
		redde;
	}
	c->hospes        = _copia(fons->hospes, piscina);
	c->hospites      = _copia(fons->hospites, piscina);
	c->destinatio    = _copia(fons->destinatio, piscina);
	c->destinationes = fons->destinationes;
	c->certificatum  = _copia(fons->certificatum, piscina);
	c->radix        = _copia(fons->radix, piscina);
	c->plagula      = _copia(fons->plagula, piscina);
	c->linea        = fons->linea;
	c->ssl          = fons->ssl;
	c->redirigit    = fons->redirigit;
}

Xar*
villa_nginx_clausulas_legere (chorda effusio, Piscina* piscina)
{
	Xar*          fructus;
	ClausulaNginx currens;
	chorda        plagula_currens;
	chorda        marca;
	chorda        linea;
	i32           positus               = ZEPHYRUM;
	i32           numerus_lineae        = ZEPHYRUM;
	i32           profunditas           = ZEPHYRUM;
	i32           profunditas_clausulae = ZEPHYRUM;
	b32           in_clausula           = FALSUM;

	fructus = xar_creare(piscina, (i32)magnitudo(ClausulaNginx));
	si (fructus == NIHIL)
	{
		redde NIHIL;
	}
	plagula_currens = _vacua();
	marca           = _ch("# configuration file ");
	_clausulam_vacuare(&currens);

	dum (_linea_proxima(effusio, &positus, &linea))
	{
		chorda nuda;
		chorda directiva;
		chorda caput;
		i32    i;
		i32    citatio = ZEPHYRUM;

		numerus_lineae++;

		/* (I) marca plagulae COMMENTARIUM est - ergo ANTE
		 *     ablationem commentariorum legenda */
		{
			chorda t = chorda_praecidere(linea);

			si (chorda_incipit(t, marca)
				&& chorda_terminatur(t, _ch(":")))
			{
				plagula_currens = chorda_sectio(t, marca.mensura,
					t.mensura - I);
			}
		}

		/* (II) commentaria tollere. Hoc bloca 'server {' IN
		 *      COMMENTARIIS delet (exemplaria mail, hospes
		 *      virtualis exemplaris) quae alioquin numerarentur. */
		nuda = chorda_praecidere(_commentarium_tollere(linea));
		si (nuda.mensura == ZEPHYRUM)
		{
			perge;
		}

		directiva = chorda_praecidere(_ante_terminum(nuda));
		caput     = _verbum(directiva, ZEPHYRUM);

		/* (III) directivas capere ANTE uncos: linea 'server {'
		 *       ipsa nihil capiendum fert, et in_clausula tunc
		 *       adhuc FALSUM est */
		si (in_clausula)
		{
			_directivam_capere(&currens, caput, directiva);
		}

		/* (IV) uncos sinistra ad dextram numerare. 'if (...) {'
		 *      intra blocum profunditatem auget - ergo numerandum
		 *      est, non lineae inspiciendae. */
		per (i = ZEPHYRUM; i < nuda.mensura; i++)
		{
			i32 c = (i32)nuda.datum[i];

			si (citatio != ZEPHYRUM)
			{
				si (c == citatio)
				{
					citatio = ZEPHYRUM;
				}
				perge;
			}
			si (c == (i32)'\'' || c == (i32)'"')
			{
				citatio = c;
				perge;
			}
			si (c == (i32)'{')
			{
				si (!in_clausula
					&& chorda_aequalis_literis(caput, "server"))
				{
					_clausulam_vacuare(&currens);
					currens.plagula       = plagula_currens;
					currens.linea         = numerus_lineae;
					profunditas_clausulae = profunditas;
					in_clausula           = VERUM;
				}
				profunditas++;
			}
			alioquin si (c == (i32)'}')
			{
				si (profunditas > ZEPHYRUM)
				{
					profunditas--;
				}
				si (in_clausula
					&& profunditas == profunditas_clausulae)
				{
					_clausulam_condere(fructus, &currens, piscina);
					in_clausula = FALSUM;
				}
			}
		}
	}
	redde fructus;
}

Xar*
villa_nginx_situs_plicare (constans Xar* clausulae, Piscina* piscina)
{
	Xar* fructus;
	i32  i;
	i32  numerus;

	fructus = xar_creare(piscina, (i32)magnitudo(SitusNginx));
	si (fructus == NIHIL || clausulae == NIHIL)
	{
		redde fructus;
	}
	numerus = xar_numerus(clausulae);

	per (i = ZEPHYRUM; i < numerus; i++)
	{
		constans ClausulaNginx* cl = (constans ClausulaNginx*)
			xar_obtinere(clausulae, i);
		SitusNginx* situs = NIHIL;
		b32         socia;
		i32         j;

		si (cl == NIHIL)
		{
			perge;
		}

		/* SOCIA = blocum quod SOLUM redirigit (Certbot geminum).
		 * Definitio negativa consulto: nulla destinatio, nulla
		 * radix - ergo nihil substantiale amittitur cum plicatur. */
		socia = (b32)(cl->redirigit && chorda_vacua(cl->destinatio)
			&& chorda_vacua(cl->radix));

		per (j = ZEPHYRUM; j < xar_numerus(fructus); j++)
		{
			SitusNginx* s = (SitusNginx*)xar_obtinere(fructus, j);

			si (s != NIHIL && chorda_aequalis(s->hospes, cl->hospes))
			{
				situs = s;
				frange;
			}
		}
		si (situs == NIHIL)
		{
			situs = (SitusNginx*)xar_addere(fructus);
			si (situs == NIHIL)
			{
				perge;
			}
			memset(situs, 0, magnitudo(SitusNginx));
			situs->hospes   = _copia(cl->hospes, piscina);
			situs->hospites = _copia(cl->hospites, piscina);
			situs->plagula  = _copia(cl->plagula, piscina);
		}
		situs->clausulae++;
		situs->destinationes += cl->destinationes;

		si (socia)
		{
			situs->habet_redirectionem = VERUM;
			perge;
		}

		/* blocum substantiale: lacunas implet (ordo blocorum in
		 * plagula ergo non refert) */
		si (chorda_vacua(situs->destinatio))
		{
			situs->destinatio = _copia(cl->destinatio, piscina);
		}
		si (chorda_vacua(situs->certificatum))
		{
			situs->certificatum = _copia(cl->certificatum, piscina);
		}
		si (chorda_vacua(situs->radix))
		{
			situs->radix = _copia(cl->radix, piscina);
		}
		si (chorda_vacua(situs->hospites))
		{
			situs->hospites = _copia(cl->hospites, piscina);
		}
		si (cl->ssl)
		{
			situs->ssl = VERUM;
		}
	}
	redde fructus;
}

Xar*
villa_nginx_legere (chorda effusio, Piscina* piscina)
{
	Xar* clausulae = villa_nginx_clausulas_legere(effusio, piscina);

	si (clausulae == NIHIL)
	{
		redde NIHIL;
	}
	redde villa_nginx_situs_plicare(clausulae, piscina);
}


/* ========================================================================
 * V. LECTORES PARVI
 * ======================================================================== */

b32
villa_discum_legere (chorda effusio, StatusDisci* fructus,
	Piscina* piscina)
{
	chorda linea;
	i32    positus = ZEPHYRUM;

	si (fructus == NIHIL)
	{
		redde FALSUM;
	}
	memset(fructus, 0, magnitudo(StatusDisci));

	dum (_linea_proxima(effusio, &positus, &linea))
	{
		chorda nuda = chorda_praecidere(linea);
		chorda capacitas;

		si (nuda.mensura == ZEPHYRUM)
		{
			perge;
		}
		/* caput transire */
		si (chorda_aequalis_literis(_verbum(nuda, ZEPHYRUM),
			"Filesystem"))
		{
			perge;
		}
		si (_numerus_verborum(nuda) < (i32)VI)
		{
			perge;
		}

		fructus->systema = _copia(_verbum(nuda, ZEPHYRUM), piscina);
		_ut_i64(_verbum(nuda, I), &fructus->frusta);
		_ut_i64(_verbum(nuda, II), &fructus->usa);
		_ut_i64(_verbum(nuda, III), &fructus->praesto);
		capacitas = _verbum(nuda, IV);   /* '29%' */
		_ut_i32(capacitas, &fructus->capacitas);
		/* punctum insertionis spatia continere potest */
		fructus->punctum = _copia(
			chorda_praecidere(_reliquum_a_verbo(nuda, V)), piscina);
		redde VERUM;
	}
	redde FALSUM;
}

b32
villa_memoriam_legere (chorda effusio, StatusMemoriae* fructus)
{
	chorda linea;
	i32    positus = ZEPHYRUM;
	b32    aliquid = FALSUM;

	si (fructus == NIHIL)
	{
		redde FALSUM;
	}
	memset(fructus, 0, magnitudo(StatusMemoriae));

	dum (_linea_proxima(effusio, &positus, &linea))
	{
		s32    index_bipuncti;
		chorda clavis;
		chorda valor;

		index_bipuncti = chorda_invenire_index(linea, _ch(":"));
		si (index_bipuncti < ZEPHYRUM)
		{
			perge;
		}
		clavis = chorda_praecidere(
			chorda_sectio(linea, ZEPHYRUM, (i32)index_bipuncti));
		valor = chorda_sectio(linea, (i32)index_bipuncti + I,
			linea.mensura);

		si (chorda_aequalis_literis(clavis, "MemTotal"))
		{
			si (_ut_i64(valor, &fructus->summa_kb))
			{
				aliquid = VERUM;
			}
		}
		alioquin si (chorda_aequalis_literis(clavis, "MemFree"))
		{
			si (_ut_i64(valor, &fructus->libera_kb))
			{
				aliquid = VERUM;
			}
		}
		alioquin si (chorda_aequalis_literis(clavis, "MemAvailable"))
		{
			si (_ut_i64(valor, &fructus->praesto_kb))
			{
				aliquid = VERUM;
			}
		}
		alioquin si (chorda_aequalis_literis(clavis, "Buffers"))
		{
			si (_ut_i64(valor, &fructus->interposita_kb))
			{
				aliquid = VERUM;
			}
		}
		alioquin si (chorda_aequalis_literis(clavis, "Cached"))
		{
			si (_ut_i64(valor, &fructus->condita_kb))
			{
				aliquid = VERUM;
			}
		}
	}
	redde aliquid;
}

b32
villa_tempus_activum_legere (chorda effusio, i64* secunda)
{
	si (secunda == NIHIL)
	{
		redde FALSUM;
	}
	/* '27223431.74 25953501.86' - pars integra primi campi */
	redde _ut_i64(_verbum(effusio, ZEPHYRUM), secunda);
}
