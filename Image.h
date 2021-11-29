//
// Created by Jiang Kairong on 4/14/18.
//

#ifndef PROG05_IMAGE_H
#define PROG05_IMAGE_H

#include "Color.h"
using Image32f = Matrix<ColorRGB32f>;
using Image8i = Matrix<ColorRGB8i>;

/// Utility class for images
class ImageUtils {
 public:
  /// convert a 32-bit float point image to an 8-bit integer image
  /// \param image float point image
  /// \return converted 8 bit image
  static std::shared_ptr<Image8i> convertFloatImage2Int(const Image32f &image) {
    auto result = std::make_shared<Image8i>();
    result->resize(image.rows(), image.cols());
    for (int i = 0; i < result->rows(); i++) {
      for (int j = 0; j < result->cols(); j++) {
        Vector3f scaledPixel = image(i, j) * 255.f;
        for (int k = 0; k < 3; k++) {
          scaledPixel(k) = std::min(255.f, scaledPixel(k));
          scaledPixel(k) = std::max(0.f, scaledPixel(k));
        }
        (*result)(i, j) = ColorRGB8i({static_cast<unsigned char>(scaledPixel(0)),
                                   static_cast<unsigned char>(scaledPixel(1)),
                                   static_cast<unsigned char>(scaledPixel(2))});
      }
    }
    return result;
  }
  /// clear image
  /// \param image original image
  static void clearImage(Image32f &image) {
    for (int i = 0; i < image.rows(); i++) {
      for (int j = 0; j < image.cols(); j++) {
        image(i, j) = ColorRGB32f({0.f, 0.f, 0.f});
      }
    }
  }

  /// get the raw data of an 8-bit image
  /// \param image
  /// \return pointer to the data chunk
  static unsigned char *getRawData(const Image8i &image) {
    auto *data = new unsigned char[image.cols() * image.rows() * 3];
    for (int i = 0; i < image.rows(); i++) {
      for (int j = 0; j < image.cols(); j++) {
        auto pos = static_cast<int>((i * image.cols() + j) * 3);
        data[pos] = image(i, j)(0);
        data[pos + 1] = image(i, j)(1);
        data[pos + 2] = image(i, j)(2);
      }
    }
    return data;
  }
};

#endif //PROG05_IMAGE_H
