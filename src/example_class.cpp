#include "example_class.h"
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/base_material3d.hpp>
#include <cmath>
#include <queue>

using namespace godot;

PixelProperties PixelDatabase::pixels[131];
bool PixelDatabase::initialized = false;

PixelDatabase::PixelDatabase() { init_database(); }
PixelDatabase::~PixelDatabase() {}

void PixelDatabase::init_database() {
	if (initialized) return;
	pixels[0]={0,"Levego",0,0,0,0,0,0,0,0,0,0,-1,-9999,-9999,0,0,100,0.0f,0.0f,0.0f};
	pixels[1]={1,"Humusz",0,5,12,4,5,1,18,10,6,12,250,-9999,-9999,8,5,10,0.30f,0.20f,0.12f}; pixels[2]={2,"Erdotalaj",0,6,15,5,6,1,19,12,8,13,280,-9999,-9999,9,6,14,0.20f,0.14f,0.09f};
	pixels[3]={3,"MocsariFold",0,7,18,6,8,1,22,14,9,16,-1,-9999,-9999,6,10,25,0.18f,0.20f,0.18f}; pixels[4]={4,"VizesIszap",0,5,16,4,12,2,25,12,7,20,-1,-9999,-9999,4,18,86,0.25f,0.23f,0.19f};
	pixels[5]={5,"SzikIszap",0,9,22,12,3,1,20,18,12,14,-1,-9999,-9999,14,1,15,0.42f,0.38f,0.32f}; pixels[6]={6,"Sar",0,8,20,7,10,2,26,16,10,22,-1,-9999,-9999,5,15,82,0.29f,0.19f,0.12f};
	pixels[7]={7,"Agyag",0,10,28,9,20,3,28,22,15,25,-1,1100,2200,7,12,75,0.58f,0.42f,0.34f}; pixels[8]={8,"Soder",0,14,32,18,1,5,30,8,5,19,-1,1300,2400,24,0,34,0.48f,0.47f,0.45f};
	pixels[9]={9,"Tozeg",0,5,16,4,10,3,12,12,8,8,180,-9999,-9999,6,1,8,0.22f,0.16f,0.10f}; pixels[10]={10,"SzarazAvar",0,1,3,1,15,5,3,2,1,4,120,-9999,-9999,45,0,35,0.54f,0.34f,0.14f};
	pixels[11]={11,"Ho",0,4,12,5,6,3,14,10,6,15,-1,0,100,8,3,80,0.95f,0.97f,1.00f}; pixels[12]={12,"SullyedoHo",0,2,5,2,2,1,12,1,1,45,-1,-5,100,4,3,75,0.88f,0.93f,0.98f};
	pixels[13]={13,"Jeghartya",0,10,20,16,2,6,19,10,6,28,-1,0,100,42,4,98,0.72f,0.88f,0.98f}; pixels[14]={14,"FosziliaCsont",1,38,95,42,4,6,30,65,45,16,-1,1600,2800,40,1,26,0.82f,0.79f,0.70f};
	pixels[15]={15,"FosziliaMesz",1,32,80,35,2,5,32,55,38,22,-1,1400,2500,36,0,24,0.78f,0.76f,0.72f}; pixels[16]={16,"Guano",0,3,8,3,5,1,10,5,3,10,150,-9999,-9999,5,8,45,0.30f,0.28f,0.25f};
	pixels[17]={17,"SimaKo",1,35,90,38,2,5,35,60,42,25,-1,1350,2500,36,0,22,0.42f,0.44f,0.47f}; pixels[18]={18,"RepedezettKo",0,4,6,28,0,2,32,1,1,24,-1,1350,2500,50,0,20,0.38f,0.40f,0.42f};
	pixels[19]={19,"ErosKo",2,75,210,72,3,7,42,140,100,28,-1,1500,2700,46,0,22,0.26f,0.29f,0.34f}; pixels[20]={20,"Andezit",1,48,125,50,2,6,42,85,60,27,-1,1450,2600,46,0,22,0.32f,0.35f,0.40f};
	pixels[21]={21,"Diorit",1,52,135,54,2,6,44,92,65,30,-1,1500,2700,50,0,25,0.72f,0.72f,0.74f}; pixels[22]={22,"Marvany",1,44,115,45,3,7,39,78,55,30,-1,1400,2550,44,0,52,0.92f,0.91f,0.94f};
	pixels[23]={23,"Gubbancsko",2,65,160,60,10,300,38,70,50,95,-1,1600,2800,80,0,24,0.85f,0.35f,0.15f}; pixels[24]={24,"Szen",1,24,60,26,2,3,26,40,28,18,350,-9999,-9999,28,14,24,0.14f,0.15f,0.18f};
	pixels[25]={25,"Vas",2,70,200,68,8,8,50,150,110,58,-1,1538,2860,48,55,26,0.68f,0.62f,0.58f}; pixels[26]={26,"Rez",2,55,160,52,28,10,54,115,80,85,-1,1085,2560,44,92,34,0.82f,0.45f,0.25f};
	pixels[27]={27,"FoszilisVasfa",2,68,190,65,16,9,40,140,100,18,1200,-9999,-9999,40,8,22,0.32f,0.33f,0.36f}; pixels[28]={28,"Kvarc",2,60,150,75,0,8,37,95,68,34,-1,1670,2950,65,4,48,0.94f,0.92f,0.96f};
	pixels[29]={29,"KintErc",3,140,380,130,22,15,40,280,210,35,-1,1850,3200,45,60,32,0.10f,0.78f,1.00f}; pixels[30]={30,"Smaragd",4,200,520,190,0,9,34,340,250,36,-1,2100,3500,72,0,58,0.12f,0.82f,0.35f};
	pixels[31]={31,"LoncoreErc",4,250,650,100,20,14,45,480,360,60,-1,2400,3800,75,75,35,1.00f,0.84f,0.20f}; pixels[32]={32,"Gyemant",5,450,1200,400,5,18,40,850,680,100,4000,3550,4800,98,0,75,0.55f,0.95f,1.00f};
	pixels[33]={33,"TolgyKemenyfa",1,32,85,30,16,8,22,75,55,11,280,-9999,-9999,34,0,20,0.54f,0.37f,0.20f}; pixels[34]={34,"TolgyKereg",0,12,25,11,10,3,12,20,14,8,240,-9999,-9999,16,0,18,0.30f,0.20f,0.12f};
	pixels[35]={35,"TolgyLevel",0,2,6,2,20,8,4,5,3,5,160,-9999,-9999,4,0,30,0.15f,0.44f,0.12f}; pixels[36]={36,"TolgyGyoker",1,28,75,26,24,8,20,80,60,10,300,-9999,-9999,25,0,18,0.34f,0.23f,0.14f};
	pixels[37]={37,"MakkHej",0,14,28,18,5,6,16,15,10,10,220,-9999,-9999,22,0,30,0.46f,0.30f,0.14f}; pixels[38]={38,"MakkBelso",0,5,10,6,8,4,12,6,4,8,200,-9999,-9999,8,0,25,0.76f,0.64f,0.38f};
	pixels[39]={39,"VasfaTorzs",2,65,185,62,18,8,38,135,95,15,1100,-9999,-9999,38,5,22,0.38f,0.39f,0.42f}; pixels[40]={40,"VasfaLevel",1,18,40,22,25,10,12,25,18,20,600,-9999,-9999,30,10,35,0.50f,0.54f,0.56f};
	pixels[41]={41,"MangroveFa",1,30,80,28,20,8,24,70,50,12,320,-9999,-9999,30,0,22,0.45f,0.28f,0.20f}; pixels[42]={42,"MangroveGyoker",1,34,90,30,26,9,25,85,62,12,340,-9999,-9999,28,0,24,0.38f,0.24f,0.16f};
	pixels[43]={43,"MangroveLevel",0,3,8,3,22,8,5,6,4,6,180,-9999,-9999,5,0,38,0.16f,0.42f,0.20f}; pixels[44]={44,"FenyoPuhafa",1,20,55,18,20,8,14,45,32,8,220,-9999,-9999,22,0,22,0.68f,0.54f,0.36f};
	pixels[45]={45,"FenyoKereg",0,9,20,9,12,4,10,16,11,7,190,-9999,-9999,14,0,20,0.42f,0.26f,0.18f}; pixels[46]={46,"Fenyotu",0,2,5,2,16,6,4,4,2,5,110,-9999,-9999,4,0,54,0.14f,0.38f,0.22f};
	pixels[47]={47,"HavasFenyotu",0,4,9,4,12,4,8,6,4,12,300,0,100,6,2,70,0.75f,0.88f,0.82f}; pixels[48]={48,"Toboz",0,8,16,10,8,5,9,8,5,8,170,-9999,-9999,14,0,26,0.40f,0.28f,0.16f};
	pixels[49]={49,"Gyanta",0,8,18,9,12,3,12,10,6,8,130,80,200,14,0,40,0.92f,0.68f,0.12f}; pixels[50]={50,"Korhadtfa",0,9,20,8,6,3,10,14,9,7,180,-9999,-9999,12,0,20,0.32f,0.28f,0.22f};
	pixels[51]={51,"ZoldFu",0,2,5,1,25,8,4,4,2,6,160,-9999,-9999,3,2,40,0.18f,0.46f,0.12f}; pixels[52]={52,"MagasFu",0,3,8,2,28,8,5,5,3,6,150,-9999,-9999,4,2,38,0.20f,0.50f,0.14f};
	pixels[53]={53,"SzarazFu",0,2,4,1,20,6,3,3,2,5,90,-9999,-9999,6,0,42,0.72f,0.64f,0.28f}; pixels[54]={54,"Viragszirom",0,1,3,1,22,6,2,2,1,4,140,-9999,-9999,2,0,35,0.96f,0.26f,0.46f};
	pixels[55]={55,"Viragszar",0,4,9,3,30,8,4,6,4,5,160,-9999,-9999,4,2,30,0.22f,0.52f,0.16f}; pixels[56]={56,"Moha",0,3,10,2,35,12,14,8,5,10,220,-9999,-9999,3,6,45,0.16f,0.44f,0.13f};
	pixels[57]={57,"IndaRost",1,16,45,8,50,12,9,35,25,5,190,-9999,-9999,5,2,20,0.26f,0.48f,0.20f}; pixels[58]={58,"Tovis",1,22,50,28,10,5,14,28,20,8,200,-9999,-9999,18,0,15,0.35f,0.22f,0.16f};
	pixels[59]={59,"SparkHus",1,5,12,4,18,6,8,6,4,30,80,-9999,-9999,6,25,35,1.00f,0.65f,0.10f}; pixels[60]={60,"SparkMag",1,10,18,16,2,10,12,8,5,60,20,-9999,-9999,20,40,25,1.00f,0.90f,0.20f};
	pixels[61]={61,"Mushspoon",0,8,22,6,28,10,8,12,8,10,240,-9999,-9999,5,4,15,0.65f,0.55f,0.42f}; pixels[62]={62,"Galoca",0,3,9,2,20,8,6,5,3,8,210,-9999,-9999,4,2,30,0.90f,0.15f,0.15f};
	pixels[63]={63,"Puffatag",0,2,5,2,35,18,4,4,2,5,150,-9999,-9999,8,0,25,0.72f,0.68f,0.55f}; pixels[64]={64,"Szarvasgomba",1,28,70,26,12,6,18,45,32,9,260,-9999,-9999,20,0,20,0.42f,0.34f,0.26f};
	pixels[65]={65,"MocsariGomba",0,4,12,3,22,9,7,6,4,8,220,-9999,-9999,4,2,28,0.58f,0.52f,0.44f}; pixels[66]={66,"ParazitaTest",2,35,110,25,55,14,16,48,35,12,250,-9999,-9999,8,10,20,0.45f,0.18f,0.25f};
	pixels[67]={67,"ParazitaFog",2,55,140,60,8,8,24,60,42,14,-1,-9999,-9999,35,2,25,0.92f,0.88f,0.82f}; pixels[68]={68,"Sziv",1,12,180,10,40,10,20,18,12,25,-1,-9999,-9999,6,30,40,0.85f,0.08f,0.12f};
	pixels[69]={69,"Agy",1,8,180,6,30,5,18,12,8,22,-1,-9999,-9999,4,45,50,0.92f,0.65f,0.72f}; pixels[70]={70,"Tudo",1,8,120,5,50,15,10,12,8,18,-1,-9999,-9999,5,20,35,0.88f,0.48f,0.52f};
	pixels[71]={71,"Kopoltyu",1,6,90,5,35,8,12,10,6,22,-1,-9999,-9999,4,25,45,0.78f,0.32f,0.38f}; pixels[72]={72,"Gyomor",1,14,150,12,55,12,18,22,15,20,-1,-9999,-9999,6,20,30,0.72f,0.45f,0.40f};
	pixels[73]={73,"Maj",1,10,140,8,25,5,22,15,10,24,-1,-9999,-9999,5,25,40,0.48f,0.18f,0.16f}; pixels[74]={74,"Vese",1,10,130,8,25,6,19,15,10,22,-1,-9999,-9999,5,25,38,0.55f,0.22f,0.20f};
	pixels[75]={75,"Bel",1,12,140,9,65,15,15,28,20,18,-1,-9999,-9999,5,18,45,0.82f,0.58f,0.55f}; pixels[76]={76,"Verer",1,10,100,7,45,10,16,15,10,28,-1,-9999,-9999,5,35,35,0.95f,0.05f,0.08f};
	pixels[77]={77,"Szem",0,4,40,4,25,8,15,6,4,20,-1,-9999,-9999,4,25,70,0.95f,0.95f,0.98f}; pixels[78]={78,"Ful",1,9,90,8,38,10,14,14,9,18,-1,-9999,-9999,95,20,30,0.86f,0.68f,0.56f};
	pixels[79]={79,"Orr",1,9,95,8,32,8,15,14,9,20,-1,-9999,-9999,6,22,35,0.82f,0.62f,0.54f}; pixels[80]={80,"Nyelv",1,14,150,12,50,10,20,20,14,22,-1,-9999,-9999,6,25,40,0.85f,0.35f,0.42f};
	pixels[81]={81,"PokMereg",1,12,85,9,35,8,16,14,9,18,-1,-9999,-9999,5,20,35,0.45f,0.78f,0.20f}; pixels[82]={82,"SkorpioMereg",1,14,95,11,30,6,17,16,11,18,-1,-9999,-9999,6,20,35,0.55f,0.85f,0.15f};
	pixels[83]={83,"Szovomirigy",1,12,90,9,45,12,15,16,11,16,220,-9999,-9999,5,10,25,0.88f,0.88f,0.92f}; pixels[84]={84,"VorosHus",1,15,180,12,40,8,22,25,18,22,-1,-9999,-9999,6,25,30,0.78f,0.22f,0.22f};
	pixels[85]={85,"Halhus",0,7,75,6,28,5,18,10,6,20,-1,-9999,-9999,4,22,40,0.92f,0.85f,0.82f}; pixels[86]={86,"FagyhalHus",1,12,105,10,30,6,20,16,11,10,-1,-9999,-9999,5,20,45,0.75f,0.88f,0.95f};
	pixels[87]={87,"HulloHus",1,12,115,10,35,8,20,18,12,18,-1,-9999,-9999,5,20,35,0.65f,0.75f,0.58f}; pixels[88]={88,"Rakhus",0,7,70,6,22,5,17,9,6,18,-1,-9999,-9999,4,20,35,0.96f,0.78f,0.68f};
	pixels[89]={89,"VekonyCsont",1,22,55,26,10,6,16,35,24,12,-1,1500,2700,32,1,28,0.90f,0.88f,0.82f}; pixels[90]={90,"NormalCsont",1,35,90,38,8,8,26,60,42,14,-1,1600,2800,38,1,28,0.92f,0.90f,0.84f};
	pixels[91]={91,"OriasCsont",2,58,165,60,6,7,36,110,80,16,-1,1700,2900,42,1,26,0.86f,0.84f,0.76f}; pixels[92]={92,"Porc",0,12,35,15,35,10,12,20,14,12,-1,-9999,-9999,12,2,35,0.85f,0.85f,0.90f};
	pixels[93]={93,"Uszony",1,10,40,9,65,20,6,25,18,10,-1,-9999,-9999,8,5,40,0.60f,0.82f,0.95f}; pixels[94]={94,"InJAP",1,30,110,22,75,22,18,75,55,12,-1,-9999,-9999,10,8,25,0.88f,0.78f,0.72f};
	pixels[95]={95,"Zsir",0,5,15,4,18,2,14,6,4,8,160,45,220,3,0,85,0.96f,0.94f,0.82f}; pixels[96]={96,"Vekonybor",1,14,40,10,45,10,12,18,12,5,240,-9999,-9999,4,2,28,0.72f,0.56f,0.44f};
	pixels[97]={97,"Szarvasbor",1,18,50,12,50,11,13,24,16,5,250,-9999,-9999,4,0,16,0.62f,0.44f,0.30f}; pixels[98]={98,"Vastagbor",1,24,65,16,42,10,16,32,22,6,280,-9999,-9999,6,2,24,0.52f,0.36f,0.24f};
	pixels[99]={99,"Medvebor",2,35,95,24,38,9,20,45,32,7,300,-9999,-9999,7,0,20,0.38f,0.26f,0.18f}; pixels[100]={100,"Szarnyhartya",0,8,22,5,55,14,5,12,8,6,210,-9999,-9999,3,2,30,0.28f,0.24f,0.28f};
	pixels[101]={101,"Halpikkely",0,10,28,12,28,6,14,14,9,15,-1,-9999,-9999,8,10,80,0.48f,0.56f,0.54f}; pixels[102]={102,"Alcabor",0,10,28,11,30,6,15,14,9,16,-1,-9999,-9999,5,10,75,0.34f,0.32f,0.26f};
	pixels[103]={103,"Gotebor",0,6,18,5,40,8,12,8,5,18,-1,-9999,-9999,3,15,92,0.25f,0.45f,0.35f}; pixels[104]={104,"Gyikpikkely",1,26,68,25,28,8,16,35,24,10,320,-9999,-9999,14,0,32,0.32f,0.52f,0.28f};
	pixels[105]={105,"Gekkotalp",1,10,32,8,50,10,10,45,32,8,230,-9999,-9999,3,0,0,0.45f,0.68f,0.38f}; pixels[106]={106,"Lopata",1,34,82,35,14,10,24,50,36,10,350,-9999,-9999,32,0,25,0.24f,0.22f,0.20f};
	pixels[107]={107,"MocsariPata",1,28,70,28,20,10,18,55,40,10,340,-9999,-9999,24,0,15,0.28f,0.26f,0.22f}; pixels[108]={108,"Sziklapata",1,36,88,36,24,12,22,65,48,10,350,-9999,-9999,28,0,2,0.22f,0.22f,0.24f};
	pixels[109]={109,"SimaSzaru",1,32,78,34,16,10,22,48,34,10,350,-9999,-9999,30,0,34,0.35f,0.32f,0.28f}; pixels[110]={110,"RackaSzaru",1,38,92,40,14,10,24,56,40,10,360,-9999,-9999,34,0,32,0.42f,0.38f,0.32f};
	pixels[111]={111,"MuflonSzaru",2,62,175,66,12,14,34,115,85,12,400,-9999,-9999,40,0,28,0.48f,0.42f,0.34f}; pixels[112]={112,"Agancs",1,40,95,42,10,8,25,62,44,12,-1,1600,2800,36,0,28,0.65f,0.58f,0.48f};
	pixels[113]={113,"Hodfog",1,48,120,55,6,6,28,70,50,18,-1,1600,2800,42,5,24,0.90f,0.52f,0.15f}; pixels[114]={114,"Tepofog",2,52,135,58,6,6,27,75,54,14,-1,1650,2850,40,0,26,0.95f,0.94f,0.88f};
	pixels[115]={115,"RagadozoKarom",2,50,130,56,10,8,24,72,52,12,380,-9999,-9999,36,0,28,0.25f,0.24f,0.22f}; pixels[116]={116,"BorzKarom",1,42,105,48,12,7,22,60,42,12,360,-9999,-9999,32,0,25,0.32f,0.30f,0.28f};
	pixels[117]={117,"Madarcsor",1,36,85,44,12,8,18,50,36,10,340,-9999,-9999,34,0,30,0.88f,0.72f,0.18f}; pixels[118]={118,"Rakollo",2,46,120,52,10,8,20,65,46,10,350,-9999,-9999,34,0,32,0.30f,0.22f,0.20f};
	pixels[119]={119,"Hodprem",1,8,28,4,45,12,6,12,8,3,220,-9999,-9999,2,0,45,0.42f,0.28f,0.18f}; pixels[120]={120,"RovidSzor",0,3,10,2,35,10,4,5,3,4,190,-9999,-9999,3,0,25,0.68f,0.56f,0.48f};
	pixels[121]={121,"Ragadozoprem",1,12,38,6,48,14,8,16,11,2,210,-9999,-9999,2,0,22,0.36f,0.26f,0.18f}; pixels[122]={122,"SimaGyapju",1,5,20,2,50,15,5,8,5,2,200,-9999,-9999,1,0,18,0.94f,0.93f,0.90f};
	pixels[123]={123,"RackaGyapju",1,9,32,4,55,18,7,14,9,1,230,-9999,-9999,1,0,16,0.86f,0.84f,0.78f}; pixels[124]={124,"Alaptoll",0,1,4,1,30,15,1,2,1,2,160,-9999,-9999,2,0,38,0.96f,0.96f,0.98f};
	pixels[125]={125,"Facantoll",0,2,7,2,35,16,1,4,2,2,170,-9999,-9999,3,0,40,0.85f,0.42f,0.16f}; pixels[126]={126,"Hollotoll",1,3,10,3,38,16,2,6,4,3,180,-9999,-9999,3,0,42,0.10f,0.11f,0.13f};
	pixels[127]={127,"Evezotoll",1,6,18,5,45,20,2,12,8,3,200,-9999,-9999,4,0,45,0.38f,0.30f,0.24f}; pixels[128]={128,"Kitin",2,44,115,48,16,12,16,60,42,9,320,-9999,-9999,28,0,42,0.26f,0.22f,0.20f};
	pixels[129]={129,"TarantulaKitin",2,40,105,42,20,10,15,55,38,8,280,-9999,-9999,15,0,25,0.18f,0.15f,0.14f}; pixels[130]={130,"Tojas",0,8,12,14,2,4,12,10,6,14,-1,-9999,-9999,22,2,35,0.96f,0.92f,0.82f};
	initialized = true;
}

