#include "..\include\HXTextureReader.h"
#include <stdio.h>
#include <fstream>
#include <Windows.h>
#include <wingdi.h>
#include <assert.h>
#include "HXTextureInfo.h"
#include "HXDDSFormat.h"
#include "png.h"
#include "HXPNGFormat.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace HX3D
{
	//--------------------------------------------------------------------------------
	// CONSTANT
	//--------------------------------------------------------------------------------
	const int32_t kMaxPostfixLen = 8;
	const int32_t kBytesPerPixel = 4;
	const int32_t kBmpXBit = 8;
	const int32_t kBmpRGBBit = 24;
	const int32_t kBmpRGBABit = 32;

	//--------------------------------------------------------------------------------
	// DECLARATION
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------

	class TextureReaderBase {
	public:
		TextureReaderBase() {
		}

		virtual ~TextureReaderBase() {
		}

	public:
		virtual int32_t ReadTexture(const char* fileName, int8_t** textureData, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat) = 0;
	};

	//--------------------------------------------------------------------------

	class BmpTextureReader : public TextureReaderBase {
	public:
		BmpTextureReader();
		virtual ~BmpTextureReader();

	public:
		virtual int32_t ReadTexture(const char* fileName, int8_t** textureData, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat);
	};

	//--------------------------------------------------------------------------

	class DDSTextureReader : public TextureReaderBase {
	public:
		DDSTextureReader();
		virtual ~DDSTextureReader();

	public:
		virtual int32_t ReadTexture(const char* fileName, int8_t** textureData, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat);

	protected:
		TEXTURE_PIXEL_FORMAT_TYPE GetPixelType(DDSSurfaceDesc desc);
		TEXTURE_TYPE GetTextureType(DDSSurfaceDesc desc);

		void ReadTexture2D(FILE* file, TEXTURE_PIXEL_FORMAT_TYPE pixel_type, DDSFile& dds, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height, int32_t& pixel_format);
		// TODO:
		void ReadTexture3D();
		void ReadTextureCube(FILE* file, TEXTURE_PIXEL_FORMAT_TYPE pixel_type, DDSFile& dds, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height, int32_t& pixel_format);

		void ReorganizeRGBAData(int8_t* data, TEXTURE_PIXEL_FORMAT_TYPE type);
		TEXTURE_PIXEL_FORMAT_TYPE ReorganizeRGBAFormat(TEXTURE_PIXEL_FORMAT_TYPE type);
	};

	//--------------------------------------------------------------------------

	struct PNGTextureInfo;
	class PNGTextureReader : public TextureReaderBase
	{
	public:
		PNGTextureReader();
		virtual ~PNGTextureReader();

	public:
		virtual int32_t ReadTexture(const char* fileName, int8_t** textureData, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat);
		TEXTURE_PIXEL_FORMAT_TYPE GetPNGtextureInfo(int color_type, PNGTextureInfo* texinfo);
	};

	//--------------------------------------------------------------------------

	class HDRTextureReader : public TextureReaderBase {
	public:
		HDRTextureReader();
		virtual~HDRTextureReader();

	public:
		virtual int32_t ReadTexture(const char* fileName, int8_t** textureData, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat);
	};

	//--------------------------------------------------------------------------

	class PFCTextureReader : public TextureReaderBase {
	public:
		PFCTextureReader();
		virtual~PFCTextureReader();

	public:
		virtual int32_t ReadTexture(const char* fileName, int8_t** textureData, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat);
	};

	//--------------------------------------------------------------------------

	class PFTTextureReader : public TextureReaderBase {
	public:
		PFTTextureReader();
		virtual~PFTTextureReader();

	public:
		virtual int32_t ReadTexture(const char* fileName, int8_t** textureData, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat);
	};

	//--------------------------------------------------------------------------------
	// DEFINITION
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------

	BmpTextureReader::BmpTextureReader() {
	}

	BmpTextureReader::~BmpTextureReader() {
	}

	int32_t BmpTextureReader::ReadTexture(const char* file_name, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height, int32_t& texture_type, int32_t& pixel_format) {
		int32_t result = 0;

		if (file_name != NULL && texture_data != NULL) {
			FILE* file = fopen(file_name, "rb");

			if (file) {
				BITMAPFILEHEADER file_header;
				fread(&file_header, sizeof(BITMAPFILEHEADER), 1, file);

				BITMAPINFOHEADER info_header;
				fread(&info_header, sizeof(BITMAPINFOHEADER), 1, file);

				(*texture_data) = new int8_t[info_header.biWidth * info_header.biHeight * kBytesPerPixel];
				if (info_header.biSizeImage == 0) {
					int32_t raw_data_size = info_header.biWidth * info_header.biHeight * info_header.biBitCount / 8;
					info_header.biSizeImage = raw_data_size;
				}
				int8_t* raw_data = new int8_t[info_header.biSizeImage];
				fread(raw_data, sizeof(int8_t), info_header.biSizeImage, file);

				if (info_header.biBitCount == kBmpXBit) {
					// Convert X8 to R8G8B8A8
					int32_t bpp_in_file = kBmpXBit / 8;
					for (int32_t i = 0; i < info_header.biHeight; i++) {
						for (int32_t j = 0; j < info_header.biWidth; j++) {
							int8_t r = 0, g = 0, b = 0;
							r = g = b = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file];
							int8_t a = static_cast<int8_t>(0xff);
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel] = r;
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 1] = g;
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 2] = b;
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 3] = a;
						}
					}
				}
				else if (info_header.biBitCount == kBmpRGBBit) {
					// Convert B8G8R8 to R8G8B8A8
					int32_t bpp_in_file = kBmpRGBBit / 8;
					for (int32_t i = 0; i < info_header.biHeight; i++) {
						for (int32_t j = 0; j < info_header.biWidth; j++) {
							int8_t b = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file];
							int8_t g = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 1];
							int8_t r = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 2];
							int8_t a = static_cast<int8_t>(0xff);
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel] = r;
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 1] = g;
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 2] = b;
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 3] = a;
						}
					}
				}
				else if (info_header.biBitCount == kBmpRGBABit) {
					// Convert B8G8R8A8 to R8G8B8A8
					int32_t bpp_in_file = kBmpRGBABit / 8;
					for (int32_t i = 0; i < info_header.biHeight; i++) {
						for (int32_t j = 0; j < info_header.biWidth; j++) {
							int8_t b = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file];
							int8_t g = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 1];
							int8_t r = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 2];
							int8_t a = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 3];
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel] = r;
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 1] = g;
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 2] = b;
							(*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 3] = a;
						}
					}
				}
				else {
					// TODO: Un-supported format
					assert(false);
				}

				delete[] raw_data;
				raw_data = NULL;

				tex_width = info_header.biWidth;
				tex_height = info_header.biHeight;
				texture_type = TT_2D;
				pixel_format = TPFT_R8G8B8A8;
				result = 1;

				fclose(file);
				file = NULL;

			}
			else {
				assert(false);
			}
		}

		return result;
	}

	//--------------------------------------------------------------------------------

	DDSTextureReader::DDSTextureReader() {
	}

	DDSTextureReader::~DDSTextureReader() {
	}

	int32_t DDSTextureReader::ReadTexture(const char* file_name, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height, int32_t& texture_type, int32_t& pixel_format) {
		int32_t result = 0;

		if (file_name != NULL && texture_data != NULL) {
			FILE* file = fopen(file_name, "rb");

			if (file) {
				DDSFile dds;
				fread(&dds.magic_num, sizeof(dds.magic_num), 1, file);
				fread(&dds.desc, sizeof(dds.desc), 1, file);

				TEXTURE_PIXEL_FORMAT_TYPE pixel_type = GetPixelType(dds.desc);
				texture_type = GetTextureType(dds.desc);
				assert(pixel_type != TPFT_UNKOWN);

				if (dds.desc.format.flag & DDS_PF_RGB) {
					if (texture_type == TT_2D) {
						ReadTexture2D(file, pixel_type, dds, texture_data, tex_width, tex_height, pixel_format);
					}
					else if (texture_type == TT_3D) {
						ReadTexture3D();
					}
					else if (texture_type == TT_CUBE) {
						ReadTextureCube(file, pixel_type, dds, texture_data, tex_width, tex_height, pixel_format);
					}

					result = 1;
				}
				else {
					// TODO: Only support uncompressed RGB format now
					assert(false);
				}

				fclose(file);
				file = NULL;
			}
			else {
				assert(false);
			}
		}

		return result;
	}

	TEXTURE_PIXEL_FORMAT_TYPE DDSTextureReader::GetPixelType(DDSSurfaceDesc desc) {
		TEXTURE_PIXEL_FORMAT_TYPE pixel_format = TPFT_UNKOWN;

		if (desc.format.flag & DDS_PF_RGB) {
			// Uncompressed image
			struct {
				TEXTURE_PIXEL_FORMAT_TYPE pixel_type;
				int32_t bit_count;
				uint32_t red_mask_bit;
				uint32_t green_mask_bit;
				uint32_t blue_mask_bit;
				uint32_t alpha_mask_bit;
			} pixel_format_tbl[] = {
				{ TPFT_R8G8B8, 24, 0xff0000, 0x00ff00, 0x0000ff, 0x0 },
				{ TPFT_A8R8G8B8, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 },
				{ TPFT_R8G8B8A8, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff },
				{ TPFT_R16G16, 32, 0xffff0000, 0x0000ffff, 0x0, 0x0 },
				{ TPFT_G16R16, 32, 0x0000ffff, 0xffff0000, 0x0, 0x0 },
				{ TPFT_R16G16B16F, 32, 0x0000ffff, 0xffff0000, 0x0, 0x0 },  // not use
				{ TPFT_R16G16B16A16F, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 },  // not use
				{ TPFT_R32G32B32A32F, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 },  // not use
			};
			static_assert(HX_ARRAY_SIZE(pixel_format_tbl) == TPFT_UNKOWN, "");

			for (int32_t i = 0; i < HX_ARRAY_SIZE(pixel_format_tbl); i++) {
				if (desc.format.rgb_bit_count == pixel_format_tbl[i].bit_count
					&& desc.format.red_bit_mask == pixel_format_tbl[i].red_mask_bit
					&& desc.format.green_bit_mask == pixel_format_tbl[i].green_mask_bit
					&& desc.format.blue_bit_mask == pixel_format_tbl[i].blue_mask_bit
					&& desc.format.alpha_bit_mask == pixel_format_tbl[i].alpha_mask_bit) {
					pixel_format = pixel_format_tbl[i].pixel_type;
					break;
				}
			}
		}
		else {
			// TODO: do not support compressed image now
			assert(false);
		}

		return pixel_format;
	}

	TEXTURE_TYPE DDSTextureReader::GetTextureType(DDSSurfaceDesc desc) {
		TEXTURE_TYPE type = TT_UNKOWN;

		if (desc.caps.caps_1 & DDS_CAPS_COMPLEX) {
			if (desc.caps.caps_2 & DDS_CAPS2_CUBEMAP) {
				type = TT_CUBE;
			}
			else if (desc.caps.caps_2 & DDS_CAPS2_3D) {
				type = TT_3D;
			}
			else {
				type = TT_2D;  // 2D texture has mipmap
			}
		}
		else {
			type = TT_2D;  // 2D texture do not have mipmap
		}

		return type;
	}

	void DDSTextureReader::ReadTexture2D(FILE* file, TEXTURE_PIXEL_FORMAT_TYPE pixel_type, DDSFile& dds, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height, int32_t& pixel_format) {
		// Read uncompressed RGB pixel format dds file
		int32_t rgb_bytes = dds.desc.format.rgb_bit_count / 8;
		int32_t width = dds.desc.width;
		int32_t height = dds.desc.height;
		int32_t total_line_bytes = width * rgb_bytes, useful_line_bytes = width * rgb_bytes;

		// Check if has DDS_DESC_PITCH
		if (dds.desc.flag & DDS_SD_PITCH) {
			total_line_bytes = dds.desc.pitch_or_linear_size;
		}

		*texture_data = new int8_t[width * height * rgb_bytes];

		int8_t* data = new int8_t[total_line_bytes];

		// Copy data
		for (int32_t i = 0; i < height; i++) {
			fread(data, sizeof(int8_t), total_line_bytes, file);

			// Flip data upside down
			memcpy((*texture_data) + (height - 1 - i) * width * rgb_bytes, data, useful_line_bytes);
		}

		// Reorgnize data to RGBA format
		for (int32_t i = 0; i < height; i++) {
			for (int32_t j = 0; j < width; j++) {
				ReorganizeRGBAData(*(texture_data)+i * width * rgb_bytes + j * rgb_bytes, pixel_type);
			}
		}

		tex_width = width;
		tex_height = height;
		pixel_format = ReorganizeRGBAFormat(pixel_type);

		delete[] data;
		data = NULL;
	}

	void DDSTextureReader::ReadTexture3D() {
	}

	void DDSTextureReader::ReadTextureCube(FILE* file, TEXTURE_PIXEL_FORMAT_TYPE pixel_type, DDSFile& dds, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height, int32_t& pixel_format) {
		// Check if all face stored
		if (!((dds.desc.caps.caps_2 & DDS_CAPS2_CUBEMAP_POSITIVE_X) && (dds.desc.caps.caps_2 & DDS_CAPS2_CUBEMAP_NEGATIVE_X)
			&& (dds.desc.caps.caps_2 & DDS_CAPS2_CUBEMAP_POSITIVE_Y) && (dds.desc.caps.caps_2 & DDS_CAPS2_CUBEMAP_NEGATIVE_Y)
			&& (dds.desc.caps.caps_2 & DDS_CAPS2_CUBEMAP_POSITIVE_Z) && (dds.desc.caps.caps_2 & DDS_CAPS2_CUBEMAP_NEGATIVE_Z))) {
			assert(false);  // Do not support omitted faces
			return;
		}

		// Read uncompressed RGB pixel format dds file
		int32_t rgb_bytes = dds.desc.format.rgb_bit_count / 8;
		int32_t width = dds.desc.width;
		int32_t height = dds.desc.height;
		int32_t faces = 6;  // Must have all 6 faces
		int32_t total_line_bytes = width * rgb_bytes, useful_line_bytes = width * rgb_bytes;

		// Check if has DDS_DESC_PITCH
		if (dds.desc.flag & DDS_SD_PITCH) {
			total_line_bytes = dds.desc.pitch_or_linear_size;
		}

		*texture_data = new int8_t[width * height * faces * rgb_bytes];

		int8_t* data = new int8_t[total_line_bytes];

		// Copy data
		for (int32_t i = 0; i < faces; i++) {
			for (int32_t j = 0; j < height; j++) {
				fread(data, sizeof(int8_t), total_line_bytes, file);

				// Flip data upside down
				memcpy((*texture_data) + (i * height + height - 1 - j) * width * rgb_bytes, data, useful_line_bytes);
			}
		}

		// Reorgnize data to RGBA format
		for (int32_t i = 0; i < height * faces; i++) {
			for (int32_t j = 0; j < width; j++) {
				ReorganizeRGBAData(*(texture_data)+i * width * rgb_bytes + j * rgb_bytes, pixel_type);
			}
		}

		tex_width = width;
		tex_height = height;
		pixel_format = ReorganizeRGBAFormat(pixel_type);

		delete[] data;
		data = NULL;
	}

	void DDSTextureReader::ReorganizeRGBAData(int8_t* data, TEXTURE_PIXEL_FORMAT_TYPE type) {
		if (data != NULL) {
			if (type == TPFT_R8G8B8) {
				uint32_t value = (uint32_t(data[0]) << 0) + (uint32_t(data[1]) << 8) + (uint32_t(data[2]) << 16);
				data[0] = (value & 0x00ff0000) >> 16;  // Red
				data[1] = (value & 0x0000ff00) >> 8;  // Green
				data[2] = (value & 0x000000ff);  // Blue
			}
			else if (type == TPFT_A8R8G8B8) {
				uint32_t value = *(reinterpret_cast<uint32_t*>(data));
				data[0] = (value & 0x00ff0000) >> 16;  // Red
				data[1] = (value & 0x0000ff00) >> 8;  // Green
				data[2] = value & 0x000000ff;  // Blue
				data[3] = (value & 0xff000000) >> 24;  // Alpha
			}
			else if (type == TPFT_G16R16) {
				uint32_t value = *(reinterpret_cast<uint32_t*>(data));
				uint16_t* data16 = reinterpret_cast<uint16_t*>(data);
				data16[0] = value & 0x0000ffff;  //  Red
				data16[1] = (value & 0xffff0000) >> 16;  // Green
			}
		}
		else {
			assert(false);
		}
	}

	TEXTURE_PIXEL_FORMAT_TYPE DDSTextureReader::ReorganizeRGBAFormat(TEXTURE_PIXEL_FORMAT_TYPE type) {
		struct {
			TEXTURE_PIXEL_FORMAT_TYPE before;
			TEXTURE_PIXEL_FORMAT_TYPE after;
		} reformat_pixel_tbl[] = {
			{ TPFT_R8G8B8, TPFT_R8G8B8 },
			{ TPFT_A8R8G8B8, TPFT_R8G8B8A8 },
			{ TPFT_R8G8B8A8, TPFT_R8G8B8A8 },
			{ TPFT_R16G16, TPFT_R16G16 },
			{ TPFT_G16R16, TPFT_R16G16 },
			{ TPFT_R16G16B16F, TPFT_R16G16B16F },  // not use
			{ TPFT_R16G16B16A16F, TPFT_R16G16B16A16F },  // not use
			{ TPFT_R32G32B32A32F, TPFT_R32G32B32A32F },  // not use
		};
		static_assert(HX_ARRAY_SIZE(reformat_pixel_tbl) == TPFT_UNKOWN, "");

		TEXTURE_PIXEL_FORMAT_TYPE result = TPFT_UNKOWN;

		for (int32_t i = 0; i < HX_ARRAY_SIZE(reformat_pixel_tbl); i++) {
			if (type == reformat_pixel_tbl[i].before) {
				result = reformat_pixel_tbl[i].after;
				break;
			}
		}

		return result;
	}

	//--------------------------------------------------------------------------------

	PNGTextureReader::PNGTextureReader()
	{

	}

	PNGTextureReader::~PNGTextureReader()
	{

	}

	int32_t PNGTextureReader::ReadTexture(const char* file_name, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height, int32_t& texture_type, int32_t& pixel_format)
	{
		int32_t result = 0;

		PNGTextureInfo *texinfo;
		png_byte magic[8];
		png_structp png_ptr;
		png_infop info_ptr;
		int bit_depth, color_type;
		FILE *fp = NULL;
		png_bytep *row_pointers = NULL;
		png_uint_32 w, h;
		int i;
		/* Open image file */
		fopen_s(&fp, file_name, "rb");
		if (!fp)
		{
			fprintf(stderr, "error: couldn't open \"%s\"!\n", file_name);
			return NULL;
		}
		/* Read magic number */
		fread(magic, 1, sizeof(magic), fp);
		/* Check for valid magic number */
		if (!png_check_sig(magic, sizeof(magic)))
		{
			fprintf(stderr, "error: \"%s\" is not a valid PNG image!\n", file_name);
			fclose(fp);
			return NULL;
		}
		/* Create a png read struct */
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr)
		{
			fclose(fp);
			return NULL;
		}
		/* Create a png info struct */
		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			fclose(fp);
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return NULL;
		}
		/* Create our OpenGL texture object */
		texinfo = (PNGTextureInfo *)malloc(sizeof(PNGTextureInfo));
		/* Initialize the setjmp for returning properly after a libpng error occured */
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			if (row_pointers) free(row_pointers);
			if (texinfo) {
				if (texinfo->texels)
					free(texinfo->texels);
				free(texinfo);
			}
			return NULL;
		}
		/* Setup libpng for using standard C fread() function with our FILE pointer */
		png_init_io(png_ptr, fp);
		/* Tell libpng that we have already read the magic number */
		png_set_sig_bytes(png_ptr, sizeof(magic));
		/* Read png info */
		png_read_info(png_ptr, info_ptr);
		/* Get some usefull information from header */
		bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);
		/* Convert index color images to RGB images */
		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png_ptr);
		/* Convert 1-2-4 bits grayscale images to 8 bits grayscale. */
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
			png_set_expand_gray_1_2_4_to_8(png_ptr);

		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png_ptr);
		if (bit_depth == 16) png_set_strip_16(png_ptr);
		else if (bit_depth < 8) png_set_packing(png_ptr);
		/* Update info structure to apply transformations */
		png_read_update_info(png_ptr, info_ptr);
		/* Retrieve updated information */
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
		texinfo->width = w;
		texinfo->height = h;
		/* Get image format and components per pixel */
		TEXTURE_PIXEL_FORMAT_TYPE tpft = GetPNGtextureInfo(color_type, texinfo);
		/* We can now allocate memory for storing pixel data */
		texinfo->texels = (unsigned char *)malloc(sizeof(unsigned char) * texinfo->width * texinfo->height * texinfo->internalFormat);
		/* Setup a pointer array. Each one points at the begening of a row. */
		row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * texinfo->height);
		for (i = 0; i < texinfo->height; ++i)
		{
			row_pointers[i] = (png_bytep)(texinfo->texels + ((texinfo->height - (i + 1)) * texinfo->width * texinfo->internalFormat));
		}
		/* Read pixel data using row pointers */
		png_read_image(png_ptr, row_pointers);
		/* Finish decompression and release memory */
		png_read_end(png_ptr, NULL);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		/* We don't need row pointers anymore */
		free(row_pointers);
		fclose(fp);
		//return texinfo;

		*texture_data = reinterpret_cast<int8_t*>(texinfo->texels);
		tex_width = texinfo->width;
		tex_height = texinfo->height;
		texture_type = TT_2D;
		pixel_format = tpft;

		result = 1;
		return result;
	}

	TEXTURE_PIXEL_FORMAT_TYPE PNGTextureReader::GetPNGtextureInfo(int color_type, PNGTextureInfo* texinfo)
	{
		TEXTURE_PIXEL_FORMAT_TYPE tpft = TPFT_UNKOWN;
		switch (color_type)
		{
		case PNG_COLOR_TYPE_GRAY:
			//texinfo->format = GL_LUMINANCE;
			texinfo->internalFormat = 1;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			//texinfo->format = GL_LUMINANCE_ALPHA;
			texinfo->internalFormat = 2;
			break;
		case PNG_COLOR_TYPE_RGB:
			//texinfo->format = GL_RGB;
			texinfo->internalFormat = 3;
			tpft = TPFT_R8G8B8;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			//texinfo->format = GL_RGBA;
			texinfo->internalFormat = 4;
			tpft = TPFT_R8G8B8A8;
			break;
		default:
			/* Badness */
			break;
		}
		return tpft;
	}

	//--------------------------------------------------------------------------------

	HDRTextureReader::HDRTextureReader() {
	}

	HDRTextureReader::~HDRTextureReader() {
	}

	int32_t HDRTextureReader::ReadTexture(const char* file_name, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height, int32_t& texture_type, int32_t& pixel_format) {
		TEXTURE_PIXEL_FORMAT_TYPE result = TPFT_UNKOWN;

		if (file_name == NULL || texture_data == NULL) return result;

		stbi_set_flip_vertically_on_load(true);
		int32_t width = 0, height = 0, component = 0;
		float* data = stbi_loadf(file_name, &width, &height, &component, 0);
		*texture_data = reinterpret_cast<int8_t*>(data);
		tex_width = width;
		tex_height = height;
		texture_type = TT_2D;
		pixel_format = result = TPFT_R16G16B16F;

		return result;
	}

	//--------------------------------------------------------------------------------

	PFCTextureReader::PFCTextureReader() {
	}

	PFCTextureReader::~PFCTextureReader() {
	}

	int32_t PFCTextureReader::ReadTexture(const char* fileName, int8_t** textureData, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat) {
		TEXTURE_PIXEL_FORMAT_TYPE result = TPFT_UNKOWN;

		if (fileName == NULL || textureData == NULL) return result;

		FILE* file = fopen(fileName, "rb");
		if (file == NULL) return result;

		fread(&texWidth, sizeof(int32_t), 1, file);
		fread(&texHeight, sizeof(int32_t), 1, file);

		int32_t miplevels = log(max(texWidth, texHeight)) / log(2) + 1;
		int32_t sizeBytes = 0;
		for (int32_t i = 0; i < miplevels; i++) {
			sizeBytes = sizeBytes + sizeof(int16_t) * 4 * (texWidth * pow(2, -i)) * (texHeight * pow(2, -i)) * 6;
		}

		*textureData = new int8_t[sizeBytes];

		fread(*textureData, sizeBytes, 1, file);

		fclose(file);
		file = NULL;

		pixelFormat = result = TPFT_R16G16B16A16F;

		return result;
	}

	//--------------------------------------------------------------------------------

	PFTTextureReader::PFTTextureReader() {
	}

	PFTTextureReader::~PFTTextureReader() {
	}

	int32_t PFTTextureReader::ReadTexture(const char* fileName, int8_t** textureData, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat) {
		TEXTURE_PIXEL_FORMAT_TYPE result = TPFT_UNKOWN;

		if (fileName == NULL || textureData == NULL) return result;

		FILE* file = fopen(fileName, "rb");
		if (file == NULL) return result;

		fread(&texWidth, sizeof(int32_t), 1, file);
		fread(&texHeight, sizeof(int32_t), 1, file);

		int32_t sizeBytes = sizeof(int32_t) * 4 * texWidth * texHeight;
		*textureData = new int8_t[sizeBytes];

		fread(*textureData, sizeBytes, 1, file);

		fclose(file);
		file = NULL;

		pixelFormat = result = TPFT_R32G32B32A32F;

		return result;
	}

	//--------------------------------------------------------------------------

	int32_t HXTextureReader::ReadTexture(const char* fileName, int8_t** data, int32_t& texWidth, int32_t& texHeight, int32_t& textureType, int32_t& pixelFormat)
	{
		int32_t result = 0;

		if (fileName != NULL) {
			// Get the postfix
			int32_t file_name_len = strlen(fileName);
			int32_t index = file_name_len - 1;
			for (index = file_name_len - 1; index >= 0; index--) {
				if (fileName[index] == '.') {
					break;
				}
			}

			char postfix[kMaxPostfixLen];
			memcpy(postfix, fileName + index, file_name_len - index);
			postfix[file_name_len - index] = 0;

			char* lowerPF = _strlwr(postfix);

			// Check format
			if (!strcmp(lowerPF, ".bmp")) {
				BmpTextureReader bmp_reader;
				result = bmp_reader.ReadTexture(fileName, data, texWidth, texHeight, textureType, pixelFormat);
			}
			else if (!strcmp(lowerPF, ".dds")) {
				DDSTextureReader dds_reader;
				result = dds_reader.ReadTexture(fileName, data, texWidth, texHeight, textureType, pixelFormat);
			}
			else if (!strcmp(lowerPF, ".png")) {
				PNGTextureReader png_reader;
				result = png_reader.ReadTexture(fileName, data, texWidth, texHeight, textureType, pixelFormat);
			}
			else if (!strcmp(lowerPF, ".hdr")) {
				HDRTextureReader hdr_reader;
				result = hdr_reader.ReadTexture(fileName, data, texWidth, texHeight, textureType, pixelFormat);
			}
			else if (!strcmp(lowerPF, ".pfc")) {
				PFCTextureReader pfcReader;
				result = pfcReader.ReadTexture(fileName, data, texWidth, texHeight, textureType, pixelFormat);
			}
			else if (!strcmp(lowerPF, ".pft")) {
				PFTTextureReader pftReader;
				result = pftReader.ReadTexture(fileName, data, texWidth, texHeight, textureType, pixelFormat);
			} else {
				assert(false);
			}
		}

		return result;
	}

	void HXTextureReader::ReleaseData(int8_t** texture_data)
	{
		if (texture_data) 
		{
			if (*texture_data) 
			{
				delete[] * texture_data;
				*texture_data = NULL;
			}
		}
	}
}