/*
** msgid.h
**
** defines for message-ids (warnings, errors,..)
**
** updated:  7-Aug-1995
** created: 30-Jul-1995
*/


#ifndef HSC_MSGID_H
#define HSC_MSGID_H

#include "ugly/types.h"

/*
** messeges
*/
#define MSG    0
#define MSG_UNKN_MACRO      (MSG   +1) /* unknown macro */
#define MSG_RPLC_MACRO      (MSG   +2) /* replacing existing macro */
#define MSG_STRIPPED_URL    (MSG   +3) /* stripped external URL */


/*
** bad style messages
*/
#define STYLE                     100
#define STYLE_WRONG_HEADING (STYLE+ 1) /* wrong heading */
#define STYLE_CLICK_HERE    (STYLE+ 2) /* click-here-syndrome */
#define STYLE_WHITE_SPACE   (STYLE+ 3) /* white space around tag */

/*
** warnings
*/
#define WARN                      200
#define CWRN                      250  /* critical warning */
#define WARN_UNKN_TAG        (WARN+ 1) /* unknown tag */
#define WARN_UNKN_CTAG       (WARN+ 2) /* unknown closing tag */
#define WARN_IMG_NOALT       (WARN+ 3) /* <IMG> without ALT */
#define WARN_NEST_TAG        (WARN+ 4) /* illegal closing tag nesting */
#define WARN_UNKN_ENTITY     (WARN+ 5) /* unknown entity */
#define WARN_MISS_RTAG       (WARN+ 6) /* required tag missing */
#define WARN_UNKN_OPTVAL     (WARN+ 7) /* unknown value for option */
#define WARN_EXPT_SEMIK      (WARN+ 8) /* ";" expected */
#define WARN_UNKN_OPTN       (WARN+ 9) /* unknown option */

#define WARN_UNMA_CTAG       (CWRN+50) /* unmatched closing tag */
#define WARN_MISS_CTAG       (CWRN+51) /* closing tag missing */

/*
** errors
*/
#define ERROR                     300
#define ERROR_EOF           (ERROR+ 1) /* unexpected eof */
#define ERROR_TOO_OFTEN     (ERROR+ 2) /* tag occured too often */
#define ERROR_UNMA_GT       (ERROR+ 3) /* unmatched ">" */
#define ERROR_FILE_OPEN     (ERROR+ 4) /* can't open file */
#define ERROR_UNEXPT_CH     (ERROR+ 5) /* unexpected char */
#define ERROR_UNEX_LI       (ERROR+ 6) /* unexpected <LI> */
#define ERROR_IMG_NOSRC     (ERROR+ 7) /* <IMG> without SRC */
#define ERROR_UNKN_HSCOPTN  (ERROR+ 8) /* unknown option in hsc-tag */
#define ERROR_A_NOARG       (ERROR+ 9) /* no HREF and NAME for <A> */
#define ERROR_EXPAND_MACRO  (ERROR+10) /* can not expand macro */
#define ERROR_NO_URLPATH    (ERROR+11) /* path to URL not found */

/*
** fatal errors
*/
#define FATAL                     400
#define FATAL_NO_MEM        (FATAL+ 1) /* out of mem */
#define FATAL_WRITE_ERR     (FATAL+ 2) /* write error */
#define FATAL_LONG_STR      (FATAL+ 3) /* string too long */

#endif /* HSC_MSGID_H */
