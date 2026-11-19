#include "example_class.h"

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/base_material3d.hpp>
#include <cmath>
#include <queue>

using namespace godot;

// ============================================================================
// 1. PIXEL DATABASE IMPLEMENTÁCIÓ (MIND A 130 SZILÁRD PIXEL - 26. FEJEZET)
// ============================================================================
PixelProperties PixelDatabase::pixels[131];
bool PixelDatabase::initialized = false;

PixelDatabase::PixelDatabase() {
	init_database();
}

PixelDatabase::~PixelDatabase() {}

void PixelDatabase::init_database() {
	if (initialized) return;

	// ID, Név, Tier, ÁK, Dur, Kem, Rug, Pat, Pm, GAP, AP_max, Hővez, Gyull, Olv, Forr, Aku, Elekt, Csúsz, R, G, B
	pixels[0] = {0, "Levego", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -9999, -9999, 0, 0, 100, 0.0f, 0.0f, 0.0f};

	// I. TALAJOK, FÖLDEK, HÓ ÉS FELSZÍNI RÉTEGEK (1–16)
	pixels[1]  = {1,  "Termekeny Fold-pixel (Humusz)", 0, 5, 12, 4, 5, 1, 18, 10, 6, 12, 250, -9999, -9999, 8, 5, 10, 0.36f, 0.25f, 0.16f};
	pixels[2]  = {2,  "Magas Humusztartalmu Fold-pixel", 0, 6, 15, 5, 6, 1, 19, 12, 8, 13, 280, -9999, -9999, 9, 6, 14, 0.22f, 0.16f, 0.11f};
	pixels[3]  = {3,  "Szurkesfekete Mocsari Fold-pixel", 0, 7, 18, 6, 8, 1, 22, 14, 9, 16, -1, -9999, -9999, 6, 10, 25, 0.20f, 0.21f, 0.20f};
	pixels[4]  = {4,  "Vizes Iszap-pixel", 0, 5, 16, 4, 12, 2, 25, 12, 7, 20, -1, -9999, -9999, 4, 18, 86, 0.28f, 0.26f, 0.22f};
	pixels[5]  = {5,  "Kiszaradt Iszap-pixel", 0, 9, 22, 12, 3, 1, 20, 18, 12, 14, -1, -9999, -9999, 14, 1, 15, 0.45f, 0.41f, 0.35f};
	pixels[6]  = {6,  "Sar-pixel", 0, 8, 20, 7, 10, 2, 26, 16, 10, 22, -1, -9999, -9999, 5, 15, 82, 0.32f, 0.22f, 0.14f};
	pixels[7]  = {7,  "Nyers Agyag-pixel", 0, 10, 28, 9, 20, 3, 28, 22, 15, 25, -1, 1100, 2200, 7, 12, 75, 0.62f, 0.46f, 0.38f};
	pixels[8]  = {8,  "Soder-pixel", 0, 14, 32, 18, 1, 5, 30, 8, 5, 19, -1, 1300, 2400, 24, 0, 34, 0.55f, 0.54f, 0.52f};
	pixels[9]  = {9,  "Tozeg-pixel", 0, 5, 16, 4, 10, 3, 12, 12, 8, 8, 180, -9999, -9999, 6, 1, 8, 0.24f, 0.18f, 0.12f};
	pixels[10] = {10, "Szaraz Avar-pixel", 0, 1, 3, 1, 15, 5, 3, 2, 1, 4, 120, -9999, -9999, 45, 0, 35, 0.58f, 0.38f, 0.16f};
	pixels[11] = {11, "Ho-pixel", 0, 4, 12, 5, 6, 3, 14, 10, 6, 15, -1, 0, 100, 8, 3, 80, 0.95f, 0.97f, 1.00f};
	pixels[12] = {12, "Sullyedo Ho-pixel", 0, 2, 5, 2, 2, 1, 12, 1, 1, 45, -1, -5, 100, 4, 3, 75, 0.88f, 0.93f, 0.98f};
	pixels[13] = {13, "Csuszos Jeghartya-pixel", 0, 10, 20, 16, 2, 6, 19, 10, 6, 28, -1, 0, 100, 42, 4, 98, 0.72f, 0.88f, 0.98f};
	pixels[14] = {14, "Foszilia-csont pixel", 1, 38, 95, 42, 4, 6, 30, 65, 45, 16, -1, 1600, 2800, 40, 1, 26, 0.82f, 0.79f, 0.70f};
	pixels[15] = {15, "Foszilia-mesz pixel", 1, 32, 80, 35, 2, 5, 32, 55, 38, 22, -1, 1400, 2500, 36, 0, 24, 0.78f, 0.76f, 0.72f};
	pixels[16] = {16, "Denever-guano pixel", 0, 3, 8, 3, 5, 1, 10, 5, 3, 10, 150, -9999, -9999, 5, 8, 45, 0.30f, 0.28f, 0.25f};

	// II. KŐZETEK, ÉRCEK ÉS DRÁGAKÖVEK (17–32)
	pixels[17] = {17, "Ko-pixel (Sima Meszko)", 1, 35, 90, 38, 2, 5, 35, 60, 42, 25, -1, 1350, 2500, 36, 0, 22, 0.50f, 0.50f, 0.52f};
	pixels[18] = {18, "Repedezett Ko-pixel", 0, 4, 6, 28, 0, 2, 32, 1, 1, 24, -1, 1350, 2500, 50, 0, 20, 0.44f, 0.44f, 0.46f};
	pixels[19] = {19, "Eros Ko-pixel (Belso Hegykozet)", 2, 75, 210, 72, 3, 7, 42, 140, 100, 28, -1, 1500, 2700, 46, 0, 22, 0.36f, 0.37f, 0.40f};
	pixels[20] = {20, "Andezit-pixel", 1, 48, 125, 50, 2, 6, 42, 85, 60, 27, -1, 1450, 2600, 46, 0, 22, 0.42f, 0.43f, 0.45f};
	pixels[21] = {21, "Diorit-pixel", 1, 52, 135, 54, 2, 6, 44, 92, 65, 30, -1, 1500, 2700, 50, 0, 25, 0.72f, 0.72f, 0.74f};
	pixels[22] = {22, "Marvany-pixel", 1, 44, 115, 45, 3, 7, 39, 78, 55, 30, -1, 1400, 2550, 44, 0, 52, 0.92f, 0.91f, 0.94f};
	pixels[23] = {23, "Gubbancsko-pixel", 2, 65, 160, 60, 10, 300, 38, 70, 50, 95, -1, 1600, 2800, 80, 0, 24, 0.85f, 0.35f, 0.15f};
	pixels[24] = {24, "Szen-pixel (Koszen)", 1, 24, 60, 26, 2, 3, 26, 40, 28, 18, 350, -9999, -9999, 28, 14, 24, 0.14f, 0.14f, 0.15f};
	pixels[25] = {25, "Vas-pixel (Vaserc / Nyersvas)", 2, 70, 200, 68, 8, 8, 50, 150, 110, 58, -1, 1538, 2860, 48, 55, 26, 0.68f, 0.62f, 0.58f};
	pixels[26] = {26, "Rez-pixel (Rezerc / Vorosrez)", 2, 55, 160, 52, 28, 10, 54, 115, 80, 85, -1, 1085, 2560, 44, 92, 34, 0.82f, 0.45f, 0.25f};
	pixels[27] = {27, "Foldalatti Vasfa-pixel", 2, 68, 190, 65, 16, 9, 40, 140, 100, 18, 1200, -9999, -9999, 40, 8, 22, 0.32f, 0.33f, 0.36f};
	pixels[28] = {28, "Kvarc-pixel", 2, 60, 150, 75, 0, 8, 37, 95, 68, 34, -1, 1670, 2950, 65, 4, 48, 0.94f, 0.92f, 0.96f};
	pixels[29] = {29, "Kint-pixel (Kinetikus Erc)", 3, 140, 380, 130, 22, 15, 40, 280, 210, 35, -1, 1850, 3200, 45, 60, 32, 0.25f, 0.78f, 0.95f};
	pixels[30] = {30, "Smaragd-pixel", 4, 200, 520, 190, 0, 9, 34, 340, 250, 36, -1, 2100, 3500, 72, 0, 58, 0.12f, 0.82f, 0.35f};
	pixels[31] = {31, "Loncore-pixel (Feny-Erc)", 4, 250, 650, 100, 20, 14, 45, 480, 360, 60, -1, 2400, 3800, 75, 75, 35, 0.98f, 0.88f, 0.32f};
	pixels[32] = {32, "Gyemant-pixel", 5, 450, 1200, 400, 5, 18, 40, 850, 680, 100, 4000, 3550, 4800, 98, 0, 75, 0.55f, 0.95f, 1.00f};

	// III. FÁK, GYÖKEREK, FÜVEK, GOMBÁK ÉS NÖVÉNYI PIXELEK (33–67)
	pixels[33] = {33, "Tolgy-kemenyfa pixel", 1, 32, 85, 30, 16, 8, 22, 75, 55, 11, 280, -9999, -9999, 34, 0, 20, 0.55f, 0.40f, 0.24f};
	pixels[34] = {34, "Tolgyfakereg-pixel", 0, 12, 25, 11, 10, 3, 12, 20, 14, 8, 240, -9999, -9999, 16, 0, 18, 0.35f, 0.25f, 0.15f};
	pixels[35] = {35, "Tolgyfalevel-pixel", 0, 2, 6, 2, 20, 8, 4, 5, 3, 5, 160, -9999, -9999, 4, 0, 30, 0.20f, 0.55f, 0.18f};
	pixels[36] = {36, "Tolgy-gyoker pixel", 1, 28, 75, 26, 24, 8, 20, 80, 60, 10, 300, -9999, -9999, 25, 0, 18, 0.42f, 0.30f, 0.18f};
	pixels[37] = {37, "Makk-hej pixel", 0, 14, 28, 18, 5, 6, 16, 15, 10, 10, 220, -9999, -9999, 22, 0, 30, 0.48f, 0.32f, 0.16f};
	pixels[38] = {38, "Makk-belso pixel (Mag-pixel)", 0, 5, 10, 6, 8, 4, 12, 6, 4, 8, 200, -9999, -9999, 8, 0, 25, 0.78f, 0.68f, 0.42f};
	pixels[39] = {39, "Felszini Vasfa-torzs pixel", 2, 65, 185, 62, 18, 8, 38, 135, 95, 15, 1100, -9999, -9999, 38, 5, 22, 0.38f, 0.39f, 0.42f};
	pixels[40] = {40, "Vasfa-level pixel", 1, 18, 40, 22, 25, 10, 12, 25, 18, 20, 600, -9999, -9999, 30, 10, 35, 0.50f, 0.54f, 0.56f};
	pixels[41] = {41, "Mangrove-fa pixel", 1, 30, 80, 28, 20, 8, 24, 70, 50, 12, 320, -9999, -9999, 30, 0, 22, 0.45f, 0.28f, 0.20f};
	pixels[42] = {42, "Mangrove-gyoker pixel", 1, 34, 90, 30, 26, 9, 25, 85, 62, 12, 340, -9999, -9999, 28, 0, 24, 0.38f, 0.24f, 0.16f};
	pixels[43] = {43, "Mangrove-level pixel", 0, 3, 8, 3, 22, 8, 5, 6, 4, 6, 180, -9999, -9999, 5, 0, 38, 0.16f, 0.42f, 0.20f};
	pixels[44] = {44, "Fenyo-puhafa pixel", 1, 20, 55, 18, 20, 8, 14, 45, 32, 8, 220, -9999, -9999, 22, 0, 22, 0.68f, 0.54f, 0.36f};
	pixels[45] = {45, "Fenyokereg-pixel", 0, 9, 20, 9, 12, 4, 10, 16, 11, 7, 190, -9999, -9999, 14, 0, 20, 0.42f, 0.26f, 0.18f};
	pixels[46] = {46, "Fenyotu-pixel", 0, 2, 5, 2, 16, 6, 4, 4, 2, 5, 110, -9999, -9999, 4, 0, 54, 0.14f, 0.38f, 0.22f};
	pixels[47] = {47, "Havas Fenyotu-pixel", 0, 4, 9, 4, 12, 4, 8, 6, 4, 12, 300, 0, 100, 6, 2, 70, 0.75f, 0.88f, 0.82f};
	pixels[48] = {48, "Fenyotoboz-pixel", 0, 8, 16, 10, 8, 5, 9, 8, 5, 8, 170, -9999, -9999, 14, 0, 26, 0.40f, 0.28f, 0.16f};
	pixels[49] = {49, "Fenyogyanta-pixel", 0, 8, 18, 9, 12, 3, 12, 10, 6, 8, 130, 80, 200, 14, 0, 40, 0.92f, 0.68f, 0.12f};
	pixels[50] = {50, "Korhadtfa- es Holtfa-pixel", 0, 9, 20, 8, 6, 3, 10, 14, 9, 7, 180, -9999, -9999, 12, 0, 20, 0.32f, 0.28f, 0.22f};
	pixels[51] = {51, "Zold Fu-pixel", 0, 2, 5, 1, 25, 8, 4, 4, 2, 6, 160, -9999, -9999, 3, 2, 40, 0.28f, 0.68f, 0.22f};
	pixels[52] = {52, "Magas Fu-pixel", 0, 3, 8, 2, 28, 8, 5, 5, 3, 6, 150, -9999, -9999, 4, 2, 38, 0.24f, 0.62f, 0.20f};
	pixels[53] = {53, "Szaraz Fu-pixel", 0, 2, 4, 1, 20, 6, 3, 3, 2, 5, 90, -9999, -9999, 6, 0, 42, 0.72f, 0.64f, 0.28f};
	pixels[54] = {54, "Viragszirom-pixel", 0, 1, 3, 1, 22, 6, 2, 2, 1, 4, 140, -9999, -9999, 2, 0, 35, 0.95f, 0.35f, 0.55f};
	pixels[55] = {55, "Viragszar-pixel", 0, 4, 9, 3, 30, 8, 4, 6, 4, 5, 160, -9999, -9999, 4, 2, 30, 0.30f, 0.65f, 0.25f};
	pixels[56] = {56, "Moha-pixel", 0, 3, 10, 2, 35, 12, 14, 8, 5, 10, 220, -9999, -9999, 3, 6, 45, 0.22f, 0.52f, 0.18f};
	pixels[57] = {57, "Logo Inda-rost pixel", 1, 16, 45, 8, 50, 12, 9, 35, 25, 5, 190, -9999, -9999, 5, 2, 20, 0.26f, 0.48f, 0.20f};
	pixels[58] = {58, "Tuskes Inda-tovis pixel", 1, 22, 50, 28, 10, 5, 14, 28, 20, 8, 200, -9999, -9999, 18, 0, 15, 0.35f, 0.22f, 0.16f};
	pixels[59] = {59, "Spark bogyo-hus pixel", 1, 5, 12, 4, 18, 6, 8, 6, 4, 30, 80, -9999, -9999, 6, 25, 35, 1.00f, 0.65f, 0.10f};
	pixels[60] = {60, "Spark bogyo-mag pixel", 1, 10, 18, 16, 2, 10, 12, 8, 5, 60, 20, -9999, -9999, 20, 40, 25, 1.00f, 0.90f, 0.20f};
	pixels[61] = {61, "Mushspoon-kanalfej es Szar-pixel", 0, 8, 22, 6, 28, 10, 8, 12, 8, 10, 240, -9999, -9999, 5, 4, 15, 0.65f, 0.55f, 0.42f};
	pixels[62] = {62, "Galoca-pixel", 0, 3, 9, 2, 20, 8, 6, 5, 3, 8, 210, -9999, -9999, 4, 2, 30, 0.90f, 0.15f, 0.15f};
	pixels[63] = {63, "Puffatag-burok pixel", 0, 2, 5, 2, 35, 18, 4, 4, 2, 5, 150, -9999, -9999, 8, 0, 25, 0.72f, 0.68f, 0.55f};
	pixels[64] = {64, "Szarvaspatagomba-pixel", 1, 28, 70, 26, 12, 6, 18, 45, 32, 9, 260, -9999, -9999, 20, 0, 20, 0.42f, 0.34f, 0.26f};
	pixels[65] = {65, "Nyujtott es Kalapos gomba-pixel", 0, 4, 12, 3, 22, 9, 7, 6, 4, 8, 220, -9999, -9999, 4, 2, 28, 0.58f, 0.52f, 0.44f};
	pixels[66] = {66, "Parazita-noveny Test-pixel", 2, 35, 110, 25, 55, 14, 16, 48, 35, 12, 250, -9999, -9999, 8, 10, 20, 0.45f, 0.18f, 0.25f};
	pixels[67] = {67, "Parazita-noveny Fog-pixel", 2, 55, 140, 60, 8, 8, 24, 60, 42, 14, -1, -9999, -9999, 35, 2, 25, 0.92f, 0.88f, 0.82f};

	// IV. ÁLLATI ÉS KARAKTER BIOLÓGIAI SZILÁRD PIXELEK (68–130)
	pixels[68]  = {68,  "Sziv-pixel (Kritikus)", 1, 12, 180, 10, 40, 10, 20, 18, 12, 25, -1, -9999, -9999, 6, 30, 40, 0.85f, 0.08f, 0.12f};
	pixels[69]  = {69,  "Agy-pixel (Kritikus)", 1, 8, 180, 6, 30, 5, 18, 12, 8, 22, -1, -9999, -9999, 4, 45, 50, 0.92f, 0.65f, 0.72f};
	pixels[70]  = {70,  "Tudo-pixel (Szerv)", 1, 8, 120, 5, 50, 15, 10, 12, 8, 18, -1, -9999, -9999, 5, 20, 35, 0.88f, 0.48f, 0.52f};
	pixels[71]  = {71,  "Kopoltyu-pixel (Szerv)", 1, 6, 90, 5, 35, 8, 12, 10, 6, 22, -1, -9999, -9999, 4, 25, 45, 0.78f, 0.32f, 0.38f};
	pixels[72]  = {72,  "Gyomor-pixel (Szerv)", 1, 14, 150, 12, 55, 12, 18, 22, 15, 20, -1, -9999, -9999, 6, 20, 30, 0.72f, 0.45f, 0.40f};
	pixels[73]  = {73,  "Maj-pixel (Szerv)", 1, 10, 140, 8, 25, 5, 22, 15, 10, 24, -1, -9999, -9999, 5, 25, 40, 0.48f, 0.18f, 0.16f};
	pixels[74]  = {74,  "Vese-pixel (Szerv)", 1, 10, 130, 8, 25, 6, 19, 15, 10, 22, -1, -9999, -9999, 5, 25, 38, 0.55f, 0.22f, 0.20f};
	pixels[75]  = {75,  "Bel-pixel (Szerv)", 1, 12, 140, 9, 65, 15, 15, 28, 20, 18, -1, -9999, -9999, 5, 18, 45, 0.82f, 0.58f, 0.55f};
	pixels[76]  = {76,  "Er-pixel (Verer-pixel)", 1, 10, 100, 7, 45, 10, 16, 15, 10, 28, -1, -9999, -9999, 5, 35, 35, 0.95f, 0.05f, 0.08f};
	pixels[77]  = {77,  "Szem-pixel (Erzekszerv)", 0, 4, 40, 4, 25, 8, 15, 6, 4, 20, -1, -9999, -9999, 4, 25, 70, 0.95f, 0.95f, 0.98f};
	pixels[78]  = {78,  "Ful-pixel (Erzekszerv)", 1, 9, 90, 8, 38, 10, 14, 14, 9, 18, -1, -9999, -9999, 95, 20, 30, 0.85f, 0.68f, 0.58f};
	pixels[79]  = {79,  "Szaglo-pixel (Orr-pixel)", 1, 9, 95, 8, 32, 8, 15, 14, 9, 20, -1, -9999, -9999, 6, 22, 35, 0.82f, 0.62f, 0.54f};
	pixels[80]  = {80,  "Nyelv-pixel (Szerv)", 1, 14, 150, 12, 50, 10, 20, 20, 14, 22, -1, -9999, -9999, 6, 25, 40, 0.85f, 0.35f, 0.42f};
	pixels[81]  = {81,  "Tarantula-meregzacsko pixel", 1, 12, 85, 9, 35, 8, 16, 14, 9, 18, -1, -9999, -9999, 5, 20, 35, 0.45f, 0.78f, 0.20f};
	pixels[82]  = {82,  "Skorpio-meregzacsko pixel", 1, 14, 95, 11, 30, 6, 17, 16, 11, 18, -1, -9999, -9999, 6, 20, 35, 0.55f, 0.85f, 0.15f};
	pixels[83]  = {83,  "Pok-szovomirigy pixel", 1, 12, 90, 9, 45, 12, 15, 16, 11, 16, 220, -9999, -9999, 5, 10, 25, 0.88f, 0.88f, 0.92f};
	pixels[84]  = {84,  "Voros Hus-pixel (Izom)", 1, 15, 180, 12, 40, 8, 22, 25, 18, 22, -1, -9999, -9999, 6, 25, 30, 0.78f, 0.22f, 0.22f};
	pixels[85]  = {85,  "Feher Halhus-pixel", 0, 7, 75, 6, 28, 5, 18, 10, 6, 20, -1, -9999, -9999, 4, 22, 40, 0.92f, 0.85f, 0.82f};
	pixels[86]  = {86,  "Fagyhal-hus pixel", 1, 12, 105, 10, 30, 6, 20, 16, 11, 10, -1, -9999, -9999, 5, 20, 45, 0.75f, 0.88f, 0.95f};
	pixels[87]  = {87,  "Hullo- es Keteltuhus-pixel", 1, 12, 115, 10, 35, 8, 20, 18, 12, 18, -1, -9999, -9999, 5, 20, 35, 0.65f, 0.75f, 0.58f};
	pixels[88]  = {88,  "Rakhus-pixel", 0, 7, 70, 6, 22, 5, 17, 9, 6, 18, -1, -9999, -9999, 4, 20, 35, 0.95f, 0.78f, 0.72f};
	pixels[89]  = {89,  "Vekony Csont-pixel", 1, 22, 55, 26, 10, 6, 16, 35, 24, 12, -1, 1500, 2700, 32, 1, 28, 0.90f, 0.88f, 0.82f};
	pixels[90]  = {90,  "Normal Csont-pixel", 1, 35, 90, 38, 8, 8, 26, 60, 42, 14, -1, 1600, 2800, 38, 1, 28, 0.92f, 0.90f, 0.84f};
	pixels[91]  = {91,  "Nehez Oriascsont-pixel", 2, 58, 165, 60, 6, 7, 36, 110, 80, 16, -1, 1700, 2900, 42, 1, 26, 0.86f, 0.84f, 0.76f};
	pixels[92]  = {92,  "Halszalka- es Porc-pixel", 0, 12, 35, 15, 35, 10, 12, 20, 14, 12, -1, -9999, -9999, 12, 2, 35, 0.85f, 0.85f, 0.90f};
	pixels[93]  = {93,  "Repulohal-uszony pixel", 1, 10, 40, 9, 65, 20, 6, 25, 18, 10, -1, -9999, -9999, 8, 5, 40, 0.60f, 0.82f, 0.95f};
	pixels[94]  = {94,  "In-pixel (JAP-izulet)", 1, 30, 110, 22, 75, 22, 18, 75, 55, 12, -1, -9999, -9999, 10, 8, 25, 0.88f, 0.78f, 0.72f};
	pixels[95]  = {95,  "Zsir-pixel (Faggyu)", 0, 5, 15, 4, 18, 2, 14, 6, 4, 8, 160, 45, 220, 3, 0, 85, 0.96f, 0.94f, 0.82f};
	pixels[96]  = {96,  "Vekonybor-pixel", 1, 14, 40, 10, 45, 10, 12, 18, 12, 5, 240, -9999, -9999, 4, 2, 28, 0.72f, 0.56f, 0.44f};
	pixels[97]  = {97,  "Szarvasbor-pixel (Velur)", 1, 18, 50, 12, 50, 11, 13, 24, 16, 5, 250, -9999, -9999, 4, 0, 16, 0.62f, 0.44f, 0.30f};
	pixels[98]  = {98,  "Vastagbor-pixel", 1, 24, 65, 16, 42, 10, 16, 32, 22, 6, 280, -9999, -9999, 6, 2, 24, 0.52f, 0.36f, 0.24f};
	pixels[99]  = {99,  "Medve- es Borzbor-pixel", 2, 35, 95, 24, 38, 9, 20, 45, 32, 7, 300, -9999, -9999, 7, 0, 20, 0.38f, 0.26f, 0.18f};
	pixels[100] = {100, "Deneverszarny-hartya pixel", 0, 8, 22, 5, 55, 14, 5, 12, 8, 6, 210, -9999, -9999, 3, 2, 30, 0.28f, 0.24f, 0.28f};
	pixels[101] = {101, "Halpikkely- es Harcsabor-pixel", 0, 10, 28, 12, 28, 6, 14, 14, 9, 15, -1, -9999, -9999, 8, 10, 80, 0.48f, 0.56f, 0.54f};
	pixels[102] = {102, "Lepenyhal-alcabor pixel", 0, 10, 28, 11, 30, 6, 15, 14, 9, 16, -1, -9999, -9999, 5, 10, 75, 0.34f, 0.32f, 0.26f};
	pixels[103] = {103, "Gote-nyalkabor pixel", 0, 6, 18, 5, 40, 8, 12, 8, 5, 18, -1, -9999, -9999, 3, 15, 92, 0.25f, 0.45f, 0.35f};
	pixels[104] = {104, "Gyik- es Leguanpikkely-pixel", 1, 26, 68, 25, 28, 8, 16, 35, 24, 10, 320, -9999, -9999, 14, 0, 32, 0.32f, 0.52f, 0.28f};
	pixels[105] = {105, "Tapado Gekkotalp-pixel", 1, 10, 32, 8, 50, 10, 10, 45, 32, 8, 230, -9999, -9999, 3, 0, 0, 0.45f, 0.68f, 0.38f};
	pixels[106] = {106, "Lo- es Szamarpata-pixel", 1, 34, 82, 35, 14, 10, 24, 50, 36, 10, 350, -9999, -9999, 32, 0, 25, 0.24f, 0.22f, 0.20f};
	pixels[107] = {107, "Szeles Mocsari Pata-pixel", 1, 28, 70, 28, 20, 10, 18, 55, 40, 10, 340, -9999, -9999, 24, 0, 15, 0.28f, 0.26f, 0.22f};
	pixels[108] = {108, "Tapado Sziklapata-pixel", 1, 36, 88, 36, 24, 12, 22, 65, 48, 10, 350, -9999, -9999, 28, 0, 2, 0.22f, 0.22f, 0.24f};
	pixels[109] = {109, "Sima Szaru-szarv pixel", 1, 32, 78, 34, 16, 10, 22, 48, 34, 10, 350, -9999, -9999, 30, 0, 34, 0.35f, 0.32f, 0.28f};
	pixels[110] = {110, "Csavart Rackaszaru-pixel", 1, 38, 92, 40, 14, 10, 24, 56, 40, 10, 360, -9999, -9999, 34, 0, 32, 0.42f, 0.38f, 0.32f};
	pixels[111] = {111, "Tomor Muflonszaru-pixel", 2, 62, 175, 66, 12, 14, 34, 115, 85, 12, 400, -9999, -9999, 40, 0, 28, 0.48f, 0.42f, 0.34f};
	pixels[112] = {112, "Agancs-pixel", 1, 40, 95, 42, 10, 8, 25, 62, 44, 12, -1, 1600, 2800, 36, 0, 28, 0.65f, 0.58f, 0.48f};
	pixels[113] = {113, "Hodfog-pixel", 1, 48, 120, 55, 6, 6, 28, 70, 50, 18, -1, 1600, 2800, 42, 5, 24, 0.90f, 0.52f, 0.15f};
	pixels[114] = {114, "Ragadozo Tepofog-pixel", 2, 52, 135, 58, 6, 6, 27, 75, 54, 14, -1, 1650, 2850, 40, 0, 26, 0.95f, 0.94f, 0.88f};
	pixels[115] = {115, "Ragadozo Karom-pixel", 2, 50, 130, 56, 10, 8, 24, 72, 52, 12, 380, -9999, -9999, 36, 0, 28, 0.25f, 0.24f, 0.22f};
	pixels[116] = {116, "Borz Asokarom-pixel", 1, 42, 105, 48, 12, 7, 22, 60, 42, 12, 360, -9999, -9999, 32, 0, 25, 0.32f, 0.30f, 0.28f};
	pixels[117] = {117, "Madarcsor- es Madarkarom-pixel", 1, 36, 85, 44, 12, 8, 18, 50, 36, 10, 340, -9999, -9999, 34, 0, 30, 0.88f, 0.72f, 0.18f};
	pixels[118] = {118, "Rakollo- es Skorpiofullank-pixel", 2, 46, 120, 52, 10, 8, 20, 65, 46, 10, 350, -9999, -9999, 34, 0, 32, 0.30f, 0.22f, 0.20f};
	pixels[119] = {119, "Vizallo Hodprem-pixel", 1, 8, 28, 4, 45, 12, 6, 12, 8, 3, 220, -9999, -9999, 2, 0, 45, 0.42f, 0.28f, 0.18f};
	pixels[120] = {120, "Sertesszor- es Rovid Szor-pixel", 0, 3, 10, 2, 35, 10, 4, 5, 3, 4, 190, -9999, -9999, 3, 0, 25, 0.68f, 0.56f, 0.48f};
	pixels[121] = {121, "Vastag Ragadozoprem-pixel", 1, 12, 38, 6, 48, 14, 8, 16, 11, 2, 210, -9999, -9999, 2, 0, 22, 0.36f, 0.26f, 0.18f};
	pixels[122] = {122, "Sima Gyapju-pixel", 1, 5, 20, 2, 50, 15, 5, 8, 5, 2, 200, -9999, -9999, 1, 0, 18, 0.94f, 0.93f, 0.90f};
	pixels[123] = {123, "Gondor Rackagyapju-pixel", 1, 9, 32, 4, 55, 18, 7, 14, 9, 1, 230, -9999, -9999, 1, 0, 16, 0.86f, 0.84f, 0.78f};
	pixels[124] = {124, "Alaptoll- es Pehelytoll-pixel", 0, 1, 4, 1, 30, 15, 1, 2, 1, 2, 160, -9999, -9999, 2, 0, 38, 0.96f, 0.96f, 0.96f};
	pixels[125] = {125, "Diszes Facantoll-pixel", 0, 2, 7, 2, 35, 16, 1, 4, 2, 2, 170, -9999, -9999, 3, 0, 40, 0.85f, 0.42f, 0.16f};
	pixels[126] = {126, "Fekete Varju- es Hollotoll-pixel", 1, 3, 10, 3, 38, 16, 2, 6, 4, 3, 180, -9999, -9999, 3, 0, 42, 0.12f, 0.12f, 0.14f};
	pixels[127] = {127, "Ragadozo Evezotoll-pixel", 1, 6, 18, 5, 45, 20, 2, 12, 8, 3, 200, -9999, -9999, 4, 0, 45, 0.38f, 0.30f, 0.24f};
	pixels[128] = {128, "Kitin-pixel", 2, 44, 115, 48, 16, 12, 16, 60, 42, 9, 320, -9999, -9999, 28, 0, 42, 0.26f, 0.22f, 0.20f};
	pixels[129] = {129, "Szoros Tarantula-kitin pixel", 2, 40, 105, 42, 20, 10, 15, 55, 38, 8, 280, -9999, -9999, 15, 0, 25, 0.18f, 0.15f, 0.14f};
	pixels[130] = {130, "Tojashej- es Tojasbelso-pixel", 0, 8, 12, 14, 2, 4, 12, 10, 6, 14, -1, -9999, -9999, 22, 2, 35, 0.96f, 0.92f, 0.82f};

	initialized = true;
}

