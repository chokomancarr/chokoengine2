#include "hdr.h"
#include <algorithm>
#include <cmath>
#include <string.h>

namespace {
	const char* szSignature = "#?RADIANCE";
	const char* szFormat = "FORMAT=32-bit_rle_rgbe";
}

unsigned char* hdr::read_hdr(const char *filename, unsigned int *w, unsigned int *h) {
	auto strm = std::ifstream(filename, std::ios::binary);
	return read_hdr(strm, w, h);
}

unsigned char* hdr::read_hdr(std::istream& strm, unsigned int *w, unsigned int *h) {
	if (!strm) {
		return nullptr;
	}

	char buf[1024] = {0};
	char col[4] = {0};
	unsigned char *imagergbe;
	//float *image;
	int bFlippedX = 0;
	int bFlippedY = 0;
	int cnt, i, component;

	/* Verify the RADIANCE signature */
	strm.read(buf, 10);
	if (strncmp(szSignature, buf, 10) != 0) {
		fprintf(stderr, "read_hdr(): RADIANCE signature not found!\n");
		return NULL;
	}

	/* Next, skip past comments until we reach the portion that
	 * tells us the dimensions of the image */
	/* Check to see if each line contains the format std::string */
	do {
		strm.getline(buf, sizeof(buf));
	} while (strm && strncmp(buf, "FORMAT", 6));

	if (!strm) {
		fprintf(stderr, "read_hdr(): unexpected EOF looking for FORMAT string\n");
		return NULL;
	}

	/* Check if the format std::string is ok */
	if (strncmp(buf, szFormat, 22) != 0) {
		fprintf(stderr, "read_hdr(): The FORMAT is not %s\n", szFormat);
		return NULL;
	}

	/* Now look for the -Y or +Y */
	/* Check to see if each line contains the size std::string */
	do {
		strm.getline(buf, sizeof(buf));
	} while (strm && (strncmp(buf, "-Y", 2) && strncmp(buf, "+Y", 2)));

	if (!strm) {
		fprintf(stderr, "read_hdr(): unexpected EOF looking for image dimensions\n");
		return NULL;
	}

	if (strncmp(buf, "-Y", 2) == 0) {
		bFlippedY = 1;
	}

	/* Find the X */
	cnt = 0;
	while ((cnt < sizeof(buf)) && buf[cnt] != 'X') {
		cnt++;
	}

	if (cnt == sizeof(buf)) {
		fprintf(stderr, "read_hdr(): error reading image dimensions\n");
		return NULL;
	}

	if (buf[cnt-1] == '-') {
		/* Flip the X */
		bFlippedX = 1;
	}

	/* Read the dimensions */
	sscanf(buf, "%*s %u %*s %u", h, w);
	if ((*w == 0) || (*h == 0)) {
		fprintf(stderr, "read_hdr(): Something is wrong with image dimensions\n");
		return NULL;
	}

	imagergbe = new unsigned char[(*w) * (*h) * 4];
	if (imagergbe == nullptr) {
		std::cerr << "Cannot alloc buffer for reading hdr!" << std::endl;
		abort();
	}
	/* Do the RLE compression stuff
	 * Some of the RLE decoding stuff comes from ggLibrary */
	for (unsigned int y = 0; y < (*h); ++y) {
		int start = bFlippedY ? ((*h)-y-1)*(*w) : y*(*w);
		int step = bFlippedX ? -1 : 1;

		/* Start by reading the first four bytes of every scanline */
		if (!strm.read(col, 4)) {
			fprintf(stderr, "read_hdr(): unexpected EOF reading data\n");
			free(imagergbe);
			return NULL;
		}

		/* This is Radiance's new RLE scheme
		 * Header of 0x2, 0x2 is RLE encoding */
		if (col[0] == 2 && col[1] == 2 && col[2] >= 0) {

			/* Each component is run length encoded seperately
			 * This will naturally lead to better compression */
			for (component = 0; component < 4; ++component) {
				int pos = start;

				/* Keep going until the end of the scanline */
				unsigned int x = 0;
				while (x < (*w)) {
					/* Check to see if we have a run */
					unsigned char num;
					if (!strm.read((char*)&num, 1)) {
						fprintf(stderr, "read_hdr(): unexpected EOF reading data\n");
						free(imagergbe);
						return NULL;
					}
					if (num <= 128) {
						/* No run, just values, just just read all the values */
						for (i=0; i<num; ++i) {
							if (!strm.read((char*)&imagergbe[component + pos * 4], 1)) {
								fprintf(stderr, "read_hdr(): unexpected EOF reading data\n");
								free(imagergbe);
								return NULL;
							}
							pos += step;
						}
					}
					else {
						/* We have a run, so get the value and set all the
						 * values for this run */
						unsigned char value;
						if (!strm.read((char*)&value, 1)) {
							fprintf(stderr, "read_hdr(): unexpected EOF reading data\n");
							free(imagergbe);
							return NULL;
						}
						num -= 128;
						for (i=0; i<num; ++i) {
							imagergbe[component+pos*4] = value;
							pos += step;
						}
					}
					x += num;
				}
			}
		}
		else {
			/* This is the old Radiance RLE scheme. it's a bit simpler.
			 * All it contains is either runs or raw data, runs have
			 * their header, which we check for right away. */
			int pos = start;
			for (unsigned int x=0; x<(*w); ++x) {
				if (x > 0) {
					if (!strm.read(col, 4)) {
						fprintf(stderr, "read_hdr(): unexpected EOF reading data\n");
						free(imagergbe);
						return NULL;
					}
				}

				/* Check for the RLE header for this scanline */
				if (col[0] == 1 && col[1] == 1 && col[2] == 1) {

					/* Do the run */
					int num = ((int) col[3])&0xFF;
					unsigned char r = imagergbe[(pos-step)*4];
					unsigned char g = imagergbe[(pos-step)*4+1];
					unsigned char b = imagergbe[(pos-step)*4+2];
					unsigned char e = imagergbe[(pos-step)*4+3];

					for (i=0; i<num; ++i) {
						imagergbe[pos*4+0] = r;
						imagergbe[pos*4+1] = g;
						imagergbe[pos*4+2] = b;
						imagergbe[pos*4+3] = e;
						pos += step;
					}

					x += num-1;
				}
				else {
					/* No runs here, so just read the data thats there */
					//edit: buf is wrong, use col instead
					imagergbe[pos*4+0] = col[0];
					imagergbe[pos*4+1] = col[1];
					imagergbe[pos*4+2] = col[2];
					imagergbe[pos*4+3] = col[3];
					pos += step;
				}
			}
		}
	}

	return imagergbe;
}

