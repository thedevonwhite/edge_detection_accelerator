/**
 * @file blob_detection_test.cpp
 * @date Thursday,  December 01,  2016 at 03:16:55 PM EST
 * @author Brandon Perez (bmperez)
 *
 * This file contains the testbench for the blob detection module.
 *
 * @bug No known bugs.
 **/

#include <ap_int.h>                 // Arbitrary precision integer types

#include "grayscale.h"              // Definition of the grayscale types
#include "edge_detection.h"         // Our interface and blob detection types
#include "image.h"                  // Definition of image info

/*----------------------------------------------------------------------------
 * Internal Definitions
 *----------------------------------------------------------------------------*/

// The size of the test vector (image)
const int TEST_VEC_WIDTH        = IMAGE_WIDTH;
const int TEST_VEC_HEIGHT       = IMAGE_HEIGHT;

/*----------------------------------------------------------------------------
 * Test Vectors
 *----------------------------------------------------------------------------*/

// The input test vector of image monochrome values
static const grayscale_t INPUT_GRAYSCALE[TEST_VEC_HEIGHT][TEST_VEC_WIDTH] = {
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},
		{0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},
		{0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},
		{0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

// The expected output vector of
static const ap_uint<1> OUTPUT_DETECTIONS[TEST_VEC_HEIGHT][TEST_VEC_WIDTH] = {
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 1, 1, 1, 1, 0},
		{0, 0, 1, 1, 0, 0, 1, 0},
		{0, 0, 1, 1, 0, 0, 1, 0},
		{0, 0, 1, 1, 1, 1, 1, 0},
		{0, 0, 1, 1, 1, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
};

/*----------------------------------------------------------------------------
 * Testbench
 *----------------------------------------------------------------------------*/

int main()
{
	grayscale_stream_t grayscale_stream;
    edge_detection_stream_t edge_detection_stream;

    for (int i = 0; i < TEST_VEC_WIDTH*TEST_VEC_HEIGHT; i++) {
    	grayscale_axis_t inpkt;
    	inpkt.tdata = INPUT_GRAYSCALE[0][i];
    	inpkt.tkeep = -1;
    	inpkt.tlast = (i == TEST_VEC_WIDTH*TEST_VEC_HEIGHT-1) ? 1 : 0;
    	grayscale_stream << inpkt;
    }

	edge_detection<8,8>(grayscale_stream, edge_detection_stream);

	int last = 0;
	int count = 0;
	int image[TEST_VEC_HEIGHT][TEST_VEC_WIDTH];

	while (last == 0) {
		if (!edge_detection_stream.empty()) {
			edge_detection_axis_t outpkt;
			edge_detection_stream >> outpkt;
			last = outpkt.tlast.to_int();
			image[0][count] = outpkt.tdata.to_int();
			count += 1;
		}
	}
	printf("Count: %d\n", count);
	for (int i = 0; i < TEST_VEC_HEIGHT; i++) {
		for (int j = 0; j < TEST_VEC_WIDTH; j++) {
			if (image[i][j]) {
				if (OUTPUT_DETECTIONS[i][j])
					printf("Edge Detected Successfully at row: %d, col: %d\n", i, j);
				else
					printf("Error: False Positive at row: %d, col: %d\n", i, j);
			}
			else {
				if (OUTPUT_DETECTIONS[i][j])
					printf("Edge Missed at row: %d, col: %d\n", i, j);
			}
		}
	}

	for (int i = 0; i < TEST_VEC_HEIGHT; i++) {
		for (int j = 0; j < TEST_VEC_WIDTH; j++) {
			printf("%d ", image[i][j]);
		}
		printf("\n");
	}

	return 0;
}
