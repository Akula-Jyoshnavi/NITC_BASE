#include "StaticBuffer.h"
#include<bits/stdc++.h>


unsigned char StaticBuffer::blocks[BUFFER_CAPACITY][BLOCK_SIZE];
unsigned char StaticBuffer::blockAllocMap[DISK_BLOCKS];
struct BufferMetaInfo StaticBuffer::metainfo[BUFFER_CAPACITY];

StaticBuffer::StaticBuffer() {

  for (int bufferIndex = 0;bufferIndex<BUFFER_CAPACITY;bufferIndex++) {
    metainfo[bufferIndex].free = true;
    metainfo[bufferIndex].dirty = false;
    metainfo[bufferIndex].timeStamp = -1;
    metainfo[bufferIndex].blockNum = -1;
  }
  
  //setting up block Allocation map
  for (int i = 0; i < BLOCK_ALLOCATION_MAP_SIZE; i++) {
		Disk::readBlock(blockAllocMap + i * BLOCK_SIZE, i);
	}
  
}

StaticBuffer::~StaticBuffer() {
   //copying bmap 
  for (int i = 0; i < BLOCK_ALLOCATION_MAP_SIZE; i++) {
		Disk::writeBlock(blockAllocMap + i * BLOCK_SIZE, i);
	}
  
   for (int bufferIndex = 0;bufferIndex<BUFFER_CAPACITY;bufferIndex++)
   {
     if(metainfo[bufferIndex].free == false&&metainfo[bufferIndex].dirty == true)
     {
       Disk::writeBlock(blocks[bufferIndex],metainfo[bufferIndex].blockNum);
     }
   }
}

int StaticBuffer::getFreeBuffer(int blockNum) {
  if (blockNum < 0 || blockNum > DISK_BLOCKS) {
    return E_OUTOFBOUND;
  }
  int free_buffer=-1,hightime_buffer=0;
  for (int bufferIndex = 0;bufferIndex<BUFFER_CAPACITY;bufferIndex++)
  {
    if(metainfo[bufferIndex].free==false)
        metainfo[bufferIndex].timeStamp++;
    else free_buffer=bufferIndex;
    
    if(metainfo[hightime_buffer].timeStamp<metainfo[bufferIndex].timeStamp)
        hightime_buffer=bufferIndex;
       
  }
  int allocatedBuffer=free_buffer;
  if(free_buffer==-1)
  {
    if(metainfo[hightime_buffer].dirty==true)
    {
      Disk::writeBlock(blocks[hightime_buffer],metainfo[hightime_buffer].blockNum);
    }
    allocatedBuffer=hightime_buffer;
  }
 
    metainfo[allocatedBuffer].free = false;
    metainfo[allocatedBuffer].dirty = false;
    metainfo[allocatedBuffer].timeStamp = 0;
    metainfo[allocatedBuffer].blockNum = blockNum;  


  return allocatedBuffer;
}


int StaticBuffer::getBufferNum(int blockNum) {
  if(blockNum<0||blockNum>DISK_BLOCKS) return E_OUTOFBOUND;
  int foundblock=-1;
  for (int bufferIndex = 0;bufferIndex<BUFFER_CAPACITY;bufferIndex++) {
    if(metainfo[bufferIndex].blockNum == blockNum) 
     {
       foundblock=bufferIndex;
       break; 
     }
  }
  if(foundblock==-1)
     return E_BLOCKNOTINBUFFER;
  else
     return foundblock;
}

int StaticBuffer::setDirtyBit(int blockNum){
    
    int bufferNum=getBufferNum(blockNum);
    if(bufferNum == E_BLOCKNOTINBUFFER||bufferNum == E_OUTOFBOUND) return bufferNum;
    
    metainfo[bufferNum].dirty = true;
     return SUCCESS;
}

int StaticBuffer::getStaticBlockType(int blockNum){
	if (blockNum >= DISK_BLOCKS || blockNum < 0) {
		return E_OUTOFBOUND;
	}

	return (int) blockAllocMap[blockNum];
}
