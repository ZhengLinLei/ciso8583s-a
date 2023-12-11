cd ../

echo "Checking if container is running"
./script/STOP.sh

echo "---- Build started ---"
docker build -t ciso8583_ots_a_engine_build -f docker/compile.dockerfile ./
docker run --rm -v $(pwd)/tmp:/opt/Ciso8583/tmp ciso8583_ots_a_engine_build

# All binaries in /tmp
# Run docker production.dockerfile
docker build -t ciso8583_ots_a -f docker/production.dockerfile ./

# Volume for logs
docker volume create ciso8583_ots_a_logs

rm -rf tmp

yes | docker image prune

echo "---- Build finished ---"

if [ "$#" -gt 0 ]; then
    echo "Exporting compiled image: PATH > $1"
    docker image save ciso8583_ots_a | gzip > $1 || {
        echo "---- Export failed!!!!! ----"
    }
fi

cd -