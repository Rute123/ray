#pragma once

#include <cstdint>

#include "../SceneBase.h"
#include "../Types.h"

#ifdef __GNUC__
#define force_inline __attribute__((always_inline)) inline
#endif
#ifdef _MSC_VER
#define force_inline __forceinline
#endif

#include "simd/aligned_allocator.h"

namespace ray {
enum eUninitialize { Uninitialize };

union rays_id_t {
    uint32_t id;
    struct {
        uint16_t x, y;
    };
};

struct tri_accel_t {
    float nu, nv;
    float np;
    float pu, pv;
    int32_t ci;
    float e0u, e0v;
    float e1u, e1v;
    uint32_t mi;
    uint32_t pad;
};
static_assert(sizeof(tri_accel_t) == 48, "!");

const uint8_t TRI_W_BITS = 0b00000011;
const uint8_t TRI_AXIS_ALIGNED_BIT = 0b00000100;

const float HIT_BIAS = 0.001f;
const float HIT_EPS = 0.000001f;
const float FLT_EPS = 0.0000001f;

const float PI = 3.141592653589793238463f;

const float MAX_DIST = 3.402823466e+38F;

const int MAX_BOUNCES = 4;

struct bvh_node_t {
    uint32_t prim_index, prim_count,
             left_child, right_child, parent, sibling,
             space_axis; // axis with maximal child's centroids distance
    float bbox[2][3];
};
static_assert(sizeof(bvh_node_t) == 52, "!");

const int MAX_MIP_LEVEL = 11;
const int NUM_MIP_LEVELS = MAX_MIP_LEVEL + 1;
const int MAX_TEXTURE_SIZE = (1 << NUM_MIP_LEVELS);

struct texture_t {
    uint16_t size[2];
    uint8_t page[NUM_MIP_LEVELS];
    uint16_t pos[NUM_MIP_LEVELS][2];
};
static_assert(sizeof(texture_t) == 64, "!");

const int MAX_MATERIAL_TEXTURES = 7;

const int NORMALS_TEXTURE = 0;
const int MAIN_TEXTURE = 1;

const int MIX_MAT1 = 2;
const int MIX_MAT2 = 3;

struct material_t {
    uint32_t type;
    uint32_t textures[MAX_MATERIAL_TEXTURES];
    float main_color[3], pad;
    union {
        float roughness;
        float strength;
    };
    union {
        float fresnel;
        float ior;
    };
    float pad1[2];
};
static_assert(sizeof(material_t) == 64, "!");

struct prim_t;

template <typename T>
using aligned_vector = std::vector<T, aligned_allocator<T, alignof(T)>>;

void PreprocessTri(const float *p, int stride, tri_accel_t *acc);

uint32_t PreprocessMesh(const float *attrs, size_t attrs_count, const uint32_t *indices, size_t indices_count, eVertexLayout layout,
                        std::vector<bvh_node_t> &out_nodes, std::vector<tri_accel_t> &out_tris, std::vector<uint32_t> &out_indices);

uint32_t PreprocessPrims(const prim_t *prims, size_t prims_count,
                         std::vector<bvh_node_t> &out_nodes, std::vector<uint32_t> &out_indices);

bool NaiivePluckerTest(const float p[9], const float o[3], const float d[3]);

void ConstructCamera(eCamType type, const float origin[3], const float fwd[3], float fov, camera_t *cam);

void TransformBoundingBox(const float bbox[2][3], const float *xform, float out_bbox[2][3]);

void InverseMatrix(const float mat[16], float out_mat[16]);

const int PrimesCount = 11;
const int g_primes[PrimesCount] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31 };
const int g_prime_sums[PrimesCount] = { 0, 2, 5, 10, 17, 28, 41, 58, 77, 100, 129 };

const int HaltonSeqLen = 256;

struct vertex_t {
    float p[3], n[3], b[3], t0[2];
};
static_assert(sizeof(vertex_t) == 44, "!");

struct mesh_t {
    uint32_t node_index, node_count;
};
static_assert(sizeof(mesh_t) == 8, "!");

struct transform_t {
    float xform[16], inv_xform[16];
};
static_assert(sizeof(transform_t) == 128, "!");

struct mesh_instance_t {
    float bbox_min[3];
    uint32_t tr_index;
    float bbox_max[3];
    uint32_t mesh_index;
};
static_assert(sizeof(mesh_instance_t) == 32, "!");

extern const float uint8_to_float_table[];

force_inline float to_norm_float(uint8_t v) {
    return uint8_to_float_table[v];
}

extern const uint8_t morton_table_16[];
extern const int morton_table_256[];

extern const float omega_step;
extern const char omega_table[];

extern const float phi_step;
extern const char phi_table[][17];

struct ray_chunk_t {
    uint32_t hash, base, size;
};

}
