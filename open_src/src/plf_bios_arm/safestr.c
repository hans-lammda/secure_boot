
#include <plf/platform.h>
#include <plf/safestr.h>

const char __attribute__ ((section (".rom.ident")))  plf_safestr_header_version[] = __PMILL_SAFESTR_H__;
const char __attribute__ ((section (".rom.ident")))  plf_safestr_code_version[]   = "$HeadURL: https://cm-ext.dev.oniteo.com/svn/nanodev/packages/plf_bios_arm/branches/haspoc/arm64_v8/plf_bios_arm/safestr.c $ $Revision: 21354 $";


#define is_digit(c)      ((c) >= '0' && (c) <= '9')




// static size_t strnlen(const char *src, size_t max)
size_t strnlen(const char *src, size_t max)
{
    size_t i = 0;

    while ((*src++) && (i < max))
        i++;
    return i;
}

static int skip_atoi(const char **s)
{
	int i=0;

	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */

#define do_div(n,base) ({ \
int __res; \
__res = ((unsigned long) n) % (unsigned) base; \
n = ((unsigned long) n) / (unsigned) base; \
__res; })

static int number(int (*tx_byte)(char **str_buf, int byte), char **buf, long long num, int base, int size, int precision
	,int type)
{
	char c,sign,tmp[66];
	const char *digits="0123456789abcdefghijklmnopqrstuvwxyz";
	int i;
	int count = 0;

	if (type & LARGE)
		digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN) {
		if (num < 0) {
			sign = '-';
			num = -num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num != 0)
		tmp[i++] = digits[do_div(num,base)];
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT)))
		while(size-->0)
		  tx_byte(buf, ' '), count++;
	if (sign)
	  tx_byte(buf, sign), count++;
	if (type & SPECIAL) {
		if (base==8)
		  tx_byte(buf, '0'), count++;
		else if (base==16) {
		  tx_byte(buf, '0'), count++;
		  tx_byte(buf, digits[33]), count++;
		}
	}
	if (!(type & LEFT))
		while (size-- > 0)
		  tx_byte(buf,c), count++;
	while (i < precision--)
	  tx_byte(buf,'0'), count++;
	while (i-- > 0)
	  tx_byte(buf,tmp[i]), count++;
	while (size-- > 0)
	  tx_byte(buf, ' '), count++;
	return count;
}


static int vtxprintf(int (*tx_byte)(char **buf_str, int byte), char **buf, const char *fmt, va_list args)
{
	int len;
	unsigned long long num;
	int i, base;
	const char *s;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */

	int count;

	for (count=0; *fmt ; ++fmt) {
		if (*fmt != '%') {
		  tx_byte(buf, *fmt), count++;
			continue;
		}

		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
				}

		/* get field width */
		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		/* default base */
		base = 10;

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
				  tx_byte(buf, ' '), count++;
			tx_byte(buf, (unsigned char) va_arg(args, int)), count++;
			while (--field_width > 0)
			  tx_byte(buf,' '), count++;
			continue;

		case 's':
			s = va_arg(args, char *);
			if (!s)
				s = "<NULL>";

			len = strnlen(s, precision);

			if (!(flags & LEFT))
				while (len < field_width--)
				  tx_byte(buf,' '), count++;
			for (i = 0; i < len; ++i)
			  tx_byte(buf,*s++), count++;
			while (len < field_width--)
			  tx_byte(buf,' '), count++;
			continue;

		case 'p':
			if (field_width == -1) {
				field_width = 2*sizeof(void *);
				flags |= ZEROPAD;
			}
			count += number(tx_byte,buf,
				(unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags);
			continue;


		case 'n':
			if (qualifier == 'l') {
				long * ip = va_arg(args, long *);
				*ip = count;
			} else {
				int * ip = va_arg(args, int *);
				*ip = count;
			}
			continue;

		case '%':
		  tx_byte(buf,'%'), count++;
			continue;

		/* integer number formats - set up the flags and "break" */
		case 'o':
			base = 8;
			break;

		case 'X':
			flags |= LARGE;
		case 'x':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			break;

		default:
		  tx_byte(buf,'%'), count++;
			if (*fmt)
			  tx_byte(buf,*fmt), count++;
			else
				--fmt;
			continue;
		}
		if (qualifier == 'L')
			num = va_arg(args, unsigned long long);
		else if (qualifier == 'l')
			num = va_arg(args, unsigned long);
		else if (qualifier == 'h') {
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)
				num = (short) num;
		} else if (flags & SIGN)
			num = va_arg(args, int);
		else
			num = va_arg(args, unsigned int);
		count += number(tx_byte, buf, num, base, field_width, precision, flags);
	}
	return count;
}

