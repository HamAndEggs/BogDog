/*
 * InFile.cpp
 *
 *  Created on: 20 Jun 2012
 *      Author: richard
 */

#include <stddef.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include "InFile.h"


namespace BogDog
{


InFile::InFile()
{
	Init();
}

InFile::InFile(const char* pFile_name)
{
	Init();
	Open(pFile_name);
}

void InFile::Init()
{
	m_file = -1;
	m_file_pos = (unsigned)-1;
	m_file_size = 0;
	m_end_of_file = false;
	m_mapped_memory = NULL;
}

bool InFile::Open(const char* pFile_name)
{
    m_file = open(pFile_name,O_RDONLY);
    if( m_file == -1 )
    {
		printf("Failed to open file %s\n",pFile_name);
        return false;
    }

    struct stat file_info;
    fstat(m_file,&file_info);
    m_file_size = file_info.st_size;
	m_file_pos = 0;

	return true;
}

InFile::~InFile()
{
	Close();
}

void InFile::Close()
{
	UnmapFile();

	if( m_file != -1 )
	{
        close(m_file);
		m_file = -1;
	}
	m_file_pos = -1;
}

InFile::operator bool()const
{
	return m_file != -1?true:false;
}

//NOTE: should really be returning the number of bytes read
bool InFile::Read(void *pDest,int pNum_bytes)
{
	if( m_mapped_memory )
	{
		if(m_file_pos < m_file_size)
		{
			memcpy(pDest,m_mapped_memory + m_file_pos,pNum_bytes);
			m_file_pos += pNum_bytes;
			m_end_of_file = m_file_pos >= m_file_size;
		}
		return (m_file_pos > m_file_size);//NOTE: not the same as EOF condition
	}

	if( m_file == -1 )
	{
		assert(m_file != -1);
		return false;
	}

    size_t num_read = read(m_file,pDest,pNum_bytes);
    if( num_read == (size_t)-1 )
    {
        m_end_of_file = true;
    }
    else
    {
        m_file_pos += num_read;
        m_end_of_file = m_file_pos >= m_file_size;
    }

	if( m_file_pos > m_file_size )//NOTE: not the same as EOF condition
		return false;

	return (int)num_read == pNum_bytes;
}

//Assumes file is a text file.
bool InFile::ReadLine(char *pDest,int pDest_max_size)
{
	//Check for end of file.
	if( m_end_of_file )
		return false;

	char v;

	//Scan to first good char.
	do
	{
		Read(&v,1);
	}while(v<32 && m_end_of_file == false);

	//Check for end of file.
	if( m_end_of_file )
		return false;

	//Now scan in as much of the line as we can into the buffer.
	int n;
	for( n = 0 ; n < pDest_max_size && ( (unsigned)v > 31 || v == '\t') && m_end_of_file == false ; n++ )
	{
		pDest[n] = v;//V is the last ok char read, first time in loop its the good char the above code read.
		Read(&v,1);
	}

	//Deals with when end of the last line is also at the end of the file.
	if( m_end_of_file )
	{
		pDest[n] = v;
		n++;
	}

	pDest[n] = '\0';
	return true;
}

size_t InFile::Seek(const size_t pByte_offset,bool pFrom_current_pos)
{
	if( m_mapped_memory )
	{
		if( pFrom_current_pos )
			m_file_pos += pByte_offset;
		else
			m_file_pos = pByte_offset;
		m_end_of_file = m_file_pos >= m_file_size;
		return m_file_pos;
	}

	m_file_pos = lseek(m_file,pByte_offset,pFrom_current_pos?SEEK_CUR:SEEK_SET);

	m_end_of_file = m_file_pos >= m_file_size;

	return m_file_pos;
}

size_t InFile::BytesLeftToRead()
{
	return m_file_size - GetPos();
}

const void *InFile::MapFile()
{
	if( m_mapped_memory == NULL && m_file != -1 )
	{
		m_mapped_memory = (uint8_t*)mmap(0,m_file_size,PROT_READ,MAP_SHARED,m_file,0);
	}
	return (void*)m_mapped_memory;
}

void InFile::UnmapFile()
{
	if( m_mapped_memory )
	{
		munmap(m_mapped_memory,m_file_size);
		m_mapped_memory = NULL;
	}

}

//Used for reading tiff files, ugh lots of seeking. Don't like tiff files,
//but geotiff are very big so best we can do is let the OS buffer the file for us.
//Remebers current file pos, seeks to pOffset, reads the data, seks back to where it was.
//Maybe a bit of tweeking in tiff loading code could speed it up, but the code is nice and clean at the mo. :)
void InFile::SeekRead(void *pDest,int pOffset,int pData_sizeof,bool pRestore_file_pos)
{
	if( m_mapped_memory )
	{
		memcpy(pDest,(uint8_t*)m_mapped_memory + pOffset,pData_sizeof);
		if( !pRestore_file_pos )
		{
			m_file_pos += pOffset + pData_sizeof;
		}
		return;
	}

	int pos = (int)m_file_pos;

	Seek(pOffset);
	Read(pDest,pData_sizeof);

	if( pRestore_file_pos )
	{
		Seek(pos);
	}
}



} /* namespace BogDog */
