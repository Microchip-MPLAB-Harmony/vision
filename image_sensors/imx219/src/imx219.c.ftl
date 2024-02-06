
#include "vision/drivers/image_sensor/drv_image_sensor.h"


#define IMX219_SLAVE_ADDRESS      0x10
#define IMX219_CHIPIDH_ADDRESS    0x00
#define IMX219_CHIPIDL_ADDRESS    0x01
#define IMX219_CHIPIDH            0x02
#define IMX219_CHIPIDL            0x19
#define IMX219_CHIPID_MASK        0xFFFF

static const DRV_IMAGE_SENSOR_REG imx219_raw_vga[] =
{
    {0x0100, 0x00},
    //Begin - Acces command sequence to access in this address area "0x3000-0x5FFF"
    {0x30EB, 0x05},
    {0x30EB, 0x0C},
    {0x300A, 0xFF},
    {0x300B, 0xFF},
    {0x30EB, 0x05},
    {0x30EB, 0x09},
    //End - Acces command sequence to access in this address area "0x3000-0x5FFF"
    {0x0114, 0x01}, //2 Lanes Mode
    {0x0128, 0x00}, //DPHY_CTRL => 0=auto mode
    {0x012A, 0x18}, //EXCLK_FREQ[15:8]
    {0x012B, 0x00}, //EXCLK_FREQ[7:0] = 24 MHz

    {0x0157, 0x40},

    {0x015a, 0x03},
    {0x015b, 0x00},

    {0x0160, 0x03},
    {0x0161, 0x00},

    {0x0162, 0x0D}, //LINE_LENGTH_A[15:8]
    {0x0163, 0x78}, //LINE_LENGTH_A[7:0] = 3448 pixels - "line_length_pck" Units: Pixels
    {0x0164, 0x01}, //XADD_STA_A[11:8]
    {0x0165, 0x48}, //XADD_STA_A[7:0]
    {0x0166, 0x0B}, //XADD_END_A[11:8]
    {0x0167, 0x47}, //XADD_END_A[7:0]

    {0x0168, 0x00}, //YADD_STA_A[11:8]
    {0x0169, 0xF0}, //YADD_STA_A[7:0]
    {0x016A, 0x08}, //YADD_END_A[11:8]
    {0x016B, 0x6F}, //YADD_END_A[7:0]

    {0x016C, 0x02}, //x_output_size[11:8]
    {0x016D, 0x80}, //x_output_size[7:0]
    {0x016E, 0x01}, //y_output_size[11:8]
    {0x016F, 0xE0}, //y_output_size[7:0]

    {0x0170, 0x01}, //X_ODD_INC_A
    {0x0171, 0x01}, //Y_ODD_INC_A
    {0x0174, 0x02}, //BINNING_MODE_H_A
    {0x0175, 0x02}, //BINNING_MODE_V_A
    {0x018C, 0x0A}, //CSI_DATA_FORMAT_A [15:8]
    {0x018D, 0x0A}, //CSI_DATA_FORMAT_A [7:0] = RAW10

    {0x0301, 0x05}, //VTPXCK_DIV[4:0] = 5
    {0x0303, 0x01}, //VTSYCK_DIV[1:0] = 1
    {0x0304, 0x03}, //PREPLLCK_VT_DIV[7:0] = 3
    {0x0305, 0x03}, //PREPLLCK_OP_DIV[7:0] = 3
    {0x0306, 0x00}, //PLL_VT_MPY[10:8]
    {0x0307, 0x39}, //PLL_VT_MPY[10:8]
    {0x0309, 0x0A}, // OPPXCK_DIV = 10
    {0x030B, 0x01}, // OPSYCK_DIV = 1
    {0x030C, 0x00}, //PLL_OP_MPY[10:8]
    {0x030D, 0x72}, //PLL_OP_MPY[7:0]

    {0x455E, 0x00},
    {0x471E, 0x4B},
    {0x4767, 0x0F},
    {0x4750, 0x14},
    {0x4540, 0x00},
    {0x47B4, 0x14},
    {0x4713, 0x30},
    {0x478B, 0x10},
    {0x478F, 0x10},
    {0x4793, 0x10},
    {0x4797, 0x0E},
    {0x479B, 0x0E},
    {0xFF, 0xFF}
};

