#include "edge_detection.h"

edge_detection_response_t SOBEL_FILTER_0[EDGE_FILTER_HEIGHT][EDGE_FILTER_WIDTH] =
{
 {1, 0, -1},
 {2, 0, -2},
 {1, 0, -1},
};

edge_detection_response_t SOBEL_FILTER_1[EDGE_FILTER_HEIGHT][EDGE_FILTER_WIDTH] =
{
 { 1,  2,  1},
 { 0,  0,  0},
 {-1, -2, -1},
};

edge_detection_response_t SOBEL_FILTER_2[EDGE_FILTER_HEIGHT][EDGE_FILTER_WIDTH] =
{
 {2,  1,  0},
 {1,  0, -1},
 {0, -1, -2},
};

edge_detection_response_t SOBEL_FILTER_3[EDGE_FILTER_HEIGHT][EDGE_FILTER_WIDTH] =
{
 { 0,  1, 2},
 {-1,  0, 1},
 {-2, -1, 0},
};
