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
#ifndef __READER_H
#define __READER_H

#include "firtex/common/StdHeader.h"
#include "Charset.h"
#include <string>
using namespace std;


FX_NS_DEF(utility)

class Reader
{	
public:
	Reader(char* buffer,size_t buffsize);
	Reader(size_t buffsize = 0);			
	virtual ~Reader(void);
public:
	/**
	 * \if EN
	 * read data
	 * @param data address to store data
	 * @param length length to read
 	 * \endif
	 *
	 * \if CN
	 * 读取数据块
	 * @param data 目标数据块首地址
	 * @param length 目标数据块大小
	 * \endif
	 */
	void		read(char* data, size_t length);

	/**
	 * \if EN
	 * read data without copy operation
	 * @param length data size
	 * @return data pointer
	 * \endif
	 *
	 * \if CN
	 * 直接读取数据块,没有数据拷贝			 
	 * @param length 数据块大小
	 * @return 数据块首地址
	 * \endif
	 */
	char*		readWithoutCopy(size_t& length);		

	/**
	 * \if EN
	 * get current position
	 * \endif
	 *
	 * \if CN
	 * 返回在文件中的当前位置，下一次读从这个位置开始
	 * \endif
	 */			
	int64_t		getFilePointer(){return m_bufferStart + m_bufferPosition;}

	/**
	 * \if EN
	 * seek to position
	 * @param pos seek to specific position
	 * \endif
	 *
	 * \if CN
	 * 设置文件的当前读位置，下一次读从这个位置开始
	 * @param pos 定位的位置
	 * \endif
	 */	
	void		seek(int64_t pos);	

	/**
	 * \if EN
	 * get file size
	 * \endif
	 *
	 * \if CN
	 * 返回文件长度			 
	 * \endif
	 */	
	int64_t		length(){return m_length;}

	/**
	 * set encoding of the reader
	 * @param _charset Charset of the content 
	 */
	void		setEncoding(charset_t _charset){m_encoding = _charset;}

	/**
	 * get encoding of the reader 
	 * @return encoding 
	 */
	charset_t	getEncoding(){return m_encoding;}

	/**
	 * \if EN
	 * end or not
	 * \endif
	 *
	 * \if CN
	 * 判断文件数据是否读完毕
	 * \endif
	 */
	virtual bool	isEof(){return ( (m_bufferStart + (int64_t )m_bufferPosition) >= m_length);}

	/**
	 * \if EN
	 * close file
	 * \endif
	 *
	 * \if CN
	 * 关闭文件，子类需实现
	 * \endif
	 */
	virtual void  close();
public:
	/**
	 * \if EN
	 * clone an object 
	 * \endif
	 *
	 * \if CN
	 * 克隆一个Reader对象,克隆对象使用buffer作为缓冲	 
	 * \endif
	 */			
	virtual Reader*	clone(char* buffer,size_t buffsize) = 0;

	/**
	* \if EN
	 * clone an object
	 *
	 * \endif
	 *
	 * \if CN
	 * 克隆一个Reader对象,克隆对象使用自己私有的内存作为缓冲	 
	 * \endif
 	 */		
	virtual Reader*	clone() = 0;
protected:
	/**
	 * \if EN
	 * seek to position, sub-class MUST implement it
	 * @param pos seek position
	 * \endif
	 *
	 * \if CN
	 * 实现文件定位操作，readInternal的读操作从这个位置开始，需子类实现
	 * @param pos 定位位置
	 * \endif
	 */				
	virtual void  seekInternal(int64_t position) = 0;

	/**
	 * \if EN
	 * read data from data source, sub-class MUST implement it 
	 * @param b target address
	 * @param offset read offset 
	 * @length length of data
	 * \endif
	 *
	 * \if CN
	 * 从文件中当前位置读数据，子类需实现此函数
	 * @param b 目标地址
	 * @param offset 偏移量
	 * @length 长度
	 \endif
	 */
	virtual void  readInternal(char* b, size_t offset, size_t length) = 0;	

protected:
	/**
	 * \if EN
	 * fill data to buffer
	 * \endif
	 *
	 * \if CN
	 * 从文件中中读数据填充到缓冲区中
	 * \endif
	 */
	void	refill();	
public:
	const static int READER_BUFFSIZE = 32768*10;
protected:
	char*		m_buffer;
	size_t		m_bufferSize;			

	int64_t		m_bufferStart;		/// position in file of m_buffer
	size_t		m_bufferLength;		/// end of valid bytes
	size_t		m_bufferPosition;	/// next byte to read

	int64_t		m_length;			/// set by subclasses
	bool		m_bOwnBuff;

	charset_t	m_encoding;			///encoding name
};

//////////////////////////////////////////////////////////////////////////
//
inline void  Reader::refill()
{
	int64_t start = m_bufferStart + (int64_t)m_bufferPosition;
	int64_t end = start + (int64_t)m_bufferSize;
	if (end > m_length)	//超过了结尾					
		end = m_length;
	m_bufferLength = (size_t) (end - start);
	if (m_bufferLength == 0)
		return;

	if (m_buffer == NULL)
		m_buffer = new char[m_bufferSize]; // allocate m_buffer lazily
	readInternal(m_buffer, 0, m_bufferLength);

	m_bufferStart = start;
	m_bufferPosition = 0;
}

FX_NS_END

#endif
