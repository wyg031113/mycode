#include "memman.h"
#include <memory.h>

MemBlock::MemBlock(size_t size)
{
	ASSERT(ptr = (byte*)malloc(size));
	offset = 0;
	len = 0;
	total_len = size;
}
MemBlock::MemBlock(const byte *b, size_t length)
{
	ASSERT(ptr = (byte*)malloc(length));
	offset = 0;
	total_len = len = length;
	memcpy(ptr, b, length);
}
MemBlock::~MemBlock()
{
	free(ptr);
	ptr = NULl;
	offset = 0;
	len = 0;
	total_len = 0;
}
MemBlock *copyof()
{
	return new MemBlock(ptr+offset, len);
}


Buffer::Buffer():len(0), mblist()
{
}
Buffer::~Buffer()
{
	list<MemBlock*>::iterator ite;
	for(ite = mblist.begin(); ite != mblist.end(); ite++)
		delete *ite;	
}
