#include "firtex/extension/util/Regex.h"

#define LINK_SIZE 2
#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>
#include "pcre2_internal.h"

using namespace std;

FX_NS_DEF(util);

SETUP_LOGGER(util, Regex);

Regex::Regex() 
    : m_pRegex(NULL)
{
}

Regex::~Regex() 
{
    if (m_pRegex != NULL)
    {
        regfree(m_pRegex);
        delete m_pRegex;
        m_pRegex = NULL;
    }
}

bool Regex::compile(const char* pattern)
{
    FX_LOG(DEBUG, "Compile pattern: [%s]", pattern);
    if (m_pRegex != NULL)
    {
        regfree(m_pRegex);
        delete m_pRegex;
    }
    m_pRegex = new regex_t;

    int ret = regcomp(m_pRegex, pattern, 0);

    if (ret != 0)
    {
        char buffer[256];
        regerror(ret, m_pRegex, buffer, sizeof(buffer));
        FX_LOG(ERROR, "Regex compilation FAILED: [%s]", buffer);
        return false;
    }
    m_sPattern = pattern;
    return true;
}

bool Regex::compile(const std::string& sPattern)
{
    return compile(sPattern.c_str());
}

Regex::MATCH_STATUS Regex::match(const std::string& sSubject,
                                 Regex::GroupVect& groups) const
{
    const size_t MATCH_SUB_STRING_COUNT = 10;
    regmatch_t subStrings[MATCH_SUB_STRING_COUNT];

    if (m_pRegex == NULL)
    {
        FX_LOG(ERROR, "Pattern not compiled");
        return MATCH_ERROR;
    }
    
    int ret = regexec(m_pRegex, sSubject.c_str(), MATCH_SUB_STRING_COUNT, subStrings, 0);

    if (ret == REG_NOMATCH)
    {
        FX_LOG(INFO, "No match"); 
        return MATCH_NOMATCH;
    }
    else if (ret != 0)
    {
        char buffer[256];
        regerror(ret, m_pRegex, buffer, sizeof(buffer));
        FX_LOG(ERROR, "Matching ERROR: [%s]", buffer);
        return MATCH_ERROR;
    }
    else 
    {
        for (size_t i = 0; i <= m_pRegex->re_nsub; i++) 
        {
            size_t len = subStrings[i].rm_eo - subStrings[i].rm_so;
            string str(sSubject.c_str() + subStrings[i].rm_so, len);
            groups.push_back(str);
        }
        return MATCH_OK;
    }
}

Regex::MATCH_STATUS Regex::match(const std::string& sSubject) const
{
    FX_LOG(DEBUG, "Match subject: [%s], pattern: [%s]", 
           sSubject.c_str(), m_sPattern.c_str());
    MATCH_STATUS ms = MATCH_OK;
    int ret = regexec(m_pRegex, sSubject.c_str(), 0, NULL, 0);
    if (ret == REG_NOMATCH)
    {
        FX_DEBUG("No match"); 
        return MATCH_NOMATCH;
    }
    else if (ret != 0)
    {
        char buffer[256];
        regerror(ret, m_pRegex, buffer, sizeof(buffer));
        FX_DEBUG("Matching ERROR: [%s]", buffer);
        return MATCH_ERROR;
    }
    else 
    {
        return MATCH_OK;
    }
}

//=======================================
static const int eint1[] = 
{
    0,           /* No error */
    Regex::REG_EESCAPE, /* \ at end of pattern */
    Regex::REG_EESCAPE, /* \c at end of pattern */
    Regex::REG_EESCAPE, /* unrecognized character follows \ */
    Regex::REG_BADBR,   /* numbers out of order in {} quantifier */
    /* 5 */
    Regex::REG_BADBR,   /* number too big in {} quantifier */
    Regex::REG_EBRACK,  /* missing terminating ] for character class */
    Regex::REG_ECTYPE,  /* invalid escape sequence in character class */
    Regex::REG_ERANGE,  /* range out of order in character class */
    Regex::REG_BADRPT,  /* nothing to repeat */
    /* 10 */
    Regex::REG_ASSERT,  /* internal error: unexpected repeat */
    Regex::REG_BADPAT,  /* unrecognized character after (? or (?- */
    Regex::REG_BADPAT,  /* POSIX named classes are supported only within a class */
    Regex::REG_BADPAT,  /* POSIX collating elements are not supported */
    Regex::REG_EPAREN,  /* missing ) */
    /* 15 */
    Regex::REG_ESUBREG, /* reference to non-existent subpattern */
    Regex::REG_INVARG,  /* pattern passed as NULL */
    Regex::REG_INVARG,  /* unknown compile-time option bit(s) */
    Regex::REG_EPAREN,  /* missing ) after (?# comment */
    Regex::REG_ESIZE,   /* parentheses nested too deeply */
    /* 20 */
    Regex::REG_ESIZE,   /* regular expression too large */
    Regex::REG_ESPACE,  /* failed to get memory */
    Regex::REG_EPAREN,  /* unmatched closing parenthesis */
    Regex::REG_ASSERT   /* internal error: code overflow */
};