void hdr::write_hdr(const char* filename, unsigned int w, unsigned int h, unsigned char* data) {
	std::ofstream strm(filename, std::ios::binary);
	strm << "#?RADIANCE\nFORMAT=32-bit_rle_rgbe\n\n"
		<< "-Y " << h << " +X " << w << "\n";
	for (int y = (int)h-1; y >= 0; y--) {
		strm << "\x02\x02" << (unsigned char)((w >> 8) & 0xff) << (unsigned char)(w & 0xff);
		unsigned char* dp = data + y * w * 4;
		for (int a = 0; a < 4; a++) {
			typedef std::pair<unsigned char, int> ccp;
			std::vector<ccp> rle;
			rle.push_back(ccp(dp[a], 1));
			for (int x = 1; x < w; x++) {
				auto v = dp[x*4 + a];
				auto& b = rle.back();
				if (b.first != v || b.second == 127) rle.push_back(ccp(v, 1));
				else ++b.second;
			}
			int of = 0, sg = 0;
			for (size_t i = 0, j = rle.size(); i < j; i++) {
				auto& r = rle[i];
				if (r.second > 1) {
					while (sg > 0) {
						int s = std::min(sg, 128);
						strm << (char)s;
						for (int si = 0; si < s; si++) {
							strm << rle[of++].first;
						}
						sg -= s;
					}
					strm << (unsigned char)(r.second + 128) << r.first;
				}
				else {
					if (!sg) of = i;
					sg++;
				}
			}
			while (sg > 0) {
				int s = std::min(sg, 128);
				strm << (char)s;
				for (int si = 0; si < s; si++) {
					strm << rle[of++].first;
				}
				sg -= s;
			}
		}
	}
}

void hdr::to_float(unsigned char imagergbe[], int w, int h, float* res) {
#pragma omp parallel for
	for (int i = w*h - 1; i >= 0; --i) {
		unsigned char exponent = imagergbe[i*4 + 3];
		if (exponent != 0) {
			double v = (1.f / 256.f) * std::pow(2, (float)(exponent - 128));
			res[i*3] = (float)((imagergbe[i*4] + 0.5f) * v);
			res[i*3 + 1] = (float)((imagergbe[i*4 + 1] + 0.5f) * v);
			res[i*3 + 2] = (float)((imagergbe[i*4 + 2] + 0.5f) * v);
		}
	}
}

void hdr::to_rgbe(float* rgb, int w, int h, unsigned char* res) {
	for (int i = w*h-1; i >= 0; --i) {
		float r = rgb[i*3];
		float g = rgb[i*3+1];
		float b = rgb[i*3+2];
		float mx = std::max(std::max(r, g), b);
		if (mx == 0) memset(res + i*4, 0, 4);
		else {
			int e = (int)std::ceil(std::log2f(mx));
			res[i*4 + 3] = (unsigned char)(e + 128);
			float mul = std::pow(2, e);
			res[i*4] = (unsigned char)(r * 255 / mul + 0.5f);
			res[i*4 + 1] = (unsigned char)(g * 255 / mul + 0.5f);
			res[i*4 + 2] = (unsigned char)(b * 255 / mul + 0.5f);
		}
	}
}
