/***
*ismbgrph - Test if character is graphical (MBCS)
*
*	Copyright (c) 1985-1994, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*	Test if character is graphical (MBCS)
*
*Revision History:
*	10-21-93  CFW	Module created.
*       11-09-93  CFW	Add code page for __crtxxx().
*       01-12-94  CFW	Add lcid for __crtxxx().
*       04-18-94  CFW	Use _ALPHA rather than _UPPER|_LOWER.
*       04-29-94  CFW	Place c in char array.
*       05-17-94  CFW	Enable non-Win32.
*
*******************************************************************************/

#ifdef _MBCS

#if defined(_WIN32) && !defined(_POSIX_)
#include <windows.h>
#include <awint.h>
#endif /* _WIN32 */

#include <cruntime.h>
#include <ctype.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>


/***
* _ismbcgraph - Test if character is graphical (MBCS)
*
*Purpose:
*	Test if the supplied character is graphical or not.
*	Handles MBCS characters correctly.
*
*	Note:  Use test against 0x00FF instead of _ISLEADBYTE
*	to ensure that we don't call SBCS routine with a two-byte
*	value.
*
*Entry:
*	unsigned int c = character to test
*
*Exit:
*	Returns TRUE if c is an graphical character; else FALSE
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _ismbcgraph(
    unsigned int c
    )
{
        if (c > 0x00FF)
        {

#if defined(_WIN32) && !defined(_POSIX_)

            char buf[2];
            unsigned short ctype[2] = {0};

            buf[0] = (c >> 8) & 0xFF;
            buf[1] = c & 0xFF;

            /* return FALSE if not in supported MB code page */
            if (__mbcodepage == 0)
                return 0;

            /*
            * Since 'c' could be two one-byte MB chars, we need room in the
            * ctype return array to handle this. In this case, the
            * second word in the return array will be non-zero.
            */

		if (__crtGetStringTypeA (CT_CTYPE1, buf,
                2, ctype, __mbcodepage, __mblcid) == 0)
			return 0;

            /* ensure single MB character and test for type */
            return (ctype[1] == 0 && ctype[0] & (_PUNCT|_ALPHA|_DIGIT));

#else /* _WIN32 */

            /* no hard-coded double-byte graphical info available */
            return _ismbcalnum(c) | _ismbcdigit(c);

#endif /* _WIN32 */

        } else

            return _ismbbgraph(c);
}

#endif	/* _MBCS */
