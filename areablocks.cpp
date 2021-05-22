#include "areablocks.h"
#include <iostream>

AreaBlocks::AreaBlocks(int width, int height) {
	areaWidth = width;
	areaHeight = height;
	// Create an array of bytes, with one bit per block. Bit is 0 if the block is free, 1 if in use
	int numBytes = (width * height + 7) / 8;
	bitmap = new unsigned char[numBytes];
	// Initialize to all free
	for (int i = 0; i < numBytes; i++) {
		bitmap[i] = 0;
	}
}

AreaBlocks::~AreaBlocks() {

}

AreaBlocks::BlockMapping AreaBlocks::getBlockMapping(int x, int y) {
	// Convert (x, y) block coordinate to a byte number within the array and a bit within this byte
	BlockMapping mapping;
	mapping.byteNum = (y * areaWidth + x) / 8;
	unsigned char bitNum = (y * areaWidth + x) % 8;
	mapping.mask = 1 << bitNum;
	return mapping;
}

void AreaBlocks::markUsed(int x, int y) {
	// Mark block (x, y) as used
	BlockMapping mapping = getBlockMapping(x, y);
	bitmap[mapping.byteNum] = bitmap[mapping.byteNum] | mapping.mask;
}

void AreaBlocks::markUsedRectangle(int x, int y, int width, int height) {
	// Mark a (width by height) rectangle starting at (x, y) as used
	for (int i = x; i > x - width; i--) {
		for (int j = y; j > y - height; j--) {
			markUsed(i, j);
		}
	}
}

void AreaBlocks::markFree(int x, int y) {
	// Mark block (x, y) as used
	BlockMapping mapping = getBlockMapping(x, y);
	bitmap[mapping.byteNum] = bitmap[mapping.byteNum] & ~mapping.mask;
}

bool AreaBlocks::isFree(int x, int y) {
//	for (int i = 0; i < 64; i++) {
//		for (int j = 0; j < 4; j++) {
//			std::cout << (int) bitmap[i * 4 + j] << " ";
//		}
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
	// Block is not free if it's out of bounds
	if (x < 0 || y < 0 || x >= areaWidth || y >= areaHeight) {
		return false;
	}
	// Block is not free if its bit is 1
	BlockMapping mapping = getBlockMapping(x, y);
	if ((int) (bitmap[mapping.byteNum] & mapping.mask) > 0) {
		return false;
	}
	return true;
}


