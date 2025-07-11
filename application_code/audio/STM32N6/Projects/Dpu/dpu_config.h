/**
  ******************************************************************************
  * @file    dpu_config.h
  * @author  STMicroelectronics - AIS - MCD Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   Configure the getting started functionality
  *
  * Each logic module of the application should define a DEBUG control byte
  * used to turn on/off the log for the module.
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DPU_CONFIG_H_
#define DPU_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define CTRL_AI_CLASS_DISTRIBUTION               (1U)
#define CTRL_AI_CLASS_IDX                        (2U)
#define CTRL_AI_SPECTROGRAM                      (3U)
#define CTRL_AI_GRAV_ROT_SUPPR                   (4U)
#define CTRL_AI_STFT                             (5U)
#define CTRL_AI_MEL_SPECTROGRAM                  (6U)
#define CTRL_AI_ISTFT                            (7U)
#define CTRL_AI_BYPASS                           (8U)
#define COM_TYPE_HEADSET                         (9U)
  
#include "ai_model_config.h"

#ifndef CTRL_AI_HW_SELECT
#define CTRL_AI_HW_SELECT                        B_U585I_IOT02A
#endif

#ifndef CTRL_SEQUENCE
#define CTRL_SEQUENCE                            {CTRL_CMD_PARAM_AI,0}
#endif

#ifndef CTRL_X_CUBE_AI_MODE_NAME
#define CTRL_X_CUBE_AI_MODE_NAME                 "X-CUBE-AI HAR"
#endif
#ifndef CTRL_X_CUBE_AI_MODE_NETWORK_MODEL_NAME
#define CTRL_X_CUBE_AI_MODE_NETWORK_MODEL_NAME   "network"
#endif

#ifndef CTRL_X_CUBE_AI_MODE_NB_OUTPUT
#define CTRL_X_CUBE_AI_MODE_NB_OUTPUT            (1U)
#endif
#ifndef CTRL_X_CUBE_AI_MODE_OUTPUT_1
#define CTRL_X_CUBE_AI_MODE_OUTPUT_1             (CTRL_AI_CLASS_DISTRIBUTION)
#endif
#ifndef CTRL_X_CUBE_AI_MODE_CLASS_NUMBER
#define CTRL_X_CUBE_AI_MODE_CLASS_NUMBER         (4U)
#endif
#ifndef CTRL_X_CUBE_AI_MODE_CLASS_LIST
#define CTRL_X_CUBE_AI_MODE_CLASS_LIST           {"Stationary","Walking","Jogging","Biking"}
#endif
#ifndef CTRL_X_CUBE_AI_SENSOR_NAME
#define CTRL_X_CUBE_AI_SENSOR_NAME               "ism330dhcx"
#endif
#ifndef CTRL_X_CUBE_AI_SENSOR_TYPE
#define CTRL_X_CUBE_AI_SENSOR_TYPE               COM_TYPE_ACC
#endif
#ifndef CTRL_X_CUBE_AI_SENSOR_ODR
#define CTRL_X_CUBE_AI_SENSOR_ODR                (26.0F)
#endif
#ifndef CTRL_X_CUBE_AI_SENSOR_FS
#define CTRL_X_CUBE_AI_SENSOR_FS                 (4.0F)
#endif
#ifndef CTRL_X_CUBE_AI_NB_SAMPLES
#define CTRL_X_CUBE_AI_NB_SAMPLES                (20U)
#endif
#ifndef CTRL_X_CUBE_AI_PREPROC
#define CTRL_X_CUBE_AI_PREPROC                   (CTRL_AI_GRAV_ROT_SUPPR)
#endif
#ifndef CTRL_X_CUBE_AI_POSTPROC
#define CTRL_X_CUBE_AI_POSTPROC                  (CTRL_AI_BYPASS)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_NMEL
#define CTRL_X_CUBE_AI_SPECTROGRAM_NMEL          (30U)
#endif


#ifdef CTRL_X_CUBE_AI_SPECTROGRAM_COL
  #ifdef CTRL_X_CUBE_AI_SPECTROGRAM_COL_NO_OVL
    #warning CTRL_X_CUBE_AI_SPECTROGRAM_COL_NO_OVL and CTRL_X_CUBE_AI_SPECTROGRAM_COL cannot be both defined
  #endif
  #ifdef CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL
    #warning CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL and CTRL_X_CUBE_AI_SPECTROGRAM_COL cannot be both defined
  #endif
  #define CTRL_X_CUBE_AI_SPECTROGRAM_COL_NO_OVL (CTRL_X_CUBE_AI_SPECTROGRAM_COL)
  #define CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL    (0)
#endif

#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_COL
  #ifndef CTRL_X_CUBE_AI_SPECTROGRAM_COL_NO_OVL
    #define CTRL_X_CUBE_AI_SPECTROGRAM_COL_NO_OVL (32)
  #endif
  #ifndef CTRL_X_CUBE_AI_SPECTROGRAM_COL_NO_OVL
    #define CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL (0)
  #endif
  #define CTRL_X_CUBE_AI_SPECTROGRAM_COL           (CTRL_X_CUBE_AI_SPECTROGRAM_COL_NO_OVL+2*CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL)
#endif

#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH
#define CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH    (1024U)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_NFFT
#define CTRL_X_CUBE_AI_SPECTROGRAM_NFFT          (1024U)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH
#define CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH (1024U)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_NORMALIZE
#define CTRL_X_CUBE_AI_SPECTROGRAM_NORMALIZE     (1U)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_FORMULA
#define CTRL_X_CUBE_AI_SPECTROGRAM_FORMULA       (MEL_SLANEY)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_FMIN
#define CTRL_X_CUBE_AI_SPECTROGRAM_FMIN          (0.0F)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_FMAX
#define CTRL_X_CUBE_AI_SPECTROGRAM_FMAX          (CTRL_X_CUBE_AI_SENSOR_ODR / 2.0F)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_TYPE
#define CTRL_X_CUBE_AI_SPECTROGRAM_TYPE          (SPECTRUM_TYPE_POWER)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_LOG_FORMULA
#define CTRL_X_CUBE_AI_SPECTROGRAM_LOG_FORMULA   (LOGMELSPECTROGRAM_SCALE_LOG) /*LOGMELSPECTROGRAM_SCALE_DB */
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_SILENCE_THR
#define CTRL_X_CUBE_AI_SPECTROGRAM_SILENCE_THR   (0) /* 0 means disabled */
#endif

