#include "example_class.h"
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/base_material3d.hpp>
#include <cmath>
#include <queue>

using namespace godot;

extern float cyd_fast_hash(int x, int y, int z, int seed);
extern int cyd_get_global_terrain_y(int gx, int gz, int seed);

VoxelBody3D::VoxelBody3D() {
	PixelDatabase::init_database();
	size_x = 64; size_y = 168; size_z = 64; max_filled_y = 0; voxel_scale = 0.1f;
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
		int grain = (id == 34 || id == 33) ? ((int)(cyd_fast_hash(x, y / 3, z, 11) * 3.0f)) : ((int)(cyd_fast_hash(x, y, z, 17) * 3.0f));
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
				int neighbor_surf_y = (is_terrain_chunk && at_border) ? cyd_get_global_terrain_y(chunk_gx_origin + x, chunk_gz_origin + z + dz_s, cached_world_seed) : -1;
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
				int neighbor_surf_y = (is_terrain_chunk && at_border) ? cyd_get_global_terrain_y(chunk_gx_origin + x + dx_s, chunk_gz_origin + z, cached_world_seed) : -1;
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
	ClassDB::bind_method(D_METHOD("build_next_tree_archetype"), &VoxelBody3D::build_next_tree_archetype);
	ClassDB::bind_method(D_METHOD("get_built_tree_archetypes_count"), &VoxelBody3D::get_built_tree_archetypes_count);
	ClassDB::bind_method(D_METHOD("refresh_tree_archetype_mesh"), &VoxelBody3D::refresh_tree_archetype_mesh);
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
