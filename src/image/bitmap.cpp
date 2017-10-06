#include "image/bitmap.h"

#include <string.h>

Bitmap::Bitmap(uint width, uint height) : width_(width), height_(height) {
  uint size = width * height;
  data_.reset(new Color[size]);
}

Bitmap::~Bitmap() {
}

void Bitmap::Set(uint x, uint y, const Color& color) {
  DCHECK(x < width_);
  DCHECK(y < height_);

  uint offset = width_ * y + x;
  data_[offset] = color;
}

void Bitmap::AntiAlias() {
  DCHECK(width_ % 2 == 0);
  DCHECK(height_ % 2 == 0);

  uint new_width = width_ / 2;
  uint new_height = height_ / 2;

  uint size = new_width * new_height;
  std::unique_ptr<Color[]> new_data(new Color[size]);

  for (uint x = 0; x < width_; x += 2) {
    for (uint y = 0; y < height_; y += 2) {
      ushort r = 0, g = 0, b = 0;

      for (uint i = x; i < x+2; ++i) {
        for (uint j = y; j < y+2; ++j) {
          uint offset = width_ * j + i;
          const Color& color = data_[offset];
          r += color.r;
          g += color.g;
          b += color.b;
        }
      }

      uint offset = new_width * (y/2) + (x/2);
      Color& color = new_data[offset];
      color.r = r / 4;
      color.g = g / 4;
      color.b = b / 4;
    }
  }

  width_ = new_width;
  height_ = new_height;
  data_.reset(new_data.release());
}

bool Bitmap::Save(const char* file_name) const {
  scoped_FILE file(OpenFile(file_name, "wb"));
  if (file.get() == NULL)
    return false;

  HeaderMagic header_magic;
  if (fwrite(&header_magic, sizeof(HeaderMagic), 1, file.get()) != 1)
    return false;

  // The row size must be 4-byte aligned.
  uint row_size = width_ * sizeof(Color);
  uint padding = (row_size % 4 == 0 ? 0 : 4 - (row_size % 4));
  uint data_size = (row_size + padding) * height_;

  Header header;
  header.file_size = sizeof(HeaderMagic) +
                     sizeof(Header) +
                     sizeof(InfoHeader) +
                     data_size;
  if (fwrite(&header, sizeof(Header), 1, file.get()) != 1)
    return false;

  InfoHeader info_header;
  info_header.width = width_;
  info_header.height = height_;
  info_header.data_size = data_size;
  if (fwrite(&info_header, sizeof(InfoHeader), 1, file.get()) != 1)
    return false;

  // Bitmap format requires that the rows be written in reverse order.
  static char zero_buffer[] = {'\0', '\0', '\0'};
  for (int y = height_ - 1; y >= 0; --y) {
    uint offset = y * width_;
    if (fwrite(data_.get() + offset, row_size, 1, file.get()) != 1)
      return false;
    if (padding != 0 && fwrite(zero_buffer, padding, 1, file.get()) != 1)
      return false;
  }

  return true;
}
