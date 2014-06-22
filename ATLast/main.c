// A T L A S T
//
// Autodesk Threaded Language Application System Toolkit
//
//  ATLast/main.c
//
//  Derived from code designed and implemented 1990 by John Walker
//  and placed into the public domain by him.
//  Original at https://www.fourmilab.ch/atlast/atlast.html
//
//  Created by Michael Henderson on 6/19/14.
//  Changes Copyright (c) 2014 Michael D Henderson. All rights reserved.
//
// Main driver program for interactive ATLAST
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>

#include "atlcfg.h"

//#include "atldef.h"
//
//  ATLast/atldef.h
//
//  Created by Michael Henderson on 6/19/14.
//  Copyright (c) 2014 Michael D Henderson. All rights reserved.
//  Portions Copyright (c) 1990 by John Walker and placed into the public domain.
//
// A T L A S T
//
// Autodesk Threaded Language Application System Toolkit
//
//  ATLast/atldef.h
//
//  Designed and implemented in January of 1990 by John Walker.
//  Original at https://www.fourmilab.ch/atlast/atlast.html
//
//  Created by Michael Henderson on 6/18/14.
//  Changes Copyright (c) 2014 Michael D Henderson. All rights reserved.
//
// Definitions for components of ATLAST
//
// This  file	contains the definitions for modules within the ATLAST
// language system itself.  Definitions used by programs that link to
// ATLAST   are   in	ATLAST.H,  which  is  included	by  this  file
// automatically.
//

#ifndef ATLast_atldef_h
#define ATLast_atldef_h

//#include "atltypes.h"
// A T L A S T
//
// Autodesk Threaded Language Application System Toolkit
//
//  ATLast/atltypes.h
//
//  Derived from code designed and implemented 1990 by John Walker
//  and placed into the public domain by him.
//  Original at https://www.fourmilab.ch/atlast/atlast.html
//
//  Created by Michael Henderson on 6/19/14.
//  Changes Copyright (c) 2014 Michael D Henderson. All rights reserved.
//

#ifndef ATLast_atltypes_h
#define ATLast_atltypes_h

// Data types

typedef long                 atl_int;               // Stack integer type
typedef double               atl_real;              // Real number type
typedef struct atl_statemark atl_statemark;
typedef void               (*codeptr)(void);        // Machine code pointer
typedef struct dw            dictword;
typedef dictword           **rstackitem;
typedef long                 stackitem;

typedef enum {atlFalse = 0, atlTrue = 1} Boolean;

// Internal state marker item

struct atl_statemark {
    stackitem  *mstack;     // Stack position marker
    stackitem  *mheap;      // Heap allocation marker
    dictword ***mrstack;    // Return stack position marker
    dictword   *mdict;      // Dictionary marker
};

// Dictionary word entry

struct dw {
    struct dw *wnext;       // Next word in dictionary
    char      *wname;       // Word name.  The first character is
    // actually the word flags, including
    // the (IMMEDIATE) bit.
    codeptr    wcode;       // Machine code implementation
};

// Primitive definition table entry

struct primfcn {
    char   *pname;
    codeptr pcode;
};


// Functions called by exported extensions.
//
stackitem *atl_body(dictword *dw);
void       atl_error(char *kind);
char      *atl_fgetsp(char *s, int n, FILE *stream);
int        atl_exec(dictword *dw);
dictword  *atl_lookup(char *name);
void       atl_primdef(struct primfcn *pt);
dictword  *atl_vardef(char *name, int size);

// internal use functions
//
int atl__token(char **cp);

#endif
// end of ATLast/atltypes.h

//#include "atlast.h"               // Define user linkage structures
// A T L A S T
//
// Autodesk Threaded Language Application System Toolkit
//
//  ATLast/atlast.h
//
//  Derived from code designed and implemented 1990 by John Walker
//  and placed into the public domain by him.
//  Original at https://www.fourmilab.ch/atlast/atlast.html
//
//  Created by Michael Henderson on 6/19/14.
//  Changes Copyright (c) 2014 Michael D Henderson. All rights reserved.
//
//  Program Linkage Definitions
//
//   This  module  contains  the  definitions  needed by programs that
//   invoke the ATLAST system.	It does contain the  definitions  used
//   internally   within  ATLAST  (which  might  create  conflicts  if
//   included in calling programs).
//

#ifndef ATLast_atlast_h
#define ATLast_atlast_h

#include "atltypes.h"

#include <stdio.h>

// External symbols accessible by the calling program.

//  ATL_EVAL return status codes

#define ATL_SNORM       0	      // Normal evaluation
#define ATL_STACKOVER	-1	      // Stack overflow
#define ATL_STACKUNDER	-2	      // Stack underflow
#define ATL_RSTACKOVER	-3	      // Return stack overflow
#define ATL_RSTACKUNDER -4	      // Return stack underflow
#define ATL_HEAPOVER	-5	      // Heap overflow
#define ATL_BADPOINTER	-6	      // Pointer outside the heap
#define ATL_UNDEFINED	-7	      // Undefined word
#define ATL_FORGETPROT	-8	      // Attempt to forget protected word
#define ATL_NOTINDEF	-9	      // Compiler word outside definition
#define ATL_RUNSTRING	-10	      // Unterminated string
#define ATL_RUNCOMM     -11	      // Unterminated comment in file
#define ATL_BREAK       -12	      // Asynchronous break signal received
#define ATL_DIVZERO     -13	      // Attempt to divide by zero
#define ATL_APPLICATION -14	      // Application primitive atl_error()

//  Entry points
void atl_break(void);
int  atl_eval(char *sp);
int  atl_load(FILE *fp);
void atl_init(void);
void atl_mark(atl_statemark *mp);
void atl_memstat(void);
void atl_unwind(atl_statemark *mp);

#endif
// end of ATLast/atlast.h

// Word flag bits

#define IMMEDIATE   1		      // Word is immediate
#define WORDUSED    2		      // Word used by program
#define WORDHIDDEN  4		      // Word is hidden from lookup

// Stack items occupied by a dictionary word definition
#define Dictwordl ((sizeof(dictword)+(sizeof(stackitem)-1))/sizeof(stackitem))

// Token types

#define TokNull     0		      // Nothing scanned
#define TokWord     1		      // Word stored in token name buffer
#define TokInt	    2		      // Integer scanned
#define TokReal     3		      // Real number scanned
#define TokString   4		      // String scanned

#ifdef EXPORT
#   define Exported
#   ifdef ALIGNMENT
// TODO: move these to the altenv structure
atl_real rbuf0, rbuf1, rbuf2;  // Real temporaries for alignment
#   endif

#   define FmodeR       1   // Read mode
#   define FmodeW       2   // Write mode
#   define FmodeB       4   // Binary file mode
#   define FmodeCre     8   // Create new file

// TODO: move these to the altenv structure

void P_create(void);
void P_dodoes(void);
#else  // EXPORT
#   define Exported static
#endif // EXPORT

void stakover(void);
void rstakover(void);
void heapover(void);
void badpointer(void);
void stakunder(void);
void rstakunder(void);

// If explicit alignment is not requested, enable it in any case for known CPU types that require alignment.

#ifndef ALIGNMENT
#   ifdef sparc
#       define ALIGNMENT
#   endif
#endif
#ifdef __TURBOC__
#   define  Keyhit()   (kbhit() ? getch() : 0)
    // DOS needs poll to detect break
#   define  Keybreak() { static int n=0; if ((n = (n+1) & 127) == 0) V kbhit(); }
#   ifdef __MSDOS__
#       define MSDOS
#   endif
#endif
#ifdef MSDOS
#   define FBmode			      // DOS requires binary file flag
#endif
#ifdef Macintosh
#   define FBmode			      // Macintosh requires binary file flag
#endif
#ifdef OS2			      // OS/2 requires binary file flag
#   define FBmode
#endif

// STATIC is defined as "static" on platforms which require this specification on declarations for forward-referenced functions.

#define STATIC static

// Dynamic storage manipulation primitives

//  Stack access definitions

#define S0      atl__env->stk[-1]             // Top of stack
#define S1      atl__env->stk[-2]             // Next on stack
#define S2      atl__env->stk[-3]             // Third on stack
#define S3      atl__env->stk[-4]             // Fourth on stack
#define S4      atl__env->stk[-5]             // Fifth on stack
#define S5      atl__env->stk[-6]             // Sixth on stack
#define Pop     atl__env->stk--               // Pop the top item off the stack
#define Pop2    atl__env->stk -= 2            // Pop two items off the stack
#define Npop(n) atl__env->stk -= (n)          // Pop N items off the stack
#define Push    *atl__env->stk++              // Push item onto stack

#ifdef MEMSTAT
#define Mss(n) if ((atl__env->stk+(n))>atl__env->stkMaxExtent) atl__env->stkMaxExtent = atl__env->stk+(n);
#define Msr(n) if ((atl__env->rs+(n))>atl__env->rsMaxExtent) atl__env->rsMaxExtent = atl__env->rs+(n);
#define Msh(n) if ((atl__env->heapAllocPtr+(n))>atl__env->heapMaxExtent) atl__env->heapMaxExtent = atl__env->heapAllocPtr+(n);
#else
#define Mss(n)
#define Msr(n)
#define Msh(n)
#endif

#ifdef NOMEMCHECK
#   define Sl(x)
#   define So(n)
#else
#   define Memerrs
#   define Sl(x) if ((atl__env->stk-atl__env->stack)<(x)) {stakunder(); return Memerrs;}
#   define So(n) Mss(n) if ((atl__env->stk+(n))>atl__env->stkTop) {stakover(); return Memerrs;}
#endif

/*  Return stack access definitions  */

#define R0  atl__env->rs[-1]		      /* Top of return stack */
#define R1  atl__env->rs[-2]		      /* Next on return stack */
#define R2  atl__env->rs[-3]		      /* Third on return stack */
#define Rpop atl__env->rs--		      /* Pop return stack */
#define Rpush *atl__env->rs++		      /* Push return stack */
#ifdef NOMEMCHECK
#   define Rsl(x)
#   define Rso(n)
#else
#   define Rsl(x) if ((atl__env->rs-atl__env->rstack)<(x)) {rstakunder(); return Memerrs;}
#   define Rso(n) Msr(n) if ((atl__env->rs+(n))>atl__env->rsTop){rstakover(); return Memerrs;}
#endif

/*  Heap access definitions  */

#ifdef NOMEMCHECK
#   define Ho(n)
#   define Hpc(n)
#else
#   define Ho(n)  Msh(n) if ((atl__env->heapAllocPtr+(n))>atl__env->heapTop){heapover(); return Memerrs;}
#   define Hpc(n) if ((((stackitem *)(n))<atl__env->heapBottom)||(((stackitem *)(n))>=atl__env->heapTop)){badpointer(); return Memerrs;}
#endif
#define Hstore *atl__env->heapAllocPtr++		      /* Store item on heap */
#define state  (*atl__env->heap)		      /* Execution state is first heap word */

#define prim static void	      /* Attributes of primitive functions */

/*  Real number definitions (used only if REAL is configured).	*/

#define Realsize (sizeof(atl_real)/sizeof(stackitem)) /* Stack cells / real */
#define Realpop  atl__env->stk -= Realsize      /* Pop real from stack */
#define Realpop2 atl__env->stk -= (2 * Realsize) /* Pop two reals from stack */

#ifdef ALIGNMENT
#   define REAL0        *((atl_real *) memcpy((char *) &rbuf0, (char *) &S1, sizeof(atl_real)))
#   define REAL1        *((atl_real *) memcpy((char *) &rbuf1, (char *) &S3, sizeof(atl_real)))
#   define REAL2        *((atl_real *) memcpy((char *) &rbuf2, (char *) &S5, sizeof(atl_real)))
#   define SREAL0(x)    rbuf2=(x); (void)memcpy((char *) &S1, (char *) &rbuf2, sizeof(atl_real))
#   define SREAL1(x)    rbuf2=(x); (void)memcpy((char *) &S3, (char *) &rbuf2, sizeof(atl_real))
#else
#   define REAL0        *((atl_real *) &S1)   /* First real on stack */
#   define REAL1        *((atl_real *) &S3)   /* Second real on stack */
#   define REAL2        *((atl_real *) &S5)   /* Third real on stack */
#   define SREAL0(x)    *((atl_real *) &S1) = (x)
#   define SREAL1(x)    *((atl_real *) &S3) = (x)
#endif

/*  File I/O definitions (used only if FILEIO is configured).  */

#define FileSent    0x831FDF9DL       /* Courtesy Marinchip Radioactive random number generator */
#define Isfile(x)   Hpc(x); if (*((stackitem *)(x))!=FileSent) {fprintf(stderr, "\nnot a file\n");return;}
#define FileD(x)    ((FILE *) *(((stackitem *) (x)) + 1))
#define Isopen(x)   if (FileD(x) == NULL) {fprintf(stderr, "\nfile not open\n");return;}

#endif
// end of ATLast/atldef.h




//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

static const atl_int atlFalsity = 0L;           // value for falsity
static const atl_int atlTruth   = ~atlFalsity;  // value for truth

typedef struct atlenv atlenv;
struct atlenv {
    // public -- visible to calling programs
    atl_int allowRedefinition;          // Allow redefinition without issuing the "not unique" message.
    atl_int enableTrace;                // Tracing if true
    atl_int enableWalkback;             // Walkback enabled if true
    atl_int heapLength;                 // Heap length
    atl_int isIgnoringComment;          // Currently ignoring a comment
    atl_int lengthTempStringBuffer;     // Temporary string buffer length
    atl_int lineNumberLastLoadFailed;   // Line where last atl_load failed or zero if no error
    atl_int numberOfTempStringBuffers;  // Number of temporary string buffers
    atl_int rsLength;                   // Return stack length
    atl_int stkLength;                  // Evaluation stack length

    // private
    dictword   *createWord;             //  address of word pending creation
    dictword   *currentWord;            // Current word being executed
    dictword   *dict;                   // dictionary chain head
    dictword   *dictFirstProtectedEntry;// first protected item in dictionary
    stackitem  *heap;                   // allocation heap
    stackitem  *heapAllocPtr;           // heap allocation pointer
    stackitem  *heapBottom;             // bottom of heap (temp string buffer)
    stackitem  *heapMaxExtent;          // heap maximum excursion
    stackitem  *heapTop;                // top of heap
    int         idxCurrTempStringBuffer;// index into current temp string buffer
    char       *inputBuffer;            // current input buffer
    dictword  **ip;                     // instruction pointer
    int       (*nextToken)(char **cp);
    dictword ***rstack;                 // return stack, root of allocated memory for the stack
    dictword ***rs;                     // return stack pointer
    dictword ***rsBottom;               // return stack bottom
    dictword ***rsMaxExtent;            // return stack maximum excursion
    dictword ***rsTop;                  // return stack top
    stackitem  *stack;                  // evaluation stack, root of allocated memory for the stack
    stackitem  *stk;                    // stack pointer
    stackitem  *stkBottom;              // pointer to stack bottom
    stackitem  *stkMaxExtent;           // stack maximum excursion
    stackitem  *stkTop;                 // pointer to stack top
    dictword  **walkback;               // walkback trace buffer
    dictword  **walkbackPointer;        // walkback trace pointer (stack trace?)

    // TODO: rename these
    char      **strbuf;                 // table of pointers to temp strings

    // real temporaries for alignment
    atl_real    rbuf0;
    atl_real    rbuf1;
    atl_real    rbuf2;
};
atlenv *atl__env = 0;

