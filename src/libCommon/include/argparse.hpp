// #include <string>
// namespace utill {
//
// enum processig_device { CPU, GPU };
//
// enum decoder_type { CPU_DECODER, GPU_DECODER };
// enum encoder_type { CPU_ENCODER, GPU_ENCODER };
// enum operation { RESIZE, MONO };
//
// struct commandline_argument {
//   std::string key;
//   std::function < void(std::string)
// }
//
// struct program_config {
//   std::string input_path;
//   std::string output_path;
//   processig_device processor;
//   decoder_type decoder;
//   encoder_type encoder;
//   operation operation;
//   int match_size;
// };
//
// int parse_cmd_args(int argc, char **argv);
//
// } // namespace utill
