# Si besoin : chmod 777 make.sh
if [ ! -d "./build_files" ]; then
    mkdir build_files
    cd build_files
    cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5
else
    cd  build_files
fi
if make -j8; then
    cd ..
    ./TP
fi