R"(

/* This file should be passed in cl program first */

typedef struct _ray_packet_t {
    float4 o, d;
    float4 c;
    float3 do_dx, dd_dx, do_dy, dd_dy;
} ray_packet_t;

typedef struct _camera_t {
    float3 origin, fwd, side, up;
} camera_t;

typedef struct _tri_accel_t {
    float nu, nv;
    float np;
    float pu, pv;
    int ci;
    float e0u, e0v;
    float e1u, e1v;
    uint mi;
    int pad1;
} tri_accel_t;

typedef struct _hit_data_t {
    int mask, obj_index, prim_index;
    float t, u, v;
    float2 ray_id;
} hit_data_t;

typedef struct _bvh_node_t {
    uint tri_index, tri_count,
         left_child, right_child, parent, sibling,
         space_axis; // axis with maximum child's centroids distance
    float bbox[2][3];
} bvh_node_t;

typedef struct _vertex_t {
    float p[3], n[3], b[3], t0[2];
} vertex_t;

typedef struct _mesh_t {
    uint node_index, node_count;
} mesh_t;

typedef struct _transform_t {
    float16 xform, inv_xform;
} transform_t;

typedef struct _mesh_instance_t {
    float bbox_min[3];
    uint tr_index;
    float bbox_max[3];
    uint mesh_index;
} mesh_instance_t;

typedef struct _texture_t {
    ushort size[2];
    uchar page[12];
    ushort pos[12][2];
} texture_t;

typedef struct _material_t {
    uint type;
    uint textures[MAX_MATERIAL_TEXTURES];
    float3 main_color;
    union {
        float roughness;
        float strength;
    };
    union {
        float fresnel;
        float ior;
    };
} material_t;

typedef struct _environment_t {
    float3 sun_dir;
    float3 sun_col;
    float3 sky_col;
    float sun_softness;
    float pad[3];
} environment_t;

typedef struct _ray_chunk_t {
    uint hash, base, size;
} ray_chunk_t;

__kernel void TypesCheck(ray_packet_t r, camera_t c, tri_accel_t t, hit_data_t i,
                         bvh_node_t b, vertex_t v, mesh_t m, mesh_instance_t mi, transform_t tr,
                         texture_t tex, material_t mat, environment_t env, ray_chunk_t ch) {}

)"