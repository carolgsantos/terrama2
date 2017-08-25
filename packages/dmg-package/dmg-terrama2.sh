#!/bin/bash

USER_DIR=$HOME
CODEBASE=`pwd`/../..
export TE_THIRD_DIR=${USER_DIR}/mylibs
export TL_PATH=${TE_THIRD_DIR}/terralib-install
export QT_DIR=${USER_DIR}/Qt5.4.1/5.4/clang_64
export PATH=$PATH:${QT_DIR}/bin:/Applications/CMake.app/Contents/bin

clear
echo "* ------------------------ *"
echo "* TerraMa2 Release Package *"
echo "* ------------------------ *"
echo ""

export TM_INSTALL=${TE_THIRD_DIR}/terrama2-package
export TM_OUT_DIR=${CODEBASE}/../build-package

if [ -d "${TL_PATH}" ]; then
  
  mkdir -p ${TM_OUT_DIR}
  cp terrama2.conf.cmake ${TM_OUT_DIR}

  cd ${TM_OUT_DIR}

  cmake -G "Unix Makefiles" -C ./terrama2.conf.cmake -DCMAKE_BUILD_TYPE="Release" ${CODEBASE}/build/cmake
  make package -j 8

fi

echo "* ---------- *"
echo "* Finished ! *"
echo "* ---------- *"
echo ""