static const int eint2[] = {
    30, Regex::REG_ECTYPE,  /* unknown POSIX class name */
    32, Regex::REG_INVARG,  /* this version of PCRE does not have UTF or UCP support */
    37, Regex::REG_EESCAPE, /* PCRE2 does not support \L, \l, \N{name}, \U, or \u */
    56, Regex::REG_INVARG,  /* internal error: unknown newline setting */
};

/* Table of texts corresponding to POSIX error codes */

static const char *const pstring[] = {
    "",                                /* Dummy for value 0 */
    "internal error",                  /* REG_ASSERT */
    "invalid repeat counts in {}",     /* BADBR      */
    "pattern error",                   /* BADPAT     */
    "? * + invalid",                   /* BADRPT     */
    "unbalanced {}",                   /* EBRACE     */
    "unbalanced []",                   /* EBRACK     */
    "collation error - not relevant",  /* ECOLLATE   */
    "bad class",                       /* ECTYPE     */
    "bad escape sequence",             /* EESCAPE    */
    "empty expression",                /* EMPTY      */
    "unbalanced ()",                   /* EPAREN     */
    "bad range inside []",             /* ERANGE     */
    "expression too big",              /* ESIZE      */
    "failed to get memory",            /* ESPACE     */
    "bad back reference",              /* ESUBREG    */
    "bad argument",                    /* INVARG     */
    "match failed"                     /* NOMATCH    */
};


int Regex::regcomp(regex_t *preg, const char *pattern, int cflags)
{
    PCRE2_SIZE erroffset;
    int errorcode;
    int options = 0;
    int re_nsub = 0;

    if ((cflags & REG_ICASE) != 0)    options |= PCRE2_CASELESS;
    if ((cflags & REG_NEWLINE) != 0)  options |= PCRE2_MULTILINE;
    if ((cflags & REG_DOTALL) != 0)   options |= PCRE2_DOTALL;
    if ((cflags & REG_NOSUB) != 0)    options |= PCRE2_NO_AUTO_CAPTURE;
    if ((cflags & REG_UTF) != 0)      options |= PCRE2_UTF;
    if ((cflags & REG_UCP) != 0)      options |= PCRE2_UCP;
    if ((cflags & REG_UNGREEDY) != 0) options |= PCRE2_UNGREEDY;

    preg->re_pcre2_code = pcre2_compile((PCRE2_SPTR)pattern, PCRE2_ZERO_TERMINATED,
            options, &errorcode, &erroffset, NULL);
    preg->re_erroffset = erroffset;

    if (preg->re_pcre2_code == NULL)
    {
        unsigned int i;
        if (errorcode < 0) return REG_BADPAT;   /* UTF error */
        errorcode -= COMPILE_ERROR_BASE;
        if (errorcode < (int)(sizeof(eint1)/sizeof(const int)))
            return eint1[errorcode];
        for (i = 0; i < sizeof(eint2)/(2*sizeof(const int)); i += 2)
            if (errorcode == eint2[i]) return eint2[i+1];
        return REG_BADPAT;
    }

    (void)pcre2_pattern_info((const pcre2_code *)preg->re_pcre2_code,
                             PCRE2_INFO_CAPTURECOUNT, &re_nsub);
    preg->re_nsub = (size_t)re_nsub;
    if ((options & PCRE2_NO_AUTO_CAPTURE) != 0) re_nsub = -1;
    preg->re_match_data = pcre2_match_data_create(re_nsub + 1, NULL);
    return 0;

}

