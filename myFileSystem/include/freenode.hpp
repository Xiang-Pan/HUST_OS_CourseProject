#ifndef _FREENODE_H_
#define _FREENODE_H_

#include <list>
#include <sys/types.h>

class FreeNode 
{
	public:
	uint block_num;		//free node num
	uint pos;			//start pos
	FreeNode(uint block_num, uint pos): block_num(block_num),pos(pos) {}
};

#endif /* _FREENODE_H_ */