static const DRV_IMAGE_SENSOR_REG imx219_raw_1080p[] =
{
    {0x0100, 0x00},
    //Begin - Acces command sequence to access in this address area "0x3000-0x5FFF"
    {0x30eb, 0x05},
    {0x30eb, 0x0c},
    {0x300a, 0xff},
    {0x300b, 0xff},
    {0x30eb, 0x05},
    {0x30eb, 0x09},
    //End - Acces command sequence to access in this address area "0x3000-0x5FFF"

    {0x0110, 0x00}, //Virtual Channel = 0
    {0x0114, 0x01}, //2 Lanes Mode
    {0x0128, 0x00}, //DPHY_CTRL => 0=auto mode              //10
    {0x012a, 0x18}, //EXCLK_FREQ[15:8]
    {0x012b, 0x00}, //EXCLK_FREQ[7:0] = 24 MHz
    {0x0162, 0x0d}, //LINE_LENGTH_A[15:8]
    {0x0163, 0x78}, //LINE_LENGTH_A[7:0] = 3448 pixels - "line_length_pck" Units: Pixels
    {0x0164, 0x02}, //XADD_STA_A[11:8]
    {0x0165, 0xa8}, //XADD_STA_A[7:0] = X top left = 680 pixels - "X-address of the top left corner of the visible pixel data" Units: Pixels
    {0x0166, 0x0a}, //XADD_END_A[11:8]
    {0x0167, 0x27}, //XADD_END_A[7:0] = X bottom right = 2599 pixels - "X-address of the bottom right corner of the visible pixel data" Units: Pixels
    {0x0168, 0x02}, //YADD_STA_A[11:8]
    {0x0169, 0xb4}, //YADD_STA_A[7:0] = Y top left = 692 lines - "Y-address of the top left corner of the visible pixel data" Units: Lines     //20
    {0x016a, 0x06}, //YADD_END_A[11:8]
    {0x016b, 0xeb}, //YADD_END_A[7:0] = Y bottom right = 1771 pixels - "X-address of the bottom right corner of the visible pixel data" Units: Pixels
    {0x016c, 0x07}, //x_output_size[11:8]
    {0x016d, 0x80}, //x_output_size[7:0] = 1920 pixels (Active Area H) - "Width of image data output from the sensor module" Units: Pixels
    {0x016e, 0x04}, //y_output_size[11:8]
    {0x016f, 0x38}, //y_output_size[7:0] = 1080 lines (Active Area V) - "Height of image data output from the sensor module" Units: Lines
    {0x0170, 0x01}, //X_ODD_INC_A
    {0x0171, 0x01}, //Y_ODD_INC_A
    {0x0174, 0x00}, //BINNING_MODE_H_A = no binning
    {0x0175, 0x00}, //BINNING_MODE_V_A = no binning             //30

    {0x018C, 0x0A}, //CSI_DATA_FORMAT_A [15:8]
    {0x018D, 0x0A}, //CSI_DATA_FORMAT_A [7:0] = RAW10

    {0x0301, 0x05}, //VTPXCK_DIV[4:0] = 5
    {0x0303, 0x01}, //VTSYCK_DIV[1:0] = 1
    {0x0304, 0x03}, //PREPLLCK_VT_DIV[7:0] = 3
    {0x0305, 0x03}, //PREPLLCK_OP_DIV[7:0] = 3
    {0x0306, 0x00}, //PLL_VT_MPY[10:8]
    {0x0307, 0x39}, //PLL_VT_MPY[7:0] = 57(0x39)
    {0x0309, 0x0A}, // OPPXCK_DIV = 10
    {0x030b, 0x01}, // OPSYCK_DIV = 1                              //40
    {0x030c, 0x00}, //PLL_OP_MPY[10:8]
    {0x030d, 0x72}, //PLL_OP_MPY[7:0] = 114(0x72)

#if 0
    {0x0600, 0x00}, //Test Pattern Mode[0]
    {0x0601, 0x02}, //Test Pattern Mode[7:0] = 0x1 (Solide Color) //0x2 (100% Color Bar)

    {0x0602, 0x0}, //Data_Red[9:8]
    {0x0603, 0x00}, //Data_Red[7:0]
    {0x0604, 0x0}, //Data_GreenR[9:8]
    {0x0605, 0x00}, //Data_GreenR[7:0]
    {0x0606, 0x3}, //Data_Blue[9:8]
    {0x0607, 0xFF}, //Data_Blue[7:0]                              //50
    {0x0608, 0x0}, //Data_GreenB[9:8]
    {0x0609, 0x00}, //Data_GreenB[7:0]

    {0x0620, 0x00}, //TP_WINDOW_X_OFFSET[11:8]
    {0x0621, 0x00}, //TP_WINDOW_X_OFFSET[7:0] = 0
    {0x0622, 0x00}, //TP_WINDOW_Y_OFFSET[11:8]
    {0x0623, 0x00}, //TP_WINDOW_Y_OFFSET[7:0] = 0
    {0x0624, 0x07}, //TP_WINDOW_WIDTH[11:8]
    {0x0625, 0x80}, //TP_WINDOW_WIDTH[7:0] = 1920 (0x780) //(largeur)
    {0x0626, 0x04}, //TP_WINDOW_HEIGHT[11:8]
    {0x0627, 0x38}, //TP_WINDOW_HEIGHT[7:0] = 1080 (0x438)//(taille)         //60
#endif

    {0x455e, 0x00},
    {0x471e, 0x4b},
    {0x4767, 0x0f},
    {0x4750, 0x14},
    {0x4540, 0x00},
    {0x47b4, 0x14},
    {0x4713, 0x30},
    {0x478b, 0x10},
    {0x478f, 0x10},
    {0x4793, 0x10},                                         //70
    {0x4797, 0x0e},
    {0x479b, 0x0e},
    {0x0162, 0x0d},
    {0x0163, 0x78},

    {0x0100, 0x01}, /* mode select streaming off */         //75
    {0xFF, 0xFF},
};