int Regex::regexec(const regex_t *preg, const char *string, size_t nmatch,
                   regmatch_t pmatch[], int eflags)
{
    int rc, so, eo;
    int options = 0;
    pcre2_match_data *md = (pcre2_match_data *)preg->re_match_data;

    if ((eflags & REG_NOTBOL) != 0) options |= PCRE2_NOTBOL;
    if ((eflags & REG_NOTEOL) != 0) options |= PCRE2_NOTEOL;
    if ((eflags & REG_NOTEMPTY) != 0) options |= PCRE2_NOTEMPTY;

    ((regex_t *)preg)->re_erroffset = (size_t)(-1);  /* Only has meaning after compile */

/* When no string data is being returned, or no vector has been passed in which
   to put it, ensure that nmatch is zero. */

    if ((((pcre2_real_code *)(preg->re_pcre2_code))->compile_options &
         PCRE2_NO_AUTO_CAPTURE) != 0 || pmatch == NULL) nmatch = 0;

/* REG_STARTEND is a BSD extension, to allow for non-NUL-terminated strings.
   The man page from OS X says "REG_STARTEND affects only the location of the
   string, not how it is matched". That is why the "so" value is used to bump the
   start location rather than being passed as a PCRE2 "starting offset". */

    if ((eflags & REG_STARTEND) != 0)
    {
        so = pmatch[0].rm_so;
        eo = pmatch[0].rm_eo;
    }
    else
    {
        so = 0;
        eo = (int)strlen(string);
    }

    rc = pcre2_match((const pcre2_code *)preg->re_pcre2_code,
                     (PCRE2_SPTR)string + so, (eo - so), 0, options, md, NULL);

/* Successful match */

    if (rc >= 0)
    {
        size_t i;
        if ((size_t)rc > nmatch) rc = (int)nmatch;
        for (i = 0; i < (size_t)rc; i++)
        {
            pmatch[i].rm_so = md->ovector[i*2];
            pmatch[i].rm_eo = md->ovector[i*2+1];
        }
        for (; i < nmatch; i++) pmatch[i].rm_so = pmatch[i].rm_eo = -1;
        return 0;
    }

/* Unsuccessful match */

    if (rc <= PCRE2_ERROR_UTF8_ERR1 && rc >= PCRE2_ERROR_UTF8_ERR21)
        return REG_INVARG;

    switch(rc)
    {
    default: return REG_ASSERT;
    case PCRE2_ERROR_BADMODE: return REG_INVARG;
    case PCRE2_ERROR_BADMAGIC: return REG_INVARG;
    case PCRE2_ERROR_BADOPTION: return REG_INVARG;
    case PCRE2_ERROR_BADUTFOFFSET: return REG_INVARG;
    case PCRE2_ERROR_MATCHLIMIT: return REG_ESPACE;
    case PCRE2_ERROR_NOMATCH: return REG_NOMATCH;
    case PCRE2_ERROR_NOMEMORY: return REG_ESPACE;
    case PCRE2_ERROR_NULL: return REG_INVARG;
    }

}

size_t Regex::regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size)
{
    const char *message, *addmessage;
    size_t length, addlength;

    message = (errcode >= (int)(sizeof(pstring)/sizeof(char *)))?
              "unknown error code" : pstring[errcode];
    length = strlen(message) + 1;

    addmessage = " at offset ";
    addlength = (preg != NULL && (int)preg->re_erroffset != -1)?
                strlen(addmessage) + 6 : 0;

    if (errbuf_size > 0)
    {
        if (addlength > 0 && errbuf_size >= length + addlength)
            sprintf(errbuf, "%s%s%-6d", message, addmessage, (int)preg->re_erroffset);
        else
        {
            strncpy(errbuf, message, errbuf_size - 1);
            errbuf[errbuf_size-1] = 0;
        }
    }

    return length + addlength;
}

void Regex::regfree(regex_t *preg)
{
    pcre2_match_data_free((pcre2_match_data*)preg->re_match_data);
    pcre2_code_free((pcre2_code*)preg->re_pcre2_code);
}


FX_NS_END

