# Image template matching using FFT
Simple image template matching in C++ using FFT.

The algorithm simply uses phase correlation, and translates into NumPy like something below:
```python
img = np.array(Image.open('test/image.png').convert('L'))
pattern = np.array(Image.open('test/pattern1.png').convert('L'))
pattern_shape = pattern.shape
pattern = np.pad(pattern, [(0, img.shape[0]-pattern.shape[0]), (0, img.shape[1]-pattern.shape[1])])

img_fft = np.fft.fft2(img)
pat_fft = np.fft.fft2(pattern)
mult = np.multiply(img_fft, np.conjugate(pat_fft))
mult = np.divide(mult, np.abs(mult))

result = np.real(np.fft.ifft2(mult))
max_pos = np.unravel_index(result.argmax(), result.shape)
print(max_pos, result[max_pos])
```

This project uses [pocketfft](https://gitlab.mpcdf.mpg.de/mtr/pocketfft/-/tree/cpp) for FFT
and [stb_image](https://github.com/nothings/stb) for image reading.


## Usage
Build the project and execute by
```bash
./cmake-build-release/imgTemplateMatch [target image path] [pattern image path]
```

Building in release mode (`-O3`) is significantly faster at runtime.
```bash
# Debug
$ time ./cmake-build-debug/imgTemplateMatch ~/Desktop/test/image.png ~/Desktop/test/pattern1.png
Target image: 3278x1052
Pattern image: 530x112
724, 512: 0.167288
8.20s user 0.17s system 703% cpu 1.189 total

# Release
$ time ./cmake-build-release/imgTemplateMatch ~/Desktop/test/image.png ~/Desktop/test/pattern1.png
Target image: 3278x1052
Pattern image: 530x112
724, 512: 0.167288
0.72s user 0.17s system 336% cpu 0.264 total
```