static int str_tx_byte(char **str_buf, int byte)
{
  **str_buf = byte;
  (*str_buf)++;
  return byte;
}


int vsprintf(char *buf, const char *fmt, va_list args)
{
	int i=0;
	i = vtxprintf(str_tx_byte, &buf, fmt, args);
	*buf = '\0';
	return i;
}

int sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	return i;
}



int NStrVSPrintf(char* target, int targetSize, const char* format, va_list v)
{
   int nWritten;

   if (targetSize<1)
      return SAFESTR_ERROR_INVALID_PARAMETER;

   nWritten = vsprintf(target, format, v);
   

  
   if (nWritten<targetSize){
     return SAFESTR_OK;
   }
   else if (nWritten>=targetSize-1)
      return SAFESTR_ERROR_INSUFFICIENT_BUFFER;
   else
      return SAFESTR_ERROR_UNKNOWN;
}

int NStrSPrintf(char* target, int targetSize, const char* format, ...)
{
   va_list v;
   int nWritten;

   if (targetSize<1)
      return SAFESTR_ERROR_INVALID_PARAMETER;

   va_start(v,format);
   nWritten = vsprintf(target, format, v);
   va_end(v);

   if (nWritten<targetSize)
      return SAFESTR_OK;
   else if (nWritten>=targetSize-1)
      return SAFESTR_ERROR_INSUFFICIENT_BUFFER;
   else
      return SAFESTR_ERROR_UNKNOWN;
}


int NStrSCopy(char* target, const char* source, int targetSize)
{
   char* pszPtrT;
   const char* pszPtrS;
   int nCopied;

   if (!target || targetSize<1)
      return SAFESTR_ERROR_INVALID_PARAMETER;

   if (!source)
   {
      target[0]=0;
      return SAFESTR_OK;
   }

   nCopied = 0;
   pszPtrS = source;
   pszPtrT = target;

   while (*pszPtrS && nCopied<targetSize-1)
   {
      *pszPtrT = *pszPtrS;
      pszPtrT++;
      pszPtrS++;
      nCopied++;
   }

   if ( *pszPtrS == '\0' )
      *pszPtrT = '\0';
   else
   {
      *pszPtrT = '\0';
      return SAFESTR_ERROR_INSUFFICIENT_BUFFER;
   }

   return SAFESTR_OK;
}


int NStrSCopyN(char* target, int targetSize, const char* source, int sourceSize)
{
   char* pszPtrT;
   const char* pszPtrS;
   int nCopied;

   if (!target || targetSize<1)
      return SAFESTR_ERROR_INVALID_PARAMETER;

   if (!source || sourceSize<1)
   {
      target[0]=0;
      return SAFESTR_OK;
   }

   nCopied = 0;
   pszPtrS = source;
   pszPtrT = target;

   while (*pszPtrS && nCopied<targetSize-1 && nCopied<sourceSize)
   {
      *pszPtrT = *pszPtrS;
      pszPtrT++;
      pszPtrS++;
      nCopied++;
   }

   if (nCopied<targetSize)
      return SAFESTR_OK;
   else if (nCopied>=targetSize-1)
   {
      *pszPtrT = 0;
      return SAFESTR_ERROR_INSUFFICIENT_BUFFER;;
   }
   else
      return SAFESTR_ERROR_UNKNOWN;
}


int NStrSCat(char* target, const char* source, int targetSize)
{
   char* pszPtrT;
   const char* pszPtrS;
   int nFilled;

   if (!target || targetSize<1)
      return SAFESTR_ERROR_INVALID_PARAMETER;

   if (!source)
   {
      target[0]=0;
      return SAFESTR_OK;
   }

   nFilled = 0;
   pszPtrT = target;
   while (*pszPtrT && nFilled<targetSize-1)
   {
      pszPtrT++;
      nFilled++;
   }

   pszPtrS = source;
   while (*pszPtrS && nFilled<targetSize-1)
   {
      *pszPtrT = *pszPtrS;
      pszPtrT++;
      pszPtrS++;
      nFilled++;
   }

   if (nFilled<targetSize) {
      *pszPtrT = 0;
      return SAFESTR_OK;
   }
   else if (nFilled>=targetSize-1)
   {
      *pszPtrT = 0;
      return SAFESTR_ERROR_INSUFFICIENT_BUFFER;;
   }
   else
      return SAFESTR_ERROR_UNKNOWN;
}

