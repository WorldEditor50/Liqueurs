#ifndef BMP_HPP
#define BMP_HPP
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <string.h>

#pragma pack(push, 1)
struct BmpHead {
    uint16_t type;
    uint32_t filesize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};
#pragma pack(pop)

#pragma pack(push 1)
struct BmpInformation {
    uint32_t infosize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t depth;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t x;
    uint32_t y;
    uint32_t colorused;
    uint32_t colorimportant;
};
#pragma pack(pop)

class BMP
{
public:
    class Image
    {
    public:
        int h;
        int w;
        int c;
        int widthstep;
        std::size_t totalSize;
        uint8_t* data;
    public:
        Image():h(0),w(0),c(0),widthstep(0),totalSize(0),data(nullptr){}
        Image(int h_, int w_, int c_):h(h_),w(w_),c(c_)
        {
            widthstep = (w*c+3)/4*4;
            totalSize = widthstep*h;
            data = new uint8_t[totalSize];
            memset(data, 0, totalSize);
        }
        Image(std::size_t totalSize_)
            :h(0),w(0),c(0),widthstep(0),totalSize(totalSize_)
        {
            data = new uint8_t[totalSize];
            memset(data, 0, totalSize);
        }
        Image(const Image &r)
            :h(r.h),w(r.w),c(r.c),totalSize(0),widthstep(0),data(nullptr)
        {
            if (r.data != nullptr) {
                widthstep = (w*c+3)/4*4;
                totalSize = widthstep*h;
                data = new uint8_t[totalSize];
                memcpy(data, r.data, totalSize);
            }
        }
        Image& operator=(const Image &r)
        {
            if (this == &r) {
                return *this;
            }
            if (r.data == nullptr) {
                return *this;
            }
            h = r.h;
            w = r.w;
            c = r.c;
            widthstep = r.widthstep;
            if (data != nullptr) {
                if (totalSize != r.totalSize) {
                    delete [] data;
                    totalSize = r.totalSize;
                    data = new uint8_t[totalSize];
                }
            } else {
                totalSize = r.totalSize;
                data = new uint8_t[totalSize];
            }
            memcpy(data, r.data, totalSize);
            return *this;
        }

        Image(Image &&r)
            :h(r.h),w(r.w),c(r.c),
              widthstep(r.widthstep),totalSize(r.totalSize),data(r.data)
        {
            r.h = 0;
            r.w = 0;
            r.c = 0;
            r.widthstep = 0;
            r.totalSize = 0;
            r.data = nullptr;
        }
        Image& operator=(Image &&r)
        {
            if (this == &r) {
                return *this;
            }
            if (r.data == nullptr) {
                return *this;
            }
            h = r.h;
            w = r.w;
            c = r.c;
            widthstep = r.widthstep;
            totalSize = r.totalSize;
            data = r.data;
            r.h = 0;
            r.w = 0;
            r.c = 0;
            r.widthstep = 0;
            r.totalSize = 0;
            r.data = nullptr;
            return *this;
        }

        ~Image()
        {
            if (data != nullptr) {
                delete [] data;
                data = nullptr;
            }
        }
        bool empty() const {return data == nullptr || totalSize == 0;}
        uint8_t operator()(int i, int j, int k)
        {
            return data[i*widthstep + j*c + k];
        }
        uint8_t* operator()(int i, int j)
        {
            return data + i*widthstep + j*c;
        }

        void fill(uint8_t val)
        {
            for (std::size_t i = 0; i < totalSize; i++) {
                data[i] = val;
            }
            return;
        }
        int fill(uint8_t r, uint8_t g, uint8_t b)
        {
            if (c != 3) {
                return -1;
            }
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    data[i*widthstep + j*c]     = r;
                    data[i*widthstep + j*c + 1] = g;
                    data[i*widthstep + j*c + 2] = b;
                }
            }
            return 0;
        }

        int fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            if (c != 4) {
                return -1;
            }
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    data[i*widthstep + j*c]     = r;
                    data[i*widthstep + j*c + 1] = g;
                    data[i*widthstep + j*c + 2] = b;
                    data[i*widthstep + j*c + 3] = a;
                }
            }
            return 0;
        }
    };
