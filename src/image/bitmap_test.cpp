#include "image/bitmap.h"

#include <cstdio>
#include "third_party/googletest/googletest/include/gtest/gtest.h"

const char kFileName[] = "test.bmp";

class TestBitmap : public Bitmap {
 public:
  TestBitmap(int width, int height) : Bitmap(width, height) {
  }

  using Bitmap::HeaderMagic;
  using Bitmap::Header;
  using Bitmap::InfoHeader;
  using Bitmap::width;
  using Bitmap::height;
  using Bitmap::data;
};

class BitmapTest : public testing::Test {
 protected:
  void TearDown() {
    std::remove(kFileName);
  }
};

TEST_F(BitmapTest, Constructor) {
  TestBitmap bitmap(2, 3);
  EXPECT_EQ(2, bitmap.width());
  EXPECT_EQ(3, bitmap.height());
  EXPECT_NE(nullptr, bitmap.data());
}

TEST_F(BitmapTest, Set) {
  TestBitmap bitmap(2, 3);

  Bitmap::Color color;
  color.r = 0xFF;
  color.g = 0x7F;
  color.b = 0x00;
  bitmap.Set(1, 0, color);
  EXPECT_EQ(0, memcmp(&bitmap.data()[1], &color, sizeof(Bitmap::Color)));

  color.r = 0x00;
  color.g = 0x7F;
  color.b = 0xFF;
  bitmap.Set(1, 2, color);
  EXPECT_EQ(0, memcmp(&bitmap.data()[5], &color, sizeof(Bitmap::Color)));
}

TEST_F(BitmapTest, AntiAlias) {
  TestBitmap bitmap(2, 4);

  Bitmap::Color color;
  color.r = 160;
  color.g = 80;
  color.b = 40;
  bitmap.Set(1, 0, color);

  color.r = 120;
  color.g = 60;
  color.b = 20;
  bitmap.Set(0, 3, color);

  bitmap.AntiAlias();

  EXPECT_EQ(1, bitmap.width());
  EXPECT_EQ(2, bitmap.height());

  color.r = 40;
  color.g = 20;
  color.b = 10;
  EXPECT_EQ(0, memcmp(&bitmap.data()[0], &color, sizeof(Bitmap::Color)));

  color.r = 30;
  color.g = 15;
  color.b = 5;
  EXPECT_EQ(0, memcmp(&bitmap.data()[1], &color, sizeof(Bitmap::Color)));
}

TEST_F(BitmapTest, Save) {
  Bitmap bitmap(2, 3);
  Bitmap::Color white;
  white.r = white.g = white.b = 0xFF;
  bitmap.Set(0, 0, white);
  bitmap.Set(1, 0, white);
  EXPECT_TRUE(bitmap.Save(kFileName));

  std::unique_ptr<std::FILE, decltype(&std::fclose)> file(
      std::fopen(kFileName, "rb"), &std::fclose);
  EXPECT_NE(nullptr, file.get());

  TestBitmap::HeaderMagic header_magic_buffer, header_magic;
  EXPECT_EQ(1U, fread(&header_magic_buffer, sizeof(TestBitmap::HeaderMagic), 1, file.get()));
  EXPECT_EQ(0, memcmp(&header_magic_buffer, &header_magic, sizeof(TestBitmap::HeaderMagic)));

  // 6 byte wide rows, padded by 2 bytes, times 3 rows
  int data_size = (6 + 2) * 3;
  TestBitmap::Header header_buffer, header;
  header.file_size = sizeof(TestBitmap::HeaderMagic) +
                     sizeof(TestBitmap::Header) +
                     sizeof(TestBitmap::InfoHeader) +
                     data_size;
  EXPECT_EQ(1U, fread(&header_buffer, sizeof(TestBitmap::Header), 1, file.get()));
  EXPECT_EQ(0, memcmp(&header_buffer, &header, sizeof(TestBitmap::Header)));

  TestBitmap::InfoHeader info_header_buffer, info_header;
  info_header.width = 2;
  info_header.height = 3;
  info_header.data_size = data_size;
  EXPECT_EQ(1U, fread(&info_header_buffer, sizeof(TestBitmap::InfoHeader), 1, file.get()));
  EXPECT_EQ(0, memcmp(&info_header_buffer, &info_header, sizeof(TestBitmap::InfoHeader)));

  // 4 black pixels followed by 2 white.
  TestBitmap::Color color_buffer, black;
  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 2; ++x) {
      EXPECT_EQ(1U, fread(&color_buffer, sizeof(TestBitmap::Color), 1, file.get()));
      if (y == 2) {
        EXPECT_EQ(0, memcmp(&color_buffer, &white, sizeof(TestBitmap::Color)));
      } else {
        EXPECT_EQ(0, memcmp(&color_buffer, &black, sizeof(TestBitmap::Color)));
      }
    }
    EXPECT_EQ(0, fseek(file.get(), 2, SEEK_CUR));
  }
}
