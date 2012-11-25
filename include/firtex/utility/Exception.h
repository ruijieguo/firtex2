//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2005/12/16
//
#ifndef ____EXCEPTION_H__
#define ____EXCEPTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/utility/StringUtils.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstddef>
#include <stdio.h>
#include <assert.h>

#ifdef FX_WINDOWS
#pragma warning(push)
#pragma warning(disable:4290)
#endif //FX_WINDOWS

FX_NS_DEF(utility);

/**
 * @class FirteXException
 *
 * Serves as a general base class for the Exception hierarchy used within
 * this package.
 *
 */
class FirteXException
{
    // Restrict heap allocation
    static void * operator new(size_t size);
    static void * operator new[](size_t size);

public:
    //! Create a new exception
    FirteXException();

    //! Create a new exception
    FirteXException(const tstring& msg);

    //! Create a new exception
    FirteXException(const tstring& msg, const tstring& file, int line);

public:
    /**
     * Get additional info about the exception
     *
     * @return const char* for the error message
     */
    const tstring& what() const {return m_sWhat;}

protected:	
    tstring	m_sWhat;
}; 

#define FIRTEX_DECLARE_EXCEPTION(exp_class)                             \
    class exp_class : public FirteXException                            \
    {                                                                   \
    public:                                                             \
        exp_class() : FirteXException(_T(#exp_class) ){}                \
        exp_class(const tstring& msg) : FirteXException(msg) { }        \
        exp_class(const tstring& msg, const tstring& file, int line)    \
            : FirteXException(msg, file, line) {}                       \
        exp_class(const tstring& prefix, const tstring& msg)            \
            : FirteXException(prefix + msg) { }                         \
    };

/**
 * @class IllegalArgumentException
 *
 * Used to describe an illegal argument exception
 */
FIRTEX_DECLARE_EXCEPTION(IllegalArgumentException);

/**
 * @class ExistsException
 *
 * Used to describe an exist exception
 */
FIRTEX_DECLARE_EXCEPTION(ExistsException);

/**
 * @class UnsupportedOpException
 *
 * Used to describe a unsupported operation exception
 */
FIRTEX_DECLARE_EXCEPTION(UnsupportedOpException);

/**
 * @class UnImplementException
 *
 * Used to describe a un-implement exception
 */
FIRTEX_DECLARE_EXCEPTION(UnImplementException);		

/**
 * @class FileIOException
 *
 * Used to describe a file I/O exception
 */
FIRTEX_DECLARE_EXCEPTION(FileIOException);

/**
 * @class OutOfRangeException
 *
 * Used to describe a out of range exception
 */
FIRTEX_DECLARE_EXCEPTION(OutOfRangeException);

/**
 * @class BadParameterException
 *
 * Used to describe a bad parameter exception
 */
FIRTEX_DECLARE_EXCEPTION(BadParameterException);

/**
 * @class MissingArgumentException
 *
 * Used to describe a bad parameter exception
 */
FIRTEX_DECLARE_EXCEPTION(MissingArgumentException);

/**
 * @class RuntimeException
 *
 * Used to describe a runtime exception
 */
FIRTEX_DECLARE_EXCEPTION(RuntimeException);

/**
 * @class NullPointerException
 *
 * Used to describe a null pointer exception
 */
FIRTEX_DECLARE_EXCEPTION(NullPointerException);


/**
 * @class SystemException
 *
 * Used to describe a runtime exception
 */
FIRTEX_DECLARE_EXCEPTION(SystemException);

/**
 * @class OutOfMemoryException
 *
 * Used to describe a runtime exception
 */
FIRTEX_DECLARE_EXCEPTION(OutOfMemoryException);

/**
 * @class UnsupportedException
 *
 * Used to describe an unsupported exception
 */
FIRTEX_DECLARE_EXCEPTION(UnsupportedException);

/**
 * @class CastException
 *
 * Used to describe a cast exception
 */
FIRTEX_DECLARE_EXCEPTION(CastException);

/**
 * @class SyntaxException
 *
 * Used to describe a syntax exception
 */
FIRTEX_DECLARE_EXCEPTION(SyntaxException);

/**
 * @class OutOfOrderException
 *
 * Used to describe a out of order exception
 */
FIRTEX_DECLARE_EXCEPTION(OutOfOrderException);

/**
 * @class ParseException
 *
 * Used to describe a parse exception
 */
FIRTEX_DECLARE_EXCEPTION(ParseException);

/**
 * @class BadXmlFormatException
 *
 * Used to describe a bad xml format exception
 */
FIRTEX_DECLARE_EXCEPTION(BadXmlFormatException);

/**
 * @class AnalyzeException
 *
 * Used to describe a out of order exception
 */
FIRTEX_DECLARE_EXCEPTION(AnalyzeException);

/**
 * @class IndexCollapseException
 *
 * Used to describe an index collapse exception
 */
FIRTEX_DECLARE_EXCEPTION(IndexCollapseException);

/**                                                                             
 * @class QueryParseException                                                   
 *                                                                              
 * Used to describe a query parse exception                                     
 */
FIRTEX_DECLARE_EXCEPTION(QueryParseException);

/**
 * @class VersionException
 *
 * Used to describe an version exception
 */
FIRTEX_DECLARE_EXCEPTION(VersionException);

/**
 * @class AssertException
 *
 * Used to describe a assert exception
 */
FIRTEX_DECLARE_EXCEPTION(AssertException);

/**
 * @class NumberFormatException
 *
 * Used to describe a number format exception
 */
FIRTEX_DECLARE_EXCEPTION(NumberFormatException);

/**
 * @class InvalidConfigException
 *
 * Thrown when there is a invalid configure 
 */
FIRTEX_DECLARE_EXCEPTION(InvalidConfigException);
  
/**
 * @class NotFoundException
 *
 * Thrown when no element can be found
 */
FIRTEX_DECLARE_EXCEPTION(NotFoundException);

/**
 * @class SynchronizationException
 *
 * Used to describe an synchronization exception
 */
FIRTEX_DECLARE_EXCEPTION(SynchronizationException );


/**
 * @class InconsisitentException
 *
 * Used to describe an state inconsisitent exception
 */
FIRTEX_DECLARE_EXCEPTION(InconsisitentException );
  

/**
 * @class InterruptedException
 *
 * Used to describe an interrupted operation that would have normally
 * blocked the calling thread
 */
FIRTEX_DECLARE_EXCEPTION(InterruptedException);
   
  
/**
 * @class DeadlockException
 *
 * Thrown when deadlock has been detected
 */
FIRTEX_DECLARE_EXCEPTION(DeadlockException);
  
/**
 * @class InvalidOpException
 *
 * Thrown when performing an illegal operation this object
 */
FIRTEX_DECLARE_EXCEPTION(InvalidOpException);

/**
 * @class AlreadyExistException
 *
 * Thrown when an object is already exist
 */
FIRTEX_DECLARE_EXCEPTION(AlreadyExistException);
  
/**
 * @class InitializationException
 *
 * Thrown when the system has no more resources to create new 
 * synchronization controls
 */
FIRTEX_DECLARE_EXCEPTION(InitializationException);

  
/**
 * @class CancellationException
 *
 * Cancellation_Exceptions are thrown by 'Canceled' objects. 
 * @see Cancelable
 */
FIRTEX_DECLARE_EXCEPTION(CancellationException);
  

/**
 * @class TimeoutException
 *
 * There is no need for error messaged simply indicates the last
 * operation timed out
 */
FIRTEX_DECLARE_EXCEPTION(TimeoutException);

/**
 * @class NoSuchElementException
 * 
 * The last operation that was attempted on a Queue could not find 
 * the item that was indicated (during that last Queue method invocation)
 */
FIRTEX_DECLARE_EXCEPTION(NoSuchElementException);

/**
 * @class TooManyElementException
 * 
 * The last operation that was attempted on a Queue could not find 
 * the item that was indicated (during that last Queue method invocation)
 */
FIRTEX_DECLARE_EXCEPTION(TooManyElementException);

/**
 * @class InvalidTaskException
 *
 * Thrown when a task is not valid (e.g. null or start()ing a thread with 
 * no overriden run() method)
 */
FIRTEX_DECLARE_EXCEPTION(InvalidTaskException);

/**
 * @class BrokenBarrierException
 *
 * Thrown when a Barrier is broken because one of the participating threads
 * has been interrupted.
 */
FIRTEX_DECLARE_EXCEPTION(BrokenBarrierException);
  
/**
 * @class FutureException
 *
 * Thrown when there is an error using a Future.
 */
FIRTEX_DECLARE_EXCEPTION(FutureException);

/**
 * @class NoThreadAvailableException
 *
 * Thrown when no thread in pool
 */
FIRTEX_DECLARE_EXCEPTION(NoThreadAvailableException);

/**
 * @class NoPermissionException
 *
 * Thrown when no permission
 */
FIRTEX_DECLARE_EXCEPTION(NoPermissionException);

#define FIRTEX_THROW(exception, msg, ...) do                            \
    {                                                                   \
        char buf[1024];                                                 \
        (void)SNPRINTF(buf, 1024, msg, ##__VA_ARGS__);                  \
        throw FX_NS(utility)::exception(buf, __FILE__, __LINE__);       \
    } while (0)

#define FIRTEX_THROW_AND_LOG(exception, msg, ...) do                    \
    {                                                                   \
        char buf[1024];                                                 \
        (void)SNPRINTF(buf, 1024, msg, ##__VA_ARGS__);                  \
        FX_LOG(ERROR, buf);                                             \
        throw FX_NS(utility)::exception(buf, __FILE__, __LINE__);       \
    } while (0)

#define FIRTEX_RETHROW(e) throw e;

#ifdef _DEBUG
#define FIRTEX_ASSERT(exp, msg) if(!(exp)) {FIRTEX_THROW(AssertException, msg);}

#ifdef FX_WINDOWS
#define FIRTEX_ASSERT2(exp) FIRTEX_ASSERT(exp, "Assert failed")
#else
#define FIRTEX_ASSERT2(exp) assert(exp)
#endif

#else 
#define FIRTEX_ASSERT(exp, msg)
#define FIRTEX_ASSERT2(exp)
#endif

FX_NS_END

#ifdef FX_WINDOWS
#pragma warning(pop)
#endif //FX_WINDOWS

#endif
