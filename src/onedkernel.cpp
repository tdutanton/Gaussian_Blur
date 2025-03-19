#include "onedkernel.h"

OneDKernel::OneDKernel(unsigned short radius) {
  double sigma = calc_sigma(radius);
  kernel_size_ = calc_kernel_size(sigma);
  kernel_ = create_kernel_(radius, kernel_size_);
}

double OneDKernel::kernel_value(unsigned short radius, int position) {
  double sigma = static_cast<double>(radius) / 2;
  return (1 / sqrt(2 * M_PI * pow(sigma, 2)) *
          exp(-(pow(position, 2) / (2 * pow(sigma, 2)))));
}

std::vector<double> OneDKernel::create_kernel_(unsigned short radius,
                                               unsigned kernel_size) {
  unsigned center = kernel_size / 2;
  double sum{0};
  std::vector<double> result;
  for (unsigned i = 0; i < kernel_size; i++) {
    int pos = i - center;
    double value = kernel_value(radius, pos);
    result.emplace_back(value);
    sum += value;
  }

  normalize_kernel(result, sum);
  return result;
}

double OneDKernel::calc_sigma(unsigned short radius) {
  return static_cast<double>(radius) / 2;
}

unsigned OneDKernel::calc_kernel_size(double sigma) {
  return 2 * static_cast<unsigned>(ceil(3 * sigma)) + 1;
}

void OneDKernel::normalize_kernel(std::vector<double> vec, double sum) {
  for (auto& i : vec) {
    i /= sum;
  }
}