atlenv *atl__NewInterpreter(void) {
    atlenv *e = malloc(sizeof(*e));
    if (!e) {
        return e;
    }

    // assign default private values (TODO: allocate memory)
    e->createWord       = 0;
    e->currentWord      = 0;
    e->dict             = 0;
    e->dictFirstProtectedEntry  = 0;
    e->heap             = 0;
    e->heapAllocPtr     = 0;
    e->heapBottom       = 0;
    e->heapMaxExtent    = 0;
    e->heapTop          = 0;
    e->idxCurrTempStringBuffer = 0;
    e->inputBuffer      = 0;
    e->ip               = 0;
    e->nextToken        = atl__token;
    e->rstack           = 0;
    e->rs               = 0;
    e->rsBottom         = 0;
    e->rsMaxExtent      = 0;
    e->rsTop            = 0;
    e->stack            = 0;
    e->stk              = 0;
    e->stkBottom        = 0;
    e->stkMaxExtent     = 0;
    e->stkTop           = 0;
    e->walkback         = 0;
    e->walkbackPointer  = 0;

    e->strbuf           = 0;

    // assign default public values
    e->allowRedefinition            = atlTruth;
    e->enableTrace                  = atlFalsity;
    e->enableWalkback               = atlTruth;
    e->heapLength                   = 1000;
    e->isIgnoringComment            = atlFalsity;
    e->lengthTempStringBuffer       =  256;
    e->lineNumberLastLoadFailed     =    0;
    e->numberOfTempStringBuffers    =    4;
    e->rsLength                     =  100;
    e->stkLength                    =  100;

    return e;
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------



// A T L A S T
//
// Autodesk Threaded Language Application System Toolkit
//
//  ATLast/atlast.c
//
//  Derived from code designed and implemented 1990 by John Walker
//  and placed into the public domain by him.
//  Original at https://www.fourmilab.ch/atlast/atlast.html
//
//  Created by Michael Henderson on 6/19/14.
//  Changes Copyright (c) 2014 Michael D Henderson. All rights reserved.
//
// Main Interpreter and Compiler
//
//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef ALIGNMENT
#ifdef __TURBOC__
#include <mem.h>
#else
#include <memory.h>
#endif
#endif

/*  Custom configuration.  If the tag CUSTOM has been defined (usually on
 the compiler call line), we include the file "atlcfig.h", which may
 then define INDIVIDUALLY and select the subpackages needed for its
 application.  */

#ifdef CUSTOM
#include "atlcfig.h"
#endif

/*  Subpackage configuration.  If INDIVIDUALLY is defined, the inclusion
 of subpackages is based on whether their compile-time tags are
 defined.  Otherwise, we automatically enable all the subpackages.  */

#ifndef INDIVIDUALLY
#   define ARRAY                   /* Array subscripting words */
#   define BREAK                   /* Asynchronous break facility */
#   define COMPILERW               /* Compiler-writing words */
#   define CONIO                   /* Interactive console I/O */
#   define DEFFIELDS               /* Definition field access for words */
#   define DOUBLE                  /* Double word primitives (2DUP) */
#   define EVALUATE                /* The EVALUATE primitive */
#   define FILEIO                  /* File I/O primitives */
#   define MATH                    /* Math functions */
#   define MEMMESSAGE              /* Print message for stack/heap errors */
#   define PROLOGUE                /* Prologue processing and auto-init */
#   define REAL                    /* Floating point numbers */
#   define SHORTCUTA               /* Shortcut integer arithmetic words */
#   define SHORTCUTC               /* Shortcut integer comparison */
#   define STRING                  /* String functions */
#   define SYSTEM                  /* System command function */
#   ifndef NOMEMCHECK
#       define TRACE               /* Execution tracing */
#       define WALKBACK            /* Walkback trace */
#       define WORDSUSED           /* Logging of words used and unused */
#   endif /* NOMEMCHECK */
#endif /* !INDIVIDUALLY */

#include "atldef.h"

#ifdef MATH
#   include <math.h>
#endif

/* LINTLIBRARY */

/* Implicit functions (work for all numeric types). */

#ifdef abs
#   undef abs
#endif
#define abs(x)	 ((x) < 0    ? -(x) : (x))
#define max(a,b) ((a) >  (b) ?	(a) : (b))
#define min(a,b) ((a) <= (b) ?	(a) : (b))

/*  Globals imported  */

/*  Data types	*/

#define EOS     '\0'                  /* End of string characters */

#define V	(void)		      /* Force result to void */

/* Utility definition to get an  array's  element  count  (at  compile time).   For  example:

 int  arr[] = {1,2,3,4,5};
 ...
 printf("%d", ELEMENTS(arr));

 would print a five.  ELEMENTS("abc") can also be used to  tell  how
 many  bytes are in a string constant INCLUDING THE TRAILING NULL. */

#define ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))

#ifdef FILEIO
// TODO: move these
static char *fopenmodes[] = {
#ifdef FBmode
#   define FMspecial
    /* Fopen() mode table for systems that require a "b" in the mode string for binary files. */
    "", "r",  "",   "r+",
    "", "rb", "",   "r+b",
    "", "",   "w",  "w+",
    "", "",   "wb", "w+b"
#endif
#ifndef FMspecial
    /* Default fopen() mode table for SVID-compatible systems not overridden by a special table above. */
    "", "r",  "",   "r+",
    "", "r",  "",   "r+",
    "", "",   "w",  "w+",
    "", "",   "w",  "w+"
#endif
};
#endif /* FILEIO */

// TODO: move these
static char tokbuf[128];	      /* Token buffer */
static long tokint;		      /* Scanned integer */
#ifdef REAL
    static atl_real tokreal;	      /* Scanned real number */
#   ifdef ALIGNMENT
        Exported atl_real rbuf0, rbuf1, rbuf2; /* Real temporary buffers */
#   endif
#endif
static long base = 10;		      /* Number base */
static int evalstat = ATL_SNORM;      /* Evaluator status */
static Boolean defpend = atlFalse;       /* Token definition pending */
static Boolean forgetpend = atlFalse;    /* Forget pending */
static Boolean tickpend = atlFalse;      /* Take address of next word */
static Boolean ctickpend = atlFalse;     /* Compile-time tick ['] pending */
static Boolean cbrackpend = atlFalse;    /* [COMPILE] pending */
static Boolean stringlit = atlFalse;     /* String literal anticipated */

// TODO: move these
#ifdef BREAK
static volatile Boolean broken = atlFalse;  // asynchronous break received
#endif

#ifdef COPYRIGHT
#   ifndef lint
static
#   endif
// TODO: move these
char copyright[] = "ATLAST: Changes Copyright (C) 2014 Michael Henderson.";
#endif

/* The following static cells save the compile addresses of words
 generated by the compiler.  They are looked up immediately after
 the dictionary is created.  This makes the compiler much faster
 since it doesn't have to look up internally-reference words, and,
 far more importantly, keeps it from being spoofed if a user redefines
 one of the words generated by the compiler.	*/

// TODO: move these
static stackitem s_exit, s_lit, s_flit, s_strlit, s_dotparen, s_qbranch, s_branch, s_xdo, s_xqdo, s_xloop, s_pxloop, s_abortq;

/*  Forward functions  */

STATIC void exword(dictword *wp), trouble(char *kind);
#ifndef NOMEMCHECK
STATIC void notcomp(void), divzero(void);
#endif
#ifdef WALKBACK
STATIC void pwalkback(void);
#endif

/*  ALLOC  --  Allocate memory and error upon exhaustion.  */

static char *alloc(unsigned int size) {
    char *cp = malloc(size);

    /* printf("\nAlloc %u", size); */
    if (cp == NULL) {
        fprintf(stderr, "\n\nOut of memory!  %u bytes requested.\n", size);
        abort();
    }
    return cp;
}

/*  UCASE  --  Force letters in string to upper case.  */

static void ucase(char *c) {
    char ch;

    while ((ch = *c) != EOS) {
        if (islower(ch)) {
            *c = toupper(ch);
        }
        c++;
    }
}

/*  TOKEN  --  Scan a token and return its type.  */

int atl__token(char **cp) {
    char *sp = *cp;

    while (atlTrue) {
        char *tp = tokbuf;
        int tl = 0;
        Boolean istring = atlFalse, rstring = atlFalse;

        if (atl__env->isIgnoringComment) {
            while (*sp != ')') {
                if (*sp == EOS) {
                    *cp = sp;
                    return TokNull;
                }
                sp++;
            }
            sp++;
            atl__env->isIgnoringComment = atlFalsity;
        }

        while (isspace(*sp))		  /* Skip leading blanks */
            sp++;

        if (*sp == '"') {                 /* Is this a string ? */

            /* Assemble string token. */

            sp++;
            while (atlTrue) {
                char c = *sp++;

                if (c == '"') {
                    sp++;
                    *tp++ = EOS;
                    break;
                } else if (c == EOS) {
                    rstring = atlTrue;
                    *tp++ = EOS;
                    break;
                }
                if (c == '\\') {
                    c = *sp++;
                    if (c == EOS) {
                        rstring = atlTrue;
                        break;
                    }
                    switch (c) {
                        case 'b':
                            c = '\b';
                            break;
                        case 'n':
                            c = '\n';
                            break;
                        case 'r':
                            c = '\r';
                            break;
                        case 't':
                            c = '\t';
                            break;
                        default:
                            break;
                    }
                }
                if (tl < (sizeof tokbuf) - 1) {
                    *tp++ = c;
                    tl++;
                } else {
                    rstring = atlTrue;
                }
            }
            istring = atlTrue;
        } else {

            /* Scan the next raw token */

            while (atlTrue) {
                char c = *sp++;

                if (c == EOS || isspace(c)) {
                    *tp++ = EOS;
                    break;
                }
                if (tl < (sizeof tokbuf) - 1) {
                    *tp++ = c;
                    tl++;
                }
            }
        }
        *cp = --sp;			  /* Store end of scan pointer */

        if (istring) {
            if (rstring) {
#ifdef MEMMESSAGE
                fprintf(stderr, "\nrunaway string: %s\n", tokbuf);
#endif
                evalstat = ATL_RUNSTRING;
                return TokNull;
            }
            return TokString;
        }

        if (tokbuf[0] == EOS)
            return TokNull;

        /* See if token is a comment to end of line character.	If so, discard
         the rest of the line and return null for this token request. */

        if (strcmp(tokbuf, "\\") == 0) {
            while (*sp != EOS)
                sp++;
            *cp = sp;
            return TokNull;
        }

        /* See if this token is a comment open delimiter.  If so, set to
         ignore all characters until the matching comment close delimiter. */

        if (strcmp(tokbuf, "(") == 0) {
            while (*sp != EOS) {
                if (*sp == ')')
                    break;
                sp++;
            }
            if (*sp == ')') {
                sp++;
                continue;
            }
            atl__env->isIgnoringComment = atlTruth;
            *cp = sp;
            return TokNull;
        }

        /* See if the token is a number. */

        if (isdigit(tokbuf[0]) || tokbuf[0] == '-') {
            char tc;
            char *tcp;

#ifdef OS2
            /* Compensate for error in OS/2 sscanf() library function */
            if ((tokbuf[0] == '-') &&
                !(isdigit(tokbuf[1]) ||
                  (((tokbuf[1] == 'x') || (tokbuf[1] == 'X')) &&
                   isxdigit(tokbuf[2])))) {
                      return TokWord;
                  }
#endif /* OS2 */
#ifdef USE_SSCANF
            if (sscanf(tokbuf, "%li%c", &tokint, &tc) == 1)
                return TokInt;
#else
    	    tokint = strtoul(tokbuf, &tcp, 0);
            if (*tcp == 0) {
                return TokInt;
            }
#endif
#ifdef REAL
            if (sscanf(tokbuf, "%lf%c", &tokreal, &tc) == 1)
                return TokReal;
#endif
        }
        return TokWord;
    }
}

/*  LOOKUP  --	Look up token in the dictionary.  */

static dictword *lookup(char *tkname) {
    dictword *dw = atl__env->dict;

    ucase(tkname);		      /* Force name to upper case */
    while (dw != NULL) {
        if (!(dw->wname[0] & WORDHIDDEN) &&
            (strcmp(dw->wname + 1, tkname) == 0)) {
#ifdef WORDSUSED
            *(dw->wname) |= WORDUSED; /* Mark this word used */
#endif
            break;
        }
        dw = dw->wnext;
    }
    return dw;
}

/* Gag me with a spoon!  Does no compiler but Turbo support #if defined(x) || defined(y) ?? */
#ifdef EXPORT
#define FgetspNeeded
#endif
#ifdef FILEIO
#ifndef FgetspNeeded
#define FgetspNeeded
#endif
#endif

#ifdef FgetspNeeded

/*  ATL_FGETSP	--  Portable database version of FGETS.  This reads the
 next line into a buffer a la fgets().  A line is
 delimited by either a carriage return or a line
 feed, optionally followed by the other character
 of the pair.  The string is always null
 terminated, and limited to the length specified - 1
 (excess characters on the line are discarded.
 The string is returned, or NULL if end of file is
 encountered and no characters were stored.	No end
 of line character is stored in the string buffer.
 */

Exported char *atl_fgetsp(char *s, int n, FILE *stream) {
	int i = 0, ch;

	while (atlTrue) {
        ch = getc(stream);
        if (ch == EOF) {
            if (i == 0)
                return NULL;
            break;
        }
        if (ch == '\r') {
            ch = getc(stream);
            if (ch != '\n')
                V ungetc(ch, stream);
            break;
        }
        if (ch == '\n') {
            ch = getc(stream);
            if (ch != '\r')
                V ungetc(ch, stream);
            break;
        }
        if (i < (n - 1))
            s[i++] = ch;
	}
	s[i] = EOS;
	return s;
}
#endif /* FgetspNeeded */

/*  ATL_MEMSTAT  --  Print memory usage summary.  */

#ifdef MEMSTAT
void atl_memstat(void) {
    fprintf(stderr, "\n             Memory Usage Summary\n\n");
    fprintf(stderr, "                 Current   Maximum    Items     Percent\n");
    fprintf(stderr, "  Memory Area     usage     used    allocated   in use \n");

    fprintf(stderr, "   %-12s %6ld    %6ld    %6ld       %3ld\n", "Stack",
            ((long) (atl__env->stk - atl__env->stack)),
            ((long) (atl__env->stkMaxExtent - atl__env->stack)),
            atl__env->stkLength,
            (100L * (atl__env->stk - atl__env->stack)) / atl__env->stkLength);
    fprintf(stderr, "   %-12s %6ld    %6ld    %6ld       %3ld\n", "Return stack",
            ((long) (atl__env->rs - atl__env->rstack)),
            ((long) (atl__env->rsMaxExtent - atl__env->rstack)),
            atl__env->rsLength,
            (100L * (atl__env->rs - atl__env->rstack)) / atl__env->rsLength);
    fprintf(stderr, "   %-12s %6ld    %6ld    %6ld       %3ld\n", "Heap",
            ((long) (atl__env->heapAllocPtr - atl__env->heap)),
            ((long) (atl__env->heapMaxExtent - atl__env->heap)),
            atl__env->heapLength,
            (100L * (atl__env->heapAllocPtr - atl__env->heap)) / atl__env->heapLength);
}
#endif /* MEMSTAT */

/*  Primitive implementing functions.  */

/*  ENTER  --  Enter word in dictionary.  Given token for word's
 name and initial values for its attributes, returns
 the newly-allocated dictionary item. */

static void enter(char *tkname) {
    /* Allocate name buffer */
    atl__env->createWord->wname = alloc(((unsigned int) strlen(tkname) + 2));
    atl__env->createWord->wname[0] = 0;	      /* Clear flags */
    strcpy(atl__env->createWord->wname + 1, tkname); /* Copy token to name buffer */
    atl__env->createWord->wnext = atl__env->dict;	      /* Chain rest of dictionary to word */
    atl__env->dict = atl__env->createWord;		      /* Put word at head of dictionary */
}

#ifdef Keyhit

/*  KBQUIT  --	If this system allows detecting key presses, handle
 the pause, resume, and quit protocol for the word
 listing facilities.  */

static Boolean kbquit(void) {
    int key;

    if ((key = Keyhit()) != 0) {
        fprintf(stderr, "\npress RETURN to stop, any other key to continue: ");
        while ((key = Keyhit()) == 0) ;
        if (key == '\r' || (key == '\n'))
            return True;
    }
    return False;
}
#endif /* Keyhit */

/*  Primitive word definitions.  */

#ifdef NOMEMCHECK
#define Compiling
#else
#define Compiling if (state == atlFalsity) {notcomp(); return;}
#endif
#define Compconst(x) Ho(1); Hstore = (stackitem) (x)
#define Skipstring atl__env->ip += *((char *) atl__env->ip)

/* Add two numbers */
prim P_plus(void) {
    Sl(2);
    /* printf("PLUS %lx + %lx = %lx\n", S1, S0, (S1 + S0)); */
    S1 += S0;
    Pop;
}

/* Subtract two numbers */
prim P_minus(void) {
    Sl(2);
    S1 -= S0;
    Pop;
}

/* Multiply two numbers */
prim P_times(void) {
    Sl(2);
    S1 *= S0;
    Pop;
}

/* Divide two numbers */
prim P_div(void) {
    Sl(2);
#ifndef NOMEMCHECK
    if (S0 == 0) {
        divzero();
        return;
    }
#endif /* NOMEMCHECK */
    S1 /= S0;
    Pop;
}

/* Take remainder */
prim P_mod(void) {
    Sl(2);
#ifndef NOMEMCHECK
    if (S0 == 0) {
        divzero();
        return;
    }
#endif /* NOMEMCHECK */
    S1 %= S0;
    Pop;
}

/* Compute quotient and remainder */
prim P_divmod(void) {
    stackitem quot;

    Sl(2);
#ifndef NOMEMCHECK
    if (S0 == 0) {
        divzero();
        return;
    }
#endif /* NOMEMCHECK */
    quot = S1 / S0;
    S1 %= S0;
    S0 = quot;
}

/* Take minimum of stack top */
prim P_min(void) {
    Sl(2);
    S1 = min(S1, S0);
    Pop;
}

/* Take maximum of stack top */
prim P_max(void) {
    Sl(2);
    S1 = max(S1, S0);
    Pop;
}

/* Negate top of stack */
prim P_neg(void) {
    Sl(1);
    S0 = - S0;
}

/* Take absolute value of top of stack */
prim P_abs(void) {
    Sl(1);
    S0 = abs(S0);
}

/* Test equality */
prim P_equal(void) {
    Sl(2);
    S1 = (S1 == S0) ? atlTruth : atlFalsity;
    Pop;
}

/* Test inequality */
prim P_unequal(void) {
    Sl(2);
    S1 = (S1 != S0) ? atlTruth : atlFalsity;
    Pop;
}

/* Test greater than */
prim P_gtr(void) {
    Sl(2);
    S1 = (S1 > S0) ? atlTruth : atlFalsity;
    Pop;
}

/* Test less than */
prim P_lss(void) {
    Sl(2);
    S1 = (S1 < S0) ? atlTruth : atlFalsity;
    Pop;
}

/* Test greater than or equal */
prim P_geq(void) {
    Sl(2);
    S1 = (S1 >= S0) ? atlTruth : atlFalsity;
    Pop;
}

/* Test less than or equal */
prim P_leq(void) {
    Sl(2);
    S1 = (S1 <= S0) ? atlTruth : atlFalsity;
    Pop;
}

/* Logical and */
prim P_and(void) {
    Sl(2);
    /* printf("AND %lx & %lx = %lx\n", S1, S0, (S1 & S0)); */
    S1 &= S0;
    Pop;
}

/* Logical or */
prim P_or(void) {
    Sl(2);
    S1 |= S0;
    Pop;
}

/* Logical xor */
prim P_xor(void) {
    Sl(2);
    S1 ^= S0;
    Pop;
}

/* Logical negation */
prim P_not(void) {
    Sl(1);
    S0 = ~S0;
}

/* Shift:  value nbits -- value */
prim P_shift(void) {
    Sl(1);
    S1 = (S0 < 0) ? (((unsigned long) S1) >> (-S0)) :
    (((unsigned long) S1) <<   S0);
    Pop;
}

#ifdef SHORTCUTA

/* Add one */
prim P_1plus(void) {
    Sl(1);
    S0++;
}

/* Add two */
prim P_2plus(void) {
    Sl(1);
    S0 += 2;
}

/* Subtract one */
prim P_1minus(void) {
    Sl(1);
    S0--;
}

/* Subtract two */
prim P_2minus(void) {
    Sl(1);
    S0 -= 2;
}

/* Multiply by two */
prim P_2times(void) {
    Sl(1);
    S0 *= 2;
}

/* Divide by two */
prim P_2div(void) {
    Sl(1);
    S0 /= 2;
}

#endif /* SHORTCUTA */

#ifdef SHORTCUTC

/* Equal to zero ? */
prim P_0equal(void) {
    Sl(1);
    S0 = (S0 == 0) ? atlTruth : atlFalsity;
}

/* Not equal to zero ? */
prim P_0notequal(void) {
    Sl(1);
    S0 = (S0 != 0) ? atlTruth : atlFalsity;
}

/* Greater than zero ? */
prim P_0gtr(void) {
    Sl(1);
    S0 = (S0 > 0) ? atlTruth : atlFalsity;
}

/* Less than zero ? */
prim P_0lss(void) {
    Sl(1);
    S0 = (S0 < 0) ? atlTruth : atlFalsity;
}

#endif /* SHORTCUTC */

/*  Storage allocation (heap) primitives  */

/* Push current heap address */
prim P_here(void) {
    So(1);
    Push = (stackitem) atl__env->heapAllocPtr;
}

/* Store value into address */
prim P_bang(void) {
    Sl(2);
    Hpc(S0);
    *((stackitem *) S0) = S1;
    Pop2;
}

/* Fetch value from address */
prim P_at(void) {
    Sl(1);
    Hpc(S0);
    S0 = *((stackitem *) S0);
}

/* Add value at specified address */
prim P_plusbang(void) {
    Sl(2);
    Hpc(S0);
    *((stackitem *) S0) += S1;
    Pop2;
}

/* Allocate heap bytes */
prim P_allot(void) {
    stackitem n;

    Sl(1);
    n = (S0 + (sizeof(stackitem) - 1)) / sizeof(stackitem);
    Pop;
    Ho(n);
    atl__env->heapAllocPtr += n;
}

/* Store one item on heap */
prim P_comma(void) {
    Sl(1);
    Ho(1);
    Hstore = S0;
    Pop;
}

/* Store byte value into address */
prim P_cbang(void) {
    Sl(2);
    Hpc(S0);
    *((unsigned char *) S0) = S1;
    Pop2;
}

/* Fetch byte value from address */
prim P_cat(void) {
    Sl(1);
    Hpc(S0);
    S0 = *((unsigned char *) S0);
}

/* Store one byte on heap */
prim P_ccomma(void) {
    unsigned char *chp;

    Sl(1);
    Ho(1);
    chp = ((unsigned char *) atl__env->heapAllocPtr);
    *chp++ = S0;
    atl__env->heapAllocPtr = (stackitem *) chp;
    Pop;
}

/* Align heap pointer after storing a series of bytes. */
prim P_cequal(void) {
    stackitem n = (((stackitem) atl__env->heapAllocPtr) - ((stackitem) atl__env->heap)) % (sizeof(stackitem));

    if (n != 0) {
        char *chp = ((char *) atl__env->heapAllocPtr);

        chp += sizeof(stackitem) - n;
        atl__env->heapAllocPtr = ((stackitem *) chp);
    }
}

/*  Variable and constant primitives  */

/* Push body address of current word */
prim P_var(void) {
    So(1);
    Push = (stackitem) (((stackitem *) atl__env->currentWord) + Dictwordl);
}

Exported void P_create(void) {	      /* Create new word */
    defpend = atlTrue;		      /* Set definition pending */
    Ho(Dictwordl);
    atl__env->createWord = (dictword *) atl__env->heapAllocPtr;   /* Develop address of word */
    atl__env->createWord->wname = NULL;	      /* Clear pointer to name string */
    atl__env->createWord->wcode = P_var;	      /* Store default code */
    atl__env->heapAllocPtr += Dictwordl;		      /* Allocate heap space for word */
}

/* Forget word */
prim P_forget(void) {
    forgetpend = atlTrue;		      /* Mark forget pending */
}

/* Declare variable */
prim P_variable(void) {
    P_create(); 		      /* Create dictionary item */
    Ho(1);
    Hstore = 0; 		      /* Initial value = 0 */
}

/* Push value in body */
prim P_con(void) {
    So(1);
    Push = *(((stackitem *) atl__env->currentWord) + Dictwordl);
}

/* Declare constant */
prim P_constant(void) {
    Sl(1);
    P_create(); 		      /* Create dictionary item */
    atl__env->createWord->wcode = P_con;	      /* Set code to constant push */
    Ho(1);
    Hstore = S0;		      /* Store constant value in body */
    Pop;
}

/*  Array primitives  */

#ifdef ARRAY
/* Array subscript calculation sub1 sub2 ... subn -- addr */
prim P_arraysub(void) {
    int i;
    long offset, esize, nsubs;
    stackitem *array;
    stackitem *isp;

    Sl(1);
    array = (((stackitem *) atl__env->currentWord) + Dictwordl);
    Hpc(array);
    nsubs = *array++;		      /* Load number of subscripts */
    esize = *array++;		      /* Load element size */
#ifndef NOMEMCHECK
    isp = &S0;
    for (i = 0; i < nsubs; i++) {
        stackitem subn = *isp--;

        if (subn < 0 || subn >= array[i])
            trouble("Subscript out of range");
    }
#endif /* NOMEMCHECK */
    isp = &S0;
    offset = *isp;		      /* Load initial offset */
    for (i = 1; i < nsubs; i++)
        offset = (offset * (*(++array))) + *(--isp);
    Npop(nsubs - 1);
    /* Calculate subscripted address.  We start at the current word,
     advance to the body, skip two more words for the subscript count
     and the fundamental element size, then skip the subscript bounds
     words (as many as there are subscripts).  Then, finally, we
     can add the calculated offset into the array. */
    S0 = (stackitem) (((char *) (((stackitem *) atl__env->currentWord) + Dictwordl + 2 + nsubs)) + (esize * offset));
}

/* Declare array sub1 sub2 ... subn n esize -- array */
prim P_array(void) {
    int i;
    long nsubs, asize = 1;
    stackitem *isp;

    Sl(2);
#ifndef NOMEMCHECK
    if (S0 <= 0) {
        trouble("Bad array element size");
    }
    if (S1 <= 0) {
        trouble("Bad array subscript count");
    }
#endif /* NOMEMCHECK */

    nsubs = S1; 		      /* Number of subscripts */
    Sl(nsubs + 2);		      /* Verify that dimensions are present */

    /* Calculate size of array as the product of the subscripts */

    asize = S0; 		      /* Fundamental element size */
    isp = &S2;
    for (i = 0; i < nsubs; i++) {
#ifndef NOMEMCHECK
        if (*isp <= 0)
            trouble("Bad array dimension");
#endif /* NOMEMCHECK */
        asize *= *isp--;
    }

    asize = (asize + (sizeof(stackitem) - 1)) / sizeof(stackitem);
    Ho(asize + nsubs + 2);	      /* Reserve space for array and header */
    P_create(); 		      /* Create variable */
    atl__env->createWord->wcode = P_arraysub;   /* Set method to subscript calculate */
    Hstore = nsubs;		      /* Header <- Number of subscripts */
    Hstore = S0;		      /* Header <- Fundamental element size */
    isp = &S2;
    for (i = 0; i < nsubs; i++) {     /* Header <- Store subscripts */
        Hstore = *isp--;
    }
    while (asize-- > 0) 	      /* Clear the array to zero */
        Hstore = 0;
    Npop(nsubs + 2);
}
#endif /* ARRAY */

/*  String primitives  */

#ifdef STRING

/* Push address of string literal */
prim P_strlit(void) {
    So(1);
    Push = (stackitem) (((char *) atl__env->ip) + 1);
#ifdef TRACE
    if (atl__env->enableTrace) {
        fprintf(stderr, "\"%s\" ", (((char *) atl__env->ip) + 1));
    }
#endif /* TRACE */
    Skipstring; 		      /* Advance IP past it */
}

/* Create string buffer */
prim P_string(void) {
    Sl(1);
    Ho((S0 + 1 + sizeof(stackitem)) / sizeof(stackitem));
    P_create(); 		      /* Create variable */
    /* Allocate storage for string */
    atl__env->heapAllocPtr += (S0 + 1 + sizeof(stackitem)) / sizeof(stackitem);
    Pop;
}

/* Copy string to address on stack */
prim P_strcpy(void) {
    Sl(2);
    Hpc(S0);
    Hpc(S1);
    V strcpy((char *) S0, (char *) S1);
    Pop2;
}

/* Append string to address on stack */
prim P_strcat(void) {
    Sl(2);
    Hpc(S0);
    Hpc(S1);
    V strcat((char *) S0, (char *) S1);
    Pop2;
}

/* Take length of string on stack top */
prim P_strlen(void) {
    Sl(1);
    Hpc(S0);
    S0 = strlen((char *) S0);
}

/* Compare top two strings on stack */
prim P_strcmp(void) {
    int i;

    Sl(2);
    Hpc(S0);
    Hpc(S1);
    i = strcmp((char *) S1, (char *) S0);
    S1 = (i == 0) ? 0L : ((i > 0) ? 1L : -1L);
    Pop;
}

/* Find character in string */
prim P_strchar(void) {
    Sl(2);
    Hpc(S0);
    Hpc(S1);
    S1 = (stackitem) strchr((char *) S1, *((char *) S0));
    Pop;
}

/* Extract and store substring  source start length/-1 dest -- */
prim P_substr(void) {
    long sl, sn;
    char *ss, *sp, *se, *ds;

    Sl(4);
    Hpc(S0);
    Hpc(S3);
    sl = strlen(ss = ((char *) S3));
    se = ss + sl;
    sp = ((char *) S3) + S2;
    if ((sn = S1) < 0)
        sn = 999999L;
    ds = (char *) S0;
    while (sn-- && (sp < se))
        *ds++ = *sp++;
    *ds++ = EOS;
    Npop(4);
}

/* Format integer using sprintf() value "%ld" str -- */
prim P_strform(void) {
    Sl(2);
    Hpc(S0);
    Hpc(S1);
    V sprintf((char *) S0, (char *) S1, S2);
    Npop(3);
}

#ifdef REAL
/* Format real using sprintf() rvalue "%6.2f" str -- */
prim P_fstrform(void) {
    Sl(4);
    Hpc(S0);
    Hpc(S1);
    V sprintf((char *) S0, (char *) S1, REAL1);
    Npop(4);
}
#endif /* REAL */

/* String to integer  str -- endptr value */
prim P_strint(void) {
    stackitem is;
    char *eptr;

    Sl(1);
    So(1);
    Hpc(S0);
    is = strtoul((char *) S0, &eptr, 0);
    S0 = (stackitem) eptr;
    Push = is;
}

#ifdef REAL
/* String to real  str -- endptr value */
prim P_strreal(void) {
    int i;
    union {
    	atl_real fs;
        stackitem fss[Realsize];
    } fsu;
    char *eptr;

    Sl(1);
    So(2);
    Hpc(S0);
    fsu.fs = strtod((char *) S0, &eptr);
    S0 = (stackitem) eptr;
    for (i = 0; i < Realsize; i++) {
    	Push = fsu.fss[i];
    }
}
#endif /* REAL */
#endif /* STRING */

/*  Floating point primitives  */

#ifdef REAL

/* Push floating point literal */
prim P_flit(void) {
    int i;

    So(Realsize);
#ifdef TRACE
    if (atl__env->enableTrace) {
        atl_real tr;

        memcpy((char *) &tr, (char *) atl__env->ip, sizeof(atl_real));
        fprintf(stderr, "%g ", tr);
    }
#endif /* TRACE */
    for (i = 0; i < Realsize; i++) {
        Push = (stackitem) *atl__env->ip++;
    }
}

/* Add floating point numbers */
prim P_fplus(void) {
    Sl(2 * Realsize);
    SREAL1(REAL1 + REAL0);
    Realpop;
}

/* Subtract floating point numbers */
prim P_fminus(void) {
    Sl(2 * Realsize);
    SREAL1(REAL1 - REAL0);
    Realpop;
}

/* Multiply floating point numbers */
prim P_ftimes(void) {
    Sl(2 * Realsize);
    SREAL1(REAL1 * REAL0);
    Realpop;
}

/* Divide floating point numbers */
prim P_fdiv(void) {
    Sl(2 * Realsize);
#ifndef NOMEMCHECK
    if (REAL0 == 0.0) {
        divzero();
        return;
    }
#endif /* NOMEMCHECK */
    SREAL1(REAL1 / REAL0);
    Realpop;
}

/* Minimum of top two floats */
prim P_fmin(void) {
    Sl(2 * Realsize);
    SREAL1(min(REAL1, REAL0));
    Realpop;
}

/* Maximum of top two floats */
prim P_fmax(void) {
    Sl(2 * Realsize);
    SREAL1(max(REAL1, REAL0));
    Realpop;
}

/* Negate top of stack */
prim P_fneg(void) {
    Sl(Realsize);
    SREAL0(- REAL0);
}

/* Absolute value of top of stack */
prim P_fabs(void) {
    Sl(Realsize);
    SREAL0(abs(REAL0));
}

/* Test equality of top of stack */
prim P_fequal(void) {
    stackitem t;

    Sl(2 * Realsize);
    t = (REAL1 == REAL0) ? atlTruth : atlFalsity;
    Realpop2;
    Push = t;
}

/* Test inequality of top of stack */
prim P_funequal(void) {
    stackitem t;

    Sl(2 * Realsize);
    t = (REAL1 != REAL0) ? atlTruth : atlFalsity;
    Realpop2;
    Push = t;
}

/* Test greater than */
prim P_fgtr(void) {
    stackitem t;

    Sl(2 * Realsize);
    t = (REAL1 > REAL0) ? atlTruth : atlFalsity;
    Realpop2;
    Push = t;
}

/* Test less than */
prim P_flss(void) {
    stackitem t;

    Sl(2 * Realsize);
    t = (REAL1 < REAL0) ? atlTruth : atlFalsity;
    Realpop2;
    Push = t;
}

/* Test greater than or equal */
prim P_fgeq(void) {
    stackitem t;

    Sl(2 * Realsize);
    t = (REAL1 >= REAL0) ? atlTruth : atlFalsity;
    Realpop2;
    Push = t;
}

/* Test less than or equal */
prim P_fleq(void) {
    stackitem t;

    Sl(2 * Realsize);
    t = (REAL1 <= REAL0) ? atlTruth : atlFalsity;
    Realpop2;
    Push = t;
}

/* Print floating point top of stack */
prim P_fdot(void) {
    Sl(Realsize);
    fprintf(stderr, "%g ", REAL0);
    Realpop;
}

/* Convert integer to floating */
prim P_float(void) {
    atl_real r;

    Sl(1)
    So(Realsize - 1);
    r = S0;
    atl__env->stk += Realsize - 1;
    SREAL0(r);
}

/* Convert floating to integer */
prim P_fix(void) {
    stackitem i;

    Sl(Realsize);
    i = (int) REAL0;
    Realpop;
    Push = i;
}

#ifdef MATH

#define Mathfunc(x) Sl(Realsize); SREAL0(x(REAL0))

/* Arc cosine */
prim P_acos(void) {
    Mathfunc(acos);
}

/* Arc sine */
prim P_asin(void) {
    Mathfunc(asin);
}

/* Arc tangent */
prim P_atan(void) {
    Mathfunc(atan);
}

/* Arc tangent:  y x -- atan */
prim P_atan2(void) {
    Sl(2 * Realsize);
    SREAL1(atan2(REAL1, REAL0));
    Realpop;
}

/* Cosine */
prim P_cos(void) {
    Mathfunc(cos);
}

/* E ^ x */
prim P_exp(void) {
    Mathfunc(exp);
}

/* Natural log */
prim P_log(void) {
    Mathfunc(log);
}

/* X ^ Y */
prim P_pow(void) {
    Sl(2 * Realsize);
    SREAL1(pow(REAL1, REAL0));
    Realpop;
}

/* Sine */
prim P_sin(void) {
    Mathfunc(sin);
}

/* Square root */
prim P_sqrt(void) {
    Mathfunc(sqrt);
}

/* Tangent */
prim P_tan(void) {
    Mathfunc(tan);
}
#undef Mathfunc
#endif /* MATH */
#endif /* REAL */

/*  Console I/O primitives  */

#ifdef CONIO
// . -- print top of stack, pop it
//
prim P_dot(void) {
    Sl(1);
    if (base == 16) {
        fprintf(stderr, "%lX ", S0);
    } else {
        fprintf(stderr, "%ld ", S0);
    }
    Pop;
}

// .? -- print value at address, pop it
//
prim P_question(void) {
    Sl(1);
    Hpc(S0);
    if (base == 16) {
        fprintf(stderr, "%lX ", *((stackitem *) S0));
    } else {
        fprintf(stderr, "%ld ", *((stackitem *) S0));
    }
    Pop;
}

// cr -- carriage return
//
prim P_cr(void) {
    fprintf(stderr, "\n");
}

// .s -- print entire contents of stack
//
prim P_dots(void) {
    stackitem *tsp;

    fprintf(stderr, "stack: ");
    if (atl__env->stk == atl__env->stkBottom) {
        fprintf(stderr, "empty.");
    } else {
        for (tsp = atl__env->stack; tsp < atl__env->stk; tsp++) {
            if (base == 16) {
                fprintf(stderr, "%lX ", *tsp);
            } else {
                fprintf(stderr, "%ld ", *tsp);
            }
        }
    }
}

/* Print literal string that follows */
prim P_dotquote(void) {
    Compiling;
    stringlit = atlTrue;		      /* Set string literal expected */
    Compconst(s_dotparen);	      /* Compile .( word */
}

/* Print literal string that follows */
prim P_dotparen(void) {
    if (atl__env->ip == NULL) {		      /* If interpreting */
        stringlit = atlTrue;	      /* Set to print next string constant */
    } else {			      /* Otherwise, */
        fprintf(stderr, "%s", ((char *) atl__env->ip) + 1); /* print string literal in in-line code. */
        Skipstring;		      /* And advance IP past it */
    }
}

/* Print string pointed to by stack */
prim P_type(void) {
    Sl(1);
    Hpc(S0);
    fprintf(stderr, "%s", (char *) S0);
    Pop;
}

// words -- list words
//
prim P_words(void) {
    dictword *dw = atl__env->dict;

    while (dw != NULL) {

        fprintf(stderr, "\n%s", dw->wname + 1);
        dw = dw->wnext;
    }
    fprintf(stderr, "\n");
}
#endif /* CONIO */

#ifdef FILEIO

/* Declare file */
prim P_file(void) {
    Ho(2);
    P_create(); 		      /* Create variable */
    Hstore = FileSent;		      /* Store file sentinel */
    Hstore = 0; 		      /* Mark file not open */
}

/* Open file: fname fmodes fd -- flag */
prim P_fopen(void) {
    FILE *fd;
    stackitem stat;

    Sl(3);
    Hpc(S2);
    Hpc(S0);
    Isfile(S0);
    fd = fopen((char *) S2, fopenmodes[S1]);
    if (fd == NULL) {
        stat = atlFalsity;
    } else {
        *(((stackitem *) S0) + 1) = (stackitem) fd;
        stat = atlTruth;
    }
    Pop2;
    S0 = stat;
}

/* Close file: fd -- */
prim P_fclose(void) {
    Sl(1);
    Hpc(S0);
    Isfile(S0);
    Isopen(S0);
    V fclose(FileD(S0));
    *(((stackitem *) S0) + 1) = (stackitem) NULL;
    Pop;
}

/* Delete file: fname -- flag */
prim P_fdelete(void) {
    Sl(1);
    Hpc(S0);
    S0 = (unlink((char *) S0) == 0) ? atlTruth : atlFalsity;
}

/* Get line: fd string -- flag */
prim P_fgetline(void) {
    Sl(2);
    Hpc(S0);
    Isfile(S1);
    Isopen(S1);
    if (atl_fgetsp((char *) S0, 132, FileD(S1)) == NULL) {
        S1 = atlFalsity;
    } else {
        S1 = atlTruth;
    }
    Pop;
}

/* Put line: string fd -- flag */
prim P_fputline(void) {
    Sl(2);
    Hpc(S1);
    Isfile(S0);
    Isopen(S0);
    if (fputs((char *) S1, FileD(S0)) == EOF) {
        S1 = atlFalsity;
    } else {
        S1 = putc('\n', FileD(S0)) == EOF ? atlFalsity : atlTruth;
    }
    Pop;
}

/* File read: fd len buf -- length */
prim P_fread(void) {
    Sl(3);
    Hpc(S0);
    Isfile(S2);
    Isopen(S2);
    S2 = fread((char *) S0, 1, ((int) S1), FileD(S2));
    Pop2;
}

/* File write: len buf fd -- length */
prim P_fwrite(void) {
    Sl(3);
    Hpc(S1);
    Isfile(S0);
    Isopen(S0);
    S2 = fwrite((char *) S1, 1, ((int) S2), FileD(S0));
    Pop2;
}

/* File get character: fd -- char */
prim P_fgetc(void) {
    Sl(1);
    Isfile(S0);
    Isopen(S0);
    S0 = getc(FileD(S0));	      /* Returns -1 if EOF hit */
}

/* File put character: char fd -- stat */
prim P_fputc(void) {
    Sl(2);
    Isfile(S0);
    Isopen(S0);
    S1 = putc((char) S1, FileD(S0));
    Pop;
}

/* Return file position:	fd -- pos */
prim P_ftell(void) {
    Sl(1);
    Isfile(S0);
    Isopen(S0);
    S0 = (stackitem) ftell(FileD(S0));
}

/* Seek file:  offset base fd -- */
prim P_fseek(void) {
    Sl(3);
    Isfile(S0);
    Isopen(S0);
    V fseek(FileD(S0), (long) S2, (int) S1);
    Npop(3);
}

/* Load source file:  fd -- evalstat */
prim P_fload(void) {
    int estat;
    FILE *fd;

    Sl(1);
    Isfile(S0);
    Isopen(S0);
    fd = FileD(S0);
    Pop;
    estat = atl_load(fd);
    So(1);
    Push = estat;
}
#endif /* FILEIO */

#ifdef EVALUATE

/* string -- status */
prim P_evaluate(void) {
    int es = ATL_SNORM;
    atl_statemark mk;
    atl_int scomm = atl__env->isIgnoringComment;    // stack comment pending state
    dictword **sip = atl__env->ip;	      /* Stack instruction pointer */
    char *sinstr = atl__env->inputBuffer; // stack input stream
    char *estring;

    Sl(1);
    Hpc(S0);
    estring = (char *) S0;	      /* Get string to evaluate */
    Pop;			      /* Pop so it sees arguments below it */
    atl_mark(&mk);		      /* Mark in case of error */
    atl__env->ip = NULL;			      /* Fool atl_eval into interp state */
    if ((es = atl_eval(estring)) != ATL_SNORM) {
        atl_unwind(&mk);
    }
    /* If there were no other errors, check for a runaway comment.  If
     we ended the file in comment-ignore mode, set the runaway comment
     error status and unwind the file.  */
    if ((es == ATL_SNORM) && (atl__env->isIgnoringComment != 0)) {
        es = ATL_RUNCOMM;
        atl_unwind(&mk);
    }
    atl__env->isIgnoringComment = scomm;    // unstack comment pending status
    atl__env->ip = sip;			      /* Unstack instruction pointer */
    atl__env->inputBuffer = sinstr; // unstack input stream
    So(1);
    Push = es;			      /* Return eval status on top of stack */
}
#endif /* EVALUATE */

/*  Stack mechanics  */

/* Push stack depth */
prim P_depth(void) {
    stackitem s = atl__env->stk - atl__env->stack;

    So(1);
    Push = s;
}

/* Clear stack */
prim P_clear(void) {
    atl__env->stk = atl__env->stack;
}

/* Duplicate top of stack */
prim P_dup(void) {
    stackitem s;

    Sl(1);
    So(1);
    s = S0;
    Push = s;
}

/* Drop top item on stack */
prim P_drop(void) {
    Sl(1);
    Pop;
}

/* Exchange two top items on stack */
prim P_swap(void) {
    stackitem t;

    Sl(2);
    t = S1;
    S1 = S0;
    S0 = t;
}

/* Push copy of next to top of stack */
prim P_over(void) {
    stackitem s;

    Sl(2);
    So(1);
    s = S1;
    Push = s;
}

/* Copy indexed item from stack */
prim P_pick(void) {
    Sl(2);
    S0 = atl__env->stk[-(2 + S0)];
}

/* Rotate 3 top stack items */
prim P_rot(void) {
    stackitem t;

    Sl(3);
    t = S0;
    S0 = S2;
    S2 = S1;
    S1 = t;
}

/* Reverse rotate 3 top stack items */
prim P_minusrot(void) {
    stackitem t;

    Sl(3);
    t = S0;
    S0 = S1;
    S1 = S2;
    S2 = t;
}

/* Rotate N top stack items */
prim P_roll(void) {
    stackitem i, j, t;

    Sl(1);
    i = S0;
    Pop;
    Sl(i + 1);
    t = atl__env->stk[-(i + 1)];
    for (j = -(i + 1); j < -1; j++)
        atl__env->stk[j] = atl__env->stk[j + 1];
    S0 = t;
}

/* Transfer stack top to return stack */
prim P_tor(void) {
    Rso(1);
    Sl(1);
    Rpush = (rstackitem) S0;
    Pop;
}

/* Transfer return stack top to stack */
prim P_rfrom(void) {
    Rsl(1);
    So(1);
    Push = (stackitem) R0;
    Rpop;
}

/* Fetch top item from return stack */
prim P_rfetch(void) {
    Rsl(1);
    So(1);
    Push = (stackitem) R0;
}

/*  Double stack manipulation items  */

#ifdef DOUBLE

/* Duplicate stack top doubleword */
prim P_2dup(void) {
    stackitem s;

    Sl(2);
    So(2);
    s = S1;
    Push = s;
    s = S1;
    Push = s;
}

/* Drop top two items from stack */
prim P_2drop(void) {
    Sl(2);
    atl__env->stk -= 2;
}

/* Swap top two double items on stack */
prim P_2swap(void) {
    stackitem t;

    Sl(4);
    t = S2;
    S2 = S0;
    S0 = t;
    t = S3;
    S3 = S1;
    S1 = t;
}

/* Extract second pair from stack */
prim P_2over(void) {
    stackitem s;

    Sl(4);
    So(2);
    s = S3;
    Push = s;
    s = S3;
    Push = s;
}

/* Move third pair to top of stack */
prim P_2rot(void) {
    stackitem t1, t2;

    Sl(6);
    t2 = S5;
    t1 = S4;
    S5 = S3;
    S4 = S2;
    S3 = S1;
    S2 = S0;
    S1 = t2;
    S0 = t1;
}

/* Declare double variable */
prim P_2variable(void) {
    P_create(); 		      /* Create dictionary item */
    Ho(2);
    Hstore = 0; 		      /* Initial value = 0... */
    Hstore = 0; 		      /* ...in both words */
}

/* Push double value in body */
prim P_2con(void) {
    So(2);
    Push = *(((stackitem *) atl__env->currentWord) + Dictwordl);
    Push = *(((stackitem *) atl__env->currentWord) + Dictwordl + 1);
}

/* Declare double word constant */
prim P_2constant(void) {
    Sl(1);
    P_create(); 		      /* Create dictionary item */
    atl__env->createWord->wcode = P_2con;       /* Set code to constant push */
    Ho(2);
    Hstore = S1;		      /* Store double word constant value */
    Hstore = S0;		      /* in the two words of body */
    Pop2;
}

/* Store double value into address */
prim P_2bang(void) {
    stackitem *sp;

    Sl(2);
    Hpc(S0);
    sp = (stackitem *) S0;
    *sp++ = S2;
    *sp = S1;
    Npop(3);
}

/* Fetch double value from address */
prim P_2at(void) {
    stackitem *sp;

    Sl(1);
    So(1);
    Hpc(S0);
    sp = (stackitem *) S0;
    S0 = *sp++;
    Push = *sp;
}
#endif /* DOUBLE */

/*  Data transfer primitives  */

/* Push instruction stream literal */
prim P_dolit(void) {
    So(1);
#ifdef TRACE
    if (atl__env->enableTrace) {
        fprintf(stderr, "%ld ", (long) *atl__env->ip);
    }
#endif
    Push = (stackitem) *atl__env->ip++;	      /* Push the next datum from the
                                       instruction stream. */
}

/*  Control flow primitives  */

/* Invoke compiled word */
prim P_nest(void) {
    Rso(1);
#ifdef WALKBACK
    *atl__env->walkbackPointer++ = atl__env->currentWord;   // append word to walkback stack
#endif
    Rpush = atl__env->ip; 		      /* Push instruction pointer */
    atl__env->ip = (((dictword **) atl__env->currentWord) + Dictwordl);
}

/* Return to top of return stack */
prim P_exit(void) {
    Rsl(1);
#ifdef WALKBACK
    atl__env->walkbackPointer = (atl__env->walkbackPointer > atl__env->walkback) ? atl__env->walkbackPointer - 1 : atl__env->walkback;
#endif
    atl__env->ip = R0;			      /* Set IP to top of return stack */
    Rpop;
}

/* Jump to in-line address */
prim P_branch(void) {
    atl__env->ip += (stackitem) *atl__env->ip;	      /* Jump addresses are IP-relative */
}

/* Conditional branch to in-line addr */
prim P_qbranch(void) {
    Sl(1);
    if (S0 == 0)		      /* If flag is false */
        atl__env->ip += (stackitem) *atl__env->ip;	      /* then branch. */
    else			      /* Otherwise */
        atl__env->ip++;			      /* skip the in-line address. */
    Pop;
}

// if -- Compile IF word
//
prim P_if(void) {
    Compiling;
    Compconst(s_qbranch);       // Compile question branch
    So(1);
    Push = (stackitem) atl__env->heapAllocPtr;    // Save backpatch address on stack
    Compconst(0);               // Compile place-holder address cell
}

// else -- Compile ELSE word
//
prim P_else(void) {
    stackitem *bp;

    Compiling;
    Sl(1);
    Compconst(s_branch);            // Compile branch around other clause
    Compconst(0);                   // Compile place-holder address cell
    Hpc(S0);
    bp = (stackitem *) S0;          // Get IF backpatch address
    *bp = atl__env->heapAllocPtr - bp;
    S0 = (stackitem) (atl__env->heapAllocPtr - 1);    // Update backpatch for THEN
}

// then -- Compile THEN word
//
prim P_then(void) {
    stackitem *bp;

    Compiling;
    Sl(1);
    Hpc(S0);
    bp = (stackitem *) S0;          // Get IF/ELSE backpatch address
    *bp = atl__env->heapAllocPtr - bp;
    Pop;
}

/* Duplicate if nonzero */
prim P_qdup(void) {
    Sl(1);
    if (S0 != 0) {
        stackitem s = S0;
        So(1);
        Push = s;
    }
}

/* Compile BEGIN */
prim P_begin(void) {
    Compiling;
    So(1);
    Push = (stackitem) atl__env->heapAllocPtr;	      /* Save jump back address on stack */
}

/* Compile UNTIL */
prim P_until(void) {
    stackitem off;
    stackitem *bp;

    Compiling;
    Sl(1);
    Compconst(s_qbranch);	      /* Compile question branch */
    Hpc(S0);
    bp = (stackitem *) S0;	      /* Get BEGIN address */
    off = -(atl__env->heapAllocPtr - bp);
    Compconst(off);		      /* Compile negative jumpback address */
    Pop;
}

/* Compile AGAIN */
prim P_again(void) {
    stackitem off;
    stackitem *bp;

    Compiling;
    Compconst(s_branch);	      /* Compile unconditional branch */
    Hpc(S0);
    bp = (stackitem *) S0;	      /* Get BEGIN address */
    off = -(atl__env->heapAllocPtr - bp);
    Compconst(off);		      /* Compile negative jumpback address */
    Pop;
}

/* Compile WHILE */
prim P_while(void) {
    Compiling;
    So(1);
    Compconst(s_qbranch);	      /* Compile question branch */
    Compconst(0);		      /* Compile place-holder address cell */
    Push = (stackitem) (atl__env->heapAllocPtr - 1);    /* Queue backpatch for REPEAT */
}

/* Compile REPEAT */
prim P_repeat(void) {
    stackitem off;
    stackitem *bp1, *bp;

    Compiling;
    Sl(2);
    Hpc(S0);
    bp1 = (stackitem *) S0;	      /* Get WHILE backpatch address */
    Pop;
    Compconst(s_branch);	      /* Compile unconditional branch */
    Hpc(S0);
    bp = (stackitem *) S0;	      /* Get BEGIN address */
    off = -(atl__env->heapAllocPtr - bp);
    Compconst(off);		      /* Compile negative jumpback address */
    *bp1 = atl__env->heapAllocPtr - bp1;                /* Backpatch REPEAT's jump out of loop */
    Pop;
}

/* Compile DO */
prim P_do(void) {
    Compiling;
    Compconst(s_xdo);		      /* Compile runtime DO word */
    So(1);
    Compconst(0);		      /* Reserve cell for LEAVE-taking */
    Push = (stackitem) atl__env->heapAllocPtr;	      /* Save jump back address on stack */
}

/* Execute DO */
prim P_xdo(void) {
    Sl(2);
    Rso(3);
    Rpush = atl__env->ip + ((stackitem) *atl__env->ip);   /* Push exit address from loop */
    atl__env->ip++;			      /* Increment past exit address word */
    Rpush = (rstackitem) S1;	      /* Push loop limit on return stack */
    Rpush = (rstackitem) S0;	      /* Iteration variable initial value to
                                       return stack */
    atl__env->stk -= 2;
}

/* Compile ?DO */
prim P_qdo(void) {
    Compiling;
    Compconst(s_xqdo);		      /* Compile runtime ?DO word */
    So(1);
    Compconst(0);		      /* Reserve cell for LEAVE-taking */
    Push = (stackitem) atl__env->heapAllocPtr;	      /* Save jump back address on stack */
}

/* Execute ?DO */
prim P_xqdo(void) {
    Sl(2);
    if (S0 == S1) {
        atl__env->ip += (stackitem) *atl__env->ip;
    } else {
        Rso(3);
        Rpush = atl__env->ip + ((stackitem) *atl__env->ip);/* Push exit address from loop */
        atl__env->ip++;			      /* Increment past exit address word */
        Rpush = (rstackitem) S1;      /* Push loop limit on return stack */
        Rpush = (rstackitem) S0;      /* Iteration variable initial value to
                                       return stack */
    }
    atl__env->stk -= 2;
}

/* Compile LOOP */
prim P_loop(void) {
    stackitem off;
    stackitem *bp;

    Compiling;
    Sl(1);
    Compconst(s_xloop); 	      /* Compile runtime loop */
    Hpc(S0);
    bp = (stackitem *) S0;	      /* Get DO address */
    off = -(atl__env->heapAllocPtr - bp);
    Compconst(off);		      /* Compile negative jumpback address */
    *(bp - 1) = (atl__env->heapAllocPtr - bp) + 1;      /* Backpatch exit address offset */
    Pop;
}

/* Compile +LOOP */
prim P_ploop(void) {
    stackitem off;
    stackitem *bp;

    Compiling;
    Sl(1);
    Compconst(s_pxloop);	      /* Compile runtime +loop */
    Hpc(S0);
    bp = (stackitem *) S0;	      /* Get DO address */
    off = -(atl__env->heapAllocPtr - bp);
    Compconst(off);		      /* Compile negative jumpback address */
    *(bp - 1) = (atl__env->heapAllocPtr - bp) + 1;      /* Backpatch exit address offset */
    Pop;
}

/* Execute LOOP */
prim P_xloop(void) {
    Rsl(3);
    R0 = (rstackitem) (((stackitem) R0) + 1);
    if (((stackitem) R0) == ((stackitem) R1)) {
        atl__env->rs -= 3;		      /* Pop iteration variable and limit */
        atl__env->ip++;			      /* Skip the jump address */
    } else {
        atl__env->ip += (stackitem) *atl__env->ip;
    }
}

/* Execute +LOOP */
prim P_xploop(void) {
    stackitem niter;

    Sl(1);
    Rsl(3);

    niter = ((stackitem) R0) + S0;
    Pop;
    if ((niter >= ((stackitem) R1)) &&
        (((stackitem) R0) < ((stackitem) R1))) {
        atl__env->rs -= 3;		      /* Pop iteration variable and limit */
        atl__env->ip++;			      /* Skip the jump address */
    } else {
        atl__env->ip += (stackitem) *atl__env->ip;
        R0 = (rstackitem) niter;
    }
}

/* Compile LEAVE */
prim P_leave(void) {
    Rsl(3);
    atl__env->ip = R2;
    atl__env->rs -= 3;
}

/* Obtain innermost loop index */
prim P_i(void) {
    Rsl(3);
    So(1);
    Push = (stackitem) R0;            /* It's the top item on return stack */
}

/* Obtain next-innermost loop index */
prim P_j(void) {
    Rsl(6);
    So(1);
    Push = (stackitem) atl__env->rs[-4];      /* It's the 4th item on return stack */
}

/* Terminate execution */
prim P_quit(void) {
    atl__env->rs = atl__env->rstack;		      /* Clear return stack */
#ifdef WALKBACK
    atl__env->walkbackPointer = atl__env->walkback;
#endif
    atl__env->ip = NULL;			      /* Stop execution of current word */
}

/* Abort, clearing data stack */
prim P_abort(void) {
    P_clear();			      /* Clear the data stack */
    P_quit();			      /* Shut down execution */
}

/* Abort, printing message */
prim P_abortq(void) {
    if (state) {
        stringlit = atlTrue;	      /* Set string literal expected */
        Compconst(s_abortq);	      /* Compile ourselves */
    } else {
        fprintf(stderr, "%s", (char *) atl__env->ip);  // otherwise, print string literal in in-line code.
#ifdef WALKBACK
        pwalkback();
#endif /* WALKBACK */
        P_abort();		      /* Abort */
        // reset all interpretation state
        atl__env->isIgnoringComment = state = atlFalsity;
        forgetpend = defpend = stringlit = tickpend = ctickpend = atlFalse;
    }
}

/*  Compilation primitives  */

/* Mark most recent word immediate */
prim P_immediate(void) {
    atl__env->dict->wname[0] |= IMMEDIATE;
}

/* Set interpret state */
prim P_lbrack(void) {
    Compiling;
    state = atlFalsity;
}

/* Restore compile state */
prim P_rbrack(void) {
    state = atlTruth;
}

/* Execute indirect call on method */
Exported void P_dodoes(void) {
    Rso(1);
    So(1);
    Rpush = atl__env->ip; 		      /* Push instruction pointer */
#ifdef WALKBACK
    *atl__env->walkbackPointer++ = atl__env->currentWord;   // append word to walkback stack
#endif
    /* The compiler having craftily squirreled away the DOES> clause
     address before the word definition on the heap, we back up to
     the heap cell before the current word and load the pointer from
     there.  This is an ABSOLUTE heap address, not a relative offset. */
    atl__env->ip = *((dictword ***) (((stackitem *) atl__env->currentWord) - 1));

    /* Push the address of this word's body as the argument to the
     DOES> clause. */
    Push = (stackitem) (((stackitem *) atl__env->currentWord) + Dictwordl);
}

// does> -- specify method for word
//
prim P_does(void) {

    // O.K., we were compiling our way through this definition and we've
    // encountered the Dreaded and Dastardly Does.  Here's what we do
    // about it.  The problem is that when we execute the word, we
    // want to push its address on the stack and call the code for the
    // DOES> clause by diverting the IP to that address.  But...how
    // are we to know where the DOES> clause goes without adding a
    // field to every word in the system just to remember it.  Recall
    // that since this system is portable we can't cop-out through
    // machine code.  Further, we can't compile something into the
    // word because the defining code may have already allocated heap
    // for the word's body.  Yukkkk.  Oh well, how about this?  Let's
    // copy any and all heap allocated for the word down one stackitem
    // and then jam the DOES> code address BEFORE the link field in
    // the word we're defining.
    //
    // Then, when (DOES>) (P_dodoes) is called to execute the word, it
    // will fetch that code address by backing up past the start of
    // the word and seting IP to it.  Note that FORGET must recognise
    // such words (by the presence of the pointer to P_dodoes() in
    // their wcode field, in case you're wondering), and make sure to
    // deallocate the heap word containing the link when a
    // DOES>-defined word is deleted.

    if (atl__env->createWord != NULL) {
        stackitem *sp = ((stackitem *) atl__env->createWord), *hp;

        Rsl(1);
        Ho(1);

        // Copy the word definition one word down in the heap to
        // permit us to prefix it with the DOES clause address.

        for (hp = atl__env->heapAllocPtr - 1; hp >= sp; hp--) {
            *(hp + 1) = *hp;
        }
        atl__env->heapAllocPtr++;               // expand allocated length of word
        *sp++ = (stackitem) atl__env->ip;         // store DOES> clause address before
                                        // word's definition structure.
        atl__env->createWord = (dictword *) sp;   // move word definition down 1 item
        atl__env->createWord->wcode = P_dodoes;   // set code field to indirect jump

        // Now simulate an EXIT to bail out of the definition without
        // executing the DOES> clause at definition time.

        atl__env->ip = R0;		      // Set IP to top of return stack
#ifdef WALKBACK
        atl__env->walkbackPointer = (atl__env->walkbackPointer > atl__env->walkback) ? atl__env->walkbackPointer - 1 : atl__env->walkback;
#endif
        Rpop;			      // Pop the return stack
    }
}

// : -- begin compilation
//
prim P_colon(void) {
    state = atlTruth;		      // Set compilation underway
    P_create(); 		      // Create conventional word
}

// ; -- end compilation
//
prim P_semicolon(void) {
    Compiling;
    Ho(1);
    Hstore = s_exit;
    state = atlFalsity;		      // No longer compiling

    // We wait until now to plug the P_nest code so that it will be
    // present only in completed definitions.
    if (atl__env->createWord != NULL) {
        atl__env->createWord->wcode = P_nest;   // Use P_nest for code
    }
    atl__env->createWord = NULL;		      // Flag no word being created
}

// ` -- take address of next word
//
prim P_tick(void) {
    // Try to get next symbol from the input stream.  If
    // we can't, and we're executing a compiled word,
    // report an error.  Since we can't call back to the
    // calling program for more input, we're stuck.

    int i = atl__env->nextToken(&(atl__env->inputBuffer));   // scan for next token
    if (i != TokNull) {
        if (i == TokWord) {
            dictword *di;

            ucase(tokbuf);
            if ((di = lookup(tokbuf)) != NULL) {
                So(1);
                Push = (stackitem) di; /* Push word compile address */
            } else {
                fprintf(stderr, " '%s' undefined ", tokbuf);
            }
        } else {
            fprintf(stderr, "\nword not specified when expected.\n");
            P_abort();
        }
    } else {
        // O.K., there was nothing in the input stream.  Set the
        // tickpend flag to cause the compilation address of the next
        // token to be pushed when it's supplied on a subsequent input
        // line.
        if (atl__env->ip == NULL) {
            tickpend = atlTrue;	      // Set tick pending
        } else {
            fprintf(stderr, "\nword requested by ` not on same input line.\n");
            P_abort();
        }
    }
}

/* Compile in-line code address */
prim P_bracktick(void) {
    Compiling;
    ctickpend = atlTrue;		      /* Force literal treatment of next
                                   word in compile stream */
}

/* Execute word pointed to by stack */
prim P_execute(void) {
    dictword *wp;

    Sl(1);
    wp = (dictword *) S0;	      /* Load word address from stack */
    Pop;			      /* Pop data stack before execution */
    exword(wp); 		      /* Recursively call exword() to run
                               the word. */
}

/* Get body address for word */
prim P_body(void) {
    Sl(1);
    S0 += Dictwordl * sizeof(stackitem);
}

/* Get state of system */
prim P_state(void) {
    So(1);
    Push = (stackitem) &state;
}

/*  Definition field access primitives	*/

#ifdef DEFFIELDS

// Look up word in dictionary
//
prim P_find(void) {
    dictword *dw;

    Sl(1);
    So(1);
    Hpc(S0);
    strcpy(tokbuf, (char *) S0);        // Use built-in token buffer...
    dw = lookup(tokbuf);                // So ucase() in lookup() doesn't wipe
    // the token on the stack
    if (dw != NULL) {
        S0 = (stackitem) dw;
        // Push immediate flag
        Push = (dw->wname[0] & IMMEDIATE) ? 1 : -1;
    } else {
        Push = 0;
    }
}

#define DfOff(fld)  (((char *) &(atl__env->dict->fld)) - ((char *) atl__env->dict))

// Find name field from compile addr
//
prim P_toname(void) {
    Sl(1);
    S0 += DfOff(wname);
}

// Find link field from compile addr
//
prim P_tolink(void) {
    if (DfOff(wnext) != 0) {
        fprintf(stderr, "\n>LINK Foulup--wnext is not at zero!\n");
    }
    // Sl(1); S0 += DfOff(wnext);
    // Null operation.  Wnext is first
}

// Get compile address from body
//
prim P_frombody(void) {
    Sl(1);
    S0 -= Dictwordl * sizeof(stackitem);
}

// Get compile address from name
//
prim P_fromname(void) {
    Sl(1);
    S0 -= DfOff(wname);
}

/* Get compile address from link */
prim P_fromlink(void) {
    if (DfOff(wnext) != 0) {
        fprintf(stderr, "\nLINK> Foulup--wnext is not at zero!\n");
    }
    /*  Sl(1);
     S0 -= DfOff(wnext);  */	      /* Null operation.  Wnext is first */
}

#undef DfOff

#define DfTran(from,to) (((char *) &(atl__env->dict->to)) - ((char *) &(atl__env->dict->from)))

/* Get from name field to link */
prim P_nametolink(void) {
    char *from, *to;

    Sl(1);
    /*
     S0 -= DfTran(wnext, wname);
     */
    from = (char *) &(atl__env->dict->wnext);
    to = (char *) &(atl__env->dict->wname);
    S0 -= (to - from);
}

/* Get from link field to name */
prim P_linktoname(void) {
    char *from, *to;

    Sl(1);
    /*
     S0 += DfTran(wnext, wname);
     */
    from = (char *) &(atl__env->dict->wnext);
    to = (char *) &(atl__env->dict->wname);
    S0 += (to - from);
}

/* Copy word name to string buffer */
prim P_fetchname(void) {
    Sl(2);			      /* nfa string -- */
    Hpc(S0);
    Hpc(S1);
    /* Since the name buffers aren't in the system heap, but
     rather are separately allocated with alloc(), we can't
     check the name pointer references.  But, hey, if the user's
     futzing with word dictionary items on the heap in the first
     place, there's a billion other ways to bring us down at
     his command. */
    strcpy((char *) S0, *((char **) S1) + 1);
    Pop2;
}

/* Store string buffer in word name */
prim P_storename(void) {
    char tflags;
    char *cp;

    Sl(2);			      /* string nfa -- */
    Hpc(S0);			      /* See comments in P_fetchname above */
    Hpc(S1);			      /* checking name pointers */
    tflags = **((char **) S0);
    free(*((char **) S0));
    *((char **) S0) = cp = alloc((unsigned int) (strlen((char *) S1) + 2));
    V strcpy(cp + 1, (char *) S1);
    *cp = tflags;
    Pop2;
}

#endif /* DEFFIELDS */

#ifdef SYSTEM

/* string -- status */
prim P_system(void) {
    Sl(1);
    Hpc(S0);
    S0 = system((char *) S0);
}
#endif /* SYSTEM */

#ifdef TRACE
/* Set or clear tracing of execution */
prim P_trace(void) {
    Sl(1);
    atl__env->enableTrace = (S0 == 0) ? atlFalsity : atlTruth;
    Pop;
}
#endif /* TRACE */

#ifdef WALKBACK
/* Set or clear error walkback */
prim P_walkback(void) {
    Sl(1);
    atl__env->enableWalkback = (S0 == 0) ? atlFalsity : atlTruth;
    Pop;
}
#endif /* WALKBACK */

#ifdef WORDSUSED
/* List words used by program */
prim P_wordsused(void) {
    dictword *dw = atl__env->dict;

    while (dw != NULL) {
        if (*(dw->wname) & WORDUSED) {
            fprintf(stderr, "\n%s", dw->wname + 1);
        }
#ifdef Keyhit
        if (kbquit()) {
            break;
        }
#endif
        dw = dw->wnext;
    }
    fprintf(stderr, "\n");
}

/* List words not used by program */
prim P_wordsunused(void) {
    dictword *dw = atl__env->dict;

    while (dw != NULL) {
        if (!(*(dw->wname) & WORDUSED)) {
            fprintf(stderr, "\n%s", dw->wname + 1);
        }
#ifdef Keyhit
        if (kbquit()) {
            break;
        }
#endif
        dw = dw->wnext;
    }
    fprintf(stderr, "\n");
}
#endif /* WORDSUSED */

#ifdef COMPILERW
/* Force compilation of immediate word */
prim P_brackcompile(void) {
    Compiling;
    cbrackpend = atlTrue;		      /* Set [COMPILE] pending */
}

/* Compile top of stack as literal */
prim P_literal(void) {
    Compiling;
    Sl(1);
    Ho(2);
    Hstore = s_lit;		      /* Compile load literal word */
    Hstore = S0;		      /* Compile top of stack in line */
    Pop;
}

/* Compile address of next inline word */
prim P_compile(void) {
    Compiling;
    Ho(1);
    Hstore = (stackitem) *atl__env->ip++;       /* Compile the next datum from the
                                       instruction stream. */
}

/* Mark backward backpatch address */
prim P_backmark(void) {
    Compiling;
    So(1);
    Push = (stackitem) atl__env->heapAllocPtr;	      /* Push heap address onto stack */
}

/* Emit backward jump offset */
prim P_backresolve(void) {
    stackitem offset;

    Compiling;
    Sl(1);
    Ho(1);
    Hpc(S0);
    offset = -(atl__env->heapAllocPtr - (stackitem *) S0);
    Hstore = offset;
    Pop;
}

/* Mark forward backpatch address */
prim P_fwdmark(void) {
    Compiling;
    Ho(1);
    Push = (stackitem) atl__env->heapAllocPtr;	      /* Push heap address onto stack */
    Hstore = 0;
}

/* Emit forward jump offset */
prim P_fwdresolve(void) {
    stackitem offset;

    Compiling;
    Sl(1);
    Hpc(S0);
    offset = (atl__env->heapAllocPtr - (stackitem *) S0);
    *((stackitem *) S0) = offset;
    Pop;
}

#endif /* COMPILERW */

/*  Table of primitive words  */

static struct primfcn primt[] = {
    {"0+", P_plus},
    {"0-", P_minus},
    {"0*", P_times},
    {"0/", P_div},
    {"0MOD", P_mod},
    {"0/MOD", P_divmod},
    {"0MIN", P_min},
    {"0MAX", P_max},
    {"0NEGATE", P_neg},
    {"0ABS", P_abs},
    {"0=", P_equal},
    {"0<>", P_unequal},
    {"0>", P_gtr},
    {"0<", P_lss},
    {"0>=", P_geq},
    {"0<=", P_leq},

    {"0AND", P_and},
    {"0OR", P_or},
    {"0XOR", P_xor},
    {"0NOT", P_not},
    {"0SHIFT", P_shift},

    {"0DEPTH", P_depth},
    {"0CLEAR", P_clear},
    {"0DUP", P_dup},
    {"0DROP", P_drop},
    {"0SWAP", P_swap},
    {"0OVER", P_over},
    {"0PICK", P_pick},
    {"0ROT", P_rot},
    {"0-ROT", P_minusrot},
    {"0ROLL", P_roll},
    {"0>R", P_tor},
    {"0R>", P_rfrom},
    {"0R@", P_rfetch},

#ifdef SHORTCUTA
    {"01+", P_1plus},
    {"02+", P_2plus},
    {"01-", P_1minus},
    {"02-", P_2minus},
    {"02*", P_2times},
    {"02/", P_2div},
#endif /* SHORTCUTA */

#ifdef SHORTCUTC
    {"00=", P_0equal},
    {"00<>", P_0notequal},
    {"00>", P_0gtr},
    {"00<", P_0lss},
#endif /* SHORTCUTC */

#ifdef DOUBLE
    {"02DUP", P_2dup},
    {"02DROP", P_2drop},
    {"02SWAP", P_2swap},
    {"02OVER", P_2over},
    {"02ROT", P_2rot},
    {"02VARIABLE", P_2variable},
    {"02CONSTANT", P_2constant},
    {"02!", P_2bang},
    {"02@", P_2at},
#endif /* DOUBLE */

    {"0VARIABLE", P_variable},
    {"0CONSTANT", P_constant},
    {"0!", P_bang},
    {"0@", P_at},
    {"0+!", P_plusbang},
    {"0ALLOT", P_allot},
    {"0,", P_comma},
    {"0C!", P_cbang},
    {"0C@", P_cat},
    {"0C,", P_ccomma},
    {"0C=", P_cequal},
    {"0HERE", P_here},

#ifdef ARRAY
    {"0ARRAY", P_array},
#endif

#ifdef STRING
    {"0(STRLIT)", P_strlit},
    {"0STRING", P_string},
    {"0STRCPY", P_strcpy},
    {"0S!", P_strcpy},
    {"0STRCAT", P_strcat},
    {"0S+", P_strcat},
    {"0STRLEN", P_strlen},
    {"0STRCMP", P_strcmp},
    {"0STRCHAR", P_strchar},
    {"0SUBSTR", P_substr},
    {"0COMPARE", P_strcmp},
    {"0STRFORM", P_strform},
#ifdef REAL
    {"0FSTRFORM", P_fstrform},
#endif
    {"0STRINT", P_strint},
    {"0STRREAL", P_strreal},
#endif /* STRING */

#ifdef REAL
    {"0(FLIT)", P_flit},
    {"0F+", P_fplus},
    {"0F-", P_fminus},
    {"0F*", P_ftimes},
    {"0F/", P_fdiv},
    {"0FMIN", P_fmin},
    {"0FMAX", P_fmax},
    {"0FNEGATE", P_fneg},
    {"0FABS", P_fabs},
    {"0F=", P_fequal},
    {"0F<>", P_funequal},
    {"0F>", P_fgtr},
    {"0F<", P_flss},
    {"0F>=", P_fgeq},
    {"0F<=", P_fleq},
    {"0F.", P_fdot},
    {"0FLOAT", P_float},
    {"0FIX", P_fix},
#ifdef MATH
    {"0ACOS", P_acos},
    {"0ASIN", P_asin},
    {"0ATAN", P_atan},
    {"0ATAN2", P_atan2},
    {"0COS", P_cos},
    {"0EXP", P_exp},
    {"0LOG", P_log},
    {"0POW", P_pow},
    {"0SIN", P_sin},
    {"0SQRT", P_sqrt},
    {"0TAN", P_tan},
#endif /* MATH */
#endif /* REAL */

    {"0(NEST)", P_nest},
    {"0EXIT", P_exit},
    {"0(LIT)", P_dolit},
    {"0BRANCH", P_branch},
    {"0?BRANCH", P_qbranch},
    {"1IF", P_if},
    {"1ELSE", P_else},
    {"1THEN", P_then},
    {"0?DUP", P_qdup},
    {"1BEGIN", P_begin},
    {"1UNTIL", P_until},
    {"1AGAIN", P_again},
    {"1WHILE", P_while},
    {"1REPEAT", P_repeat},
    {"1DO", P_do},
    {"1?DO", P_qdo},
    {"1LOOP", P_loop},
    {"1+LOOP", P_ploop},
    {"0(XDO)", P_xdo},
    {"0(X?DO)", P_xqdo},
    {"0(XLOOP)", P_xloop},
    {"0(+XLOOP)", P_xploop},
    {"0LEAVE", P_leave},
    {"0I", P_i},
    {"0J", P_j},
    {"0QUIT", P_quit},
    {"0ABORT", P_abort},
    {"1ABORT\"", P_abortq},

#ifdef SYSTEM
    {"0SYSTEM", P_system},
#endif
#ifdef TRACE
    {"0TRACE", P_trace},
#endif
#ifdef WALKBACK
    {"0WALKBACK", P_walkback},
#endif

#ifdef WORDSUSED
    {"0WORDSUSED", P_wordsused},
    {"0WORDSUNUSED", P_wordsunused},
#endif

#ifdef MEMSTAT
    {"0MEMSTAT", atl_memstat},
#endif

    {"0:", P_colon},
    {"1;", P_semicolon},
    {"0IMMEDIATE", P_immediate},
    {"1[", P_lbrack},
    {"0]", P_rbrack},
    {"0CREATE", P_create},
    {"0FORGET", P_forget},
    {"0DOES>", P_does},
    {"0'", P_tick},
    {"1[']", P_bracktick},
    {"0EXECUTE", P_execute},
    {"0>BODY", P_body},
    {"0STATE", P_state},

#ifdef DEFFIELDS
    {"0FIND", P_find},
    {"0>NAME", P_toname},
    {"0>LINK", P_tolink},
    {"0BODY>", P_frombody},
    {"0NAME>", P_fromname},
    {"0LINK>", P_fromlink},
    {"0N>LINK", P_nametolink},
    {"0L>NAME", P_linktoname},
    {"0NAME>S!", P_fetchname},
    {"0S>NAME!", P_storename},
#endif /* DEFFIELDS */

#ifdef COMPILERW
    {"1[COMPILE]", P_brackcompile},
    {"1LITERAL", P_literal},
    {"0COMPILE", P_compile},
    {"0<MARK", P_backmark},
    {"0<RESOLVE", P_backresolve},
    {"0>MARK", P_fwdmark},
    {"0>RESOLVE", P_fwdresolve},
#endif /* COMPILERW */

#ifdef CONIO
    {"0.", P_dot},
    {"0?", P_question},
    {"0CR", P_cr},
    {"0.S", P_dots},
    {"1.\"", P_dotquote},
    {"1.(", P_dotparen},
    {"0TYPE", P_type},
    {"0WORDS", P_words},
#endif /* CONIO */

#ifdef FILEIO
    {"0FILE", P_file},
    {"0FOPEN", P_fopen},
    {"0FCLOSE", P_fclose},
    {"0FDELETE", P_fdelete},
    {"0FGETS", P_fgetline},
    {"0FPUTS", P_fputline},
    {"0FREAD", P_fread},
    {"0FWRITE", P_fwrite},
    {"0FGETC", P_fgetc},
    {"0FPUTC", P_fputc},
    {"0FTELL", P_ftell},
    {"0FSEEK", P_fseek},
    {"0FLOAD", P_fload},
#endif /* FILEIO */

#ifdef EVALUATE
    {"0EVALUATE", P_evaluate},
#endif /* EVALUATE */

    {NULL, (codeptr) 0}
};

/*  ATL_PRIMDEF  --  Initialise the dictionary with the built-in primitive
 words.  To save the memory overhead of separately
 allocated word items, we get one buffer for all
 the items and link them internally within the buffer. */

Exported void atl_primdef(struct primfcn *pt) {
    struct primfcn *pf = pt;
    dictword *nw;
    int i, n = 0;
#ifdef WORDSUSED
#ifdef READONLYSTRINGS
    unsigned int nltotal;
    char *dynames, *cp;
#endif /* READONLYSTRINGS */
#endif /* WORDSUSED */

    /* Count the number of definitions in the table. */

    while (pf->pname != NULL) {
        n++;
        pf++;
    }

#ifdef WORDSUSED
#ifdef READONLYSTRINGS
    nltotal = n;
    for (i = 0; i < n; i++) {
        nltotal += strlen(pt[i].pname);
    }
    cp = dynames = alloc(nltotal);
    for (i = 0; i < n; i++) {
        strcpy(cp, pt[i].pname);
        cp += strlen(cp) + 1;
    }
    cp = dynames;
#endif /* READONLYSTRINGS */
#endif /* WORDSUSED */

    nw = (dictword *) alloc((unsigned int) (n * sizeof(dictword)));

    nw[n - 1].wnext = atl__env->dict;
    atl__env->dict = nw;
    for (i = 0; i < n; i++) {
        nw->wname = pt->pname;
#ifdef WORDSUSED
#ifdef READONLYSTRINGS
    	nw->wname = cp;
        cp += strlen(cp) + 1;
#endif /* READONLYSTRINGS */
#endif /* WORDSUSED */
        nw->wcode = pt->pcode;
        if (i != (n - 1)) {
            nw->wnext = nw + 1;
        }
        nw++;
        pt++;
    }
}

#ifdef WALKBACK

/*  PWALKBACK  --  Print walkback trace.  */

static void pwalkback(void) {
    if (atl__env->enableWalkback && ((atl__env->currentWord != NULL) || (atl__env->walkbackPointer > atl__env->walkback))) {
        fprintf(stderr, "walkback:\n");
        if (atl__env->currentWord != NULL) {
            fprintf(stderr, "   %s\n", atl__env->currentWord->wname + 1);
        }
        while (atl__env->walkbackPointer > atl__env->walkback) {
            dictword *wb = *(--atl__env->walkbackPointer);
            fprintf(stderr, "   %s\n", wb->wname + 1);
        }
    }
}
#endif /* WALKBACK */

/*  TROUBLE  --  Common handler for serious errors.  */

static void trouble(char *kind) {
#ifdef MEMMESSAGE
    fprintf(stderr, "\n%s.\n", kind);
#endif
#ifdef WALKBACK
    pwalkback();
#endif /* WALKBACK */
    P_abort();			      /* Abort */
    // reset all interpretation state */
    atl__env->isIgnoringComment = state = atlFalsity;
    forgetpend = defpend = stringlit = tickpend = ctickpend = atlFalse;
}

/*  ATL_ERROR  --  Handle error detected by user-defined primitive.  */

Exported void atl_error(char *kind) {
    trouble(kind);
    evalstat = ATL_APPLICATION;       /* Signify application-detected error */
}

#ifndef NOMEMCHECK

/*  STAKOVER  --  Recover from stack overflow.	*/

Exported void stakover(void) {
    trouble("Stack overflow");
    evalstat = ATL_STACKOVER;
}

/*  STAKUNDER  --  Recover from stack underflow.  */

Exported void stakunder(void) {
    trouble("Stack underflow");
    evalstat = ATL_STACKUNDER;
}

/*  RSTAKOVER  --  Recover from return stack overflow.	*/

Exported void rstakover(void) {
    trouble("Return stack overflow");
    evalstat = ATL_RSTACKOVER;
}

/*  RSTAKUNDER	--  Recover from return stack underflow.  */

Exported void rstakunder(void) {
    trouble("Return stack underflow");
    evalstat = ATL_RSTACKUNDER;
}

/*  HEAPOVER  --  Recover from heap overflow.  Note that a heap
 overflow does NOT wipe the heap; it's up to
 the user to do this manually with FORGET or
 some such. */

Exported void heapover(void) {
    trouble("Heap overflow");
    evalstat = ATL_HEAPOVER;
}

/*  BADPOINTER	--  Abort if bad pointer reference detected.  */

Exported void badpointer(void) {
    trouble("Bad pointer");
    evalstat = ATL_BADPOINTER;
}

/*  NOTCOMP  --  Compiler word used outside definition.  */

static void notcomp(void) {
    trouble("Compiler word outside definition");
    evalstat = ATL_NOTINDEF;
}

/*  DIVZERO  --  Attempt to divide by zero.  */

static void divzero(void) {
    trouble("Divide by zero");
    evalstat = ATL_DIVZERO;
}

#endif /* !NOMEMCHECK */

/*  EXWORD  --	Execute a word (and any sub-words it may invoke). */

static void exword(dictword *wp) {
    atl__env->currentWord = wp;
#ifdef TRACE
    if (atl__env->enableTrace) {
        fprintf(stderr, "\ntrace: %s ", atl__env->currentWord->wname + 1);
    }
#endif /* TRACE */
    (*atl__env->currentWord->wcode)();	      /* Execute the first word */
    while (atl__env->ip != NULL) {
#ifdef BREAK
#ifdef Keybreak
        Keybreak();		      /* Poll for asynchronous interrupt */
#endif
        if (broken) {		      /* Did we receive a break signal */
            trouble("Break signal");
            evalstat = ATL_BREAK;
            break;
        }
#endif /* BREAK */
        atl__env->currentWord = *atl__env->ip++;
#ifdef TRACE
        if (atl__env->enableTrace) {
            fprintf(stderr, "\ntrace: %s ", atl__env->currentWord->wname + 1);
        }
#endif /* TRACE */
        (*atl__env->currentWord->wcode)();	      /* Execute the next word */
    }
    atl__env->currentWord = NULL;
}

/*  ATL_INIT  --  Initialise the ATLAST system.  The dynamic storage areas
 are allocated unless the caller has preallocated buffers
 for them and stored the addresses into the respective
 pointers.  In either case, the storage management
 pointers are initialised to the correct addresses.  If
 the caller preallocates the buffers, it's up to him to
 ensure that the length allocated agrees with the lengths
 given by the atl_... cells.  */

void atl_init(void) {
    if (atl__env->dict == NULL) {
        atl_primdef(primt);	      /* Define primitive words */
        atl__env->dictFirstProtectedEntry = atl__env->dict;	      /* Set protected mark in dictionary */

        /* Look up compiler-referenced words in the new dictionary and
         save their compile addresses in static variables. */

#define Cconst(cell, name)  cell = (stackitem) lookup(name); if(cell==0)abort()
        Cconst(s_exit, "EXIT");
        Cconst(s_lit, "(LIT)");
        Cconst(s_flit, "(FLIT)");
        Cconst(s_strlit, "(STRLIT)");
        Cconst(s_dotparen, ".(");
        Cconst(s_qbranch, "?BRANCH");
        Cconst(s_branch, "BRANCH");
        Cconst(s_xdo, "(XDO)");
        Cconst(s_xqdo, "(X?DO)");
        Cconst(s_xloop, "(XLOOP)");
        Cconst(s_pxloop, "(+XLOOP)");
        Cconst(s_abortq, "ABORT\"");
#undef Cconst

        if (atl__env->stack == NULL) {	      /* Allocate stack if needed */
            atl__env->stack = (stackitem *) alloc(((unsigned int) atl__env->stkLength) * sizeof(stackitem));
        }
        atl__env->stk = atl__env->stkBottom = atl__env->stack;
#ifdef MEMSTAT
        atl__env->stkMaxExtent = atl__env->stack;
#endif
        atl__env->stkTop = atl__env->stack + atl__env->stkLength;
        if (atl__env->rstack == NULL) {	      /* Allocate return stack if needed */
            atl__env->rstack = (dictword ***) alloc(((unsigned int) atl__env->rsLength) * sizeof(dictword **));
        }
        atl__env->rs = atl__env->rsBottom = atl__env->rstack;
#ifdef MEMSTAT
        atl__env->rsMaxExtent = atl__env->rstack;
#endif
        atl__env->rsTop = atl__env->rstack + atl__env->rsLength;
#ifdef WALKBACK
        if (atl__env->walkback == NULL) {
            atl__env->walkback = (dictword **) alloc(((unsigned int) atl__env->rsLength) * sizeof(dictword *));
        }
        atl__env->walkbackPointer = atl__env->walkback;
#endif
        if (atl__env->heap == NULL) {

            /* The temporary string buffers are placed at the start of the
             heap, which permits us to pointer-check pointers into them
             as within the heap extents.  Hence, the size of the buffer
             we acquire for the heap is the sum of the heap and temporary
             string requests. */

            int i;
            char *cp;

            /* Force length of temporary strings to even number of stackitems. */
            atl__env->lengthTempStringBuffer += sizeof(stackitem) - (atl__env->lengthTempStringBuffer % sizeof(stackitem));
            cp = alloc((((unsigned int) atl__env->heapLength) * sizeof(stackitem)) + ((unsigned int) (atl__env->numberOfTempStringBuffers * atl__env->lengthTempStringBuffer)));
            atl__env->heapBottom = (stackitem *) cp;
            atl__env->strbuf = (char **) alloc(((unsigned int) atl__env->numberOfTempStringBuffers) * sizeof(char *));
            for (i = 0; i < atl__env->numberOfTempStringBuffers; i++) {
                atl__env->strbuf[i] = cp;
                cp += ((unsigned int) atl__env->lengthTempStringBuffer);
            }
            atl__env->idxCurrTempStringBuffer = 0;
            atl__env->heap = (stackitem *) cp;  // allocatable heap starts after the temporary strings
        }
        /* The system state word is kept in the first word of the heap
         so that pointer checking doesn't bounce references to it.
         When creating the heap, we preallocate this word and initialise
         the state to the interpretive state. */
        atl__env->heapAllocPtr = atl__env->heap + 1;
        state = atlFalsity;
#ifdef MEMSTAT
        atl__env->heapMaxExtent = atl__env->heapAllocPtr;
#endif
        atl__env->heapTop = atl__env->heap + atl__env->heapLength;

        // now that dynamic memory is up and running, allocate constants and variables built into the system.

#ifdef FILEIO
        {   static struct {
            char *sfn;
            FILE *sfd;
	    } stdfiles[] = {
            {"STDIN", NULL},
            {"STDOUT", NULL},
            {"STDERR", NULL}
	    };
            int i;
            dictword *dw;

    	    /* On some systems stdin, stdout, and stderr aren't
             constants which can appear in an initialisation.
             So, we initialise them at runtime here. */

    	    stdfiles[0].sfd = stdin;
            stdfiles[1].sfd = stdout;
            stdfiles[2].sfd = stderr;

            for (i = 0; i < ELEMENTS(stdfiles); i++) {
                if ((dw = atl_vardef(stdfiles[i].sfn,
                                     2 * sizeof(stackitem))) != NULL) {
                    stackitem *si = atl_body(dw);
                    *si++ = FileSent;
                    *si = (stackitem) stdfiles[i].sfd;
                }
            }
        }
#endif /* FILEIO */
        atl__env->dictFirstProtectedEntry = atl__env->dict; // protect all standard words
    }
}

/*  ATL_LOOKUP	--  Look up a word in the dictionary.  Returns its
 word item if found or NULL if the word isn't
 in the dictionary. */

dictword *atl_lookup(char *name) {
    V strcpy(tokbuf, name);	      /* Use built-in token buffer... */
    ucase(tokbuf);                    /* so ucase() doesn't wreck arg string */
    return lookup(tokbuf);	      /* Now use normal lookup() on it */
}

// ATL_BODY  --  Returns the address of the body of a word, given its dictionary entry.
//
stackitem *atl_body(dictword *dw) {
    return ((stackitem *) dw) + Dictwordl;
}

/*  ATL_EXEC  --  Execute a word, given its dictionary address.  The
 evaluation status for that word's execution is
 returned.  The in-progress evaluation status is
 preserved. */

int atl_exec(dictword *dw) {
    int sestat = evalstat, restat;

    evalstat = ATL_SNORM;
#ifdef BREAK
    broken = atlFalse;		      /* Reset break received */
#endif
#undef Memerrs
#define Memerrs evalstat
    Rso(1);
    Rpush = atl__env->ip; 		      /* Push instruction pointer */
    atl__env->ip = NULL;			      /* Keep exword from running away */
    exword(dw);
    if (evalstat == ATL_SNORM) {      /* If word ran to completion */
        Rsl(1);
        atl__env->ip = R0;		      /* Pop the return stack */
        Rpop;
    }
#undef Memerrs
#define Memerrs
    restat = evalstat;
    evalstat = sestat;
    return restat;
}

/*  ATL_VARDEF  --  Define a variable word.  Called with the word's
 name and the number of bytes of storage to allocate
 for its body.  All words defined with atl_vardef()
 have the standard variable action of pushing their
 body address on the stack when invoked.  Returns
 the dictionary item for the new word, or NULL if
 the heap overflows. */

dictword *atl_vardef(char *name, int size) {
    dictword *di;
    int isize = (size + (sizeof(stackitem) - 1)) / sizeof(stackitem);

#undef Memerrs
#define Memerrs NULL
    evalstat = ATL_SNORM;
    Ho(Dictwordl + isize);
#undef Memerrs
#define Memerrs
    if (evalstat != ATL_SNORM)	      /* Did the heap overflow */
        return NULL;		      /* Yes.  Return NULL */
    atl__env->createWord = (dictword *) atl__env->heapAllocPtr;   /* Develop address of word */
    atl__env->createWord->wcode = P_var;	      /* Store default code */
    atl__env->heapAllocPtr += Dictwordl;		      /* Allocate heap space for word */
    while (isize > 0) {
        Hstore = 0;		      /* Allocate heap area and clear it */
        isize--;
    }
    strcpy(tokbuf, name);	      /* Use built-in token buffer... */
    ucase(tokbuf);                    /* so ucase() doesn't wreck arg string */
    enter(tokbuf);		      /* Make dictionary entry for it */
    di = atl__env->createWord;		      /* Save word address */
    atl__env->createWord = NULL;		      /* Mark no word underway */
    return di;			      /* Return new word */
}

/*  ATL_MARK  --  Mark current state of the system.  */

void atl_mark(atl_statemark *mp) {
    mp->mstack = atl__env->stk;		      /* Save stack position */
    mp->mheap = atl__env->heapAllocPtr;		      /* Save heap allocation marker */
    mp->mrstack = atl__env->rs; 	      /* Set return stack pointer */
    mp->mdict = atl__env->dict;		      /* Save last item in dictionary */
}

/*  ATL_UNWIND	--  Restore system state to previously saved state.  */

void atl_unwind(atl_statemark *mp) {

    /* If atl_mark() was called before the system was initialised, and
     we've initialised since, we cannot unwind.  Just ignore the
     unwind request.	The user must manually atl_init before an
     atl_mark() request is made. */

    if (mp->mdict == NULL)	      /* Was mark made before atl_init ? */
        return; 		      /* Yes.  Cannot unwind past init */

    atl__env->stk = mp->mstack;		      /* Roll back stack allocation */
    atl__env->heapAllocPtr = mp->mheap;		      /* Reset heap state */
    atl__env->rs = mp->mrstack; 	      /* Reset the return stack */

    /* To unwind the dictionary, we can't just reset the pointer,
     we must walk back through the chain and release all the name
     buffers attached to the items allocated after the mark was
     made. */

    while (atl__env->dict != NULL && atl__env->dict != atl__env->dictFirstProtectedEntry && atl__env->dict != mp->mdict) {
        free(atl__env->dict->wname);	      /* Release name string for item */
        atl__env->dict = atl__env->dict->wnext;	      /* Link to previous item */
    }
}

#ifdef BREAK

// ATL_BREAK  --  Asynchronously interrupt execution.
// Note that this function only sets a flag, broken, that causes
// exword() to halt after the current word.  Since this can be
// called at any time, it daren't touch the system state directly,
// as it may be in an unstable condition.
//
void atl_break(void) {
    broken = atlTrue;		      /* Set break request */
}
#endif /* BREAK */

/*  ATL_LOAD  --  Load a file into the system.	*/

int atl_load(FILE *fp) {
    int es = ATL_SNORM;
    char s[134];
    atl_statemark mk;
    atl_int scomm = atl__env->isIgnoringComment;    // stack comment pending state
    dictword **sip = atl__env->ip;	      /* Stack instruction pointer */
    char *sinstr = atl__env->inputBuffer;   // stack input stream
    int lineno = 0;		      /* Current line number */

    atl__env->lineNumberLastLoadFailed = 0; // reset line number of error
    atl_mark(&mk);
    atl__env->ip = NULL;			      /* Fool atl_eval into interp state */
    while (atl_fgetsp(s, 132, fp) != NULL) {
        lineno++;
        if ((es = atl_eval(s)) != ATL_SNORM) {
            atl__env->lineNumberLastLoadFailed = lineno; // save line number of error
            atl_unwind(&mk);
            break;
        }
    }
    /* If there were no other errors, check for a runaway comment.  If
     we ended the file in comment-ignore mode, set the runaway comment
     error status and unwind the file.  */
    if ((es == ATL_SNORM) && (atl__env->isIgnoringComment == atlTruth)) {
#ifdef MEMMESSAGE
        fprintf(stderr, "\nrunaway `(' comment.\n");
#endif
        es = ATL_RUNCOMM;
        atl_unwind(&mk);
    }
    atl__env->isIgnoringComment = scomm;    // unstack comment pending status
    atl__env->ip = sip;			      /* Unstack instruction pointer */
    atl__env->inputBuffer = sinstr; // unstack input stream
    return es;
}

// ATL_PROLOGUE  --  Recognise and process prologue statement.
// Returns 1 if the statement was part of the prologue and 0 otherwise.
//
int atl_prologue(char *sp) {
    if (strncmp(sp, "\\ *", 3) == 0) {
        char *ap;
        char *vp = sp + 3;
        char *tail;

        ucase(vp);
        const char *proName = "STACK ";
        if (strncmp(vp, proName, strlen(proName)) == 0) {
            if ((ap = strchr(vp, ' ')) != NULL) {
                atl__env->stkLength = strtol(ap + 1, &tail, 10);
#ifdef PROLOGUEDEBUG
                fprintf(stderr, "prologue set %sto %ld\n", proName, atl__env->stkLength);
#endif
                return 1;
            }
        }
        proName = "RSTACK ";
        if (strncmp(vp, proName, strlen(proName)) == 0) {
            if ((ap = strchr(vp, ' ')) != NULL) {
                //sscanf(ap + 1, "%li", &atl__env->rsLength);
                atl__env->rsLength = strtol(ap + 1, &tail, 10);
#ifdef PROLOGUEDEBUG
                fprintf(stderr, "prologue set %sto %ld\n", proName, atl__env->rsLength);
#endif
                return 1;
            }
        }
        proName = "HEAP ";
        if (strncmp(vp, proName, strlen(proName)) == 0) {
            if ((ap = strchr(vp, ' ')) != NULL) {
                atl__env->heapLength = strtol(ap + 1, &tail, 10);
#ifdef PROLOGUEDEBUG
                fprintf(stderr, "prologue set %sto %ld\n", proName, atl__env->heapLength);
#endif
                return 1;
            }
        }
        proName = "TEMPSTRN ";
        if (strncmp(vp, proName, strlen(proName)) == 0) {
            if ((ap = strchr(vp, ' ')) != NULL) {
                atl__env->numberOfTempStringBuffers = strtol(ap + 1, &tail, 10);
#ifdef PROLOGUEDEBUG
                fprintf(stderr, "prologue set %sto %ld\n", proName, atl__env->numberOfTempStringBuffers);
#endif
                return 1;
            }
        }
    }
    return 0;
}

// ATL_EVAL  --  Evaluate a string containing ATLAST words.
//
int atl_eval(char *sp) {
    int i;

#undef  Memerrs
#define Memerrs evalstat
    atl__env->inputBuffer = sp;
    evalstat = ATL_SNORM;	      // Set normal evaluation status

#ifdef BREAK
    broken = atlFalse;		      // Reset asynchronous break
#endif

    // If automatic prologue processing is configured and we haven't yet
    // initialised, check if this is a prologue statement. If so, execute
    // it. Otherwise automatically initialise with the memory specifications
    // currently operative.

#ifdef PROLOGUE
    if (atl__env->dict == NULL) {
        if (atl_prologue(sp)) {
            return evalstat;
        }
        atl_init();
    }
#endif // PROLOGUE

    while ((evalstat == ATL_SNORM) && (i = atl__env->nextToken(&(atl__env->inputBuffer))) != TokNull) {
        dictword *di;

        switch (i) {
            case TokWord:
                if (forgetpend) {
                    forgetpend = atlFalse;
                    ucase(tokbuf);
                    if ((di = lookup(tokbuf)) != NULL) {
                        dictword *dw = atl__env->dict;

                        // Pass 1.  Rip through the dictionary to make sure
                        // this word is not past the marker that
                        // guards against forgetting too much.

                        while (dw != NULL) {
                            if (dw == atl__env->dictFirstProtectedEntry) {
#ifdef MEMMESSAGE
                                fprintf(stderr, "\nforget protected.\n");
#endif
                                evalstat = ATL_FORGETPROT;
                                di = NULL;
                            }
                            if (strcmp(dw->wname + 1, tokbuf) == 0) {
                                break;
                            }
                            dw = dw->wnext;
                        }

                        // Pass 2.  Walk back through the dictionary
                        // items until we encounter the target
                        // of the FORGET.  Release each item's
                        // name buffer and dechain it from the
                        // dictionary list. */

                        if (di != NULL) {
                            do {
                                dw = atl__env->dict;
                                if (dw->wname != NULL) {
                                    free(dw->wname);
                                }
                                atl__env->dict = dw->wnext;
                            } while (dw != di);
                            // Finally, back the heap allocation pointer
                            // up to the start of the last item forgotten.
                            atl__env->heapAllocPtr = (stackitem *) di;
                            // Uhhhh, just one more thing.  If this word
                            // was defined with DOES>, there's a link to
                            // the method address hidden before its
                            // wnext field.  See if it's a DOES> by testing
                            // the wcode field for P_dodoes and, if so,
                            // back up the heap one more item.
                            if (di->wcode == (codeptr) P_dodoes) {
#ifdef FORGETDEBUG
                                fprintf(stderr, " forgetting DOES> word. ");
#endif
                                atl__env->heapAllocPtr--;
                            }
                        }
                    } else {
#ifdef MEMMESSAGE
                        fprintf(stderr, " '%s' undefined ", tokbuf);
#endif
                        evalstat = ATL_UNDEFINED;
                    }
                } else if (tickpend) {
                    tickpend = atlFalse;
                    ucase(tokbuf);
                    if ((di = lookup(tokbuf)) != NULL) {
                        So(1);
                        Push = (stackitem) di; // push word compile address
                    } else {
#ifdef MEMMESSAGE
                        fprintf(stderr, " '%s' undefined ", tokbuf);
#endif
                        evalstat = ATL_UNDEFINED;
                    }
                } else if (defpend) {
                    // If a definition is pending, define the token and
                    // leave the address of the new word item created for
                    // it on the return stack.
                    defpend = atlFalse;
                    ucase(tokbuf);
                    if (atl__env->allowRedefinition && (lookup(tokbuf) != NULL)) {
                        fprintf(stderr, "\n%s isn't unique.", tokbuf);
                    }
                    enter(tokbuf);
                } else {
                    di = lookup(tokbuf);
                    if (di != NULL) {
                        /* Test the state.  If we're interpreting, execute
                         the word in all cases.  If we're compiling,
                         compile the word unless it is a compiler word
                         flagged for immediate execution by the
                         presence of a space as the first character of
                         its name in the dictionary entry. */
                        if (state &&
                            (cbrackpend || ctickpend ||
                             !(di->wname[0] & IMMEDIATE))) {
                                if (ctickpend) {
                                    /* If a compile-time tick preceded this
                                     word, compile a (lit) word to cause its
                                     address to be pushed at execution time. */
                                    Ho(1);
                                    Hstore = s_lit;
                                    ctickpend = atlFalse;
                                }
                                cbrackpend = atlFalse;
                                Ho(1);	  /* Reserve stack space */
                                Hstore = (stackitem) di;/* Compile word address */
                            } else {
                                exword(di);   /* Execute word */
                            }
                    } else {
#ifdef MEMMESSAGE
                        fprintf(stderr, " '%s' undefined ", tokbuf);
#endif
                        evalstat = ATL_UNDEFINED;
                        state = atlFalsity;
                    }
                }
                break;

            case TokInt:
                if (state) {
                    Ho(2);
                    Hstore = s_lit;   /* Push (lit) */
                    Hstore = tokint;  /* Compile actual literal */
                } else {
                    So(1);
                    Push = tokint;
                }
                break;

#ifdef REAL
            case TokReal:
                if (state) {
                    int i;
    	    	    union {
                        atl_real r;
                        stackitem s[Realsize];
                    } tru;

                    Ho(Realsize + 1);
                    Hstore = s_flit;  /* Push (flit) */
    	    	    tru.r = tokreal;
                    for (i = 0; i < Realsize; i++) {
                        Hstore = tru.s[i];
                    }
                } else {
                    int i;
    	    	    union {
                        atl_real r;
                        stackitem s[Realsize];
                    } tru;

                    So(Realsize);
    	    	    tru.r = tokreal;
                    for (i = 0; i < Realsize; i++) {
                        Push = tru.s[i];
                    }
                }
                break;
#endif /* REAL */

#ifdef STRING
            case TokString:
                if (stringlit) {
                    stringlit = atlFalse;
                    if (state) {
                        size_t l = (strlen(tokbuf) + 1 + sizeof(stackitem)) /
                        sizeof(stackitem);
                        Ho(l);
                        *((char *) atl__env->heapAllocPtr) = l;  /* Store in-line skip length */
                        strcpy(((char *) atl__env->heapAllocPtr) + 1, tokbuf);
                        atl__env->heapAllocPtr += l;
                    } else {
                        fprintf(stderr, "%s", tokbuf);
                    }
                } else {
                    if (state) {
                        size_t l = (strlen(tokbuf) + 1 + sizeof(stackitem)) /
                        sizeof(stackitem);
                        Ho(l + 1);
                        /* Compile string literal instruction, followed by
                         in-line skip length and the string literal */
                        Hstore = s_strlit;
                        *((char *) atl__env->heapAllocPtr) = l;  /* Store in-line skip length */
                        strcpy(((char *) atl__env->heapAllocPtr) + 1, tokbuf);
                        atl__env->heapAllocPtr += l;
                    } else {
                        So(1);
                        strcpy(atl__env->strbuf[atl__env->idxCurrTempStringBuffer], tokbuf);
                        Push = (stackitem) atl__env->strbuf[atl__env->idxCurrTempStringBuffer];
                        atl__env->idxCurrTempStringBuffer = (atl__env->idxCurrTempStringBuffer + 1) % ((int) atl__env->numberOfTempStringBuffers);
                    }
                }
                break;
#endif /* STRING */
            default:
                fprintf(stderr, "\nunknown token type %d\n", i);
                break;
        }
    }
    return evalstat;
}
// end of ATLast/atlast.c


#define FALSE	0
#define TRUE	1

#define V   (void)

#ifdef FBmode
#define OUR_READ_MODE "rb"
#else
#define OUR_READ_MODE "r"
#endif

// Globals imported

//=======================================================================
// CatchCtrlC  --  Catch a user console break signal.  If your C library
// does not provide this Unix-compatibile facility (registered with the
// call on signal() in main()), just turn this code off or, better still,
// replace it with the equivalent on your system.
//
static void CatchCtrlC(int sig) {
    if (sig == SIGINT) {
        atl_break();
    }
}

//=======================================================================
//
int main(int argc, const char *argv[]) {
    atl__env = atl__NewInterpreter();

    int   idx;
    int   fname = FALSE, defmode = FALSE;
    FILE *fpInput = stdin;

#define MAX_FNAME_LENGTH 1024

    fprintf(stderr, "ATLast 1.2a (2014/06/19)\n");
    for (idx = 1; idx < argc; idx++) {
        const char *cp = argv[idx];

        if (*cp == '-') {
            const char opt = *(++cp);
            switch (opt) {
                case 'd':
                    defmode = TRUE;
                    argv[idx] = 0;
                    break;
                case 'h':
                    atl__env->heapLength = atol(cp + 1);
                    argv[idx] = 0;
                    break;
                case 'i':
                    break;
                case 'r':
                    atl__env->rsLength = atol(cp + 1);
                    argv[idx] = 0;
                    break;
                case 's':
                    atl__env->stkLength = atol(cp + 1);
                    argv[idx] = 0;
                    break;
                case 't':
                    atl__env->enableTrace = atlTruth;
                    argv[idx] = 0;
                    break;
                case '?':
                case 'u':
                    fprintf(stderr, "Usage:  ATLAST [options] [inputfile]\n");
                    fprintf(stderr, "        Options:\n");
                    fprintf(stderr, "           -d     treat file as definitions\n");
                    fprintf(stderr, "           -ifile include named definition file\n");
                    fprintf(stderr, "           -h##   set heap         length to ##\n");
                    fprintf(stderr, "           -r##   set return stack length to ##\n");
                    fprintf(stderr, "           -s##   set stack        length to ##\n");
                    fprintf(stderr, "           -t     set TRACE mode\n");
                    fprintf(stderr, "           -u     print this message\n");
                    return 0;
            }
        } else if (fname) {
            fprintf(stderr, "error:\tduplicate file name.\n");
            return 1;
        } else {
            char fn[MAX_FNAME_LENGTH];

            fname = TRUE;
            strncpy(fn, cp, MAX_FNAME_LENGTH - 5);
            if (strchr(fn, '.') == NULL) {
                strcat(fn, ".atl");
            }
            fpInput = fopen(fn, "r");
            if (fpInput == NULL) {
                perror(fn);
                fprintf(stderr, "error:\tunable to open file %s\n", fn);
                return 1;
            }
        }
    }

    // initialize the interpreter?
    //
    atl_init();

    for (idx = 1; idx < argc; idx++) {
        if (!argv[idx]) {
            continue;
        }
        if (strncmp(argv[idx], "-i", 2) == 0) {
            // load each include as passed in
            //
            char fileNameInclude[MAX_FNAME_LENGTH];

            strncpy(fileNameInclude, argv[idx] + 2, MAX_FNAME_LENGTH - 5);
            if (strchr(fileNameInclude, '.') == NULL) {
                strcat(fileNameInclude, ".atl");
            }
            FILE *fpInclude = fopen(fileNameInclude, OUR_READ_MODE);
            if (!fpInclude) {
                perror(fileNameInclude);
                fprintf(stderr, "error:\ttnable to open include file %s\n", fileNameInclude);
                return 1;
            }
            int statusInclude = atl_load(fpInclude);
            fclose(fpInclude);
            if (statusInclude != ATL_SNORM) {
                fprintf(stderr, "\nerror:\t%d in include file %s\n", statusInclude, fileNameInclude);
                return 1;
            }
        } else {
            fprintf(stderr, "error:\tunknown option '%s'\n", argv[idx]);
            return 1;
        }
    }

    // Now that all the preliminaries are out of the way, fall into the main ATLAST execution loop.

    signal(SIGINT, CatchCtrlC);

    while (TRUE) {
        char t[132];

        if (!fname) {
            // prompt shows pending comment and compiling state
            fprintf(stderr, atl__env->isIgnoringComment ? "(  " : (((atl__env->heap != NULL) && state) ? ":> " : "-> "));
        }
        if (fgets(t, 132, fpInput) == NULL) {
            if (fname && defmode) {
                fname = defmode = FALSE;
                fpInput = stdin;
                continue;
            }
            break;
        }
        atl_eval(t);
    }
    if (!fname) {
        fprintf(stderr, "\n");
    }
    atl_memstat();

    return 0;
}
