#include "areablocks.h"

AreaBlocks::AreaBlocks(int width, int height) {
	xBlocks = width;
	yBlocks = height;
}

AreaBlocks::~AreaBlocks() {

}

bool AreaBlocks::isFree(int x, int y) {
	if (x < 0 || y < 0 || x >= xBlocks || y >= yBlocks) {
		return false;
	}
	return true;
}


