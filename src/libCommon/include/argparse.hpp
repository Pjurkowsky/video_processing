#include <functional>
#include <string>
namespace utill {

enum processig_device { CPU, GPU };

enum decoder_type { CPU_DECODER, GPU_DECODER };
enum encoder_type { CPU_ENCODER, GPU_ENCODER };
enum filter { RESIZE, MONO };

struct program_config {
  std::string input_path;
  std::string output_path;
  processig_device processor;
  decoder_type decoder;
  encoder_type encoder;
  filter operation;
  int batch_size;
};

struct commandline_argument {
  std::string arg;
  std::string arg_short;
  std::function<bool(std::string, program_config &config)> f;
  bool required;
};

int parse_cmd_args(int argc, char **argv, program_config &config);

} // namespace utill
