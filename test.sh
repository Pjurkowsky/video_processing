#!/bin/env bash

widths=(640 1280 1600 1920)
heights=(360 720 900 1080)
input_files=(640x360.mp4 1280x720.mp4 1600x900.mp4 1920x1080.mp4)
output_file="out.mp4"

# echo "input_name;processor;operation;input_width;input_height;output_width;output_height;batch_size;time" >results.csv

for i in "${!input_files[@]}"; do
  input_file="${input_files[i]}"
  input_width="${widths[i]}"
  input_height="${heights[i]}"

  for j in "${!widths[@]}"; do
    for k in {1..20}; do
      output_width="${widths[j]}"
      output_height="${heights[j]}"
      ./build/main \
        --input "$input_file" \
        --input-width "$input_width" \
        --input-height "$input_height" \
        --width "$output_width" \
        --height "$output_height" \
        --output "$output_file" \
        --processor CPU \
        --batch-size 100 \
        --filter RESIZE | tee -a results.csv
    done
  done
done

# for i in "${!input_files[@]}"; do
#   for k in {1..20}; do
#     input_file="${input_files[i]}"
#     input_width="${widths[i]}"
#     input_height="${heights[i]}"
#     ./build/main \
#       --input "$input_file" \
#       --input-width "$input_width" \
#       --input-height "$input_height" \
#       --width "$input_width" \
#       --height "$input_height" \
#       --output "out.mp4" \
#       --processor CPU \
#       --batch-size 200 \
#       --filter MONO | tee -a results.csv
#   done
# done
#
# for i in "${!input_files[@]}"; do
#   for k in {1..20}; do
#     input_file="${input_files[i]}"
#     input_width="${widths[i]}"
#     input_height="${heights[i]}"
#     ./build/main \
#       --input "$input_file" \
#       --input-width "$input_width" \
#       --input-height "$input_height" \
#       --width "$input_width" \
#       --height "$input_height" \
#       --output "out.mp4" \
#       --processor GPU \
#       --batch-size 200 \
#       --filter MONO | tee -a results.csv
#   done
# done
