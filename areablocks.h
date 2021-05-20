#ifndef AreaBlocks_H
#define AreaBlocks_H

class AreaBlocks {
public:
	struct BlockMapping {
		int byteNum;
		unsigned char mask;
	};

	AreaBlocks(int width, int height);
	~AreaBlocks();

	void block(int x, int y);
	void blockRectangle(int x, int y, int width, int height);

	BlockMapping getBlockMapping(int x, int y);

	bool isFree(int x, int y);

private:
	unsigned char * bitmap;

	int xBlocks;
	int yBlocks;
};

#endif


