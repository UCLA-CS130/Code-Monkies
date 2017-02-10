#include "gtest/gtest.h"
#include "handler/file_handler.h"
#include "http/request.h"
#include "http/response.h"
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

	std::string image_jpeg = handler_->getContentTypeFromExtension(jpg);
	std::string image_png = handler_->getContentTypeFromExtension(png);
	std::string text_html = handler_->getContentTypeFromExtension(html);
	std::string text_plain = handler_->getContentTypeFromExtension(plain);

	std::string contentTypeImageJpeg = "Content-Type: image/jpeg";
	std::string contentTypeImagePng = "Content-Type: image/png";
	std::string contentTypeTextHtml = "Content-Type: text/html";
	std::string contentTypeTextPlain = "Content-Type: text/plain";

	EXPECT_EQ(contentTypeImageJpeg, image_jpeg);
	EXPECT_EQ(contentTypeImagePng, image_png);
	EXPECT_EQ(contentTypeTextHtml, text_html);
	EXPECT_EQ(contentTypeTextPlain, text_plain);
}

TEST_F(FileRequestHandlerTest, Build200Response) {
	// TODO: Determine a more robust file/system for faking ifstream
	handler_ = new FileRequestHandler(".");
	
}