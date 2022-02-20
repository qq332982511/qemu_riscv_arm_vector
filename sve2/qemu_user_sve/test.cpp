#include <arm_sve.h>
#include <stdio.h>
#include <stdlib.h>
// Scalar version.
void add_arrays(double *dst, double *src, double c, const int N) {
  for (int i = 0; i < N; i++)
    dst[i] = src[i] + c;
}

// Vector version
void vla_add_arrays(double *dst, double *src, double c, const int N) {
  int64_t i = 0;

  svbool_t pg = svwhilelt_b64(i, (int64_t)N);
  while (svptest_any(svptrue_b64(), pg)) {
    svfloat64_t vsrc = svld1(pg, src + i);
    svfloat64_t vdst = svadd_x(pg, vsrc, c);
    svst1(pg, dst + i, vdst);

    i += svcntd();
    pg = svwhilelt_b64(i, (int64_t)N);
  }
}

// Vector version
void vla_add_arrays_2(double *dst, double *src, double c, const int N) {
  for (int i = 0; i < N; i += svcntd()) {
    svbool_t Pg = svwhilelt_b64(i, N);
    svfloat64_t vsrc = svld1(Pg, &src[i]);
    svfloat64_t vdst = svadd_x(Pg, vsrc, c);
    svst1(Pg, &dst[i], vdst);
  }
}

// Vector version
void vla_add_arrays_3(double *dst, double *src, double c, const int N) {
  int elem_per_simd = svcntd();
  int n_end = N / elem_per_simd * elem_per_simd;
  svbool_t Pg = svptrue_b64();
  for (int i = 0; i < n_end; i += elem_per_simd) {
    svfloat64_t vsrc = svld1(Pg, &src[i]);
    svfloat64_t vdst = svadd_x(Pg, vsrc, c);
    svst1(Pg, &dst[i], vdst);
  }
  Pg = svwhilelt_b64(n_end, N);
  svfloat64_t vsrc = svld1(Pg, &src[n_end]);
  svfloat64_t vdst = svadd_x(Pg, vsrc, c);
  svst1(Pg, &dst[n_end], vdst);
}

void run_test() {
  int nr_elem = 33;
  double src[nr_elem];
  double c;
  double dst_serial[nr_elem], dst_vla[nr_elem], dst_vla2[nr_elem], dst_vla3[nr_elem];
  for (int i = 0; i < nr_elem; ++i) {
    src[i] = (double)i / ((double)i + 1);
  }

  c = src[rand() % nr_elem];

  add_arrays(dst_serial, src, c, nr_elem);
  vla_add_arrays(dst_vla, src, c, nr_elem);
  vla_add_arrays_2(dst_vla2, src, c, nr_elem);
  vla_add_arrays_3(dst_vla3, src, c, nr_elem);

  for (int i = 0; i < nr_elem; ++i) {
    printf("%f %f %f %f, [%f, %f]\n", dst_serial[i], dst_vla[i], dst_vla2[i], dst_vla3[i], src[i], c);
  }
}
int main() { 
  run_test();
  return 0;
}