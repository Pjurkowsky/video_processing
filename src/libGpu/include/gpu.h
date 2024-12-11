#include <argparse.hpp>
#include <common.h>
#include <cuda.hpp>
#include <logger.h>
#include <opencv2/videoio.hpp>
#include <video.h>

namespace gpu {

void gpu(Video video);
void gpu(Video video, utill::filter, int batch_size, std::string out, int width,
         int height);
cv::Ptr<cv::VideoWriter> createVideoWriter(std::string filenem);
cv::Ptr<cv::VideoCapture> createVideoReader(std::string filenem);
void apply_transform(utill::filter operation);
} // namespace gpu
