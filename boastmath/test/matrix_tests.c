#include <string.h>
#include "include/boastmath/matrix.h"
#include "include/testboat.h"
#include "include/boastmath/vector.h"

void vec4_dump(bm_vec4 v) {
    printf("[%f %f %f %f]",
        v[0], v[1], v[2], v[3]
    );
}

void mat4_dump(bm_mat4 mat) {
    for (int i = 0; i < 4; ++i) {
        printf("[%f %f %f %f]\n",
            bm_m(mat, i, 0), bm_m(mat, i, 1),
            bm_m(mat, i, 2), bm_m(mat, i, 3)
        );
    }
}

void test_mat4_x_vec4() {
    // Given
    bm_mat4 mat = {
        0.5, 1.0, 1.5, 2.0,
        1.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0
    };

    bm_vec4 vec = {
        1.0,
        2.0,
        3.0,
        4.0
    };

    bm_vec4 expected = {
        0.5 + 2.0 + 4.5 + 8.0,
        1.0 + 2.0 + 3.0 + 4.0,
        0.0 + 2.0 + 0.0 + 0.0,
        0.0 + 0.0 + 0.0 + 0.0
    };

    bm_vec4 actual;

    // When
    bm_mattrans(actual, mat, vec);

    // Then
    test_assert(memcmp(expected, actual, sizeof(bm_vec4)) == 0,
        "Result vector should be expected");

    printf("Vector:");
    vec4_dump(vec);

    printf("\nMatrix:\n");
    mat4_dump(mat);

    printf("Actual: ");
    vec4_dump(actual);
    printf("\nExpected: ");
    vec4_dump(expected);
    printf("\n");
}

void test_mat4_x_identity() {
    // Given
    bm_mat4 a = {
        0.5, 1.0, 1.5, 2.0,
        1.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0
    };

    bm_mat4 identity = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    bm_mat4 expected = {
        0.5, 1.0, 1.5, 2.0,
        1.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0
    };

    bm_mat4 actual;

    // When
    bm_matmul(actual, a, identity);

    // Then
    test_assert(memcmp(expected, actual, sizeof(bm_mat4)) == 0,
        "Result matrix should be expected");

    printf("Actual: \n");
    mat4_dump(actual);
    printf("Expected: \n");
    mat4_dump(expected);
}

void test_lookat () {
    // Given
    bm_vec4 from = {1.0, 0.0, 0.0, 1.0};
    bm_vec4 at = {1.0, 1.0, 0.0, 1.0};
    bm_vec4 up = {0.0, 0.0, 1.0, 1.0};

    bm_vec4 front = {1.0, 2.0, 0.0, 1.0};
    bm_vec4 left = {-1.0, 0.0, 0.0, 1.0};
    bm_vec4 top = {1.0, 0.0, 1.0, 1.0};

    bm_vec4 expectedFront = {0.0, 0.0, -2.0, 1.0};
    bm_vec4 expectedLeft = {-2.0, 0.0, 0.0, 1.0};
    bm_vec4 expectedUp = {0.0, 1.0, 0.0, 1.0};

    bm_vec4 actualFront, actualLeft, actualUp;

    // When
    bm_mat4 mat;
    bm_mat_lookat(mat,
        bm_spread_vec3(from),
        bm_spread_vec3(at),
        bm_spread_vec3(up)
    );

    bm_mattrans(actualFront, mat, front);
    bm_mattrans(actualLeft, mat, left);
    bm_mattrans(actualUp, mat, top);

    // Then
    test_assert(memcmp(expectedFront, actualFront, sizeof(bm_vec4)) == 0,
        "Front result should be expected");
    test_assert(memcmp(expectedLeft, actualLeft, sizeof(bm_vec4)) == 0,
        "Left result should be expected");
    test_assert(memcmp(expectedUp, actualUp, sizeof(bm_vec4)) == 0,
        "Up result should be expected");

    printf("Matrix:\n");
    mat4_dump(mat);

    printf("Camera: ");
    vec4_dump(from);
    vec4_dump(at);
    vec4_dump(up);

    printf("\nTransform: ");
    vec4_dump(front);
    vec4_dump(left);
    vec4_dump(top);

    printf("\nActual: ");
    vec4_dump(actualFront);
    vec4_dump(actualLeft);
    vec4_dump(actualUp);

    printf("\nExpected: ");
    vec4_dump(expectedFront);
    vec4_dump(expectedLeft);
    vec4_dump(expectedUp);
    printf("\n");
}

int main() {
    test("mat4 x vec4",
        test_mat4_x_vec4();
    );

    test("mat4 x identity",
        test_mat4_x_identity();
    );

    test("lookat front",
        test_lookat();
    );

    return test_summarize();
}
