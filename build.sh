rm -r build
echo Going to build Mind Net

now=`date +"%Y%m%d_%H%M%S"`

cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=TRUE \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache || exit

time cmake --build build --config Release || exit

# ------------------------------
# Optimize binary
# ------------------------------
cd build/src/mind-net-app || exit
strip --strip-all mind_net_app || exit
upx --best --lzma mind_net_app || exit
cd ../.. || exit
directory=build_$now || exit
mkdir $directory || exit

mv src/mind-net-app/mind_net_app $directory/mindnet || exit
cp ../configuration/mindnet.properties $directory/mindnet.properties || exit
cp ../start-mindnet.sh $directory/start-mindnet.sh || exit
mkdir $directory/frontend || exit
cp -r ../frontend/* $directory/frontend || exit


echo "Mind Net was successfully built."
cd ..
