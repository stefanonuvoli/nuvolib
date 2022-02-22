# nuvolib - C++ geometry processing library

nuvolib is a structured templated geometry processing library written in C++. It provides algorithms and data-structures for handling animable 3D models. You can find several data-structures such as meshes, skeletons, graphs, ... and several algorithms for geometry processing.

The code is organised in several folders:
- math: mathematical data-structures and algorithms
- models: data-structures and algorithms to handle animable 3D models
- structures: several general purpose data-structures, such as graphs and trees
- utilities: various utilities
- viewer: tools to view and draw the 3D models

### Build
It is a header-only library. For now, only qmake is supported, but CMake build will be available as soon as possible. 

You need to install the library eigen to use most of the functionalities of the library. In Ubuntu you can install eigen easily with the following terminal commands:
```
sudo apt-get install libeigen3-dev
```

To access to some other functionalities, you would have to install some libraries: libigl, vcglib, boost, libqglviewer, fbxsdk. Once installed, you will have to set the following variables in the qmake pro file with the corresponding path (default path is shown in this example):
```
LIBIGL_PATH         = /opt/libigl/
VCGLIB_PATH         = /opt/vcglib/
EIGEN_PATH          = /usr/include/eigen3/
BOOST_PATH          = /usr/include/boost/
LIBGQLVIEWER_PATH   = /usr/lib/x86_64-linux-gnu/
FBXSDK_PATH         = /opt/fbxsdk/
```

### Note
This library has been developed for research purposes, hence it is a prototype and some functionalities could have not been properly tested. Also the documentation is not complete and compilation could not be supported for all platforms. In case you have technical issues or building problems, please write to [stefano.nuvoli@gmail.com](mailto:stefano.nuvoli@gmail.com).
