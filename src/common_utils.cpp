#include "common_utils.h"

BPP get_image_bpp(const QImage &img) {
  BPP result;
  int bpp_depth = img.bitPlaneCount();
  result = (bpp_depth == 8) ? BPP::bpp_8 : BPP::bpp_24;
  return result;
}