const PixelProperties& PixelDatabase::get_pixel_fast(uint8_t p_id) {
	if (!initialized) init_database();
	return (p_id <= 130) ? pixels[p_id] : pixels[0];
}

Dictionary PixelDatabase::get_pixel_info(int p_id) const {
	const PixelProperties& p = get_pixel_fast((uint8_t)CLAMP(p_id, 0, 130));
	Dictionary d;
	d["id"]=(int)p.id; d["name"]=String(p.name); d["tier"]=(int)p.tier;
	d["ak_threshold"]=p.ak_threshold; d["max_durability"]=p.max_durability;
	d["hardness"]=p.hardness; d["elasticity"]=p.elasticity; d["bounciness"]=p.bounciness;
	d["mass_pm"]=p.mass_pm; d["gap_resistance"]=p.gap_resistance; d["ap_max"]=p.ap_max;
	d["thermal_cond"]=p.thermal_cond; d["ignition_point"]=p.ignition_point;
	d["melting_point"]=p.melting_point; d["boiling_point"]=p.boiling_point;
	d["acoustic_vib"]=p.acoustic_vib; d["electric_cond"]=p.electric_cond;
	d["slipperiness"]=p.slipperiness; d["color"]=Color(p.r, p.g, p.b, 1.0f);
	return d;
}

