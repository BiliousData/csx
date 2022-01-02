/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "kasso.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//KassO character structure
enum
{
	KassO_ArcMain_Idle0,
	KassO_ArcMain_Idle1,
	KassO_ArcMain_Idle2,
	KassO_ArcMain_Idle3,
	KassO_ArcMain_Idle4,
	KassO_ArcMain_Left0,
	KassO_ArcMain_Left1,
	KassO_ArcMain_Left2,
	KassO_ArcMain_Down0,
	KassO_ArcMain_Down1,
	KassO_ArcMain_Down2,
	KassO_ArcMain_Down3,
	KassO_ArcMain_Up0,
	KassO_ArcMain_Up1,
	KassO_ArcMain_Up2,
	KassO_ArcMain_Up3,
	KassO_ArcMain_Up4,
	KassO_ArcMain_Right0,
	KassO_ArcMain_Right1,
	KassO_ArcMain_Right2,
	KassO_ArcMain_Right3,
	
	KassO_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[KassO_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_KassO;

//KassO character definitions
static const CharFrame char_kasso_frame[] = {
	{KassO_ArcMain_Idle0, {  0,   0, 104, 165}, { 0, 165}}, //0 idle 1
	{KassO_ArcMain_Idle0, {105,   0, 104, 164}, { 0, 164}}, //1 idle 2
	{KassO_ArcMain_Idle1, {  0,   0, 104, 164}, { 0, 164}}, //2 idle 3
	{KassO_ArcMain_Idle1, {105,   0, 103, 164}, { 0, 164}}, //3 idle 4
	{KassO_ArcMain_Idle2, {  0,   0, 103, 167}, { 0, 167}}, //4 idle 5
	{KassO_ArcMain_Idle2, {104,   0, 104, 168}, { 0, 168}}, //5 idle 6
	{KassO_ArcMain_Idle3, {  0,   0, 102, 169}, { 0, 168}}, //6 idle 7
	{KassO_ArcMain_Idle3, {103,   0, 102, 169}, { 0, 168}}, //7 idle 8
	{KassO_ArcMain_Idle4, {  0,   0, 102, 169}, { 0, 168}}, //8 idle 9

	{KassO_ArcMain_Left0, {  0,   0, 112, 160}, {11, 159}}, //9 left 1
	{KassO_ArcMain_Left0, {113,   0, 111, 161}, {10, 160}}, //10 left 2
	{KassO_ArcMain_Left1, {  0,   0, 110, 161}, { 9, 160}}, //11 left 3
	{KassO_ArcMain_Left1, {111,   0, 111, 161}, {10, 160}}, //12 left 4
	{KassO_ArcMain_Left2, {  0,   0, 110, 161}, { 9, 160}}, //13 left 5
	{KassO_ArcMain_Left2, {111,   0, 111, 161}, {10, 160}}, //14 left 6

	{KassO_ArcMain_Down0, {  0,   0, 112, 145}, { 9, 144}}, //15 down 1
	{KassO_ArcMain_Down0, {113,   0, 108, 146}, { 7, 146}}, //16 down 2
	{KassO_ArcMain_Down1, {  0,   0, 109, 146}, { 7, 146}}, //17 down 3
	{KassO_ArcMain_Down1, {110,   0, 108, 146}, { 7, 146}}, //18 down 4
	{KassO_ArcMain_Down2, {  0,   0, 109, 146}, { 7, 146}}, //19 down 5
	{KassO_ArcMain_Down2, {110,   0, 108, 146}, { 7, 146}}, //20 down 6
	{KassO_ArcMain_Down3, {  0,   0, 109, 146}, { 7, 146}}, //21 down 7

	{KassO_ArcMain_Up0, {  0,   0, 107, 176}, { 6, 175}}, //22 up 1
	{KassO_ArcMain_Up0, {108,   0, 107, 177}, { 6, 176}}, //23 up 2
	{KassO_ArcMain_Up1, {  0,   0, 101, 173}, { 0, 172}}, //24 up 3
	{KassO_ArcMain_Up1, {102,   0, 101, 173}, { 0, 172}}, //25 up 4
	{KassO_ArcMain_Up2, {  0,   0, 101, 173}, { 0, 172}}, //26 up 5
	{KassO_ArcMain_Up2, {102,   0, 101, 172}, { 0, 171}}, //27 up 6
	{KassO_ArcMain_Up3, {  0,   0, 100, 172}, { 1, 171}}, //28 up 7
	{KassO_ArcMain_Up3, {101,   0, 101, 172}, { 0, 171}}, //29 up 8
	{KassO_ArcMain_Up4, {  0,   0, 101, 172}, { 0, 171}}, //30 up 9

	{KassO_ArcMain_Right0, {  0,   0, 118, 175}, {-1, 174}}, //31 right 1
	{KassO_ArcMain_Right0, {119,   0, 116, 173}, {-2, 172}}, //32 right 2
	{KassO_ArcMain_Right1, {  0,   0, 117, 173}, {-1, 172}}, //33 right 3
	{KassO_ArcMain_Right1, {  0,   0, 116, 173}, {-2, 172}}, //34 right 4
	{KassO_ArcMain_Right2, {  0,   0, 117, 172}, {-2, 171}}, //35 right 5
	{KassO_ArcMain_Right2, {  0,   0, 117, 171}, {-1, 170}}, //36 right 6
	{KassO_ArcMain_Right3, {  0,   0, 117, 171}, {-2, 170}}, //37 right 7
	{KassO_ArcMain_Right3, {  0,   0, 117, 171}, {-1, 170}}, //38 right 8
};

static const Animation char_kasso_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 8, ASCR_BACK, 3}}, //CharAnim_Idle
	{2, (const u8[]){ 9, 10, 11, 12, 13, 14, ASCR_BACK, 4}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 15, 16, 17, 18, 19, 20, 21, ASCR_BACK, 4}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 22, 23, 24, 25, 26, 27, 28, 29, ASCR_BACK, 4}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 31, 32, 33, 34, 35, 36, 37, 38, ASCR_BACK, 4}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//KassO character functions
