# /*---------------------------------------------------------------------------------------------
#  * Copyright (c) 2022 STMicroelectronics.
#  * All rights reserved.
#  *
#  * This software is licensed under terms that can be found in the LICENSE file in
#  * the root directory of this software component.
#  * If no LICENSE file comes with this software, it is provided AS-IS.
#  *--------------------------------------------------------------------------------------------*/

import pathlib
import numpy as np
import tensorflow as tf
import tqdm
import sys
import os
from hydra.core.hydra_config import HydraConfig
from omegaconf import DictConfig

from common.utils import get_model_name_and_its_input_shape
from common.quantization import quantize_onnx
from common.evaluation import model_is_quantized
from common.optimization import model_formatting_ptq_per_tensor
from common.onnx_utils import onnx_model_converter
from src.utils import tf_segmentation_dataset_to_np_array

from typing import Optional



def _tflite_ptq_quantizer(model: tf.keras.Model = None, quantization_ds: tf.data.Dataset = None,
                         output_dir: str = None, export_dir: str = None, input_shape: tuple = None,
                         quantization_granularity: str = None, quantization_input_type: str = None,
                         quantization_output_type: str = None) -> None:
    """
    Perform post-training quantization on a TensorFlow Lite model.

    Args:
        model (tf.keras.Model): The TensorFlow model to be quantized.
        quantization_ds (tf.data.Dataset): The quantization dataset if it's provided by the user else the training
        dataset. Defaults to None
        output_dir (str): Path to the output directory. Defaults to None.
        export_dir (str): Name of the export directory for quantized model. Defaults to None.
        input_shape (tuple: The input shape of the model. Defaults to None.
        quantization_granularity (str): 'per_tensor' or 'per_channel'. Defaults to None.
        quantization_input_type (str): The quantization type for the input. Defaults to None.
        quantization_output_type (str): The quantization type for the output. Defaults to None.

    Returns:
        None
    """

    def representative_data_gen():
        for image, msk in tqdm.tqdm(quantization_ds, total=len(quantization_ds)):
            yield [image]

    def fake_data_gen():
        for _ in range(10):
            image = tf.random.uniform((1,) + input_shape, minval=0.0, maxval=1.0, dtype=tf.float32)
            yield [image]

    # Create the TFLite converter
    converter = tf.lite.TFLiteConverter.from_keras_model(model)

    # Create the output directory
    tflite_models_dir = pathlib.Path(os.path.join(output_dir, "{}/".format(export_dir)))
    tflite_models_dir.mkdir(exist_ok=True, parents=True)

    # Set the quantization types for the input and output
    if quantization_input_type == 'int8':
        converter.inference_input_type = tf.int8
    elif quantization_input_type == 'uint8':
        converter.inference_input_type = tf.uint8

    if quantization_output_type == 'int8':
        converter.inference_output_type = tf.int8
    elif quantization_output_type == 'uint8':
        converter.inference_output_type = tf.uint8

    # Set the optimizations and representative dataset generator
    converter.optimizations = [tf.lite.Optimize.DEFAULT]

    if quantization_ds is not None:
        converter.representative_dataset = representative_data_gen
    else:
        print("[WARNING] No representative dataset was provided. Quantization will be performed using fake data.")
        converter.representative_dataset = fake_data_gen

    # Set the quantization per tensor if requested
    if quantization_granularity == 'per_tensor':
        converter._experimental_disable_per_channel = True

    # Convert the model to a quantized TFLite model
    tflite_model_quantized = converter.convert()
    tflite_model_quantized_file = tflite_models_dir / "quantized_model.tflite"
    tflite_model_quantized_file.write_bytes(tflite_model_quantized)


def quantize(cfg: DictConfig = None, quantization_ds: Optional[tf.data.Dataset] = None,
             float_model_path: Optional[str] = None) -> str:
    """
    Quantize the TensorFlow model with training data.

    Args:
        cfg (DictConfig): The configuration dictionary. Defaults to None.
        quantization_ds (tf.data.Dataset): The quantization dataset if it's provided by the user else the training
        dataset. Defaults to None.
        float_model_path (str, optional): Model path to quantize

    Returns:
        quantized model path (str)
    """

    model_path = float_model_path if float_model_path else cfg.general.model_path
    _, input_shape = get_model_name_and_its_input_shape(model_path=model_path)
    output_dir = HydraConfig.get().runtime.output_dir
    export_dir = cfg.quantization.export_dir

    print("[INFO] : Quantizing the model ... This might take few minutes ...")

    # Check the model file extension
    file_extension = model_path.split('.')[-1]
    if file_extension == 'h5':
        # we expect a float Keras model
        float_model = tf.keras.models.load_model(model_path, compile=False)
        quantization_granularity = cfg.quantization.granularity
        quantization_optimize = cfg.quantization.optimize
        print(f'[INFO] : Quantization granularity : {quantization_granularity}')
        # if per-tensor quantization is required some optimizations are possible on the float model
        if quantization_granularity == 'per_tensor' and quantization_optimize:
            print("[INFO] : Optimizing the model for improved per_tensor quantization...")
            float_model = model_formatting_ptq_per_tensor(model_origin=float_model)
            model_path = os.path.join(output_dir, export_dir, "optimized_model.h5")
            float_model.save(model_path)

    if cfg.quantization.quantizer.lower() == "onnx_quantizer" and cfg.quantization.quantization_type == "PTQ":
        # Convert the dataset to numpy array
        target_opset = cfg.quantization.target_opset
        data, _ = tf_segmentation_dataset_to_np_array(quantization_ds, nchw=True)

        if file_extension == 'h5':
            # Convert the model and then quantize 
            converted_model_path = os.path.join(output_dir, 'converted_model', 'converted_model.onnx')
            onnx_model_converter(input_model_path=model_path, target_opset=target_opset,
                                 output_dir=converted_model_path)
            cfg.general.model_path = converted_model_path
            quantized_model = quantize_onnx(quantization_samples=data, configs=cfg) 
            return quantized_model
        
        elif file_extension == 'onnx':
            # Check if the model is already quantized
            if model_is_quantized(model_path):
                print('[INFO]: The input model is already quantized!\n\tReturning the same model!')
                return model_path
            else:
                # Quantize the model
                quantized_model = quantize_onnx(quantization_samples=data, configs=cfg)
                return quantized_model    
        else:
            raise ValueError(f"Unsupported model file extension: {file_extension}")
    
    elif cfg.quantization.quantizer.lower() == "tflite_converter" and cfg.quantization.quantization_type == "PTQ":
        if file_extension == 'h5':
            _tflite_ptq_quantizer(model=float_model, quantization_ds=quantization_ds, output_dir=output_dir,
                                 export_dir=export_dir, input_shape=input_shape,
                                 quantization_granularity=quantization_granularity,
                                 quantization_input_type=cfg.quantization.quantization_input_type,
                                 quantization_output_type=cfg.quantization.quantization_output_type)

            quantized_model_path = os.path.join(output_dir, export_dir, "quantized_model.tflite")
            return quantized_model_path
        else:
            raise ValueError(f"Unsupported model file extension: {file_extension}")
    else: 
        raise TypeError("Quantizer or quantization type not supported."
                        "Check the `quantization` section of your user_config.yaml file!")
