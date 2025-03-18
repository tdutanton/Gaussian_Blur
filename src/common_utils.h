#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <QImage>

enum class BPP {
  bpp_8 = 8,
  bpp_24 = 24,
};

enum class dialog_type {
  error,
  info,
};

static constexpr unsigned short min_radius_ = 2;
static constexpr unsigned short max_radius_ = 20;

static constexpr unsigned short default_height_ = 600;
static constexpr unsigned short default_width_ = 800;

BPP get_image_bpp(const QImage& img);

#endif  // COMMON_UTILS_H
