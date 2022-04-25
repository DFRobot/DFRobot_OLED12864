/*!
 * @file  OLEDDisplayUi.h
 * @brief  OLED Display Ui
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @copyright  Copyright (c) 2016 by Daniel Eichhorn
 * @copyright  Copyright (c) 2016 by Fabrice Weinberg
 * @licence  The MIT License (MIT)
 * @author  [wxzed](xiao.wu@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-25
 * @url  https://github.com/DFRobot/DFRobot_OLED12864
 */
#ifndef OLEDDISPLAYUI_h
#define OLEDDISPLAYUI_h

#include <Arduino.h>
#include "OLEDDisplay.h"

// #define DEBUG_OLEDDISPLAYUI(...) Serial.printf( __VA_ARGS__ )

#ifndef DEBUG_OLEDDISPLAYUI
#define DEBUG_OLEDDISPLAYUI(...)
#endif

/**
 * @enum AnimationDirection
 * @brief Animation Direction
 */
enum AnimationDirection {
  SLIDE_UP,
  SLIDE_DOWN,
  SLIDE_LEFT,
  SLIDE_RIGHT
};

/**
 * @enum IndicatorPosition
 * @brief Indicator Position
 */
enum IndicatorPosition {
  TOP,
  RIGHT,
  BOTTOM,
  LEFT
};

/**
 * @enum IndicatorDirection
 * @brief Indicator Direction
 */
enum IndicatorDirection {
  LEFT_RIGHT,
  RIGHT_LEFT
};

/**
 * @enum FrameState
 * @brief Frame State
 */
enum FrameState {
  IN_TRANSITION,
  FIXED
};


const char ANIMATION_activeSymbol[] PROGMEM = {
  0x00, 0x18, 0x3c, 0x7e, 0x7e, 0x3c, 0x18, 0x00
};

const char ANIMATION_inactiveSymbol[] PROGMEM = {
  0x00, 0x0, 0x0, 0x18, 0x18, 0x0, 0x0, 0x00
};


//!< Structure of the UiState
struct OLEDDisplayUiState {
uint64_t     lastUpdate                = 0;
uint16_t      ticksSinceLastStateSwitch = 0;

FrameState    frameState                = FIXED;
uint8_t       currentFrame              = 0;

bool          isIndicatorDrawen         = true;

// Normal = 1, Inverse = -1;
int8_t        frameTransitionDirection  = 1;

bool          manuelControll            = false;

//!< Custom data that can be used by the user
void*         userData                  = NULL;
};

/**
 * @enum LoadingStage
 * @brief Loading Stage
 */
struct LoadingStage {
  const char* process;
  void (*callback)();
};

typedef void (*FrameCallback)(OLEDDisplay *display,  OLEDDisplayUiState* state, int16_t x, int16_t y);
typedef void (*OverlayCallback)(OLEDDisplay *display,  OLEDDisplayUiState* state);
typedef void (*LoadingDrawFunction)(OLEDDisplay *display, LoadingStage* stage, uint8_t progress);

class OLEDDisplayUi
{
public:

    /**
     * @fn OLEDDisplayUi
     * @brief Initialise the OLED Display Ui
     * @param display - OLEDDisplay
     * @return None
     */
    OLEDDisplayUi(OLEDDisplay *display);

    /**
     * @fn init
     * @brief Initialise the display
     * @return None
     */
    void init(void);

    /**
     * @fn setTargetFPS
     * @brief Configure the internal used target FPS
     * @param fps - FPS
     * @return None
     */
    void setTargetFPS(uint8_t fps);

/******************* Automatic Controll ******************/

    /**
     * @fn enableAutoTransition
     * @brief Enable automatic transition to next frame after the some time can be configured with `setTimePerFrame` and `setTimePerTransition`.
     * @return None
     */
    void enableAutoTransition(void);

    /**
     * @fn disableAutoTransition
     * @brief Disable automatic transition to next frame.
     * @return None
     */
    void disableAutoTransition(void);

    /**
     * @fn setAutoTransitionForwards
     * @brief Set the direction if the automatic transitioning
     * @return None
     */
    void setAutoTransitionForwards(void);
    /**
     * @fn setAutoTransitionBackwards
     * @brief Set the direction if the automatic transitioning
     * @return None
     */
    void setAutoTransitionBackwards(void);

    /**
     * @fn setTimePerFrame
     * @brief Set the approx.
     * @param time - time a frame is displayed
     * @return None
     */
    void setTimePerFrame(uint16_t time);

    /**
     * @fn setTimePerTransition
     * @brief Set the approx.
     * @param time - time a transition will take
     * @return None
     */
    void setTimePerTransition(uint16_t time);

/******************* Customize indicator position and style ******************/

    /**
     * @fn enableIndicator
     * @brief Draw the indicator.
     * @n This is the defaut state for all frames if
     * @n the indicator was hidden on the previous frame
     * @n it will be slided in.
     * @return None
     */
    void enableIndicator(void);

    /**
     * @fn disableIndicator
     * @brief Don't draw the indicator.
     * @n This will slide out the indicator
     * @n when transitioning to the next frame.
     * @return None
     */
    void disableIndicator(void);

    /**
     * @fn enableAllIndicators
     * @brief Enable drawing of indicators
     * @return None
     */
    void enableAllIndicators(void);

    /**
     * @fn disableAllIndicators
     * @brief Disable draw of indicators.
     * @return None
     */
    void disableAllIndicators(void);

