#ifndef Person_H
#define Person_H

#include "visibleobject.h"
#include "areablocks.h"

class Person : public VisibleObject {
public:

	// Stages of the person's gait when walking
	enum GAIT {
		STANDING,
		LEFT_FOOT,
		RIGHT_FOOT
	};

	// Direction the person is facing
	enum DIRECTION {
		DOWN,
		RIGHT,
		LEFT,
		UP
	};

	Person(AreaBlocks* blocks, int x, int y, int width, int height);
	virtual ~Person() = 0;

	void setDirection();

	void move();

	// Timeskip to keep the person still during dialog or menu-related pauses
	virtual void timeSkip(int time) = 0;

	// Accessors
	int getSpriteDirection();
	void setSpriteDirection(int direction);

protected:

	AreaBlocks* areaBlocks;			// Used to track which blocks can be moved to
	double xVel;			// x direction velocity
	double yVel;			// y direction velocity
	bool moving;			// True if in the process of moving, false otherwise
	int lastMove;			// Time when the object was last moved on the screen
	int lastStep;			// Indicates the type of step the previous move was (i.e. left foot vs right foot)
	int spriteDirection;	// Direction the person is moving (from DIRECTION enum)
	int spriteGait;			// y-coordinate of the person's current sprite on the sheet (from GAIT enum)
	bool wantsToMove;		// True if the person wants to move, false otherwise
	int desiredDirection;	// Direction in which the person wants to move (from DIRECTION enum)
	int desiredSpeed;		// Speed at which the person wants to move
};

#endif
