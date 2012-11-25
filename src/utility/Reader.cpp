//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: 郭瑞杰(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/5/20
//
#include "firtex/common/StdHeader.h"
#include "firtex/utility/Reader.h"

FX_NS_DEF(utility)

Reader::Reader(char* buffer,size_t buffsize)
	: m_encoding(Charset::UNKNOWN)
{
	if(buffer)
	{
		m_buffer = buffer;
		m_bufferSize = buffsize;
		m_bOwnBuff = false;
	}				

	m_bufferStart = 0; 
	m_bufferLength = 0; 
	m_bufferPosition = 0;

	m_length = 0;
}

Reader::Reader(size_t buffsize)
	: m_encoding(Charset::UNKNOWN)
{
	if(buffsize > 0)
	{
		m_buffer = new char[buffsize];
		m_bufferSize = buffsize;
	}
	else 
	{
		m_buffer = new char[READER_BUFFSIZE];
		m_bufferSize = READER_BUFFSIZE;
	}				

	m_bOwnBuff = true;

	m_bufferStart = 0; 
	m_bufferLength = 0; 
	m_bufferPosition = 0;

	m_length = 0;
}	

Reader::~Reader(void)
{
	if(m_bOwnBuff)
	{
		if(m_buffer)
		{
			delete[]m_buffer;
			m_buffer = NULL;
		}
	}
}

void Reader::read(char* data, size_t length)
{
	if(m_length == 0)
	{
		length = 0;
		return;
	}
	if (m_bufferPosition >= m_bufferLength)
		refill();
	if(length <= (m_bufferLength-m_bufferPosition))
	{
		memcpy(data,m_buffer + m_bufferPosition,length);
		m_bufferPosition += length;
	}
	else 
	{
		size_t start = m_bufferLength-m_bufferPosition;
		if(start > 0)//缓存中还有数据，先读缓存
		{
			memcpy(data,m_buffer + m_bufferPosition,start);						
		}

		readInternal(data,start,length - start); //直接读		
		m_bufferStart += length;
	}
}	

char* Reader::readWithoutCopy(size_t& length)
{
	if (m_bufferPosition >= m_bufferLength)
		refill();
	length = m_bufferLength;
	m_bufferPosition += length;
	return m_buffer;
}

void  Reader::seek(int64_t pos)
{
	if (pos >= m_bufferStart && pos < (m_bufferStart + (int64_t)m_bufferLength))
		m_bufferPosition = (size_t) (pos - m_bufferStart);//新位置在缓冲区间				
	else
	{
		m_bufferStart = pos;
		m_bufferPosition = 0;
		m_bufferLength = 0; // trigger refill() on read()
		seekInternal(pos);
	}
}
void Reader::close()
{
	m_bufferStart = 0; 
	m_bufferLength = 0; 
	m_bufferPosition = 0;
	m_length = 0;
}

FX_NS_END


