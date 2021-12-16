# TM1637

**STM32** Low Layer(LL) library. TM1637 is a kind of LED (light-emitting diode display) drive control special circuit.

<img src="https://github.com/ximtech/TM1637/blob/main/example/TM1637.PNG" alt="image" width="300"/>

### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME TM1637
        GITHUB_REPOSITORY ximtech/TM1637
        GIT_TAG origin/main)
```

### Project configuration

1. Start project with STM32CubeMX:
2. Select: Project Manager -> Advanced Settings -> GPIO -> LL
3. Generate Code
4. Add sources to project:

```cmake
include_directories(${includes}
        ${TM1637_DIRECTORY})   # source directories

file(GLOB_RECURSE SOURCES ${sources}
        ${TM1637_SOURCES})    # source files
```

3. Then Build -> Clean -> Rebuild Project

## Wiring

<img src="https://github.com/ximtech/TM1637/blob/main/example/wiring.PNG" alt="image" width="300"/>

## Usage

In `TM1637.h` default defines. Override them in `main.h` if needed

```c
#define TM1637_DIGITS 4

#define TM_PERIPHERAL  LL_AHB1_GRP1_PERIPH_GPIOB
#define TM_PORT        GPIOB
#define TM_CLK_PIN     LL_GPIO_PIN_0
#define TM_DAT_PIN     LL_GPIO_PIN_1
```

- Usage example: [link](https://github.com/ximtech/TM1637/blob/main/example/example.c)
