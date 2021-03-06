using namespace ray::NS;

{
    std::cout << "Test simd_fvec4 native? = " << simd_fvec4::is_native() << " | ";

    simd_fvec4 v1, v2 = { 42.0f }, v3 = { 1.0f, 2.0f, 3.0f, 4.0f };

    require(v2[0] == 42.0f);
    require(v2[1] == 42.0f);
    require(v2[2] == 42.0f);
    require(v2[3] == 42.0f);

    require(v3[0] == 1.0f);
    require(v3[1] == 2.0f);
    require(v3[2] == 3.0f);
    require(v3[3] == 4.0f);

    simd_fvec4 v4(v2), v5 = v3;

    require(v4[0] == 42.0f);
    require(v4[1] == 42.0f);
    require(v4[2] == 42.0f);
    require(v4[3] == 42.0f);

    require(v5[0] == 1.0f);
    require(v5[1] == 2.0f);
    require(v5[2] == 3.0f);
    require(v5[3] == 4.0f);

    v1 = v5;

    require(v1[0] == 1.0f);
    require(v1[1] == 2.0f);
    require(v1[2] == 3.0f);
    require(v1[3] == 4.0f);

    float unaligned_array[] = { 0.0f, 2.0f, 30.0f, 14.0f };
    alignas(alignof(simd_fvec4)) float aligned_array[] = { 0.0f, 2.0f, 30.0f, 14.0f };
    
    simd_fvec4 v7 = { &unaligned_array[0] },
               v8 = { &aligned_array[0], simd_mem_aligned };

    require(v7[0] == 0.0f);
    require(v7[1] == 2.0f);
    require(v7[2] == 30.0f);
    require(v7[3] == 14.0f);

    require(v8[0] == 0.0f);
    require(v8[1] == 2.0f);
    require(v8[2] == 30.0f);
    require(v8[3] == 14.0f);

    v5.copy_to(&unaligned_array[0]);
    v1.copy_to(&aligned_array[0], simd_mem_aligned);

    require(unaligned_array[0] == 1.0f);
    require(unaligned_array[1] == 2.0f);
    require(unaligned_array[2] == 3.0f);
    require(unaligned_array[3] == 4.0f);

    require(aligned_array[0] == 1.0f);
    require(aligned_array[1] == 2.0f);
    require(aligned_array[2] == 3.0f);
    require(aligned_array[3] == 4.0f);

    v1 = { 1.0f, 2.0f, 3.0f, 4.0f };
    v2 = { 4.0f, 5.0f, 6.0f, 7.0f };

    v3 = v1 + v2;
    v4 = v1 - v2;
    v5 = v1 * v2;
    auto v6 = v1 / v2;
    
    require(v3[0] == Approx(5));
    require(v3[1] == Approx(7));
    require(v3[2] == Approx(9));
    require(v3[3] == Approx(11));

    require(v4[0] == Approx(-3));
    require(v4[1] == Approx(-3));
    require(v4[2] == Approx(-3));
    require(v4[3] == Approx(-3));

    require(v5[0] == Approx(4));
    require(v5[1] == Approx(10));
    require(v5[2] == Approx(18));
    require(v5[3] == Approx(28));

    require(v6[0] == Approx(0.25));
    require(v6[1] == Approx(0.4));
    require(v6[2] == Approx(0.5));
    require(v6[3] == Approx(0.57142));

    v5 = sqrt(v5);

    require(v5[0] == Approx(2));
    require(v5[1] == Approx(3.1623));
    require(v5[2] == Approx(4.2426));
    require(v5[3] == Approx(5.2915));

    simd_fvec4 v9 = { 3.0f, 6.0f, 7.0f, 6.0f };

    auto v10 = v2 < v9;

    require(v10[0] == 0);
    require(reinterpret_cast<const uint32_t&>(v10[1]) == 0xFFFFFFFF);
    require(reinterpret_cast<const uint32_t&>(v10[2]) == 0xFFFFFFFF);
    require(v10[3] == 0);

    auto v11 = v2 > v9;

    require(reinterpret_cast<const uint32_t&>(v11[0]) == 0xFFFFFFFF);
    require(v11[1] == 0);
    require(v11[2] == 0);
    require(reinterpret_cast<const uint32_t&>(v11[3]) == 0xFFFFFFFF);

    std::cout << "OK" << std::endl;
}