const PixelProperties& PixelDatabase::get_pixel_fast(uint8_t p_id) {
	if (!initialized) init_database();
	if (p_id > 130) return pixels[0];
	return pixels[p_id];
}

Dictionary PixelDatabase::get_pixel_info(int p_id) const {
	const PixelProperties& p = get_pixel_fast((uint8_t)CLAMP(p_id, 0, 130));
	Dictionary d;
	d["id"] = (int)p.id;
	d["name"] = String(p.name);
	d["tier"] = (int)p.tier;
	d["ak_threshold"] = p.ak_threshold;
	d["max_durability"] = p.max_durability;
	d["hardness"] = p.hardness;
	d["elasticity"] = p.elasticity;
	d["bounciness"] = p.bounciness;
	d["mass_pm"] = p.mass_pm;
	d["gap_resistance"] = p.gap_resistance;
	d["ap_max"] = p.ap_max;
	d["thermal_cond"] = p.thermal_cond;
	d["ignition_point"] = p.ignition_point;
	d["melting_point"] = p.melting_point;
	d["boiling_point"] = p.boiling_point;
	d["acoustic_vib"] = p.acoustic_vib;
	d["electric_cond"] = p.electric_cond;
	d["slipperiness"] = p.slipperiness;
	d["color"] = Color(p.r, p.g, p.b, 1.0f);
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
	// Minden pixelhez 4 float (R, G, B, MaxDurability) = 16 bájt (std430 GPU igazítás a Compute Shadernek)
	PackedByteArray buf;
	buf.resize(131 * 16);
	float* ptr = reinterpret_cast<float*>(buf.ptrw());
	for (int i = 0; i <= 130; ++i) {
		const PixelProperties& p = get_pixel_fast((uint8_t)i);
		ptr[i * 4 + 0] = p.r;
		ptr[i * 4 + 1] = p.g;
		ptr[i * 4 + 2] = p.b;
		ptr[i * 4 + 3] = (float)p.max_durability;
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

// ============================================================================
// 2. VOXEL BODY 3D IMPLEMENTÁCIÓ (50x50x50 BLOKK, FIZIKA ÉS 60 FPS MESHER)
// ============================================================================
VoxelBody3D::VoxelBody3D() {
	PixelDatabase::init_database();
	size_x = 50;
	size_y = 50;
	size_z = 50;
	voxel_scale = 0.1f;
	is_static_body = true;
	total_mass_pm = 0;
	center_of_mass = Vector3(0, 0, 0);
	kint_stored_damage = 0.0f;
	kint_timer = 0.0f;

	voxel_mesh.instantiate();
	set_mesh(voxel_mesh);

	voxel_material.instantiate();
	voxel_material->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
	voxel_material->set_roughness(0.85f);
	set_material_override(voxel_material);

	initialize_grid(50, 50, 50, 0.1f);
}

VoxelBody3D::~VoxelBody3D() {}

void VoxelBody3D::initialize_grid(int p_x, int p_y, int p_z, float p_voxel_scale) {
	size_x = CLAMP(p_x, 1, 128);
	size_y = CLAMP(p_y, 1, 128);
	size_z = CLAMP(p_z, 1, 128);
	voxel_scale = p_voxel_scale;

	int total = size_x * size_y * size_z;
	voxel_ids.resize(total);
	voxel_ids.fill(0);
	voxel_durability.resize(total);
	voxel_durability.fill(0);
	voxel_heat.resize(total);
	voxel_heat.fill(20); // 20 °C szobahőmérséklet
}

void VoxelBody3D::fill_block(int p_pixel_id) {
	uint8_t id = (uint8_t)CLAMP(p_pixel_id, 0, 130);
	int32_t max_dur = PixelDatabase::get_pixel_fast(id).max_durability;
	voxel_ids.fill(id);
	voxel_durability.fill(max_dur);
	recalculate_mass_and_center();
	rebuild_mesh();
}

void VoxelBody3D::fill_box(Vector3i p_min, Vector3i p_max, int p_pixel_id) {
	uint8_t id = (uint8_t)CLAMP(p_pixel_id, 0, 130);
	int32_t max_dur = PixelDatabase::get_pixel_fast(id).max_durability;
	uint8_t* ids_w = voxel_ids.ptrw();
	int32_t* dur_w = voxel_durability.ptrw();

	int x0 = CLAMP(p_min.x, 0, size_x - 1), x1 = CLAMP(p_max.x, 0, size_x - 1);
	int y0 = CLAMP(p_min.y, 0, size_y - 1), y1 = CLAMP(p_max.y, 0, size_y - 1);
	int z0 = CLAMP(p_min.z, 0, size_z - 1), z1 = CLAMP(p_max.z, 0, size_z - 1);

	for (int z = z0; z <= z1; ++z) {
		for (int y = y0; y <= y1; ++y) {
			for (int x = x0; x <= x1; ++x) {
				int idx = get_index(x, y, z);
				ids_w[idx] = id;
				dur_w[idx] = max_dur;
			}
		}
	}
	recalculate_mass_and_center();
	rebuild_mesh();
}

void VoxelBody3D::set_voxel(int x, int y, int z, int p_pixel_id) {
	if (!is_valid_coord(x, y, z)) return;
	uint8_t id = (uint8_t)CLAMP(p_pixel_id, 0, 130);
	int idx = get_index(x, y, z);
	voxel_ids.ptrw()[idx] = id;
	voxel_durability.ptrw()[idx] = PixelDatabase::get_pixel_fast(id).max_durability;
}

int VoxelBody3D::get_voxel(int x, int y, int z) const {
	if (!is_valid_coord(x, y, z)) return 0;
	return voxel_ids.ptr()[get_index(x, y, z)];
}

int VoxelBody3D::get_voxel_durability(int x, int y, int z) const {
	if (!is_valid_coord(x, y, z)) return 0;
	return voxel_durability.ptr()[get_index(x, y, z)];
}

PackedByteArray VoxelBody3D::get_voxel_ids_buffer() const {
	return voxel_ids;
}

void VoxelBody3D::recalculate_mass_and_center() {
	const uint8_t* ids_r = voxel_ids.ptr();
	total_mass_pm = 0;
	double sum_x = 0.0, sum_y = 0.0, sum_z = 0.0;

	for (int z = 0; z < size_z; ++z) {
		for (int y = 0; y < size_y; ++y) {
			for (int x = 0; x < size_x; ++x) {
				uint8_t id = ids_r[get_index(x, y, z)];
				if (id != 0) {
					int32_t pm = PixelDatabase::get_pixel_fast(id).mass_pm;
					total_mass_pm += pm;
					sum_x += (double)x * pm;
					sum_y += (double)y * pm;
					sum_z += (double)z * pm;
				}
			}
		}
	}

	if (total_mass_pm > 0) {
		center_of_mass = Vector3(
			(float)(sum_x / total_mass_pm) * voxel_scale,
			(float)(sum_y / total_mass_pm) * voxel_scale,
			(float)(sum_z / total_mass_pm) * voxel_scale
		);
	} else {
		center_of_mass = Vector3(0, 0, 0);
	}
}

// 60+ FPS C++ Face-Culling Mesher (Csak a levegővel érintkező látható oldalakat rajzolja ki!)
void VoxelBody3D::rebuild_mesh() {
	if (voxel_mesh.is_null()) return;
	voxel_mesh->clear_surfaces();

	PackedVector3Array vertices;
	PackedVector3Array normals;
	PackedColorArray colors;

	const uint8_t* ids_r = voxel_ids.ptr();
	const int32_t* dur_r = voxel_durability.ptr();
	const float s = voxel_scale;

	auto add_quad = [&](const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& norm, const Color& col) {
		vertices.push_back(v0); vertices.push_back(v1); vertices.push_back(v2);
		vertices.push_back(v0); vertices.push_back(v2); vertices.push_back(v3);
		for (int i = 0; i < 6; ++i) {
			normals.push_back(norm);
			colors.push_back(col);
		}
	};

	for (int z = 0; z < size_z; ++z) {
		for (int y = 0; y < size_y; ++y) {
			for (int x = 0; x < size_x; ++x) {
				int idx = get_index(x, y, z);
				uint8_t id = ids_r[idx];
				if (id == 0) continue;

				const PixelProperties& prop = PixelDatabase::get_pixel_fast(id);
				float hp_ratio = (prop.max_durability > 0) ? CLAMP((float)dur_r[idx] / (float)prop.max_durability, 0.25f, 1.0f) : 1.0f;
				Color col(prop.r * hp_ratio, prop.g * hp_ratio, prop.b * hp_ratio, 1.0f);

				float x0 = x * s, x1 = (x + 1) * s;
				float y0 = y * s, y1 = (y + 1) * s;
				float z0 = z * s, z1 = (z + 1) * s;

				// +X oldal
				if (x + 1 >= size_x || ids_r[get_index(x + 1, y, z)] == 0)
					add_quad(Vector3(x1, y0, z1), Vector3(x1, y1, z1), Vector3(x1, y1, z0), Vector3(x1, y0, z0), Vector3(1, 0, 0), col);
				// -X oldal
				if (x - 1 < 0 || ids_r[get_index(x - 1, y, z)] == 0)
					add_quad(Vector3(x0, y0, z0), Vector3(x0, y1, z0), Vector3(x0, y1, z1), Vector3(x0, y0, z1), Vector3(-1, 0, 0), col);
				// +Y oldal (Teteje)
				if (y + 1 >= size_y || ids_r[get_index(x, y + 1, z)] == 0)
					add_quad(Vector3(x0, y1, z1), Vector3(x0, y1, z0), Vector3(x1, y1, z0), Vector3(x1, y1, z1), Vector3(0, 1, 0), col);
				// -Y oldal (Alja)
				if (y - 1 < 0 || ids_r[get_index(x, y - 1, z)] == 0)
					add_quad(Vector3(x0, y0, z0), Vector3(x0, y0, z1), Vector3(x1, y0, z1), Vector3(x1, y0, z0), Vector3(0, -1, 0), col);
				// +Z oldal (Eleje)
				if (z + 1 >= size_z || ids_r[get_index(x, y, z + 1)] == 0)
					add_quad(Vector3(x0, y0, z1), Vector3(x0, y1, z1), Vector3(x1, y1, z1), Vector3(x1, y0, z1), Vector3(0, 0, 1), col);
				// -Z oldal (Hátulja)
				if (z - 1 < 0 || ids_r[get_index(x, y, z - 1)] == 0)
					add_quad(Vector3(x1, y0, z0), Vector3(x1, y1, z0), Vector3(x0, y1, z0), Vector3(x0, y0, z0), Vector3(0, 0, -1), col);
			}
		}
	}

	if (vertices.size() > 0) {
		Array surface_array;
		surface_array.resize(Mesh::ARRAY_MAX);
		surface_array[Mesh::ARRAY_VERTEX] = vertices;
		surface_array[Mesh::ARRAY_NORMAL] = normals;
		surface_array[Mesh::ARRAY_COLOR] = colors;
		voxel_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, surface_array);
	}
}

// 15. FEJEZET: BECSAPÓDÁSI FIZIKA, FELÜLETI ERŐ, NEWTON III., KINT ÉS LONCORE ÉRC
Dictionary VoxelBody3D::apply_impact_damage(
	Vector3i hit_pos,
	int contact_radius,
	int attacker_pixel_id,
	float impact_speed,
	float attacker_mass_pm,
	float k_forma,
	float damage_scale,
	int light_level
) {
	Dictionary result;
	uint8_t* ids_w = voxel_ids.ptrw();
	int32_t* dur_w = voxel_durability.ptrw();

	// 1. Támadó Keménysége (0 = Puszta kezes ököl: alap-keménység 10)
	float attacker_hardness = 10.0f;
	int32_t attacker_max_dur = 180; // Emberi kéz 180 Durability
	if (attacker_pixel_id > 0 && attacker_pixel_id <= 130) {
		const PixelProperties& att_prop = PixelDatabase::get_pixel_fast((uint8_t)attacker_pixel_id);
		attacker_hardness = (float)att_prop.hardness;
		attacker_max_dur = att_prop.max_durability;

		// LONCORE ÉRC (31-es pixel): Fényfüggő sebzés (24.2.4. Fejezet)
		if (attacker_pixel_id == 31) {
			if (light_level <= 0) attacker_hardness = 50.0f;       // Sötétben: 50
			else if (light_level == 1) attacker_hardness = 100.0f; // Alap fényben: 100
			else if (light_level == 2) attacker_hardness = 500.0f; // Nagy fényben: 500
			else attacker_hardness = 650.0f;                       // Direkt fényben: 650!
		}
	}

	// 2. Sebességszorzó (M_speed) és Tömegszorzó (M_tömeg) - 15.1. Fejezet
	float m_speed = 1.0f;
	if (impact_speed < 50.0f) {
		m_speed = MAX(0.05f, 1.0f - (50.0f - impact_speed) * 0.02f);
	} else {
		m_speed = 1.0f + (impact_speed - 50.0f) * 0.005f;
	}
	float m_mass = 1.0f + attacker_mass_pm * 0.005f;

	// 3. Nyers Becsapódási Erő (D_nyers) + Kint Érc eltárolt bónusz sebzése
	float d_nyers = (attacker_hardness * m_speed * m_mass * damage_scale) + kint_stored_damage;
	if (kint_stored_damage > 0.0f) {
		kint_stored_damage = 0.0f;
		kint_timer = 0.0f;
	}

	// 4. Érintkező pixelek (N_kontakt) összegyűjtése
	std::vector<int> contacted_indices;
	int r = MAX(0, contact_radius);
	for (int dz = -r; dz <= r; ++dz) {
		for (int dy = -r; dy <= r; ++dy) {
			for (int dx = -r; dx <= r; ++dx) {
				if (dx * dx + dy * dy + dz * dz <= r * r) {
					int vx = hit_pos.x + dx, vy = hit_pos.y + dy, vz = hit_pos.z + dz;
					if (is_valid_coord(vx, vy, vz)) {
						int idx = get_index(vx, vy, vz);
						if (ids_w[idx] != 0) contacted_indices.push_back(idx);
					}
				}
			}
		}
	}

	int n_kontakt = MAX(1, (int)contacted_indices.size());
	// 5. Felületi Erőeloszlási Képlet (15.3. Fejezet)
	float e_pixel = d_nyers / (1.0f + (float)(n_kontakt - 1) * k_forma);

	int destroyed_count = 0;
	int damaged_count = 0;
	float max_recoil_damage = 0.0f;
	bool instakill_triggered = false;
	bool bleeding_triggered = false;
	bool jap_severed = false;

	for (size_t i = 0; i < contacted_indices.size(); ++i) {
		int idx = contacted_indices[i];
		uint8_t target_id = ids_w[idx];
		const PixelProperties& t_prop = PixelDatabase::get_pixel_fast(target_id);

		// Newton III. törvénye: Visszaható sebzés (15.5. Fejezet)
		float hardness_ratio = (float)t_prop.hardness / MAX(1.0f, attacker_hardness);
		float e_visszahato = e_pixel * (hardness_ratio * hardness_ratio);
		if (e_visszahato > max_recoil_damage) max_recoil_damage = e_visszahato;

		// Áttörési küszöb (ÁK) vizsgálata (15.4. Fejezet)
		if (e_pixel > (float)t_prop.ak_threshold) {
			int32_t final_dmg = (int32_t)std::round(e_pixel - (float)t_prop.ak_threshold);
			dur_w[idx] -= final_dmg;
			damaged_count++;

			if (dur_w[idx] <= 0) {
				// KINT ÉRC (29-es pixel): Kinetikus elnyelés gyengébb anyag átvágásakor!
				if (attacker_pixel_id == 29 && t_prop.max_durability < attacker_max_dur) {
					kint_stored_damage = (float)(attacker_max_dur - t_prop.max_durability);
					kint_timer = 5.0f;
					max_recoil_damage = 0.0f; // A Kint kard NEM sebződik gyengébb anyagon!
				}

				// Kritikus szervek és JAP ellenőrzése (5.2. Fejezet)
				if (target_id == 68 || target_id == 69) instakill_triggered = true; // Szív vagy Agy
				if (target_id == 76) bleeding_triggered = true;                     // Ér-pixel (Exponenciális vérzés)
				if (target_id == 94) jap_severed = true;                            // Ín-pixel (JAP csonkolás)

				ids_w[idx] = 0;
				dur_w[idx] = 0;
				destroyed_count++;
			}
		}
	}

	// Hátralökő Impulzus (Knockback Speed), ha az ütés nem vágta át az anyagot
	float knockback_speed = 0.0f;
	if (total_mass_pm > 0) {
		knockback_speed = (d_nyers * (1.0f + (float)n_kontakt * 0.02f)) / (float)total_mass_pm;
	}

	if (damaged_count > 0 || destroyed_count > 0) {
		if (destroyed_count > 0) recalculate_mass_and_center();
		rebuild_mesh();
	}

	result["d_nyers"] = d_nyers;
	result["e_pixel"] = e_pixel;
	result["n_kontakt"] = n_kontakt;
	result["damaged_count"] = damaged_count;
	result["destroyed_count"] = destroyed_count;
	result["recoil_damage"] = max_recoil_damage;
	result["knockback_speed"] = knockback_speed;
	result["kint_stored_damage"] = kint_stored_damage;
	result["instakill"] = instakill_triggered;
	result["bleeding"] = bleeding_triggered;
	result["jap_severed"] = jap_severed;
	return result;
}

Dictionary VoxelBody3D::update_kint_timer(float delta) {
	Dictionary res;
	res["shockwave_discharged"] = false;
	res["shockwave_power"] = 0.0f;

	if (kint_timer > 0.0f) {
		kint_timer -= delta;
		if (kint_timer <= 0.0f) {
			// 5 mp lejárt ütés nélkül -> Shockwave kisülés a levegőbe! (24.2.1. Fejezet)
			res["shockwave_discharged"] = true;
			res["shockwave_power"] = kint_stored_damage;
			kint_stored_damage = 0.0f;
			kint_timer = 0.0f;
		}
	}
	res["kint_remaining_time"] = kint_timer;
	res["kint_stored_damage"] = kint_stored_damage;
	return res;
}

// 16. FEJEZET: 3D FLOOD-FILL KETTÉSZAKADÁS ÉS CSONKOLÁS VIZSGÁLAT
Array VoxelBody3D::check_and_extract_disconnected_bodies() {
	Array detached_clusters;
	int total = size_x * size_y * size_z;
	uint8_t* ids_w = voxel_ids.ptrw();
	int32_t* dur_w = voxel_durability.ptrw();

	std::vector<uint8_t> visited(total, 0);
	std::vector<std::vector<int>> components;

	const int dx[6] = {1, -1, 0, 0, 0, 0};
	const int dy[6] = {0, 0, 1, -1, 0, 0};
	const int dz[6] = {0, 0, 0, 0, 1, -1};

	for (int i = 0; i < total; ++i) {
		if (ids_w[i] == 0 || visited[i]) continue;

		std::vector<int> cluster;
		std::queue<int> q;
		q.push(i);
		visited[i] = 1;

		while (!q.empty()) {
			int curr = q.front();
			q.pop();
			cluster.push_back(curr);

			int cx = curr % size_x;
			int cy = (curr / size_x) % size_y;
			int cz = curr / (size_x * size_y);

			for (int d = 0; d < 6; ++d) {
				int nx = cx + dx[d], ny = cy + dy[d], nz = cz + dz[d];
				if (is_valid_coord(nx, ny, nz)) {
					int n_idx = get_index(nx, ny, nz);
					if (ids_w[n_idx] != 0 && !visited[n_idx]) {
						visited[n_idx] = 1;
						q.push(n_idx);
					}
				}
			}
		}
		components.push_back(cluster);
	}

	// Ha több mint 1 különálló részre szakadt a test (pl. átvágták a fát vagy a végtagot):
	if (components.size() > 1) {
		size_t largest_idx = 0;
		for (size_t i = 1; i < components.size(); ++i) {
			if (components[i].size() > components[largest_idx].size()) largest_idx = i;
		}

		for (size_t i = 0; i < components.size(); ++i) {
			if (i == largest_idx) continue;

			PackedInt32Array coords_and_ids;
			for (int idx : components[i]) {
				int cx = idx % size_x;
				int cy = (idx / size_x) % size_y;
				int cz = idx / (size_x * size_y);
				coords_and_ids.push_back(cx);
				coords_and_ids.push_back(cy);
				coords_and_ids.push_back(cz);
				coords_and_ids.push_back(ids_w[idx]);

				// Leválasztott voxelek törlése az eredeti testből
				ids_w[idx] = 0;
				dur_w[idx] = 0;
			}
			detached_clusters.push_back(coords_and_ids);
		}
		recalculate_mass_and_center();
		rebuild_mesh();
	}

	return detached_clusters;
}

void VoxelBody3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("initialize_grid", "size_x", "size_y", "size_z", "voxel_scale"), &VoxelBody3D::initialize_grid, DEFVAL(0.1f));
	ClassDB::bind_method(D_METHOD("fill_block", "pixel_id"), &VoxelBody3D::fill_block);
	ClassDB::bind_method(D_METHOD("fill_box", "min_pos", "max_pos", "pixel_id"), &VoxelBody3D::fill_box);
	ClassDB::bind_method(D_METHOD("set_voxel", "x", "y", "z", "pixel_id"), &VoxelBody3D::set_voxel);
	ClassDB::bind_method(D_METHOD("get_voxel", "x", "y", "z"), &VoxelBody3D::get_voxel);
	ClassDB::bind_method(D_METHOD("get_voxel_durability", "x", "y", "z"), &VoxelBody3D::get_voxel_durability);
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