int PixelDatabase::get_pixel_ak(int p_id) const { return get_pixel_fast((uint8_t)CLAMP(p_id, 0, 130)).ak_threshold; }
int PixelDatabase::get_pixel_durability(int p_id) const { return get_pixel_fast((uint8_t)CLAMP(p_id, 0, 130)).max_durability; }
int PixelDatabase::get_pixel_hardness(int p_id) const { return get_pixel_fast((uint8_t)CLAMP(p_id, 0, 130)).hardness; }
int PixelDatabase::get_pixel_mass_pm(int p_id) const { return get_pixel_fast((uint8_t)CLAMP(p_id, 0, 130)).mass_pm; }
int PixelDatabase::get_pixel_slipperiness(int p_id) const { return get_pixel_fast((uint8_t)CLAMP(p_id, 0, 130)).slipperiness; }
Color PixelDatabase::get_pixel_color(int p_id) const {
	const PixelProperties& p = get_pixel_fast((uint8_t)CLAMP(p_id, 0, 130));
	return Color(p.r, p.g, p.b, 1.0f);
}

PackedByteArray PixelDatabase::get_gpu_material_buffer() const {
	PackedByteArray buf; buf.resize(131 * 16);
	float* ptr = reinterpret_cast<float*>(buf.ptrw());
	for (int i = 0; i <= 130; ++i) {
		const PixelProperties& p = get_pixel_fast((uint8_t)i);
		ptr[i*4+0]=p.r; ptr[i*4+1]=p.g; ptr[i*4+2]=p.b; ptr[i*4+3]=(float)p.max_durability;
	}
	return buf;
}

void PixelDatabase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_pixel_info", "id"), &PixelDatabase::get_pixel_info);
	ClassDB::bind_method(D_METHOD("get_pixel_ak", "id"), &PixelDatabase::get_pixel_ak);
	ClassDB::bind_method(D_METHOD("get_pixel_durability", "id"), &PixelDatabase::get_pixel_durability);
	ClassDB::bind_method(D_METHOD("get_pixel_hardness", "id"), &PixelDatabase::get_pixel_hardness);
	ClassDB::bind_method(D_METHOD("get_pixel_mass_pm", "id"), &PixelDatabase::get_pixel_mass_pm);
	ClassDB::bind_method(D_METHOD("get_pixel_slipperiness", "id"), &PixelDatabase::get_pixel_slipperiness);
	ClassDB::bind_method(D_METHOD("get_pixel_color", "id"), &PixelDatabase::get_pixel_color);
	ClassDB::bind_method(D_METHOD("get_gpu_material_buffer"), &PixelDatabase::get_gpu_material_buffer);
}

static inline float fast_hash(int x, int y, int z, int seed) {
	int n = x * 374761393 + y * 668265263 + z * 2147483647 + seed * 1610612741;
	n = (n ^ (n >> 13)) * 1274126177;
	return (float)((n ^ (n >> 16)) & 0x7FFFFFFF) / 2147483647.0f;
}

