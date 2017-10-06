#ifndef IMAGE_BITMAP_H_
#define IMAGE_BITMAP_H_

#include <memory>

class Bitmap {
 public:
  struct Color {
    Color() {
      r = g = b = 0;
    }

    uchar b;
    uchar g;
    uchar r;
  };

  Bitmap(uint width, uint height);
  Bitmap(const Bitmap&) = delete;
  Bitmap& operator=(const Bitmap&) = delete;
  ~Bitmap();

  void Set(uint x, uint y, const Color& color);

  void AntiAlias();
  bool Save(const char* file_name) const;

 protected:
  struct HeaderMagic {
    HeaderMagic() {
      magic[0] = 'B';
      magic[1] = 'M';
    }

    uchar magic[2];
  };

  struct Header {
    Header() {
      reserved1 = 0;
      reserved2 = 0;
      data_offset = 54;
    }

    uint file_size;
    ushort reserved1;
    ushort reserved2;
    uint data_offset;
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

    uint info_header_size;
    uint width;
    uint height;
    ushort planes;
    ushort bpp;
    uint compression;
    uint data_size;
    uint horiz_res;
    uint vert_res;
    uint num_colors;
    uint num_imp_colors;
  };

  uint width() const { return width_; }
  uint height() const { return height_; }
  const Color* data() const { return data_.get(); }

 private:
  uint width_;
  uint height_;
  std::unique_ptr<Color[]> data_;
};

#endif  // IMAGE_BITMAP_H_
