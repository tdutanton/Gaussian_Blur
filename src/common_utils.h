#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <QImage>
#include <QtGlobal>

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

template <typename T>
inline auto getPixmap(T* label) -> decltype(auto) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  return label->pixmap();
#else
  return *(label->pixmap());
#endif
}

#endif  // COMMON_UTILS_H
