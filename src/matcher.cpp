#include <complex>
#include <vector>
#include "matcher.h"
#include "pocketfft.h"

void copy(uint8_t* in, double* out, int inWidth, int inHeight, int outWidth, int outHeight) {
    if (inWidth*inHeight < outWidth*outHeight) {
        for (int i=0; i<outWidth*outHeight; i++) {
            out[i] = 0;
        }
    }
    for (int i=0; i<inWidth*inHeight; i++) {
        out[i/inWidth*outWidth+i%inWidth] = in[i];
    }
}

void fftForward(double* vIn, std::complex<double>* vOut, int width, int height) {
    auto shape_in = pocketfft::shape_t{(size_t)height, (size_t)width};
    ptrdiff_t s = sizeof(double);
    auto inStride = pocketfft::stride_t{s*width, s};
    auto outStride = pocketfft::stride_t{2*s*width, 2*s};
    pocketfft::r2c<double>(shape_in, inStride, outStride, {0, 1},true,
                           vIn, vOut, 1., 0);

    for (auto x=0; x<height; x++) {
        for (auto y=width/2+1; y<width; y++) {
            auto otherX = x == 0 ? 0 : (height-x);
            auto otherY = y == 0 ? 0 : (width-y);
            vOut[x*width+y] = conj(vOut[otherX*width+otherY]);
        }
    }
}

void fftBackward(std::complex<double>* vIn, double* vOut, int width, int height) {
    auto shape = pocketfft::shape_t{(size_t)(height), (size_t)(width)};
    auto inStride = pocketfft::stride_t{(ptrdiff_t)(sizeof(vIn[0]) * width), sizeof(vIn[0])};
    auto outStride = pocketfft::stride_t{(ptrdiff_t)(sizeof(vOut[0]) * width), sizeof(vOut[0])};
    pocketfft::c2r<double>(shape, inStride, outStride, {0, 1},false,
                           vIn, vOut, 1. / (width * height), 0);
}

void pointWiseUnit(std::complex<double>* src, std::complex<double>* other, int width, int height) {
    for (auto i=0; i<height; i++) {
        for (auto j=0; j<width; j++) {
            src[i*width+j] = src[i*width+j]*conj(other[i*width+j]);
            src[i*width+j] /= abs(src[i*width+j]);
        }
    }
}

SearchResult search(uint8_t* img, uint8_t* pattern, int imgWidth, int imgHeight, int patternWidth, int patternHeight) {
    auto arrSize = imgWidth * imgHeight;
    auto realArr1 = new double[2*arrSize];
    auto realArr2 = realArr1 + arrSize;
    auto complexArr1 = new std::complex<double>[2*arrSize];
    auto complexArr2 = complexArr1 + arrSize;

    copy(img, realArr1, imgWidth, imgHeight, imgWidth, imgHeight);
    copy(pattern, realArr2, patternWidth, patternHeight, imgWidth, imgHeight);
    fftForward(realArr1, complexArr1, imgWidth, imgHeight);
    fftForward(realArr2, complexArr2, imgWidth, imgHeight);
    pointWiseUnit(complexArr1, complexArr2, imgWidth, imgHeight);
    fftBackward(complexArr1, realArr1, imgWidth, imgHeight);

    double maxVal = 0;
    int maxInd = 0;
    for (auto i=0; i<imgWidth*imgHeight; i++) {
        if (realArr1[i] > maxVal) {
            maxVal = realArr1[i];
            maxInd = i;
        }
    }

    delete[] realArr1;
    delete[] complexArr1;

    return SearchResult{
        .x = maxInd/imgWidth,
        .y = maxInd%imgWidth,
        .value = maxVal,
    };
}