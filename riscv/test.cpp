#include <riscv_vector.h>
#include <stdio.h>
#include <stdlib.h>
// Scalar version.
void add_arrays(float *dst, float *src, float c, const int N) {
  for (int i = 0; i < N; i++)
    dst[i] = src[i] + c;
}

// // Vector version
void vla_add_arrays(float *dst, float *src, float c, const int N) {
  int n = N;
  while (n > 0) {
    int vl = (int)vsetvl_e32m1(n);
    vfloat32m1_t vsrc = vle32_v_f32m1(src, vl);
    vfloat32m1_t vdst = vfadd_vf_f32m1(vsrc, c, vl);
    vse32_v_f32m1(dst, vdst, vl);
    src += vl;
    dst += vl;
    n -= vl;
  }
}

// // Vector version
void vla_add_arrays_2(float *dst, float *src, float c, const int N) {
  int elem_per_simd = (int)vsetvlmax_e32m1();
  for (int i = 0; i < N; i += elem_per_simd) {
    int vl = (int)vsetvl_e32m1(N - i);
    vfloat32m1_t vsrc = vle32_v_f32m1(&src[i], vl);
    vfloat32m1_t vdst = vfadd_vf_f32m1(vsrc, c, vl);
    vse32_v_f32m1(&dst[i], vdst, vl);
  }
}

// // Vector version
void vla_add_arrays_3(float *dst, float *src, float c, const int N) {
  int elem_per_simd = (int)vsetvlmax_e32m1();
  int n_end = N / elem_per_simd * elem_per_simd;
  int n_remain = N - n_end;

  for (int i = 0; i < n_end; i += elem_per_simd) {
    vfloat32m1_t vsrc = vle32_v_f32m1(&src[i], elem_per_simd);
    vfloat32m1_t vdst = vfadd_vf_f32m1(vsrc, c, elem_per_simd);
    vse32_v_f32m1(&dst[i], vdst, elem_per_simd);
  }
  if (n_remain > 0) {
    vfloat32m1_t vsrc = vle32_v_f32m1(&src[n_end], n_remain);
    vfloat32m1_t vdst = vfadd_vf_f32m1(vsrc, c, n_remain);
    vse32_v_f32m1(&dst[n_end], vdst, n_remain);
  }
}

void run_test() {

  int nr_elem = 33;
  float src[nr_elem];
  float c;
  float dst_serial[nr_elem], dst_vla[nr_elem], dst_vla2[nr_elem], dst_vla3[nr_elem];
  for (int i = 0; i < nr_elem; ++i) {
    src[i] = (float)i / ((float)i + 1);
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
  printf("Hello world!\n");
  run_test();
  printf("Bye world!\n");
  return 0;
}