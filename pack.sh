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

cp -R api $dist_dir
cp -R deps $dist_dir
cp -R src $dist_dir
cp -R tools $dist_dir
cp -R pack.sh $dist_dir
cp -R build.sh $dist_dir
cp -R README.md $dist_dir
cp -R TODO $dist_dir
cp -R version $dist_dir
cp -R Makefile.dist $dist_dir/Makefile
cp -R ssdb.conf.dist $dist_dir/ssdb.conf
cp -R ssdb_slave.conf.dist $dist_dir/ssdb_slave.conf

cd $dist_dir
make clean

cd ..
tar czf ${dist}.tar.gz $dist

cd "$DIR"

