// CopyRegister.cpp

#include "StdAfx.h"

#include "../Common/RegisterCodec.h"

#include "CopyCoder.h"

namespace NCompress {

REGISTER_CODEC_CREATE(CreateCodec, CCopyCoder())

REGISTER_CODEC_2(Copy, CreateCodec, CreateCodec, 0, "Copy")

}

namespace NCompress { void force_copy_registration() {} }
