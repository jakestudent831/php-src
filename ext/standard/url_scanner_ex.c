/* Generated by re2c 0.5 on Tue Sep 19 20:15:55 2000 */
#line 1 "/home/sas/src/php4/ext/standard/url_scanner_ex.re"
/*
  +----------------------------------------------------------------------+
  | PHP version 4.0                                                      |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997, 1998, 1999, 2000 The PHP Group                   |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.02 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/2_02.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Sascha Schumann <sascha@schumann.cx>                        |
  +----------------------------------------------------------------------+
*/

#include "php.h"

#ifdef TRANS_SID

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "php_globals.h"
#define STATE_TAG SOME_OTHER_STATE_TAG
#include "basic_functions.h"
#undef STATE_TAG

#define url_adapt_ext url_adapt_ext_ex
#define url_scanner url_scanner_ex
#define url_adapt_state url_adapt_state_ex
#define url_adapt_state_t url_adapt_state_ex_t

static inline void smart_str_append(smart_str *dest, smart_str *src)
{
	size_t newlen;

	if (!dest->c)
		dest->len = dest->a = 0;
	
	newlen = dest->len + src->len;
	if (newlen >= dest->a) {
		dest->c = realloc(dest->c, newlen + 101);
		dest->a = newlen + 100;
	}
	memcpy(dest->c + dest->len, src->c, src->len);
	dest->c[dest->len = newlen] = '\0';
}

static inline void smart_str_free(smart_str *s)
{
	if (s->c) {
		free(s->c);
		s->c = NULL;
	}
	s->a = s->len = 0;
}

static inline void smart_str_copyl(smart_str *dest, const char *src, size_t len)
{
	dest->c = realloc(dest->c, len + 1);
	memcpy(dest->c, src, len);
	dest->c[len] = '\0';
	dest->a = dest->len = len;
}

static inline void smart_str_appendl(smart_str *dest, const char *src, size_t len)
{
	smart_str s;

	s.c = (char *) src;
	s.len = len;

	smart_str_append(dest, &s);
}

static inline void smart_str_setl(smart_str *dest, const char *src, size_t len)
{
	dest->len = len;
	dest->a = len + 1;
	dest->c = src;
}

static inline void smart_str_appends(smart_str *dest, const char *src)
{
	smart_str_appendl(dest, src, strlen(src));
}

static inline void smart_str_copys(smart_str *dest, const char *src)
{
	smart_str_copyl(dest, src, strlen(src));
}

static inline void smart_str_sets(smart_str *dest, const char *src)
{
	smart_str_setl(dest, src, strlen(src));
}

static inline void attach_url(smart_str *url, smart_str *name, smart_str *val, const char *separator)
{
	if (strchr(url->c, ':')) return;

	if (strchr(url->c, '?'))
		smart_str_appendl(url, separator, 1);
	else
		smart_str_appendl(url, "?", 1);

	smart_str_append(url, name);
	smart_str_appendl(url, "=", 1);
	smart_str_append(url, val);
}

static char *check_tag_arg[] = {
	"a", "href",
	"area", "href",
	"frame", "source",
	"img", "src",
	NULL
};

static inline void tag_arg(url_adapt_state_t *ctx PLS_DC)
{
	char f = 0;
	int i;

	for (i = 0; check_tag_arg[i]; i += 2) {
		if (strcasecmp(ctx->tag.c, check_tag_arg[i]) == 0
				&& strcasecmp(ctx->arg.c, check_tag_arg[i + 1]) == 0) {
			f = 1;
			break;
		}
	}

	smart_str_appends(&ctx->result, "\"");
	if (f) {
		attach_url(&ctx->para, &ctx->name, &ctx->value, PG(arg_separator));
	}
	smart_str_append(&ctx->result, &ctx->para);
	smart_str_appends(&ctx->result, "\"");
}

#line 151


#define NEXT continue

#define COPY_ALL \
	smart_str_appendl(&ctx->result, start, YYCURSOR - start); \
	start = NULL; \
	NEXT
	
