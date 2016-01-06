#ifndef __MEMMAN_H__
#define __MEMMAN_H__
#include <iostream>
#include <list.h>
#include "inc.h"
typedef unsigned char byte;
typedef size_t uint;
class Buffer;
class MemBlock
{
public:
	friend Buffer;
	MemBlock(size_t size);
	MemBlock(const byte *b, size_t length);
	~MemBlock();
	MemBlock *copyof();
private:
	byte *ptr;
	size_t offset;
	size_t len;
	size_t total_len;
};
class Buffer
{
public:
	Buffer();
	~Buffer();
	void append(Buffer *b);
	void append(MemBlock *mb);
	void append(const byte *b, size_t len);
	void remove(size_t start, size_t len);
	void remove(size_t len); /*start == 0 */
	byte *tobuffer();
	byte &operator[](uint i);
	Buffer *copyof();
	size_t get_len();
private:
	size_t len;
	list<MemBlock*> mblist;	
};

#endif /*__MEMMAN_H__*/
