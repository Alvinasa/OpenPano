// File: imageref.hh
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#pragma once
#include "lib/imgproc.hh"
#include "lib/mat.h"
#include "match_info.hh"
#include <memory>
#include <string>
#include <cassert>

namespace pano {
// A transparent reference to a image in file
struct ImageRef {
    ImageRef() = delete;

    ImageRef(const ImageRef &rhs) {
        fname = rhs.fname;
        _width = rhs._width;
        _height = rhs._height;
        img = new Mat32f(rhs.img->clone());

        assert(img);
    }

    ImageRef(ImageRef &&rhs) {
        fname = move(rhs.fname);
        img = rhs.img;
        _width = rhs._width;
        _height = rhs._height;
        rhs.img = nullptr;

        assert(img);
    }

    ImageRef(Mat32f *data) {
        _width = data->width();
        _height = data->height();
        img = data;

        assert(img);
    }

    ImageRef(const Matuc &data) {
        constexpr int CHANNEL = 3;
        _width = data.width();
        _height = data.height();
        img = new Mat32f(_height, _width, CHANNEL);
        for(int i=0; i<_height; ++i) {
            for(int j=0; j<_width; ++j) {
                for(int k=0; k<CHANNEL; ++k) {
                    img->at(i,j,k) = data.at(i,j,k) / 255.0;
                }
            }
        }

        assert(img);
        // memcpy(img->ptr(), data.ptr(), _width * _height * CHANNEL);
    }

    ImageRef &operator=(ImageRef &&rhs) {
        fname = move(rhs.fname);
        img = rhs.img;
        
        _width = rhs._width;
        _height = rhs._height;
        rhs.img = nullptr;

        assert(img);
        return *this;
    }
    
    ImageRef &operator=(ImageRef &rhs) {
        fname = rhs.fname;
        _width = rhs._width;
        _height = rhs._height;
        img = new Mat32f(rhs.img->clone());

        assert(img);
        return *this;
    }

    std::string fname;
    Mat32f *img = nullptr;
    int _width, _height;

    void load() {
        if (img)
            return;
        img = new Mat32f{read_img(fname.c_str())};
        _width = img->width();
        _height = img->height();
    }

    void release() {
        if (img)
            delete img;
        img = nullptr;
    }

    int width() const { return _width; }
    int height() const { return _height; }
    Shape2D shape() const { return {_width, _height}; }

    ImageRef(const std::string &fname) : fname(fname) {}

    ~ImageRef() {
        if (img)
            delete img;
    }
};

} // namespace pano
