/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "hell.h"

#include "../mem.h"

//Hell background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
} Back_Hell;

//Hell background functions
void Back_Hell_Free(StageBack *back)
{
	Back_Hell *this = (Back_Hell*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Hell_New(void)
{
	//Allocate background structure
	Back_Hell *this = (Back_Hell*)Mem_Alloc(sizeof(Back_Hell));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = NULL;
	this->back.free = Back_Hell_Free;
	
	//Use non-pitch black background
	Gfx_SetClear(253, 253, 253);
	
	return (StageBack*)this;
}
