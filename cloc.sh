current_dir=`pwd`

cd src && cloc .
cd $current_dir
pwd
echo current_dir=$current_dir
cd include && cloc .

cd $current_dir