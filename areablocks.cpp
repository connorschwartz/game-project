#include "areablocks.h"

AreaBlocks::AreaBlocks(int width, int height) {
	xBlocks = width;
	yBlocks = height;
	int numBytes = (width * height + 7) / 8;
	bitmap = new unsigned char[numBytes];
	for (int i = 0; i < numBytes; i++) {
		bitmap[i] = 0;
	}
}

AreaBlocks::~AreaBlocks() {

}

AreaBlocks::BlockMapping AreaBlocks::getBlockMapping(int x, int y) {
	BlockMapping mapping;
	mapping.byteNum = (y * xBlocks + x) / 8;
	unsigned char bitNum = (y * xBlocks + x) % 8;
	mapping.mask = 1 << bitNum;
	return mapping;
}

void AreaBlocks::block(int x, int y) {
	BlockMapping mapping = getBlockMapping(x, y);
	bitmap[mapping.byteNum] = bitmap[mapping.byteNum] | mapping.mask;
}

void AreaBlocks::blockRectangle(int x, int y, int width, int height) {
	for (int i = x; i > x - width; i--) {
		for (int j = y; j > y - height; j--) {
			block(i, j);
		}
	}
}

bool AreaBlocks::isFree(int x, int y) {

	if (x < 0 || y < 0 || x >= xBlocks || y >= yBlocks) {
		return false;
	}
	BlockMapping mapping = getBlockMapping(x, y);
	if ((int) (bitmap[mapping.byteNum] & mapping.mask) > 0) {
		return false;
	}
	return true;
}


