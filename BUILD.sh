echo "Checking if container is running"
./STOP.sh

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