static float smooth_noise3(float x, float y, float z, int seed) {
	int ix = (int)std::floor(x), iy = (int)std::floor(y), iz = (int)std::floor(z);
	float fx = x - ix, fy = y - iy, fz = z - iz;
	fx = fx*fx*(3.0f-2.0f*fx); fy = fy*fy*(3.0f-2.0f*fy); fz = fz*fz*(3.0f-2.0f*fz);
	auto h = [&](int dx, int dy, int dz) { return fast_hash(ix+dx, iy+dy, iz+dz, seed); };
	float c00 = h(0,0,0)*(1-fx)+h(1,0,0)*fx, c10 = h(0,1,0)*(1-fx)+h(1,1,0)*fx;
	float c01 = h(0,0,1)*(1-fx)+h(1,0,1)*fx, c11 = h(0,1,1)*(1-fx)+h(1,1,1)*fx;
	return (c00*(1-fy)+c10*fy)*(1-fz) + (c01*(1-fy)+c11*fy)*fz;
}

static int get_global_terrain_y(int gx, int gz, int seed) {
	float n1 = smooth_noise3(gx * 0.0055f, 0.0f, gz * 0.0055f, seed) * 12.0f;
	float n2 = smooth_noise3(gx * 0.018f, 0.0f, gz * 0.018f, seed + 101) * 3.0f;
	return CLAMP((int)(14.0f + n1 + n2), 7, 32);
}

static inline int floor_div_int(int a, int b) {
	int d = a / b, r = a % b;
	return (r != 0 && ((r < 0) ^ (b < 0))) ? (d - 1) : d;
}

VoxelBody3D::VoxelBody3D() {
	PixelDatabase::init_database();
	size_x = 64; size_y = 128; size_z = 64; max_filled_y = 0; voxel_scale = 0.1f;
	min_terrain_y = 0; max_terrain_y = 32;
	high_min_x = 0; high_max_x = -1; high_min_z = 0; high_max_z = -1;
	chunk_gx_origin = 0; chunk_gz_origin = 0; cached_world_seed = 0;
	is_terrain_chunk = false; is_sleeping = true; ores_generated = false;
	is_static_body = true; total_mass_pm = 0; center_of_mass = Vector3(0, 0, 0);
	kint_stored_damage = 0.0f; kint_timer = 0.0f;
	set_process(false); set_physics_process(false);
}

VoxelBody3D::~VoxelBody3D() {}

void VoxelBody3D::ensure_render_resources() {
	if (voxel_mesh.is_null()) { voxel_mesh.instantiate(); set_mesh(voxel_mesh); }
	if (voxel_material.is_null()) {
		voxel_material.instantiate();
		voxel_material->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
		voxel_material->set_roughness(0.82f);
		set_material_override(voxel_material);
	}
}

void VoxelBody3D::initialize_grid(int p_x, int p_y, int p_z, float p_voxel_scale) {
	size_x = CLAMP(p_x, 1, 256); size_y = CLAMP(p_y, 1, 256); size_z = CLAMP(p_z, 1, 256);
	max_filled_y = 0; min_terrain_y = 0; max_terrain_y = size_y - 1;
	high_min_x = 0; high_max_x = size_x - 1; high_min_z = 0; high_max_z = size_z - 1;
	voxel_scale = p_voxel_scale;
	int total = size_x * size_y * size_z;
	voxel_ids.resize(total); voxel_ids.fill(0);
	voxel_durability.resize(0); voxel_heat.resize(0);
	is_sleeping = true; ores_generated = false;
	set_process(false); set_physics_process(false);
}

void VoxelBody3D::wake_up() {
	if (!is_sleeping && voxel_durability.size() == voxel_ids.size()) return;
	uint8_t* ids_w = voxel_ids.ptrw();
	if (is_terrain_chunk && !ores_generated) {
		for (int z = 0; z < size_z; ++z) {
			int gz = chunk_gz_origin + z;
			for (int x = 0; x < size_x; ++x) {
				int gx = chunk_gx_origin + x, sy = get_surface_y(x, z);
				for (int y = 1; y < sy; ++y) {
					int idx = get_index(x, y, z);
					if (ids_w[idx] != 17 && ids_w[idx] != 1) continue;
					uint8_t pid = 17;
					if (y >= sy - 4) pid = (smooth_noise3(gx*0.08f, y*0.08f, gz*0.08f, cached_world_seed+5) > 0.72f) ? 7 : 1;
					else if (y >= sy - 7) pid = 8;
					else if (smooth_noise3(gx*0.12f, y*0.12f, gz*0.12f, cached_world_seed+19) > 0.68f) {
						float ro = fast_hash(gx, y, gz, cached_world_seed + 33);
						pid = (ro < 0.03f) ? 29 : ((ro < 0.20f) ? 28 : ((ro < 0.50f) ? 25 : 24));
					}
					ids_w[idx] = pid;
				}
			}
		}
		ores_generated = true;
	}
	int total = size_x * size_y * size_z;
	voxel_durability.resize(total);
	int32_t* dur_w = voxel_durability.ptrw();
	for (int i = 0; i < total; ++i) {
		uint8_t id = ids_w[i];
		dur_w[i] = (id != 0) ? PixelDatabase::get_pixel_fast(id).max_durability : 0;
	}
	is_sleeping = false;
}

void VoxelBody3D::sleep_sector() {
	voxel_durability.resize(0); voxel_heat.resize(0);
	is_sleeping = true; set_process(false); set_physics_process(false);
}

void VoxelBody3D::fill_block(int p_pixel_id) {
	uint8_t id = (uint8_t)CLAMP(p_pixel_id, 0, 130);
	voxel_ids.fill(id);
	if (!is_sleeping && voxel_durability.size() == voxel_ids.size()) voxel_durability.fill(PixelDatabase::get_pixel_fast(id).max_durability);
	max_filled_y = (id != 0) ? (size_y - 1) : 0;
	rebuild_mesh();
}

void VoxelBody3D::fill_box(Vector3i p_min, Vector3i p_max, int p_pixel_id) {
	uint8_t id = (uint8_t)CLAMP(p_pixel_id, 0, 130);
	uint8_t* ids_w = voxel_ids.ptrw();
	int x0 = CLAMP(p_min.x, 0, size_x-1), x1 = CLAMP(p_max.x, 0, size_x-1);
	int y0 = CLAMP(p_min.y, 0, size_y-1), y1 = CLAMP(p_max.y, 0, size_y-1);
	int z0 = CLAMP(p_min.z, 0, size_z-1), z1 = CLAMP(p_max.z, 0, size_z-1);
	if (id != 0 && y1 > max_filled_y) max_filled_y = y1;
	bool has_dur = (!is_sleeping && voxel_durability.size() == voxel_ids.size());
	int32_t max_dur = PixelDatabase::get_pixel_fast(id).max_durability;
	int32_t* dur_w = has_dur ? voxel_durability.ptrw() : nullptr;
	for (int z = z0; z <= z1; ++z)
		for (int y = y0; y <= y1; ++y)
			for (int x = x0; x <= x1; ++x) {
				int idx = get_index(x, y, z); ids_w[idx] = id;
				if (has_dur) dur_w[idx] = max_dur;
			}
	rebuild_mesh();
}

void VoxelBody3D::set_voxel(int x, int y, int z, int p_pixel_id) {
	if (!is_valid_coord(x, y, z)) return;
	uint8_t id = (uint8_t)CLAMP(p_pixel_id, 0, 130);
	int idx = get_index(x, y, z);
	voxel_ids.ptrw()[idx] = id;
	if (!is_sleeping && voxel_durability.size() == voxel_ids.size()) voxel_durability.ptrw()[idx] = PixelDatabase::get_pixel_fast(id).max_durability;
	if (id != 0) {
		if (y > max_filled_y) max_filled_y = y;
		if (is_terrain_chunk && y > max_terrain_y + 2) {
			if (x < high_min_x) high_min_x = x;
			if (x > high_max_x) high_max_x = x;
			if (z < high_min_z) high_min_z = z;
			if (z > high_max_z) high_max_z = z;
		}
	}
}

int VoxelBody3D::get_voxel(int x, int y, int z) const { return is_valid_coord(x, y, z) ? voxel_ids.ptr()[get_index(x, y, z)] : 0; }
int VoxelBody3D::get_voxel_durability(int x, int y, int z) const {
	if (!is_valid_coord(x, y, z)) return 0;
	int idx = get_index(x, y, z);
	if (!is_sleeping && voxel_durability.size() == voxel_ids.size()) return voxel_durability.ptr()[idx];
	uint8_t id = voxel_ids.ptr()[idx];
	return (id != 0) ? PixelDatabase::get_pixel_fast(id).max_durability : 0;
}
PackedByteArray VoxelBody3D::get_voxel_ids_buffer() const { return voxel_ids; }

int VoxelBody3D::get_surface_y(int x, int z) const {
	if (x < 0 || x >= size_x || z < 0 || z >= size_z) return 14;
	const uint8_t* ids_r = voxel_ids.ptr();
	for (int y = MIN(size_y - 1, max_filled_y); y >= 0; --y) {
		uint8_t id = ids_r[get_index(x, y, z)];
		if (id == 51 || id == 1 || id == 7 || id == 8 || id == 17 || id == 56 || id == 36) return y;
	}
	return 14;
}

