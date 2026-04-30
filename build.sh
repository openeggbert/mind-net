rm -r build
echo Going to build Hive

now=`date +"%Y%m%d_%H%M%S"`

cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=TRUE \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache || exit

time cmake --build build --config Release || exit

# ------------------------------
# Optimize binary
# ------------------------------
cd build/src/hive-app || exit
strip --strip-all hive_app || exit
upx --best --lzma hive_app || exit
cd ../.. || exit
directory=build_$now || exit
mkdir $directory || exit

mv src/hive-app/hive_app $directory/hive || exit
cp ../configuration/hive.properties $directory/hive.properties || exit
cp ../start-hive.sh $directory/start-hive.sh || exit
mkdir $directory/frontend || exit
cp -r ../frontend/* $directory/frontend || exit


echo "Hive was successfully built."
cd ..