{
    std::cout << "Test simd_ivec4 native? = " << simd_ivec4::is_native() << " | ";

    simd_ivec4 v1, v2 = { 42 }, v3 = { 1, 2, 3, 4 };

    require(v2[0] == 42);
    require(v2[1] == 42);
    require(v2[2] == 42);
    require(v2[3] == 42);

    require(v3[0] == 1);
    require(v3[1] == 2);
    require(v3[2] == 3);
    require(v3[3] == 4);

    simd_ivec4 v4(v2), v5 = v3;

    require(v4[0] == 42);
    require(v4[1] == 42);
    require(v4[2] == 42);
    require(v4[3] == 42);

    require(v5[0] == 1);
    require(v5[1] == 2);
    require(v5[2] == 3);
    require(v5[3] == 4);

    v1 = v5;

    require(v1[0] == 1);
    require(v1[1] == 2);
    require(v1[2] == 3);
    require(v1[3] == 4);

    int unaligned_array[] = { 0, 2, 30, 14 };
    alignas(alignof(simd_ivec4)) int aligned_array[] = { 0, 2, 30, 14 };
    
    simd_ivec4 v7 = { &unaligned_array[0] },
               v8 = { &aligned_array[0], simd_mem_aligned };

    require(v7[0] == 0);
    require(v7[1] == 2);
    require(v7[2] == 30);
    require(v7[3] == 14);

    require(v8[0] == 0);
    require(v8[1] == 2);
    require(v8[2] == 30);
    require(v8[3] == 14);

    v5.copy_to(&unaligned_array[0]);
    v1.copy_to(&aligned_array[0], simd_mem_aligned);

    require(unaligned_array[0] == 1);
    require(unaligned_array[1] == 2);
    require(unaligned_array[2] == 3);
    require(unaligned_array[3] == 4);

    require(aligned_array[0] == 1);
    require(aligned_array[1] == 2);
    require(aligned_array[2] == 3);
    require(aligned_array[3] == 4);

    v1 = { 1, 2, 3, 4 };
    v2 = { 4, 5, 6, 7 };

    v3 = v1 + v2;
    v4 = v1 - v2;
    v5 = v1 * v2;
    auto v6 = v1 / v2;
    
    require(v3[0] == 5);
    require(v3[1] == 7);
    require(v3[2] == 9);
    require(v3[3] == 11);

    require(v4[0] == -3);
    require(v4[1] == -3);
    require(v4[2] == -3);
    require(v4[3] == -3);

    require(v5[0] == 4);
    require(v5[1] == 10);
    require(v5[2] == 18);
    require(v5[3] == 28);

    require(v6[0] == 0);
    require(v6[1] == 0);
    require(v6[2] == 0);
    require(v6[3] == 0);

    require(!v3.all_zeros());
    require(v6.all_zeros());

    //simd_ivec4 v9 = { 3, 6, 7, 6 };

    //auto v10 = v2 < v9;

    std::cout << "OK" << std::endl;
}

