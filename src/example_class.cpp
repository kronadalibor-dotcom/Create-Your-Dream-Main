#include "example_class.h"
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace godot;

struct TreeVoxelOffset { int8_t dx, dy, dz; uint8_t id; };
static std::vector<TreeVoxelOffset> tree_archetypes[100];
int VoxelBody3D::built_tree_archetypes_count = 0;

float cyd_fast_hash(int x, int y, int z, int seed) {
	int n = x * 374761393 + y * 668265263 + z * 2147483647 + seed * 1610612741;
	n = (n ^ (n >> 13)) * 1274126177;
	return (float)((n ^ (n >> 16)) & 0x7FFFFFFF) / 2147483647.0f;
}

static float smooth_noise3(float x, float y, float z, int seed) {
	int ix = (int)std::floor(x), iy = (int)std::floor(y), iz = (int)std::floor(z);
	float fx = x - ix, fy = y - iy, fz = z - iz;
	fx = fx*fx*(3.0f-2.0f*fx); fy = fy*fy*(3.0f-2.0f*fy); fz = fz*fz*(3.0f-2.0f*fz);
	auto h = [&](int dx, int dy, int dz) { return cyd_fast_hash(ix+dx, iy+dy, iz+dz, seed); };
	float c00 = h(0,0,0)*(1-fx)+h(1,0,0)*fx, c10 = h(0,1,0)*(1-fx)+h(1,1,0)*fx;
	float c01 = h(0,0,1)*(1-fx)+h(1,0,1)*fx, c11 = h(0,1,1)*(1-fx)+h(1,1,1)*fx;
	return (c00*(1-fy)+c10*fy)*(1-fz) + (c01*(1-fy)+c11*fy)*fz;
}

int cyd_get_global_terrain_y(int gx, int gz, int seed) {
	float base_und = smooth_noise3(gx * 0.007f, 0.0f, gz * 0.007f, seed + 101) * 9.0f;
	float detail = smooth_noise3(gx * 0.022f, 0.0f, gz * 0.022f, seed + 202) * 2.5f;
	float hill_n = smooth_noise3(gx * 0.0026f, 0.0f, gz * 0.0026f, seed);
	float hill_h = 0.0f;
	if (hill_n > 0.34f) {
		float t = CLAMP((hill_n - 0.34f) / 0.66f, 0.0f, 1.0f);
		t = t * t * (3.0f - 2.0f * t);
		hill_h = t * 52.0f;
	}
	return CLAMP((int)(10.0f + base_und + detail + hill_h), 7, 74);
}

static inline int floor_div_int(int a, int b) {
	int d = a / b, r = a % b;
	return (r != 0 && ((r < 0) ^ (b < 0))) ? (d - 1) : d;
}

