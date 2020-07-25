#ifndef PERLIN_H_
#define PERLIN_H_

#include <math.h>

#include "random.h"
#include "utility.h"
#include "vec3.h"

namespace rtx {
class perlin {
public:
  perlin(rtx::random &r) {
    ranvec_ = new vec3[POINT_COUNT];
    for (int i = 0; i < POINT_COUNT; ++i) {
      ranvec_[i] = unit_vector(r.random_vec3(-1, 1));
    }

    perm_x_ = perlin_generate_perm(r);
    perm_y_ = perlin_generate_perm(r);
    perm_z_ = perlin_generate_perm(r);
  }

  ~perlin() {
    delete[] ranvec_;
    delete[] perm_x_;
    delete[] perm_y_;
    delete[] perm_z_;
  }

  float noise(const point3 &p) const {
    float u = p.x() - floor(p.x());
    float v = p.y() - floor(p.y());
    float w = p.z() - floor(p.z());

    int i = std::floor(p.x());
    int j = std::floor(p.y());
    int k = std::floor(p.z());

    vec3 c[2][2][2];

    for (int di = 0; di < 2; ++di) {
      for (int dj = 0; dj < 2; ++dj) {
        for (int dk = 0; dk < 2; ++dk) {
          c[di][dj][dk] =
              ranvec_[perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^
                      perm_z_[(k + dk) & 255]];
        }
      }
    }
    return perlin_interpolate(c, u, v, w);
  }

  float turbulence(const point3 &p, int depth = 7) const {
    float accumulated = 0.0;
    point3 temp_p = p;
    float weight = 1.0;

    for (int i = 0; i < depth; ++i) {
      accumulated += weight * noise(temp_p);
      weight *= 0.5;
      temp_p *= 2;
    }
    return std::abs(accumulated);
  }

private:
  static constexpr int POINT_COUNT = 256;
  vec3 *ranvec_;
  int *perm_x_;
  int *perm_y_;
  int *perm_z_;

  static int *perlin_generate_perm(rtx::random &r) {
    int *p = new int[POINT_COUNT];

    for (int i = 0; i < POINT_COUNT; ++i) {
      p[i] = i;
    }

    permute(p, POINT_COUNT, r);

    return p;
  }

  static void permute(int *p, int n, rtx::random &r) {
    for (int i = 0; i < n; ++i) {
      int target = r.random_int(0, i);
      int tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }

  static float trilinear_interpolate(float c[2][2][2], float u, float v,
                                     float w) {
    float accumulated = 0.0;
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
          accumulated += (i * u + (1 - i) * (1 - u)) *
                         (j * v + (1 - j) * (1 - v)) *
                         (k * w + (1 - k) * (1 - w)) * c[i][j][k];
        }
      }
    }

    return accumulated;
  }

  static float perlin_interpolate(vec3 c[2][2][2], float u, float v, float w) {

    float uu = u * u * (3 - 2 * u);
    float vv = v * v * (3 - 2 * v);
    float ww = w * w * (3 - 2 * w);
    float accumulated = 0.0;

    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
          vec3 weight_v(u - i, v - j, w - k);
          accumulated +=
              (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) *
              (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
        }
      }
    }
    return accumulated;
  }
};

} // namespace rtx
#endif // PERLIN_H_