#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_WIN
#define CTRL_X_CUBE_AI_SPECTROGRAM_WIN           (hannWin_1024)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_MEL_LUT
#define CTRL_X_CUBE_AI_SPECTROGRAM_MEL_LUT       (melFilterLut_1024_30)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_MEL_START_IDX
#define CTRL_X_CUBE_AI_SPECTROGRAM_MEL_START_IDX (melFiltersStartIndices_1024_30)
#endif
#ifndef CTRL_X_CUBE_AI_SPECTROGRAM_MEL_STOP_IDX
#define CTRL_X_CUBE_AI_SPECTROGRAM_MEL_STOP_IDX  (melFiltersStopIndices_1024_30)
#endif
#ifndef CTRL_X_CUBE_AI_AUDIO_OUT_DB_THRESHOLD
#define CTRL_X_CUBE_AI_AUDIO_OUT_DB_THRESHOLD (-100)
#endif

#define CTRL_X_CUBE_AI_ACQ_LENGTH (CTRL_X_CUBE_AI_SPECTROGRAM_COL_NO_OVL*CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH)
#define CTRL_X_CUBE_AI_ACQ_LENGTH_MS (uint32_t)(1000.0F * (float) CTRL_X_CUBE_AI_ACQ_LENGTH / (float) CTRL_X_CUBE_AI_SENSOR_ODR )

#ifdef __cplusplus
}
#endif

#endif /* DPU_CONFIG_H_ */