    /**
     * @fn setIndicatorPosition
     * @brief Set the position of the indicator bar.
     * @param pos - TOP, RIGHT, BOTTOM, LEFT
     * @return None
     */
    void setIndicatorPosition(IndicatorPosition pos);

    /**
     * @fn setIndicatorDirection
     * @brief Set the direction of the indicator bar.
     * @n Defining the order of frames ASCENDING / DESCENDING
     * @param dir - LEFT_RIGHT, RIGHT_LEFT
     * @return None
     */
    void setIndicatorDirection(IndicatorDirection dir);

    /**
     * @fn setActiveSymbol
     * @brief Set the symbol to indicate an active frame in the indicator bar.
     * @param symbol - symbol
     * @return None
     */
    void setActiveSymbol(const char* symbol);

    /**
     * @fn setInactiveSymbol
     * @brief Set the symbol to indicate an inactive frame in the indicator bar.
     * @param symbol - symbol
     * @return None
     */
    void setInactiveSymbol(const char* symbol);

/******************* Frame settings ******************/

    /**
     * @fn setFrameAnimation
     * @brief Configure what animation is used to transition from one frame to another
     * @param dir - SLIDE_UP, SLIDE_DOWN, SLIDE_LEFT, SLIDE_RIGHT
     * @return None
     */
    void setFrameAnimation(AnimationDirection dir);

    /**
     * @fn setFrames
     * @brief Add frame drawing functions
     * @param frameFunctions - frame Functions
     * @param frameCount - frame Count
     * @return None
     */
    void setFrames(FrameCallback* frameFunctions, uint8_t frameCount);

/******************* Overlay ******************/

    /**
     * @fn setOverlays
     * @brief Add overlays drawing functions that are draw independent of the Frames
     * @param overlayFunctions - overlay Functions
     * @param overlayCount - overlay Count
     * @return None
     */
    void setOverlays(OverlayCallback* overlayFunctions, uint8_t overlayCount);

/******************* Loading animation ******************/

    /**
     * @fn setLoadingDrawFunction
     * @brief Set the function that will draw each step
     * @n in the loading animation
     * @param loadingFunction - loading Function
     * @return None
     */
    void setLoadingDrawFunction(LoadingDrawFunction loadingFunction);

    /**
     * @fn runLoadingProcess
     * @brief Run the loading process
     * @param stages - stages
     * @param stagesCount - stages Count
     * @return None
     */
    void runLoadingProcess(LoadingStage* stages, uint8_t stagesCount);

/******************* Manual Control ******************/

    /**
     * @fn nextFrame
     * @brief next Frame
     * @return None
     */
    void nextFrame(void);

    /**
     * @fn previousFrame
     * @brief previous Frame
     * @return None
     */
    void previousFrame(void);

    /**
     * @fn switchToFrame
     * @brief Switch without transition to frame `frame`.
     * @param frame - frame
     * @return None
     */
    void switchToFrame(uint8_t frame);

    /**
     * @fn transitionToFrame
     * @brief Transition to frame `frame`, when the `frame` number is bigger than the current
     * @n frame the forward animation will be used, otherwise the backwards animation is used.
     * @param frame - frame
     * @return None
     */
    void transitionToFrame(uint8_t frame);

    /**
     * @fn getUiState
     * @brief State Info
     * @return None
     */
    OLEDDisplayUiState* getUiState(void);

    /**
     * @fn update
     * @brief update
     * @return int8_t
     */
    int8_t update(void);

private:
    OLEDDisplay             *display;

    //!< Symbols for the Indicator
    IndicatorPosition   indicatorPosition         = BOTTOM;
    IndicatorDirection  indicatorDirection        = LEFT_RIGHT;

    const char*         activeSymbol              = ANIMATION_activeSymbol;
    const char*         inactiveSymbol            = ANIMATION_inactiveSymbol;

    bool                shouldDrawIndicators      = true;

    //!< Values for the Frames
    AnimationDirection  frameAnimationDirection   = SLIDE_RIGHT;

    int8_t              lastTransitionDirection   = 1;

    uint16_t            ticksPerFrame             = 151; //!< ~ 5000ms at 30 FPS
    uint16_t            ticksPerTransition        = 15;  //!< ~  500ms at 30 FPS

    bool                autoTransition            = true;

    FrameCallback*      frameFunctions;
    uint8_t             frameCount                = 0;

    //!< Internally used to transition to a specific frame
    int8_t              nextFrameNumber           = -1;

    //!< Values for Overlays
    OverlayCallback*    overlayFunctions;
    uint8_t             overlayCount              = 0;

    //!< Will the Indicator be drawen
    //!< 3 Not drawn in both frames
    //!< 2 Drawn this frame but not next
    //!< 1 Not drown this frame but next
    //!< 0 Not known yet
    uint8_t                indicatorDrawState        = 1;

    //!< Loading screen
    LoadingDrawFunction loadingDrawFunction       = [](OLEDDisplay *display, LoadingStage* stage, uint8_t progress) {
      display->setTextAlignment(TEXT_ALIGN_CENTER);
      display->setFont(ArialMT_Plain_10);
      display->drawString(64, 18, stage->process);
      display->drawProgressBar(4, 32, 120, 8, progress);
    };

    //!< UI State
    OLEDDisplayUiState      state;

    //!< Bookeeping for update
    uint8_t             updateInterval            = 33;

    uint8_t             getNextFrameNumber(void);
    void                drawIndicator(void);
    void                drawFrame(void);
    void                drawOverlays(void);
    void                tick(void);
    void                resetState(void);
};
#endif
