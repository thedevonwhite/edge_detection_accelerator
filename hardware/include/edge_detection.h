/* @file edge_detection.h
 * @author Devon White (dww)
 *
 * This file contains the interface to the edge detection module.
 *
 * This defines the edge detection module interface, as both a sequential
 * (streaming) interface, and a combinational interface. It also defines the
 * input and outptut types for the module.
 *
 * @bug No known bugs.
 **/

#ifndef EDGE_DETECTION_H_
#define EDGE_DETECTION_H_

#include <hls_stream.h>             // Definition of the hls::stream class
#include <ap_int.h>                 // Arbitrary precision integer types

#include "axis.h"                   // Definition of the AXIS protocol structure
#include "grayscale.h"              // Definition of the grayscale types
#include "windowfetch.h"            // The window operation pipeline

/*----------------------------------------------------------------------------
 * Definitions
 *----------------------------------------------------------------------------*/

/**
 * The dimensions of the sobel compass filter. This also determines the
 * size of the window operated on in the image.
 **/
static const int EDGE_FILTER_WIDTH = 5;
static const int EDGE_FILTER_HEIGHT = EDGE_FILTER_WIDTH;

/**
 * A convenient alias for the window of monochrome values matching the filter
 * size.
 **/
typedef grayscale_t grayscale_window_t[EDGE_FILTER_HEIGHT][EDGE_FILTER_WIDTH];

/**
 * The input stream type is a grayscale AXIS packet. The output stream type is
 * a 1-bit boolean value AXIS packet. This boolean indicates if the pixel
 * corresponds to an edge.
 **/
typedef ap_uint<1> edge_detection_t;
typedef axis<edge_detection_t, 1> edge_detection_axis_t;
typedef hls::stream<edge_detection_axis_t> edge_detection_stream_t;

/**
 * The precision for the edge detection calculation.
 * Max value is 255*1*4 + 255*2*2 = 2040 < 2048 = 2**11
 * So we need 12 bits because two's complement signed
 **/
static const int EDGE_RESPONSE_BITS = 12;

/**
 * The type of a sobel compass response. This is a LOG_INTEGER_BITS signed integer.
 **/
typedef ap_int<EDGE_RESPONSE_BITS> edge_detection_response_t;

/*----------------------------------------------------------------------------
 * Interface
 *----------------------------------------------------------------------------*/

/**
 * Decides if the given window in the image corresponds an edge detection.
 *
 * This computes the response to the eight sobel compass filter and then
 * thresholds to see if it corresponds to an edge. This is the combinational
 * interface to the module.
 *
 * @param[in] window A window of grayscale values from an image.
 * @return 1 if the window corresponds to an edge, 0 otherwise.
 **/
edge_detection_t compute_edge_detection(grayscale_window_t window,
        int start_row, int start_col);

/**
 * Converts the grayscale input stream into an output edge detection stream.
 *
 * This is done by computing the sobel compass response on windows of the image, and
 * thresholding to determine if it is an edge. The output stream indicates if the
 * pixel is an edge in an image. This is the sequential interface to the module.
 *
 * @tparam IMAGE_WIDTH The width of the image being processed.
 * @tparam IMAGE_HEIGHT The height of the image being processed.
 *
 * @param[in] grayscale_stream The input stream of grayscale values.
 * @param[out] edge_detection_stream The output stream of LoG detections.
 **/
template <int IMAGE_WIDTH, int IMAGE_HEIGHT>
void edge_detection(grayscale_stream_t& grayscale_stream,
        edge_detection_stream_t& edge_detection_stream) {
#pragma HLS INLINE

    // Declare a window object
    window_pipeline<grayscale_t, edge_detection_t, 1, 1, IMAGE_HEIGHT,
            IMAGE_WIDTH, EDGE_FILTER_HEIGHT, EDGE_FILTER_WIDTH,
            compute_edge_detection> w;

    // Apply the LoG operation
    w.window_op(grayscale_stream, edge_detection_stream);
    return;
}

#endif /* EDGE_DETECTION_H_ */
