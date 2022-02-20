#include <arm_sve.h>
#include <stdio.h>
#include <stdlib.h>

void daxpy(int64_t n, double da, double *dx, int64_t incx, double *dy, int64_t incy) {
  svint64_t incx_vec = svindex_s64(0, incx); // [1]
  svint64_t incy_vec = svindex_s64(0, incy); // [1]
  int64_t i = 0;
  svbool_t pg = svwhilelt_b64(i, n); // [2]
  do {
    svfloat64_t dx_vec = svld1_gather_index(pg, dx, incx_vec);              // [3]
    svfloat64_t dy_vec = svld1_gather_index(pg, dy, incy_vec);              // [3]
    svst1_scatter_index(pg, dy, incy_vec, svmla_x(pg, dy_vec, dx_vec, da)); // [4]
    dx += incx * svcntd();                                                  // [5]
    dy += incy * svcntd();                                                  // [5]
    i += svcntd();                                                          // [6]
    pg = svwhilelt_b64(i, n);                                               // [2]
  } while (svptest_any(svptrue_b64(), pg));                                 // [7]
}

// Scalar version.
void add_arrays(double *dst, double *src, double c, const int N) {
  for (int i = 0; i < N; i++)
    dst[i] = src[i] + c;
}

// // Vector version
// void vla_add_arrays(double *dst, double *src, double c, const int N) {
//   int64_t i = 0;

//   svbool_t pg = svwhilelt_b64(i, (int64_t)N);
//   while (svptest_any(svptrue_b64(), pg)) {
//     svfloat64_t vsrc = svld1(pg, src + i);
//     svfloat64_t vdst = svadd_x(pg, vsrc, c);
//     svst1(pg, dst + i, vdst);

//     i += svcntd();
//     pg = svwhilelt_b64(i, (int64_t)N);
//     printf("n=%ld\n", i);
//   }
// }

// Vector version
void vla_add_arrays_2(double *dst, double *src, double c, const int N) {
  for (int i = 0; i < N; i += svcntd()) {
    // svbool_t Pg = svwhilelt_b64(i, N);
    // svfloat64_t vsrc = svld1(Pg, &src[i]);
    // svfloat64_t vdst = svadd_x(Pg, vsrc, c);
    // svst1(Pg, &dst[i], vdst);
    printf("n=%d\n", i);
  }
}

int main(void) {
  double src[100];
  double c;
  double dst_serial[100], dst_vla[100], dst_vla2[100];
  for (int i = 0; i < 100; ++i) {
    src[i] = (double)i / ((double)i + 1);
  }

  c = src[rand() % 100];

  add_arrays(dst_serial, src, c, 100);
  // vla_add_arrays(dst_vla, src, c, 100);
  vla_add_arrays_2(dst_vla2, src, c, 100);

  for (int i = 0; i < 100; ++i) {
    printf("%f %f %f, %f, %f\n", dst_serial[i], dst_vla[i], dst_vla2[i], src[i], c);
  }
  return 0;
}