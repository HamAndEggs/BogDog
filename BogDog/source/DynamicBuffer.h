/*
 *
 *
 *  BogDog GLES 2.0 3D Engine for Raspberry Pi
 *	Copyright (C) 2012  Richard e Collins
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * 	You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DYNAMICBUFFER_H__
#define __DYNAMICBUFFER_H__

#include <assert.h>

namespace BogDog
{

/*!
	This is a dynamic memory array and a memory buffer, it's a proper dynamic array template.
	Two ways to use, one is to have an instance of this class as static in a source file.
	Then when you enter the function that needs a block of work ram call 'SetSize' to make sure its bigenough and then
	get the memory pointer using 'Get'. This is So the buffer only does a mem alloc when there is not any room. Cuts down on memory allocator thrashing. (Can be a big win)

	Or the other was is to treat it as a normal array, for example DynamicBuffer<UString,16,16>texture_names;
	Then to add in sequential order do texture_names.Push(a_texture_name);
	Or if the template is an array of structures do MY_TYPE &next_obj = texture_names.Push();
	The access 'next_obj' to fill in its members etc...

	I've not added a 'Pop' command as that could cause problems that when resulved would cause a slowdown and you may as well as use std::vector.
	The main reason for this template is the big speed up when used as a normal memory array using 'Get' when accessing the array many times.
	Adding could take just as long as it does with std::vector. Or it could be quicker, depends on how you setup the template.
	In all the cases i've used it its given me a significant speed boost over std::vector.

	@param BUFFER_TYPE The base type of this template.
	@param pStart_size The number of elements in the array, so memory allocated will be sizeof(TYPE) * pStart_size. This start size is only used on the first use of the array.
	@param pOver_allocate When an array is not large enough pOver_allocate is added to the size requested, this can help in reducing memory allocations in code where the required size can gradually increase.
*/
template <class BUFFER_TYPE,int pStart_size = 1024,int pOver_allocate = 128> struct DynamicBuffer
{
	typedef DynamicBuffer<BUFFER_TYPE,pStart_size,pOver_allocate> Definition;

	//!Constructor
	DynamicBuffer()
	{
		data = 0L;
		buffer_size = 0;
		size = 0;
	}

	//!Destructor
	~DynamicBuffer()
	{
		FreeBuffer();
	}

	/*!
		Array [] operator used to access element as if they were in a normal array on the stack.
		The array will always return a reference to a valid object as it will grow the array if not big enough.
		As its a non const reference you can use this function for asignment too.
		@param pIndex The index of the object, if the array is not big enough then its is enlarged.
		@return BUFFER_TYPE& A non constant reference to the object at the passed index.
	*/
	BUFFER_TYPE &operator[](size_t pIndex)
	{
		EnsureBufferSize(pIndex,true);
		assert( pIndex < buffer_size );
		return data[pIndex];
	}

	/*!
		Const Array [] operator used to access element as if they were in a normal array on the stack.
		The array will NOT always return a reference to a valid object, it can NOT grow the array if not big enough.
		As its a CONST reference you can NOT use this function for asignment too.
		@param pIndex The index of the const object, if the array is not big enough then its is enlarged.
		@return const BUFFER_TYPE& A non constant reference to the object at the passed index.
	*/
	const BUFFER_TYPE &operator[](size_t pIndex)const
	{
		assert( pIndex < GetSize() );
		return data[pIndex];
	}

	/*!
		Returns the next free slot, enlarges the array. Same as calling "array[array.GetSize()]".
		Has to be a pointer other wise using a reference would cause the code to do a struct copy and not a reassignment of the reference.
	*/
	BUFFER_TYPE *PushBack()
	{
		size_t new_index = size;		//This works as the array is zero based. So this has the effect of adding one on the end.
		EnsureBufferSize(new_index,true);
		assert( new_index < buffer_size );
		return data + new_index;
	}

	/*!
		Copies the data in the passed reference into the array, returns the index of the new item.
	*/
	size_t PushBack(const BUFFER_TYPE& pNew_item)
	{
		size_t new_index = size;
		EnsureBufferSize(new_index,true);
		assert( new_index < buffer_size );
		data[new_index] = pNew_item;
		return new_index;
	}

	/*!
		Copies (Appends) the passed in raw array onto the end of the buffer.
		Returns the index at which the added array starts
	*/
	size_t PushBack(const BUFFER_TYPE* pArray,size_t pArray_length)
	{
		size_t start_point = size;
		size_t x = size;
		SetSize(size + pArray_length,true);
		while( pArray_length-- )
		{
			data[x] = *pArray;//Have to use copy operator. We don't know what the type is!
			pArray++;
			x++;
		}
		return start_point;
	}

	/*!
		Appends the passed dynamic buffer onto *this.
	*/
	void PushBack(const Definition& pSource)
	{
		if( pSource.data && pSource.size > 0 )
		{
			size_t x = size;
			SetSize(size + pSource.size,true);
			assert(data);
			for( size_t n = 0 ; n < pSource.size ; n++ , x++ )
			{
				data[x] = pSource.data[n];
			}
		}
	}

	/*!
		Returns the item at the end of the buffer and reduces the size of the buffer so to give the effect
		of the item being 'popped' off the stack.
	*/
	BUFFER_TYPE& PopBack()
	{
		BUFFER_TYPE& item = GetLast();
		size--;
		return item;
	}

	/*!
		Returns the item at the start of the array, shffles everything down one, reduces the array by one.
		Depending one the array size and the type in the array this could be slow.
	*/
	BUFFER_TYPE& PopFront()
	{
		assert( data );
		assert( size > 0 );

		BUFFER_TYPE& item = data[0];
		if( size > 0 )
		{
			size--;
			for( size_t n = 0 ; n < size ; n++ )
			{
				data[n] = data[n+1];
			}
		}
		return item;
	}

	/*!
		Removes the given item, as the buffer is optimsied for access and not item removal this op can be slow.
	*/
	void Erase(size_t pItem)
	{
		assert( data );
		assert( size > 0 );
		assert( pItem < size );

		for( size_t n = pItem+1 ; n < size ; n++ )
		{
			data[n-1] = data[n];
		}
		size--;//Reduce this as we just reduced in size.
	}

	/*!
		Searches for a item in the array that has the same value, returns it's index.
		If it's a complex class type then you need to implement the == operator, but only if you use this method.
		For arrays of std types you don't have to, for example 'int'.
	*/
	int Find(const BUFFER_TYPE& pItem)
	{
		for( size_t n = 0 ; n < size ; n++ )
		{
			if( data[n] == pItem )
				return (int)n;
		}
		return -1;
	}

	/*!
		This function is for when you need to access the array a large number of times, normally in a for loop.
		It can not do reallocation on array access if the returned array so the caller must not index into the object
		with indices greater than size of the array. Its for improving preformace, lets you access as you would in C.
		@return BUFFER_TYPE* Pointer to the internal memory array. Use with causing as its normal C/C++ so no checking code.
	*/
	BUFFER_TYPE *Get(){return data;}

	/*!
		This function is for when you need to access the array a large number of times, normally in a for loop.
		It returns a const object so modification can not happen. Its used for when there is a DynamicBuffer object in a
		const member of a class and you need to do an optimal array read.
	*/
	const BUFFER_TYPE *Get()const{return data;}

	/*!
		This function is for when you need to access the array a large number of times, normally in a for loop.
		It can not do reallocation on array access if the returned array so the caller must not index into the object
		with indices greater than size of the array. Its for improving preformace, lets you access as you would in C.
		Any data in the array could be lost if the array is grown and pCopy_old_data is set to false. This is what you want to happen most of the time.
		@param pSize The size the buffer must at least be, will only realloc if array is too small. The size of the array will be reported as pSize after this is called.
		@param pClear_array Clears the array with a memset, not the best way and wrong if array is a complex type. But handy for index lists etc...
		@param pCopy_old_data If true the code will copy the old buffer to the new one.
		@return BUFFER_TYPE* Pointer to the internal memory array. Use with causing as its normal C/C++ so no checking code.
	*/
	BUFFER_TYPE *Get(size_t pSize,bool pClear_array = false,bool pCopy_old_data = false)
	{
		SetSize(pSize,pCopy_old_data);
		assert(data);
		if( pClear_array )
		{
			Clear();
		}
		return data;
	}

	/*!
		Returns the last value as a reference. So that you don't have to mess about doing "buffer[buffer.GetSize()-1]".
	*/
	BUFFER_TYPE& GetLast()
	{
		assert( data );
		assert( size > 0 );
		return data[size-1];
	}

	/*!
		Returns the first value as a reference. Just so we have a match for GetLast. :)
	*/
	BUFFER_TYPE& GetFirst()
	{
		assert( data );
		assert( size > 0 );
		return data[0];
	}

	/*!
		@return size_t The size of the array.
	*/
	size_t GetSize()const{return size;}

	/*!
		@return size_t The size of the array in bytes.
	*/
	size_t GetSizeInBytes()const{return size*sizeof(BUFFER_TYPE);}

	/*!
		Does not and can not take into account any dynamic allocation the type does.
		@return size_t The number of bytes in total used, this is the size of the buffer and not number of items * item size.
	*/
	size_t GetMemoryUsage()const{return buffer_size*sizeof(BUFFER_TYPE);}

	/*!
		Returns the index of the item is in the array, if the passed item is not a member then 0xffffffff is returned.
		@param pItem The item to get the index for.
		@return size_t The index of the item or 0xffffffff when there has been a problem.
	*/
	size_t GetIndex(const BUFFER_TYPE *pItem)
	{
		if( pItem < data )
			return 0xffffffff;
		if( pItem >= (data+size) )
			return 0xffffffff;

		return (size_t)(pItem - data);
	}

	/*!
		Resets the array back to zero size, may release the internal memory depending on pFree_internal_memory.
		@param pFree_internal_memory If true the internal memory is free, if false it is not and just cleared instead.
	*/
	void Reset(bool pFree_internal_memory = false)
	{
		if( pFree_internal_memory )
		{
			FreeBuffer();
		}
		//I used to clear exsisting data here but that could cause a memory leek on the objects.
		//but thinking about it I should not need to zero the memory.
		size = 0;
		write_pos = 0;
	}

	/*!
		Sets the size of the buffer, if the size is smaller than the buffer nothing happens.
		If size is larger then the buffer is set to the size + the over allocation value.
		GetSize will return the new size until the array has to grow bigger.
		@param pSize The new size you need the array to be.
		@param pCopy_old_data If true the code will copy the old buffer to the new one.
	*/
	void SetSize(size_t pSize,bool pCopy_old_data = true)
	{
		assert( pSize > 0 );
		EnsureBufferSize(pSize-1,pCopy_old_data);//-1 is because 'EnsureBufferSize' takes an index that the array must be bigenough to accomidate.
		size = pSize;
	}

	/*!
		Copies the data pointed to by pData to the internal memory, makes sure the array is the big enough and also sets
		the 'reported' size of the buffer to pNew_array_size.
		@param pData The data to copy.
		@param pNew_array_size The new size the buffer reports it self to be.
	*/
	void Copy(const BUFFER_TYPE *pData,size_t pNew_array_size)
	{
		if( pNew_array_size < 1 )
		{
			Reset();
		}
		else
		{
			SetSize(pNew_array_size,false);
			for( size = 0 ; size < pNew_array_size ; size++ )
			{
				assert( size < buffer_size );
				//Using copy operator as users type may have one for a good reason.
				data[size] = pData[size];
			};
		}
	}

	/*!
		Copy operator.
		@param pSource The obejct to copy.
		@return const Definition Const version of this object now its been setup to be the same as pSource.
	*/
	const Definition &operator =(const Definition& pSource)
	{
		Copy(pSource.data,pSource.size);
		return *this;
	}

	/*!
		Clears the entire memory buffer setting it all to zero.
		This could be dangerus for a dynamic array of complex objects.
	*/
	void Clear()
	{
		if( data )
		{
			memset(data,0,sizeof(BUFFER_TYPE)*buffer_size);
		}
	}

	//The following functions are for buffering file writes.
	/*!
		This funtion treats the memory as just a load of bytes, so its best to use default buffer with uint8_t as its type.
		Writes data at the sudo file pos, then advances this by the number of bytes written.
		If you are using the dynamic buffer for another type and want to copy and array of the type to the buffer then use 'Copy'
		This is only ment for when the buffer is a file write buffer.
	*/
	template <class TYPE>int Write(const TYPE &pData,size_t pArray_size = 1)
	{
		return WriteBytes((const uint8_t*)(&pData),pArray_size * sizeof(TYPE));
	}

	/*!
		Writes the number of bytes from the pointer passed. Also handy as a lowlevel write.
		Can't be called 'Write' as the template gets its nickers in a twist.
		@param pData The data to be written.
		@param pNum_to_write How many bytes to be written from the pointer passed.
		@return int How many bytes were written
	*/
	int WriteBytes(const void* pData,size_t pNum_to_write)
	{
		if( pNum_to_write < 1 )
			return 0;

		assert( pData );
		assert( sizeof(BUFFER_TYPE) == 1 );//Make sure its a byte type.

		//Ensure buffer is bigenough.
		EnsureBufferSize(write_pos + pNum_to_write - 1,true);

		//Now write to the buffer at 'write_pos'
		//We are emulating file writes so don't use assingment operator of buffer type, should be uint8_t anyways.
		memcpy(data + write_pos,pData,pNum_to_write);

		//Advance file sudo pointer.
		write_pos += pNum_to_write;
		return (int)pNum_to_write;
	}

	/*!
		Returns the pos of the virtual file pointer.
	*/
	int GetPos(){return (int)write_pos;}

	/*!
		Changes the write pos of the data.
		Does not change the size of the buffer if the seek passes the end.
		If the pos is past the end then the next write will expand the buffer.
	*/
	int Seek(int pByte_offset,bool pFrom_current_pos = false)
	{
		if( pFrom_current_pos )
			write_pos += pByte_offset;
		else
			write_pos = pByte_offset;
		return (int)write_pos;
	}

	/*!
		Seeks to an absolute location, writes the passed data then seeks back to the orginal location, only if the current pos is less that what we started at.
		This func is handy for rewriting a file header at the start of the file.
		@param pByte_offset How many bytes to move from the start of the file.
		@param pData The object or array to write.
		@param pArray_size The number of elements to the array.
		@return int The number of bytes written. If the file pos before we call is > than after the call then the file pos is reset.
	*/
	template <class TYPE>int SeekWrite(int pByte_offset,const TYPE &pData,size_t pArray_size = 1)
	{
		int pos = GetPos();
		Seek(pByte_offset);
		int written = Write(pData,pArray_size);
		if( pos > GetPos() )
		{
			Seek(pos);
		}
		return written;
	}

	/*!
		Aligns the virtual "file pos" on a byte, word etc boundory. This is needed for files read by systems that need correct type alignment.
		@param pAlignment The boundy to align the file pos on, the file pos may move from zero to pAlignment-1 bytes to achive the required alignment.
	*/
	void AlignFilePos(int pAlignment)
	{
		assert( ((pAlignment-1)&pAlignment) == 0 );//Must be power of 2
		pAlignment--;//So its now the bit pattern of the disallowed bits.

		int pos = GetPos();
		int new_pos = (pos+pAlignment)&(~pAlignment);

		//I just can't seek to the aligned pos as if the caller seeks backwards before writing anything then the alignment is lost.
		//So I need to write the correct number of bytes to align the file pos.
		if( pos != new_pos )
		{
			assert( pos < new_pos );
			assert( pos != new_pos );
			assert( (new_pos-pos) > 0 );
			assert( (new_pos-pos) <= pAlignment );
			assert( (new_pos&pAlignment) == 0 );
			assert( (pos&pAlignment) != 0 );

			uint8_t padding = 0;
			Write(padding,new_pos - pos);

			assert( (write_pos&pAlignment) == 0 );
			assert(write_pos == new_pos );
		}
	}

	/*!
		Returns true if the memory item uses is managed by this buffer.
	*/
	bool IsMember(const BUFFER_TYPE* pItem)
	{
		return GetIndex(pItem) != 0xffffffff;
	}
	bool IsMember(const BUFFER_TYPE& pItem)
	{
		return GetIndex(&pItem) != 0xffffffff;
	}


private:

	size_t buffer_size;	//!<The buffer is enlarged by an extra amout to stop thrashing and so is always bigger than size.
	size_t size;		//!<The size of the array that is its == to the largest index used, which maybe less than buffer size
	size_t write_pos;	//!<This is used when we are using the code as a buffer for writing to files.
	BUFFER_TYPE *data;	//!<The data of the array.

	/*!
		Reallocates the memory buffer maintaining the contents that is already there.
		@param pNew_size The new size of the array, asserts if the buffer is already bigger that this. Use 'SetSize' instead.
		@param pCopy_old_data If true the code will copy the old buffer to the new one.
	*/
	void Realloc(size_t pNew_size,bool pCopy_old_data)
	{
		assert( pNew_size > buffer_size );
		BUFFER_TYPE *newb = new BUFFER_TYPE[pNew_size];
		if( data )
		{
			//Have to copy using the copy operator. Using a memory copy is bad as it may copy pointers that are not copyable!
			//We copy upto size and not buffer_size as buffer_size maybe > than size and so some of the
			//items at the end may not be setup and cause a crash.
			if( pCopy_old_data )
			{
				for( size_t n = 0 ; n < size ; n++ )
				{
					newb[n] = data[n];
				}
			}
			delete []data;
		}
		buffer_size = pNew_size;
		data = newb;
	}

	/*!
		Frees the memory buffer and resets the internal counters.
	*/
	void FreeBuffer()
	{
		delete []data;
		data =0L;
		buffer_size = 0;
		size = 0;
	}

	/*!
		Makes sure that the buffer is big enough so that the passed index is in the buffer.
		@param pIndex This is the index we want to ensure that the array is big enough to hold.
		@param pCopy_old_data If true the code will copy the old buffer to the new one.
	*/
	void EnsureBufferSize(size_t pIndex,bool pCopy_old_data)
	{
		assert( pIndex < 0x0fffffff );//This way big enough, this traps daft indices causing memory allocation failure which tends to bomb the debugger.
		if( pIndex >= buffer_size )
		{
			//Use index + over allocation size as the new size.
			//Doing this does mean that the real size of the buffer will not me multiples of 'over allocation size' but does
			//mean I don't have to work out how many 'over allocation size' increments I need to do.
			size_t new_size = pIndex + 1 + pOver_allocate;
			if( new_size < pStart_size )
			{
				new_size = pStart_size;
			}
			Realloc(new_size,pCopy_old_data);
		}

		//Make sure that the 'sudo' size is at least the size of the index would intermate.
		if( size < (pIndex+1) )//zero based array, so index of 3 means 4 items.
		{
			size = (pIndex+1);
		}
	}
};

} /* namespace BogDog */
#endif /* __DYNAMICBUFFER_H__ */
