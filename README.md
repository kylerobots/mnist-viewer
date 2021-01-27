# mnist-viewer
A simple GUI to view and train a neural network on MNIST using LibTorch (C++ Frontend for PyTorch). This project is
primarily to practice all the tools and features of Qt and PyTorch. It was created when I just started using PyTorch,
so not all of the usage is best practice.

For full documentation, see https://kylerobots.github.io/mnist-viewer

Source code is available at https://github.com/kylerobots/mnist-viewer

## Compilation
Currently, you have to have Qt, CMake, and MSVC. Use those to compile the project. This uses CMake and should have
everything needed to compile, assuming you have those dependencies. After compilation, you can run the mnist-viewer
executable.

## Usage
Right now, you can only click through and see really crappy predictions of the different images in the MNIST test
dataset. In version 1.0, you will be able to instruct the system to train to improve accuracy.
