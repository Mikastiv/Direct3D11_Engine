#include "GDIPlusInclude.hpp"
#include "Surface.hpp"
#include "Helpers.hpp"

#include <sstream>
#include <cassert>
#include <vector>
#include <string_view>
#include <algorithm>

Surface::Exception::Exception(int line, const char* file, std::string note)
    : MikastivException(line, file)
    , note(std::move(note))
{
}

auto Surface::Exception::what() const noexcept -> const char*
{
    std::ostringstream oss;
    oss << MikastivException::what() << "\n[Note] " << GetNote();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

auto Surface::Exception::GetType() const noexcept -> const char*
{
    return "Mikastiv Graphics Exception";
}

auto Surface::Exception::GetNote() const noexcept -> const std::string&
{
    return note;
}

auto Surface::FromFile(const std::wstring& file) -> Surface
{
    unsigned int width = 0u;
    unsigned int height = 0u;
    unsigned int pitch = 0u;
    std::unique_ptr<Color[]> pBuffer{};

    {
        Gdiplus::Bitmap bitmap(file.c_str());
        if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
        {
            std::ostringstream oss{};
            oss << "Loading image [" << WCharToString(file.c_str()) << "]: failed to load.";
            throw Exception(__LINE__, __FILE__, oss.str());
        }

        width = bitmap.GetWidth();
        height = bitmap.GetHeight();
        pBuffer = std::make_unique<Color[]>((size_t)width * height);

        for (size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                Gdiplus::Color c{};
                bitmap.GetPixel((int)x, (int)y, &c);
                pBuffer[y * width + x] = Color{ c.GetValue() };
            }
        }
    }

    return Surface(width, height, std::move(pBuffer));
}

Surface::Surface(unsigned int width, unsigned int height) noexcept
    : pBuffer(std::make_unique<Color[]>((size_t)width * height))
    , width(width)
    , height(height)
{
}

Surface::Surface(const Surface& other) noexcept(!IS_DEBUG)
    : pBuffer(std::make_unique<Color[]>((size_t)other.width * other.height))
    , width(other.width)
    , height(other.height)
{
    memcpy(pBuffer.get(), other.pBuffer.get(), (size_t)width * height * sizeof(Color));
}

Surface::Surface(Surface&& other) noexcept
    : pBuffer(std::move(other.pBuffer))
    , width(other.width)
    , height(other.height)
{
    other.width = 0;
    other.height = 0;
}

auto Surface::operator=(const Surface& rhs) noexcept(!IS_DEBUG) -> Surface&
{
    if (rhs.pBuffer != pBuffer)
    {
        width = rhs.width;
        height = rhs.height;
        pBuffer = std::make_unique<Color[]>((size_t)width * height);
        memcpy(pBuffer.get(), rhs.pBuffer.get(), (size_t)width * height * sizeof(Color));
    }
    return *this;
}

auto Surface::operator=(Surface&& rhs) noexcept -> Surface&
{
    if (rhs.pBuffer != pBuffer)
    {
        width = rhs.width;
        height = rhs.height;
        pBuffer = std::move(rhs.pBuffer);
        rhs.width = 0;
        rhs.height = 0;
    }
    return *this;
}

auto Surface::Clear(Color fill) noexcept -> void
{
    const auto size = (size_t)width * height;
    for (size_t i = 0; i < size; ++i)
    {
        pBuffer[i] = Color{ fill.packedColor };
    }
}

auto Surface::PutPixel(unsigned int x, unsigned int y, Color c) noexcept(!IS_DEBUG) -> void
{
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    pBuffer[(size_t)y * width + x] = c;
}

auto Surface::GetPixel(unsigned int x, unsigned int y) const noexcept(!IS_DEBUG) -> Color
{
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    return pBuffer[(size_t)y * width + x];
}

auto Surface::GetWidth() const noexcept -> unsigned int
{
    return width;
}

auto Surface::GetHeight() const noexcept -> unsigned int
{
    return height;
}

auto Surface::GetBufferPtr() noexcept -> Color*
{
    return pBuffer.get();
}

auto Surface::GetBufferPtr() const noexcept -> const Color*
{
    return pBuffer.get();
}

auto Surface::GetBufferCPtr() const noexcept -> const Color*
{
    return pBuffer.get();
}

auto Surface::SaveBMP(const std::wstring& file) const -> void
{
    const auto GetEncoderCLSID = [&file](const wchar_t* format, CLSID* pCLSID) {
        UINT num = 0u;
        UINT size = 0u;

        Gdiplus::GetImageEncodersSize(&num, &size);

        if (size == 0u)
        {
            std::ostringstream oss{};
            oss << "Saving surface to [" << WCharToString(file.c_str()).c_str()
                << "]: failed to get encoder; size == 0.";
            throw Exception(__LINE__, __FILE__, oss.str());
        }

        auto pImageCodecInfo = static_cast<Gdiplus::ImageCodecInfo*>(malloc(size));
        if (pImageCodecInfo == nullptr)
        {
            std::ostringstream oss{};
            oss << "Saving surface to [" << WCharToString(file.c_str()).c_str()
                << "]: failed to get encoder; failed to allocate memory.";
            throw Exception(__LINE__, __FILE__, oss.str());
        }

        Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

        for (UINT i = 0; i < num; i++)
        {
            if (std::wstring_view{ pImageCodecInfo[i].MimeType } == std::wstring_view{ format })
            {
                *pCLSID = pImageCodecInfo[i].Clsid;
                free(pImageCodecInfo);
                return;
            }
        }

        free(pImageCodecInfo);
        std::ostringstream oss{};
        oss << "Saving surface to [" << WCharToString(file.c_str()).c_str()
            << "]: failed to get encoder; failed to find matching encoder.";
        throw Exception(__LINE__, __FILE__, oss.str());
    };

    CLSID bmpID{};
    GetEncoderCLSID(L"image/bmp", &bmpID);

    Gdiplus::Bitmap bitmap(
        width, height, width * sizeof(Color), PixelFormat32bppARGB, reinterpret_cast<BYTE*>(pBuffer.get()));

    if (bitmap.Save(file.c_str(), &bmpID, nullptr) != Gdiplus::Status::Ok)
    {
        std::ostringstream oss{};
        oss << "Saving surface to [" << WCharToString(file.c_str()).c_str() << "]: failed to save.";
        throw Exception(__LINE__, __FILE__, oss.str());
    }
}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pColor) noexcept
    : pBuffer(std::move(pColor))
    , width(width)
    , height(height)
{
}
