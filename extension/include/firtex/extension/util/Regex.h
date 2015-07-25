//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2015-07-12 20:46:29

#ifndef __FX_REGEX_H
#define __FX_REGEX_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"

FX_NS_DEF(util);

class Regex
{
public:
    const static int REG_ICASE = 0x0001;  /* Maps to PCRE2_CASELESS */
    const static int REG_NEWLINE   = 0x0002;  /* Maps to PCRE2_MULTILINE */
    const static int REG_NOTBOL    = 0x0004;  /* Maps to PCRE2_NOTBOL */
    const static int REG_NOTEOL    = 0x0008;  /* Maps to PCRE2_NOTEOL */
    const static int REG_DOTALL    = 0x0010;  /* NOT defined by POSIX; maps to PCRE2_DOTALL */
    const static int REG_NOSUB     = 0x0020;  /* Maps to PCRE2_NO_AUTO_CAPTURE */
    const static int REG_UTF       = 0x0040;  /* NOT defined by POSIX; maps to PCRE2_UTF */
    const static int REG_STARTEND  = 0x0080;  /* BSD feature: pass subject string by so,eo */
    const static int REG_NOTEMPTY  = 0x0100;  /* NOT defined by POSIX; maps to PCRE2_NOTEMPTY */
    const static int REG_UNGREEDY  = 0x0200;  /* NOT defined by POSIX; maps to PCRE2_UNGREEDY */
    const static int REG_UCP       = 0x0400;  /* NOT defined by POSIX; maps to PCRE2_UCP */

/* This is not used by PCRE2, but by defining it we make it easier
to slot PCRE2 into existing programs that make POSIX calls. */

    const static int REG_EXTENDED  = 0;

    enum 
    {
        REG_ASSERT = 1,  /* internal error ? */
        REG_BADBR,       /* invalid repeat counts in {} */
        REG_BADPAT,      /* pattern error */
        REG_BADRPT,      /* ? * + invalid */
        REG_EBRACE,      /* unbalanced {} */
        REG_EBRACK,      /* unbalanced [] */
        REG_ECOLLATE,    /* collation error - not relevant */
        REG_ECTYPE,      /* bad class */
        REG_EESCAPE,     /* bad escape sequence */
        REG_EMPTY,       /* empty expression */
        REG_EPAREN,      /* unbalanced () */
        REG_ERANGE,      /* bad range inside [] */
        REG_ESIZE,       /* expression too big */
        REG_ESPACE,      /* failed to get memory */
        REG_ESUBREG,     /* bad back reference */
        REG_INVARG,      /* bad argument */
        REG_NOMATCH      /* match failed */
    };

    struct regex_t
    {
        void *re_pcre2_code;
        void *re_match_data;
        size_t re_nsub;
        size_t re_erroffset;
    };

    typedef int regoff_t;

    struct regmatch_t
    {
        regoff_t rm_so;
        regoff_t rm_eo;
    };


public:
    const static int MATCH_OK = 1;
    const static int MATCH_NOMATCH = 2;
    const static int MATCH_ERROR = 2;
    
    typedef int MATCH_STATUS;

    typedef std::vector<std::string> GroupVect;

    // struct regex_t
    // {
    //     regex_t() 
    //         : re_pcre2_code(NULL)
    //         , re_match_data(NULL)
    //         , re_nsub(0)
    //         , re_erroffset(0)
    //     {}
         
    //     void *re_pcre2_code;
    //     void *re_match_data;
    //     size_t re_nsub;
    //     size_t re_erroffset;
    // };

public:
    Regex();
    ~Regex();

public:
    /** 
     * Compile the regular expression pattern
     */
    bool compile(const char* pattern);

    /** 
     * Compile the regular expression pattern
     */
    bool compile(const std::string& sPattern);

    /**
     * Match subject
     * @param sSubject subject string
     * @param groups matched groups
     * @return MATCH_OK if matched
     */
    MATCH_STATUS match(const std::string& sSubject, GroupVect& groups) const;

    /**
     * Match subject
     * @param sSubject subject string
     * @return MATCH_OK if matched
     */
    MATCH_STATUS match(const std::string& sSubject) const;

protected:
    static int regcomp(regex_t *preg, const char *pattern, int cflags);
    static int regexec(const regex_t *preg, const char *string, size_t nmatch,
                       regmatch_t pmatch[], int eflags);
    static size_t regerror(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size);
    static void regfree(regex_t* preg);

private:
    regex_t* m_pRegex;
    std::string m_sPattern;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(Regex);

FX_NS_END

#endif //__FX_REGEX_H
