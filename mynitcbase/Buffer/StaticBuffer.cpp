#include "StaticBuffer.h"
#include<string.h>

// the declarations for this class can be found at "StaticBuffer.h"

unsigned char StaticBuffer::blocks[BUFFER_CAPACITY][BLOCK_SIZE];
struct BufferMetaInfo StaticBuffer::metainfo[BUFFER_CAPACITY];
unsigned char StaticBuffer::blockAllocMap[DISK_BLOCKS];

StaticBuffer::StaticBuffer() {

  unsigned char buffer[BLOCK_SIZE]; // BLOCK_SIZE is typically 512

  for (int blockNum = 0; blockNum <= 3; blockNum++) {
      // Read the physical block from disk into the buffer
      Disk::readBlock(buffer, blockNum);
      memcpy(blockAllocMap+blockNum*BLOCK_SIZE,buffer,BLOCK_SIZE);
  }

  // initialise all blocks as free
  for (int i=0;i<BUFFER_CAPACITY;i++) {
    metainfo[i].free = true;
    metainfo[i].dirty=false;
    metainfo[i].blockNum=-1;
    metainfo[i].timeStamp=-1;
  }
}

/*
At this stage, we are not writing back from the buffer to the disk since we are
not modifying the buffer. So, we will define an empty destructor for now. In
subsequent stages, we will implement the write-back functionality here.
*/
StaticBuffer::~StaticBuffer() {
  unsigned char bufferPtr[BLOCK_SIZE];
  for (int blockNum = 0; blockNum <= 3; blockNum++) {
      memcpy(bufferPtr,blockAllocMap+blockNum*BLOCK_SIZE,BLOCK_SIZE);
      // Read the physical block from disk into the buffer
      Disk::writeBlock(bufferPtr,blockNum);
  }

  unsigned char *bufferptr;
   for(int bufferIndex=0;bufferIndex<BUFFER_CAPACITY;bufferIndex++)
   {
    if(metainfo[bufferIndex].free==false && metainfo[bufferIndex].dirty==true)
    {
      bufferptr=blocks[bufferIndex];
      Disk::writeBlock(bufferptr,metainfo[bufferIndex].blockNum);
    }
   }
}

/* Get the buffer index where a particular block is stored
   or E_BLOCKNOTINBUFFER otherwise
*/
int StaticBuffer::getBufferNum(int blockNum) {
  // Check if blockNum is valid (between zero and DISK_BLOCKS)
  if(blockNum<0 || blockNum>=DISK_BLOCKS)
  {
    // and return E_OUTOFBOUND if not valid.
    return E_OUTOFBOUND;
  }
  
  // find and return the bufferIndex which corresponds to blockNum (check metainfo)
  for(int i=0;i<BUFFER_CAPACITY;i++)
  {
    if(metainfo[i].free==false && metainfo[i].blockNum==blockNum)
    {
        return i;
    }
  }
  // if block is not in the buffer
  return E_BLOCKNOTINBUFFER;
}

int StaticBuffer::getFreeBuffer(int blockNum){
    // Check if blockNum is valid (non zero and less than DISK_BLOCKS)
    // and return E_OUTOFBOUND if not valid.
    if(blockNum <0 || blockNum>=DISK_BLOCKS)
      return E_OUTOFBOUND;

    // increase the timeStamp in metaInfo of all occupied buffers.
    for(int i=0;i<BUFFER_CAPACITY;i++)
    {
      if(!metainfo[i].free)
      {
        metainfo[i].timeStamp++;
      }
    }

    // let bufferNum be used to store the buffer number of the free/freed buffer.
    int bufferNum=-1;

    // iterate through metainfo and check if there is any buffer free
    for(int i=0;i<BUFFER_CAPACITY;i++)
    {
      if(metainfo[i].free)
      {
        bufferNum = i;
        break;
      }
    }

    // if a free buffer is available, set bufferNum = index of that free buffer.

    // if a free buffer is not available,
    //     find the buffer with the largest timestamp
    //     IF IT IS DIRTY, write back to the disk using Disk::writeBlock()
    //     set bufferNum = index of this buffer
    if(bufferNum==-1)
    {
      int maxTimeStamp = -1;
        for (int i = 0; i < BUFFER_CAPACITY; i++) {
          if (metainfo[i].timeStamp > maxTimeStamp) {
              maxTimeStamp = metainfo[i].timeStamp;
              bufferNum = i;
          }
      }
      if (metainfo[bufferNum].dirty) {
          Disk::writeBlock(blocks[bufferNum], metainfo[bufferNum].blockNum);
      }
    }

    // update the metaInfo entry corresponding to bufferNum with
    // free:false, dirty:false, blockNum:the input block number, timeStamp:0.
    metainfo[bufferNum].free=false;
    metainfo[bufferNum].dirty=false;
    metainfo[bufferNum].blockNum=blockNum;
    metainfo[bufferNum].timeStamp=0;
    

    // return the bufferNum.
    return bufferNum;
}

int StaticBuffer::setDirtyBit(int blockNum){
    // find the buffer index corresponding to the block using getBufferNum().
    int bufIndex = getBufferNum(blockNum);

    // if block is not present in the buffer (bufferNum = E_BLOCKNOTINBUFFER)
    //     return E_BLOCKNOTINBUFFER
    if(bufIndex == E_BLOCKNOTINBUFFER)
    {
      return E_BLOCKNOTINBUFFER;
    }

    // if blockNum is out of bound (bufferNum = E_OUTOFBOUND)
    //     return E_OUTOFBOUND
    if(bufIndex == E_OUTOFBOUND) return E_OUTOFBOUND;

    // else
    //     (the bufferNum is valid)
    //     set the dirty bit of that buffer to true in metainfo
    else
    {
      metainfo[bufIndex].dirty=true;
    }

    // return SUCCESS
    return SUCCESS;
}

int StaticBuffer::getStaticBlockType(int blockNum){
    // Check if blockNum is valid (non zero and less than number of disk blocks)
    // and return E_OUTOFBOUND if not valid.
    if(blockNum<0 || blockNum>=DISK_BLOCKS) return E_OUTOFBOUND;

    // Access the entry in block allocation map corresponding to the blockNum argument
    // and return the block type after type casting to integer.
    unsigned char blockType = (unsigned char)StaticBuffer::blockAllocMap[blockNum];
    return (int)blockType;
}