void VoxelBody3D::build_single_tree_archetype(int idx) {
	if (idx < 0 || idx >= 100 || !tree_archetypes[idx].empty()) return;
	const int TX = 84, TY = 102, TZ = 84;
	std::vector<uint8_t> tg(TX * TY * TZ, 0);
	auto t_idx = [&](int x, int y, int z) -> int { return x + TX * (y + TY * z); };
	auto t_set = [&](int x, int y, int z, uint8_t id) {
		if (x >= 0 && x < TX && y >= 0 && y < TY && z >= 0 && z < TZ) tg[t_idx(x, y, z)] = id;
	};
	auto t_get = [&](int x, int y, int z) -> uint8_t {
		return (x >= 0 && x < TX && y >= 0 && y < TY && z >= 0 && z < TZ) ? tg[t_idx(x, y, z)] : 0;
	};

	int seed = 1337 + idx * 9173, tgx = 42, tgz = 42, tby = 6;
	auto draw_leaf = [&](float cx, float cy, float cz, float rad) {
		int ir = (int)std::ceil(rad);
		for (int dz = -ir; dz <= ir; ++dz)
			for (int dy = -ir; dy <= ir; ++dy)
				for (int dx = -ir; dx <= ir; ++dx) {
					float d2 = (dx*dx + dy*dy*1.4f + dz*dz) / (rad*rad);
					int vx = (int)cx + dx, vy = (int)cy + dy, vz = (int)cz + dz;
					if (d2 <= 1.0f && cyd_fast_hash(vx, vy, vz, seed + 77) > (d2 * 0.45f)) {
						if (t_get(vx, vy, vz) == 0) t_set(vx, vy, vz, 35);
					}
				}
	};

	float rot_off = cyd_fast_hash(idx, 9, idx * 3, seed) * 6.2831f;
	int trunk_h = 54 + (int)(cyd_fast_hash(idx, 3, idx * 7, seed) * 20.0f);
	float base_r = 5.4f + cyd_fast_hash(idx, 5, 2, seed) * 1.2f;
	for (int h = -6; h < trunk_h; ++h) {
		float wobble = (h > 4) ? 0.6f : 0.0f;
		float cx = tgx + std::sin(h * 0.05f + rot_off) * wobble, cz = tgz + std::cos(h * 0.05f + rot_off) * wobble;
		int cy = tby + h;
		float rad = base_r - (float)MAX(0, h) / trunk_h * 2.3f + (h < 6 ? (6 - MAX(0, h)) * 0.25f : 0.0f);
		int ir = (int)std::ceil(rad);
		for (int dz = -ir; dz <= ir; ++dz)
			for (int dx = -ir; dx <= ir; ++dx) {
				float d = std::sqrt((float)(dx*dx + dz*dz));
				if (d <= rad - 1.3f) t_set((int)cx + dx, cy, (int)cz + dz, 33);
				else if (d <= rad) t_set((int)cx + dx, cy, (int)cz + dz, 34);
			}
	}

	int branch_cnt = 7 + (idx % 4);
	for (int b = 0; b < branch_cnt; ++b) {
		int start_h = (trunk_h - 24) + (b * 3), b_len = 18 + ((b + idx) % 4) * 3;
		float ang = rot_off + b * 2.39996f, dir_x = std::cos(ang), dir_z = std::sin(ang), pitch = 0.30f + ((b + idx) % 3) * 0.11f;
		float bx = (float)tgx, by = (float)(tby + start_h), bz = (float)tgz;
		for (int step = 1; step <= b_len; ++step) {
			bx += dir_x; bz += dir_z; by += pitch;
			int brad = (step < 8) ? 2 : 1;
			for (int oz = -brad; oz <= brad; ++oz)
				for (int oy = -brad; oy <= brad; ++oy)
					for (int ox = -brad; ox <= brad; ++ox)
						if (ox*ox + oy*oy + oz*oz <= brad*brad)
							t_set((int)bx + ox, (int)by + oy, (int)bz + oz, (brad == 2 && ox==0 && oy==0 && oz==0) ? 33 : 34);
			if (step == 10 || step == b_len - 4) {
				for (int side = -1; side <= 1; side += 2) {
					float sang = ang + side * 0.75f, sx = bx, sy = by, sz = bz;
					for (int ss = 1; ss <= 9; ++ss) {
						sx += std::cos(sang); sz += std::sin(sang); sy += 0.25f;
						t_set((int)sx, (int)sy, (int)sz, 33);
						t_set((int)sx, (int)sy + 1, (int)sz, 34);
					}
					draw_leaf(sx, sy + 2.0f, sz, 6.2f);
				}
			}
		}
		draw_leaf(bx, by + 2.5f, bz, 7.8f);
		t_set((int)bx, (int)by - 2, (int)bz, 37);
		t_set((int)bx, (int)by - 3, (int)bz, 38);
	}
	draw_leaf((float)tgx, (float)(tby + trunk_h + 5), (float)tgz, 10.2f);

	std::vector<TreeVoxelOffset>& out = tree_archetypes[idx];
	out.reserve(5000);
	for (int z = 0; z < TZ; ++z)
		for (int y = 0; y < TY; ++y)
			for (int x = 0; x < TX; ++x) {
				uint8_t id = tg[t_idx(x, y, z)];
				if (id != 0) out.push_back({(int8_t)(x - tgx), (int8_t)(y - tby), (int8_t)(z - tgz), id});
			}
}

int VoxelBody3D::build_next_tree_archetype() {
	if (built_tree_archetypes_count >= 100) return 100;
	build_single_tree_archetype(built_tree_archetypes_count);
	built_tree_archetypes_count++;
	return built_tree_archetypes_count;
}

