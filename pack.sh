#!/bin/sh
DIR=`pwd`

dist=ssdb-`cat version`
dist_dir=_pack/$dist

rm -rf $dist_dir

mkdir -p $dist_dir
mkdir $dist_dir/var
mkdir $dist_dir/var_slave
touch $dist_dir/var/.gitignore
touch $dist_dir/var_slave/.gitignore

cp -r api $dist_dir
cp -r deps $dist_dir
cp -r src $dist_dir
cp -r tools $dist_dir
cp -r pack.sh $dist_dir
cp -r build.sh $dist_dir
cp -r README.md $dist_dir
cp -r TODO $dist_dir
cp -r version $dist_dir
cp -r Makefile.dist $dist_dir/Makefile
cp -r ssdb.conf.dist $dist_dir/ssdb.conf
cp -r ssdb_slave.conf.dist $dist_dir/ssdb_slave.conf

cd $dist_dir
make clean

cd ..
tar czf ${dist}.tar.gz $dist

cd "$DIR"
cp -r $dist_dir/* git/
