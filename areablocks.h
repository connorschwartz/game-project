#ifndef AreaBlocks_H
#define AreaBlocks_H

class AreaBlocks {
public:
	AreaBlocks(int width, int height);
	~AreaBlocks();

	bool isFree(int x, int y);

private:
	int xBlocks;
	int yBlocks;
};

#endif


