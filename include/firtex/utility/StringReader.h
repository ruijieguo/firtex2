//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/5/20
//
#ifndef _STRINGREADER_H
#define _STRINGREADER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "firtex/common/StdHeader.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/Exception.h"
#include <string>
#include "Reader.h"

FX_NS_DEF(utility)

class StringReader :	public Reader
{
public:
	StringReader()
		: Reader()
	{

	}

	StringReader(const char* value)
		: Reader(FX_NS(utility)::strLength(value) + 1)
	{			
		if(value && value[0] != 0)
		{	
			strcpy(m_buffer,value);
			m_bufferLength = FX_NS(utility)::strLength(value);
			m_length = m_bufferLength;
		}				
	}

	StringReader(const char* value,const size_t length)
		: Reader(length + 1)
	{				
		memcpy(m_buffer,value,length);
		m_bufferLength = length;
		m_buffer[length] = 0; 				
		m_length = m_bufferLength;
	}

	StringReader(const size_t length)
		: Reader(length + 1)
	{		
		m_bufferLength = length;
		m_buffer[length] = 0; 				
		m_length = m_bufferLength;
	}

	StringReader(const tstring& value)
		: Reader((size_t)value.size() + 1)
	{	
		m_bufferLength = value.size();
		if(m_bufferLength > 0)
		{					
			memcpy(m_buffer,value.c_str(),m_bufferLength);
			m_buffer[m_bufferLength] = 0; 
		}						
		m_length = m_bufferLength;
	}

	virtual ~StringReader(void)
	{				
	}
public:
	Reader*	clone(char* buffer,size_t buffsize)
	{				
		FIRTEX_THROW(UnsupportedException, _T("Unsupport clone a string reader."));
		return NULL;
	}

	Reader*	clone()
	{
		StringReader* pClone = new StringReader(m_buffer,m_bufferLength);
		return pClone;
	}

	char	readChar()
	{
		if (m_bufferPosition >= m_bufferLength)
			refill();
		return m_buffer[m_bufferPosition++];
	}

	char	peek()
	{
		if (m_bufferPosition >= m_bufferLength)
			refill();
		return m_buffer[m_bufferPosition];
	}
	
	bool	open(const char* strBuffer,size_t len)
	{
		Reader::close();
		if(m_bOwnBuff)
			delete[] m_buffer;
		m_buffer = (char*)strBuffer;
		m_length = m_bufferSize = len;
		m_bOwnBuff = false;		
		return true;
	}			

	char* str(){return m_buffer;};

	const char* getReaderName() const{return "StringReader";}
protected:
	void	readInternal(char* b, size_t offset, size_t len){}			
	void	seekInternal(int64_t position){}				
};

FX_NS_END

#endif
