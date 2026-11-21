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

struct PixelProperties {
	uint8_t id;
	const char* name;
	uint8_t tier;
	int32_t ak_threshold;
	int32_t max_durability;
	int32_t hardness;
	int32_t elasticity;
	int32_t bounciness;
	int32_t mass_pm;
	int32_t gap_resistance;
	int32_t ap_max;
	int32_t thermal_cond;
	int32_t ignition_point;
	int32_t melting_point;
	int32_t boiling_point;
	int32_t acoustic_vib;
	int32_t electric_cond;
	int32_t slipperiness;
	float r, g, b;
};

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

	Dictionary get_pixel_info(int p_id) const;
	int get_pixel_ak(int p_id) const;
	int get_pixel_durability(int p_id) const;
	int get_pixel_hardness(int p_id) const;
	int get_pixel_mass_pm(int p_id) const;
	int get_pixel_slipperiness(int p_id) const;
	Color get_pixel_color(int p_id) const;
	PackedByteArray get_gpu_material_buffer() const;
};

class VoxelBody3D : public MeshInstance3D {
	GDCLASS(VoxelBody3D, MeshInstance3D)

private:
	static int built_tree_archetypes_count;

	int size_x;
	int size_y;
	int size_z;
	int max_filled_y;
	int min_terrain_y;
	int max_terrain_y;
	int high_min_x, high_max_x;
	int high_min_z, high_max_z;
	int chunk_gx_origin;
	int chunk_gz_origin;
	int cached_world_seed;
	bool is_terrain_chunk;
	bool is_sleeping;
	bool ores_generated;
	float voxel_scale;

	PackedByteArray voxel_ids;
	PackedInt32Array voxel_durability;
	PackedInt32Array voxel_heat;

	bool is_static_body;
	int64_t total_mass_pm;
	Vector3 center_of_mass;

	float kint_stored_damage;
	float kint_timer;

	Ref<ArrayMesh> voxel_mesh;
	Ref<StandardMaterial3D> voxel_material;

	inline int get_index(int x, int y, int z) const {
		return x + size_x * (y + size_y * z);
	}

	inline bool is_valid_coord(int x, int y, int z) const {
		return (x >= 0 && x < size_x && y >= 0 && y < size_y && z >= 0 && z < size_z);
	}

	void ensure_render_resources();
	static void build_single_tree_archetype(int archetype_idx);
	void stamp_cached_tree_in_sector(int gx_min, int gz_min, int tree_gx, int tree_gz, int tree_base_y, int arch_id);

protected:
	static void _bind_methods();

public:
	VoxelBody3D();
	~VoxelBody3D();

	void initialize_grid(int p_x, int p_y, int p_z, float p_voxel_scale = 0.1f);
	void wake_up();
	void sleep_sector();
	bool is_sector_sleeping() const { return is_sleeping; }

	int build_next_tree_archetype();
	int get_built_tree_archetypes_count() const { return built_tree_archetypes_count; }
	void refresh_tree_archetype_mesh() {}

	void fill_block(int p_pixel_id);
	void fill_box(Vector3i p_min, Vector3i p_max, int p_pixel_id);

	void set_voxel(int x, int y, int z, int p_pixel_id);
	int get_voxel(int x, int y, int z) const;
	int get_voxel_durability(int x, int y, int z) const;
	int get_surface_y(int x, int z) const;

	bool check_cylinder_collision(Vector3 world_feet_pos, float radius, float height) const;
	float get_ground_y_near(float world_x, float current_feet_y, float world_z) const;

	void generate_biome1_sector(int chunk_x, int chunk_z, int world_seed);
	Dictionary raycast_voxel(Vector3 world_origin, Vector3 world_dir, float max_dist) const;

	void rebuild_mesh();
	PackedByteArray get_voxel_ids_buffer() const;
	void recalculate_mass_and_center();

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

	Dictionary update_kint_timer(float delta);
	Array check_and_extract_disconnected_bodies();

	int64_t get_total_mass_pm() const { return total_mass_pm; }
	float get_total_mass_kg() const { return (float)total_mass_pm * 0.5f; }
	Vector3 get_center_of_mass() const { return center_of_mass; }
	Vector3i get_grid_size() const { return Vector3i(size_x, size_y, size_z); }
};

} // namespace godot

#endif // CYD_VOXEL_ENGINE_H
