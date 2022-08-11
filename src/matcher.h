#include <cstdint>
#include <complex>
#include <vector>

struct SearchResult {
    int x, y;
    double value;
};

void fftForward(double* vIn, std::complex<double>* vOut, int width, int height);
void fftBackward(std::complex<double>* vIn, double* vOut, int width, int height);
void pointWiseUnit(std::complex<double>* src, std::complex<double>* other, int width, int height);
SearchResult search(uint8_t* img, uint8_t* pattern, int imgWidth, int imgHeight, int patternWidth, int patternHeight);