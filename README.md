# STM32 based intelligent game console

## Introduction
This project is based on the <strong>STM32F407</strong> single-chip microcomputer and aims to create a multifunctional game console. The unique feature of this game console is its Type-C interface. Through the Type-C interface, games, music, and pictures can be downloaded. The equipped high-definition display screen can show clear pictures, smooth game interfaces, and music playback interfaces. The sound playback is realized through a buzzer. This project also includes functions such as time setting and stopwatch timing.

## source file
this repository contains **Schematic diagram** and **InfoNes**, The Ide is keil5, you can open it,just by click it.

### File structure
```
├─InfoNES
│  ├─Core
│  │  ├─Inc
│  │  └─Src
│  ├─Drivers
│  │  ├─CMSIS
│  │  │  ├─Core
│  │  │  │  ├─Include
│  │  │  │  └─Template
│  │  │  │      └─ARMv8-M
│  │  │  ├─Core_A
│  │  │  │  ├─Include
│  │  │  │  └─Source
│  │  │  ├─Device
│  │  │  │  └─ST
│  │  │  │      └─STM32F4xx
│  │  │  │          ├─Include
│  │  │  │          └─Source
│  │  │  │              └─Templates
│  │  │  │                  ├─arm
│  │  │  │                  ├─gcc
│  │  │  │                  └─iar
│  │  │  │                      └─linker
│  │  │  ├─docs
│  │  │  │  └─General
│  │  │  │      └─html
│  │  │  ├─DSP
│  │  │  │  ├─DSP_Lib_TestSuite
│  │  │  │  │  ├─Common
│  │  │  │  │  │  ├─inc
│  │  │  │  │  │  │  ├─basic_math_tests
│  │  │  │  │  │  │  ├─complex_math_tests
│  │  │  │  │  │  │  ├─controller_tests
│  │  │  │  │  │  │  ├─fast_math_tests
│  │  │  │  │  │  │  ├─filtering_tests
│  │  │  │  │  │  │  ├─intrinsics_tests
│  │  │  │  │  │  │  ├─matrix_tests
│  │  │  │  │  │  │  ├─statistics_tests
│  │  │  │  │  │  │  ├─support_tests
│  │  │  │  │  │  │  ├─templates
│  │  │  │  │  │  │  └─transform_tests
│  │  │  │  │  │  ├─JTest
│  │  │  │  │  │  │  ├─inc
│  │  │  │  │  │  │  │  ├─arr_desc
│  │  │  │  │  │  │  │  ├─opt_arg
│  │  │  │  │  │  │  │  └─util
│  │  │  │  │  │  │  └─src
│  │  │  │  │  │  ├─platform
│  │  │  │  │  │  │  ├─ARMCC
│  │  │  │  │  │  │  ├─ARMCLANG
│  │  │  │  │  │  │  └─GCC
│  │  │  │  │  │  └─src
│  │  │  │  │  │      ├─basic_math_tests
│  │  │  │  │  │      ├─complex_math_tests
│  │  │  │  │  │      ├─controller_tests
│  │  │  │  │  │      ├─fast_math_tests
│  │  │  │  │  │      ├─filtering_tests
│  │  │  │  │  │      ├─intrinsics_tests
│  │  │  │  │  │      ├─matrix_tests
│  │  │  │  │  │      ├─statistics_tests
│  │  │  │  │  │      ├─support_tests
│  │  │  │  │  │      └─transform_tests
│  │  │  │  │  ├─DspLibTest_FVP
│  │  │  │  │  ├─DspLibTest_MPS2
│  │  │  │  │  └─RefLibs
│  │  │  │  │      ├─inc
│  │  │  │  │      └─src
│  │  │  │  │          ├─BasicMathFunctions
│  │  │  │  │          ├─ComplexMathFunctions
│  │  │  │  │          ├─ControllerFunctions
│  │  │  │  │          ├─FastMathFunctions
│  │  │  │  │          ├─FilteringFunctions
│  │  │  │  │          ├─HelperFunctions
│  │  │  │  │          ├─Intrinsics
│  │  │  │  │          ├─MatrixFunctions
│  │  │  │  │          ├─StatisticsFunctions
│  │  │  │  │          ├─SupportFunctions
│  │  │  │  │          └─TransformFunctions
│  │  │  │  ├─Examples
│  │  │  │  │  └─ARM
│  │  │  │  │      ├─arm_class_marks_example
│  │  │  │  │      ├─arm_convolution_example
│  │  │  │  │      ├─arm_dotproduct_example
│  │  │  │  │      ├─arm_fft_bin_example
│  │  │  │  │      ├─arm_fir_example
│  │  │  │  │      ├─arm_graphic_equalizer_example
│  │  │  │  │      ├─arm_linear_interp_example
│  │  │  │  │      ├─arm_matrix_example
│  │  │  │  │      ├─arm_signal_converge_example
│  │  │  │  │      ├─arm_sin_cos_example
│  │  │  │  │      └─arm_variance_example
│  │  │  │  ├─Include
│  │  │  │  └─Source
│  │  │  │      ├─BasicMathFunctions
│  │  │  │      ├─CommonTables
│  │  │  │      ├─ComplexMathFunctions
│  │  │  │      ├─ControllerFunctions
│  │  │  │      ├─FastMathFunctions
│  │  │  │      ├─FilteringFunctions
│  │  │  │      ├─MatrixFunctions
│  │  │  │      ├─StatisticsFunctions
│  │  │  │      ├─SupportFunctions
│  │  │  │      └─TransformFunctions
│  │  │  ├─Include
│  │  │  ├─Lib
│  │  │  │  ├─ARM
│  │  │  │  ├─GCC
│  │  │  │  └─IAR
│  │  │  ├─NN
│  │  │  │  ├─Examples
│  │  │  │  │  └─ARM
│  │  │  │  │      └─arm_nn_examples
│  │  │  │  │          ├─cifar10
│  │  │  │  │          │  └─RTE
│  │  │  │  │          │      ├─Compiler
│  │  │  │  │          │      ├─_ARMCM0
│  │  │  │  │          │      ├─_ARMCM3
│  │  │  │  │          │      ├─_ARMCM4_FP
│  │  │  │  │          │      └─_ARMCM7_SP
│  │  │  │  │          └─gru
│  │  │  │  │              └─RTE
│  │  │  │  │                  ├─Compiler
│  │  │  │  │                  ├─_ARMCM0
│  │  │  │  │                  ├─_ARMCM3
│  │  │  │  │                  ├─_ARMCM4_FP
│  │  │  │  │                  └─_ARMCM7_SP
│  │  │  │  ├─Include
│  │  │  │  ├─NN_Lib_Tests
│  │  │  │  │  └─nn_test
│  │  │  │  │      ├─Ref_Implementations
│  │  │  │  │      └─RTE
│  │  │  │  │          ├─_ARMCM0
│  │  │  │  │          ├─_ARMCM3
│  │  │  │  │          ├─_ARMCM4_FP
│  │  │  │  │          └─_ARMCM7_SP
│  │  │  │  └─Source
│  │  │  │      ├─ActivationFunctions
│  │  │  │      ├─ConvolutionFunctions
│  │  │  │      ├─FullyConnectedFunctions
│  │  │  │      ├─NNSupportFunctions
│  │  │  │      ├─PoolingFunctions
│  │  │  │      └─SoftmaxFunctions
│  │  │  ├─RTOS
│  │  │  │  └─Template
│  │  │  └─RTOS2
│  │  │      ├─Include
│  │  │      ├─Source
│  │  │      └─Template
│  │  └─STM32F4xx_HAL_Driver
│  │      ├─Inc
│  │      │  └─Legacy
│  │      └─Src
│  ├─EWARM
│  ├─HARDWARE
│  │  ├─BMP_CODE
│  │  ├─CALENDAR
│  │  ├─FatFs
│  │  │  ├─documents
│  │  │  │  ├─doc
│  │  │  │  └─res
│  │  │  └─option
│  │  ├─GAME
│  │  ├─IN_FLASH
│  │  ├─JPEG
│  │  │  └─TjpgDec
│  │  ├─KEY
│  │  ├─L298N
│  │  ├─MALLOC
│  │  ├─MENU
│  │  ├─NES
│  │  │  └─mapper
│  │  │      └─MAP源文件(未修改的)
│  │  ├─OLED
│  │  ├─OV2640
│  │  ├─OV7670
│  │  ├─ST7735S
│  │  ├─ST7789
│  │  ├─TIMER
│  │  ├─UART
│  │  ├─W25QXX
│  │  ├─WAV
│  │  └─zbar-0.10
│  │      └─zbar-0.10
│  │          ├─config
│  │          ├─doc
│  │          │  ├─api
│  │          │  ├─man
│  │          │  └─ref
│  │          ├─examples
│  │          ├─gtk
│  │          ├─include
│  │          │  └─zbar
│  │          ├─perl
│  │          │  ├─examples
│  │          │  ├─inc
│  │          │  │  └─Devel
│  │          │  ├─t
│  │          │  └─ZBar
│  │          ├─plugin
│  │          ├─pygtk
│  │          ├─python
│  │          │  └─test
│  │          ├─qt
│  │          ├─test
│  │          ├─zbar
│  │          │  ├─decoder
│  │          │  ├─processor
│  │          │  ├─qrcode
│  │          │  ├─video
│  │          │  └─window
│  │          ├─zbarcam
│  │          └─zbarimg
│  ├─MDK-ARM
│  │  ├─DebugConfig
│  │  ├─InfoNES
│  │  └─RTE
│  │      └─_InfoNES
│  ├─Middlewares
│  │  └─ST
│  │      └─STM32_USB_Device_Library
│  │          ├─Class
│  │          │  └─MSC
│  │          │      ├─Inc
│  │          │      └─Src
│  │          └─Core
│  │              ├─Inc
│  │              └─Src
│  └─USB_DEVICE
│      ├─App
│      └─Target
└─Schematic_diagram
```

## conclusion
This game console provides users with a rich entertainment experience. With its advanced features and functions, it meets the diverse needs of users. Whether it's playing games, listening to music, or viewing pictures, this console can offer high-quality performance. The time setting and stopwatch functions add practicality to the device, making it more useful in daily life.