void Char_KassO_SetFrame(void *user, u8 frame)
{
	Char_KassO *this = (Char_KassO*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_kasso_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_KassO_Tick(Character *character)
{
	Char_KassO *this = (Char_KassO*)character;
	
	//Perform idle
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
	{
		Character_CheckEndSing(character);
		
		if (stage.flag & STAGE_FLAG_JUST_STEP)
		{
			if ((Animatable_Ended(&character->animatable)) &&
				(character->animatable.anim != CharAnim_Left &&
				 character->animatable.anim != CharAnim_Down &&
				 character->animatable.anim != CharAnim_Up &&
				 character->animatable.anim != CharAnim_Right) &&
				(stage.song_step & 0x7) == 0)
				character->set_anim(character, CharAnim_Idle);
		}
	}
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_KassO_SetFrame);
	Character_Draw(character, &this->tex, &char_kasso_frame[this->frame]);
}

void Char_KassO_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_KassO_Free(Character *character)
{
	Char_KassO *this = (Char_KassO*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_KassO_New(fixed_t x, fixed_t y)
{
	//Allocate kasso object
	Char_KassO *this = Mem_Alloc(sizeof(Char_KassO));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_KassO_New] Failed to allocate kasso object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_KassO_Tick;
	this->character.set_anim = Char_KassO_SetAnim;
	this->character.free = Char_KassO_Free;
	
	Animatable_Init(&this->character.animatable, char_kasso_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\KASSO.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",
        "idle1.tim",
        "idle2.tim",
        "idle3.tim",
        "idle4.tim",
        "left0.tim",
        "left1.tim",
        "left2.tim",
        "down0.tim",
        "down1.tim",
        "down2.tim",
        "down3.tim",
        "up0.tim",
        "up1.tim",
        "up2.tim",
        "up3.tim",
        "up4.tim",
        "right0.tim",
        "right1.tim",
        "right2.tim",
        "right3.tim", 
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
