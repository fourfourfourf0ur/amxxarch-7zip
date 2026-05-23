cmake_minimum_required(VERSION 3.18)

project(7zip LANGUAGES C CXX)

set(ZIP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(ZIP_SOURCES
    "${ZIP_ROOT}/C/7zBuf.c"
    "${ZIP_ROOT}/C/7zBuf2.c"
    "${ZIP_ROOT}/C/7zCrc.c"
    "${ZIP_ROOT}/C/7zCrcOpt.c"
    "${ZIP_ROOT}/C/7zStream.c"
    "${ZIP_ROOT}/C/Aes.c"
    "${ZIP_ROOT}/C/AesOpt.c"
    "${ZIP_ROOT}/C/Alloc.c"
    "${ZIP_ROOT}/C/Bcj2.c"
    "${ZIP_ROOT}/C/Bra.c"
    "${ZIP_ROOT}/C/Bra86.c"
    "${ZIP_ROOT}/C/BraIA64.c"
    "${ZIP_ROOT}/C/Blake2s.c"
    "${ZIP_ROOT}/C/CpuArch.c"
    "${ZIP_ROOT}/C/Delta.c"
    "${ZIP_ROOT}/C/Lzma2Dec.c"
    "${ZIP_ROOT}/C/Lzma2DecMt.c"
    "${ZIP_ROOT}/C/LzmaDec.c"
    "${ZIP_ROOT}/C/Md5.c"
    "${ZIP_ROOT}/C/MtDec.c"
    "${ZIP_ROOT}/C/Ppmd7.c"
    "${ZIP_ROOT}/C/Ppmd7Dec.c"
    "${ZIP_ROOT}/C/Ppmd7aDec.c"
    "${ZIP_ROOT}/C/Ppmd8.c"
    "${ZIP_ROOT}/C/Ppmd8Dec.c"
    "${ZIP_ROOT}/C/Ppmd8Enc.c"
    "${ZIP_ROOT}/C/Sha1.c"
    "${ZIP_ROOT}/C/Sha1Opt.c"
    "${ZIP_ROOT}/C/Sha256.c"
    "${ZIP_ROOT}/C/Sha256Opt.c"
    "${ZIP_ROOT}/C/Sha3.c"
    "${ZIP_ROOT}/C/Sha512.c"
    "${ZIP_ROOT}/C/Sha512Opt.c"
    "${ZIP_ROOT}/C/SwapBytes.c"
    "${ZIP_ROOT}/C/Threads.c"
    "${ZIP_ROOT}/C/Xxh64.c"
    "${ZIP_ROOT}/C/Xz.c"
    "${ZIP_ROOT}/C/XzCrc64.c"
    "${ZIP_ROOT}/C/XzCrc64Opt.c"
    "${ZIP_ROOT}/C/XzDec.c"
    "${ZIP_ROOT}/C/XzIn.c"
    "${ZIP_ROOT}/C/ZstdDec.c"

    "${ZIP_ROOT}/CPP/7zip/Archive/7z/7zCompressionMode.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/7z/7zDecode.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/7z/7zExtract.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/7z/7zHandler.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/7z/7zHeader.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/7z/7zIn.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/7z/7zProperties.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/7z/7zRegister.cpp"

    "${ZIP_ROOT}/CPP/7zip/Archive/Zip/ZipHandler.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Zip/ZipIn.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Zip/ZipItem.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Zip/ZipRegister.cpp"

    "${ZIP_ROOT}/CPP/7zip/Archive/Bz2Handler.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/DeflateProps.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/GzHandler.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/HandlerCont.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/XzHandler.cpp"

    "${ZIP_ROOT}/CPP/7zip/Archive/Common/CoderMixer2.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Common/DummyOutStream.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Common/FindSignature.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Common/HandlerOut.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Common/InStreamWithCRC.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Common/ItemNameUtils.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Common/MultiStream.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Common/OutStreamWithCRC.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Common/ParseProperties.cpp"

    "${ZIP_ROOT}/CPP/7zip/Archive/Rar/Rar5Handler.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Rar/RarHandler.cpp"

    "${ZIP_ROOT}/CPP/7zip/Archive/Tar/TarHandler.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Tar/TarHeader.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Tar/TarIn.cpp"
    "${ZIP_ROOT}/CPP/7zip/Archive/Tar/TarRegister.cpp"

    "${ZIP_ROOT}/CPP/7zip/Common/CWrappers.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/CreateCoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/FilterCoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/InBuffer.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/LimitedStreams.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/MethodId.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/MethodProps.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/OffsetStream.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/OutBuffer.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/ProgressUtils.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/PropId.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/StreamBinder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/StreamObjects.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/StreamUtils.cpp"
    "${ZIP_ROOT}/CPP/7zip/Common/VirtThread.cpp"

    "${ZIP_ROOT}/CPP/7zip/Compress/BZip2Crc.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/BZip2Decoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/BZip2Register.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Bcj2Coder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Bcj2Register.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/BcjCoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/BcjRegister.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/BitlDecoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/BranchMisc.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/BranchRegister.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/ByteSwap.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/CopyCoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/CopyRegister.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Deflate64Register.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/DeflateDecoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/DeflateRegister.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/ImplodeDecoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/ImplodeHuffmanDecoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/DeltaFilter.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/LzOutWindow.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Lzma2Decoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Lzma2Register.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/LzmaDecoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/LzmaRegister.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/PpmdDecoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/PpmdRegister.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/PpmdZip.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Rar1Decoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Rar2Decoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Rar3Decoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Rar3Vm.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/Rar5Decoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/RarCodecsRegister.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/ShrinkDecoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/XzDecoder.cpp"
    "${ZIP_ROOT}/CPP/7zip/Compress/ZstdDecoder.cpp"

    "${ZIP_ROOT}/CPP/7zip/Crypto/7zAes.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/7zAesRegister.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/HmacSha1.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/HmacSha256.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/MyAes.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/MyAesReg.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/Pbkdf2HmacSha1.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/Rar20Crypto.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/Rar5Aes.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/RarAes.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/RandGen.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/WzAes.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/ZipCrypto.cpp"
    "${ZIP_ROOT}/CPP/7zip/Crypto/ZipStrong.cpp"

    "${ZIP_ROOT}/CPP/Common/AmxxArchInitGuid.cpp"
    "${ZIP_ROOT}/CPP/Common/CRC.cpp"
    "${ZIP_ROOT}/CPP/Common/CrcReg.cpp"
    "${ZIP_ROOT}/CPP/Common/DynLimBuf.cpp"
    "${ZIP_ROOT}/CPP/Common/IntToString.cpp"
    "${ZIP_ROOT}/CPP/Common/Md5Reg.cpp"
    "${ZIP_ROOT}/CPP/Common/MyMap.cpp"
    "${ZIP_ROOT}/CPP/Common/MyString.cpp"
    "${ZIP_ROOT}/CPP/Common/MyVector.cpp"
    "${ZIP_ROOT}/CPP/Common/MyWindows.cpp"
    "${ZIP_ROOT}/CPP/Common/NewHandler.cpp"
    "${ZIP_ROOT}/CPP/Common/Sha1Prepare.cpp"
    "${ZIP_ROOT}/CPP/Common/Sha1Reg.cpp"
    "${ZIP_ROOT}/CPP/Common/Sha256Prepare.cpp"
    "${ZIP_ROOT}/CPP/Common/Sha256Reg.cpp"
    "${ZIP_ROOT}/CPP/Common/Sha3Reg.cpp"
    "${ZIP_ROOT}/CPP/Common/Sha512Prepare.cpp"
    "${ZIP_ROOT}/CPP/Common/Sha512Reg.cpp"
    "${ZIP_ROOT}/CPP/Common/StringConvert.cpp"
    "${ZIP_ROOT}/CPP/Common/StringToInt.cpp"
    "${ZIP_ROOT}/CPP/Common/UTFConvert.cpp"
    "${ZIP_ROOT}/CPP/Common/Wildcard.cpp"
    "${ZIP_ROOT}/CPP/Common/Xxh64Reg.cpp"
    "${ZIP_ROOT}/CPP/Common/XzCrc64Init.cpp"
    "${ZIP_ROOT}/CPP/Common/XzCrc64Reg.cpp"

    "${ZIP_ROOT}/CPP/Windows/PropVariant.cpp"
    "${ZIP_ROOT}/CPP/Windows/PropVariantConv.cpp"
    "${ZIP_ROOT}/CPP/Windows/PropVariantUtils.cpp"
    "${ZIP_ROOT}/CPP/Windows/Synchronization.cpp"
    "${ZIP_ROOT}/CPP/Windows/System.cpp"
    "${ZIP_ROOT}/CPP/Windows/TimeUtils.cpp"
    "${ZIP_ROOT}/CPP/7zip/UI/Common/LoadCodecs.cpp"
)

add_library(7zip STATIC
    ${ZIP_SOURCES}
)

add_library(amxxarch::7zip ALIAS 7zip)

target_include_directories(7zip PUBLIC
    "${ZIP_ROOT}"
    "${ZIP_ROOT}/C"
    "${ZIP_ROOT}/CPP"
    "${ZIP_ROOT}/CPP/Common"
    "${ZIP_ROOT}/CPP/Windows"
    "${ZIP_ROOT}/CPP/7zip"
    "${ZIP_ROOT}/CPP/7zip/Common"
    "${ZIP_ROOT}/CPP/7zip/Archive"
    "${ZIP_ROOT}/CPP/7zip/Archive/Common"
    "${ZIP_ROOT}/CPP/7zip/Archive/7z"
    "${ZIP_ROOT}/CPP/7zip/Archive/Rar"
    "${ZIP_ROOT}/CPP/7zip/Archive/Tar"
    "${ZIP_ROOT}/CPP/7zip/Archive/Zip"
    "${ZIP_ROOT}/CPP/7zip/Compress"
    "${ZIP_ROOT}/CPP/7zip/Crypto"
    "${ZIP_ROOT}/CPP/7zip/UI/Common"
)

target_compile_definitions(7zip PUBLIC
    Z7_EXTRACT_ONLY
    Z7_ST
    Z7_ZIP_LZFSE_DISABLE
)

if(MSVC)
    target_compile_options(7zip PRIVATE
        /EHsc
        /GR
        /permissive-
        /FIwindows_compat.h
    )
else()
    target_compile_options(7zip PRIVATE
        -fPIC
    )
endif()

set_target_properties(7zip PROPERTIES
    OUTPUT_NAME "7zip"
)