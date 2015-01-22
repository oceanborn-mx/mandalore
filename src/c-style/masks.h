// 5x5 shapes
// MSHAPE_CROSS   MSHAPE_DIAMOND    MSHAPE_OCTO    MSHAPE_FULL
//        00100            00100          01110          11111
//        00100            01110          11111          11111
//        11111            11111          11111          11111
//        00100            01110          11111          11111
//        00100            00100          01110          11111

// masks 3x3
// shaped-cross mask
const unsigned int MASK33CROSS[3][3]   =  {{0xFF000000, 0xFFFFFFFF, 0xFF000000},
				           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
				           {0xFF000000, 0xFFFFFFFF, 0xFF000000}};                                     
				           

// shaped-full mask
const unsigned int MASK33FULL[3][3]    =  {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                                           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                                           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};

// masks 5x5
// shaped-cross mask
const unsigned int MASK55CROSS[5][5]   =  {{0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFF000000},
				           {0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFF000000},
				           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                                           {0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFF000000},
				           {0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFF000000}};

// shaped-diamond mask
const unsigned int MASK55DIAMOND[5][5] =  {{0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFF000000},
				           {0xFF000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000},
				           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                                           {0xFF000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000},
                                           {0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFF000000}};

// shaped-octo mask
const unsigned int MASK55OCTO[5][5]    =  {{0xFF000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000},
				           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
				           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                                           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                                           {0xFF000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000}};

// shaped-full mask
const unsigned int MASK55FULL[5][5]    =  {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
				           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
				           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                                           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
                                           {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