void VoxelBody3D::stamp_cached_tree_in_sector(int gx_min, int gz_min, int tree_gx, int tree_gz, int tree_base_y, int arch_id) {
	if (tree_gx + 42 < gx_min || tree_gx - 42 >= gx_min + size_x || tree_gz + 42 < gz_min || tree_gz - 42 >= gz_min + size_z) return;
	const std::vector<TreeVoxelOffset>& tv = tree_archetypes[arch_id];
	for (size_t i = 0; i < tv.size(); ++i) {
		const TreeVoxelOffset& v = tv[i];
		int lx = (tree_gx + v.dx) - gx_min, lz = (tree_gz + v.dz) - gz_min, ly = tree_base_y + v.dy;
		if (lx >= 0 && lx < size_x && lz >= 0 && lz < size_z && ly >= 0 && ly < size_y) {
			if (v.id == 35 && get_voxel(lx, ly, lz) != 0) continue;
			set_voxel(lx, ly, lz, v.id);
		}
	}

	if (tree_gx + 26 < gx_min || tree_gx - 26 >= gx_min + size_x || tree_gz + 26 < gz_min || tree_gz - 26 >= gz_min + size_z) return;
	float base_ang = (arch_id * 1.13f);
	for (int r = 0; r < 8; ++r) {
		float ang = base_ang + r * 0.7854f + cyd_fast_hash(r, tree_gx, tree_gz, cached_world_seed) * 0.25f;
		float dx = std::cos(ang), dz = std::sin(ang);
		int root_len = 16 + (r % 3) * 3;
		for (int step = 3; step <= root_len; ++step) {
			float curve = std::sin(step * 0.35f + r) * 1.4f;
			int rgx = (int)(tree_gx + dx * step - dz * curve), rgz = (int)(tree_gz + dz * step + dx * curve);
			int lx = rgx - gx_min, lz = rgz - gz_min;
			if (lx >= 0 && lx < size_x && lz >= 0 && lz < size_z) {
				int surf_y = cyd_get_global_terrain_y(rgx, rgz, cached_world_seed);
				int top_expose = (step < 8) ? 2 : 1;
				for (int ry = surf_y - 1; ry <= surf_y + top_expose; ++ry) {
					set_voxel(lx, ry, lz, 36);
					if (step < 11 && lx + 1 < size_x) set_voxel(lx + 1, ry, lz, 36);
				}
			}
			if (step == 8 || step == 13) {
				float side_ang = ang + ((step == 8) ? 0.85f : -0.85f), sdx = std::cos(side_ang), sdz = std::sin(side_ang);
				for (int ss = 1; ss <= 7; ++ss) {
					int sgx = rgx + (int)(sdx * ss), sgz = rgz + (int)(sdz * ss);
					int slx = sgx - gx_min, slz = sgz - gz_min;
					if (slx >= 0 && slx < size_x && slz >= 0 && slz < size_z) {
						int s_sy = cyd_get_global_terrain_y(sgx, sgz, cached_world_seed);
						set_voxel(slx, s_sy, slz, 36);
						set_voxel(slx, s_sy + 1, slz, 36);
					}
				}
			}
		}
	}
}