bool VoxelBody3D::check_cylinder_collision(Vector3 world_feet_pos, float radius, float height) const {
	Vector3 local_pos = (world_feet_pos - get_global_position()) / voxel_scale;
	int min_x = MAX(0, (int)std::floor(local_pos.x - radius / voxel_scale));
	int max_x = MIN(size_x - 1, (int)std::floor(local_pos.x + radius / voxel_scale));
	int min_z = MAX(0, (int)std::floor(local_pos.z - radius / voxel_scale));
	int max_z = MIN(size_z - 1, (int)std::floor(local_pos.z + radius / voxel_scale));
	int min_y = MAX(0, (int)std::floor(local_pos.y + 0.35f / voxel_scale));
	int max_y = MIN(MIN(size_y - 1, max_filled_y), (int)std::floor(local_pos.y + height / voxel_scale));
	float r_vox_sq = (radius / voxel_scale) * (radius / voxel_scale);
	const uint8_t* ids_r = voxel_ids.ptr();
	for (int z = min_z; z <= max_z; ++z) {
		float dz = (z + 0.5f) - local_pos.z;
		for (int x = min_x; x <= max_x; ++x) {
			float dx = (x + 0.5f) - local_pos.x;
			if (dx * dx + dz * dz > r_vox_sq) continue;
			for (int y = min_y; y <= max_y; ++y) {
				uint8_t pid = ids_r[get_index(x, y, z)];
				if (pid != 0 && pid != 52 && pid != 54 && pid != 55) return true;
			}
		}
	}
	return false;
}

float VoxelBody3D::get_ground_y_near(float world_x, float current_feet_y, float world_z) const {
	Vector3 gp = get_global_position();
	int lx = CLAMP((int)std::floor((world_x - gp.x) / voxel_scale), 0, size_x - 1);
	int lz = CLAMP((int)std::floor((world_z - gp.z) / voxel_scale), 0, size_z - 1);
	int start_vy = CLAMP((int)std::floor((current_feet_y + 0.35f - gp.y) / voxel_scale), 0, MIN(size_y - 1, max_filled_y));
	const uint8_t* ids_r = voxel_ids.ptr();
	for (int y = start_vy; y >= 0; --y) {
		uint8_t pid = ids_r[get_index(lx, y, lz)];
		if (pid != 0 && pid != 35 && pid != 52 && pid != 54 && pid != 55) return (float)(y + 1) * voxel_scale;
	}
	return (float)(get_surface_y(lx, lz) + 1) * voxel_scale;
}

void VoxelBody3D::place_giant_oak_in_sector(int gx_min, int gz_min, int tree_gx, int tree_gz, int tree_base_y, int seed) {
	if (tree_gx + 55 < gx_min || tree_gx - 55 >= gx_min + size_x || tree_gz + 55 < gz_min || tree_gz - 55 >= gz_min + size_z) return;
	auto draw_leaf_cluster = [&](float cx, float cy, float cz, float rad) {
		int ir = (int)std::ceil(rad);
		for (int dz = -ir; dz <= ir; ++dz) {
			int gz = (int)cz + dz; if (gz < gz_min || gz >= gz_min + size_z) continue;
			for (int dy = -ir; dy <= ir; ++dy) {
				int gy = (int)cy + dy; if (gy < 0 || gy >= size_y) continue;
				for (int dx = -ir; dx <= ir; ++dx) {
					int gx = (int)cx + dx; if (gx < gx_min || gx >= gx_min + size_x) continue;
					float d2 = (dx*dx + dy*dy*1.4f + dz*dz) / (rad*rad);
					if (d2 <= 1.0f && fast_hash(gx, gy, gz, seed + 77) > (d2 * 0.45f)) {
						int lx = gx - gx_min, lz = gz - gz_min;
						if (get_voxel(lx, gy, lz) == 0) set_voxel(lx, gy, lz, 35);
					}
				}
			}
		}
	};
	float rot_off = fast_hash(tree_gx, 9, tree_gz, seed) * 6.2831f;
	for (int r = 0; r < 5; ++r) {
		float ang = rot_off + r * 1.2566f + fast_hash(r, tree_gx, tree_gz, seed) * 0.3f, dx = std::cos(ang), dz = std::sin(ang);
		for (int step = 3; step < 16; ++step) {
			int rx = (int)(tree_gx + dx * step), rz = (int)(tree_gz + dz * step), ry = (int)(tree_base_y + 2.5f - step * 0.25f), rad = (step < 8) ? 2 : 1;
			for (int oz = -rad; oz <= rad; ++oz)
				for (int oy = -rad; oy <= rad; ++oy)
					for (int ox = -rad; ox <= rad; ++ox)
						if (ox*ox + oy*oy + oz*oz <= rad*rad) set_voxel(rx + ox - gx_min, ry + oy, rz + oz - gz_min, 36);
		}
	}
	int trunk_h = 58 + (int)(fast_hash(tree_gx, 3, tree_gz, seed) * 16.0f);
	for (int h = 0; h < trunk_h; ++h) {
		float cx = tree_gx + std::sin(h * 0.05f + rot_off) * 1.1f, cz = tree_gz + std::cos(h * 0.05f + rot_off) * 1.1f;
		int cy = tree_base_y + h;
		float rad = 6.0f - (float)h / trunk_h * 2.4f + (h < 6 ? (6 - h) * 0.3f : 0.0f);
		int ir = (int)std::ceil(rad);
		for (int dz = -ir; dz <= ir; ++dz)
			for (int dx = -ir; dx <= ir; ++dx) {
				float d = std::sqrt((float)(dx*dx + dz*dz));
				if (d <= rad - 1.3f) set_voxel((int)cx + dx - gx_min, cy, (int)cz + dz - gz_min, 33);
				else if (d <= rad) set_voxel((int)cx + dx - gx_min, cy, (int)cz + dz - gz_min, 34);
			}
	}
	for (int b = 0; b < 9; ++b) {
		int start_h = (trunk_h - 26) + (b * 3), b_len = 20 + (b % 3) * 4;
		float ang = rot_off + b * 2.39996f, dir_x = std::cos(ang), dir_z = std::sin(ang), pitch = 0.32f + (b % 3) * 0.11f;
		float bx = (float)tree_gx, by = (float)(tree_base_y + start_h), bz = (float)tree_gz;
		for (int step = 1; step <= b_len; ++step) {
			bx += dir_x; bz += dir_z; by += pitch;
			int brad = (step < 8) ? 2 : 1;
			for (int oz = -brad; oz <= brad; ++oz)
				for (int oy = -brad; oy <= brad; ++oy)
					for (int ox = -brad; ox <= brad; ++ox)
						if (ox*ox + oy*oy + oz*oz <= brad*brad)
							set_voxel((int)bx + ox - gx_min, (int)by + oy, (int)bz + oz - gz_min, (brad == 2 && ox==0 && oy==0 && oz==0) ? 33 : 34);
			if (step == 11 || step == b_len - 4) {
				for (int side = -1; side <= 1; side += 2) {
					float sang = ang + side * 0.75f, sx = bx, sy = by, sz = bz;
					for (int ss = 1; ss <= 10; ++ss) {
						sx += std::cos(sang); sz += std::sin(sang); sy += 0.25f;
						set_voxel((int)sx - gx_min, (int)sy, (int)sz - gz_min, 33);
						set_voxel((int)sx - gx_min, (int)sy + 1, (int)sz - gz_min, 34);
					}
					draw_leaf_cluster(sx, sy + 2.0f, sz, 6.5f);
				}
			}
		}
		draw_leaf_cluster(bx, by + 2.5f, bz, 8.0f);
		set_voxel((int)bx - gx_min, (int)by - 2, (int)bz - gz_min, 37);
		set_voxel((int)bx - gx_min, (int)by - 3, (int)bz - gz_min, 38);
	}
	draw_leaf_cluster((float)tree_gx, (float)(tree_base_y + trunk_h + 6), (float)tree_gz, 10.5f);
}

