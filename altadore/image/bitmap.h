#ifndef IMAGE_BITMAP_H_
#define IMAGE_BITMAP_H_

#include <cstdint>
#include <memory>

class Bitmap {
 public:
  struct Color {
    Color() {
      r = g = b = 0;
    }

    uint8_t b;
    uint8_t g;
    uint8_t r;
  };

  Bitmap(int width, int height);
  Bitmap(const Bitmap&) = delete;
  Bitmap& operator=(const Bitmap&) = delete;
  ~Bitmap();

  void Set(int x, int y, const Color& color);

  void AntiAlias();
  bool Save(const char* file_name) const;

 protected:
  struct HeaderMagic {
    HeaderMagic() {
      magic[0] = 'B';
      magic[1] = 'M';
    }

    char magic[2];
  };

  struct Header {
    Header() {
      reserved1 = 0;
      reserved2 = 0;
      data_offset = 54;
    }

    int file_size;
    ushort reserved1;
    ushort reserved2;
    int data_offset;
  };

  struct InfoHeader {
    InfoHeader() {
      info_header_size = sizeof(InfoHeader);
      planes = 1;
      bpp = 24;
      compression = 0;
      horiz_res = 0;
      vert_res = 0;
      num_colors = 0;
      num_imp_colors = 0;
    }

    int info_header_size;
    int width;
    int height;
    ushort planes;
    ushort bpp;
    int compression;
    int data_size;
    int horiz_res;
    int vert_res;
    int num_colors;
    int num_imp_colors;
  };

  int width() const { return width_; }
  int height() const { return height_; }
  const Color* data() const { return data_.get(); }

 private:
  int width_;
  int height_;
  std::unique_ptr<Color[]> data_;
};

#endif  // IMAGE_BITMAP_H_