{
    std::cout << "Test simd_fvec8 native? = " << simd_fvec8::is_native() << " | ";

    simd_fvec8 v1, v2 = { 42.0f }, v3 = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };

    require(v2[0] == 42.0f);
    require(v2[1] == 42.0f);
    require(v2[2] == 42.0f);
    require(v2[3] == 42.0f);
    require(v2[4] == 42.0f);
    require(v2[5] == 42.0f);
    require(v2[6] == 42.0f);
    require(v2[7] == 42.0f);

    require(v3[0] == 1.0f);
    require(v3[1] == 2.0f);
    require(v3[2] == 3.0f);
    require(v3[3] == 4.0f);
    require(v3[4] == 5.0f);
    require(v3[5] == 6.0f);
    require(v3[6] == 7.0f);
    require(v3[7] == 8.0f);

    simd_fvec8 v4(v2), v5 = v3;

    require(v4[0] == 42.0f);
    require(v4[1] == 42.0f);
    require(v4[2] == 42.0f);
    require(v4[3] == 42.0f);
    require(v4[4] == 42.0f);
    require(v4[5] == 42.0f);
    require(v4[6] == 42.0f);
    require(v4[7] == 42.0f);

    require(v5[0] == 1.0f);
    require(v5[1] == 2.0f);
    require(v5[2] == 3.0f);
    require(v5[3] == 4.0f);
    require(v5[4] == 5.0f);
    require(v5[5] == 6.0f);
    require(v5[6] == 7.0f);
    require(v5[7] == 8.0f);

    v1 = v5;

    require(v1[0] == 1.0f);
    require(v1[1] == 2.0f);
    require(v1[2] == 3.0f);
    require(v1[3] == 4.0f);
    require(v1[4] == 5.0f);
    require(v1[5] == 6.0f);
    require(v1[6] == 7.0f);
    require(v1[7] == 8.0f);

    v1 = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f,  3.0f, 2.0f };
    v2 = { 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 10.0f, 12.0f, 1.0f };

    v3 = v1 + v2;
    v4 = v1 - v2;
    v5 = v1 * v2;
    auto v6 = v1 / v2;
    
    require(v3[0] == Approx(5));
    require(v3[1] == Approx(7));
    require(v3[2] == Approx(9));
    require(v3[3] == Approx(11));
    require(v3[4] == Approx(13));
    require(v3[5] == Approx(14));
    require(v3[6] == Approx(15));
    require(v3[7] == Approx(3));

    require(v4[0] == Approx(-3));
    require(v4[1] == Approx(-3));
    require(v4[2] == Approx(-3));
    require(v4[3] == Approx(-3));
    require(v4[4] == Approx(-3));
    require(v4[5] == Approx(-6));
    require(v4[6] == Approx(-9));
    require(v4[7] == Approx(1));

    require(v5[0] == Approx(4));
    require(v5[1] == Approx(10));
    require(v5[2] == Approx(18));
    require(v5[3] == Approx(28));
    require(v5[4] == Approx(40));
    require(v5[5] == Approx(40));
    require(v5[6] == Approx(36));
    require(v5[7] == Approx(2));

    require(v6[0] == Approx(0.25));
    require(v6[1] == Approx(0.4));
    require(v6[2] == Approx(0.5));
    require(v6[3] == Approx(0.57142));
    require(v6[4] == Approx(0.625));
    require(v6[5] == Approx(0.4));
    require(v6[6] == Approx(0.25));
    require(v6[7] == Approx(2.0));

    v5 = sqrt(v5);

    require(v5[0] == Approx(2));
    require(v5[1] == Approx(3.1623));
    require(v5[2] == Approx(4.2426));
    require(v5[3] == Approx(5.2915));
    require(v5[4] == Approx(6.3246));
    require(v5[5] == Approx(6.3246));
    require(v5[6] == Approx(6));
    require(v5[7] == Approx(1.4142));

    simd_fvec8 v9 = { 3.0f, 6.0f, 7.0f, 6.0f, 2.0f, 12.0f, 18.0f, 0.0f };

    auto v10 = v2 < v9;

    require(v10[0] == 0);
    require(reinterpret_cast<const uint32_t&>(v10[1]) == 0xFFFFFFFF);
    require(reinterpret_cast<const uint32_t&>(v10[2]) == 0xFFFFFFFF);
    require(v10[3] == 0);
    require(v10[4] == 0);
    require(reinterpret_cast<const uint32_t&>(v10[5]) == 0xFFFFFFFF);
    require(reinterpret_cast<const uint32_t&>(v10[6]) == 0xFFFFFFFF);
    require(v10[7] == 0);

    auto v11 = v2 > v9;

    require(reinterpret_cast<const uint32_t&>(v11[0]) == 0xFFFFFFFF);
    require(v11[1] == 0);
    require(v11[2] == 0);
    require(reinterpret_cast<const uint32_t&>(v11[3]) == 0xFFFFFFFF);
    require(reinterpret_cast<const uint32_t&>(v11[4]) == 0xFFFFFFFF);
    require(v11[5] == 0);
    require(v11[6] == 0);
    require(reinterpret_cast<const uint32_t&>(v11[7]) == 0xFFFFFFFF);

    std::cout << "OK" << std::endl;
}

