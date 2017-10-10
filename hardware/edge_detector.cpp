/**
 * @file blob_detector.cpp
 *
 * This file contains the implementation of the multi-scale blob detector
 * pipeline.
 *
 * The blob detection is performed at the given number of scales, with the same
 * steps taken in each one. The processor streams in an input image to the
 * hardware. The bounding boxes of the blobs results are recombined at the
 * end and streamed back to the processor.
 *
 * @bug No known bugs.
 **/

#include "edge_detection.h"     // Our interface and blob detection types
#include "image.h"              // Definition of image info
#include "grayscale.h"          // Definition of grayscale info

/*----------------------------------------------------------------------------
 * Internal Definitions
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 * Edge Detector
 *----------------------------------------------------------------------------*/

void edge_detector(pixel_stream_t& rgba_image, edge_detection_stream_t& edges) {
#pragma HLS INTERFACE axis port=rgba_image
#pragma HLS INTERFACE axis port=edges
#pragma HLS INTERFACE ap_ctrl_none port=return

#pragma HLS DATAFLOW
    // Convert the image to grayscale, and duplicate the stream
    grayscale_stream_t gray_image;
    grayscale(rgba_image, gray_image);

    edge_detection<IMAGE_WIDTH, IMAGE_HEIGHT>(gray_image, edges);

    return;
}
