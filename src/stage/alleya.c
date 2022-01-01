/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "alleya.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
#include "../random.h"
#include "../timer.h"
#include "../animation.h"

//Week 4 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	IO_Data arc_bleft, arc_bleft_ptr[2];
	IO_Data arc_bright, arc_bright_ptr[3];
	
	Gfx_Tex tex_back0; //Alley left
	Gfx_Tex tex_back1; //Alley right
	
	
	//bleft state
	Gfx_Tex tex_bleft;
	u8 bleft_frame, bleft_tex_id;
	
	Animatable bleft_animatable;

	//bright state
	Gfx_Tex tex_bright;
	u8 bright_frame, bright_tex_id;

	Animatable bright_animatable;
} Back_Alleya;

//bleft animation and rects
static const CharFrame bleft_frame[3] = {
	{0, {  0,  0,  113,  139}, {71, 98}}, //Idle0
	{0, {113,  0,  110,  141}, {69, 100}}, //Idle1
	{1, {  0,  0,  111,  141}, {70, 100}}, //Idle2
};

static const Animation bleft_anim[1] = {
    {2, (const u8[]){0, 1, 2, ASCR_BACK, 1}},
};

//bright animation and rects
static const CharFrame bright_frame[3] = {
	{0, {  0,  0,  140,  131}, {71, 98}}, //Idle0
	{1, {  0,  0,  142,  134}, {71, 101}}, //Idle1
	{2, {  0,  0,  141,  133}, {71, 101}}, //Idle2
};

static const Animation bright_anim[1] = {
	{2, (const u8[]){0, 1, 2, ASCR_BACK, 1}},
};

//Bleft functions
void Alleya_Bleft_SetFrame(void *user, u8 frame)
{
	Back_Alleya *this = (Back_Alleya*)user;
	
	//Check if this is a new frame
	if (frame != this->bleft_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &bleft_frame[this->bleft_frame = frame];
		if (cframe->tex != this->bleft_tex_id)
			Gfx_LoadTex(&this->tex_bleft, this->arc_bleft_ptr[this->bleft_tex_id = cframe->tex], 0);
	}
}

void Alleya_Bleft_Draw(Back_Alleya *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &bleft_frame[this->bleft_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_bleft, &src, &dst, stage.camera.bzoom);
}

//BRight functions
void Alleya_Bright_SetFrame(void *user, u8 frame)
{
	Back_Alleya *this = (Back_Alleya*)user;
	
	//Check if this is a new frame
	if (frame != this->bright_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &bright_frame[this->bright_frame = frame];
		if (cframe->tex != this->bright_tex_id)
			Gfx_LoadTex(&this->tex_bright, this->arc_bright_ptr[this->bright_tex_id = cframe->tex], 0);
	}
}

void Alleya_Bright_Draw(Back_Alleya *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &bright_frame[this->bright_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_bright, &src, &dst, stage.camera.bzoom);
}


void Back_Alleya_DrawFG(StageBack *back)
{
	Back_Alleya *this = (Back_Alleya*)back;
	
	fixed_t fx, fy;
	
	
}