public:
    inline static int align4(int width, int channel) {return  (width*channel+3)/4*4;}

    static uint32_t size(int h, int w, int c) {return 54 + h * align4(w, 3);}

    static int rgb24ToBmp(const Image &img, Image &bmp)
    {
        if (img.empty() || bmp.empty()) {
            return -1;
        }
        int w = img.w;
        int h = img.h;
        int alignstep = align4(w, 3);
        /* write header */
        BmpHead head;
        head.type = 0x4d42;
        head.filesize = 54 + h*alignstep;
        head.reserved1 = 0;
        head.reserved2 = 0;
        head.offset = 54;
        memcpy(bmp.data, &head, sizeof(BmpHead));
        /* write info */
        BmpInformation info;
        info.infosize = sizeof(BmpInformation);
        info.height = h;
        info.width  = w;
        info.planes = 1;
        info.depth  = 24;
        info.compression = 0;
        info.imagesize   = h*alignstep;
        info.x = 5000;
        info.y = 5000;
        info.colorused      = 0;
        info.colorimportant = 0;
        memcpy(bmp.data + sizeof(BmpHead), &info, sizeof(BmpInformation));

        uint8_t *rgb = bmp.data + sizeof(BmpHead) + sizeof(BmpInformation);
        int c = 3;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                rgb[i*alignstep + j*c]     = img.data[(h - 1 - i)*w*c + j*c + 2];
                rgb[i*alignstep + j*c + 1] = img.data[(h - 1 - i)*w*c + j*c + 1];
                rgb[i*alignstep + j*c + 2] = img.data[(h - 1 - i)*w*c + j*c];
            }
        }
        return 0;
    }

    static int save(const std::string& fileName, const Image &img)
    {
        if (fileName.empty()) {
            return -1;
        }
        if (img.empty()) {
            return -2;
        }
        std::fstream file(fileName, std::ios::binary|std::ios::out);
        if (file.is_open() == false) {
            return -3;
        }
        Image bmp(img.totalSize + 54);
        rgb24ToBmp(img, bmp);
        file.write((char*)bmp.data, bmp.totalSize);
        file.close();
        return 0;
    }

    static Image load(const std::string& fileName, int channel)
    {
        Image img;
        if (fileName.empty()) {
            return img;
        }
        std::fstream file(fileName, std::ios::in|std::ios::binary);
        if (file.is_open() == false) {
            return img;
        }
        /* read header */
        BmpHead header;
        file.read((char*)&header, sizeof (BmpHead));
        /* check format */
        if (header.type != 0x4d42) {
            std::cout<<"invalid format"<<std::endl;
            return img;
        }
        /* check depth */
        BmpInformation info;
        file.read((char*)&info, sizeof (BmpInformation));
        if (info.depth != 24) {
            std::cout<<"invalid depth"<<std::endl;
            return img;
        }
        /* width */
        int w = info.width;
        /* height */
        int h = info.height;
        /* totalsize */
        uint32_t totalsize = info.imagesize;
        std::shared_ptr<uint8_t[]> raw = std::shared_ptr<uint8_t[]>(new uint8_t[totalsize]);
        /* read img */
        file.read((char*)raw.get(), totalsize);
        file.close();
        /* reverse: B1G1R1B2G2R2 -> R2G2B2R1G1B1 */
        int c = 3;
        img = Image(h, w, channel);
        int alignstep1 = align4(w, c);
        int alignstep2 = align4(w, channel);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                img.data[(h - 1 - i)*alignstep2 + j*channel + 2] = raw[i*alignstep1 + j*c];
                img.data[(h - 1 - i)*alignstep2 + j*channel + 1] = raw[i*alignstep1 + j*c + 1];
                img.data[(h - 1 - i)*alignstep2 + j*channel]     = raw[i*alignstep1 + j*c + 2];
                if (channel == 4) {
                    img.data[(h - 1 - i)*alignstep2 + j*channel + 3] = 120;
                }
            }
        }
        return img;
    }
};

#endif // BMP_HPP
