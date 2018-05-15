#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem::v1;

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cout << " Usage: logo-recognizer <path-to-an-image>\n";
    return -1;
  }

  std::cout << fs::path(argv[1]).filename().string() << "\n";

  cv::Mat image;
  image = cv::imread(argv[1]);

  if (!image.data) {
    std::cout << "Could not open or find the image!\n";
    return -1;
  }

  cv::namedWindow(fs::path(argv[1]).filename().string(), cv::WINDOW_AUTOSIZE);
  cv::imshow(fs::path(argv[1]).filename().string(), image);

  std::cout << "To end the program press 'q' in image window.\n";

  char c = 0;

  while(c != 'q')
   c = cv::waitKey(0);

  return 0;
}