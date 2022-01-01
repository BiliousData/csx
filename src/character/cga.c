/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "cga.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//cga character structure
enum
{
	cga_ArcMain_Idle0,
	cga_ArcMain_Idle1,
	cga_ArcMain_Idle2,
	cga_ArcMain_Idle3,
	cga_ArcMain_Idle4,
	cga_ArcMain_Lefta0,
	cga_ArcMain_Lefta1,
	cga_ArcMain_Lefta2,
	cga_ArcMain_Downa0,
	cga_ArcMain_Downa1,
	cga_ArcMain_Downa2,
	cga_ArcMain_Downa3,
	cga_ArcMain_Upa0,
	cga_ArcMain_Upa1,
	cga_ArcMain_Upa2,
	cga_ArcMain_Upa3,
	cga_ArcMain_Upa4,
	cga_ArcMain_Righta0,
	cga_ArcMain_Righta1,
	cga_ArcMain_Righta2,
	cga_ArcMain_Righta3,
	
	cga_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[cga_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_cga;

//cga character definitions
static const CharFrame char_cga_frame[] = {
	{cga_ArcMain_Idle0, {  0,   91, 104, 165}, { 0, 165}}, //0
	{cga_ArcMain_Idle0, {  152,   92, 104, 164}, { 0, 164}}, //1
	{cga_ArcMain_Idle1, {  0,   92, 104, 164}, { 0, 164}}, //2
	{cga_ArcMain_Idle1, {  153,   92, 103, 164}, { 0, 164}}, //3
	{cga_ArcMain_Idle2, {  0,   89, 103, 167}, { 0, 167}}, //4
	{cga_ArcMain_Idle2, {  154,   88, 102, 168}, { 0, 168}}, //5
	{cga_ArcMain_Idle3, {  0,   87, 102, 169}, { 0, 169}},//6
	{cga_ArcMain_Idle3, {  154,   87, 102, 169}, { 0, 169}},//7
	{cga_ArcMain_Idle4, {  0,   87, 102, 169}, { 0, 169}},//8
	
	{cga_ArcMain_Lefta0, {  0,   96, 112, 160}, { 0, 160}},//9
	{cga_ArcMain_Lefta0, {  145,   95, 111, 161}, { 0, 161}},//10
	{cga_ArcMain_Lefta1, {  0,   95, 110, 161}, { 0, 161}},//11
	{cga_ArcMain_Lefta1, {  145,   95, 111, 161}, { 0, 161}},//12
	{cga_ArcMain_Lefta2, {  0,   95, 110, 161}, { 0, 161}},//13
	{cga_ArcMain_Lefta2, {  145,   95, 111, 161}, { 0, 161}},//14
	
	{cga_ArcMain_Downa0, {  0,   110, 112, 145}, { 0, 145}},//15
	{cga_ArcMain_Downa0, {  148,   110, 108, 146}, { 0, 146}},//16
	{cga_ArcMain_Downa1, {  0,   110, 109, 146}, { 0, 146}},//17
	{cga_ArcMain_Downa1, {  148,   110, 108, 146}, { 0, 146}},//18
	{cga_ArcMain_Downa2, {  0,   110, 109, 147}, { 0, 147}},//19
	{cga_ArcMain_Downa2, {  148,   110, 108, 146}, { 0, 146}},//20
	{cga_ArcMain_Downa3, {    0,   110, 109, 146}, { 0, 146}},//21
	
	{cga_ArcMain_Upa0, {  0,   80, 107, 176}, { 0, 176}}, //22
	{cga_ArcMain_Upa0, {  149,   79, 107, 177}, { 0, 177}},//23
	{cga_ArcMain_Upa1, {  0,   83, 101, 173}, { 0, 173}}, //24
	{cga_ArcMain_Upa1, {  155,   83, 101, 173}, { 0, 173}},//25
	{cga_ArcMain_Upa2, {  0,   0, 0, 0}, { 0, 0}}, 
	{cga_ArcMain_Upa2, {  0,   0, 0, 0}, { 0, 0}},
	{cga_ArcMain_Upa3, {  0,   0, 0, 0}, { 0, 0}}, 
	{cga_ArcMain_Upa3, {  0,   0, 0, 0}, { 0, 0}},
	{cga_ArcMain_Upa4, {  0,   0, 0, 0}, { 0, 0}}, 
	
	{cga_ArcMain_Righta0, {  0,   0, 0, 0}, { 0, 0}}, 
	{cga_ArcMain_Righta0, {  0,   0, 0, 0}, { 0, 0}}, 
	{cga_ArcMain_Righta1, {  0,   0, 0, 0}, { 0, 0}}, 
	{cga_ArcMain_Righta1, {  0,   0, 0, 0}, { 0, 0}}, 
	{cga_ArcMain_Righta2, {  0,   0, 0, 0}, { 0, 0}}, 
	{cga_ArcMain_Righta2, {  0,   0, 0, 0}, { 0, 0}}, 
	{cga_ArcMain_Righta3, {  0,   0, 0, 0}, { 0, 0}}, 
	{cga_ArcMain_Righta3, {  0,   0, 0, 0}, { 0, 0}}, 
};

static const Animation char_cga_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, 8, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 9, 10, 11, 12, 13, 14, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 15, 16, 17, 18, 19, 20, 21, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 22, 23, 24, 25, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){10, 11, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//cga character functions
void Char_cga_SetFrame(void *user, u8 frame)
{
	Char_cga *this = (Char_cga*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_cga_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_cga_Tick(Character *character)
{
	Char_cga *this = (Char_cga*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_cga_SetFrame);
	Character_Draw(character, &this->tex, &char_cga_frame[this->frame]);
}

void Char_cga_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_cga_Free(Character *character)
{
	Char_cga *this = (Char_cga*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_cga_New(fixed_t x, fixed_t y)
{
	//Allocate cga object
	Char_cga *this = Mem_Alloc(sizeof(Char_cga));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_cga_New] Failed to allocate cga object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_cga_Tick;
	this->character.set_anim = Char_cga_SetAnim;
	this->character.free = Char_cga_Free;
	
	Animatable_Init(&this->character.animatable, char_cga_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\CGA.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",
        "idle1.tim",
        "idle2.tim",
        "idle3.tim",
        "idle4.tim",
        "lefta0.tim",
        "lefta1.tim",
        "lefta2.tim",
        "downa0.tim",
        "downa1.tim",
        "downa2.tim",
        "downa3.tim",
        "upa0.tim",
        "upa1.tim",
        "upa2.tim",
        "upa3.tim",
        "upa4.tim",
        "righta0.tim",
        "righta1.tim",
        "righta2.tim",
        "righta3.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
