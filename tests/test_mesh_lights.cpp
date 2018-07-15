#include "test_common.h"

#include <cstdint>

#include <fstream>

#include "../RendererFactory.h"

#include "test_scene1.h"
#include "test_img1.h"

void WriteTGA(const uint8_t *data, int w, int h, const std::string &name) {
    int bpp = 3;

    std::ofstream file(name, std::ios::binary);

    unsigned char header[18] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    header[12] = w & 0xFF;
    header[13] = (w >> 8) & 0xFF;
    header[14] = (h) & 0xFF;
    header[15] = (h >> 8) & 0xFF;
    header[16] = bpp * 8;

    file.write((char *)&header[0], sizeof(unsigned char) * 18);

    auto out_data = std::unique_ptr<uint8_t[]>{ new uint8_t[w * h * bpp] };
    for (int i = 0; i < w * h; i++) {
        out_data[i * 3 + 0] = data[i * 3 + 2];
        out_data[i * 3 + 1] = data[i * 3 + 1];
        out_data[i * 3 + 2] = data[i * 3 + 0];
    }

    file.write((const char *)&out_data[0], w * h * bpp);

    static const char footer[26] = "\0\0\0\0" // no extension area
        "\0\0\0\0"// no developer directory
        "TRUEVISION-XFILE"// yep, this is a TGA file
        ".";
    file.write((const char *)&footer, sizeof(footer));
}

void test_mesh_lights() {
    const int NUM_SAMPLES = 4096;

    const ray::pixel_color8_t white = { 255, 255, 255, 255 };

    const float view_origin[] = { 2.0f, 2.0f, 0.0f };
    const float view_dir[] = { -1.0f, 0.0f, 0.0f };

    ray::environment_desc_t env_desc;
    env_desc.sun_dir[0] = env_desc.sun_dir[1] = env_desc.sun_dir[2] = 0.0f;
    env_desc.sun_col[0] = env_desc.sun_col[1] = env_desc.sun_col[2] = 0.0f;
    env_desc.sky_col[0] = env_desc.sky_col[1] = env_desc.sky_col[2] = 0.5f;
    env_desc.sun_softness = 0.0f;

    ray::tex_desc_t tex_desc1;
    tex_desc1.w = 1;
    tex_desc1.h = 1;
    tex_desc1.generate_mipmaps = true;
    tex_desc1.data = &white;

    ray::mat_desc_t mat_desc1;
    mat_desc1.type = ray::EmissiveMaterial;
    mat_desc1.strength = 10.0f;
    mat_desc1.main_color[0] = 1.0f;
    mat_desc1.main_color[1] = 0.0f;
    mat_desc1.main_color[2] = 0.0f;

    ray::mat_desc_t mat_desc2;
    mat_desc2.type = ray::EmissiveMaterial;
    mat_desc2.strength = 10.0f;
    mat_desc2.main_color[0] = 0.0f;
    mat_desc2.main_color[1] = 1.0f;
    mat_desc2.main_color[2] = 0.0f;

    ray::mat_desc_t mat_desc3;
    mat_desc3.type = ray::EmissiveMaterial;
    mat_desc3.strength = 10.0f;
    mat_desc3.main_color[0] = 0.0f;
    mat_desc3.main_color[1] = 0.0f;
    mat_desc3.main_color[2] = 1.0f;

    ray::mat_desc_t mat_desc4;
    mat_desc4.type = ray::DiffuseMaterial;
    mat_desc4.main_color[0] = 1.0f;
    mat_desc4.main_color[1] = 1.0f;
    mat_desc4.main_color[2] = 1.0f;

    ray::mesh_desc_t mesh_desc;
    mesh_desc.prim_type = ray::TriangleList;
    mesh_desc.layout = ray::PxyzNxyzTuv;
    mesh_desc.vtx_attrs = &attrs[0];
    mesh_desc.vtx_attrs_count = attrs_count / 8;
    mesh_desc.vtx_indices = &indices[0];
    mesh_desc.vtx_indices_count = indices_count;

    {
        ray::settings_t s;
        s.w = 64;
        s.h = 64;
        auto renderer = ray::CreateRenderer(s, ray::RendererOCL);

        auto scene = renderer->CreateScene();

        uint32_t cam = scene->AddCamera(ray::Persp, view_origin, view_dir, 45.0f, 1.0f);
        scene->set_current_cam(cam);

        uint32_t t1 = scene->AddTexture(tex_desc1);

        mat_desc1.main_texture = t1;
        uint32_t m1 = scene->AddMaterial(mat_desc1);

        mat_desc2.main_texture = t1;
        uint32_t m2 = scene->AddMaterial(mat_desc2);

        mat_desc3.main_texture = t1;
        uint32_t m3 = scene->AddMaterial(mat_desc3);

        mat_desc4.main_texture = t1;
        uint32_t m4 = scene->AddMaterial(mat_desc4);

        mesh_desc.shapes.push_back({ m1, groups[0], groups[1] });
        mesh_desc.shapes.push_back({ m2, groups[2], groups[3] });
        mesh_desc.shapes.push_back({ m3, groups[4], groups[5] });
        mesh_desc.shapes.push_back({ m4, groups[6], groups[7] });

        uint32_t mesh = scene->AddMesh(mesh_desc);

        float xform[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f };

        uint32_t mesh_instance = scene->AddMeshInstance(mesh, xform);

        renderer->Clear();

        auto reg = ray::RegionContext{ { 0, 0, 64, 64 } };
        for (int i = 0; i < NUM_SAMPLES; i++) {
            renderer->RenderScene(scene, reg);
        }

        const ray::pixel_color_t *pixels = renderer->get_pixels_ref();

        uint64_t diff = 0;

        uint8_t img_data_u8[img_w * img_h * 3];

        for (int j = 0; j < img_h; j++) {
            for (int i = 0; i < img_w; i++) {
                ray::pixel_color_t p = pixels[j * img_w + i];

                if (p.r > 1.0f) p.r = 1.0f;
                if (p.g > 1.0f) p.g = 1.0f;
                if (p.b > 1.0f) p.b = 1.0f;

                uint8_t r = uint8_t(p.r * 255);
                uint8_t g = uint8_t(p.g * 255);
                uint8_t b = uint8_t(p.b * 255);
                uint8_t a = uint8_t(p.a * 255);

                img_data_u8[3 * ((img_h - j - 1) * img_w + i) + 0] = r;
                img_data_u8[3 * ((img_h - j - 1) * img_w + i) + 1] = g;
                img_data_u8[3 * ((img_h - j - 1) * img_w + i) + 2] = b;

                diff += std::abs(r - img_data[4 * ((img_h - j - 1) * img_h + i) + 0]);
                diff += std::abs(g - img_data[4 * ((img_h - j - 1) * img_h + i) + 1]);
                diff += std::abs(b - img_data[4 * ((img_h - j - 1) * img_h + i) + 2]);

                //require(std::abs(r - img_data[4 * ((img_h - j - 1) * img_h + i) + 0]) < 4);
            }
        }

        WriteTGA(&img_data_u8[0], img_w, img_h, "fail.tga");

        printf("%f\n", double(diff) / (img_w * img_h * 3));
    }
}