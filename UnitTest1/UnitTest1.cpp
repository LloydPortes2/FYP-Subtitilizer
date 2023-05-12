#include "pch.h"
#include "TestUtils.hpp"
#include "CppUnitTest.h"
#include "../src/video_reader.hpp"
#include "../src/video_reader.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(TestMethod1)
        {
            // Call the video_reader_open function
            VideoReaderState state; // Create a test VideoReaderState object
            const char* filename = "C:\\Users\\porte\\OneDrive\\Pictures\\Camera Roll\\WIN_20230501_18_57_06_Pro.mp4"; // Specify the filename

            bool openResult = video_reader_open(&state, filename);

            // Assert the result of video_reader_open
            Assert::IsTrue(openResult, L"Failed to open video", LINE_INFO());

            // Call the video_reader_read_frame function
            uint8_t frame_buffer[1024]; // Create a frame buffer with an appropriate size
            int64_t pts;

            bool readResult = video_reader_read_frame(&state, frame_buffer, &pts);

            // Assert the result of video_reader_read_frame
            Assert::IsTrue(readResult, L"Failed to read frame", LINE_INFO());

            // Call the video_reader_close function
            video_reader_close(&state);
        }
    };
}
