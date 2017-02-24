#include "gtest/gtest.h"
#include "handler/file_handler.h"
#include <string>

class FileRequestHandlerTest : public ::testing::Test {
public:
	FileRequestHandler* handler_ = nullptr;

	void TearDown() {
		if (handler_ != nullptr)
			delete handler_;
	}
};

TEST_F(FileRequestHandlerTest, GetFileExtensionTest) {
	std::string jpg = "file.jpg";
	std::string lotsOfDots = "f.i.l.e.jpg";
	std::string jpgFileExtension = "jpg";

	std::string jpgExtension = handler_->getFileExtension(jpg);
	std::string jpgExtension2 = handler_->getFileExtension(lotsOfDots);

	EXPECT_EQ(jpgExtension,  jpgFileExtension);
	EXPECT_EQ(jpgExtension2, jpgFileExtension);
}

TEST_F(FileRequestHandlerTest, GetContentTypeFromExtensionTest) {
	std::string jpg = "jpg";
	std::string png = "png";
	std::string html = "html";
	std::string plain = "garbage"; // Default content type is text/plain

	Header image_jpeg = handler_->getContentTypeFromExtension(jpg);
	Header image_png = handler_->getContentTypeFromExtension(png);
	Header text_html = handler_->getContentTypeFromExtension(html);
	Header text_plain = handler_->getContentTypeFromExtension(plain);

	Header contentTypeImageJpeg = std::make_pair("Content-Type", "image/jpeg");
	Header contentTypeImagePng = std::make_pair("Content-Type", "image/png");
	Header contentTypeTextHtml = std::make_pair("Content-Type", "text/html");
	Header contentTypeTextPlain = std::make_pair("Content-Type", "text/plain");

	EXPECT_EQ(contentTypeImageJpeg, image_jpeg);
	EXPECT_EQ(contentTypeImagePng, image_png);
	EXPECT_EQ(contentTypeTextHtml, text_html);
	EXPECT_EQ(contentTypeTextPlain, text_plain);
}