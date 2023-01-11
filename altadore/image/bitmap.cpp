#include "altadore/image/bitmap.h"

#include <assert.h>
#include <cstdio>
#include <memory>

Bitmap::Bitmap(int width, int height) : width_(width), height_(height) {
  int size = width * height;
  data_ = std::make_unique<Color[]>(size);
}

Bitmap::~Bitmap() {
}

void Bitmap::Set(int x, int y, const Color& color) {
  assert(x < width_);
  assert(y < height_);

  int offset = width_ * y + x;
  data_[offset] = color;
}

void Bitmap::AntiAlias() {
  assert(width_ % 2 == 0);
  assert(height_ % 2 == 0);

  int new_width = width_ / 2;
  int new_height = height_ / 2;

  int size = new_width * new_height;
  auto new_data = std::make_unique<Color[]>(size);

  for (int x = 0; x < width_; x += 2) {
    for (int y = 0; y < height_; y += 2) {
      ushort r = 0, g = 0, b = 0;

      for (int i = x; i < x+2; ++i) {
        for (int j = y; j < y+2; ++j) {
          int offset = width_ * j + i;
          const Color& color = data_[offset];
          r += color.r;
          g += color.g;
          b += color.b;
        }
      }

      int offset = new_width * (y/2) + (x/2);
      Color& color = new_data[offset];
      color.r = r / 4;
      color.g = g / 4;
      color.b = b / 4;
    }
  }

  width_ = new_width;
  height_ = new_height;
  data_ = std::move(new_data);
}

bool Bitmap::Save(const char* file_name) const {
  std::unique_ptr<std::FILE, decltype(&std::fclose)> file(
      std::fopen(file_name, "wb"), &std::fclose);
  if (!file)
    return false;

  HeaderMagic header_magic;
  if (std::fwrite(&header_magic, sizeof(HeaderMagic), 1, file.get()) != 1)
    return false;

  // The row size must be 4-byte aligned.
  int row_size = width_ * sizeof(Color);
  int padding = (row_size % 4 == 0 ? 0 : 4 - (row_size % 4));
  int data_size = (row_size + padding) * height_;

  Header header;
  header.file_size = sizeof(HeaderMagic) +
                     sizeof(Header) +
                     sizeof(InfoHeader) +
                     data_size;
  if (std::fwrite(&header, sizeof(Header), 1, file.get()) != 1)
    return false;

  InfoHeader info_header;
  info_header.width = width_;
  info_header.height = height_;
  info_header.data_size = data_size;
  if (std::fwrite(&info_header, sizeof(InfoHeader), 1, file.get()) != 1)
    return false;

  // Bitmap format requires that the rows be written in reverse order.
  static char zero_buffer[] = {'\0', '\0', '\0'};
  for (int y = height_ - 1; y >= 0; --y) {
    int offset = y * width_;
    if (std::fwrite(data_.get() + offset, row_size, 1, file.get()) != 1)
      return false;
    if (padding != 0 && fwrite(zero_buffer, padding, 1, file.get()) != 1)
      return false;
  }

  return true;
}