void Back_Alleya_DrawBG(StageBack *back)
{
	Back_Alleya *this = (Back_Alleya*)back;
	

	
	fixed_t fx, fy;

	//Animate and draw bleft
	fx = stage.camera.x;
	fy = stage.camera.y;
	if (stage.stage_id == StageId_1_1)
	    {
	        if (stage.flag & STAGE_FLAG_JUST_STEP)
	        {
	        	switch (stage.song_step & 7)
	        	{
	        		case 0:
	        			Animatable_SetAnim(&this->bleft_animatable, 0);
	        			break;
	        		case 4:
	        			Animatable_SetAnim(&this->bleft_animatable, 0);
	        			break;
	        	}
	        }
	    }
	else
	    {
	        if (stage.flag & STAGE_FLAG_JUST_STEP)
	        {
	        	switch (stage.song_step & 7)
	        	{
	        		case 0:
	        			Animatable_SetAnim(&this->bleft_animatable, 0);
	        			break;
	        		case 4:
	        			break;
	        	}
	        }
	    }
	Animatable_Animate(&this->bleft_animatable, (void*)this, Alleya_Bleft_SetFrame);

	//Animate and draw right
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.stage_id == StageId_1_1)
	    {
	        if (stage.flag & STAGE_FLAG_JUST_STEP)
	        {
	        	switch (stage.song_step & 7)
	        	{
	        		case 0:
	        			Animatable_SetAnim(&this->bright_animatable, 0);
	        			break;
	        		case 4:
	        			Animatable_SetAnim(&this->bright_animatable, 0);
	        			break;
	        	}
	        }
	    }
	else
	    {
	        if (stage.flag & STAGE_FLAG_JUST_STEP)
	        {
	        	switch (stage.song_step & 7)
	        	{
	        		case 0:
	        			Animatable_SetAnim(&this->bright_animatable, 0);
	        			break;
	        		case 4:
	        			break;
	        	}
	        }
	    }
	Animatable_Animate(&this->bright_animatable, (void*)this, Alleya_Bright_SetFrame);

	Alleya_Bleft_Draw(this, FIXED_DEC(-30,1) - fx, FIXED_DEC(30,1) - fy);
	Alleya_Bright_Draw(this, FIXED_DEC(290,1) - fx, FIXED_DEC(30,1) - fy);
	
	
	//Draw sunset
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT alleyl_src = {0, 0, 256, 256};
	RECT_FIXED alleyl_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(340 + SCREEN_WIDEOADD,1),
		FIXED_DEC(260,1)
	};

	RECT alleyr_src = {0, 0, 256, 256};
	RECT_FIXED alleyr_dst = {
		FIXED_DEC(173 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(340 + SCREEN_WIDEOADD,1),
		FIXED_DEC(260,1)
	};
	
	Stage_DrawTex(&this->tex_back0, &alleyl_src, &alleyl_dst, stage.camera.bzoom);
	Stage_DrawTex(&this->tex_back1, &alleyr_src, &alleyr_dst, stage.camera.bzoom);
}

void Back_Alleya_Free(StageBack *back)
{
	Back_Alleya *this = (Back_Alleya*)back;
	
	//Free henchmen archive
	Mem_Free(this->arc_bleft);
	Mem_Free(this->arc_bright);
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Alleya_New(void)
{
	//Allocate background structure
	Back_Alleya *this = (Back_Alleya*)Mem_Alloc(sizeof(Back_Alleya));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = Back_Alleya_DrawFG;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Alleya_DrawBG;
	this->back.free = Back_Alleya_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\ALLEY\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Mem_Free(arc_back);
	
	//Load bleft textures
	this->arc_bleft = IO_Read("\\ALLEY\\BLEFT.ARC;1");
	this->arc_bleft_ptr[0] = Archive_Find(this->arc_bleft, "bleft0.tim");
	this->arc_bleft_ptr[1] = Archive_Find(this->arc_bleft, "bleft1.tim");

	//Load bright textures
	this->arc_bright = IO_Read("\\ALLEY\\BRIGHT.ARC;1");
	this->arc_bright_ptr[0] = Archive_Find(this->arc_bright, "bright0.tim");
	this->arc_bright_ptr[1] = Archive_Find(this->arc_bright, "bright1.tim");
	this->arc_bright_ptr[2] = Archive_Find(this->arc_bright, "bright2.tim");
	
	
	//Initialize henchmen state
	Animatable_Init(&this->bleft_animatable, bleft_anim);
	Animatable_SetAnim(&this->bleft_animatable, 0);
	this->bleft_frame = this->bleft_tex_id = 0xFF; //Force art load

	Animatable_Init(&this->bright_animatable, bright_anim);
	Animatable_SetAnim(&this->bright_animatable, 0);
	this->bright_frame = this->bright_tex_id = 0xFF; //Force art load
	
	return (StageBack*)this;
}
