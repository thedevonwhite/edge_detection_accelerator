/**
 * @file edge_detection.cpp
 * @author Devon White (dww)
 *
 * This file contains the implementation of the edge detection module.
 *
 * This module uses the Sobel Compass edge detector to detect edges in an image.
 *
 * @return 1 bit signifying whether the 3x3 corresponds to an edge
 *
 * @bug No known bugs.
 **/

#include <ap_int.h>                 //Arbitrary precision integers

#include "axis.h"                   // Definition of the AXIS protocol structure
#include "grayscale.h"             // Definition of the grayscale types
#include "edge_detection.h"         // Our interface and edge detection types
#include "windowfetch.h"            // Our implementation of window operation
#include "image.h"                  // Definition of image info
#include "filters.h"

/*----------------------------------------------------------------------------
 * Internal Definitions
 *----------------------------------------------------------------------------*/


/**
 * The threshold value used to determine if a sobel compass response corresponds to an
 * edge detection. If the response is greater than or equal to the threshold,
 * it becomes 1, otherwise, it becomes 0.
 **/
static const edge_detection_response_t EDGE_THRESHOLD = 500;

/*----------------------------------------------------------------------------
 * LoG Filter Module
 *----------------------------------------------------------------------------*/

/**
 * Decides if the given window in the image corresponds an edge detection.
 *
 * This computes the response to the 4 different Sobel Compass filters, and
 * compares them (as well as their absolute values, corresponding to an edge
 * in the opposite direction) to the threshold. If any of them exceed the
 * threshold, an edge is detected and 1 is returned.
 *
 * @param[in] window A window of monochrome values from an image.
 * @return 1 if the window corresponds to a edge, 0 otherwise.
 **/
edge_detection_t compute_edge_detection(grayscale_window_t window,
        int start_row, int start_col) {
#pragma HLS INLINE

    edge_detection_response_t response0 = 0;
    edge_detection_response_t response1 = 0;
    edge_detection_response_t response2 = 0;
    edge_detection_response_t response3 = 0;
    edge_detect_row: for(int i = 0; i < EDGE_FILTER_HEIGHT ; i++) {
        edge_detect_col: for(int j = 0; j < EDGE_FILTER_WIDTH; j++) {
            int row = (start_row + i < EDGE_FILTER_HEIGHT) ? start_row + i
                    : start_row + i - EDGE_FILTER_HEIGHT;
            int col = (start_col + j < EDGE_FILTER_WIDTH) ? start_col + j
                    : start_col + j - EDGE_FILTER_WIDTH;
            response0 += window[row][col] * SOBEL_FILTER_0[i][j];
            response1 += window[row][col] * SOBEL_FILTER_1[i][j];
            response2 += window[row][col] * SOBEL_FILTER_2[i][j];
            response3 += window[row][col] * SOBEL_FILTER_3[i][j];
        }
    }

    return response0 >= EDGE_THRESHOLD || (~response0 + 1) >= EDGE_THRESHOLD ||
    	   response1 >= EDGE_THRESHOLD || (~response1 + 1) >= EDGE_THRESHOLD ||
		   response2 >= EDGE_THRESHOLD || (~response2 + 1) >= EDGE_THRESHOLD ||
		   response3 >= EDGE_THRESHOLD || (~response3 + 1) >= EDGE_THRESHOLD;
}

/*----------------------------------------------------------------------------
 * Top Function for Synthesis
 *----------------------------------------------------------------------------*/

/**
 * The top function for the edge detection module when it is synthesized by
 * itself.
 *
 * This is the function that HLS will look for if the edge detection module is
 * synthesized into its own IP block.
 **/
void edge_detection_top(grayscale_stream_t& grayscale_stream,
        edge_detection_stream_t& edge_detection_stream) {
#pragma HLS INTERFACE axis port=grayscale_stream
#pragma HLS INTERFACE axis port=edge_detection_stream

    edge_detection<IMAGE_WIDTH, IMAGE_HEIGHT>(grayscale_stream,
            edge_detection_stream);
    return;
}
