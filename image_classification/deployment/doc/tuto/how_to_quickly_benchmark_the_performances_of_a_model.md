# How can I quickly benchmark a model using the model zoo?

With ST Model Zoo, you can easily measure the memory footprints and inference time of a model on multiple hardwares using the [ST Edge AI Development Cloud](https://stm32ai.st.com/st-edge-ai-developer-cloud/)

## Operation modes:

Depending on the model format you have, you can use the operation modes below:
- Benchmarking:
    - To benchmark a quantized model (.tflite or QDQ onnx)
- Chain_qb:
    - To quantize and benchmark a float model (.h5 or .onnx) in one pass
<div align="left" style="width:100%; margin: auto;">

![image.png](../img/chain_qb.png)
</div>

For any details regarding the parameters of the config file, you can look here:
- [Quantization documentation](../../../src/quantization/README.md)
- [Benchmark documentation](../../../src/benchmarking/README.md)



## Available boards for benchmark:

'STM32N6570-DK', 'STM32H747I-DISCO', 'STM32H7B3I-DK', 'STM32H573I-DK', 'NUCLEO-H743ZI2', 'STM32F769I-DISCO', 'STM32H735G-DK', 'STM32H7S78-DK', 'STM32F469I-DISCO', 'STM32F746G-DISCO', 'B-U585I-IOT02A', 'STM32L4R9I-DISCO', 'NUCLEO-F401RE', 'NUCLEO-G474RE', 'STM32MP257F-EV1', 'STM32MP135F-DK' and 'STM32MP157F-DK2'

## User_config.yaml:

The way ST Model Zoo works is that you edit the user_config.yaml available for each use case and run the stm32ai_main.py python script. 

Here is an example where we benchmark a model already quantized available in ST model zoo.

The most important parts here are to define:
- The path to the model
- The operation mode
- The benchmarking parameters (online or locally, see below)
- The benchmarking hardware target

```yaml
# user_config.yaml 

general:
  # path to the model to benchmark
  model_path: ../../stm32ai-modelzoo/image_classification/mobilenetv2/ST_pretrainedmodel_public_dataset/flowers/mobilenet_v2_0.35_128_fft/mobilenet_v2_0.35_128_fft_int8.tflite

operation_mode: benchmarking

# preprocessing used during training
preprocessing:
  rescaling: { scale: 1/127.5, offset: -1 }
  resizing:
    interpolation: nearest
    aspect_ratio: fit
  color_mode: rgb

tools:
  stedgeai:
    version: 10.0.0
    optimization: balanced
    on_cloud: True # ST Edge AI Cloud or locally with ST Edge AI
    path_to_stedgeai: C:/Users/<XXXXX>/STM32Cube/Repository/Packs/STMicroelectronics/X-CUBE-AI/<*.*.*>/Utilities/windows/stedgeai.exe
  path_to_cubeIDE: C:/ST/STM32CubeIDE_<*.*.*>/STM32CubeIDE/stm32cubeide.exe

# the hardware used for the benchmark
benchmarking:
  board: STM32H747I-DISCO

mlflow:
  uri: ./src/experiments_outputs/mlruns

hydra:
  run:
    dir: ./src/experiments_outputs/${now:%Y_%m_%d_%H_%M_%S}
  
```

Here is another example where we quantize the model first, then we launch the benchmark:

```yaml
# user_config.yaml 

general:
  # path to the model to benchmark
  model_path: ../../stm32ai-modelzoo/image_classification/mobilenetv2/ST_pretrainedmodel_public_dataset/flowers/mobilenet_v2_0.35_128_fft/mobilenet_v2_0.35_128_fft.h5

operation_mode: chain_qb

# preprocessing used during training
preprocessing:
  rescaling: { scale: 1/127.5, offset: -1 }
  resizing:
    interpolation: nearest
    aspect_ratio: fit
  color_mode: rgb

# quantization for the quantization
quantization:
  quantizer: TFlite_converter
  quantization_type: PTQ
  quantization_input_type: uint8
  quantization_output_type: float
  granularity: per_channel  # Optional, defaults to "per_channel".
  optimize: False           # Optional, defaults to False.
  target_opset: 17          # Optional, defaults to 17 if not provided. Only used for when using Onnx_quantizer
  export_dir: quantized_models

tools:
  stedgeai:
    version: 10.0.0
    optimization: balanced 
    on_cloud: True # ST Edge AI Cloud or locally with ST Edge AI
    path_to_stedgeai: C:/Users/<XXXXX>/STM32Cube/Repository/Packs/STMicroelectronics/X-CUBE-AI/<*.*.*>/Utilities/windows/stedgeai.exe
  path_to_cubeIDE: C:/ST/STM32CubeIDE_<*.*.*>/STM32CubeIDE/stm32cubeide.exe

# the hardware used for benchmark
benchmarking:
  board: STM32H747I-DISCO

mlflow:
  uri: ./src/experiments_outputs/mlruns

hydra:
  run:
    dir: ./src/experiments_outputs/${now:%Y_%m_%d_%H_%M_%S}
```

Here the quantization is made with random data as no data were provided and main goal was to have a quick insight on the performances of a quantized model for a specific HW. When evaluating the model, it is highly recommended to use real data for the final quantization of course.

## Run the script:

Edit the user_config.yaml then open a CMD (make sure to be in the UC folder). Finally, run the command:

```powershell
python stm32ai_main.py
```
You can also use any .yaml file using command below:
```powershell
python stm32ai_main.py --config-path=path_to_the_folder_of_the_yaml --config-name=name_of_your_yaml_file
```

## Local benchmarking:

To make the benchmark locally instead of using the ST Edge AI Development Cloud you need to add the path for path_to_stedgeai and to set on_cloud to false in the yaml.

To download the tools:
- [ST Edge AI](https://www.st.com/en/embedded-software/x-cube-ai.html)
- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

