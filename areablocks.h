#ifndef AreaBlocks_H
#define AreaBlocks_H

class AreaBlocks {
public:
	// Structure containing the index number of the byte within the map and the mask to retrieve the desired bit
	struct BlockMapping {
		int byteNum;
		unsigned char mask;
	};

	AreaBlocks(int width, int height);
	~AreaBlocks();

	void markUsed(int x, int y);									// Mark a single block as used
	void markUsedRectangle(int x, int y, int width, int height);	// Mark a rectangular group of blocks as used

	void markFree(int x, int y);									// Mark a single block as free
	void markFreeRectangle(int x, int y, int width, int height);	// Mark a rectangular group of blocks as free

	// Convert an (x, y) pair to a byte number and mask to find the corresponding bit
	BlockMapping getBlockMapping(int x, int y);

	// Return true if the block at (x, y) is free, false otherwise
	bool isFree(int x, int y);

private:
	unsigned char * bitmap;	// The map

	int areaWidth;			// Width of the area
	int areaHeight;			// Height of the area
};

#endif