#define YYFILL(n) goto finish
#define YYCTYPE unsigned char
#define YYLIMIT (ctx->work.c+ctx->work.len)
#define YYCURSOR cursor
#define YYMARKER marker

#define HANDLE_FORM \
	if (strcasecmp(ctx->tag.c, "form") == 0) { \
		smart_str_appends(&ctx->result, "><INPUT TYPE=HIDDEN NAME=\""); \
		smart_str_append(&ctx->result, &ctx->name); \
		smart_str_appends(&ctx->result, "\" VALUE=\""); \
		smart_str_append(&ctx->result, &ctx->value); \
		smart_str_appends(&ctx->result, "\""); \
	}

#define GO(n) ctx->state = n

enum {
	STATE_PLAIN,
	STATE_TAG,
	STATE_NEXT_ARG,
	STATE_ARG,
	STATE_PARA
};

static void mainloop(url_adapt_state_t *ctx, smart_str *newstuff)
{
	char *para_start, *arg_start, *tag_start;
	char *start = NULL;
	char *cursor, *marker;
	PLS_FETCH();

	arg_start = para_start = tag_start = NULL;
	smart_str_append(&ctx->work, newstuff);
	smart_str_free(&ctx->result);

	cursor = ctx->work.c;
	
	while (YYCURSOR < YYLIMIT) {
		start = YYCURSOR;

#ifdef SCANNER_DEBUG
	printf("state %d:%s'\n", ctx->state, YYCURSOR);
#endif
		switch (ctx->state) {
			
		case STATE_PLAIN:
{
	YYCTYPE yych;
	unsigned int yyaccept;
	static unsigned char yybm[] = {
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128,   0, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	};
	goto yy0;
yy1:	++YYCURSOR;
yy0:
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	if(yybm[0+yych] & 128)	goto yy4;
yy2:	yych = *++YYCURSOR;
yy3:
#line 208
	{ tag_start = YYCURSOR; GO(STATE_TAG); COPY_ALL;}
yy4:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy5:	if(yybm[0+yych] & 128)	goto yy4;
yy6:
#line 209
	{ COPY_ALL; }
}
#line 210

  			break;
		
		case STATE_TAG:
{
	YYCTYPE yych;
	unsigned int yyaccept;
	static unsigned char yybm[] = {
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128,   0,   0,   0,   0,   0, 
	  0, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	};
	goto yy7;
yy8:	++YYCURSOR;
yy7:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	if(yych <= '@')	goto yy11;
	if(yych <= 'Z')	goto yy9;
	if(yych <= '`')	goto yy11;
	if(yych >= '{')	goto yy11;
yy9:	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yybm[0+yych] & 128)	goto yy14;
	if(yych == ' ')	goto yy12;
	if(yych == '>')	goto yy12;
yy10:
#line 225
	{ 
  						YYCURSOR--; 
						GO(STATE_PLAIN);
						tag_start = NULL; 
						NEXT;
				}
yy11:	yych = *++YYCURSOR;
	goto yy10;
yy12:	yych = *++YYCURSOR;
yy13:
#line 215
	{ 
  						YYCURSOR--; 
  						arg_start = YYCURSOR;
						smart_str_copyl(&ctx->tag, start, YYCURSOR - start); 
#ifdef SCANNER_DEBUG
						printf("TAG(%s)\n", ctx->tag.c);
#endif
						GO(STATE_NEXT_ARG);
						COPY_ALL; 
				}
yy14:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy15:	if(yybm[0+yych] & 128)	goto yy14;
	if(yych == ' ')	goto yy12;
	if(yych == '>')	goto yy12;
yy16:	YYCURSOR = YYMARKER;
	switch(yyaccept){
	case 0:	goto yy10;
	}
}
#line 231

			break;

		case STATE_NEXT_ARG:
{
	YYCTYPE yych;
	unsigned int yyaccept;
	static unsigned char yybm[] = {
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	128,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	};
	goto yy17;
yy18:	++YYCURSOR;
yy17:
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	if(yybm[0+yych] & 128)	goto yy20;
	if(yych == '>')	goto yy23;
yy19:yy20:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy21:	if(yybm[0+yych] & 128)	goto yy20;
yy22:
#line 236
	{
  						GO(STATE_ARG);
						NEXT; 
				}
yy23:	yych = *++YYCURSOR;
yy24:
#line 240
	{
  						HANDLE_FORM;
						GO(STATE_PLAIN);
						tag_start = NULL;
						COPY_ALL;
				}
}
#line 246

			break;

		case STATE_ARG:
				smart_str_appendl(&ctx->result, " ", 1);
{
	YYCTYPE yych;
	unsigned int yyaccept;
	static unsigned char yybm[] = {
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	128,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,   0,   0,  64, 
	 64,  96,  96,  96,  96,  96,  96,  96, 
	 96,  96,  96,  96,  96,  96,  96,  96, 
	 96,  96,  96,  96,  96,  96,  96,  96, 
	 96,  96,  96,  64,  64,  64,  64,  64, 
	 64,  96,  96,  96,  96,  96,  96,  96, 
	 96,  96,  96,  96,  96,  96,  96,  96, 
	 96,  96,  96,  96,  96,  96,  96,  96, 
	 96,  96,  96,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	 64,  64,  64,  64,  64,  64,  64,  64, 
	};
	goto yy25;
yy26:	++YYCURSOR;
yy25:
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	if(yybm[0+yych] & 32)	goto yy28;
	if(yych == ' ')	goto yy27;
	if(yych <= '<')	goto yy30;
	if(yych >= '?')	goto yy30;
yy27:
#line 264
	{
  						arg_start = YYCURSOR;
						ctx->state--;
						COPY_ALL;
				}
yy28:	yyaccept = 0;
	YYMARKER = ++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy29:	if(yybm[0+yych] & 128)	goto yy32;
	if(yych <= '@'){
		if(yych <= '<')	goto yy30;
		if(yych <= '=')	goto yy35;
		if(yych <= '>')	goto yy27;
	} else {
		if(yych <= 'Z')	goto yy28;
		if(yych <= '`')	goto yy30;
		if(yych <= 'z')	goto yy28;
	}
yy30:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy31:	if(yybm[0+yych] & 64)	goto yy30;
	goto yy27;
yy32:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy33:	if(yybm[0+yych] & 128)	goto yy32;
	if(yych == '=')	goto yy35;
yy34:	YYCURSOR = YYMARKER;
	switch(yyaccept){
	case 0:	goto yy27;
	}
yy35:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy36:	if(yych == ' ')	goto yy35;
yy37:
#line 252
	{
  						char *p;

						for (p = start; isalpha(*p); p++);
						smart_str_setl(&ctx->arg, start, p - start);
#ifdef SCANNER_DEBUG
						printf("ARG(%s)\n", ctx->arg.c);
#endif
						para_start = YYCURSOR;
						ctx->state++;
						COPY_ALL;
				}
}
#line 269

			break;

		case STATE_PARA:
{
	YYCTYPE yych;
	unsigned int yyaccept;
	static unsigned char yybm[] = {
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	128, 224,  32, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224,   0, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224,   0, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	224, 224, 224, 224, 224, 224, 224, 224, 
	};
	goto yy38;
yy39:	++YYCURSOR;
yy38:
	if((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
	yych = *YYCURSOR;
	if(yych <= '"'){
		if(yych == ' ')	goto yy43;
		if(yych <= '!')	goto yy42;
	} else {
		if(yych <= '>'){
			if(yych <= '=')	goto yy42;
			goto yy43;
		} else {
			if(yych == '^')	goto yy43;
			goto yy42;
		}
	}
yy40:	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yych != '^')	goto yy51;
yy41:
#line 298
	{
  						YYCURSOR--;
						ctx->state = 2;
						NEXT;
				}
yy42:	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yych == '^')	goto yy41;
	goto yy47;
yy43:	yych = *++YYCURSOR;
	goto yy41;
yy44:	yych = *++YYCURSOR;
yy45:
#line 286
	{
  						YYCURSOR--;
						para_start = NULL;
						smart_str_setl(&ctx->para, start, YYCURSOR - start);
#ifdef SCANNER_DEBUG
						printf("PARA(%s)\n", ctx->para.c);
#endif
						tag_arg(ctx PLS_CC);
						arg_start = YYCURSOR;
						GO(STATE_NEXT_ARG);
						NEXT;
				}
yy46:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy47:	if(yybm[0+yych] & 32)	goto yy46;
	if(yych <= ']')	goto yy44;
yy48:	YYCURSOR = YYMARKER;
	switch(yyaccept){
	case 1:	goto yy45;
	case 0:	goto yy41;
	}
yy49:	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yych == '>')	goto yy45;
	if(yych == '^')	goto yy45;
	goto yy57;
yy50:	++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
yy51:	if(yybm[0+yych] & 64)	goto yy50;
	if(yych <= '!')	goto yy49;
	if(yych <= '"')	goto yy52;
	if(yych <= ']')	goto yy44;
	goto yy48;
yy52:	yych = *++YYCURSOR;
	if(yych == ' ')	goto yy53;
	if(yych != '>')	goto yy47;
yy53:	yych = *++YYCURSOR;
yy54:
#line 274
	{
  						YYCURSOR--;
						para_start = NULL;
						smart_str_setl(&ctx->para, start + 1, YYCURSOR - start - 2);
#ifdef SCANNER_DEBUG
						printf("PARA(%s)\n", ctx->para.c);
#endif
						tag_arg(ctx PLS_CC);
						arg_start = YYCURSOR;
						GO(STATE_NEXT_ARG);
						NEXT;
				}
yy55:	yych = *++YYCURSOR;
	if(yych == ' ')	goto yy58;
	if(yych == '>')	goto yy58;
	goto yy48;
yy56:	++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
yy57:	if(yybm[0+yych] & 128)	goto yy56;
	if(yych <= '=')	goto yy55;
	goto yy48;
yy58:	yych = *++YYCURSOR;
	goto yy54;
}
#line 303

			break;
		}
	}