void VoxelBody3D::generate_biome1_sector(int chunk_x, int chunk_z, int world_seed) {
	voxel_ids.fill(0); max_filled_y = 0;
	min_terrain_y = 999; max_terrain_y = 0;
	high_min_x = size_x; high_max_x = -1; high_min_z = size_z; high_max_z = -1;
	is_terrain_chunk = true; ores_generated = false; cached_world_seed = world_seed;
	int gx_min = chunk_x * size_x, gz_min = chunk_z * size_z;
	chunk_gx_origin = gx_min; chunk_gz_origin = gz_min;
	uint8_t* ids_w = voxel_ids.ptrw();

	for (int z = 0; z < size_z; ++z) {
		int gz = gz_min + z;
		for (int x = 0; x < size_x; ++x) {
			int gx = gx_min + x, surf_y = get_global_terrain_y(gx, gz, world_seed);
			if (surf_y < min_terrain_y) min_terrain_y = surf_y;
			if (surf_y > max_terrain_y) max_terrain_y = surf_y;
			if (surf_y > max_filled_y) max_filled_y = surf_y;
			for (int y = 0; y < surf_y - 3; ++y) ids_w[get_index(x, y, z)] = 17;
			for (int y = MAX(0, surf_y - 3); y < surf_y; ++y) ids_w[get_index(x, y, z)] = 1;
			ids_w[get_index(x, surf_y, z)] = 51;
			float fl = fast_hash(gx, surf_y, gz, world_seed + 99);
			if (fl < 0.018f) {
				int gh = 1 + (int)(fast_hash(gx, 1, gz, world_seed) * 2.0f);
				for (int h = 1; h <= gh; ++h) set_voxel(x, surf_y + h, z, 52);
			} else if (fl > 0.996f) {
				set_voxel(x, surf_y + 1, z, 55); set_voxel(x, surf_y + 2, z, 54);
			}
		}
	}
	const int CELL = 104;
	int center_cx = floor_div_int(gx_min + size_x / 2, CELL), center_cz = floor_div_int(gz_min + size_z / 2, CELL);
	for (int tcz = center_cz - 1; tcz <= center_cz + 1; ++tcz) {
		for (int tcx = center_cx - 1; tcx <= center_cx + 1; ++tcx) {
			int cell_seed = world_seed + ((tcx * 73856093) ^ (tcz * 19349663));
			if ((tcx == 0 && tcz == 0) || fast_hash(tcx, 11, tcz, world_seed) < 0.72f) {
				int tgx = tcx * CELL + 28 + (int)(fast_hash(tcx, 23, tcz, world_seed) * 48.0f);
				int tgz = tcz * CELL + 28 + (int)(fast_hash(tcx, 37, tcz, world_seed) * 48.0f);
				if (tcx == 0 && tcz == 0) { tgx = 42; tgz = 42; }
				place_giant_oak_in_sector(gx_min, gz_min, tgx, tgz, get_global_terrain_y(tgx, tgz, world_seed), cell_seed);
			}
			if (fast_hash(tcx, 59, tcz, world_seed) < 0.42f) {
				int rgx = tcx * CELL + 12 + (int)(fast_hash(tcx, 61, tcz, world_seed) * 14.0f);
				int rgz = tcz * CELL + 76 + (int)(fast_hash(tcx, 67, tcz, world_seed) * 14.0f);
				if (rgx + 10 >= gx_min && rgx - 10 < gx_min + size_x && rgz + 10 >= gz_min && rgz - 10 < gz_min + size_z) {
					int ry_base = get_global_terrain_y(rgx, rgz, world_seed);
					for (int dz = -7; dz <= 7; ++dz)
						for (int dy = -3; dy <= 7; ++dy)
							for (int dx = -7; dx <= 7; ++dx)
								if ((dx*dx)/45.0f + (dy*dy)/28.0f + (dz*dz)/42.0f <= 1.0f)
									set_voxel(rgx + dx - gx_min, ry_base + dy, rgz + dz - gz_min, (dy > 3) ? 56 : 17);
				}
			}
		}
	}
	rebuild_mesh(); sleep_sector();
}

Dictionary VoxelBody3D::raycast_voxel(Vector3 world_origin, Vector3 world_dir, float max_dist) const {
	Dictionary res; res["hit"] = false;
	Vector3 dir = world_dir.normalized();
	if (dir.length_squared() < 0.0001f) return res;
	Vector3 local_start = (world_origin - get_global_position()) / voxel_scale;
	int max_steps = (int)((max_dist / voxel_scale) * 2.2f);
	Vector3 pos = local_start, step = dir * 0.42f;
	Vector3i prev_voxel((int)std::floor(pos.x), (int)std::floor(pos.y), (int)std::floor(pos.z));
	const uint8_t* ids_r = voxel_ids.ptr();
	for (int i = 0; i < max_steps; ++i) {
		int vx = (int)std::floor(pos.x), vy = (int)std::floor(pos.y), vz = (int)std::floor(pos.z);
		if (is_valid_coord(vx, vy, vz)) {
			uint8_t id = ids_r[get_index(vx, vy, vz)];
			if (id != 0) {
				res["hit"] = true; res["voxel_pos"] = Vector3i(vx, vy, vz); res["prev_pos"] = prev_voxel;
				res["pixel_id"] = (int)id; res["durability"] = get_voxel_durability(vx, vy, vz);
				res["distance"] = (pos - local_start).length() * voxel_scale;
				return res;
			}
			prev_voxel = Vector3i(vx, vy, vz);
		}
		pos += step;
	}
	return res;
}

void VoxelBody3D::recalculate_mass_and_center() {
	const uint8_t* ids_r = voxel_ids.ptr();
	total_mass_pm = 0; double sx = 0.0, sy = 0.0, sz = 0.0;
	int y_lim = MIN(size_y - 1, max_filled_y);
	for (int z = 0; z < size_z; ++z)
		for (int y = 0; y <= y_lim; ++y)
			for (int x = 0; x < size_x; ++x) {
				uint8_t id = ids_r[get_index(x, y, z)];
				if (id != 0) {
					int32_t pm = PixelDatabase::get_pixel_fast(id).mass_pm;
					total_mass_pm += pm; sx += (double)x * pm; sy += (double)y * pm; sz += (double)z * pm;
				}
			}
	center_of_mass = (total_mass_pm > 0) ? Vector3((float)(sx/total_mass_pm)*voxel_scale, (float)(sy/total_mass_pm)*voxel_scale, (float)(sz/total_mass_pm)*voxel_scale) : Vector3(0,0,0);
}