{
    std::cout << "Test simd_ivec8 native? = " << simd_ivec8::is_native() << " | ";

    simd_ivec8 v1, v2 = { 42 }, v3 = { 1, 2, 3, 4, 5, 6, 7, 8 };

    require(v2[0] == 42);
    require(v2[1] == 42);
    require(v2[2] == 42);
    require(v2[3] == 42);
    require(v2[4] == 42);
    require(v2[5] == 42);
    require(v2[6] == 42);
    require(v2[7] == 42);

    require(v3[0] == 1);
    require(v3[1] == 2);
    require(v3[2] == 3);
    require(v3[3] == 4);
    require(v3[4] == 5);
    require(v3[5] == 6);
    require(v3[6] == 7);
    require(v3[7] == 8);

    simd_ivec8 v4(v2), v5 = v3;

    require(v4[0] == 42);
    require(v4[1] == 42);
    require(v4[2] == 42);
    require(v4[3] == 42);
    require(v4[4] == 42);
    require(v4[5] == 42);
    require(v4[6] == 42);
    require(v4[7] == 42);

    require(v5[0] == 1);
    require(v5[1] == 2);
    require(v5[2] == 3);
    require(v5[3] == 4);
    require(v5[4] == 5);
    require(v5[5] == 6);
    require(v5[6] == 7);
    require(v5[7] == 8);

    v1 = v5;

    require(v1[0] == 1);
    require(v1[1] == 2);
    require(v1[2] == 3);
    require(v1[3] == 4);
    require(v1[4] == 5);
    require(v1[5] == 6);
    require(v1[6] == 7);
    require(v1[7] == 8);

    v1 = { 1, 2, 3, 4, 5, 4,  3,  2 };
    v2 = { 4, 5, 6, 7, 8, 10, 12, 1 };

    v3 = v1 + v2;
    v4 = v1 - v2;
    v5 = v1 * v2;
    auto v6 = v1 / v2;
    
    require(v3[0] == 5);
    require(v3[1] == 7);
    require(v3[2] == 9);
    require(v3[3] == 11);
    require(v3[4] == 13);
    require(v3[5] == 14);
    require(v3[6] == 15);
    require(v3[7] == 3);

    require(v4[0] == -3);
    require(v4[1] == -3);
    require(v4[2] == -3);
    require(v4[3] == -3);
    require(v4[4] == -3);
    require(v4[5] == -6);
    require(v4[6] == -9);
    require(v4[7] == 1);

    require(v5[0] == 4);
    require(v5[1] == 10);
    require(v5[2] == 18);
    require(v5[3] == 28);
    require(v5[4] == 40);
    require(v5[5] == 40);
    require(v5[6] == 36);
    require(v5[7] == 2);

    require(v6[0] == 0);
    require(v6[1] == 0);
    require(v6[2] == 0);
    require(v6[3] == 0);
    require(v6[4] == 0);
    require(v6[5] == 0);
    require(v6[6] == 0);
    require(v6[7] == 2);

    std::cout << "OK" << std::endl;
}