#ifndef ONE_D_KERNEL_H
#define ONE_D_KERNEL_H

#define _USE_MATH_DEFINES
#include <math.h>

#include <sstream>
#include <stdexcept>
#include <vector>

#include "common_utils.h"

class OneDKernel {
 public:
  OneDKernel(unsigned short radius);
  ~OneDKernel() = default;

  unsigned get_kernel_size() const { return kernel_size_; }
  double get_kernel_value(size_t index) const { return kernel_[index]; }
  std::vector<double> get_kernel() const { return kernel_; }
  double kernel_value(unsigned short radius, int position);

 private:
  OneDKernel() = delete;

  unsigned kernel_size_;
  std::vector<double> kernel_;
  std::vector<double> create_kernel_(unsigned short radius,
                                     unsigned kernel_size);
  double calc_sigma(unsigned short radius);
  unsigned calc_kernel_size(double sigma);
  void normalize_kernel(std::vector<double> vec, double sum);
};

#endif  // ONE_D_KERNEL_H