void VoxelBody3D::rebuild_mesh() {
	ensure_render_resources(); voxel_mesh->clear_surfaces();
	PackedVector3Array vertices, normals; PackedColorArray colors;
	const uint8_t* ids_r = voxel_ids.ptr();
	bool has_dur = (!is_sleeping && voxel_durability.size() == voxel_ids.size());
	const int32_t* dur_r = has_dur ? voxel_durability.ptr() : nullptr;
	const float s = voxel_scale; int y_lim = MIN(size_y - 1, max_filled_y);
	int y_solid_bottom = (is_terrain_chunk && is_sleeping) ? MAX(1, min_terrain_y - 1) : 0;
	int surf_band_top = MIN(y_lim, max_terrain_y + 2);

	auto get_key = [&](int x, int y, int z, int idx) -> uint16_t {
		uint8_t id = ids_r[idx]; if (id == 0) return 0;
		int hp_b = 4;
		if (has_dur) {
			int32_t md = PixelDatabase::get_pixel_fast(id).max_durability;
			hp_b = (md > 0) ? CLAMP((dur_r[idx] * 4) / md, 1, 4) : 4;
		}
		int grain = (id == 34 || id == 33) ? ((int)(fast_hash(x, y / 3, z, 11) * 3.0f)) : ((int)(fast_hash(x, y, z, 17) * 3.0f));
		if (y + 1 < size_y && (ids_r[get_index(MIN(size_x-1,x+1), y+1, z)] != 0 || ids_r[get_index(x, y+1, MIN(size_z-1,z+1))] != 0)) grain = MAX(0, grain - 1);
		if (id == 35 && y + 2 < size_y && ids_r[get_index(x, y + 2, z)] != 0) grain = MAX(0, grain - 1);
		uint16_t shade = (uint16_t)CLAMP(hp_b * 2 + grain - 2, 1, 7);
		return ((uint16_t)id << 3) | shade;
	};
	auto key_to_color = [&](uint16_t key, float dir_light) -> Color {
		const PixelProperties& p = PixelDatabase::get_pixel_fast((uint8_t)(key >> 3));
		float r = (0.46f + 0.54f * ((float)(key & 7) / 7.0f)) * dir_light;
		return Color(p.r * r, p.g * r, p.b * r, 1.0f);
	};
	auto add_quad = [&](const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& norm, const Color& col) {
		vertices.push_back(v0); vertices.push_back(v1); vertices.push_back(v2);
		vertices.push_back(v0); vertices.push_back(v2); vertices.push_back(v3);
		for (int i = 0; i < 6; ++i) { normals.push_back(norm); colors.push_back(col); }
	};

	std::vector<uint16_t> mask_xz(size_x * size_z, 0);
	for (int dir = 0; dir < 2; ++dir) {
		int dy = (dir == 0) ? 1 : -1; float dl = (dir == 0) ? 0.96f : 0.45f;
		for (int y = y_solid_bottom; y <= y_lim; ++y) {
			int x0_b = 0, x1_b = size_x - 1, z0_b = 0, z1_b = size_z - 1;
			if (is_terrain_chunk && is_sleeping && y > surf_band_top) {
				if (high_max_x < high_min_x) break;
				x0_b = MAX(0, high_min_x); x1_b = MIN(size_x - 1, high_max_x);
				z0_b = MAX(0, high_min_z); z1_b = MIN(size_z - 1, high_max_z);
			}
			bool any = false;
			for (int z = z0_b; z <= z1_b; ++z)
				for (int x = x0_b; x <= x1_b; ++x) {
					int idx = get_index(x, y, z); uint16_t k = 0;
					if (ids_r[idx] != 0 && (y + dy < 0 || y + dy >= size_y || ids_r[get_index(x, y + dy, z)] == 0)) { k = get_key(x, y, z, idx); any = true; }
					mask_xz[x + z * size_x] = k;
				}
			if (!any) continue;
			for (int z = z0_b; z <= z1_b; ++z)
				for (int x = x0_b; x <= x1_b; ++x) {
					uint16_t k = mask_xz[x + z * size_x]; if (k == 0) continue;
					int w = 1; while (x + w <= x1_b && mask_xz[(x + w) + z * size_x] == k) w++;
					int h = 1;
					while (z + h <= z1_b) {
						bool ok = true;
						for (int kx = 0; kx < w; ++kx) if (mask_xz[(x + kx) + (z + h) * size_x] != k) { ok = false; break; }
						if (!ok) break;
						h++;
					}
					for (int dz = 0; dz < h; ++dz) for (int dx = 0; dx < w; ++dx) mask_xz[(x + dx) + (z + dz) * size_x] = 0;
					float x0 = x * s, x1 = (x + w) * s, z0 = z * s, z1 = (z + h) * s, yf = (dir == 0 ? (y + 1) : y) * s;
					Color col = key_to_color(k, dl);
					if (dir == 0) add_quad(Vector3(x0, yf, z1), Vector3(x0, yf, z0), Vector3(x1, yf, z0), Vector3(x1, yf, z1), Vector3(0, 1, 0), col);
					else add_quad(Vector3(x0, yf, z0), Vector3(x0, yf, z1), Vector3(x1, yf, z1), Vector3(x1, yf, z0), Vector3(0, -1, 0), col);
				}
		}
	}

	int y_count = y_lim + 1;
	std::vector<uint16_t> mask_xy(size_x * y_count, 0);
	for (int dir = 0; dir < 2; ++dir) {
		int dz_s = (dir == 0) ? 1 : -1; float dl = (dir == 0) ? 0.85f : 0.66f;
		for (int z = 0; z < size_z; ++z) {
			bool any = false, at_border = (z + dz_s < 0 || z + dz_s >= size_z);
			int z_ylim = (is_terrain_chunk && is_sleeping && (z < high_min_z - 1 || z > high_max_z + 1)) ? surf_band_top : y_lim;
			for (int x = 0; x < size_x; ++x) {
				int neighbor_surf_y = (is_terrain_chunk && at_border) ? get_global_terrain_y(chunk_gx_origin + x, chunk_gz_origin + z + dz_s, cached_world_seed) : -1;
				for (int y = y_solid_bottom; y <= z_ylim; ++y) {
					int idx = get_index(x, y, z); uint16_t k = 0;
					if (ids_r[idx] != 0) {
						bool exposed = at_border ? (y > neighbor_surf_y) : (ids_r[get_index(x, y, z + dz_s)] == 0);
						if (exposed) { k = get_key(x, y, z, idx); any = true; }
					}
					mask_xy[x + y * size_x] = k;
				}
			}
			if (!any) continue;
			for (int y = y_solid_bottom; y <= z_ylim; ++y)
				for (int x = 0; x < size_x; ++x) {
					uint16_t k = mask_xy[x + y * size_x]; if (k == 0) continue;
					int w = 1; while (x + w < size_x && mask_xy[(x + w) + y * size_x] == k) w++;
					int h = 1;
					while (y + h <= z_ylim) {
						bool ok = true;
						for (int kx = 0; kx < w; ++kx) if (mask_xy[(x + kx) + (y + h) * size_x] != k) { ok = false; break; }
						if (!ok) break;
						h++;
					}
					for (int dy = 0; dy < h; ++dy) for (int dx = 0; dx < w; ++dx) mask_xy[(x + dx) + (y + dy) * size_x] = 0;
					float x0 = x * s, x1 = (x + w) * s, y0 = y * s, y1 = (y + h) * s, zf = (dir == 0 ? (z + 1) : z) * s;
					Color col = key_to_color(k, dl);
					if (dir == 0) add_quad(Vector3(x0, y0, zf), Vector3(x0, y1, zf), Vector3(x1, y1, zf), Vector3(x1, y0, zf), Vector3(0, 0, 1), col);
					else add_quad(Vector3(x1, y0, zf), Vector3(x1, y1, zf), Vector3(x0, y1, zf), Vector3(x0, y0, zf), Vector3(0, 0, -1), col);
				}
		}
	}

	std::vector<uint16_t> mask_zy(size_z * y_count, 0);
	for (int dir = 0; dir < 2; ++dir) {
		int dx_s = (dir == 0) ? 1 : -1; float dl = (dir == 0) ? 0.78f : 0.72f;
		for (int x = 0; x < size_x; ++x) {
			bool any = false, at_border = (x + dx_s < 0 || x + dx_s >= size_x);
			int x_ylim = (is_terrain_chunk && is_sleeping && (x < high_min_x - 1 || x > high_max_x + 1)) ? surf_band_top : y_lim;
			for (int z = 0; z < size_z; ++z) {
				int neighbor_surf_y = (is_terrain_chunk && at_border) ? get_global_terrain_y(chunk_gx_origin + x + dx_s, chunk_gz_origin + z, cached_world_seed) : -1;
				for (int y = y_solid_bottom; y <= x_ylim; ++y) {
					int idx = get_index(x, y, z); uint16_t k = 0;
					if (ids_r[idx] != 0) {
						bool exposed = at_border ? (y > neighbor_surf_y) : (ids_r[get_index(x + dx_s, y, z)] == 0);
						if (exposed) { k = get_key(x, y, z, idx); any = true; }
					}
					mask_zy[z + y * size_z] = k;
				}
			}
			if (!any) continue;
			for (int y = y_solid_bottom; y <= x_ylim; ++y)
				for (int z = 0; z < size_z; ++z) {
					uint16_t k = mask_zy[z + y * size_z]; if (k == 0) continue;
					int w = 1; while (z + w < size_z && mask_zy[(z + w) + y * size_z] == k) w++;
					int h = 1;
					while (y + h <= x_ylim) {
						bool ok = true;
						for (int kz = 0; kz < w; ++kz) if (mask_zy[(z + kz) + (y + h) * size_z] != k) { ok = false; break; }
						if (!ok) break;
						h++;
					}
					for (int dy = 0; dy < h; ++dy) for (int dz = 0; dz < w; ++dz) mask_zy[(z + dz) + (y + dy) * size_z] = 0;
					float z0 = z * s, z1 = (z + w) * s, y0 = y * s, y1 = (y + h) * s, xf = (dir == 0 ? (x + 1) : x) * s;
					Color col = key_to_color(k, dl);
					if (dir == 0) add_quad(Vector3(xf, y0, z1), Vector3(xf, y1, z1), Vector3(xf, y1, z0), Vector3(xf, y0, z0), Vector3(1, 0, 0), col);
					else add_quad(Vector3(xf, y0, z0), Vector3(xf, y1, z0), Vector3(xf, y1, z1), Vector3(xf, y0, z1), Vector3(-1, 0, 0), col);
				}
		}
	}

	if (vertices.size() > 0) {
		Array sa; sa.resize(Mesh::ARRAY_MAX);
		sa[Mesh::ARRAY_VERTEX] = vertices; sa[Mesh::ARRAY_NORMAL] = normals; sa[Mesh::ARRAY_COLOR] = colors;
		voxel_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, sa);
	}
}