#define PRESERVE(s) \
	size_t n = ctx->work.len - (s - ctx->work.c); \
	memmove(ctx->work.c, s, n + 1); \
	ctx->work.len = n
	
finish:
	if (ctx->state >= 2) {
		if (para_start) {
			PRESERVE(para_start);
			ctx->state = 4;
		} else {
			if (arg_start) { PRESERVE(arg_start); }
			ctx->state = 2;
		}
	} else if (tag_start) {
		PRESERVE(tag_start);
		ctx->state = 1;
	} else {
		ctx->state = 0;
		if (start) smart_str_appendl(&ctx->result, start, YYCURSOR - start);
		smart_str_free(&ctx->work);
	}

#ifdef SCANNER_DEBUG 
	if (ctx->work.c) {
		printf("PRESERVING %s'\n", ctx->work.c);
	}
#endif
}

char *url_adapt_ext(const char *src, size_t srclen, const char *name, const char *value, size_t *newlen)
{
	smart_str str = {0,0};
	char *ret;
	BLS_FETCH();

	smart_str_sets(&BG(url_adapt_state).name, name);
	smart_str_sets(&BG(url_adapt_state).value, value);
	str.c = (char *) src;
	str.len = srclen;
	mainloop(&BG(url_adapt_state), &str);

	*newlen = BG(url_adapt_state).result.len;

#ifdef SCANNER_DEBUG
	printf("(%d)NEW(%d): %s'\n", srclen, BG(url_adapt_state).result.len, BG(url_adapt_state).result.c);
#endif

#if 1
	ret = BG(url_adapt_state).result.c;
	BG(url_adapt_state).result.c = NULL;
	return ret;
#else
	return strdup(BG(url_adapt_state).result.c);
#endif
}

PHP_RINIT_FUNCTION(url_scanner)
{
	BLS_FETCH();

	memset(&BG(url_adapt_state), 0, sizeof(BG(url_adapt_state)));

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(url_scanner)
{
	BLS_FETCH();

	smart_str_free(&BG(url_adapt_state).result);
	smart_str_free(&BG(url_adapt_state).work);

	return SUCCESS;
}

#endif
