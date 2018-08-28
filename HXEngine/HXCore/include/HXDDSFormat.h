#pragma once

#include <stdint.h>

namespace HX3D
{
	//--------------------------------------------------------------------------------

	struct DDSPixelFormat {
		int32_t size;
		int32_t flag;
		int32_t four_cc;
		int32_t rgb_bit_count;
		uint32_t red_bit_mask;
		uint32_t green_bit_mask;
		uint32_t blue_bit_mask;
		uint32_t alpha_bit_mask;
	};

	//--------------------------------------------------------------------------------

	struct DDSCaps {
		int32_t caps_1;
		int32_t caps_2;
		int32_t reserved[2];
	};

	//--------------------------------------------------------------------------------

	struct DDSSurfaceDesc {
		int32_t size;
		int32_t flag;
		int32_t height;
		int32_t width;
		int32_t pitch_or_linear_size;
		int32_t depth;
		int32_t mip_map_count;
		int32_t reserved[11];
		DDSPixelFormat format;
		DDSCaps caps;
		int32_t reserved_2;
	};

	//--------------------------------------------------------------------------------

	struct DDSFile {
		int32_t magic_num;
		DDSSurfaceDesc desc;
		int8_t*     main_data;
		int8_t*     attach_data;
	};

	//--------------------------------------------------------------------------------

	// DDSPixelFormat.flag
#define DDS_PF_RGB (0x40)

	// DDSCaps.caps1
#define DDS_CAPS_COMPLEX (0x8)
#define DDS_CAPS_TEXTURE (0x1000)

	// DDSCaps.caps2
#define DDS_CAPS2_CUBEMAP (0x200)
#define DDS_CAPS2_CUBEMAP_POSITIVE_X (0x400)
#define DDS_CAPS2_CUBEMAP_NEGATIVE_X (0x800)
#define DDS_CAPS2_CUBEMAP_POSITIVE_Y (0x1000)
#define DDS_CAPS2_CUBEMAP_NEGATIVE_Y (0x2000)
#define DDS_CAPS2_CUBEMAP_POSITIVE_Z (0x4000)
#define DDS_CAPS2_CUBEMAP_NEGATIVE_Z (0x8000)

#define DDS_CAPS2_3D (0x200000)

	// DDSSurfaceDesc.flag
#define DDS_SD_CAPS (0x1)
#define DDS_SD_HEIGHT (0x2)
#define DDS_SD_WIDTH (0x4)
#define DDS_SD_PITCH (0x8)
#define DDS_SD_PIXELFORMAT (0x1000)
}