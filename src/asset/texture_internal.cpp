#include "texture_internal.hpp"

extern "C" {
#define INT32 J_INT32
#include "jpeglib.h"
#include "jerror.h"
#undef INT32
}
#include "3rdparty/lodepng.h"
#include "3rdparty/hdr.h"

CE_BEGIN_NAMESPACE

void Texture_I::FlipY(std::vector<byte>& data, uint w, uint h) {
    #pragma omp parallel for
	for (int a = 0; a < h / 2; ++a) {
		std::vector<byte> tmp(w * 4);
		memcpy(&tmp[0], &data[a * w * 4], w * 4);
		memcpy(&data[a * w * 4], &data[(h - a - 1) * w * 4], w * 4);
		memcpy(&data[(h - a - 1) * w * 4], &tmp[0], w * 4);
	}
}

bool Texture_I::FromJPG(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data) {
	unsigned long data_size;     // length
	unsigned char* rowptr[1];
	struct jpeg_decompress_struct info; //for our jpeg info
	struct jpeg_error_mgr err;          //the error handler

	FILE* file = fopen(path.c_str(), "rb");  //open the file

	if (!file) {					//if the jpeg file doesn't load
		Debug::Error("Texture", "Read jpg error: cannot open file!");
		return false;
	}

	info.err = jpeg_std_error(&err);
	err.error_exit = [](j_common_ptr) {
		abort();
	};
	jpeg_create_decompress(&info);   //fills info structure

	jpeg_stdio_src(&info, file);

	jpeg_read_header(&info, TRUE);   // read jpeg file header

	jpeg_start_decompress(&info);    // decompress the file

	w = info.output_width;
	h = info.output_height;
	channels = info.num_components;

	data_size = w * h * channels;

	data.resize(data_size);

	while (info.output_scanline < h) {
		// Enable jpeg_read_scanlines() to fill our jdata array
		rowptr[0] = &data[channels * w * (h - info.output_scanline - 1)];

		jpeg_read_scanlines(&info, rowptr, 1);
	}

	jpeg_finish_decompress(&info);   //finish decompressing
	jpeg_destroy_decompress(&info);
	fclose(file);
	return true;
}

bool Texture_I::FromPNG(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data) {
    channels = 4;
	uint err = lodepng::decode(data, w, h, path.c_str());
	if (err) {
		Debug::Error("Texture", "Read png error: " + std::string(lodepng_error_text(err)));
		return false;
	}
	FlipY(data, w, h);
	return true;
}

#define BMPERR(msg) \
    do {\
        Debug::Error("Texture", "Read bmp error: " msg "!");\
        return false;\
    } while(0)

bool Texture_I::FromBMP(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data) {
	char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int imageSize;   // = width*height*3
	unsigned short bpi;

	std::ifstream strm(path, std::ios::binary);

	if (!strm)
		BMPERR("cannot open image");

	strm.read(header, 54);
	if (!strm)
		BMPERR("cannot read header");
	
	if (header[0] != 'B' || header[1] != 'M')
		BMPERR("header signature incorrect");
	
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	w = *(int*)&(header[0x12]);
	h = *(int*)&(header[0x16]);
	bpi = *(short*)&(header[0x1c]);
	if (bpi != 24 && bpi != 32)
		return false;
	else channels = (bpi == 24) ? 3 : 4;
	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = w * h * channels; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way
	data.resize(imageSize);
	// Read the actual data from the file into the buffer
	strm.read((char*)data.data(), imageSize);
	return true;
}

bool Texture_I::FromHDR(const std::string& path, uint& w, uint& h, byte& channels, std::vector<byte>& data) {
	byte* d = hdr::read_hdr(path.c_str(), &w, &h);
	if (!d) return false;

	channels = 3;
	data.resize(w * h * 3 * sizeof(float));
	hdr::to_float(d, w, h, (float*)data.data());
	delete[](d);

	return true;
}

CE_END_NAMESPACE