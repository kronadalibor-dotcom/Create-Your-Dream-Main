#ifndef CYD_VOXEL_ENGINE_H
#define CYD_VOXEL_ENGINE_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <cstdint>
#include <vector>

namespace godot {

// ============================================================================
// 1. A 130 SZILÁRD PIXEL 15+2 TULAJDONSÁGÁNAK C++ STRUKTÚRÁJA (6. és 26. Fejezet)
// ============================================================================
struct PixelProperties {
	uint8_t id;                  // 0 = Levegő, 1..130 = Szilárd pixelek
	const char* name;            // Pixel neve (pl. "Kint-pixel", "Szív-pixel")
	uint8_t tier;                // Tier 0 - Tier 10
	int32_t ak_threshold;        // Áttörési küszöb (HP / ÁK) - 0..10000
	int32_t max_durability;      // Durability / pixel - 0..10000
	int32_t hardness;            // Keménység (Alapsebzés és Newton III.) - 0..10000
	int32_t elasticity;          // Rugalmasság
	int32_t bounciness;          // Pattogósság (Gubbancskő = 300)
	int32_t mass_pm;             // Tömeg (Pixelmass: 1 Pm = 0.5 kg)
	int32_t gap_resistance;      // GAP-ellenállás (Beomlás ellen)
	int32_t ap_max;              // AP_max (Kihajlási védelem 1 pixeles oszlopok ellen)
	int32_t thermal_cond;        // Hővezetés
	int32_t ignition_point;      // Gyulladáspont (°C, -1 ha nem éghető)
	int32_t melting_point;       // Olvadáspont (°C, -9999 ha nem olvad)
	int32_t boiling_point;       // Forráspont (°C, -9999 ha nem forr)
	int32_t acoustic_vib;        // Akusztikai rezgés (0 = Némafa, 45 = Száraz Avar)
	int32_t electric_cond;       // Elektromos vezetés
	int32_t slipperiness;        // Csúszósság (0 = Gekkótalp, 98 = Jéghártya)
	float r, g, b;               // Alapszín (Vertex Color a 60 FPS rendereléshez)
};

// ============================================================================
// 2. PIXEL DATABASE (C++ Singleton Osztály a 130 Pixelhez és GPU Bufferhez)
// ============================================================================
class PixelDatabase : public Object {
	GDCLASS(PixelDatabase, Object)

private:
	static PixelProperties pixels[131];
	static bool initialized;

protected:
	static void _bind_methods();

public:
	PixelDatabase();
	~PixelDatabase();

	static void init_database();
	static const PixelProperties& get_pixel_fast(uint8_t p_id);

	// GDScriptből és Compute Shaderből is hívható lekérdezők:
	Dictionary get_pixel_info(int p_id) const;
	int get_pixel_ak(int p_id) const;
	int get_pixel_durability(int p_id) const;
	int get_pixel_hardness(int p_id) const;
	int get_pixel_mass_pm(int p_id) const;
	int get_pixel_slipperiness(int p_id) const;
	Color get_pixel_color(int p_id) const;

	// A Compute Shader (voxel_compute.glsl) számára GPU StorageBuffer előállítása:
	PackedByteArray get_gpu_material_buffer() const;
};

// ============================================================================
// 3. VOXEL BODY 3D (100%-ban Törhető, 50x50x50 Pixeles 3D Test és Fizikai Motor)
// ============================================================================
class VoxelBody3D : public MeshInstance3D {
	GDCLASS(VoxelBody3D, MeshInstance3D)

private:
	// Méretek (Alapértelmezett: 1 Blokk = 50x50x50 pixel = 125 000 voxel)
	int size_x;
	int size_y;
	int size_z;
	float voxel_scale; // 1 pixel mérete a 3D térben (alapból 0.1 egység)

	// Szupergyors, folytonos memóriatömbök a 60+ FPS-hez és a Compute Shaderhez:
	PackedByteArray voxel_ids;         // 0 = Levegő, 1..130 = Pixel ID
	PackedInt32Array voxel_durability; // Minden egyes pixel aktuális egyéni Durability-je
	PackedInt32Array voxel_heat;       // Minden egyes pixel aktuális hőmérséklete (°C)

	// Test Fizikai Tulajdonságok (4. Fejezet - Tömeg és 3D Súlypont)
	bool is_static_body;
	int64_t total_mass_pm;             // Össztömeg Pixelmassban (1 Pm = 0.5 kg)
	Vector3 center_of_mass;            // 3D-s tömegközéppont (a fák dőléséhez és forgáshoz)

	// Kint Érc (29-es pixel) Kinetikus Töltés Rendszere (24.2.1. Fejezet)
	float kint_stored_damage;          // Eltárolt (Dur_Kint - Dur_cél) bónusz sebzés
	float kint_timer;                  // 5.0 másodperces visszaszámláló

	Ref<ArrayMesh> voxel_mesh;
	Ref<StandardMaterial3D> voxel_material;

	inline int get_index(int x, int y, int z) const {
		return x + size_x * (y + size_y * z);
	}

	inline bool is_valid_coord(int x, int y, int z) const {
		return (x >= 0 && x < size_x && y >= 0 && y < size_y && z >= 0 && z < size_z);
	}

protected:
	static void _bind_methods();

public:
	VoxelBody3D();
	~VoxelBody3D();

	// 1. Inicializálás és Blokk-kitöltés:
	void initialize_grid(int p_x, int p_y, int p_z, float p_voxel_scale = 0.1f);
	void fill_block(int p_pixel_id);
	void fill_box(Vector3i p_min, Vector3i p_max, int p_pixel_id);

	// 2. Pixel-pontos Építés (Build) és Lekérdezés:
	void set_voxel(int x, int y, int z, int p_pixel_id);
	int get_voxel(int x, int y, int z) const;
	int get_voxel_durability(int x, int y, int z) const;

	// 3. 60 FPS Mesh Generálás és GPU Buffer:
	void rebuild_mesh();
	PackedByteArray get_voxel_ids_buffer() const;
	void recalculate_mass_and_center();

	// 4. Harci és Becsapódási Fizika (15. Fejezet: D_nyers, N_kontakt, K_forma, Newton III.):
	Dictionary apply_impact_damage(
		Vector3i hit_pos,
		int contact_radius,
		int attacker_pixel_id,
		float impact_speed,
		float attacker_mass_pm,
		float k_forma,
		float damage_scale,
		int light_level
	);

	// 5. Kint Érc 5 mp-es Időzítő és Shockwave Kisülés:
	Dictionary update_kint_timer(float delta);

	// 6. GAP / JAP Statika és 3D Flood-Fill Kettészakadás / Csonkolás (5.2. és 16. Fejezet):
	Array check_and_extract_disconnected_bodies();

	// Getterek a GDScript vezérlőknek:
	int64_t get_total_mass_pm() const { return total_mass_pm; }
	float get_total_mass_kg() const { return (float)total_mass_pm * 0.5f; }
	Vector3 get_center_of_mass() const { return center_of_mass; }
	Vector3i get_grid_size() const { return Vector3i(size_x, size_y, size_z); }
};

} // namespace godot

#endif // CYD_VOXEL_ENGINE_H
