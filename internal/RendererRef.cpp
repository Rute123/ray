#include "RendererRef.h"

#include "SceneRef.h"

#include <math/math.hpp>

namespace ray {
namespace ref {
bool bbox_test(const float o[3], const float inv_d[3], const float t, const float bbox_min[3], const float bbox_max[3]);
}
}

ray::ref::Renderer::Renderer(int w, int h) : framebuf_(w, h) {
    auto u_0_to_1 = []() {
        return float(rand()) / RAND_MAX;
    };

    for (int i = 0; i < 64; i++) {
        color_table_.push_back({ u_0_to_1(), u_0_to_1(), u_0_to_1(), 1 });
    }
}

std::shared_ptr<ray::SceneBase> ray::ref::Renderer::CreateScene() {
    return std::make_shared<ref::Scene>();
}

void ray::ref::Renderer::RenderScene(const std::shared_ptr<SceneBase> &_s, region_t region) {
    using namespace math;

    const auto s = std::dynamic_pointer_cast<ref::Scene>(_s);
    if (!s) return;

    const auto &cam = s->GetCamera(s->current_cam());

    const auto num_tris = (uint32_t)s->tris_.size();
    const auto *tris = num_tris ? &s->tris_[0] : nullptr;

    const auto num_indices = (uint32_t)s->tri_indices_.size();
    const auto *tri_indices = num_indices ? &s->tri_indices_[0] : nullptr;

    const auto num_nodes = (uint32_t)s->nodes_.size();
    const auto *nodes = num_nodes ? &s->nodes_[0] : nullptr;

    const auto macro_tree_root = (uint32_t)s->macro_nodes_start_;

    const auto num_meshes = (uint32_t)s->meshes_.size();
    const auto *meshes = num_meshes ? &s->meshes_[0] : nullptr;

    const auto num_transforms = (uint32_t)s->transforms_.size();
    const auto *transforms = num_transforms ? &s->transforms_[0] : nullptr;

    const auto num_mesh_instances = (uint32_t)s->mesh_instances_.size();
    const auto *mesh_instances = num_mesh_instances ? &s->mesh_instances_[0] : nullptr;

    const auto num_mi_indices = (uint32_t)s->mi_indices_.size();
    const auto *mi_indices = num_mi_indices ? &s->mi_indices_[0] : nullptr;

    const auto num_vertices = (uint32_t)s->vertices_.size();
    const auto *vertices = num_vertices ? &s->vertices_[0] : nullptr;

    const auto num_vtx_indices = (uint32_t)s->vtx_indices_.size();
    const auto *vtx_indices = num_vtx_indices ? &s->vtx_indices_[0] : nullptr;

    const auto num_textures = (uint32_t)s->textures_.size();
    const auto *textures = num_textures ? &s->textures_[0] : nullptr;

    const auto num_materials = (uint32_t)s->materials_.size();
    const auto *materials = num_materials ? &s->materials_[0] : nullptr;

    const auto w = framebuf_.w(), h = framebuf_.h();

    if (region.w == 0 || region.h == 0) {
        region = { 0, 0, w, h };
    }

    aligned_vector<ray_packet_t> primary_rays;
    aligned_vector<hit_data_t> intersections;

    GeneratePrimaryRays(cam, region, w, h, primary_rays);

    intersections.reserve(primary_rays.size());

    for (size_t i = 0; i < primary_rays.size(); i++) {
        hit_data_t inter;
        inter.id = primary_rays[i].id;

        const ray_packet_t &r = primary_rays[i];
        const float inv_d[3] = { 1.0f / r.d[0], 1.0f / r.d[1], 1.0f / r.d[2] };

        if (Traverse_MacroTree_CPU(r, inv_d, nodes, macro_tree_root, mesh_instances, mi_indices, meshes, transforms, tris, tri_indices, inter)) {
            intersections.push_back(inter);
        }
    }

    for (size_t i = 0; i < intersections.size(); i++) {
        const auto &ii = intersections[i];

        const int x = ii.id.x;
        const int y = ii.id.y;

        /*const pixel_color_t col1 = color_table_[ii.prim_indices[0] % color_table_.size()];

        if (ii.mask_values[0]) {
            framebuf_.SetPixel(x, y, col1);
        }*/

        /////////////////

        const auto &t = s->texture_atlas_;

        if (!ii.mask_values[0]) continue;

        const auto &tri = tris[ii.prim_indices[0]];

        const auto &mat = materials[tri.mi];

        const auto &v1 = vertices[vtx_indices[ii.prim_indices[0] * 3 + 0]];
        const auto &v2 = vertices[vtx_indices[ii.prim_indices[0] * 3 + 1]];
        const auto &v3 = vertices[vtx_indices[ii.prim_indices[0] * 3 + 2]];

        const vec2 u1 = make_vec2(v1.t0);
        const vec2 u2 = make_vec2(v2.t0);
        const vec2 u3 = make_vec2(v3.t0);

        float w = 1.0f - ii.u - ii.v;
        vec2 uvs = u1 * w + u2 * ii.u + u3 * ii.v;

        if (mat.type == DiffuseMaterial) {
            const auto &diff_tex = textures[mat.textures[MAIN_TEXTURE]];

            pixel_color_t col = t.SampleBilinear(diff_tex, uvs, 0);

            framebuf_.SetPixel(x, y, col);
        } else {
            framebuf_.SetPixel(x, y, { 0, 1.0f, 1.0f, 1.0f });
        }
    }

    /*const auto &t = s->texture_atlas_;

    const int M = 4;

    for (int j = region.y; j < region.y + region.h; j++) {
        for (int i = region.x; i < region.x + region.w; i++) {
            if ((M * i) >= MAX_TEXTURE_SIZE || (M * j) >= MAX_TEXTURE_SIZE) continue;

            pixel_color_t col;
            col.r = t.pages_[0][(M * j) * MAX_TEXTURE_SIZE + (M * i)].r / 255.0f;
            col.g = t.pages_[0][(M * j) * MAX_TEXTURE_SIZE + (M * i)].g / 255.0f;
            col.b = t.pages_[0][(M * j) * MAX_TEXTURE_SIZE + (M * i)].b / 255.0f;
            col.a = 1;

            framebuf_.SetPixel(i, j, col);
        }
    }*/
}