void VoxelBody3D::wake_up() {
	if (!is_sleeping && voxel_durability.size() == voxel_ids.size()) return;
	uint8_t* ids_w = voxel_ids.ptrw();
	if (is_terrain_chunk && !ores_generated) {
		for (int z = 0; z < size_z; ++z) {
			int gz = chunk_gz_origin + z;
			for (int x = 0; x < size_x; ++x) {
				int gx = chunk_gx_origin + x, sy = get_surface_y(x, z);
				for (int y = 1; y < sy - 4; ++y) {
					int idx = get_index(x, y, z);
					if (ids_w[idx] != 17 && ids_w[idx] != 1) continue;
					uint8_t pid = 17;
					if (y >= sy - 7) pid = 8;
					else if (smooth_noise3(gx*0.12f, y*0.12f, gz*0.12f, cached_world_seed+19) > 0.68f) {
						float ro = cyd_fast_hash(gx, y, gz, cached_world_seed + 33);
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

void VoxelBody3D::generate_biome1_sector(int chunk_x, int chunk_z, int world_seed) {
	if (size_y < 168) initialize_grid(size_x, 168, size_z, voxel_scale);
	while (built_tree_archetypes_count < 16) build_next_tree_archetype();

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
			int gx = gx_min + x, surf_y = cyd_get_global_terrain_y(gx, gz, world_seed);
			if (surf_y < min_terrain_y) min_terrain_y = surf_y;
			if (surf_y > max_terrain_y) max_terrain_y = surf_y;
			if (surf_y > max_filled_y) max_filled_y = surf_y;
			for (int y = 0; y < surf_y - 3; ++y) ids_w[get_index(x, y, z)] = 17;
			for (int y = MAX(0, surf_y - 3); y < surf_y; ++y) ids_w[get_index(x, y, z)] = 1;
			ids_w[get_index(x, surf_y, z)] = 51;
			float fl = cyd_fast_hash(gx, surf_y, gz, world_seed + 99);
			if (fl < 0.016f) {
				int gh = 1 + (int)(cyd_fast_hash(gx, 1, gz, world_seed) * 2.0f);
				for (int h = 1; h <= gh; ++h) set_voxel(x, surf_y + h, z, 52);
			} else if (fl > 0.996f) {
				set_voxel(x, surf_y + 1, z, 55); set_voxel(x, surf_y + 2, z, 54);
			}
		}
	}

	const int CELL = 84;
	int center_cx = floor_div_int(gx_min + size_x / 2, CELL), center_cz = floor_div_int(gz_min + size_z / 2, CELL);
	for (int tcz = center_cz - 1; tcz <= center_cz + 1; ++tcz) {
		for (int tcx = center_cx - 1; tcx <= center_cx + 1; ++tcx) {
			int tgx = tcx * CELL + 22 + (int)(cyd_fast_hash(tcx, 23, tcz, world_seed) * 40.0f);
			int tgz = tcz * CELL + 22 + (int)(cyd_fast_hash(tcx, 37, tcz, world_seed) * 40.0f);
			if (tcx == 0 && tcz == 0) { tgx = 42; tgz = 42; }

			int tby = cyd_get_global_terrain_y(tgx, tgz, world_seed);
			int yn = cyd_get_global_terrain_y(tgx, tgz - 10, world_seed), ys = cyd_get_global_terrain_y(tgx, tgz + 10, world_seed);
			int ye = cyd_get_global_terrain_y(tgx + 10, tgz, world_seed), yw = cyd_get_global_terrain_y(tgx - 10, tgz, world_seed);
			int slope = MAX(MAX(yn, ys), MAX(ye, yw)) - MIN(MIN(yn, ys), MIN(ye, yw));

			float meadow_n = smooth_noise3(tgx * 0.0022f, 0.0f, tgz * 0.0022f, world_seed + 777);
			float hill_style = smooth_noise3(tgx * 0.0032f, 0.0f, tgz * 0.0032f, world_seed + 999);

			float spawn_chance = 0.55f;
			if (meadow_n < 0.35f) spawn_chance = 0.0f;
			else if (tby >= 36 && slope <= 6) spawn_chance = (hill_style < 0.28f) ? 0.0f : 0.88f;
			else if (slope > 6) spawn_chance = (hill_style > 0.68f) ? 0.0f : 0.16f;

			if ((tcx == 0 && tcz == 0) || cyd_fast_hash(tcx, 11, tcz, world_seed) < spawn_chance) {
				int arch_id = std::abs((tcx * 73856093) ^ (tcz * 19349663)) % MAX(1, built_tree_archetypes_count);
				stamp_cached_tree_in_sector(gx_min, gz_min, tgx, tgz, tby, arch_id);
			}

			if (cyd_fast_hash(tcx, 59, tcz, world_seed) < 0.35f) {
				int rgx = tcx * CELL + 10 + (int)(cyd_fast_hash(tcx, 61, tcz, world_seed) * 12.0f);
				int rgz = tcz * CELL + 62 + (int)(cyd_fast_hash(tcx, 67, tcz, world_seed) * 12.0f);
				if (rgx + 10 >= gx_min && rgx - 10 < gx_min + size_x && rgz + 10 >= gz_min && rgz - 10 < gz_min + size_z) {
					int ry_base = cyd_get_global_terrain_y(rgx, rgz, world_seed);
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
