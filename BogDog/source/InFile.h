/*
 * InFile.h
 *
 *  Created on: 20 Jun 2012
 *      Author: richard
 */

#ifndef INFILE_H_
#define INFILE_H_

#include <stddef.h>
#include <stdint.h>

namespace BogDog
{

/*!
	A Util class I use for file loading, uses the OS file io, quicker than std file io.
*/
struct InFile
{
	/*!
		Default constructor
	*/
	InFile();

	/*!
		Constructor
		@param pFile_name Pathed file name to open.
	*/
	InFile(const char* fileName);

	//!Destructor, calls Close() if file is open.
	virtual ~InFile();

	/*!
		Will close a file that the object may have already opened so the user does not need to worry about calling Close() before Open().
		@param pFile_name Pathed file name to open.
	*/
	bool Open(const char* fileName);

	//!Closes an opened file so the object can be reused on another file.
	void Close();

	/*!
		@returns bool True if file is open.
	*/
	operator bool()const;

	/*!
		@returns bool True if file position is at the end of the file.
	*/
	bool eof()const{return m_end_of_file;}

	/*!
		Reads a given number of bytes to the passed memory buffer.
		@param rDest The memory to recive the bytes read from the file.
		@param pNum_bytes The number of bytes to read into the passed buffer.
		@returns bool True if read ok, false if there was an issue. Either end of file, file not open or file mapped to memory.
	*/
	bool Read(void *rDest,int pNum_bytes);

	/*!
		Template to read an unknow type from a file.
		@param rDest The destination of the file read.
		@returns bool True if read ok, false if there was an issue. File not open or file mapped to memory. For end of file check the return value of 'eof()'
	*/
	template <class TYPE> bool Read(TYPE &rDest)
	{
		return Read(&rDest,sizeof(TYPE));
	}

	/*
		Reads an array of the type. Could not call it Read as it wil interfere with Read(void *rDest,int pNum_bytes)
		And removing that for this one or reanming to ReadBytes will break a lot of code that will compile ok but just crash!
		One prob with this temp plate is when used with arrays of say int fred[10][16] sizeof(*rDest)
		will return the size of fred[0] which is sizeof(int) * 16......... Tried many ways to try to fix but can't.
		So if in doubt use Read(void *rDest,int pNum_bytes)
	*/
	template <class TYPE> bool ReadArray(TYPE *rDest,int pArray_size)
	{
		return Read(rDest,sizeof(*rDest) * pArray_size);
	}

	//!Reads a line of text, assumes file is a text file.
	/*
		@param pDest This is the destination buffer of the line.
		@param pDest_max_size The byte size of the destination buffer for safty.
		@return Returns true if line was read or false on end of file.
	*/
	bool ReadLine(char *pDest,int pDest_max_size);

	/*!
		Remebers current file pos, seeks to pOffset, reads the data, seeks back to where it was if pRestore_file_pos is true.
		Good for reading files like tiff.
		@param rDest The memory to recive the bytes read from the file.
		@param pByte_offset How many bytes to move from the start of the file before reading.
		@param pNum_bytes The number of bytes to read into the passed buffer.
		@param pRestore_file_pos If true then the file position is reset to where it was before the call.
	*/
	void SeekRead(void *rDest,int pOffset,int pData_sizeof,bool pRestore_file_pos = true);

	/*!
		Moves the file position.
		@param pByte_offset How many bytes to move from the start of the file or the current position.
		@param pFrom_current_pos States weather pByte_offset is relative from the current position or the start of the file.
		@return size_t The new position of the file.
	*/
	size_t Seek(const size_t pByte_offset,const bool pFrom_current_pos = false);

	/*!
		For large files of more than 4 gig.
		@return int The current position of the file.
	*/
	size_t GetPos()const{return m_file_pos;}

	/*!
		If the file size is more than 4 gig then this will return 0xffffffff and you should call GetFileSize64 instead.
		@return u64 The size of the file.
	*/
	size_t GetFileSize()const{return m_file_size;}

	/*!
		@return u64 The number of bytes left to read from the file based on its current file pos.
	*/
	size_t BytesLeftToRead();

	/*!
		Returns a memory pointer to the start of the file, if its the first time its been called
		then calls to the OS will be made to map the file to an virtual address space, this is done with the OS in all supported platforms.
		Once called any subsequent calls to standard file reads will fail.
	*/
	const void *MapFile();

	/*!
		Cleans up virtual memory mapping of the file, does not close the file, the file will then be free for
		standard file read calls as before MapFile was first called.
	*/
	void UnmapFile();


private:

	int m_file;

	uint8_t* m_mapped_memory;		//!<The virtual memory pointer the file is mapped to after the first call to MapFile.
	size_t m_file_pos;				//!<Current pos in the file.
	size_t m_file_size;			//!<The size of the file open.
	bool m_end_of_file;			//!<True if we are at the end of the file.

	/*!
		Sets up default values for the files members.
	*/
	void Init();
};

} /* namespace BogDog */
#endif /* INFILE_H_ */