static const DRV_IMAGE_SENSOR_CONFIGS imx219_raw_vga_config =
{
    0,
    DRV_IMAGE_SENSOR_VGA,
    DRV_IMAGE_SENSOR_RAW_BAYER,
    DRV_IMAGE_SENSOR_10_BIT,
    1,
    640,
    480,
    imx219_raw_vga
};

static const DRV_IMAGE_SENSOR_CONFIGS imx219_raw_1080p_config =
{
    0,
    DRV_IMAGE_SENSOR_1080P,
    DRV_IMAGE_SENSOR_RAW_BAYER,
    DRV_IMAGE_SENSOR_10_BIT,
    1,
    1920,
    1080,
    imx219_raw_1080p
};

const DRV_IMAGE_SENSOR_OBJ imx219_device =
{
    "IMX219",
    IMX219_SLAVE_ADDRESS,
    DRV_IMAGE_SENSOR_I2C_REG_2BYTE_DATA_BYTE,    /* I2C interface mode  */
    IMX219_CHIPIDH_ADDRESS,             /* Register address for product ID high byte */
    IMX219_CHIPIDL_ADDRESS,             /* Register address for product ID low byte*/
    IMX219_CHIPIDH,                     /* product ID high byte */
    IMX219_CHIPIDL,                     /* product ID low byte */
    IMX219_CHIPID_MASK,             /* version mask */
    0x0100,
    0x01,
    0x00,
    {
        &imx219_raw_vga_config,
        & imx219_raw_1080p_config
    },
    (DRV_HANDLE)NULL
};
