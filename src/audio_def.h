#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(9455)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(3840)},  //XA_GameOver
	//SIDEA.XA
	{XA_SideA, XA_LENGTH(11600)}, //XA_Ukiyo
	{XA_SideA, XA_LENGTH(10000)}, //XA_Earworm
	//SIDEB.XA
	{XA_SideB, XA_LENGTH(12100)}, //XA_Machina
	{XA_SideB, XA_LENGTH(10300)}, //XA_SodaGroove
	//TAPE2A.XA
	{XA_Tape2A, XA_LENGTH(10100)}, //XA_CityFunk
	{XA_Tape2A, XA_LENGTH(110)}, //OUUGH
};

static const char *xa_paths[] = {
	"\\MUSIC\\MENU.XA;1",   //XA_Menu
	"\\MUSIC\\SIDEA.XA;1", //just bloody read it
	"\\MUSIC\\SIDEB.XA;1",
	"\\MUSIC\\TAPE2A.XA;1",
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3;

static const XA_Mp3 xa_mp3s[] = {
	//MENU.XA
	{"freaky", false},   //XA_GettinFreaky
	{"gameover", false}, //XA_GameOver
	//WEEK1A.XA
	{"ukiyo", true}, //XA_Bopeebo
	{"earworm", true},   //XA_Fresh
	//WEEK1B.XA
	{"machina", true}, //XA_Dadbattle
	{"sodagroove", true}, //XA_Tutorial
	//WEEK2A.XA
	{"cityfunk", true}, //XA_Spookeez
	{"ouugh", false},
	
	{NULL, false}
};
