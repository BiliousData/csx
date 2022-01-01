/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

//OH BOY THIS WILL BE LOADS OF FUN!

#include "cgm.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//CGM character structure
enum
{
	CGM_ArcMain_Idle0,
	CGM_ArcMain_Idle1,
	CGM_ArcMain_Idle2,
	CGM_ArcMain_Idle3,
	CGM_ArcMain_Idle4,
	CGM_ArcMain_Idle5,
	CGM_ArcMain_Idle6,
	CGM_ArcMain_Idle7,
	CGM_ArcMain_Idle8,
	CGM_ArcMain_Idle9,
	CGM_ArcMain_Idle10,
	CGM_ArcMain_Idle11,
	CGM_ArcMain_Idle12,
	CGM_ArcMain_Idle13,
	CGM_ArcMain_Idle14,
	CGM_ArcMain_Idle15,

	CGM_ArcMain_Free0,
    CGM_ArcMain_Free1,
    CGM_ArcMain_Free2,
    CGM_ArcMain_Free3,
    CGM_ArcMain_Free4,
    CGM_ArcMain_Free5,
    CGM_ArcMain_Free6,
    CGM_ArcMain_Free7,
    CGM_ArcMain_Free8,
    CGM_ArcMain_Free9,
    CGM_ArcMain_Free10,
    CGM_ArcMain_Free11,
	
	CGM_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[CGM_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_CGM;

//CGM character definitions
static const CharFrame char_cgm_frame[] = {
	{CGM_ArcMain_Idle0,  {  0, 0, 166, 233}, { 42, 183}}, //0 idle 
	{CGM_ArcMain_Idle1,  {  0, 0, 167, 233}, { 43, 183}}, //1 idle 
	{CGM_ArcMain_Idle2,  {  0, 0, 167, 233}, { 43, 183}}, //2 idle 
	{CGM_ArcMain_Idle3,  {  0, 0, 167, 234}, { 42, 183}}, //3 idle 
	{CGM_ArcMain_Idle4,  {  0, 0, 166, 233}, { 42, 183}}, //4 idle 
	{CGM_ArcMain_Idle5,  {  0, 0, 167, 233}, { 43, 183}}, //5 idle 
	{CGM_ArcMain_Idle6,  {  0, 0, 166, 233}, { 42, 183}}, //6 idle 
	{CGM_ArcMain_Idle7,  {  0, 0, 167, 233}, { 43, 183}}, //7 idle 
	{CGM_ArcMain_Idle8,  {  0, 0, 167, 233}, { 43, 183}}, //8 idle 
	{CGM_ArcMain_Idle9,  {  0, 0, 167, 233}, { 42, 183}}, //9 idle 
	{CGM_ArcMain_Idle10, {  0, 0, 166, 233}, { 42, 183}}, //10 idle 
	{CGM_ArcMain_Idle11, {  0, 0, 167, 233}, { 43, 183}}, //11 idle 
	{CGM_ArcMain_Idle12, {  0, 0, 166, 233}, { 42, 183}}, //12 idle 
	{CGM_ArcMain_Idle13, {  0, 0, 167, 233}, { 43, 183}}, //13 idle 
	{CGM_ArcMain_Idle14, {  0, 0, 167, 233}, { 42, 183}}, //14 idle 
	{CGM_ArcMain_Idle15, {  0, 0, 167, 233}, { 42, 183}}, //15 idle 

	{CGM_ArcMain_Free0,   {  0, 0, 166, 238}, { 42, 188}},//16 freeplay
	{CGM_ArcMain_Free1,   {  0, 0, 167, 222}, { 43, 172}},//17 freeplay
	{CGM_ArcMain_Free2,   {  0, 0, 166, 222}, { 42, 172}},//18 freeplay
	{CGM_ArcMain_Free3,   {  0, 0, 167, 222}, { 43, 172}},//19 freeplay
	{CGM_ArcMain_Free4,   {  0, 0, 167, 222}, { 43, 172}},//20 freeplay
	{CGM_ArcMain_Free5,   {  0, 0, 167, 222}, { 42, 172}},//21 freeplay
	{CGM_ArcMain_Free6,   {  0, 0, 166, 222}, { 42, 172}},//22 freeplay
	{CGM_ArcMain_Free7,   {  0, 0, 167, 222}, { 43, 172}},//23 freeplay
	{CGM_ArcMain_Free8,   {  0, 0, 166, 222}, { 42, 172}},//24 freeplay
	{CGM_ArcMain_Free9,   {  0, 0, 167, 222}, { 43, 172}},//25 freeplay
	{CGM_ArcMain_Free10,  {  0, 0, 167, 222}, { 42, 172}},//26 freeplay
	{CGM_ArcMain_Free11,  {  0, 0, 167, 222}, { 42, 172}},//27 freeplay
};

static const Animation char_cgm_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, ASCR_BACK, 15}}, //CharAnim_Idle
	{2, (const u8[]){ 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, ASCR_BACK, 7}},   //CharAnim_Free
	{2, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_Sets
};

//CGM character functions
void Char_CGM_SetFrame(void *user, u8 frame)
{
	Char_CGM *this = (Char_CGM*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_cgm_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_CGM_Tick(Character *character)
{
	Char_CGM *this = (Char_CGM*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_CGM_SetFrame);
	Character_Draw(character, &this->tex, &char_cgm_frame[this->frame]);
}

void Char_CGM_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_CGM_Free(Character *character)
{
	Char_CGM *this = (Char_CGM*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_CGM_New(fixed_t x, fixed_t y)
{
	//Allocate cgm object
	Char_CGM *this = Mem_Alloc(sizeof(Char_CGM));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_CGM_New] Failed to allocate cgm object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_CGM_Tick;
	this->character.set_anim = Char_CGM_SetAnim;
	this->character.free = Char_CGM_Free;
	
	Animatable_Init(&this->character.animatable, char_cgm_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\CGM.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",
        "idle1.tim",
        "idle2.tim",
        "idle3.tim",
        "idle4.tim",
        "idle5.tim",
        "idle6.tim",
        "idle7.tim",
        "idle8.tim",
        "idle9.tim",
        "idle10.tim",
        "idle11.tim",
        "idle12.tim",
        "idle13.tim",
        "idle14.tim",
        "idle15.tim",
		"free0.tim",
        "free1.tim",
        "free2.tim",
        "free3.tim",
        "free4.tim",
        "free5.tim",
        "free6.tim",
        "free7.tim",
        "free8.tim",
        "free9.tim",
        "free10.tim",
        "free11.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