Dictionary VoxelBody3D::apply_impact_damage(Vector3i hit_pos, int contact_radius, int attacker_pixel_id, float impact_speed, float attacker_mass_pm, float k_forma, float damage_scale, int light_level) {
	wake_up();
	Dictionary result; uint8_t* ids_w = voxel_ids.ptrw(); int32_t* dur_w = voxel_durability.ptrw();
	float attacker_hardness = 10.0f; int32_t attacker_max_dur = 180;
	if (attacker_pixel_id > 0 && attacker_pixel_id <= 130) {
		const PixelProperties& ap = PixelDatabase::get_pixel_fast((uint8_t)attacker_pixel_id);
		attacker_hardness = (float)ap.hardness; attacker_max_dur = ap.max_durability;
		if (attacker_pixel_id == 31) {
			if (light_level <= 0) attacker_hardness = 50.0f;
			else if (light_level == 1) attacker_hardness = 100.0f;
			else if (light_level == 2) attacker_hardness = 500.0f;
			else attacker_hardness = 650.0f;
		}
	}
	float m_speed = (impact_speed < 50.0f) ? MAX(0.05f, 1.0f - (50.0f - impact_speed) * 0.02f) : (1.0f + (impact_speed - 50.0f) * 0.005f);
	float m_mass = 1.0f + attacker_mass_pm * 0.005f;
	float d_nyers = (attacker_hardness * m_speed * m_mass * damage_scale) + kint_stored_damage;
	if (kint_stored_damage > 0.0f) { kint_stored_damage = 0.0f; kint_timer = 0.0f; }

	auto is_exposed_to_air = [&](int x, int y, int z) -> bool {
		const int dx[6] = {1,-1,0,0,0,0}, dy[6] = {0,0,1,-1,0,0}, dz[6] = {0,0,0,0,1,-1};
		for (int d = 0; d < 6; ++d) {
			int nx = x + dx[d], ny = y + dy[d], nz = z + dz[d];
			if (!is_valid_coord(nx, ny, nz) || ids_w[get_index(nx, ny, nz)] == 0) return true;
		}
		return false;
	};

	std::vector<int> contacted; int r = MAX(0, contact_radius);
	for (int dz = -r; dz <= r; ++dz)
		for (int dy = -r; dy <= r; ++dy)
			for (int dx = -r; dx <= r; ++dx)
				if (dx*dx + dy*dy + dz*dz <= r*r) {
					int vx = hit_pos.x + dx, vy = hit_pos.y + dy, vz = hit_pos.z + dz;
					if (is_valid_coord(vx, vy, vz)) {
						int idx = get_index(vx, vy, vz);
						if (ids_w[idx] != 0 && is_exposed_to_air(vx, vy, vz)) contacted.push_back(idx);
					}
				}
	int n_kontakt = MAX(1, (int)contacted.size());
	float e_pixel = d_nyers / (1.0f + (float)(n_kontakt - 1) * k_forma);
	int destroyed_count = 0, damaged_count = 0; float max_recoil = 0.0f;
	bool instakill = false, bleeding = false, jap_severed = false;

	for (size_t i = 0; i < contacted.size(); ++i) {
		int idx = contacted[i]; uint8_t tid = ids_w[idx];
		const PixelProperties& tp = PixelDatabase::get_pixel_fast(tid);
		float hr = (float)tp.hardness / MAX(1.0f, attacker_hardness);
		float recoil = e_pixel * (hr * hr);
		if (recoil > max_recoil) max_recoil = recoil;
		if (e_pixel > (float)tp.ak_threshold) {
			dur_w[idx] -= (int32_t)std::round(e_pixel - (float)tp.ak_threshold);
			damaged_count++;
			if (dur_w[idx] <= 0) {
				if (attacker_pixel_id == 29 && tp.max_durability < attacker_max_dur) {
					kint_stored_damage = (float)(attacker_max_dur - tp.max_durability);
					kint_timer = 5.0f; max_recoil = 0.0f;
				}
				if (tid == 68 || tid == 69) instakill = true;
				if (tid == 76) bleeding = true;
				if (tid == 94) jap_severed = true;
				ids_w[idx] = 0; dur_w[idx] = 0; destroyed_count++;
			}
		}
	}
	float kb = (total_mass_pm > 0) ? ((d_nyers * (1.0f + (float)n_kontakt * 0.02f)) / (float)total_mass_pm) : 0.0f;
	if (damaged_count > 0 || destroyed_count > 0) {
		if (destroyed_count > 0) recalculate_mass_and_center();
		rebuild_mesh();
	}
	result["d_nyers"] = d_nyers; result["e_pixel"] = e_pixel; result["n_kontakt"] = n_kontakt;
	result["damaged_count"] = damaged_count; result["destroyed_count"] = destroyed_count;
	result["recoil_damage"] = max_recoil; result["knockback_speed"] = kb;
	result["kint_stored_damage"] = kint_stored_damage; result["instakill"] = instakill;
	result["bleeding"] = bleeding; result["jap_severed"] = jap_severed;
	return result;
}

Dictionary VoxelBody3D::update_kint_timer(float delta) {
	Dictionary res; res["shockwave_discharged"] = false; res["shockwave_power"] = 0.0f;
	if (kint_timer > 0.0f) {
		kint_timer -= delta;
		if (kint_timer <= 0.0f) {
			res["shockwave_discharged"] = true; res["shockwave_power"] = kint_stored_damage;
			kint_stored_damage = 0.0f; kint_timer = 0.0f;
		}
	}
	res["kint_remaining_time"] = kint_timer; res["kint_stored_damage"] = kint_stored_damage;
	return res;
}

Array VoxelBody3D::check_and_extract_disconnected_bodies() {
	wake_up();
	Array detached; int total = size_x * size_y * size_z;
	uint8_t* ids_w = voxel_ids.ptrw(); int32_t* dur_w = voxel_durability.ptrw();
	std::vector<uint8_t> visited(total, 0); std::vector<std::vector<int>> comps;
	const int dx[6] = {1,-1,0,0,0,0}, dy[6] = {0,0,1,-1,0,0}, dz[6] = {0,0,0,0,1,-1};
	for (int i = 0; i < total; ++i) {
		if (ids_w[i] == 0 || visited[i]) continue;
		std::vector<int> cl; std::queue<int> q; q.push(i); visited[i] = 1;
		while (!q.empty()) {
			int c = q.front(); q.pop(); cl.push_back(c);
			int cx = c % size_x, cy = (c / size_x) % size_y, cz = c / (size_x * size_y);
			for (int d = 0; d < 6; ++d) {
				int nx = cx + dx[d], ny = cy + dy[d], nz = cz + dz[d];
				if (is_valid_coord(nx, ny, nz)) {
					int ni = get_index(nx, ny, nz);
					if (ids_w[ni] != 0 && !visited[ni]) { visited[ni] = 1; q.push(ni); }
				}
			}
		}
		comps.push_back(cl);
	}
	if (comps.size() > 1) {
		size_t max_i = 0;
		for (size_t i = 1; i < comps.size(); ++i) if (comps[i].size() > comps[max_i].size()) max_i = i;
		for (size_t i = 0; i < comps.size(); ++i) {
			if (i == max_i) continue;
			PackedInt32Array c_ids;
			for (int idx : comps[i]) {
				c_ids.push_back(idx % size_x); c_ids.push_back((idx / size_x) % size_y);
				c_ids.push_back(idx / (size_x * size_y)); c_ids.push_back(ids_w[idx]);
				ids_w[idx] = 0; dur_w[idx] = 0;
			}
			detached.push_back(c_ids);
		}
		recalculate_mass_and_center(); rebuild_mesh();
	}
	return detached;
}

void VoxelBody3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize_grid", "size_x", "size_y", "size_z", "voxel_scale"), &VoxelBody3D::initialize_grid, DEFVAL(0.1f));
	ClassDB::bind_method(D_METHOD("wake_up"), &VoxelBody3D::wake_up);
	ClassDB::bind_method(D_METHOD("sleep_sector"), &VoxelBody3D::sleep_sector);
	ClassDB::bind_method(D_METHOD("is_sector_sleeping"), &VoxelBody3D::is_sector_sleeping);
	ClassDB::bind_method(D_METHOD("fill_block", "pixel_id"), &VoxelBody3D::fill_block);
	ClassDB::bind_method(D_METHOD("fill_box", "min_pos", "max_pos", "pixel_id"), &VoxelBody3D::fill_box);
	ClassDB::bind_method(D_METHOD("set_voxel", "x", "y", "z", "pixel_id"), &VoxelBody3D::set_voxel);
	ClassDB::bind_method(D_METHOD("get_voxel", "x", "y", "z"), &VoxelBody3D::get_voxel);
	ClassDB::bind_method(D_METHOD("get_voxel_durability", "x", "y", "z"), &VoxelBody3D::get_voxel_durability);
	ClassDB::bind_method(D_METHOD("get_surface_y", "x", "z"), &VoxelBody3D::get_surface_y);
	ClassDB::bind_method(D_METHOD("check_cylinder_collision", "world_feet_pos", "radius", "height"), &VoxelBody3D::check_cylinder_collision);
	ClassDB::bind_method(D_METHOD("get_ground_y_near", "world_x", "current_feet_y", "world_z"), &VoxelBody3D::get_ground_y_near);
	ClassDB::bind_method(D_METHOD("generate_biome1_sector", "chunk_x", "chunk_z", "world_seed"), &VoxelBody3D::generate_biome1_sector);
	ClassDB::bind_method(D_METHOD("raycast_voxel", "world_origin", "world_dir", "max_dist"), &VoxelBody3D::raycast_voxel);
	ClassDB::bind_method(D_METHOD("rebuild_mesh"), &VoxelBody3D::rebuild_mesh);
	ClassDB::bind_method(D_METHOD("get_voxel_ids_buffer"), &VoxelBody3D::get_voxel_ids_buffer);
	ClassDB::bind_method(D_METHOD("recalculate_mass_and_center"), &VoxelBody3D::recalculate_mass_and_center);
	ClassDB::bind_method(D_METHOD("apply_impact_damage", "hit_pos", "contact_radius", "attacker_pixel_id", "impact_speed", "attacker_mass_pm", "k_forma", "damage_scale", "light_level"), &VoxelBody3D::apply_impact_damage);
	ClassDB::bind_method(D_METHOD("update_kint_timer", "delta"), &VoxelBody3D::update_kint_timer);
	ClassDB::bind_method(D_METHOD("check_and_extract_disconnected_bodies"), &VoxelBody3D::check_and_extract_disconnected_bodies);
	ClassDB::bind_method(D_METHOD("get_total_mass_pm"), &VoxelBody3D::get_total_mass_pm);
	ClassDB::bind_method(D_METHOD("get_total_mass_kg"), &VoxelBody3D::get_total_mass_kg);
	ClassDB::bind_method(D_METHOD("get_center_of_mass"), &VoxelBody3D::get_center_of_mass);
	ClassDB::bind_method(D_METHOD("get_grid_size"), &VoxelBody3D::get_grid_size);
}
